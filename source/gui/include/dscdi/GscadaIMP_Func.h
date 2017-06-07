#ifndef GSCADAIMP_FUNC_H_HEADER_INCLUDED_BB046556
#define GSCADAIMP_FUNC_H_HEADER_INCLUDED_BB046556
//################################################
//############���������˵��######################
//################################################

#include "ddef/ddes_comm.h"
//***********************************��������**************************************
#define	GSCDM_CTRLREQ		0x100
//ң��

//ң�ء��������·���ֵ���·���ֵ���ŵ���������ѭ�������̵������붨�壺
#define  GYKCMD_NULL			      0   //��Ч
#define  GYKCMD_PRESET		      1	  //Ԥ��
#define  GYKCMD_EXEC               2   //ִ��
#define  GYKCMD_CANCEL		      3   //����
#define  GYKCMD_CONFIRM            4   //ȷ��

//ң�ء������ķ�ʽ���壺
#define  GYKFANGSHI_STANDARD	      0	//��׼
#define  GYKFANGSHI_UPDOWNCMD      1	//��������


//����Ա���淢��SCADA��������ң�ض����붨�壺
#define  GACTTYPE_OFF              0   //�ط�
#define  GACTTYPE_ON               1   //�غ�

//����Ա���淢��SCADA��������ң�������붨�壺
#define  GACTTYPE_DOWN             0   //��
#define  GACTTYPE_UP               1   //��
#define  GACTTYPE_PAUSE            2   //��ͣ

typedef struct __tagGScdCTRLReqStruct
{ 
	char	username[ GDB_CODE_LEN ];
	char	password[ GDB_CODE_LEN ];
	char	objectname[ GDB_CODE_LEN ];
	char	tablename[ GDB_CODE_LEN ];
	int32	cmdcode;
	int32	actcode;
	int32	eflag;
	float	trgvalue;//����ң����㷽ʽ��Ŀ��ֵ
}GSCDCTRLREQ, *PGSCDCTRLREQ;

typedef struct 
{
   char  objectname[ GDB_CODE_LEN ];
   uchar resulttype;
   char  operatordes[ GDB_DESC_LEN ];
   char  confirmdes[ GDB_DESC_LEN ];
   int   errorcode;
   char  errorinf[ GDB_DESC_LEN*2 ];
} GSCADA_YKRESULT_STRU, *PGSCADA_YKRESULT_STRU;

//GSCDCTRLREQ scdctrl;
//success=sendCMD(GSCDCTRLREQ,(uint32)&scdctrl,0);
#define	GSCDM_YK			( GSCDM_CTRLREQ + 1 )
//ң��
//GSCDCTRLREQ scdctrl;
//success=sendCMD(GSCDM_YT,(uint32)&scdctrl,0);
#define	GSCDM_YT			( GSCDM_YK + 1 )
//***********************************����ң��**************************************

typedef struct 
{
    char  objname[GDB_CODE_LEN];
    char  tablename[GDB_CODE_LEN];
    char  acttype; //�Ϸ֡�����
	char  res[3];
}GSCADA_YKOBJNAME_STRU,*PGSCADA_YKOBJNAME_STRU;

typedef struct 
{
	int		grpno;
	char	username[ GDB_CODE_LEN ];
	char	password[ GDB_CODE_LEN ];
	char	serialname[ GDB_CODE_LEN ];
	int		serialtype,mode,objnums;
	PGSCADA_YKOBJNAME_STRU serobjp;
	int		ctrlcmd;
}GSCADA_SERIAL_YK_STRU,*PGSCADA_SERIAL_YK_STRU;
//SendSerialYkCmd
#define GSCDM_SERIAL_YK			(GSCDM_YT+1)
//ctrl_serialyk
#define GSCDM_SERIAL_YK_CTRL	(GSCDM_SERIAL_YK+1)

typedef struct 
{
	int	grpno;
	char	username[ GDB_CODE_LEN ];
	char	password[ GDB_CODE_LEN ];
	char	groupname[ GDB_CODE_LEN ];
	int	objnums;
	PGSCADA_YKOBJNAME_STRU groupobjp;
	int	ctrlcmd;
}GSCADA_GROUP_YK_STRU,*PGSCADA_GROUP_YK_STRU;
//SendGroupYkCmd
#define GSCDM_GROUP_YK			(GSCDM_SERIAL_YK_CTRL+1)
//ctrl_groupyk
#define GSCDM_GROUP_YK_CTRL	(GSCDM_GROUP_YK+1)

//***********************************��λ����**************************************

