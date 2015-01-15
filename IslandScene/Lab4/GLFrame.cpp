// GLFrame.cpp : implementation file
//

#include "stdafx.h"

#include  "CVector.h"
#include "GLFrame.h"
#include "Lab4.h"

// CGLFrame
IMPLEMENT_DYNCREATE(CGLFrame, CFrameWnd)

CGLFrame::CGLFrame()                       //��������� ���������� ���������
{
	PosLight[0]=-32.0;
	PosLight[1]=177.0;                     //��������� �� ������ (�)
	PosLight[2]=-10.0;
	PosLight[3]=0.0;

	

	PosLightFonar1[0]=-120.0;
	PosLightFonar1[1]=-200.0;                     //��������� �� ������ (�)
	PosLightFonar1[2]=-1012.0;
	PosLightFonar1[3]=1.0;
	Camera.LightFonari = true;
	Camera.Fog = true;
	Camera.KeysUp = true;
	Camera.CinemaFlag = false;
	Camera.CameraType = false;
	Camera.KorablMove = true;
}

CGLFrame::~CGLFrame()
{
	pDC=NULL;
	hDC=NULL;
	hRC=NULL;
	
}


BEGIN_MESSAGE_MAP(CGLFrame, CFrameWnd)
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_KEYUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// ��� ���� ����� �������: ������ ������ �������� ��� ������� � ��
void CreateRenderTexture(CTexture *texture, int sizex,int sizey, int channels, int type)
{
	// ��������� ��� ���������� ������ ��������
	unsigned int *pTexture = NULL;
 	// ����� ������� ������ �������� ��� ������� ������������ ��������.
	// ����� ������� ���, ������ ������� ������ ��� �������� ������ �
	// �������� ��� OpenGL. �������� �������� � ����������, ��� ��� �� �����
	// ���������� ������ � ����� ������.
	// ��� ������� ��������� ���������� ������ ��� ���������� ��������,
	// ������ ��������, ������ � ������, ������ (1,3,4), ��� (RGB, RGBA, etc)
	// � ���������� ID.
 
	// �������������� ������ ��� ������ �������� � �������� � pTexture
	pTexture = new unsigned int [sizex * sizey * channels];
	memset(pTexture, 0, sizex * sizey * channels * sizeof(unsigned int));
 
	// �������������� �������� � OpenGL � �������� � ID
	glGenTextures(1, (texture->GetTexPt()));
	glBindTexture(GL_TEXTURE_2D,*( texture->GetTexPt()));
 
	// ������� �������� � ��������� � ������
	glTexImage2D(GL_TEXTURE_2D, 0, channels, sizex, sizey, 0, type, GL_UNSIGNED_INT, pTexture);
 
	// ������������� ��������
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // ( ����� )
   //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // ( ����� )
	// �������� �������� � ���. OpenGL, �� ����� ������� ��������� ������
	delete [] pTexture;
}

