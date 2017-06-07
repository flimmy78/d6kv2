/**
@file baseprotocol.cpp
@brief ��Լ�����ʵ��

@author hongxiang 
@version 1.0.0
@date 2013-10-24
*/
#include "fes/baseprotocol.h"
#include "fes/platformwrapper.h"
#include "fes/globalbuf_raw.h"
#include "fes/globalbuf_evevnt.h"
#include "fes/globalbuf_yk_check.h"
#include "fes/fesdatashm_allocator.h"
#include <ace/OS.h>
#include <ace/Log_Msg.h>

using namespace std;

/**************************************************************************
���캯��
**************************************************************************/
Base_protocol::Base_protocol(const int& channel_no)
:m_channel_no(channel_no),m_chanpara(NULL),m_chandata(NULL)
{
	ACE_OS::memset(&m_read_buf, 0, REC_LEN);
	ACE_OS::memset(&m_write_buf, 0, TRN_LEN);

	m_read_lenth = 0;
	m_write_lenth = 0;

	m_chanpara = Fes_shm::get().chan_para_ptr + channel_no;
	m_chandata = Fes_shm::get().chan_data_ptr + channel_no;

	m_chandata->recv_count = m_chandata->send_count = 0;	//����������0
	m_chandata->last_sync_time = 0;
	m_chandata->sync_time_success = 0;
    m_chandata->last_recv_time = ACE_OS::time(NULL);
	m_rtu_no = m_chanpara->child_rtu;		//rtuno

	//�����豸���͵Ĳ�ͬ��������ͬ��ͨѶ�豸
	switch(m_chanpara->dev_type)
	{
	case SERIAL_DEV:
		{
			m_dev_ptr = dynamic_cast<Dev_base*>(new Serial(m_chanpara->dev_name, m_chanpara->backup_dev_name,\
				m_chanpara->band, m_chanpara->check_bit, m_chanpara->stop_bit, m_chanpara->data_bit, \
				m_chanpara->flow));
// 			ACE_DEBUG((LM_DEBUG, "%d��ͨ���豸����Ϊserial!\n", channel_no));
		}
		break;
	case TCP_CLINET_DEV:
		{
			m_dev_ptr = dynamic_cast<Dev_base*>(new Tcp_client(m_chanpara->dev_name, m_chanpara->port, \
				m_chanpara->backup_dev_name, m_chanpara->back_up_port));
// 			ACE_DEBUG((LM_DEBUG, "%d��ͨ���豸����Ϊtcp_client!\n", channel_no));
		}
		break;
	case TCP_SERVER_DEV:
		{
			m_dev_ptr = dynamic_cast<Dev_base*>(new Tcp_server(m_chanpara->local_dev, m_chanpara->local_port, \
				m_chanpara->dev_name, m_chanpara->backup_dev_name));
// 			ACE_DEBUG((LM_DEBUG, "%d��ͨ���豸����Ϊtcp_server!\n", channel_no));
		}
		break;
	case UDP_DEV:
		{
			m_dev_ptr = dynamic_cast<Dev_base*>(new Udp_dev(m_chanpara->local_dev, m_chanpara->local_port, \
				m_chanpara->dev_name, m_chanpara->port, m_chanpara->backup_dev_name, m_chanpara->back_up_port));
// 			ACE_DEBUG((LM_DEBUG, "%d��ͨ���豸����Ϊudp!\n", channel_no));
		}
		break;
	default:
		{
			m_dev_ptr = NULL;
			ACE_ERROR((LM_ERROR, "�豸���ʹ�!\n"));
		}
		break;
	}

 	m_msg_tran_instance = Msg_sender::instance();		//��ȡ����ת��ʵ����ָ��
	m_nEnd = false;
}

/**************************************************************************
��������
**************************************************************************/
Base_protocol::~Base_protocol()
{
	ACE_OS::memset(&m_read_buf, 0, REC_LEN);
	ACE_OS::memset(&m_write_buf, 0, TRN_LEN);
	m_read_lenth = 0;
	m_write_lenth = 0;
}

