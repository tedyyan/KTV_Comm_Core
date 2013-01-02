// MoiveControl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "MoiveControl.h"
#include "memfile.h"
#define DLLWORDPASS 710719
HRESULT SelectAndRender(CMemReader *pReader, IFilterGraph **ppFG);
HRESULT PlayFileWait(IFilterGraph *pFG);

#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

#define REGISTER_FILTERGRAPH 1
#define ENCODE_PWD "998211"
//#include "memfile.h"
#include "playfile.h"
#include "resource.h"


#include <windows.h>
#include <mmsystem.h>
#include <initguid.h>
#include "sdinterf.h"
#include "dsound.h"
#include "garguids.h"
#include "igargle.h"
#include "atlbase.h"
#include "dmodshow.h"
#include "DCDSPFilterInterfaces.h"
#include "Mmreg.h"
#include "mpegtype.h"
#include "iAudioTransformFrame.h"


// {04FE9017-F873-410e-871E-AB91661A4EF7}
DEFINE_GUID(CLSID_FFDSHOW         , 
0x04fe9017, 0xf873, 0x410e, 0x87, 0x1e, 0xab, 0x91, 0x66, 0x1a, 0x4e, 0xf7);
// {0F40E1E5-4F79-4988-B1A9-CC98794E6B55}
DEFINE_GUID(CLSID_FFDSHOW_AUDIO        , 
0x0F40E1E5, 0x4F79, 0x4988, 0xB1, 0xA9, 0xCC, 0x98, 0x79, 0x4E, 0x6B, 0x55);

// c553f2c0-1529-11d0-b4d1-00805f6cbbea
DEFINE_GUID(CLSID_AsyncSample_TEST,
0xc553f2c0, 0x1529, 0x11d0, 0xb4, 0xd1, 0x00, 0x80, 0x5f, 0x6c, 0xcc, 0xea);	
// c553f2c0-1529-11d0-b4d1-00805f6cbbea
//DEFINE_GUID(CLSID_AsyncSample,
//0xc553f2c0, 0x1529, 0x11d0, 0xb4, 0xd1, 0x00, 0x80, 0x5f, 0x6c, 0xbb, 0xea);

// Our media subtype guid
// 03b01b20-1534-11d0-b4d1-00805f6cbbea
DEFINE_GUID(MEDIASUBTYPE_SampleAsync,
0x03b01b20, 0x1534, 0x11d0, 0xb4, 0xd1, 0x00, 0x80, 0x5f, 0x6c, 0xbb, 0xea);


//{BD6FD780-B1DC-11D1-8BE4-00A0768008A8}
//Sigma Designs MPEG Stream Splitter
DEFINE_GUID(CLSID_RM_MPEG2_VIDEO_SPLITTER,
0xbd6fd780, 0xb1dC, 0x11d1, 0x8B, 0xE4, 0x00, 0xa0, 0x76, 0x80, 0x08, 0xa8);

//{4E3ABD41-458E-11D1-917E-00001B4F006F}
//Sigma Designs MPEG-2 hardware decoder
DEFINE_GUID(CLSID_RM_MPEG2_VIDEO_DECODER,
0x4E3ABD41, 0x458E, 0x11D1, 0x91, 0x7E, 0x00, 0x00, 0x1B, 0x4F, 0x00, 0x6F);


//{70E102B0-5556-11CE-97C0-00AA0055595A}
//Video Render
DEFINE_GUID(CLSID_RM_MPEG2_VIDEO_RENDER,
0x70E102B0, 0x5556, 0x11CE, 0x97, 0xC0, 0x00, 0xAA, 0x00, 0x55, 0x59, 0x5A);

//{E436EBB5-524F-11CE-9F53-0020AF0BA770}
//Video source
DEFINE_GUID(CLSID_RM_MPEG2_VIDEO_SOURCE,
0xE436EBB5, 0x524F, 0x11CE, 0x9F, 0x53, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70);

//  {3AE86B20-7BE8-11D1-ABE6-00A0C905F375}
DEFINE_GUID(CLSID_MPEG2Splitter,
0x3AE86B20, 0x7BE8, 0x11D1, 0xab, 0xe6, 0x00, 0xa0, 0xc9, 0x05, 0xf3, 0x75);

//-----------------------------------------------------------------------------
//{1236F6AF-1A72-4A62-9B2B-C5F7B62C6B1C}
//Sigma Designs PCM Swapper
DEFINE_GUID(CLSID_RM_PCM_SWAPPER,
0x1236F6AF, 0x1A72, 0x4A62, 0x9B, 0x2B, 0xC5, 0xF7, 0xB6, 0x2C, 0x6B, 0x1C);

//{6A08CF80-0E18-11CF-A24D-0020AFD79767}
//Microsoft ACM
DEFINE_GUID(CLSID_RM_ACM_WRAPPWE,
0x6A08CF80, 0x0E18, 0x11CF, 0xA2, 0x4D, 0x00, 0x20, 0xAF, 0xD7, 0x97, 0x67);

//{DFD5043E-B0C1-4406-BF29-3275573EAEEF}
//Sigma Designs AVI Splitter
DEFINE_GUID(CLSID_RM_AVI_SPLITTER,
0xDFD5043E, 0xB0C1, 0x4406, 0xBF, 0x29, 0x32, 0x75, 0x57, 0x3E, 0xAE, 0xEF);


//{38BE3000-DBF4-11D0-860E-00A024CFEF6D}
//MPEG Layer-3 Decoder
DEFINE_GUID(CLSID_MPEG_LAYER_3_DECODER,
0x38BE3000, 0xDBF4, 0x11D0, 0x86, 0x0E, 0x00, 0xA0, 0x24, 0xCF, 0xEF, 0x6D);


// -- MPEG audio decoder properties

// {4A2286E0-7BEF-11CE-9BD9-0000E202599C}
DEFINE_GUID(CLSID_MpegAudioDecoder,
0x4A2286E0, 0x7BEF, 0x11CE, 0x9B, 0xD9, 0x00, 0x00, 0xE2, 0x02, 0x59, 0x9C);

// A753A1EC-973E-4718-AF8E-A3F554D45C44
DEFINE_GUID(CLSID_AC3Filter,
0xA753A1EC, 0x973E, 0x4718, 0xAF, 0x8E, 0xA3, 0xF5, 0x54, 0xD4, 0x5C, 0x44);

// Collection of interfaces
	DWORD g_dwGraphRegister;
IBaseFilter   *pif   = NULL;
IGraphBuilder *pigb  = NULL;
IMediaControl *pimc  = NULL;
IMediaEventEx *pimex = NULL;
IVideoWindow  *pivw  = NULL;
IBasicVideo   *pibv   = NULL;
bool globalP;
IBasicAudio* piBAudio=NULL;
//IAudioOutput* piOAudio;
IMpegAudioDecoder* piOAudio;
IAMStreamSelect* channelAudioSelect;
IDirectSoundFXFlanger8* pidirectSoundFlanger;

