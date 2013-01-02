
//=========================================================================
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
// Copyright (c) 1996 - 1998  Microsoft Corporation.  All Rights Reserved.
//
// Microsoft DirectShow sample modified:
// 1 - To support Hardware Overlay display (Analog or digital) by adding 
//     WS_CLIPCHILDREN style to the parent Window.
// 2 - To show an example of Frame capture into Windows Clipboard during
//     capture.
//=========================================================================

#include <windows.h>
#include <mmsystem.h>
#include <streams.h>
#include "playfile.h"
#include "resource.h"

#include <initguid.h>
#include "sdinterf.h"

//#define MPEG4_SUPPORT

#define APPLICATIONNAME "PlayFile"
#define CLASSNAME "PlayFile"

#define WM_GRAPHNOTIFY	WM_USER+13

#define HELPER_RELEASE(x) { if (x) x->Release(); x = NULL; }

HWND      ghApp;
HINSTANCE ghInst;
HMENU     ghMenu;

HRESULT   hr;
LONG      evCode;
LONG      evParam1;
LONG      evParam2;

// Collection of interfaces
IBaseFilter   *pif   = NULL;
IGraphBuilder *pigb  = NULL;
IMediaControl *pimc  = NULL;
IMediaEventEx *pimex = NULL;
IVideoWindow  *pivw  = NULL;
IBasicVideo   *pibv   = NULL;
IBaseFilter *pMPEG4SourceFilter = NULL;



void UpdateFilePos(HWND hWnd)
{
	if (pivw != NULL)
	{
		RECT rcWndPos;
		GetClientRect(hWnd, &rcWndPos);
		pivw->SetWindowPosition(rcWndPos.left, rcWndPos.top, 
			rcWndPos.right - rcWndPos.left, rcWndPos.bottom - rcWndPos.top);
	}
}

IPin *GetPin(IBaseFilter *filter, int iPinNum)
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

void PlayFile (LPSTR szFile)
{
	WCHAR wFile[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, szFile, -1, wFile, MAX_PATH);
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
		IID_IGraphBuilder, (void **) &pigb);

	if (SUCCEEDED(hr))
	{ // Graphbuilder instance

		// QueryInterface for some basic interfaces
		pigb->QueryInterface(IID_IMediaControl, (void **) &pimc);
		pigb->QueryInterface(IID_IMediaEventEx, (void **) &pimex);
		pigb->QueryInterface(IID_IVideoWindow, (void **) &pivw);

		// Have the graph construct its the appropriate graph automatically
#ifdef MPEG4_SUPPORT
		hr = CoCreateInstance(CLSID_SDMP4FileSource, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter, (LPVOID *) &pMPEG4SourceFilter);
		if (FAILED(hr))
		{
			MessageBox(ghApp, "Cannot create instance of the MPEG4 source filter!!!", "Error", MB_ICONSTOP);
			return;
		}

		if (FAILED(pigb->AddFilter(pMPEG4SourceFilter, L"MPEG4 Source")))
		{
			MessageBox(ghApp, "Cannot insert MPEG4 source filter in Graph!!!", "Error", MB_ICONSTOP);
			return;
		}

		// Get file source interface
		IFileSourceFilter *pifsf = NULL;
		if (FAILED(pMPEG4SourceFilter->QueryInterface(IID_IFileSourceFilter, (void **) &pifsf)))
		{
			MessageBox(ghApp, "Cannot get IFileSourceFilter interface!!!", "Error", MB_ICONSTOP);
			return;
		}
		
		if (FAILED(pifsf->Load(wFile, NULL)))
		{
			HELPER_RELEASE(pifsf);
			MessageBox(ghApp, "Cannot insert load source file!!!", "Error", MB_ICONSTOP);
			return;
		}
		HELPER_RELEASE(pifsf);

		IPin *pPin = GetPin(pMPEG4SourceFilter, 0);
		if (pPin == NULL)
		{
			MessageBox(ghApp, "Cannot find MPEG4 source pin 0!!!", "Error", MB_ICONSTOP);
			return;
		}
		if (FAILED(pigb->Render(pPin)))
		{
			MessageBox(ghApp, "Cannot render source pin 0!!!", "Error", MB_ICONSTOP);
			HELPER_RELEASE(pPin);
			return;
		}
		HELPER_RELEASE(pPin);
		
		pPin = GetPin(pMPEG4SourceFilter, 1);
		if (pPin == NULL)
		{
			MessageBox(ghApp, "Cannot find MPEG4 source pin 1!!!", "Error", MB_ICONSTOP);
			return;
		}
		if (FAILED(pigb->Render(pPin)))
		{
			MessageBox(ghApp, "Cannot render source pin 1!!!", "Error", MB_ICONSTOP);
			HELPER_RELEASE(pPin);
			return;
		}
		HELPER_RELEASE(pPin);
		
#else
		
		
		hr = pigb->RenderFile(wFile, NULL);
#endif

		// Have the graph signal event via window callbacks for performance
		pimex->SetNotifyWindow((OAHWND) ghApp, WM_GRAPHNOTIFY, 0);

		EnableMenuItem(ghMenu, ID_FILE_OPENCLIP, MF_DISABLED|MF_GRAYED);

		// Hide window until displayed at correct position
		if (pivw != NULL)
		{
			pivw->put_AutoShow(OAFALSE);
			pivw->put_WindowStyle(WS_CHILD);
			pivw->put_Owner((OAHWND) ghApp);
		}

		// Run the graph if RenderFile succeeded
		if (SUCCEEDED(hr))
			pimc->Run();

		// Fix position and then show
		if (pivw)
		{
			UpdateFilePos(ghApp);
			pivw->put_Visible(OATRUE);
		}
	} // Graphbuilder instance
} // PlayFile 

