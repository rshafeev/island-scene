#include "stdafx.h"
#include "afx.h"
static unsigned char* GetBufFromDC(CDC *pDC, int W, int H)
{

	int bRes;
	HBITMAP hbmScreen;
	unsigned char *buffer=new unsigned char[3*W*H];
	CDC * dc=new CDC();
	
	BITMAPINFO bi;
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = W;
	bi.bmiHeader.biHeight = H;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = 3*W*H;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;
	bi.bmiHeader.biXPelsPerMeter=0;
	bi.bmiHeader.biYPelsPerMeter=0;
	
	hbmScreen =(HBITMAP) pDC->GetCurrentBitmap()->m_hObject;
	dc->CreateCompatibleDC(NULL);
	dc->SelectObject(hbmScreen);
	bRes = GetDIBits( dc->m_hDC, hbmScreen, 0, H, (LPVOID)buffer, &bi, DIB_RGB_COLORS);
	ReleaseDC(NULL,dc->m_hDC);
	delete dc;
	return buffer;
}
static CString GetStrFromEdit(CEdit *Edit)
{
//функция возвращает строку, введенную в CEdit
  CString result="";
int i, nLineCount = Edit->GetLineCount();
CString strText, strLine;

for (i=0; i < nLineCount; i++)
{
   int len = Edit->LineLength(Edit->LineIndex(i));
   if(len==0)continue;
   Edit->GetLine(i, strText.GetBuffer(len), len+1);
   
   strText.ReleaseBuffer(len);
   result+=strText;
}

  return result;
}

static CString GetFileName(CString FullName)
{
	//возвращает имя файла с расширением
		int pos=FullName.ReverseFind('\\');
		FullName.Delete(0,pos+1);
        return FullName;
}
static CString GetPuth(CString FullName)
{ 
	//возвращает путь к файлу
		int pos=FullName.ReverseFind('\\');
		FullName.Delete(pos,FullName.GetLength()-pos);
        return FullName;
}
static CString GetFileNameWithoutExp(CString FullName)  
{
	//возвращает имя файла без расширения
		int pos=FullName.ReverseFind('\\');
		FullName.Delete(0,pos+1);
        pos=FullName.ReverseFind('.');
        FullName.Delete(pos,FullName.GetLength()-pos);
        return FullName;
}

static CString GetExp(CString FullName) 
{
	 //возвращает  расширение
		int pos=FullName.ReverseFind('.');
		FullName.Delete(0,pos+1);
        return FullName;
}
static bool IsFileExists(CString FullName)
{
   //проверяет существование файла на диске
	WIN32_FIND_DATA d;
if(FindFirstFile(FullName,&d)==INVALID_HANDLE_VALUE)
 return false;
return true;
}
static int GetFileSize(CString FullName)
{
	//const int MAX =  
	//char filename[MAX];
 FILE * fp;
//clrscr();
// printf("Input the filename: ");
// scanf("%s", filename);
 if((fp=fopen(FullName, "r"))==NULL)
     return -1;
 fseek(fp, 0L, SEEK_END);
 
 //printf("\n\nThe size of file %s is %l d bytes", filename, ftell(fp));
 //getch();
 int size = ftell(fp);
 fclose(fp);
return size;
}