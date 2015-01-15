
// Lab4.cpp : Определяет поведение классов для приложения.
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


// создание CLab4App

CLab4App::CLab4App()
{
	theApp.GLContainer = NULL;
	// TODO: замените ниже строку идентификатора приложения строкой уникального идентификатора; рекомендуемый
	// формат для строки: ИмяКомпании.ИмяПродукта.СубПродукт.СведенияОВерсии
	SetAppID(_T("Lab4.AppID.NoVersion"));

	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}

// Единственный объект CLab4App

CLab4App theApp;


// инициализация CLab4App

BOOL CLab4App::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// Для использования элемента управления RichEdit требуется метод AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// Стандартная инициализация
	// Если эти возможности не используются и необходимо уменьшить размер
	// конечного исполняемого файла, необходимо удалить из следующего
	// конкретные процедуры инициализации, которые не требуются
	// Измените раздел реестра, в котором хранятся параметры
	// TODO: следует изменить эту строку на что-нибудь подходящее,
	// например на название организации
	SetRegistryKey(_T("Локальные приложения, созданные с помощью мастера приложений"));


	// Чтобы создать главное окно, этот код создает новый объект окна
	// рамки, а затем задает его как объект основного окна приложения
	CGLFrame * pFrame = new CGLFrame;
	//CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	pFrame->Create(NULL,"Terrain3D",WS_POPUP,CRect(0,0,100,100),NULL);
	// создайте и загрузите рамку с его ресурсами
	/*pFrame->LoadFrame(NULL,
		WS_MAXIMIZE|WS_POPUPWINDOW, NULL,
		NULL);
*/





	// Одно и только одно окно было инициализировано, поэтому отобразите и обновите его
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// вызов DragAcceptFiles только при наличии суффикса
	//  В приложении SDI это должно произойти после ProcessShellCommand
	return TRUE;
}

int CLab4App::ExitInstance()
{
	//TODO: обработайте дополнительные ресурсы, которые могли быть добавлены
	if(theApp.GLContainer!=NULL)
		delete theApp.GLContainer;
	return CWinApp::ExitInstance();
}

// обработчики сообщений CLab4App



