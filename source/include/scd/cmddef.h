#ifndef  _CMDDEF_H
#define  _CMDDEF_H

#include "dev.h"

#ifdef WIN32
	#if !defined(SCD_DLL)
	#define _SCD_DLLPORT _declspec(dllimport)
	#else
	#define _SCD_DLLPORT _declspec(dllexport)
	#endif
#endif


//���������������������ȶ���
#define	TABNAME_LEN		    CODE_LEN		//��������
#define	FLDNAME_LEN		    CODE_LEN		//��������

#define	ENTDESC_LEN		    128		        //��������
#define ENTNAME_LEN         CODE_LEN		//����������
#define PROCNAME_LEN        CODE_LEN		//�û�����������


#define ERRORINFO_LEN       80		        //������Ϣ����	
#define PROTINFO_LEN	    DESC_LEN		//������Ϣ����
#define PROT_UNIT_LEN		5		        //������λ����

#define HOSTNAME_LEN        HOSTNAMELEN
#define USERNAME_LEN        CODE_LEN

#define MAXSECUREGRADE      32

//#define SCADA_MAX_TERMINALNUMS   128
#define SCADA_MAX_TERMINALNUMS   5120

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


#define STATUS_CHGON          1
#define STATUS_CHGOFF         0

#define STATUS_VAL_ON         0x01
#define STATUS_VAL_OFF        0x00

#define  ACT_TO_VAL(a)   (((a)==STATUS_CHGON) ? STATUS_VAL_ON:STATUS_VAL_OFF)


////////////////////////////////////////////////////////////////
//////////////// ң�ض��м�ң�ع��̡�������� //////////////////
////////////////////////////////////////////////////////////////


//ң��ģʽ���壺
#define  YKMODE_WAIT_CHECK		0	//�ȴ���У
#define  YKMODE_AUTO_EXEC	    1	//�Զ�ִ��


//�·���ǰ�û���ң�ض����룺
#define  YKACT_OFF			0x33	// �ط�
#define  YKACT_ON			0xCC	// �غ�

//ң�����Ͷ��壺
#define  YKTYPE_YK            0		// ң��
#define  YKTYPE_UPDOWN        1		// ����

//ǰ�û���У��Ϣ����:
#define  YKCHECK_OK           1		// ң�ط�У��ȷ
#define  YKCHECK_ERR          0		// ң�ط�У����

//ң�ع����붨�壺
#define  YK_PRESET            0		// ң��Ԥ��
#define  YK_CHECKKOK          1		// ң�ط�У��ȷ
#define  YK_CHECKERR          2		// ң�ط�У����
#define	 YK_CHECKTIMEOUT	  3		// ң�ط�У��ʱ
#define  YK_CONFIRM           4		// ң��ȷ��
#define	 YK_CONFIRMTIMEOUT	  5		// ң��ȷ�Ϸ��ʱ
#define  YK_EXEC              6		// ң��ִ��
#define  YK_EXECTIMEOUT		  7		// ң��ִ�з��ʱ
#define  YK_CANCEL            8		// ң�س���
#define  YK_SUCCESS           9		// ң�سɹ�
#define  YK_REFUSE            10	// �ܶ�
#define  YK_NULL              127	// �գ��������ս�

inline int  YkSerClassify(int yksercode)
{
switch(yksercode) {
	case YK_CHECKKOK:
	case YK_PRESET:
	case YK_EXEC:
	case YK_CONFIRM:
		return(0);
	case YK_SUCCESS:
		return(1);
	default :
		return(-1);
 }
}


//Ⱥң���·���ʽ����:
#define	   GRPYKMODE_ONEBYONE	  0
#define	   GRPYKMODE_ALLATONCE	  1

//ң�ء��������·���ֵ���·���ֵ���ŵ���������ѭ�������̵������붨�壺
#define  YKCMD_NULL			      0   //��Ч
#define  YKCMD_PRESET		      1	  //Ԥ��
#define  YKCMD_EXEC               2   //ִ��
#define  YKCMD_CANCEL		      3   //����
#define  YKCMD_CONFIRM            4   //ȷ��

//ң�ء������ķ�ʽ���壺
#define  YKFANGSHI_STANDARD	      0	//��׼
#define  YKFANGSHI_UPDOWNCMD      1	//��������


//����Ա���淢��SCADA��������ң�ض����붨�壺
#define  ACTTYPE_OFF              0   //�ط�
#define  ACTTYPE_ON               1   //�غ�

//����Ա���淢��SCADA��������ң�������붨�壺
#define  ACTTYPE_DOWN             0   //��
#define  ACTTYPE_UP               1   //��
#define  ACTTYPE_PAUSE            2   //��ͣ


#define	 YK_COND_FAILRETURN	      0
#define	 YK_COND_FAILCONTINUE     1

