#ifndef   _SCDAPI_H
#define   _SCDAPI_H

#include  "net/netapi.h"
#include  "scd/scddef.h"
#include "fes/fesscdshm.h"

#define SCADA_OK       0


//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////�ڵ�Ȩ��У��begin////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////�������ñ�/////////////////////////////////

typedef  struct _DISPATCHER_STRU{
	uchar  type;                            //����Ա���
	char   typedescribe[OBJDESCRIBE_LEN];   //�������
	char   username[USERNAME_LEN];		    //�û���
	char   describe[OBJDESCRIBE_LEN];       //����
	char   password[USERNAME_LEN];		    //����
	char   groupname[USERNAME_LEN];		    //����
	uchar  ismonitor;                       //�鳤��

/*	_DISPATCHER_STRU()
	{
		memset(this, 0, sizeof(_DISPATCHER_STRU));
	}

	//���ز����� ">", ">=", "<".
	BOOL operator > (_DISPATCHER_STRU dsp) 
	{ 
		if(type>dsp.type)
			return TRUE;
		return FALSE;
	}
	BOOL operator >= (_DISPATCHER_STRU dsp) 
	{ 
		if(type>dsp.type || type == dsp.type)
			return TRUE;
		return FALSE;
	}
	BOOL operator < (_DISPATCHER_STRU dsp) 
	{ 
		if(type<dsp.type)
			return TRUE;
		return FALSE;
	}
	BOOL operator <= (_DISPATCHER_STRU dsp) 
	{ 
		if(type<dsp.type || type == dsp.type)
			return TRUE;
		return FALSE;
	}*/

}DISPATCHER_STRU;

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Dispatcher{
//		static  int  openf;
//		static  Rdb_Tab dispatchertab;
		int rcdno;
	public:
		Dispatcher(void);
		~Dispatcher(void);
		DISPATCHER_STRU  *dispatcherret;
		void GetDispatcher(void);
		int  GetDispatcherNum(void);
};

////////////////////////�ڵ�Ȩ�ޱ�////////////////////////////
typedef  struct {
	char	hostname[HOSTNAME_LEN];		//������
	char	username[USERNAME_LEN];		//�û���
	uchar   modifyjhcurve;              //�޸ļƻ�����
	uchar   modifyrealdata;             //�޸�ʵʱ����
	uchar   modifyhiscurve;             //�޸���ʷ����
	uchar   modifyhisent;               //�޸���ʷ����
	uchar   modifygraph;                //�޸�ͼ��
	uchar   modifynetpara;              //�޸��������
	uchar   modifydbpara;               //�޸����ݿ����
	uchar   modifytable;                //�޸ı���
	uchar   viewjhcurve;                //�鿴�ƻ�����
	uchar   viewrealdata;               //�鿴ʵʱ����
	uchar   viewhiscurve;               //�鿴��ʷ����
	uchar   viewhisent;                 //�鿴��ʷ����
	uchar   viewgraph;                  //�鿴ͼ��
	uchar   viewnetpara;                //�鿴�������
	uchar   viewdbpara;                 //�鿴���ݿ����
	uchar   viewtable;                  //�鿴����
	uchar   viewxbdata;                 //�鿴г������
	uchar   viewlbdata;                 //�鿴¼������
	uchar   viewdebuginfo;              //�鿴������Ϣ
	uchar   viewrawcode;                //�鿴ͨѶԭ��
	uchar   ykright;                    //ң��
	uchar   updownright;                //ң��
	uchar   superykright;               //����ң��
	uchar   stopfresh;                  //ͣ��
	uchar   putcard;                    //����
	uchar   entconfirm;                 //����ȷ��
	uchar   netchecktime;               //�����ʱ
	uchar   switchhost;                 //�л�����
	uchar   switchchannel;              //�л�ͨ��
	uchar   computerprot;               //΢������
	uchar   modifyprotvalue;            //�޸Ķ�ֵ
	uchar   protrunstop;                //����Ͷ��
	uchar   modifysetvalnum;            //�޸Ķ�ֵ����
	uchar   protreserverd1;             //΢������Ԥ��1
	uchar   protreserverd2;             //΢������Ԥ��2
	uchar   protreserverd3;             //΢������Ԥ��3
	uchar   stoprtu;                    //ͣ��RTU
	uchar   controlmnp;                 //����ģ����
	uchar   lockforedata;               //����ǰ������
	uchar   paradownload;               //��������
	uchar   paraupload;                 //��������
	uchar   guangzipai;                 //������
	uchar	opright[MAXSECUREGRADE];	//����Ȩ�ޣ�=1Ϊ����Ӧ��ȫ������Ȩ��
} USEROPERRIGHT_STRU;
  
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
UserOperRight{
		static  int  openf;
		static  Rdb_Tab useroprighttab;
		int recordno;
	public:
		UserOperRight(void);
		~UserOperRight(void);
		// �������������û����������ָ����ȫ���Ĳ���Ȩ��
		int CheckSecureGrade(USEROPERRIGHT_STRU *uor, int securegrade);//У���û���ȫ��
		int CheckSecureGrade(char *hostname, char *username, int securegrade);
//		USEROPERRIGHT_STRU  *GetUserOperRight(char *hostname, char *username);
		USEROPERRIGHT_STRU  *useroperrightret;
		void GetOperright(void);
		int GetOperrightNum(void);
		int CheckUserPwd(char *username,char *password);//У���û�����
};

