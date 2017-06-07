#include "scadaproc.h"
#include "procthread.h"
#include "prot_DMP.h"

				//////////////////////////////////////////////////////////////
				/*					DMP ΢������������					*/
				//////////////////////////////////////////////////////////////

Rdb_Tab DMP_Protect::rdbtable(MyUserName,MyPassWord);
int DMP_Protect::opentabf;


DMP_Protect::DMP_Protect(void)
{

}

PROTECTMDL *DMP_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
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
DMP_PROTECTACTION	*DMP_Protect::GetActionPara(short type,short actioncode)
{
	KEY_DMP key;
	key.devicetype = type;
	key.code = actioncode;
	return (DMP_PROTECTACTION *)table.RdbGetRecordAddr(DMP_PROTACTION_TABNAME,&key);
}

//ȡ������Ϣ����
DMP_PROTECTTROUBLE	*DMP_Protect::GetTroublePara(short type,short troublecode)
{
	KEY_DMP key;
	key.devicetype = type;
	key.code = troublecode;
	return (DMP_PROTECTTROUBLE *)table.RdbGetRecordAddr(DMP_PROTTRBL_TABNAME,&troublecode);
}

//ȡ����ֵ��Ϣ����
DMP_PROTECTMEASURE	*DMP_Protect::GetMeasurePara(short type,short measurecode)
{
	KEY_DMP key;
	key.devicetype = type;
	key.code = measurecode;
	return (DMP_PROTECTMEASURE *)table.RdbGetRecordAddr(DMP_PROTMSR_TABNAME,&measurecode);
}



//ȡ������������
int DMP_Protect::GetActionInfo(short type,short actiontype,char *name,char *info)
{
	DMP_PROTECTACTION *actionp = GetActionPara(type,actiontype);
	if(actionp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,actionp->actioninfo);
	if(info) strcpy(info,actionp->actioninfo);
	return(OK);
}


//ȡ������������
int DMP_Protect::GetTroubleInfo(short type,short troubletype,char *info)
{
	DMP_PROTECTTROUBLE *troublep = GetTroublePara(type,troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}

//ȡʱ��
int DMP_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag)
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
		clock->day = gram_part[0]/16 *10 + gram_part[0]%16;
		clock->hour = gram_part[1]/16 *10 + gram_part[1]%16;
		clock->minute = gram_part[2] /16 *10 + gram_part[2]%16;
		clock->second= gram_part[3] /16 *10 + gram_part[3]%16;
		clock->msecond=(gram_part[5] &0x0f)* 100 + gram_part[4]/16*10 +gram_part[4]%16;
	}

	return OK;
}

//����FEP����������������
int DMP_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret=OK,i;

	if(gram_len<16) return PROT_ERR_MAILLEN;

	//
	uchar address,index,measurenum=0;

	if(gram[10] == 0x40)		//�����¼�
	{
		int num = gram[11]&0x7f;
		if (num==0) 
			return PROT_ERR_MESSAGE;
		index = 13;//

		for (i=0;i<num;i++)
		{
			if (index>=gram_len) break;
			address = gram[index];
			mdlp = GetModulePara(terminalno,protocoltype,address);			//΢����������
			if (mdlp ) 
			{
				GetProtinfoTime(gram+index+1, TIMEBYBIN, &sys_clock,1);	//����ʱ��
				make_action_apevent(&sys_clock,mdlp,gram+index+7);
				save_action_apevent(&sys_clock,mdlp,gram+index+7);
			}
			index =index + 14 ;
		}
	}
	else 
		return PROT_ERR_MESSAGE;

	return ret;
}

//���ɱ�������Ӧ���¼�
void DMP_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	DMP_PROTECTACTION *DMP_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_DMP);	//������Ϣ
	msg.type_value = event[0];			//��������
	
	if((DMP_action = GetActionPara(mdlp->type1,msg.type_value))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = DMP_action->sgflag;
	strcpy(msg.enttypedescribe,DMP_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int DMP_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN],longdes[120];
	DMP_PROTECTACTION *DMP_action;

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

	tmpevent.evt.state = 0x01;			//��������-----״ֵ̬
	short typp = event[2];
	if((DMP_action = GetActionPara(mdlp->type1,typp)) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ

	sprintf(tmpevent.evt.member1,"%d",DMP_action->actioncode);
	strcpy(tmpevent.member_des1,DMP_action->actioninfo);
	tmpevent.evt.ToneOut = DMP_action->alarmf;
	strcpy(entname,DMP_action->entname);

	typp = event[1];
	GetTroubleInfo(mdlp->type1,typp, tmpevent.member_des2);	//����

	short measurevalue =0;
	typp = event[4];
	DMP_PROTECTMEASURE *DMP_measure = GetMeasurePara(mdlp->type1,typp);
	if(DMP_measure)
	{
		measurevalue = event[5]+event[6]*256;
		sprintf(longdes,"%s:%d%s",
			DMP_measure->measureinfo,measurevalue,DMP_measure->measureunit);
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


