#ifdef __alpha
#include<time.h>
#endif
#include "scadaproc.h"
#include "procthread.h"
#include "prot_DFDZ.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					DFDZ ΢������������					*/
				//////////////////////////////////////////////////////////////



DFDZ_Protect::DFDZ_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *DFDZ_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2,ushort address3,ushort address4)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1)
			&& (mdlp->address2==address2) && (mdlp->address3==address3) && (mdlp->address4==address4))
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//ȡ������������
DFDZ_PROTECTACTION	*DFDZ_Protect::GetActionPara(short actiontype,short actioncode)
{
	ACTION_KEY_DFDZ key;
	key.moduletype = actiontype;
	key.actioncode = actioncode;
	return (DFDZ_PROTECTACTION *)table.RdbGetRecordAddr(DFDZ_PROTACTION_TABNAME,&key);
}


//ȡ�Լ���Ϣ����
DFDZ_PROTECTDETECT	*DFDZ_Protect::GetDetectPara(short detectcode)
{
	return (DFDZ_PROTECTDETECT *)table.RdbGetRecordAddr(DFDZ_PROTDETECT_TABNAME,&detectcode);
}

//ȡ������Ϣ����
DFDZ_PROTECTMEASURE	*DFDZ_Protect::GetMeasurePara(short measurecode)
{
	return (DFDZ_PROTECTMEASURE *)table.RdbGetRecordAddr(DFDZ_PROTMEASURE_TABNAME,&measurecode);
}

//ȡ�����Լ���Ϣ����
int DFDZ_Protect::GetDetectInfo(short detecttype,char *name,char *info)
{
	DFDZ_PROTECTDETECT *detectp = GetDetectPara(detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}


//����FEP����������������
int DFDZ_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;
	NetDbg dbg;
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�յ�DFDZ�ı����¼��ʼ�");
	if(gram_len<10) return PROT_ERR_MAILLEN;

	int i,index =0;
	for (i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}

	uchar eventnums = m_Buf[0];				//�¼�����Ŀ
	m_address1=m_Buf[1];					//��ַ	��Ӧ΢���������������������еõ�ַ1
	m_address2=m_Buf[2];					//��ַ	��Ӧ΢���������������������еõ�ַ1
	m_address3=m_Buf[3];					//��ַ	��Ӧ΢���������������������еõ�ַ1
	m_address4=m_Buf[4];					//��ַ	��Ӧ΢���������������������еõ�ַ1
	mdlp = GetModulePara(terminalno,protocoltype,m_address1,m_address2,m_address3,m_address4);			//΢����������

	if(mdlp == NULL) return PROT_ERR_MODULE;
	

	index = 5;
	for (i=0;i<eventnums;i++)
	{
		if (m_Buf[index+3] == 0x0b)//�����¼�
		{
			//΢�����������¼�
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�������ɶ����¼�");
			char text[60];
			sprintf(text,"�ն˺�:%d\t��Լ����:%d\t��ַ:%d %d %d %d",terminalno,protocoltype,m_address1,m_address2,m_address3,m_address4);
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
			GetProtinfoTime(m_Buf+index+4, TIMEBYBIN, &sys_clock);		//����ʱ��
			make_action_apevent(&sys_clock,mdlp,m_Buf+index);
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣�涯���¼�");
			ret = save_action_apevent(&sys_clock,mdlp,m_Buf+index);
			uchar measurenum=m_Buf[index+8];
			index += 8;
			index += 1;
			index += measurenum *5;//��������ξ���3�� ��������Ϊ5��
		}
		else if (m_Buf[index+3] == 0x0a)//�Լ��¼�------�쳣�¼�
		{
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"���뱣���Լ��¼�");
			GetProtinfoTime(m_Buf+index+4, TIMEBYBIN, &sys_clock);		//����ʱ��
			ret = save_detect_apevent(&sys_clock,mdlp,m_Buf+index);
			index += 8;
		}
		else return PROT_ERR_MESSAGE;
	}


	return ret;
}

//���ɱ�������Ӧ���¼�
void DFDZ_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	DFDZ_PROTECTACTION *DFDZ_action;

	if(mdlp == NULL || sysclock == NULL) return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FIFTYZERO);	//������Ϣ
	short inf = event[0] + event[1]*256;
	msg.type_value = inf;			//��������
	
	if((DFDZ_action = GetActionPara(mdlp->type1,inf))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = DFDZ_action->sgflag;
	strcpy(msg.enttypedescribe,DFDZ_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int DFDZ_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN],longdes[120];
	int i = 0;

	entname[0] = 0;

	DFDZ_PROTECTACTION *DFDZ_action;
	DFDZ_PROTECTMEASURE *DFDZ_measure;

	if(mdlp == NULL || sysclock == NULL) return PROT_ERR_ACTION;

	short inf = event[0] +event[1]*256;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	/////////���ɴ�������//////////////////////////////////////////////////

	tmpevent.evt.state = inf;			//��������-----״ֵ̬

	if((DFDZ_action = GetActionPara(mdlp->type1,inf)) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ

	sprintf(tmpevent.evt.member1,"%d",DFDZ_action->actioncode);
	strcpy(tmpevent.member_des1,DFDZ_action->actioninfo);
	tmpevent.evt.ToneOut = DFDZ_action->alarmf;
	strcpy(entname,DFDZ_action->entname);

	uchar measurenum = event[8];

	if (measurenum>3) 
		measurenum = 3;//��������̫��������

	for(i=0;i<measurenum;i++)
	{
		short measureno = event[5*i +9];
		DFDZ_measure = GetMeasurePara(measureno);
		if(DFDZ_measure){
			float measureval =0;
			char ttt[4];
#if( ( defined(__sun) && defined(__sparc) ) || defined(_AIX) || defined(__hpux))	
			ttt[0] = event[9+ 5*i +4];
			ttt[1] = event[9+ 5*i +3];
			ttt[2] = event[9+ 5*i +2];
			ttt[3] = event[9+ 5*i +1];
#else
			ttt[0] = event[9+ 5*i +1];
			ttt[1] = event[9+ 5*i +2];
			ttt[2] = event[9+ 5*i +3];
			ttt[3] = event[9+ 5*i +4];
#endif

			memcpy(&measureval,ttt,4);
			tmpevent.evt.val = measureval;	//����ֵ
			sprintf(longdes,"%s:%4.2f%s",
				DFDZ_measure->measureinfo,(float)tmpevent.evt.val,DFDZ_measure->measureunit);
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
int DFDZ_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	DFDZ_PROTECTDETECT *DFDZ_detect;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	short inf = event[0] + event[1]*256 ;	//INFֵ---�Լ��¼�����
	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	tmpevent.evt.state = inf;
	if((DFDZ_detect = GetDetectPara(inf))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",DFDZ_detect->detectcode);
	strcpy(tmpevent.member_des1,DFDZ_detect->detectinfo);

	yield_entdescribe(DFDZ_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = DFDZ_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(DFDZ_detect->entname,&tmpevent);
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


int DFDZ_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock)
{
	if(clock == NULL)return -1;
	uchar hour,minute,second;
	short ms;
	ms = (gram_part[0] + gram_part[1]*256)%1000;
	second = (gram_part[0] + gram_part[1]*256)/1000;
	minute = gram_part[2];
	hour = gram_part[3];

	SYS_CLOCK sysclock;
	GetCurrentClock(&sysclock);	
	clock->year = sysclock.year;
	clock->month = sysclock.month;
	clock->day = sysclock.day;
	clock->hour=hour;
	clock->minute=minute;
	clock->second=second;
	clock->msecond = ms;
	return OK;
}