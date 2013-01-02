// CommCommander.cpp: implementation of the CCommCommander class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoiveCommControl.h"
#include "CommCommander.h"
#include "../XCardOCX/ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

unsigned char*	CCommCommander:: m_innerBuffer = 0;
int CCommCommander:: m_innerBufferLength = 0;

#define ACC_BUFFER_SIZE  1024

CCommCommander::CCommCommander()
{
	m_nSendBytes = 0;
	m_stoppingFlag = false;
	m_bHexS = true;
	m_ReceiveEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_innerBuffer = new unsigned char[ACC_BUFFER_SIZE];
	m_innerBufferLength = 0;
}

CCommCommander::~CCommCommander()
{	
	m_SerialPort.StopMonitoring();
	m_stoppingFlag = true;
}
CString* StringConvert(CString str,CString cl)
{
    CString sa[1000];
    int i = 0;
    while(1){
        int pos = str.Find(cl);
        if(pos>=0){
            sa[i] = str.Left(pos);
            str = str.Mid(pos+1);
        } else {
            sa[i] = str;
            break;
        }
        i++;
    }

    int n = i+1;
	CString* m_sx = new CString[n];
		TRACE("\n HEX:");
    for(i = 0;i<n;i++)
    {
		sa[i].TrimLeft(" ");
		sa[i].TrimRight(" ");
        m_sx[i] = sa[i];
		TRACE("_%s_",m_sx[i]);
    }
	TRACE("\n");
    return m_sx;
}


DWORD WINAPI ProcessThreadStub(LPVOID lpParam) 
{
	TRACE("----- ProcessThreadStub");
	CCommCommander* comm = (CCommCommander*) lpParam;

	CString* SongBegingArr = StringConvert(comm->m_SongBegin," ");
	CString* SongCancelArr = StringConvert(comm->m_SongCancel," ");
	CString* SongIdelArr = StringConvert(comm->m_SongIdel," ");
	int songBeginLen = (comm->m_SongBegin.GetLength()+1)/3;
	int SongCancelLen = (comm->m_SongCancel.GetLength()+1)/3;
	int SongIdealLen = (comm->m_SongIdel.GetLength()+1)/3;
	TRACE("\n HEX length %d, %d , %d",songBeginLen,SongCancelLen,SongIdealLen);
	while ((comm->m_stoppingFlag) == false) { 
		int processedBytesNum = 0;
		//10 is good value, if 100, too slow
		Sleep(10);
		for(int i=0; i < comm->m_innerBufferLength; i++)
		{
			int j = 0;
			//SongBegin----------------------------------------------------------
			for(j=0; j < songBeginLen; j++)
			{			
				char *str;
				unsigned char tmp2 = comm->m_innerBuffer[i+j];
				unsigned char tmpint = (unsigned char)strtoul(SongBegingArr[j],&str,16);
								
				if (tmp2 != tmpint)
					break;				
			}
			if (j == songBeginLen)				
			{
				comm->m_Status = SONG_BEGIN;
				processedBytesNum = i+j;
				break;
			}

			//SongCancel---------------------------------------------------------			
			for(j=0; j < SongCancelLen; j++)
			{
				char *str;
				unsigned int tmp2 = (unsigned int) comm->m_innerBuffer[i+j];
				unsigned int tmpint = (unsigned int)strtoul(SongCancelArr[j],&str,16);
				if (tmp2 != tmpint)
					break;
			}
			if (j == SongCancelLen)
			{
				comm->m_Status = SONG_CANCEL;
				processedBytesNum = i+j;
				break;
			}

			//SongIdel-----------------------------------------------------------
			for(j=0; j < SongIdealLen; j++)
			{
				char *str;
				unsigned int tmp2 = (unsigned int) comm->m_innerBuffer[i+j];
				unsigned int tmpint = (unsigned int)strtoul(SongIdelArr[j],&str,16);
				//TRACE("\n HEX___ left:%u char:%c,right: %u",tmp2,comm->m_innerBuffer[i+j],tmpint);
				if (tmp2 != tmpint)
					break;
			}
			if (j == SongIdealLen)
			{
				comm->m_Status = SONG_IDEL;
				processedBytesNum = i+j;
				break;
			}
		} // for

		if (processedBytesNum > 0){
			int keepLength = comm->m_innerBufferLength - processedBytesNum;
			for(int i = 0; i < keepLength; i++)
			{
				comm->m_innerBuffer[i] = comm->m_innerBuffer[processedBytesNum + i];			
			}
			ZeroMemory(comm->m_innerBuffer + keepLength, ACC_BUFFER_SIZE - keepLength );
			comm->m_innerBufferLength = keepLength;			
			SetEvent(comm->m_ReceiveEvent);
		}
	}// while
	TRACE("\n+++++++++++++++++Exit ProcessThreadStub ");
	return S_OK;
}