#define GSCDM_SETFLAG		0x200
//�˹�����
typedef struct _tagGScdManSetStruct
{
	char	objectname[ GDB_CODE_LEN ];
	char	tablename[ GDB_CODE_LEN ];
	char	username[ GDB_CODE_LEN ];
	int32	setflag;
	uchar	datatype;
	void	*valaddr;
}GSCDMANSET,*PGSCDMANSET;
//GSCDMANSET scdmanset;
//success=sendCMD(GSCDM_MANFLAG,(uint32)&scdmanset,0);
#define GSCDM_MANFLAG		( GSCDM_SETFLAG + 1 )
//ң��ͣ��
typedef struct _tagScdClrChgFlagStruct
{
	char	username[ GDB_CODE_LEN ];
	char	code[ GDB_CODE_LEN ];
}GSCDCLRCHGFLAG, *PGSCDCLRCHGFLAG;
//GSCDCLRCHGFLAG scdclr;
//success=sendCMD(GSCDM_CLRCHGFLAG,(uint32)&scdclr,0);
#define GSCDM_CLRCHGFLAG	( GSCDM_MANFLAG + 1 )
//***********************************��λ����**************************************


//***********************************���Ӽ�¼����**********************************
#define	GSCDM_ADDRCD		0x300
//�������
typedef struct _tagGScdWfbsStruct
{
	char	username[ GDB_CODE_LEN ];
	uint16	no;
	uchar	wfbsflag;
	uchar	lockallf;
}GSCDWFBS,*PGSCDWFBS;

#define  GWFBS_UNLOCK   0	//�������
#define  GWFBS_LOCK     1	//����
#define  GWFBS_LOCKSINGLE   0	//����ң�ű���
#define  GWFBS_LOCKALL      1	//ȫ����
#define	GSCDM_WFBS			( GSCDM_ADDRCD + 1 )
//GSCDWFBS	scdwfbs;
//success=sendCMD(GSCDM_WFBS,(uint32)&scdwfbs,0);

//��־�Ʋ���
typedef struct 
{
	int16  groupno;
	int16  firststate;
	int16  secondstate;
	int16  objtype;
	char   objname[24];
	int16  cardtype;
	int16  cardno;
	int32    startx;
	int32    starty;
	int32	metaid;
	char   hostname[24];
	char   username[24];
	char   cardtime[80];
	char   graphpath[255];
	char   metapath[255];
	char   context[255];
	char   devdescribe[40];	
}GCARD_RCD_STRU;//��־�Ƽ�¼��

typedef struct _tagGScdCardReqStruct
{
	GCARD_RCD_STRU	cardrcdstru;
	int				reqtype;		//0	ժ��,1 ����
}GSCDCRADREQ,*PGSCDCRADREQ;
//GSCDCRADREQ	scdcardreq;
//success=sendCMD(GSCDM_ADDCARD,(uint32)&scdcardreq,0);
#define	GSCDM_ADDCARD		( GSCDM_WFBS + 1 )

typedef struct _tagGScdLineReplaceStruct
{
	char username[ GDB_CODE_LEN ];
	char breakname[ GDB_CODE_LEN ]; 
	char replaced_breakname[ GDB_CODE_LEN ];
	char mode;
}GSCDLINEREP,*PGSCDLINEREP;

//GSCDLINEREP scdlinerep;
//success=sendCMD(GSCDM_LINEREPLACE,(uint32)&scdlinerep,0);
#define	GSCDM_LINEREPLACE	( GSCDM_ADDCARD + 1 )
//***********************************���Ӽ�¼����**********************************

//***********************************��������**************************************
#define	GSCDM_RETREQ		0x400
typedef struct _tagGScdReturnStruct
{
	void	*Result;
	int32	RetLen;
}GSCDRET,*PGSCDRET;

//GSCDRET scdret
//success=sendCMD(GSCDM_RET,(uint32)&scdret,0);
#define	GSCDM_RET			( GSCDM_RETREQ + 1 )
//***********************************��������**************************************

//***********************************�¹�׷������**************************************
#define	GSCDM_STARTPDR		0x500

typedef struct _tagGScdPDRStruct
{
	char username[ GDB_CODE_LEN ];
	char breakname[ GDB_CODE_LEN ]; 
	char zygroupno;
}GSCDPDR,*PGSCDPDR;

//***********************************SCADA�ʼ������붨��***************************


