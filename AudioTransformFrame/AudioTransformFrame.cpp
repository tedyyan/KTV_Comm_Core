#include <windows.h>
#include <streams.h>
#include <initguid.h>
#if (1100 > _MSC_VER)
#include <olectlid.h>
#else
#include <olectl.h>
#endif
#include "iAudioTransformFrame.h"
#include "AudioTransformFrameProp.h"
#include "AudioTransformFrame.h"
#include "resource.h"
#include <assert.h>
#include <tchar.h>
#include <stdio.h>

#define TRANSFORM_NAME L"AudioTransformFrame Filter"


// Setup information
const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_Audio,       // Major type
    &MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN sudpPins[] =
{
    { L"Input",             // Pins string name
      FALSE,                // Is it rendered
      FALSE,                // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    },
    { L"Output",            // Pins string name
      FALSE,                // Is it rendered
      TRUE,                 // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    }
};

const AMOVIESETUP_FILTER sudAudioTransformFrame =
{
    &CLSID_AudioTransformFrame,	// Filter CLSID
    TRANSFORM_NAME,				// String name
    MERIT_DO_NOT_USE,			// Filter merit
    2,							// Number of pins
    sudpPins					// Pin information
};

// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance

CFactoryTemplate g_Templates[] = {
    { TRANSFORM_NAME
    , &CLSID_AudioTransformFrame
    , CAudioTransformFrame::CreateInstance
    , NULL
    , &sudAudioTransformFrame }
  ,
    { TRANSFORM_NAME L" Properties"
    , &CLSID_AudioTransformFramePropertyPage
    , CAudioTransformFrameProperties::CreateInstance }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

//
// DllRegisterServer
//
// Handles sample registry and unregistry
//
STDAPI DllRegisterServer()
{
    return AMovieDllRegisterServer2( TRUE );
}

//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
    return AMovieDllRegisterServer2( FALSE );
}

//
// Constructor
//
CAudioTransformFrame::CAudioTransformFrame(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr) :
    CTransformFilter(tszName, punk, CLSID_AudioTransformFrame),
    CPersistStream(punk, phr)
{
	// TODO: read parameters from profile
	m_AudioTransformFrameParameters.param1 = GetProfileInt(TEXT("AudioTransformFrame"), TEXT("param1"), 0);
	m_AudioTransformFrameParameters.param2 = GetProfileInt(TEXT("AudioTransformFrame"), TEXT("param2"), 0);
} 

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
// ~CAudioTransformFrame
//
CAudioTransformFrame::~CAudioTransformFrame() 
{
	// TODO: write parameters from profile
	WriteProfileInt(TEXT("AudioTransformFrame"), TEXT("param1"), m_AudioTransformFrameParameters.param1);
	WriteProfileInt(TEXT("AudioTransformFrame"), TEXT("param2"), m_AudioTransformFrameParameters.param2);
}

//
// CreateInstance
//
// Provide the way for COM to create a AudioTransformFrame object
//
CUnknown *CAudioTransformFrame::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
    CAudioTransformFrame *pNewObject = new CAudioTransformFrame(NAME("AudioTransformFrame"), punk, phr);
    if (pNewObject == NULL) {
        *phr = E_OUTOFMEMORY;
    }
    return pNewObject;
}

//
// NonDelegatingQueryInterface
//
// Reveals IAudioTransformFrame and ISpecifyPropertyPages
//
STDMETHODIMP CAudioTransformFrame::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);

    if (riid == IID_IAudioTransformFrame) {
        return GetInterface((IAudioTransformFrame *) this, ppv);
    } else if (riid == IID_ISpecifyPropertyPages) {
        return GetInterface((ISpecifyPropertyPages *) this, ppv);
    } else {
        return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
    }
}

//
// Transform
//
// Transforms the input and saves results in the the output
//

