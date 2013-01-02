//********************************************
//	Ini ��غ���
//********************************************

#include <stdafx.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <io.h>
#include <fcntl.h>
#include <assert.h>
#include "ini.h"

////////////////////////////////////////////////
// ͨ�ýӿ�
////////////////////////////////////////////////

//��ʼ��
CIni::CIni()
{
	m_lDataLen = 0;
	m_strData = NULL;
	IndexNum = 0;
	IndexList = NULL;
}

//��ʼ��
CIni::CIni(char *filename)
{
	m_lDataLen=0;
	m_strData=NULL;
	IndexNum=0;
	IndexList=NULL;
	Open(filename);
}

//�����ͷ�
CIni::~CIni()
{
	if( m_lDataLen != 0 )
	{
		SAFE_DELETE( m_strData );
		m_lDataLen = 0;
	}

	if( IndexNum != 0 )
	{
		SAFE_DELETE( IndexList );
		IndexNum = 0;
	}
}

//�����ļ�
bool CIni::Open(char *filename)
{
	strcpy(m_strFileName, filename);

	SAFE_FREE( m_strData );

	//��ȡ�ļ�����
	int fh;
	fh = _open( filename, _O_RDONLY );
	if( fh== -1 )
	{
		m_lDataLen = -1;
	}
	m_lDataLen = _filelength(fh);
	_close(fh);
	
	//�ļ�����
	if( m_lDataLen > 0 )
	{
		m_strData = new char[m_lDataLen];

		FILE *fp;
		fp=fopen(filename, "rb");
		assert( fp!=NULL );
		fread(m_strData, m_lDataLen, 1, fp);		//������
		fclose(fp);

		//��ʼ������
		InitIndex();
		return true;
	}
	else	// �ļ�������
	{
		// �Ҳ����ļ�
		m_lDataLen=1;
		m_strData = new char[m_lDataLen];
		memset(m_strData, 0, 1);
		InitIndex();
	}

	return false;
}

//�ر��ļ�
void CIni::Close()
{
	if( m_lDataLen != 0 )
	{
		SAFE_DELETE( m_strData );
		m_lDataLen = 0;
	}

	if( IndexNum != 0 )
	{
		SAFE_DELETE( IndexList );
		IndexNum = 0;
	}
}

//д���ļ�
bool CIni::Save(char *filename)
{
	if( filename==NULL )
	{
		filename=m_strFileName;
	}

	FILE *fp;
	fp=fopen(filename, "wb");
	assert( fp!=NULL );

	fwrite(m_strData, m_lDataLen, 1, fp);
	fclose(fp);

	return true;
}

//�����ļ�����
char *CIni::GetData()
{
	return m_strData;
}

//����ļ�������
int CIni::GetLines(int cur)
{
	int n=1;
	for(int i=0; i<cur; i++)
	{
		if( m_strData[i]=='\n' )
			n++;
	}
	return n;
}

////////////////////////////////////////////////
// �ڲ�����
////////////////////////////////////////////////

//��������е�����λ��
void CIni::InitIndex()
{
	IndexNum=0;

	for(int i=0; i<m_lDataLen; i++)
	{
		//�ҵ�
		if( m_strData[i]=='[' && ( m_strData[i-1]=='\n' || i==0 ) )
		{
			IndexNum++;
		}
	}

	//�����ڴ�
	SAFE_DELETE( IndexList );
	if( IndexNum>0 )
		IndexList=new int[IndexNum];

	int n=0;

	for(i=0; i<m_lDataLen; i++)
	{
		if( m_strData[i]=='[' && ( m_strData[i-1]=='\n' || i==0 ) )
		{
			IndexList[n]=i+1;
			n++;
		}
	}
}

//����ָ������λ��
int CIni::FindIndex(char *string)
{
	for(int i=0; i<IndexNum; i++)
	{
		char *str=ReadText( IndexList[i] );
		if( strcmp(string, str) == 0 )
		{
			SAFE_FREE( str );
			return IndexList[i];
		}
		SAFE_FREE( str );
	}
	return -1;
}

