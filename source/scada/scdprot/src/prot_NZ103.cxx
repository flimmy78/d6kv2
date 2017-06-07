#include "scadaproc.h"
#include "procthread.h"
#include "prot_NZ103.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					NZ103 ΢������������					*/
				//////////////////////////////////////////////////////////////



NZ103_Protect::NZ103_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *NZ103_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1))
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//ȡ������������
NZ103_PROTECTACTION	*NZ103_Protect::GetActionPara(short devicetype,short actioncode)
{
	ACTION_KEY_NZ103 key;
	key.moduletype = devicetype;
	key.actioncode = actioncode;
	return (NZ103_PROTECTACTION *)table.RdbGetRecordAddr(NZ103_PROTACTION_TABNAME,&key);
}


//ȡ�Լ���Ϣ����
NZ103_PROTECTDETECT	*NZ103_Protect::GetDetectPara(short devicetype,short detectcode)
{
	ACTION_KEY_NZ103 key;
	key.moduletype = devicetype;
	key.actioncode = detectcode;
	return (NZ103_PROTECTDETECT *)table.RdbGetRecordAddr(NZ103_PROTDETECT_TABNAME,&key);
}



//ȡ�����Լ���Ϣ����
int NZ103_Protect::GetDetectInfo(short devicetype,short detecttype,char *name,char *info)
{
	NZ103_PROTECTDETECT *detectp = GetDetectPara(devicetype,detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}


//����FEP����������������
int NZ103_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;
	NetDbg dbg;
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�յ�NZ103�ı����¼��ʼ�");

	for (int i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}

	m_GramLength=m_Buf[3];									
	GetProtinfoTime(m_Buf, TIMEBYBCD, &sys_clock);	//����ʱ��
	RdbDebugMesg("NZ103�ʼ�ʱ��Ϊ %d-%d-%d %d-%d\n",sys_clock.year,sys_clock.month,sys_clock.day,sys_clock.hour,sys_clock.minute);

	address1=1;						//��ַ1	

	mdlp = GetModulePara(terminalno,protocoltype,address1);			//΢����������
	

	if(m_Buf[0] == 0x04)		//�����¼�
	{
		//΢�����������¼�
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�������ɶ����¼�");
		char text[60];
		if(mdlp == NULL) return PROT_ERR_MODULE;
		sprintf(text,"�ն˺�:%d\t��Լ����:%d\t",terminalno,protocoltype);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
		make_action_apevent(&sys_clock,mdlp,m_Buf);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣�涯���¼�");
		ret = save_action_apevent(&sys_clock,mdlp,m_Buf);

	}
	else if(m_Buf[0] == 0x05)	//�Լ��¼�------�쳣�¼�
	{
		if(mdlp == NULL) return PROT_ERR_MODULE;
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣���Լ��¼�");
		ret = save_detect_apevent(&sys_clock,mdlp,m_Buf);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//���ɱ�������Ӧ���¼�
void NZ103_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	NZ103_PROTECTACTION *NZ103_action;

	if(mdlp == NULL || sysclock == NULL)return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));
	msg.type_value = event[3]	;			//��������
	if((NZ103_action = GetActionPara(mdlp->type1,msg.type_value))==NULL)return;	//ȡ������Ϣ

	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FORTYFIVE);	//������Ϣ

	msg.sgflag = NZ103_action->sgflag;
	strcpy(msg.enttypedescribe,NZ103_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));

}


//�����������
int NZ103_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	NZ103_PROTECTACTION *NZ103_action;
	NetDbg dbg;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	//make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	/////////���ɴ�������//////////////////////////////////////////////////
	tmpevent.evt.state = event[3];		//��������
	tmpevent.evt.val = event[4];		//
	
	if((NZ103_action = GetActionPara(mdlp->type1,tmpevent.evt.state))==NULL)return PROT_ERR_ACTION;	//ȡ������Ϣ

	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);	//������Ϣ
	sprintf(tmpevent.evt.member1,"%d",NZ103_action->actioncode);

	strcpy(tmpevent.member_des1,NZ103_action->actioninfo);
	tmpevent.evt.ToneOut = NZ103_action->alarmf;
	strcpy(entname,NZ103_action->entname);
			

	yield_entdescribe(entname,&tmpevent);
	if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
	//2003 03
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��ʼ���涯������");
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���涯���������");

	return OK;
}

//�Լ��������
int NZ103_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	NZ103_PROTECTDETECT *NZ103_detect;
	NetDbg dbg;
	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	tmpevent.evt.state = event[3];				//״ֵ̬---�Լ��¼�����

	tmpevent.evt.val = event[4];

	if((NZ103_detect = GetDetectPara(mdlp->type1,tmpevent.evt.state))==NULL)return PROT_ERR_DETECT;

	sprintf(tmpevent.evt.member1,"%d",NZ103_detect->detectcode);

	strcpy(tmpevent.member_des1,NZ103_detect->detectinfo);

	yield_entdescribe(NZ103_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = NZ103_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(NZ103_detect->entname,&tmpevent);
		//2003 03
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��ʼ�����Լ�����");
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���������Լ�����");

	return OK;
}


int NZ103_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock)
{
	if(clock == NULL)return -1;

	uchar i=5;
	clock->year=gram_part[i++]+1900;
	clock->month=gram_part[i++];
	clock->day=gram_part[i++];
	clock->hour=gram_part[i++];
	clock->minute=gram_part[i++];
	clock->second=gram_part[i++];
	clock->msecond=gram_part[i]+gram_part[i+1]*256;
	return OK;
}