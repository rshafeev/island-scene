#pragma once
#include "GlLib.h"
#include "CVector.h"
#include "Shader.h"
#include "Texture.h"
#include "3dLib.h"

struct TLandProperties
{//свойства  ландшафта
  CString LandFile;                         //путь к файлу высот
  CString TexFile;                          //путь к файлу текстуры
  int ElemSize;                             //в каком формате хранятся данные 
  int StepSize;                             //шаг, с которым читается файл высот
  int width,height;                         //размеры карты высот 
  int MaxHeight,MinHeight;                  //максимальная и минимальная высота на карте
  bool AutoCalcMaxMinHeight;                //макс/мин высоты вычислять программно или их задал пользователь?
                    
  int DW,DH;                                //размер с учетом StepSize(w/StepSize;h/StepSize)
  
  bool FilterTaling;                        //размытие
  bool FilterFractal;                       //фрактальное построение гор
  bool FilterAverage;                       //усреднение
  bool FilterSmooth;                        //сглаживание
  CVector2 ScaleXY;// масштабирование по X,Y(растяжение или сжатие)
  CVector2 PosX;
};

class CLandshaft
{
struct TLandState
{
	bool LandState;
	bool TextureState;
	bool InitState;   //сосотояние инициализации обїекта (0-не инициализирован, 1-иниц)
} States;
private:
	GLuint MemoryDnoPlosc;

	TLandProperties LandProperties; 
	int **MassHeight;
	int **Mass;//
	CVector3 **MassVNormal;

    CShader MixingShader;                            // объект(программа) шейдер смешивания
	GLuint  MemoryPt;
	CTexture TexMixing[4];                           // 3 текстуры, которые используются для смешивания(буду ее тоже называть текстурной картоц)

	CVector3 GenTrianglePNormal(CVector3 p1,CVector3 p2,CVector3 p3);// вычисляем нормать плоскости (p1,p2,p3)
	void LoadMapFromDC(CDC *pDC,int w,int h);
	void LoadToMemory();
	void GenVNormals();
	void LoadTextures();
	int** LoadRawFile(LPSTR strName, int w,int h);
public:
	bool IsEnableFog;
	CLandshaft(void);
	
	bool Init(TLandProperties Properties);
	//void GetHeight(int i, int j);
	void Render();
	double GetHeight(int x,int y)
	{
		return Mass[x][y];
	}


	bool IsInitState(){return States.InitState;}
	~CLandshaft(void);
};