#define  SERIALYK_CANCEL          0  // ��������
#define  SERIALYK_PAUSE           1  // ��ͣ����
#define  SERIALYK_CONTINUE        2  // ��������

//ң����չ��־
#define	 EXEC_YK_NOTCHECKSTATUS	  1	//�����״̬

// ң������
#define  YT_SENDCMD               0		// ң������
#define  YT_SUCCESS               1		// ң���ɹ�
#define  YT_FAIL                  2		// ң��ʧ��
#define  YT_NULL                  127	// ң�����̽���


///////////////////////////////////////////////////////////////
//////////////////// ��ʱ���̷�ʽ����  ֻ��˲ʱֵ/////////////////////////////
///////////////////////////////////////////////////////////////////
/*
#define  TIMER_NOSAVE			   0
#define	 TIMER_SAVE_REAL		   1
#define	 TIMER_SAVE_AVERAGE		   2
*/
/////////////////////////////////////////////////////////////////////
///////////////////// �¹��жϷ�ʽ���� //////////////////////////////
/////////////////////////////////////////////////////////////////////

#define  ACCIDENT_BY_SGZ             0	 // �¹���
#define  ACCIDENT_BY_SGZANDPROT      1	 // �¹����뱣��
#define  ACCIDENT_BY_SGZORPROT       2	 // �¹��ܻ򱣻�
#define  ACCIDENT_BY_SGZANDYC        3	 // �¹�����ң�����
#define  ACCIDENT_BY_LOGPROC		 4   // �߼�����
#define  ACCIDENT_BY_SGZANDSWITCH	 5   // �¹����뵶բ
#define  ACCIDENT_BY__NULL           127 // �����¹�

////////////////////////////////////////////////////////////////////
//////////////////// ң��������ʽ ////////////////////////////
////////////////////////////////////////////////////////////////////
//�쳣�źŲ�����
/*
#define  ODDCHG_NOPROC			0	//���쳣������ 
#define  ODDCHG_OFFALARM		1	//����ִ���
#define  ODDCHG_ONALARM			2	//	��ϴ���
*/
#define  CHGENTTYPE_NORMALCHG   1	//��������λ
#define  CHGENTTYPE_ODDCHG      2	//	�쳣��λ
#define  CHGENTTYPE_SOE         3	//	SOE

#define  ADDENTTYPE_NORMAL      0		// ����
#define  ADDENTTYPE_ODD         1		// �쳣
#define  ADDENTTYPE_SOE         2		// SOE
#define  ADDENTTYPE_SG          3		// �¹�
#define	 ADDENTTYPE_BREAKOFF    4		// ���ط֣������


//////////////////////////////////////////////////////////////////
///////////////////  ��������  ///////////////////////////////////
//////////////////////////////////////////////////////////////////

//ң��Խ�����Ͷ��壺
#define  ANAOL_NORMAL           0		//����
#define  ANAOL_HIGH             1		//Խ����
#define  ANAOL_LOW              2		//Խ����
#define  ANAOL_HIHI             3		//Խ������
#define  ANAOL_LOLO             4		//Խ������
/*
#define  ANACAL_COSYC		   1	    // ���㹦������
#define  ANACAL_SPQ			   2	    // ͨ��P,Q�������ڹ���
#define  ANACAL_SIV			   3	    // ͨ��I,V�������ڹ���
*/

#define  MANSET_REPLACE_END    0		// �˹��������
#define  MANSET_REPLACE_BEGIN  1		// �˹������ʼ
#define  AUTO_REPLACE_BEGIN    2		// �Զ������ʼ
#define  AUTO_REPLACE_ADD      3		// �Զ��������
#define  AUTO_REPLACE_END      4		// �Զ��������

#define  IS_BYREPLACE_ADD(a)	(0)  //((a)==AUTO_REPLACE_ADD)
#define  IS_BYREPLACE_EQU(a)	((a)==AUTO_REPLACE_BEGIN || (a)==MANSET_REPLACE_BEGIN)

#define	 REPLACETYPE_BYPASS	   1   // ��·���
#define  REPLACETYPE_PAIR      2   // �Զ����

#define  TERMINALSTATUS_STOP        0     // �ն�ֹͣ (����ͨ����ֹͣ)
#define  TERMINALSTATUS_MAINCHAN    1     // �ն����д���ͨ������
#define  TERMINALSTATUS_BACKCHAN    2     // �ն����дӱ�ͨ������


#define  KWHSAMPLING_16BIT     0
#define  KWHSAMPLING_19BIT     1
#define  KWHSAMPLING_20BIT     2
#define  KWHSAMPLING_24BIT     3
#define  KWHSAMPLING_32BIT     4
#define  KWHSAMPLING_DEF	   5
#define  KWHSAMPLING_TABCODE   10
#define  KWHSAMPLING_HOURCLN   11
#define  KWHSAMPLING_SAVEINC   12    //ֱ�Ӵ洢����ڵ���


#define  CONFIRMMODE_TIMER	   0
#define  CONFIRMMODE_MANSET	   1


#define  ALARM_TYPE_YCINCONSIST   1
#define  ALARM_TYPE_YXINCONSIST   2
#define  ALARM_TYPE_DEADDATA	  3




//////////////////////////////////////////////////////////////////////
//////////////////////////ʵʱ���������//////////////////////////////
//////////////////////////////////////////////////////////////////////
#define  PROGREG_TABNAME          "progreg"        //����ע���
#define  VOLTGRADE_TABNAME		  "voltgrade"      //��ѹ�ȼ���
#define  VOLTLIMIT_TABNAME		  "voltlimit"      //��ѹ��ֵ��

#define	 BREAKPROT_TABNAME		  "breakprot"	   //���ر�����
#define	 BREAKPROTMDL_TABNAME	  "breakprotmdl"   //���ر���ģ���	
#define	 BREAKREPLACE_TABNAME	  "breakreplace"   //���������	
#define	 BREAKLINKDEV_TABNAME	  "breaklinkdevice"//���������豸��	
#define	 BREAKLINKSWITCH_TABNAME  "breaklinkswitch"//�������ӵ�բ��

#define  ANALOG_TABNAME           "analog"         //ģ������
#define  TRENDDB_TABNAME          "trenddbpara"    //ʱ�����ݿ��
#define  BREAK_TABNAME			  "break"          //��������
#define  SWITCH_TABNAME           "switch"         //��բ����
#define  PROT_TABNAME		      "prot"           //��������
#define  STATUS_TABNAME           "status"         //״̬����
#define  DMSSTATUS_TABNAME        "dmsdiagnose"    //�����ϼ��״̬��

#define  MULTISTATUS_TABNAME	  "multistatus"    //��̬����
#define  KWH_TABNAME			  "kwh"            //�������
#define  CALCULATE_TABNAME		  "calculate"      //��������
#define	 YXNOSORT_TABNAME		  "yxnosort"       //ң�ź������
#define	 SUBYXNOSORT_TABNAME	  "subyxnosort"    //��ң�ź������
#define	 YCNOSORT_TABNAME		  "ycnosort"       //ң��������
#define	 KWHNOSORT_TABNAME		  "kwhnosort"       //��Ⱥ������
#define	 CHANNELNOSORT_TABNAME		  "channelnosort"       //ͨ���������
#define  STATION_TABNAME		  "station"        //��վ��
#define  CHANNEL_TABNAME		  "channel"        //ͨ����
#define  TERMINAL_TABNAME		  "terminal"       //�ն˱�
#define  APEVENT_TABNAME          "apevent"        //Ӧ�ó����¼���
#define  YKQUE_TABNAME            "ykqueue"        //ң�ض��б�
#define  YKCOND_TABNAME           "ykcondition"    //״̬����������
#define  YKPARA_TABNAME           "ykpara"         //ң�ز�����
#define  YTPARA_TABNAME           "ytpara"         //�����Ʋ�����
#define  UPDOWN_TABNAME           "updownpara"     //����������
#define  USERPROC_TABNAME         "userproc"       //�û����̱�
#define  USEROPMODE_TABNAME       "userright"      //�ڵ�Ȩ�ޱ�
#define  DISPATCHER_TABNAME       "dispatcher"     //�������ñ�
#define  HOSTSTATUS_TABNAME		  "hoststatus"     //����״̬��


#define  SDPARA_TABNAME           "sdpara"         //ʱ�β�����
#define  SDQUERY_TABNAME          "sdquery"        //ʱ�β�ѯ��
#define  DAYSDPARA_TABNAME        "daysdpara"      //��ʱ�β�����
#define  DAYSDQUERY_TABNAME       "daysdquery"     //��ʱ�β�ѯ��
#define  PROGCTRL_TABNAME         "progctrl"       //Ӧ�ó�����Ʊ�
#define  STATUSMESG_TABNAME       "statusmesg"     //״̬��Ϣ��

#ifdef GIS
#define  CARDRCD_TABNAME		  "giscardrcd"     //GIS��־�Ƽ�¼��
#else
#define  CARDRCD_TABNAME		  "cardrcd"        //��־�Ƽ�¼��
#endif

#define  NOTIFYEVTTYPE_TABNAME    "notifyevttype"  //����ȷ�����ͱ�
#define  NOTIFYEVTQUERCD_TABNAME  "notifyevtrcd"   //��ȷ�������¼��
#define  WFBSRCD_TABNAME          "wfbspara"       //�������������

