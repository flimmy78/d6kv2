#include "scadaproc.h"
#include "procthread.h"
#include "prot_ROAD.h"

				//////////////////////////////////////////////////////////////
				/*					ROAD ΢������������					*/
				//////////////////////////////////////////////////////////////

Rdb_Tab ROAD_Protect::rdbtable(MyUserName,MyPassWord);
int ROAD_Protect::opentabf;


ROAD_Protect::ROAD_Protect(void)
{

}

PROTECTMDL *ROAD_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
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
ROAD_PROTECTACTION	*ROAD_Protect::GetActionPara(short type,short actioncode)
{
	KEY_ROAD key;
	key.devicetype = type;
	key.code = actioncode;
	return (ROAD_PROTECTACTION *)table.RdbGetRecordAddr(ROAD_PROTACTION_TABNAME,&key);
}

//ȡ������Ϣ����
ROAD_PROTECTTROUBLE	*ROAD_Protect::GetTroublePara(short type,short troublecode)
{
	KEY_ROAD key;
	key.devicetype = type;
	key.code = troublecode;
	return (ROAD_PROTECTTROUBLE *)table.RdbGetRecordAddr(ROAD_PROTTRBL_TABNAME,&key);
}

//ȡ����ֵ��Ϣ����
ROAD_PROTECTMEASURE	*ROAD_Protect::GetMeasurePara(short type,short measurecode)
{
	KEY_ROAD key;
	key.devicetype = type;
	key.code = measurecode;
	return (ROAD_PROTECTMEASURE *)table.RdbGetRecordAddr(ROAD_PROTMSR_TABNAME,&key);
}

//ȡ�Լ���Ϣ����
ROAD_PROTECTDETECT	*ROAD_Protect::GetDetectPara(short type,short detectcode)
{
	KEY_ROAD key;
	key.devicetype = type;
	key.code = detectcode;
	return (ROAD_PROTECTDETECT *)table.RdbGetRecordAddr(ROAD_PROTDETECT_TABNAME,&key);
}


//ȡ������������
int ROAD_Protect::GetActionInfo(short type,short actiontype,char *name,char *info)
{
	ROAD_PROTECTACTION *actionp = GetActionPara(type,actiontype);
	if(actionp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,actionp->actioninfo);
	if(info) strcpy(info,actionp->actioninfo);
	return(OK);
}

