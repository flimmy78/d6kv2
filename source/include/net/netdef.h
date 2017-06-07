#ifndef	NET_DEF_H
#define	NET_DEF_H

#include "sysstr.h"

#define NET_DATATYPE_UCHAR          0     //�޷����ַ���
#define NET_DATATYPE_CHAR           1     //�з����ַ���
#define NET_DATATYPE_USHORT         2     //�޷��Ŷ�������
#define NET_DATATYPE_SHORT          3     //�з��Ŷ�������
#define NET_DATATYPE_UINT           4     //�޷���������
#define NET_DATATYPE_INT            5     //�з���������
#define NET_DATATYPE_ULONG          6     //�޷��ų�������
#define NET_DATATYPE_LONG           7     //�з��ų�������(8 byte long)
#define NET_DATATYPE_FLOAT          8     //�����ȸ�����
#define NET_DATATYPE_DOUBLE         9     //˫���ȸ�����
#define NET_DATATYPE_STRING         10    //�ַ�����

/*-------------------------------------------------------------------------*/

//��������������
#define	RXTXBUFSIZE	16384								//16k
#define	GRAMLEN		4096								//4k
#define DEADDATA_TXSIZE			(1024*64)	//64K jhongtu(1024*1000)				//1M

//����Э�鶨��
#define	TCPIP					0						/*TCP/IPЭ��*/

//port default defination

#define	MONPORT					5800					//��ض˿�
#define	CMDPORT					5800					//����ͨ��
#define	DATPORT					5801					//����ͨ��
//#define	FEPPORT					5802					//ǰ��ͨ��״̬
#define	DBGPORT					5803					//���Զ˿�
#define	MIRRORPORT				5804					//����˿�
#define	LINKPORT				5805					//LINKͨ��

#define TIME_MODE_DIRECT_ONLY	0x01		//��ֱ�Ӷ�ʱ
#define TIME_MODE_BUF_ONLY		0x02		//������
//#define TIME_MODE_DIRECT_BUF	0x03		//ֱ�Ӷ�ʱ������
#define TIME_MODE_STEP			0x04		//�ֲ���ʱ  for unix

/*-------------------------------------------------------------------------*/

//�������Ͷ���
#define	HOST_TYPE_BASE			0
#define	HOST_TYPE_CENTER		(HOST_TYPE_BASE+1)		/*������*/
#define	HOST_TYPE_NORMAL		(HOST_TYPE_BASE+2)		/*�ͻ�Ӧ�û�*/

//����״̬����
#define	HOST_STATUS_BASE		-1
#define	HOST_STATUS_STOP		(HOST_STATUS_BASE+1)	/*ͣ��̬*/
#define	HOST_STATUS_MAIN		(HOST_STATUS_BASE+2)	/*����̬*/
#define	HOST_STATUS_BACK		(HOST_STATUS_BASE+3)	/*����̬*/
#define	HOST_STATUS_WORK		(HOST_STATUS_BASE+4)	/*����̬*/

/*-------------------------------------------------------------------------*/

//ϵͳ����ʱ�䶨��
#define	HOSTTIME	1									/*�������״̬ʱ��*/
#define	STOPTIME	20									/*ͣ�����ʱ��*/
#define	REGTIME		3									/*������¼�ȴ�ʱ��*/
#define	ATBTIME		4									/*ƽ�����ķ���ʱ��*/
#define	CHECKTIME	30									/*ϵͳУʱʱ��*/
#define	PERIODTIME	1000000								/*ϵͳ������ʱ*/
#define	SOCKTIMEOUT	8									/*�����շ���ʱʱ��*/
#define	LINK_CONN_TIME	5
#define	LINK_TX_TIME	500
#define	LINK_RX_TIME	500

/*-------------------------------------------------------------------------*/

#if defined(WIN32)					//����WINDOWS��Ϣ
#define	MAIN_TIMER					10000
#define	HOST_STATUS_TIMER			10001
#define	GRAM_TIMER					10002
#define	MAIL_DISP_TIMER				10003
#define	SELF_INFOR_TIMER			10004

