/**
@file scadaserver.cpp
@brief ǰ��ƽ̨��װʵ��֮SCADA����ʵ��

@author hongxiang  
@version 1.0.0
@date 2013-11-13
*/
#include "fes/platformwrapper.h"
#include "scd/scdapi.h"
#include "scd/scddef.h"

static ScadaServer scadasrv;

int Fe_scasrv::fe_send_ykret(int rtuno, int ykno, int yk_ret,const char* msg)
{
	int flag = (yk_ret == -1 ? 0 : 1);		//todo �޸�ң�ط�У��Ϣ,�Ƿ�ֱ�Ӵ�ӡ��ʧ��ԭ��
	int ret = scadasrv.ForeSendYkCheckMesg(DEFAULT_GRP_NO, "",rtuno,ykno,flag);
	return ret == 1 ? 0 : -1;
}

int Fe_scasrv::fe_send_electric_event(const FE_DATA_EVENT_NODE* tmpEventData, int eventnum)		//todo
{
	int ret = scadasrv.ForeSendRawEventToScd("",(FE_DATA_EVENT_NODE*)tmpEventData, eventnum );
	return ret == 1 ? 0 : -1;

}

int Fe_scasrv::fe_send_prot_event( void* mail)
{
	SCADA_PROT_STRU *prot_ack_mail = (SCADA_PROT_STRU *)mail;
	int ret = scadasrv.ForeSendProtInfo(1,"",prot_ack_mail);
	return ret == 1 ? 0 : -1;
}