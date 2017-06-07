/*************************************************
Copyright (C),SHENZHEN SNR TECH.CO.,LTD
File name: dbdef.h
Author: zhaolong
Version: 1.0.0.0
Date: 2011.2.15
Description: ���ݿ�ӿڹ���ͷ�ļ������ݽṹ�嶨��
Others: 
Function List: 
History: 
*************************************************/

#ifndef _SQLDBAPI_H
#define _SQLDBAPI_H

//֧�ֵ����ݿ����ͣ�����ѡ��
#define _ORACLE
//#define _SYBASE
//#define _SQLSERVER
//#define _ODBC
//#define _MYSQL
//#define _DMSQL

#define DBTYPE_SYBASE		1001	//SYBASE
#define DBTYPE_ORACLE		1002	//Oracle
#define DBTYPE_SQLSERVER	1003	//SQL SERVER2000
#define DBTYPE_DB2			1004	//DB2
#define DBTYPE_MYSQL	1005//MySql
#define DBTYPE_DMSQL  1006 //DMSQL

#if defined (_SYBASE)
	#define	DBTYPE_CURRENT		DBTYPE_SYBASE	    //����������ݿ�����-------Sybase
#elif defined (_SQLSERVER)
	#define	DBTYPE_CURRENT		DBTYPE_SQLSERVER	//����������ݿ�����-------Sqlserver
#elif defined (_ODBC)
	#define	DBTYPE_CURRENT		DBTYPE_SQLSERVER	//����������ݿ�����-------Odbc
#elif defined (_ORACLE)
	#define	DBTYPE_CURRENT		DBTYPE_ORACLE    	//����������ݿ�����-------Oracle
#elif defined (_MYSQL)
	#define	DBTYPE_CURRENT		DBTYPE_MYSQL    	//����������ݿ�����-------MySql
#elif defined (_DMSQL)
	#define	DBTYPE_CURRENT		DBTYPE_DMSQL    	//����������ݿ�����-------�������ݿ�
#else
	#define	DBTYPE_CURRENT		DBTYPE_ORACLE       //Ĭ�����ݿ�����-------Oracle      
#endif

#if defined (_SYBASE)
	#ifdef WIN32		
		#pragma comment(lib,"libblk")
		#pragma comment(lib,"libcs")
		#pragma comment(lib,"libct")
	#endif
#elif defined (_ORACLE)
	#ifdef WIN32
		#pragma comment(lib,"oci")	
	#endif

#endif

#if defined (_SYBASE)
#include "cstypes.h"
#else
#define CS_RETCODE				long
#define	CS_PUBLIC
#ifdef WIN32
	typedef long int			CS_INT;      //����
#else
	typedef  int				CS_INT;		 //����
#endif
typedef unsigned char			CS_TINYINT;	 // ΢���� 
typedef short					CS_SMALLINT; // ������ 
typedef char					CS_CHAR;	 // �ַ��� 
typedef unsigned char			CS_BINARY;	 // �������� 
typedef unsigned char			CS_BIT;		 // λ���� 
typedef float					CS_REAL;	 // ʵ���� 
typedef double					CS_FLOAT;    // ������
typedef unsigned char			CS_BYTE;	 // ���ֽ� 
typedef unsigned char			CS_TEXT;	 // �ı��� 
typedef unsigned char			CS_IMAGE;	 // ͼƬ�� 
typedef unsigned char			CS_LONGCHAR; // ���ַ��� 
typedef unsigned char			CS_LONGBINARY;	// ����������
typedef long 					CS_LONG;	    // �������� 
typedef unsigned short			CS_USHORT;       
typedef  void					CS_VOID;

//CS��������ֵ
#define CS_SUCCEED				(CS_RETCODE)1
#define CS_FAIL					(CS_RETCODE)0
#define CS_MEM_ERROR			(CS_RETCODE)-1
#define	CS_PENDING				(CS_RETCODE)-2
#define	CS_QUIET				(CS_RETCODE)-3
#define	CS_BUSY					(CS_RETCODE)-4
#define	CS_INTERRUPT			(CS_RETCODE)-5
#define CS_BLK_HAS_TEXT			(CS_RETCODE)-6
#define CS_CONTINUE				(CS_RETCODE)-7
#define CS_FATAL				(CS_RETCODE)-8
#define CS_RET_HAFAILOVER		(CS_RETCODE)-9