bool CGLFrame::Init()
{
	InitGL();
	theApp.GLContainer = new CGLContainer;
	theApp.GLContainer->Init();

	postRender.shader.LoadShader("Shaders\\PostShader\\PostShader.vert","Shaders\\PostShader\\PostShader.frag");
	postRender.randValue = 7;
	CreateRenderTexture(&postRender.tex,1400,900,3,GL_RGB);
	postRender.textTexture.InitTexture("Resources/textureL0 - �����.bmp");
	SetTimer(korablTimer,50,0);
//	CreateRenderTexture(&renderTexture,tw,th,3,GL_RGB);
	return true;
}
bool CGLFrame::InitGL()									// ��������� ��������� ������� OpenGL, ��� ������� "���������" (�������� �������)
{	
	CRect rect;
	int pixelformat;

	pDC = GetDC();
	hDC = pDC->GetSafeHdc();
  static  PIXELFORMATDESCRIPTOR pfd=                            // pfd �������� Windows ����� ����� ����� �� ����� ������� �������
  {
    sizeof(PIXELFORMATDESCRIPTOR),                              // ������ ����������� ������� ������� ��������
    1,                                                          // ����� ������
    PFD_DRAW_TO_WINDOW |                                        // ������ ��� ����
    PFD_SUPPORT_OPENGL |                                        // ������ ��� OpenGL
    PFD_DOUBLEBUFFER,                                           // ������ ��� �������� ������
    PFD_TYPE_RGBA,                                              // ��������� RGBA ������
    32,                                                         // ���������� ��� ������� �����
    0, 0, 0, 0, 0, 0,                                           // ������������� �������� �����
    0,                                                          // ��� ������ ������������
    0,                                                          // ��������� ��� ������������
    0,                                                          // ��� ������ ����������
    0, 0, 0, 0,                                                 // ���� ���������� ������������
    32,                                                         // 32 ������ Z-����� (����� �������)
    0,                                                          // ��� ������ ���������
    0,                                                          // ��� ��������������� �������
    PFD_MAIN_PLANE,                                             // ������� ���� ���������
    0,                                                          // ���������������
    0, 0, 0                                                     // ����� ���� ������������
  };

	if ((pixelformat = ChoosePixelFormat(pDC->GetSafeHdc(), &pfd)) == 0)
	{
		MessageBox("ChoosePixelFormat failed");
		return FALSE;
	}
	if (SetPixelFormat(pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}
 hRC = wglCreateContext(hDC);
 wglMakeCurrent(hDC, hRC);

 glShadeModel( GL_SMOOTH );                                   // ��������� ������� �������� �����������
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                        // ������� ������ � ������ ����
 glClearDepth( 1.0f );                                        // ��������� ������� ������ �������
 glEnable( GL_DEPTH_TEST );                                   // ��������� ���� �������
 glDepthFunc( GL_LEQUAL );									  // ��� ����� �������
 glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );		  // ��������� � ���������� ����������� (��������� �������������� �������� ��� ������������� ��������� ��������� ��� ������������)

 
  GetWindowRect(rect);
 glViewport( 0, 0, rect.Width(), rect.Height());			  // ����� ������� ������� ������ 
//���������

	 GLfloat LightAmbient[] = {0.0, 0.0, 0.0};
	 GLfloat LightDiff[] = {1.0, 1.0, 1.0, 1.0};
	 GLfloat MaterialAmbient[] = {0.9, 0.9, 0.9, 0.0};
     GLfloat MaterialDiffuse[] = {0.9, 0.9, 0.9, 0.0 };//
     GLfloat MaterialSpecular[] = {0.9, 0.9, 0.9, 1.0 };//
     GLfloat MaterialShininess[] = {110.0};
     GLfloat MaterialEmis[] = {0.21, 0.01, 0.01, 0.0};
////////////////////////////////////////


	 glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialDiffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, MaterialShininess);
 	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiff);


       GLfloat light_diffuse[] = {0.9, 0.9, 0.9};
       
	   GLfloat light1_position[] = {120.0, 200, 1050.0, 1.0};
	   GLfloat light2_position[] = {120.0, 200, 1400.0, 1.0};
	   glEnable(GL_LIGHT1);
       glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
       glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
       glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.0);
       glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.00001);
       glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0001);



	   glEnable(GL_LIGHT2);
       glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
       glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
       glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.0);
       glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.00001);
       glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0001);


//�����������
 glMatrixMode( GL_PROJECTION );                                       // ����� ������� ��������
 glLoadIdentity();                                                    // ����� ������� ��������
 gluPerspective( 45.0f, (GLfloat)rect.Width()/(GLfloat)rect.Height(), 5.1f,25500.0f ); // ���������� ����������� �������������� �������� ��� ����
 glMatrixMode( GL_MODELVIEW );                                        // ����� ������� ���� ������
 glLoadIdentity();                                                  // ����� ������� ���� ������
 glEnable(GL_NORMALIZE);										      //������������ ������� ���������
 glEnable(GL_LIGHT0);	


 //������ 
 Camera.Angle.x=30;
 Camera.Angle.y=40.0;
 Camera.Angle.z=0.0;
 Camera.Pos.x=250;
 Camera.Pos.y=-270;
 Camera.Pos.z=-250.0;
 glEnable(GL_TEXTURE_2D);
 
 glewInit();

 //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR }; // ������ ��� ���� ������

GLfloat fogColor[4]= {0.5f, 0.5f, 0.7f, 1.0f}; // ���� ������
 glFogi(GL_FOG_MODE, fogMode[0]);// �������� ��� ������
glFogfv(GL_FOG_COLOR, fogColor);        // ������������� ���� ������
glFogf(GL_FOG_DENSITY, 0.0001f);          // ��������� ������ ����� �����
glHint(GL_FOG_HINT, GL_NICEST );      // ��������������� ��������� ������
glFogf(GL_FOG_START, 800.0f);             // �������, � ������� ���������� �����
glFogf(GL_FOG_END, 16700.0f);               // �������, ��� ����� �������������.


 return true;												        // ������������� ������ �������
}
void CGLFrame::DestroyGL(GLvoid)
{
		glClear(GL_COLOR_BUFFER_BIT);
		wglMakeCurrent( NULL, NULL );
    wglDeleteContext( hRC );
	ReleaseDC(pDC);
	
	pDC=NULL;
	hDC=NULL;
	hRC=NULL;
}

void CGLFrame::RenderScene()										//����������� ����
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//���� ��������� ������� ������ �������,��������� ������ ����������
  glClearColor(0.5f,0.5f,0.5f,1.0f);	
  glLoadIdentity();                                               // ����� ���������
  glPushMatrix();
  glRotatef(Camera.Angle.y, 1.0f, 0.0, 0.0);					    //��������� �������
  glRotatef(Camera.Angle.x, 0.0, 1.0f, 0.0);
  glRotatef(Camera.Angle.z, 0.0, 0.0, 1.0f);
  glTranslatef(Camera.Pos.x,Camera.Pos.y,Camera.Pos.z);

  // ������������� �����
  if(Camera.Fog)
	glEnable(GL_FOG);                       // �������� ����� (GL_FOG)
  else
	glDisable(GL_FOG);                       // �������� ����� (GL_FOG)

  glPushMatrix();
  glLightfv(GL_LIGHT0, GL_POSITION, PosLight);						//��������� ��������� ����� LIGHT0
  glPopMatrix();
  if(Camera.LightFonari)
  {
	   GLfloat light1_position[] = {120.0, 200, 1050.0, 1.0};
	   GLfloat light2_position[] = {120.0, 200, 1400.0, 1.0};
	   glEnable(GL_LIGHT1);
       glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	   glEnable(GL_LIGHT2);
       glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	   	 /* GLUquadric *Cylinder = gluNewQuadric();
		    glPushMatrix();
			glTranslatef(light2_position[0],light2_position[1],light2_position[2]);
			gluSphere(Cylinder,20,20,20);
			glPopMatrix();*/
  }
  else
  {
	  glDisable(GL_LIGHT1);
	  glDisable(GL_LIGHT2);
  }

   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHTING);
 
  theApp.GLContainer->Render();
  glPopMatrix();
  if(Camera.CinemaFlag)
  {
	  int tw = Camera.WindowRect.Width();
	  int th = Camera.WindowRect.Height();
	postRender.tex.Bind();
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, tw,th, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.9f,0.0f,0.0f,1.0f);	
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);   
	postRender.tex.Bind();
	glLoadIdentity();  
	double kw = 5.1*37*1.8;
    double kh = 5.1*37;
    postRender.shader.Use();
    glActiveTexture(GL_TEXTURE1);
	postRender.tex.Bind();
    postRender.randValue = 2 + 4*rand()/RAND_MAX;
	postRender.shader.SetUniformInt("tex1",1);
    postRender.shader.SetUniformFloat("randValue",postRender.randValue);
    glTranslatef(-tw/2/kw,-th/2/kh,-8.1);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 0); 		glVertex3f(tw/kw, 0,0); 
		glTexCoord2f(1, 1); 		glVertex3f(tw/kw,th/kh,0); 
		glTexCoord2f(0, 1); 		glVertex3f(0, th/kh,0); 
		glTexCoord2f(0, 0); 		glVertex3f(0,0,0);
    glEnd();
    postRender.shader.Close();
    glActiveTexture(GL_TEXTURE0);

  }	
 