/**************************************************************************
����
**************************************************************************/
int Base_protocol::open(void *args)
{
//	activate();
	size_t stacksize = 2*1024*1024;
	activate( THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, 1, 0,
		ACE_DEFAULT_THREAD_PRIORITY, -1, 0, 0, 0, &stacksize );
	return 0;
}

/**************************************************************************
ֹͣ
**************************************************************************/
int Base_protocol::close (u_long flags)
{
	if (m_msg_tran_instance)
	{
		m_msg_tran_instance->close_dev();
		m_msg_tran_instance == NULL;
	}
	close_dev();
	m_dev_ptr = NULL;
	return 0;
}

/**************************************************************************
��Լ�����߳����߳���
**************************************************************************/
int Base_protocol::svc(void)
{
	protocol_proc();
	return 0;
}

/**************************************************************************
��ʼ����Լʵ��
**************************************************************************/
void Base_protocol::init_protocol()
{
		
}

/**************************************************************************
��ͨ���˿�
**************************************************************************/
int Base_protocol::open_dev(void)		
{
	int ret = -1;
	if (m_dev_ptr)		//modify by hongxiang
	{
		ret = m_dev_ptr->open_dev();
		if (!ret)	//�˿ڴ򿪳ɹ�������������
		{
			m_chandata->last_connected_time = ACE_OS::time(NULL);
		}
		else
		{
			Proc_manager::putdbg(MAINDBG, "ͨ���� = %d,����·ʧ��",m_channel_no );
		}
	}
	else
	{
		ACE_ERROR((LM_ERROR, "ͨѶ�豸��!\n"));
	}
	return ret;

}

/**************************************************************************
�ر�ͨ���˿�
**************************************************************************/
void Base_protocol::close_dev(void)
{
	if (m_dev_ptr)		//modify by hongxiang
	{
		Proc_manager::putdbg(MAINDBG, "ͨ���� = %d,�ر���·", m_channel_no );
		m_dev_ptr->close_dev();
		if( m_chanpara->dev_type == TCP_SERVER_DEV )
		{
			Tcp_server * pSrv = dynamic_cast<Tcp_server*>(m_dev_ptr);
			pSrv->close_listen();
		}

	}
	else
	{
		ACE_ERROR((LM_ERROR, "ͨѶ�豸��!\n"));
	}
}

/**************************************************************************
ͨ���Ƿ��
**************************************************************************/
bool Base_protocol::is_open(void)
{
	if (!m_dev_ptr)		//modify by hongxiang
	{
		ACE_ERROR((LM_ERROR, "ͨѶ�豸��!\n"));
		return false;
	}

	return m_dev_ptr->get_stat() == 0 ? true : false;
}

/**************************************************************************
�������ݵ����ջ�����
**************************************************************************/
int Base_protocol::recv_msg(void)
{
	int ret = -1;
	//���Ľ���
	if (!m_dev_ptr)		//modify by hongxiang
	{
		ACE_ERROR((LM_ERROR, "ͨѶ�豸��!\n"));
		return ret;
	}
	unsigned char buf[REC_BUF];
	ACE_OS::memset(buf, 0, REC_BUF);

	ret = m_dev_ptr->read_dev(buf, REC_BUF,3);
	if(ret>=0)
	{
		ACE_OS::memcpy(m_read_buf + m_read_lenth, buf, ret);
		m_read_lenth += ret;

		//���ͽ��ձ��ĵ�msgengine
		if (ret)		//�ɹ����պ���ת����msgengine
		{
			FE_MSG fe_msg_send;
			ACE_OS::memset(&fe_msg_send, 0, sizeof(FE_MSG));
			fe_msg_send.channel_no = m_channel_no;
			fe_msg_send.recv_flag = 1;
			fe_msg_send.lenth = ret;
			get_time(&fe_msg_send.time);
			ACE_OS::memcpy(fe_msg_send.msg, buf, ret);
			m_msg_tran_instance->put_msg(fe_msg_send);

			int channel_cuurent_IO = 48 + ret;//(TCPЭ�������ͷ����48)
			m_chandata->recv_count += channel_cuurent_IO;
		}

		//��д���յ����ݵ�ʱ��
		m_chandata->last_recv_time = ACE_OS::time(NULL);
	}
	return ret < 0 ? -1 : ret;
}


