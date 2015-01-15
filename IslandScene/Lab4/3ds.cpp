#include "stdafx.h"
#include "3DLib.h"
#include "3ds.h"
#include <math.h>
int gBuffer[90000] = {0};	// ������������ ��� ������ ������������� ������
 
// � ���� ����� ��������� ���� ���, ����������� ��� �������� ������ .3ds.
// ��� ��� ��������: �� ���������� chunk, ����� ���������� ��� ID.
// � ����������� �� ��� ID, ���������� ����������, ���������� � chunk-�.
// ���� �� �� ������ ������ ��� ����������, ������� ������ ��.
// �� ������, ��� ����� ���� ����� ����������, ��� ��� ������ chunk ������
// ���� ������ � ������.
 


///////////////////////////////// ROUND FLOAT """"""""""\\*
/////
/////	����� �������: ��������� float �� ����, ���� �� ������ 0.001 ��� ������ -0.001
/////
///////////////////////////////// ROUND FLOAT """"""""""\\*
 
float RoundFloat(float number)
{
 
	// �� �����-�� �������� ������� ����� �������� ����� ��������� �����,
	// ��� ����������� � �����-�� ������������, ������� ����������� ��������������
	// OpenGL. ��� ��� �� ������ �������� �� �� 0. ������� �� �� ��������, �
	// �������� ����� �����.
 
	// ���� �������� ����� ��������� �����, ������������� ��� � 0
	if(number > 0 && number <  0.001f) number = 0;
	if(number < 0 && number > -0.001f) number = 0;
 
	// ���������� ��������� ��� ����������� float
	return number;
}


 
///////////////////////////////// CLOAD3DS """"""""""\\*
/////
/////	����������� �������������� ������ tChunk
/////
///////////////////////////////// CLOAD3DS """"""""""\\*

CLoad3DS::CLoad3DS()
{
	m_CurrentChunk = new tChunk;
	m_TempChunk = new tChunk;
	m_CurrentObject=NULL;
}
CLoad3DS::~CLoad3DS(void)
{
    CleanUp();
}
///////////////////////////////// IMPORT 3DS """"""""""\\*
/////
/////	���������� �������� ��� ��������, ������ � ����� ������� .3ds
/////
///////////////////////////////// IMPORT 3DS """"""""""\\*
 
bool CLoad3DS::Import3DS(t3DModel *pModel,const char *strFileName)
{
	char strMessage[2550] = {0};
    pModel->numOfMaterials = 0;
	pModel->numOfObjects   = 0;
	pModel->numberOfFrames = 0;
	// ��������� .3ds ����
	m_FilePointer = fopen(strFileName, "rb");
 
	// ��������, ��� ��������� �� ���� ����� (�� ������� ����)
	if(!m_FilePointer)
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
 
	// ������ ����, ����� ��������� ����� �����, ����� ��������, ��� ��� 3DS.
	// ���� ��� ������ ����, �� ������ ID chunk-� ����� PRIMARY
 
	// ������ ������ chunk �����, ����� ��������, ��� ��� 3DS
	ReadChunk(m_CurrentChunk);
 
	// ��������, ��� ��� 3DS
	if (m_CurrentChunk->ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
 	// ������ �������� ������ ������. ProcessNextChunk() - ����������� �������
 	// �������� �������� �������� ������� ����������� �������
	ProcessNextChunk(pModel, m_CurrentChunk);
 	// ����� ��������� ����� ����� ��� ����� ���������� ������� ������
	ComputeNormals(pModel);
 	fclose(m_FilePointer);
 	return true;
}
 
///////////////////////////////// CLEAN UP """"""""""\\*
/////
/////	������� ������ ��� ������� ������ � ��������� ����
/////
///////////////////////////////// CLEAN UP """"""""""\\*
 
void CLoad3DS::CleanUp()
{

	
	delete m_CurrentChunk;
	delete m_TempChunk;
}
 
 
///////////////////////////////// PROCESS NEXT CHUNK""""""""""\\*
/////
/////	������� ������ ������� ������ �����, ����� ���������� ��� ������
/////
///////////////////////////////// PROCESS NEXT CHUNK""""""""""\\*

void CLoad3DS::ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	t3DObject newObject = {0};		// ������������ ��� ���������� ������� � ������
	tMaterialInfo newTexture = {0};		// ������������ ��� ���������� ���������
	int version = 0;                    // ������ �����
 
	m_CurrentChunk = new tChunk;		// ������� chunk ��� ��������
 
	// ���������� ������ ���������, ���� �� ������ �� ����� ������ �����.
	// ����� ������ ���� ������, ����������� ����������� ����� � ����������
	// �� � ����� �������.
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// ������ ��������� chunk
		ReadChunk(m_CurrentChunk);
 
		// �������� chunk ID
		switch (m_CurrentChunk->ID)
		{
		case VERSION:			// ������ �����
 
			// ������ ������ ����� � ��������� ����������� ����� � ���������� bytesRead
			m_CurrentChunk->bytesRead += fread(&version, 1, m_CurrentChunk->length - 
				m_CurrentChunk->bytesRead, m_FilePointer);
 
			// ���� ������ ����� ������ 3, ������� ��������������, ��� �����
			// ���������� ��������.
			if (version > 0x03)
				MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
			break;

		case KEYFRAME:
 
			// ����� �� �������� ������ ���������� � �������� ������.
			// ��� �������� � ����� �����. 
 
			// ���������� ��������� chunk
			ProcessNextKeyFrameChunk(pModel, m_CurrentChunk);
 
			// ����������� ������� ��������� ����
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, m_CurrentChunk->length - 
						m_CurrentChunk->bytesRead, m_FilePointer);
//			MessageBox(NULL,"test","e",MB_OK);
			break;
 
		case OBJECTINFO:		// �������� ������ ����
			// ���� chunk �������� ������ ����. ����� ��� ��������� ��� chunk-�� MATERIAL
			// � OBJECT. ������ �� �������� ������ ���������� ���������� � ��������.
 
			// ������ ��������� chunk
			ReadChunk(m_TempChunk);
 
			// �������� ������ ����
			m_TempChunk->bytesRead += fread(&version, 1, m_TempChunk->length - 
				m_TempChunk->bytesRead, m_FilePointer);
 
			// ����������� bytesRead �� ����� ����������� ����
			m_CurrentChunk->bytesRead += m_TempChunk->bytesRead;
 
			// ��������� � ���������� chunk-�, ��� ����� MATERIAL, ����� OBJECT
			ProcessNextChunk(pModel, m_CurrentChunk);
			break;
		case MATERIAL:			// �������� ���������� � ���������
 
			// ���� chunk - ����� ��� ���������� � ���������
 
			// ����������� ����� ����������
			pModel->numOfMaterials++;
 
 
			// ��������� ������ ��������� �������� � ��� ������ �������.
			pModel->pMaterials.push_back(newTexture);
 
			// �������� �������, �������������� ��������
			ProcessNextMaterialChunk(pModel, m_CurrentChunk);
			break;
 
		case OBJECT:			// ������ ��� ��������� �������
 
			// ���� chunk - ����� ��� chunk-��, �������� ���������� �������.
			// ����� �� ������ ��� �������.
 
			// ����������� ������� ��������
			pModel->numOfObjects++;
 
			// ��������� ����� ������� tObject � ������ ��������
			pModel->pObject.push_back(newObject);
 
			// �������������� ������ � ��� ��� ������
			memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));
 
			// �������� � ��������� ��� �������, ����� ����������� ������� ����������� ����
			m_CurrentChunk->bytesRead += GetString(pModel->pObject[pModel->numOfObjects - 1].strName);
 
			// ��������� � ������ ���������� ���������� �������
			ProcessNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), m_CurrentChunk);
			break;
 
		default:
 
			// ��������� chunk-�, ������� ��� �� �����, ����� ���������� �����. ���
			// �� ��� ����� ��������� � "��������" ���������� ����������� ��� ������������
			// chunk-� � ��������� ������� ����������� ����.
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, m_CurrentChunk->length - 
				m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}
 
		// �������� ����������� ����� ���������� chunk-� � ��������
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}
 
