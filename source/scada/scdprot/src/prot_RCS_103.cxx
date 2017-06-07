#include "scadaproc.h"
#include "procthread.h"
#include "prot_RCS_103.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					RCS_103 ΢������������					*/
				//////////////////////////////////////////////////////////////



RCS_103_Protect::RCS_103_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *RCS_103_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1) 
			)
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//ȡ������������
RCS_103_PROTECTACTION	*RCS_103_Protect::GetActionPara(short actiontype,short actioncode)
{
	ACTION_KEY_RCS_103 key;
	key.moduletype = actiontype;
	key.actioncode = actioncode;
	return (RCS_103_PROTECTACTION *)table.RdbGetRecordAddr(RCS_103_PROTACTION_TABNAME,&key);
}


//ȡ�Լ���Ϣ����
RCS_103_PROTECTDETECT	*RCS_103_Protect::GetDetectPara(short detecttype,short detectcode)
{
	ACTION_KEY_RCS_103 key;
	key.moduletype = detecttype;
	key.actioncode = detectcode;

	return (RCS_103_PROTECTDETECT *)table.RdbGetRecordAddr(RCS_103_PROTACTION_TABNAME,&key);
}


//ȡ�����Լ���Ϣ����
int RCS_103_Protect::GetDetectInfo(short detecttype,short detectcode,char *name,char *info)
{
	RCS_103_PROTECTDETECT *detectp = GetDetectPara(detecttype,detectcode);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}


//����FEP����������������
int RCS_103_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;
	NetDbg dbg;

	int index =0;

	for (int i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}


	m_GramLength=m_Buf[3+index];									

	address1 = m_Buf[0+index];						//��ַ1	
	address2 = m_Buf[10+index];

	if (protocoltype == PROTTYPE_FORTYNINE)
	{//����103
		address1 = m_Buf[5];
		address2 = m_Buf[9];
		index = 5;
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�յ�����103�ı����¼��ʼ�");
		mdlp = GetModulePara(terminalno,protocoltype,address1,address2);			//΢����������
		if(m_Buf[1+index] == 0x02)		//�����¼�
		{
			//΢�����������¼�
			GetProtinfoTime(m_Buf+12+index, TIMEBYBCD, &sys_clock);	//����ʱ��
			RdbDebugMesg("LFP_103�ʼ�ʱ��Ϊ %d-%d-%d %d-%d\n",sys_clock.year,sys_clock.month,sys_clock.day,sys_clock.hour,sys_clock.minute);
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�������ɶ����¼�");
			char text[60];
			if(mdlp == NULL) return PROT_ERR_MODULE;
			sprintf(text,"�ն˺�:%d\t��Լ����:%d\t",terminalno,protocoltype);
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
			make_LFPaction_apevent(&sys_clock,mdlp,m_Buf);
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣�涯���¼�");
			ret = save_LFPaction_apevent(&sys_clock,mdlp,m_Buf);

		}
		else if(m_Buf[1+index] == 0x01)	//�Լ��¼�------�쳣�¼�
		{
			GetProtinfoTime(m_Buf+8+index, TIMEBYBCD, &sys_clock);	//����ʱ��
			RdbDebugMesg("LFP_103�ʼ�ʱ��Ϊ %d-%d-%d %d-%d\n",sys_clock.year,sys_clock.month,sys_clock.day,sys_clock.hour,sys_clock.minute);
			if(mdlp == NULL) return PROT_ERR_MODULE;
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣���Լ��¼�");
			ret = save_LFPdetect_apevent(&sys_clock,mdlp,m_Buf);
		}	

		return ret;

	}

	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�յ�RCS_103�ı����¼��ʼ�");

	mdlp = GetModulePara(terminalno,protocoltype,address1,address2);			//΢����������
	

	if(m_Buf[1+index] == 0x02)		//�����¼�
	{
		//΢�����������¼�
		GetProtinfoTime(m_Buf+12+index, TIMEBYBCD, &sys_clock);	//����ʱ��
		RdbDebugMesg("RCS_103�ʼ�ʱ��Ϊ %d-%d-%d %d-%d\n",sys_clock.year,sys_clock.month,sys_clock.day,sys_clock.hour,sys_clock.minute);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�������ɶ����¼�");
		char text[60];
		if(mdlp == NULL) return PROT_ERR_MODULE;
		sprintf(text,"�ն˺�:%d\t��Լ����:%d\t",terminalno,protocoltype);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
		make_action_apevent(&sys_clock,mdlp,m_Buf);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣�涯���¼�");
		ret = save_action_apevent(&sys_clock,mdlp,m_Buf);

	}
	else if(m_Buf[1+index] == 0x01)	//�Լ��¼�------�쳣�¼�
	{
		GetProtinfoTime(m_Buf+8, TIMEBYBCD, &sys_clock);	//����ʱ��
		RdbDebugMesg("RCS_103�ʼ�ʱ��Ϊ %d-%d-%d %d-%d\n",sys_clock.year,sys_clock.month,sys_clock.day,sys_clock.hour,sys_clock.minute);
		if(mdlp == NULL) return PROT_ERR_MODULE;
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣���Լ��¼�");
		ret = save_detect_apevent(&sys_clock,mdlp,m_Buf);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//���ɱ�������Ӧ���¼�
void RCS_103_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	RCS_103_PROTECTACTION *RCS_103_action;

	if(mdlp == NULL || sysclock == NULL)return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));
	msg.type_value = event[6]	;			//��������

	if (event[7]&0x01 == 1) return;

	if((RCS_103_action = GetActionPara(mdlp->type1,msg.type_value))==NULL)return;	//ȡ������Ϣ

	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FORTYSEVEN);	//������Ϣ

	msg.sgflag = RCS_103_action->sgflag;
	strcpy(msg.enttypedescribe,RCS_103_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int RCS_103_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	RCS_103_PROTECTACTION *RCS_103_action;
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
	tmpevent.evt.state = event[6];				//��������


	if (event[7]&0x01==1) return PROT_ERR_TROUBLE;

	tmpevent.evt.val = 1;
	
	if((RCS_103_action = GetActionPara(mdlp->type1,tmpevent.evt.state))==NULL)return PROT_ERR_ACTION;	//ȡ������Ϣ

	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);	//������Ϣ
	sprintf(tmpevent.evt.member1,"%d",RCS_103_action->actioncode);

	strcpy(tmpevent.member_des1,RCS_103_action->actioninfo);
	tmpevent.evt.ToneOut = RCS_103_action->alarmf;
	strcpy(entname,RCS_103_action->entname);
			
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
int RCS_103_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	RCS_103_PROTECTDETECT *RCS_103_detect;
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

	tmpevent.evt.state = event[6];				//״ֵ̬---�Լ��¼�����

	if (event[7]&0x01==1) return PROT_ERR_TROUBLE;

	tmpevent.evt.val = 1;

	if((RCS_103_detect = GetDetectPara(mdlp->type1,tmpevent.evt.state))==NULL)return PROT_ERR_DETECT;

	sprintf(tmpevent.evt.member1,"%d",RCS_103_detect->detectcode);

	strcpy(tmpevent.member_des1,RCS_103_detect->detectinfo);

	yield_entdescribe(RCS_103_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = RCS_103_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(RCS_103_detect->entname,&tmpevent);
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


int RCS_103_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock)
{
	if(clock == NULL)return -1;

	time_t ltime;
	struct tm *curtime;
	time(&ltime);
	curtime = localtime(&ltime);
	uchar i=0;
	unsigned short ms=0;
	ms = gram_part[i] + gram_part[i+1]*256;

	clock->year=curtime->tm_year + 1900;
	clock->month=curtime->tm_mon+1;
	clock->day=curtime->tm_mday;
	clock->msecond=ms%1000;
	clock->second=(uchar)(ms/1000);
	clock->minute=gram_part[i+2]&0x3f;
	clock->hour=gram_part[i+3]&0x1f;
	return OK;
}


