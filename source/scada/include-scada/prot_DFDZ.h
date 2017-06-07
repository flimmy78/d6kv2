#ifndef _PROT_DFDZ_H
#define _PROT_DFDZ_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					DFDZ						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			DFDZ������Ϣ������				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actiontype;						//ģ������
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} DFDZ_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DFDZ�Լ���Ϣ������			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} DFDZ_PROTECTDETECT;

										//////////////////////////////////////////////
										/*			DFDZ�������Ͳ�����			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		float   measurepoint;					//ϵ��
		char	measureunit[PROT_UNIT_LEN];		//��λ
} DFDZ_PROTECTMEASURE;

typedef struct 
{
		ushort   moduletype;
		ushort	actioncode;
} ACTION_KEY_DFDZ;



							//////////////////////////////////////////////////////////////
							/*						DFDZ ΢������������			*/
							//////////////////////////////////////////////////////////////
class DFDZ_Protect : public BaseProtect
{

public:
	DFDZ_Protect(void);
	ushort m_address1,m_address2,m_address3,m_address4;//��ַ
	int m_GramLength;//���ĳ���
	unsigned char m_Buf[1024];
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2,ushort address3,ushort address4);
	DFDZ_PROTECTACTION	*GetActionPara(short actiontype,short actioncode);		//ȡ������������
	DFDZ_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����
	DFDZ_PROTECTMEASURE	*GetMeasurePara(short measurecode);		//ȡ������Ϣ����

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
