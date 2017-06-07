#ifndef _PROT_NZ103_H
#define _PROT_NZ103_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					NZ103						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			NZ103������Ϣ������				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actiontype;						//ģ������
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} NZ103_PROTECTACTION;

										//////////////////////////////////////////////
										/*			NZ103�Լ���Ϣ������			*/
										//////////////////////////////////////////////
typedef	struct {
		short	devicetype;						//ģ������
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
   		char    entname[ENTNAME_LEN];		//���������
} NZ103_PROTECTDETECT;

	

typedef struct 
{
		ushort   moduletype;
		ushort	actioncode;
} ACTION_KEY_NZ103;



							//////////////////////////////////////////////////////////////
							/*						NZ103 ΢������������			*/
							//////////////////////////////////////////////////////////////
class NZ103_Protect : public BaseProtect
{

public:
	NZ103_Protect(void);
	ushort address1,address2;//��վ��ַ����վ��ַ
	int m_GramLength;//���ĳ���
	unsigned char m_Buf[1024];
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	NZ103_PROTECTACTION	*GetActionPara(short actiontype,short actioncode);		//ȡ������������
	NZ103_PROTECTDETECT	*GetDetectPara(short,short detecttype);		//ȡ�Լ���Ϣ����

	int GetDetectInfo(short,short detecttype,char *name,char *info);	//ȡ�����Լ���Ϣ����

	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//ʱ��ת��
	int GetProtinfoTime(uchar *gram_part,uchar binorbcd,SYS_CLOCK *clock);
	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