//ȡLFP_������Ϣ	"lfp_action"
LFP_PROTECTACTION* RCS_103_Protect::GetLFPActionPara(short moduletype,unsigned char boardnum,short actbit)
{
	LFP_ACTION_KEY key;
	key.moduletype = moduletype;
	key.flag1 = boardnum;
	key.flag2 = (unsigned char )actbit;
	return ((LFP_PROTECTACTION *)table.RdbGetRecordAddr(LFP_PROTACTION_TABNAME,&key)); 
}


//ȡLFP_�Լ���Ϣ	"lfp_detect"
LFP_PROTECTDETECT* RCS_103_Protect::GetLFPDetectPara(short moduletype,unsigned char flagtype,short chkbit)
{
	LFP_ACTION_KEY key;
	key.moduletype = moduletype;
	key.flag1 = flagtype;
	key.flag2 = (unsigned char )chkbit;
	return ((LFP_PROTECTDETECT *)table.RdbGetRecordAddr(LFP_PROTDETECT_TABNAME,&key)); 
}

//���ɱ�������Ӧ���¼�
void RCS_103_Protect::make_LFPaction_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	LFP_PROTECTACTION *lfp_action;

	if(mdlp == NULL || sysclock == NULL)return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));
	msg.type_value = event[11]	;			//��������

	if (event[12]&0x01 == 1) return;

	if((lfp_action = GetLFPActionPara(mdlp->type1,0,msg.type_value))==NULL)return;	//ȡ������Ϣ

	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FORTYNINE);	//������Ϣ

	msg.sgflag = lfp_action->sgflag;
	strcpy(msg.enttypedescribe,lfp_action->describe);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int RCS_103_Protect::save_LFPaction_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	LFP_PROTECTACTION *lfp_action;
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
	tmpevent.evt.state = event[11];				//��������


	if (event[12]&0x01==1) return PROT_ERR_TROUBLE;

	tmpevent.evt.val = 1;
	
	if((lfp_action = GetLFPActionPara(mdlp->type1,0,tmpevent.evt.state))==NULL)return PROT_ERR_ACTION;	//ȡ������Ϣ

	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);	//������Ϣ
	sprintf(tmpevent.evt.member1,"%d",lfp_action->actbit);

	strcpy(tmpevent.member_des1,lfp_action->describe);
	tmpevent.evt.ToneOut = lfp_action->alarmf;
	strcpy(entname,lfp_action->entname);
			
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
int RCS_103_Protect::save_LFPdetect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	LFP_PROTECTDETECT *lfp_detect;
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

	tmpevent.evt.state = event[11];				//״ֵ̬---�Լ��¼�����

	if (event[12]&0x01==1) return PROT_ERR_TROUBLE;

	tmpevent.evt.val = 1;

	if((lfp_detect = GetLFPDetectPara(mdlp->type1,0,tmpevent.evt.state))==NULL)return PROT_ERR_DETECT;

	sprintf(tmpevent.evt.member1,"%d",lfp_detect->chkbit);

	strcpy(tmpevent.member_des1,lfp_detect->describe);

	yield_entdescribe(lfp_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = lfp_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(lfp_detect->entname,&tmpevent);
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
