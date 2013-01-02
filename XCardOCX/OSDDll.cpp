// OSDDll.cpp: implementation of the OSDDll class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "OSDDll.h"

#undef INNER_LEFT 
#undef INNER_RIGHT 

// file io/printf headers
#include <stdio.h>
#include <conio.h>
#include <initguid.h>    

#define WIDTHBYTES(i)    ((i+31)/32*4)

#include "mpegcmn.h"
#ifndef USE_RMBASE_CLSID
	#define DEFINE_KS
	#include <impegapi.h>	
	typedef struct {
		DWORD				HeadersCount;
		KSSTREAM_HEADER		header;
	}HEADER;
#else
	#include "irminc.h"
	typedef struct {
		RMMULTIPLE_ITEM		multi;
		RMSTREAM_HEADER		header;
	}HEADER;
#endif

#ifndef USE_RMBASE_CLSID
	IRmOSD		* pOSD;
	IRmDevice	* pDevice;
#else
	IRmStream		* pOSD;
	IRmControlNode	* pDevice;
	IRmMapMem		* pMemAlloc;
#endif

HRESULT	hr;
PUCHAR	pshared = NULL;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern TCHAR BmpFileName[_MAX_PATH];
extern TCHAR OSDFileName[_MAX_PATH];
OSDDll::OSDDll()
{
		singleFlag = false;
}

OSDDll::~OSDDll()
{
	//this->ExitOSD();
}



int OSDDll::FormatAndDraw(CString text,	int iWidth,
		int iHeight, BYTE *pBits2, CString pcolor,CString bcolor,int psize,CString pfont,int wi,int hi, bool han)
{
	TRACE("FormatAndDraw begin");
	//定义图形大小
	
	
    //定义图形大小
	DWORD NumColors=256;
    int iPixel  = 8;
	//图形格式参数
    LPBITMAPINFO lpbmih = new BITMAPINFO;
    lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    lpbmih->bmiHeader.biWidth = iWidth;
    lpbmih->bmiHeader.biHeight = iHeight;
    lpbmih->bmiHeader.biPlanes = 1;
    lpbmih->bmiHeader.biBitCount = iPixel;
    lpbmih->bmiHeader.biCompression = BI_RGB;
    lpbmih->bmiHeader.biSizeImage = iWidth*iHeight;
    lpbmih->bmiHeader.biXPelsPerMeter = 0;
    lpbmih->bmiHeader.biYPelsPerMeter = 0;
    lpbmih->bmiHeader.biClrUsed = 0;
    lpbmih->bmiHeader.biClrImportant = 0;

    //创建位图数据
    HDC hdc,hdcMem;
    HBITMAP hBitMap = NULL;
    CBitmap *pBitMap = NULL;
    CDC *pMemDC = NULL;
    BYTE *pBits;




	HPALETTE           hPalette=NULL;
	HBITMAP            hBitmap=NULL;
	HGLOBAL            hImgData=NULL;


	DWORD              ImgWidth=0 , ImgHeight=0;
    



	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);
   

    hdcMem = CreateCompatibleDC(hdc);
    hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&pBits,NULL,0);
    pBitMap = new CBitmap;
    int kk=pBitMap->Attach(hBitMap);
    pMemDC = new CDC;
    int rr=pMemDC->Attach(hdcMem);
    class CBitmap * dd=pMemDC->SelectObject(pBitMap);


  	
  	 

	//CRect rc(0,0,iWidth,iHeight);
	//pMemDC->SetBkMode(TRANSPARENT);

    //添加自绘图形 
	DrawMemory(pMemDC,psize,text,1,1,1,pfont,han);

	int iBMPBytes = iWidth * iHeight * iPixel / 8;

	int ecolor=atoi(bcolor);
	int cc=atoi(pcolor);
	if (pBits==NULL) {
		for(int z=0;z<iBMPBytes;z++){
			pBits2[z]=0x01;
		}
		TRACE("Error in CreateDIBSection and pMemDC");
		return -1;
	}
	//-------------bian kuang-------------
	for(int j=0;j<iHeight;j++){
		for(int i=0;i<iWidth;i++){
			 int ss=i+j*iWidth;
			 pBits2[ss]=pBits[(iHeight-j-1)*iWidth+i];
			 //change color
			 if (pBits2[ss]==0){
			 	pBits2[ss]=0x01;
			 }else{				
				pBits2[ss]=cc;					
			 }
		}
	}

	if (wi!=0 && hi!=0){
		for(j=0;j<iHeight;j++){
			for(int i=0;i<iWidth;i++){
				int temp=i+j*iWidth;
				if (i<wi && j< hi && pBits2[temp]!=cc){
					pBits2[temp]=ecolor;
				}
			}
		}
	}
	

	/*
	for(j=0;j<iHeight;j++){
		for(int i=0;i<iWidth;i++){
			int ss=i*j;
			int tt=0;
			if(pBits2[ss]==cc){			
				for (int k=-1;k<2;k++){
					if (k==0) continue;
					tt=ss+k;
					if (tt<iBMPBytes && tt>-1 && pBits2[tt]!=cc){
						pBits2[tt]=ecolor;
					}
					tt=ss+iWidth*k;
					if (tt<iBMPBytes && tt>-1 && pBits2[tt]!=cc){
						pBits2[tt]=ecolor;
						tt++;
						if (tt<iBMPBytes && tt>-1 && pBits2[tt]!=cc){
							pBits2[tt]=ecolor;
						}
						tt--;
						tt--;
						if (tt<iBMPBytes && tt>-1 && pBits2[tt]!=cc){
							pBits2[tt]=ecolor;
						}
					}
				}
			}
		 }
	}*/

	pMemDC->DeleteDC();
	pBitMap->DeleteObject();
	//DeleteObject(hdcMem);

    delete pMemDC;	pMemDC  = NULL;
    delete pBitMap; pBitMap = NULL;
    delete lpbmih;  lpbmih  = NULL;
	TRACE("FormatAndDraw end");
	return 0;
}