IBaseFilter* pAudioChannelMerger;
IAudioTransformFrame *pAudioInterface;
		
		//				CComPtr<IBaseFilter> pAudioChannelMerger;
		//	CComQIPtr<IAudioTransformFrame, &IID_IAudioTransformFrame>	pAudioInterface(pAudioChannelMerger);
/*IBaseFilter* splitter;
IBaseFilter* decoder;
IBaseFilter* render;
IBaseFilter* source;
*/

#define WM_GRAPHNOTIFY	WM_USER+13
//#define MAX_PATH 400
#define HELPER_RELEASE(x) { if (x) x->Release(); x = NULL; }


IBaseFilter* g_splitter;
IBaseFilter* g_decoder;
IBaseFilter* g_source_render;
IBaseFilter* g_vedio_render;
IBaseFilter* g_mpeg_audio_decorder;
IBaseFilter* g_direct_sound_render;

IMpegAudioDecoder* g_mpegAudioDecoder;

IDCDSPFilterInterface* pDCPFilterInterface;
IDCPitchScale* pDSPPitchScale;
IDCPitchShift* pDSPPitchShift;
IDCAmplify* pDSPAmplify;

boolean mpeg2Flag;

IFileSourceFilter *pifsf = NULL;

IPin *pinspA,*pinspV,*pindecodeA,*pindecodeV,*pinmsspA,*pinmsspV;
CString splitterMe="splitter";
CString decoderMe="decoder";
CString msSplitterMe="msSplitterMe";
HRESULT   hr1,hr;
LONG      evCode;
LONG      evParam1;
LONG      evParam2;
IBaseFilter *pMPEG4SourceFilter = NULL;
bool release_flag=true;
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CMoiveControlApp

// An application can advertise the existence of its filter graph
// by registering the graph with a global Running Object Table (ROT).
// The GraphEdit application can detect and remotely view the running
// filter graph, allowing you to 'spy' on the graph with GraphEdit.
//
// To enable registration in this sample, define REGISTER_FILTERGRAPH.
//
//#define REGISTER_FILTERGRAPH


BEGIN_MESSAGE_MAP(CMoiveControlApp, CWinApp)
	//{{AFX_MSG_MAP(CMoiveControlApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoiveControlApp construction

CMoiveControlApp::CMoiveControlApp()
{
	globalP=false;
	g_dwGraphRegister=0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMoiveControlApp object

CMoiveControlApp theApp;

IPin* CMoiveControlApp::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir)
{
    BOOL       bFound = FALSE;
    IEnumPins  *pEnum;
    IPin       *pPin;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return NULL;
    }
    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;
        pPin->QueryDirection(&PinDirThis);
        if (bFound = (PinDir == PinDirThis))
            break;
        pPin->Release();
    }
    pEnum->Release();
    return (bFound ? pPin : NULL);  
}

IPin* CMoiveControlApp::EmunPin(IBaseFilter *pFilter, CString filterflag, PIN_DIRECTION PinDir)
{
    BOOL       bFound = FALSE;
    IEnumPins  *pEnum;
    IPin       *pPin;

	//IPin i_pPinV;
	//IPin i_pPinA;
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return NULL;
    }
	//TRACE("all count %d \n");
    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;
        pPin->QueryDirection(&PinDirThis);
		
		/*WCHAR nid1;
		LPWSTR a=&nid1;
        pPin->QueryId(&a);
		TRACE("\n pin id is %s ,oriental: %d\n",a,PinDirThis);
		WCHAR vWChar[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, "V", -1, vWChar, MAX_PATH);
		WCHAR aWChar[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, "A", -1, aWChar, MAX_PATH);*/
		LPWSTR a;
		pPin->QueryId(&a);
		CString type1;
		type1.Format("%s",a);

		if (bFound = (PinDir == PinDirThis)){
			if (type1=="V"){
				if (filterflag==splitterMe){
					pinspV=pPin;
				}
				if (filterflag==decoderMe){
					pindecodeV=pPin;
				}
				if (filterflag==msSplitterMe){
					pinmsspV=pPin;
				}
				TRACE("got the video pin");
			}else if(type1=="A"){
				if (filterflag==splitterMe){
					pinspA=pPin;
				}
				if (filterflag==decoderMe){
					pindecodeA=pPin;
				}
				if (filterflag==msSplitterMe){
					pinmsspA=pPin;
				}
				TRACE("got the audio pin");
			}else if(type1=="I"){
				if (filterflag==decoderMe){
					pindecodeV=pPin;
				}
			}
		}else{
			pPin->Release();
		}
		
		
    }
    pEnum->Release();
    return NULL;  
}


IPin* CMoiveControlApp::FindPinByDirection(IBaseFilter *pFilter, PIN_DIRECTION PinDir)
{
	BOOL       bFound = FALSE;
    IEnumPins  *pEnum;
    IPin       *pPin,*iterPin;

	//IPin i_pPinV;
	//IPin i_pPinA;
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return NULL;
    }
	//TRACE("all count %d \n");
    while(pEnum->Next(1, &iterPin, 0) == S_OK)
    {
		pPin=iterPin;
        PIN_DIRECTION PinDirThis;
        pPin->QueryDirection(&PinDirThis);	
		LPWSTR idme;
	
		pPin->QueryId(&idme);
		CString type1;
		type1.Format("%s",idme);
		if (bFound = (PinDir == PinDirThis)){
			break;
		}
		//pPin->Release();		
    }
    pEnum->Release();
    return (bFound ? pPin : NULL);  
}

void CMoiveControlApp::GoOn()
{
	TRACE("\n CMoiveControlApp::GoOn ");
	if (pimc!=NULL)
		pimc->Run();
}

void CMoiveControlApp::Pause()
{
	TRACE("\n CMoiveControlApp::Pause ");
	if (pimc!=NULL)
		pimc->Pause();

}

void CMoiveControlApp::InitCard()
{
	CoInitialize(NULL);
}

void CMoiveControlApp::MovieProcess()
{
	
}

