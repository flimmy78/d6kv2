#ifndef   _PROTECT_H
#define   _PROTECT_H
#include  <string.h>

#include "scadatime.h"
#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "net/netdef.h"
#include "scd/scddef.h"

#include    "scadaproc.h"
#include    "scadainterface.h"

#define TIMEBYBIN 0
#define TIMEBYBCD 1
#define	BCD1BIN1(a)		(((a)/16)*10 + (a)%16)



//΢���������ش�����
#define  PROT_ERR_MAILLEN		   2		//�����ʼ����ȴ���
#define  PROT_ERR_MODULE		   3		//����ģ�����
#define  PROT_ERR_MESSAGE		   4		//�޸�΢��������Ϣ
#define  PROT_ERR_ACTION		   5		//����������Ϣ����
#define  PROT_ERR_DETECT		   6		//�����Լ���Ϣ����
#define  PROT_ERR_TROUBLE		   7		//����������Ϣ����
#define  PROT_ERR_NOTSUPPORT       10		//��֧�ֵı�������

////////////////////////////////////////////////////////////////////////////////
/////////////////  ΢�����������ʵʱ����� //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define  PROTMDL_TABNAME            "protmodule"	//΢������������

//LFP
#define  LFP_PROTMODULE_TABNAME     "lfp_module"	//LFP_ģ����Ϣ
#define  LFP_PROTACTION_TABNAME     "lfp_action"	//LFP_������Ϣ
#define  LFP_PROTDETECT_TABNAME     "lfp_detect"	//LFP_�Լ���Ϣ
#define  LFP_PROTEVENT_TABNAME      "lfp_event" 	//LFP_�¼���Ϣ

//ISA
#define  ISA_PROTACTION_TABNAME     "isa_action"	//ISA_������Ϣ
#define  ISA_PROTDETECT_TABNAME     "isa_detect"	//ISA_�������Ϣ
#define  ISA_PROTTRBL_TABNAME       "isa_trouble"	//ISA_������Ϣ
#define  ISA_PROTMSR_TABNAME        "isa_measure"	//ISA_����ֵ��Ϣ
#define  ISA_SMALLCURRENT_TABNAME   "isa_smallcurrent" 


//ISA300
#define  ISA300_PROTACTION_TABNAME  "isa300_action"	//ISA300_������Ϣ
#define  ISA300_PROTDETECT_TABNAME  "isa300_detect"	//ISA300_�������Ϣ
#define  ISA300_PROTTRBL_TABNAME    "isa300_trouble"//ISA300_������Ϣ
#define  ISA300_PROTMSR_TABNAME     "isa300_measure"//ISA300_����ֵ��Ϣ


//CAN2000
#define  CAN2000_PROTACTION_TABNAME  "can_action"	//C2000_������Ϣ
#define  CAN2000_PROTDETECT_TABNAME  "can_detect"	//C2000_�Լ���Ϣ
#define  CAN2000_PROTMSR_TABNAME     "can_measure"	//C2000_��������

//LSAP
#define  LSAP_PROTACTION_TABNAME     "lsap_action"	//LSA_P������Ϣ
#define  LSAP_PROTDETECT_TABNAME     "lsap_detect"	//LSA_P�������Ϣ
#define  LSAP_PROTTRBL_TABNAME       "lsap_trouble"	//LSA_P������Ϣ
#define  LSAP_PROTMSR_TABNAME        "lsap_measure"	//LSA_P����ֵ��Ϣ
#define  PTOTPRSN_TABNAME			 "lsap_prcsn"   //LSA_P����������Ϣ��

//DSA_301
#define  DSA_301_PROTACTION_TABNAME  "dsa301_action"//DSA_301������Ϣ
#define  DSA_301_PROTDETECT_TABNAME  "dsa301_detect"//DSA_301�������Ϣ
#define  DSA_301_PROTTRBL_TABNAME    "dsa301_trouble"//DSA_301������Ϣ
#define  DSA_301_PROTMSR_TABNAME     "dsa301_measure"//DSA_301����ֵ��Ϣ

//SEL
#define  SEL_PROTACTION_TABNAME     "sel_action"	//SEL_������Ϣ
#define  SEL_PROTDETECT_TABNAME     "sel_detect"	//SEL���������Ϣ��

//DFP
#define  DFP_PROTACTION_TABNAME     "dfp_action"	//DFP_������Ϣ
#define  DFP_PROTTRBL_TABNAME       "dfp_trouble"	//DFP����������Ϣ��
#define  DFP_PROTDETECT_TABNAME     "dfp_detect"	//DFP_�Լ���Ϣ

