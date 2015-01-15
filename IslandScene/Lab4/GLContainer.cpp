#include "StdAfx.h"
#include "GLContainer.h"
#include "Texture.h"
#include "Functions.h"
#include "Lab4.h"
CGLContainer::CGLContainer(void)
{
	korablCamera.pos = CVector3(0,0,0);
	korablCamera.rotate = CVector3(0,0,0);
	korablCamera.scale = CVector3(1,1,1);
	korablCamera.fi = 0.0;
}

CGLContainer::~CGLContainer(void)
{

}
void CGLContainer::Create(CVector2 ZInterval, CVector2 ScaleXY)		//
{
	this->ScaleXY=ScaleXY;			//растяжение 
	this->ZInterval=ZInterval;
}
void CGLContainer::Init()		//
{
// инициализация ландшафта
	TLandProperties LandProp;
	LandProp.AutoCalcMaxMinHeight  = true;
	LandProp.PosX = CVector2(-3100,-3100);
	LandProp.LandFile = "Resources\\my.raw";
	LandProp.TexFile = "Resources\\texVec.bmp";
	LandProp.StepSize = 32;
	LandProp.width = LandProp.height = 4096;
	LandProp.ScaleXY = CVector2(2.5,2.5);
	LandProp.MinHeight = -800;
	LandProp.MaxHeight = -1;
	landshaft.Init(LandProp);
	//return;

// инициализация плоскости
	CTexture text;
	text.InitTexture("Models\\plane\\tortuga\\ADS_stre.jpg");
	MemoryPloscPt   = LoadPloscToMemory(CRect(150,-100,2500,2500),0,&text,30);
	LoadQuardToMemory();
	LoadWaterPloscToMemory(CRect(-12048,-12048,12048,12048),-700);
// инициализация моделей
	LoadSkyBoxToMemory();
	
	MemoryStena2Pt=LoadModelToMemory("Models\\stena2.3ds",CVector3(0,0,0),CVector3(300.03,300.03,300.03),CVector3(0,90,0));
	MemoryVorotaPt=LoadModelToMemory("Models\\Plane\\Tortuga\\vorota.3ds",CVector3(-870.0,1220.0,770.0),CVector3(150.03,240.03,150.03),CVector3(0,-90,0));
	MemoryKolokol2Pt=LoadModelToMemory("Models\\Plane\\Tortuga\\kolokol2.3ds",CVector3(0,0,0),CVector3(150.03,150.03,150.03),CVector3(0,0,0));
	MemoryHouse1Pt=LoadModelToMemory("Models\\Plane\\Tortuga\\hause1.3ds",CVector3(300,170.0,1300),CVector3(70.03,80.03,70.03),CVector3(0,0,0));
	MemoryHouse2Pt=LoadModelToMemory("Models\\Plane\\Tortuga\\house2.3ds",CVector3(700,170.0,1700.0),CVector3(70.03,80.03,70.03),CVector3(0,180,0));
	MemoryHouse3Pt=LoadModelToMemory("Models\\Plane\\Tortuga\\house3.3ds",CVector3(2100.0,680.0,600.0),CVector3(70.03,80.03,70.03),CVector3(0,0,0));
	MemoryHouse4Pt=LoadModelToMemory("Models\\Plane\\Tortuga\\house4.3ds",CVector3(1800,-200.0,2100.0),CVector3(50.03,60.03,50.03),CVector3(0,110,0));
	

	MemoryKolokolPt=LoadModelToMemory("Models\\Plane\\Tortuga\\kolokol.3ds",CVector3(1650.0,-20.0,1650.0),CVector3(150.03,150.03,150.03),CVector3(0,0,0));
	
	MemoryKorablPt=LoadModelToMemory("Models\\JUNK\\korabl.3ds",CVector3(-200.0,-220.0,-2100.0),CVector3(0.03,0.04,0.03),CVector3(0,0,0));

	
/*	MemoryStena3Pt=LoadModelToMemory("Models\\Plane\\Tortuga\\stena3.3ds",CVector3(1300.0,30.0,400.0),CVector3(30.03,50.03,30.03),CVector3(0,0,0));
	MemoryStena1Pt=LoadModelToMemory("Models\\Plane\\Tortuga\\stena1.3ds",CVector3(1300.0,450.0,1600.0),CVector3(100.03,200.03,100.03),CVector3(0,0,0));
	MemoryStolb1Pt=LoadModelToMemory("Models\\Plane\\Tortuga\\stolb1.3ds",CVector3(1300.0,0.0,1000.0),CVector3(70.03,80.03,70.03),CVector3(110,0,0));
	MemoryStolb2Pt=LoadModelToMemory("Models\\Plane\\Tortuga\\stolb2.3ds",CVector3(1300.0,0.0,1600.0),CVector3(30.03,30.03,30.03),CVector3(0,0,0));
	MemoryVisPt=LoadModelToMemory("Models\\Plane\\Tortuga\\vis.3ds",CVector3(1600.0,-190.0,800.0),CVector3(70.03,80.03,70.03),CVector3(0,0,0));
	MemoryKolodecPt=LoadModelToMemory("Models\\Plane\\Tortuga\\kolodec.3ds",CVector3(700.0,0.0,800.0),CVector3(40.03,50.03,40.03),CVector3(0,0,0));
	MemoryLavkaPt=LoadModelToMemory("Models\\Plane\\Tortuga\\lavka.3ds",CVector3(1300.0,170.0,1600.0),CVector3(70.03,80.03,70.03),CVector3(0,0,0));
	MemoryTelegaPt= LoadModelToMemory("Models\\Plane\\Tortuga\\telega.3ds",CVector3(1700.0,-70.0,2300.0),CVector3(80.03,100.03,80.03),CVector3(0,0,0));
	MemoryFonarPt = LoadModelToMemory("Models\\Plane\\Tortuga\\fonar.3ds",CVector3(2400.0,-540.0,1600.0),CVector3(70.03,80.03,70.303),CVector3(0,0,0));
	MemoryFontanPt=LoadModelToMemory("Models\\Plane\\Tortuga\\fontan.3ds",CVector3(1700.0,0.0,1500.0),CVector3(70.03,80.03,70.03),CVector3(0,270,0));
	*/
}

