

#ifndef _GLOBLALDEF_H_XWQ20030519_
#define _GLOBLALDEF_H_XWQ20030519_
#include "sysstr.h"
#include "netapi.h"
#include "dadef.h"
#define __PROGRAM_DEBUG_  //�����д�׶ε�����Ϣ�������
//#define __PROGRAM_DEBUG_XXX   ///����ײ㲻֪ͨ
#define NODE_MAX_SHAREBRKNUM 11
//#define DA_YK_USERNAME SYS_PWD
//#define DA_YK_PASSWORD SYS_PWD
#define DA_YK_USERNAME "sa"
#define DA_YK_PASSWORD "supernb"
#define DASERVER_NAME "DA"

#define EVENT_VALID_GAP			15 ////�յ������ʱ��������ķ���ʱ�����Ч���
#define DINAMIC_BUF_BLOCK_SIZE	100 ///��100��Ϊһ���ڴ浥λ���ж�̬�����ڴ�
#define REALDATA_BUF_SIZE		60 //��һ���ˢ�����ڼ��㣬��������
#define BACK_REALDATA_TIME		2000 //���¹���բǰ����ʵʱ���ݵ�ʱ�䣨ms��
#define MAX_ONEBRK_EVTNUM		8 //��һ���¹��У�һ�����ص������������
#define MAX_SUB_REPORT_SIZE		8 //��վ�����У���λ���Ϻͻָ�����󿪹�����
#define WAIT_SUBREPORT_TIME		3 //�ȴ���վ���ĳ�ʱʱ����ֵ

#define MAX_WAIT_SECONDS		30 //�·�ң������󣬵ȴ�ʱ�����ֵ��s��
///////�ڵ����Ͷ���
#define NODETYPE_POWER			0 //��Դ��
#define NODETYPE_LINE			1	//�ܿ���
#define NODETYPE_CABLE			2 //����
#define NODETYPE_LINK			3	//������
#define NODETYPE_TRANSFORMER	4 //��ѹ��

///////�ڵ���ϴ���ģʽ
#define CENTERISOLATE_CENTERRESTORE 0
#define SUBISOLATE_CENTERRESTORE	1
#define SUBISOLATE_SUBRESTORE		2
////////
///////////��վDAģʽ
#define DONT_BACKUP		0
#define ISOLATE_BACKUP	1
#define RESTORE_BACKUP	2
#define ALL_BACKUP		3
//////////////////////////////////////////
typedef struct
{
	SYS_TIME recvTime;//DA�����յ������ʱ�䣡
	SYS_EVENT evt;
} EX_SYS_EVENT;

//��վ����
typedef struct
{
	unsigned int Minitues;
	unsigned short Seconds;
} TIME_STRU;
typedef struct
{
	unsigned char DAType;//1 ��ʾ����ʽ���ƣ�2 ��ʾ�ֲ�ʽ���ƣ�
	unsigned char ReportType;
	//�������ͣ�0x01��ʾ���߹��ϴ����ģ�0x02��ʾ����������Ͷ���� ��0x04 ��ʾ���������ϴ����� ��
	//0x08 ��ʾ����Ͷ�ָ����ģ�0x10 DA��ʽת������
	unsigned short ReportFun;
	//����������Ч��־��D0��ʹ��0x01 , ����ʶ��0x02��D1�����ϸ���0x04  D2���ָ�0x08 ��
	//1 ��ʾ��Ч��0��ʾ��Ч��
	unsigned short FailProc;   //0���޹���,0x01����λ�׶�,0x02������׶�,0x04���ָ��׶�
} NORMAL_STRU;
typedef struct
{
	unsigned short StartSW ;
	//����������ʼ���ر��
	unsigned short EndSW ;
	//��������ĩ�˿��ر��
} FAULT_POS;
typedef struct
{
	unsigned short SWNo;	/*���غ�*/
	unsigned short GZCSwWNo; /*���ϲ࿪�ر��*/
	unsigned char Phases;	/*��������0x01��ң��Ԥ�ã�0x02��ң��ִ��,0x04��ͨѶ��飬0x08��ң��ȷ��*/
	unsigned char Status;	/*ʧ��ԭ��,0���޹���,0x01����ŷǷ�,0x02���õ����ڱ�����,
	0x03������Ӳ�����⣬0x05��ͨѶ����,0x06�����ع���*/
} ACT_BRKINFO;
typedef struct
{
	unsigned short swnum;
	//DA��ʽ�����仯�ĳ��ڶ�·�����غ�
	unsigned short datype;
	//1 �����ڷֲ�ʽ���Ʒ�ʽ�£�3�����ڼ��п��Ʒ�ʽ��
} DAMODE_CONVERT;
typedef struct
{
	TIME_STRU time;
	NORMAL_STRU normal_info;
	FAULT_POS fault_pos;
//	unsigned char isolate_num;
	ACT_BRKINFO isolate_info[MAX_SUB_REPORT_SIZE];
//	unsigned char restore_num;
	ACT_BRKINFO restore_info[MAX_SUB_REPORT_SIZE];
	DAMODE_CONVERT mode_convert[MAX_SUB_REPORT_SIZE];
} REPORT_STRU;//���Ľṹ

typedef struct _ex_report_stru_
{
	SYS_TIME recvTime;//����ʱ��
	REPORT_STRU Report;
	_ex_report_stru_* pNext;
} EX_REPORT_STRU;


typedef struct
{
	int FaultPos;
	unsigned char FaultType;//��������
} FAULT;
typedef struct _fault_info
{
	FAULT fault;
	int bIsolate;//�������������־,��θ���ʱ��
	_fault_info* pNext;
} FAULTINFO;


typedef struct _isolate_brk_
{
	int brkaddr;
	int faultNodeAddr;
	BOOL allfault;//���඼�й��ϱ�־
	BOOL isosuccess;//����ɹ���־
	_isolate_brk_* pNext;
} ISOLATE_BRK;

#ifndef _WINDOWS
void ReadStreamData(int);
void DaQuit(int);
void* 
#else
unsigned int 
#endif
#if defined(_WINDOWS)
DaProc(LPVOID pParam);///DA�̺߳���
#else
DaProc(void* pVoid);
#endif
#ifndef _WINDOWS
void* 
#else
unsigned int 
#endif
#if defined(_WINDOWS)
EvtProc(LPVOID pParam);///�����̺߳���
#else
EvtProc(void* pVoid);
#endif
#endif