void CCommCommander::Clear()
{
	m_innerBufferLength = 0;
}

CString CCommCommander::GetEvent(bool *stopping)
{
	
	if (NULL == m_threadhandle)
	{
		m_threadhandle = CreateThread(NULL, 0, ProcessThreadStub, this, 0, &m_dwThreadId);
	}
	
	ResetEvent(m_ReceiveEvent);
	DWORD WaitResult=0;
	while (!(*stopping))
	{
		DWORD WaitResult = ::WaitForSingleObject(m_ReceiveEvent,50);
		if (WaitResult == WAIT_OBJECT_0 )
			break;
	}
	TRACE("\nMovie stopped in Comm.");
	return m_Status;

}

void CCommCommander::OnReceiveChar(UINT ch, LONG port)
{
	
	static int i = 0;
	/*
	 *	
	if ((i%80) ==0 )
	{
		if ((ch>19)&&(ch<128))
		TRACE("\nReceived: %c",ch);
	}
	*/
	memcpy(m_innerBuffer+m_innerBufferLength,&ch,1);
	/*
	if ((ch>19)&&(ch<128))
			TRACE("%c",ch);
			*/
	m_innerBufferLength++;
	i++;
}

bool CCommCommander::OpenComm(CString iniFilePathAndName)
{
	char* iniFile = iniFilePathAndName.GetBuffer(iniFilePathAndName.GetLength());
	CIni Ini(iniFile);

	int SelPortNO,SelBaudRate,SelDataBits,SelStopBits;
	char SelParity;
	CString temp;


	temp = Ini.ReadText("comm","PortNO");
	temp.Delete(0,3);
	SelPortNO=atoi(temp);
	SelBaudRate=atoi(Ini.ReadText("comm","BaudRate"));
	SelDataBits=atoi(Ini.ReadText("comm","DataBits"));
	SelParity = atoi(Ini.ReadText("comm","Parity"));
	SelStopBits=atoi(Ini.ReadText("comm","StopBits"));
	if(m_SerialPort.InitPort(&(this->OnReceiveChar),SelPortNO,SelBaudRate,SelParity,SelDataBits,SelStopBits,EV_RXCHAR|EV_CTS,512))		
	{
		m_SerialPort.StartMonitoring();
		TRACE("----Open Comm-------------");
		
	}
	else 
	{
		AfxMessageBox("该串口已经被其他应用程序所占用!\n请选择其它的串口");
		return false;
	}
	m_Zero = Ini.ReadText("comm","Zero");
	m_One = Ini.ReadText("comm","One");
	m_Two = Ini.ReadText("comm","Two");
	m_Three = Ini.ReadText("comm","Three");
	m_Four = Ini.ReadText("comm","Four");
	m_Five= Ini.ReadText("comm","Five");
	m_Six = Ini.ReadText("comm","Six");
	m_Seven = Ini.ReadText("comm","Seven");
	m_Eight = Ini.ReadText("comm","Eight");
	m_Nine = Ini.ReadText("comm","Nine");
	m_VolumneUp = Ini.ReadText("comm","VolumneUp");
	m_VolumneDown = Ini.ReadText("comm","VolumneDown");
	m_SingerOn = Ini.ReadText("comm","SingerOn");
	m_SingerOff = Ini.ReadText("comm","SingerOff");
	

	m_SongBegin = Ini.ReadText("comm","SongBegin");
	m_SongIdel = Ini.ReadText("comm","SongIdel");
	m_SongCancel = Ini.ReadText("comm","SongCancel");

	m_Play = Ini.ReadText("comm","Play");
	m_Cancel1 = Ini.ReadText("comm","Cancel1");
	m_Cancel2 = Ini.ReadText("comm","Cancel2");
	m_Pause = Ini.ReadText("comm","Pause");
	m_Applause = Ini.ReadText("comm","Applause");
	m_TuneUp = Ini.ReadText("comm","TuneUp");
	m_TuneDown = Ini.ReadText("comm","TuneDown");
	m_Service = Ini.ReadText("comm","Service");
	m_micUp = Ini.ReadText("comm","MicUp");
	m_micDown = Ini.ReadText("comm","MicDown");
	return true;

}
char CCommCommander::CombineHexChar(char CharH,char CharL)
{
	char result;
	CString temp;
	if(CharH>='0'&&CharH<='9')			result=(CharH-'0');
	else if(CharH>='a'&&CharH<='f')		result=(CharH-'a'+10);
	else if(CharH>='A'&&CharH<='F')		result=(CharH-'A'+10);
	else								result=0;
	result<<=4;	
	if(CharL>='0'&&CharL<='9')			result+=(CharL-'0');
	else if(CharL>='a'&&CharL<='f')		result+=(CharL-'a'+10);
	else if(CharL>='A'&&CharL<='F')		result+=(CharL-'A'+10);
	else								result+=0;
	return result;
}
CString CCommCommander::ChangeCharstr2Hexstr(CString Charstr)
{
	CString Hexstr=_T("");
	Charstr.MakeUpper();
	HexStringFilter(Charstr);
	int Length=Charstr.GetLength();
	if (Length<1) return "";
	if(Length%2)
		Charstr.Delete(Length-1);
	Length=Charstr.GetLength();
	for(int i=0;i<Length/2;i++)
	{
		Hexstr+=CombineHexChar(Charstr.GetAt(i*2),Charstr.GetAt(i*2+1));
	}
	return Hexstr;
}
void CCommCommander::HexStringFilter(CString &str)
{
	BOOL bOK;
	for(int i=0;i<str.GetLength();)
	{
		bOK=((str.GetAt(i)>='0')&&(str.GetAt(i)<='9'))||
			((str.GetAt(i)>='A')&&(str.GetAt(i)<='F'))||
			((str.GetAt(i)>='a')&&(str.GetAt(i)<='f'));
		if(!bOK)
			str.Delete(i);
		else i++;	
	}
}
void CCommCommander::Pause()
{
	CString temp = m_Pause;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Pause);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());

}

