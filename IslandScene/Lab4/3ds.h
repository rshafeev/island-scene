#ifndef _3DS_H
#define _3DS_H
 
//>------ Главный Chunk, в начале каждого 3ds-файла
#define PRIMARY       0x4D4D
 
//>------ Главнык Chunk-и
#define OBJECTINFO    0x3D3D			// Это предоставляет версию меша перед информацией об обьекте
#define VERSION       0x0002			// Предоставляет версию .3ds файла
#define EDITKEYFRAME  0xB000			// Хидер для всей информации о кадрах
 
//>------ под-дефайны OBJECTINFO
#define MATERIAL	  0xAFFF		// Информация о текстурах
#define OBJECT		  0x4000		// Полигоны, вершины, и т.д...
 
//>------ под-дефайны для MATERIAL
#define MATNAME       0xA000			// Название материала
#define MATDIFFUSE    0xA020			// Хранит цвет обьекта/материала
#define MATMAP        0xA200			// Хидер для нового материала
#define MATMAPFILE    0xA300			// Хранит имя файла текстуры
 
#define OBJECT_MESH   0x4100			// Даёт нам знать, что начинаем считывать новый обьект
 
//>------ под-дефайны для OBJECT_MESH
#define OBJECT_VERTICES		0x4110		// Вершины обьекта
#define OBJECT_FACES		0x4120		// Полигоны обьекта
#define OBJECT_MATERIAL		0x4130		// Дефайн находится, если обьект имеет материал, иначе цвет/текстура
#define OBJECT_UV		0x4140		// UV текстурные координаты
 

// Константы для чтения анимации:
#define	KEYFRAME			0xB000	// Даёт нам знать, что мы считываем ключевой кадр
#define KEYFRAME_MESH_INFO		0xB002
#define KEYFRAME_OBJECT_NAME		0xB010
#define KEYFRAME_START_AND_END		0xB008
#define PIVOT				0xB013
#define POSITION_TRACK_TAG		0xB020
#define ROTATION_TRACK_TAG		0xB021
#define SCALE_TRACK_TAG			0xB022

#include "3DLib.h"
// Структура для индексов 3DS (так как .3ds хранит 4 unsigned short)
struct tIndices {
	unsigned short a, b, c, bVisible;	// Это хранит индексы для точки 1,2,3 массива
						// вершин, плюс флаг видимости
};
 
// Хранит информацию о chunk-е
struct tChunk
{
	unsigned short int ID;			// ID chunk-а
	unsigned int length;			// Длинна chunk-а
	unsigned int bytesRead;			// Число читаемых байт для этого chunk-а
};
 
// Класс содержит весь код загрузки
class CLoad3DS
{
public:

// Это структура полигона. Она используется для индексирования массивов координат
// вершин и текстур. Эта информация сообщает нам о том, какие номера вершин в массиве
// какому полигону принадлежат. То же самое касается текстурных координат.
struct tFace
{
	int vertIndex[3];			// indicies for the verts that make up this triangle
	int coordIndex[3];			// indicies for the tex coords to texture this face
};
 
 
// Эта структура хранит информацию о материале. Это может быть текстурная карта света.
// Некоторые значения не используются, но я оставил их, чтобы могли увидеть их для
// примера.
struct tMaterialInfo
{
	char  strName[255];			// Имя текстуры
	char  strFile[255];			// Имя файла текстуры
	BYTE  color[3];				// Цвет обьекта (R, G, B)
	int   texureId;				// ID текстуры
	float uTile;				// u-tiling текстуры (Сейчас не используется)
	float vTile;				// v-tiling текстуры (Сейчас не используется)
	float uOffset;				// u-offset текстуры (Сейчас не используется)
	float vOffset;				// v-offset текстуры (Сейчас не используется)
};
struct CVector3{
	float x,y,z;
};

struct CVector2{
	float x,y;
};
// Содержит всю информацию о модели/сцене.
struct t3DObject
{
	int  numOfVerts;			// Число вершин в модели
	int  numOfFaces;			// Число полигонов в модели
	int  numTexVertex;			// Число текстурных координат
	int  materialID;			// ID текстуры для использования, индекс массива текстур
	bool bHasTexture;			// TRUE если есть текстурная карта для этого обьекта
	char strName[255];			// Имя обьекта
	CVector3  *pVerts;			// Массив вершин обьекта
	CVector3  *pNormals;			// Нормали обьекта
	CVector2  *pTexVerts;			// Текстурные координаты
	tFace *pFaces;				// Полигоны обьекта