int CMoiveControlApp::PlayFile(CString szFile,bool encode,int movieType,int mode)
{
	if (!globalP){
		return -1;
	}
#ifdef MOCK
	return 0;
#endif
	HELPER_RELEASE(g_splitter);
	HELPER_RELEASE(g_source_render);
	HELPER_RELEASE(g_vedio_render);

	piOAudio=NULL;
	//return -1 play next
	//return -2 hardware error
	try{

	WCHAR wFile[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, szFile, -1, wFile, MAX_PATH);


	hr1 = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
		IID_IGraphBuilder, (void **) &pigb);

	IBaseFilter *pFileSourceFilter = NULL;
	if (!SUCCEEDED(hr1))
	{ // Graphbuilder instance
		return -2;
	} 
	
	// QueryInterface for some basic interfaces
	pigb->QueryInterface(IID_IMediaControl, (void **) &pimc);
	pigb->QueryInterface(IID_IMediaEventEx, (void **) &pimex);
	pigb->QueryInterface(IID_IVideoWindow, (void **) &pivw);
	//IFileSourceFilter
	
	

	WCHAR sourceFilterName[MAX_PATH];
	
	MultiByteToWideChar(CP_ACP, 0, "Sample File Source (Async.)", -1, sourceFilterName, MAX_PATH);
	bool divxflag=false;
	//audio render--------------------------------------------------------------------------
		hr1 = CoCreateInstance(CLSID_AudioRender, NULL, CLSCTX_INPROC_SERVER, 
			IID_IBaseFilter, (void **) &g_direct_sound_render);
		LPCWSTR	str=L"Waveout Render";

		hr1 = g_direct_sound_render->QueryInterface(IID_IBasicAudio,(void **) &piBAudio);
		if (FAILED(hr1)){				
			TRACE("\nerror in query a pDCPFilterInterface \n");
			return -1;				
		}

		if (FAILED(hr1)){
			TRACE("error in creating a direct sound render \n");
		}
		pigb->AddFilter(g_direct_sound_render,str);

		IPin* directSoundInput=FindPinByDirection(g_direct_sound_render,PINDIR_INPUT);
		if (directSoundInput==NULL)
		{
			TRACE("error in finding input of direct sound render \n");
			return -1;
		}
		
	//pitch audio filter-------------------------------------------------------
		CComPtr<IBaseFilter> pDCDSPFilter;
		hr1 = CoCreateInstance(CLSID_DCDSPFilter,NULL,CLSCTX_INPROC,IID_IBaseFilter,(void**)&pDCDSPFilter);
		if (FAILED(hr1)){
			TRACE("\nerror create pDCDSPFilter! \n");	
			return -1;
		}
		pigb->AddFilter(pDCDSPFilter,L"audio gargle");

		BOOL enableDelay=true;
		//CComQIPtr<IDCPitchShift, &IID_DCDSPFilter>	pDSPPitchShift(pDCDSPFilter);	
				
		hr1 = pDCDSPFilter->QueryInterface(IID_DCDSPFilter,(void **) &pDCPFilterInterface);
		if (FAILED(hr1)){				
			TRACE("\nerror in query a pDCPFilterInterface \n");
			return -1;				
		}

		hr1 = pDCPFilterInterface->set_RemoveAllFilters();
		hr1 = pDCPFilterInterface->set_AddFilter(-1,ftPitchScale);
		if (FAILED(hr1)){				
			TRACE("\nerror in query a ftPitchScale \n");
			return -1;				
		}
		hr1 = pDCPFilterInterface->set_AddFilter(-1,ftPitchShift);
		if (FAILED(hr1)){				
			TRACE("\nerror in query a pDSPPitchShift \n");
			return -1;				
		}
		hr1 = pDCPFilterInterface->set_AddFilter(-1,ftAmplify);
		if (FAILED(hr1)){				
			TRACE("\nerror in query a ftAmplify \n");
			return -1;				
		}

		int filterCount=0;
		pDCPFilterInterface->get_FilterCount(&filterCount);

		hr1 = pDCPFilterInterface->get_FilterInterface(0,(void **) &pDSPPitchScale);
		if (FAILED(hr1)){				
			TRACE("\nerror in query a pDSPPitchScale \n");
			return -1;				
		}			
		hr1 = pDCPFilterInterface->get_FilterInterface(1,(void **) &pDSPPitchShift);
		if (FAILED(hr1)){				
			TRACE("\nerror in query a pDSPPitchShift \n");
			return -1;				
		}
		hr1 = pDCPFilterInterface->get_FilterInterface(2,(void **) &pDSPAmplify);
		if (FAILED(hr1)){				
			TRACE("\nerror in query a pDSPAmplify \n");
			return -1;				
		}
		

		IPin* pGargleInput=FindPinByDirection(pDCDSPFilter,PINDIR_INPUT);
		IPin* pGargleOutput=FindPinByDirection(pDCDSPFilter,PINDIR_OUTPUT);

		hr1 = CoCreateInstance(CLSID_AudioTransformFrame, NULL, CLSCTX_INPROC_SERVER, 
			IID_IBaseFilter, (void **) &pAudioChannelMerger);
		LPCWSTR	str1=L"pAudioChannel Merger ";
		hr1 =	pigb->AddFilter(pAudioChannelMerger,str1);
		if (FAILED(hr1)){				
			TRACE("\nerror add AudioTransformFrame into graph  \n");
			return -1;				
		}

		hr1 = pAudioChannelMerger->QueryInterface(IID_IAudioTransformFrame,(void **) &pAudioInterface);
		if (FAILED(hr1)){				
			TRACE("\nerror in query a IID_IAudioTransformFrame \n");
			return -1;				
		}

		//pAudioChannelMerger.CoCreateInstance(CLSID_AudioTransformFrame);

		//pAudioInterface->put_AudioMode(AM_AUDIO_DUAL_LEFT);

	
		IPin* pMergerInput=FindPinByDirection(pAudioChannelMerger,PINDIR_INPUT);
		IPin* pMergerOutput=FindPinByDirection(pAudioChannelMerger,PINDIR_OUTPUT);



	//-------------------------------------------------------------------------------

//Divx===================================================================================
	if ((movieType==2)||(movieType==3)){
		bool divxflag=true;
		
		if (pFileSourceFilter==NULL)
		{
			if (encode){
				hr1 = CoCreateInstance(CLSID_AsyncSample_TEST, NULL, CLSCTX_INPROC_SERVER,
				IID_IBaseFilter, (LPVOID *) &pFileSourceFilter);
			
			}else{
				hr = CoCreateInstance(CLSID_AsyncReader, NULL, CLSCTX_INPROC_SERVER,
				IID_IBaseFilter, (LPVOID *) &pFileSourceFilter);
			}
		}
	
		if (FAILED(pigb->AddFilter(pFileSourceFilter, L"MPEG4 Source")))
		{
			TRACE("Cannot add async source filter into Graph!!!");
			divxflag=false;
			return -2;
		}
		
		// Get File source interface
		
		if (divxflag){
			if (FAILED(pFileSourceFilter->QueryInterface(IID_IFileSourceFilter, (void **) &pifsf)))
			{
				TRACE("Cannot get IFileSourceFilter interface!!!");
				HELPER_RELEASE(pFileSourceFilter);
				divxflag=false;
			}
		}
		if (encode){
			//-----------------------------------------------------------------------------
			char * pszAlloc=ENCODE_PWD;
			LPOLESTR pwszCode;
			int CharCount = MultiByteToWideChar(CP_ACP,0,pszAlloc,-1,NULL,0);
			pwszCode = new WCHAR[CharCount];
			MultiByteToWideChar(CP_ACP,0,pszAlloc,-1,pwszCode,CharCount);
			TRACE("\n tranfer password:%s",pwszCode);
			pifsf->GetCurFile(&pwszCode,NULL);
			//delete pwszCode;
			//-------------------------------------------------------------------------------------
		}
		if (divxflag){
			if (FAILED(pifsf->Load(wFile, NULL)))
			{
				HELPER_RELEASE(pifsf);
				HELPER_RELEASE(pFileSourceFilter);
				TRACE("Cannot load source file!!!");
				divxflag=false;
			}
		}
		
		
		if (divxflag){
			if (g_decoder==NULL){
				hr = CoCreateInstance(CLSID_RM_MPEG2_VIDEO_SUBPICTURE, NULL, CLSCTX_INPROC_SERVER,
				IID_IBaseFilter, (LPVOID *) &g_decoder);
			}
			if (FAILED(pigb->AddFilter(g_decoder, L"MPEG4 Source")))
			{
				TRACE("Cannot async decoder filter in Graph!!!");
				HELPER_RELEASE(g_decoder);
				divxflag=false;
			}
			
			
		}
		
		//--------------------------------------------------------------------
		IBaseFilter* pcm=NULL;
		if (divxflag){
			if (pcm==NULL){
				hr = CoCreateInstance(CLSID_RM_PCM_SWAPPER, NULL, CLSCTX_INPROC_SERVER,
				IID_IBaseFilter, (LPVOID *) &pcm);
			}
			if (FAILED(pigb->AddFilter(pcm, L"MPEG4 PCM")))
			{
				TRACE("Cannot PCM filter in Graph!!!");
				HELPER_RELEASE(pcm);
				divxflag=false;
			}
		}
		

		IBaseFilter* aviSplitter=NULL;
		if (divxflag){
			if (aviSplitter==NULL){
				hr = CoCreateInstance(CLSID_RM_AVI_SPLITTER, NULL, CLSCTX_INPROC_SERVER,
				IID_IBaseFilter, (LPVOID *) &aviSplitter);
			}
			if (FAILED(pigb->AddFilter(aviSplitter, L"CLSID_RM_AVI_SPLITTER")))
			{
				TRACE("Cannot CLSID_RM_AVI_SPLITTER filter in Graph!!!");
				
				divxflag=false;
			}
		}



		IBaseFilter* layer3=NULL;
		if (divxflag){
			if (layer3==NULL){
				hr = CoCreateInstance(CLSID_MPEG_LAYER_3_DECODER, NULL, CLSCTX_INPROC_SERVER,
				IID_IBaseFilter, (LPVOID *) &layer3);
			}
			if (FAILED(pigb->AddFilter(layer3, L"CLSID_MPEG_LAYER_3_DECODER")))
			{
				TRACE("Cannot CLSID_MPEG_LAYER_3_DECODER filter in Graph!!!");
				HELPER_RELEASE(layer3);
				divxflag=false;
			}
		}
		
		

		IBaseFilter* acm=NULL;
		if (divxflag){
			if (acm==NULL){
				hr = CoCreateInstance(CLSID_RM_ACM_WRAPPWE, NULL, CLSCTX_INPROC_SERVER,
				IID_IBaseFilter, (LPVOID *) &acm);
			}
			if (FAILED(pigb->AddFilter(acm, L"CLSID_RM_ACM_WRAPPWE")))
			{
				TRACE("Cannot CLSID_RM_ACM_WRAPPWE filter in Graph!!!");
				HELPER_RELEASE(acm);
				divxflag=false;
			}
		}

		IPin* source2out=GetPin(pFileSourceFilter,PINDIR_OUTPUT);
		IPin* splitter2in=GetPin(aviSplitter,PINDIR_INPUT);
		hr=pigb->ConnectDirect(source2out,splitter2in,NULL);
	
		IPin* splitterAout=FetchpPin(aviSplitter, PINDIR_OUTPUT,"A");
		IPin* splitterVout=FetchpPin(aviSplitter, PINDIR_OUTPUT,"V");
		
		IPin* decodeAin=FetchpPin(g_decoder, PINDIR_INPUT,"A");
		IPin* decodeVin=FetchpPin(g_decoder, PINDIR_INPUT,"V");
		
		IPin* layer3in=GetPin(layer3, PINDIR_INPUT);
		hr=pigb->ConnectDirect(splitterAout,layer3in,NULL);
		hr=pigb->ConnectDirect(splitterVout,decodeVin,NULL);

		
		IPin* layer3out=GetPin(layer3, PINDIR_OUTPUT);
		
		IPin* pcm3in=GetPin(pcm, PINDIR_INPUT);
		IPin* pcm3out=GetPin(pcm, PINDIR_OUTPUT);
		IPin* acm3in;
		IPin* acm3out;
		//if (movieType==2){
			//acm3in=GetPin(acm, PINDIR_INPUT);
			//hr=pigb->ConnectDirect(layer3out,acm3in,NULL);			
			//acm3out=GetPin(acm, PINDIR_OUTPUT);
			//hr=pigb->ConnectDirect(acm3in,directSoundInput,NULL);
			//hr=pigb->ConnectDirect(acm3out,pcm3in,NULL);			
		//}else{
			//hr=pigb->ConnectDirect(layer3out,pcm3in,NULL);			
		//}
		//hr=pigb->ConnectDirect(layer3out,directSoundInput,NULL);
		hr=pigb->ConnectDirect(layer3out,pGargleInput,NULL);
		if (FAILED(hr)){
			TRACE("\nDirectly link mpeg1audioOutput with pGargle failed\n");
			return -1;
		}

		hr=pigb->ConnectDirect(pGargleOutput,pMergerInput,NULL);
		if (FAILED(hr)){
			TRACE("\nDirectly link pMergerInput with pGargle failed\n");
			return -1;
		}


		hr=pigb->ConnectDirect(pMergerOutput,directSoundInput,NULL);
		if (FAILED(hr)){
			TRACE("\nDirectly link pMergerInput with directSoundInput failed\n");
			return -1;
		}

		
			

		//if (movieType==2){
		//	HELPER_RELEASE(acm3in);
		//	HELPER_RELEASE(acm3out);
		//}
		HELPER_RELEASE(splitterVout);
		HELPER_RELEASE(decodeVin);
		HELPER_RELEASE(source2out);
		HELPER_RELEASE(splitter2in);
		HELPER_RELEASE(splitterAout);
		HELPER_RELEASE(layer3in);
		HELPER_RELEASE(layer3out);
		HELPER_RELEASE(pcm3in);
		HELPER_RELEASE(pcm3out);
		HELPER_RELEASE(decodeAin);

		HELPER_RELEASE(pcm);
		HELPER_RELEASE(layer3);
		HELPER_RELEASE(aviSplitter);
		HELPER_RELEASE(acm);
		//--------------------------------------------------------------------
		
		// Hide window until displayed at correct position
		if (divxflag){
			//volume
			//hr1 = g_decoder->QueryInterface(IID_IBasicAudio,(void **) &piBAudio);
			//channel
			//hr1 = g_decoder->QueryInterface(IID_IAudioOutput,(void **) &piOAudio);
			if (pivw != NULL)
			{
				pivw->put_AutoShow(OAFALSE);
				pivw->put_WindowStyle(WS_CHILD);
			}
			
			//++++++++
			//setTVOutput(mode);
			HELPER_RELEASE(g_decoder);
			HELPER_RELEASE(pFileSourceFilter);
			if (SUCCEEDED(hr1)){
				hr1=pimc->Run();
			}else{
				return -1;
			}
			if (!SUCCEEDED(hr1)){
				
				TRACE("\nError in create mpeg4 stream!!!");
				return -1;
			}
		#ifdef REGISTER_FILTERGRAPH
       		 hr = AddGraphToRot(pigb, &g_dwGraphRegister);
       		 if (FAILED(hr))
        		{
            			TRACE(TEXT("Failed to register filter graph with ROT!  hr=0x%x"), hr);
            			g_dwGraphRegister = 0;
        		}
		#endif
		
			return 0;
		}
		return -1;
		
	}//(movieType==2)||(movieType==3)
	//----------------------------------------------------------------------------------------------------


	

		if (encode){
			hr1 = CoCreateInstance(CLSID_AsyncSample_TEST, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter, (LPVOID *) &g_source_render);
			if (FAILED(hr1))
			{
				TRACE("\nerror in create myself g_source_render");
				return -2;
			}
			TRACE("\npEncodeSourceFilter");

			if (g_source_render==NULL){
				TRACE("\nerror in get file render");
				return -2;
			}
			
			CMediaType mt;
			mt.majortype = MEDIATYPE_Stream;
			mt.subtype = MEDIASUBTYPE_MPEG1System;
			
			IFileSourceFilter *pBase;
			g_source_render->QueryInterface(IID_IFileSourceFilter,(void**)&pBase);

			//-----------------------------------------------------------------------------
			char * pszAlloc=ENCODE_PWD;
			LPOLESTR pwszCode;
			int CharCount = MultiByteToWideChar(CP_ACP,0,pszAlloc,-1,NULL,0);
			pwszCode = new WCHAR[CharCount];
			MultiByteToWideChar(CP_ACP,0,pszAlloc,-1,pwszCode,CharCount);
			TRACE("\n tranfer password:%s",pwszCode);
			pBase->GetCurFile(&pwszCode,NULL);
			delete pwszCode;
			//-------------------------------------------------------------------------------------
			pBase->Load(wFile,NULL);
			HELPER_RELEASE(pBase);
			pigb->AddFilter(g_source_render,sourceFilterName);
		
		}else{
			hr1 = CoCreateInstance(CLSID_AsyncReader, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter, (LPVOID *) &g_source_render);
			if (FAILED(hr1))
			{
				TRACE("\nerror in create ms g_source_render");
				return -2;
			}
			TRACE("\npSourceFilter");

			IFileSourceFilter *pBase;
			g_source_render->QueryInterface(IID_IFileSourceFilter,(void**)&pBase);
			pBase->Load(wFile,NULL);
			HELPER_RELEASE(pBase);

			pigb->AddFilter(g_source_render,sourceFilterName);

		}
	

		IPin* sourcepin=GetPin(g_source_render,PINDIR_OUTPUT);
		if (sourcepin==NULL){
			return -1;
		}

		//++++++++++---CLSID_RM_MPEG2_VIDEO_SUBPICTURE----CLSID_CMpegVideoCodec--------------------
		if (g_decoder==NULL){
			hr1 = CoCreateInstance(CLSID_RM_MPEG2_VIDEO_SUBPICTURE, NULL, CLSCTX_INPROC_SERVER, 
				IID_IBaseFilter, (void **) &g_decoder);
		}
		if(g_decoder==NULL){
			TRACE("\nError in create decoder");
			HELPER_RELEASE(g_source_render);
			return -1;
		}

	
		str=L"decoder";
		pigb->AddFilter(g_decoder,str);

		//++++++++++-----CLSID_RM_MPEG2_VIDEO_SPLITTER-----CLSID_MPEG1Splitter---CLSID_RM_AVI_SPLITTER----CLSID_MPEG2Splitter----
		hr1 = CoCreateInstance(CLSID_MPEG2Splitter, NULL, CLSCTX_INPROC_SERVER, 
			IID_IBaseFilter, (void **) &g_splitter);
		if (FAILED(hr1)){
			TRACE("error in creating a mpeg2splitter \n");
		}

		//connect source filter to splitter			
		IPin* splitterIn=FindPinByDirection(g_splitter,PINDIR_INPUT);
		str=L"sigma splitter";
		pigb->AddFilter(g_splitter,str);
		HRESULT hrd=pigb->ConnectDirect(sourcepin,splitterIn,NULL);

		IPin* mpeg1audioInput=NULL;
		IPin* mpeg1audioOutput=NULL;

		if (FAILED(hrd)){
			TRACE("\nDirectly Link with source error\n");

			
			HELPER_RELEASE(splitterIn);

			hr1 = CoCreateInstance(CLSID_RM_MPEG2_VIDEO_SPLITTER, NULL, CLSCTX_INPROC_SERVER, 
			IID_IBaseFilter, (void **) &g_splitter);
			if (FAILED(hr1)){
				TRACE("error in creating a mpeg2splitter \n");
				return -2;
			}

			//connect source filter to splitter			
			IPin* splitterIn=FindPinByDirection(g_splitter,PINDIR_INPUT);
			str=L"sigma splitter";
			pigb->AddFilter(g_splitter,str);
			HRESULT hrd=pigb->ConnectDirect(sourcepin,splitterIn,NULL);
			if (FAILED(hrd)){
				TRACE("error in linking sourcepin and splitterIn \n");
				return -2;
			}

			//find all pin and set to the variable
			EmunPin(g_splitter,splitterMe,PINDIR_OUTPUT);
			EmunPin(g_decoder,decoderMe,PINDIR_INPUT);


						
			//---pitch ------------------------------------------------
			hr1 = CoCreateInstance(CLSID_MpegAudioDecoder, NULL, CLSCTX_INPROC_SERVER, 
			IID_IBaseFilter, (void **) &g_mpeg_audio_decorder);
			if (FAILED(hr1)){
				TRACE("error in creating a mpeg1 audio decorder \n");
				return -2;
			}
			str=L"Mpeg I Audio Decoder";
			pigb->AddFilter(g_mpeg_audio_decorder,str);

			mpeg1audioInput=FindPinByDirection(g_mpeg_audio_decorder,PINDIR_INPUT);
			if (mpeg1audioInput==NULL)
			{
				TRACE("error in finding input of mpeg1 audio decorder \n");
				return -2;
			}
			
			mpeg1audioOutput=FindPinByDirection(g_mpeg_audio_decorder,PINDIR_OUTPUT);
			if (mpeg1audioOutput==NULL)
			{
				TRACE("error in finding output of mpeg1 audio decorder \n");
				return -2;
			}
			hr1 = g_mpeg_audio_decorder->QueryInterface(IID_IMpegAudioDecoder,(void**)&piOAudio);
			if (FAILED(hr1)){				
				TRACE("\nerror in query a IID_IMpegAudioDecoder \n");
				return -2;
			}
			//Audio
			//piOAudio=NULL;
			//hr1 = g_splitter->QueryInterface(IID_IAMStreamSelect,(void **) &channelAudioSelect);
			//if (FAILED(hr1)){
			//	TRACE("\nerror in query a SIGMA IAMStreamSelect \n");
			//	return -1;
			//}
			hr1 = g_mpeg_audio_decorder->QueryInterface(IID_IMpegAudioDecoder,(void**)&piOAudio);
			if (FAILED(hr1)){				
				TRACE("\nerror in query a IID_IMpegAudioDecoder \n");
				return -2;
			}else
			{
				piOAudio->put_DualMode(AM_MPEG_AUDIO_DUAL_LEFT);
				TRACE("\ndirectly link IMpegAudioDecoder\n");
			}
						
			
			//connect splitter A/V out 
			hrd=pigb->ConnectDirect(pinspA,mpeg1audioInput,NULL);		
			if (FAILED(hrd)){
				TRACE("\nDirectly link with pindecodeA failed\n");
			}

			//split------------------------
			hrd=pigb->ConnectDirect(mpeg1audioOutput,pGargleInput,NULL);
			if (FAILED(hrd)){
				TRACE("\nDirectly link mpeg1audioOutput with pGargle failed\n");
				return -1;
			}

			hr=pigb->ConnectDirect(pGargleOutput,pMergerInput,NULL);
			if (FAILED(hr)){
				TRACE("\nDirectly link pMergerInput with pGargle failed\n");
				return -1;
			}


			hrd=pigb->ConnectDirect(pMergerOutput,directSoundInput,NULL);//pMergerOutput
			if (FAILED(hrd)){
				TRACE("\nDirectly link pGargle with directSoundInput failed\n");
				return -1;
			}
			//split------------------------

			hrd=pigb->ConnectDirect(pinspV,pindecodeV,NULL);
			if (FAILED(hrd)){
				TRACE("\nDirectly link with pindecodeV failed\n");
				return -1;
			}

		}else{

			HELPER_RELEASE(sourcepin);
			HELPER_RELEASE(splitterIn);

			//find all pin and set to the variable
			EmunPin(g_splitter,splitterMe,PINDIR_OUTPUT);
			EmunPin(g_decoder,decoderMe,PINDIR_INPUT);


			//-----CLSID_FFDSHOW_AUDIO-------------CLSID_AC3Filter----------------------------------------------
			hr1 = CoCreateInstance(CLSID_AC3Filter, NULL, CLSCTX_INPROC_SERVER, 
				IID_IBaseFilter, (void **) &g_mpeg_audio_decorder);
			if (FAILED(hr1)){
				TRACE("error in creating AC3 audio decorder \n");
				return -2;
			}
			str=L"AC3 Audio Decoder";
			pigb->AddFilter(g_mpeg_audio_decorder,str);
			
			
			/*
			hr1 = g_mpeg_audio_decorder->QueryInterface(IID_IMpegAudioDecoder,(void**)&piOAudio);
			if (FAILED(hr1)){				
				TRACE("\nerror in query a IID_IMpegAudioDecoder \n");

			}else
			{
				piOAudio->put_DualMode(AM_MPEG_AUDIO_DUAL_LEFT);
				TRACE("\ndirectly link CLSID_AC3Filter\n");
			}
			*/

			IPin* mpeg1audioInput=FindPinByDirection(g_mpeg_audio_decorder,PINDIR_INPUT);
			if (mpeg1audioInput==NULL)
			{
				TRACE("error in finding input of AC3 audio decorder \n");
				return -2;
			}
			
			IPin* mpeg1audioOutput=FindPinByDirection(g_mpeg_audio_decorder,PINDIR_OUTPUT);
			if (mpeg1audioOutput==NULL)
			{
				TRACE("error in finding output of AC3 audio decorder \n");
				return -2;
			}

			hrd=pigb->ConnectDirect(pinspA,mpeg1audioInput,NULL);
			if (FAILED(hrd)){
				TRACE("\nDirectly link with pinspA failed\n");
				return -1;
			}

			hrd=pigb->ConnectDirect(pinspV,pindecodeV,NULL);
			if (FAILED(hrd)){
				TRACE("\nDirectly link with pindecodeV failed\n");
				return -1;
			}

			
			hrd=pigb->ConnectDirect(mpeg1audioOutput,pGargleInput,NULL);
			if (FAILED(hrd)){
				TRACE("\nDirectly link mpeg1audioOutput with pGargle failed\n");
				return -1;
			}

			hr=pigb->ConnectDirect(pGargleOutput,pMergerInput,NULL);
			if (FAILED(hr)){
				TRACE("\nDirectly link pMergerInput with pGargle failed\n");
				return -1;
			}


			hrd=pigb->ConnectDirect(pMergerOutput,directSoundInput,NULL);//pMergerOutput
			if (FAILED(hrd)){
				TRACE("\nDirectly link pGargle with directSoundInput failed\n");
				return -1;
			}
			
			//split---------------------------------------------------------------

			/*--Audio
			piOAudio=NULL;
			hr1 = g_splitter->QueryInterface(IID_IAMStreamSelect,(void **) &channelAudioSelect);
			if (FAILED(hr1)){				
				TRACE("\nerror in query a SIGMA IAMStreamSelect \n");
				channelAudioSelect=NULL;				
			}else {
				DWORD count1=0;
				channelAudioSelect->Count(&count1);
				if (count1==1){
					HELPER_RELEASE(channelAudioSelect);					
				}
			}			
			TRACE("\ndirectly link \n");
			*/
		}//FAILED(hr)

		HELPER_RELEASE(pinspA);
		HELPER_RELEASE(pinspV);
		HELPER_RELEASE(pindecodeA);
		HELPER_RELEASE(pindecodeV);
		HELPER_RELEASE(pinmsspA);
		HELPER_RELEASE(pinmsspV);
		HELPER_RELEASE(g_splitter);	 
		
		//++++++++
		//setTVOutput(mode);
		HELPER_RELEASE(g_decoder);

//---------------------------------------------------------------------------------
//  Begin to show movie
//---------------------------------------------------------------------------------
	// Hide window until displayed at correct position
	if (pivw != NULL)
	{
		pivw->put_AutoShow(OAFALSE);
		pivw->put_WindowStyle(WS_CHILD);
		//pivw->put_Owner((OAHWND) ghApp);
	}
	if (SUCCEEDED(hr1))
		hr1=pimc->Run();

#ifdef REGISTER_FILTERGRAPH
        hr = AddGraphToRot(pigb, &g_dwGraphRegister);
        if (FAILED(hr))
        {
            TRACE(TEXT("Failed to register filter graph with ROT!  hr=0x%x"), hr);
            g_dwGraphRegister = 0;
        }
#endif
		HELPER_RELEASE(directSoundInput);
		HELPER_RELEASE(mpeg1audioInput);
		HELPER_RELEASE(mpeg1audioOutput);
		HELPER_RELEASE(g_mpeg_audio_decorder);
		HELPER_RELEASE(g_direct_sound_render);
	}catch (CException* e){			
		TRACE("\nERROR Play Movie of dll");
		LPTSTR err;
		UINT maxerr=10;
		e->GetErrorMessage(err,maxerr);
		TRACE(err);
		return -1;
	}
	
}