bool OSDDll::DrawMemory(CDC *pDC, int fsize,CString text,long r,long g,long b,CString textFont,bool han)
{
	
    //页面背景色
	CFont* m_Font=new CFont;
	
	LOGFONT log;
	GetObject(::GetStockObject(DEFAULT_GUI_FONT),sizeof(log),&log);
	log.lfHeight=fsize;
	log.lfWidth=fsize;
	if (han){
		log.lfCharSet=HANGEUL_CHARSET;//HANGEUL_CHARSET;//GB2312_CHARSET;
		//Batang
		lstrcpy(log.lfFaceName,textFont);
	}else{
		log.lfCharSet=GB2312_CHARSET;//HANGEUL_CHARSET;//GB2312_CHARSET;
		lstrcpy(log.lfFaceName,textFont);
	}
	
	m_Font->CreateFontIndirect(&log);
	
	CFont *pOldFond=pDC->SelectObject(m_Font);

	if (pOldFond==NULL){
		TRACE("\nFont ERROR");
	}
	delete m_Font;

	COLORREF m_BackgroundColor=RGB(0x40,0x40,0x40);
	COLORREF m_PenColor=RGB(0xA0,0xA0,0xA0);
	COLORREF oldBKColor=pDC->SetBkColor(m_BackgroundColor);
	COLORREF oldPenColor=pDC->SetTextColor(m_PenColor);

	
	if (oldBKColor!=NULL){
		TRACE("\noldBKColor is ok \n");
	}
	if (oldPenColor!=NULL){
		TRACE("\noldPenColor is ok \n");
	}

	//CPen pen;
	//pen.CreatePen(PS_SOLID, 2, RGB(0x60,0x40,0xE0));
	//CPen *oldPen = pDC->SelectObject(&pen);	
	//if (oldPen!=NULL){
	//	TRACE("OK3\n");
	//}

	//pDC->SelectObject(oldPen);	
	//pen.DeleteObject();
	//oldPen = NULL;
	
	if (han){
		int nret;
		int sizebuffer= 2*text.GetLength();
		char* lpBuffer=new char[sizebuffer];
		int charset;
		charset=HANGEUL_CHARSET;//GB2312_CHARSET;
			CString s;
		BSTR bb = text.AllocSysString();

	#ifdef UNICODE
		s = text;
	#else
		LPSTR p = s.GetBuffer(SysStringLen(bb)  + 1); 
		::WideCharToMultiByte(CP_ACP,            // ANSI Code Page
							  0,                 // no flags
							  bb,                 // source widechar string
							  -1,                // assume NUL-terminated
							  p,                 // target buffer
							  SysStringLen(bb) +1, // target buffer length
							  NULL,              // use system default char
							  NULL);             // don''t care if default used
		s.ReleaseBuffer();
	#endif

		pDC->TextOut(INNER_LEFT,INNER_RIGHT,s);
		delete[]lpBuffer;
	}else{
		pDC->TextOut(INNER_LEFT,INNER_RIGHT,text);
	}
	

	//DEFAULT_CHARSET
	//JOHAB_CHARSET
	//GB2312_CHARSET
	//HANGUL_CHARSET
	//HANGEUL_CHARSET

	//CHARSETINFO csinfo={0}; 
	//TranslateCharsetInfo((DWORD*)charset,&csinfo,TCI_SRCCHARSET); 
	//nret=WideCharToMultiByte(csinfo.ciACP,0,LPCWSTR(text.GetBuffer(text.GetLength())) , text.GetLength(),
	//							lpBuffer, sizebuffer ,NULL,NULL);





	return 1;
}
/*void CLangConvert::ConvertUTF8toGB2312(CString str,_bstr_t& bstr)
{
	int n=MultiByteToWideChar(CP_UTF8,0,str,str.GetLength(),NULL,0);
	WCHAR * pChar = new WCHAR[n+1];
	n=MultiByteToWideChar(CP_UTF8,0,str,str.GetLength(),pChar,n);
	pChar[n]=0;
	OLECHAR * pTempBuffer = pChar;
	BSTR strPassword = SysAllocString(pTempBuffer);
	_bstr_t aa (strPassword,FALSE);
	bstr=aa;
	SysFreeString(strPassword);
	delete []pChar;
}*/