#define	DATA_MESSAGE				(WM_USER+100)			/*������׼������Ϣ*/
#define	EVENT_MESSAGE				(WM_USER+101)			/*����׼������Ϣ*/
#define	PROC_CHECK					(WM_USER+102)			/*���̼����Ϣ*/
#define	MAIL_MESSAGE				(WM_USER+103)			/*�����ʼ�ʱ�������ڷ��͵���Ϣ*/
#define	RAWD_MESSAGE				(WM_USER+104)			/*������׼������Ϣ*/
#define	DATA_CHANGED				(WM_USER+105)
#define END_PROC_SIGNAL				WM_CLOSE
#elif defined(__unix)
#define	DATA_CHANGED				SIGUSR1
#define	END_PROC_SIGNAL				SIGKILL
#endif

/*-------------------------------------------------------------------------*/
#define PORT_NUM_MAX				16
#define	GROUP_MAX					16				/*���������*/
#define	HOST_MAX					256				/*�����������������*/
#define	SERVER_NODE_MAX				16
#define	RTDB_MAX					(HOST_MAX/SERVER_NODE_MAX)

#define	MAILPROCTYPE				int32s
#define	MAIL_NUM_MAX				1024				//����������ʼ�����
#define	MAIL_LEN_MAX				8192			//����������ʼ�����
#define	MAIL_TYPE_TX				0
#define	MAIL_TYPE_RX				1
#define	WIN_BUF_MAX					128

#define MAIL_LEVEL_GRADE			10
#define MAIL_BASE_LEVEL				20

/*-------------------------------------------------------------------------*/

#define EVENT_NUM_MAX				65536
#define EVENTDESCRIBE_LEN			40

#define SND_EVENT_MAX				65536
#define GEN_EVENT_MAX				65536
#define OPT_EVENT_MAX				1024

/*-------------------------------------------------------------------------*/

//���̹������ݽṹ
#define	PROC_MAX					16*2
#define	PROC_NAME_LEN				32*2
#define	AUTO_PROC_MAX				16*2
#define	SRV_PROC_MAX				8*2

//���̿�����
#define	PROC_CTRL_NULL				-1
#define	PROC_CTRL_START				(PROC_CTRL_NULL+1)
#define	PROC_CTRL_KILL				(PROC_CTRL_NULL+2)
#define	PROC_CTRL_GET				(PROC_CTRL_NULL+3)
#define	PROC_CTRL_FRESH				(PROC_CTRL_NULL+4)
#define	PROC_CTRL_RESTARTBUS		(PROC_CTRL_NULL+5)

//����״̬����
#define PROC_STATUS_BASE			-1
#define	PROC_STATUS_START			(PROC_STATUS_BASE+1)			/*����*/
#define	PROC_STATUS_STOP			(PROC_STATUS_BASE+2)			/*�����˳�*/
#define	PROC_STATUS_ERROR			(PROC_STATUS_BASE+3)			/*�쳣�˳�*/
#define	PROC_STATUS_NOTEXIST		(PROC_STATUS_BASE+4)			/*�����ڸÿ�ִ���ļ�*/
#define	PROC_STATUS_TIME			(PROC_STATUS_BASE+5)			/*���̼���*/

//����������ʽ
#define	PROC_START_MODE_BASE		-1
#define	PROC_START_MODE_NOTSTART	(PROC_START_MODE_BASE+1)		//������
#define	PROC_START_MODE_RESPAWN		(PROC_START_MODE_BASE+2)		//�ػ�
#define	PROC_START_MODE_AT			(PROC_START_MODE_BASE+3)		//��ʱ����
#define	PROC_START_MODE_CYCLE		(PROC_START_MODE_BASE+4)		//��������

/*-------------------------------------------------------------------------*/

//���¶���Ϊ������·��Ϣ����ʹ�õĶ���
#ifndef MAX_ITEM
#define								MAX_ITEM 30
#endif

//��·��Ϣ����
#ifndef LINK_MAX
#define	LINK_MAX					256
#endif

#define	HEADLEN						sizeof(HEAD)


//�����ڴ����ݿ�ṹ
#if defined(__unix)
#define		NET_SHM_DB_KEY			980415
#elif defined(WIN32)
#define		NET_SHM_DB_KEY			"SAC_NET_SHM_DB_KEY"
#endif

