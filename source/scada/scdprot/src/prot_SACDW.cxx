#include "scadaproc.h"
#include "procthread.h"
#include "prot_SACDW.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					SACDW ΢������������					*/
				//////////////////////////////////////////////////////////////



SACDW_Protect::SACDW_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *SACDW_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1) )
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//ȡ�¼���Ϣ����
SACDW_PROTECTACTION	*SACDW_Protect::GetActionPara(short actioncode,short actiontype)
{
	ACTION_KEY_SACDW key;
	key.moduletype = actiontype;
	key.actioncode = actioncode;
	return (SACDW_PROTECTACTION *)table.RdbGetRecordAddr(SACDW_PROTACTION_TABNAME,&key);
}


//����FEP����������������
int SACDW_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;
	NetDbg dbg;
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�յ�SACDW�ı����¼��ʼ�");

	for (int i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}

	address1=m_Buf[0];										//��վ��ַ	��Ӧ΢���������������������еõ�ַ1
	mdlp = GetModulePara(terminalno,protocoltype,address1);			//΢����������
	
	//΢�����������¼�
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��������SACDW�����¼�");
	char text[60];
	sprintf(text,"�ն˺�:%d\t��Լ����:%d\t��ַ:%d\n",terminalno,protocoltype,address1);
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
	if(mdlp == NULL)return PROT_ERR_MODULE;
	make_action_apevent(&sys_clock,mdlp,m_Buf);
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣��SACDW�����¼�");
	ret = save_action_apevent(&sys_clock,mdlp,m_Buf);

	return ret;
}

//���ɱ�������Ӧ���¼�
void SACDW_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	SACDW_PROTECTACTION *action;
	short	actioncode,actiontype;
	SYS_CLOCK  sys_clock;
	int ms,minute,hour;

	if(mdlp == NULL )return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));

	actioncode=event[1];	//�������
	ms = event[3]+event[4]*256;
	minute = event[5];
	hour = event[6];
	GetCurrentClock(&sys_clock);
	sys_clock.hour =hour;
	sys_clock.minute =minute;
	sys_clock.second =ms/1000;
	sys_clock.msecond =ms%1000;

	msg.type_value = (int)actioncode;
	make_common_apentinfo(&msg,&sys_clock,mdlp,0,PROTTYPE_SACDW);	//������Ϣ
	actiontype = mdlp->type1;
	if((action = GetActionPara(actioncode,actiontype))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = action->sgflag;
	strcpy(msg.enttypedescribe,action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));

}


//�����������
int SACDW_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	SACDW_PROTECTACTION *action;
	short	actioncode,actiontype;
	int actionnum = 0;//��������
	SYS_CLOCK  sys_clock;
	NetDbg dbg;

	entname[0] = 0;
	if(mdlp == NULL )return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	int ms,minute,hour;
	ms = event[3]+event[4]*256;
	minute = event[5];
	hour = event[6];
	GetCurrentClock(&sys_clock);
	sys_clock.hour =hour;
	sys_clock.minute =minute;
	sys_clock.second =ms/1000;
	sys_clock.msecond =ms%1000;
	/////////���ɴ�������//////////////////////////////////////////////////
	tmpevent.evt.state = event[1];	//�������
	actioncode=event[1];	//�������
	tmpevent.evt.state = (int)actioncode;
	make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 0);	//������Ϣ
	actiontype = mdlp->type1;
	if((action = GetActionPara(actioncode,actiontype))==NULL)return PROT_ERR_ACTION;	//ȡ������Ϣ
	sprintf(tmpevent.evt.member1,"%d",action->actioncode);
	strcpy(tmpevent.member_des1,action->actioninfo);
	tmpevent.evt.ToneOut = action->alarmf;
	strcpy(entname,action->entname);
	
	yield_entdescribe(entname,&tmpevent);
	if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
	//2003 03
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��ʼ����SACDW��������");
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"����SACDW�����������");

	return OK;
}