BOOL OSDDll::initialDevice(CString text,int left,int top,CString pcolor,CString bcolor,int psize,CString pfont,int wi,int hi,bool han)
{
	TRACE("\nOSD show text:%s",text);
#ifdef MOCK
	return true;
#endif
	if (singleFlag==true) return 1;
	singleFlag=true;

	//int width=500;
	//int heigth=300;
	int width=680;
	int heigth=200;

	BYTE* p=(BYTE*)malloc(width*heigth);
	int r=FormatAndDraw(text,width,heigth,p,pcolor,bcolor,psize,pfont,wi,hi,han);
	if (r<0){
		//Error in format and draw
		singleFlag=false;
		return false;
	}
	showScreen(left,top,width,heigth,p);
	free(p);
	singleFlag=false;
	return true;
}

int OSDDll::showScreen(int destx, int desty,int width, int heigth, BYTE *pBits)
{
	TRACE("showScreen begin");
#ifdef MOCK
	return 1;
#endif
	if (pBits==NULL) return 1;
	
	try{
		if (NULL != pDevice)
			pDevice->Release();
	}catch(CException e){

	}	
	#ifndef USE_RMBASE_CLSID
		if (NULL != pshared)
			free(pshared);
	#else
		if (NULL != pMemAlloc){
			if (NULL != pshared)
				pMemAlloc->FreeDMABuffer(pshared);
			pMemAlloc->Release();
		}
	#endif


	if (pOSD!=NULL){
		pOSD->Release();
	}



	int size1=width*heigth;

	FILE	*f;
	int		i;
	size_t	count, size= 0;
	BYTE	buff_8[8];
	MPEG_OVERLAY_RECT rc_dest ;/*= {100,10,100,100};*/
	
	rc_dest.X=destx;
	rc_dest.Y=desty;
	rc_dest.cX = width;
	rc_dest.cY = heigth;

	#ifndef USE_RMBASE_CLSID

	// use first avaliable device
	//
	hr = CoCreateInstance(CLSID_rm8400,NULL,CLSCTX_INPROC_SERVER,IID_IRmOSD,(void**)&pOSD);
	if (FAILED(hr)) goto exit_1;
#else
	IRmObjectFinder *pIRmObjectFinder;
	hr = CoCreateInstance(CLSID_RMBASE,NULL,CLSCTX_INPROC_SERVER,IID_IRmObjectFinder,(void**)&pIRmObjectFinder);
	if (FAILED(hr)) goto exit_1;

	// GET OSD
	//
	hr = pIRmObjectFinder->FindObject(
					NULL,
					FIRST_AVALIABLE_INSTANCE,
					CATEGORY_PIN_OSD,
					IID_IRmStream,
					(void**)&pOSD);

	pIRmObjectFinder->Release();
	if (FAILED(hr))	goto exit_1;

#endif

  	/////////////////////////////////////////////////////////////////////////////////////
	// do something useful

	pOSD->Reset();

	long dwTvOutKeep;
	{
		pDevice = NULL;
#ifndef USE_RMBASE_CLSID
		hr = pOSD->QueryInterface(IID_IRmDevice,(void**)&pDevice);
#else
		hr = pOSD->QueryInterface(IID_IRmControlNode,(void**)&pDevice);
		if (FAILED(hr)) goto exit_1;
		hr = pOSD->QueryInterface(IID_IRmMapMem,(void**)&pMemAlloc);
#endif
		if (FAILED(hr)) goto exit_1;

		long HwLibVersion = 0, dwTvOut;

		// switch to TV
		//
		pDevice->GetAttributes(MpegAttrCodeVersion,&HwLibVersion);
		pDevice->GetAttributes(MpegAttrVideoTv,&dwTvOutKeep);
		dwTvOut = dwTvOutKeep &~OUTPUT_OFF;

		if (HwLibVersion < 9){
			dwTvOut |= SET_TV;
		}else{
			dwTvOut |= SET_TV | 0x80000000;
			dwTvOut &= ~SET_HDTV;
		}
		pDevice->SetAttributes(MpegAttrVideoTv,dwTvOut);
		
	}

	f = fopen(OSDFileName,"rb");
	if (NULL == f) goto exit_10;

	// Read header
	//
	count = fread(buff_8,sizeof(char),8,f);
	if (count != 8) goto exit_20;

	// extract some bitmap information from header
	//
	
	size = size1+1024+8;

	// rewind to beginning
	//
	if (0 == size) goto exit_20;{
		fpos_t pos = 0;
		if(fsetpos(f,&pos)) goto exit_20;
	}

#ifndef USE_RMBASE_CLSID
	pshared = (PUCHAR)malloc(size);
	if (pshared == NULL){
		goto exit_20;
	}
#else

	// if data will be sent using physically continious memory buffer
	// it will be transferred by one DMA operation. OSD only: anything 
	// more then ~63K will be devided by separate transfers ~63K each,
	// up to 8 transfers can be programmed by one VSYNC.
	//
	PVOID	phys;
	if (pMemAlloc==NULL) {
		return 0;
	}
	hr = pMemAlloc->GetDMABuffer(size,(void**)&pshared,&phys);
	if (pshared == NULL){
		TRACE("showScreen ERROR pshared == NULL");
		goto exit_20;
	}
#endif
	fread(pshared,sizeof(char),1024+8,f);
	
	pshared[1]=size>>16;
	pshared[2]=(size-(size>>16)*65536)>>8;
	pshared[3]=(&size)[0];

	pshared[4]=width>>8;
	pshared[5]=(&width)[0];

	pshared[6]=heigth>>8;
	pshared[7]=(&heigth)[0];

	for(i=0;i<size1;i++){
		if (1024+8+i>size){
			break;
		}
		pshared[1024+8+i]=pBits[i];
	}	

	pOSD->Play();
	for(i=0;i<2;i++)
	{	
		ULONG	ret;
		HEADER hdr;
		ZeroMemory(&hdr,sizeof(hdr));

#ifndef USE_RMBASE_CLSID
		OVERLAPPED ovr = {0,0,0,0,CreateEvent(NULL,TRUE,FALSE,NULL)};
		hdr.HeadersCount = 1;
		hdr.header.Size = sizeof(KSSTREAM_HEADER);
		hdr.header.Data = pshared;
		hdr.header.FrameExtent = size;

		// Send whole bitmap down
		//
		pOSD->Write(&hdr.header,hdr.HeadersCount,&ovr);
#else
		RMOVERLAPIO ovr = {0,0,0,0,CreateEvent(NULL,TRUE,FALSE,NULL)};

		hdr.multi.Count = 1;
		hdr.multi.Size  = sizeof(HEADER);
		hdr.header.Size = sizeof(RMSTREAM_HEADER);
		hdr.header.pData = pshared;
		hdr.header.FrameExtent = size;

		// Send whole bitmap down
		//
		pOSD->Write(&hdr.multi,&ovr);
#endif

		// Wait for completion
		//
		ret = WaitForSingleObject(ovr.hEvent,10000);
		if (ret == WAIT_TIMEOUT){
			TRACE("\n  !!! TIMEOUT\n");
		}

		// Attributes can be set only if bitmap was completely loaded in DRAM
		// or after last packet with bitmap data has been released by the driver.
		//

		//printf("Set OSD Destination: x - %X, y - %X, w - %X, h - %X\n", 
		//	rc_dest.X, rc_dest.Y, rc_dest.cX, rc_dest.cY);

		try{
			pDevice->SetAttributes(MpegAttrOsdDest,(long)&rc_dest);
			pDevice->SetAttributes(MpegAttrOsdON,1);		
			CloseHandle(ovr.hEvent);
			
		}catch (CException e){
			LPTSTR err;
			UINT maxerr=10;
			e.GetErrorMessage(err,maxerr);
			TRACE(err);
		}

	}
	fclose(f);
	TRACE("showScreen end successfully!");
	return 1;

	/////////////////////////////////////////////////////////////////////////////////////


exit_20:
	TRACE("\nERROR exit_20 ");
	fclose(f);
	if (NULL != pDevice)
		pDevice->Release();
exit_10:
	TRACE("\nERROR exit_10");
	#ifndef USE_RMBASE_CLSID
		if (NULL != pshared)
			free(pshared);
	#else
		if (NULL != pMemAlloc){
			if (NULL != pshared)
				pMemAlloc->FreeDMABuffer(pshared);
			pMemAlloc->Release();
		}
	#endif
	pOSD->Release();
exit_1:
	CoUninitialize();
	TRACE("showScreen ERROR Return");
	return 0;
}

