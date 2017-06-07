#include <math.h>
#include "scadaproc.h"
#include "procthread.h"
#include "prot_FBZ.h"


				//////////////////////////////////////////////////////////////
				/*					FBZ(LSA_P) ΢������������					*/
				//////////////////////////////////////////////////////////////


FBZ_Protect::FBZ_Protect(void)
{
}

PROTECTMDL *FBZ_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
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
FBZ_PROTECTACTION	*FBZ_Protect::GetActionPara(short actioncode)
{
	return (FBZ_PROTECTACTION *)table.RdbGetRecordAddr(FBZ_PROTACTION_TABNAME,&actioncode);
}

//ȡ������Ϣ����
FBZ_PROTECTTROUBLE	*FBZ_Protect::GetTroublePara(short troublecode)
{
	return (FBZ_PROTECTTROUBLE *)table.RdbGetRecordAddr(FBZ_PROTTRBL_TABNAME,&troublecode);
}

//ȡ����ֵ��Ϣ����
FBZ_PROTECTMEASURE	*FBZ_Protect::GetMeasurePara(short measurecode)
{
	return (FBZ_PROTECTMEASURE *)table.RdbGetRecordAddr(FBZ_PROTMSR_TABNAME,&measurecode);
}

//ȡ�Լ���Ϣ����
FBZ_PROTECTDETECT	*FBZ_Protect::GetDetectPara(short detectcode)
{
	return (FBZ_PROTECTDETECT *)table.RdbGetRecordAddr(FBZ_PROTDETECT_TABNAME,&detectcode);
}


//ȡ������������
int FBZ_Protect::GetActionInfo(short actiontype,char *name,char *info)
{
	FBZ_PROTECTACTION *actionp = GetActionPara(actiontype);
	if(actionp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,actionp->actioninfo);
	if(info) strcpy(info,actionp->actioninfo);
	return(OK);
}

//ȡ�����Լ���Ϣ����
int FBZ_Protect::GetDetectInfo(short detecttype,char *name,char *info)
{
	FBZ_PROTECTDETECT *detectp = GetDetectPara(detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}

//ȡ������������
int FBZ_Protect::GetTroubleInfo(short troubletype,char *info)
{
	FBZ_PROTECTTROUBLE *troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}


//����FEP����������������
int FBZ_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	unsigned char gramtime[10];
	gramtime[0] = (gram[5]+gram[4]*100)%100; //year
	gramtime[1] = gram[6]; //month
	gramtime[2] = gram[7];  //day
	gramtime[3] = gram[8];  //hour
	gramtime[4] = gram[9];  //minute
	gramtime[5] = gram[10];  //second
	gramtime[6] = gram[11];  //ms(L)
	gramtime[7] = gram[12];  //ms(H)
	GetProtinfoTime((unsigned char*)gramtime, TIMEBYBIN, &sys_clock);	//����ʱ��

	mdlp = GetModulePara(terminalno,protocoltype,gram[1],gram[3]);			//΢����������

	if(gram[2] == 0x8c)		//�����¼�
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		make_action_apevent(&sys_clock,mdlp,gram+13);
		ret = save_action_apevent(&sys_clock,mdlp,gram+13);
	}
	else if(gram[2] == 0x89)		//���ϱ���
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		ret = save_trouble_apevent(&sys_clock,mdlp,gram+13,(gram[3]-14)/3);
	}
	else if(gram[2] == 0x8a)	//�Լ��¼�
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		ret = save_detect_apevent(&sys_clock,mdlp,gram+14);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//���ɱ�������Ӧ���¼�
void FBZ_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	short tmpactype;
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	FBZ_PROTECTACTION *isa_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FBZ);	//������Ϣ
	tmpactype=event[0] + event[1] * 256; //��������
	msg.type_value = tmpactype;
	msg.event_int_info[3] = tmpactype;	//��������
	
	if((isa_action = GetActionPara(msg.type_value))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = isa_action->sgflag;
	strcpy(msg.enttypedescribe,isa_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int FBZ_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	short tmpactype;
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	FBZ_PROTECTACTION *isa_action;

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

	tmpactype=event[0] + event[1] * 256;			//��������
	tmpevent.evt.state = tmpactype;			//��������-----״ֵ̬
	if((isa_action = GetActionPara(tmpactype)) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ

	sprintf(tmpevent.evt.member1,"%d",isa_action->actioncode);
	strcpy(tmpevent.member_des1,isa_action->actioninfo);
	tmpevent.evt.ToneOut = isa_action->alarmf;
	strcpy(entname,isa_action->entname);

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


//���ϱ����������
int FBZ_Protect::save_trouble_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event,int measurenums)
{
	short tmpactype;
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN],longdes[120];
	FBZ_PROTECTACTION *isa_action;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 2);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	/////////���ɴ�������//////////////////////////////////////////////////

	sprintf(tmpevent.evt.member1,"%d",event[0]);
	sprintf(tmpevent.member_des1,"��·%d",event[0]);

	GetTroubleInfo(event[2], tmpevent.member_des2);	//����

	tmpactype=event[3] + event[4] * 256;			//��������
	tmpevent.evt.state = tmpactype;			//��������-----״ֵ̬
	if((isa_action = GetActionPara(tmpactype)) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ

	sprintf(tmpevent.evt.member3,"%d",isa_action->actioncode);
	strcpy(tmpevent.member_des3,isa_action->actioninfo);
	tmpevent.evt.ToneOut = isa_action->alarmf;
	strcpy(entname,isa_action->entname);


	FBZ_PROTECTMEASURE *FBZ_measure;
	float	tmpfloat;
	char    prtfmt[20];
	for(int i=0;i<measurenums;i++) {
		FBZ_measure = GetMeasurePara(event[5+i*3]);
		if(FBZ_measure)
		{
			if(FBZ_measure->dotnums>6 || FBZ_measure->dotnums<1) {
				tmpfloat=(float)(event[6+i*3]+event[7+i*3] * 256);
				sprintf(longdes," %s:%.1f%s",FBZ_measure->measureinfo,tmpfloat,FBZ_measure->measureunit);
			}
			else {
				tmpfloat=(float)(event[6+i*3]+event[7+i*3] * 256) / (float)pow(10.0,(double)FBZ_measure->dotnums);
				sprintf(prtfmt," %%s:%%.%df%%s",FBZ_measure->dotnums);
				sprintf(longdes,prtfmt,FBZ_measure->measureinfo,tmpfloat,FBZ_measure->measureunit);
			}
			
			if((strlen(longdes)+strlen(tmpevent.longdes))>(sizeof(tmpevent.longdes)-2)) break;
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
	}//for--no
	return OK;
}


//�Լ��������
int FBZ_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	FBZ_PROTECTDETECT *isa_detect;

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

	sprintf(tmpevent.evt.member2,"%d",event[1]);
	sprintf(tmpevent.member_des2,"���1:%.2xH",event[1]);
	sprintf(tmpevent.evt.member3,"%d",event[1]);
	sprintf(tmpevent.member_des3,"���2:%.2xH",event[2]);

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


