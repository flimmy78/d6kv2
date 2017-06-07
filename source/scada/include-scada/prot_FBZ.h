#ifndef _PROT_FBZ_H
#define _PROT_FBZ_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					FBZ						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			FBZ_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} FBZ_PROTECTACTION;

										//////////////////////////////////////////////
										/*			FBZ_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} FBZ_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			FBZ_����ֵ��Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
		uchar	dotnums;		//С��λ
} FBZ_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			FBZ_�������Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} FBZ_PROTECTDETECT;



							//////////////////////////////////////////////////////////////
							/*						FBZ ΢������������			*/
							//////////////////////////////////////////////////////////////
class FBZ_Protect : public BaseProtect
{
public:
	FBZ_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort type1,ushort address1);
	FBZ_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	FBZ_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//ȡ������Ϣ����
	FBZ_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//ȡ����ֵ��Ϣ����
	FBZ_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����

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
	int save_trouble_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event,int measurenums);
};

#endif
