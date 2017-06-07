#include "scadaproc.h"
#include "procthread.h"
#include "prot_ISA300.h"

				//////////////////////////////////////////////////////////////
				/*					ISA300(LSA_P) ΢������������					*/
				//////////////////////////////////////////////////////////////


ISA300_Protect::ISA300_Protect(void)
{
}

PROTECTMDL *ISA300_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1) 
			&& (mdlp->address2==address2)) 
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//ȡ������������
ISA300_PROTECTACTION	*ISA300_Protect::GetActionPara(short actioncode)
{
	return (ISA300_PROTECTACTION *)table.RdbGetRecordAddr(ISA300_PROTACTION_TABNAME,&actioncode);
}

//ȡ������Ϣ����
ISA300_PROTECTTROUBLE	*ISA300_Protect::GetTroublePara(short troublecode)
{
	return (ISA300_PROTECTTROUBLE *)table.RdbGetRecordAddr(ISA300_PROTTRBL_TABNAME,&troublecode);
}

//ȡ����ֵ��Ϣ����
ISA300_PROTECTMEASURE	*ISA300_Protect::GetMeasurePara(short measurecode)
{
	return (ISA300_PROTECTMEASURE *)table.RdbGetRecordAddr(ISA300_PROTMSR_TABNAME,&measurecode);
}

//ȡ�Լ���Ϣ����
ISA300_PROTECTDETECT	*ISA300_Protect::GetDetectPara(short detectcode)
{
	return (ISA300_PROTECTDETECT *)table.RdbGetRecordAddr(ISA300_PROTDETECT_TABNAME,&detectcode);
}


//ȡ������������
int ISA300_Protect::GetActionInfo(short actiontype,char *name,char *info)
{
	ISA300_PROTECTACTION *actionp = GetActionPara(actiontype);
	if(actionp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,actionp->actioninfo);
	if(info) strcpy(info,actionp->actioninfo);
	return(OK);
}

//ȡ�����Լ���Ϣ����
int ISA300_Protect::GetDetectInfo(short detecttype,char *name,char *info)
{
	ISA300_PROTECTDETECT *detectp = GetDetectPara(detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}

//ȡ������������
int ISA300_Protect::GetTroubleInfo(short troubletype,char *info)
{
	ISA300_PROTECTTROUBLE *troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}


//����FEP����������������
int ISA300_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	gram[6]=gram[5];
	GetProtinfoTime(gram+6, TIMEBYBCD, &sys_clock);	//����ʱ��
	mdlp = GetModulePara(terminalno,protocoltype,gram[0],gram[3]);			//΢����������

	if(gram[4] == 0x91)		//�����¼�
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		make_action_apevent(&sys_clock,mdlp,gram+14);
		ret = save_action_apevent(&sys_clock,mdlp,gram+14);
	}
	else if(gram[4] == 0x94)	//�Լ��¼�
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		ret = save_detect_apevent(&sys_clock,mdlp,gram+14);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//���ɱ�������Ӧ���¼�
void ISA300_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	short tmpactype;
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	ISA300_PROTECTACTION *isa_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_ISA300);	//������Ϣ
	tmpactype=event[3] + event[4] * 256; //��������
	msg.type_value = tmpactype;
	msg.event_int_info[0] = event[0];	//�¼����
	msg.event_int_info[1] = event[1];	//�ö��������ۼƷ�����
	msg.event_int_info[2] = event[2];	//�¹�����
	msg.event_int_info[3] = tmpactype;	//��������
	msg.event_int_info[4] = event[5] + event[6] * 256;	//����ֵ����
	msg.event_float_info[0] = (float)(event[7]+event[8]*256)/100.0f;
	
	if((isa_action = GetActionPara(msg.type_value))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = isa_action->sgflag;
	strcpy(msg.enttypedescribe,isa_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int ISA300_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	short tmpactype;
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN],longdes[120];
	ISA300_PROTECTACTION *isa_action;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	/////////���ɴ�������//////////////////////////////////////////////////

	tmpactype=event[3] + event[4] * 256;			//��������
	tmpevent.evt.state = tmpactype;			//��������-----״ֵ̬
	tmpevent.evt.val = (float)(event[7]+event[8]*256)/100.0f;	//����ֵ
	if((isa_action = GetActionPara(tmpactype)) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ

	sprintf(tmpevent.evt.member1,"%d",isa_action->actioncode);
	strcpy(tmpevent.member_des1,isa_action->actioninfo);
	tmpevent.evt.ToneOut = isa_action->alarmf;
	strcpy(entname,isa_action->entname);

	GetTroubleInfo(event[2], tmpevent.member_des2);	//����

	ISA300_PROTECTMEASURE *ISA300_measure = GetMeasurePara(event[5] + event[6] * 256);
	if(ISA300_measure)
	{
		sprintf(longdes,"%s:%4.2f%s",
			ISA300_measure->measureinfo,(float)tmpevent.evt.val,ISA300_measure->measureunit);
		strcat(tmpevent.longdes,longdes);
	}

	yield_entdescribe(entname,&tmpevent);
	if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
	//2003 03
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	return OK;
}

//�Լ��������
int ISA300_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	ISA300_PROTECTDETECT *isa_detect;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	tmpevent.evt.state = event[0];	//״ֵ̬---�Լ��¼�����
	if((isa_detect = GetDetectPara(event[0]))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",isa_detect->detectcode);
	strcpy(tmpevent.member_des1,isa_detect->detectinfo);

	yield_entdescribe(isa_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = isa_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(isa_detect->entname,&tmpevent);
	//2003 03
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	return OK;
}


