
// Lab4.h : ������� ���� ��������� ��� ���������� Lab4
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"       // �������� �������
#include "GLContainer.h"

// CLab4App:
// � ���������� ������� ������ ��. Lab4.cpp
//

class CLab4App : public CWinApp
{
public:
	CLab4App();
	CGLContainer *GLContainer;

// ���������������
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����������

public:

	DECLARE_MESSAGE_MAP()
};

extern CLab4App theApp;