void CCommCommander::Applause()
{
	CString temp = m_Applause;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Applause);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());

}

void CCommCommander::TuneUp()
{
	CString temp = m_TuneUp;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_TuneUp);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());

}

void CCommCommander::TuneDown()
{
	CString temp = m_TuneDown;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_TuneDown);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());

}
void CCommCommander::One()
{
	
	CString temp = m_One;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_One);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
	m_nSendBytes+=temp.GetLength();
	m_strSendBytes.Format("%d",m_nSendBytes);
	return ;
}
void CCommCommander::Zero()
{
	CString temp = m_Zero;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Zero);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());

}

void CCommCommander::Two()
{
	CString temp = m_Two;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Two);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());

}
void CCommCommander::Play()
{
	CString temp = m_Play;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Play);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}


void CCommCommander::Three()
{
	CString temp = m_Three;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Three);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}


void CCommCommander::Four()
{
	CString temp = m_Four;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Four);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}


void CCommCommander::Five()
{
	CString temp = m_Five;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Five);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}


void CCommCommander::Six()
{
	CString temp = m_Six;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Six);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}


void CCommCommander::Seven()
{
	CString temp = m_Seven;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Seven);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}


void CCommCommander::Eight()
{
	CString temp = m_Eight;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Eight);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}


void CCommCommander::Nine()
{
	CString temp = m_Nine;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Nine);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}

void CCommCommander::VolumneUp()
{
	CString temp = m_VolumneUp;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_VolumneUp);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}

void CCommCommander::VolumneDown()
{
	CString temp = m_VolumneDown;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_VolumneDown);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}
void CCommCommander::Shutdown()
{
	m_SerialPort.StopMonitoring();
}
void CCommCommander::SingerOn()
{
	CString temp = m_SingerOn;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_SingerOn);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}

void CCommCommander::SingerOff()
{
	CString temp = m_SingerOff;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_SingerOff);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}

void CCommCommander::MicUp()
{
	CString temp = m_micUp;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_micUp);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}

void CCommCommander::MicDown()
{
	CString temp = m_micDown;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_micDown);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}

void CCommCommander::Service()
{
	CString temp = m_Service;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Service);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}

void CCommCommander::Cancel1()
{
	CString temp = m_SingerOff;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Cancel1);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}

void CCommCommander::Cancel2()
{
	CString temp = m_SingerOff;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(m_Cancel2);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
}