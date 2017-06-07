#include "scadaproc.h"
#include "procthread.h"
#include "prot_NEWZDB.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					NEWZDB ΢������������					*/
				//////////////////////////////////////////////////////////////



NEWZDB_Protect::NEWZDB_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *NEWZDB_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
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

//ȡ�¼���Ϣ����
NEWZDB_PROTECTACTION	*NEWZDB_Protect::GetActionPara(short actioncode)
{
	return (NEWZDB_PROTECTACTION *)table.RdbGetRecordAddr(NEWZDB_PROTACTION_TABNAME,&actioncode);
}

//ȡ�����Լ���Ϣ
IEC103_PROTECTACTION* NEWZDB_Protect::GetIEC103ActionPara(unsigned short devicetype,unsigned short actioncode)
{
	IEC103_ACTION_KEY key;
	key.devicetype = devicetype;
	key.actioncode = actioncode;
	return ((IEC103_PROTECTACTION *)table.RdbGetRecordAddr(IEC103_PROTACTION_TABNAME, &key));
}

//����FEP����������������
int NEWZDB_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;
	NetDbg dbg;
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�յ�NEWZDB�ı����¼��ʼ�");
	if(gram_len<30) return PROT_ERR_MAILLEN;

	for (int i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}

	m_GramLength=m_Buf[3];									//���ĳ���(������������ֵ�ĸ���)
//	GetProtinfoTime(gram+24, TIMEBYBCD, &sys_clock);	//����ʱ��
	address2=m_Buf[4]+m_Buf[5]*256;						//��վ��ַ	��Ӧ΢���������������������еõ�ַ2
	address1=m_Buf[6]+m_Buf[7]*256;						//��վ��ַ	��Ӧ΢���������������������еõ�ַ1
	mdlp = GetModulePara(terminalno,protocoltype,address1,address2);			//΢����������
	

	if(m_Buf[16] == 0xa3)		//�����¼�
	{
		//΢�����������¼�
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��������NEWZDB�����¼�");
		char text[60];
		sprintf(text,"�ն˺�:%d\t��Լ����:%d\t��վ��ַ:%d\t��վ��ַ:%d\t",terminalno,protocoltype,address1,address2);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
		if(mdlp == NULL)return PROT_ERR_MODULE;
		make_action_apevent(&sys_clock,mdlp,m_Buf+20);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣��NEWZDB�����¼�");
		ret = save_action_apevent(&sys_clock,mdlp,m_Buf+20);

	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//���ɱ�������Ӧ���¼�
void NEWZDB_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	NEWZDB_PROTECTACTION *newzdb_action;
	short	actioncode;
	int actionnum = 0;//��������
	SYS_CLOCK  sys_clock;

	if(mdlp == NULL || sysclock == NULL)return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));
	actionnum = event[0];
	
	if (actionnum<=0) return;
	for (int i=0;i<actionnum;i++)//����actionnum����������Ӧ���¼�
	{

		actioncode=event[i*10+2];	//�������
		msg.type_value = (int)actioncode;
		GetProtinfoTime(event+4+i*10, TIMEBYBCD, &sys_clock);	//����ʱ��
		make_common_apentinfo(&msg,&sys_clock,mdlp,0,PROTTYPE_NEWZDB);	//������Ϣ

		if((newzdb_action = GetActionPara(actioncode))==NULL)return;	//ȡ������Ϣ
		msg.sgflag = newzdb_action->sgflag;
		strcpy(msg.enttypedescribe,newzdb_action->actioninfo);		//�¼���������
		apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
		
	}
}


//�����������
int NEWZDB_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	NEWZDB_PROTECTACTION *newzdb_action;
	IEC103_PROTECTACTION *iec103_action;//200507
	short	actioncode;
	int actionnum = 0;//��������
	SYS_CLOCK  sys_clock;
	NetDbg dbg;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	/////////���ɴ�������//////////////////////////////////////////////////
	actionnum = event[0];
	if (actionnum<=0) return PROT_ERR_ACTION;

	tmpevent.evt.state = event[2];	//�������

	if (mdlp->type1 ==0)
	{
		for (int i=0;i<actionnum;i++)//����actionnum����������Ӧ���¼�
		{
			actioncode=event[i*10+2];	//�������
			tmpevent.evt.state = (int)actioncode;
			GetProtinfoTime(event+4+i*10, TIMEBYBCD, &sys_clock);	//����ʱ��
			make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 0);	//������Ϣ

			if((newzdb_action = GetActionPara(actioncode))==NULL)return PROT_ERR_ACTION;	//ȡ������Ϣ
			sprintf(tmpevent.evt.member1,"%d",newzdb_action->actioncode);
			strcpy(tmpevent.member_des1,newzdb_action->actioninfo);
			tmpevent.evt.ToneOut = newzdb_action->alarmf;
			strcpy(entname,newzdb_action->entname);
			
			yield_entdescribe(entname,&tmpevent);
			if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
			//2003 03
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��ʼ����NEWZDB��������");
			NetStatus netcfg;
			int groupnum=netcfg.GetHostGrpNum();
			int *grp = netcfg.GetHostGrpEnm();
			for(int no=0;no<groupnum;no++)
			{
				tmpevent.evt.grpno=grp[no];
				save_event(&tmpevent,1);
			}//for--no
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"����NEWZDB�����������");
		}
	}
	else
	{
		for (int i=0;i<actionnum;i++)//����actionnum����������Ӧ���¼�
		{
			actioncode=event[i*10+2];	//�������
			tmpevent.evt.state = (int)actioncode;
			GetProtinfoTime(event+4+i*10, TIMEBYBCD, &sys_clock);	//����ʱ��
			make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 0);	//������Ϣ

			if((iec103_action = GetIEC103ActionPara(mdlp->type1,actioncode))==NULL)return PROT_ERR_ACTION;	//ȡ������Ϣ
			sprintf(tmpevent.evt.member1,"%d",iec103_action->actioncode);
			strcpy(tmpevent.member_des1,iec103_action->actioninfo);
			tmpevent.evt.ToneOut = iec103_action->alarmf;
			strcpy(entname,iec103_action->entname);
			
			yield_entdescribe(entname,&tmpevent);
			if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
			//2003 03
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��ʼ����NEWZDB��������");
			NetStatus netcfg;
			int groupnum=netcfg.GetHostGrpNum();
			int *grp = netcfg.GetHostGrpEnm();
			for(int no=0;no<groupnum;no++)
			{
				tmpevent.evt.grpno=grp[no];
				save_event(&tmpevent,1);
			}//for--no
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"����NEWZDB�����������");
		}
	}
	return OK;
}



int NEWZDB_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock)
{
	if(clock == NULL)return -1;
	time_t Sec;
	Sec=gram_part[0]+(gram_part[1]+(gram_part[2]+gram_part[3]*256)*256)*256;
	struct tm *time;
	time=gmtime(&Sec);
	clock->year=time->tm_year+1900;
	clock->month=time->tm_mon+1;
	clock->day=time->tm_mday;
	clock->hour=time->tm_hour;
	clock->minute=time->tm_min;
	clock->second=time->tm_sec;
	long mSec;
	mSec=gram_part[4]+(gram_part[5]+(gram_part[6]+gram_part[7]*256)*256)*256;
	clock->msecond=mSec/1000;
	return OK;
}
