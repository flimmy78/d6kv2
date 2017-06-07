#ifndef _PROT_CSC2000_H
#define _PROT_CSC2000_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					CSC2000						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			CSC2000_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} CSC2000_PROTECTACTION;

										//////////////////////////////////////////////
										/*			CSC2000_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} CSC2000_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			CSC2000_����ֵ��Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} CSC2000_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			CSC2000_�������Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} CSC2000_PROTECTDETECT;



							//////////////////////////////////////////////////////////////
							/*						CSC2000 ΢������������			*/
							//////////////////////////////////////////////////////////////
class CSC2000_Protect : public BaseProtect
{
public:
	CSC2000_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort type1,ushort address1);
	CSC2000_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	CSC2000_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//ȡ������Ϣ����
	CSC2000_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//ȡ����ֵ��Ϣ����
	CSC2000_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����

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
