#ifndef _PROT_AHJY_H
#define _PROT_AHJY_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"

									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			AHJY������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		char    entname[ENTNAME_LEN];		//������
} AHJY_PROTECTACTION;

							//////////////////////////////////////////////////////////////
							/*						AHJY ΢������������			*/
							//////////////////////////////////////////////////////////////
class AHJY_Protect : public BaseProtect
{
public:
	AHJY_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	AHJY_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	
	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	
	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