HRESULT CAudioTransformFrame::Transform(IMediaSample *pIn, IMediaSample *pOut)
{

	HRESULT hr = S_OK;
	// input
    AM_MEDIA_TYPE* pTypeIn = &m_pInput->CurrentMediaType();
    WAVEFORMATEX *pihIn = (WAVEFORMATEX *)pTypeIn->pbFormat;
	unsigned char *pSrc = 0;
    pIn->GetPointer((unsigned char **)&pSrc);
	assert(pSrc);

	// output
    AM_MEDIA_TYPE *pTypeOut = &m_pOutput->CurrentMediaType();
	WAVEFORMATEX *pihOut = (WAVEFORMATEX *)pTypeOut->pbFormat;
	short *pDst = 0;
    pOut->GetPointer((unsigned char **)&pDst);
	assert(pDst);

	// TODO: insert procesing code here
	// for now, just make a copy of the input
    hr = Copy(pIn, pOut);
    if (hr != S_OK)
        return hr;
   
	return NOERROR;

}


//
// CheckInputType
//
// Check the input type is OK - return an error otherwise
//
HRESULT CAudioTransformFrame::CheckInputType(const CMediaType *mtIn)
{
    // check this is an audio format that we can support
    if (*mtIn->FormatType() != FORMAT_WaveFormatEx) {
        return E_INVALIDARG;
    }

    // Can we transform this type
    if (CanPerformTransform(mtIn)) {
		CopyMediaType(&m_mt, mtIn);
    	return NOERROR;
    }
    return E_FAIL;
}

//
// Checktransform
//
// Check a transform can be done between these formats
//
HRESULT CAudioTransformFrame::CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut)
{
    if (CanPerformTransform(mtIn)) {
		return S_OK;
    }
    return VFW_E_TYPE_NOT_ACCEPTED;
}

//
// DecideBufferSize
//
// Tell the output pin's allocator what size buffers we
// require. Can only do this when the input is connected
//
HRESULT CAudioTransformFrame::DecideBufferSize(IMemAllocator *pAlloc,ALLOCATOR_PROPERTIES *pProperties)
{
    // Is the input pin connected
    if (m_pInput->IsConnected() == FALSE) {
        return E_UNEXPECTED;
    }

    ASSERT(pAlloc);
    ASSERT(pProperties);
    HRESULT hr = NOERROR;

	// get input dimensions
	CMediaType inMediaType = m_pInput->CurrentMediaType();
	WAVEFORMATEX *pwfx = (WAVEFORMATEX *)m_mt.Format();
    pProperties->cBuffers = 1;
	int size = pwfx->nAvgBytesPerSec / 2;
    pProperties->cbBuffer = size; // same as input pin
    ASSERT(pProperties->cbBuffer);

    // Ask the allocator to reserve us some sample memory, NOTE the function
    // can succeed (that is return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if (FAILED(hr)) {
        return hr;
    }

    //ASSERT( Actual.cBuffers == 1 );

    if (pProperties->cBuffers > Actual.cBuffers ||
            pProperties->cbBuffer > Actual.cbBuffer) {
                return E_FAIL;
    }
    return NOERROR;
}

