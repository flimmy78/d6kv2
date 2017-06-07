#ifndef _PROT_DFP_H
#define _PROT_DFP_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "protect.h"
										//////////////////////////////////////////////
										/*			DFP_������Ϣ(dfp_action)		*/
										//////////////////////////////////////////////
typedef	struct {
		uchar	actbit;			//��λ
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;			//����
		uchar	sgflag;			//�¹�
    	char    entname[ENTNAME_LEN];		//������
} DFP_PROTECTACTION;
										//////////////////////////////////////////
										/*		DFP_�Լ���Ϣ(dfp_detect)		*/
										//////////////////////////////////////////
typedef	struct {
		uchar	boardtype;  //������
		uchar   chkbit;		//��λ
		char	detectinfo[PROTINFO_LEN];	//����
		uchar   alarmf;		//����
   		char    entname[ENTNAME_LEN];	//������
} DFP_PROTECTDETECT;

							//////////////////////////////////////////////////////////////
							/*						DFP ΢������������			*/
							//////////////////////////////////////////////////////////////
class DFP_Protect : public BaseProtect
{
public:
	DFP_Protect(void);
	
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	DFP_PROTECTACTION	*GetActionPara(uchar actbit);
	DFP_PROTECTDETECT	*GetDetectPara(uchar boardtype,uchar chkbit);
	
	int GetActionInfo(uchar actbit,char *name,char *info);
	int GetTroubleInfo(short troubletype,char *name,char *info);
	int GetDetectInfo(uchar boardtype,uchar chkbit,char *name,char *info);

	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);		//�γɶ����¼�;
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);			//���涯���¼�;
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);	//�����Լ��¼�;
	int save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);	//����Ӳ�����ϱ���
};

#endif