HRESULT CMoiveControlApp::EnumFilters (IGraphBuilder *pGraph, IBaseFilter *o_pFilter) //IFilterGraph
{
/*
    IEnumFilters *pEnum = NULL;
    IBaseFilter *pFilter;
    ULONG cFetched;

    HRESULT hr = pGraph->EnumFilters(&pEnum);
    if (FAILED(hr)) return hr;
    while(pEnum->Next(1, &pFilter, &cFetched) == S_OK)
    {
        FILTER_INFO FilterInfo;
        hr = pFilter->QueryFilterInfo(&FilterInfo);
        if (FAILED(hr))
        {
            MessageBox(NULL, TEXT("Could not get the filter info"),
                TEXT("Error"), MB_OK | MB_ICONERROR);
            continue;  // Maybe the next one will work.
        }
		
		CString filterName=FilterInfo.achName;
		
		if (filterName=="Sigma Designs MPEG-2 hardware decoder"){
			TRACE("\nWE GOT it Sigma Designs MPEG-2 hardware decoder:");
			TRACE("\n");
			pFilter->QueryInterface(IID_IBasicAudio,(void **) &piBAudio);
			pFilter->QueryInterface(IID_IAudioOutput,(void **) &piOAudio);
			if (piBAudio!=NULL){
				TRACE("piBAudio great!!!!!");
			}
			if (piOAudio!=NULL){
				TRACE("piOAudio great!!!!!");
			}
			g_decoder=pFilter;
		
		}else if(filterName=="Sigma Designs MPEG Stream Splitter"){
			g_splitter=pFilter;
		}else if(filterName=="MPEG-2 Splitter"){
			g_splitter=pFilter;
		}else if(filterName=="Video Renderer"){
			g_vedio_render=pFilter;
		}else {
			if (FindPinByDirection(pFilter,PINDIR_INPUT)==NULL){
				g_source_render=pFilter;
			}
		}
#ifdef UNICODE
		TRACE("UNICODE: %s\n\n",FilterInfo.achName);
#else
        char szName[MAX_FILTER_NAME];
        int cch = WideCharToMultiByte(CP_ACP, 0, FilterInfo.achName,
            MAX_FILTER_NAME, szName, MAX_FILTER_NAME, 0, 0);
        if (cch > 0)
			TRACE("NO UNICODE: %s\n\n",szName);
#endif


        // The FILTER_INFO structure holds a pointer to the Filter Graph
        // Manager, with a reference count that must be released.
        if (FilterInfo.pGraph != NULL)
        {
            FilterInfo.pGraph->Release();
        }
        pFilter->Release();
    }
    pEnum->Release();
	o_pFilter=NULL;*/
    return S_OK;
}


