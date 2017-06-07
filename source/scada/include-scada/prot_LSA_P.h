#ifndef _PROT_LSA_P_H
#define _PROT_LSA_P_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
										//////////////////////////////////////////////
										/*			LSA_P_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} LSA_P_PROTECTACTION;

										//////////////////////////////////////////////
										/*			LSA_P_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} LSA_P_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			LSA_P_����ֵ��Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} LSA_P_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			LSA_P_�������Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} LSA_P_PROTECTDETECT;

										//////////////////////////////////////////////
										/*				LSA_P΢������������Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		ushort	protocoltype;
		ushort	precisiontype;
		char	precisionname[OBJECTNAME_LEN];
		char	precisioninfo[PROTINFO_LEN];
} LSAP_PRECISION;


							//////////////////////////////////////////////////////////////
							/*						LSA_P ΢������������			*/
							//////////////////////////////////////////////////////////////
class LSA_P_Protect : public BaseProtect
{
public:
	LSA_P_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	LSA_P_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	LSA_P_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//ȡ������Ϣ����
	LSA_P_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//ȡ����ֵ��Ϣ����
	LSA_P_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����

	int GetTroubleInfo(short troubletype,char *info);//ȡ������������
	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
	
	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};


#endif