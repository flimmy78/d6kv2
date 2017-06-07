#ifndef _PROT_SACJD_H
#define _PROT_SACJD_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					SACJD						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			SACJD�¼���Ϣ������				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;
		short	moduletype;				//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;					//�Ƿ񱨾�	
		uchar	sgflag;					//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} SACJD_PROTECTACTION;

typedef struct 
{
		ushort   moduletype;
		ushort	actioncode;
} ACTION_KEY_SACJD;


							//////////////////////////////////////////////////////////////
							/*						SACJD ΢������������			*/
							//////////////////////////////////////////////////////////////
class SACJD_Protect : public BaseProtect
{

public:
	SACJD_Protect(void);
	ushort address1;//��ַ(������ն����)
	int m_GramLength;//���ĳ���
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	SACJD_PROTECTACTION	*GetActionPara(short actioncode,short actiontype);		//ȡ�¼���Ϣ����
	SACJD_PROTECTACTION	*GetDetectPara(short actioncode,short actiontype);		//ȡ�¼���Ϣ����

	unsigned char m_Buf[1024];
	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	//���ɴ����¼�
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