//
// GetMediaType
//
// Returns the supported media types for the output pin in order of preferred types 
// starting with iPosition=0
//
HRESULT CAudioTransformFrame::GetMediaType(int iPosition, CMediaType *pMediaType)
{
    // Is the input pin connected
    if (m_pInput->IsConnected() == FALSE)
        return E_UNEXPECTED;

    // This should never happen
    if (iPosition < 0)
        return E_INVALIDARG;

    // Do we have more items to offer
    if (iPosition >= 2)
        return VFW_S_NO_MORE_ITEMS;

	WAVEFORMATEX *pwfxin = (WAVEFORMATEX *)m_mt.pbFormat;

	if (iPosition == 0)
	{	// advertise the extensible format first
		WAVEFORMATEXTENSIBLE *pwfx = 
			(WAVEFORMATEXTENSIBLE *)pMediaType->AllocFormatBuffer(sizeof(WAVEFORMATEXTENSIBLE));

		pwfx->Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
		pwfx->Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
		pwfx->Format.nChannels = pwfxin->nChannels;
		pwfx->Format.nSamplesPerSec = pwfxin->nSamplesPerSec;
		pwfx->Format.wBitsPerSample = pwfxin->wBitsPerSample;
		pwfx->Format.nAvgBytesPerSec = pwfx->Format.nSamplesPerSec * pwfx->Format.wBitsPerSample * pwfx->Format.nChannels / 8;
		pwfx->Format.nBlockAlign = pwfx->Format.wBitsPerSample * pwfxin->nChannels / 8;
		pwfx->dwChannelMask = (1 << pwfx->Format.nChannels) - 1;
		pwfx->Samples.wValidBitsPerSample = pwfx->Format.wBitsPerSample;
		pwfx->SubFormat = MEDIASUBTYPE_PCM;

		pMediaType->SetFormat((BYTE*)pwfx, sizeof(WAVEFORMATEXTENSIBLE));

		// Clear source and target rectangles
		pMediaType->SetType(&MEDIATYPE_Audio);
		pMediaType->SetFormatType(&FORMAT_WaveFormatEx);
		pMediaType->SetTemporalCompression(FALSE);

		GUID SubTypeGUID = MEDIASUBTYPE_PCM;
		pMediaType->SetSubtype(&SubTypeGUID);
		pMediaType->SetSampleSize(1);
	}
	else if (iPosition == 1)
	{	// our backup legacy format
		WAVEFORMATEX *pwfx = 
			(WAVEFORMATEX *)pMediaType->AllocFormatBuffer(sizeof(WAVEFORMATEX));
		pwfx->wFormatTag = WAVE_FORMAT_PCM;
		pwfx->cbSize = 0; // no extra data for basic PCM
		pwfx->nChannels = pwfxin->nChannels;
		pwfx->nSamplesPerSec = pwfxin->nSamplesPerSec;
		pwfx->wBitsPerSample = pwfxin->wBitsPerSample;
		pwfx->nAvgBytesPerSec = pwfx->nSamplesPerSec * pwfx->wBitsPerSample * pwfx->nChannels / 8;
		pwfx->nBlockAlign = pwfx->wBitsPerSample * pwfxin->nChannels / 8;
		
		pMediaType->SetFormat((BYTE*)pwfx, sizeof(WAVEFORMATEX));

		pMediaType->SetType(&MEDIATYPE_Audio);
		pMediaType->SetFormatType(&FORMAT_WaveFormatEx);
		pMediaType->SetTemporalCompression(FALSE);

		GUID SubTypeGUID = MEDIASUBTYPE_PCM;
		pMediaType->SetSubtype(&SubTypeGUID);
		pMediaType->SetSampleSize(1);
	}

    return NOERROR;
}

//
// CanPerformAudioTransformFrame
//
// Check that it is audio and a PCM format.  Can get more specific here if needed (i.e. samplerate, channels etc)
//
BOOL CAudioTransformFrame::CanPerformTransform(const CMediaType *pMediaType) const
{
    if (IsEqualGUID(*pMediaType->Type(), MEDIATYPE_Audio)) {
		GUID SubTypeGUID = MEDIASUBTYPE_PCM;

        if (IsEqualGUID(*pMediaType->Subtype(), SubTypeGUID)) {
            WAVEFORMATEX *pwfx = (WAVEFORMATEX *) pMediaType->Format();
            return TRUE;
        }
    }
	return FALSE;
} 

#define WRITEOUT(var)  hr = pStream->Write(&var, sizeof(var), NULL); \
		       if (FAILED(hr)) return hr;

#define READIN(var)    hr = pStream->Read(&var, sizeof(var), NULL); \
		       if (FAILED(hr)) return hr;

//
// GetClassID
//
// This is the only method of IPersist
//
STDMETHODIMP CAudioTransformFrame::GetClassID(CLSID *pClsid)
{
    return CBaseFilter::GetClassID(pClsid);
}