#define FileDic    "log/filedic"

/*-------------------------------------------------------------------------*/

#define SOCKET_STATUS_NULL			0x00		//������
#define SOCKET_STATUS_READABLE		0x01		//�ɶ�
#define SOCKET_STATUS_WRITEABLE		0x02		//��д
#define SOCKET_STATUS_WRITEREAD		0x03		//��д��

//�������ݿ�ṹ����
//Ȩ�޼�����
#define	attr_switch_host			attr1.bit0
#define	attr_check_time				attr1.bit1
#define	attr_dial_node				attr1.bit2
#define	attr_config_net				attr1.bit3
#define	attr_make_event				attr1.bit4

//ϵͳ�¼�����

/*-------------------------------------------------------------------------------*/

//˫������Ŷ���
#define	NET_NO_NULL					-1
#define	NET_NO_A					(NET_NO_NULL+1)				//A��
#define	NET_NO_B					(NET_NO_NULL+2)				//B��

//�������Ͷ���
#define	NET_TYPE_NULL				0
#define	NET_TYPE_DATA				(NET_TYPE_NULL+1)			/*������*/
#define	NET_TYPE_CMD				(NET_TYPE_NULL+2)			/*������*/
#define	NET_TYPE_STREAM				NET_TYPE_CMD				/*��������*/
#define	NET_TYPE_RAW				NET_TYPE_DATA				/*��������*/
#define NET_TYPE_MIRROR				NET_TYPE_DATA				/*����������*/
#define NET_TYPE_DBG				NET_TYPE_CMD				/*����������*/


//����״̬����
#define	NET_STATUS_BASE				0
#define NET_STATUS_NOTCONFIG		(NET_STATUS_BASE+1)			/*����δ����*/
#define NET_STATUS_GOOD				(NET_STATUS_BASE+2)			/*��������*/
#define NET_STATUS_NET_ERROR		(NET_STATUS_BASE+3)			/*�����쳣*/

/*-------------------------------------------------------------------------------*/


//��������
#define GramNull					0								/*�ձ���*/
//#define	NetConfigGram				(GramNull+1)					/*��������*/
//#define	ServerCfgGram				(GramNull+2)					/*���������ñ���*/
#define	RegGram						(GramNull+3)					/*ע�ᱨ��*/
#define	AtbGram						(GramNull+4)					/*ƽ������*/
#define	MailGram					(GramNull+5)					/*�ʼ�����*/
#define	ProcCtrlGram				(GramNull+6)					/*���̿���*/
#define	SyncTimeGram				(GramNull+7)					/*Уʱ����*/
#define	SysInfoGram					(GramNull+8)					/*ϵͳ��Ϣ����*/
#define	SysStateGram				(GramNull+9)					/*ϵͳ״̬������״̬����������Ϣ��*/
#define	SysEventGram				(GramNull+10)					/*ϵͳ�¼�*/
#define	SwitchHostGram				(GramNull+11)					/*�л�����*/
#define	SwitchServerGram			(GramNull+12)					/*�л�����������*/
#define	FileInforGram				(GramNull+13)					/*�ļ���Ϣ*/
#define	CenterInfoGram				(GramNull+14)					/*���ƻ���Ϣ*/
#define	DetectCenterGram			(GramNull+15)					/*��������Ϣ*/
#define	SrvInforGram				(GramNull+16)					/*��������Ϣ*/
#define	StreamGram					(GramNull+17)					/*ϵͳ������*/
#define	StopStreamGram				(GramNull+18)					/*ֹͣˢ��ϵͳ������*/
#define	RawDataGram					(GramNull+19)					/*�����ݱ���*/
#define	RawEventGram				(GramNull+20)					/*�������*/
#define	RTDBProcGram				(GramNull+21)					/*ʵʱ���ݿ����*/
#define	DeviceInforGram				(GramNull+22)					/*device information gram */
#define	DebugControlGram			(GramNull+23)					/*���Ա���*/
#define	ChanDataGram				(GramNull+24)					/*ͨ�����ݱ���*/
#define	MirrorDataGram				(GramNull+25)					/*�������ݱ���*/
#define	AtbGram2					(GramNull+26)					/*ƽ������2*/
#define	DBGram						(GramNull+27)					/*���ݿⱨ��*/
#define	HisStreamGram				(GramNull+28)					/*ϵͳ������*/
#define	StopHisStreamGram			(GramNull+29)					/*ֹͣˢ��ϵͳ������*/
#define MAX_SELDEF_MAIL_TYPE		256

