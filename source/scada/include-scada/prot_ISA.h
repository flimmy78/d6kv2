#ifndef _PROT_ISA_H
#define _PROT_ISA_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					ISA						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			ISA_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//���
		char	actioninfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�	
		uchar	sgflag;							//�Ƿ����¹�
    	char    entname[ENTNAME_LEN];		//������
} ISA_PROTECTACTION;

										//////////////////////////////////////////////
										/*			ISA_������Ϣ				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//���
		char	troubleinfo[PROTINFO_LEN];		//����
} ISA_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			ISA_����ֵ��Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//���
		char	measureinfo[PROTINFO_LEN];		//����
		char	measureunit[PROT_UNIT_LEN];		//��λ
} ISA_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			ISA_�������Ϣ			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//���
		char	detectinfo[PROTINFO_LEN];		//����
		uchar   alarmf;							//�Ƿ񱨾�
   		char    entname[ENTNAME_LEN];		//���������
} ISA_PROTECTDETECT;


										//////////////////////////////////////////////////
										/*	ISA΢������С�����ӵ�ѡ��װ�ò���  	*/
										//////////////////////////////////////////////////
#define ISA_SMALLCUR_LINETYPE_MX 0				//ISAС�����ӵ�ĸ������
#define ISA_SMALLCUR_LINETYPE_XL 1				//ISAС�����ӵ���·����

typedef struct {
		char name[OBJECTNAME_LEN];				//ISAС�����ӵ���·����
		char describe[PROTINFO_LEN];			//ISAС�����ӵ���·����
		short   terminalno;						//�ն����
		ushort  lineno;							//��·��(һ��Ϊ0-10)
		ushort  linetype;						//��·����(ĸ��/��·)
		uchar alarmf;
		char	entname[ENTNAME_LEN];		//���������
}ISA_SMALLCUR;


							//////////////////////////////////////////////////////////////
							/*						ISA ΢������������			*/
							//////////////////////////////////////////////////////////////
class ISA_Protect : public BaseProtect
{
	static Rdb_Tab rdbtable;
	static int opentabf;
public:
	ISA_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	ISA_PROTECTACTION	*GetActionPara(short actiontype);		//ȡ������������
	ISA_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//ȡ������Ϣ����
	ISA_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//ȡ����ֵ��Ϣ����
	ISA_PROTECTDETECT	*GetDetectPara(short detecttype);		//ȡ�Լ���Ϣ����
	ISA_SMALLCUR        *GetSmallCurPara(short terminalno,short lineno,short linetype);	//ȡС�����ӵ�װ�ò���

	int GetActionInfo(short actiontype,char *name,char *info);	//ȡ������������
	int GetDetectInfo(short detecttype,char *name,char *info);	//ȡ�����Լ���Ϣ����

	//����FEP����������������
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	int GetTroubleInfo(short troubletype,char *info);//ȡ������������

	//����Ӧ���¼�
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	void make_smallcurrent_apevent(SYS_CLOCK *sysclock,ISA_SMALLCUR *smallcurp);

	//���ɴ����¼�
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	void save_smallcurrent_event(SYS_CLOCK *sysclock,ISA_SMALLCUR *smallcurp);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
