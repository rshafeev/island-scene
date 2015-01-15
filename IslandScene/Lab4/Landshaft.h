#pragma once
#include "GlLib.h"
#include "CVector.h"
#include "Shader.h"
#include "Texture.h"
#include "3dLib.h"

struct TLandProperties
{//��������  ���������
  CString LandFile;                         //���� � ����� �����
  CString TexFile;                          //���� � ����� ��������
  int ElemSize;                             //� ����� ������� �������� ������ 
  int StepSize;                             //���, � ������� �������� ���� �����
  int width,height;                         //������� ����� ����� 
  int MaxHeight,MinHeight;                  //������������ � ����������� ������ �� �����
  bool AutoCalcMaxMinHeight;                //����/��� ������ ��������� ���������� ��� �� ����� ������������?
                    
  int DW,DH;                                //������ � ������ StepSize(w/StepSize;h/StepSize)
  
  bool FilterTaling;                        //��������
  bool FilterFractal;                       //����������� ���������� ���
  bool FilterAverage;                       //����������
  bool FilterSmooth;                        //�����������
  CVector2 ScaleXY;// ��������������� �� X,Y(���������� ��� ������)
  CVector2 PosX;
};

class CLandshaft
{
struct TLandState
{
	bool LandState;
	bool TextureState;
	bool InitState;   //���������� ������������� ������ (0-�� ���������������, 1-����)
} States;
private:
	GLuint MemoryDnoPlosc;

	TLandProperties LandProperties; 
	int **MassHeight;
	int **Mass;//
	CVector3 **MassVNormal;

    CShader MixingShader;                            // ������(���������) ������ ����������
	GLuint  MemoryPt;
	CTexture TexMixing[4];                           // 3 ��������, ������� ������������ ��� ����������(���� �� ���� �������� ���������� ������)

	CVector3 GenTrianglePNormal(CVector3 p1,CVector3 p2,CVector3 p3);// ��������� ������� ��������� (p1,p2,p3)
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
