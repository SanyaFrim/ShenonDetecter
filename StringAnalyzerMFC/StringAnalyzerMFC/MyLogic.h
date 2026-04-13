#pragma once

#include <string>

struct Result {
    int N = 0;
    int K = 0;
    double H = 0.0;
    double Hnorm = 0.0;

    int digits = 0;
    int letters = 0;
    int spaces = 0;
    int specials = 0;
    int printable = 0;

    bool onlyDigits = false;
    bool onlyHex = false;
    bool onlyBase64Chars = false;
    bool monotonicSequence = false;
    bool repeatedPattern = false;

    std::string verdict;
    std::string explanation;
};

double myLog2(double x);

bool isHexChar(unsigned char c);
bool isBase64Char(unsigned char c);
bool isPrintableChar(unsigned char c);
bool isLikelyLetter(unsigned char c);

bool isMonotonicSequence(const std::string& s);
bool hasRepeatedPattern(const std::string& s);

bool isLikelyHumanReadable(const std::string& s, const Result& r);
bool isLikelyHexHash(const std::string& s, const Result& r);
bool isLikelyBase64Block(const std::string& s, const Result& r);

std::string buildExplanation(const Result& r);
Result analyzeString(const std::string& s);