void CGLContainer::LoadSkyBoxToMemory()
{
	MemoryskyBoxPt = glGenLists(1);
	GLUquadric *Cylinder = gluNewQuadric();
	CTexture texture;
	texture.InitTexture("Resources\\textureWater.bmp");
	
	glNewList(MemoryskyBoxPt, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	texture.Bind();
	gluQuadricTexture(Cylinder, GL_TRUE);
    gluQuadricDrawStyle(Cylinder, GLU_FILL);
	glPushMatrix();
		glTranslatef(0.0,-800,0.0);
		gluSphere(Cylinder,12000,300,300);
	glPopMatrix();
	glEndList();	
}
void CGLContainer::Render()			//перерисовка
{
	landshaft.Render();
	//return;
	glCallList(MemoryPloscPt);
	glCallList(MemoryskyBoxPt);
	glCallList(MemoryKolokolPt);

	//
	glCallList(MemoryWaterPlosc);
	glCallList(MemoryQuardPt);
	glPushMatrix();
	    glRotatef(korablCamera.rotate.x, 1.0f, 0.0, 0.0);					    //выполняет поворот
		glRotatef(korablCamera.rotate.y, 0.0, 1.0f, 0.0);
		glRotatef(korablCamera.rotate.z, 0.0, 0.0, 1.0f);
	    glTranslatef(korablCamera.pos.x,korablCamera.pos.y,korablCamera.pos.z);
		glCallList(MemoryKorablPt);
	glPopMatrix();
		glCallList(MemoryVorotaPt);//=============vorota==============
		glPushMatrix();
			glTranslatef(300.0,-1120.0,600.0);
			glCallList(MemoryStena2Pt);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(300,-1120.0,1295);
			glCallList(MemoryStena2Pt);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(300.0,-1120.0,1990);
			glCallList(MemoryStena2Pt);
		glPopMatrix();


		glPushMatrix();
			glTranslatef(-2000.0,-1120.0,600.0);
			glCallList(MemoryStena2Pt);
		glPopMatrix();

		
		glPushMatrix();
			glTranslatef(-2000.0,-1120.0,1990);
			glCallList(MemoryStena2Pt);
		glPopMatrix();



		glPushMatrix();
			glTranslatef(2100.0,120.0,2100);
			glCallList(MemoryKolokol2Pt);//=============kolokol==============
		glPopMatrix();

		glPushMatrix();
			glTranslatef(2100.0,120.0,-200.0);
			glCallList(MemoryKolokol2Pt);//=============kolokol==============
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-200.0,120.0,2100);
			glCallList(MemoryKolokol2Pt);//=============kolokol==============
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-200,120.0,-200);
			glCallList(MemoryKolokol2Pt);//=============kolokol==============
		glPopMatrix();

		glPushMatrix();
			glRotatef(90,0.0,1.0,0.0);
			glTranslatef(-2100.0,-1120.0,1950.0);
			glCallList(MemoryStena2Pt);
		glPopMatrix();
		glPushMatrix();
			glRotatef(90,0.0,1.0,0.0);
			glTranslatef(-2100.0,-1120.0,1260.0);
			glCallList(MemoryStena2Pt);
		glPopMatrix();
		glPushMatrix();
			glRotatef(90,0.0,1.0,0.0);
			glTranslatef(-2100,-1120.0,570.0);
			glCallList(MemoryStena2Pt);
		glPopMatrix();

		glPushMatrix();
			glRotatef(90,0.0,1.0,0.0);
			glTranslatef(-4650.0,-1120.0,1950.0);
			glCallList(MemoryStena2Pt);
		glPopMatrix();
		glPushMatrix();
			glRotatef(90,0.0,1.0,0.0);
			glTranslatef(-4650.0,-1120.0,1260.0);
			glCallList(MemoryStena2Pt);
		glPopMatrix();

		glPushMatrix();
			glRotatef(90,0.0,1.0,0.0);
			glTranslatef(-4650.0,-1120.0,570.0);
			glCallList(MemoryStena2Pt);
		glPopMatrix();

