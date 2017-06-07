#ifndef  _SCD_DEF_H
#define  _SCD_DEF_H

#include "sysdef.h"
#include "sysstr.h"
#include "cmddef.h"
#include "rdb/rdbapi.h"


/***********************SCADA �ʼ��ṹ����**************************/

#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
#define scdnetbyte(type,addr)   RdbNetbyte(type,addr,RDB_DATA_FROM_HOST_TO_NET)
#define scdhostbyte(type,addr)  RdbNetbyte(type,addr,RDB_DATA_FROM_NET_TO_HOST)
#else
#define scdnetbyte(type,addr)
#define scdhostbyte(type,addr)
#endif

typedef unsigned char uchar ;
typedef unsigned short ushort;


typedef struct 
{
   short    servicetype;
   char		password[PASSWORD_LEN];
   char     reserve_char[6];
}SCADA_SRV_HEAD_STRU;


typedef struct 
{
   short    servicetype;
   char     reserve_char[6];
}SCADA_CLI_HEAD_STRU;


typedef struct 
{
   int   errorcode;
   short servicetype;
   char  errorinf[ERRORINFO_LEN];
} SCADA_ERR_STRU;

typedef struct 
{
   int   groupno;
   char  tablename[OBJECTNAME_LEN];
   char  objectname[OBJECTNAME_LEN];
   uchar setflag;
   uchar datatype;
   double  setval;
 } SCADA_MANSET_STRU;
 
typedef struct 
{
	int groupno;
	uchar  setflag;
	CARD_RCD_STRU  cardrcd;
} SCADA_SET_SPECIALF_STRU;


typedef struct {
	int groupno;
	uchar  clearf;
	SYS_EVENT_EX notifyque;
} SCADA_CLR_NOTIFYQUE_STRU;
 
typedef struct {
	int groupno;
	short no;
	uchar wfbsflag;
	uchar lockallflag;
} SCADA_WFBS_STRU;

#define  WFBS_UNLOCK   0
#define  WFBS_LOCK     1

#define  WFBS_LOCKSINGLE   0
#define  WFBS_LOCKALL      1

typedef struct
{
	int groupno;
	int	mode;
	int nums;
}  SCADA_CLRALARMF_STRU;

typedef struct 
{
   int groupno;
   char  tablename[OBJECTNAME_LEN];
   char  objectname[OBJECTNAME_LEN];
   uchar cmdcode;
   uchar actcode;
   uchar eflag;
 } SCADA_YK_STRU;
 

typedef struct 
{
   int groupno;
   char  objectname[OBJECTNAME_LEN];
   uchar cmdcode;
   float setvalue;
 } SCADA_YT_STRU;


typedef struct 
{
   int groupno;
   char  objectname[OBJECTNAME_LEN];
   uchar cmdcode;
   uchar actcode;
 } SCADA_UPDOWN_STRU;


typedef struct
{
    char  objname[OBJECTNAME_LEN];
    char  tablename[OBJECTNAME_LEN];
    char  acttype; //�Ϸ֡�����
	char  res[3];
}SCADA_YKOBJNAME_STRU;

typedef struct
{
	int  groupno;
	char groupname[OBJECTNAME_LEN];
	int  objnums;
}SCADA_GROUPYK_STRU; 

typedef struct
{
	int groupno;
	char serialname[OBJECTNAME_LEN];
	uchar  serialtype;  // 0 -- Ԥ��������   1--- ��ʱ����
	char grpykmode;  // 0--����ִ��    1--����ִ��
	int  objnums;
}SCADA_SERIALYK_STRU;

typedef struct
{
	int groupno;
	char serialname[OBJECTNAME_LEN];
	uchar  ctrlcmd;
}SCADA_SERIALYK_CTRL_STRU; 



typedef struct
{
    int groupno;
	char breakname[OBJECTNAME_LEN];
	char break_replaced[OBJECTNAME_LEN];
	char mode;
	char res[7];
}SCADA_LINE_REPLACE_STRU;

typedef struct
{
	int groupno;
	char breakname[OBJECTNAME_LEN];
	char zygroupno;
	char res[3];
}SCADA_STARTPDR_STRU;

typedef struct
{
   	short  	terminalno;             //�ն˺�
   	unsigned char protocoltype;     //��Լ����  (����������Ϣ�Ľ��ͷ���) 0-->LFP, 1-->ISA ,   2-->SEL
   	unsigned char gram_len;			//�������ĵĳ���
	unsigned char gram[256];		//��������
}SCADA_PROT_STRU;

