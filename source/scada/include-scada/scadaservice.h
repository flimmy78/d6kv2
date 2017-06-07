#ifndef _SCADA_SERVICE_H
#define _SCADA_SERVICE_H

#include "scd/cmddef.h"
#include "scadatime.h"

//////////////////////////////////////////////////////////
/*														*/
/*			΢������Ӧ���¼��ṹ����(ͨ��)				*/
/*														*/
//////////////////////////////////////////////////////////
typedef struct
{
	SYS_CLOCK   clock;							//΢�������¼�����ʱ��
	char    name[OBJECTNAME_LEN];				//����
    char    substation[OBJECTNAME_LEN];			//վ����ϵ����
   	short   protocoltype;						//������Լ����
	short	eventtype;							//�����¼�����--�����¼������Լ���Ϣ,�򴿹��ϱ���
	short	type_value;							//������������ֵ�����Լ�����ֵ
	uchar	sgflag;								//���������Ƿ����¹�
	char	enttypedescribe[OBJDESCRIBE_LEN];	//�¼���������
	int		event_int_info[8];					//΢���������͵�����������Ϣ
	float	event_float_info[8];				//΢���������͵�������������Ϣ
	char	event_char_info[32];				//΢���������͵������ַ�����Ϣ
}PROTECT_MSG;


///////////////////////////////////////////////////////////////////////////////
/////////////////////// �������йصı� ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


// ������ṹ
typedef	struct {
		char  entidxname[ENTNAME_LEN];		// �������
		char  entexpression[ENTDESC_LEN];	// ������ʽ
		char  sndexpression[ENTDESC_LEN];	// �������ʽ
} ENTEXPRESSION;

/*
// ����ṹ
typedef	struct {
		SYS_EVENT	sysevent;
		char		valuedes[OBJDESCRIBE_LEN];
		char		groupdes[OBJDESCRIBE_LEN];
		char		memberdes0[OBJDESCRIBE_LEN];
		char		memberdes1[OBJDESCRIBE_LEN];
		char		memberdes2[OBJDESCRIBE_LEN];
		char		memberdes3[OBJDESCRIBE_LEN];
		char		memberdes4[OBJDESCRIBE_LEN];
		char		memberdes5[OBJDESCRIBE_LEN];
		char		otherdes0[OBJDESCRIBE_LEN];
		char		otherdes1[OBJDESCRIBE_LEN];
		char		otherdes2[OBJDESCRIBE_LEN];
		char		otherdes3[OBJDESCRIBE_LEN];
		char		otherdes4[OBJDESCRIBE_LEN];
		char		otherdes5[OBJDESCRIBE_LEN];
		char		longdes[120];
} SCDEVENT;
*/

//����ϵͳ����
#define SCDEVENT SYS_EVENT_EX

//�������ͱ�ṹ
typedef struct {
    short eventsortid; //�����
    short eventtypeid; //���ͺ�
    char  eventname[OBJECTNAME_LEN]; //������
    char  describecode[OBJECTNAME_LEN]; //�������
} EVENTTYPE;

//����״̬������ṹ
typedef struct {
//	short eventclassid;
    short eventtypeid;
    short statevalue;
    char  statedescribe[OBJDESCRIBE_LEN];
} EVENTSTATEDESC;

/*
//����ģʽ��
typedef struct {
    short  mode;
    char   describe[OBJDESCRIBE_LEN];
	uchar  notmakef;
	uchar  security;
	uchar  savediskf;
	uchar  printf;
	uchar  logf;
	uchar  alarmf;
	uchar  popgraf;
	char   entname[ENTNAME_LEN];	//  �����
	char   almgraname[GRAP_LEN];	//  ��ͼ��
} EVENTMODE;
*/

/******************************************************************/
/*********************  �����ʽ������  ***********************/
/******************************************************************/
typedef struct {
	    char    name[OBJECTNAME_LEN];//����
        char    describe[OBJDESCRIBE_LEN];//����
		uchar	jgprintf;//�����ӡ
		uchar	jglogf;//������ʾ
		uchar	jgalarmf;//��������
		uchar	jgalarmmodef;//�������췽ʽ
		uchar	jgpopgraf;//������ͼ
		uchar	jgsavef;//�������
		uchar	jgconfirmf;//����ȷ�Ϸ�ʽ

		uchar	jbprintf;//������ӡ
		uchar	jblogf;//������ʾ
		uchar	jbalarmf;//��������
		uchar	jbalarmmodef;//�������췽ʽ
		uchar	jbpopgraf;//������ͼ
		uchar	jbsavef;//��������
		uchar	jbconfirmf;//����ȷ�Ϸ�ʽ
		char    almtelno[OBJECTNAME_LEN];//�����绰
		uchar	eventlevel;
}  EVENTDEALMODE_STRU;


//event control struct
typedef	struct  {
		int  staval;						// ״ֵ̬
		char describe[OBJDESCRIBE_LEN];		// ����
} SCDENTDEFSTR;

 
// ������

class 
#ifdef RDB_PLATFORM_WIN32
 __declspec(dllexport)
#endif
ScdComnEvent: public NetDbg{
	protected:
		static int			opentabf;
		static	Rdb_Tab		eventquetab;		// ������б�
		static	Rdb_Tab		entexpressiontab;	// ������
		static	Rdb_Tab		enttypetab;			// �������ͱ�
		static	Rdb_Tab		entstatusdestab;	// ����״̬������ṹ	
		static	Rdb_Tab		eventdealmodetab;	// �����ʽ��ṹ
		char	*curexpression;					// ��ǰ���ʽָ��
	public:
		ScdComnEvent(void);
		~ScdComnEvent(void);
		// ȡ�÷��ѵ��ִ�
		int		get_splitdefstr(char *srcstr,char* keystr,SCDENTDEFSTR *defstr);
		// ȡ���������ֱ��ʽ
		char	*get_entexpression(char  *entidxname);
		// ȡ�������������ʽ
		char	*get_sndexpression(char  *entidxname);
		// ����������������
		int		yield_entdescribe(char* entname,SCDEVENT  *eventp,char firstflag=0);
		// ����������������
		int		yield_snddescribe(char* entname,SCDEVENT  *eventp);
		// �����������͡�����״ֵ̬ȡ������״̬����
		int		GetStavalueDes(short eventtype, short stavalue, char *description);
		// ������������ȡ���������
		int		Get_EventExpressName(short eventtype, char *eventname);
		// ����������еĹؼ���ȡ�������ļ���
		int		GetToneFileName(SYS_EVENT *eventp, char *keystr, char *filename);
// ��������ģʽ
//		EVENTMODE *GetEventMode(short mode);
		EVENTDEALMODE_STRU *GetEventDealMode(char* name);
		//
		int		save_event(SCDEVENT *eventp,int nums);
		int		add_userdef_event(short groupno,short entsort,short enttype,short stval,short stval1,float floatval,char *group_des,char * value_des,char *member_name0,char *member_des0,char *member_des1,char *member_des2,char *otherdes0,char *otherdes1,char *entname);
};

///////////////////////////////////////////////////////////////////////
#endif
	