/**************************************************************************
�ѷ��ͻ����������ݷ��ͳ�ȥ
**************************************************************************/
int Base_protocol::send_msg(void)
{
	if (!m_dev_ptr)		//modify by hongxiang
	{
		ACE_ERROR((LM_ERROR, "ͨѶ�豸��!\n"));
		return -1;
	}

// 	ACE_DEBUG((LM_DEBUG, "���ͱ���!\n"));

	m_write_lenth = m_dev_ptr->write_dev(m_write_buf,m_write_lenth);		//modify by hongxiang ���ͳ���

 	//�ɹ����ͺ󣬽����ͱ���ת����msgengine
	if (m_write_lenth > 0)
	{
		FE_MSG fe_msg_send;
		ACE_OS::memset(&fe_msg_send, 0, sizeof(FE_MSG));
		fe_msg_send.channel_no = m_channel_no;
		fe_msg_send.recv_flag = 0;
		fe_msg_send.lenth = m_write_lenth;
		get_time(&fe_msg_send.time);
		ACE_OS::memcpy(fe_msg_send.msg, m_write_buf, m_write_lenth);
		m_msg_tran_instance->put_msg(fe_msg_send);

		int channel_cuurent_IO = 48 + m_write_lenth;//(TCPЭ�������ͷ����48)
		m_chandata->send_count += channel_cuurent_IO;
}

	return m_write_lenth > 0 ? 0 : -1;
}

/**************************************************************************
�����ɵĵ����������ȫ�ֻ���
**************************************************************************/
void Base_protocol::buf_event(FE_DATA_EVENT_NODE& event)
{
	event.master_flag = m_chandata->master_flag;
	Global_buf_Event::get().put_event(event);
}

/**************************************************************************
�����ɵĵ��������ݷ���ȫ�ֻ���
**************************************************************************/
void Base_protocol::buf_fe_raw(FE_REAL_DATA_NODE& fe_real_data)
{
	fe_real_data.master_flag = m_chandata->master_flag;
	Global_buf_Raw::get().put_raw(fe_real_data);
}

/**************************************************************************
�����ɵĵ���ң�ط�У���ݷ���ȫ�ֻ���
**************************************************************************/
void Base_protocol::buf_yk_check(FE_YK_CHECK_NODE& fe_yk_check_data)
{
	fe_yk_check_data.master_flag = m_chandata->master_flag;
	Global_buf_yk_check::get().put_yk_check(fe_yk_check_data);
}


/**************************************************************************
����ʱ��
**************************************************************************/
void Base_protocol::set_time(const unsigned short& year, 
							 const unsigned char& month,
							 const unsigned char& day, 
							 const unsigned char& hour, 
							 const unsigned char& minute, 
							 const unsigned char& second, 
							 const unsigned short& milisecond)
{
	//���õ�ʱ������Լ��
	if (check_time_ok(year, month, day, hour, minute, second, milisecond) == 0)
	{
		int ret = Fe_net_control::check_time(year, month, day, hour, minute, second, milisecond);	//fesϵͳ��gps��ʱ
	}
}

