// OSDDll.cpp: implementation of the OSDDll class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "OSDDll.h"

#define INNER_LEFT 10
#define INNER_RIGHT 10

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
	
}

OSDDll::~OSDDll()
{
	//this->ExitOSD();
}



OSDDll::FormatAndDraw(CString text,	int iWidth,
	int iHeight, BYTE *pBits2, CString pcolor,int psize,CString pfont)
{
	
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


	BITMAPFILEHEADER   bf;
	BITMAPINFOHEADER   bi;
	HPALETTE           hPalette=NULL;
	HBITMAP            hBitmap=NULL;
	HGLOBAL            hImgData=NULL;

	DWORD              LineBytes;
	HINSTANCE          ghInst;
	DWORD              ImgWidth=0 , ImgHeight=0;
    
	HFILE              hf;
    LPBITMAPINFOHEADER lpImgData;
    LOGPALETTE         *pPal;
    LPRGBQUAD          lpRGB;
    HPALETTE           hPrevPalette; 
    HDC                hDc;
	HLOCAL             hPal;
	DWORD 		       ImgSize;
	DWORD              i;

    if((hf=_lopen(BmpFileName,OF_READ))==HFILE_ERROR){
        AfxMessageBox("BMP File not found!");
        return FALSE;
	}
	_lread(hf,(LPSTR)&bf,sizeof(BITMAPFILEHEADER)); 
	_lread(hf,(LPSTR)&bi,sizeof(BITMAPINFOHEADER));

	ImgWidth=bi.biWidth;
	ImgHeight=bi.biHeight;

	LineBytes=(DWORD)WIDTHBYTES(bi.biWidth*bi.biBitCount);
	ImgSize=(DWORD)LineBytes*bi.biHeight;
    if(bi.biClrUsed!=0)
		NumColors=(DWORD)bi.biClrUsed;
	else
        switch(bi.biBitCount){
	       	case 1:
        	    NumColors=2;
        	    break;
        	case 4:
        	    NumColors=16;
        	    break;
        	case 8:
        	    NumColors=256;
        	    break;
        	case 24:
        	    NumColors=0;
        	    break;
              default:
                  AfxMessageBox("Invalid color numbers!");
                  _lclose(hf);
                  return FALSE; 
        }
	if(bf.bfOffBits!=(DWORD)(NumColors*sizeof(RGBQUAD)+sizeof(BITMAPFILEHEADER)
							+sizeof(BITMAPINFOHEADER)))
	{
    	AfxMessageBox("Invalid color numbers!");
		_lclose(hf);
		return FALSE; 
	}
	bf.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+NumColors*sizeof(RGBQUAD)+ImgSize;
	if((hImgData=GlobalAlloc(GHND,(DWORD)(sizeof(BITMAPINFOHEADER)+
						     NumColors*sizeof(RGBQUAD)+ImgSize)))==NULL)
	{
    	AfxMessageBox("Error alloc memory!");
	    _lclose(hf);
		return FALSE;
	}
  
	lpImgData=(LPBITMAPINFOHEADER)GlobalLock(hImgData); 
    _llseek(hf,sizeof(BITMAPFILEHEADER),FILE_BEGIN);
	_hread(hf,(char *)lpImgData,(long)sizeof(BITMAPINFOHEADER)
           +(long)NumColors*sizeof(RGBQUAD)+ImgSize);
	_lclose(hf);
    if(NumColors!=0)
	{                    
	    hPal=LocalAlloc(LHND,sizeof(LOGPALETTE) + NumColors* sizeof(PALETTEENTRY));
	    pPal =(LOGPALETTE *)LocalLock(hPal);
	    pPal->palNumEntries =(WORD) NumColors;
		pPal->palVersion    = 0x300;
	    lpRGB = (LPRGBQUAD)((LPSTR)lpImgData + (DWORD)sizeof(BITMAPINFOHEADER));
		for (i = 0; i < NumColors; i++) {
     		pPal->palPalEntry[i].peRed=lpRGB->rgbRed;
			pPal->palPalEntry[i].peGreen=lpRGB->rgbGreen;
			pPal->palPalEntry[i].peBlue=lpRGB->rgbBlue;
			pPal->palPalEntry[i].peFlags=(BYTE)0;
			lpRGB++;
		}
		hPalette=CreatePalette(pPal);
		LocalUnlock(hPal);
		LocalFree(hPal);
	}

	GlobalUnlock(hImgData);

	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);

	
	
	//hBitmap=CreateDIBitmap(hDc,	(LPBITMAPINFOHEADER)lpImgData, (LONG)CBM_INIT,
	//			(LPSTR)lpImgData+sizeof(BITMAPINFOHEADER) +NumColors*sizeof(RGBQUAD),
   	//			(LPBITMAPINFO)lpImgData, DIB_RGB_COLORS);
	//if(hPalette && hPrevPalette){
	//	SelectPalette(hDc,hPrevPalette,FALSE);
	//	RealizePalette(hDc);
	//}
 
	//ReleaseDC(hWnd,hdc);
	
   
    hdcMem = CreateCompatibleDC(hdc);
    hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&pBits,NULL,0);
    pBitMap = new CBitmap;
    int kk=pBitMap->Attach(hBitMap);
    pMemDC = new CDC;
    int rr=pMemDC->Attach(hdcMem);
    class CBitmap * dd=pMemDC->SelectObject(pBitMap);

	//CPalette* myp=new CPalette();
	//myp->CreatePalette(pPal);
	//CPalette* oldPalette=pMemDC->SelectPalette(myp,TRUE);
	//RealizePalette(hdc);



	//hBitmap=CreateDIBitmap(hDc,	(LPBITMAPINFOHEADER)lpImgData, (LONG)CBM_INIT,
	//			(LPSTR)lpImgData+sizeof(BITMAPINFOHEADER) +NumColors*sizeof(RGBQUAD),
   	//			(LPBITMAPINFO)lpImgData, DIB_RGB_COLORS);
	//if(hPalette && hPrevPalette){
	//	SelectPalette(hDc,hPrevPalette,FALSE);
	//	RealizePalette(hDc);
	//}
 
	//ReleaseDC(hWnd,hDc);


	//CRect rc(0,0,iWidth,iHeight);
	//pMemDC->SetBkMode(TRANSPARENT);

    //添加自绘图形 
	DrawMemory(pMemDC,psize,text,1,1,1,pfont);

	
	if(hPalette){
        hPrevPalette=SelectPalette(hdc,hPalette,TRUE);
		//hPrevPalette=SelectPalette(hdc,hPalette,FALSE);
		RealizePalette(hdc);
	}

	if(hPalette && hPrevPalette ){
		TRACE("select palette is OK\n");
	}


	int iBMPBytes = iWidth * iHeight * iPixel / 8;


	for(int j=0;j<iHeight;j++)
		 for(int i=0;i<iWidth;i++)
	{
			 pBits2[i+j*iWidth]=pBits[(iHeight-j-1)*iWidth+i];
			 //change color
			 if (pBits2[i+j*iWidth]==0){
			 	pBits2[i+j*iWidth]=0x00;
			 }else{
				pBits2[i+j*iWidth]=atoi(pcolor);
			 }
	}
	


    pMemDC->DeleteDC();
    delete pMemDC;	pMemDC  = NULL;
    delete pBitMap; pBitMap = NULL;
    delete lpbmih;  lpbmih  = NULL;
	
}

