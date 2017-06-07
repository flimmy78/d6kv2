#include "scadaproc.h"
#include "procthread.h"
#include "prot_SACJD.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					SACJD ΢������������					*/
				//////////////////////////////////////////////////////////////



SACJD_Protect::SACJD_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *SACJD_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
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
SACJD_PROTECTACTION	*SACJD_Protect::GetActionPara(short actioncode,short actiontype)
{
	ACTION_KEY_SACJD key;
	key.moduletype = actiontype;
	key.actioncode = actioncode;
	return (SACJD_PROTECTACTION *)table.RdbGetRecordAddr(SACJD_PROTACTION_TABNAME,&key);
}

//ȡ�������Ϣ����
SACJD_PROTECTACTION	*SACJD_Protect::GetDetectPara(short actioncode,short actiontype)
{
	ACTION_KEY_SACJD key;
	key.moduletype = actiontype;
	key.actioncode = actioncode;
	return (SACJD_PROTECTACTION *)table.RdbGetRecordAddr(SACJD_PROTDETECT_TABNAME,&key);
}


//����FEP����������������
int SACJD_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;
	NetDbg dbg;
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�յ�SACJD�ı����¼��ʼ�");

	for (int i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}

	address1=m_Buf[1];	
	
	//��վ��ַ	��Ӧ΢���������������������еõ�ַ1
	mdlp = GetModulePara(terminalno,protocoltype,address1);			//΢����������
	
	//΢�����������¼�
	char text[60];
	sprintf(text,"�ն˺�:%d\t��Լ����:%d\t��ַ:%d\n",terminalno,protocoltype,address1);
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
	if(mdlp == NULL)return PROT_ERR_MODULE;
	if (m_Buf[14] == 0x27)
	{
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��������SACJD�����¼�");
	make_action_apevent(&sys_clock,mdlp,m_Buf);
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣��SACJD�����¼�");
	ret = save_action_apevent(&sys_clock,mdlp,m_Buf);
	}
	else if (m_Buf[14] == 0x28)
	{
		ret = save_detect_apevent(&sys_clock,mdlp,m_Buf);
	}

	return ret;
}

//���ɱ�������Ӧ���¼�
void SACJD_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	SACJD_PROTECTACTION *action;
	short	actioncode,actiontype;
	SYS_CLOCK  sys_clock;
	int second,ms,minute,hour;

	if(mdlp == NULL )return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));

	actioncode=event[16];	//�������

	ms = event[11]+event[12]*256;
	second = event[10];
	minute = event[9];
	hour = event[8];
	//GetCurrentClock(&sys_clock);
	sys_clock.year = event[4]+event[5]*100;
	sys_clock.month = event[6];
	sys_clock.day = event[7];
	sys_clock.hour =hour;
	sys_clock.minute =minute;
	sys_clock.second =event[10];
	sys_clock.msecond =ms;

	msg.type_value = (int)actioncode;
	make_common_apentinfo(&msg,&sys_clock,mdlp,0,PROTTYPE_SACJD);	//������Ϣ
	actiontype = event[15];
	if((action = GetActionPara(actioncode,actiontype))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = action->sgflag;
	strcpy(msg.enttypedescribe,action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));

}


//�����������
int SACJD_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	SACJD_PROTECTACTION *action;
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
	if(mdlp == NULL )return 0;
	

	actioncode=event[16];	//�������

	ms = event[11]+event[12]*256;
	minute = event[9];
	hour = event[8];
	//GetCurrentClock(&sys_clock);
	sys_clock.year = event[4]+event[5]*100;
	sys_clock.month = event[6];
	sys_clock.day = event[7];
	sys_clock.hour =hour;
	sys_clock.minute =minute;
	sys_clock.second =event[10];
	sys_clock.msecond =ms;

	actiontype = event[15];//�豸����

	/////////���ɴ�������//////////////////////////////////////////////////
	tmpevent.evt.state = actioncode;	//�������
	tmpevent.evt.state = (int)actioncode;
	make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 0);	//������Ϣ
	if((action = GetActionPara(actioncode,actiontype))==NULL)return PROT_ERR_ACTION;	//ȡ������Ϣ
	sprintf(tmpevent.evt.member1,"%d",action->actioncode);
	strcpy(tmpevent.member_des1,action->actioninfo);
	tmpevent.evt.ToneOut = action->alarmf;
	strcpy(entname,action->entname);
	
	yield_entdescribe(entname,&tmpevent);
	if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
	//2003 03
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��ʼ����SACJD��������");
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"����SACJD�����������");

	return OK;
}

//�Լ��������
int SACJD_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	SACJD_PROTECTACTION *detect;
	NetDbg dbg;
	short	actioncode,actiontype;
	SYS_CLOCK  sys_clock;
	int ms,minute,hour;
	if(mdlp == NULL )return 0;

	actioncode=event[16];	//�������

	ms = event[11]+event[12]*256;
	minute = event[9];
	hour = event[8];
	//GetCurrentClock(&sys_clock);
	sys_clock.year = event[4]+event[5]*100;
	sys_clock.month = event[6];
	sys_clock.day = event[7];
	sys_clock.hour =hour;
	sys_clock.minute =minute;
	sys_clock.second =event[10];
	sys_clock.msecond =ms;


	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;


	actiontype = event[15];//�豸����
	if((detect = GetDetectPara(actioncode,actiontype))==NULL)
		return PROT_ERR_DETECT;

	sprintf(tmpevent.evt.member1,"%d",detect->actioncode);
	strcpy(tmpevent.member_des1,detect->actioninfo);

	yield_entdescribe(detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(detect->entname,&tmpevent);

	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��ʼ�����Լ�����");
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�����Լ��������");

	return OK;
}