//========================================================	
		glCallList(MemoryHouse1Pt);//=============dom1==============
		glPushMatrix();
			glTranslatef(400,0,0);
			glCallList(MemoryHouse1Pt);
		glPopMatrix();

		glCallList(MemoryHouse2Pt);//=============dom2==============

		glCallList(MemoryHouse3Pt);//=============dom3==============
		glPushMatrix();
			glTranslatef(0,0,700);
		glCallList(MemoryHouse3Pt);//=============dom3==============
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0,0,1400);
		glCallList(MemoryHouse3Pt);//=============dom3==============
		glPopMatrix();

		glCallList(MemoryHouse4Pt);//=============dom4==============
	
	


}
void CGLContainer::LoadWaterPloscToMemory(CRect r, int h)
{
	double koef = 20;
	CTexture text;
	MemoryWaterPlosc = glGenLists(1);
	text.InitTexture("Resources\\textureWater.bmp");
	glNewList(MemoryWaterPlosc, GL_COMPILE);
	
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glDisable(GL_TEXTURE_2D);
	//text.Bind();
	glColor4f(0.1,0.1,0.5,0.9);
	glBegin(GL_QUADS);
		glNormal3f(1,2,2);	
		glTexCoord2f(0,0);
		glVertex3i(r.left, h, r.top); 
		glTexCoord2f(0,koef);
		glVertex3i(r.left, h, r.bottom); 
		glTexCoord2f(koef,koef);
		glVertex3i(r.right, h, r.bottom); 
		glTexCoord2f(koef,0);
		glVertex3i(r.right, h, r.top);  
	glEnd();
	h+=100;
	glEnable(GL_TEXTURE_2D);
	text.Bind();
	glBegin(GL_QUADS);
		glNormal3f(1,2,2);	
		glTexCoord2f(0,0);
		glVertex3i(r.left, h, r.top); 
		glTexCoord2f(0,koef);
		glVertex3i(r.left, h, r.bottom); 
		glTexCoord2f(koef,koef);
		glVertex3i(r.right, h, r.bottom); 
		glTexCoord2f(koef,0);
		glVertex3i(r.right, h, r.top);  
	glEnd();		
		
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);


	glEndList();
}

