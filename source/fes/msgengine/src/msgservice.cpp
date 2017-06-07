#include "fes/msgservice.h"
#include "fes/msgserviceacceptor.h"

#include "fes/fesshm.h"

#include <ace/Log_Msg.h>
#include <ace/Message_Block.h>

#include <ace/OS.h>


Msg_service::Msg_service()
{
	m_channel_no = FREE_SERVICE;		//��ʼ��ͨ����ΪFREE_SERVICE��Ĭ�Ͽ�ת
	m_current_host_no = Fes_shm::get().fe_sys_data_ptr->host_no;		//��ǰ������hostno
}

Msg_service::~Msg_service()
{
	close();
}

int Msg_service::open(void *)
{
	ACE_DEBUG((LM_DEBUG,"(%t) Msg_Service open !\n")); 
	//Activate the object with a thread in it. 
	activate(); 
	return 0; 

}

int Msg_service::close(u_long)
{
	ACE_DEBUG((LM_DEBUG, "(%t) Msg_Service being closed down !\n"));

	ACE_OS::memset(recv_buf,0,SIZE_DATA);
	m_peer.close();
	return 0; 

}

int Msg_service::svc()
{
	ACE_DEBUG((LM_DEBUG, "(%t) Msg_Service start thread !\n")); 
	// do thread specific work here 

	ACE_INET_Addr client_addr;
	int byte_count=0; 
	int new_channel_no = 0;

	while(true)
	{
		if(m_peer.get_remote_addr(client_addr) == -1)
		{
			ACE_OS::sleep(ACE_Time_Value(0,1*1000));			//��ת�̣߳�����cpuʹ�ã�����cpuʹ����
			continue;
		}
		else
		{
			ACE_DEBUG((LM_DEBUG,"(%t) Msg_service connect client !\n"));
			m_channel_no = UN_INIT;
		}

		while(true)
		{
			ACE_OS::memset(recv_buf,0,SIZE_DATA); //��ս��ջ�����

			byte_count = m_peer.recv(recv_buf,SIZE_DATA);
			if ( byte_count == 0 )			//�����ж� break��
			{
				ACE_ERROR ((LM_ERROR, "%p\n", "(%t) Msg_Service Stream Closed !")); 
				m_peer.close();
				break;
			}
			else if( byte_count == -1 )		//���տգ����������޽������� break;
			{
				if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
				{
					if (m_channel_no == UN_INIT)		//msgview���ӳɹ�����δ����ͨ��
					{
						ACE_OS::sleep(ACE_Time_Value(0,1*1000));	//�׳�cpu
						continue;
					}	
				}
				else
				{
					ACE_ERROR ((LM_ERROR, "%p\n", "(%t) Msg_Service Stream recv error!")); 
					m_peer.close();
					break;
				}
			}
			else 							//�ɹ�����	
			{ 
				recv_buf[byte_count]=0; 
				new_channel_no = recv_buf[0] + recv_buf[1]*256;		//��ʱԼ����λ��ǰ����λ�ں�
				ACE_DEBUG((LM_DEBUG,"(%t) Msg_Service received request channel_no = %d !\n",new_channel_no));

				//�����ڴ��ȡ�����бȶ� hostno
				
				CHANDATA* pchanneldata =  Fes_shm::get().chan_data_ptr + new_channel_no;
				int channel_host_no = pchanneldata->curr_attend_host;				//��ͨ����ֵ��ڵ�no


				unsigned char confirmbuf[2];
				if (channel_host_no != m_current_host_no)//channel_no���ɱ���ֵ�� break
				{
// 					confirmbuf[0] = 0xff;
// 					confirmbuf[1] = 0xff;
// 					m_peer.send(confirmbuf,2);
					m_peer.close();
					break;
				}
				else
				{
					confirmbuf[0] = 0xcc;
					confirmbuf[1] = 0xcc;
					m_peer.send(confirmbuf,2);
				}
				

				//��ͨ���Ÿı�,����ע�����
				if( new_channel_no != m_channel_no )		
				{
					Msg_service_acceptor::get()->remove_service(m_channel_no, this);
					Msg_service_acceptor::get()->add_service(new_channel_no, this);
					m_channel_no = new_channel_no;
				}
			}

			//����Ϣ���л�ȡ����
			ACE_Message_Block * rmb = 0;
 			ACE_Time_Value timeout(1,0);			//��ʱʱ��1S
			if(getq( rmb, &timeout )==-1)		//��ʱ��������ֱ��continue
			{
				ACE_OS::sleep(ACE_Time_Value(0,1*1000));
				continue;
			}
			ACE_OS::memset((unsigned char*)&msgmail,0,sizeof(FE_MSG));
			ACE_OS::memcpy((unsigned char*)&msgmail,rmb->rd_ptr(),sizeof(FE_MSG));			//copy����
			delete rmb;


			//������յ��ı��Ĳ��ǵ�ǰ��Ҫ����ͨ���ı��ģ���գ�break��
			if(msgmail.channel_no != m_channel_no)				
			{
				ACE_OS::memset((unsigned char*)&msgmail,0,sizeof(FE_MSG));
				continue;
			}


			//��peer������ʧ��break��
			if (m_peer.send(&msgmail,sizeof(FE_MSG)) == -1) 
			{ 
				ACE_ERROR ((LM_ERROR,"%p\n","(%t) Msg_Service send error !"));
				m_peer.close();
				break; 
			}
		}

		Msg_service_acceptor::get()->remove_service(m_channel_no, this);
		m_channel_no = FREE_SERVICE;

		ACE_OS::sleep(ACE_Time_Value(0,1*1000));			//��ת�̣߳�����cpuʹ�ã�����cpuʹ����

	}
	return 0; 

}