//SBC
#define  SBC_PROTACTION_TABNAME		"sbc_action"    //SBC_������Ϣ

//FWB
#define  FWB_PROTACTION_TABNAME		"fwb_action"    //FWB_������Ϣ
#define  FWB_PROTDETECT_TABNAME		"fwb_detect"	//FWB_�����Ϣ

//DF3003
#define DF3003_PROTACTION_TABNAME	"df3003_action"	//DF3003_�����Լ���Ϣ
#define DF3003_PROTMEASURE_TABNAME	"df3003_measure"//DF3003_������Ϣ
#define DF3003_PROTTROUBLE_TABNAME	"df3003_trouble"//DF3003_������Ϣ

//WBX
#define WBX_PROTACTION_TABNAME	    "wbx_action"	//WBX_�¼���Ϣ

//DFP500
#define  DFP500_PROTACTION_TABNAME  "dfp500_action"	 //DFP500_������Ϣ
#define  DFP500_PROTDETECT_TABNAME  "dfp500_detect"	 //DFP500_�������Ϣ
#define  DFP500_PROTTRBL_TABNAME    "dfp500_trouble" //DFP500_������Ϣ
#define  DFP500_PROTMSR_TABNAME     "dfp500_measure" //DFP500_����ֵ


//CSC2000
#define  CSC2000_PROTACTION_TABNAME "csc2000_action"	//CSC2000_������Ϣ
#define  CSC2000_PROTDETECT_TABNAME "csc2000_detect"	//CSC2000_�������Ϣ
#define  CSC2000_PROTTRBL_TABNAME   "csc2000_trouble"	//CSC2000_������Ϣ
#define  CSC2000_PROTMSR_TABNAME    "csc2000_measure"	//CSC2000_����ֵ��Ϣ


//FBZ
#define  FBZ_PROTACTION_TABNAME     "fbz_action"	//FBZ_������Ϣ
#define  FBZ_PROTDETECT_TABNAME     "fbz_detect"	//FBZ_�������Ϣ
#define  FBZ_PROTTRBL_TABNAME       "fbz_trouble"	//FBZ_������Ϣ
#define  FBZ_PROTMSR_TABNAME        "fbz_measure"	//FBZ_����ֵ��Ϣ

//DWK
#define  DWK_PROTACTION_TABNAME     "dwk_action"	//DWK_������Ϣ
#define  DWK_PROTTRBL_TABNAME       "dwk_trouble"	//DWK_������Ϣ

//ZDB
#define  ZDB_PROTACTION_TABNAME     "zdb_action"	//ZDB������Ϣ
#define  ZDB_PROTDETECT_TABNAME     "zdb_detect"	//ZDB�������Ϣ
#define  ZDB_PROTMEASURE_TABNAME		"zdb_measure"	//ZDB�������

//PS6000
#define PS6000_PROTACTION_TABNAME		"ps6000_action"			//PS6000������Ϣ

//���DEP
#define  DEP_PROTACTION_TABNAME     "dep_action"	//DEP_������Ϣ
#define  DEP_PROTDETECT_TABNAME     "dep_detect"	//DEP_�������Ϣ
#define  DEP_PROTTRBL_TABNAME       "dep_trouble"	//DEP_������Ϣ
#define  DEP_PROTMSR_TABNAME        "dep_measure"	//DEP_����ֵ��Ϣ 

//SACDW
#define  SACDW_PROTACTION_TABNAME     "sacdw_action"	//SACDW������Ϣ
//SACJD
#define  SACJD_PROTACTION_TABNAME     "sacjd_action"	//SACJD������Ϣ
#define  SACJD_PROTDETECT_TABNAME     "sacjd_detect"	//SACJD�������Ϣ


//NSR	//add 2004 12
#define  NSR_PROTACTION_TABNAME     "nsr_action"	//NSR_������Ϣ
#define  NSR_PROTDETECT_TABNAME     "nsr_detect"	//NSR_�������Ϣ
#define  NSR_PROTTRBL_TABNAME       "nsr_trouble"	//NSR_������Ϣ
#define  NSR_PROTMSR_TABNAME        "nsr_measure"	//NSR_����ֵ��Ϣ

//NEWZDB
#define  NEWZDB_PROTACTION_TABNAME     "newzdb_action"	//NEWZDB������Ϣ

#define  IEC103_PROTACTION_TABNAME     "iec103_action"	//IEC103������Ϣ

