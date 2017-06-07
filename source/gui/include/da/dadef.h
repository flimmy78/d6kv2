
#ifndef _DA_DEF_H__
#define _DA_DEF_H__
//
//DA����������Ϣ��������
////////////////////////////////////////////////////////////////////
//#include "sysdef.h"

#ifdef TRACE
#undef TRACE
#endif
#define TRACE		init.PutDbg

////da communicate mail type 
#define DAMAILTYPE_APPLYRESEARCH	0
#define DAMAILTYPE_SELRESPRJ		1
#define DAMAILTYPE_CLEARRDB			2
#define DAMAILTYPE_APPLYDELAY		3 //Ҫ���ӳ��ȴ�����ѡ���ʱ��
#define DAMAILTYPE_SELISOLATE		4 //zl20111026, ѡ���ֶ�������
#define DAMAILTYPE_RESTORESTATUS	5 //zxm20140224, �ָ�����ǰ״̬
//-------------------------------
#define DAMAILOBJ_NEEDRESEARCH 		1
#define DAMAILOBJ_CANCELRESEARCH 	0
//--------------------------

//da working state defines
#define DAWORKING_REAL		0x00 //ʵʱ̬	
#define DAWORKING_DEBUG		0x01 //����̬

//who locate fault 
#define CENTERLOCATE		0x01	//��վ��λ
#define SUBLOCATE		0x02	//��վ��λ
#define ALLLOCATE		0x03	//����λ��
//
//kg yk result
#define RESULT_SUCCESS		0x01	//�طֻ��߿غϳɹ�
#define RESULT_FAIL		0x00	
//
//who send yk command
#define CENTER_EXE		0x00
#define SUB_EXE			0x01

//isolate times
#define CENTER_ISO		0x00 //��վ����
#define SUB_ISO			0x01 //��վ����
#define CENTER_BACKISO		0x02 //��վ�󱸸���
#define CENTER_EXISO		0x03 //��վ����Χ����
#define SUB_EXISO		0x04 //��վ����Χ����

//restore perform mode
#define SEL_EXE			0x00
#define AUTO_EXE		0x01

enum DAEventtype
{
	DACOMTYPE = 4200 ,              //DA����潻����������ʼ���ͺ�
	DACOMTYPE_DASTART,              //4201 DA������̿�ʼ
	DACOMTYPE_DALOCATEFAULT,        //4202 DA���϶�λ
	DACOMTYPE_ISOLATE,				//4203 DA��վ����������ϸ���
	DACOMTYPE_EXPLAINFAULT,			//4204 DA������վ����
	DACOMTYPE_BACKISOLATE,			//4205 DA�󱸸���
	DACOMTYPE_EXISOLATE,			//4206 DA����Χ����
	DACOMTYPE_RESPRJOCCUR,			//4207 DA�ָ���������
	DACOMTYPE_RESPRJSEL,			//4208 DA�ָ�����ѡ��
	DACOMTYPE_RESTORE,				//4209 DA�ǹ�������ָ�
	DACOMTYPE_WORKSTATE,			//4210 DA������ʽת��
	DACOMTYPE_ANALYSE,				//4211 DA���ܷ���
	DACOMTYPE_DALOCATESUCCESS		//4212 DA���϶�λ�ɹ�//zl20111026
};

typedef struct
{
	unsigned char type;             //�������о��ʼ�\����ѡ���ʼ�\��������ʵʱ��
	char	machine[40];		//���������
/*	char	machine_desc[DESC_LEN];	//��������
	char	oper[CODE_LEN];			//����Ա����
	char	oper_desc[DESC_LEN];	//����Ա����
	char	pwd[CODE_LEN];			//����
*/	char	objstate;				//Ŀ������
	char	extend[24];					
} DAMAIL;

#endif