#include "scadaproc.h"
#include "procthread.h"
#include "prot_ZDB.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					ZDB ΢������������					*/
				//////////////////////////////////////////////////////////////



ZDB_Protect::ZDB_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *ZDB_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
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
ZDB_PROTECTACTION	*ZDB_Protect::GetActionPara(short actiontype,short actioncode)
{
	ACTION_KEY_ZDB key;
	key.moduletype = actiontype;
	key.actioncode = actioncode;
	return (ZDB_PROTECTACTION *)table.RdbGetRecordAddr(ZDB_PROTACTION_TABNAME,&key);
}


//ȡ�Լ���Ϣ����
ZDB_PROTECTDETECT	*ZDB_Protect::GetDetectPara(short detectcode)
{
	return (ZDB_PROTECTDETECT *)table.RdbGetRecordAddr(ZDB_PROTDETECT_TABNAME,&detectcode);
}

//ȡ������Ϣ����
ZDB_PROTECTMEASURE	*ZDB_Protect::GetMeasurePara(short measurecode)
{
	return (ZDB_PROTECTMEASURE *)table.RdbGetRecordAddr(ZDB_PROTMEASURE_TABNAME,&measurecode);
}

//ȡ�����Լ���Ϣ����
int ZDB_Protect::GetDetectInfo(short detecttype,char *name,char *info)
{
	ZDB_PROTECTDETECT *detectp = GetDetectPara(detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}


//����FEP����������������
int ZDB_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;
	NetDbg dbg;
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�յ�ZDB�ı����¼��ʼ�");
	if(gram_len<30) return PROT_ERR_MAILLEN;

	for (int i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}

	m_GramLength=m_Buf[3];									//���ĳ���(������������ֵ�ĸ���)
	GetProtinfoTime(m_Buf+18, TIMEBYBCD, &sys_clock);	//����ʱ��
	RdbDebugMesg("ZDB�ʼ�ʱ��Ϊ %d-%d-%d %d-%d\n",sys_clock.year,sys_clock.month,sys_clock.day,sys_clock.hour,sys_clock.minute);

	address2=m_Buf[4]+m_Buf[5]*256;						//��վ��ַ	��Ӧ΢���������������������еõ�ַ2
	address1=m_Buf[6]+m_Buf[7]*256;						//��վ��ַ	��Ӧ΢���������������������еõ�ַ1
	mdlp = GetModulePara(terminalno,protocoltype,address1,address2);			//΢����������
/*	if(mdlp == NULL)
	{
		return (NewZdbProt.protectinfo(terminalno,PROTTYPE_NEWZDB,gram_len,gram));//ǿ��ת����Լ����
	}*/
	

	if(m_Buf[16] == 0xa3)		//�����¼�
	{
		//΢�����������¼�
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�������ɶ����¼�");
		char text[60];
		sprintf(text,"�ն˺�:%d\t��Լ����:%d\t��վ��ַ:%d\t��վ��ַ:%d\t",terminalno,protocoltype,address1,address2);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
		if(mdlp == NULL) return PROT_ERR_MODULE;
		int actiontype=m_Buf[32]+m_Buf[33]*256;//ģ������-----------װ������ 
		make_action_apevent(&sys_clock,mdlp,m_Buf+32);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣�涯���¼�");
		ret = save_action_apevent(&sys_clock,mdlp,m_Buf+32);

	}
	else if(m_Buf[16] == 0xe4)	//�Լ��¼�------�쳣�¼�
	{
		if(mdlp == NULL) return PROT_ERR_MODULE;
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣���Լ��¼�");
		ret = save_detect_apevent(&sys_clock,mdlp,m_Buf+26);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//���ɱ�������Ӧ���¼�
void ZDB_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	ZDB_PROTECTACTION *zdb_action;
	short	actioncode;
	SYS_CLOCK  sys_clock;

	if(mdlp == NULL || sysclock == NULL)return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));
	msg.type_value = event[0]+event[1]*256	;	//��������-----------װ������
	unsigned long troublebit,tempdword;
	memcpy(&troublebit,event+2,4);//����λ
	tempdword=troublebit;

	int nn=0;
	for (int i=0;i<32;i++)//����n����������Ӧ���¼�
	{
		tempdword=tempdword>>i;
		int tempint=tempdword&0x00000001;
		if (tempint)
		{
			actioncode=i;	//���--------------------����λ
			GetProtinfoTime(event+6+nn*8, TIMEBYBCD, &sys_clock);	//����ʱ��
			make_common_apentinfo(&msg,&sys_clock,mdlp,0,PROTTYPE_ZDB);	//������Ϣ

			if((zdb_action = GetActionPara(msg.type_value,actioncode))==NULL)return;	//ȡ������Ϣ
			msg.sgflag = zdb_action->sgflag;
			strcpy(msg.enttypedescribe,zdb_action->actioninfo);		//�¼���������
			apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
			nn++;
		}
		tempdword=troublebit;
	}
}


