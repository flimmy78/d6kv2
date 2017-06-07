#ifndef _PROT_SEL_H
#define _PROT_SEL_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "protect.h"


									//////////////////////////////////////////////////
									/*				S E Lģ�鶨��					*/
									//////////////////////////////////////////////////


#define PDEVTYPE_SEL_251				0
#define PDEVTYPE_SEL_501				1
#define PDEVTYPE_SEL_587				2
#define PDEVTYPE_SEL_551				3
#define PDEVTYPE_SEL_287				4
#define PDEVTYPE_SEL_387				5
#define PDEVTYPE_SEL_351				6


// ģ�����ͣ��������ͣ�����

#define PTYPE_MTMAIN					1	//����������
#define PTYPE_MTBACK_110KV				2	//����󱸱���(110KV��)
#define PTYPE_MTBACK_35KV				3	//����󱸱���(35KV��)
#define PTYPE_MTBACK_10KV				4	//����󱸱���(10KV��)
#define PTYPE_MTMIDZERO					5	//�������͵����򱣻�
#define PTYPE_MTZTYPE					6	//����Z�ͱ䱣��
#define PTYPE_SUBSECTION				7	//�ֶα���
#define PTYPE_BUS_35KV					8	//35KVĸ�߱���
#define PTYPE_CAPACITOR_V				9	//��������ѹ����
#define PTYPE_CAPACITOR_I				10	//��������������
#define PTYPE_LINE						11	//��·����
				

									//////////////////////////////////////////////////
									/*					S E L						*/
									//////////////////////////////////////////////////
										//////////////////////////////////////////
										/*			SEL_������Ϣ(sel_action)	*/
										//////////////////////////////////////////
typedef	struct {
		short	actioncode;		//���
		short	actiontype;		//��������
		short	circletype;		//��������
		char	actioninfo[PROTINFO_LEN];	//����
		uchar   alarmf;			//����
		uchar	sgflag;			//�¹�
    	char    entname[ENTNAME_LEN];	//������
} SEL_PROTECTACTION;


							//////////////////////////////////////////////////////////////
							/*						SEL ΢������������					*/
							//////////////////////////////////////////////////////////////
class SEL_Protect : public BaseProtect
{
	static Rdb_Tab protacttab;
	static int opentabf;
public:
	SEL_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort adddress1,ushort address2);
	int GetActionType(short devtype,short moduletype,char *action_string,char *target,ushort *actiontype,int totalnums);
	SEL_PROTECTACTION	*GetActionPara(ushort actiontype,ushort circletype);
	int GetActionInfo(ushort actiontype,ushort circletype,char *name,char *info);

	int  GetProtinfoTime(uchar *gram_part, SYS_CLOCK *clock);
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
};
 

#endif
