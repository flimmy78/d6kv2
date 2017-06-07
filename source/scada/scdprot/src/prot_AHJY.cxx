#include "scadaproc.h"
#include "procthread.h"
#include "prot_AHJY.h"

				//////////////////////////////////////////////////////////////
				/*					AHJY ΢������������				��Զ	*/
				//////////////////////////////////////////////////////////////


AHJY_Protect::AHJY_Protect(void)
{
}

PROTECTMDL *AHJY_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
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
AHJY_PROTECTACTION	*AHJY_Protect::GetActionPara(short actioncode)
{
	return (AHJY_PROTECTACTION *)table.RdbGetRecordAddr(AHJY_PROTACTION_TABNAME,&actioncode);
}


//����FEP����������������
int AHJY_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;
	NetDbg dbg;
	if(gram_len<5) return PROT_ERR_MAILLEN;

	gram[4]=gram[3];
	GetProtinfoTime(gram+4, TIMEBYBCD, &sys_clock);	//����ʱ��
	mdlp = GetModulePara(terminalno,protocoltype,gram[0],gram[12]);			//gram[0]��ַ1��gram[12]��ַ2��΢����������
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"AHJY΢�������ʼ����ȴ���5");
	if(mdlp == NULL)return PROT_ERR_MODULE;

	if(gram[1] == 0x40)		//�����¼�
	{
		make_action_apevent(&sys_clock,mdlp,gram+13);
		ret = save_action_apevent(&sys_clock,mdlp,gram+13);
	}
	else if(gram[1] == 0x51)	//�Լ��¼�
	{
		//ret = save_detect_apevent(&sys_clock,mdlp,gram+13);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//���ɱ�������Ӧ���¼�
void AHJY_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	AHJY_PROTECTACTION *AHJY_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FIFTYONE);	//������Ϣ
	msg.type_value = event[3];			//��������
	msg.event_int_info[0] = event[0];	//�¼����
	msg.event_int_info[1] = event[1];	//�ö��������ۼƷ�����
	msg.event_int_info[2] = event[2];	//�¹�����
	msg.event_int_info[3] = event[3];	//��������
	msg.event_int_info[4] = event[4];	//����ֵ����
	msg.event_float_info[0] = (float)(event[5]+event[6]*256)/100.0f;
	
	if((AHJY_action = GetActionPara(msg.type_value))==NULL)return;	//ȡ������Ϣ
	strcpy(msg.enttypedescribe,AHJY_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//�����������
int AHJY_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	AHJY_PROTECTACTION *AHJY_action;

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
	if((AHJY_action = GetActionPara(event[3])) == NULL) return PROT_ERR_ACTION;	//ȡ������Ϣ

	sprintf(tmpevent.evt.member1,"%d",AHJY_action->actioncode);
	strcpy(tmpevent.member_des1,AHJY_action->actioninfo);
	tmpevent.evt.ToneOut = AHJY_action->alarmf;
	strcpy(entname,AHJY_action->entname);

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

