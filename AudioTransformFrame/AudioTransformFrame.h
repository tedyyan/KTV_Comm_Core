#pragma once

class CAudioTransformFrame : public CTransformFilter,
		 public IAudioTransformFrame,
		 public ISpecifyPropertyPages,
		 public CPersistStream
{

public:

    DECLARE_IUNKNOWN;
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

    // Reveals IAudioTransformFrame and ISpecifyPropertyPages
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

    // CPersistStream stuff
    HRESULT ScribbleToStream(IStream *pStream);
    HRESULT ReadFromStream(IStream *pStream);
    STDMETHODIMP GetClassID(CLSID *pClsid);

    // Overrriden from CTransformFilter base class
    HRESULT Transform(IMediaSample *pIn, IMediaSample *pOut);
	//HRESULT Transform(IMediaSample *pSample);

    HRESULT CheckInputType(const CMediaType *mtIn);
    HRESULT CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut);
    HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProperties);
    HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);

    // These implement the custom IAudioTransformFrame interface
    STDMETHODIMP get_AudioTransformFrame(AudioTransformFrameParameters *irp);
    STDMETHODIMP put_AudioTransformFrame(AudioTransformFrameParameters irp);

    // ISpecifyPropertyPages interface
    STDMETHODIMP GetPages(CAUUID *pPages);



private:

    // Constructor
    CAudioTransformFrame(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);
	~CAudioTransformFrame();

    BOOL CanPerformTransform(const CMediaType *pMediaType) const;

    CCritSec m_AudioTransformFrameLock;         // Private play critical section
	AudioTransformFrameParameters m_AudioTransformFrameParameters;
	CMediaType m_mt;

	HRESULT Copy(IMediaSample *pSource, IMediaSample *pDest) const;
	int m_Audio_Channel_Mode;  
	STDMETHODIMP put_AudioMode(int inAudio_Channel_Mode);
	STDMETHODIMP get_AudioMode(int *outAudio_Channel_Mode);

};

