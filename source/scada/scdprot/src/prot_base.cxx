#include  "net/netdef.h"
#include  "rdb/rdbapi.h"
#include  "protect.h"
#include  "prot_ISA.h"
#include  "prot_ISA300.h"
#include  "prot_LFP.h"
#include  "prot_DFP.h"
#include  "prot_SEL.h"
#include  "prot_FWB.h"
#include  "prot_DF3003.h"
#include  "prot_CAN2000.h"
#include  "prot_WBX.h"
#include  "prot_DFP500.h"
#include  "prot_LSA_P.h"
#include  "prot_DSA301.h"
#include  "prot_CSC2000.h"
#include  "prot_FBZ.h"
#include  "prot_DWK.h"
#include  "procthread.h"
#include  "prot_DSA301.h"
#include  "prot_ZDB.h"
#include  "prot_DF3003_NEW.h"
#include  "prot_PS.h"
#include  "prot_DEP.h"
#include  "prot_NEWZDB.h"
#include  "prot_NSR.h"
#include  "prot_SACDW.h"
#include  "prot_SACJD.h"
#include  "prot_DF33.h"	//add 2006 09
#include  "prot_NZ103.h"//add 2006 11  for ��������103
#include  "prot_RCS_103.h"//add 2006 12  for ���� RCS 103
#include  "prot_DMP.h"	//add 2007 05 for ����DMP
#include  "prot_DFDZ.h" //add 2007 11 for ��������
#include  "prot_AHJY.h"//add 2007 12 for ���ռ�Զ�ۺ��Զ���

	//////////////////////////////////////////////////////////////////////////////////
	/*																				*/
	/*									΢�������������							*/
	/*																				*/
	//////////////////////////////////////////////////////////////////////////////////

Rdb_QuickQuery BaseProtect::table(MyUserName,MyPassWord);
Rdb_Tab BaseProtect::promdltab;
int BaseProtect::opentabf;

BaseProtect::BaseProtect()
{
	if(opentabf==0)
	{
		opentabf=2;
		if(promdltab.RdbLogin(MyUserName,MyPassWord)!=RDB_OK){opentabf=0;return;}
		if(promdltab.RdbOpenTab(PROTMDL_TABNAME)!=RDB_OK){opentabf=0;return;}
		opentabf=1;
	}
}
							//////////////////////////////////////////
							/*				ʱ��ת������			*/
							//////////////////////////////////////////
int BaseProtect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock,uchar chbyte,uchar nomsflag)
{
	char timeprtstr[128];

	if(nomsflag) 
		sprintf(timeprtstr,"ProtTime%d: ����:%.2x ʱ��:%.2x-%.2x-%.2x %.2x:%.2x:%.2x\n",
			binorbcd,chbyte,gram_part[0],gram_part[1],gram_part[2],gram_part[3],gram_part[4],
			gram_part[5]);
	else
		sprintf(timeprtstr,"ProtTime%d: ����:%.2x ʱ��:%.2x-%.2x-%.2x %.2x:%.2x:%.2x %.2x-%.2x\n",
			binorbcd,chbyte,gram_part[0],gram_part[1],gram_part[2],gram_part[3],gram_part[4],
			gram_part[5],gram_part[6],gram_part[7]);
	RdbDebugMesg(timeprtstr);

	if(binorbcd==TIMEBYBIN){ // ������ʱ���ʽ
		if(chbyte==0xFF) {
			if(gram_part[0] < 98)
				clock->year = gram_part[0] + 2000;
			else
				clock->year = gram_part[0] + 1900;
		}
		else clock->year = gram_part[0] + chbyte * 256;

		clock->month=gram_part[1];
		clock->day=gram_part[2];
		clock->hour=gram_part[3];
		clock->minute=gram_part[4];
		clock->second=gram_part[5];
		if(nomsflag==0)clock->msecond=gram_part[6]+gram_part[7]*256;
		else clock->msecond=0;
	}
	else {	//TIMEBYBCD      BCD��ʱ���ʽ
		if(chbyte==0xFF) {
			if(BCD1BIN1(gram_part[0]) < 98)
				clock->year = BCD1BIN1(gram_part[0]) + 2000;
			else
				clock->year = BCD1BIN1(gram_part[0]) + 1900;
		}
		else clock->year = BCD1BIN1(gram_part[0]) + BCD1BIN1(chbyte) * 100;

		clock->month=BCD1BIN1(gram_part[1]);
		clock->day=BCD1BIN1(gram_part[2]);
		clock->hour=BCD1BIN1(gram_part[3]);
		clock->minute=BCD1BIN1(gram_part[4]);
		clock->second=BCD1BIN1(gram_part[5]);
		if(nomsflag==0) clock->msecond=BCD1BIN1(gram_part[6])+BCD1BIN1(gram_part[7])*100;
		else clock->msecond=0;
	}
	return OK;
}

							//////////////////////////////////////////////////
							/*			����ͬ������ʽȡ΢����������		*/
							//////////////////////////////////////////////////

