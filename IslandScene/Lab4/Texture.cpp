#include "StdAfx.h"
#include "Texture.h"
#include "memory.h"
#include "Functions.h"
CTexture::CTexture(void)
{
	createdFlag=false;
	texPtr = -1;
}

CTexture::~CTexture(void)
{
}
//====================================================================================================
CString CTexture::GetTexFileName()			 //получения имени файла, возвращает имя файла
{
   return FileName;
}
//====================================================================================================
/*void CTexture::Bind()
{
	if(texPtr!=-1)
		glBindTexture(GL_TEXTURE_2D, this->texPtr); 
}*/

void CTexture::Bind()
{
	if(texPtr!=-1)
		glBindTexture(GL_TEXTURE_2D, this->texPtr); 
}

void CTexture::Create(AUX_RGBImageRec *texture)
{
  if(texture!=NULL)
	  this->texture=texture;
}
//====================================================================================================
GLvoid CTexture::LoadGLTextures()
{
   texture=auxDIBImageLoadA((LPCSTR)FileName);
   glGenTextures(1,&texPtr);                                  // Создание текстуры
  texture->sizeX -= texture->sizeX%2;
  texture->sizeY -= texture->sizeY%2;
     glBindTexture(GL_TEXTURE_2D, texPtr);	
	 gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture->sizeX, texture->sizeY,
                       GL_RGB, GL_UNSIGNED_BYTE, texture->data); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	 createdFlag=true;   //если выполнилась загрузка текстуры успешно
}
//=======================================================================================================
void CTexture::InitTexture(CString FileName)
{
	this->FileName=FileName;
	if(IsFileExists(FileName)==true)			 //если путь к текстуре указан, то выполняем загрузку 
	   LoadGLTextures();
}
void CTexture::InitTexture(CImage *image)
{
	int W,H;
	W=image->GetWidth();
	H=image->GetHeight();
	texture = new AUX_RGBImageRec;
	texture->sizeX=W;
	texture->sizeY=H;
	texture->data = GetBufFromDC(CDC::FromHandle(image->GetDC()),W,H);
   glGenTextures(1,&texPtr);                                  // Создание текстуры
   texture->sizeX -= texture->sizeX%2;
   texture->sizeY -= texture->sizeY%2;
     glBindTexture(GL_TEXTURE_2D, texPtr);	
	 gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture->sizeX, texture->sizeY,
		 GL_BGR, GL_UNSIGNED_BYTE, texture->data); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	 createdFlag=true;   //если выполнилась загрузка текстуры успешно
}
//=======================================================================================================
GLuint* CTexture::GetTexPt()
{
	return &texPtr;
}
//=======================================================================================================
bool CTexture::IsCreated()
{
  return createdFlag;
}
//=======================================================================================================
int CTexture::PictWigth()   //получение ширины по х
{
	if(createdFlag==false)   return 0;
	return (int)texture->sizeX;
}
//=======================================================================================================
int CTexture::PictHeight()   //получение высоты по у
{
	if(createdFlag==false)   return 0;
	return (int)texture->sizeY;
}
//=======================================================================================================
void CTexture::Free()
{
 if(createdFlag==true)
 {
   createdFlag=false;
   FileName="";
   glDeleteTextures(1,&texPtr);
   delete [] texture->data;
   free(texture);
   texture=NULL;
 }
}
//=======================================================================================================