#define  YCMULTISRC_TABNAME		  "ycmultisource"  //ң���Դ��
//#define  DMSFAULTTYPE_TABNAME	  "dmsflttype"     //�����ϼ��״̬��

//�¹�׷��
#define  PDRGRP_TABNAME           "pdrgroup"       //׷�����
#define  PDRPARA_TABNAME          "pdrpara"        //׷����
#define  PDRDESC_TABNAME          "pdrdescribe"
#define  PDRDATA_TABNAME          "pdrdata"   
#define  DEVSTAT_TABNAME          "�豸����ͳ�Ʊ�"

#define  ENTEXP_TABNAME           "eventexpress"      //������
#define  SCDEVENT_TABNAME         "eventqueue"        //ʵʱ�������
#define  EVENTTYPE_TABNAME        "eventtype"         //�������ͱ�
#define  EVENTSTATUSDESC_TABNAME  "eventstatusdesc"   //����״̬������
//#define  EVENTMODE_TABNAME      "eventmode"         //����ģʽ��
#define  EVENTDEALMODE_TABNAME    "eventdealmode"     //�����ʽ������

#define  SERIALYKGRP_TABNAME      "serialykgrp"       //���п������
#define  SERIALYKOBJ_TABNAME	  "serialykobj"       //���п��ƶ����
#define  SCDDEVICE_TABNAME		  "scddevice"		  //SCD�豸������ 2004 05 add
#define  SCDYCTIMELIMIT_TABNAME	  "yctimelimit"		  //ң��ʱ����ֵ��

#define  SCDTIMETYPE_HTABNAME     "ʱ�β�����"
#define  YKACT_MSGNAME            "ң�ض���"
#define  YKPROC_MSGNAME           "ң�ع���"
#define  UPDOWNACT_MSGNAME        "��������"
#define  UPDOWNPROC_MSGNAME       "��������"
#define  HOSTSTATUS_MSGNAME       "����״̬"
#define  NETSTATUS_MSGNAME        "����״̬"
#define  CHANSTATUS_MSGNAME       "ͨ��״̬"


//////////////////////////////////////////////////////////////////////
/////////////////////////ʵʱ����������///////////////////////////////
//////////////////////////////////////////////////////////////////////
#define  ANAATTR_PLNAME           "bypassname"
#define  ANAATTR_PLF              "bypassf"
#define  BRKATTR_PLF			  "bypassf"
#define  UPDOWNTIMES			  "updowntimes"
#define  YKTIMES			 	  "yktimes"

#define  SWATTR_LINENAME          "linename"

#define  COMNATTR_NAME			  "name"
#define  COMNATTR_SUBSTNNAME	  "substation"
#define  COMNATTR_LINENAME		  "devname"
#define  COMNATTR_VALUE			  "value"
#define  COMNATTR_YKTAKEF		  "yktakef"
#define  COMNATTR_DESCRIBE		  "describe"
#define  COMNATTR_SECUREGRADE	  "securegrade"
#define  COMNATTR_VOLTGRADE		  "voltgrade"

#define  YKATTR_NAME              "name"
#define  UPDOWNATTR_NAME          "name"

#define  SCDPROC_USERNAME         "scadaproc"



///////////////////////////////////////////////////////////////////////
///////////////////  SCADA�ʼ������붨�� ///////////////////////////////////
///////////////////////////////////////////////////////////////////////

#define SCADA_TYPE_NULL			    0
#define SCADA_TYPE_MANSET			1		//�˹�����
#define SCADA_TYPE_YK				2		//YK
#define SCADA_TYPE_UPDOWN			3		//����
#define SCADA_TYPE_STUDY		 	4		//�о�
#define SCADA_TYPE_YKCHK			5		//YK��У
#define SCADA_TYPE_YKRESULT		    6		//YK���
#define SCADA_TYPE_UPDOWNRESULT     7		//�������
#define SCADA_TYPE_FOREYKCHK		8		//ǰ�û�YK��У
#define SCADA_TYPE_SETSPECIALF   	9		//�ñ�־��

#define SCADA_TYPE_LINEREPLACE      10		//��·���
#define SCADA_TYPE_CLRCHGFLAG       11      //ң�Ŷ�λ
#define SCADA_TYPE_MASKCHGENT		12		//������λ����
#define SCADA_TYPE_CLRALARMFLAG     13      //�徯ʾ��־
#define SCADA_TYPE_YT				14		//������

#define SCADA_TYPE_CLRNOTIFYQUE   	15		//��֪ͨ�������
#define SCADA_TYPE_WFBS   	        16		//�������


#define SCADA_TYPE_PROT             20		//�����¼�	
#define SCADA_TYPE_PROTEVENT        21 		//���������
#define SCADA_TYPE_PROTDETECT       22		//�����Լ챨��

#define SCADA_TYPE_RAWEVENT	    	30		//������
#define SCADA_TYPE_OLEVENT	    	31		//ң��Խ��������


#define SCADA_TYPE_STARTPDR         90		 //�����¹�׷��

#define	SCADA_TYPE_FTUFAULTRPT		91	     //FTU���ϱ���	
#define SCADA_TYPE_CALLFAULTRPT		92	     //mmi�ٻ�����¼������	
#define	SCADA_TYPE_FOREFAULTRPT		93   	 //ǰ�û�����SCADA�������Ĺ���¼������
#define SCADA_TYPE_FAULTRCD			94	     //SCADA����������MMI�Ĺ���¼����� 

#define SCADA_TYPE_SERIALYK			100      // Ԥ�������п���
#define SCADA_TYPE_GROUPYK			101      // ��·Ⱥ��
#define SCADA_TYPE_SERIALYKCTRL		102		 // �������� 
#define SCADA_TYPE_GROUPYKCTRL		103		 // ���ƶ�·Ⱥ�� 
#define SCADA_TYPE_SERIALYKMSG		104		 // ���п��Ʒ�����Ϣ 

#define SCADA_TYPE_CCFORMULA		110		

#define SCADA_TYPE_NETYCDATA		111		
#define SCADA_TYPE_NETYXDATA		112		
#define SCADA_TYPE_NETKWHDATA		113	


#define SCADA_TYPE_TONGQICMD		150	    //����Ա�·���ͬ���������
#define SCADA_TYPE_TONGQIANS		151     //ǰ�û�����SCADA��������ͬ��Ӧ��
#define SCADA_TYPE_TONGQIRES		152     //SCADA��������������Ա��ͬ�ڽ��Ӧ��
#define SCADA_TYPE_ERRRET			255     //��������

#define MAXSTRINGVALLEN				256
#ifdef QTGRAPH
#define MAXAPENTMSG_LEN				256*8  
#else
#define MAXAPENTMSG_LEN				256*2  
#endif

///////////////////////////////////////////////////////////////////
///////////  Ӧ���¼����Ͷ��弰����  //////////////////////////////
///////////////////////////////////////////////////////////////////

#define  APEVENT_NULL               0	    // ���¼�
#define  APEVENT_NOEVENT            1	    // ���¼�����Ҫ�������ÿ���ʱ��������
#define  APEVENT_ALL                2	    // ȫ���¼�

#define  APEVENT_OVERLIMIT          20		// Խ��
#define  APEVENT_MSTATUSCHG         21		// ��̬���仯
#define  APEVENT_STACHG             22		// ״̬���仯	
#define  APEVENT_STASOE             23		// ״̬��SOE
#define  APEVENT_YK                 24		// ң��
#define  APEVENT_UPDOWN             25		// ����
#define  APEVENT_YKCMD              26		// ң������
#define  APEVENT_BREAK              27		// ����	
#define  APEVENT_DBUPD              28		// ���ݿ����޸�
#define  APEVENT_REQLOADTAB         29		// ����װ�����ݿ��
#define  APEVENT_LOADTABOK          30		// װ�����ݿ��ɹ�
#define  APEVENT_MANSET             31		// �˹�����
#define	 APEVENT_SETSPECIALF	    32		// �������־
#define  APEVENT_FLDCHG             33		// ���ݿ���仯
#define  APEVENT_FORECHANNELENT     34		// ǰ�û�ͨ���¼�
#define	 APEVENT_PROT       	    35		// �����¼�
#define  APEVENT_REPLACE		    36		// ��·���
#define  APEVENT_DASDIAGNOSE	    37		// �������¼�
#define	 APEVENT_STARTPDR		    38		// ����׷��
#define	 APEVENT_FTUFAULT           39		// FTU����
#define	 APEVENT_CCFORMULA	        40	    // ���������ʽ��

#define  APEVENT_PROT_DETECT        41      // �����Լ�
#define  APEVENT_PROT_TROUBLE       42      // ��������
#define  APEVENT_YT			        43      // YT

#define  APEVENT_WFBS               44		// ���

//������Լ���Ͷ���
#define PROTTYPE_LFP                0       //����LFP����
#define PROTTYPE_ISA                1       //����ISA����
#define PROTTYPE_SEL                2       //SEL����
#define PROTTYPE_DFP                3       //DFP����
#define PROTTYPE_LSA_P              4       //LSA_P����
#define PROTTYPE_SBC                5       //SBC����
#define PROTTYPE_DF3003             6       //����DF3003����
#define PROTTYPE_FWB		     	7       //����FWB����
#define PROTTYPE_CAN2000	    	8       //CAN2000����
#define	PROTTYPE_TIGER			    9	    //Tiger����
#define	PROTTYPE_XJS99			    10      //��̱���
#define PROTTYPE_WBX			    11	    //WBX����
#define PROTTYPE_DFP500             12      //DFP500����
#define PROTTYPE_DSA_301		    13	    //LSA_P1.0 ��Լ��չ
#define PROTTYPE_ISA300			    14	    //ISA v3.0 ���������(025)3429900-2562
#define PROTTYPE_CSC2000		    15	    //�ķ�����
#define PROTTYPE_HUANENG		    16	    //���ܱ��� 0731-8906888-8312
#define PROTTYPE_ZH				    18      //ZH����	  
#define PROTTYPE_FBZ			    19		//FBZ���� --tel:13505171372 ��
#define PROTTYPE_DWK			    20		//DWK����
#define PROTTYPE_LFP3			    21		//����LFP3.0����
#define PROTTYPE_KGNSA			    22		//���Ժ����������
#define PROTTYPE_HNSD			    23		//����˼�ﱣ��
#define PROTTYPE_XAKY			    24		//������Ԫ
#define PROTTYPE_PA100			    25		//�Ͼ���̩��
#define PROTTYPE_DFP_301		    26		//����DFP_301����
#define PROTTYPE_ZDB			    27		//ZDB
#define PROTTYPE_DF3003_NEW         28      //�¶���DF3003����
#define PROTTYPE_RCS		        29      //����RCS����
#define PROTTYPE_PS					30      //����PS6000����
#define PROTTYPE_DEP				31		//����DEP����
#define PROTTYPE_XWJK				32		//��̱���
#define PROTTYPE_NEWZDB				33		//�����е��±���
#define	PROTTYPE_NSR				34		//����NSR
#define	PROTTYPE_NEPCAN				35		//����CAN
#define	PROTTYPE_IEC103				36		//��׼103
#define	PROTTYPE_SACDW				37		//���Ե���
#define	PROTTYPE_KT90				38		//KT90
#define	PROTTYPE_QHZG				39		//�廪�Ϲ�
#define	PROTTYPE_SACJD				40		//���Ի���
#define	PROTTYPE_FORTYONE			41		//41�ű�����Լ
#define	PROTTYPE_FORTYTWO			42		//42�ű�����Լ
#define	PROTTYPE_FORTYTHREE			43		//43�ű�����Լ
#define	PROTTYPE_FORTYFOUR			44		//44�ű�����Լ
#define	PROTTYPE_FORTYFIVE			45		//45�ű�����Լ
#define	PROTTYPE_FORTYSIX			46		//46�ű�����Լ
#define	PROTTYPE_FORTYSEVEN			47		//47�ű�����Լ
#define	PROTTYPE_FORTYEIGHT			48		//48�ű�����Լ
#define	PROTTYPE_FORTYNINE			49		//49�ű�����Լ
#define	PROTTYPE_FIFTYZERO			50		//50�ű�����Լ
#define	PROTTYPE_FIFTYONE			51		//51�ű�����Լ
#define	PROTTYPE_FIFTYTWO			52		//52�ű�����Լ
#define	PROTTYPE_FIFTYTHREE			53		//53�ű�����Լ
#define	PROTTYPE_FIFTYFOUR			54		//54�ű�����Լ
#define	PROTTYPE_FIFTYFIVE			55		//55�ű�����Լ
#define	PROTTYPE_FIFTYSIX			56		//56�ű�����Լ
#define	PROTTYPE_FIFTYSEVEN			57		//57�ű�����Լ
#define	PROTTYPE_FIFTYEIGHT			58		//58�ű�����Լ
#define	PROTTYPE_FIFTYNINE			59		//59�ű�����Լ
#define	PROTTYPE_SIXTYZERO			60		//60�ű�����Լ

/////////////////////////////////////////////////////////////////////
//////////////////  ϵͳ������ ////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef  OK
#define  OK      0
#endif

// interface layer error
#define  SCADA_ERR_OPENMEMLIB        -200
#define  SCADA_ERR_OPENTAB           -201
#define  SCADA_ERR_NOFLD             -202
#define  SCADA_ERR_DBADDR            -203
#define  SCADA_ERR_NORCD             -204
#define  SCADA_ERR_DELRCD            -207
#define  SCADA_ERR_ADDRCD            -208
#define  SCADA_ERR_NOTAB             -209
#define  SCADA_ERR_SQLSELECT         -210
#define  SCADA_ERR_SQLWHERE          -211
#define  SCADA_ERR_UPDRCD            -212
#define  SCADA_ERR_MEMDIC            -213
#define  SCADA_ERR_MALLOC            -214
#define  SCADA_ERR_GETRCD            -215

// system layer error

