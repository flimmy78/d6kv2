#ifndef _PROT_RCS_103_H
#define _PROT_RCS_103_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
#include "prot_LFP.h"
									//////////////////////////////////////////////////////
									/*					RCS_103						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			RCS_103������Ϣ������				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actiontype;						//ģ������
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} RCS_103_PROTECTACTION;

										//////////////////////////////////////////////
										/*			RCS_103�Լ���Ϣ������			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detecttype;						//ģ������
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
		uchar	sgflag;							//�Ƿ����¹�
   		char    entname[ENTNAME_LEN];		//���������
} RCS_103_PROTECTDETECT;


typedef struct 
{
		ushort   moduletype;
		ushort	actioncode;
} ACTION_KEY_RCS_103;



							//////////////////////////////////////////////////////////////
							/*						RCS_103 ΢������������			*/
							//////////////////////////////////////////////////////////////
class RCS_103_Protect : public BaseProtect
{

public:
	RCS_103_Protect(void);
	ushort address1,address2;//��վ��ַ����վ��ַ
	int m_GramLength;//���ĳ���
	unsigned char m_Buf[1024];
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	RCS_103_PROTECTACTION	*GetActionPara(short actiontype,short actioncode);		//ȡ������������
	RCS_103_PROTECTDETECT	*GetDetectPara(short detecttype,short detectcode);		//ȡ�Լ���Ϣ����

	LFP_PROTECTACTION *GetLFPActionPara(short moduletype,unsigned char boardnum,short actbit);
	LFP_PROTECTDETECT *GetLFPDetectPara(short moduletype,unsigned char flagtype,short chkbit);


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

	//����Ӧ���¼�
	void make_LFPaction_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_LFPaction_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_LFPdetect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
