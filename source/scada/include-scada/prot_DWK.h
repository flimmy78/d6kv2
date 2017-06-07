#ifndef _PROT_DWK_H
#define _PROT_DWK_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					DWK						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			DWK_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
    	char    entname[ENTNAME_LEN];		//������
} DWK_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DWK_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} DWK_PROTECTTROUBLE;



							//////////////////////////////////////////////////////////////
							/*						DWK ΢������������			*/
							//////////////////////////////////////////////////////////////
class DWK_Protect : public BaseProtect
{
public:
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	DWK_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	DWK_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//ȡ������Ϣ����

	int GetActionInfo(short actiontype,char *name,char *info);	//ȡ������������
	int GetTroubleInfo(short troubletype,char *info);//ȡ������������

	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event,int restoreflag);
};

#endif
