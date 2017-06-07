#ifndef _PROT_ISA300_H
#define _PROT_ISA300_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					ISA300						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			ISA300_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} ISA300_PROTECTACTION;

										//////////////////////////////////////////////
										/*			ISA300_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} ISA300_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			ISA300_����ֵ��Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} ISA300_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			ISA300_�������Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} ISA300_PROTECTDETECT;



							//////////////////////////////////////////////////////////////
							/*						ISA300 ΢������������			*/
							//////////////////////////////////////////////////////////////
class ISA300_Protect : public BaseProtect
{
public:
	ISA300_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort type1,ushort address1);
	ISA300_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	ISA300_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//ȡ������Ϣ����
	ISA300_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//ȡ����ֵ��Ϣ����
	ISA300_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����

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
