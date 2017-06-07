/**
@file msgtransceiver.h
@brief �����շ�������
@author hongxiang 
@version 1.0.0
@date 2013-11-01
*/
#ifndef MSG_TRANSCEIVER_H_
#define MSG_TRANSCEIVER_H_

//#include "sysdef.h"
#include "fes/arraylist.h"
#include "fes/fesshare.h"
#include "fes/fesshm.h"
#include "fes/fesdev.h"
#include "ace/Task.h"
#include "ace/Thread_Mutex.h"

#ifdef WIN32
#ifdef FES_PROTOCOL_API_DLL
#define	MSG_TRANSCEIVER_EXPORT __declspec(dllexport) 
#else
#define	MSG_TRANSCEIVER_EXPORT __declspec(dllimport) 
#endif
#else
#define	MSG_TRANSCEIVER_EXPORT
#endif

class Msg_transceiver;

//�����շ���ĵ�ʵ��
typedef ACE_Singleton<Msg_transceiver, ACE_SYNCH_MUTEX> Msg_sender;

/**
@brief �����շ���
*/

class MSG_TRANSCEIVER_EXPORT Msg_transceiver : public ACE_Task<ACE_MT_SYNCH>
{
	//�����ַ�����
	enum
	{
		NET_NUM = 2,
		SHORTCHAR_LEN = 24,
		DEFAULTCHAR_LEN = 64,
		NORMALCHAR_LEN = 128,
		LONGCHAR_LEN = 256,
		DEVICE_TX_BUF_SIZE = 16384,
		SEND_MSG_TIME_SPAN = 1
	};

	friend class ACE_Singleton<Msg_transceiver, ACE_SYNCH_MUTEX>;

public:

	~Msg_transceiver();

	/**
	@brief ����
	*/
	virtual int open (void *args = 0);

	/**
	@brief ֹͣ
	*/
	virtual int close (u_long flags = 0);

	/**
	@brief �߳���
	*/
	virtual int svc(void);
	/**
	@brief ��ͨ���˿�
	*/
	void open_dev(void);

	/**
	@brief �ر�ͨ���˿�
	*/
	void close_dev(void);

	/**
	@brief ͨ���˿��Ƿ��
	�򿪷���true��δ�򿪷���false
	*/
	bool is_open(void);

	/**
	@brief �ѵ������Ľṹ������д��ȫ�ֻ���
	@param in FE_MSG& msg ����
	*/
	void put_msg(const FE_MSG& msg)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_msg_mutex);
		m_msg_list.insert_data(msg);
	};

	/**
	@brief �Ѷ�����Ľṹ������д��ȫ�ֻ���
	@param in vector<FE_MSG>& msgs ��������
	*/
	void put_msg(vector<FE_MSG>& msgs)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_msg_mutex);
		m_msg_list.insert_data(msgs);
	};

	/**
	@brief �����һ����������
	*/
	void clear_head_msg(void){m_msg_list.clear_head();};

	/**
	@brief ��ȡ�׸����������ָ��
	@return �ɹ�����ָ�룬ʧ�ܷ���NULL
	*/
	FE_MSG* get_head_msg(void){return m_msg_list.get_head_array();};

	/**
	@brief ��ȡ�׸���������ĳ���
	@return �ɹ����س��ȣ�ʧ�ܷ���-1
	*/
	int	get_head_msg_size(void){return m_msg_list.get_head_size();};

protected:

	/**
	@brief �ѷ��ͻ����������ݷ��ͳ�ȥ

	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int send_msg(void);

private:
	/**
	@brief ���캯������ʼ����Ա��������ʽ����
	*/
	Msg_transceiver();
	
	/**
	@brief �������캯����ֻ������ʵ��
	
	��������������л����Ĭ�Ͽ������캯�������ֽڿ����������ܻ��𻵵���
	�е����ݣ���ֻ������ʵ�ֵķ�������Ĭ�Ͽ������캯���������ʹ���е���
	�������캯����������������
	*/
	Msg_transceiver(const Msg_transceiver&);

private:
	ACE_INET_Addr	m_addr; 
	ACE_SOCK_Stream		m_peer; 
	ACE_SOCK_Connector	m_connector; 
//	map<int, FE_MSG> m_msg_map; ///ͨ������ͨ������֮���ӳ��
	Array_list<FE_MSG> m_msg_list;  ///�����շ�����
	ACE_Thread_Mutex   m_msg_mutex;	///<�������
};

#endif