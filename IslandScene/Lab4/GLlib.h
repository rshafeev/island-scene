#pragma once
//#include "stdafx.h"
//using namespace std;
#include "gl\glew.h"
#include <gl\gl.h>
#include "gl\glaux.h"
#include <gl\glu.h>
#include "gl\glext.h"
#include "gl\glut.h"
#include "gl\wglext.h"
#include "CVector.h"

struct TTreeProperties
{
	TTreeProperties()
	{

	}
	TTreeProperties(int x, int y, int H, int r, int MaxLevel,int scaleX, int scaleY)
	{
		this->x = x;
		this->y = y;
		this->H = H;
		this->r = r;
		this->MaxLevel = MaxLevel;
		this->ScaleXY.x =(double) scaleX;
		this->ScaleXY.y =(double) scaleY;
	}
int x;
int y;
int H;  // высота
int r;

int MaxLevel;
CVector2 ScaleXY;       
};

#pragma comment(lib, "glew32.lib")
//#pragma comment(lib, "glut32.lib")

#pragma comment (lib,"OpenGL32.Lib")
#pragma comment (lib,"GLu32.Lib")
#pragma comment (lib,"WinMm.Lib")
#pragma comment (lib,"GLaux.Lib")

