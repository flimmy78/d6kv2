#ifndef _PROT_ROAD_H
#define _PROT_ROAD_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
	
#define PROTTYPE_ROAD PROTTYPE_FORTYONE
#define TYPE_140	  140
#define TYPE_141	  141
	
									//////////////////////////////////////////////////////
									/*					ROAD						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			ROAD_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short devicetype;						//װ�õ�ַ
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    char    entname[ENTNAME_LEN];		//������
} ROAD_PROTECTACTION;

										//////////////////////////////////////////////
										/*			ROAD_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short devicetype;						//װ�õ�ַ
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} ROAD_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			ROAD_����ֵ��Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short devicetype;						//װ�õ�ַ
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} ROAD_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			ROAD_�������Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	devicetype;						//װ�õ�ַ
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   	char    entname[ENTNAME_LEN];		//���������
} ROAD_PROTECTDETECT;

typedef struct 
{
		short   devicetype;
		short	code;
} KEY_ROAD;

							//////////////////////////////////////////////////////////////
							/*						ROAD ΢������������			*/
							//////////////////////////////////////////////////////////////
class ROAD_Protect : public BaseProtect
{
	static Rdb_Tab rdbtable;
	static int opentabf;
public:
	ROAD_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	ROAD_PROTECTACTION	*GetActionPara(short type1,short actiontype);		//ȡ������������
	ROAD_PROTECTTROUBLE	*GetTroublePara(short type1,short troubletype);	//ȡ������Ϣ����
	ROAD_PROTECTMEASURE  *GetMeasurePara(short type1,short measuretype);	//ȡ����ֵ��Ϣ����
	ROAD_PROTECTDETECT	*GetDetectPara(short type1,short detecttype);		//ȡ�Լ���Ϣ����

	int GetActionInfo(short type1,short actiontype,char *name,char *info);	//ȡ������������
	int GetDetectInfo(short type1,short detecttype,char *name,char *info);	//ȡ�����Լ���Ϣ����
	int GetTroubleInfo(short type1,short troubletype,char *info);//ȡ������������
	
	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag);
};

#endif
