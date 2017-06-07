#include "scadaproc.h"
#include "procthread.h"
#include "prot_DFP500.h"

				//////////////////////////////////////////////////////////////
				/*					DFP500 ΢������������					*/
				//////////////////////////////////////////////////////////////


DFP500_Protect::DFP500_Protect(void)
{
}

PROTECTMDL *DFP500_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1) 
			&&(mdlp->address2==address2)) 
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//ȡ������������
DFP500_PROTECTACTION	*DFP500_Protect::GetActionPara(short actioncode)
{
	 return (DFP500_PROTECTACTION *)table.RdbGetRecordAddr(DFP500_PROTACTION_TABNAME,&actioncode);
}

//ȡ������Ϣ����
DFP500_PROTECTTROUBLE	*DFP500_Protect::GetTroublePara(short troublecode)
{
	return (DFP500_PROTECTTROUBLE *)table.RdbGetRecordAddr(DFP500_PROTTRBL_TABNAME,&troublecode);
}

//ȡ����ֵ��Ϣ����
DFP500_PROTECTMEASURE	*DFP500_Protect::GetMeasurePara(short measurecode)
{
	return (DFP500_PROTECTMEASURE *)table.RdbGetRecordAddr(DFP500_PROTMSR_TABNAME,&measurecode);
}

//ȡ�Լ���Ϣ����
DFP500_PROTECTDETECT	*DFP500_Protect::GetDetectPara(short detectcode)
{
	return (DFP500_PROTECTDETECT *)table.RdbGetRecordAddr(DFP500_PROTDETECT_TABNAME,&detectcode);
}

//ȡ������������
int DFP500_Protect::GetTroubleInfo(short troubletype,char *info)
{
	DFP500_PROTECTTROUBLE *troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}


int DFP500_Protect::GetProtInfoTime(uchar *gram_part, SYS_CLOCK *clock)
{
	if(BCD1BIN1(gram_part[0]) < 98)
		clock->year = BCD1BIN1(gram_part[0]) + 2000;
	else
		clock->year = BCD1BIN1(gram_part[0]) + 1900;
	clock->month=BCD1BIN1(gram_part[1]);
	clock->day=BCD1BIN1(gram_part[2]);
	clock->hour=BCD1BIN1(gram_part[3]);
	clock->minute=BCD1BIN1(gram_part[4]);
	clock->second=BCD1BIN1(gram_part[5]);
	clock->msecond=BCD1BIN1(gram_part[6])+BCD1BIN1(gram_part[7])*100;
	clock->msecond = clock->msecond/10;
	return OK;
}

//����FEP����������������
int DFP500_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	gram[4]=gram[3];
	GetProtInfoTime(gram+4, &sys_clock);	//����ʱ��
	mdlp = GetModulePara(terminalno,protocoltype,gram[0],gram[12]);			//΢����������
	if(mdlp == NULL) return PROT_ERR_MODULE;

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
void DFP500_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	DFP500_PROTECTACTION *DFP500_action;

	if(mdlp == NULL || sysclock == NULL)return;
	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_DFP500);	//������Ϣ
	msg.type_value = event[3];			//��������
	msg.event_int_info[0] = event[0];	//�¼����
	msg.event_int_info[1] = event[1];	//�ö��������ۼƷ�����
	msg.event_int_info[2] = event[2];	//�¹�����
	msg.event_int_info[3] = event[3];	//��������
	msg.event_int_info[4] = event[4];	//����ֵ����
	msg.event_float_info[0] = (float)(event[5]+event[6]*256)/100.0f;
	
	if((DFP500_action = GetActionPara(msg.type_value))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = DFP500_action->sgflag;
	strcpy(msg.enttypedescribe,DFP500_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int DFP500_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN],longdes[120];
	short measureval;
	DFP500_PROTECTACTION *DFP500_action;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL) return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	/////////���ɴ�������//////////////////////////////////////////////////

	tmpevent.evt.state = event[3];			//��������-----״ֵ̬
	measureval = (event[5]+(event[6]&0x7f)*256);	
	if(event[6]&0x80) tmpevent.evt.val = (float)measureval * 0.01f;
	else tmpevent.evt.val = (float)measureval * 0.001f;//����ֵFMT1��ʽ
	if((DFP500_action = GetActionPara(event[3])) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ

	sprintf(tmpevent.evt.member1,"%d",DFP500_action->actioncode);
	strcpy(tmpevent.member_des1,DFP500_action->actioninfo);
	tmpevent.evt.ToneOut = DFP500_action->alarmf;
	strcpy(entname,DFP500_action->entname);

	GetTroubleInfo(event[2], tmpevent.member_des2);	//����

	DFP500_PROTECTMEASURE *DFP500_measure = GetMeasurePara(event[4]);
	if(DFP500_measure)
	{
		sprintf(longdes,"%s:%4.2f%s",
			DFP500_measure->measureinfo,(float)tmpevent.evt.val,DFP500_measure->measureunit);
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
int DFP500_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	DFP500_PROTECTDETECT *DFP500_detect;

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
	if((DFP500_detect = GetDetectPara(event[0]))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",DFP500_detect->detectcode);
	strcpy(tmpevent.member_des1,DFP500_detect->detectinfo);

	yield_entdescribe(DFP500_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = DFP500_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(DFP500_detect->entname,&tmpevent);
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
