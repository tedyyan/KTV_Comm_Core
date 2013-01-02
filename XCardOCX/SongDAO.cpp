// SongDAO.cpp: implementation of the CSongDAO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SongDAO.h"

#include <windows.h>
#include <mmsystem.h>


#include "resource.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static bool openflag=false;

extern TCHAR dbPath[_MAX_PATH];
	_ConnectionPtr	m_pConnection;
	_CommandPtr		m_pCommand;
	_RecordsetPtr	m_pRecordset;
	_RecordsetPtr	m_pRecordsetAD;
	
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSongDAO::CSongDAO()
{

	
	m_pConnection.CreateInstance(__uuidof(Connection));
	m_pRecordset.CreateInstance(__uuidof(Recordset));


}

CSongDAO::~CSongDAO()
{
	
}



CString CSongDAO::getNextSongName(CString* value,boolean *encode,
				int* volumeSong,int* volumeSinger,int *songChannel,int *singerChannel,bool *han,int* mpeg4flag)
{	
	if (value!=NULL)
		TRACE("\n-------------getNextSongName ");
	else
		TRACE("\n-------------getCurrentSongName ");
	
	this->initDB();

	VARIANT vaResult;
	VariantInit(&vaResult);
	try
	{
		m_pRecordset = m_pConnection->Execute("SELECT * FROM temp order by orders"
			,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		this->closeDAO1();
		return "";
	}

	//play the song
	_variant_t var,var_server,var_songname;
	CString songFileName,songName=" 无";
	songFileName="";
	try
	{
		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		else
		{
			TRACE("temp table is empty");
			
		}

		if (!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect("bianhao"); //songFile
			
			//var_server = m_pRecordset->GetCollect("serverName");
			if(var.vt != VT_NULL)
				songFileName = (LPCSTR)_bstr_t(var);
			//if(var_server.vt != VT_NULL)
			//	songFileName = (LPCSTR)_bstr_t(var_server)+songFileName;

			var = m_pRecordset->GetCollect("bianhao");
			songFileName = (LPCSTR)_bstr_t(var);

			//------------------------------------
				var = m_pRecordset->GetCollect("encode");
			if(var.vt != VT_NULL)				
				*encode=var.boolVal;
			*volumeSong = fetchvol(m_pRecordset->GetCollect("volumeSong").dblVal);
			*volumeSinger =fetchvol( m_pRecordset->GetCollect("volumeSinger").dblVal);

			*songChannel = m_pRecordset->GetCollect("SongChannel").intVal%5;
			*singerChannel = m_pRecordset->GetCollect("StereoChannel").intVal%5;
			*mpeg4flag = m_pRecordset->GetCollect("Others1").intVal;
			TRACE("\nsongv: %d, singv:%d, songchannnel:%d, singerch:%d\n",*volumeSong,*volumeSinger,*songChannel,*singerChannel);
			
			//return next song name
			m_pRecordset->MoveNext();
			if ((value!=NULL)&&(!m_pRecordset->adoEOF)){
				*han=m_pRecordset->GetCollect("hanFlag").boolVal;
				if (*han){
					var_songname = m_pRecordset->GetCollect("bianhao");  //songHanName
				}else{
					var_songname = m_pRecordset->GetCollect("bianhao");  //songName
				}
				
				if(var_songname.vt != VT_NULL)
				{
					
					ASSERT(var_songname.vt == VT_BSTR);
					//ASSERT(var_songname.vt ==VT_LPWSTR);
					
					CString songName1(var_songname.bstrVal);
					songName=songName1;
					//songName = var_songname.bstrVal; // (LPCSTR)_bstr_t(var_songname);
				}
				*value = songName;
				
			}

		}
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		this->closeDAO1();
		return "";
	}
	m_pRecordset->Close();
	m_pRecordset.Release();
	this->closeDAO1();
	TRACE("++++++++++++++++getNextSongName \n");
	return songFileName;
}


CString CSongDAO::getNextADSongName(int i,boolean *encode,int *volumeSong,int *volumeSinger,int *songChannel,int *singerChannel,int* mpeg4flag)
{
	TRACE("\n-------------getNextADSongName ");		
	this->initDB();

	//get sum of ad songs
	long ADcount=2;

	VARIANT vaResult,var1;
	VariantInit(&vaResult);
	try
	{
		m_pRecordset = m_pConnection->Execute("SELECT count(id) as sum1 FROM adv"
			,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		this->closeDAO1();
		return "";
	}
	var1 = m_pRecordset->GetCollect("sum1");
	
	if(var1.vt != VT_NULL)
		ADcount = var1.intVal;

	srand((unsigned)time(NULL));
	int tempr=rand();
	ADcount = tempr%ADcount;

	try
	{
		m_pRecordset = m_pConnection->Execute("SELECT * FROM adv order by id"
			,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
	}
	//play the song
	_variant_t var;
	CString songFileName;
	songFileName="";
	try
	{
		if(!m_pRecordset->BOF){
			m_pRecordset->MoveFirst();
			for (int j=0;j< ADcount;j++){
				m_pRecordset->MoveNext();
			}
		}
		else
		{
			TRACE("getNextADSongName 表内数据为空");
			
		}
		// 读入库中各字段并加入列表框中
		if (!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect("bianhao"); //songFile
			if(var.vt != VT_NULL)
				songFileName = (LPCSTR)_bstr_t(var);
			//------------------------------------
			//var = m_pRecordset->GetCollect("serverName");
			//if(var.vt != VT_NULL)
			//	songFileName = (LPCSTR)_bstr_t(var)+songFileName;

			*volumeSong = m_pRecordset->GetCollect("volumeSong").intVal;
			*volumeSinger = m_pRecordset->GetCollect("volumeSinger").intVal;
			*songChannel = m_pRecordset->GetCollect("SongChannel").intVal;
			*singerChannel = m_pRecordset->GetCollect("StereoChannel").intVal;
			*mpeg4flag = m_pRecordset->GetCollect("Others1").intVal;

			TRACE("\nsongv: %d, singv:%d, songchannnel:%d, singerchannnel:%d",volumeSong,volumeSinger,songChannel,singerChannel);
			//------------------------------------
			var = m_pRecordset->GetCollect("encode");
			if(var.vt != VT_NULL)				
				*encode=var.boolVal;

		}
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		
	}
	m_pRecordset->Close();
	m_pRecordset.Release();
	this->closeDAO1();
	TRACE("\n+++++++++++++++getNextADSongName ");	
	return songFileName;
}

CSongDAO* CSongDAO::getInstance(){
	static CSongDAO* _instance ;
	if (_instance == NULL){
		_instance = new CSongDAO(); //lazy initialization
	}
	return _instance;
}

CString CSongDAO::deleteCurrentSong()
{
	TRACE("-------------deleteCurrentSong \n");
	this->initDB();
	VARIANT vaResult;
	VariantInit(&vaResult);
	
	try
	{
		m_pRecordset =m_pConnection->Execute("SELECT * FROM temp order by orders"
			,&vaResult,adCmdText); 
		//play the song
		_variant_t var;
		int orders;
		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		else
		{
			TRACE("表内数据为空");
			
		}
		// 读入库中各字段并加入列表框中
		if (!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect("orders");
			if(var.vt != VT_NULL)
				orders = var.intVal;
		}
		
		m_pRecordset->Close();
		m_pRecordset.Release();
		
		CString sqlstr;
		sqlstr.Format("DELETE FROM temp where orders=%d",orders);
		m_pConnection->Execute(_bstr_t(sqlstr),&vaResult,adCmdText);

	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
	}
	this->closeDAO1();
	TRACE("++++++++++++++++deleteCurrentSong \n");
	return "";
	
}

void CSongDAO::initDB()
{
	//TRACE("\n-------initDB");
	if (openflag==false){		
		TRACE("\n Setup db connection");
		try                 
		{	
		
			//Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\\ktvdb.mdb
			m_pConnection->Open(dbPath,"","",adModeUnknown);
			
		}
		catch(_com_error e)
		{
			TCHAR errMsg[MAX_PATH];		
			TRACE("error in connecting db");
			sprintf(errMsg,_T("Code meaning = %s\n"), (char*) e.ErrorMessage());
			TRACE(errMsg);
			
			//LPWSTR err;
			//e.ErrorInfo()->GetDescription(&err);
			//sprintf(errMsg,_T("Code meaning = %s\n"), (char*) err);
			//TRACE(errMsg);	
		} 
		openflag=true;
	}
	//TRACE("++++++++initDB\n");
	
}

void CSongDAO::closeDAO1()
{
	//TRACE("\n---------------closeDAO1");
	//TRACE("\nDo nothing");
	
	//TRACE("+++++++++++++++closeDAO1\n");
	//m_pConnection= NULL;
}

/*int CSongDAO::getCurrentSingerChannel()
{
	this->initDB();
	VARIANT vaResult;
	VariantInit(&vaResult);
	try
	{
		m_pRecordset = m_pConnection->Execute("SELECT * FROM temp order by orders"
			,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
	}

	//play the song
	_variant_t var;
	int ch;
	try
	{
		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		else
		{
			TRACE("表内数据为空");
			
		}

		if (!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect("singerChannel");
			if(var.vt != VT_NULL)
				ch=var.intVal;
		}
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		
	}
	m_pRecordset->Close();
	m_pRecordset.Release();
	this->closeDAO1();
	return ch;
}*/

int CSongDAO::getCurrentSongChannel()
{
	TRACE("\n-----------CSongDAO::getCurrentSongChannel");
	this->initDB();
	VARIANT vaResult;
	VariantInit(&vaResult);
	try
	{
		m_pRecordset = m_pConnection->Execute("SELECT * FROM temp order by orders"
			,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
	}

	//play the song
	_variant_t var;
	int ch;
	try
	{
		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		else
		{
			TRACE("\ntemp Table is empty");
			
		}

		if (!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect("songChannel");
			if(var.vt != VT_NULL)
				ch=var.intVal;
		}
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		
	}
	m_pRecordset->Close();
	m_pRecordset.Release();
	this->closeDAO1();
	TRACE("\n+++++++++++++CSongDAO::getCurrentSongChannel");
	return ch;
}

int CSongDAO::getCurrentStereoChannel()
{
	TRACE("\n-------------getCurrentStereoChannel ");
	this->initDB();	
	VARIANT vaResult;
	VariantInit(&vaResult);
	try
	{
		m_pRecordset = m_pConnection->Execute("SELECT * FROM temp order by orders"
			,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
	}

	//play the song
	_variant_t var;
	int ch;
	try
	{
		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		else
		{
			TRACE("表内数据为空");
			
		}

		if (!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect("StereoChannel");
			if(var.vt != VT_NULL)
				ch = var.intVal;
		}
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		
	}
	m_pRecordset->Close();
	m_pRecordset.Release();
	this->closeDAO1();
	TRACE("\n++++++++++++++getCurrentStereoChannel ");
	return ch;
}

CString CSongDAO::getRandomSongName(int i,boolean* encode,int* volumeSong,
								int* volumeSinger,int *songChannel,int *singerChannel,int *mpeg4flag)
{
	TRACE("\n-------------getRandomSongName ");

	
	
	this->initDB();

	VARIANT vaResult,var;
	VariantInit(&vaResult);

	char temp[50];
	

	//---------------------------
	int sum1=1;
	_bstr_t sql=_bstr_t("SELECT count(id) as sum1 FROM songs ");
	try
	{
		m_pRecordset = m_pConnection->Execute(sql,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
	}
	var = m_pRecordset->GetCollect("sum1");
	
	if(var.vt != VT_NULL)
		sum1 = var.intVal;
	
	srand((unsigned)time(NULL));
	int r=rand();
	sum1=r%sum1;
	
	TRACE("\nDAO  getRandomSongName sum is %d\n",sum1);

	//-------------------------------------------------------
	sql=_bstr_t("SELECT * FROM songs");
	try
	{
		m_pRecordset = m_pConnection->Execute(sql,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
	}

	CString songFileName;
	try
	{
		if(!m_pRecordset->BOF){
			m_pRecordset->MoveFirst();
			for (int j=0;j<sum1-1;j++){
				m_pRecordset->MoveNext();
			}
			
		}
		else
		{
			TRACE("getRandomSongName AD表内数据为空");
			
		}
		if (!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect("bianhao"); //songFile
			if(var.vt != VT_NULL)
				songFileName = (LPCSTR)_bstr_t(var);
			//var = m_pRecordset->GetCollect("serverName");
			//if(var.vt != VT_NULL)
			//	songFileName = (LPCSTR)_bstr_t(var)+songFileName;
			*volumeSong = m_pRecordset->GetCollect("volumeSong").intVal;
			*volumeSinger = m_pRecordset->GetCollect("volumeSinger").intVal;
			*songChannel = m_pRecordset->GetCollect("SongChannel").intVal;
			*singerChannel = m_pRecordset->GetCollect("StereoChannel").intVal;
			*mpeg4flag = m_pRecordset->GetCollect("Others1").intVal;
				var = m_pRecordset->GetCollect("encode");
			if(var.vt != VT_NULL)				
				*encode=var.boolVal;
			TRACE("\nDAO songv: %d, singv:%d, songchannnel:%d",volumeSong,volumeSinger,songChannel);
		}
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		
	}
	m_pRecordset->Close();
	m_pRecordset.Release();
	this->closeDAO1();
	TRACE("\n++++++++++++++++getRandomSongName ");
	return songFileName;
}

CString CSongDAO::getScoreMovie(int score)
{
	TRACE("\n-------------getScoreMovie ");

	CString songFileName="";

	this->initDB();

	VARIANT vaResult,var;
	VariantInit(&vaResult);
	CString id;
	char temp[50];
	id=itoa(score,temp,10);
	_bstr_t sql=_bstr_t("SELECT * FROM score where scoreNum='"+id+"'");
	try
	{
		m_pRecordset = m_pConnection->Execute(
			sql//"SELECT * FROM temp order by orders where id="+id
			,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		return songFileName;
	}

	
	try
	{
		if(!m_pRecordset->BOF){
			m_pRecordset->MoveFirst();
			
		}
		else
		{
			TRACE("getScoreMovie 表内数据为空");
			
		}
		if (!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect("bianhao"); //songFile
			if(var.vt != VT_NULL)
				songFileName = (LPCSTR)_bstr_t(var);
			//var = m_pRecordset->GetCollect("serverName");
			//if(var.vt != VT_NULL)
			//	songFileName = (LPCSTR)_bstr_t(var)+songFileName;
		}
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		return songFileName;
	}
	m_pRecordset->Close();
	m_pRecordset.Release();
	this->closeDAO1();
	TRACE("\n++++++++++++++++getScoreMovie ");
	return songFileName;
}

CString CSongDAO::getMovieName(int id1,CString* path,bool* han)
{
	TRACE("\n-------------getMovieName ");

	CString songFileName="";

	this->initDB();
	CString songSongName;
	VARIANT vaResult,var;
	VariantInit(&vaResult);
	CString id;
	char temp[50];
	id=itoa(id1,temp,10);
	int len=id.GetLength();
	for (int i=0;i<5-len;i++){
		id="0"+id;
	}
	TRACE("\nsong id:%s",id);
	_bstr_t sql=_bstr_t("SELECT * FROM songs where (((songs.bianhao) = '"+id+"'))");
	try
	{
		m_pRecordset = m_pConnection->Execute(sql,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		return songFileName;
	}
	
	
	try
	{
		if(!m_pRecordset->BOF){
			m_pRecordset->MoveFirst();
		}
		else
		{
			TRACE("\ngetMovieName 表内数据为空:\n");
		}
		
		if (!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect("bianhao"); //songFile
			if(var.vt != VT_NULL)
				songFileName = (LPCSTR)_bstr_t(var);
			//var = m_pRecordset->GetCollect("serverName");
			//if(var.vt != VT_NULL)
			//	songFileName = (LPCSTR)_bstr_t(var)+songFileName;
			var = m_pRecordset->GetCollect("songName");
			if(var.vt != VT_NULL)
				songSongName = var.bstrVal;
			var = m_pRecordset->GetCollect("hanFlag");
			if(var.vt != VT_NULL){
				bool han1 = var.boolVal;	
				*han=han1;
				if (han1){
					var = m_pRecordset->GetCollect("songHanName");
					ASSERT(var.vt == VT_BSTR);
					CString songName1(var.bstrVal);
					songSongName=songName1;
				}
			}
			path=&songFileName;
			
		}
	}catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		return songFileName;
	}
	m_pRecordset->Close();
	m_pRecordset.Release();
	this->closeDAO1();
	TRACE("\n+++++++++++++++++getMovieName ");
	return songSongName;
}

void CSongDAO::testUpdate()
{
	VARIANT vaResult,var;
	_bstr_t sql=_bstr_t("SELECT songName FROM adv where id=111");
	try
	{
		m_pRecordset = m_pConnection->Execute(
			sql,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		return ;
	}

	CString songName;
	try
	{	
		if (!m_pRecordset->adoEOF)
		{
			var = m_pRecordset->GetCollect("songName");
			if(var.vt != VT_NULL)
				songName = (LPCSTR)_bstr_t(var);
		}
	}catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		return ;
	}

	
}

CString CSongDAO::getPlayList(CString rr)
{
	TRACE("\n-------------getPlayList ");

	CString songFileName="";
	int cnt=0;
	this->initDB();
	VARIANT vaResult,var;
	VariantInit(&vaResult);
	_bstr_t sql=_bstr_t("SELECT * FROM temp order by orders");
	try
	{
		m_pRecordset = m_pConnection->Execute(
			sql,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		return songFileName;
	}	
	try
	{
		if(!m_pRecordset->BOF){
			m_pRecordset->MoveFirst();
			m_pRecordset->MoveNext();
			
		}
		else
		{
			TRACE("\nAD table is empty.");
		}
		songFileName="";
		while (!m_pRecordset->BOF&&!m_pRecordset->adoEOF) {
			
			cnt++;
			var = m_pRecordset->GetCollect("bianhao");
			if (cnt<5){
				if(var.vt == VT_BSTR)
					songFileName = songFileName+var.bstrVal+" ";
			}else{
				
			}
			
			m_pRecordset->MoveNext();
		}
	}catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
		return songFileName;
	}
	m_pRecordset->Close();
	m_pRecordset.Release();
	this->closeDAO1();
	char temp[50];
	itoa(cnt,temp,10);
	CString a;
	a.Format("%s",temp);
	if (a.GetLength()==1) {
		a="0"+a;
	}
	songFileName=rr+a+" "+songFileName;
	TRACE("\n++++++++++++++++getPlayList ");
	return songFileName;
}

void CSongDAO::deleteAllTemp()
{
	TRACE("\n-------------deleteAllTemp ");
	this->initDB();
	VARIANT vaResult,var;
	VariantInit(&vaResult);
	_bstr_t sql=_bstr_t("delete from temp ");
	try
	{
		m_pRecordset = m_pConnection->Execute(
			sql,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
	}
	this->closeDAO1();
	TRACE("\n++++++++++++++deleteAllTemp ");	
}

int CSongDAO::getPlayCount()
{
	TRACE("\n-------------getPlayCount ");
	this->initDB();

	//get sum of ad songs
	long ADcount=2;

	VARIANT vaResult,var1;
	VariantInit(&vaResult);
	try
	{
		m_pRecordset = m_pConnection->Execute("SELECT count(id) as sum1 FROM temp"
			,&vaResult,adCmdText); 
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
	}
	var1 = m_pRecordset->GetCollect("sum1");
	
	if(var1.vt != VT_NULL)
		ADcount = var1.intVal;
	this->closeDAO1();
	TRACE("\n+++++++++++++getPlayCount ");
	return ADcount;
}

int CSongDAO::fetchvol(double v)
{
	int vol,temp,vol1,vol2,vol3;
	if (v==0) return 0;
	if (v<0)
		v=-v;
	if (v>=1){
		return (int)-v;
	}else{
		temp=v*1000;
		vol1=(((temp/100)+5)%10)*100;
		vol=(temp/100)*100;
		vol2=((((temp-vol)/10)+5)%10)*10;
		vol3=(temp%10+5)%10;
	}
	return -(vol1+vol2+vol3);
}

bool CSongDAO::closeDBConnection()
{
	if (openflag){
		if(m_pConnection->State)
			m_pConnection->Close();
		openflag=false;
		return true;
	}else{
		return false;
	}
}