//
// ScribbleToStream
//
// Overriden to write our state into a stream
//
HRESULT CAudioTransformFrame::ScribbleToStream(IStream *pStream)
{
	// TODO: write transform parameters to stream
    HRESULT hr;
    WRITEOUT(m_AudioTransformFrameParameters.param1);
    WRITEOUT(m_AudioTransformFrameParameters.param2);
    return NOERROR;

}

//
// ReadFromStream
//
// Likewise overriden to restore our state from a stream
//
HRESULT CAudioTransformFrame::ReadFromStream(IStream *pStream)
{
	// TODO: read transform parameters from stream
    HRESULT hr;
    READIN(m_AudioTransformFrameParameters.param1);
    READIN(m_AudioTransformFrameParameters.param2);
    return NOERROR;
}

//
// GetPages
//
// Returns the clsid's of the property pages we support
//
STDMETHODIMP CAudioTransformFrame::GetPages(CAUUID *pPages)
{
    pPages->cElems = 1;
    pPages->pElems = (GUID *) CoTaskMemAlloc(sizeof(GUID));
    if (pPages->pElems == NULL) {
        return E_OUTOFMEMORY;
    }
    *(pPages->pElems) = CLSID_AudioTransformFramePropertyPage;
    return NOERROR;
}

//
// get_AudioTransformFrame
//
// Copies the transform parameters to the given destination.
//
STDMETHODIMP CAudioTransformFrame::get_AudioTransformFrame(AudioTransformFrameParameters *irp)
{
    CAutoLock cAutolock(&m_AudioTransformFrameLock);
    CheckPointer(irp,E_POINTER);

	*irp = m_AudioTransformFrameParameters;

    return NOERROR;
}

//
// put_AudioTransformFrame
//
// Copies the transform parameters from the given source.
//
STDMETHODIMP CAudioTransformFrame::put_AudioTransformFrame(AudioTransformFrameParameters irp)
{
    CAutoLock cAutolock(&m_AudioTransformFrameLock);

	m_AudioTransformFrameParameters = irp;
    SetDirty(TRUE);


	this->put_AudioMode(irp.param1);
	// reconnect
	CMediaType &mt = m_pOutput->CurrentMediaType();
    WAVEFORMATEXTENSIBLE *pfmtOut = (WAVEFORMATEXTENSIBLE *)mt.pbFormat;
	if (!pfmtOut)
		return NOERROR;
	// TODO: modify pfmtOut if output type has changed
	HRESULT hr = ReconnectPin(m_pOutput, &mt);

    return NOERROR;
} 