//�����������
int ZDB_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN],longdes[120];
	ZDB_PROTECTACTION *zdb_action;
	short actioncode;
	SYS_CLOCK  sys_clock;
	NetDbg dbg;
	ZDB_PROTECTMEASURE *zdb_measure;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	//make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	/////////���ɴ�������//////////////////////////////////////////////////
	tmpevent.evt.state = event[0]+event[1]*256	;	//��������-----------װ������
	unsigned long  troublebit,tempdword;
	memcpy(&troublebit,event+2,4);//����λ
	tempdword=troublebit;
	
	//*ͳ�ƶ����ĸ���
	int actionnum=0;
	int j;
	for (j=0;j<32;j++)
	{
		tempdword=tempdword>>j;
		int temp1=tempdword&0x01;
		if (temp1)
		{
			actionnum++;
		}
		tempdword=troublebit;
	}
	int measurenum=(m_GramLength-35-8*actionnum)/4;//��������ĸ���
	int tempint1=0;
	float xisu=0.00;
	int nn=0;
	for (int i=0;i<32;i++)//����n����������Ӧ���¼�
	{
		tempdword=tempdword>>i;
		int tempint=tempdword&0x00000001;
		if (tempint)
		{
			actioncode=i;	//���--------------------����λ
			GetProtinfoTime(event+6+nn*8, TIMEBYBCD, &sys_clock);	//����ʱ��
			make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 0);	//������Ϣ

			if((zdb_action = GetActionPara(tmpevent.evt.state,actioncode))==NULL)return PROT_ERR_ACTION;	//ȡ������Ϣ
			sprintf(tmpevent.evt.member1,"%d",zdb_action->actioncode);
			strcpy(tmpevent.member_des1,zdb_action->actioninfo);
			tmpevent.evt.ToneOut = zdb_action->alarmf;
			strcpy(entname,zdb_action->entname);
			
			//��������ֵ�����������
			for(j=0;j<measurenum;j++)
			{
				if (nn>0) break;
				zdb_measure = GetMeasurePara(event[6+8*actionnum+j*4+1]);
				if(zdb_measure){
					tempint1=(int)zdb_measure->measurepoint;
					switch(tempint1)
					{
					case 0:xisu=1;break;
					case 1:xisu=(float)0.01;break;
					case 2:xisu=(float)100/2047;break;
					case 3:xisu=(float)5/2047;break;
					case 4:xisu=(float)1500/2047;break;
					case 5:xisu=(float)1/2047;break;
					default :break;
					}
					tmpevent.evt.val = (float)(event[6+8*actionnum+j*4+2]+event[6+8*actionnum+j*4+3]*256)*xisu;	//����ֵ
					sprintf(longdes,"%s:%4.2f%s",
						zdb_measure->measureinfo,(float)tmpevent.evt.val,zdb_measure->measureunit);
					if((strlen(tmpevent.longdes)+strlen(longdes))>=120) break;
					strcat(tmpevent.longdes,longdes);
				}
			}

			yield_entdescribe(entname,&tmpevent);
			if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
				//2003 03
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��ʼ���涯������");
			NetStatus netcfg;
			int groupnum=netcfg.GetHostGrpNum();
			int *grp = netcfg.GetHostGrpEnm();
			for(int no=0;no<groupnum;no++)
			{
				tmpevent.evt.grpno=grp[no];
				save_event(&tmpevent,1);
			}//for--no
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���涯���������");
			nn++;
		}
		tempdword=troublebit;
	}

	return OK;
}

//�Լ��������
int ZDB_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	ZDB_PROTECTDETECT *zdb_detect;
	NetDbg dbg;
	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	unsigned long troublebit,tempdword;
	memcpy(&troublebit,event,4);//����λ
	tempdword=troublebit;
	for (int i=0;i<32;i++)
	{
		tempdword=tempdword>>i;
		int tempint=tempdword&0x00000001;
		if (tempint)
		{
			tmpevent.evt.state = i;	//״ֵ̬---�Լ��¼�����
			if((zdb_detect = GetDetectPara(tmpevent.evt.state))==NULL)return PROT_ERR_DETECT;
			sprintf(tmpevent.evt.member1,"%d",zdb_detect->detectcode);
			strcpy(tmpevent.member_des1,zdb_detect->detectinfo);

			yield_entdescribe(zdb_detect->entname,&tmpevent);
			tmpevent.evt.ToneOut = zdb_detect->alarmf;
			if(tmpevent.evt.ToneOut)yield_snddescribe(zdb_detect->entname,&tmpevent);
				//2003 03
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"��ʼ�����Լ�����");
			NetStatus netcfg;
			int groupnum=netcfg.GetHostGrpNum();
			int *grp = netcfg.GetHostGrpEnm();
			for(int no=0;no<groupnum;no++)
			{
				tmpevent.evt.grpno=grp[no];
				save_event(&tmpevent,1);
			}//for--no
			break;
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�����Լ��������");
		}
		tempdword=troublebit;
	}
	return OK;
}


int ZDB_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock)
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