// CS_DATAFMT ��ʽ��ֵ
#define CS_FMT_UNUSED		(CS_INT)0x0
#define CS_FMT_NULLTERM		(CS_INT)0x1
#define CS_FMT_PADNULL		(CS_INT)0x2
#define CS_FMT_PADBLANK		(CS_INT)0x4
#define CS_FMT_JUSTIFY_RT	(CS_INT)0x8
#define CS_FMT_STRIPBLANKS	(CS_INT)0x10
#define CS_FMT_SAFESTR		(CS_INT)0x20


#define CS_HIDDEN				(CS_INT)0x1
#define CS_KEY					(CS_INT)0x2
#define CS_VERSION_KEY			(CS_INT)0x4
#define CS_NODATA				(CS_INT)0x8
#define CS_UPDATABLE			(CS_INT)0x10
#define CS_CANBENULL			(CS_INT)0x20
#define CS_DESCIN				(CS_INT)0x40
#define CS_DESCOUT				(CS_INT)0x80
#define CS_INPUTVALUE			(CS_INT)0x100
#define CS_UPDATECOL			(CS_INT)0x200
#define CS_RETURN 				(CS_INT)0x400
#define CS_TIMESTAMP			(CS_INT)0x2000
#define CS_NODEFAULT			(CS_INT)0x4000
#define CS_IDENTITY				(CS_INT)0x8000


//֧�ֵ���������
#define CS_ILLEGAL_TYPE		(CS_INT)(-1)
#define CS_CHAR_TYPE		(CS_INT)0
#define CS_BINARY_TYPE		(CS_INT)1
#define CS_LONGCHAR_TYPE	(CS_INT)2
#define CS_LONGBINARY_TYPE	(CS_INT)3
#define CS_TEXT_TYPE		(CS_INT)4
#define CS_IMAGE_TYPE		(CS_INT)5
#define CS_TINYINT_TYPE		(CS_INT)6
#define CS_SMALLINT_TYPE	(CS_INT)7
#define CS_INT_TYPE			(CS_INT)8
#define CS_REAL_TYPE		(CS_INT)9
#define CS_FLOAT_TYPE		(CS_INT)10
#define CS_BIT_TYPE			(CS_INT)11
#define CS_DATETIME_TYPE	(CS_INT)12
#define CS_DATETIME4_TYPE	(CS_INT)13
#define CS_MONEY_TYPE		(CS_INT)14
#define CS_MONEY4_TYPE		(CS_INT)15
#define CS_NUMERIC_TYPE		(CS_INT)16
#define CS_DECIMAL_TYPE		(CS_INT)17
#define CS_VARCHAR_TYPE		(CS_INT)18
#define CS_VARBINARY_TYPE	(CS_INT)19
#define CS_LONG_TYPE		(CS_INT)20
#define CS_SENSITIVITY_TYPE	(CS_INT)21
#define CS_BOUNDARY_TYPE	(CS_INT)22
#define CS_VOID_TYPE		(CS_INT)23
#define CS_USHORT_TYPE		(CS_INT)24


#define	CS_MAX_NAME				(CS_INT)(128 + 4)

// #pragma pack(push)
// #pragma pack(8)

typedef struct _cs_datetime
{
	CS_INT		dtdays;		// ���� 1/1/1900 
	CS_INT		dttime;		// ����
} CS_DATETIME;

typedef struct _cs_datafmt
{
	CS_CHAR		name[CS_MAX_NAME];
	CS_INT		namelen;
	CS_INT		datatype;
	CS_INT		format;
	CS_INT		maxlength;
	CS_INT		scale;
	CS_INT		precision;
	CS_INT		status;
	CS_INT		count;
	CS_INT		usertype;
	void		*locale;
} CS_DATAFMT;

/*#pragma pack(pop)*/

#endif

#include "net/netapi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_NUM			3
#define LOGINFO_NUM			10 
#define MAX_COLNUM			96
#define MAX_STRING_LENGTH	255
#define COMPUTERNAMELEN		20
#define APPNAMELEN			20
#define MAX_CHAR_BUF		4096
#define	EX_ERROR_OUT		stderr
#define EX_STDOUT			stderr
#define	EX_CTLIB_VERSION	CS_VERSION_110
#define	EX_BUFSIZE			4096
#define MSGMAXLEN			EX_BUFSIZE
#define FUNC_ISQLLEN		4096
#define MAXTIME				1440	// ÿ�������

#define YES					TRUE
#define NO					FALSE

#define LOCK_SHARE			1
#define LOCK_EXCLUSIVE		2

#define DB_OP_W				0x01
#define DB_OP_R				0x02
#define DB_OP_WR			0x03
#define DB_OP_WR2			0x05 // opcר�ö�д����
#define DB_NEED_OPEN_FALG   100  // DB��򿪱�־

#define DB_APPNAME			"wrdapp"
#define DB_SRVNAME			"nbdb"	  // �����ݿ������,ȱʡΪ��������
#define DB_SRVNAMEB			"nbdbb"  // �����ݿ������
#define DB_NAME				"modeldb"  // ���������
#define DB_HNAME			"hisdb"   // ��ʱ�����
#define DB_USERNAME			"modeldb"  // �������û���
#define DB_HUSERNAME		"hisdb"	  // ��ʱ���û���
#define DB_PASSWORD			"supernb"// ���������
#define DB_HPASSWORD		"supernb"// ��ʷ�����

//����Ϣ
#define  TABLE_INFOMATION		"select * from %s where 0=1"

#define  DELETE_NOSNAME_DATA	"delete %s where ddate=%d and time=%d "	

#define  DELETE_DATA			"delete %s where ddate=%d and sname='%s' and time>=%d and time<=%d "

#define  INSERT_ANY_ONE_RECORDER	"insert %s values("

#define  UPDATE_ANY_ONE_RECORDER	"update %s set "

#define	 UPDATE_ANY_ONE_RECORDER_CONDITION	" where ����=\"%s\""

#define  UPDATE_RECORDERS_CONDITION			"where %s= "

#define  DELETE_ANY_ONE_RECORDER			"delete %s where ����=\"%s\""

#define  DELETE_ANY_RECORDERS				"delete %s "

#define  SELECT_ANY_ONE_RECORDER			"select * from %s where ����=\"%s\""

#define  DELETE_ONE_RECORDER_HOURDDATA		"delete from hourddata    \
                where   (id =%d) and (ddate=%d) and (time=%d) and (flag=%d)" 

#define  INSERT_ONE_RECORDER_HOURDDATA		"insert hourddata values (%d,%d,%d,%d,%d,%d,%d,%f)"

// DB��������
#define BLK_IN					0
#define BLK_OUT					1
#define ISQL_DELETE				14
#define ISQL_COMMAND_DUMP_LOG	50	
#define ISQL_COMMAND			51
#define GET_TABLE_INFO		52
#define ISQL_EXCOMMAND      53
#define INS_ONE_RECORDER	116
#define INS_RECORDERS		117
#define UPD_ONE_RECORDER	118
#define UPD_RECORDERS		119
#define DEL_ONE_RECORDER	120
#define DEL_RECORDERS		121
#define SEL_ONE_RECORDER	122
#define SEL_RECORDERS		123
#define SEL_ISQL_RECORDERS	124
#define SEL_ISQL_RESULTS	125
#define UPD_ISQL_RECORDERS	126
#define DEL_ISQL_RECORDERS	127

#define INS_MIN_HISDATA		130	
#define UPD_MIN_HISDATA		131
#define SEL_MIN_HISDATA		132
#define INS_STAT_HISDATA	140	
#define UPD_STAT_HISDATA	141
#define SEL_STAT_HISDATA	142

#define ALLTYPE				-1
#define	REALTIMEVALUE		1
#define	LMANSETVALUE		2
#define	RMANSETVALUE		3
#define DEADVALUE			4

#define EXCEPTPROJECT		50
#define PROJECT				51

				
#define	ISPULSE				99
#define	NOTPULSE			0

#define DAY_SNAME_READ		5
#define DAY_SNAME_DELETE	6

#define TIME_SNAME_READ	 	7	
#define TIME_SNAME_DELETE 	8
	
#define RECORDER_SNAME_READ	9
#define RECORDER_SNAME_DELETE	10

#define TIME_NOSNAME_READ	11
#define TIME_NOSNAME_DELETE	12

