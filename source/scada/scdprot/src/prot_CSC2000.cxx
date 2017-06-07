#include "scadaproc.h"
#include "procthread.h"
#include "prot_CSC2000.h"

				//////////////////////////////////////////////////////////////
				/*					CSC2000 ΢������������					*/
				//////////////////////////////////////////////////////////////


CSC2000_Protect::CSC2000_Protect(void)
{
}

PROTECTMDL *CSC2000_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
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
CSC2000_PROTECTACTION	*CSC2000_Protect::GetActionPara(short actioncode)
{
	return (CSC2000_PROTECTACTION *)table.RdbGetRecordAddr(CSC2000_PROTACTION_TABNAME,&actioncode);
}

//ȡ������Ϣ����
CSC2000_PROTECTTROUBLE	*CSC2000_Protect::GetTroublePara(short troublecode)
{
	return (CSC2000_PROTECTTROUBLE *)table.RdbGetRecordAddr(CSC2000_PROTTRBL_TABNAME,&troublecode);
}

//ȡ����ֵ��Ϣ����
CSC2000_PROTECTMEASURE	*CSC2000_Protect::GetMeasurePara(short measurecode)
{
	return (CSC2000_PROTECTMEASURE *)table.RdbGetRecordAddr(CSC2000_PROTMSR_TABNAME,&measurecode);
}

//ȡ�Լ���Ϣ����
CSC2000_PROTECTDETECT	*CSC2000_Protect::GetDetectPara(short detectcode)
{
	return (CSC2000_PROTECTDETECT *)table.RdbGetRecordAddr(CSC2000_PROTDETECT_TABNAME,&detectcode);
}


//ȡ������������
int CSC2000_Protect::GetActionInfo(short actiontype,char *name,char *info)
{
	CSC2000_PROTECTACTION *actionp = GetActionPara(actiontype);
	if(actionp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,actionp->actioninfo);
	if(info) strcpy(info,actionp->actioninfo);
	return(OK);
}

//ȡ�����Լ���Ϣ����
int CSC2000_Protect::GetDetectInfo(short detecttype,char *name,char *info)
{
	CSC2000_PROTECTDETECT *detectp = GetDetectPara(detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}

//ȡ������������
int CSC2000_Protect::GetTroubleInfo(short troubletype,char *info)
{
	CSC2000_PROTECTTROUBLE *troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}


//����FEP����������������
int CSC2000_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	gram[4]=gram[3];
	GetProtinfoTime(gram+4, TIMEBYBCD, &sys_clock);	//����ʱ��
	mdlp = GetModulePara(terminalno,protocoltype,gram[0],gram[12]);			//΢����������

	if(gram[1] == 0x40)		//�����¼�
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		make_action_apevent(&sys_clock,mdlp,gram+13);
		ret = save_action_apevent(&sys_clock,mdlp,gram+13);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//���ɱ�������Ӧ���¼�
void CSC2000_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	CSC2000_PROTECTACTION *csc2000_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_CSC2000);	//������Ϣ
	msg.type_value = event[3];			//��������
	msg.event_int_info[0] = event[0];	//�¼����
	msg.event_int_info[1] = event[1];	//�ö��������ۼƷ�����
	msg.event_int_info[2] = event[2];	//�¹�����
	msg.event_int_info[3] = event[3];	//��������
	msg.event_int_info[4] = event[4];	//����ֵ����
	msg.event_float_info[0] = (float)(event[5]+event[6]*256)/100.0f;
	
	if((csc2000_action = GetActionPara(msg.type_value))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = csc2000_action->sgflag;
	strcpy(msg.enttypedescribe,csc2000_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int CSC2000_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN],longdes[120];
	CSC2000_PROTECTACTION *csc2000_action;

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

	tmpevent.evt.state = event[3];			//��������-----״ֵ̬
	tmpevent.evt.val = (float)(event[5]+event[6]*256)/100.0f;	//����ֵ
	if((csc2000_action = GetActionPara(event[3])) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ

	sprintf(tmpevent.evt.member1,"%d",csc2000_action->actioncode);
	strcpy(tmpevent.member_des1,csc2000_action->actioninfo);
	tmpevent.evt.ToneOut = csc2000_action->alarmf;
	strcpy(entname,csc2000_action->entname);

	GetTroubleInfo(event[2], tmpevent.member_des2);	//����

	CSC2000_PROTECTMEASURE *CSC2000_measure = GetMeasurePara(event[4]);
	if(CSC2000_measure)
	{
		sprintf(longdes,"%s:%4.2f%s",
			CSC2000_measure->measureinfo,(float)tmpevent.evt.val,CSC2000_measure->measureunit);
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
	}
	return OK;
}

//�Լ��������
int CSC2000_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	CSC2000_PROTECTDETECT *csc2000_detect;

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
	if((csc2000_detect = GetDetectPara(event[0]))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",csc2000_detect->detectcode);
	strcpy(tmpevent.member_des1,csc2000_detect->detectinfo);

	yield_entdescribe(csc2000_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = csc2000_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(csc2000_detect->entname,&tmpevent);
	//2003 03
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}
	return OK;
}