typedef struct {
   CARD_RCD_STRU   cardrcd;	
//   char		res[2];								//����
   char    devdescribe[OBJDESCRIBE_LEN];		// �豸����
} SCADA_NEW_SPECIALF_STRU;//2004 05 add
typedef struct {
   char   name[OBJECTNAME_LEN];	
   char    describe[OBJDESCRIBE_LEN];		// ����
} SCDDEVICE_STRU;//2004 05 add

typedef struct
{
   	unsigned char groupno;    
   	unsigned char security;	
	unsigned char wav_flag;	
}RALARM_WAV;


/****************************************************************************************************/
/*****************************************SCADA_YK_MACRO DEFINE**************************************/
/****************************************************************************************************/
#define  SCADA_YK_PRESET            0		// ң��Ԥ��
#define  SCADA_YK_CHECK_OK          1		// ң�ط�У��ȷ
#define  SCADA_YK_CHECK_ERR         2		// ң�ط�У����
#define	 SCADA_YK_CHECK_TIMEOUT	    3		// ң�ط�У��ʱ
#define  SCADA_YK_COMFIRM           4		// ң��ȷ��
#define	 SCADA_YK_COMFIRM_TIMEOUT	5		// ң��ȷ�Ϸ��ʱ
#define  SCADA_YK_EXEC              6		// ң��ִ��
#define  SCADA_YK_EXEC_TIMEOUT	    7		// ң��ִ�з��ʱ
#define  SCADA_YK_CANCEL            8		// ң�س���
#define  SCADA_YK_SUCCESS           9		// ң�سɹ�
#define  SCADA_YK_EXEC_FAIL         10		// ң�ؾܶ�
#define  SCADA_YK_CMD_ERR		    126		// ң���������
#define  SCADA_YK_NULL              127		// �գ��������ս�




typedef struct {
   char  objectname[OBJECTNAME_LEN];
   uchar resulttype;
   char  operatordes[OBJDESCRIBE_LEN];
   char  confirmdes[OBJDESCRIBE_LEN];
   int   errorcode;
   char  errorinf[ERRORINFO_LEN];
 } SCADA_YKRESULT_STRU;


#define  SCADA_SERIALYK_MESG_PAUSE     0	 // ������ͣ
#define  SCADA_SERIALYK_MESG_CMD_ERR   126	 // �������
#define  SCADA_SERIALYK_MESG_END       127	 // ���н���

typedef struct {
	int   groupno;
	char  serialname[OBJECTNAME_LEN];
	uchar curstate;
	char  pauseobjname[OBJECTNAME_LEN];
    int   errorcode;
    char  errorinf[ERRORINFO_LEN];
} SCADA_SERIALYK_MESG_STRU;

//////////////////////////////////////////////////////////////////////////////////// 
 
typedef struct 
{
  int   groupno;
  int   terminalno;
  int   ykno;
  uchar chkokf;
} SCADA_FOREYKCHK_STRU;

typedef struct {
	int     groupno;
	short	Year;
	char	Month;
	char	Day;
	char	Hour;
	char	Minute;
	char	Second;
	char	reserve;
	short	Ms;
	short	reserve1;
	char	Source;
	char	type;
	short	TERMINALNo;
	union
	{
		float	YCVal;
		struct
		{
			unsigned char	YXVal;
			unsigned char	YXres;
			unsigned short	yxOrder;
		}yx;
		struct
		{
			unsigned char status;
		}chan;
	}Val;
		//��type Ϊ�������ʱ�õ����������
    char            accidenttype; //�¹�����:0--�޷����� 1-����ӵ� 2-����·
    char            accidentline; //0-���� 1-A�� 2-B�� 3-C�� 4-AB 5-BC 6-AC 7-ABC
	short			YxOrder1;
    short           FTUNo1;
	short			YxOrder2;
    short           FTUNo2;

} SCADA_RAWEVENT_STRU;

typedef struct {
	int  groupno;
	char	tabname[TABNAME_LEN];
	char	objectname[OBJECTNAME_LEN];
	uchar	olgrade;
	float	olvalue;
} SCADA_OVERLIMIT_STRU;

typedef struct
{
	int  groupno;
	char name[OBJECTNAME_LEN];
	char isstation;
	char setflag;
}SCADA_MASKCHG_STRU;


typedef struct 
{
	uchar	accidenttype;
	uchar	accidentline;
	uchar	Month;
	uchar	Day;
	uchar	Hour;
	uchar	Minute;
	uchar	Second;
	uchar	reserve;
	ushort	Ms;
	ushort	Year;
	ushort	TERMINALNo;
	ushort   FTUNo;
	ushort   LineNo;
	ushort   YXNo;
	short	ycvalue[16];
}SCADA_FTUFAULT_RPT_STRU;