#define  SCADA_ERR_PARA              -250
#define  SCADA_ERR_NOOBJECT          -251   // no this object
#define  SCADA_ERR_SETVALINVALID     -252
#define  SCADA_ERR_QUEUE             -253
#define  SCADA_ERR_YKCMD             -254
#define  SCADA_ERR_NORIGHT           -255
#define  SCADA_ERR_SAMESTUDY         -256
#define  SCADA_ERR_STUDENTFULL       -257
#define  SCADA_ERR_YKCOND            -258
#define  SCADA_ERR_YKACTION          -259
#define  SCADA_ERR_YKPROC            -260
#define  SCADA_ERR_YKOTHEROP         -261
#define  SCADA_ERR_YKCONFIRM         -262
#define	 SCADA_ERR_YKQUEUEFULL	     -263
#define	 SCADA_ERR_OPERATION		 -264
#define  SCADA_ERR_NOFAULTFTU		 -265   // û�ж�Ӧ��FTU
#define  SCADA_ERR_NOFAULTLINE	     -266   // �޹�����·
#define  SCADA_ERR_NOFAULTRCD		 -267   // FTUû��¼������
#define  SCADA_ERR_UPDOWNCOND		 -268
#define	 SCADA_ERR_YKMONITOR		 -269
#define  SCADA_ERR_YKMANSET		     -270
#define  SCADA_ERR_NOYKCONFIRM	     -271	// ���üල
#define  SCADA_ERR_LOCKED			 -272	// ����-����ң��
#define  SCADA_ERR_NOTSHAREYK		 -273
#define  SCADA_ERR_GROUNDCOND		 -274	
#define  SCADA_ERR_GROUNDED		     -275	// �ѽӵ�
#define  SCADA_ERR_DWINVALID		 -276	// ��λֵ���ڷǷ�״̬
#define  SCADA_ERR_WFLOCKED			 -277	// �������-����ң��


/////////////////////////////////////////////////////////////////////
////////////////////// ��־�Ƽ�¼�� /////////////////////////////////
/////////////////////////////////////////////////////////////////////

//��־�����Ͷ���
#define CARDTYPE_JIANXIU		1		// ����
#define CARDTYPE_LINJIAN		2		// �ټ�
#define CARDTYPE_XIANDIAN		3		// �޵�
#define CARDTYPE_LAZHA			4		// ��բ 
#define CARDTYPE_BAODIAN		5		// ����
#define CARDTYPE_LOCK		    6		// ��ס
#define CARDTYPE_KEEPOFF	    7		// ���ַ�բ
#define CARDTYPE_KEEPON		    8		// ���ֺ�բ

#define CARDTYPE_JIEDI			10		// �ӵ�
#define CARDTYPE_GUAPAI			11		// ����
#define CARDTYPE_JUMPER			12		// ����


#define CARDTYPE_GAOYA			50		// ��ѹΣ��
#define CARDTYPE_YOUREN			51		// ���˲���
#define CARDTYPE_FAULT			52		// ȱ��
#define CARDTYPE_PROTDEVCHG		53		// �̵籣�����
#define CARDTYPE_BUSBREAK		54		// ĸ�߲��
#define CARDTYPE_KEYUSER		55		// ��Ҫ�û�
#define CARDTYPE_WORKENABLE		56		// �������
#define CARDTYPE_ZQ				57		// ����

#define CARDTYPE_DAIDIANZUOYE	60		// ������ҵ
#define CARDTYPE_LINEYOUREN		61		// ��·���˲���
#define CARDTYPE_DUZHANG		62		// ����
#define CARDTYPE_DIDIAOYK		63		// �ص�ң��
#define CARDTYPE_WARNING		65		// ����

#define CARDTYPE_MAXNO          100

#define IS_LINE_JIANXIU(a)     ((a)==CARDTYPE_JIANXIU || (a)==CARDTYPE_LINJIAN)

//�������Ͷ���

#define  CARDOBJTYPE_GRAPH			255

//////////////////////////

typedef struct {
	short  groupno;
	short  firststate;
	short  secondstate;
	short  objtype;
	char   objname[24];
	short  cardtype;
	short  cardno;
#ifdef GIS
	double startx;
	double starty;
	double width;
	double height;
#else
	int    startx;
	int    starty;
#ifndef QTGRAPH
	int    width;
	int    height;
#endif
#endif
#ifndef QTGRAPH
	short  imagetype;
	char   imagename[40];
#endif
#ifdef QTGRAPH
	int	metaid;
#endif
	char   hostname[24];
	char   username[24];
	char   cardtime[80];
#ifndef QTGRAPH
	char   bitmap[24];
	char   userdefgr[24];
#endif
#ifdef QTGRAPH
	char   graphpath[255];
	char   metapath[255];
#endif
	char   context[255];
	char	  devdescribe[40];
} CARD_RCD_STRU;//��־�Ƽ�¼��



typedef struct {
	short  groupno;
	short  objtype;
	char   objname[24];
	short  cardtype;
	short  cardno;
} CARD_RCDKEY_STRU;


