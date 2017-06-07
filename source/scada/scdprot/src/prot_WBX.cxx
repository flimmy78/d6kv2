#include "scadaproc.h"
#include "procthread.h"
#include "prot_WBX.h"

				//////////////////////////////////////////////////////////////
				/*					WBX ΢������������					*/
				//////////////////////////////////////////////////////////////

WBX_Protect::WBX_Protect(void)
{

}

PROTECTMDL *WBX_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
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
WBX_PROTECTACTION	*WBX_Protect::GetActionPara(ushort actiontype,ushort actioncode)
{
	struct ACTION_KEY{
		ushort actiontype;
		ushort actioncode;
	}actionkey;
	actionkey.actiontype = actiontype;
	actionkey.actioncode = actioncode;
	return (WBX_PROTECTACTION *)table.RdbGetRecordAddr(WBX_PROTACTION_TABNAME,&actionkey);
}


//����FEP����������������
int WBX_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//΢����������ָ��
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	gram[5] = gram[4];
	GetProtinfoTime(gram+5, TIMEBYBCD, &sys_clock);	//����ʱ��
	mdlp = GetModulePara(terminalno,protocoltype,gram[0],gram[3]);			//΢����������
			//gram[0]������ַ��gram[3]CPU��
	if(gram[1] == 0xb0)		//�����¼�
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		make_action_apevent(&sys_clock,mdlp,gram+13);
		ret = save_action_apevent(&sys_clock,mdlp,gram+13);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//���ɱ�������Ӧ���¼�
void WBX_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//Ӧ���¼�������
	PROTECT_MSG msg;
	WBX_PROTECTACTION *WBX_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_WBX);	//������Ϣ
	msg.type_value = event[0];			//��������
	
	if((WBX_action = GetActionPara(event[0],event[1]))==NULL)return;	//ȡ������Ϣ
	msg.sgflag = WBX_action->sgflag;
	strcpy(msg.enttypedescribe,WBX_action->actioninfo);		//�¼���������
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}

//�����������
int WBX_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//�����¼�
	char entname[ENTNAME_LEN];
	int i = 0;

	entname[0] = 0;

	WBX_PROTECTACTION *WBX_action;

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

	WBX_action = GetActionPara(event[0],event[1]);	//ȡ������Ϣ
	if(WBX_action) {
		sprintf(tmpevent.evt.member1,"%d",WBX_action->actioncode);
		strcpy(tmpevent.member_des1,WBX_action->actioninfo);
		tmpevent.evt.ToneOut = WBX_action->alarmf;
		strcpy(entname,WBX_action->entname);
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
	}
	return OK;
}

