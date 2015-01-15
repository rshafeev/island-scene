#pragma once
#include "GLlib.h"
#include "Shader.h"
#include "Texture.h"
// CGLFrame frame

class CGLFrame : public CFrameWnd
{
struct TCamera
{
  CVector3 Pos; 
  CVector3 Angle; 
  bool  PressingLB,PressingRB;		//нажата ли лева€/права€ кнопка
  CVector3 Center;					//цент окружности, по которой вращаетс€ камера
  CRect WindowRect;

  bool KeysUp;
  bool LightFonari;
  bool Fog;
  bool CameraType;
  bool CinemaFlag;
  bool KorablMove;
};
struct TPostRender // пост-бработка сцены(эффект старого кино)
{
	CShader  shader;     // шейдер, обраб. отрисовку текстуры
	CTexture tex; // текстура сцены
	double randValue;
	CTexture textTexture;
};
	DECLARE_DYNCREATE(CGLFrame)
	DECLARE_MESSAGE_MAP()
protected:
	UINT_PTR timer;
	UINT_PTR korablTimer;
    float PosLight[4],PosLightFonar1[4],PosLightFonar2[4];					  //позици€ источника света
	TCamera Camera;
	HGLRC  hRC;						     // ѕосто€нный контекст рендеринга
	HDC  hDC;							 // ѕриватный контекст устройства GDI
	CDC *pDC;
	TPostRender postRender;
	public:
		bool Init();
		bool InitGL();
		void DestroyGL(GLvoid);
		void RenderScene();

		CGLFrame();
		virtual ~CGLFrame();
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnDestroy();
		afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
		afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		static void CALLBACK EXPORT OnNoiceTimer(HWND hWnd,	UINT nMsg,	UINT_PTR nIDEvent,	DWORD dwTime);
		static void CALLBACK EXPORT OnKorablTimer(HWND hWnd,	UINT nMsg,	UINT_PTR nIDEvent,	DWORD dwTime);

};