//ISA��LSAP��DFP500��DSA301�ı���ģ�飬ֻ����TERMINALNO��ADDRESS1��ADDRESS2
PROTECTMDL *BaseProtect::GetModulePara(ushort terminalno,ushort address1,ushort address2)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->address1==address1) && (mdlp->address2==address2)) 
		{
			if(mdlp->prottype==PROTTYPE_ISA || mdlp->prottype==PROTTYPE_ISA300 || 
				mdlp->prottype==PROTTYPE_LSA_P || mdlp->prottype==PROTTYPE_DFP500 ||
				mdlp->prottype==PROTTYPE_DSA_301)
			return(mdlp);
		}
	}
	return(NULL);
}

PROTECTMDL *BaseProtect::GetModulePara(ushort terminalno,ushort prottype,ushort type1,ushort type2,ushort type3,ushort type4,ushort address1,ushort address2,ushort address3,ushort address4)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->type1==type1) &&
			(mdlp->type2==type2) && (mdlp->type3==type3) && (mdlp->type4==type4) &&	(mdlp->address1==address1) &&
			(mdlp->address2==address2) &&(mdlp->address3==address3) && (mdlp->address4==address4)) 
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//�γɹ��ô����¼���Ϣ
void BaseProtect::make_common_saveinfo(SCDEVENT *tmpevent, SYS_CLOCK *sysclock, PROTECTMDL *mdlp, int entflag)
{
	SUBSTATION_STRU *m_pSubStation; //substation
	if(tmpevent == NULL)return;
	tmpevent->evt.sort = ENTCLASS_PROTECT;				//�¼������
	if(entflag == 0) tmpevent->evt.type = ENTTYPE_PROTECT;				//�¼�����
	else if(entflag == 1) tmpevent->evt.type = ENTTYPE_PROTECT_DETECT;
	else tmpevent->evt.type = ENTTYPE_RELAYFAULT;

	//add 2007 04 begin
	if ((mdlp->type3 != 0) && (mdlp->prottype != PROTTYPE_FORTYTWO))
	{//����3��Ϊ���ʾ�Է������յ�������Ļ���ʱ����Ϊ�����ʱ�䣬�������͵�ʱ����Ч
		SYS_CLOCK curclock;
		GetCurrentClock(&curclock);
		sysclock = &curclock;
	}
	//add 2007 04 end


	if(sysclock) {
		tmpevent->evt.ymd = sysclock->year*10000 + sysclock->month*100 + sysclock->day;	//������
		tmpevent->evt.hmsms = sysclock->hour*10000000 + sysclock->minute*100000				//ʱ�������
										+ sysclock->second*1000 + sysclock->msecond;
	}

	if(mdlp) {
		strcpy(tmpevent->evt.Object,mdlp->substation);					//����
		m_pSubStation=(SUBSTATION_STRU *)table.RdbGetRecordAddr(STATION_TABNAME,mdlp->substation);	//������
		if(m_pSubStation!=NULL)strcpy(tmpevent->group_des,m_pSubStation->describe);
		strcpy(tmpevent->evt.member0,mdlp->name);										//������0
		strcpy(tmpevent->member_des0,mdlp->describe);												//������0����
		tmpevent->evt.security = mdlp->securegrade;											//��ȫ��
	}
	return;
}

//�γɹ���Ӧ���¼���Ϣ
void BaseProtect::make_common_apentinfo(PROTECT_MSG *msg, SYS_CLOCK *sysclock, PROTECTMDL *mdlp, int entflag,int protocoltype)
{
	if(msg == NULL || sysclock == NULL)return;
	msg->clock = *sysclock;
	if(mdlp) {
		strcpy(msg->name,mdlp->name);
		strcpy(msg->substation,mdlp->substation);
	}
	msg->protocoltype = protocoltype;
	msg->eventtype = entflag;
	return;
}

