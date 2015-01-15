#ifndef _3DS_H
#define _3DS_H
 
//>------ ������� Chunk, � ������ ������� 3ds-�����
#define PRIMARY       0x4D4D
 
//>------ ������� Chunk-�
#define OBJECTINFO    0x3D3D			// ��� ������������� ������ ���� ����� ����������� �� �������
#define VERSION       0x0002			// ������������� ������ .3ds �����
#define EDITKEYFRAME  0xB000			// ����� ��� ���� ���������� � ������
 
//>------ ���-������� OBJECTINFO
#define MATERIAL	  0xAFFF		// ���������� � ���������
#define OBJECT		  0x4000		// ��������, �������, � �.�...
 
//>------ ���-������� ��� MATERIAL
#define MATNAME       0xA000			// �������� ���������
#define MATDIFFUSE    0xA020			// ������ ���� �������/���������
#define MATMAP        0xA200			// ����� ��� ������ ���������
#define MATMAPFILE    0xA300			// ������ ��� ����� ��������
 
#define OBJECT_MESH   0x4100			// ��� ��� �����, ��� �������� ��������� ����� ������
 
//>------ ���-������� ��� OBJECT_MESH
#define OBJECT_VERTICES		0x4110		// ������� �������
#define OBJECT_FACES		0x4120		// �������� �������
#define OBJECT_MATERIAL		0x4130		// ������ ���������, ���� ������ ����� ��������, ����� ����/��������
#define OBJECT_UV		0x4140		// UV ���������� ����������
 

// ��������� ��� ������ ��������:
#define	KEYFRAME			0xB000	// ��� ��� �����, ��� �� ��������� �������� ����
#define KEYFRAME_MESH_INFO		0xB002
#define KEYFRAME_OBJECT_NAME		0xB010
#define KEYFRAME_START_AND_END		0xB008
#define PIVOT				0xB013
#define POSITION_TRACK_TAG		0xB020
#define ROTATION_TRACK_TAG		0xB021
#define SCALE_TRACK_TAG			0xB022

#include "3DLib.h"
// ��������� ��� �������� 3DS (��� ��� .3ds ������ 4 unsigned short)
struct tIndices {
	unsigned short a, b, c, bVisible;	// ��� ������ ������� ��� ����� 1,2,3 �������
						// ������, ���� ���� ���������
};
 
// ������ ���������� � chunk-�
struct tChunk
{
	unsigned short int ID;			// ID chunk-�
	unsigned int length;			// ������ chunk-�
	unsigned int bytesRead;			// ����� �������� ���� ��� ����� chunk-�
};
 
// ����� �������� ���� ��� ��������
class CLoad3DS
{
public:

// ��� ��������� ��������. ��� ������������ ��� �������������� �������� ���������
// ������ � �������. ��� ���������� �������� ��� � ���, ����� ������ ������ � �������
// ������ �������� �����������. �� �� ����� �������� ���������� ���������.
struct tFace
{
	int vertIndex[3];			// indicies for the verts that make up this triangle
	int coordIndex[3];			// indicies for the tex coords to texture this face
};
 
 
// ��� ��������� ������ ���������� � ���������. ��� ����� ���� ���������� ����� �����.
// ��������� �������� �� ������������, �� � ������� ��, ����� ����� ������� �� ���
// �������.
struct tMaterialInfo
{
	char  strName[255];			// ��� ��������
	char  strFile[255];			// ��� ����� ��������
	BYTE  color[3];				// ���� ������� (R, G, B)
	int   texureId;				// ID ��������
	float uTile;				// u-tiling �������� (������ �� ������������)
	float vTile;				// v-tiling �������� (������ �� ������������)
	float uOffset;				// u-offset �������� (������ �� ������������)
	float vOffset;				// v-offset �������� (������ �� ������������)
};
struct CVector3{
	float x,y,z;
};

struct CVector2{
	float x,y;
};
// �������� ��� ���������� � ������/�����.
struct t3DObject
{
	int  numOfVerts;			// ����� ������ � ������
	int  numOfFaces;			// ����� ��������� � ������
	int  numTexVertex;			// ����� ���������� ���������
	int  materialID;			// ID �������� ��� �������������, ������ ������� �������
	bool bHasTexture;			// TRUE ���� ���� ���������� ����� ��� ����� �������
	char strName[255];			// ��� �������
	CVector3  *pVerts;			// ������ ������ �������
	CVector3  *pNormals;			// ������� �������
	CVector2  *pTexVerts;			// ���������� ����������
	tFace *pFaces;				// �������� �������

	int  positionFrames;		// ����� ���������� �����������
	int  rotationFrames;		// ����� ���������� ��������
	int  scaleFrames;		// ����� ���������� ���������������
 
	CVector3 vPivot;		// ����� ����� �������
 
	vector<CVector3> vPosition;	// ������ ������� �������
	vector<CVector3> vRotation;	// ������ �������� �������
	vector<CVector3> vScale;	// ������ �������� �������
 
	vector<float> vRotDegree;	// ���� �������� �������
};
 
 
 
// �������� ���������� � ������. ���� ������� �� �������� � �����. 
struct t3DModel
{
	int numOfObjects;			// ����� �������� � ������
	int numOfMaterials;			// ����� ���������� ������
	vector<tMaterialInfo> pMaterials;	// ����� �������� ���������� (�������� � �����)
	vector<t3DObject> pObject;		// ������ �������� � ������

	int numberOfFrames;	// ����� ������ �������� ��� ������ ������ (��� ������� 1)
	int currentFrame;	// ������� ���� �������� ������
};


public:
	CLoad3DS();		// ��� ����� ����������������� ��� ������
    ~CLoad3DS(void);
	// ��� �-� � ����� ��������� ��� �������� 3DS
	bool Import3DS(t3DModel *pModel,const char *strFileName);
 
private:
	// ������ ������ � ��������� � � ���������� ������ char-��
	int GetString(char *);
 
	// ������ ��������� chunk
	void ReadChunk(tChunk *);
 
	// ������ ��������� ������� chunk
	void ProcessNextChunk(t3DModel *pModel, tChunk *);
 
	// ������ chunk-� �������
	void ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *);
 
	// ������ chunk-� ���������
	void ProcessNextMaterialChunk(t3DModel *pModel, tChunk *);
 
	// ������ RGB-�������� ����� �������
	void ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk);
 
	// ������ ������� �������
	void ReadVertices(t3DObject *pObject, tChunk *);
 
	// ������ ���������� ��������� �������
	void ReadVertexIndices(t3DObject *pObject, tChunk *);
 
	// ������ ���������� ���������� �������
	void ReadUVCoordinates(t3DObject *pObject, tChunk *);
 
	// ������ ��� ���������, ������������ �������, � ������������� materialID
	void ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk);
 
	// ������������ ������� ������ �������
	void ComputeNormals(t3DModel *pModel);
 
	// This frees memory and closes the file
	// ����������� ������ � ��������� ����
	void CleanUp();

// ����� �������:
	// ������ chunk-� �������� ������ (������ ��������
	void ProcessNextKeyFrameChunk(t3DModel *pModel, tChunk *);
 
	// ������ ������� ��� ������� ����� ��������
	void ReadKeyFramePositions(t3DModel *pModel, tChunk *pPreviousChunk);
 
	// ������ �������� ��� �������� ������
	void ReadKeyFrameRotations(t3DModel *pModel, tChunk *pPreviousChunk);
 
	// ������ ������ scale ��� �������� ������
	void ReadKeyFrameScales(t3DModel *pModel, tChunk *pPreviousChunk);
 
	// ������������� ������� ������, ��� ������� �������� ��������
	void SetCurrentObject(t3DModel *pModel, char *strObjectName);
 
// � ����� ����������
	// ��� ��� ������������ � �������� �������� ��� �������� ���������� chunk-��
	tChunk *m_CurrentChunk;
	tChunk *m_TempChunk;
 
	// ������ ������, ��� �������� �������� ������ ��������
	t3DObject *m_CurrentObject;

	// ��������� �� ����
	FILE *m_FilePointer;
};
 
 
#endif