//==================================
  postRender.textTexture.Bind();
  CGLContainer::print(20, 30, "Position: %f ; %f;  %f; ",Camera.Pos.x,Camera.Pos.y,Camera.Pos.z );
  CGLContainer::print(20, 50, "Rotate: %f ; %f;  %f; ",Camera.Angle.x,Camera.Angle.y,Camera.Angle.z );
  CGLContainer::print(20, Camera.WindowRect.Height()-40, "F - on / off light bulbs");
  CGLContainer::print(20, Camera.WindowRect.Height()-60, "T - fog");
  CGLContainer::print(20, Camera.WindowRect.Height()-80, "O - review the scene");
  CGLContainer::print(20, Camera.WindowRect.Height()-100, "C - effect 'old movie'");
  CGLContainer::print(20, Camera.WindowRect.Height()-120, "K -  on / off motion of the ship");
  SwapBuffers(wglGetCurrentDC());									//���������� �� hRC � pDC
  
}
void CGLFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)		//������� ������ ������������ (�������)
{ 
	const double PI = 3.1415;
const double Speed=50;
 float X=Camera.Pos.x,Z=Camera.Pos.z;

if(nChar==75)
{
	Camera.KorablMove = ! Camera.KorablMove;
	if(Camera.KorablMove== true)
		SetTimer(timer,50,0);
	Camera.KeysUp = false;
}

if(nChar==79)
{
	Camera.CameraType = ! Camera.CameraType;
	if(Camera.CameraType== true)	
	{
		Camera.Pos.y-=200;
		SetTimer(timer,50,0);
	}
	else
	{
		Camera.Pos.y+=200;
		//KillTimer(timer);
	}
	Camera.KeysUp = false;
}

 if(nChar==67)
{
	Camera.CinemaFlag =!Camera.CinemaFlag ;
	if(Camera.CinemaFlag==true)
		SetTimer(timer,50,0);
		//SetTimer(postRender.noiceTimer,50,OnNoiceTimer);
	/*else
		KillTimer(postRender.noiceTimer);*/
	Camera.KeysUp = false;
}

 if(nChar==84)
{
	Camera.Fog =!Camera.Fog ;
	theApp.GLContainer->SetFog(Camera.Fog);
	Camera.KeysUp = false;
}

if(nChar==70)
{
	Camera.LightFonari =!Camera.LightFonari ;
	Camera.KeysUp = false;
}
if (nChar==27)
{
	ShowCursor(true);
	DestroyWindow();
	return;
}
if(nChar==38)//������� ������
  {
	  X -= Speed*sin((Camera.Angle.x)/180.0*PI);
	  Z += Speed*cos((Camera.Angle.x)/180.0*PI);
	  Camera.KeysUp = false;
    }

if(nChar==40)//������� �����
  {
     X += (float)sin((Camera.Angle.x)* PI/180.0) * Speed;
     Z -= (float)cos((Camera.Angle.x)* PI/180.0) * Speed;
	 Camera.KeysUp = false;
  }

if(nChar==39)//������� �����
  {
     X -= Speed*sin((Camera.Angle.x+90)/180.0*PI);
     Z += Speed*cos((Camera.Angle.x+90)/180.0*PI);
	 Camera.KeysUp = false;

}
  
if(nChar==37)//������� ������
  {
     X += Speed*sin((Camera.Angle.x+90)/180.0*PI);
     Z -= Speed*cos((Camera.Angle.x+90)/180.0*PI);
	 Camera.KeysUp = false;

}

if(nChar==36)
{
	Camera.Pos.y-=Speed/5.0;
	Camera.KeysUp = false;
	

}

if(nChar==35)
{
	Camera.Pos.y+=Speed/5.0;
	Camera.KeysUp = false;

}
	 Camera.Pos.x = X;
     Camera.Pos.z = Z;
 if(Camera.KorablMove== true)	
	theApp.GLContainer->MoveKorabl();
 if(Camera.KorablMove+Camera.CameraType+Camera.CinemaFlag==0)
	 KillTimer(timer);
if(Camera.KeysUp == false)
	RenderScene();

	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	
}

int CGLFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)//��� �������� ���� ������
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	pDC = GetDC();
	Init();
	RenderScene();

	ShowCursor(false);
	return 0;
}

