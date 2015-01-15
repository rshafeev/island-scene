#pragma once
#include "stdafx.h"
#include <math.h>
//===========================================================================================================================
class CVector3
{
public:
	long double x,y,z;	
	CVector3(){x=y=z=0.0;}
	~CVector3(){}
	CVector3(double x,double y,double z)
	{
       this->x=x;
	   this->y=y;
	   this->z=z;
	}

	double GetModul()
	{
      return sqrt(x*x+y*y+z*z);
	}
	static double GetLength(CVector3 p1,CVector3 p2)
	{
      return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z));
	} 
	CVector3 operator+(CVector3 &add)
	{
       CVector3 p;
	   p.x=this->x+add.x;
	   p.y=this->y+add.y;
	   p.z=this->z+add.z;
	   return p;
	}
	double operator*(CVector3 &add)
	{
       CVector3 p;
	   p.x=this->x*add.x;
	   p.y=this->y*add.y;
	   p.z=this->z*add.z;
	   return (p.x+p.y+p.z);
	}
    CVector3 operator-(double sub)
	{
       CVector3 p;
	   p.x=this->x-sub;
	   p.y=this->y-sub;
	   p.z=this->z-sub;
	   return p;
	}
	CVector3 operator-(CVector3 &add)
	{
       CVector3 p;
	   p.x=this->x-add.x;
	   p.y=this->y-add.y;
	   p.z=this->z-add.z;
	   return p;
	}

	CVector3 operator*(double add)
	{
       CVector3 p;
	   p.x=this->x*add;
	   p.y=this->y*add;
	   p.z=this->z*add;   
	   return p;
	}
	CVector3 operator/(double div)
	{
       CVector3 p;
	   if(div==0) return p;
	   p.x=this->x/div;
	   p.y=this->y/div;
	   p.z=this->z/div;   
	   return p;
	}
	bool operator==(CVector3 compare)
 	{
      bool result=false;
       if(compare.x==this->x&&compare.y==this->y&&compare.z==this->z)
		   result=true;
	  return result;
 	}
friend CVector3 operator*(double c,CVector3 v)
{
   return v*c;
}
double GetMinXYZ()
{
  if(x<y&&x<z) return x;
  else
  if(y<x&&y<z) return y;
  else
  return z;

}
};
//===========================================================================================================================
class CVector2
{
public:
	double x,y;	
	CVector2(){x=y=0.0;}
	~CVector2(){}
	CVector2(double x,double y)
	{
       this->x=x;
	   this->y=y;
	}

	double GetModul()
	{
      return sqrt(x*x+y*y);
	}
	static double GetLength(CVector2 p1,CVector2 p2)
	{
      return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
	} 
	CVector2 operator+(CVector2 &add)
	{
       CVector2 p;
	   p.x=this->x+add.x;
	   p.y=this->y+add.y;
	   return p;
	}
	CVector2 operator*(CVector2 &add)
	{
       CVector2 p;
	   p.x=this->x*add.x;
	   p.y=this->y*add.y;
	   return p;
	}

	CVector2 operator-(CVector2 &add)
	{
       CVector2 p;
	   p.x=this->x-add.x;
	   p.y=this->y-add.y;
	   return p;
	}

	CVector2 operator*(double add)
	{
       CVector2 p;
	   p.x=this->x*add;
	   p.y=this->y*add;
	   return p;
	}
	CVector2 operator/(double div)
	{
       CVector2 p;
	   if(div==0) return p;
	   p.x=this->x/div;
	   p.y=this->y/div;
	   return p;
	}
friend CVector2 operator*(double c,CVector2 v)
{
   return v*c;
}
double GetMinXY()
{
  if(x<y) return x;
  else
  return y;

}
};
//===========================================================================================================================