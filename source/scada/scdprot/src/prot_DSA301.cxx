#include "scadaproc.h"
#include "procthread.h"
#include "prot_DSA301.h"

				//////////////////////////////////////////////////////////////
				/*					DSA_301 ΢������������					*/
				//////////////////////////////////////////////////////////////


DSA_301_Protect::DSA_301_Protect(void)
{
}

PROTECTMDL *DSA_301_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
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
DSA_301_PROTECTACTION	*DSA_301_Protect::GetActionPara(short actioncode)
{
	return (DSA_301_PROTECTACTION *)table.RdbGetRecordAddr(DSA_301_PROTACTION_TABNAME,&actioncode);
}

//ȡ������Ϣ����
DSA_301_PROTECTTROUBLE	*DSA_301_Protect::GetTroublePara(short troublecode)
{
	return (DSA_301_PROTECTTROUBLE *)table.RdbGetRecordAddr(DSA_301_PROTTRBL_TABNAME,&troublecode);
}

//ȡ����ֵ��Ϣ����
DSA_301_PROTECTMEASURE	*DSA_301_Protect::GetMeasurePara(short measurecode)
{
	return (DSA_301_PROTECTMEASURE *)table.RdbGetRecordAddr(DSA_301_PROTMSR_TABNAME,&measurecode);
}

//ȡ�Լ���Ϣ����
DSA_301_PROTECTDETECT	*DSA_301_Protect::GetDetectPara(short detectcode)
{
	return (DSA_301_PROTECTDETECT *)table.RdbGetRecordAddr(DSA_301_PROTDETECT_TABNAME,&detectcode);
}

//ȡ������������
int DSA_301_Protect::GetTroubleInfo(short troubletype,char *info)
{
	DSA_301_PROTECTTROUBLE *troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}

//����FEP����������������
int DSA_301_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	gram[4]=gram[3];
	GetProtinfoTime(gram+4, TIMEBYBCD, &sys_clock);	//����ʱ��
	mdlp = GetModulePara(terminalno,protocoltype,gram[0],gram[12]);			//gram[0]��ַ1��gram[12]��ַ2��΢����������
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
void DSA_301_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	DSA_301_PROTECTACTION *DSA_301_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_DSA_301);	//������Ϣ
	msg.type_value = event[3];			//��������
	msg.event_int_info[0] = event[0];	//�¼����
	msg.event_int_info[1] = event[1];	//�ö��������ۼƷ�����
	msg.event_int_info[2] = event[2];	//�¹�����
	msg.event_int_info[3] = event[3];	//��������
	msg.event_int_info[4] = event[4];	//����ֵ����
	msg.event_float_info[0] = (float)(event[5]+event[6]*256)/100.0f;
	
	if((DSA_301_action = GetActionPara(msg.type_value))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = DSA_301_action->sgflag;
	strcpy(msg.enttypedescribe,DSA_301_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int DSA_301_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN],longdes[120];
	DSA_301_PROTECTACTION *DSA_301_action;

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
	if((DSA_301_action = GetActionPara(event[3])) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ

	sprintf(tmpevent.evt.member1,"%d",DSA_301_action->actioncode);
	strcpy(tmpevent.member_des1,DSA_301_action->actioninfo);
	tmpevent.evt.ToneOut = DSA_301_action->alarmf;
	strcpy(entname,DSA_301_action->entname);

	GetTroubleInfo(event[2], tmpevent.member_des2);	//����

	DSA_301_PROTECTMEASURE *DSA_301_measure = GetMeasurePara(event[4]);
	if(DSA_301_measure)
	{
		sprintf(longdes,"%s:%4.2f%s",
			DSA_301_measure->measureinfo,(float)tmpevent.evt.val,DSA_301_measure->measureunit);
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
int DSA_301_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	DSA_301_PROTECTDETECT *DSA_301_detect;

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
	if((DSA_301_detect = GetDetectPara(event[0]))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",DSA_301_detect->detectcode);
	strcpy(tmpevent.member_des1,DSA_301_detect->detectinfo);

	yield_entdescribe(DSA_301_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = DSA_301_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(DSA_301_detect->entname,&tmpevent);
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