/**************************************************************************
�������ʱ���Ƿ����
**************************************************************************/
int Base_protocol::check_time_ok(const unsigned short& year, 
								 const unsigned char& month,
								 const unsigned char& day, 
								 const unsigned char& hour, 
								 const unsigned char& minute, 
								 const unsigned char& second, 
								 const unsigned short& milisecond)
{
	static int  lastgpstime = 0;
	static int  lastsystime = 0;
	static int  errret = -1;

	ACE_DEBUG((LM_DEBUG, "\nSet time: year=%d,month=%d,day=%d,hour=%d,minute=%d,second=%d,milisecond=%d\n",year,month,day,hour,minute,second,milisecond));
	
	//���ʱ���Ƿ����
	if ((year<2000 || year>2060)||(month<1 || month>12)||(day<1 || day>31)||(hour<0 || hour>23)||(minute<0 || minute>59)||(second<0 || second>59)||(milisecond<0 || milisecond>999))
	{
		ACE_ERROR_RETURN((LM_ERROR, "���õ�ʱ�䲻�Ϸ���\n"), -1);
	}
	else
	{
		//ת��ʱ��
		struct tm tmp_time;
		tmp_time.tm_year = year-1900;
		tmp_time.tm_mon = month-1;
		tmp_time.tm_mday = day;
		tmp_time.tm_hour = hour;
		tmp_time.tm_min = minute;
		tmp_time.tm_sec = second;
		tmp_time.tm_isdst = -1;
		int curgpstime = (int)ACE_OS::mktime(&tmp_time);
		int cursystime = (int)ACE_OS::time(NULL);

		//�״ζ�ʱ������¼ʱ��
		if(lastgpstime == 0) 
		{
			lastgpstime = curgpstime;
			lastsystime = cursystime;
			return errret;
		}
		//��ֹGPSʱ�䲻�ȶ�
		if(curgpstime <= lastgpstime)
		{
			lastgpstime = curgpstime;
			lastsystime = cursystime;
			return errret;
		}
		//��ֹʱ������,��ͬһʱ����ڣ�GPS�仯��ʱ����ϵͳ�仯��ʱ���ܴ���20����
		int gpsdestime = curgpstime-lastgpstime;
		int sysdestime = cursystime-lastsystime;
		lastgpstime = curgpstime;
		lastsystime = cursystime;
		if(abs(gpsdestime-sysdestime)>1200)
		{
			lastgpstime = 0;
			lastsystime = 0;
			return errret;
		}
	}
	return 0;
}

/**************************************************************************
����ң�ط����ʼ�
**************************************************************************/
void Base_protocol::send_yk_ret(const int& rtu_no, const int& ykno, const int& yk_ret, const char* msg)
{
// 	Fe_scasrv::fe_send_ykret(rtu_no, ykno, yk_ret, msg);
	char str[NORMALCHAR_LEN];
	FE_YK_CHECK_NODE yk_check_tmp;
	ACE_OS::memset(&yk_check_tmp, 0, sizeof(FE_YK_CHECK_NODE));	//��ʼ��

	yk_check_tmp.rtu_no = rtu_no;
	yk_check_tmp.yk_no = ykno;
	yk_check_tmp.yk_ret = yk_ret;
	strcpy(yk_check_tmp.msg, msg);
	buf_yk_check(yk_check_tmp);
	ACE_OS::sprintf(str,"��scada����ң�ط�У�ʼ�,״̬��%s\n",msg);
}

/**************************************************************************
���ɵ���������뻺��
**************************************************************************/
void Base_protocol::make_event(const FETIME& soe_time, const int& data_no, const unsigned char& yx_value)
{
	FE_DATA_EVENT_NODE soe_event;
	Fes_data_shm_allocator& fes_data_shm_allocator = Fes_data_shm_allocator::get();

	ACE_OS::memset(&soe_event,0,sizeof(FE_DATA_EVENT_NODE));
	soe_event.source = RTU_SOE;
	soe_event.channel = m_channel_no;
	soe_event.rtuno = m_rtu_no;
	soe_event.time = soe_time;
	soe_event.val.yx.value = yx_value ^ (fes_data_shm_allocator.const_yx_negate(m_channel_no,data_no));;
	soe_event.val.yx.order = data_no;
	buf_event(soe_event);
}

