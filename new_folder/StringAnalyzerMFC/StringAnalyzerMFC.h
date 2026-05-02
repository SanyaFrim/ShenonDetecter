
// StringAnalyzerMFC.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CStringAnalyzerMFCApp:
// Сведения о реализации этого класса: StringAnalyzerMFC.cpp
//

class CStringAnalyzerMFCApp : public CWinApp
{
public:
	CStringAnalyzerMFCApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CStringAnalyzerMFCApp theApp;