//����ָ�����ݵ�λ��
int CIni::FindData(int index, char *string)
{
	int p=index;	//ָ��

	while(1)
	{
		p=GotoNextLine(p);
		char *name=ReadDataName(p);
		if( strcmp(string, name)==0 )
		{
			SAFE_FREE( name );
			return p;
		}

		SAFE_FREE( name );
		if( p>=m_lDataLen ) return -1;
	}
	return -1;
}

//����
int CIni::GotoNextLine(int p)
{
	for(int i=p; i<m_lDataLen; i++)
	{
		if( m_strData[i]=='\n' )
			return i+1;
	}
	return i;
}

//��ָ��λ�ö�һ��������
char *CIni::ReadDataName(int &p)
{
	char chr;
	char *Ret;
	int m=0;

	Ret=new char[64];
	memset(Ret, 0, 64);

	for(int i=p; i<m_lDataLen; i++)
	{
		chr = m_strData[i];

		//����
		if( chr == '\r' )
		{
			p=i+1;
			return Ret;
		}
		
		//����
		if( chr == '=' || chr == ';' )
		{
			p=i+1;
			return Ret;
		}
		
		Ret[m]=chr;
		m++;
	}
	return Ret;
}

//��ָ��λ�ö�һ�ַ���
char *CIni::ReadText(int p)
{
	char chr;
	char *Ret;
	int n=p, m=0;

	int LineNum = GotoNextLine(p) - p + 1;
	Ret=new char[LineNum];
	memset(Ret, 0, LineNum);

	for(int i=0; i<m_lDataLen-p; i++)
	{
		chr = m_strData[n];

		//����
		if( chr == ';' || chr == '\r' || chr == '\t' || chr == ']' )
		{
			//ShowMessage(Ret);
			return Ret;
		}
		
		Ret[m]=chr;
		m++;
		n++;
	}

	return Ret;
}

//����һ������
bool CIni::AddIndex(char *index)
{
	char str[256];
	memset(str, 0, 256);
	int n=FindIndex(index);

	if( n == -1 )	//�½�����
	{
		sprintf(str,"\r\n[%s]\r\n",index);
		m_strData = (char *)realloc(m_strData, m_lDataLen+strlen(str));	//���·����ڴ�
		sprintf(&m_strData[m_lDataLen], "%s", str);
		m_lDataLen+=strlen(str);

		InitIndex();
		return true;
	}
	
	return false;	//�Ѿ�����
}

//�ڵ�ǰλ�ü���һ������
bool CIni::AddData(int p, char *name, char *string)
{
	char *str;
	int len=strlen(string);
	str=new char[len+256];
	memset(str, 0, len+256);
	sprintf(str,"%s=%s\r\n",name,string);
	len=strlen(str);

	p=GotoNextLine(p);	//����
	m_strData = (char *)realloc(m_strData, m_lDataLen+len);	//���·����ڴ�

	char *temp=new char[m_lDataLen-p];
	memcpy(temp, &m_strData[p], m_lDataLen-p);
	memcpy(&m_strData[p+len], temp, m_lDataLen-p);	//�Ѻ���İᵽĩβ
	memcpy(&m_strData[p], str, len);
	m_lDataLen+=len;

	SAFE_DELETE( temp );
	SAFE_DELETE( str );
	return true;
}

//�ڵ�ǰλ���޸�һ�����ݵ�ֵ
bool CIni::ModityData(int p, char *name, char *string)
{
	int n=FindData(p, name);

	char *t=ReadText(n);
	p=n+strlen(t);
	if( strlen(t)>0 ) free(t);

	int newlen=strlen(string);
	int oldlen=p-n;

	m_strData = (char *)realloc(m_strData, m_lDataLen+newlen-oldlen);	//���·����ڴ�

	char *temp=new char[m_lDataLen-p];
	memcpy(temp, &m_strData[p], m_lDataLen-p);
	memcpy(&m_strData[n+newlen], temp, m_lDataLen-p);			//�Ѻ���İᵽĩβ
	memcpy(&m_strData[n], string, newlen);
	m_lDataLen+=newlen-oldlen;

	SAFE_DELETE( temp );
	return true;
}