/**************************************************************************
����ң��ʵʱ���ݵ㣬�����뻺��
**************************************************************************/
void Base_protocol::make_yc_real_data(const int& data_no, const float& yc_value, const unsigned char& quanity, FETIME* time)
{
	Fes_data_shm_allocator& fes_data_shm_allocator = Fes_data_shm_allocator::get();
	FE_REAL_DATA_NODE buf_data;
	ACE_OS::memset(&buf_data, 0, sizeof(FE_REAL_DATA_NODE));	//��ʼ��

	FE_YC* yc_temp = fes_data_shm_allocator.set_yc_value(m_channel_no,data_no,yc_value,quanity,time);
	
	if (!yc_temp)
	{
		return;
	}
	buf_data.type = FE_REAL_DATA_NODE::YC_REALDATA;
	buf_data.channel_no = m_channel_no;
	buf_data.rtu_no = m_rtu_no;
	buf_data.order = data_no;
	memcpy(&(buf_data.value.yc_value), yc_temp, sizeof(FE_YC));
	buf_fe_raw(buf_data);		//����scadaʵʱ���ݻ�����


}

/**************************************************************************
����ң��ʵʱ���ݵ㣬�����뻺��
**************************************************************************/
void Base_protocol::make_yx_real_data(const int& data_no, const unsigned char& yx_value, const unsigned char& quanity, FETIME* time)
{
	Fes_data_shm_allocator& fes_data_shm_allocator = Fes_data_shm_allocator::get();
	FE_REAL_DATA_NODE buf_data;
	ACE_OS::memset(&buf_data, 0, sizeof(FE_REAL_DATA_NODE));	//��ʼ��

	FE_YX* yx_old =  fes_data_shm_allocator.channel_yx(m_channel_no) + data_no;		//ȡ��ԭʼֵ��Ϊ��λ�ж�����
	unsigned char yx_value_old = yx_old->processed_yx;

	FE_YX* yx_temp = fes_data_shm_allocator.set_yx_value(m_channel_no,data_no,yx_value,quanity,time);
	if (!yx_temp)
	{
		return;
	}
	Proc_manager::putdbg(MAINDBG, "yx_temp->processed_yx  = %d,ͨ����=%d,ң�ź�=%d",yx_temp->processed_yx, m_channel_no,data_no );
	//����λΪ0 �Ƿ����ݲ�����scada
	if (!quanity)
	{
		return;
	}
	buf_data.type = FE_REAL_DATA_NODE::YX_REALDATA;
	buf_data.channel_no = m_channel_no;
	buf_data.rtu_no = m_rtu_no;
	buf_data.order = data_no;
	memcpy(&(buf_data.value.yx_value), yx_temp, sizeof(FE_YX));

	buf_fe_raw(buf_data);		//����scadaʵʱ���ݻ�����

	if (yx_value_old != yx_temp->processed_yx && yx_value_old != UCHAR_DEFAULT)		//���������λ���״μӵ�ʱ���������
	{
		Proc_manager::putdbg(MAINDBG, "yx_temp->processed_yx  = %d,ͨ����=%d,ң�ź�=%d",yx_temp->processed_yx, m_channel_no,data_no );

		FE_DATA_EVENT_NODE soe_event;
		ACE_OS::memset(&soe_event,0,sizeof(FE_DATA_EVENT_NODE));
		soe_event.source = HOST_SOE;
		soe_event.channel = m_channel_no;
		soe_event.rtuno = m_rtu_no;
		soe_event.time = yx_temp->modifier.time;
		soe_event.val.yx.value = yx_temp->processed_yx;
		soe_event.val.yx.order = data_no;
		buf_event(soe_event);
	}

}

/**************************************************************************
����ң��ʵʱ���ݵ㣬�����뻺��
**************************************************************************/
void Base_protocol::make_kwh_real_data(const int& data_no, const unsigned long& kwh_value, const unsigned char& quanity, FETIME* time)
{
	Fes_data_shm_allocator& fes_data_shm_allocator = Fes_data_shm_allocator::get();
	FE_REAL_DATA_NODE buf_data;
	ACE_OS::memset(&buf_data, 0, sizeof(FE_REAL_DATA_NODE));	//��ʼ��

	FE_KWH* kwh_temp = fes_data_shm_allocator.set_kwh_value(m_channel_no,data_no,kwh_value,quanity,time);
	if (!kwh_temp)
	{
		return;
	}
	buf_data.type = FE_REAL_DATA_NODE::KWH_REALDATA;
	buf_data.channel_no = m_channel_no;
	buf_data.rtu_no = m_rtu_no;
	buf_data.order = data_no;
	memcpy(&(buf_data.value.kwh_value), kwh_temp, sizeof(FE_KWH));

	buf_fe_raw(buf_data);		//����scadaʵʱ���ݻ�����

}