int OSDDll::Close()
{
	
	pOSD->Reset();

	//printf("Press any key\n");
	//_kbhit();
  	//_getch();

	if (NULL != pDevice){
//		pDevice->SetAttributes(MpegAttrVideoTv,dwTvOutKeep);
	}
	pDevice->SetAttributes(MpegAttrOsdOFF,1);
	//pDevice->SetAttributes(MpegAttrOsdFLUSH,0);

	return 0;
	
}

int OSDDll::InitOSD(int top,int left)
{
	this->INNER_LEFT=left;
	this->INNER_RIGHT=top;
#ifdef MOCK
	return 1;
#endif

CoInitialize(NULL);

#ifndef USE_RMBASE_CLSID

	// use first avaliable device
	//
	hr = CoCreateInstance(CLSID_rm8400,NULL,CLSCTX_INPROC_SERVER,IID_IRmOSD,(void**)&pOSD);
	if (FAILED(hr)) goto exit_1;
#else
	IRmObjectFinder *pIRmObjectFinder;
	hr = CoCreateInstance(CLSID_RMBASE,NULL,CLSCTX_INPROC_SERVER,IID_IRmObjectFinder,(void**)&pIRmObjectFinder);
	if (FAILED(hr)) goto exit_1;

	// GET OSD
	//
	hr = pIRmObjectFinder->FindObject(
					NULL,
					FIRST_AVALIABLE_INSTANCE,
					CATEGORY_PIN_OSD,
					IID_IRmStream,
					(void**)&pOSD);

	pIRmObjectFinder->Release();
	if (FAILED(hr))	goto exit_1;

#endif

  	/////////////////////////////////////////////////////////////////////////////////////
	// do something useful

	pOSD->Reset();

	long dwTvOutKeep;
	{
		pDevice = NULL;
#ifndef USE_RMBASE_CLSID
		hr = pOSD->QueryInterface(IID_IRmDevice,(void**)&pDevice);
#else
		hr = pOSD->QueryInterface(IID_IRmControlNode,(void**)&pDevice);
		if (FAILED(hr)) goto exit_1;
		hr = pOSD->QueryInterface(IID_IRmMapMem,(void**)&pMemAlloc);
#endif
		if (FAILED(hr)) goto exit_1;

		long HwLibVersion = 0, dwTvOut;

		// switch to TV
		//
		pDevice->GetAttributes(MpegAttrCodeVersion,&HwLibVersion);
		pDevice->GetAttributes(MpegAttrVideoTv,&dwTvOutKeep);
		dwTvOut = dwTvOutKeep &~OUTPUT_OFF;

		if (HwLibVersion < 9){
			dwTvOut |= SET_TV;
		}else{
			dwTvOut |= SET_TV | 0x80000000;
			dwTvOut &= ~SET_HDTV;
		}
		pDevice->SetAttributes(MpegAttrVideoTv,dwTvOut);
		
	}


exit_1:
	return 0;
	
}