GLuint CGLContainer::LoadPloscToMemory(CRect r,int h,CTexture *texture,double texKoef)
{
	double koef = texKoef;
	GLuint MemoryPt = glGenLists(1);
	glNewList(MemoryPt, GL_COMPILE);
	if(texture!=NULL)
	{
		glEnable(GL_TEXTURE_2D);
		texture->Bind();
	}
	glBegin(GL_QUADS);
	
		glNormal3f(1,2,2);	
		glTexCoord2f(0,0);
		glVertex3i(r.left, h, r.top); 
		glTexCoord2f(0,koef);
		glVertex3i(r.left, h, r.bottom); 
		glTexCoord2f(koef,koef);
		glVertex3i(r.right, h, r.bottom); 
		glTexCoord2f(koef,0);
		glVertex3i(r.right, h, r.top);  
		glEnd();
	glEnable(GL_LIGHTING);
	glEndList();

	return MemoryPt;
}

void CGLContainer::LoadQuardToMemory()
{

	CTexture texture;
	texture.InitTexture("Resources\\bassein.bmp");
	
	glEnable(GL_TEXTURE_2D);
	

    MemoryQuardPt = glGenLists(1);
	glNewList(MemoryQuardPt,GL_COMPILE);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0,0.0,1.0);

	GLUquadricObj *Cylinder = gluNewQuadric();

	glEnable(GL_TEXTURE_2D);
	texture.Bind();

	gluQuadricTexture(Cylinder, GL_TRUE);
    gluQuadricDrawStyle(Cylinder, GLU_FILL);
 //первый квадрат (маленький)
glPushMatrix();
glTranslatef(900,0,300);
glRotatef(-90, 1.0, 0.0f, 0.0);
glRotatef(45, 0.0, 0.0f, 1.0);

gluCylinder(Cylinder, 300, 300, 80,4,50);
glPopMatrix();

//второй квадрат (второй ярус)
glPushMatrix();
glTranslatef(900,0,300);
glRotatef(-90, 1.0, 0.0f, 0.0);
glRotatef(45, 0.0, 0.0f, 1.0);
gluCylinder(Cylinder, 330, 330, 60,4,50);
glPopMatrix();

//третий ярус
glPushMatrix();
glTranslatef(900,0,300);
glRotatef(-90, 1.0, 0.0f, 0.0);
glRotatef(45, 0.0, 0.0f, 1.0);
gluCylinder(Cylinder, 370, 370, 40,4,50);

glBegin(GL_QUADS);

double koef = 4;
  glNormal3f(1,2,2); 
  glTexCoord2f(0,0);
  glVertex3i(0, 40, 0); 
  
  
  glNormal3f(1,2,2); 
  glTexCoord2f(0,koef);
  glVertex3i(0, 40, koef); 
  

  glNormal3f(1,2,2); 
  glTexCoord2f(koef,0);
  glVertex3i(1, 40, koef); 
  
  
  glNormal3f(1,2,2); 
  glTexCoord2f(koef,koef);
  glVertex3i(1,40, 0); 
  glEnd();
  glPopMatrix();

  
//вертикальная первый ярус справа
glPushMatrix();
glTranslatef(1100,80,510);
glRotatef(-90, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 13, 13,410,50,50);
glPopMatrix();

 //вертикальная второй уровень справа
    glPushMatrix();
glTranslatef(1150,60,530);
glRotatef(-90, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 16, 16,500,50,50);
 glPopMatrix();

//вертикальная третий уровень справа
glPushMatrix();
glTranslatef(1180,40,560);
glRotatef(-90, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 16, 16,550,50,50);
 glPopMatrix();


 
 //вертикальная первый ярус слева