///////////////////////////////// PROCESS NEXT KEYFRAME CHUNK """"""""""\\*
/////
/////	��� ������� � ��������� ������ ��������.
/////	�� �������� ��� �������, ��� �������� �������� ��������, ����� ������������� ������,
/////	����� �����, �����������, �������� � ����������� ��� �������� ������. ����� ��
/////	������������� ��� ������ ����� ��������� �������.
/////
///////////////////////////////// PROCESS NEXT KEYFRAME CHUNK """"""""""\\*
 
void CLoad3DS::ProcessNextKeyFrameChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
//	MessageBox(NULL,"test","e",MB_OK);
	char strKeyFrameObject[500] = {0};	// ������ ��� �������� �������
	float temp = 0.0f;			// ��� ����� ��������� y � z ����� �����
 
	// �������������� ����� chunk ��� ������ � ���
	m_CurrentChunk = new tChunk;
 
	// ���������� ������ ������, ���� �� ��������� ����� ���������
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{

//	MessageBox(NULL,"yes","e",MB_OK);

		// ������ ��������� chunk
		ReadChunk(m_CurrentChunk);
 
		// ���������, ��� �� ������ �� ������
		switch (m_CurrentChunk->ID)
		{
		case KEYFRAME_MESH_INFO:	// ������� ���, ��� ����������� ���������� ��� ������ �������

			// ��� �������� ���, ��� �������� �������� ��� ���������� �������, ��� ���
			// ����� ���������� ������ ������ chunk
			ProcessNextKeyFrameChunk(pModel, m_CurrentChunk);
			break;
 
		case KEYFRAME_OBJECT_NAME:	// ������ ��� �������� �������
 
			// �������� ��� �������, ��� �������� �������� ������ ��������
			m_CurrentChunk->bytesRead += GetString(strKeyFrameObject);
 
			// ������, �����, ��� ������ ������� �������� ������, ������������� m_CurrentObject.
			// ����� ������� � �� �������� ��������� �� ������ ������, ���������� ������
			// ��������.
			SetCurrentObject(pModel, strKeyFrameObject);
 
			// ������ 2 ���������� ����� � ������������� ����� (3 short-� - �� ������������
			// � ���� �����).
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, 
					m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
 
		case KEYFRAME_START_AND_END:		// ���� chunk ������ ��������� � �������� �����.
 
			// ������ ��������� � �������� �����. ������ ������������ ��������� ����, �.�.
			// ���������������, ��� �� ������ �������� � 0 �����
			m_CurrentChunk->bytesRead += fread(&(pModel->numberOfFrames), 1, 4, m_FilePointer);
			m_CurrentChunk->bytesRead += fread(&(pModel->numberOfFrames), 1, 4, m_FilePointer);
			break;
 
		case PIVOT:			// ������ ����� ����� �������
 
			// ����� �� ������ 3 float-� (X, Y, Z) ��� ����� ����� �������.
			// ����� ����� - ��������� ���, ������ ������� ��������� ������. �� ���������
			// � ���������� (0, 0, 0), �� ��� ����� ���� ��������.
			m_CurrentObject->numOfFaces;
			if(m_CurrentObject==NULL) break;  //ROMA
			m_CurrentChunk->bytesRead +=
			fread(&(m_CurrentObject->vPivot), 1, sizeof(CVector3), m_FilePointer);
 
			// ��� ��� � 3DS Max ��� Z � Y �����������, ������ ������� ����������
			// z � y ����� �����, ����� ������ ������������� Z.
			
			temp = m_CurrentObject->vPivot.y;
			m_CurrentObject->vPivot.y = m_CurrentObject->vPivot.z;
			m_CurrentObject->vPivot.z = -temp;
			break;
 
		case POSITION_TRACK_TAG:	// ������ ����������� ��� ������� �����
 
			// ������ ��� ����� ��������� ������� ��� ������� ����� ��������
			ReadKeyFramePositions(pModel, m_CurrentChunk);
			break;
 
		case ROTATION_TRACK_TAG:	// ������ �������� ��� ������� ��������� �����
 
			// ������ ��� ����� ��������� �������� ��� ������� ��������� ����� ��������.
			// ������ �������� ��� ������� �������� ����� �� �������� ���, ��������,
			// �����������, ��� ��� ��� ����� ��������������� �� �������.
			ReadKeyFrameRotations(pModel, m_CurrentChunk);
			break;
 
		case SCALE_TRACK_TAG:		// ������ scale-�������� ��� ������� �����
 
			// ������ ������ �������� ����������� ��� ������� ����� ��������
			ReadKeyFrameScales(pModel, m_CurrentChunk);
			break;
 
		default:
 
			// ������ � "�����" ��������/����������� ������
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, 
					m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}
 
		// ����������� ������� ����������� ����
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}
 
	// ����������� ������� chunk � ������������ � �����������
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}

