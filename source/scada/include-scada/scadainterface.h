/********************************************************************/
/*						scadainterface.h								*/
/*				����������Ľӿ� ϵͳ�ӿڡ�������					*/
/********************************************************************/

#ifndef  _SCADA_INTERFACE_H
#define  _SCADA_INTERFACE_H

#include "rdb/rdbapi.h"
#include "scd/scddef.h" 
#include "fes/fesscdshm.h"


int  scadaserv_init(void);


int  notice_loadtable(char *hostname,char *username,char *tabname);
int  dump_hostname(void);


#define   mailtid      pid
#define   mailtidtype  MAILPROCTYPE

//	����SCADA���������ʼ�
#if defined(WIN32)
_SCD_DLLPORT 
#endif
int  SendScdResultMail(int groupno,char *hm,char *um,uhlong tid,int resultype,void *result,int resultlen);
//	��������SCADA������ʼ�
#if defined(WIN32)
_SCD_DLLPORT 
#endif
int  RecvScdRequestMail(int mail_level,char *hm,char *um,char *pd,uhlong *tid,void *request,int *reqlen);


#if defined(WIN32)
_SCD_DLLPORT 
#endif
int SendYkResMail(int groupno,char *ohm,char *oum,uhlong otid,char *chm,char *cum,uhlong ctid,
					char *objname,char *tablename, uchar resulttype,int errcode,int updownf=0);


#if defined(WIN32)
_SCD_DLLPORT 
#endif
int send_rlycmdres_mail(char *ohm,char *oum,uhlong otid,char *chm,char *cum,uhlong ctid,
						char *objname,uchar resulttype,int errorcode);

class 
#if defined(WIN32)
  _SCD_DLLPORT
#endif
Scdservice_serv {
 protected:
   char  hostname[HOSTNAME_LEN];
   char  username[USERNAME_LEN];
   char  password[PASSWORD_LEN];
   uhlong taskid;
 public:
   Scdservice_serv(char *hm,char *um,char *pd,uhlong tid);
   // �˹�����
   int manset(SCADA_MANSET_STRU *);
   // ң������
   int ykcmd(SCADA_YK_STRU *);
   // FEP������ң�ط�У
   int fertykchk(SCADA_FOREYKCHK_STRU *);
   // ң������
   int ytcmd(SCADA_YT_STRU *);
   //
   int updowncmd(SCADA_UPDOWN_STRU *);
   //
   int SetSpecialF(SCADA_SET_SPECIALF_STRU *);
   //
   int ClrNotifyEvtQue(SCADA_CLR_NOTIFYQUE_STRU *clrnotifyquep);
    //
   int DealWfbs(SCADA_WFBS_STRU  *wfbsp);
   
   //�������Ա������Ⱥ������
	int GroupYKCmd(SCADA_GROUPYK_STRU *grpykcode, SCADA_YKOBJNAME_STRU *ykgrp,hlong tid);
	int GroupYKCtrl(SCADA_SERIALYK_CTRL_STRU *);

	//����Ԥ��������ң������
	int SerialYKCmd(SCADA_SERIALYK_STRU *srlykcode,SCADA_YKOBJNAME_STRU *,hlong tid);
	int SerialYKCtrl(SCADA_SERIALYK_CTRL_STRU *);

	//������·�������
	int Line_Replace(SCADA_LINE_REPLACE_STRU *linerepl);

	//����ң������������
	int YcOl_Event(SCADA_OVERLIMIT_STRU *ycol);

	//����׷������
	int Start_PDR(SCADA_STARTPDR_STRU *stpdr);

	//���ң�ű�λ��־
	int ClearChangeF(char *substation);

	//�����ʾ��λ��־
	int ClearAlarmF(int mode,int nums,char *name);

	int put_fertrawevent(FE_DATA_EVENT_NODE *ebuf,int fertrenums);
    int maskchgent(SCADA_MASKCHG_STRU *);

	int CCFormula(SCADA_CC_FORMULA_STRU *);

	int proc_netycdata(int nums,SCADA_NET_YCDATA_STRU *netycdatap);
	int proc_netyxdata(int nums,SCADA_NET_YXDATA_STRU *netyxdatap);
	int proc_netkwhdata(int nums,SCADA_NET_KWHDATA_STRU *netkwhdatap);
	};


#endif