#define GSCADA_TYPE_NULL			    0
#define GSCADA_TYPE_MANSET			1		//�˹�����
#define GSCADA_TYPE_YK				2		//YK
#define GSCADA_TYPE_UPDOWN			3		//����
#define GSCADA_TYPE_STUDY		 	4		//�о�
#define GSCADA_TYPE_YKCHK			5		//YK��У
#define GSCADA_TYPE_YKRESULT		    6		//YK���
#define GSCADA_TYPE_UPDOWNRESULT     7		//�������
#define GSCADA_TYPE_FOREYKCHK		8		//ǰ�û�YK��У
#define GSCADA_TYPE_SETSPECIALF   	9		//�ñ�־��

#define GSCADA_TYPE_LINEREPLACE      10		//��·���
#define GSCADA_TYPE_CLRCHGFLAG       11      //ң�Ŷ�λ
#define GSCADA_TYPE_MASKCHGENT		12		//������λ����
#define GSCADA_TYPE_CLRALARMFLAG     13      //�徯ʾ��־
#define GSCADA_TYPE_YT				14		//������

#define GSCADA_TYPE_CLRNOTIFYQUE   	15		//��֪ͨ�������
#define GSCADA_TYPE_WFBS   	        16		//�������


#define GSCADA_TYPE_PROT             20		//�����¼�	
#define GSCADA_TYPE_PROTEVENT        21 		//���������
#define GSCADA_TYPE_PROTDETECT       22		//�����Լ챨��

#define GSCADA_TYPE_RAWEVENT	    	30		//������
#define GSCADA_TYPE_OLEVENT	    	31		//ң��Խ��������


#define GSCADA_TYPE_STARTPDR         90		 //�����¹�׷��

#define	GSCADA_TYPE_FTUFAULTRPT		91	     //FTU���ϱ���	
#define GSCADA_TYPE_CALLFAULTRPT		92	     //mmi�ٻ�����¼������	
#define	GSCADA_TYPE_FOREFAULTRPT		93   	 //ǰ�û�����GSCADA�������Ĺ���¼������
#define GSCADA_TYPE_FAULTRCD			94	     //GSCADA����������MMI�Ĺ���¼����� 

#define GSCADA_TYPE_SERIALYK			100      // Ԥ�������п���
#define GSCADA_TYPE_GROUPYK			101      // ��·Ⱥ��
#define GSCADA_TYPE_SERIALYKCTRL		102		 // �������� 
#define GSCADA_TYPE_GROUPYKCTRL		103		 // ���ƶ�·Ⱥ�� 
#define GSCADA_TYPE_SERIALYKMSG		104		 // ���п��Ʒ�����Ϣ 

#define GSCADA_TYPE_CCFORMULA		110		

#define GSCADA_TYPE_NETYCDATA		111		
#define GSCADA_TYPE_NETYXDATA		112		
#define GSCADA_TYPE_NETKWHDATA		113		


#define GSCADA_TYPE_TONGQICMD		150	    //����Ա�·���ͬ���������
#define GSCADA_TYPE_TONGQIANS		151     //ǰ�û�����GSCADA��������ͬ��Ӧ��
#define GSCADA_TYPE_TONGQIRES		152     //GSCADA��������������Ա��ͬ�ڽ��Ӧ��
#define GSCADA_TYPE_ERRRET			255     //��������
//***********************************GSCADA�ʼ������붨��***************************


//*******************************ϵͳ������*******************************


// interface layer error
#define  GSCADA_ERR_OPENMEMLIB        -200
#define  GSCADA_ERR_OPENTAB           -201
#define  GSCADA_ERR_NOFLD             -202
#define  GSCADA_ERR_DBADDR            -203
#define  GSCADA_ERR_NORCD             -204
#define  GSCADA_ERR_DELRCD            -207
#define  GSCADA_ERR_ADDRCD            -208
#define  GSCADA_ERR_NOTAB             -209
#define  GSCADA_ERR_SQLSELECT         -210
#define  GSCADA_ERR_SQLWHERE          -211
#define  GSCADA_ERR_UPDRCD            -212
#define  GSCADA_ERR_MEMDIC            -213
#define  GSCADA_ERR_MALLOC            -214
#define  GSCADA_ERR_GETRCD            -215

// system layer error