//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////�ڵ�Ȩ��У�� end/////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////




int
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ScdGetDesc(int groupno,char *tabname,char *fldname,char *desc,uchar *datatype,int *len);

int
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ScdGetYkPara(char *name,int  *cmdtime);


int
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ScdGetUpdownPara(char *name);

///////////////////////////////////
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ScadaServer {
     MailBox  scadamail;
public:
     ScadaServer(void);
     ~ScadaServer(void);
	 //HMI����SCADA���������˹���������
     int  SendManSetValReq(int groupno,char *username,char *objectname,char *tablename,int setflag,uchar datatype,void *valaddr);
                 
	 //HMI����SCADA��������ң����������
	 //eflagΪ����ң�ر�־��eflag=1ʱ���ж����ң�ر�������
     int  SendYkCmdReq(int groupno,char *username,char *password,char *objectname,char *tablename,int cmdcode,int actcode,int eflag=0);
     
	 //FEP����������SCADA��������ң�ط�У��Ϣ
     int  ForeSendYkCheckMesg(int groupno,char *username,int terminalno,int ykno,int chkokf);
     
	 //FEP����������SCADA�������Ĺ�����Ϣ
     int  ForePutCard(int groupno,char *username,int terminalno,int yxno,int cardtype,int setflag);

	 //HMI����SCADA��������������������
     int  SendUpdownCmdReq(int groupno,char *username,char *password,char *objname,int cmdcode,int actcode);
 
	 //HMI����SCADA��������ң��(������)��������
     int  SendYtCmdReq(int groupno,char *username,char *password,char *objname,int cmdcode,float setval);
	 
	 //SCADA�����������ͻ�������������Ϣ
     int  ScadaReturn(char *username,void **result,int *reslen);
	
	 //HMI����SCADA��������Ⱥ������ 
	 int  SendGroupYkCmd(int groupno,char *username,char *password, char *groupname,int  objnum,  SCADA_YKOBJNAME_STRU *ykgrp);//Ⱥ��
	 int  ctrl_groupyk(int groupno,char *username,char *password, char *groupname,int ctrlcmd);

	 //HMI����SCADA������Ԥ�������п���
	 //serialtype:  0 -- Ԥ��������   1--- ��ʱ����
	 //mode:        0 -- ����ִ��     1--- ����ִ��
	 //�����ʼ�����: SCADA_TYPE_YKRESULT ��ң�ع��̵���Ϣ��,  SCADA_TYPE_SERIALYKMSG ������״̬����Ϣ��
     int  SendSerialYkCmd(int groupno,char *username,char *password, char *serialname, int serialtype,
					int mode,int objnums,SCADA_YKOBJNAME_STRU *serobjp);
	 int  ctrl_serialyk(int groupno,char *username,char *password,char *serialname, int ctrlcmd);

	 //HMI����SCADA����������·�����������  
     int  SendLineReplaceReq(int groupno,char *username, char *breakname, char *replaced_breakname,char mode);
     
	 //HMI����SCADA������������׷������
     int  SendStartPdrCmd(int groupno,char *username, char *breakname, char zygroupno);
     
	 //HMI����SCADA�����������ñ�־����
	 int  SendSetSpecialFlagReq(int groupno,char *username, char setflag,CARD_RCD_STRU *cardrcdp);

	 //HMI����SCADA��������ɾ��ȷ�������б�����
	 //clearf=1 ɾ������ȷ������; clearf=2 ���ȷ�������б�;
	 int  SendClearNotifyQueueReq(int groupno,char *username, char clearf, SYS_EVENT_EX *nqrcdp);

	 //HMI����SCADA�����������ң�ű�λ��־����	
	 // ��ڲ���:username:�û��� 
	 //  substation Ϊվ��Ĭ��Ϊϵͳר��վ,ָ����ϵͳ��ң�ű�λ��־�������
	 //  ����ֻ���һ��վ��
	 int  SendClrChgFlagReq(int groupno,char *username, char *substation =SYS_SUBST_NAME);

	 //HMI����SCADA�������������ʾ��λ��־����	
	 // ��ڲ���:
     //		username:�û��� 
	 //		mode:    0 : ��ȫϵͳ,  1 : ��������վ  2 : ��ָ������
	 //		nums:  ����mode=2ʱ����ʾ�������,���������Ч
	 //		name: ��mode=0ʱ��Ч,
	 //				mode=1ʱ��ʾվ����ϵ����,
	 //			    mode=2ʱΪ��Ŷ��������ַ�����,ÿ������ռOBJECTNAME_LEN����
	 int SendClearAlarmFReq(int groupno,char *username,int mode, int nums,char *name);

	 	
	 //FEP����������SCADA��������������
	 //int ForeSendRawEventToScd(char* username,SCADA_RAWEVENT_STRU *rawevent,int num);
       int ForeSendRawEventToScd(char* username,FE_DATA_EVENT_NODE *rawevent,int num);
	 //FEP����������SCADA��������ң��Խ��������
	 int ForeSendOverLimitEventToScd(int groupno,char* username,SCADA_OVERLIMIT_STRU* oleventp);

	 //HMI����SCADA�������ķ�����λ��������,isstation==1ʱ,nameΪstation�����֣�
	 //isstation==0ʱ��Ϊ�ض����������,setflag==1Ϊ���÷�����==0Ϊ�������
	 int SendMaskChgEventReq(int groupno,char *username,char *name,char isstation,char setf);
	 
	 //FEP����������SCADA��������FTU���ϱ���
	 int ForeSendFtuFaultRpt(int groupno,char *username, SCADA_FTUFAULT_RPT_STRU *ftureport, char num);

	 //FEP����������SCADA�������Ĺ���¼�����ݱ���
	 int ForeSendFtuFaultRcd(int groupno,char* username,FAULT_TIMEANDNUM_STRU *rcdnum_time, FAULT_RCD_FROM_FORE_STRU *fturcd);

	 //HMI����SCADA�������Ĺ���¼�������ʼ� 
	 int SendCallFaultRcdReq(int groupno,char *username, char *breakname, unsigned char  datatype);

	 //����SCADA�������ļ�����ʽ���������ʼ�
	 int UserprocSendCompileReq(int groupno,char *username, char *tabname);

	 //FEP����������SCADA��������΢��������Ϣ�ʼ�
	 int ForeSendProtInfo(int groupno,char *username, SCADA_PROT_STRU *protectp);

	 //FEP��������HMI����SCADA�������������������
	 //noΪ��������������еı�����ţ�
	 //wfbsflagΪ������־��WFBS_UNLOCK�����������WFBS_LOCK������
	 //lockallflagΪȫ������־��WFBS_LOCKSINGLE��ֻ������ң�ŵı�����WFBS_LOCKALL������wfbsflag����������������������е�ȫ��ң�ţ����������ϵͳͨѶ��ʼ�����������
	 int SendWfbsReq(int groupno, char* username, short no, unsigned char wfbsflag, unsigned char lockallf);

	 //������ȷ��
	 //tabnameΪ������ȷ�ϵı�����objnameΪ������ȷ�ϵĶ�������clearflagΪ������ȷ�ϱ�־:1,���������ȷ�ϱ�־
//	 int SendClrGzpReq(int groupno, char* username, char* tabname,char* objname, unsigned char clearflag);

	 //����SCADA��������������
	 int NetSendYcData(int groupno,char *username,char *password,int nums,SCADA_NET_YCDATA_STRU *ycdatap);
	 int NetSendYxData(int groupno,char *username,char *password,int nums,SCADA_NET_YXDATA_STRU *ycdatap);
	 int NetSendKwhData(int groupno,char *username,char *password,int nums,SCADA_NET_KWHDATA_STRU *ycdatap);
     };


#endif