	int  positionFrames;		// Число кейфреймов перемещения
	int  rotationFrames;		// Число кейфреймов вращения
	int  scaleFrames;		// Число кейфреймов масштабирования
 
	CVector3 vPivot;		// Точка опоры обьекта
 
	vector<CVector3> vPosition;	// Массив позиций обьекта
	vector<CVector3> vRotation;	// Массив вращений обьекта
	vector<CVector3> vScale;	// Массив размеров обьекта
 
	vector<float> vRotDegree;	// Углы вращения обьекта
};
 
 
 
// Содержит информацию о модели. Тоже неплохо бы обернуть в класс. 
struct t3DModel
{
	int numOfObjects;			// Число обьектов в модели
	int numOfMaterials;			// Число материалов модели
	vector<tMaterialInfo> pMaterials;	// Число обьектов материалов (текстуры и цвета)
	vector<t3DObject> pObject;		// Список обьектов в модели

	int numberOfFrames;	// Число кадров анимации для данной модели (как минимум 1)
	int currentFrame;	// Текущий кадр анимации модели
};


public:
	CLoad3DS();		// Тут будут инициализироватся все данные
    ~CLoad3DS(void);
	// Эта ф-я и будет вызыватся для загрузки 3DS
	bool Import3DS(t3DModel *pModel,const char *strFileName);
 
private:
	// Читает строку и сохраняет её в переданный массив char-ов
	int GetString(char *);
 
	// Читает следующий chunk
	void ReadChunk(tChunk *);
 
	// Читает следующий длинный chunk
	void ProcessNextChunk(t3DModel *pModel, tChunk *);
 
	// Читает chunk-и обьекта
	void ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *);
 
	// Читает chunk-и материала
	void ProcessNextMaterialChunk(t3DModel *pModel, tChunk *);
 
	// Читает RGB-значение цвета обьекта
	void ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk);
 
	// Читает вершины обьекта
	void ReadVertices(t3DObject *pObject, tChunk *);
 
	// Читает информацию полигонов обьекта
	void ReadVertexIndices(t3DObject *pObject, tChunk *);
 
	// Читает текстурные координаты обьекта
	void ReadUVCoordinates(t3DObject *pObject, tChunk *);
 
	// Читает имя материала, присвоенного обьекту, и устанавливает materialID
	void ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk);
 
	// Рассчитывает нормали вершин обьекта
	void ComputeNormals(t3DModel *pModel);
 
	// This frees memory and closes the file
	// Освобождает память и закрывает файл
	void CleanUp();

// Новые функции:
	// Читает chunk-и ключевых кадров (данные анимации
	void ProcessNextKeyFrameChunk(t3DModel *pModel, tChunk *);
 
	// Читает позицию для каждого кадра анимации
	void ReadKeyFramePositions(t3DModel *pModel, tChunk *pPreviousChunk);
 
	// Читает вращения для ключевых кадров
	void ReadKeyFrameRotations(t3DModel *pModel, tChunk *pPreviousChunk);
 
	// Читает данные scale для ключевых кадров
	void ReadKeyFrameScales(t3DModel *pModel, tChunk *pPreviousChunk);
 
	// Устанавливает текущую модель, для которой читается анимация
	void SetCurrentObject(t3DModel *pModel, char *strObjectName);
 
// И новые переменные
	// Эти две используются в процессе загрузки для хранения информации chunk-ов
	tChunk *m_CurrentChunk;
	tChunk *m_TempChunk;
 
	// Хранит обьект, для которого читаются данные анимации
	t3DObject *m_CurrentObject;

	// Указатель на файл
	FILE *m_FilePointer;
};
 
 
#endif
