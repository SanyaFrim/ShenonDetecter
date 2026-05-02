
// StringAnalyzerMFCDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "StringAnalyzerMFC.h"
#include "StringAnalyzerMFCDlg.h"
#include "afxdialogex.h"
#include "Mylogic.h"
#include <fstream>
#include <sstream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CStringAnalyzerMFCDlg



CStringAnalyzerMFCDlg::CStringAnalyzerMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STRINGANALYZERMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStringAnalyzerMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStringAnalyzerMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT2, &CStringAnalyzerMFCDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON1, &CStringAnalyzerMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CStringAnalyzerMFCDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Обработчики сообщений CStringAnalyzerMFCDlg

BOOL CStringAnalyzerMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CStringAnalyzerMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CStringAnalyzerMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CStringAnalyzerMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CStringAnalyzerMFCDlg::OnEnChangeEdit2()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}

void CStringAnalyzerMFCDlg::OnBnClickedButton1()
{
	//                ↑ если у тебя класс называется иначе → поменяй
// ЗДЕСЬ ПОМЕНЯТЬ: имя функции может отличаться, если кнопка называется иначе
	{
		CString inputCString;

		// ЗДЕСЬ ПОМЕНЯТЬ: ID поля ввода (input Edit Control)
		GetDlgItemText(IDC_EDIT1, inputCString);
		//                  ↑ поставь свой ID, например IDC_EDIT1

		CT2A converted(inputCString);
		std::string inputText(converted);

		// ЗДЕСЬ НЕ МЕНЯТЬ (если ты подключил свой .h)
		Result r = analyzeString(inputText);

		CString output;
		output.Format(
			_T("N = %d\r\n")
			_T("K = %d\r\n")
			_T("H = %.4f\r\n")
			_T("Hnorm = %.4f\r\n")
			_T("Буквы = %d\r\n")
			_T("Цифры = %d\r\n")
			_T("Пробелы = %d\r\n")
			_T("Спецсимволы = %d\r\n")
			_T("Вердикт: %S\r\n")
			_T("Пояснение: %S"),
			r.N,
			r.K,
			r.H,
			r.Hnorm,
			r.letters,
			r.digits,
			r.spaces,
			r.specials,
			r.verdict.c_str(),
			r.explanation.c_str()
		);

		// ЗДЕСЬ ПОМЕНЯТЬ: ID поля вывода (output Edit Control)
		SetDlgItemText(IDC_EDIT2, output);
		//                  ↑ поставь свой ID, например IDC_EDIT2
	}
}
void CStringAnalyzerMFCDlg::OnBnClickedButton2()
{
	CFileDialog dlg(
		TRUE,
		_T("txt"),
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("Text files (*.txt)|*.txt|All files (*.*)|*.*||")
	);

	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();

	CFile file;
	if (!file.Open(path, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("Не удалось открыть файл."));
		return;
	}

	ULONGLONG fileSize = file.GetLength();

	std::string buffer;
	buffer.resize(static_cast<size_t>(fileSize));

	file.Read((void*)buffer.data(), static_cast<UINT>(fileSize));	file.Close();

	// Убираем UTF-8 BOM, если он есть
	if (buffer.size() >= 3 &&
		(unsigned char)buffer[0] == 0xEF &&
		(unsigned char)buffer[1] == 0xBB &&
		(unsigned char)buffer[2] == 0xBF)
	{
		buffer.erase(0, 3);
	}

	int wideSize = MultiByteToWideChar(
		CP_UTF8,
		0,
		buffer.c_str(),
		static_cast<int>(buffer.size()),
		NULL,
		0
	);

	if (wideSize <= 0)
	{
		AfxMessageBox(_T("Не удалось преобразовать текст из UTF-8."));
		return;
	}

	CString text;
	MultiByteToWideChar(
		CP_UTF8,
		0,
		buffer.c_str(),
		static_cast<int>(buffer.size()),
		text.GetBuffer(wideSize),
		wideSize
	);
	text.ReleaseBuffer(wideSize);

	SetDlgItemText(IDC_EDIT1, text);
}