void CGLFrame::OnPaint()		//������������ ��� �������� �������
{
	CPaintDC dc(this); 
	CRect t;					//��� �������� ��������� ������������ ������ CRect, ������� ����� ������ ����������: l � ����� ����������, t - ������� ����������, r � ������ ����������, b � ������ ����������.
	GetWindowRect(t);
    dc.BitBlt(0,0,t.Width(), t.Height(),pDC,0,0,SRCCOPY);
	RenderScene();
}

void CGLFrame::OnSize(UINT nType, int cx, int cy)		//������� ���� �������
{

	  //��� �������� ��������� ������������ ������ CRect, ������� ����� ������ ����������: l � ����� ����������, t - ������� ����������, r � ������ ����������, b � ������ ����������.
	GetClientRect(Camera.WindowRect);
	glViewport( 0, 0, Camera.WindowRect.Width(), Camera.WindowRect.Height());            //������������� ���� ��������� � ��������� ���� (������ ������� ����, ��� OpenGl ����� ��������� ���������)

	CreateRenderTexture(&postRender.tex,Camera.WindowRect.Width(), Camera.WindowRect.Height(),3,GL_RGB);
 	CFrameWnd::OnSize(nType, cx, cy); 
}
static long xCurPosOld=0, yCurPosOld=0;
void CGLFrame::OnMouseMove(UINT nFlags, CPoint point)	//�������� ������
{
	if(xCurPosOld!=0&&yCurPosOld!=0)
	{
	  Camera.Angle.y -= (yCurPosOld-point.y)*0.05f;
	  Camera.Angle.x -= (xCurPosOld-point.x)*0.05f;
	}
	
	if(point.x>=Camera.WindowRect.Width()-50)
	{
		point.x = 51;
		SetCursorPos(point.x,point.y);
	}
	else
	if (point.x<=50)
	{
		point.x=Camera.WindowRect.Width()-51;
		SetCursorPos(point.x,point.y);
	}
	else
	if(point.y>=Camera.WindowRect.Height()-60)
	{
		point.y = 61;
		SetCursorPos(point.x,point.y);
	}
	else
	if (point.y<=60)
	{
		point.y=Camera.WindowRect.Height()-61;
		SetCursorPos(point.x,point.y);
	}
	RenderScene();
	xCurPosOld = point.x;
	yCurPosOld = point.y;	

	/*CString text;
	text.Format("alfax = %f; alfay=%f; ",Camera.Angle.x,Camera.Angle.y); 
	SetWindowText(text);
	*/
	CFrameWnd::OnMouseMove(nFlags, point);
}

void CGLFrame::OnDestroy()  // ��� �������� GLFrame ������� ���� ���������� ��������
{
	DestroyGL();
	CFrameWnd::OnDestroy();
}


void CGLFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	ShowWindow(SW_MAXIMIZE);
	CRect r;
	GetWindowRect(r);
	MoveWindow(-20,-20,r.Width()+40,r.Height()+40);
//	MoveWindow(0,0,r.Width(),r.Height());
	CFrameWnd::OnShowWindow(bShow, nStatus);
}


void CGLFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	this->Camera.KeysUp = true;
	CFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

double t = 0;
void CGLFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(Camera.CameraType)
	{
		t+=0.002;
		double ro = 2000 * cos(2*t);
		Camera.Pos.x = ro*cos(t);
		Camera.Pos.z = ro*sin(t);
		if(t>6.28) t = 0; 
	}

	if(Camera.KorablMove)
	{
	  CGLFrame* pFrame =(CGLFrame*)theApp.m_pMainWnd;
	  theApp.GLContainer->MoveKorabl();
	}
	RenderScene();
	CFrameWnd::OnTimer(nIDEvent);
}


void CALLBACK EXPORT CGLFrame::OnNoiceTimer(HWND hWnd,	// ���������� CWND ������� ������ SetTimer
	UINT nMsg,	// WM_TIMER
	UINT_PTR nIDEvent,	// ������������� �������
	DWORD dwTime	// ��������� �����
	)
{
	

}

void CALLBACK EXPORT CGLFrame::OnKorablTimer(HWND hWnd,	// ���������� CWND ������� ������ SetTimer
	UINT nMsg,	// WM_TIMER
	UINT_PTR nIDEvent,	// ������������� �������
	DWORD dwTime	// ��������� �����
	)
{

}
