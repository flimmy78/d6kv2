#ifndef _PROT_NEWZDB_H
#define _PROT_NEWZDB_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					NEWZDB						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			NEWZDB�¼���Ϣ������				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} NEWZDB_PROTECTACTION;


										//////////////////////////////////////////////
										/*			IEC103�¼���Ϣ������				*/
										//////////////////////////////////////////////
typedef	struct {
		short devicetype;						//װ������
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} IEC103_PROTECTACTION;
	
typedef struct {
		ushort devicetype;						//װ������
		ushort  actioncode;						//���
	} IEC103_ACTION_KEY;

						//////////////////////////////////////////////////////////////
							/*						NEWZDB ΢������������			*/
							//////////////////////////////////////////////////////////////
class NEWZDB_Protect : public BaseProtect
{

public:
	NEWZDB_Protect(void);
	ushort address1,address2;//��վ��ַ����վ��ַ
	int m_GramLength;//���ĳ���
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	NEWZDB_PROTECTACTION	*GetActionPara(short actioncode);		//ȡ�¼���Ϣ����
	IEC103_PROTECTACTION	*GetIEC103ActionPara(unsigned short devicetype,unsigned short actioncode);

	unsigned char m_Buf[1024];
	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//ʱ��ת��
	int GetProtinfoTime(uchar *gram_part,uchar binorbcd,SYS_CLOCK *clock);
	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