void CMoiveControlApp::ReleaseMovie()
{

	if(pivw)
    {
        pivw->put_Visible(OAFALSE);
        pivw->put_Owner(NULL);
    }

	if(pimc)
		pimc->Pause();
	
	if(pimc){
		pimc->Stop();
	}
	//pimex->CancelDefaultHandling();

	//HELPER_RELEASE(g_decoder);
	
	
	
	HELPER_RELEASE(g_source_render);
	HELPER_RELEASE(channelAudioSelect);
	HELPER_RELEASE(pibv);

	HELPER_RELEASE(piBAudio);
	HELPER_RELEASE(piOAudio);
	HELPER_RELEASE(pimc);
	HELPER_RELEASE(pimex);
	HELPER_RELEASE(pivw);
	HELPER_RELEASE(pif);
	HELPER_RELEASE(pigb);
	
	HELPER_RELEASE(pMPEG4SourceFilter);

	if ((pifsf!=NULL)){
		release_flag=false;
		/*IFileSourceFilter *pifsf = NULL;
	
		if (FAILED(pFileSourceFilter->QueryInterface(IID_IFileSourceFilter, (void **) &pifsf)))
		{
			TRACE("Cannot get IFileSourceFilter interface!!!");
			HELPER_RELEASE(pFileSourceFilter);
		}*/
		//-----------------------------------------------------------------------------
			char * pszAlloc="1";
			LPOLESTR pwszCode;
			int CharCount = MultiByteToWideChar(CP_ACP,0,pszAlloc,-1,NULL,0);
			pwszCode = new WCHAR[CharCount];
			MultiByteToWideChar(CP_ACP,0,pszAlloc,-1,pwszCode,CharCount);
			TRACE("\n tranfer password:%s",pwszCode);
			pifsf->GetCurFile(&pwszCode,NULL);
			HELPER_RELEASE(pifsf);
			//HELPER_RELEASE(pFileSourceFilter);
			//delete pwszCode;
		//-------------------------------------------------------------------------------------
	}
#ifdef REGISTER_FILTERGRAPH
    if (g_dwGraphRegister)
    {
        RemoveGraphFromRot(g_dwGraphRegister);
        g_dwGraphRegister = 0;
    }
#endif
}

