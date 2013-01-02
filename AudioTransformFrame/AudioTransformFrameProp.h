#pragma once

class CAudioTransformFrameProperties : public CBasePropertyPage
{

public:

    static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

private:

    BOOL OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
    HRESULT OnConnect(IUnknown *pUnknown);
    HRESULT OnDisconnect();
    HRESULT OnActivate();
    HRESULT OnDeactivate();
    HRESULT OnApplyChanges();

    void    GetControlValues();

    CAudioTransformFrameProperties(LPUNKNOWN lpunk, HRESULT *phr);

    BOOL m_bIsInitialized;							// Used to ignore startup messages
    IAudioTransformFrame *m_pIAudioTransformFrame;      // The custom interface on the filter
	AudioTransformFrameParameters m_AudioTransformFrameParameters;
};

