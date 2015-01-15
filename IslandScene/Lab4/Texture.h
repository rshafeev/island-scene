#pragma once
#include "afx.h"
#include "GlLib.h"
#include <atlimage.h>

class CTexture
{
	AUX_RGBImageRec *texture;                                             // Загрузка картинки
	 CString FileName;
	 GLuint	texPtr;
	 GLvoid LoadGLTextures();
	 bool createdFlag;          //была ли загружена текстура
void    Create(AUX_RGBImageRec *texture); //создает объект по уже созданному texture 
	
public:
	void    InitTexture(CString FileName);
	void    InitTexture(CImage *image);
	void    Bind();
	GLuint*  GetTexPt();
	bool    IsCreated();
	CString GetTexFileName();
	int     PictWigth();
	int     PictHeight();
	void    Free();
	CTexture(void);
	~CTexture(void);
};
