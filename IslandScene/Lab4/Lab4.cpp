
// Lab4.cpp : ���������� ��������� ������� ��� ����������.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Lab4.h"
#include "GLFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab4App

BEGIN_MESSAGE_MAP(CLab4App, CWinApp)
	
END_MESSAGE_MAP()


// �������� CLab4App

CLab4App::CLab4App()
{
	theApp.GLContainer = NULL;
	// TODO: �������� ���� ������ �������������� ���������� ������� ����������� ��������������; �������������
	// ������ ��� ������: �����������.�����������.����������.���������������
	SetAppID(_T("Lab4.AppID.NoVersion"));

	// TODO: �������� ��� ��������,
	// ��������� ���� ������ ��� ������������� � InitInstance
}

// ������������ ������ CLab4App

CLab4App theApp;


// ������������� CLab4App

BOOL CLab4App::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// ��� ������������� �������� ���������� RichEdit ��������� ����� AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ����������� �������������
	// ���� ��� ����������� �� ������������ � ���������� ��������� ������
	// ��������� ������������ �����, ���������� ������� �� ����������
	// ���������� ��������� �������������, ������� �� ���������
	// �������� ������ �������, � ������� �������� ���������
	// TODO: ������� �������� ��� ������ �� ���-������ ����������,
	// �������� �� �������� �����������
	SetRegistryKey(_T("��������� ����������, ��������� � ������� ������� ����������"));


	// ����� ������� ������� ����, ���� ��� ������� ����� ������ ����
	// �����, � ����� ������ ��� ��� ������ ��������� ���� ����������
	CGLFrame * pFrame = new CGLFrame;
	//CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	pFrame->Create(NULL,"Terrain3D",WS_POPUP,CRect(0,0,100,100),NULL);
	// �������� � ��������� ����� � ��� ���������
	/*pFrame->LoadFrame(NULL,
		WS_MAXIMIZE|WS_POPUPWINDOW, NULL,
		NULL);
*/





	// ���� � ������ ���� ���� ���� ����������������, ������� ���������� � �������� ���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// ����� DragAcceptFiles ������ ��� ������� ��������
	//  � ���������� SDI ��� ������ ��������� ����� ProcessShellCommand
	return TRUE;
}

int CLab4App::ExitInstance()
{
	//TODO: ����������� �������������� �������, ������� ����� ���� ���������
	if(theApp.GLContainer!=NULL)
		delete theApp.GLContainer;
	return CWinApp::ExitInstance();
}

// ����������� ��������� CLab4App



