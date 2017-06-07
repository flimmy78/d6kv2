#ifndef _PROT_DMP_H
#define _PROT_DMP_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
	
#define PROTTYPE_DMP PROTTYPE_FORTYEIGHT
	
									//////////////////////////////////////////////////////
									/*					DMP						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			DMP_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short devicetype;						//װ�õ�ַ
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    char    entname[ENTNAME_LEN];		//������
} DMP_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DMP_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} DMP_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			DMP_����ֵ��Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} DMP_PROTECTMEASURE;

typedef struct 
{
		short   devicetype;
		short	code;
} KEY_DMP;

							//////////////////////////////////////////////////////////////
							/*						DMP ΢������������			*/
							//////////////////////////////////////////////////////////////
class DMP_Protect : public BaseProtect
{
	static Rdb_Tab rdbtable;
	static int opentabf;
public:
	DMP_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	DMP_PROTECTACTION	*GetActionPara(short type1,short actiontype);		//ȡ������������
	DMP_PROTECTTROUBLE	*GetTroublePara(short type1,short troubletype);	//ȡ������Ϣ����
	DMP_PROTECTMEASURE  *GetMeasurePara(short type1,short measuretype);	//ȡ����ֵ��Ϣ����
	
	int GetActionInfo(short type1,short actiontype,char *name,char *info);	//ȡ������������
	int GetTroubleInfo(short type1,short troubletype,char *info);//ȡ������������
	
	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag);
};

#endif
