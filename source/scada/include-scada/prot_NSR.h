#ifndef _PROT_NSR_H
#define _PROT_NSR_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					NSR						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			NSR_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} NSR_PROTECTACTION;

										//////////////////////////////////////////////
										/*			NSR_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} NSR_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			NSR_����ֵ��Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} NSR_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			NSR_�������Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} NSR_PROTECTDETECT;



							//////////////////////////////////////////////////////////////
							/*						NSR ΢������������			*/
							//////////////////////////////////////////////////////////////
class NSR_Protect : public BaseProtect
{
public:
	NSR_Protect(void);

	//ʱ��ת��
	int GetProtinfoTime(uchar *gram_part,uchar binorbcd,SYS_CLOCK *clock);
	NSR_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	NSR_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//ȡ������Ϣ����
	NSR_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//ȡ����ֵ��Ϣ����
	NSR_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	int GetActionInfo(short actiontype,char *name,char *info);	//ȡ������������
	int GetDetectInfo(short detecttype,char *name,char *info);	//ȡ�����Լ���Ϣ����

	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	int GetTroubleInfo(short troubletype,char *info);//ȡ������������

	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