void CMoiveControlApp::setCallbackObject(bool* flag)
{
	TRACE("\n-----------Came in Event loop ");
	if (!globalP){
		return ;
	}
#ifdef MOCK
	//loop for ever
	int i=0;
	while (!(*flag))
	{
		if (i<1000){
			Sleep(200);
		}
	}
	*flag=true;
	return;
#endif

	long evCode, param1, param2;
	HRESULT hr;	

	while (!(*flag))
	{
		if (pimex==NULL){
			TRACE("\n++++++++Event loop exit cased by pimex is NULL");
			break;
		}	
		hr = pimex->GetEvent(&evCode, &param1, &param2, -1);
		TRACE("\nevent loop, evcode:%d",evCode);
		if (!SUCCEEDED(hr)) {
			(*flag)=true;
			TRACE("\n++++++++Event loop exit cased by flag is true");
			return;
		}
		if (pimex==NULL){
			TRACE("\n++++++++Event loop exit cased by pimex is NULL");
			break;
		}
			
		
		switch(evCode) 
		{ 
			case EC_COMPLETE:
				TRACE("\n****************Song to End ");
				(*flag)=true;
				break;
			case EC_PAUSED:
				TRACE("\n****************Song to Pause ");
				break;
			case EC_ERRORABORT:
			case EC_SHUTTING_DOWN:
			case EC_WINDOW_DESTROYED:
			case EC_USERABORT:
				TRACE("\n****************Song to Stop ");
				(*flag)=true;
				break;
			
			default:
				TRACE("\n------Unhandle, evcode:%d",evCode);
				;
		} 
		if (pimex!=NULL){
			hr = pimex->FreeEventParams(evCode, param1, param2);
		}
		else{
			TRACE("\n++++++++Event loop exit cased by pimex is NULL");
			break;
		}
		if (*flag){
			TRACE("\n++++++++Event loop exit cased by flag is true");
			break;
		}
		
	}
	
	TRACE("\n+++++++++++++++++Song to End ");
	return;
	
}

