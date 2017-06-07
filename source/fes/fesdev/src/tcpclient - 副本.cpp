#include "fes/fesdev.h"

#if defined (__unix)
//#include "linux/tcp.h" 
#include "netinet/tcp.h"
#endif


Tcp_client::Tcp_client(const char* dev1, const int& port1, const char* dev2, const int& port2)
:Net_dev(dev1, port1, dev2, port2)
{

}


int Tcp_client::open_dev(void)
{
	if (0 == valid_addr_num)
	{
		printf("there are no valid address.\n");
		return -1;
	}

	addr_index = (addr_index + 1) % valid_addr_num;

	if(-1 == peer.open(SOCK_STREAM,AF_INET,IPPROTO_TCP,0))
	{
		ACE_ERROR((LM_ERROR,"%p\n","Error in open"));
		if(-1 == peer.close())
		{
			ACE_ERROR((LM_ERROR,"%p\n","Error in close"));
			return -1;
		}
		return -1;

	}
 	ACE_DEBUG((LM_INFO,ACE_TEXT("Tcp_client �����׽��ֳɹ���\n")));


	ACE_SOCK_CONNECTOR client_Connector;

// 	ACE_Time_Value time_out = ACE_Time_Value::zero;		//add by chenkai connect����Ϊ����ģʽ�����޷����ӵȴ�һ��ʱ��᷵��-1

	if(-1 == client_Connector.connect(peer, addr[addr_index]/*, &time_out*/))
	{
		ACE_ERROR((LM_ERROR,"%p\n","Tcp_client Error in Connect" ));

		return -1;
	}

	set_fd_opt();

// 	ACE_DEBUG((LM_INFO,ACE_TEXT("���ӳɹ���\n")));
	return 0;

}


void Tcp_client::set_fd_opt() const
{
	struct linger l;
	l.l_onoff = 0;
	l.l_linger = 0;
	peer.set_option(SOL_SOCKET, SO_LINGER, (void*)&l, sizeof(linger));

	//����Nagle�㷨
	int on = 1;
	peer.set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(on));

	//���÷��ͽ��ջ�����
	int temp = DEVICE_TX_BUF_SIZE;
	peer.set_option(SOL_SOCKET, SO_RCVBUF, (char *)&temp, sizeof(temp));
	peer.set_option(SOL_SOCKET, SO_SNDBUF, (char *)&temp, sizeof(temp));

	//����Ϊ������ģʽ
	peer.enable(ACE_NONBLOCK);

}
/*

int Tcp_client::read_dev(unsigned char* buf, int len)
{
	int recv_count = peer.recv(buf, len);

	if(recv_count == -1)
	{
		if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
		{
			recv_count = 0;
		}
		else
		{
// 			ACE_ERROR((LM_ERROR, "%p\n", "Error in Recv"));
		}
	}
	else if (recv_count == 0 && len != 0)
	{
		recv_count = -1;		//�Ͽ����ӵķ���ֵҲӦΪ-1,�����˿�ʧ��
		ACE_DEBUG((LM_INFO, ACE_TEXT("�Է��Ѿ��Ͽ����ӣ�\n")));
	}
	else
	{
// 	    ACE_DEBUG((LM_INFO, ACE_TEXT("�ɹ�����%d���ֽڣ�\n"), recv_count));
	}
	return recv_count;
}
*/

int Tcp_client::read_dev(unsigned char* buf, int len,unsigned int nTimeOut)
{
	ACE_Time_Value nVal(nTimeOut);

	int recv_count = peer.recv(buf, len,&nVal);

	if(recv_count == -1)
	{
		if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
		{
			recv_count = 0;
		}
		else
		{
			// 			ACE_ERROR((LM_ERROR, "%p\n", "Error in Recv"));
		}
	}
	else if (recv_count == 0 && len != 0)
	{
		recv_count = -1;		//�Ͽ����ӵķ���ֵҲӦΪ-1,�����˿�ʧ��
		ACE_DEBUG((LM_INFO, ACE_TEXT("�Է��Ѿ��Ͽ����ӣ�\n")));
	}
	else
	{
		// 	    ACE_DEBUG((LM_INFO, ACE_TEXT("�ɹ�����%d���ֽڣ�\n"), recv_count));
	}
	return recv_count;
}

int Tcp_client::write_dev(unsigned char* buf, int len)
{
	int send_count = 0;
	if((send_count = peer.send(buf, len)) == -1)
	{
// 		ACE_ERROR((LM_ERROR, "%p\n", "Error in Send"));
		return -1;
	}

// 	ACE_DEBUG((LM_INFO, ACE_TEXT("�ɹ�����%d���ֽڣ�\n"), send_count));
	return send_count;
}

void Tcp_client::close_dev(void)
{
	if(-1 == peer.close())
	{
// 		ACE_ERROR((LM_ERROR, "%p\n", "Error in close"));
		return;
	}
// 	ACE_DEBUG((LM_INFO,ACE_TEXT("�ر��׽��ֳɹ���\n")));

}


Tcp_client::~Tcp_client()
{
	close_dev();
}

int Tcp_client::get_stat(void)
{
	return peer.get_handle() >= 0 ? 0 : -1;
}