#define ISQL_READ		13
#define ISQL_DELETE		14
#define PROJECT_DATA_INSERT	15	

// ��������by sname 
#define SNAME_DATA_INSERT 16

// ��������־
#define ISQL_COMMAND_DUMP_LOG	50	

// �����У�define  func.rox=-1 is no roxnum return =0 expect roxnum
#define ISQL_COMMAND		51

//ִ��
#define ISQL_EXCOMMAND      53


#define DB_OUT_FILE			54
#define DB_IN_FILE			55
#define DB_GET_SPACE		56

#ifndef MAX
#define MAX(X,Y)	(((X) > (Y)) ? (X) : (Y))
#endif

#ifndef MIN
#define MIN(X,Y)	(((X) < (Y)) ? (X) : (Y))
#endif

//��־
typedef struct _log_infomation
{
	CS_SMALLINT		groupno;
    CS_CHAR			servername[CODE_LEN];
	//  CS_CHAR			dbname[CODE_LEN];
    CS_CHAR			appname[CODE_LEN];
    CS_CHAR			username[CODE_LEN];
    CS_CHAR			password[CODE_LEN];
    CS_CHAR			hostname[CODE_LEN];
    CS_SMALLINT 	mainflag;
	CS_SMALLINT 	gateflag;
    CS_SMALLINT 	useflag;
} LOG_INFO;

// #pragma pack(push)
// #pragma pack(8)

typedef struct _ex_column_data
{
	CS_SMALLINT	indicator;
	CS_CHAR		*value;
	CS_INT		valuelen;
} EX_COLUMN_DATA;

typedef struct _ex_column_data_sname
{
	CS_SMALLINT	indicator;
	CS_CHAR		value[CODE_LEN];
	CS_INT		valuelen;
} EX_COLUMN_DATA_SNAME;

typedef struct _column_data_cahr
{
    CS_SMALLINT     indicator;
    CS_CHAR         value[256];
    CS_INT          valuelen;
} EX_COLUMN_DATA_CHAR;

typedef struct _column_data_int
{
        CS_SMALLINT     indicator;
        CS_INT          value;
        CS_INT          valuelen;
} EX_COLUMN_DATA_INT;

typedef struct _column_data_smallint
{
        CS_SMALLINT     indicator;
        CS_SMALLINT     value;
        CS_INT          valuelen;
} EX_COLUMN_DATA_SMALLINT;

typedef struct _column_data_usmallint
{
        CS_SMALLINT     indicator;
        CS_USHORT     value;
        CS_INT          valuelen;
} EX_COLUMN_DATA_USMALLINT;

typedef struct _column_data_tinyint
{
        CS_SMALLINT     indicator;
        CS_TINYINT         value;
        CS_INT          valuelen;
} EX_COLUMN_DATA_TINYINT;

typedef struct _column_data_real
{
        CS_SMALLINT     indicator;
        CS_REAL         value;
        CS_INT          valuelen;
} EX_COLUMN_DATA_REAL;

typedef struct _column_data_float
{
        CS_SMALLINT     indicator;
        CS_FLOAT         value;
        CS_INT          valuelen;
} EX_COLUMN_DATA_FLOAT;

typedef union
{
	CS_SMALLINT		smallvalue;
	CS_USHORT		usmallvalue;
    CS_INT          intvalue;
	CS_FLOAT		floatvalue;
	CS_DATETIME		datetimevalue;
	CS_REAL         realvalue;
	CS_CHAR			charvalue[MAX_STRING_LENGTH+1];
	CS_TINYINT      tinyvalue;
}VAL_UNION;

typedef struct _column_str
{
    CS_CHAR		name[CODE_LEN];
	VAL_UNION	val;
	VAL_UNION	val_o;		//
	CS_SMALLINT	order;
	CS_TINYINT  order_d;
	CS_TINYINT	select;
	CS_TINYINT	condition;
} EX_COLUMN_STR;

