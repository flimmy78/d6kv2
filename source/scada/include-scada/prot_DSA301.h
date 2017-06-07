#ifndef _PROT_DSA301_H
#define _PROT_DSA301_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
									//////////////////////////////////////////////////////
									/*					DSA_301						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			DSA_301������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} DSA_301_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DSA_301������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} DSA_301_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			DSA_301����ֵ��Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} DSA_301_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			DSA_301�������Ϣ			*/
										/////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} DSA_301_PROTECTDETECT;



							//////////////////////////////////////////////////////////////
							/*						DSA_301 ΢������������			*/
							//////////////////////////////////////////////////////////////
class DSA_301_Protect : public BaseProtect
{
public:
	DSA_301_Protect(void);

	uchar m_Buf[256];		//200707
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	DSA_301_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	DSA_301_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//ȡ������Ϣ����
	DSA_301_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//ȡ����ֵ��Ϣ����
	DSA_301_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����

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
