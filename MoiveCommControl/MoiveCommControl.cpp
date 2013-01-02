// MoiveControl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "MoiveCommControl.h"
#include "memfile.h"
#include "../XCardOCX/ini.h"


#define DLLWORDPASS 710719
HRESULT SelectAndRender(CMemReader *pReader, IFilterGraph **ppFG);
HRESULT PlayFileWait(IFilterGraph *pFG);

#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

//#define REGISTER_FILTERGRAPH 0
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

IPin* splitterIn = NULL;
IPin* splitterAout = NULL;
IPin* splitterVout = NULL;
IPin* decodeVin = NULL;
//Prepare source filter
IPin* sourcepin=NULL;

IBaseFilter* pAudioChannelMerger;
IAudioTransformFrame *pAudioInterface;
IBaseFilter*  pDCDSPFilter;



#define WM_GRAPHNOTIFY	WM_USER+13
//#define MAX_PATH 400
#define HELPER_RELEASE(x) { if (x) x->Release(); x = NULL; }


IBaseFilter* g_splitter;
IBaseFilter* g_decoder;
IBaseFilter* g_source_render;
IBaseFilter* g_source_render_encryptor;

IBaseFilter* g_vedio_render;
IBaseFilter* g_mpeg_audio_decorder;
IBaseFilter* g_direct_sound_render;

IBaseFilter* aviSplitter=NULL;
IBaseFilter* layer3=NULL;
IBaseFilter* acm=NULL;

IMpegAudioDecoder* g_mpegAudioDecoder;

IDCDSPFilterInterface* pDCPFilterInterface;
IDCPitchScale* pDSPPitchScale;
IDCPitchShift* pDSPPitchShift;
IDCAmplify* pDSPAmplify;

boolean mpeg2Flag;

IFileSourceFilter *pifsf = NULL;

//IPin *pinspA,*pinspV,*pindecodeA,*pindecodeV,*pinmsspA,*pinmsspV;
CString splitterMe="splitter";
CString decoderMe="decoder";
CString msSplitterMe="msSplitterMe";
HRESULT   hr;
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
    return NULL;
}

IPin* CMoiveControlApp::EmunPin(IBaseFilter *pFilter, CString filterflag, PIN_DIRECTION PinDir)
{
	
    return NULL;  
}


IPin* CMoiveControlApp::FindPinByDirection(IBaseFilter *pFilter, PIN_DIRECTION PinDir)
{
	return NULL;
}

void CMoiveControlApp::GoOn()
{
	m_CommCommander.Pause();
}

void CMoiveControlApp::Pause()
{
	m_CommCommander.Pause();
}


void CMoiveControlApp::InitCard()
{
	TRACE("Serial Open Port\n");
	m_CommCommander.OpenComm("c:\\ktv\\test.ini");
//TCHAR* iniFilePathAndName

}

void CMoiveControlApp::MovieProcess()
{
	
}
long CMoiveControlApp::m_Volume = -1000000;
long CMoiveControlApp::m_Pitch = -1000000;
CCommCommander CMoiveControlApp::m_CommCommander;

int CMoiveControlApp::PlayFile(CString szFile,bool encode,int movieType,int mode)
{
	TRACE("Play comm movie:%s",szFile);
	if (!globalP){
		return -1;
	}

	for (int i=0; i<szFile.GetLength(); i++)
	{
		switch (szFile.GetAt(i))
		{
		case '1':
			m_CommCommander.One();
			break;
		case '2':
			m_CommCommander.Two();
			break;
		case '3':
			m_CommCommander.Three();
			break;
		case '4':
			m_CommCommander.Four();
			break;
		case '5':
			m_CommCommander.Five();
			break;
		case '6':
			m_CommCommander.Six();
			break;
		case '7':
			m_CommCommander.Seven();
			break;
		case '8':
			m_CommCommander.Eight();
			break;
		case '9':
			m_CommCommander.Nine();
			break;
		case '0':
			m_CommCommander.Zero();
			break;	
		}
		Sleep(100);
	}
	m_CommCommander.Play();
	return 0;
		
}