void OSDDll::ExitOSD()
{
#ifdef MOCK
	return;
#endif
	try{
		if (NULL != pDevice)
			pDevice->Release();
	}catch(CException e){

	}	
	#ifndef USE_RMBASE_CLSID
		if (NULL != pshared)
			free(pshared);
	#else
		if (NULL != pMemAlloc){
			if (NULL != pshared)
				pMemAlloc->FreeDMABuffer(pshared);
			pMemAlloc->Release();
		}
	#endif


	if (pOSD!=NULL){
		pOSD->Release();
	}

	CoUninitialize();
	return;
	
}

void OSDDll::hideText()
{
#ifdef MOCK
	return;
#endif
	//pOSD->Stop();
	if (NULL != pDevice){
		//pDevice->SetAttributes(MpegAttrVideoTv,dwTvOutKeep);
		pDevice->SetAttributes(MpegAttrOsdOFF,1);
	}

	#ifndef USE_RMBASE_CLSID
		if (NULL != pshared)
			free(pshared);
	#else
		if (NULL != pMemAlloc){
			if (NULL != pshared)
				pMemAlloc->FreeDMABuffer(pshared);
			//pMemAlloc->Release();
		}
	#endif
	singleFlag=false;
		//pOSD->Reset();
		//pOSD->Pause();
		
}