glPushMatrix();
glTranslatef(1115,80,88);
glRotatef(-90, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 13, 13,430,50,50);
 glPopMatrix();

  //вертикальная второй уровень слева
glPushMatrix();
glTranslatef(1150,60,68);
glRotatef(-90, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 16, 16,500,50,50);
 glPopMatrix();

 //вертикальная третий уровень слева
glPushMatrix();
glTranslatef(1180,40,48);
glRotatef(-90, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 16, 16,548,50,50);
 glPopMatrix();

 //горизонтальная первый ярус
glPushMatrix();
glTranslatef(1115,80,510);
glRotatef(-180, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 13, 13,420,50,50);
 glPopMatrix();

//горизонтальная первый ярус дальняя
glPushMatrix();
glTranslatef(687,80,515);
glRotatef(-180, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 13, 13,433,100,50);
 glPopMatrix();


 //горизонтальная второй уровень

 glPushMatrix();
glTranslatef(1138,60,530);
glRotatef(-180, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 16, 16,460,50,50);
 glPopMatrix();

 //горизонтальная второй уровень дальняя

 glPushMatrix();
glTranslatef(660,60,540);
glRotatef(-180, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 16, 16,480,50,50);
 glPopMatrix();

  //горизонтальная третий уровень

 glPushMatrix();
glTranslatef(640,40,560);
glRotatef(-180, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 15, 15,520,50,50);
 glPopMatrix();


  //горизонтальная третий уровень дальння

 glPushMatrix();
glTranslatef(1167,40,560);
glRotatef(-180, 0.0, 1.0f, 0.0);
gluCylinder(Cylinder, 15, 15,520,50,50);
 glPopMatrix();

 glEndList();

}
void CGLContainer::MoveKorabl()
{
	double x,y;
	double r = 5600;
	korablCamera.fi+=0.001;
	x = r * cos(korablCamera.fi);
	y = r * sin(korablCamera.fi);
	korablCamera.pos.x = x+3000;
	korablCamera.pos.z = y+2300;
	if(korablCamera.fi>100000)
		korablCamera.fi = 0.0;
}
	

void CGLContainer::LoadConeToMemory()
{
	    CTexture text;
        MemoryConePt = glGenLists(1);
	//	 text.InitTexture("Shaders\\textureL3.bmp");
		glNewList(MemoryConePt, GL_COMPILE);
		glEnable(GL_TEXTURE_2D);
	text.Bind();

	glPushMatrix();
		glTranslatef(300,200,300);
		glRotatef(-90, 1.0, 0.0f, 0.0);
		GLUquadricObj *cylquad = gluNewQuadric();
		gluCylinder(cylquad, 60, 0.08, 100, 10, 200 );
		glPopMatrix();
		glEnd();
	glEndList();

}
void CGLContainer::AnimateModel(CLoad3DS::t3DModel *pModel, CLoad3DS::t3DObject *pObject)
{
	// Это самое главное изменение в этом файле. Эта функция контролирует
	// изменения обьекта для каждого кадра анимации. В зависимости от кадра
	// мы перемещаем, затем масштабируем, затем вращаем обьект на необходимые
	// значения. Для каждого обьекта есть список значений для каждого кадра.
 
	// Получаем текущую позицию для этого кадра
	if(pModel->currentFrame>=(int)pObject->vPosition.size()||pModel->currentFrame<0) return;

	CLoad3DS::CVector3 vPosition = pObject->vPosition[pModel->currentFrame];
 
	// Перемещаем обьект
	glTranslatef(vPosition.x, vPosition.y, vPosition.z);
 
	// Получаем значение scale для этого кадра
	CLoad3DS::CVector3 vScale = pObject->vScale[pModel->currentFrame];
 
	// Масштабируем обьект на значение для этого кадра
	glScalef(vScale.x, vScale.y, vScale.z);
 
	// Здесь всё немного по другому. Создаём цикл через все
	// предыдущие кадры, пока не достигнем первого кадра, и
	// вызываем для каждого кадра glRotatef(). Это будет вращать 
	// обьект на маленькие углы, пока не получится значение текущего
	// кадра. Если бы мы использовали матрицы (но я решил не перегружать
	// этот урок), можно было бы просто умножить матрицу вращения
	// на текущую матрицу. 
	// Устанавливаем "i" в 1, т.к. мы игнорируем нулевой кадр анимации.
	for(int i = 1; i <=1/* pModel->currentFrame*/; i++)
	{
		// Получаем текущий угол и ось вращения для текущего кадра
		CLoad3DS::CVector3 vRotation = pObject->vRotation[i];
		float rotDegree = pObject->vRotDegree[i];
 
		// Проверяем, не равен ли угол вращения нулю
		if(rotDegree)
		{
			// Вращаем обьект на угол для текущего кадра
			glRotatef(rotDegree, vRotation.x, vRotation.y, vRotation.z);
		}
	}
}