//����FEP�����������ʼ�
int Protect_Scdservice_serv::yieldprotectinfo(SCADA_PROT_STRU* protectp,int  datalen)
{
	int ret;
	scdhostbyte(RDB_DATATYPE_SHORT,&protectp->terminalno);
	NetDbg dbg;

	if (protectp->protocoltype == PROTTYPE_ISA)
	{
		PROTECTMDL	*mdlp=NULL;		//΢������ģ�����ָ��
		BaseProtect     c_rxprot;
		mdlp = c_rxprot.GetModulePara(protectp->terminalno,protectp->gram[0],protectp->gram[12]);			//΢����������
		if(mdlp!=NULL)  protectp->protocoltype = (uchar)mdlp->prottype;
	}

	switch(protectp->protocoltype)
	{
	case PROTTYPE_CAN2000:
		{
			CAN2000_Protect prot;
			RdbDebugMesg("�յ�CAN2000΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DF3003:
		{
			DF3003_Protect prot;
			RdbDebugMesg("�յ�DF3003΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DFP:
		{
			DFP_Protect prot;
			RdbDebugMesg("�յ�DFP΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DFP500:
		{
			DFP500_Protect prot;
			RdbDebugMesg("�յ�DFP500΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DSA_301:
		{
			DSA_301_Protect prot;
			RdbDebugMesg("�յ�DSA_301΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FWB:
		{
			FWB_Protect prot;
			RdbDebugMesg("�յ�FWB΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_ISA:
		{
			ISA_Protect prot;
			RdbDebugMesg("�յ�ISA΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_ISA300:
		{
			ISA300_Protect prot;
			RdbDebugMesg("�յ�ISA300΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_LFP:
		{
			LFP_Protect prot;
			RdbDebugMesg("�յ�LFP΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_LSA_P:
		{
			LSA_P_Protect prot;
			RdbDebugMesg("�յ�LSA_P΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;

	case PROTTYPE_SEL:
		{
			SEL_Protect prot;
			RdbDebugMesg("�յ�SEL΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_WBX:
		{
			WBX_Protect prot;
			RdbDebugMesg("�յ�WBX΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_CSC2000:
		{
			CSC2000_Protect prot;
			RdbDebugMesg("�յ�CSC2000΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FBZ:
		{
			FBZ_Protect prot;
			RdbDebugMesg("�յ�FBZ΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DWK:
		{
			DWK_Protect prot;
			RdbDebugMesg("�յ�DWK��ѹ�޹��ۺ�װ���ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_ZDB://2003 03
		{
			ZDB_Protect prot;
			RdbDebugMesg("�յ�ZDB΢�������ʼ�\n");
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�յ�ZDB΢�������ʼ�");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_NEWZDB://2004 08
		{
			NEWZDB_Protect prot;
			RdbDebugMesg("�յ�NEWZDB΢�������ʼ�\n");
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�յ�NEWZDB΢�������ʼ�");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DF3003_NEW://2003 03
		{
			DF3003_NEW_Protect prot;
			RdbDebugMesg("�յ�DF3003_NEW΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DEP://2004 06
		{
			DEP_Protect prot;
			RdbDebugMesg("�յ�DEP΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_PS://2004 05
		{
			PS_Protect prot;
			RdbDebugMesg("�յ�PS6000΢�������ʼ�\n");
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�յ�PS6000΢�������ʼ�");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_SACDW:
		{
			SACDW_Protect prot;
			RdbDebugMesg("�յ�SACDW΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DF33://add 2006 09 ƽ�� 
		{
			DF33_Protect prot;
			RdbDebugMesg("�յ�DF33΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FORTYFIVE://add 2006 11  for ��������103
		{
			NZ103_Protect prot;
			RdbDebugMesg("�յ�NZ103΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FORTYSEVEN://add 2006 12  for ���� RCS����103
	case PROTTYPE_FORTYNINE://add 2007 06  for ���� ����103
		{
			RCS_103_Protect prot;
			RdbDebugMesg("�յ�RCS103΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FORTYEIGHT://add 2007 05  for ����DMP
		{
			DMP_Protect prot;
			RdbDebugMesg("�յ�DMP΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FIFTYZERO://add 2007 11  for ��������
		{
			DFDZ_Protect prot;
			RdbDebugMesg("�յ���������΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FIFTYONE://add 2007 12  for ���ռ�Զ�ۺ��Զ���
		{
			AHJY_Protect prot;
			RdbDebugMesg("�յ����ռ�Զ�ۺ��Զ���΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_SACJD:
		{
			SACJD_Protect prot;
			RdbDebugMesg("�յ�SACJD΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_NSR:
		{
			NSR_Protect prot;
			RdbDebugMesg("�յ�NSR΢�������ʼ�\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	default:
		{
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"�޷�ʶ��Ĺ�Լ����");
			ret = PROT_ERR_NOTSUPPORT;   
			break;
		}
	}
	return ret;
}


