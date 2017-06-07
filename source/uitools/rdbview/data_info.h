/**
@file data_info.h
@brief ���ݽṹ

@author chenkai
@version 1.2.0
@date 2016-11-1
*/

#ifndef DATAINFO_H
#define DATAINFO_H

#include "sysstr.h"
#include "rdb/rdbdef.h"

#define LONG_CHAR_DESCRIBE		64
#define CHAR_DESCRIBE						40
#define CHAR_NAME								24
#define FIELDMAXNUM_REALTAB	128
#define MAX_TERMINALE_NUM		10000
#define MAX_RTDB_TABLE_NUM		100
#define ROOT_NUM								4

typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef unsigned short USHORT;

typedef struct TERMINAL_STRU
{
    short	terminalno ;															//�ն����
	char		termname[ CHAR_NAME ] ;								//�ն˵Ĵ���
    char		termdescribe[ LONG_CHAR_DESCRIBE ] ;	//�ն˵�����
	short	areaid ;
    char		areaname[ LONG_CHAR_DESCRIBE ] ;
} TERMINAL ;

typedef struct STATIONAREAPARA_STRU
{
	short	id ;															//id
	char		name[ LONG_CHAR_DESCRIBE ] ;	//name
	short	masterid ;												//masterAreaId
}STATIONAREAPARA,*PSTATIONAREAPARA ;

typedef struct RTDBTABLECOL_STRU
{
	char		tablename[ CHAR_NAME ] ;						//ʵʱ����
    char		colname[ CHAR_NAME ] ;							//����
    char		coldescribe[ CHAR_DESCRIBE ] ;				//�е�����
	BOOL	flag ;																//��ʾ��־
} RTDBTABLECOL ;

typedef struct RTDB_STRU
{
	char  tablename[ CHAR_NAME ] ;							//ʵʱ����
	char  orderfield[ CHAR_NAME ] ;							//��������
	char  headfield[ CHAR_DESCRIBE ] ;					//��ͷ����
	char  tabledescribe[CHAR_DESCRIBE] ;				//ʵʱ������
	BOOL			sortflag ;													//����������־
	RTDBTABLECOL *col ;											//�ñ���Ӧ���е���Ϣ
	int	  colnum ;															//�ñ��е�����
} RTDB ;

typedef struct REALDATA_STRU
{
    char			name[ CHAR_NAME ] ;					//����
    char			describe[ CHAR_DESCRIBE ] ;		//����
	short		terminalno ;										//�ն����
	short		no ;														//���(YC,YX,KWH)
	BOOL	mansetf ;													//�����־
	BOOL	calcultf ;													//��������־
	BOOL	inconsistf ;												//���ɱ�־
	float		slope ;													//����
	float		scale ;													//б��
	float		value ;													//ʵʱֵ(YC,YX)
	double		daykwh ;												//���̵��ֵ
} REALDATA ;

typedef struct SHOWCFG_STRU
{	
	BOOL	FreshFlag ;									//�Ƿ�ˢ��
	BOOL	ShowYcCalcult ;						//�Ƿ���ʾң������־
	BOOL	ShowYxCalcult ;						//�Ƿ���ʾң�ż����־
	BOOL	ShowYcManset ;						//�Ƿ���ʾң�������־
	BOOL	ShowYxManset ;						//�Ƿ���ʾң�������־
	BOOL	ShowYcxs ;									//�Ƿ���ʾң��ϵ��
	BOOL	ShowYcKy ;								//�Ƿ���ʾң�����
	int		FreshTime ;								//ˢ������
	BOOL	ShowKwhTableCode ;				//�����ʾ��ʽ
	BOOL OrderByStation ;						//���ն˼�����־
}SHOWCFG ;

class DataInfo
{
public:
	DataInfo();
	~DataInfo();

public:
	TERMINAL *Terminal ; //����ȡ�ն˵Ľṹ
	REALDATA *RealData ;	//ʵʱ����(YX,YC,KWH)�Ľṹ
	REALDATA *Yx_Break,*Yx_Switch,*Yx_Prot,*Yx_Status,*Yx_Dmsdiagnose ;//������ʾYX����ʱ�ṹ
	short Yx_BreakNum,Yx_SwitchNum,Yx_ProtNum,Yx_StatusNum,Yx_DmsdiagnoseNum ;//������ʾYX����ʱ����
	int FreshTime ;		//ˢ��ʱ�䣨�룩
	short RealFlag ;		//0:yc,1:yx,2:kwh 
	short RealDataNum,TerminalNum,TerminalNo ;	//��ȡ��ʵʱ����Ŀ,�ն���Ŀ,�ն����
	char Station[ CHAR_NAME ] ;								//��վ����
	BOOL DataOK,OrderByStation ;								//�Ƿ��ȡ��,�Ƿ��ɳ�վ����

public:
	short RealDataRecLen ;											//���ݼ�¼������
	short RealDataRecNum ;											//��ǰʵʱ���ݱ�����ݼ�¼��(���ݿ���м�¼������)
	short RealDataColNum ;											//��ǰʵʱ���ݱ����������
	void *pRealDataInfo ;												//ʵʱ������ͷ��Ϣ������
	void *pRealData ;														//ʵʱ�������ݻ�����
	short RtdbTableNum ;												//SCADAʵʱ��ĸ���
	RTDB *Rtdb ;																//�û����õĹ���ʵʱ��Ľṹ
};