int OSDDll::Flush()
{
#ifdef MOCK
	return S_OK;
#endif
	pOSD->Stop();

	if (NULL != pDevice){
		pDevice->SetAttributes(MpegAttrOsdFLUSH,1);
	}
	//	pDevice->SetAttributes(MpegAttrOsdFLUSH,0);
	pOSD->Reset();
	pOSD->BeginFlush();
	return 1;
	
}


/*
	int i;
    //保存到文件并创建位图结构
	CString filename="c:\\b.bmp";
    BITMAPFILEHEADER bmfh;
    ZeroMemory(&bmfh,sizeof(BITMAPFILEHEADER));
    *((char *)&bmfh.bfType) = 'B';
    *(((char *)&bmfh.bfType) + 1) = 'M';

	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+4*256;
    bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;

	tagPALETTEENTRY  bmiColors[256];
	
	for (i=0;i<256;i++){
		bmiColors[i].peBlue=i;
		bmiColors[i].peGreen=i*10;
		bmiColors[i].peRed=1*2;
	}
    
	TCHAR szBMPFileName[128];
    strcpy(szBMPFileName,filename);
    CFile file;
    if(file.Open(szBMPFileName,CFile::modeWrite | CFile::modeCreate))
    {
        file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
        file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));

		for (i=0;i<256;i++){
			file.Write(&(bmiColors[i]),sizeof(tagRGBQUAD));
		}
		
        file.Write(pBits,iBMPBytes);
        file.Close();
    }

    */

void OSDDll::showPicture(CString filename,int destx,int desty)
{
#ifdef MOCK
	return;
#endif
	
	TRACE("\nShow button file:"+filename+"\n");
	if (singleFlag==true) return ;
	singleFlag=true;
	
	FILE* f = fopen(filename,"rb");
	if (NULL == f) return ;
	int width=640;
	int heigth=480;
	int sizeImg=width*heigth;
	int sizeAll=sizeImg+1078;
	BYTE* pAllData = (BYTE*)malloc(sizeAll);
	BYTE* pImgData = (BYTE*)malloc(sizeImg);
	int count = fread(pAllData,sizeof(char),sizeAll,f);
	for (int i=0; i<sizeImg; i++){
		pImgData[i]=pAllData[1078+i];
	}
	showScreen(destx,desty,width,heigth,pImgData);
	free(pAllData);
	free(pImgData);
	fclose(f);
	singleFlag=false;
}