///////////////////////////////// READ KEYFRAME POSITIONS """"""""""\\*
/////
/////	������� ������ ������� �������� ������� ��� ������� �����
/////
///////////////////////////////// READ KEYFRAME POSITIONS """"""""""\\*
 
void CLoad3DS::ReadKeyFramePositions(t3DModel *pModel, tChunk *pPreviousChunk)
{
	short frameNumber= 0, flags= 0, ignored= 0;
	long  lunknown= 0;
	int i;
 
 
	// ��� ������� ����� ������ ��������� ������� ��� ������� �����. ���������, ���
	// ��������� ���������� ��� ����� �����, � �� �� ��� ������������ ��������� �������.
	// ������ 5 �������� � ���� ����� ������������.
 
	// ��������� � "�����" ������������ ������
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
 
	// ������ ����� ������ � ���������� �������, �.�. ������� ��� ������ �����������
	pPreviousChunk->bytesRead += fread(&(m_CurrentObject->positionFrames), 1, sizeof(short), m_FilePointer);
 
	// ������ � "�����" ������������ ������
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
 
	// ������ ��� ����� ������ ����� ��� ����� �������� � ���������� �������
	// ������� ��� ������� �����. ����� �� ���� ��� 50 �������, ��������� �� 100 ������,
	// ��� ����� ���������� ������� � ���������� �����.
	for(i = 0; i <= (pModel->numberOfFrames + 1); i++)
	{
		// ����� �� ��������� ����� CVector3 � ��� ������ �������. �� ����� ���������
		// ������� ������� ��� �������� ����� �������� 'i'. ��������� STL vector, ��
		// ������ ������� ����������� CVector3.
		m_CurrentObject->vPosition.push_back(CVector3());
 
		// ���� ������� ���� �������� ������ � ����� ��������� �������, ������ ���������
		// ������� ��� �������� �����.
		if(i < m_CurrentObject->positionFrames)
		{
			// ������ ������� ����������
			pPreviousChunk->bytesRead += fread(&frameNumber, 1, sizeof(short), m_FilePointer);
 
			// ������ � "�����" ����������� long
			pPreviousChunk->bytesRead += fread(&lunknown, 1, sizeof(long), m_FilePointer);
 
			// ������ 3 float, ���������� (x,y,z) �������. ���������, CVector3 - ���
			// 3 ������, ��� ��� ������� ��� ������ sizeof(float)*3
			pPreviousChunk->bytesRead +=
			fread(&(m_CurrentObject->vPosition[i]), 1, sizeof(CVector3), m_FilePointer);
 
			// ��� ��� 3DS Max ������ ������� Y<->Z, ������������ ���� ����������
			float temp = m_CurrentObject->vPosition[i].y;
			m_CurrentObject->vPosition[i].y = m_CurrentObject->vPosition[i].z;
			m_CurrentObject->vPosition[i].z = -temp;
		}
		// ����� ������ ������������� ������� ������� ������ �������
		else
		{
			m_CurrentObject->vPosition[i] = m_CurrentObject->vPosition[m_CurrentObject->positionFrames - 1];
		}
	}
 
	// ������ ��� ����� ������� ����� ����� �� ������ �������. 3DS ����� ���������
	// ������� ������� ��� ������� ���� ����� ����� (�����). �� ��������, ��� ��
	// ����� �������� ������� �������� ����� �� ������ �����. ��� ������, ���
	// 3DS ����� ������ ������� ����� ����� ��� ������� �����. �� �����, ����� 
	// ����� ����� ���������� � 0, ��� ��� ����� �������� glTranslate(), ��� �����
	// ���������� ������ �������.
	for(i = 0; i < m_CurrentObject->numOfVerts; i++)
	{
		// �������� ������� ������� ����� � ����� ����� �� ������ �������
		m_CurrentObject->pVerts[i].x -= m_CurrentObject->vPosition[0].x + m_CurrentObject->vPivot.x;
		m_CurrentObject->pVerts[i].y -= m_CurrentObject->vPosition[0].y + m_CurrentObject->vPivot.y;
		m_CurrentObject->pVerts[i].z -= m_CurrentObject->vPosition[0].z + m_CurrentObject->vPivot.z;
	}
}
 
 
///////////////////////////////// READ KEYFRAME ROTATIONS """"""""""\\*
/////
/////	����� �������: ������ �������� �������� ������� �� ������ ��������
/////
///////////////////////////////// READ KEYFRAME ROTATIONS """"""""""\\*
 