bool CMoiveControlApp::getCurrentPlaying()
{
	return (pigb!=NULL);
}

bool CMoiveControlApp::OpenChannel(int channelid)
{
	//11-27-----------------------------------------------------------
	//if (mpeg2Flag)
	//{
		
	//}

	//end-----------------------------------------------------------
	//if this is mpeg 2 and MS splitter
	/*if (channelAudioSelect!=NULL){
		channelid--;
		DWORD count1=0;
		channelAudioSelect->Count(&count1);
		TRACE("\n channels:");
		piOAudio->SetChannel(STEREO);
		channelAudioSelect->Enable(channelid,AMSTREAMSELECTENABLE_ENABLE);

	}else if (piOAudio!=NULL){
	//if this is mpeg 1 
		AUDIO_CHANNEL channel=LEFT_ONLY;
		switch(channelid){
		case 0:
			channel=STEREO;
			break;
		case 1:
			channel=LEFT_ONLY;
			break;
		case 2:
			channel=RIGHT_ONLY;
			break;
		default:
			channel=STEREO;
		}
		piOAudio->SetChannel(channel);
	}*/
	if (piOAudio!=NULL)
	{
		switch(channelid){
		case 0:
			piOAudio->put_DualMode(AM_MPEG_AUDIO_DUAL_MERGE);
			break;
		case 1:
			piOAudio->put_DualMode(AM_MPEG_AUDIO_DUAL_LEFT);
			break;
		case 2:
			piOAudio->put_DualMode(AM_MPEG_AUDIO_DUAL_RIGHT);
			break;
		default:
			piOAudio->put_DualMode(AM_MPEG_AUDIO_DUAL_MERGE);
		}
	}else
	{
		AudioTransformFrameParameters para;
		para.param1=channelid;
		para.param2=0;
		pAudioInterface->put_AudioTransformFrame(para);
	}	
	return true;
}

