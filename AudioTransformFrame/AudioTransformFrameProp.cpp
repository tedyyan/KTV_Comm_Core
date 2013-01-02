#include <windows.h>
#include <windowsx.h>
#include <streams.h>
#include <commctrl.h>
#include <olectl.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include "resource.h"
#include "iAudioTransformFrame.h"
#include "AudioTransformFrame.h"
#include "AudioTransformFrameprop.h"

//
// CreateInstance
//
// Used by the DirectShow base classes to create instances
//
CUnknown *CAudioTransformFrameProperties::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
    CUnknown *punk = new CAudioTransformFrameProperties(lpunk, phr);
    if (punk == NULL) {
	*phr = E_OUTOFMEMORY;
    }
    return punk;

}

//
// Constructor
//
CAudioTransformFrameProperties::CAudioTransformFrameProperties(LPUNKNOWN pUnk, HRESULT *phr) :
    CBasePropertyPage(NAME("AudioTransformFrame Property Page"),
                      pUnk,IDD_AudioTransformFramePROP,IDS_TITLE),
    m_pIAudioTransformFrame(NULL),
    m_bIsInitialized(FALSE)
{
    ASSERT(phr);
}

//
// OnReceiveMessage
//
// Handles the messages for our property window
//
BOOL CAudioTransformFrameProperties::OnReceiveMessage(HWND hwnd,
                                          UINT uMsg,
                                          WPARAM wParam,
                                          LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_COMMAND:
        {
            if (m_bIsInitialized)
            {
                m_bDirty = TRUE;
                if (m_pPageSite)
                {
                    m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
                }
            }
            return (LRESULT) 1;
        }

    }
    return CBasePropertyPage::OnReceiveMessage(hwnd,uMsg,wParam,lParam);

}

//
// OnConnect
//
// Called when we connect to a transform filter
//
HRESULT CAudioTransformFrameProperties::OnConnect(IUnknown *pUnknown)
{
    ASSERT(m_pIAudioTransformFrame == NULL);

    HRESULT hr = pUnknown->QueryInterface(IID_IAudioTransformFrame, (void **) &m_pIAudioTransformFrame);
    if (FAILED(hr)) {
        return E_NOINTERFACE;
    }

    ASSERT(m_pIAudioTransformFrame);

    m_pIAudioTransformFrame->get_AudioTransformFrame(&m_AudioTransformFrameParameters);
    m_bIsInitialized = FALSE ;
    return NOERROR;
}

//
// OnDisconnect
//
// Likewise called when we disconnect from a filter
//
HRESULT CAudioTransformFrameProperties::OnDisconnect()
{
    if (m_pIAudioTransformFrame == NULL) {
        return E_UNEXPECTED;
    }

    m_pIAudioTransformFrame->Release();
    m_pIAudioTransformFrame = NULL;
    return NOERROR;
}

//
// OnActivate
//
// We are being activated
//
HRESULT CAudioTransformFrameProperties::OnActivate()
{
    TCHAR   sz[60];

    _stprintf(sz, TEXT("%d"), m_AudioTransformFrameParameters.param1);
    Edit_SetText(GetDlgItem(m_Dlg, IDC_PARAM1), sz);
    _stprintf(sz, TEXT("%d"), m_AudioTransformFrameParameters.param2);
    Edit_SetText(GetDlgItem(m_Dlg, IDC_PARAM2), sz);

	m_bIsInitialized = TRUE;

	return NOERROR;
}

//
// OnDeactivate
//
// We are being deactivated
//
HRESULT CAudioTransformFrameProperties::OnDeactivate(void)
{
    ASSERT(m_pIAudioTransformFrame);
    m_bIsInitialized = FALSE;
    GetControlValues();
    return NOERROR;
}

//
// OnApplyChanges
//
// Apply any changes so far made
//
HRESULT CAudioTransformFrameProperties::OnApplyChanges()
{
    GetControlValues();
    m_pIAudioTransformFrame->put_AudioTransformFrame(m_AudioTransformFrameParameters);

    return NOERROR;
}

//
// GetControlValues
//
void CAudioTransformFrameProperties::GetControlValues()
{
    TCHAR sz[STR_MAX_LENGTH];

    Edit_GetText(GetDlgItem(m_Dlg, IDC_PARAM1), sz, STR_MAX_LENGTH);
	m_AudioTransformFrameParameters.param1 = _ttoi(sz);

    Edit_GetText(GetDlgItem(m_Dlg, IDC_PARAM2), sz, STR_MAX_LENGTH);
	m_AudioTransformFrameParameters.param2 = _ttoi(sz);
}