void CLoad3DS::ReadKeyFrameRotations(t3DModel *pModel, tChunk *pPreviousChunk)
{
	short frameNumber = 0, flags = 0, rotkeys = 0, ignored = 0;
	long lunknown = 0;
	float rotationDegree = 0;
	vector<int> vFrameNumber;
	vector<float> vRotDegree;
	vector<CVector3> vRotation;
	int i;
 
	// ��� ������� ����� ������ ��� � ���� �������� ��� ������� ��������� �����.
	// �������, �������� ���������� ������ ������� ����� �������. ������ 5 
	// short-�������� ������������ � ���� �����.
 
	// ������ � "�����" ������������ ������
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1,  sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1,  sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1,  sizeof(short), m_FilePointer);
 
	// ������ ����� ������ �������� ��� ��������. �������, ��� ������ �����������
	// �� ��� ������� �����, � ������ ��� ��������. ������� ��� ���� �����
	// ������� ��������������� ������ ����� ������ ����������.
	pPreviousChunk->bytesRead += fread(&(m_CurrentObject->rotationFrames), 1, sizeof(short), m_FilePointer);
 
	// ������ ������������ ������
	pPreviousChunk->bytesRead += fread(&ignored, 1,  sizeof(short), m_FilePointer);
 
	// ������ ����� ������ ����� ��� ����� �������� � ���������� �������� �������
	// ��� ������� �����. ��� ����� ��������������� �������� ����� ���������
	// �������, ���� �� ����� 1.
	for(i = 0; i < m_CurrentObject->rotationFrames; i++)
	{
		// ��������� ����� CVector3 � ������ ��������. �� ����� ������� ������� ����
		// �������� ��� ����� 'i'. ��������� STL vector, ������ ��������� �����������
		// CVector3 ����� ���������� �����������.
		vRotation.push_back(CVector3());
 
		// ����� ������ ����� �����
		pPreviousChunk->bytesRead += fread(&frameNumber, 1, sizeof(short), m_FilePointer);
 
		// ��������� ����� ����� � ������ ������� ������ ��� ���������� ������������
		vFrameNumber.push_back(frameNumber);
 
		// ������ ����������� ������ � "�����"
		pPreviousChunk->bytesRead += fread(&lunknown, 1, sizeof(long), m_FilePointer);
 
		// ������ ������� ���� �������� ��� ������� ��������� �����.
		pPreviousChunk->bytesRead += fread(&rotationDegree, 1, sizeof(float), m_FilePointer);
 
		// ��� ��� 3DS Max ������ ���� � ��������, � �� ��������, ����������� ��������.
		// ������������ radian->degree (Radians * (180 / PI) = degrees)
		rotationDegree = rotationDegree * (180.0f / 3.14159f);
 
		// ��������� ���� �������� � ������
		vRotDegree.push_back(rotationDegree);
 
		// ������ ���, ������ ������� ��������� ������.
		pPreviousChunk->bytesRead += fread(&(vRotation[i]), 1, sizeof(CVector3), m_FilePointer);
 
		// ��������� ���� �� ��������� �������� �������
		vRotation[i].x = RoundFloat(vRotation[i].x);
		vRotation[i].y = RoundFloat(vRotation[i].y);
		vRotation[i].z = RoundFloat(vRotation[i].z);
 
		// ����������� ��� y<->z
		float temp = vRotation[i].y;
		vRotation[i].y = -vRotation[i].z;
		vRotation[i].z = temp;
 
		// ������ �������� X �������������
		vRotation[i].x *= -1;
	}
 
	// ������ � ��� ���� ��� ������ � ��������. ������, ��������� ������ � ��������
	// �������� ��� �������� ������, ����� �� ���������������.
 
	// ��������� � ������ ��� ��������
	m_CurrentObject->vRotation.push_back(vRotation[0]);
 
	// ��������� ���� �������� ������� ����� � ������. ���� �������� ���, ���� == 0
	m_CurrentObject->vRotDegree.push_back(vRotDegree[0]);
 
	// ������� ��� �������� ����� ��������
	int currentKey = 1;
 
	// ����� ������ ����� ��� ����� �������� ��� ���, ����� ��������������� ������
	// ��������. ���� ���� �������� ������ ����, ������ ������������� ��������� ����
	// �������� � ����.
 
	// �������� ����� ��� ����� ��������
	for(i = 1; i <= (pModel->numberOfFrames + 1); i++)
	{
		// ���������, �� ����� �� ������� ���� ����� ������ ��������
		if(currentKey < m_CurrentObject->rotationFrames)
		{
			// �������� ������� � ���������� ������ ������ � ���� ��������.
			int currentFrame = vFrameNumber[currentKey];
			int previousFrame = vFrameNumber[currentKey - 1];
			float degree = vRotDegree[currentKey];
 
			// ������������� ���� �������� ����� ������� � ���������� ���������
			// �������. �������� �����, ������ ��������� ��� ����� ������ ������ ��
			// �������� �� �������� ���������. ����� ����� ���� �������� �� ��� 
			// �������. ��������, ������, ���� ���� 0 � ���� 50. 50-0 = 50, �� ����
			// ����� ������ �������� ��� ���������� ������. 
			float rotDegree = degree / (currentFrame - previousFrame);
 
			// ��������� ���� � ������ �������� ��� ����� �����
			m_CurrentObject->vRotation.push_back(vRotation[currentKey]);
			m_CurrentObject->vRotDegree.push_back(rotDegree);
 
			// ��������, ����� �� ���������� � ����. ���������
			if(vFrameNumber[currentKey] <= i)
				currentKey++;
		}
		// ����� �� ��������� � ��������� �������
		else
		{
			// ������������� ��������� �������� � 0, �.�. ��� ������ �� ����� ��������
			m_CurrentObject->vRotation.push_back(vRotation[currentKey - 1]);
			m_CurrentObject->vRotDegree.push_back(0.0f);
		}
	}
}
 
 
///////////////////////////////// READ KEYFRAME SCALE """"""""""\\*
/////
/////	����� �������: ������ �������� scale ��� �������� ������� � �����
/////
///////////////////////////////// READ KEYFRAME SCALES """"""""""\\*
 