typedef struct {
	unsigned char datatype; //FTU����¼�����ݵ�����
	char res[3];
	char breakname[OBJECTNAME_LEN];
}SCADA_CALL_FAULT_STRU;

typedef struct
{
	uchar	Month;
	uchar	Day;
	uchar	Hour;
	uchar	Minute;
	uchar	Second;
	uchar	reserve;
	ushort	Ms;
	ushort	Year;
	uchar  recordnum;
	uchar  res1;
}FAULT_TIMEANDNUM_STRU;

typedef struct
{
	char    breakname[OBJECTNAME_LEN];
	char    ftuname[OBJECTNAME_LEN];
	char	ycname[OBJECTNAME_LEN];
	uchar	datatype;
	uchar   reserve;
	float	ycvalue[128]; //�ĸ��ܲ�128������
}FAULT_RCD_FROM_SCADA_STRU;

typedef struct
{
	short	TERMINALNo;
	short   FTUNo;
	short   YXNo;//ң�ű��
	ushort  DataType; //D0=1 
	short	ycvalue[128]; //�ĸ��ܲ�128������
}FAULT_RCD_FROM_FORE_STRU;



typedef struct
{
	char tabname[40];
} SCADA_CC_FORMULA_STRU;



typedef struct {
	int   groupno;
	char  name[OBJECTNAME_LEN];
	float value;
	uchar dataflag;
} SCADA_NET_YCDATA_STRU;

typedef struct {
	int   groupno;
	char  name[OBJECTNAME_LEN];
	short value;
	uchar dataflag;
} SCADA_NET_YXDATA_STRU;

typedef struct {
	int   groupno;
	char  name[OBJECTNAME_LEN];
	float value;
	uchar datatype;
	uchar dataflag;
} SCADA_NET_KWHDATA_STRU;

#define  scdnetbyte_scdshead(p)  { scdnetbyte(RDB_DATATYPE_SHORT,&((p)->servicetype));}
#define  scdhostbyte_scdshead(p)  { scdhostbyte(RDB_DATATYPE_SHORT,&((p)->servicetype));}

//from after.h zxm 20140506
#define		MAILYKCMD	0
#define		MAILYKEXE	1
#define		MAILYKDEL	2
//ͨ������
/*#define		CHANUNCHECK	0	
#define		CHANSEARCH	1	//ͨ������
#define		CHANRUN		2 	//ͨ������
#define		CHANSTOP	3	//ͨ��ֹͣ
#define		CHANRATE	4	//ͨ������
#define		CHANUNSYN	5	//ͨ��ʧ��*/

//��������
#define		RTUSOE		0x1
#define		HOSTSOE		0x2
#define		STATIONEVENT    0x5
//#define		DAS_DIAGNOSE	0x15

//��̨ң������
#define		MAILYXNOTTAB	2
#define		MAILRTUPARA		10
#define		MAILCHANPARA	11
#define		MAILSCIPARA		12
#define		MAILN4FPARA		13
#define		MAILCOMPARA		14
#define		MAILDBYKPARA	15
#define		TJ_MAILSETPOINT 15	//����ѹ�趨
#define		MAILMBCHAN		20
#define     MAILSWITCHCHAN  30  //����ͨ���л�

//	#define     MAXCOLUMN	64
#define		MAILMNPCMD	107
#define		MAILYKOPERATION	100
#define		MAILYKCMD	0//zxm 20140109 
#define		MAILYKEXE	1
#define		MAILYKDEL	2
#define		MAILGROUPYKOPERATION	104
#define		MAILPROTCMD	105
#define         MAILGUAPAI  110
#define		MAILVQCCMD	120
#define 	MAILSETPOINTCMD   106
#define 	MAILSETPOINTEXE   107
#define		MAILSETPOINTDEL   108
#define		MAILYTOPERATION		109
#define     MAILGUAPAI  110
//#define		MAILMNPCMD	111
#define		MAILYXRESET		112		//�Ϻ�ң�ظ���
#define		MAILVQCCMD	120

#define		MAILWAVE		130  //����¼��
#define     MAILWAVE_DIR    131  //����¼��Ŀ¼
#define     MAILWAVE_FILE   132  //����¼���ļ�
#define     MAILWFYK   133  //����ʼ�
#define		MAILREGISTER	  135  //�Ĵ��� adtool->forcom
#define		MAILADTOOL   	  136  //forecom->adtool
#endif

#if defined (__unix)
#define         LOBYTE(w)       ( (unsigned char) (w) )
#define         HIBYTE(w)       ( (unsigned char) ( (unsigned short) (w)>>8) )
#define         LOWORD(l)       ( (unsigned short) (l) )
#define         HIWORD(l)	( (unsigned short) ( (unsigned int) (l)>>16) ) 
#endif