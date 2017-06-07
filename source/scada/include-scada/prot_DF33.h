#ifndef _PROT_DF33_H
#define _PROT_DF33_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
	
#define PROTTYPE_DF33 PROTTYPE_FORTYTWO

	
									//////////////////////////////////////////////////////
									/*					DF33						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			DF33_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		uchar devicetype;						//װ������
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    char    entname[ENTNAME_LEN];		//������
} DF33_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DF33_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} DF33_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			DF33_����ֵ��Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} DF33_PROTECTMEASURE;


typedef struct 
{
		short   devicetype;
		short	code;
} KEY_DF33;

							//////////////////////////////////////////////////////////////
							/*						DF33 ΢������������			*/
							//////////////////////////////////////////////////////////////
class DF33_Protect : public BaseProtect
{
	static Rdb_Tab rdbtable;
	static int opentabf;
public:
	DF33_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2,ushort,ushort);
	DF33_PROTECTACTION	*GetActionPara(short type1,short actiontype);		//ȡ������������
	DF33_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//ȡ������Ϣ����
	DF33_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//ȡ����ֵ��Ϣ����

	int GetActionInfo(short type1,int actiontype,char *name,char *info);	//ȡ������������
	int GetTroubleInfo(short type1,short troubletype,char *info);//ȡ������������
	
	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *actevent);	
	void SunFloat(uchar *source, uchar *des);
	int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag);
};

#endif
