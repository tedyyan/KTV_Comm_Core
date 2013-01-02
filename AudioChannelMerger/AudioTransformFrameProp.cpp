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
#include "AudioChannelMerger.h"
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
    m_pIZQAudioInterface(NULL),
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
    ASSERT(m_pIZQAudioInterface == NULL);

    HRESULT hr = pUnknown->QueryInterface(IID_IZQAudioInterface, (void **) &m_pIZQAudioInterface);
    if (FAILED(hr)) {
        return E_NOINTERFACE;
    }

    ASSERT(m_pIZQAudioInterface);
	int out=0;
    m_pIZQAudioInterface->get_AudioMode(&out);
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
    if (m_pIZQAudioInterface == NULL) {
        return E_UNEXPECTED;
    }

    m_pIZQAudioInterface->Release();
    m_pIZQAudioInterface = NULL;
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
	int out=0;
    m_pIZQAudioInterface->get_AudioMode(&out);
    _stprintf(sz, TEXT("%d"), out);
    Edit_SetText(GetDlgItem(m_Dlg, IDC_PARAM1), sz);


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
    ASSERT(m_pIZQAudioInterface);
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
    

    return NOERROR;
}

//
// GetControlValues
//
void CAudioTransformFrameProperties::GetControlValues()
{
    TCHAR sz[STR_MAX_LENGTH];

    Edit_GetText(GetDlgItem(m_Dlg, IDC_PARAM1), sz, STR_MAX_LENGTH);
	int in=_ttoi(sz);

	m_pIZQAudioInterface->put_AudioMode(in); 

    Edit_GetText(GetDlgItem(m_Dlg, IDC_PARAM2), sz, STR_MAX_LENGTH);
	//m_AudioTransformFrameParameters.param2 = _ttoi(sz);
}