#define  GSCADA_ERR_PARA              -250
#define  GSCADA_ERR_NOOBJECT          -251   // no this object
#define  GSCADA_ERR_SETVALINVALID     -252
#define  GSCADA_ERR_QUEUE             -253
#define  GSCADA_ERR_YKCMD             -254
#define  GSCADA_ERR_NORIGHT           -255
#define  GSCADA_ERR_SAMESTUDY         -256
#define  GSCADA_ERR_STUDENTFULL       -257
#define  GSCADA_ERR_YKCOND            -258
#define  GSCADA_ERR_YKACTION          -259
#define  GSCADA_ERR_YKPROC            -260
#define  GSCADA_ERR_YKOTHEROP         -261
#define  GSCADA_ERR_YKCONFIRM         -262
#define	 GSCADA_ERR_YKQUEUEFULL	     -263
#define	 GSCADA_ERR_OPERATION		 -264
#define  GSCADA_ERR_NOFAULTFTU		 -265   // û�ж�Ӧ��FTU
#define  GSCADA_ERR_NOFAULTLINE	     -266   // �޹�����·
#define  GSCADA_ERR_NOFAULTRCD		 -267   // FTUû��¼������
#define  GSCADA_ERR_UPDOWNCOND		 -268
#define	 GSCADA_ERR_YKMONITOR		 -269
#define  GSCADA_ERR_YKMANSET		     -270
#define  GSCADA_ERR_NOYKCONFIRM	     -271	// ���üල
#define  GSCADA_ERR_LOCKED			 -272	// ����-����ң��
#define  GSCADA_ERR_NOTSHAREYK		 -273
#define  GSCADA_ERR_GROUNDCOND		 -274	
#define  GSCADA_ERR_GROUNDED		     -275	// �ѽӵ�
#define  GSCADA_ERR_DWINVALID		 -276	// ��λֵ���ڷǷ�״̬
#define  GSCADA_ERR_WFLOCKED			 -277	// �������-����ң��
//*******************************ϵͳ������*******************************

//******************************��־�Ƽ�¼��********************************

//��־�����Ͷ���
#define GCARDTYPE_JIANXIU		1		// ����
#define GCARDTYPE_LINJIAN		2		// �ټ�
#define GCARDTYPE_XIANDIAN		3		// �޵�
#define GCARDTYPE_LAZHA			4		// ��բ 
#define GCARDTYPE_BAODIAN		5		// ����
#define GCARDTYPE_SUOZHU		6		// ��ס
#define GCARDTYPE_BCFENGZHA		7		// ���ַ�բ
#define GCARDTYPE_BCHEZHA		8		// ���ֺ�բ

#define GCARDTYPE_JIEDI			10		// �ӵ�
#define GCARDTYPE_GUAPAI			11		// ����
#define GCARDTYPE_TIAOJIE			12		// ������

#define GCARDTYPE_GAOYA			50		// ��ѹΣ��
#define GCARDTYPE_YOUREN			51		// ���˲���
#define GCARDTYPE_FAULT			52		// ȱ��
#define GCARDTYPE_PROTDEVCHG		53		// �̵籣�����
#define GCARDTYPE_BUSBREAK		54		// ĸ�߲��
#define GCARDTYPE_KEYUSER		55		// ��Ҫ�û�
#define GCARDTYPE_WORKENABLE		56		// �������
#define GCARDTYPE_ZQ				57		// ����

#define GCARDTYPE_DAIDIANZUOYE	60		// ������ҵ
#define GCARDTYPE_LINEYOUREN		61		// ��·���˲���
#define GCARDTYPE_DUZHANG		62		// ����
#define GCARDTYPE_DIDIAOYK		63		// �ص�ң��

#define GCARDTYPE_WARNING		65		// ������

#define GCARDTYPE_MAXNO          100

#define  GCARDOBJTYPE_GRAPH			255

//******************************��־�Ƽ�¼��********************************

/*****************************GSCADA_YK_MACRO DEFINE**************************************/
#define  GSCADA_YK_PRESET            0		// ң��Ԥ��
#define  GSCADA_YK_CHECK_OK          1		// ң�ط�У��ȷ
#define  GSCADA_YK_CHECK_ERR         2		// ң�ط�У����
#define	 GSCADA_YK_CHECK_TIMEOUT	    3		// ң�ط�У��ʱ
#define  GSCADA_YK_COMFIRM           4		// ң��ȷ��
#define	 GSCADA_YK_COMFIRM_TIMEOUT	5		// ң��ȷ�Ϸ��ʱ
#define  GSCADA_YK_EXEC              6		// ң��ִ��
#define  GSCADA_YK_EXEC_TIMEOUT	    7		// ң��ִ�з��ʱ
#define  GSCADA_YK_CANCEL            8		// ң�س���
#define  GSCADA_YK_SUCCESS           9		// ң�سɹ�
#define  GSCADA_YK_EXEC_FAIL         10		// ң�ؾܶ�
#define  GSCADA_YK_CMD_ERR		    126		// ң���������
#define  GSCADA_YK_NULL              127		// �գ��������ս�
/*****************************GSCADA_YK_MACRO DEFINE**************************************/

#endif