BOOL GetClipFileName(LPSTR szName)
{
	OPENFILENAME ofn;

	ofn.lStructSize       = sizeof(OPENFILENAME);
	ofn.hwndOwner         = ghApp;
	ofn.lpstrFilter       = NULL;
	ofn.lpstrFilter       = "Video files (*.mpg; *.mpeg; *.mp4)\0*.mpg; *.mpeg; *.mp4\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex      = 1;
	*szName = 0;
	ofn.lpstrFile         = szName;
	ofn.nMaxFile          = MAX_PATH;
	ofn.lpstrInitialDir   = NULL;
	ofn.lpstrTitle        = NULL;
	ofn.lpstrFileTitle    = NULL;
	ofn.lpstrDefExt       = "MPG";
	ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;

	return GetOpenFileName((LPOPENFILENAME)&ofn);

} // GetClipFileName

// Get first pin matching a direction on any filter. This is mostly usefull on
// renderers or transform filters which have only one input and/or output pin.
IPin *GetFirstPin(IBaseFilter *filter, PIN_DIRECTION PinDir)
{
	IEnumPins	*pEnumPins = NULL;
	IPin		*pRetPin = NULL;
	BOOL		bDone = FALSE;

	filter->EnumPins(&pEnumPins);
	if (pEnumPins)
	{
		ULONG uFetchedPins = 0;
		while ( (!bDone)
			 && (SUCCEEDED(pEnumPins->Next(1, &pRetPin, &uFetchedPins)))
		     && (uFetchedPins == 1) )
		{
			PIN_DIRECTION TempPinDir;
			HRESULT hResult = pRetPin->QueryDirection(&TempPinDir);
			if (hResult == S_OK)
			{
				if (TempPinDir == PinDir)
					bDone = TRUE;
			}
			if (!bDone)
				HELPER_RELEASE(pRetPin);
		}
		pEnumPins->Release();
	}
	return bDone ? pRetPin : NULL;
}

// This is a routine that can be used to retrieve IBasicVideo interface from
// Video Renderer or filter connected to Video Renderer when Video Renderer
// is only used as an Overlay surface.
HRESULT GetBasicVideoInterface(IGraphBuilder *pigb, IBasicVideo **pIBasicVideo)
{
	TCHAR szFilter[] = "Video Renderer";
	WCHAR wFile[MAX_PATH]; 
	IBaseFilter *m_pfVRender;

	*pIBasicVideo = NULL;

	MultiByteToWideChar(CP_ACP, 0, szFilter, -1, wFile, MAX_PATH);
	HRESULT hResult = pigb->FindFilterByName(wFile, &m_pfVRender);
	// If we fail here, let the graph manager handle this
	if(FAILED(hResult) || !m_pfVRender)
		return pigb->QueryInterface(IID_IBasicVideo, (void **) pIBasicVideo);

	// Get input pin of video renderer
	IPin *pInputPin = GetFirstPin(m_pfVRender, PINDIR_INPUT);
	HELPER_RELEASE(m_pfVRender);
	if (!pInputPin)
		return pigb->QueryInterface(IID_IBasicVideo, (void **) pIBasicVideo);
		
	// Check who is connected
	IPin *pConnectedPin = NULL;
	hResult = pInputPin->ConnectedTo(&pConnectedPin);
	if (FAILED(hResult) || !pConnectedPin)
	{
		HELPER_RELEASE(pInputPin);
		return pigb->QueryInterface(IID_IBasicVideo, (void **) pIBasicVideo);
	}
	HELPER_RELEASE(pInputPin);

	PIN_INFO InInfo;
	hResult = pConnectedPin->QueryPinInfo(&InInfo);
	if (FAILED(hResult) || !InInfo.pFilter)
	{
		HELPER_RELEASE(pConnectedPin);
		return pigb->QueryInterface(IID_IBasicVideo, (void **) pIBasicVideo);
	}
	HELPER_RELEASE(pConnectedPin);

	// Query interface on this filter
	if (FAILED(InInfo.pFilter->QueryInterface(IID_IBasicVideo, 
				(void **) pIBasicVideo)) || !pIBasicVideo)
	{
		// Finally if previous filter doesn't have this interface, we give up
		hResult = pigb->QueryInterface(IID_IBasicVideo, (void **) pIBasicVideo);

	}
	HELPER_RELEASE(InInfo.pFilter);
	return hResult;
}