class CField
{
public:
	short colno;			//����������
	short fieldlen;		//��ĳ���
	short fieldvalueaddr;
	BYTE type;				//���������double/float/long.....
	BYTE changeflag;	//�����Ƿ��޸�
	BYTE useflag;
	int status;			
	union	
	{
		char str[CHAR_NAME]; 
		double dblv;
		float fv;
		hlong lv;
		int iv;
		UINT uv;
		short sv;
		USHORT wv;
		BYTE bv;
		char cv;
		BOOL ov;
	}val;
	short GetColumnNo()	{return colno;}
	BYTE GetType()	{return type;}
	short GetFieldLen()	{return fieldlen;}
	short GetFieldValueAddr()	{return fieldvalueaddr;}
	BYTE GetUseFlag()	{return useflag;}
	BYTE GetChangeFlag()	{return changeflag;}
	int GetStatus()	{return status;}
	char *GetStrv()	{return val.str;}
	double GetDoublev()	{return val.dblv;}
	float GetFloatv()	{return val.fv;}
	hlong GetInt64v()	{return val.lv;}
	int GetIntv()	{return val.iv;}
	UINT GetUintv()	{return val.uv;}
	USHORT GetUshortv()	{return val.wv;}
	short GetShortv()	{return val.sv;}
	BYTE GetBytev()	{return val.bv;}
	char GetCharv()	{return val.cv;}
	BOOL GetBoolv()	{return val.ov;}

	void SetColumnNo(short cno)	{colno = cno;}
	void SetType(BYTE t)	{type = t;}
	void SetFieldLen(short len)	{fieldlen = len;};
	void SetFieldValueAddr(short addr)	{fieldvalueaddr = addr;}
	void SetUseFlag(BYTE use)	{useflag = use;}
	void SetChangeFlag(BYTE cf)	{changeflag = cf;}
	void SetStatus(int s)	{status = s;}
	void SetStrv(char *asc)	{if(asc)	strcpy(val.str,asc);}
	void SetDoublev(double dv)	{val.dblv = dv;}
	void SetFloatv(float f)	{val.fv = f;}
	void SetInt64v(hlong i64)	{val.lv = i64;}
	void SetIntv(int i)	{val.iv = i;}
	void SetUintv(UINT u)	{val.uv = u;}
	void SetUshortv(USHORT w)	{val.wv = w;}
	void SetShortv(short s)	{val.sv = s;}
	void SetBytev(BYTE b)	{val.bv = b;}
	void SetCharv(char c)	{val.cv = c;}
	void SetBoolv(BOOL o)	{val.ov = o;}
};

enum  RDBTYPE 
{
	UNKNOW,
	LOCAL,SERVER
};

#define MAXCOLUMNNUM 100
#define MAXCOLUMNNAMELEN   50
typedef struct
{
	UINT ColumnNum,OldColumnNum,RcdNum,RcdLen;
	UINT ColumnLen[MAXCOLUMNNUM];
	UINT ColumnOffset[MAXCOLUMNNUM];	
	char ColumnName[MAXCOLUMNNUM][MAXCOLUMNNAMELEN];
	char *pretinfo;  
}RetInfo ; 

#define   CONNECT_OK         0
#define   USERNAMEEMPTY     -1
#define   UNKNOW_RDBTYPE    -2

#define  OPERATION_OK   0
#define  SYNTAXERR     -1
#define  GETDICTERR    -2
#define  ALLOCMEMERR   -3
#define  INITMEMERR    -4

#define NEW_RSQLCONTENT    100L
#define NEW_DBGRIDCONTENT    101L

#define	YC	0
#define	YX	1
#define	KWH		2
#define	SCADA_REAL 3

#define	MINTYPE_BREAK			1001
#define	MINTYPE_SWITCH			1002
#define	MINTYPE_PROT				1003
#define	MINTYPE_STATUS			1004
#define	MINTYPE_DMSDIAGNOSE		1005
#define	MINTYPE_YXEND			1010

// #define  _Q2C(str) (str.toAscii().data())
#define  _Q2C(str) (str.toLocal8Bit().data())
#define _C2Q(str) (QString::fromLocal8Bit(str))

static  char* datatype_text[] = {"uchar","char","ushort","short","uint","int",
"ulong","long","float","double","string"
};

#endif