void CLoad3DS::ReadKeyFrameScales(t3DModel *pModel, tChunk *pPreviousChunk)
{
	short frameNumber = 0, ignore = 0, flags = 0;
	long lunknown = 0;
	int i = 0;
 
	// ��� � ������ �����������, ��������� �������� ����������� ��� �������
	// ����� ��������. ��� ������, ��� �� ����� ��������������� ��� ������ 
	// �������. ������ 5 ���������� ������������. 
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
 
	// ������ ����� ������-scale. ���� ���� 100 ������ �������� � ������ 50
	// ������ ��������� �������, ������ ������������� ��������� 50 ������ �
	// �������� ���������� �����.
	pPreviousChunk->bytesRead += fread(&(m_CurrentObject->scaleFrames), 1, sizeof(short), m_FilePointer);
 
	// ������ ������������ ������
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
 
	// ������ ��� ����� ������ ����� ��� ����� �������� � ���������� �������� scale.
	for(i = 0; i <= (pModel->numberOfFrames + 1); i++)
	{
		// ��������� ����� CVector3 � ������.
		m_CurrentObject->vScale.push_back(CVector3());
 
		// ���� ������� ���� ������ �� ������ ����������, ������ ��� ������
		if(i < m_CurrentObject->scaleFrames)
		{
			// ������ ������� ����� �����
			pPreviousChunk->bytesRead += fread(&frameNumber, 1, sizeof(short), m_FilePointer);
 
			// ������ ����������� long
			pPreviousChunk->bytesRead += fread(&lunknown, 1, sizeof(long), m_FilePointer);
 
			// ������ (X, Y, Z) scale-�������� ��� �������� �����. �� ����� ����������
			// �� � glScalef().
			pPreviousChunk->bytesRead +=
			fread(&(m_CurrentObject->vScale[i]), 1, sizeof(CVector3), m_FilePointer);
 
			// ������ ������� Z � Y ���
			float temp = m_CurrentObject->vScale[i].y;
			m_CurrentObject->vScale[i].y = m_CurrentObject->vScale[i].z;
			m_CurrentObject->vScale[i].z = temp;
		}
		// ����� �� ��������� � ������� scale
		else
		{
			// ������������� ������� scale-�������� � �������� ���������� �����
			m_CurrentObject->vScale[i] = m_CurrentObject->vScale[m_CurrentObject->scaleFrames - 1];
		}
	}
}
 
 
///////////////////////////////// SET CURRENT OBJECT """"""""""\\*
/////
/////	����� �������: ������������� ������� ������, ��� ������� ����� ������� ��������
/////
///////////////////////////////// SET CURRENT OBJECT """"""""""\\*
 
void CLoad3DS::SetCurrentObject(t3DModel *pModel, char *strObjectName)
{
	// ������� ��������� ������ � ��� ������� ������ ���� ������.
	// ����� � ������ ������ ������ � ��������������� ������.
 
	// ��������, ��� �������� ���������� ��� �������
	if(!strObjectName)
	{
		// ������������� ������� ������ � NULL � �������
		m_CurrentObject = NULL;
		return;
	}
 
	// �������� ����� ��� ������� � ������ � ���������� �� ��� � ����������
	for(int i = 0; i < pModel->numOfObjects; i++)
	{
		// ��������� ���������� ���
     

		if(strcmp(pModel->pObject[i].strName, strObjectName) == 0)
		{
			// �������� ��������� �� ������ � ���������� ������
			// ��� ����� ������, ��� �������� ������ ����� ���������� ������ ��������.
			m_CurrentObject =&(pModel->pObject[i]);
			return;
		}
	}

	// ������� ��������� �� ������
	//MessageBox(NULL, "ERROR: No object in model with given name! (SetCurrentObject)", "Error!", MB_OK);
 
	// ������������� ������� ������ � NULL, �.�. �� ����� ���
	m_CurrentObject = NULL;
}
 

 
///////////////////////////////// PROCESS NEXT OBJECT CHUNK """"""""""\\*
/////
/////	������� ��������� ��� ���������� �� �������
/////
///////////////////////////////// PROCESS NEXT OBJECT CHUNK """"""""""\\*
 
