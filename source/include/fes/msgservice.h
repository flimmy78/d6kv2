#ifndef MSG_SERVICE_H_
#define MSG_SERVICE_H_

#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Task.h>

#include "fes/fesshare.h"

#define  SIZE_DATA 2		//����msgview���������ͨ����

class Msg_service_acceptor;

class Msg_service : public ACE_Task<ACE_MT_SYNCH>
{
public:
	enum
	{
		FREE_SERVICE = -2,			//����״̬
		UN_INIT = -1			//δ��ʼ��ͨ����
	};
	Msg_service();
	~Msg_service();

	/**
	@brief �̳���Task��������ʵ�������麯��
	*/
	int open(void *);
	int close(u_long flags = 0);

	/**
	@brief �߳���

	���ڲ���һ��ѭ��

	while(true)
	{
		while(true)
		{
			//�����ж� break��
			//channel_no���ɱ���ֵ�� break��

			//����Ϣ���л�ȡ����

			//��peer������ʧ��break��
			

			//��peer����,����channel_no��ʧ��break��

			if(ͨ���ŷ����仯)
			{
				acceptor.remove_service(old_channel_no, this);
				acceptor.add_service(m_channel_no, this);
			}
		}

		acceptor.remove_service(m_channel_no, this);
		m_channel_no = -1��
	}
	*/
	int svc(void);

	/**
	@brief �������
	*/
	int service_free(){return m_channel_no < -1 ? 1 : 0;};

	ACE_SOCK_Stream& get_peer(void){return m_peer;};

private:
	int						m_channel_no;
	int						m_current_host_no;
	ACE_SOCK_Stream			m_peer;
	FE_MSG					msgmail;
	unsigned char			recv_buf[SIZE_DATA];
// 	Msg_service_acceptor*   acceptor;
};



#endif