//��ʷ����
typedef struct  _data
{
    CS_CHAR		name[CODE_LEN];
    CS_USHORT   date;
    CS_USHORT   time;
	CS_USHORT   ms;
    CS_TINYINT  flag;
	union
	{
		CS_TINYINT	yxdata;
        CS_REAL     ycdata;
		CS_REAL		kwhdata;
	} val;
	union
	{
		CS_TINYINT	 yxdata;
        CS_SMALLINT  ycdata;
		CS_INT		 kwhdata;
	} rawval;
    CS_USHORT     odate;
    CS_USHORT     otime;
    CS_TINYINT    oflag;
} HIS_DATA;

//��ʷ��Сң������
typedef struct  _minyc_data
{
    char				name[CODE_LEN];
    unsigned short		date;
    unsigned short		time;
    unsigned char		flag;
    float				data;
    short				raw;
} HIS_MINYC_DATA;

//��ʷ���ң������
typedef struct  _minkwh_data
{
    char				name[CODE_LEN];
    unsigned short		date;
    unsigned short		time;
    unsigned char		flag;
    double				data;
    int					raw;
} HIS_MINKWH_DATA;

//��ʷң��
typedef struct  _statyc_data
{
    char				name[CODE_LEN];
    unsigned short		date;
    unsigned short		time;
    unsigned char		flag;
    float				data;
    short				raw;
	unsigned short		odate;
    unsigned short		otime;
    unsigned char		oflag;
} HIS_STATYC_DATA;

//��ʷ���
typedef struct  _statkwh_data
{
    char				name[CODE_LEN];
    unsigned short		date;
    unsigned short		time;
    unsigned char		flag;
    double				data;
    int					raw;
	unsigned short		odate;
    unsigned short		otime;
    unsigned char		oflag;
} HIS_STATKWH_DATA;

typedef struct  _fun_his
{
	CS_CHAR			name[CODE_LEN];	
	CS_INT			condition;	 
    CS_INT			order;
    CS_SMALLINT		typegroup;	 
	CS_SMALLINT		type;		 
	CS_SMALLINT		year;		 
	CS_SMALLINT		month;	 
	CS_USHORT       date;	 
	CS_USHORT       time;	 
	CS_SMALLINT		weekday; 
	CS_SMALLINT		hour;		 
	CS_SMALLINT		ehour;		 
	CS_SMALLINT		minute;		 
	CS_SMALLINT 	seconed;	 
	CS_SMALLINT 	mseconed; 
	CS_USHORT       odate;
	CS_USHORT       otime;
	CS_INT			objgroupname[CODE_LEN];
    CS_CHAR			nodesname[CODE_LEN];
	CS_CHAR			operatersname[CODE_LEN];
	CS_SMALLINT		pulseflag;
	CS_TINYINT		flag;
} FUN_HIS;

//���ܽṹ��

typedef struct  _func_str
{
	CS_SMALLINT     func;
	CS_SMALLINT		colnum;
	CS_SMALLINT		recorderlen;
	CS_SMALLINT		distinctflag;
	CS_CHAR			dbname[64];
	CS_CHAR			table[64];
	CS_CHAR			isql_count[FUNC_ISQLLEN];
	CS_CHAR			isql[FUNC_ISQLLEN];
	FUN_HIS			his;
 	EX_COLUMN_STR	col[MAX_COLNUM];
	CS_INT     		roxnum;
	CS_INT			ret_roxnum;
	CS_INT			groupno;
	CS_SMALLINT		retcode;
} FUNC_STR;

//��ƽ̨���ܽṹ��

typedef struct  _func_str_a
{
	int16s			func;
	int16s			colnum;
	int16s			recorderlen;
	int16s			distinctflag;
	CS_CHAR			dbname[64];
	CS_CHAR			table[64];
	CS_CHAR			isql_count[FUNC_ISQLLEN];
	CS_CHAR			isql[FUNC_ISQLLEN];
	FUN_HIS			his;
	EX_COLUMN_STR	col[MAX_COLNUM];
	int32s     		roxnum;
	int32s			ret_roxnum;
	int32s			groupno;
	int16s			retcode;
} FUNC_STR_A;

typedef struct _cs_datafmt_a
{
	CS_CHAR		name[CS_MAX_NAME];
	int32s		namelen;
	int32s		datatype;
	int32s		format;
	int32s		maxlength;
	int32s		scale;
	int32s		precision;
	int32s		status;
	int32s		count;
	int32s		usertype;
	void		*locale;
} CS_DATAFMT_A;

// #pragma pack(pop)

#endif
