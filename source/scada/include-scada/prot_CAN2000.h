#ifndef _PROT_CAN2000_H
#define _PROT_CAN2000_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
									//////////////////////////////////////////////////////
									/*					C2000						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			C2000_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		    //������
} CAN2000_PROTECTACTION;

										//////////////////////////////////////////////
										/*			C2000_��������			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		float   measurepoint;					//ϵ��
		char	measureunit[PROT_UNIT_LEN];		//��λ
} CAN2000_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			C2000_�Լ���Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//�Լ������Ϣ����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} CAN2000_PROTECTDETECT;


							//////////////////////////////////////////////////////////////
							/*						C2000 ΢������������			*/
							//////////////////////////////////////////////////////////////
class CAN2000_Protect : public BaseProtect
{
public:
	CAN2000_Protect(void);
	
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort type1,ushort address1);
	CAN2000_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	CAN2000_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//ȡ����ֵ��Ϣ����
	CAN2000_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����

	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
	
	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event,int measurenum);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