//ROAD //add 2006 04 for ��·��ҵ��
#define  ROAD_PROTACTION_TABNAME     "road_action"	//ROAD_������Ϣ
#define  ROAD_PROTDETECT_TABNAME     "road_detect"	//ROAD_�������Ϣ
#define  ROAD_PROTTRBL_TABNAME       "road_trouble"	//ROAD_������Ϣ
#define  ROAD_PROTMSR_TABNAME        "road_measure"	//ROAD_����ֵ��Ϣ

//DF33 //add 2006 09 for ɽ��ƽ��
#define  DF33_PROTACTION_TABNAME     "df33_action"	//DF33_������Ϣ
#define  DF33_PROTTRBL_TABNAME       "df33_trouble"	//DF33_������Ϣ
#define  DF33_PROTMSR_TABNAME        "df33_measure"	//DF33_����ֵ��Ϣ

//NZ103 //add 2006 11 for ����103
#define  NZ103_PROTACTION_TABNAME     "nz103_action"		//����103������Ϣ
#define  NZ103_PROTDETECT_TABNAME     "nz103_detect"		//����103������Ϣ

//RCS_103 //add 2006 12 for ���� RCS 103
#define  RCS_103_PROTACTION_TABNAME     "rcs103_action"		//����103������Ϣ
#define  RCS_103_PROTDETECT_TABNAME     "rcs103_detect"		//����103������Ϣ


//DMP  //add 2007 05 for ����DMP
#define  DMP_PROTACTION_TABNAME     "dmp_action"	//DMP_������Ϣ
#define  DMP_PROTTRBL_TABNAME       "dmp_trouble"	//DMP_������Ϣ
#define  DMP_PROTMSR_TABNAME        "dmp_measure"	//DMP_����ֵ��Ϣ

//DFDZ  //add 2007 11 for ��������
#define  DFDZ_PROTACTION_TABNAME     "dfdz_action"	//DFDZ_������Ϣ
#define  DFDZ_PROTDETECT_TABNAME     "dfdz_trouble"	//DFDZ_������Ϣ
#define  DFDZ_PROTMEASURE_TABNAME    "dfdz_measure"	//DFDZ_����ֵ��Ϣ

//���ռ�Զ��˾�ۺ��Զ������� 2007 11
#define  AHJY_PROTACTION_TABNAME	"ahjy_action"	//AHJY_������Ϣ
////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
/*														*/
/*					΢������������(ͨ��)				*/
/*														*/
//////////////////////////////////////////////////////////
typedef struct {
	char    name[OBJECTNAME_LEN];		//����
    char    describe[OBJDESCRIBE_LEN];	//����
    int     securegrade;				//��ȫ��
	char	substation[OBJECTNAME_LEN];	//վ����ϵ����
    short   terminalno;					//�ն����	
    ushort  prottype;					//������Լ����
	ushort  type1;						//����1 
	ushort  type2;						//����2 
	ushort  type3;						//����3 
	ushort  type4;						//����4 
	ushort  address1;					//��ַ1 
	ushort  address2;					//��ַ2 
	ushort  address3;					//��ַ3 
	ushort  address4;					//��ַ4 
}PROTECTMDL;




//////////////////////////////////////////
/*				΢�������������		*/
//////////////////////////////////////////
class BaseProtect : public Event
{
public:
		static Rdb_QuickQuery table;
		static Rdb_Tab promdltab;
        static int opentabf;
public:
		BaseProtect(void);
		int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock,uchar chbyte=0xFF,uchar nomsflag=0);	//ʱ�䴦����
		PROTECTMDL *GetModulePara(ushort terminalno,ushort address1,ushort address2);
		PROTECTMDL	*GetModulePara(ushort terminalno,ushort prottype,ushort type1,ushort type2,ushort type3,ushort type4,ushort address1,
			ushort address2,ushort address3,ushort address4);
				//���ն˺š���Լ���͡�����1������2������3������4����ַ1����ַ2����ַ3����ַ4ȡ΢����������
					
		void make_common_saveinfo(SCDEVENT *,SYS_CLOCK *,PROTECTMDL *,int );
		void make_common_apentinfo(PROTECT_MSG *,SYS_CLOCK *,PROTECTMDL *,int ,int );
};


/********************************************************************/
/*						΢������ԭʼ���Ĵ�����						*/
/********************************************************************/
class Protect_Scdservice_serv : public Scdservice_serv {
   public:
	 Protect_Scdservice_serv(char *hm,char *um,char *pd,hlong  pid):Scdservice_serv(hm,um,pd,pid)
	 	{};
	 int  yieldprotectinfo(SCADA_PROT_STRU* protectp,int  datalen);
};

#endif
