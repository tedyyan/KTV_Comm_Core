// XCardOCXPpg.cpp : Implementation of the CXCardOCXPropPage property page class.

#include "stdafx.h"
#include "XCardOCX.h"
#include "XCardOCXPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CXCardOCXPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CXCardOCXPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CXCardOCXPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CXCardOCXPropPage, "XCARDOCX.XCardOCXPropPage.1",
	0xb9d697e2, 0x992f, 0x4b45, 0xa7, 0x6a, 0xde, 0x96, 0x9e, 0xd2, 0x85, 0xc9)


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXPropPage::CXCardOCXPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CXCardOCXPropPage

BOOL CXCardOCXPropPage::CXCardOCXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_XCARDOCX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXPropPage::CXCardOCXPropPage - Constructor

CXCardOCXPropPage::CXCardOCXPropPage() :
	COlePropertyPage(IDD, IDS_XCARDOCX_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CXCardOCXPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXPropPage::DoDataExchange - Moves data between page and properties

void CXCardOCXPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CXCardOCXPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CXCardOCXPropPage message handlers
