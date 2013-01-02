// PlayWav.h: interface for the CPlayWav class.
//
//////////////////////////////////////////////////////////////////////

#include "DSUtil.h"
#include "DXUtil.h"

#if !defined(AFX_PLAYWAV_H__CB4B7A87_638E_4399_832B_9D0DFEF288CA__INCLUDED_)
#define AFX_PLAYWAV_H__CB4B7A87_638E_4399_832B_9D0DFEF288CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlayWav  
{
public:
	void init(HWND h);

	void playWave(char * filename,HWND h);
	CSoundManager* g_pSoundManager;
	CSound*        g_pSound ;
	BOOL           g_bBufferPaused;
	CPlayWav();
	virtual ~CPlayWav();

};

#endif // !defined(AFX_PLAYWAV_H__CB4B7A87_638E_4399_832B_9D0DFEF288CA__INCLUDED_)
