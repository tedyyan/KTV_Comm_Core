// XCardOCXCtl.cpp : Implementation of the CXCardOCXCtrl ActiveX Control class.

#include "stdafx.h"
#include "XCardOCX.h"
#include "XCardOCXCtl.h"
#include "XCardOCXPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CXCardOCXCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CXCardOCXCtrl, COleControl)
	//{{AFX_MSG_MAP(CXCardOCXCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CXCardOCXCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CXCardOCXCtrl)
	DISP_FUNCTION(CXCardOCXCtrl, "initAll", initAll, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "volumeUp", volumeUp, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "songAhead", songAhead, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "songNext", songNext, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "songStop", songStop, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "songPause", songPause, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "songStart", songStart, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "volumeDown", volumeDown, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "serviceOn", serviceOn, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "serviceOff", serviceOff, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "singerOn", singerOn, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "singerOff", singerOff, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "songControl", songControl, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "exitCard", exitCard, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "showSongNum", showSongNum, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "showSongN", showSongN, VT_VARIANT, VTS_PBSTR)
	DISP_FUNCTION(CXCardOCXCtrl, "startZhangSheng", startZhangSheng, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "upAir", upAir, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "airDown", airDown, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "airUp", airUp, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "micUp", micUp, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "micDown", micDown, VT_VARIANT, VTS_NONE)
	DISP_FUNCTION(CXCardOCXCtrl, "setSongP", setSongP, VT_VARIANT, VTS_PBSTR)
	DISP_FUNCTION(CXCardOCXCtrl, "updateSLOSD", updateSLOSD, VT_VARIANT, VTS_PBSTR)
	DISP_STOCKPROP_READYSTATE()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CXCardOCXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CXCardOCXCtrl, COleControl)
	//{{AFX_EVENT_MAP(CXCardOCXCtrl)
	EVENT_CUSTOM("RefreshGUI", FireRefreshGUI, VTS_NONE)
	EVENT_STOCK_READYSTATECHANGE()
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CXCardOCXCtrl, 1)
	PROPPAGEID(CXCardOCXPropPage::guid)
END_PROPPAGEIDS(CXCardOCXCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CXCardOCXCtrl, "XCARDOCX.XCardOCXCtrl.1",
	0xca9cbe8b, 0x698e, 0x4cf9, 0xa0, 0xc2, 0x38, 0xd3, 0x89, 0xc2, 0x4b, 0x43)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CXCardOCXCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DXCardOCX =
		{ 0x42565790, 0x5c5, 0x40e4, { 0x9f, 0x1f, 0x81, 0xca, 0x90, 0xdc, 0xbe, 0 } };
const IID BASED_CODE IID_DXCardOCXEvents =
		{ 0xc512f5ea, 0xc53e, 0x4e04, { 0xba, 0x55, 0x6d, 0x64, 0x63, 0x93, 0x18, 0xbd } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwXCardOCXOleMisc =
	OLEMISC_SIMPLEFRAME |
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_IGNOREACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CXCardOCXCtrl, IDS_XCARDOCX, _dwXCardOCXOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXCtrl::CXCardOCXCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CXCardOCXCtrl

BOOL CXCardOCXCtrl::CXCardOCXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_XCARDOCX,
			IDB_XCARDOCX,
			afxRegInsertable | afxRegApartmentThreading,
			_dwXCardOCXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXCtrl::CXCardOCXCtrl - Constructor

CXCardOCXCtrl::CXCardOCXCtrl()
{
	InitializeIIDs(&IID_DXCardOCX, &IID_DXCardOCXEvents);

	EnableSimpleFrame();

	m_lReadyState = READYSTATE_LOADING;
	// TODO: Call InternalSetReadyState when the readystate changes.

	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXCtrl::~CXCardOCXCtrl - Destructor

CXCardOCXCtrl::~CXCardOCXCtrl()
{
	// TODO: Cleanup your control's instance data here.
	
}


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXCtrl::OnDraw - Drawing function

void CXCardOCXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);

	if (!IsOptimizedDraw())
	{
		// The container does not support optimized drawing.

		// TODO: if you selected any GDI objects into the device context *pdc,
		//		restore the previously-selected objects here.
		//		For more information, please see MFC technical note #nnn,
		//		"Optimizing an ActiveX Control".
	}
}


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXCtrl::DoPropExchange - Persistence support

void CXCardOCXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
// For information on using these flags, please see MFC technical note
// #nnn, "Optimizing an ActiveX Control".
DWORD CXCardOCXCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();


	// The control can activate without creating a window.
	// TODO: when writing the control's message handlers, avoid using
	//		the m_hWnd member variable without first checking that its
	//		value is non-NULL.
	dwFlags |= windowlessActivate;

	// The control can receive mouse notifications when inactive.
	// TODO: if you write handlers for WM_SETCURSOR and WM_MOUSEMOVE,
	//		avoid using the m_hWnd member variable without first
	//		checking that its value is non-NULL.
	dwFlags |= pointerInactive;

	// The control can optimize its OnDraw method, by not restoring
	// the original GDI objects in the device context.
	dwFlags |= canOptimizeDraw;
	return dwFlags;
}


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXCtrl::OnResetState - Reset control to default state

void CXCardOCXCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXCtrl::AboutBox - Display an "About" box to the user

void CXCardOCXCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_XCARDOCX);
	dlgAbout.DoModal();
	
}


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXCtrl message handlers

VARIANT CXCardOCXCtrl::initAll() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	main.initxCard();
	main.initKTV();
	return vaResult;
}

VARIANT CXCardOCXCtrl::volumeUp() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);

		main.volumeUp();
	
	return vaResult;
}


VARIANT CXCardOCXCtrl::songAhead() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);

	main.songAhead();
	return vaResult;
}

VARIANT CXCardOCXCtrl::songNext() 
{
	TRACE("----CXCardOCXCtrl::songNext-----\n");
	VARIANT vaResult;
	VariantInit(&vaResult);
	main.songNext();

	return vaResult;
}

VARIANT CXCardOCXCtrl::songStop() 
{
	TRACE("----CXCardOCXCtrl::songStop-----\n");
	VARIANT vaResult;
	VariantInit(&vaResult);

		main.songStop();
	
	return vaResult;
}

VARIANT CXCardOCXCtrl::songPause() 
{
	TRACE("----CXCardOCXCtrl::songPause-----\n");
	VARIANT vaResult;
	VariantInit(&vaResult);

		main.songPause();	

	return vaResult;
}

VARIANT CXCardOCXCtrl::songStart() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	main.songStart();

	return vaResult;
}

VARIANT CXCardOCXCtrl::volumeDown() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);

		main.volumeDown();

	return vaResult;
}

VARIANT CXCardOCXCtrl::serviceOn() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	main.serviceOn();

	return vaResult;
}

VARIANT CXCardOCXCtrl::serviceOff() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	
		main.serviceOff();
	
	
	return vaResult;
}

VARIANT CXCardOCXCtrl::singerOn() 
{

	VARIANT vaResult;
	VariantInit(&vaResult);

		main.singerOn();

	return vaResult;
}

VARIANT CXCardOCXCtrl::singerOff() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);

		main.singerOff();

	return vaResult;
}

VARIANT CXCardOCXCtrl::songControl() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	// TODO: Add your dispatch handler code here

	return vaResult;
}

VARIANT CXCardOCXCtrl::exitCard() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	main.exitMainProcess();

	return vaResult;
}

VARIANT CXCardOCXCtrl::showSongNum() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	//main.showSongID(vaResult);
	main.hideShowIDOSD();
	return vaResult;
}

VARIANT CXCardOCXCtrl::showSongN(BSTR FAR* num) 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	CString s(*num == NULL ? L"" : *num);
	TRACE("\n----CXCardOCXCtrl::showSongN-- %s",s);
	CString osdNum=_com_util::ConvertBSTRToString(*num); 
	main.showSongN(osdNum);
	return vaResult;
}

VARIANT CXCardOCXCtrl::startZhangSheng() 
{
	TRACE("----CXCardOCXCtrl::startZhangSheng--");
	VARIANT vaResult;
	VariantInit(&vaResult);
	
	HWND h;
	main.startZhangSheng(h);

	return vaResult;
}

VARIANT CXCardOCXCtrl::upAir() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	//main.airUp();
	CString s="1";
	main.showSongN(s);
	return vaResult;
}

VARIANT CXCardOCXCtrl::airDown() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	main.airDown();
	return vaResult;
}

VARIANT CXCardOCXCtrl::airUp() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	main.airUp();
	
	return vaResult;
}

VARIANT CXCardOCXCtrl::micUp() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	main.micUp();
	return vaResult;
}

VARIANT CXCardOCXCtrl::micDown() 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	main.micDown();
	return vaResult;
}

VARIANT CXCardOCXCtrl::setSongP(BSTR FAR* mypd) 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	
	CString osdNum=_com_util::ConvertBSTRToString(*mypd); 

	main.setSongP(osdNum);

	return vaResult;
}

VARIANT CXCardOCXCtrl::updateSLOSD(BSTR FAR* songlist) 
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	TRACE("\n-------CXCardOCXCtrl::updateSLOSD");
	main.updateOSDFlag=true;

	return vaResult;
}
