#include <stdio.h>
#include "scadainterface.h"
#include "net/netapi.h"
#include "scadaproc.h"
#include "procthread.h"
#include "protect.h"
#include "verapi.h"


// �� ScadaServaice �Ľ������ʼ����ظ��������Ķ���HostName, UserName, TaskID��
int  send_scadaservice_err(int groupno,char *hm,char *um,hlong tid,int servicetype,int errorcode)
{
    SCADA_ERR_STRU  scdserr;
    scdserr.servicetype=servicetype;
    scdserr.errorcode=errorcode;
    ScdPrintError(errorcode,scdserr.errorinf);	// ������������ת��Ϊ��Ӧ������Ϣ
	scdnetbyte(RDB_DATATYPE_INT,&scdserr.errorcode);
	scdnetbyte(RDB_DATATYPE_SHORT,&scdserr.servicetype);
    return(SendScdResultMail(groupno,hm,um,tid,SCADA_TYPE_ERRRET,&scdserr,sizeof(scdserr)));
}


// �����ʼ��е��������ScadaService����
int  proc_scadaservice(char *hm,char *um,char *pd,hlong tid,int servicetype,void *scdsbuf,int sbuflen)
{
	int ret;
	NetDbg dbg;
	
	Protect_Scdservice_serv  scadaservice(hm,um,pd,tid);
	RdbDebugMesg("-------start to get prot mail from fert-------\n");	
	switch(servicetype) {
		case	SCADA_TYPE_PROT:
			dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t�������ʼ�");
			ret = scadaservice.yieldprotectinfo((SCADA_PROT_STRU*)scdsbuf,sbuflen);
			if(ret == PROT_ERR_MAILLEN) RdbDebugMesg("�����ʼ����ȴ���");
			else if(ret == PROT_ERR_MODULE) RdbDebugMesg("δ�õ�����ģ����Ϣ\n");
			else if(ret == PROT_ERR_MESSAGE) RdbDebugMesg("�޸�΢��������Ϣ\n");
			else if(ret == PROT_ERR_ACTION) RdbDebugMesg("����������Ϣ����\n");
			else if(ret == PROT_ERR_DETECT) RdbDebugMesg("�����Լ���Ϣ����\n");
			else if(ret == PROT_ERR_TROUBLE)	RdbDebugMesg("����������Ϣ����\n");
			else if(ret == PROT_ERR_NOTSUPPORT)	RdbDebugMesg("��֧�ֵı�������\n");
			else if(ret == OK)			RdbDebugMesg("����΢������������Ϣ\n");
			break;
		default:
			RdbDebugMesg("δ�յ������¼��ʼ�\n");
	}
	RdbDebugMesg("-------end to get mail prot from fert-------\n");
	return(OK);
}

// ����Scada�ʼ�����Ϊ2���߳�
// ���� RecvScdRequestMail ����ȡ���ʼ�
#if defined WIN32
UINT proc_scadamail_level2(LPVOID pParam)
#else
void * proc_scadamail_level2(void *argp)
#endif
{
	hlong	scdsbuf[1000]; 
	uhlong	tid; 
	int		tmplen,servicetype;    
	char    hostname[HOSTNAME_LEN];
	char    username[USERNAME_LEN];
	char    password[PASSWORD_LEN];
	NetDbg dbg;

	for (;;) 
	{
		if ((servicetype=RecvScdRequestMail(2,hostname,username,password,&tid,(void *)scdsbuf,&tmplen))!=SCADA_TYPE_NULL) 
		{
			dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t�������յ��ʼ�������2");
			proc_scadaservice(hostname,username,password,tid,servicetype,scdsbuf,tmplen);
		}
		else sleepms(150);
	}
	return NULL;
}



/////////////////////////////

#ifdef __unix
int main(int argc,char **argv)
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("scdprot,1.51.1");
				printf("scdprot, 1.51.1");
				return 0;
			}
		}
	}
	uhlong   timecount=0;

	ProcMan  procman;

	procman.StartProc("scdprot","�����¼��������","",NULL);

	NetDbg  dbg;
	dbg.RegProc("�����¼�����");

	if (!procman.CheckParentProc())	{
		printf("scadaprot exit : procman.CheckParentProc returning 0\n");
		exit(1);
	}
	//  scada service  
	scd_pthread_create(proc_scadamail_level2,NULL,100);

    for (;;) {
		if (!procman.CheckParentProc())	{
			printf("scadaproc exit: procman.CheckParentProc returning 0\n");
			exit(1);
		}
		timecount++;
		sleepms(1,0);
    } 

	dbg.UnRegProc();
    return 0;
}
#endif

 