void CLoad3DS::ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	// ������� chunk, � ������� ��������
	m_CurrentChunk = new tChunk;
 
	// ���������� ������ ��� chunk-�, ���� �� ����� �� ����� ���� ������
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// ������ ��������� ������
		ReadChunk(m_CurrentChunk);
 
		// ���������, ��� ��� �� ������
		switch (m_CurrentChunk->ID)
		{
		case OBJECT_MESH:		// ��� ��� �����, ��� �� ������ ����� ������
 
			// ����� ����� ������, ��������� ��� ���������� ���������
			ProcessNextObjectChunk(pModel, pObject, m_CurrentChunk);
			break;
 
		case OBJECT_VERTICES:		// ������� ������ �������
			ReadVertices(pObject, m_CurrentChunk);
			break;
 
		case OBJECT_FACES:		// �������� �������
			ReadVertexIndices(pObject, m_CurrentChunk);
			break;
 
		case OBJECT_MATERIAL:		// ��� ��������� �������
 
			// ��� ������ ������ ��� ���������, ���������� � ������� ��������. ��� ����� ����
			// ���� ��� ���������� �����. ��� ������ ����� �������� ��������, � �������
			// ��������� �������� (���� �������� �� ������� ��������� �������, ��� ������
			// �������� �������� ������ �� ����� �������). ������ � ��� ����� ������ ����
			// �������� �� ���� ������, ��� ��� ������� ������ ID ���������.
 
			// ������ �� ������ ��� ���������, ������������ � �������
			ReadObjectMaterial(pModel, pObject, m_CurrentChunk);
			break;
 
		case OBJECT_UV:		// ������ ���������� ���������� �������
 
			// ��� ������ �������� ��� UV-���������� �������. ��������� ��.
			ReadUVCoordinates(pObject, m_CurrentChunk);
			break;
 
		default:
 
			// Read past the ignored or unknown chunks
			// ������ ������������/����������� ������ � "��������" ������
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, m_CurrentChunk->length 
						- m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}
 
		// ���������� ����������� ������ � ��������
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}

	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}
 
 
///////////////////////////////// PROCESS NEXT MATERIAL CHUNK """"""""""\\*
/////
/////	��� ������� ������ ��� ���������� � ��������� (��������)
/////
///////////////////////////////// PROCESS NEXT MATERIAL CHUNK """"""""""\\*
 
void CLoad3DS::ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	// ������� chunk ��� ������
	m_CurrentChunk = new tChunk;
 
	// ���������� ������ ��� chunk-�, ���� �� ����� �� ����� ���������
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// ������ ��������� ������
		ReadChunk(m_CurrentChunk);
 
		// ���������, ��� ������ �� ���������
		switch (m_CurrentChunk->ID)
		{
		case MATNAME:		// ��� ������ ������ ��� ���������
 
			// ������ ��� ���������
			m_CurrentChunk->bytesRead += 
						fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName,
						1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
 
		case MATDIFFUSE:		// ������ RGB-���� �������
			ReadColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), m_CurrentChunk);
			break;
 
		case MATMAP:			// ��� ����� ���������� � ��������
 
			// ������ ���������� ���������� � ���������
			ProcessNextMaterialChunk(pModel, m_CurrentChunk);
			break;
 
		case MATMAPFILE:		// ������ ��� ����� ���������
 
			// ������ ��� ����� ���������
			m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile,
						1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
 
		default:
 
			// ������ ��������� ������ � "�����"
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, 
						m_CurrentChunk->length - m_CurrentChunk->bytesRead,
						m_FilePointer);
			break;
		}
 
		// ���������� ����������� ������ � ��������
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}
 
///////////////////////////////// READ CHUNK """"""""""\\*
/////
/////	������� ������ ID chunk-� � ��� ������ � ������
/////
///////////////////////////////// READ CHUNK """"""""""\\*
 
void CLoad3DS::ReadChunk(tChunk *pChunk)
{
	// ������� ������ ID ������ (2 �����).
	// ID chunk-� - ���, ��������, OBJECT/MATERIAL. ��� ������� ���,
	// ����� ������ ����� ���� ��������� � ���� ������.
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);
 
	// ����� ������ ������ ������ (4 �����). ������ �� �����,
	// ������� ������ ��� ����� ����� ���������.
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}
 
///////////////////////////////// GET STRING """"""""""\\*
/////
/////	������ ������ � ������ char-��
/////
///////////////////////////////// GET STRING """"""""""\\*
 
int CLoad3DS::GetString(char *pBuffer)
{
	int index = 0;
 
	// ������ 1 ���� ������, ������ ����� ������
	fread(pBuffer, 1, 1, m_FilePointer);
 
	// ����, ���� �� �������� NULL
	while (*(pBuffer + index++) != 0) {
 
		// ������ ������� �� �����, ���� �� ������� NULL
		fread(pBuffer + index, 1, 1, m_FilePointer);
	}
 
	// ����� ������ ������, �.�. ������� ������ �� ��������� (������� NULL)
	return strlen(pBuffer) + 1;
}
 
 
///////////////////////////////// READ COLOR """"""""""\\*
/////
/////	������ ������ RGB-�����
/////
///////////////////////////////// READ COLOR """"""""""\\*
 
void CLoad3DS::ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk)
{ 
	// ������ ���������� � �����
	ReadChunk(m_TempChunk);
 
	// ������ RGB-���� (3 ����� - �� 0 �� 255)
	m_TempChunk->bytesRead += fread(pMaterial->color, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);
 
	// ����������� �������
	pChunk->bytesRead += m_TempChunk->bytesRead;
}
 
 
///////////////////////////////// READ VERTEX INDECES """"""""""\\*
/////
/////	������� ������ ������� ��� ������� ������
/////
///////////////////////////////// READ VERTEX INDECES """"""""""\\*
 
