#include "pch.h"
#include "Mylogic.h"
#include <array>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <clocale>


double myLog2(double x) {
    return log(x) / log(2.0);
}

bool isHexChar(unsigned char c) {
    return std::isxdigit(c) != 0;
}

bool isBase64Char(unsigned char c) {
    return std::isalnum(c) || c == '+' || c == '/' || c == '=';
}

bool isPrintableChar(unsigned char c) {
    return std::isprint(c) != 0;
}

bool isLikelyLetter(unsigned char c) {
    return std::isalpha(c) != 0;
}

bool isMonotonicSequence(const std::string& s) {
    if (s.size() < 4) return false;

    bool inc = true;
    bool dec = true;

    for (size_t i = 1; i < s.size(); ++i) {
        if ((unsigned char)s[i] != (unsigned char)s[i - 1] + 1) inc = false;
        if ((unsigned char)s[i] != (unsigned char)s[i - 1] - 1) dec = false;
    }

    return inc || dec;
}

bool hasRepeatedPattern(const std::string& s) {
    if (s.size() < 4) return false;

    for (size_t len = 1; len <= s.size() / 2; ++len) {
        if (s.size() % len != 0) continue;

        std::string part = s.substr(0, len);
        bool ok = true;

        for (size_t i = len; i < s.size(); i += len) {
            if (s.substr(i, len) != part) {
                ok = false;
                break;
            }
        }

        if (ok) return true;
    }

    return false;
}

bool isLikelyHumanReadable(const std::string& s, const Result& r) {
    if (s.empty()) return false;

    double letterRatio = static_cast<double>(r.letters) / r.N;
    double specialRatio = static_cast<double>(r.specials) / r.N;
    double spaceRatio = static_cast<double>(r.spaces) / r.N;
    double printableRatio = static_cast<double>(r.printable) / r.N;

    bool hasWordsStyle =
        (letterRatio >= 0.5) &&
        (specialRatio <= 0.25) &&
        (printableRatio >= 0.95) &&
        (r.Hnorm >= 0.35 && r.Hnorm <= 0.85);

    bool hasSpacesOrSeparators =
        (spaceRatio > 0.0) ||
        (s.find('_') != std::string::npos) ||
        (s.find('-') != std::string::npos);

    return hasWordsStyle || (hasWordsStyle && hasSpacesOrSeparators);
}

bool isLikelyHexHash(const std::string& s, const Result& r) {
    static const std::vector<int> typicalLengths = { 32, 40, 56, 64, 96, 128 };

    if (!r.onlyHex) return false;
    if (std::find(typicalLengths.begin(), typicalLengths.end(), r.N) == typicalLengths.end()) return false;
    if (r.Hnorm < 0.75) return false;

    return true;
}

bool isLikelyBase64Block(const std::string& s, const Result& r) {
    if (!r.onlyBase64Chars) return false;
    if (r.N < 8) return false;
    if (r.N % 4 != 0) return false;
    if (r.Hnorm < 0.70) return false;
    return true;
}

std::string buildExplanation(const Result& r) {
    setlocale(LC_ALL, "RU");

    std::string text;
    text += "Длина строки N = " + std::to_string(r.N) + ". ";
    text += "Число различных символов K = " + std::to_string(r.K) + ". ";
    text += "Энтропия H = " + std::to_string(r.H) +
        ", нормированная энтропия Hnorm = " + std::to_string(r.Hnorm) + ". ";

    text += "Символьный состав: букв = " + std::to_string(r.letters) +
        ", цифр = " + std::to_string(r.digits) +
        ", пробелов = " + std::to_string(r.spaces) +
        ", спецсимволов = " + std::to_string(r.specials) + ". ";

    if (r.onlyDigits) text += "Строка состоит только из цифр. ";
    if (r.onlyHex) text += "Строка состоит только из hex-символов. ";
    if (r.onlyBase64Chars) text += "Строка содержит только символы, допустимые в Base64. ";
    if (r.monotonicSequence) text += "Обнаружена монотонная последовательность символов. ";
    if (r.repeatedPattern) text += "Обнаружен повторяющийся шаблон. ";

    return text;
}

Result analyzeString(const std::string& s) {
    Result r;
    r.N = static_cast<int>(s.size());

    if (s.empty()) {
        r.verdict = "Пустая строка";
        r.explanation = "Строка не содержит данных для анализа.";
        return r;
    }

    std::array<int, 256> freq{};
    bool hexOk = true;
    bool base64Ok = true;

    for (unsigned char c : s) {
        freq[c]++;

        if (std::isdigit(c)) r.digits++;
        else if (isLikelyLetter(c)) r.letters++;
        else if (std::isspace(c)) r.spaces++;
        else r.specials++;

        if (isPrintableChar(c)) r.printable++;

        if (!isHexChar(c)) hexOk = false;
        if (!isBase64Char(c)) base64Ok = false;
    }

    for (int x : freq) {
        if (x > 0) r.K++;
    }

    for (int x : freq) {
        if (x > 0) {
            double p = static_cast<double>(x) / r.N;
            r.H += -(p * myLog2(p));
        }
    }

    if (r.K > 1) {
        double maxH = myLog2(static_cast<double>(r.K));
        if (maxH > 0.0) r.Hnorm = r.H / maxH;
    }

    r.onlyDigits = (r.digits == r.N);
    r.onlyHex = hexOk;
    r.onlyBase64Chars = base64Ok;
    r.monotonicSequence = isMonotonicSequence(s);
    r.repeatedPattern = hasRepeatedPattern(s);

    if (r.N < 4) {
        r.verdict = "Слишком короткая строка";
    }
    else if (r.onlyDigits && (r.monotonicSequence || r.repeatedPattern || r.K <= 3)) {
        r.verdict = "Предсказуемая цифровая последовательность";
    }
    else if (r.repeatedPattern) {
        r.verdict = "Повторяющийся шаблон";
    }
    else if (isLikelyHexHash(s, r)) {
        r.verdict = "Похоже на hex-хэш";
    }
    else if (isLikelyBase64Block(s, r)) {
        r.verdict = "Похоже на Base64 / кодированный блок";
    }
    else if (isLikelyHumanReadable(s, r)) {
        r.verdict = "Похоже на человекочитаемую строку";
    }
    else if (r.N >= 16 && r.Hnorm >= 0.88 && r.spaces == 0 && r.K >= 10) {
        r.verdict = "Похоже на шифртекст / случайные данные";
    }
    else if (r.Hnorm < 0.45) {
        r.verdict = "Предсказуемая строка";
    }
    else {
        r.verdict = "Смешанная неопределённая строка";
    }

    r.explanation = buildExplanation(r);
    return r;
}