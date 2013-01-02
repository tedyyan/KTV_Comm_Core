// SongDAO.h: interface for the CSongDAO class.
//
//////////////////////////////////////////////////////////////////////

#import "c:\program files\common files\system\ado\msado15.dll" \
	no_namespace \
	rename ("EOF", "adoEOF") 

#if !defined(AFX_SONGDAO_H__5ACC7454_5003_4937_BBAA_A59A5A7FC7E1__INCLUDED_)
#define AFX_SONGDAO_H__5ACC7454_5003_4937_BBAA_A59A5A7FC7E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSongDAO  
{
public:
	bool closeDBConnection();
	int fetchvol(double v);
	int getPlayCount();
	void deleteAllTemp();
	CString getPlayList(CString rr);
	void testUpdate();
	CString getMovieName(int id,CString* filename,bool* han);
	CString getScoreMovie(int score);
	CString getRandomSongName(int i,boolean* encode,int* volumeSong,int* volumeSinger,int *songChannel,int *singerChannel,int *mpeg4flag);
	int getCurrentStereoChannel();
	int getCurrentSongChannel();
	/*int getCurrentSingerChannel();*/
	void closeDAO1();
	
	CString deleteCurrentSong();
	static CSongDAO* getInstance();

	CString getNextSongName(CString* value,boolean *encode,int* volumeSong,int* volumeSinger,int *songChannel,int *singerChannel,bool *han,int* mpeg4flag);
	CString getNextADSongName(int i,boolean *encode,int *volumeSong,int *volumeSinger,int *songChannel,int *singerChannel,int* mpeg4flag);

	
//protected:
	CSongDAO();
	virtual ~CSongDAO();

	

private:

	//CSongDAO(const CSongDAO&);
	//CSongDAO& operator=(const CSongDAO&);
	void initDB();


};

#endif // !defined(AFX_SONGDAO_H__5ACC7454_5003_4937_BBAA_A59A5A7FC7E1__INCLUDED_)
