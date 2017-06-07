#include "scadaproc.h"
#include "procthread.h"
#include "prot_LSA_P.h"

				//////////////////////////////////////////////////////////////
				/*					LSA_P ΢������������					*/
				//////////////////////////////////////////////////////////////


LSA_P_Protect::LSA_P_Protect(void)
{
}

PROTECTMDL *LSA_P_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1) &&
			(mdlp->address2==address2))
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//ȡ������������
LSA_P_PROTECTACTION	*LSA_P_Protect::GetActionPara(short actioncode)
{
	return (LSA_P_PROTECTACTION *)table.RdbGetRecordAddr(LSAP_PROTACTION_TABNAME,&actioncode);
}

//ȡ������Ϣ����
LSA_P_PROTECTTROUBLE	*LSA_P_Protect::GetTroublePara(short troublecode)
{
	return (LSA_P_PROTECTTROUBLE *)table.RdbGetRecordAddr(LSAP_PROTTRBL_TABNAME,&troublecode);
}

//ȡ����ֵ��Ϣ����
LSA_P_PROTECTMEASURE	*LSA_P_Protect::GetMeasurePara(short measurecode)
{
	return (LSA_P_PROTECTMEASURE *)table.RdbGetRecordAddr(LSAP_PROTMSR_TABNAME,&measurecode);
}

//ȡ�Լ���Ϣ����
LSA_P_PROTECTDETECT	*LSA_P_Protect::GetDetectPara(short detectcode)
{
	return (LSA_P_PROTECTDETECT *)table.RdbGetRecordAddr(LSAP_PROTDETECT_TABNAME,&detectcode);
}

//ȡ������������
int LSA_P_Protect::GetTroubleInfo(short troubletype,char *info)
{
	LSA_P_PROTECTTROUBLE *troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}

//����FEP����������������
int LSA_P_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	gram[4]=gram[3];
	GetProtinfoTime(gram+4, TIMEBYBCD, &sys_clock);	//����ʱ��
	mdlp = GetModulePara(terminalno,protocoltype,gram[0],gram[12]);			//΢����������
	if(mdlp == NULL)return PROT_ERR_MODULE;

	if(gram[1] == 0x40)		//�����¼�
	{
		make_action_apevent(&sys_clock,mdlp,gram+13);
		ret = save_action_apevent(&sys_clock,mdlp,gram+13);
	}
	else if(gram[1] == 0x51)	//�Լ��¼�
	{
		ret = save_detect_apevent(&sys_clock,mdlp,gram+13);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//���ɱ�������Ӧ���¼�
void LSA_P_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	LSA_P_PROTECTACTION *LSA_P_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_LSA_P);	//������Ϣ
	msg.type_value = event[3];			//��������
	msg.event_int_info[0] = event[0];	//�¼����
	msg.event_int_info[1] = event[1];	//�ö��������ۼƷ�����
	msg.event_int_info[2] = event[2];	//�¹�����
	msg.event_int_info[3] = event[3];	//��������
	msg.event_int_info[4] = event[4];	//����ֵ����
	msg.event_float_info[0] = (float)(event[5]+event[6]*256)/100.0f;
	
	if((LSA_P_action = GetActionPara(msg.type_value))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = LSA_P_action->sgflag;
	strcpy(msg.enttypedescribe,LSA_P_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int LSA_P_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN],longdes[120];
	LSA_P_PROTECTACTION *LSA_P_action;

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

	if((LSA_P_action = GetActionPara(event[3])) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ
	sprintf(tmpevent.evt.member1,"%d",LSA_P_action->actioncode);
	strcpy(tmpevent.member_des1,LSA_P_action->actioninfo);
	tmpevent.evt.ToneOut = LSA_P_action->alarmf;
	strcpy(entname,LSA_P_action->entname);

	GetTroubleInfo(event[2], tmpevent.member_des2);	//����

	LSA_P_PROTECTMEASURE *LSA_P_measure = GetMeasurePara(event[4]);

	if(LSA_P_measure)
	{
		sprintf(longdes,"%s:%4.2f%s",
			LSA_P_measure->measureinfo,(float)tmpevent.evt.val,LSA_P_measure->measureunit);
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
int LSA_P_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	LSA_P_PROTECTDETECT *LSA_P_detect;

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
	if((LSA_P_detect = GetDetectPara(event[0]))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",LSA_P_detect->detectcode);
	strcpy(tmpevent.member_des1,LSA_P_detect->detectinfo);

	yield_entdescribe(LSA_P_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = LSA_P_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(LSA_P_detect->entname,&tmpevent);
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

