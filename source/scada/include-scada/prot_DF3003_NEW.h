#ifndef _PROT_DF3003_NEW_H
#define _PROT_DF3003_NEW_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "protect.h"
#include "prot_DF3003.h"






							//////////////////////////////////////////////////////////////
							/*						DF3003 ΢������������					*/
							//////////////////////////////////////////////////////////////
class DF3003_NEW_Protect : public BaseProtect
{
public:
	DF3003_NEW_Protect(void);
	float Dword2Float(long TempDword);
	long Dword2Val_Sun(long *TempDword);//Sun�����µĸ����㷨

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort type2,ushort address1);
	DF3003_PROTECTACTION	*GetActionPara(ushort funaction,ushort actcode);
	DF3003_PROTECTMEASURE  *GetMeasurePara(ushort measurecode);		//ȡ����ֵ��Ϣ����
//	DF3003_PROTECTTROUBLE	*GetTroublePara(ushort troublecode);	//ȡ������Ϣ����

	int GetActionInfo(ushort actiontype,char *name,char *info);
	int GetMeasureInfo(ushort measuretype,char *info,char *unit);	//ȡ��������ֵ����
//	int GetTroubleInfo(ushort troublecode,char *name,char *info);//ȡ������������

	int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag);
	int  protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *actevent);	
	int save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
};
 

#endif
