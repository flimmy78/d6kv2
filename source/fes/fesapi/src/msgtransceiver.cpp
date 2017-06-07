/**
@file msgtransceiver.cpp
@brief ���Ľ��շ������ʵ��

@author hongxiang 
@version 1.0.0
@date 2013-11-1
*/
#include "fes/msgtransceiver.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"

#if defined (__unix)
/*#include "linux/tcp.h"*/
#include "netinet/tcp.h"
#endif

using namespace std;

//ͬ��ͷ
unsigned char SYNC_FRAME_VAL[6]={0xEB,0x90,0xEB,0x90,0xEB,0x90};


/**************************************************************************
���캯��
**************************************************************************/
Msg_transceiver::Msg_transceiver()
:m_msg_list(Array_list<FE_MSG>(4096, 4))
{
	m_addr.set(MSG_MNG_PORT,"127.0.0.1",1,AF_INET);
	open();
}

/**************************************************************************
��������
**************************************************************************/
Msg_transceiver::~Msg_transceiver()
{

}

/**************************************************************************
����
**************************************************************************/
int Msg_transceiver::open(void *args)
{
	activate();
	ACE_DEBUG((LM_DEBUG, "�����շ��߳�������\n"));
	return 0;
}

/**************************************************************************
ֹͣ
**************************************************************************/
int Msg_transceiver::close(u_long flags)
{
	m_msg_list.~Array_list();
	close_dev();
	ACE_DEBUG((LM_DEBUG, "�����շ��߳��˳���\n"));
	return 0;
}

/**************************************************************************
�߳���
**************************************************************************/
int Msg_transceiver::svc(void)
{
	while(true)
	{
		open_dev();
		while(is_open())
		{
// 			ACE_Guard<ACE_Thread_Mutex> guard (m_msg_mutex);
			int ret = send_msg();
			if (ret<0)
			{
				ACE_DEBUG((LM_DEBUG, "���ķ����̷߳��ͳ���\n"));
				break;
			}
			ACE_OS::sleep(SEND_MSG_TIME_SPAN);
		}
		ACE_OS::sleep(SEND_MSG_TIME_SPAN);
	}
	return 0;
}

/**************************************************************************
��ͨ���˿�
**************************************************************************/
void Msg_transceiver::open_dev(void)
{
	if(m_peer.open(SOCK_STREAM,AF_INET,IPPROTO_TCP,0) == -1)
	{
		ACE_ERROR((LM_ERROR,"%p\n","Error in open"));

		if(m_peer.close() == -1)
		{
			ACE_ERROR((LM_ERROR,"%p\n","Error in close"));
			return;
		}
		return;
	}
	ACE_DEBUG((LM_INFO,ACE_TEXT("Msg_transceiver �����׽��ֳɹ���\n")));

	struct linger m_lin;
	m_lin.l_onoff = 1;	//�޸�Ϊ1��linger��Ϊ0���رն˿�ʱ������CLOSE_WAIT��״̬
	m_lin.l_linger = 0;
	m_peer.set_option(SOL_SOCKET, SO_LINGER, (void*)&m_lin, sizeof(linger));

	//����Nagle�㷨
	int on = 1;
	m_peer.set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(on));

	//���÷��ͽ��ջ�����
	int temp = DEVICE_TX_BUF_SIZE;
	m_peer.set_option(SOL_SOCKET, SO_RCVBUF, (char *)&temp, sizeof(temp));
	m_peer.set_option(SOL_SOCKET, SO_SNDBUF, (char *)&temp, sizeof(temp));

	//����Ϊ������ģʽ
	m_peer.enable(ACE_NONBLOCK);

	ACE_Time_Value time_out(1, 0);		//add by lujiashun ������ģʽ���賬ʱ��connect�᷵��-1;

	if(m_connector.connect(m_peer, m_addr, &time_out) == -1)
	{
		ACE_ERROR((LM_ERROR,"%p\n","Msg_transceiver Error in Connect"));
		return;
	}
	ACE_DEBUG((LM_INFO,ACE_TEXT("Msg_transceiver ���ӳɹ���\n")));
}

/**************************************************************************
�ر�ͨ���˿�
**************************************************************************/
void Msg_transceiver::close_dev(void)
{
	if(m_peer.close() == -1)
	{
		ACE_ERROR((LM_ERROR, "%p\n", "Error in close"));
		return;
	}
	ACE_DEBUG((LM_INFO,ACE_TEXT("�ر�ͨ���ɹ���\n")));
}

/**************************************************************************
ͨ���Ƿ��
**************************************************************************/
bool Msg_transceiver::is_open(void)
{
	return m_peer.get_handle() >= 0 ? true : false;
}


/**************************************************************************
�ѷ��ͻ����������ݷ��ͳ�ȥ
**************************************************************************/
int Msg_transceiver::send_msg(void)
{
	int m_send_lenth = 0;	//���ͱ��ĵĳ���
	unsigned char send_buf[1024];		//���ͻ���
	FE_MSG *send_msg_ptr;

	if(!m_msg_list.empty())		///�޿ɷ��ͱ��Ĳ�����
	{
		FE_MSG *msg_ptr = get_head_msg();
		if (!msg_ptr)
		{
			return 0;
		}
		for(int n_vector_num =0; n_vector_num < get_head_msg_size(); n_vector_num++)
		{
			send_msg_ptr = msg_ptr + n_vector_num;
			ACE_OS::memset(send_buf, 0, 1024);
			ACE_OS::memcpy(send_buf, SYNC_FRAME_VAL, 6); //����ͷ��
			ACE_OS::memcpy(&send_buf[6], (unsigned char*)send_msg_ptr, sizeof(FE_MSG)); //����
			m_send_lenth = m_peer.send(send_buf, sizeof(FE_MSG)+6);
			if (m_send_lenth <= 0)	//����ʧ�ܣ�����ѭ��
			{
				break;
			}
		}
		//��շ��͹�������
		ACE_Guard<ACE_Thread_Mutex> guard (m_msg_mutex);	//������d6000
		clear_head_msg();
	}
	else
		return 0;
	
	return m_send_lenth > 0 ? 0 : -1;
}
