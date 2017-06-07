#ifndef _PROT_DEP_H
#define _PROT_DEP_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					DEP						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			DEP_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} DEP_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DEP_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} DEP_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			DEP_����ֵ��Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} DEP_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			DEP_�������Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} DEP_PROTECTDETECT;
									
							//////////////////////////////////////////////////////////////
							/*						DEP ΢������������			*/
							//////////////////////////////////////////////////////////////
class DEP_Protect : public BaseProtect
{
	static Rdb_Tab rdbtable;
	static int opentabf;
public:
	DEP_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	DEP_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	DEP_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//ȡ������Ϣ����
	DEP_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//ȡ����ֵ��Ϣ����
	DEP_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����

	int GetActionInfo(short actiontype,char *name,char *info);	//ȡ������������
	int GetDetectInfo(short detecttype,char *name,char *info);	//ȡ�����Լ���Ϣ����

	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	int GetTroubleInfo(short troubletype,char *info);//ȡ������������
	//ʱ��ת��
	int GetProtinfoTime(uchar *gram_part,uchar binorbcd,SYS_CLOCK *clock);
	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