void CMoiveControlApp::setVolume(long value)
{
	if (piBAudio!=NULL){
		//from -10,000 to 0
		//value=value-10000;
		if (value>0) value=0;
		if (value<-1000) value=-1000;
		piBAudio->put_Volume(value*10);
	}

	if (value>0) value=0;
	if (value<-10000) value=-10000;
	BOOL enableDelay=true;
	//pDSPAmplify->set_Enabled(enableDelay);
	//pDSPAmplify->set_Volume(0,-value*100);
}

void CMoiveControlApp::setPitch(long pitchValue)
{
	BOOL enableDelay=false;
	pDSPPitchScale->set_Enabled(enableDelay);
	enableDelay=true;
	pDSPPitchScale->set_Enabled(enableDelay);
	unsigned char aQuality=1;
	pDSPPitchScale->get_Quality(0,&aQuality);

			//DWORD pPitchVolumn=0;
			//pDSPPitchShift->get_Pitch(&pPitchVolumn);
			//pPitchVolumn=pPitchVolumn;
			//pDSPPitchShift->set_Pitch(pPitchVolumn);
	pDSPPitchScale->set_Pitch(0,pitchValue);
}

void CMoiveControlApp::Stop()
{
	TRACE("CMoiveControlApp::Stop \n");
	if (pimc!=NULL)
		pimc->Stop();
}

AM_MEDIA_TYPE* CMoiveControlApp::EnumMediaType(IPin* thepin)
{
	//BSTR a;
	//PIN_INFO pininfo;
	//thepin->QueryPinInfo(&pininfo);	
	//TRACE("\n pin name: ");
	//TRACE(pininfo.achName);
	//char * buf =  _com_util::ConvertBSTRToString(pininfo.achName);
	//TRACE(buf);
	//TRACE(reinterpret_cast<char *> pininfo.achName);

	IEnumMediaTypes* pEnumTypes;
	thepin->EnumMediaTypes(&pEnumTypes);
	AM_MEDIA_TYPE* mtype;
	while(pEnumTypes->Next(1,&mtype, 0) == S_OK)
    {
		//TRACE("\npins");
		//TRACE("\nMedia type:"+(mtype->majortype).Data1);
		/*if (mtype->majortype==MEDIATYPE_Stream){
			TRACE("\n MEDIATYPE_Stream");
		}
		if (mtype->majortype==MEDIATYPE_Video){
			TRACE("\n MEDIATYPE_Video");
		}
		if (mtype->majortype==MEDIATYPE_Audio){
			TRACE("\n MEDIATYPE_Audio");
		}
		if (mtype->majortype==MEDIATYPE_File){
			TRACE("\n MEDIATYPE_File");
		}*/
        if (mtype!=NULL){
			HELPER_RELEASE(pEnumTypes);
			return mtype;
        }
    }
	return NULL;
	
}


void CMoiveControlApp::setTVOutput(int mode)
{
	ITvVgaOutput* vga;
	if (g_decoder!=NULL){
		g_decoder->QueryInterface(IID_ITvVgaOutput,(void**)&vga);
		if (mode==2){
			//SET_TV
			vga->SetTvOutput(SET_TV|SET_NTSC);
		}else if (mode==1){
			vga->SetTvOutput(SET_TV|SET_PAL);
		}else{
			vga->SetTvOutput(SET_TV|SET_TV_AS_SOURCE);
		}
		vga->Release();
	}

}

//get pin of iPinNum 
IPin* CMoiveControlApp::GetPin(IBaseFilter *filter, int iPinNum)
{
#define MAX_FETCHED 10

	IEnumPins	*pEnumPins = NULL;
	IPin		*pRetPin = NULL;
	BOOL		bDone = FALSE;
	
	filter->EnumPins(&pEnumPins);
	if (pEnumPins)
	{
		IPin *Pins[MAX_FETCHED];
		ULONG uFetchedPins = 0;
		pEnumPins->Next(MAX_FETCHED, (IPin **) &Pins, &uFetchedPins);
		
		for (int i = 0; i < (int) uFetchedPins; i++)
		{
			if (iPinNum == i) 
				pRetPin = Pins[i];
			else
				Pins[i]->Release();
		}
		
		pEnumPins->Release();
	}
	return pRetPin;
}

int CMoiveControlApp::setP(long pp)
{
	if (pp==DLLWORDPASS){
		globalP=true;
	}else{
		globalP=false;
	}
	return 1;
}

void CMoiveControlApp::RemoveGraphFromRot(DWORD pdwRegister)
{
	IRunningObjectTable *pROT;
    if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) {
        pROT->Revoke(pdwRegister);
        pROT->Release();
    }
}

HRESULT CMoiveControlApp::AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister)
{
	IMoniker * pMoniker;
    IRunningObjectTable *pROT;
    if (FAILED(GetRunningObjectTable(0, &pROT))) {
        return E_FAIL;
    }
    WCHAR wsz[128];
    wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
    HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
    if (SUCCEEDED(hr)) {
        hr = pROT->Register(0, pUnkGraph, pMoniker, pdwRegister);
        pMoniker->Release();
    }
    pROT->Release();
    return hr;
}

IPin* CMoiveControlApp::FetchpPin(IBaseFilter *pFilter, PIN_DIRECTION PinDirection, CString mtype)
{
	BOOL       bFound = FALSE;
    IEnumPins  *pEnum;
    IPin       *pPin;


    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return NULL;
    }

    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;
        pPin->QueryDirection(&PinDirThis);
		
		
		LPWSTR a;
		pPin->QueryId(&a);
		CString type1;
		type1.Format("%s",a);

		if (bFound = (PinDirection == PinDirThis)){
			if (type1==mtype){
				pEnum->Release();
				return pPin;
			}else{
				pPin->Release();
			}
		}else{
			pPin->Release();
		}
		
		
    }
    pEnum->Release();
    return NULL;
}
