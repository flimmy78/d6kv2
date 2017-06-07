/**
@file platformwrayyer.h
@brief ǰ��ƽ̨��װͷ�ļ�

����ACE��pthread-win32������Դ����windows�¶�������pid��timespec��posix��׼
��Ҫ�����ͣ��������߻��ñ���ʱ������ͻ��������ƽ̨ͷ�ļ�������pthread��
Ϊ����ǰ��ʹ��ACE����ͬʱ����ƽ̨����дƽ̨��װ

@author zhangyun 
@version 1.0.0
@date 2013-11-04
*/
#ifndef FES_PLATFORM_WRAPPER_H_
#define FES_PLATFORM_WRAPPER_H_ 

#ifdef WIN32
#ifdef PLATFORM_WRAPPER_DLL
#define	PLATFORM_WRAPPER_EXPORT __declspec(dllexport) 
#else
#define	PLATFORM_WRAPPER_EXPORT __declspec(dllimport) 
#endif
#else
#define	PLATFORM_WRAPPER_EXPORT
#endif

#include "fes/fesshare.h"

//��ȡ���뼶ʱ��
void PLATFORM_WRAPPER_EXPORT get_time(FETIME* time_ptr);

//���ú�����ͬ��ͷ��BCHУ��
#define  START_CODE		0xEB
#define  SYNC_CODE		0x90

//CRCУ����״̬
enum CRC_STATE
{
	CRC_READ = 0 ,
	CRC_WRITE = 1
};

bool PLATFORM_WRAPPER_EXPORT Scan_msg_head(unsigned char* buf);
void PLATFORM_WRAPPER_EXPORT MakeBch(unsigned char* bch,unsigned char* buf);
void PLATFORM_WRAPPER_EXPORT MakeBCHList (void);
bool PLATFORM_WRAPPER_EXPORT CRC_test(unsigned char *buf , int len , CRC_STATE flag ) ;


//�ɼ���״̬
#define		MAINDBG		201
#define		G101DBG		202

#define		PUB_FEP_SERVER				"PUBFEP"			/*3���ɼ�ǰ�û� front end processor*/

/**
@brief �����ڴ���
*/
class PLATFORM_WRAPPER_EXPORT Share_memery
{
public:
	/**
	@brief ���������ڴ�
	@param in int shm_key �����ڴ�ID
	@param in int shm_lenth �����ڴ泤��
	@return �ɹ����ع����ڴ��ַ��ʧ�ܷ���NULL
	*/
	static void* create_share_memory(int shm_key, int shm_lenth);
};

/**
@brief ���̹�����
*/
class PLATFORM_WRAPPER_EXPORT Proc_manager
{
public:
	/**
	@brief ������ע�����
	@param in char* asc_name ����ascii��
	@param in char* ch_name ���̺�����
	@param in char* username �����û���
	@param in HWND hwnd ���������ھ��
	*/
//#ifdef WIN32
//	static void start_proc(char *asc_name, char *ch_name, char *username, HWND hwnd = NULL);
//#else
	static void start_proc(char *asc_name, char *ch_name, char *username);	
//#endif
	/**
	@brief �жϽ����Ƿ��Ѿ�����
	@param in char* proc ������
	@return �����ڷ���-1�����ڷ���0
	*/
	static int proc_exist(char* proc);
	/**
	@brief ��������Ƿ������У�֪ͨ���߱�������������
	
	������һ��ǳ���ʱ�Ĳ���ʱ����Ҫ��������֮����ֹ����
	ɱ������
	@return ���з���-1�����з���0
	*/
	static int check_parent(void);
	/**
	@brief �����߷�ע�����
	*/
	static void end_proc(void);

	static void putdbg(int type,char *format,...);

};

/**
@brief ����״̬��

��ʵ���˵�ǰ��Ҫ�ļ����ӿڣ�������µ���Ҫ�����������
*/
class PLATFORM_WRAPPER_EXPORT Net_stat
{
public:
	/**
	@brief ��ȡ����������
	@return ����������
	*/
	static int get_host_no(void);
	/**
	@brief ���ݽڵ�����ȡ�ڵ��
	@param in const char* node_name �ڵ���
	@return ������
	*/
	static int get_host_no(char* node_name);
	/**
	@brief ���ݽڵ�����ȡ������
	@param in const char* node_name �ڵ���
	@return ������
	*/
	static char* get_host_name(char* node_name);
	/**
	@brief ��ȡ����ǰ�ýڵ�Ľڵ��
	@param out int* nodes д�ڵ�ŵ��ڴ�
	@param in int lenth �ڴ�ĳ���
	@return �ڵ������
	*/
	static int get_fe_nodes(int* nodes, const int&lenth);
	/**
	@brief ��ȡ����I��ǰ�ýڵ��������
	@param out char** hosts д���������ڴ�
	@param in int lenth �ڴ�ĳ���
	@return ����������
	*/
	static int get_fe_hosts(char** hosts, const int&lenth);
	/**
	@brief ��ȡ������ǰ�ýڵ��е����
	@param in int host_no �������
	@return ��ȡ�ɹ�������ţ�ʧ�ܷ���-1
	*/
	static int get_fe_host_index(int host_no);
	/**
	@brief ��ȡ����״̬
	@param in int host_no �������
	@return ״̬��������0���˳�����-1
	*/
	static int get_host_stat(int host_no);
	/**
	@brief ��ȡ�ڵ���
	@return �ڵ���
	*/
	static char* get_node_name(void);