//NetConfigGram����--��
//ServerCfgGram����--��
//ServerCfgGram����--��
//RegGram����

//AtbGram����
#define	ATB_GRAM_BASE			-1
#define	ATB_GRAM_STOP			(ATB_GRAM_BASE+1)
#define	ATB_GRAM_NETAB_UNKOWN	(ATB_GRAM_BASE+2)
#define	ATB_GRAM_NETA			(ATB_GRAM_BASE+3)
#define	ATB_GRAM_NETB			(ATB_GRAM_BASE+4)
#define	ATB_GRAM_NETAB			(ATB_GRAM_BASE+5)

//MailGram����---�ʼ�����
//ProcCtrlGram����--��
//SyncTimeGram����--��
//SysInforGram����--��
//SysStateGram����--��

//SysEventGram����
#define	SYS_EVENT_GRAM_BASE			-1
#define SYS_EVENT_GRAM_SAVE			(SYS_EVENT_GRAM_BASE+1)	/*ȫ����*/
#define SYS_EVENT_GRAM_PUT			(SYS_EVENT_GRAM_BASE+2)	/*���ش�*/

//SwitchHostGram����--��
//SwitchServerGram����--��
//CopyCRTGram����--��

//FileInforGram����--��
#define FILE_INFO_GRAM_BASE			-1
#define FILE_INFO_GRAM_CALLINFO		(FILE_INFO_GRAM_BASE+1)
#define FILE_INFO_GRAM_SAVEREQ		(FILE_INFO_GRAM_BASE+2)
#define FILE_INFO_GRAM_DELREQ		(FILE_INFO_GRAM_BASE+3)
#define FILE_INFO_GRAM_SAVESRV		(FILE_INFO_GRAM_BASE+4)
#define FILE_INFO_GRAM_DELSRV		(FILE_INFO_GRAM_BASE+5)

//CtrlNodeInfoGram����--��  -----(CtrlNodeInfoGramû����)
//SrvInforGram����--�� -----(SrvInforGramû����)
//StreamGram����--��
//StopStreamGram����--�����

//RawDataGram����
#define	RAW_DATA_GRAM_BASE				-1
#define RAW_DATA_GRAM_NUMS				(RAW_DATA_GRAM_BASE+1)
#define RAW_DATA_GRAM_YC				(RAW_DATA_GRAM_BASE+2)
#define RAW_DATA_GRAM_YX				(RAW_DATA_GRAM_BASE+3)
#define RAW_DATA_GRAM_KWH				(RAW_DATA_GRAM_BASE+4)
#define RAW_DATA_GRAM_STATE				(RAW_DATA_GRAM_BASE+5)
#define RAW_DATA_GRAM_CYCLE				(RAW_DATA_GRAM_BASE+6)
#define RAW_DATA_GRAM_CHANSTATE			(RAW_DATA_GRAM_BASE+7)
#define RAW_DATA_GRAM_DEVSTATE			(RAW_DATA_GRAM_BASE+8)
#define RAW_DATA_GRAM_YC_DATAFLAG		(RAW_DATA_GRAM_BASE+9)
#define RAW_DATA_GRAM_YX_DATAFLAG		(RAW_DATA_GRAM_BASE+10)
#define RAW_DATA_GRAM_KWH_DATAFLAG		(RAW_DATA_GRAM_BASE+11)
#define RAW_DATA_GRAM_STATE_DATAFLAG	(RAW_DATA_GRAM_BASE+12)
#define RAW_DATA_GRAM_CYCLE_DATAFLAG	(RAW_DATA_GRAM_BASE+13)
#define RAW_DATA_GRAM_CHAN_DATAFLAG		(RAW_DATA_GRAM_BASE+14)
#define RAW_DATA_GRAM_DEV_DATAFLAG		(RAW_DATA_GRAM_BASE+15)