//��ָ���ƶ�����INDEX�����һ��
int CIni::GotoLastLine(char *index)
{
	int n=FindIndex(index);
	n=GotoNextLine(n);
	while(1)
	{
		if( m_strData[n] == '\r' || m_strData[n] == EOF || m_strData[n] == -3 || m_strData[n] == ' ' || m_strData[n] == '/' || m_strData[n] == '\t' || m_strData[n] == '\n' )
		{
			return n;
		}
		else
		{
			n=GotoNextLine(n);
			if( n >= m_lDataLen ) return n;
		}
	}
}

/////////////////////////////////////////////////////////////////////
// ����ӿ�
/////////////////////////////////////////////////////////////////////

//����ͨ��ʽ��һ�ַ�������
char *CIni::ReadText(char *index, char *name)
{
	int n=FindIndex(index);
	assert( n != -1 );

	int m=FindData(n, name);
	assert( m != -1 );

	return ReadText(m);
}
	
//��ָ�����ж�һ�ַ���
char *CIni::ReadText(char *index, int lines)
{
	int n=FindIndex(index);
	assert( n != -1 );

	//����ָ������
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	//������
	while( n<=m_lDataLen )
	{
		if( m_strData[n] == '=' )
		{
			n++;
			return ReadText(n);
		}
		if( m_strData[n] == '\r' )
		{
			return "";
		}
		n++;
	}

	return "";
}

//����ͨ��ʽ��һ����
int CIni::ReadInt(char *index, char *name)
{
	int n=FindIndex(index);
	assert( n != -1 );

	int m=FindData(n, name);
	assert( m != -1 );

	char *str=ReadText(m);
	int ret=atoi(str);
	free(str);
	return ret;
}

//��ָ�����ж�һ����
int CIni::ReadInt(char *index, int lines)
{
	int n=FindIndex(index);
	assert( n != -1 );

	//����ָ������
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	//������
	while( n<m_lDataLen )
	{
		if( m_strData[n] == '=' )
		{
			n++;
			char *str=ReadText(n);
			int ret=atoi(str);
			free(str);
			return ret;
		}
		if( m_strData[n] == '\r' )
		{
			return ERROR_DATA;
		}
		n++;
	}

	return ERROR_DATA;
}

//��ָ�����ж�һ��������
char *CIni::ReadCaption(char *index, int lines)
{
	int n=FindIndex(index);
	assert( n != -1 );

	//����ָ������
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	return ReadDataName(n);
}

//����ͨ��ʽдһ�ַ�������
bool CIni::Write(char *index, char *name, char *string)
{
	int n=FindIndex(index);
	if( n == -1 )	//�½�����
	{
		AddIndex(index);
		n=FindIndex(index);
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	int m=FindData(n, name);
	if( m==-1 )		//�½�����
	{
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	ModityData(n, name, string);	//�޸�һ������

	return true;
}

//����ͨ��ʽдһ����
bool CIni::Write(char *index, char *name, int num)
{
	char string[32];
	sprintf(string, "%d", num);

	int n=FindIndex(index);
	if( n == -1 )	//�½�����
	{
		AddIndex(index);
		n=FindIndex(index);
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	int m=FindData(n, name);
	if( m==-1 )		//�½�����
	{
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	ModityData(n, name, string);	//�޸�һ������

	return true;
}

//��������������
int CIni::GetContinueDataNum(char *index)
{
	int num=0;
	int n=FindIndex(index);
	n=GotoNextLine(n);
	while(1)
	{
		if( m_strData[n] == '\r' || m_strData[n] == EOF || m_strData[n] == -3 || m_strData[n] == ' ' || m_strData[n] == '/' || m_strData[n] == '\t' || m_strData[n] == '\n' )
		{
			return num;
		}
		else
		{
			num++;
			n=GotoNextLine(n);
			if( n >= m_lDataLen ) return num;
		}
	}
}
