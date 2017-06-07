#include "fes/msgserviceacceptor.h"
#include "fes/msgservice.h"

#include <ace/Message_Block.h>
#include <ace/Log_Msg.h>
#include <ace/OS.h>

#include <ace/Guard_T.h>

Msg_service_acceptor* Msg_service_acceptor::instance = NULL;

Msg_service_acceptor*  Msg_service_acceptor::get()
{
	if (NULL == instance)
	{
		instance = new Msg_service_acceptor();
	}

	return instance;
}


Msg_service_acceptor::Msg_service_acceptor()
{

}

Msg_service_acceptor::~Msg_service_acceptor()
{
	close();
}

void Msg_service_acceptor::init()
{
	//����acceptor
	ACE_INET_Addr server_addr(PORT_NO);
	m_acceptor.open(server_addr);
// 	while( !m_acceptor.open(server_addr))
// 	{
// 		ACE_ERROR((LM_ERROR,"%p\n","Msg_service_acceptor Error in open !"));
// 	}

	if ( (m_acceptor.get_local_addr(server_addr)) == -1 )
	{
		ACE_ERROR((LM_ERROR,"%p\n","Msg_service_acceptor Error in get_local_addr !"));
	}
	else
	{
		ACE_DEBUG ((LM_DEBUG,"Msg_service_acceptor starting server at port %d !\n",
			server_addr.get_port_number())); 
	}

	//�����̳߳�
	for ( int ntasknum = 0; ntasknum < MAX_THREAD_SIZE; ntasknum++ )
	{
		Msg_service* create_service = new Msg_service();

		create_service->open(0);

		m_service_list.push_back(create_service);

	}

}

int Msg_service_acceptor::open(void *)
{
	init();
	activate();
	return 0;
}

int Msg_service_acceptor::close(u_long)
{
	m_active_task.clear();
	m_service_list.clear();
	m_acceptor.close();
	return 0;
}

int Msg_service_acceptor::svc()
{
	ACE_DEBUG((LM_DEBUG, "Msg_service_acceptor start thread !\n")); 
	// do thread specific work here 

	Msg_service* free_service;
// 	ACE_Time_Value timeout (0,0);
// 	ACE_Time_Value timeout = ACE_Time_Value::zero;
	while(true)
	{
		//��ȡ���е�service
		free_service = get_free_service();
		if (!free_service)	//�޿���service
		{
			ACE_ERROR((LM_ERROR,"There's no free service !\n"));
			continue;
		}

		//����accept
		int retcode = m_acceptor.accept(free_service->get_peer(), 0);
		if ( -1 == retcode )
		{
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
				//continue;
			}
			else
			{
// 				ACE_ERROR((LM_ERROR, "%p\n", "msgserviceacceptor accept error"));
				continue;			//����continue
			}
			ACE_OS::sleep(ACE_Time_Value(0,1*1000));

		}
		else
		{
			ACE_DEBUG((LM_DEBUG,"%p\n","Msg_service_acceptor accept new connect !"));
			free_service->get_peer().enable(ACE_NONBLOCK);			//������tcpΪ������ģʽ
		}


		ACE_OS::sleep(ACE_Time_Value(0,1*1000));
		//���ӳɹ���Ȼ�Ͳ�free��
	}

	return 0;

}

void Msg_service_acceptor::put_msg(const FE_MSG& msg)
{
	if(m_active_task.empty())		//�ޱ�������
	{
		return;
	}
	Range range = m_active_task.equal_range(msg.channel_no);	//Ѱ��ͨ���Ŷ�Ӧ��service;

	for(CIT i = range.first; i != range.second; ++i)
	{
		ACE_Message_Block *sendmb = new ACE_Message_Block(sizeof(FE_MSG));
		ACE_OS::memcpy(sendmb->wr_ptr(), (unsigned char*)&msg, sizeof(FE_MSG));
		(i->second)->putq(sendmb);
	}

// 	delete sendmb;

};



void Msg_service_acceptor::remove_service(const int& channel_no, const Msg_service* service)
{
	//����ʵ��
	if ( channel_no < 0 )		//��ͨ����Ϊ������������
	{
		return;		
	}

	ACE_Guard<ACE_Thread_Mutex> guard(task_list_mutex);
	//���������ٲ���
	{
		if (!m_active_task.empty())
		{
			Range_NOCONST range = m_active_task.equal_range(channel_no);

			for(IT i = range.first; i != range.second; ++i)
			{
				if (i->second == service)
				{
					m_active_task.erase(i);
					break;
				}
			}
		}
	}
}

void Msg_service_acceptor::add_service(const int& channel_no, Msg_service* service)
{
	//����ʵ��
	bool found = false;
	ACE_Guard<ACE_Thread_Mutex> guard(task_list_mutex);
	//���������ٲ���
	{
		if (!m_active_task.empty())
		{
			Range range = m_active_task.equal_range(channel_no);
			for(CIT i = range.first; i != range.second; ++i)
			{
				if (i->second == service)
				{
					found = true;
					break;
				}
			}	
		}

		if (!found)
		{
			m_active_task.insert(make_pair<int, Msg_service*>(channel_no, service));
		}

	}
}


Msg_service*  Msg_service_acceptor::get_free_service(void)
{
	Msg_service* free_service = NULL;
	for ( int ntasknum = 0; ntasknum < MAX_THREAD_SIZE; ++ntasknum )
	{
		if (m_service_list[ntasknum]->service_free())
		{
			free_service = m_service_list[ntasknum];
			break;
		}
	}
	return free_service;
}