//RawEventGram����--��

//RTDBProcGram����
#define	RTDB_PROC_GRAM_BASE			-1
#define	RTDB_PROC_GRAM_GET			(RTDB_PROC_GRAM_BASE+1)	/*ȡ������*/
#define	RTDB_PROC_GRAM_PUT			(RTDB_PROC_GRAM_BASE+2)	/*�޸�������*/
#define	RTDB_PROC_GRAM_UPDATE		(RTDB_PROC_GRAM_BASE+3)	/*SQL֮UPDATE����*/
#define	RTDB_PROC_GRAM_SELECT		(RTDB_PROC_GRAM_BASE+4)	/*SQL֮SELECT����*/
#define	RTDB_PROC_GRAM_RECALL		(RTDB_PROC_GRAM_BASE+5)	/*����һ����*/
#define	RTDB_PROC_GRAM_RELOAD		(RTDB_PROC_GRAM_BASE+6)	/*���¼���ʵʱ���ݿ�*/
#define	RTDB_PROC_GRAM_MODE			(RTDB_PROC_GRAM_BASE+7)	/*װ�ؿ�ģʽ*/
#define	RTDB_PROC_GRAM_SRV			(RTDB_PROC_GRAM_BASE+8)	/*�����з���������*/

//DeviceInforGram����--��

//DebugControlGram����
#define	DBG_GRAM_BASE				-1
#define	DBG_GRAM_GET_PROCINFO		(DBG_GRAM_BASE+1)		/*ȡ������Ϣ*/
#define	DBG_GRAM_RET_PROCINFO		(DBG_GRAM_BASE+2)		/*���ؽ�����Ϣ*/
#define	DBG_GRAM_CALL_INFO			(DBG_GRAM_BASE+3)		/*ȡ���̵�����Ϣ*/
#define	DBG_GRAM_RET_INFO			(DBG_GRAM_BASE+4)		/*���س̵�����Ϣ*/

//MirrorDataGram����
#define	MIRROR_GRAM_BASE			-1
#define	MIRROR_GRAM_REQ				(MIRROR_GRAM_BASE+1)		/*��������*/
#define	MIRROR_GRAM_DATA			(MIRROR_GRAM_BASE+2)		/*��������*/

/*-------------------------------------------------------------------------*/

//������Ϣ����
#define DEBUG_MAX_TYPE				64							//����ÿ�����������Ϣ����
#define DEBUG_MAX_PROC				16*2						//������������Ŀ
#define DEBUG_MAX_LINE				128							//������Ϣ����
#define DEBUG_MAX_LEN				256							//������Ϣ����

#define	DBGTYPE_BASE				-1
#define DBGTYPE_NORMAL				(DBGTYPE_BASE+1)
#define	NET_DBGTYPE_SCADA			(DBGTYPE_BASE+2)		//scada information
#define	NET_DBGTYPE_RX_GRAM			(DBGTYPE_BASE+3)		//���ռ�ر���
#define	NET_DBGTYPE_TX_GRAM			(DBGTYPE_BASE+4)		//���ͼ�ر���
#define	NET_DBGTYPE_CHECK_BUS		(DBGTYPE_BASE+5)		//check process BUS
#define	NET_DBGTYPE_CHECK_PROC		(DBGTYPE_BASE+6)		//BUS check child process 
#define	NET_DBGTYPE_RX_EVENT		(DBGTYPE_BASE+7)		//received event
#define	NET_DBGTYPE_TX_EVENT		(DBGTYPE_BASE+8)		//received event
#define	NET_DBGTYPE_RX_MAIL			(DBGTYPE_BASE+9)		//receive mail
#define	NET_DBGTYPE_TX_MAIL			(DBGTYPE_BASE+10)		//receive mail
#define	NET_DBGTYPE_FILE_INFO		(DBGTYPE_BASE+11)		//file management information
#define	NET_DBGTYPE_RX_CMD			(DBGTYPE_BASE+12)		//recevie command
#define	NET_DBGTYPE_CHECK_SRV_PROC	(DBGTYPE_BASE+13)		//check server process
#define	NET_DBGTYPE_SYS_CTRL		(DBGTYPE_BASE+14)		//system control
#define	NET_DBGTYPE_STREAM			(DBGTYPE_BASE+15)		//system data
#define	NET_DBGTYPE_RTDB_PROC		(DBGTYPE_BASE+16)		//rtdb procedures
#define	NET_DBGTYPE_RAWD			(DBGTYPE_BASE+17)
#define	NET_DBGTYPE_RAWDTHREAD		(DBGTYPE_BASE+18)
#define	NET_DBGTYPE_RAWDSRV			(DBGTYPE_BASE+19)
#define	NET_DBGTYPE_STREAMSRV		(DBGTYPE_BASE+20)
#define	NET_DBGTYPE_SYNCTIME		(DBGTYPE_BASE+21)
#define	NET_DBGTYPE_COPYSCR			(DBGTYPE_BASE+22)
#define	NET_DBGTYPE_ROMOTEDBG		(DBGTYPE_BASE+23)		//remote dbg
#define	NET_DBGTYPE_SOCKET			(DBGTYPE_BASE+24)		//remote socket level info
#define	NET_DBGTYPE_MIRROR			(DBGTYPE_BASE+25)		//mirror srv/client


