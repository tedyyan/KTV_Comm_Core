// PlayWav.cpp: implementation of the CPlayWav class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XCardOCX.h"
#include "PlayWav.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayWav::CPlayWav()
{
	g_pSoundManager = NULL;
	g_pSound = NULL;
}

CPlayWav::~CPlayWav()
{

}

void CPlayWav::playWave(char * strFileName,HWND h)
{
	TRACE("----CPlayWav::playWave-- \n");
	this->init(h);
	HRESULT hr;
	//WCHAR wFile[MAX_PATH];
	//MultiByteToWideChar(CP_ACP, 0, strFileName, -1, wFile, MAX_PATH);
	if( FAILED( hr = g_pSoundManager->Create( &g_pSound, strFileName, 0, GUID_NULL ) ) )
    {
		TRACE("Error in init playWave\n");
        return; 
    }
	bool bLooped=false;
	DWORD dwFlags = bLooped ? DSBPLAY_LOOPING : 0L;
    if (FAILED( hr = g_pSound->Play( 0, dwFlags )  )){
		TRACE("Error in play\n");
	}
	return; 
}

void CPlayWav::init(HWND h)
{
	TRACE("----CPlayWav::init-- \n");
	g_pSoundManager = new CSoundManager();
	HRESULT hr;
    if( FAILED( hr = g_pSoundManager->Initialize( h, DSSCL_PRIORITY, 2, 22050, 16 ) ) )
    {
        //DXTRACE_ERR( TEXT("Initialize"), hr );
        /*MessageBox( hDlg, "Error initializing DirectSound.  Sample will now exit.", 
                            "DirectSound Sample", MB_OK | MB_ICONERROR );
        EndDialog( hDlg, IDABORT );*/
		TRACE("Error in init wav\n");
        return;
    }

    g_bBufferPaused = FALSE;
}
