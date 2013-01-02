//ZQAudio.cpp
//�����л�������һ��directshow fileter
// Copyright (c) 2004, ������ƣ���ǿ. All rights reserved.
// hndamofy@163.com
// �����Ҫ������Դ�룬�뱣���ò���

#include <windows.h>
#include <streams.h>
#include <initguid.h>
#if (1100 > _MSC_VER)
#include <olectlid.h>
#else
#include <olectl.h>
#endif

#include "resource.h"
#include <assert.h>
#include <tchar.h>
#include <stdio.h>

#include "AudioChannelMerger.h"    // �Զ���ӿ�
#include "AudioTransformFrameProp.h"

#define TRANSFORM_NAME L"AudioTransformFrame Filter"

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{ &MEDIATYPE_Audio        // clsMajorType
, &MEDIASUBTYPE_PCM };    // clsMinorType

const AMOVIESETUP_PIN psudPins[] =
{ 
	{ L"Input"            // strName
	, FALSE               // bRendered
	, FALSE               // bOutput
	, FALSE               // bZero
	, FALSE               // bMany
	, &CLSID_NULL         // clsConnectsToFilter
	, L""                 // strConnectsToPin
	, 1                   // nTypes
	, &sudPinTypes        // lpTypes
	}
	, { L"Output"           // strName
	, FALSE               // bRendered
	, TRUE                // bOutput
	, FALSE               // bZero
	, FALSE               // bMany
	, &CLSID_NULL         // clsConnectsToFilter
	, L""                 // strConnectsToPin
	, 1                   // nTypes
	, &sudPinTypes        // lpTypes
	}
};


const AMOVIESETUP_FILTER sudNullNull =
{ &CLSID_ZQAudio                  // clsID
	, L"zhang qiang Filter"                 // strName
	, MERIT_DO_NOT_USE                // dwMerit
	, 2                               // nPins
	, psudPins };                     // lpPin

// CZQAudio
//
class CZQAudio: 
       public CTransInPlaceFilter,
    public IZQAudioInterface
{

public:

	//COM ��������
    static CUnknown *WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	DECLARE_IUNKNOWN;

	STDMETHODIMP put_AudioMode(int inAudio_Channel_Mode);
	STDMETHODIMP get_AudioMode(int *outAudio_Channel_Mode);

private:

	int Audio_Channel_Mode;              //����ģʽ����
	//int s;
	CCritSec m_Mylock;                  //��������

    //���캯����ͬʱ���û���Ĺ��캯��
    CZQAudio(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
        : CTransInPlaceFilter (tszName, punk, CLSID_ZQAudio, phr)
    { Audio_Channel_Mode=0; }

	//��Ҫִ�к���
	HRESULT Transform(IMediaSample *pSample);
	//�����������
	HRESULT CheckInputType(const CMediaType *mtIn);                    
	//�����������������Ƿ�һ��
	//HRESULT CheckTransform(const CMediaType *mtIn,const CMediaType *mtOut){ return S_OK; };    
	//ȡ������
	HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);     

};

// Needed for the CreateInstance mechanism
CFactoryTemplate g_Templates[]=
    {   { L"zhang qiang Filter"
        , &CLSID_ZQAudio
        , CZQAudio::CreateInstance
        , NULL
        , &sudNullNull }
	  ,
		{ TRANSFORM_NAME L" Properties"
		, &CLSID_AudioTransformFramePropertyPage
		, CAudioTransformFrameProperties::CreateInstance }
    };
int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);


//
// CreateInstance
//
// ����һ������ʵ��
CUnknown * WINAPI CZQAudio::CreateInstance(LPUNKNOWN punk, HRESULT *phr) {

    CZQAudio *pNewObject = new CZQAudio(NAME("Zhang qiang audio filter"), punk, phr );
    if (pNewObject == NULL) {
        *phr = E_OUTOFMEMORY;
    }

    return pNewObject;
} // CreateInstance

//
// WriteProfileInt
//
// Writes an integer to the profile.
//
void WriteProfileInt(TCHAR *section, TCHAR *key, int i)
{
	TCHAR str[80];
	_stprintf(str, TEXT("%d"), i);
	WriteProfileString(section, key, str);
}

//
// NonDelegatingQueryInterface
//
// Reveals IAudioTransformFrame and ISpecifyPropertyPages
//
STDMETHODIMP CZQAudio::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);

    if (riid == IID_IZQAudioInterface) {
        return GetInterface((IZQAudioInterface *) this, ppv);
    } else if (riid == IID_ISpecifyPropertyPages) {
        return GetInterface((ISpecifyPropertyPages *) this, ppv);
    } else {
        return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
    }
}

HRESULT CZQAudio::Transform(IMediaSample *pSample)
{
	BYTE *pOutData;
	int i=0,w_pos=0,r_pos=0,SampleSize=0,n=0;

	//��������ѡ��
	switch(Audio_Channel_Mode)
	{
	case 1:r_pos=0;
		 w_pos=2;
		 break;             //������

	case 2:r_pos=2;
		 w_pos=0; 
		 break;             //������

	default:return NOERROR;           //������˫����ģʽ��
	}

	pSample->GetPointer(&pOutData);    //ȡ�û�����ָ��
	SampleSize=pSample->GetActualDataLength();     //ȡ����Ч���ݴ�С
	n=SampleSize/4;                    //ѭ������

	//ִ�������л����� 
	for(i=0;i<n;i++){ 
		memcpy(pOutData+w_pos,pOutData+r_pos,2); 
		w_pos+=4;
		r_pos+=4;
	}
	return NOERROR;
}

HRESULT CZQAudio::GetMediaType(int iPosition,CMediaType *pMediaType)
{
	return NOERROR;
}

HRESULT CZQAudio::CheckInputType(const CMediaType *mtIn)
{
	//����Ƿ���ֹͣ״̬�����Ƿ�����Ƶ��������
	if(IsStopped() && *mtIn->Type()==MEDIATYPE_Audio)
	{
		//����Ƿ���PCM��ʽ������
		if(*mtIn->Subtype()==MEDIASUBTYPE_PCM ||
		   *mtIn->Subtype()==MEDIASUBTYPE_WAVE)
		{
		   return S_OK;     //�ɹ�
		}
	}
	return E_INVALIDARG;     //��Ч����
}


//��������
STDMETHODIMP CZQAudio::put_AudioMode(int inAudio_Channel_Mode)
{
	//�жϲ����Ƿ�Ϸ�
	if(inAudio_Channel_Mode>=0 && inAudio_Channel_Mode<=2)
	{
		Audio_Channel_Mode=inAudio_Channel_Mode;
		return S_OK;
	}
	return E_INVALIDARG;       //������Ч
}

//ȡ������ģʽ����
STDMETHODIMP CZQAudio::get_AudioMode(int *outAudio_Channel_Mode)
{
	//ȡ������ģʽ����
	*outAudio_Channel_Mode=Audio_Channel_Mode;
	return S_OK;
}


/******************************ȫ�ֺ���******************************\
* ע�ἰ��ע�ắ��
\**************************************************************************/
STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2( TRUE );
}

STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2( FALSE );
}
//���� 4514 ����
#pragma warning( disable:4514)

// If we declare the correct C runtime entrypoint and then forward it to the DShow base
// classes we will be sure that both the C/C++ runtimes and the base classes are initialized
// correctly
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpReserved)
{
   return DllEntryPoint(reinterpret_cast<HINSTANCE>(hDllHandle), dwReason, lpReserved);
}
