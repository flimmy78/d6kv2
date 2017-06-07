#ifndef _PROT_DFP500_H
#define _PROT_DFP500_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
										//////////////////////////////////////////////
										/*			DFP500_������Ϣ(dfp500_action)	*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} DFP500_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DFP500_������Ϣ	(dfp500_trouble)*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} DFP500_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			DFP500_����ֵ��Ϣ(dfp500_measure)*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} DFP500_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*	DFP500_�������Ϣ(dfp500_detect)	*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} DFP500_PROTECTDETECT;


							//////////////////////////////////////////////////////////////
							/*						DFP500 ΢������������			*/
							//////////////////////////////////////////////////////////////
class DFP500_Protect : public BaseProtect
{
public:
	DFP500_Protect(void);
	
	PROTECTMDL	*GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);			
	DFP500_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	DFP500_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//ȡ������Ϣ����
	DFP500_PROTECTMEASURE   *GetMeasurePara(short measuretype);	//ȡ����ֵ��Ϣ����
	DFP500_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����

	int GetProtInfoTime(uchar *gram_part, SYS_CLOCK *clock);
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