//
// Copy
//
// Make destination an identical copy of source
//
/* */
HRESULT CAudioTransformFrame::Copy(IMediaSample *pSource, IMediaSample *pDest) const
{
    // Copy the sample data

    BYTE *pSourceBuffer, *pDestBuffer;
	BYTE *re_pSourceBuffer, *re_pDestBuffer;
    long lSourceSize = pSource->GetActualDataLength();
    long lDestSize	= pDest->GetSize();

    ASSERT(lDestSize >= lSourceSize);

    pSource->GetPointer(&pSourceBuffer);
    pDest->GetPointer(&pDestBuffer);

	BYTE *pOutData;
	int i=0,SampleSize=0,n=0;
	unsigned char w_pos=0,r_pos=0;

	BYTE * pBuffer;   
	HRESULT hres;   
	hres = pSource->GetPointer(&pBuffer);   
	int nDataLen = pSource->GetActualDataLength();   
	div_t div_result;   
	div_result = div( nDataLen,  4 );   
	if (div_result.rem!=0)   
	{   
		nDataLen=div_result.quot*4;   
	}   
	BYTE *pBuf = pBuffer;   
	for( int i = 0; i < nDataLen; i += 4 ) //这个只针对16位音频
	{   

		switch(m_Audio_Channel_Mode) //m_Channel==0代表切换左声道 m_Channel==1代表切换右声道
		{   
		case 1:   
			CopyMemory((PVOID)(pBuf+2),(const void*)pBuf,2);   
			break;   
		case 2:   
			CopyMemory((PVOID)pBuf,(const void*)(pBuf+2),2);   
			break;   
		case 0:   
			break;
		}   
		pBuf += 4;   
	}   
	CopyMemory(pDestBuffer,pBuffer,nDataLen);
    // Copy the sample times

    REFERENCE_TIME TimeStart, TimeEnd;
    if (NOERROR == pSource->GetTime(&TimeStart, &TimeEnd)) {
        pDest->SetTime(&TimeStart, &TimeEnd);
    }

    LONGLONG MediaStart, MediaEnd;
    if (pSource->GetMediaTime(&MediaStart,&MediaEnd) == NOERROR) {
        pDest->SetMediaTime(&MediaStart,&MediaEnd);
    }

    // Copy the Sync point property

    HRESULT hr = pSource->IsSyncPoint();
    if (hr == S_OK) {
        pDest->SetSyncPoint(TRUE);
    }
    else if (hr == S_FALSE) {
        pDest->SetSyncPoint(FALSE);
    }
    else {  // an unexpected error has occured...
        return E_UNEXPECTED;
    }

    // Copy the media type

    AM_MEDIA_TYPE *pMediaType;
    pSource->GetMediaType(&pMediaType);
    pDest->SetMediaType(pMediaType);
    DeleteMediaType(pMediaType);

    // Copy the preroll property

    hr = pSource->IsPreroll();
    if (hr == S_OK) {
        pDest->SetPreroll(TRUE);
    }
    else if (hr == S_FALSE) {
        pDest->SetPreroll(FALSE);
    }
    else {  // an unexpected error has occured...
        return E_UNEXPECTED;
    }

    // Copy the discontinuity property

    hr = pSource->IsDiscontinuity();
    if (hr == S_OK) {
	pDest->SetDiscontinuity(TRUE);
    }
    else if (hr == S_FALSE) {
        pDest->SetDiscontinuity(FALSE);
    }
    else {  // an unexpected error has occured...
        return E_UNEXPECTED;
    }

    // Copy the actual data length

    long lDataLength = pSource->GetActualDataLength();
    pDest->SetActualDataLength(lDataLength);

    return NOERROR;
}


// If we declare the correct C runtime entrypoint and then forward it to the DShow base
// classes we will be sure that both the C/C++ runtimes and the base classes are initialized
// correctly
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpReserved)
{
   return DllEntryPoint(reinterpret_cast<HINSTANCE>(hDllHandle), dwReason, lpReserved);
}

//设置声道
STDMETHODIMP CAudioTransformFrame::put_AudioMode(int inAudio_Channel_Mode)
{
	//判断参数是否合法
	if(inAudio_Channel_Mode>=0 && inAudio_Channel_Mode<=2)
	{
		m_Audio_Channel_Mode=inAudio_Channel_Mode;
		return S_OK;
	}
	return E_INVALIDARG;       //参数无效
}

//取得声道模式设置
STDMETHODIMP CAudioTransformFrame::get_AudioMode(int *outAudio_Channel_Mode)
{
	//取得声道模式设置
	*outAudio_Channel_Mode=m_Audio_Channel_Mode;
	return S_OK;
}


//
// Transform
//
// Transforms the input and saves results in the the output

/* 
HRESULT CAudioTransformFrame::Transform(IMediaSample *pSample)
{
	BYTE *pOutData;
	int i=0,w_pos=0,r_pos=0,SampleSize=0,n=0;

	//进行声道选择
	switch(m_Audio_Channel_Mode)
	{
	case 1:r_pos=0;
		 w_pos=2;
		 break;             //左声道

	case 2:r_pos=2;
		 w_pos=0; 
		 break;             //右声道

	default:return NOERROR;           //不处理（双声道模式）
	}

	pSample->GetPointer(&pOutData);    //取得缓冲区指针
	SampleSize=pSample->GetActualDataLength();     //取得有效数据大小
	n=SampleSize/4;                    //循环次数

	//执行声道切换操作 
	for(i=0;i<n;i++){ 
		memcpy(pOutData+w_pos,pOutData+r_pos,2); 
		w_pos+=4;
		r_pos+=4;
	}
	return NOERROR;
}
*/