void CLoad3DS::ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	unsigned short index = 0;		// ������������ ��� ������ ������� �������� ��������
 
	// ����� ��������� ������� ������ ��� �������, ����� ������� ��������� ��
	// �����, ����� ��� �� �����. ���������, ��� ����� ��������� ������ 3 ��
	// 4 �������� ��� ������� ��������. ��� ��������� �������� - ���� ���������
	// ��� 3DS Max, ������� ������ ��� ��� �� ������.
 
	// ������ ����� ��������� ����� �������
	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);
 
	// �������� ���������� ������ ��� ��������� � �������������� ���������
	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);
 
	// �������� ����� ��� �������� ����� �������
	for(int i = 0; i < pObject->numOfFaces; i++)
	{
		// ����� ������ A-B-C ������� ��� ��������, �� ���������� 4-� ��������.
		for(int j = 0; j < 4; j++)
		{
			// ������ ������ ������ ������� ��� �������� ��������
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);
 
			if(j < 3)
			{
				// ��������� ������ � ��������� ���������
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}
 
 
///////////////////////////////// READ UV COORDINATES """"""""""\\*
/////
/////	������� ������ UV-���������� �������
/////
///////////////////////////////// READ UV COORDINATES """"""""""\\*
 
void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
	// ����� ��������� ������� UV-��������� ��� �������, ������� �����
	// ��������� �� ������ ����������, ����� ��� �� �����.
 
	// ������ ����� UV-���������
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);
 
	// �������� ������ ��� �������� UV-���������
	pObject->pTexVerts = new CVector2 [pObject->numTexVertex];
 
	// ������ ���������� ���������� (������ �� 2� float)
	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1,
		pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}
 
 
///////////////////////////////// READ VERTICES """"""""""\\*
/////
/////	������� ������ ������� �������
/////
///////////////////////////////// READ VERTICES """"""""""\\*
 
void CLoad3DS::ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	// ��� � � ����������� chunk-��, ������ ��� ������ ���� �������,
	// ����� ����� �� ����������.
 
	// ������ ����� ������
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);
 
	// �������� ������ ��� ������ � �������������� ���������
	pObject->pVerts = new CVector3 [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(CVector3) * pObject->numOfVerts);
 
	// ������ � ������ ������ (������ �� 3 float)
	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, 
		pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
 
	// ������ ��� ������� ���������. ��� ��� � ������� 3DS Max ������ ����������
	// ���, ����� �������� Y-�������� � Z-�������� ����� ������.
 
	// �������� ����� ��� ������� � ������ y<->z
	for(int i = 0; i < pObject->numOfVerts; i++)
	{
		// ��������� ������ ����-� Y
		float fTempY = pObject->pVerts[i].y;
 
		// ������������� �������� Y � Z
		pObject->pVerts[i].y = pObject->pVerts[i].z;
 
		// ������������� �������� Z � Y
		// � ������ ��� �������������, �.�. � 3ds max Z-��� �����������
		pObject->pVerts[i].z = -fTempY;
	}
}
 
 
///////////////////////////////// READ OBJECT MATERIAL """"""""""\\*
/////
/////	������� ������ ��� ���������, ����������� �� ������, � ������������� materialID
/////
///////////////////////////////// READ OBJECT MATERIAL """"""""""\\*
 
void CLoad3DS::ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	char strMaterial[255] = {0};		// ������ ��� ���������
 
	// *��� ����� ��������?* - �������� - ��� ���� + ���������� ����� �������.
	// ����� �� ������������ ������ ���������� ���� �������, "�����������" � �.�.
	// ������ ��� ����� ������ ���� ��� ��� ���������� �����.
 
	// ������ ��� ���������, ������������ � �������� �������.
	// strMaterial ������ ������ ��������� ������ � ������ ���������, ���� "Material #2" � �.�...
	pPreviousChunk->bytesRead += GetString(strMaterial);
 
	// ������, ���� ��� ���������, ����� ������ ����� ��� ��������� � ��������� ��
	// ����� �� ���������� � �����. ����� ������ �������� � ������ ��� ����������� 
	// ������, ����������� materialID ������� � ������� ����� ���������. 
 
	// �������� ����� ��� ���������
	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		// ���� ������ ��� ����������� �������� ��������� � ������ �������
		if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
		{
			// ���������, ���� �� ���������� �����. ���� strFile ��������
			// ������ � ������� >=1, �������� ����.
			if(strlen(pModel->pMaterials[i].strFile) > 0)
			{
				// ������������� ID ��������� � ������� ������ 'i' � ����������� ��������
				pObject->materialID = i;
 
				// ������������� ���� ��������������� � true
				pObject->bHasTexture = true;
			}
			break;
		}
		else
		{
			// ��������� ����, ����� �������, ���� �� ��� �������� �� ���� �������
			if(pObject->bHasTexture != true)
			{
				// ������������� ID ��������� � -1, ����� �������, ��� ��������� ��� ������� ���
				pObject->materialID = -1;
			}
		}
	}
 
	// ��������� ������ � "�����"
	pPreviousChunk->bytesRead += fread(gBuffer, 1, 
		pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}
 
// *Note*
//
// ���� ���� ��������� �������������� �������, ����������� ������� ������. ��� ���
// �����, ����� ������ ��������� ������������� �����. � ������� ������ �� ��� ������ 
// ��� �������, ��� ��� ��� ������� ����� ������ ���������� ����� 3dmath.h/.cpp
 
//////////////////////////////	Math Functions  ////////////////////////////////*
 