#define DB_DBGTYPE_BASE				(DBGTYPE_BASE+100)
#define DB_DBGTYPE_WR				(DBGTYPE_BASE+101)

#define DBG_TYPE_NORMAL		DBGTYPE_NORMAL

enum RemoteDebug
{
	SetDebug = 0,
	ReadDebug,
	READCONFIG,
	READRTDB,
	SWITCHHOST,
	SWITCHSERVER,
	LOADRDB,
	GETPROCINFO,
	KILLPROC,
	STARTPROC
};

/*-------------------------------------------------------------------------*/

//���������Ϣ����
#define	NET_ERR_BASE				0
#define	NET_ERR_NOT_IN_SYSTEM		(NET_ERR_BASE-1)
#define	NET_ERR_NOT_WORKING			(NET_ERR_BASE-2)
#define	NET_ERR_NO_RIGHT			(NET_ERR_BASE-3)
#define	NET_ERR_TX_ERROR			(NET_ERR_BASE-4)
#define	NET_ERR_RX_ERROR			(NET_ERR_BASE-5)
#define	NET_ERR_MEM_ERROR			(NET_ERR_BASE-6)
#define	NET_ERR_LINK_ERROR			(NET_ERR_BASE-7)
#define	NET_ERR_FERT_STOP			(NET_ERR_BASE-8)
#define	NET_ERR_SERVER_STOP			(NET_ERR_BASE-9)


/*-------------------------------------------------------------------------*/

//zip  define

//"15 Bit VBR Encoder"
#define  ZIP_BITS				15		
#define  ZIP_MAX_CODE			0x7fff
//#define  ZIP_MAX_CODE			((1<<ZIP_BITS)-1)
#define  ZIP_TABLE_SIZE			(256*1024)//35023	
#define  ZIP_TABLE_BANKS		0x89
//#define  ZIP_TABLE_BANKS		((ZIP_TABLE_SIZE>>8)+1)
#define  ZIP_END_OF_STREAM		256
#define  ZIP_BUMP_CODE			257
#define  ZIP_FLUSH_CODE			258
#define  ZIP_FIRST_CODE			259
#define  ZIP_UNUSED				-1

#define  ZIP_COMPRESSMEMSIZE_FACTOR				16				
#define  ZIP_COMPRESSMEMADDSIZE_FACTOR			16				
#define  ZIP_EXPANDMEMSIZE_FACTOR				1				
#define  ZIP_EXPANDMEMADDSIZE_FACTOR			1				

#define  COMPRESS_OK		0
#define  COMPRESS_ERR		-1
#define  EXPAND_OK			0
#define  EXPAND_ERR			-1
#define  COMPRESS_KEY		0xEB90
#define  COMPRESSED			1
#define  UNCOMPRESSED		0

/*-------------------------------------------------------------------------*/

#endif		//all of end
