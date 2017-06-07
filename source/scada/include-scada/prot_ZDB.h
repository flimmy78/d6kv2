#ifndef _PROT_ZDB_H
#define _PROT_ZDB_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					ZDB						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			ZDB������Ϣ������				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actiontype;						//ģ������
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} ZDB_PROTECTACTION;

										//////////////////////////////////////////////
										/*			ZDB�Լ���Ϣ������			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} ZDB_PROTECTDETECT;

										//////////////////////////////////////////////
										/*			ZDB�������Ͳ�����			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		float   measurepoint;					//ϵ��
		char	measureunit[PROT_UNIT_LEN];		//��λ
} ZDB_PROTECTMEASURE;

typedef struct 
{
		ushort   moduletype;
		ushort	actioncode;
} ACTION_KEY_ZDB;



							//////////////////////////////////////////////////////////////
							/*						ZDB ΢������������			*/
							//////////////////////////////////////////////////////////////
class ZDB_Protect : public BaseProtect
{

public:
	ZDB_Protect(void);
	ushort address1,address2;//��վ��ַ����վ��ַ
	int m_GramLength;//���ĳ���
	unsigned char m_Buf[1024];
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	ZDB_PROTECTACTION	*GetActionPara(short actiontype,short actioncode);		//ȡ������������
	ZDB_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����
	ZDB_PROTECTMEASURE	*GetMeasurePara(short measurecode);		//ȡ������Ϣ����

	int GetDetectInfo(short detecttype,char *name,char *info);	//ȡ�����Լ���Ϣ����

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