// ������������ �������� ������� (magnitude = sqrt(x^2 + y^2 + z^2)
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))
 
// ������������ ������� ����� 2 ������� � ���������� ���������
CLoad3DS::CVector3 Vector(CLoad3DS::CVector3 vPoint1, CLoad3DS::CVector3 vPoint2)
{
	CLoad3DS::CVector3 vVector;			// ������ �������������� ������
 
	vVector.x = vPoint1.x - vPoint2.x;
	vVector.y = vPoint1.y - vPoint2.y;
	vVector.z = vPoint1.z - vPoint2.z;
 
	return vVector;				// ����� �������������� ������
}
 
// This adds 2 vectors together and returns the result
// ���������� 2 ������� � ���������� ���������
CLoad3DS::CVector3 AddVector(CLoad3DS::CVector3 vVector1, CLoad3DS::CVector3 vVector2)
{
	CLoad3DS::CVector3 vResult;				// ������ �������������� ������
 
	vResult.x = vVector2.x + vVector1.x;
	vResult.y = vVector2.y + vVector1.y;
	vResult.z = vVector2.z + vVector1.z;
 
	return vResult;					// ����� ���������
}
 
// ����� ������ �� ���������� ����� � ���������� ���������
CLoad3DS::CVector3 DivideVectorByScaler(CLoad3DS::CVector3 vVector1, float Scaler)
{
	CLoad3DS::CVector3 vResult;
 
	vResult.x = vVector1.x / Scaler;
	vResult.y = vVector1.y / Scaler;
	vResult.z = vVector1.z / Scaler;
 
	return vResult;
}
 
// ���������� ��������� ������������ (dot product) ���� ��������
CLoad3DS::CVector3 Cross(CLoad3DS::CVector3 vVector1, CLoad3DS::CVector3 vVector2)
{
	CLoad3DS::CVector3 vCross;
 
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
 
	return vCross;
}
 
// ���������� ������� �������
CLoad3DS::CVector3 Normalize(CLoad3DS::CVector3 vNormal)
{
	double Magnitude;
 
	Magnitude = Mag(vNormal);
 
	vNormal.x /= (float)Magnitude;
	vNormal.y /= (float)Magnitude;
	vNormal.z /= (float)Magnitude;
 
	return vNormal;
}
 
///////////////////////////////// COMPUTER NORMALS """"""""""\\*
/////
/////	������� ������������ ������� ��� ������� � ��� ������
/////
///////////////////////////////// COMPUTER NORMALS """"""""""\\*
 
void CLoad3DS::ComputeNormals(t3DModel *pModel)
{
	CVector3 vVector1, vVector2, vNormal, vPoly[3];
 
	// ���� �������� ���, ���������� ���� ���
	if(pModel->numOfObjects <= 0)
		return;
 
	// ��� ����� ������� ������? ��� ��� ���������� �� ��������� ��������? ���� ��
	// ����� ������� ������������, ��� "������� ��������". ���� �� �������� OpenGL
	// ������� �������� ��� ���������, ��� ������ ����� ��������� ������� � ������.
	// ���� �� �� ������� ������� ��� ������ �������, ���������� ������ �����
	// ��������� ����������, �.�. ����� ������������.
 
	// �������� ����� ��� ������� ��� ���������� �� ������
	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		// ������� ������� ������
		t3DObject *pObject = &(pModel->pObject[index]);
 
		// �������� ������ ��� ������ ����������
		CVector3 *pNormals		= new CVector3 [pObject->numOfFaces];
		CVector3 *pTempNormals	= new CVector3 [pObject->numOfFaces];
		pObject->pNormals		= new CVector3 [pObject->numOfVerts];
 
		// �������� ����� ��� �������� �������
		for(int i=0; i < pObject->numOfFaces; i++)
		{
			// ��������� 3 ����� ����� ��������, ����� �������� �������� ����
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];
 
			// ������ �������� ������� ���������
 
			vVector1 = Vector(vPoly[0], vPoly[2]);	// ������ �������� (�� 2� ��� ������)
			vVector2 = Vector(vPoly[2], vPoly[1]);	// ������ ������ ��������
 
			vNormal  = Cross(vVector1, vVector2);	// �������� cross product ��������
			pTempNormals[i] = vNormal;		// �������� ��������� ��-����������������� �������
								// ��� ������
			vNormal  = Normalize(vNormal);		// ������������� cross product ��� �������� ��������
 
			pNormals[i] = vNormal;			// ��������� ������� � ������
		}
 
		//////////////// ������ �������� ��������� ������� /////////////////
 
		CVector3 vSum = {0.0};
		CVector3 vZero = vSum;
		int shared=0;
 
		for (int i = 0; i < pObject->numOfVerts; i++)	// �������� ����� ��� �������
		{
			for (int j = 0; j < pObject->numOfFaces; j++)	// �������� ����� ��� ������������
			{				// ���������, ������������ �� ������� ������ ���������
				if (pObject->pFaces[j].vertIndex[0] == i ||
					pObject->pFaces[j].vertIndex[1] == i ||
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);	// ���������� ��-
								// ����������������� ������� ������� ��������
					shared++;		// ����������� ����� ��������� � ������ ���������
				}
			}
 
			// �������� ������� �������� �� ����� ����� ���������. ������ � �������.
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));
 
			// ����������� ������� ��� �������
			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);
 
			vSum = vZero;			// ���������� �����
			shared = 0;			// � ����� ��������
		}
 
		// ����������� ������ ��������� ����������
		delete [] pTempNormals;
		delete [] pNormals;
	}
}