HRESULT CMoiveControlApp::EnumFilters (IGraphBuilder *pGraph, IBaseFilter *o_pFilter) //IFilterGraph
{

    return S_OK;
}

void CMoiveControlApp::Shutdown()
{
	m_CommCommander.Shutdown();
}
void CMoiveControlApp::ReleaseMovie()
{

	m_CommCommander.Cancel1();
	Sleep(100);
	m_CommCommander.Cancel1();
	Sleep(200);
}

void CMoiveControlApp::ReleaseMovie_2()
{
	m_CommCommander.Cancel2();
}
int CMoiveControlApp::commStatus = 1;
int CMoiveControlApp::getCommStatus()
{
	return commStatus;
}
int CMoiveControlApp::setCallbackObject(bool* flag)
{
	//0: cancel
	//1: playing
	//2: ideal
	TRACE("\n-----------Came in Event loop ");

	long evCode, param1, param2;
	HRESULT hr;	
	CString myStatus;

	m_CommCommander.Clear();
	while (!(*flag))
	{		
		myStatus = m_CommCommander.GetEvent(flag);
		TRACE("\nevent loop, evcode:%s\n",myStatus);

		int compareR = myStatus.CompareNoCase(SONG_BEGIN);
		if (compareR == 0)
		{
			commStatus = 1;
			return commStatus;
		}
		compareR = myStatus.CompareNoCase(SONG_CANCEL);
		if (compareR == 0)
		{
			commStatus = 0;
			return commStatus;
		}

		compareR = myStatus.CompareNoCase(SONG_IDEL);
		if (compareR == 0)
		{
			if (commStatus == 0)
				TRACE("stay in cancel");
			else
			{
				commStatus = 2;
				return commStatus;
			}
		}		
	
		if (*flag){
			TRACE("\n++++++++Event loop exit cased by flag is true");
			break;
		}
		
	}
	m_CommCommander.m_stoppingFlag = true;
	TRACE("\n+++++++++++++++++Exit setCallbackObject ");
	return -1;
	
}

bool CMoiveControlApp::getCurrentPlaying()
{
	return (pigb!=NULL);
}
void CMoiveControlApp::micDown()
{
	m_CommCommander.MicDown();
}
void CMoiveControlApp::micUp()
{
	m_CommCommander.MicUp();
}
void CMoiveControlApp::Service()
{
	m_CommCommander.Service();
}
bool CMoiveControlApp::OpenChannel(int channelid)
{
	m_CommCommander.SingerOff();
	return true;
}
void CMoiveControlApp::Applause()
{
	m_CommCommander.Applause();
}
void CMoiveControlApp::setVolume(long value)
{
	
		if(value>0)
			m_CommCommander.VolumneUp();
		else
			m_CommCommander.VolumneDown();
	
	m_Volume = value;
}

void CMoiveControlApp::setPitch(long pitchValue)
{
	
		if(pitchValue>0)
			m_CommCommander.TuneUp();
		else
			m_CommCommander.TuneDown();
	
	m_Pitch = pitchValue;
}

void CMoiveControlApp::Stop()
{
	TRACE("CMoiveControlApp::Stop \n");
	m_CommCommander.Cancel1();
}

AM_MEDIA_TYPE* CMoiveControlApp::EnumMediaType(IPin* thepin)
{
	
	return NULL;
	
}


void CMoiveControlApp::setTVOutput(int mode)
{
	m_CommCommander.m_stoppingFlag = true;

}

//get pin of iPinNum 
IPin* CMoiveControlApp::GetPin(IBaseFilter *filter, int iPinNum)
{
	return NULL;
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

}
HRESULT CMoiveControlApp::AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister)
{
	
    return 0;
}
IPin* CMoiveControlApp::FetchpPin(IBaseFilter *pFilter, PIN_DIRECTION PinDirection, CString mtype)
{
	
    return NULL;
}