//����ȷ��
typedef struct {
	short  eventsortid;
	char   eventsortdesc[24];
	short  eventtypeid;
	char   eventname[24];
} NOTIFY_EVTTYPE_STRU;//����ȷ�����ͱ�

typedef struct {
	short  eventtypeid;
} NOTIFYEVTTYPE_RCDKEY_STRU;


typedef struct {
	uchar  groupno;
	short  sort;
	short  type;
	int    ymd;
	int    hmsms;
} NOTIFYEVTQUE_RCDKEY_STRU;

//////////////////////////////////////////////////////////////////////////////
///////////////////////   �Ը������Ϸ����ж�    //////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <float.h>
inline int IS_VALID_FLOAT(float value)
{
	int ret=_fpclass((double)value);
	if(ret==_FPCLASS_NN || ret==_FPCLASS_PN ||ret==_FPCLASS_NZ || ret==_FPCLASS_PZ)
		return 1;
	return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
	int ret=_fpclass(value);
	if(ret==_FPCLASS_NN || ret==_FPCLASS_PN ||ret==_FPCLASS_NZ || ret==_FPCLASS_PZ)
		return 1;
	return 0;
}
#endif

#ifdef __unix
#ifdef __sun
#include <ieeefp.h>
inline int IS_VALID_FLOAT(float value)
{
	fpclass_t ret=fpclass((double)value);
	if(ret==FP_NZERO || ret==FP_PZERO ||ret==FP_NNORM || ret==FP_PNORM)
		return 1;
	return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
	fpclass_t ret=fpclass(value);
	if(ret==FP_NZERO || ret==FP_PZERO ||ret==FP_NNORM || ret==FP_PNORM)
		return 1;
	return 0;
}
#endif
#endif

#ifdef __unix
#ifdef __hpux
#include <math.h>
inline int IS_VALID_FLOAT(float value)
{
        int ret=fpclassify(value);
        if(ret==FP_NORMAL || ret==FP_ZERO)
                return 1;
        return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
        int  ret=fpclassify(value);
        if(ret==FP_NORMAL || ret==FP_ZERO)
                return 1;
        return 0;
}
#endif
#endif

#ifdef __unix
#ifdef __alpha
#include <math.h>
#include <fp_class.h>
inline int IS_VALID_FLOAT(float value)
{
	int ret=fp_classf(value);
	if(ret==FP_POS_ZERO || ret==FP_NEG_ZERO ||ret==FP_NEG_NORM || ret==FP_POS_NORM)
		return 1;
	return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
	int ret=fp_class(value);
	if(ret==FP_POS_ZERO || ret==FP_NEG_ZERO ||ret==FP_NEG_NORM || ret==FP_POS_NORM)
		return 1;
	return 0;
}
#endif
#endif

#ifdef __unix
#ifdef _AIX
#include <math.h>
#include <float.h>
inline int IS_VALID_FLOAT(float value)
{
	int ret=fpclassify(value);
	if(ret==FP_PLUS_ZERO || ret==FP_MINUS_ZERO ||ret==FP_PLUS_NORM || ret==FP_MINUS_NORM)
		return 1;
	return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
	int ret=fpclassify(value);
	if(ret==FP_PLUS_ZERO || ret==FP_MINUS_ZERO ||ret==FP_PLUS_NORM || ret==FP_MINUS_NORM)
		return 1;
	return 0;
}
#endif
#endif

#ifdef __linux
#include <math.h>
inline int IS_VALID_FLOAT(float value)
{
	int ret=fpclassify(value);
	if(ret==FP_NORMAL || ret==FP_ZERO)
		return 1;
	return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
	int  ret=fpclassify(value);
	if(ret==FP_NORMAL || ret==FP_ZERO)
		return 1;
	return 0;
}
#endif


////////////////////////////���ú�������begin////////////////////////////
#define  ishanzhi(a)  (((a) & 0x80) == 0x80)

#ifndef __hpux
#ifndef   MAX
#define   MAX(a,b) (((a)>(b)) ? (a):(b))
#endif

#ifndef   MIN
#define   MIN(a,b) (((a)<(b)) ? (a):(b))
#endif
#endif


#ifndef   ABS
#define   ABS(a)      (((a)<0) ? (-(a)):(a))
#define   ABSMAX(a,b) (((ABS(a))>(ABS(b))) ? (a):(b))
#define   ABSMIN(a,b) (((ABS(a))<(ABS(b))) ? (a):(b))
#endif

# define ISZERO(a)    ((a<0.000001) && (a>-0.000001))

# define FVAL5(a) (((a)-(int)(a))>=0.5) ? ((int)(a)+1):((int)(a))
////////////////////////////���ú�������end////////////////////////////

#endif