	/**
	@brief ��ȡ�ڵ���
	@param in int host_no �������
	@return �ڵ���
	*/
	static char* get_node_name(int host_no);

	/**
	@brief ��ȡ�Ƿ���ǰ��
	@param in int host_no �������
	@return ����������0�����Ƿ���-1
	*/
	static int get_host_master_flag(int host_no);

	/**
	@brief ��ȡ������ַ
	@param in int host_no �������
	*/
	static void get_host_addr(int host_no, struct in_addr* addr);

	/**
	@brief ��ȡ�����Ƿ������ɼ�ǰ��
	@return �������ɼ�ǰ��1�����Ƿ���0
	*/
	static int get_PUBFEP_flag( );

};

/**
@brief ���ݿ�ӿ���

��ʵ����SQL��ʽ��ȡ�Ľӿ�

һ��������������һ��ǰ�õļ��ع��̣���Ҫһ��open�����ɴ�select��һ��
close���

ע��ýӿڵļ������������̲߳���ȫ�ģ���ΪĿǰû�ж��̼߳��ص���Ҫ
*/
class PLATFORM_WRAPPER_EXPORT Db_connector
{
public:
	/**
	@brief �����ݿ�
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	static int open(void);
	/**
	@brief �����ݿ�
	@return �ɹ�����������ʧ�ܷ���-1
	*/
	static int select(const char* sql, void**result_pt);
	/**
	@brief �ر����ݿ�
	*/
	static void close(void);
};

/**
@brief ǰ���ʼ���
*/
class PLATFORM_WRAPPER_EXPORT Fe_mail
{
public:
	/**
	@brief �����ʼ�
	@param out void** mail �ʼ�������
	@param out int* lenth �ʼ��ĳ���
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	static int receive(void** mail, int* lenth);
	/**
	@brief �����ʼ�������scada�ڵ�
	@param in void* mail �ʼ�������
	@param in int* lenth �ʼ��ĳ���
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	static int send_to_scada(void* mail, int lenth, int level);
	/**
	@brief �����ʼ�������ǰ�ýڵ�
	@param in void* mail �ʼ�������
	@param in int* lenth �ʼ��ĳ���
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	static int send_to_fe(void* mail, int lenth);

	static int send_prot_ack( void* mail, int lenth, int rtu_no);
private:
	/**
	@brief �����ʼ�
	@param in char* user_name �ʼ����û�
	@param in void* mail �ʼ�������
	@param in int* lenth �ʼ��ĳ���
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	static int send(char* user_name, void* mail, int lenth, int level);
};

/**
@brief ǰ���¼���

Ŀǰ��ʵ����ͨ�����ն��¼�����������ͨ���л����˿�ֵ���¼�
��������չ
*/
class PLATFORM_WRAPPER_EXPORT Fe_event
{
public:
	/**
	@brief ����ͨ������
	@param in const char* station ��վ
	@param in const char* channel_name ͨ����
	@param in const char* text�¼����ı�
	*/
	static void make_channel_event(float& val, const char* station, const char* channel_order, 
							  const char* text);

	static void make_rtu_event(float& val, const char* station, const char* rtu_name, 
							   const char* text);

private:
	/**
	@brief ��������
	@param in const int& sort �¼�����
	@param in const int& type �¼�����
	@param in const char* object ����
	@param in const char* member0 ��Ա0
	@param in const char* member1 ��Ա1
	@param in const char* text�¼����ı�
	*/
	static void make_event(const int& sort, const int& type, float& val,
					  const char* object, const char* member0, 
					  const char* member1, const char* text);
};

/**
@brief ǰ��SCADA������
*/
class PLATFORM_WRAPPER_EXPORT Fe_scasrv
{
public:
	/**
	@brief FEP����������SCADA��������ң�ط�У��Ϣ
	@param in int rtuno �ն���
	@param in int ykno ң�غ�
	@param in int yk_ret ң�ط�У�������ȷΪ0������Ϊ-1
	@param in const char* msg ң�ط�У����
	*/
	static int fe_send_ykret(int rtuno, int ykno, int yk_ret, const char* msg);

	/**
	@brief FEP����������SCADA�������ĵ��������ʼ�
	@param in FE_DATA_EVENT_NODE* ��������vector��v[0]��ָ��
	@param in int eventnum �������
	*/
	static int fe_send_electric_event(const FE_DATA_EVENT_NODE* tmpEventData, int eventnum);

	/**
	@brief FEP����������SCADA�������ĵ��������ʼ�
	@param in FE_DATA_EVENT_NODE* ��������vector��v[0]��ָ��
	@param in int eventnum �������
	*/
	static int fe_send_prot_event( void* mail);

private:

};

/**
@brief ǰ������ӿ���
*/
class PLATFORM_WRAPPER_EXPORT Fe_net_control
{
public:
	/**
	@brief FESǰ��ϵͳ��gps��ʱ
	�ɹ�����0�����󷵻�-1
	*/
	static int check_time(int year,int month,int day,int hour,int minute,int second,int milisecond);

private:

};

#endif






