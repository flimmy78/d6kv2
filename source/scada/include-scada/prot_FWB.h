#ifndef _PROT_FWB_H
#define _PROT_FWB_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
/*
	FWB���������Ϣ�ϼ򵥣�������ڳ�����Ӳ������ͣ�
	FWB����������Ϣ��ģ���йأ�����ڶ�����Ϣ���к���ģ������

#define PDEVTYPE_FWB_START				PDEVTYPE_FWB_1200
#define PDEVTYPE_FWB_1200				0		//3130
#define PDEVTYPE_FWB_1201				1
#define PDEVTYPE_FWB_1210				2		//3131
#define PDEVTYPE_FWB_1220				3
#define PDEVTYPE_FWB_1221				4		//3121
#define PDEVTYPE_FWB_1222				5		//3122
#define PDEVTYPE_FWB_1230				6		//3160
#define PDEVTYPE_FWB_1240				7
#define PDEVTYPE_FWB_END				PDEVTYPE_FWB_1240
*/
									//////////////////////////////////////////////////////
									/*					FWB								*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			FWB_������Ϣ(fwb_action)		*/
										//////////////////////////////////////////////
typedef struct 
{
		ushort   moduletype;
		ushort	actioncode;
} ACTION_KEY1;


typedef	struct {
		ushort	moduletype;						//ģ������
		ushort	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} FWB_PROTECTACTION;

										//////////////////////////////////////////////
										/*			FWB_�����Ϣ(fwb_detect)		*/
										//////////////////////////////////////////////
typedef	struct {
		uchar	chkbit;							//��λ
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//������
} FWB_PROTECTDETECT;

							//////////////////////////////////////////////////////////////
							/*						FWB ΢������������					*/
							//////////////////////////////////////////////////////////////
class FWB_Protect : public BaseProtect
{
public:
	FWB_Protect(void){};

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	FWB_PROTECTACTION	*GetActionPara(unsigned short moduletype,short actioncode);		//ȡ������������
	FWB_PROTECTDETECT	*GetDetectPara(unsigned char chkbit);		//ȡ�Լ���Ϣ����

	int GetActionInfo(unsigned short moduletype, short actioncode,char *name,char *info);	//ȡ������������
	int GetDetectInfo(unsigned char chkbit,char *name,char *info);	//ȡ�����Լ���Ϣ����

    int GetFWBProtinfoTime(uchar *gram_part, SYS_CLOCK *clock);
    int GetTime(SYS_CLOCK *clock); 

	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
	
	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