LRESULT Draw(HDC hDC)
{
	RECT rcClient;
	GetClientRect(ghApp, &rcClient);
	HBRUSH hBrush = CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));	// For test only
	FillRect(hDC, &rcClient, hBrush);
	DeleteObject(hBrush);
	return 1L;
}

LRESULT CALLBACK WndMainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{

	case WM_COMMAND:

		switch(wParam)
		{ // Menus
		case ID_FILE_OPENCLIP:
			TCHAR szFilename[MAX_PATH];
			if (GetClipFileName(szFilename))
				PlayFile(szFilename);

			// Tests
			GetBasicVideoInterface(pigb, &pibv);
			break;

		case ID_FILE_EXIT:
			if (pimc)
				pimc->Stop();
			if (pivw)
				pivw->put_Visible(OAFALSE);

			HELPER_RELEASE(pMPEG4SourceFilter);
			HELPER_RELEASE(pibv);
			HELPER_RELEASE(pivw);
			HELPER_RELEASE(pif);
			HELPER_RELEASE(pigb);
			HELPER_RELEASE(pimc);
			HELPER_RELEASE(pimex);

			PostQuitMessage(0);
			break;

		case ID_COPY_CLIPBOARD:
			if (pibv)
			{
				OAFilterState fs;
				pimc->Pause();

				pimc->GetState(1000, &fs);

				// Call one time GetCurrentImage with NULL buffer to get frame size
				LONG lFrameSize = 0;
				if (FAILED(pibv->GetCurrentImage(&lFrameSize, NULL)))
				{
					pimc->Run();
					return 0;
				}
				
				HGLOBAL hBmpBuffer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,	lFrameSize);
				PLONG pBmpBuffer = (PLONG) GlobalLock(hBmpBuffer);
				if (pBmpBuffer != NULL)
				{
					HRESULT hResult = pibv->GetCurrentImage(&lFrameSize, pBmpBuffer);
					// Possible errors: E_OUTOFMEMORY, E_FAIL, VFW_E_NOT_PAUSED, E_UNEXPECTED
					if ( (SUCCEEDED(hResult))
					  && (OpenClipboard(hWnd)) )
					{	// Insert DIB in clipboard
						EmptyClipboard();
						SetClipboardData(CF_DIB, hBmpBuffer);
						CloseClipboard();
					}
					else
					{
						GlobalUnlock(hBmpBuffer);
						GlobalFree (hBmpBuffer); 
						pimc->Run();
						return 0;
					}
				}
				pimc->Run();
			}
			break;

		} // Menus

	break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		Draw(ps.hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_GRAPHNOTIFY:

		while (SUCCEEDED(pimex->GetEvent(&evCode, &evParam1, &evParam2, 0)))
		{ // Spin through the events
			hr = pimex->FreeEventParams(evCode, evParam1, evParam2);
			if (EC_COMPLETE == evCode) 
			{
				pimc->Stop();
				pimc->Run();
			}
			else
			if (EC_USERABORT == evCode)
			{ // Finished
				if (pivw != NULL)
					pivw->put_Visible(OAFALSE);

				HELPER_RELEASE(pMPEG4SourceFilter);
				HELPER_RELEASE(pibv);
				HELPER_RELEASE(pivw);
				HELPER_RELEASE(pif);
				HELPER_RELEASE(pigb);
				HELPER_RELEASE(pimc);
				HELPER_RELEASE(pimex);

				EnableMenuItem(ghMenu, ID_FILE_OPENCLIP, MF_ENABLED);
				break;
			} // Finished
		} // Spin through the events
		break;

	case WM_MOVE:
	case WM_SIZE:
		if (pivw != NULL)
			UpdateFilePos(hWnd);
		break;

	case WM_CLOSE:
		HELPER_RELEASE(pMPEG4SourceFilter);
		HELPER_RELEASE(pibv);
		HELPER_RELEASE(pivw);
		HELPER_RELEASE(pif);
		HELPER_RELEASE(pigb);
		HELPER_RELEASE(pimc);
		HELPER_RELEASE(pimex);
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	} // Window msgs handling

	return FALSE;
} // WndMainProc

int PASCAL WinMain(HINSTANCE hInstC, HINSTANCE hInstP, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wc;

	// OLE subsystem requires applications to initialize things first!
	CoInitialize(NULL);

	ZeroMemory(&wc, sizeof wc);
	wc.lpfnWndProc = WndMainProc;
	ghInst = wc.hInstance = hInstC;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = CLASSNAME;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstC, MAKEINTRESOURCE(IDI_PLAYFILE));
	RegisterClass(&wc);

	ghApp = CreateWindow(CLASSNAME,
		APPLICATIONNAME,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,	// WS_CLIPCHILDREN else Drawing
		CW_USEDEFAULT,							// client content will overwrite
		CW_USEDEFAULT,							// overlay window!!!!!
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		ghInst,
		0);

	ghMenu = GetMenu(ghApp);

	ShowWindow(ghApp, SW_NORMAL);
	UpdateWindow(ghApp);

	while (GetMessage(&msg,NULL,0,0))
	{ // Message loop
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	} // Message loop

	// Finished with OLE subsystem
	CoUninitialize();

	return msg.wParam;
} // WinMain
