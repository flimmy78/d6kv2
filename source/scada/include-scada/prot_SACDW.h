#ifndef _PROT_SACDW_H
#define _PROT_SACDW_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					SACDW						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			SACDW�¼���Ϣ������				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;
		short	moduletype;				//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;					//�Ƿ񱨾�	
		uchar	sgflag;					//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} SACDW_PROTECTACTION;

typedef struct 
{
		ushort   moduletype;
		ushort	actioncode;
} ACTION_KEY_SACDW;


							//////////////////////////////////////////////////////////////
							/*						SACDW ΢������������			*/
							//////////////////////////////////////////////////////////////
class SACDW_Protect : public BaseProtect
{

public:
	SACDW_Protect(void);
	ushort address1;//��ַ(������ն����)
	int m_GramLength;//���ĳ���
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	SACDW_PROTECTACTION	*GetActionPara(short actioncode,short actiontype);		//ȡ�¼���Ϣ����

	unsigned char m_Buf[1024];
	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

		//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
