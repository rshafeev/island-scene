
// Lab4.h : главный файл заголовка для приложения Lab4
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы
#include "GLContainer.h"

// CLab4App:
// О реализации данного класса см. Lab4.cpp
//

class CLab4App : public CWinApp
{
public:
	CLab4App();
	CGLContainer *GLContainer;

// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

public:

	DECLARE_MESSAGE_MAP()
};

extern CLab4App theApp;
