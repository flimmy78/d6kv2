#ifndef _PROT_LFP_H
#define _PROT_LFP_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "protect.h"
#define LFP_EVTTYPE0					0
#define LFP_EVTTYPE1					1
#define LFP_EVTTYPE2					2
#define LFP_EVTTYPE3					3
#define LFP_EVTTYPE4					4
#define LFP_EVTTYPE5					5

#define LFP_ACTTYPE0					0
#define LFP_ACTTYPE1					1
#define LFP_ACTTYPE2					2
#define LFP_ACTTYPE3					3
#define LFP_ACTTYPE4					4

									//////////////////////////////////////////////////////
									/*						LFP						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*				LFP_ģ����Ϣ(lfp_module)	*/
										//////////////////////////////////////////////
typedef struct
{
		ushort	moduletype;						//ģ������
		char	describe[OBJDESCRIBE_LEN];		//����
		char	prtboard0[OBJDESCRIBE_LEN];		//������0��
		char	prtboard1[OBJDESCRIBE_LEN];		//������1��
		char	prtboard2[OBJDESCRIBE_LEN];		//������2��
		uchar   eventtype;						//�¼���������
		uchar	flagtype;						//���ޱ�־�ֽ�
		uchar   totrouble;						//ͬ�����¼�
		uchar	prtbd0tz;						//������0��բ
		uchar	prtbd1tz;						//������1��բ
		uchar   circuitval;						//�غ�բ������
} LFP_PROTECTMODULE;	
										//////////////////////////////////////////////
										/*				LFP_�¼���Ϣ(lfp_event)		*/
										//////////////////////////////////////////////
typedef struct
{
		ushort	moduletype;						//ģ������
		uchar	actbit;							//��λ
		char	describe[OBJDESCRIBE_LEN];		//����
		uchar   alarmf;							//����
		uchar	sgflag;							//�¹�
   		char    entname[ENTNAME_LEN];		//������
} LFP_PROTECTEVENT;
										//////////////////////////////////////////////
										/*				LFP_������Ϣ(lfp_action)	*/
										//////////////////////////////////////////////
typedef struct
{
		ushort	moduletype;						//ģ������
		uchar	boardnum;						//�������
		uchar	actbit;							//��λ
		char	describe[OBJDESCRIBE_LEN];		//����
		uchar   alarmf;							//����
		uchar	sgflag;							//�¹�
   		char    entname[ENTNAME_LEN];		//������
} LFP_PROTECTACTION;

										//////////////////////////////////////////////
										/*				LFP_�Լ���Ϣ(lfp_detect)	*/
										//////////////////////////////////////////////
typedef	struct {
		ushort	moduletype;						//ģ������
		uchar   flagtype;						//��־����
		uchar	chkbit;							//��λ
		char	describe[OBJDESCRIBE_LEN];		//����
		uchar   alarmf;							//����
   		char    entname[ENTNAME_LEN];		//�����
} LFP_PROTECTDETECT;


typedef struct {
		ushort moduletype;						//ģ������
		uchar  flag1;							//��־1
		uchar  flag2;							//��־2
	} LFP_ACTION_KEY;


							//////////////////////////////////////////////////////////////
							/*						LFP ΢������������					*/
							//////////////////////////////////////////////////////////////
class LFP_Protect : public BaseProtect
{
public:
	int actiontype;
	LFP_Protect(void){};

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	LFP_PROTECTMODULE  *GetLfpMdlPara(unsigned short moduletype);
	LFP_PROTECTEVENT  *GetEventPara(unsigned short moduletype,unsigned char actbit);
	LFP_PROTECTACTION *GetActionPara(unsigned short moduletype,unsigned char boardnum,unsigned char actbit);
	LFP_PROTECTDETECT *GetDetectPara(unsigned short moduletype,unsigned char flagtype,unsigned char chkbit);

	void make_event_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event);	//�����¼���Ϣ--�򵥹��ϱ���
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event);	//���ɹ�����Ϣ--���ӹ��ϱ���
	int save_event_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event);		//�����¼���Ϣ--�򵥹��ϱ���
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event);		//���̹�����Ϣ--���ӹ��ϱ���
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event, int m_Flag);		//�����Լ��¼�
};

#endif
