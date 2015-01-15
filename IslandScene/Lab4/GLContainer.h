#pragma once

#include <vector>
#include "Shader.h"
#include "3ds.h"
#include"Texture.h"
#include "Landshaft.h"
using namespace std;

struct TModelCamera
{
	CVector3 pos;
	CVector3 rotate;
	CVector3 scale;
	double fi;

};
class CGLContainer
{
	TModelCamera korablCamera;
private:
	GLuint  MemoryPloscPt; // плоскость
	GLuint  MemoryQuardPt;
	GLuint  MemoryConePt;

	GLuint  MemoryPlanePt,MemoryHousePt, MemoryTreePt,MemoryVisPt,MemoryHouse1Pt, MemoryHouse2Pt,MemoryKolodecPt  ;
	GLuint MemoryHouse3Pt,MemoryHouse4Pt,MemoryKolokolPt,MemoryKolokol2Pt,MemoryLavkaPt,MemoryStena1Pt,MemoryStena2Pt;
	GLuint MemoryStena3Pt,MemoryStolb1Pt, MemoryStolb2Pt,MemoryTelegaPt,MemoryFonarPt,MemoryFontanPt,MemoryVorotaPt;
	GLuint MemoryWaterPlosc;
	GLuint MemoryKorablPt;
	GLuint MemoryskyBoxPt;
	CLandshaft landshaft;
	void LoadSkyBoxToMemory();
	void LoadWaterPloscToMemory(CRect r, int h);
	void LoadQuardToMemory();
	void LoadConeToMemory();
	GLuint LoadModelToMemory(CString FileName,CVector3 translated,CVector3 scaled, CVector3 rotated);
    void   AnimateModel(CLoad3DS::t3DModel *pModel, CLoad3DS::t3DObject *pObject);

	vector<CTexture> modelTextures; // список всех текстур моделей

	int FindTextureForFileName(CString name); //ищет индекс текстуры в векторе modelTextures, которая загружена с файла name, если такой нет, вернет -1
private: 
	CVector2 ZInterval;
	CVector2 ScaleXY;
public:
	static GLuint LoadPloscToMemory(CRect r, int h,CTexture *texture,double texKoef);
	CGLContainer(void);
	~CGLContainer(void);
	void SetFog(bool fog)
	{
		landshaft.IsEnableFog = fog;
	}
	void Create(CVector2 ZInterval, CVector2 ScaleXY);
	void Init();
	void Render();
	void MoveKorabl(); // перемещение корабля


	static void print(int x, int y, const char * string, ...);
};