/**************************************************************************
�ʼ�������̣�
**************************************************************************/
#pragma  pack(1)
void Base_protocol::check_event(void)
{
	//�����ա��ʼ�����ʱ�����أ������¼�
	ACE_Message_Block * msg_block =0;
	ACE_Time_Value timeout(1,0);			//��ʱʱ��1S
	msg_block = 0;
	if(getq( msg_block, &timeout )==-1)		//��ʱ��������ֱ��continue
	{
		// 		ACE_DEBUG((LM_DEBUG, "���ʼ�!\n"));
		return;
	}
	FE_MAIL_STRUCT* recv_mail = (FE_MAIL_STRUCT*)msg_block->rd_ptr();
	// 	ACE_OS::memset(&recv_mail,0,sizeof(FE_MSG));
	// 	ACE_OS::memcpy(&recv_mail,msg_block->rd_ptr(),sizeof(FE_MAIL_STRUCT));			//copy����
	switch(recv_mail->mail_type)
	{
	case CALLING:			//�ٻ��ʼ�
		{
			FE_CALLING_MAIL* yk_mail = (FE_CALLING_MAIL*)recv_mail->mail_body;
			switch(yk_mail->call_type)
			{
			case FE_CALLING_MAIL::CALL_ALL:
				m_call_all_data_flag = NEED_CALL;
				break;
			case FE_CALLING_MAIL::RTU_INIT:
				m_rtu_init_flag = NEED_CALL;
				break;
			case FE_CALLING_MAIL::CHANNEL_INIT:		//��Լ���޷���������
				break;
			case FE_CALLING_MAIL::CHECK_TIME:
				m_sync_time_flag = NEED_CALL;
				break;
			case FE_CALLING_MAIL::CALL_KWH:
				m_call_kwh_data_flag = NEED_CALL;
				break;
			case FE_CALLING_MAIL::CHECK_PUBLIC_KEY:
				m_check_public_key = NEED_CALL;
				break;
			case FE_CALLING_MAIL::REFRESH_PUBLIC_KEY:
				m_refresh_public_key = NEED_CALL;
				break;
			default:
				break;
			}
		}
		break;
	case YK:				//ң���ʼ�
		{
			FE_YK_MAIL* yk_mail = (FE_YK_MAIL*)recv_mail->mail_body;
			send_yk_frame(yk_mail->yk_no, yk_mail->action, yk_mail->yk_flag);			
		}
		break;
	case SET_POINT:
		{
			FE_SETPOINT_MAIL* set_mail = (FE_SETPOINT_MAIL*)recv_mail->mail_body;
			send_yt_frame(set_mail->yt_no, set_mail->setvalue, set_mail->yt_flag);			
		}
		break;
	case PROT:
		{
			FE_PROT_MAIL* prot_mail = (FE_PROT_MAIL*)recv_mail->mail_body;
			send_command_frame(prot_mail->prot_type,prot_mail->exe_flag,prot_mail->cmd_lenth,prot_mail->prot_buf);
		}
		break;
	case WAVE:
		{
			//¼���ʼ�Ҳʹ�ñ����ʼ���ʽ
			FE_PROT_MAIL* prot_mail = (FE_PROT_MAIL*)recv_mail->mail_body;
			send_WaveCommand(prot_mail->exe_flag,prot_mail->prot_buf);
		}
		break;
	case SEND_YX:
		{
			FE_TRANS_YX_MAIL* trans_yx_mail = (FE_TRANS_YX_MAIL*)recv_mail->mail_body;
			send_trans_yx( trans_yx_mail->yx_no , trans_yx_mail->yx_value ) ;
		}
		break;
	default:
		break;
	}
	delete msg_block;
}
#pragma pack()

void Base_protocol::exit_proc()
{
	m_nEnd = true;
	if (0 == wait())
	{
		delete this;
	}

}