bool OSDDll::DrawMemory(CDC *pDC, int fsize,CString text,long r,long g,long b,CString textFont)
{
	
    //页面背景色
	POINT point;
	//CBrush brushCtl;
	//brushCtl.CreateSolidBrush(RGB(0x20,0x40,0xE0));
	
  	//pDC->Rectangle(rcClient);
	//pDC->FillRect(rcClient,&brushCtl) ;
	//brushCtl.DeleteObject();


	//int heigth=rcClient.Height();
	//int width=rcClient.Width();

	CFont* m_Font=new CFont;

	m_Font->CreateFont(fsize,fsize,0,0,200,false,false,false,false,1,1,1,1,_T(textFont));
	CFont *pOldFond=pDC->SelectObject(m_Font);

	if (pOldFond==NULL){
		AfxMessageBox("456");
	}

	//int pOldBkMode=pDC->SetBkMode(this->m_BkMode);

	COLORREF m_BackgroundColor=RGB(0x40,0x40,0x40);
	COLORREF m_PenColor=RGB(0xA0,0xA0,0xA0);
	COLORREF oldBKColor=pDC->SetBkColor(m_BackgroundColor);
	COLORREF oldPenColor=pDC->SetTextColor(m_PenColor);

	
	if (oldBKColor!=NULL){
		TRACE("oldBKColor is ok \n");
	}
	if (oldPenColor!=NULL){
		TRACE("oldPenColor is ok \n");
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

	pDC->TextOut(INNER_LEFT,INNER_RIGHT,text);

	return 1;
}


BOOL OSDDll::initialDevice(CString text,int left,int top,CString pcolor,int psize,CString pfont)
{

	int width=500;
	
	int heigth=300;

	BYTE* p=(BYTE*)malloc(width*heigth);
	FormatAndDraw(text,width,heigth,p,pcolor,psize,pfont);

	showScreen(left,top,width,heigth,p);
	return true;
}

int OSDDll::showScreen(int destx, int desty,int width, int heigth, BYTE *pBits)
{

	/*destx=200;
	desty=30;
	width=200;
	heigth=30;*/
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

	
	f = fopen(OSDFileName,"rb");
	if (NULL == f) goto exit_10;

	// Read header
	//
	count = fread(buff_8,sizeof(char),8,f);
	if (count != 8) goto exit_20;

	// extract some bitmap information from header
	//
	
	size = size1+1024+8;


	//size = buff_8[1]<<8*2 | buff_8[2]<<8 | buff_8[3];
//	rc_dest.cX = buff_8[4] << 8 | buff_8[5];
	//rc_dest.cY = buff_8[6] << 8 | buff_8[7];


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
	hr = pMemAlloc->GetDMABuffer(size,(void**)&pshared,&phys);
	if (pshared == NULL){
		goto exit_20;
	}
#endif
	fread(pshared,sizeof(char),1024+8,f);

	

	//pshared2 = (PUCHAR)malloc(size1+1024+8);
	//temp=(PUCHAR)malloc(1024);
	//fread(temp,sizeof(char),1024,f);
	//pshared2[0]=0x3E;

	
	pshared[1]=size>>16;
	pshared[2]=(size-(size>>16)*65536)>>8;
	pshared[3]=(&size)[0];

	pshared[4]=width>>8;
	pshared[5]=(&width)[0];

	pshared[6]=heigth>>8;
	pshared[7]=(&heigth)[0];
	//for(i=8;i<1024+8;i++){
	//	pshared2[i]=pshared[i];
	//}

	for(i=0;i<size1;i++){
		pshared[1024+8+i]=pBits[i];
	}
	

	pOSD->Play();
	for(i=0;i<20;i++)
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
			printf("  !!! TIMEOUT\n");
		}

		// Attributes can be set only if bitmap was completely loaded in DRAM
		// or after last packet with bitmap data has been released by the driver.
		//

		//printf("Set OSD Destination: x - %X, y - %X, w - %X, h - %X\n", 
		//	rc_dest.X, rc_dest.Y, rc_dest.cX, rc_dest.cY);

		pDevice->SetAttributes(MpegAttrOsdDest,(long)&rc_dest);

//		rc_dest.X += 10;
//		rc_dest.Y += 10;

		pDevice->SetAttributes(MpegAttrOsdON,1);

		
		CloseHandle(ovr.hEvent);
///#endif
	}

	/*pOSD->Stop();

	printf("Press any key\n");
	_kbhit();
  	_getch();

	if (NULL != pDevice){
//		pDevice->SetAttributes(MpegAttrVideoTv,dwTvOutKeep);
	}
	pDevice->SetAttributes(MpegAttrOsdOFF,1);
//	pDevice->SetAttributes(MpegAttrOsdFLUSH,0);
*/
	return 1;

	/////////////////////////////////////////////////////////////////////////////////////
	// end

//exit(0);

	exit_20:
	fclose(f);
	if (NULL != pDevice)
		pDevice->Release();
	exit_10:
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
		//CoUninitialize();
	
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

int OSDDll::InitOSD()
{
	

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

	//CoUninitialize();
	return;
	
}

void OSDDll::hideText()
{
	
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
		//pOSD->Reset();
		//pOSD->Pause();
		
}

int OSDDll::Flush()
{
	
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
}
