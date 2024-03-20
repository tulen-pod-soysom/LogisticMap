
// LogisticMap.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CLogisticMapApp:
// Сведения о реализации этого класса: LogisticMap.cpp
//

class CLogisticMapApp : public CWinApp
{
public:
	CLogisticMapApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CLogisticMapApp theApp;
