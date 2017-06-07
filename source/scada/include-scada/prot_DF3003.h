#ifndef _PROT_DF3003_H
#define _PROT_DF3003_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "protect.h"

									//////////////////////////////////////////////////
									/*					DF3003						*/
									//////////////////////////////////////////////////
										//////////////////////////////////////////
										/*	DF3003_�����Լ�(df3003_action)		*/
										//////////////////////////////////////////


typedef struct {
		ushort funaction;						//��������
		ushort  actcode;						//���
	} DF3003_ACTION_KEY;

typedef	struct {
		ushort	funaction;						//��������
		ushort	actcode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//������־
		uchar	sgflag;							//�¹ʱ�־
    	char    entname[ENTNAME_LEN];		//������
} DF3003_PROTECTACTION;


										//////////////////////////////////////////////
										/*			DF3003_������Ϣ	(df3003_measure)*/
										//////////////////////////////////////////////
typedef	struct {
		ushort	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} DF3003_PROTECTMEASURE;


										//////////////////////////////////////////////
										/*			DF3003_������Ϣ(df3003_trouble)	*/
										//////////////////////////////////////////////
typedef	struct {
		ushort	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} DF3003_PROTECTTROUBLE;


							//////////////////////////////////////////////////////////////
							/*						DF3003 ΢������������					*/
							//////////////////////////////////////////////////////////////
class DF3003_Protect : public BaseProtect
{
public:
	DF3003_Protect(void);
	float Dword2Float(long TempDword);
	long Dword2Val_Sun(long *TempDword);//Sun�����µĸ����㷨

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort type2,ushort address1);
	DF3003_PROTECTACTION	*GetActionPara(ushort funaction,ushort actcode);
	DF3003_PROTECTMEASURE  *GetMeasurePara(ushort measurecode);		//ȡ����ֵ��Ϣ����
	DF3003_PROTECTTROUBLE	*GetTroublePara(ushort troublecode);	//ȡ������Ϣ����

	int GetActionInfo(ushort actiontype,char *name,char *info);
	int GetMeasureInfo(ushort measuretype,char *info,char *unit);	//ȡ��������ֵ����
	int GetTroubleInfo(ushort troublecode,char *name,char *info);//ȡ������������

	int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag);
	int  protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *actevent);	
	int save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
};
 

#endif
