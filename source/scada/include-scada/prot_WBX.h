#ifndef _PROT_WBX_H
#define _PROT_WBX_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
									//////////////////////////////////////////////////////
									/*					WBX						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			WBX_�¼���Ϣ(wbx_action)				*/
										//////////////////////////////////////////////
typedef	struct {
		ushort   actiontype;						//�¼�����
		ushort	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} WBX_PROTECTACTION;

								
							//////////////////////////////////////////////////////////////
							/*						WBX ΢������������			*/
							//////////////////////////////////////////////////////////////
class WBX_Protect : public BaseProtect
{
public:
	WBX_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	WBX_PROTECTACTION	*GetActionPara(ushort actiontype,ushort actioncode);		//ȡ������������

	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
	
	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
