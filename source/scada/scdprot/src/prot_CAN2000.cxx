#include "scadaproc.h"
#include "procthread.h"
#include "prot_CAN2000.h"
#include "math.h"

				//////////////////////////////////////////////////////////////
				/*					C2000 ΢������������					*/
				//////////////////////////////////////////////////////////////


CAN2000_Protect::CAN2000_Protect(void)
{
}

PROTECTMDL *CAN2000_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort type1,ushort address1)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->type1==type1) 
			&&(mdlp->address1==address1)) 
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//ȡ������������
CAN2000_PROTECTACTION	*CAN2000_Protect::GetActionPara(short actioncode)
{
	return (CAN2000_PROTECTACTION *)table.RdbGetRecordAddr(CAN2000_PROTACTION_TABNAME,&actioncode);
}


//ȡ����ֵ��Ϣ����
CAN2000_PROTECTMEASURE	*CAN2000_Protect::GetMeasurePara(short measurecode)
{
	return (CAN2000_PROTECTMEASURE *)table.RdbGetRecordAddr(CAN2000_PROTMSR_TABNAME,&measurecode);
}

//ȡ�Լ���Ϣ����
CAN2000_PROTECTDETECT	*CAN2000_Protect::GetDetectPara(short detectcode)
{
	return (CAN2000_PROTECTDETECT *)table.RdbGetRecordAddr(CAN2000_PROTDETECT_TABNAME,&detectcode);
}


//����FEP����������������
int CAN2000_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	char gramtime[10];
	int measurenum ,ret ;
	
	if(gram_len<5) return PROT_ERR_MAILLEN;

	gramtime[0] = (gram[12]+gram[13]*256)%100; //year
	gramtime[1] = gram[11]; //month
	gramtime[2] = gram[10];  //day
	gramtime[3] = gram[9];  //hour
	gramtime[4] = gram[8];  //minute
	gramtime[5] = gram[7];  //second
	gramtime[6] = gram[5];  //ms(L)
	gramtime[7] = gram[6];  //ms(H)
	GetProtinfoTime((unsigned char*)gramtime, TIMEBYBIN, &sys_clock);	//����ʱ��
	mdlp = GetModulePara(terminalno,protocoltype,gram[0],gram[1]);			//΢����������
	if(mdlp == NULL) return PROT_ERR_MODULE;

	if(gram[2] == 0x2c)		//�����¼�
	{
		measurenum = (gram[3]-12)/3;
		make_action_apevent(&sys_clock,mdlp,gram+14);
		ret = save_action_apevent(&sys_clock,mdlp,gram+14,measurenum);
	}
	else if(gram[2] == 0x0a)	//�Լ��¼�
	{
		ret = save_detect_apevent(&sys_clock,mdlp,gram+14);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//���ɱ�������Ӧ���¼�
void CAN2000_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	CAN2000_PROTECTACTION *CAN2000_action;

	if(mdlp == NULL || sysclock == NULL) return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_CAN2000);	//������Ϣ
	msg.type_value = event[0];			//��������
	msg.event_int_info[0] = event[2]*256+event[1];	//�ö��������ۼƷ�����
	
	if((CAN2000_action = GetActionPara(msg.type_value))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = CAN2000_action->sgflag;
	strcpy(msg.enttypedescribe,CAN2000_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}

//�����������
int CAN2000_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event, int measurenum)
{
	
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN],longdes[120];
	int i = 0;

	entname[0] = 0;

	CAN2000_PROTECTACTION *CAN2000_action;
	CAN2000_PROTECTMEASURE *CAN2000_measure;

	if(mdlp == NULL || sysclock == NULL) return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	/////////���ɴ�������//////////////////////////////////////////////////

	tmpevent.evt.state = event[0];			//��������-----״ֵ̬

	if((CAN2000_action = GetActionPara(event[0])) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ

	sprintf(tmpevent.evt.member1,"%d",CAN2000_action->actioncode);
	strcpy(tmpevent.member_des1,CAN2000_action->actioninfo);
	tmpevent.evt.ToneOut = CAN2000_action->alarmf;
	strcpy(entname,CAN2000_action->entname);

	for(i=0;i<measurenum;i++)
	{
		CAN2000_measure = GetMeasurePara(event[3*i+3]);
		if(CAN2000_measure){
			tmpevent.evt.val = (float)(event[3*i+4]+event[3*i+5]*256)*(float)pow((float)0.1,(float)CAN2000_measure->measurepoint);	//����ֵ
			sprintf(longdes,"%s:%4.2f%s",
				CAN2000_measure->measureinfo,(float)tmpevent.evt.val,CAN2000_measure->measureunit);
			if((strlen(tmpevent.longdes)+strlen(longdes))>=120) break;
			strcat(tmpevent.longdes,longdes);
		}
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
int CAN2000_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	CAN2000_PROTECTDETECT *CAN2000_detect;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL) return PROT_ERR_DETECT;
	
	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	tmpevent.evt.state = event[0];	//״ֵ̬---�Լ��¼�����
	if((CAN2000_detect = GetDetectPara( event[0]))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",CAN2000_detect->detectcode);
	strcpy(tmpevent.member_des1,CAN2000_detect->detectinfo);
	sprintf(tmpevent.member_des2,"�ۼƹ��ϴ���:%d",event[1]+event[2]*256);

	yield_entdescribe(CAN2000_detect->entname,&tmpevent);
	if(tmpevent.evt.ToneOut)yield_snddescribe(CAN2000_detect->entname,&tmpevent);
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

