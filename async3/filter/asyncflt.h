//------------------------------------------------------------------------------
// File: AsyncFlt.h
//
// Desc: DirectShow sample code - header file for async filter.
//
// Copyright (c) 1996 - 2000, Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


//  Note:  This filter does not support AVI format


//
//  Define an internal filter that wraps the base CBaseReader stuff
//
#define mmPart 1118192
#define ENCODE_PWD "999111"
BOOL m_load_flag;
class CMemStream : public CAsyncStream
{
public:
    CMemStream() :
        m_llPosition(0)
    {
    }

    /*  Initialization */
    void Init(LPBYTE pbData, LONGLONG llLength, DWORD dwKBPerSec = INFINITE)
    {
		TRACE("\n source ax init begin,length:%d",llLength);
        m_pbData = pbData;
        m_llLength = llLength;
        m_dwKBPerSec = dwKBPerSec;
        m_dwTimeStart = timeGetTime();
    }

    HRESULT SetPointer(LONGLONG llPos)
    {
		TRACE("\nSetPointer:%d, length:%d",llPos,m_llPosition);
        if (llPos < 0 || llPos > m_llLength) {
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
		TRACE("\nRead bytes:%d",dwBytesToRead);
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
        //CopyMemory((PVOID)pbBuffer, (PVOID)(m_pbData + m_llPosition),
        //           dwReadLength);
		//*****************************************************
		if ((m_llPosition<mmPart) && (m_llPosition+dwBytesToRead>mmPart)){
			DWORD firstPart= mmPart-m_llPosition;
			char* mm=(char *)malloc(dwBytesToRead);
			
			//m_pbData->Seek(m_llPosition,CFile::begin);
			//m_pbData->Read(mm,dwBytesToRead);

			for (int i=0;i<dwBytesToRead;i++){
				if (i<firstPart){
					mm[i]=m_pbData[m_llPosition+i]-1;
				}else{
					mm[i]=m_pbData[m_llPosition+i];
				}
			}
			
			CopyMemory((PVOID)pbBuffer, (PVOID)(mm),dwBytesToRead);
			free(mm);
			mm=NULL;
		}else if ((m_llPosition<mmPart) && (m_llPosition+dwBytesToRead<mmPart)){
			char* mm=(char *)malloc(dwBytesToRead);
			
			//m_pbData->Seek(m_llPosition,CFile::begin);
			//m_pbData->Read(mm,dwBytesToRead);

			for (int i=0;i<dwBytesToRead;i++){
				mm[i]=m_pbData[m_llPosition+i]-1;
			}
			CopyMemory((PVOID)pbBuffer, (PVOID)(mm), dwBytesToRead);
			free(mm);
			mm=NULL;
		}else{
			//char* mm=(char *)malloc(dwBytesToRead);

			//m_pbData->Seek(m_llPosition,CFile::begin);
			//m_pbData->Read(mm,dwBytesToRead);
			CopyMemory((PVOID)pbBuffer, (PVOID)(m_pbData + m_llPosition),
                   dwReadLength);
			//CopyMemory((PVOID)pbBuffer, mm, dwBytesToRead);
			//free(mm);
			//mm=NULL;
		}
		
		//*****************************************************/
        m_llPosition += dwReadLength;
        *pdwBytesRead = dwReadLength;
        return S_OK;
    }

    LONGLONG Size(LONGLONG *pSizeAvailable)
    {


        LONGLONG llCurrentAvailable =
            static_cast <LONGLONG> (UInt32x32To64((timeGetTime() - m_dwTimeStart),m_dwKBPerSec));
        *pSizeAvailable =  min(m_llLength, llCurrentAvailable);
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
    PBYTE          m_pbData;
    LONGLONG       m_llLength;
    LONGLONG       m_llPosition;
    DWORD          m_dwKBPerSec;
    DWORD          m_dwTimeStart;
};

class CAsyncFilter : public CAsyncReader, public IFileSourceFilter
{
public:
    CAsyncFilter(LPUNKNOWN pUnk, HRESULT *phr) :
        CAsyncReader(NAME("Mem Reader"), pUnk, &m_Stream, phr),
        m_pFileName(NULL),
        m_pbData(NULL)
    {
			TRACE("\nconstruct");
    }
    ~CAsyncFilter()
    {
		TRACE("\ndeconstruct it");
        delete [] m_pbData;
        delete [] m_pFileName;
    }

    static CUnknown *CreateInstance(LPUNKNOWN, HRESULT *);

    DECLARE_IUNKNOWN

    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv)
    {
        if (riid == IID_IFileSourceFilter) {
            return GetInterface((IFileSourceFilter *)this, ppv);
        } else {
            return CAsyncReader::NonDelegatingQueryInterface(riid, ppv);
        }
    }

    /*  IFileSourceFilter methods */
    //  Load a (new) file

    STDMETHODIMP Load(LPCOLESTR lpwszFileName, const AM_MEDIA_TYPE *pmt)
    {
		/*if (m_load_flag){
			TRACE("\nload new, so empty memory");
			if (m_pbData!=NULL){
				delete [] m_pbData;
			}
			if (m_pFileName!=NULL){
				delete [] m_pFileName;
			}
			
		}*/
		m_load_flag=true;
        CheckPointer(lpwszFileName, E_POINTER);

        // lstrlenW is one of the few Unicode functions that works on win95
        int cch = lstrlenW(lpwszFileName) + 1;
        TCHAR *lpszFileName;
#ifndef UNICODE
        lpszFileName = new char[cch * 2];
        if (!lpszFileName) {
      	    return E_OUTOFMEMORY;
        }
        WideCharToMultiByte(GetACP(), 0, lpwszFileName, -1,
    			lpszFileName, cch, NULL, NULL);
#else
        lpszFileName = lpwszFileName;
#endif
        CAutoLock lck(&m_csFilter);

        /*  Check the file type */
        CMediaType cmt;
        if (NULL == pmt) {
            cmt.SetType(&MEDIATYPE_Stream);
            cmt.SetSubtype(&MEDIASUBTYPE_NULL);
        } else {
            cmt = *pmt;
        }


        if (!ReadTheFile(lpszFileName)) {
#ifndef UNICODE
            delete [] lpszFileName;
#endif
            return E_FAIL;
        }
        m_Stream.Init(m_pbData, m_llSize);

        m_pFileName = new WCHAR[cch];
        if (m_pFileName!=NULL) {
	    CopyMemory(m_pFileName, lpwszFileName, cch*sizeof(WCHAR));
        }
        // this is not a simple assignment... pointers and format
        // block (if any) are intelligently copied
	m_mt = cmt;

        /*  Work out file type */
        cmt.bTemporalCompression = TRUE;	       //???
        cmt.lSampleSize = 1;

        return S_OK;
    }

    // Modelled on IPersistFile::Load
    // Caller needs to CoTaskMemFree or equivalent.

    STDMETHODIMP GetCurFile(LPOLESTR * ppszFileName, AM_MEDIA_TYPE *pmt)
    {
		int CharCount2 = WideCharToMultiByte(CP_ACP,0,*ppszFileName,-1,NULL,0,NULL,0);
		TRACE("\nGetCurFile %d--------------------------------------",CharCount2);
		if (CharCount2==2){
			TRACE("\n Pass --------------------------------------");
			if (m_pbData!=NULL){
				delete [] m_pbData;
			}
			TRACE("\n delete FileName --------------------------------------");
			//if (m_pFileName!=NULL){
			//	delete [] m_pFileName;
			//}
		}else{
			TRACE("\n No Pass --------------------------------------");
		}
        CheckPointer(ppszFileName, E_POINTER);
        *ppszFileName = NULL;
        if (m_pFileName!=NULL) {
    	DWORD n = sizeof(WCHAR)*(1+lstrlenW(m_pFileName));

            *ppszFileName = (LPOLESTR) CoTaskMemAlloc( n );
            if (*ppszFileName!=NULL) {
                  CopyMemory(*ppszFileName, m_pFileName, n);
            }
        }

        if (pmt!=NULL) {
            CopyMediaType(pmt, &m_mt);
        }

        return NOERROR;
    }

private:
    BOOL CAsyncFilter::ReadTheFile(LPCTSTR lpszFileName);

private:
    LPWSTR     m_pFileName;
    LONGLONG   m_llSize;
    PBYTE      m_pbData;
    CMemStream m_Stream;
	
};