int CGLContainer::FindTextureForFileName(CString name)
{
	int ind = -1;
	int count = modelTextures.size();
	for(int i=0;i<count;i++)
		if(modelTextures[i].GetTexFileName() ==  name)
			return i;
	return ind;
}
GLuint CGLContainer::LoadModelToMemory(CString FileName,CVector3 translated,CVector3 scaled, CVector3 rotated)
{
	CLoad3DS g_Load3ds;
	CLoad3DS::t3DModel g_3DModel;
	 GLfloat ColorMaterial;
	 int i,j;
	 int koef=22;
	 int whichVertex;
	 int   g_ViewMode	= GL_TRIANGLES;		// По умолчанию режим рендера - GL_TRIANGLES
	 
	 int ind=-1;
	 GLuint MemoryPt = glGenLists(1);
	glEnable(GL_TEXTURE_2D);
	
	if(g_Load3ds.Import3DS(&g_3DModel,(LPCSTR)FileName)==true)
	{
	   		
	    for(int i = 0; i < g_3DModel.numOfMaterials; i++)
	    {
			if(strlen(g_3DModel.pMaterials[i].strFile) > 0)// Проверяем, есть ли в загруженном материале имя файла
			{
				
				int ind = FindTextureForFileName(GetPuth(FileName)+"\\"+g_3DModel.pMaterials[i].strFile);
				if(ind==-1)
				{
					CTexture newTexture;
					newTexture.InitTexture(GetPuth(FileName)+"\\"+g_3DModel.pMaterials[i].strFile);
					modelTextures.push_back(newTexture);
					g_3DModel.pMaterials[i].texureId = modelTextures.size()-1;
				}
				else
					g_3DModel.pMaterials[i].texureId = ind;
				//g_3DModel.pMaterials[i].texureId = i;
				//textures[i].InitTexture(GetPuth(FileName)+"\\"+g_3DModel.pMaterials[i].strFile);// Используем имя файла для загрузки битмапа с текстурным ID (i).
			}
			
	    }
	
	glNewList(MemoryPt,GL_COMPILE);
	
	glPushMatrix();
	glTranslated(translated.x,translated.y,translated.z);
	glScaled(scaled.x,scaled.y,scaled.z);
	glRotatef(rotated.x, 1.0f, 0.0, 0.0);					    //выполняет поворот
	glRotatef(rotated.y, 0.0, 1.0f, 0.0);
	glRotatef(rotated.z, 0.0, 0.0, 1.0f);
	glEnable(GL_COLOR_MATERIAL);		
	for(i = 0; i < g_3DModel.numOfObjects; i++) // Так как мы знаем число обьектов в нашей модели, проходим через каждый из них
	{
		
		if(g_3DModel.pObject.size() <= 0) break;		// Убедимся, что передан верный обьект
		glPushMatrix();
		CLoad3DS::t3DObject *pObject = &(g_3DModel.pObject[i]);// Получим текущий обьект
		g_3DModel.currentFrame =0;
		//AnimateModel(&g_3DModel, pObject);// Анимируем модель
		if(pObject->bHasTexture)                                  // Проверим, имеет ли обьект тексурную карту, если да - биндим на него текстуру
		{
			glEnable(GL_TEXTURE_2D);                              // Включаем текстуры
            glColor3f(0.9,0.9,0.9);
           // GLfloat MaterialAmbient[] = {0.9, 0.9, 0.9, 0.0};
	       // glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);

			//g_3DModel.pMaterials[i].texureId
			int ind = g_3DModel.pMaterials[pObject->materialID].texureId;
			modelTextures[ind].Bind();
	       // glBindTexture(GL_TEXTURE_2D, textures[pObject->materialID].GetTexPt());  // Биндим текстурную карту на обьект по его materialID
		} 
		else
		{
			   glDisable(GL_TEXTURE_2D);// Иначе выключим текстуры
			   glColor3f(0.87,0.87,0.87);
		}
		// Начинаем отрисовку в выбранном режиме
		glBegin(g_ViewMode);	// Рисуем обьекты (треугольники или линии)
		for(j = 0; j < pObject->numOfFaces; j++)// Проходим через все полигоны обьекта и рисуем их
		{
				for(whichVertex = 0; whichVertex < 3; whichVertex++)// Проходим через каждый угол треугольника и рисуем его
				{
					int index = pObject->pFaces[j].vertIndex[whichVertex];// Получаем индекс для каждой точки полигона
					// Передаём текущую вершину обьекта
					if(index<0||pObject->numOfVerts<index) continue;

					glNormal3f(pObject->pNormals[ index ].x, 
					pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);// Передаём OpenGL нормаль этой вершины
					if(pObject->bHasTexture)// Если обьект имеет текстуру, передаем текст. координаты

					{
						
 						if(pObject->pTexVerts)
							glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
						//glTexCoord2f(0,0);
					}
					else
					{
 						// Убедимся, что у нас есть верный материал/цвет, привязанный
						// к обьекту. Вообще практически всегда к обьекту привязан как 
						// минимум цвет, но просто на всякий случай проверим это.
						// Если размер материала минимум 1, и materialID != -1, 
						// материал верен.
						if((g_3DModel.pMaterials.size()>0)&&(pObject->materialID >= 0))
						{
							// Получаем и устанавливаем цвет обьекта, если он
							// не имеет текстуры
							BYTE *pColor = g_3DModel.pMaterials[pObject->materialID].color;
							// Применяем цвет к модели
							glColor3ub(pColor[0], pColor[1], pColor[2]);
						}
					}
  				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y,
								pObject->pVerts[ index ].z);
				}
		}
 		glEnd();
		glPopMatrix();
	  }
	  glPopMatrix();

      glDisable(GL_COLOR_MATERIAL);
      GLfloat MaterialAmbient[] = {0.87, 0.87, 0.87, 0.0};
	  glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);
      glEndList();
	   return MemoryPt;
    }
   
	 return -1;
}
void CGLContainer::print(int x, int y, const char * string, ...)
{
static GLuint base = 0;

	if ( !base )
	{
		base = glGenLists(255);

		HFONT font = CreateFont(18,								// Height Of Font
			0,								// Width Of Font
			0,								// Angle Of Escapement
			0,								// Orientation Angle
			FW_BOLD,						// Font Weight
			FALSE,							// Italic
			FALSE,							// Underline
			FALSE,							// Strikeout
			ANSI_CHARSET,					// Character Set Identifier
			OUT_TT_PRECIS,					// Output Precision
			CLIP_DEFAULT_PRECIS,			// Clipping Precision
			ANTIALIASED_QUALITY,			// Output Quality
			FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
			"Times new Roman");						// Font Name

		HFONT oldfont = (HFONT)SelectObject(wglGetCurrentDC(), font);
		wglUseFontBitmaps(wglGetCurrentDC(), 0, 255, base);
		SelectObject(wglGetCurrentDC(), oldfont);
		DeleteObject(font);
	}

	char text[1024];
	va_list va;

	if ( !string ) return;

	va_start(va, string);
	vsprintf(text, string, va);
	va_end(va);

	glWindowPos2i(x, y);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}