//ȡ�����Լ���Ϣ����
int ROAD_Protect::GetDetectInfo(short type,short detecttype,char *name,char *info)
{
	ROAD_PROTECTDETECT *detectp = GetDetectPara(type,detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}

//ȡ������������
int ROAD_Protect::GetTroubleInfo(short type,short troubletype,char *info)
{
	ROAD_PROTECTTROUBLE *troublep = GetTroublePara(type,troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}

//ȡʱ��
int ROAD_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag)
{
	SYS_CLOCK sysclock;
	struct tm tim;
	struct tm *tim1;
	time_t tt;
	if(clock == NULL)return -1;
	GetCurrentClock(&sysclock);
	if(flag==0)
	{
		tim.tm_year=sysclock.year-1900;
		tim.tm_mon=sysclock.month-1;
		tim.tm_mday=sysclock.day;
		tim.tm_hour=gram_part[7] & 0x1f;
		tim.tm_min=gram_part[6] & 0x3f;
		tim.tm_sec=(gram_part[4] * 256 + gram_part[5])/1000;
		tim.tm_isdst=0;
		tt=mktime(&tim);
		tt=tt+(gram_part[0] * 256 + gram_part[1])/1000;
		tim1=localtime(&tt);
		clock->year=((uchar) tim1->tm_year)+1900;
        clock->month=(uchar)tim1->tm_mon+1;
        clock->day=(uchar)tim1->tm_mday;
        clock->hour=(uchar) tim1->tm_hour;
        clock->minute=(uchar) tim1->tm_min;
        clock->second=(uchar) tim1->tm_sec;
		clock->msecond=((gram_part[7] * 256 + gram_part[6])%1000+gram_part[3] * 256 + gram_part[2])%1000;
	}
	else if(flag==1)
	{
		clock->year = sysclock.year;
		clock->month = sysclock.month;
		clock->day = sysclock.day;
		clock->hour = gram_part[3] & 0x1f;
		clock->minute = gram_part[2] & 0x3f;
		clock->second=(gram_part[1] * 256 + gram_part[0])/1000;
		clock->msecond=(gram_part[1] * 256 + gram_part[0])%1000;
	}

	return OK;
}

//����FEP����������������
int ROAD_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret=OK,i;

	if(gram_len<16) return PROT_ERR_MAILLEN;

	//
	int num = gram[1]&0x3f;
	if (num==0) 
		return PROT_ERR_MESSAGE;

	uchar address,index,measurenum=0;

	if(gram[0] == TYPE_140)		//�����¼�
	{
		index = 4;//
		for (i=0;i<num;i++)
		{
			if (index>=gram_len) break;
			address = gram[index];
			mdlp = GetModulePara(terminalno,protocoltype,address);			//΢����������
			measurenum = gram[index+12];
			GetProtinfoTime(gram+index+8, TIMEBYBIN, &sys_clock,1);	//����ʱ��
			make_action_apevent(&sys_clock,mdlp,gram+index);
			save_action_apevent(&sys_clock,mdlp,gram+index);
			index =index + 13 + measurenum*4;
		}
	}
	else if(gram[1] == TYPE_141)	//�Լ��¼�
	{
		index = 4;//
		for (i=0;i<num;i++)
		{
			if (index>=gram_len) break;
			address = gram[index];
			mdlp = GetModulePara(terminalno,protocoltype,address);			//΢����������
			GetProtinfoTime(gram+index+3, TIMEBYBIN, &sys_clock,1);	//����ʱ��
			save_detect_apevent(&sys_clock,mdlp,gram+index);
			index =index + 7;
		}
	}
	else 
		return PROT_ERR_MESSAGE;

	return ret;
}

//���ɱ�������Ӧ���¼�
void ROAD_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	ROAD_PROTECTACTION *ROAD_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_ROAD);	//������Ϣ
	msg.type_value = event[2];			//��������
	
	if((ROAD_action = GetActionPara(mdlp->type1,msg.type_value))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = ROAD_action->sgflag;
	strcpy(msg.enttypedescribe,ROAD_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int ROAD_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN],longdes[120];
	ROAD_PROTECTACTION *ROAD_action;

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

	tmpevent.evt.state = event[3] - 1;			//��������-----״ֵ̬
	if((ROAD_action = GetActionPara(mdlp->type1,event[2])) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ

	sprintf(tmpevent.evt.member1,"%d",ROAD_action->actioncode);
	strcpy(tmpevent.member_des1,ROAD_action->actioninfo);
	tmpevent.evt.ToneOut = ROAD_action->alarmf;
	strcpy(entname,ROAD_action->entname);

	GetTroubleInfo(mdlp->type1,event[6]+event[7]*256, tmpevent.member_des2);	//����

	uchar measurenum = event[12];
	uchar value;

	for (int i=0;i<measurenum;i++)
	{
		ROAD_PROTECTMEASURE *ROAD_measure = GetMeasurePara(mdlp->type1,event[13+i*4]+event[14+i*4]*256);
		if(ROAD_measure)
		{
			value = event[16+i*4];
			sprintf(longdes,"%s:%d%s",
				ROAD_measure->measureinfo,value,ROAD_measure->measureunit);
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
int ROAD_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	ROAD_PROTECTDETECT *ROAD_detect;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	tmpevent.evt.state = event[2] - 1;	//״ֵ̬---�Լ��¼�����
	if((ROAD_detect = GetDetectPara(mdlp->type1,event[1]))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",ROAD_detect->detectcode);
	strcpy(tmpevent.member_des1,ROAD_detect->detectinfo);

	yield_entdescribe(ROAD_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = ROAD_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(ROAD_detect->entname,&tmpevent);
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

