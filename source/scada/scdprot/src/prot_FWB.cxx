
///////////////////////////////////////////////////////////////////////////////////
//////////////////////  FWB������ /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#include "procthread.h"
#include "prot_FWB.h"


PROTECTMDL *FWB_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) &&(mdlp->address1==address1)) 
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//ȡ������Ϣ
FWB_PROTECTACTION* FWB_Protect::GetActionPara(unsigned short moduletype, short actioncode)
{
	ACTION_KEY1 key;
	key.moduletype = moduletype;
	key.actioncode = actioncode;
	return ((FWB_PROTECTACTION *)table.RdbGetRecordAddr(FWB_PROTACTION_TABNAME,&key)); 
}

//ȡ�Լ���Ϣ
FWB_PROTECTDETECT* FWB_Protect::GetDetectPara(unsigned char chkbit)
{
	return ((FWB_PROTECTDETECT *)table.RdbGetRecordAddr(FWB_PROTDETECT_TABNAME,&chkbit)); 
}


int  FWB_Protect::GetFWBProtinfoTime(uchar *gram_part,SYS_CLOCK *clock)
{      
	GetCurrentClock(clock);
    clock->msecond=gram_part[0]+gram_part[1]*256;
    return OK;
}

int FWB_Protect::GetTime(SYS_CLOCK *clock)
{
   GetCurrentClock(clock);
   return OK;
}


//FEP���������������Ľ���
int FWB_Protect::protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram)
{
	SYS_CLOCK sys_clock;
	PROTECTMDL	*mdlp;	//΢����������ָ��
	int ret ;

	if(gram_len<5) return PROT_ERR_MAILLEN;

    mdlp = GetModulePara(terminalno,protocoltype,gram[2]);  // gram[2]Ϊ��ַ��
    if(mdlp == NULL)return PROT_ERR_MODULE;

	if(gram[4] == 0)		// �����¼�����---���¼�����
	{       
		GetFWBProtinfoTime(gram+7,&sys_clock);		//����ʱ��
		make_action_apevent(&sys_clock,mdlp,gram+5);
		ret = save_action_apevent(&sys_clock,mdlp,gram+5);
	}
	else if(gram[4] == 0x0f)		//�Լ챨��
	{
		GetTime(&sys_clock);
		ret = save_detect_apevent(&sys_clock,mdlp,gram+6);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}


//���ɱ�������Ӧ���¼�
void FWB_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	FWB_PROTECTACTION *fwb_action;

	if(mdlp == NULL || sysclock == NULL)return;
	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FWB);	//������Ϣ
	msg.type_value = event[0];			//��������
	msg.event_float_info[0] = 0.0f;
	
	if((fwb_action = GetActionPara(mdlp->type2, msg.type_value))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = fwb_action->sgflag;
	strcpy(msg.enttypedescribe,fwb_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int FWB_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];

	entname[0] = 0;
	FWB_PROTECTACTION *fwb_action;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);

	tmpevent.evt.DiskOut = 1;			//���̱�־
	tmpevent.evt.PrintOut = 1;			//��ӡ��־
	tmpevent.evt.DispOut = 1;			//��¼��־
	tmpevent.evt.GraphOut = 0;			//��ͼ��־
	tmpevent.evt.ToneOut = 0;

	/////////���ɴ�������//////////////////////////////////////////////////

	tmpevent.evt.state = event[0];			//��������-----״ֵ̬
	tmpevent.evt.val = 0.0f;	//����ֵ
	fwb_action = GetActionPara(mdlp->type2,event[0]);	//ȡ������Ϣ
	if(fwb_action) {
		sprintf(tmpevent.evt.member1,"%d",fwb_action->actioncode);
		strcpy(tmpevent.member_des1,fwb_action->actioninfo);
		tmpevent.evt.ToneOut = fwb_action->alarmf;
		strcpy(entname,fwb_action->entname);
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


int FWB_Protect::save_detect_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)	//�����Լ��¼�
{ 
	SCDEVENT tmpevent;	//�����¼�
	int i = 0,bitnums;
	unsigned short eventinfo;
	uchar chkbit;
	FWB_PROTECTDETECT *fwb_detect;
         
	chkbit=-1;
    bitnums=5;
	eventinfo = event[0];
	for(i=0;i<bitnums;i++)
	{
		if(eventinfo & 0x01) {
			eventinfo = eventinfo>>1;
			chkbit = i;
		}
		else {
			eventinfo = eventinfo>>1;
			continue;
		}
		fwb_detect = GetDetectPara(chkbit);
		if(fwb_detect == NULL) continue ;
        memset(&tmpevent,0,sizeof(SCDEVENT));
        make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);

        tmpevent.evt.DiskOut = 1;                  //���̱�־
        tmpevent.evt.PrintOut = 1;                 //��ӡ��־
        tmpevent.evt.DispOut = 1;                  //��¼��־
		tmpevent.evt.ToneOut = fwb_detect->alarmf;		//���챨����־

//		sprintf(tmpevent.evt.member1,"%d",fwb_detect->detectcode);//������1--��������
		strcpy(tmpevent.member_des1,fwb_detect->detectinfo);	//������1����---��������
		yield_entdescribe(fwb_detect->entname,&tmpevent);
		if(tmpevent.evt.ToneOut)yield_snddescribe(fwb_detect->entname, &tmpevent);
		//2003 03
		NetStatus netcfg;
		int groupnum=netcfg.GetHostGrpNum();
		int *grp = netcfg.GetHostGrpEnm();
		for(int no=0;no<groupnum;no++)
		{
			tmpevent.evt.grpno=grp[no];
			save_event(&tmpevent,1);
		}//for--no
	}
	return OK;
}

