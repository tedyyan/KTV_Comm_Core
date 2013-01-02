//------------------------------------------------------------------------------
// File: MemFile.h
//
// Desc: DirectShow sample code - header file for application using async 
//       filter.
//
// Copyright (c) 1996 - 2000, Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


//
//  Define an internal filter that wraps the base CBaseReader stuff
//

#include "asyncio.h"
#include "asyncrdr.h"
#define mmPart 8192
class CMemStream : public CAsyncStream
{
public:
    CMemStream(LPBYTE pbData, LONGLONG llLength, DWORD dwKBPerSec = INFINITE) :
        m_pbData(pbData),
        m_llLength(llLength),
        m_llPosition(0),
        m_dwKBPerSec(dwKBPerSec)
    {
        m_dwTimeStart = timeGetTime();
    }
    HRESULT SetPointer(LONGLONG llPos)
    {
		//TRACE("\n----setPointer-----------------------");
        if (llPos < 0 || llPos > m_llLength) {
			//TRACE("\n  FLASE in setPointer");
            return S_FALSE;
        } else {
            m_llPosition = llPos;
            return S_OK;
        }
    }
    HRESULT Read(PBYTE pbBuffer,
                 DWORD dwBytesToRead,
                 BOOL bAlign,
                 LPDWORD pdwBytesRead)
    {
		TRACE("\n=Read==%d===========",dwBytesToRead);
        CAutoLock lck(&m_csLock);
        DWORD dwReadLength;
        /*  Wait until the bytes are here! */
        DWORD dwTime = timeGetTime();
        if (m_llPosition + dwBytesToRead > m_llLength) {
            dwReadLength = (DWORD)(m_llLength - m_llPosition);
        } else {
            dwReadLength = dwBytesToRead;
        }
        DWORD dwTimeToArrive =
            ((DWORD)m_llPosition + dwReadLength) / m_dwKBPerSec;
        if (dwTime - m_dwTimeStart < dwTimeToArrive) {
            Sleep(dwTimeToArrive - dwTime + m_dwTimeStart);
        }
		//*****************************************************
		if (m_llPosition<mmPart && m_llPosition+dwBytesToRead>mmPart){
			DWORD firstPart= mmPart-m_llPosition;
			char* mm=(char *)malloc(dwBytesToRead);
			for (int i=0;i<dwBytesToRead;i++){
				if (i<firstPart)
					mm[i]=m_pbData[m_llPosition+i]-1;
				else
					mm[i]=m_pbData[m_llPosition+i];
			}

			CopyMemory((PVOID)pbBuffer, (PVOID)(mm),dwBytesToRead);
		}else if (m_llPosition<mmPart && m_llPosition+dwBytesToRead<mmPart){
			DWORD firstPart= dwBytesToRead;
			
			char* mm=(char *)malloc(firstPart);
			for (int i=0;i<firstPart;i++){
				mm[i]=m_pbData[m_llPosition+i]-1;
			}
			CopyMemory((PVOID)pbBuffer, (PVOID)(mm), firstPart);
		}else{
			CopyMemory((PVOID)pbBuffer, (PVOID)(m_pbData + m_llPosition),
                   dwReadLength);
		}

		//*****************************************************
        m_llPosition += dwReadLength;
        *pdwBytesRead = dwReadLength;
        return S_OK;
    }
    LONGLONG Size(LONGLONG *pSizeAvailable)
    {
		//TRACE("\n==============size=====================\n");
        LONGLONG llCurrentAvailable =
            Int32x32To64((timeGetTime() - m_dwTimeStart),m_dwKBPerSec);
        *pSizeAvailable = min(m_llLength, llCurrentAvailable);
		//TRACE("\n  length %d",m_llLength);
        return m_llLength;
    }
    DWORD Alignment()
    {
        return 1;
    }
    void Lock()
    {
        m_csLock.Lock();
    }
    void Unlock()
    {
        m_csLock.Unlock();
    }

private:
    CCritSec       m_csLock;
    const PBYTE    m_pbData;
    const LONGLONG m_llLength;
    LONGLONG       m_llPosition;
    DWORD          m_dwKBPerSec;
    DWORD          m_dwTimeStart;
};

class CMemReader : public CAsyncReader
{
public:

    //  We're not going to be CoCreate'd so we don't need registration
    //  stuff etc
    STDMETHODIMP Register()
    {
        return S_OK;
    }
    STDMETHODIMP Unregister()
    {
        return S_OK;
    }
    CMemReader(CMemStream *pStream, CMediaType *pmt, HRESULT *phr) :
        CAsyncReader(NAME("Mem Reader"), NULL, pStream, phr)
    {
        m_mt = *pmt;
    }
};
