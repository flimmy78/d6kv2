/**
@file iec101b.cpp
@brief 101b��Լ����Դ�ļ�

@author libinghong 
@version 1.0.0
@date 2016-08-12
*/

#include "iec101b.h"

#include "fes/timer.h"
#include "fes/fesshm.h"
#include "fes/platformwrapper.h"
#include "fes/fesdatashm_allocator.h"
#include "fes/femailstruct.h"

#include <ace/OS.h>
#include <ace/Log_Msg.h>
#include <ace/Message_Block.h>
#include <ace/Time_Value.h>
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;

const char* const version_string = "1.0.0";
const char* const desc_string = "the very beginning 101";

//���Ӳ��ɹ��󣬵ȴ�2���ٳ�������
const int CONN_RETRY_TIME = 2;
const int SECOND_LOOP_TIME = 1;

/**
@brief IEC101������
*/
static IEC101B_CONFIG iec101b_config[MAX_CHAN_NUM];

Base_protocol* get_protocol(const int& channel)
{
	return new Iec101b(channel);
}

void get_config(void)
{
	//todo
	//�����ݿ��ȡCDT������Ϣ��д��CDT_config

}

char* get_version(void)
{
	return const_cast<char*>(version_string);
}

char* get_desc(void)
{
	return const_cast<char*>(desc_string);
}

FETIME T101Time_To_Realtime(CP24Time2a& SoeTime);	//CP24time����Ϊʵ��ʱ��

Iec101b::Iec101b(const int& channel)
:Base_protocol(channel), m_config_ptr(iec101b_config + channel)
{
	m_common_addr = m_chanpara->common_addr;	//������ַ
}

Iec101b::~Iec101b()
{

}

void Iec101b::init_protocol(void)
{
// 	ACE_DEBUG((LM_DEBUG, "iec101 init\n"));
	m_mainstation_FCB = 0 ;
	m_resend_count = 0 ;
	m_resend_limit = 3 ;

	m_t0_timer.restart();
	m_heart_timer.restart();

	m_call_all_data_flag = NO_NEED;	
	m_call_kwh_data_flag = NO_NEED;	
	m_sync_time_flag = NO_NEED;
	m_rtu_init_flag = NO_NEED;
	m_request_link_flag = true ;
	m_reset_link_flag = false ;
	m_resend_flag = false ;
	m_subrequest_link_flag = false;
	m_subreset_link_flag = false;
	m_sublink_flag = false;
	m_heart_flag = false;

	m_confirm_link_flag = false;
	m_call_delaytime_flag = false ;
	m_send_delaytime_flag = false ;
	m_t1 = 0 ;
	m_t3 = 0 ;
	m_t4 = 0 ;
	m_link_flag = REQUESTLINKSTATUS;

	m_call_all_data_timer.restart();	
	m_call_kwh_timer.restart();		
	m_sync_time_timer.restart();

	m_current_ykno = -1;
	m_current_ykoder = 0;

	m_yx_all_num = Fes_shm::get().chan_para_ptr->yx_num;
	m_yc_all_num = Fes_shm::get().chan_para_ptr->yc_num;
	m_pi_all_num = Fes_shm::get().chan_para_ptr->kwh_num;

	ACE_OS::memset(&m_read_buf, 0, REC_LEN);
	ACE_OS::memset(&m_write_buf, 0, TRN_LEN);

}

void Iec101b::protocol_proc(void)
{
	short curr_host_no = Fes_shm::get().fe_sys_data_ptr->host_no;
	while (!m_nEnd)
	{
		short curr_attend_host = m_chandata->curr_attend_host;
		if (curr_attend_host != curr_host_no || curr_attend_host == -1)	//�Ǳ�ǰ�ýӹ�ͨ��,ֻ�������ѭ��
		{
			ACE_OS::sleep(ACE_Time_Value(0,10*1000));
			continue;
		}

		if (m_dev_ptr)
		{
			m_dev_ptr->close_dev();
		}
		if (open_dev() < 0 )		//����˿�δ��
		{
			ACE_OS::sleep(CONN_RETRY_TIME);
			ACE_DEBUG((LM_DEBUG, "��·��ʧ��!\n"));
			continue;
		}
		else		//�µ�����,����101b��Լ����
		{
			init_protocol();
		}

		ACE_OS::sleep(CONN_RETRY_TIME); //��������TCP���ӽ�������֮����ȴ�2s���������Ĵ���
		//����ѭ������
		while (!m_nEnd)		//��������Ϊ��ͨ��ֵ���Ϊ��ǰǰ��
		{

			int write_ret = 0;
			int read_ret = 0;
			int proc_ret = 0;

			//����ʼ��������ʼ�������ش���
			check_event();

			if (m_sublink_flag == false && m_resend_flag == false)
			{
				write_ret = Iec101b_send_proc();
				if (-1 == write_ret)
				{
					ACE_DEBUG((LM_DEBUG, "д�˿�ʧ��\n"));
					break;
				}
			}

			ACE_OS::sleep(ACE_Time_Value(0,100*1000));
			read_ret  = recv_msg();	

			//���ձ��ģ����жϸ��ֳ�ʱ
			if (-1 == read_ret)			//���˿�ʧ�ܻᵼ�±���ѭ���˳�
			{
				ACE_DEBUG((LM_DEBUG, "���˿�ʧ��\n"));
				break;
			}
			else if (0 == read_ret)		//δ���յ����ݣ��жϳ�ʱ
			{
				if (m_t0_timer.elapsed() > m_config_ptr->t0 )
				{
					m_resend_count++ ;
					m_resend_flag = true ;
					send_msg();
					m_t0_timer.restart();
					m_heart_timer.stop();

					if ( m_resend_count > m_resend_limit )
					{
						ACE_DEBUG((LM_DEBUG, "t0��ʱ\n"));
						m_link_flag = REQUESTLINKSTATUS ;
						close_dev();
						break;
					}
				}
				if (m_heart_timer.elapsed() > 60 ) //1min��һ������
				{
					m_heart_flag = true ;
				}
			}
			else		//��������������t0��ʱ��
			{
				m_resend_count = 0 ;
				m_resend_flag = false ;
				m_t0_timer.stop();
//				m_heart_timer.restart();
			}

			//�������١��ٵ�ȼ���ʱ��ʱ��
			if (m_config_ptr->call_all_data_gap != -1 && m_call_all_data_timer.elapsed() >= m_config_ptr->call_all_data_gap)
			{
				m_call_all_data_flag = NEED_CALL;
				m_call_all_data_timer.restart();
			}

			if (m_config_ptr->call_kwh_gap != -1 && m_call_kwh_timer.elapsed() >= m_config_ptr->call_kwh_gap)
			{
				m_call_kwh_data_flag = NEED_CALL;
				m_call_kwh_timer.restart();
			}

			if (m_config_ptr->send_time_gap != -1 && m_sync_time_timer.elapsed() >= m_config_ptr->send_time_gap)
			{
				m_sync_time_flag = NEED_CALL;
				m_sync_time_timer.restart();
			}

			//����ȷ�ϼ��ظ�
			if(m_read_lenth > 0)	//���ջ������б����ٽ��д���
			{
				//��������
				
					int frame_type = Iec101b_receive_proc() ;
					switch ( frame_type )
					{
					case FIXED_FRAME:
						{
							proc_ret = deal_fixed_frame();
							break;
						}
					case UNFIXED_FRAME:
						{
							proc_ret = deal_unfixed_frame();
							break;
						}
					case YKCONFIRM_FRAME:
						{
							proc_ret = DEAL_SUCCESS ;
							break;
						}
					case INVALID_FRAME:
						{
							proc_ret = deal_error_frame();
							break;
						}
					default:
						break;
				    }
				//
				if ( DEAL_SUCCESS != proc_ret)  //��������ѭ�� 
				{
					break;
				}
			}
			if (m_chandata->curr_attend_host != curr_host_no)		//�жϹ黹����
			{
				close_dev();
				if( m_chanpara->dev_type == TCP_SERVER_DEV )
				{
					Tcp_server * pSrv = dynamic_cast<Tcp_server*>(m_dev_ptr);
					pSrv->close_listen();
				}

				break;
			}
			ACE_OS::sleep(ACE_Time_Value(0,100*1000));

		}//while (true) ����ѭ��
	}//while (true)  һ��ѭ��
}

int Iec101b::Iec101b_send_proc(void)
{
	int ret = 0xffff ;
	if ( m_request_link_flag == true )
	{
		m_write_lenth = 0;  //���ͳ�������
		ret = request_link_status() ;
		m_request_link_flag = false ;
		m_resend_flag = false ;
		m_t0_timer.restart();
		m_heart_timer.stop();
	}
	else if ( m_reset_link_flag == true )
	{
		m_write_lenth = 0;  //���ͳ�������
		ret = reset_link() ;
		m_reset_link_flag = false ;
		m_resend_flag = false ;
		m_t0_timer.restart();
		m_heart_timer.stop();
	}
	else if ( m_subrequest_link_flag == true )
	{
		m_write_lenth = 0;  //���ͳ�������
		ret = response_link_status() ;
		m_subrequest_link_flag = false ;
		m_resend_flag = false ;
		m_t0_timer.restart();
		m_heart_timer.stop();
	}
	else if ( m_subreset_link_flag == true )
	{
		m_write_lenth = 0;  //���ͳ�������
		ret = confirm_reset_link() ;
		m_subreset_link_flag = false ;
		m_resend_flag = false ;
		m_t0_timer.restart();
		m_heart_timer.stop();
	}
	else if ( m_confirm_link_flag == true )
	{
		m_write_lenth = 0;  //���ͳ�������
		ret = confirm_link() ;
		m_confirm_link_flag = false ;
		m_resend_flag = false ;
		m_t0_timer.stop();
		m_heart_timer.restart();//ȷ��֡�����жϻظ�
	}
	else if (m_call_all_data_flag == NEED_CALL)		//�ʼ��оݵı��ķ���
	{
		m_write_lenth = 0;  //���ͳ�������
		ret = call_all_data();
		m_link_flag = CALLTOTAL;
		m_resend_flag = false ;
		m_t0_timer.restart();
		m_heart_timer.stop();
	}
	else if (m_sync_time_flag == NEED_CALL && m_call_all_data_flag != ON_PROCESS)//���ٹ����в����ж�ʱ
	{
		m_write_lenth = 0;  //���ͳ�������
		ret = sync_time();
		m_resend_flag = false ;
		m_t0_timer.restart();
		m_heart_timer.stop();
	}
/*	else if ( m_link_flag != REQUESTLINKSTATUS && m_link_flag != RESET && m_call_all_data_flag == NO_NEED && m_sync_time_flag == NO_NEED )
	{
        m_link_flag = -1;
		m_hearttext_cnt++;
	}
	
	if( m_link_flag ==-1)
	{
		if (m_hearttext_cnt%75 == 0)
		{
			m_write_lenth = 0;  //���ͳ�������
			ret = test_link() ;
			m_resend_flag = false ;
		}
		else
		{
			ret = 0;
		}
	}*/
	else if ( m_heart_flag )
	{
		m_write_lenth = 0;  //���ͳ�������
		ret = test_link() ;
		m_resend_flag = false ;
		m_heart_flag = false ;
		m_t0_timer.restart();
		m_heart_timer.stop();
	}
	else if( m_heart_timer.elapsed() <= 0 )
	{
		m_heart_timer.restart();
	}

	return ret;
}

int Iec101b::reset_link( void )
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = FIXED_FRAME_HEAD ;

	ControlField SFC;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 0;
	SFC.FCB = 0 ;
	SFC.FC  = RESET;

	//m_mainstation_FCB = 1 ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end	
	m_write_buf[ 1 ] = tmpFC ;
	m_write_buf[ 2 ] = m_common_addr ;
	CRC_test( m_write_buf , 4 , CRC_WRITE ) ;
	m_write_buf[ 5 ] = FRAME_TAIL ;

	m_write_lenth = FIXED_FRAME_LEN;

	return send_msg();
}

int Iec101b::confirm_reset_link( void )
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = FIXED_FRAME_HEAD ;

	ControlField SFC;
	SFC.DIR = 0;
	SFC.PRM = 0;
	SFC.FCV = 0;
	SFC.FCB = 0 ;
	SFC.FC  = ACK;

	//m_mainstation_FCB = 1 ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end	
	m_write_buf[ 1 ] = tmpFC ;
	m_write_buf[ 2 ] = m_common_addr ;
	CRC_test( m_write_buf , 4 , CRC_WRITE ) ;
	m_write_buf[ 5 ] = FRAME_TAIL ;

	m_write_lenth = FIXED_FRAME_LEN;

	return send_msg();
}

int Iec101b::confirm_link( void )
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = FIXED_FRAME_HEAD ;

	ControlField SFC;
	SFC.DIR = 0;
	SFC.PRM = 0;
	SFC.FCV = 0;
	SFC.FCB = 0 ;
	SFC.FC  = ACK;


	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end	
	m_write_buf[ 1 ] = tmpFC ;
	m_write_buf[ 2 ] = m_common_addr ;
	CRC_test( m_write_buf , 4 , CRC_WRITE ) ;
	m_write_buf[ 5 ] = FRAME_TAIL ;

	m_write_lenth = FIXED_FRAME_LEN;

	return send_msg();
}


int Iec101b::test_link( void )
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = FIXED_FRAME_HEAD ;

	ControlField SFC;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 0;
	SFC.FCB = 0 ;
	SFC.FC  = TESTLINK;


	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end	
	m_write_buf[ 1 ] = tmpFC ;
	m_write_buf[ 2 ] = m_common_addr ;
	CRC_test( m_write_buf , 4 , CRC_WRITE ) ;
	m_write_buf[ 5 ] = FRAME_TAIL ;

	m_write_lenth = FIXED_FRAME_LEN;

	return send_msg();
}

int Iec101b::request_link_status( void )
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = FIXED_FRAME_HEAD ;
	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 0;
	SFC.FCB = 0 ;
	SFC.FC = REQUESTLINKSTATUS;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end
	m_write_buf[ 1 ] = tmpFC ;
	m_write_buf[ 2 ] = m_common_addr ;
	CRC_test( m_write_buf , 4 , CRC_WRITE ) ;
	m_write_buf[ 5 ] = FRAME_TAIL ;

	m_write_lenth = FIXED_FRAME_LEN;

	return send_msg();
}

int Iec101b::response_link_status( void )
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = FIXED_FRAME_HEAD ;
	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 0;
	SFC.FCV = 0;
	SFC.FCB = 0 ;
	SFC.FC = LINKREDAY;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end
	m_write_buf[ 1 ] = tmpFC ;
	m_write_buf[ 2 ] = m_common_addr ;
	CRC_test( m_write_buf , 4 , CRC_WRITE ) ;
	m_write_buf[ 5 ] = FRAME_TAIL ;

	m_write_lenth = FIXED_FRAME_LEN;

	return send_msg();
}

int Iec101b::call_all_data( void )
{
//	ACE_OS::sleep(1); //����������Ҫ��֡����
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x0B;
	m_write_buf[ 2 ] = 0x0B;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ] = tmpFC;
	m_write_buf[ 5 ] = m_common_addr % 256 ;
	m_write_buf[ 6 ] = m_common_addr / 256 ;
	m_write_buf[ 7 ] = C_IC_NA_1; //���ͱ�ʶ
	m_write_buf[ 8 ] = 0x01; //�ɱ�ṹ�޶���
	m_write_buf[ 9 ] = COT_ACT; //����ԭ��
	m_write_buf[ 10 ] = m_common_addr % 256 ;
	m_write_buf[ 11 ] = m_common_addr / 256 ;
	m_write_buf[ 12 ] = 0x00;	//��Ϣ���ַ
	m_write_buf[ 13 ] = 0x00;
	m_write_buf[ 14 ] = CALLTOTAL ;	

	CRC_test( m_write_buf+ 3  , 12 , CRC_WRITE ) ;
	m_write_buf[ 16 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 2 ] + UNFIXED_DATA_LEN ;

	m_call_all_data_flag = ON_PROCESS;	

	return send_msg();
}

int Iec101b::call_delay_time( void )
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x0C;
	m_write_buf[ 2 ] = 0x0C;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ]  = tmpFC ;
	m_write_buf[ 5 ]  = m_common_addr %256;
	m_write_buf[ 6 ]  = m_common_addr /256;
	m_write_buf[ 7 ]  = C_CD_NA_1; //���ͱ�ʶ
	m_write_buf[ 8 ]  = 0x01; //�ɱ�ṹ�޶���
	m_write_buf[ 9 ]  = COT_ACT; //����ԭ��
	m_write_buf[ 10 ]  = m_common_addr %256;
	m_write_buf[ 11 ]  = m_common_addr /256;
	m_write_buf[ 12 ]  = 0x00 ; //��Ϣ���ַ
	m_write_buf[ 13 ]  = 0x00 ;

	FETIME tim;
	get_time(&tim);

	m_t1 = tim.milisecond + tim.second*1000;
	m_write_buf[ 14 ]  = m_t1 % 256 ;
	m_write_buf[ 15 ]  = m_t1 / 256 ;

	CRC_test( m_write_buf + 3 , 13 , CRC_WRITE ) ;
	m_write_buf[ 17 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 1 ] + UNFIXED_DATA_LEN;

	m_call_delaytime_flag = true;

	return send_msg();
}

int Iec101b::send_delay_time( void )
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x0C;
	m_write_buf[ 2 ] = 0x0C;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ]  = tmpFC ;
	m_write_buf[ 5 ]  = m_common_addr %256;
	m_write_buf[ 6 ]  = m_common_addr /256;
	m_write_buf[ 7 ]  = C_CD_NA_1; //���ͱ�ʶ
	m_write_buf[ 8 ]  = 0x01; //�ɱ�ṹ�޶���
	m_write_buf[ 9 ]  = COT_SPONT; //����ԭ��
	m_write_buf[ 10 ]  = m_common_addr %256;
	m_write_buf[ 11 ]  = m_common_addr /256;
	m_write_buf[ 12 ]  = 0x00 ; //��Ϣ���ַ
	m_write_buf[ 13 ]  = 0x00 ;

	FETIME tim;
	get_time(&tim);

	m_t4 = tim.milisecond + tim.second*1000;
	unsigned int delay_time  = ( m_t4 - m_t3 ) / 2 ;
	m_write_buf[ 14 ]  = m_t4 % 256 ;
	m_write_buf[ 15 ]  = m_t4 / 256 ;

	CRC_test( m_write_buf + 3 , 13 , CRC_WRITE ) ;
	m_write_buf[ 17 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 1 ] + UNFIXED_DATA_LEN;

	m_call_delaytime_flag = true;

	return send_msg();
}

int Iec101b::sync_time()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x11;
	m_write_buf[ 2 ] = 0x11;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ]  = tmpFC ;
	m_write_buf[ 5 ]  = m_common_addr %256;
	m_write_buf[ 6 ]  = m_common_addr /256;
	m_write_buf[ 7 ]  = C_CS_NA_1; //���ͱ�ʶ
	m_write_buf[ 8 ]  = 0x01; //�ɱ�ṹ�޶���
	m_write_buf[ 9 ]  = COT_ACT; //����ԭ��
	m_write_buf[ 10 ]  = m_common_addr %256;
	m_write_buf[ 11 ]  = m_common_addr /256;
	m_write_buf[ 12 ]  = 0x00 ; //��Ϣ���ַ
	m_write_buf[ 13 ]  = 0x00 ;

	SYNC_TIME_STRUCT* pt = (SYNC_TIME_STRUCT*)( m_write_buf + 14 );

	FETIME tim;
	get_time(&tim);

	pt->cp56time.minute = tim.minute;
	pt->cp56time.hour = tim.hour;
	pt->cp56time.day = tim.day;
	pt->cp56time.month = tim.month;
	pt->cp56time.year = (((tim.year + 1900)-2000)&0xff);
	pt->cp56time.millsecond = tim.milisecond + tim.second*1000;

	CRC_test( m_write_buf + 3 , 18 , CRC_WRITE ) ;
	m_write_buf[ 22 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 1 ] + UNFIXED_DATA_LEN;

	m_sync_time_flag = ON_PROCESS;

	m_chandata->last_sync_time = ACE_OS::time(NULL);
	m_chandata->sync_time_success = 0;

	return send_msg();

}

int Iec101b::call_kwh_data()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x09;
	m_write_buf[ 2 ] = 0x09;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ]  = tmpFC ;
	m_write_buf[ 5 ]  = m_common_addr ;
	m_write_buf[ 7 ]  = C_CI_NA_1; //���ͱ�ʶ,101���������ٻ�
	m_write_buf[ 8 ]  = 0x01; //�ɱ�ṹ�޶���
	m_write_buf[ 9 ]  = COT_ACT; //����ԭ��
	m_write_buf[ 11 ]  = m_common_addr ;
	m_write_buf[ 12 ]  = 0x00 ; //��Ϣ���ַ
	m_write_buf[ 13 ]  = 0x00 ;

	m_write_buf[ 14 ] = CALLKWHTOTAL ;

	CRC_test( m_write_buf + 3 , 12 , CRC_WRITE ) ;
	m_write_buf[ 16 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 1 ] + UNFIXED_DATA_LEN;

	m_call_kwh_data_flag = ON_PROCESS;

	return send_msg();
}

int Iec101b::send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x0B;
	m_write_buf[ 2 ] = 0x0B;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ]  = tmpFC ;
	m_write_buf[ 5 ]  = m_common_addr %256;
	m_write_buf[ 6 ]  = m_common_addr /256;
	m_write_buf[ 7 ]  = m_config_ptr->yk_type; //���ͱ�ʶ,101���������ٻ�
	m_write_buf[ 8 ]  = 0x01; //�ɱ�ṹ�޶���
	m_write_buf[ 9 ]  = (yk_flag == 2) ? COT_DEACT : COT_ACT;		//�Ƿ�ȡ��
	m_write_buf[ 10 ]  = m_common_addr %256;
	m_write_buf[ 11 ]  = m_common_addr /256;
	m_write_buf[ 12 ] = ( m_config_ptr->yk_begin + ykno ) % 256 ;
	m_write_buf[ 13 ] = ( m_config_ptr->yk_begin + ykno ) / 256 ;

	//YKDCO �غϿط�,�ɵ����˫��ң��ȷ��
	YKDCO ykdco ;
	if ( yk_flag == 1 )
	{
		ykdco.dco_se = 0 ;
	}
	else
	{
		ykdco.dco_se = 1 ;
	}
	if (m_config_ptr->yk_type == C_SC_NA_1)		//����ң��
	{
		if(bclosed)
		{
			ykdco.dco_dcs = STATE_SP_ON ;
		}	
		else
		{
			ykdco.dco_dcs = STATE_SP_OFF ;
		}
	}
	else if (m_config_ptr->yk_type == C_DC_NA_1)	//˫��ң��
	{
		if(bclosed)
		{
			ykdco.dco_dcs = STATE_DP_ON ;
		}	
		else
		{
			ykdco.dco_dcs = STATE_DP_OFF ;
		}
	}
	ykdco.dco_qu = 0 ;

	//modify for IBM bit begin
	unsigned char tmpbuf; 
	unsigned char tmpbuf1; // 0001 1111
	unsigned char tmpbuf2; 

	tmpbuf = (ykdco.dco_se<<7);
	tmpbuf1 = (ykdco.dco_qu)&0x1f;
	tmpbuf1 = tmpbuf1<<2;
	tmpbuf2 = ( ykdco.dco_dcs )&0x03;

	m_write_buf[ 14 ] = tmpbuf|tmpbuf1|tmpbuf2;

	m_current_ykno = ykno;				//�洢��ǰ�·�ң����ţ��Ա�У��
	m_current_ykoder = ykdco.dco_dcs;			//�洢��ǰ�·�ң������Ա�У��

	CRC_test( m_write_buf + 3 , 12 , CRC_WRITE ) ;
	m_write_buf[ 16 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 1 ] + UNFIXED_DATA_LEN;

	return send_msg();
}

int Iec101b::send_yt_frame(unsigned char ytno, float set_value, unsigned char yt_flag)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x0B;
	m_write_buf[ 2 ] = 0x0B;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ]  = tmpFC ;
	m_write_buf[ 5 ]  = m_common_addr %256;
	m_write_buf[ 6 ]  = m_common_addr /256;
	m_write_buf[ 7 ]  = C_RC_NA_1; //���ͱ�ʶ,101���������ٻ�
	m_write_buf[ 8 ]  = 0x01; //�ɱ�ṹ�޶���
	m_write_buf[ 9 ]  = (yt_flag == 2) ? COT_DEACT : COT_ACT;		//�Ƿ�ȡ��
	m_write_buf[ 10 ]  = m_common_addr %256;
	m_write_buf[ 10 ]  = m_common_addr /256;
	m_write_buf[ 12 ] = ( m_config_ptr->yk_begin + ytno ) % 256 ;
	m_write_buf[ 13 ] = ( m_config_ptr->yk_begin + ytno ) / 256 ;

	//YKDCO �غϿط�,�ɵ����˫��ң��ȷ��
	YKDCO ytdco ;
	if ( yt_flag == 1 )
	{
		ytdco.dco_se = 0 ;
	}
	else
	{
		ytdco.dco_se = 1 ;
	}
	if (m_config_ptr->yk_type == C_SC_NA_1)		//����ң��
	{
		if(1)
		{
			ytdco.dco_dcs = STATE_SP_ON ;
		}	
		else
		{
			ytdco.dco_dcs = STATE_SP_OFF ;
		}
	}
	else if (m_config_ptr->yk_type == C_DC_NA_1)	//˫��ң��
	{
		if(1)
		{
			ytdco.dco_dcs = STATE_DP_ON ;
		}	
		else
		{
			ytdco.dco_dcs = STATE_DP_OFF ;
		}
	}
	ytdco.dco_qu = 0 ;

	//modify for IBM bit begin
	unsigned char tmpbuf; 
	unsigned char tmpbuf1; // 0001 1111
	unsigned char tmpbuf2; 

	tmpbuf = (ytdco.dco_se<<7);
	tmpbuf1 = (ytdco.dco_qu)&0x1f;
	tmpbuf1 = tmpbuf1<<2;
	tmpbuf2 = ( ytdco.dco_dcs )&0x03;

	m_write_buf[ 14 ] = tmpbuf|tmpbuf1|tmpbuf2;

	m_current_ykno = ytno;				//�洢��ǰ�·�ң����ţ��Ա�У��
	m_current_ykoder = ytdco.dco_dcs;			//�洢��ǰ�·�ң������Ա�У��

	CRC_test( m_write_buf + 3 , 12 , CRC_WRITE ) ;
	m_write_buf[ 16 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 1 ] + UNFIXED_DATA_LEN;

	return send_msg();
}

int Iec101b::Iec101b_receive_proc(void)
{
	int cur_position = 0 ;
	int frame_type = INVALID_FRAME ;
	int frame_len = 0 ;
	ACE_OS::memset( m_rec_buf , 0 , MAX_BUF_SIZE ) ;
	if ( m_read_lenth == YK_CONFIRM_LEN && m_rec_buf[ 0] == YK_CONFIRM_HEAD ) //ң����·��ȷ��֡
	{
		frame_type = YKCONFIRM_FRAME ;
		m_read_lenth = YK_CONFIRM_LEN ;
		ACE_OS::memcpy( m_rec_buf, &m_read_buf[ 0 ], YK_CONFIRM_LEN );
		return frame_type ;
	}
	if ( m_read_lenth < FIXED_FRAME_LEN ) //֡��С��6
	{
		return frame_type ;
	}

	for ( cur_position = 0 ; cur_position < m_read_lenth ; cur_position++ )
	{
		//�̶�֡����
		if ( m_read_buf[ cur_position ] == FIXED_FRAME_HEAD && m_read_buf[ cur_position + FIXED_FRAME_LEN - 1 ] == FRAME_TAIL )
		{
			ACE_OS::memcpy( m_rec_buf, &m_read_buf[ cur_position ], FIXED_FRAME_LEN );   //����һ֡�������Ĵ�����
			frame_type = FIXED_FRAME ;
			ACE_OS::memmove( m_read_buf, &m_read_buf[cur_position + FIXED_FRAME_LEN], m_read_lenth - FIXED_FRAME_LEN - cur_position ) ;
			m_read_lenth = m_read_lenth - cur_position - FIXED_FRAME_LEN;
			ACE_OS::memset( &m_read_buf[m_read_lenth], 0, REC_LEN - m_read_lenth ) ;	//��λ������������ʼ��
			break;
		}

		//�ɱ�֡����
		else if ( m_read_buf[ cur_position ] == UNFIXED_FRAME_HEAD && m_read_buf[ cur_position + 3 ] == UNFIXED_FRAME_HEAD )
		{
			frame_len = m_read_buf[ cur_position + 1 ] + UNFIXED_DATA_LEN ;

			if( ( cur_position + frame_len ) > m_read_lenth )				//���������ı������´δ���	
			{
				ACE_OS::memmove(m_read_buf, &m_read_buf[cur_position], m_read_lenth - cur_position);
				m_read_lenth -= cur_position;
				frame_len = 0;
			}
			else
			{
				ACE_OS::memcpy( m_rec_buf, &m_read_buf[ cur_position ], frame_len);   //����һ֡�������Ĵ�����
				frame_type = UNFIXED_FRAME ;
				//��m_read_buf��δ����ı�����λ
				ACE_OS::memmove( m_read_buf, &m_read_buf[cur_position + frame_len], m_read_lenth - frame_len - cur_position ) ;
				m_read_lenth = m_read_lenth - cur_position - frame_len;
			}
			if( m_read_lenth < 0 || m_read_lenth > REC_LEN )	//���Ļ��������ޣ�ȫ�����
			{
				m_read_lenth = 0;
				ACE_OS::memset(m_read_buf, 0, REC_LEN);	
			}
			ACE_OS::memset( &m_read_buf[m_read_lenth], 0, REC_LEN - m_read_lenth ) ;	//��λ������������ʼ��
			break;
		}
	}
		if (cur_position == m_read_lenth)	//�������������ݶ����ԣ�ȫ�����
	{
		m_read_lenth = 0;
		ACE_OS::memset(m_read_buf, 0, REC_LEN);	
	}

	return frame_type ;
}

int Iec101b::deal_error_frame(void)
{
	ACE_DEBUG((LM_DEBUG, "recv invaild frame!\n"));
	return 0;
}

int Iec101b::deal_fixed_frame(void)
{
	if ( !CRC_test( m_rec_buf ,  4 , CRC_READ ) )
	{
		return DEFAULT_ERROR ;
	}

	ControlField	SFC;
	//modify for IBM bit begin
	unsigned char tmpFC = m_rec_buf[ 1 ] ;

	SFC.DIR = (tmpFC>>7) & 0x01;
	SFC.PRM = (tmpFC>>6) & 0x01;
	SFC.FCB = (tmpFC>>5) & 0x01;
	SFC.FCV = (tmpFC>>4) & 0x01;
	SFC.FC = tmpFC & 0x0F;
	//end	

	if( m_rec_buf[ 2 ] != m_common_addr )
	{
		return DEFAULT_ERROR ;
	}

	if ( SFC.PRM == 1 )	// PRM=1 �ն�����
	{
		if (SFC.FC == REQUESTLINKSTATUS)
		{
			m_subrequest_link_flag = true;
			m_sublink_flag = false;
		}
		else if (SFC.FC == RESET)
		{
			m_subreset_link_flag = true;
		}
		else
		{
			return DEFAULT_ERROR ;
		}
	}
	else
	{
		if ( m_link_flag == REQUESTLINKSTATUS && SFC.FC == LINKREDAY )
	    {
		    m_link_flag = RESET ;
		    m_reset_link_flag = true ;
	    }
	    else if ( m_link_flag == RESET && SFC.FC == ACK )
	    {
//			m_sublink_flag = true;			
	    }
		else if ( m_link_flag == CALLTOTAL && SFC.FC == ACK )
		{
//			m_sublink_flag = true;
		}
		else if ( m_link_flag == -1 && SFC.FC == ACK )
		{
			//m_sublink_flag = true;
			//m_t0_timeout = true ;
		}
	}
	
	return DEAL_SUCCESS ;
}

int Iec101b::deal_unfixed_frame(void)
{
	if ( m_rec_buf[ 1 ] != m_rec_buf[ 2 ] ) //���Ȳ�һ��,ȫ�����
	{
		return DEFAULT_ERROR ;
	}

	if ( !CRC_test( m_rec_buf + 3 ,  m_rec_buf[ 1 ] + 1 , CRC_READ ) )
	{
		return DEFAULT_ERROR ;
	}

	if( m_rec_buf[ 5 ] != m_common_addr )
	{
		return DEFAULT_ERROR ;
	}

	ControlField	SFC;
	//modify for IBM bit begin
	unsigned char tmpFC = m_rec_buf[ 4 ] ;

	SFC.DIR = (tmpFC>>7) & 0x01;
	SFC.PRM = (tmpFC>>6) & 0x01;
	SFC.FCB = (tmpFC>>5) & 0x01;
	SFC.FCV = (tmpFC>>4) & 0x01;
	SFC.FC = tmpFC & 0x0F;
	//end

	int ret = DEFAULT_ERROR ;
// 	if ( SFC.FC == SENDCONFIRM )//������=0 Ϊȷ��֡
// 	{
		switch ( m_rec_buf[ 7 ] )
		{
		case C_CD_NA_1 ://��ʱȷ��
			{
				if ( (m_rec_buf[ 9 ]& 0x3f) == COT_ACTCON)
				{
					m_confirm_link_flag = true ;
					m_send_delaytime_flag = true ;
					m_t3 =  m_rec_buf[ 14 ] + m_rec_buf[ 15 ] * 256 ;
				}
			}
		case C_CS_NA_1://��ʱȷ��
			{
				if ( (m_rec_buf[ 9 ]& 0x3f) == COT_ACTCON)
				{
					m_chandata->sync_time_success = 1;
					m_sync_time_flag = NO_NEED;		//��ʱ����
					m_sublink_flag = false;
					m_confirm_link_flag = true;		//ȷ��
					ret = DEAL_SUCCESS ;
				}
			}
			break;
		case M_SP_NA_1:							//yx����
			ret = deal_sequence_M_SP_NA_1();
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_DP_NA_1:
			ret = deal_sequence_M_DP_NA_1();
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_PS_NA_1:
			ret = deal_sequence_M_PS_NA_1();
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_SP_TA_1:							//yxsoe����
			ret = deal_sequence_M_SP_TA_1();
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_DP_TA_1:
			ret = deal_sequence_M_DP_TA_1();
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_SP_TB_1:
			ret = deal_sequence_M_SP_TB_1();
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_DP_TB_1:
			ret = deal_sequence_M_DP_TB_1();
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_ME_NA_1:							//yc����
		case M_ME_NB_1:
			ret = deal_sequence_M_ME_NA_1();
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_ME_TA_1:
			ret = deal_sequence_M_ME_TA_1();
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_ME_ND_1:
			ret = deal_sequence_M_ME_ND_1();
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_ME_NC_1:
			ret = deal_sequence_M_ME_NC_1();
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_IT_NA_1:							//kwh����
			ret = deal_sequence_M_IT_NA_1();	
			m_confirm_link_flag = true;		//ȷ��
			break;
		case M_IT_TA_1:  //����
		case M_IT_TB_1:
			break;
		case M_BO_NA_1:		//7:��վԶ���ն�״̬���� ����
			break;
		case C_SC_NA_1:
		case C_DC_NA_1:
			{
				ret = proc_yk_frame();			//yk����
				m_confirm_link_flag = true;		//ȷ��
			}
			break;
		case C_RC_NA_1://47://������У
			//		A_DAResult(StaNo,ChNo,DataHead,comp);
			break;
		case M_EI_NA_1:
			{
				m_sublink_flag = false ;
				m_call_all_data_flag  = NEED_CALL ;//�ൺ�ǵ��ն˻ᷢ��ʼ����������
				m_confirm_link_flag = true;
				//m_link_flag = CALLTOTAL ;
				ret=DEAL_SUCCESS;
			}
			break;
		case C_IC_NA_1:		//����
			{
				if ((m_rec_buf[ 9 ] & 0x3f) == COT_ACTTERM)
				{
					m_call_all_data_flag = NO_NEED;		//���ٻ�����,ȷ�ϲ�������
					m_sync_time_flag = NEED_CALL ;
					m_confirm_link_flag = true;		//ȷ��
					m_sublink_flag = false;
				}
				else if ((m_rec_buf[ 9 ] & 0x3f) == COT_ACTCON)
				{
					m_call_all_data_flag = ON_PROCESS;		//���ٻ�����
					m_confirm_link_flag = true;		//ȷ��
					m_sublink_flag = false;
				}
                ret=DEAL_SUCCESS;
			}
			break;
		case C_CI_NA_1:		//����ٻ�
			{
				ret = 0;
				if ((m_rec_buf[ 9 ] & 0x3f) == COT_ACTTERM)
				{
					m_call_kwh_data_flag = NO_NEED;		//����ٻ�����,ȷ�ϲ�������
				}
				m_confirm_link_flag = true;		//ȷ��
			}
			break;		
		default:
			break;
		}
//	}
	if (ret == DEFAULT_ERROR)
	{
		ACE_DEBUG((LM_DEBUG, "���ݴ������\n"));
	}

	return ret;
}

//YX���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Iec101b::deal_sequence_M_SP_NA_1(void)
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;
	if( comm_addr != m_common_addr )		//�жϹ�����ַ
	{
		return DEFAULT_ERROR ;
	}

	int data_no = 0;		//ң�ŵ��

	if ( sq == 1 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 12 );
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;

		unsigned char* pt = (unsigned char*)( m_rec_buf + 14 );
		int i;
		for (i = 0; i < info_num; i++)
		{
			make_yx_real_data(data_no, *pt, 1);
			data_no++;
			pt++;
		}		
	}
	else			//ÿ֡����Ϣ���ַ
	{
		SEQ_M_NA_1* yx_struct = (SEQ_M_NA_1*)( m_rec_buf + 12 );
		int i;
		char quanity ;
		for (i = 0; i < info_num; i++)
		{
			data_no = yx_struct->addr1 - m_config_ptr->yx_begin;
			quanity = ( yx_struct->yx_value & 0xF0 ) >> 4 ;
			yx_struct->yx_value = yx_struct->yx_value & 0x01 ;
			make_yx_real_data(data_no, yx_struct->yx_value, 1);
			yx_struct++;
		}		
	}

	return DEAL_SUCCESS ;
}

int Iec101b::deal_sequence_M_DP_NA_1(void)
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;
	if( comm_addr != m_common_addr )		//�жϹ�����ַ
	{
		return DEFAULT_ERROR ;
	}
	int data_no = 0;		//ң�ŵ��
	unsigned char value = 0;

	if ( sq == 1 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 12 );
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;

		unsigned char* pt = (unsigned char*)( m_rec_buf + 14 );
		int i;
		for (i = 0; i < info_num; i++)
		{
			//˫��ң�ż���״̬
			value  = *pt  & 0x03;			//����״̬����
			if (value == 1)
			{
				value = 0;	//״̬��
			}
			else if (value == 2)
			{
				value = 1;	//״̬��
			}
			else
			{
				value = 2;	//״̬��Ч
			}
			make_yx_real_data(data_no, value, 1);
			data_no++;
			pt++;
		}		
	}
	else			//ÿ֡����Ϣ���ַ
	{
		SEQ_M_NA_1* yx_struct = (SEQ_M_NA_1*)( m_rec_buf + 12 );
		int i;
		for (i = 0; i < info_num; i++)
		{ 
			data_no = yx_struct->addr1 - m_config_ptr->yx_begin;

			//˫��ң�ż���״̬
			value  = yx_struct->yx_value  & 0x03;			//����״̬����
			if (value == 1)
			{
				value = 0;	//״̬��
			}
			else if (value == 2)
			{
				value = 1;	//״̬��
			}
			else
			{
				value = 2;	//״̬��Ч
			}
			make_yx_real_data(data_no, value, 1);

			yx_struct++;
		}		
	}

	return DEAL_SUCCESS ;
}

int Iec101b::deal_sequence_M_PS_NA_1(void)			//need for check �ô���ʽ�Բ���ȫ������֤
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;
	if( comm_addr != m_common_addr )		//�жϹ�����ַ
	{
		return DEFAULT_ERROR ;
	}
	int data_no = 0;		//ң�ŵ��
	unsigned char value = 0;

	if ( sq == 1 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 12 );
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;

		SEQ_M_PS_NA_1* yx_struct = (SEQ_M_PS_NA_1*)( m_rec_buf + 14 );
		for (int j = 0; j< info_num; j++)		//��Ϣ��ѭ��
		{
			for (int i=0; i<16; i++)		//16��Ϊһ��
			{
				if ( (yx_struct->statuscheck>>i) &0x01 )		//״̬λ���ټ����һ��״̬�仯
				{
					data_no ++;
					if (data_no > m_chanpara->yx_num )		//yxnum����
					{
						break;
					}
					value = (yx_struct->value>>i) & 0x01;

					make_yx_real_data(data_no, value, yx_struct->quanity);
				}
				else
				{
					ACE_DEBUG((LM_DEBUG,"yxno = %d yx has no change", data_no));
				}
			}	//�������һ��
			yx_struct ++;	
		}

	}
	else
	{
		struct SEQ_M_PS_NA_1_B
		{
			INFO_ADDR addr;
			SEQ_M_PS_NA_1 seq_data;
		};

		SEQ_M_PS_NA_1_B* yx_struct = (SEQ_M_PS_NA_1_B*)( m_rec_buf + 12 );

		for (int j = 0; j< info_num; j++)
		{
			data_no = yx_struct->addr.addr1 - m_config_ptr->yx_begin;

			for (int i=0; i<16; i++)		//16��Ϊһ��
			{
				if ( (yx_struct->seq_data.statuscheck>>i) &0x01 )		//״̬λ���ټ����һ��״̬�仯
				{
					data_no ++;
					if (data_no > m_chanpara->yx_num )		//yxnum����
					{
						break;
					}
					value = (yx_struct->seq_data.value>>i) & 0x01;

					make_yx_real_data(data_no, value, yx_struct->seq_data.quanity);

				}
				else
				{
					ACE_DEBUG((LM_DEBUG,"yxno = %d yx has no change", data_no));
				}
			}		
		}
	}

	return DEAL_SUCCESS ;

}
//YX���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//YXSOE���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Iec101b::deal_sequence_M_SP_TA_1()		//M_SP_TA_1
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;
	int data_no = 0;		//ң�ŵ��
	FETIME soe_time;
	SEQ_M_TA_1* seq_struct = (SEQ_M_TA_1*)( m_rec_buf + 12 );

	for (int i = 0; i < info_num; i++)
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin;  //����yx���
		soe_time = T101Time_To_Realtime(seq_struct->short_time);	

		make_event(soe_time, data_no, seq_struct->value);

		seq_struct++;
	}

	return DEAL_SUCCESS ;
}

int Iec101b::deal_sequence_M_DP_TA_1()		//M_DP_TA_1
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;
	int data_no = 0;		//ң�ŵ��
	unsigned char value = 0;

	FETIME soe_time;
	SEQ_M_TA_1* seq_struct = (SEQ_M_TA_1*)( m_rec_buf + 12 );

	for (int i = 0; i < info_num; i++)
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin;  //����yx���
		soe_time = T101Time_To_Realtime(seq_struct->short_time);	

		//˫��ң�ż���״̬
		value = seq_struct->value & 0x03;
		if (value == 1)
		{
			value = 0;	//״̬��
		}
		else if (value == 2)
		{
			value = 1;	//״̬��
		}
		else
		{
			value = 2;	//״̬��Ч
		}

		make_event(soe_time, data_no, seq_struct->value);

		seq_struct++;
	}

	return DEAL_SUCCESS ;
}


int Iec101b::deal_sequence_M_SP_TB_1()	//M_SP_TB_1
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;
	int data_no = 0;		//ң�ŵ��
	FETIME soe_time;
	SEQ_M_TB_1* seq_struct = (SEQ_M_TB_1*)( m_rec_buf + 12 );

	for (int i = 0; i < info_num; i++)
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin;  //����yx���

		//ʱ��ת��ΪFETIME
		soe_time.year = seq_struct->long_time.year + 100;
		soe_time.month = seq_struct->long_time.month;
		soe_time.day = seq_struct->long_time.day;
		soe_time.hour = seq_struct->long_time.hour;
		soe_time.minute = seq_struct->long_time.minute;
		soe_time.second = seq_struct->long_time.millsecond / 1000;
		soe_time.milisecond = seq_struct->long_time.millsecond %1000;

		make_event(soe_time, data_no, seq_struct->value);
//		make_yx_real_data(data_no, seq_struct->value, 1);
		seq_struct++;
	}

	return DEAL_SUCCESS ;
}

int Iec101b::deal_sequence_M_DP_TB_1()	//M_DP_TB_1
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;
	int data_no = 0;		//ң�ŵ��
	unsigned char value = 0;
	FETIME soe_time;
	SEQ_M_TB_1* seq_struct = (SEQ_M_TB_1*)( m_rec_buf + 12 );

	for (int i = 0; i < info_num; i++)
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin;  //����yx���
		//ʱ��ת��ΪFETIME
		soe_time.year = seq_struct->long_time.year + 100;
		soe_time.month = seq_struct->long_time.month;
		soe_time.day = seq_struct->long_time.day;
		soe_time.hour = seq_struct->long_time.hour;
		soe_time.minute = seq_struct->long_time.minute;
		soe_time.second = seq_struct->long_time.millsecond / 1000;
		soe_time.milisecond = seq_struct->long_time.millsecond %1000;

		//˫��ң�ż���״̬
		value = seq_struct->value & 0x03;
		if (value == 1)
		{
			value = 0;	//״̬��
		}
		else if (value == 2)
		{
			value = 1;	//״̬��
		}
		else
		{
			value = 2;	//״̬��Ч
		}

		make_event(soe_time, data_no, seq_struct->value);

		seq_struct++;
	}

	return DEAL_SUCCESS ;
}
//YXSOE���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//YC���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Iec101b::deal_sequence_M_ME_NA_1()		//9��11��ʶYC����
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char datanum = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;

	if( comm_addr != m_common_addr )		//�жϹ�����ַ
	{
		return DEFAULT_ERROR ;
	}

	int data_no = 0;		//ң����

	if ( sq == 1 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 12 );
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//����ң����

		SEQ_M_ME_NA_1 * seq_data = (SEQ_M_ME_NA_1*)( m_rec_buf + 14 );
		for (int i =0; i< datanum; ++i)
		{
			make_yc_real_data(data_no, seq_data->value, seq_data->quanity);
			data_no++;		//ң������������
			seq_data ++;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		struct SEQ_M_ME_NA_1_B
		{
			INFO_ADDR addr;
			SEQ_M_ME_NA_1 seq_data;
		};

		int sizen = sizeof(SEQ_M_ME_NA_1_B);
		SEQ_M_ME_NA_1_B * yc_struct = (SEQ_M_ME_NA_1_B*)( m_rec_buf + 12 );
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//����ң����
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity);
			yc_struct ++;
		}

	}
	return DEAL_SUCCESS ;
}

int Iec101b::deal_sequence_M_ME_TA_1()	//10��ʱ��ң��ֵ
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char datanum = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;

	if( comm_addr != m_common_addr )		//�жϹ�����ַ
	{
		return DEFAULT_ERROR ;
	}

	int data_no = 0;		//ң����
	FETIME time;			//ң�ⷢ��ʱ��
	SEQ_M_ME_TA_1 * yc_struct = (SEQ_M_ME_TA_1*)( m_rec_buf + 12 );
	for (int i =0; i< datanum; ++i)
	{
		time = T101Time_To_Realtime(yc_struct->short_time);
		data_no = yc_struct->info_addr.addr1 - m_config_ptr->yc_begin;
		make_yc_real_data(data_no, yc_struct->value, yc_struct->quanity, &time);
		yc_struct++;
	}
	return DEAL_SUCCESS ;
}

int Iec101b::deal_sequence_M_ME_NC_1()		//13��ʶYC����
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char datanum = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;

	if( comm_addr != m_common_addr )		//�жϹ�����ַ
	{
		return DEFAULT_ERROR ;
	}

	int data_no = 0;		//ң����

	if ( sq == 1 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 12 );
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//����ң����

		SEQ_M_ME_NC_1 * seq_data = (SEQ_M_ME_NC_1*)( m_rec_buf + 14 );
		for (int i =0; i< datanum; ++i)
		{
			make_yc_real_data(data_no, seq_data->value, seq_data->quanity);

			data_no++;		//ң������������
			seq_data ++;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		SEQ_M_ME_NC_1_B * yc_struct = (SEQ_M_ME_NC_1_B*)( m_rec_buf + 12 );
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//����ң����
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity);
			yc_struct ++;
		}

	}
	return DEAL_SUCCESS ;
}

int Iec101b::deal_sequence_M_ME_ND_1()		//21:��Ʒ������ң��ֵ
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char datanum = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;

	if( comm_addr != m_common_addr )		//�жϹ�����ַ
	{
		return DEFAULT_ERROR ;
	}

	int data_no = 0;		//ң����

	if ( sq == 1 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 12 );
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//����ң����

		int16s* yc_data = (int16s*)( m_rec_buf + 14 );
		for (int i =0; i< datanum; ++i)
		{
			make_yc_real_data(data_no, *yc_data, 1);
			data_no++;		//ң������������
			yc_data ++;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		struct SEQ_M_ME_ND_1_B
		{
			INFO_ADDR addr;
			int16s yc_data;
		};

		SEQ_M_ME_ND_1_B * yc_struct = (SEQ_M_ME_ND_1_B*)( m_rec_buf + 12 );
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//����ң����
			make_yc_real_data(data_no, yc_struct->yc_data, 1);
			yc_struct ++;
		}

	}
	return DEAL_SUCCESS ;
}
//YC���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//KWH���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Iec101b::deal_sequence_M_IT_NA_1()		//0x15���
{
	unsigned char sq = ( m_rec_buf[ 8 ] &0x80 )>>7;
	unsigned char datanum = m_rec_buf[ 8 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] ;

	if( comm_addr != m_common_addr )		//�жϹ�����ַ
	{
		return DEFAULT_ERROR ;
	}
	int data_no = 0;		//ң�����

	if ( sq == 1 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 12 );
		data_no = pt_addr->addr1 - m_config_ptr->kwh_begin;//����ң�����

		SEQ_M_IT_NA_1 * seq_data = (SEQ_M_IT_NA_1*)( m_rec_buf + 14 );
		for (int i =0; i< datanum; ++i)
		{
			make_kwh_real_data(data_no, seq_data->value, seq_data->quanity);
			data_no++;		//ң�������������
			seq_data ++;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		struct SEQ_M_IT_NA_1_B
		{
			INFO_ADDR addr;
			SEQ_M_IT_NA_1 seq_data;
		};

		SEQ_M_IT_NA_1_B * kwh_struct = (SEQ_M_IT_NA_1_B*)( m_rec_buf + 12 );
		for (int i =0; i< datanum; ++i)
		{
			data_no = kwh_struct->addr.addr1 - m_config_ptr->kwh_begin;//����ң�����
			make_kwh_real_data(data_no, kwh_struct->seq_data.value, kwh_struct->seq_data.quanity);
			kwh_struct ++;
		}

	}
	return DEAL_SUCCESS ;
}
//KWH���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//YK���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Iec101b::proc_yk_frame()
{
	char str[NORMALCHAR_LEN];

	unsigned char cause = m_rec_buf[ 9 ] ;
	unsigned char comm_addr = m_rec_buf[ 10 ] + m_rec_buf[ 11 ] * 256 ;
	int ret_ykno = m_rec_buf[ 12 ] + m_rec_buf[ 13 ] * 256 - m_config_ptr->yk_begin;

	int yk_ret = DEAL_SUCCESS ;
	if( comm_addr != m_common_addr )		//�жϹ�����ַ
	{
		ACE_OS::sprintf(str,"ң�ع�����ַ��,��ȷ��ַ:%d,��У��ַ:%d",m_common_addr,comm_addr);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if (ret_ykno != m_current_ykno)	//ң�غų���
	{
		ACE_ERROR((LM_ERROR, "�·���ң�غźͷ�У��ң�غŲ�һ��!"));
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if ((cause & 0x3f) != COT_ACTCON && (cause & 0x3f) != COT_DEACTCON&& (cause & 0x3f) != COT_ACTTERM)	//����ԭ���
	{
		ACE_OS::sprintf(str,"ң�ط�У�Ĵ���ԭ����Ч,cot = %02x",cause & 0x3f);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	int tmp_ykcoder = m_rec_buf[ 14 ] & 0x03 ;
	if (tmp_ykcoder != m_current_ykoder)	//ң���������
	{
		ACE_OS::sprintf(str,"ң�������,�·�����:%02x ��У����:%02x",m_current_ykoder,tmp_ykcoder);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if ( (cause & 0x3f) == COT_ACTTERM )
	{
		ACE_OS::sprintf(str,"ң�ؽ���!");
		yk_ret  = DEAL_SUCCESS;
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
	}
	else if((cause & 0x40) == 0 )		//�жϷ�У���
	{
		ACE_OS::sprintf(str,"ң�ط�У��ȷ!");
		yk_ret  = DEAL_SUCCESS;
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
	}
	else if((cause & 0x40) == 0x40 )
	{
		ACE_OS::sprintf(str,"ң�ط�У������!");
		yk_ret  = DEAL_SUCCESS;
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

//	if (!(tmp_ykcoder & 0x80))		//ң��ִ�л�ȡ����ɺ�
	if ( cause & 0x3f == COT_ACTTERM )		//ң��ִ�л�ȡ����ɺ�
	{
		ACE_DEBUG((LM_DEBUG, "ң�ض������!"));
		m_current_ykoder = 0;	//���ң�غ�ң��״̬��λ
	}

	//������ȷ��Ϣ
	yk_ret = DEAL_SUCCESS;
	return yk_ret;
}


//YK���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����Լ�ϴ��Ķ�ʱ��ʱ�䣬ת��Ϊ��ʵʱ��
FETIME T101Time_To_Realtime(CP24Time2a& SoeTime)
{
	//�˺�����101��ʱ��ת����ʵ��ʱ�䣬��ʱ��ֻ����һСʱ�����Ͳſɼ���
	struct tm TimeMinus,TimeAdd,TimeNochange;//�ֱ���Сʱ��1��Сʱ��1��Сʱ�����ʱ����������ʱ��
	struct tm *UseTime;
	struct tm *CurTime;
	FETIME Time ;
	time_t ltime,tmptime;
	long Diff_Minus,Diff_Add,Diff_Nochange;

	time( &ltime );
	tmptime =  ltime - 3600;
	CurTime = localtime( &tmptime );
	//��Сʱ�ֱ��ȥ�����ϡ����䣬�Ա�Ƚ�
	TimeMinus.tm_year = CurTime->tm_year;
	TimeMinus.tm_mon = CurTime->tm_mon;
	TimeMinus.tm_mday = CurTime->tm_mday;
	TimeMinus.tm_hour = CurTime->tm_hour;
	TimeMinus.tm_min = SoeTime.minutes;
	TimeMinus.tm_sec = SoeTime.millsecond / 1000;

	tmptime = ltime + 3600;
	CurTime = localtime( &tmptime );
	TimeAdd.tm_year = CurTime->tm_year;
	TimeAdd.tm_mon = CurTime->tm_mon;
	TimeAdd.tm_mday = CurTime->tm_mday;
	TimeAdd.tm_hour = CurTime->tm_hour;
	TimeAdd.tm_min = SoeTime.minutes;
	TimeAdd.tm_sec = SoeTime.millsecond / 1000;

	CurTime = localtime( &ltime );
	TimeNochange.tm_year = CurTime->tm_year;
	TimeNochange.tm_mon = CurTime->tm_mon;
	TimeNochange.tm_mday = CurTime->tm_mday;
	TimeNochange.tm_hour = CurTime->tm_hour;
	TimeNochange.tm_min = SoeTime.minutes;
	TimeNochange.tm_sec = SoeTime.millsecond / 1000;

	Diff_Minus = (long)fabs( double(mktime( &TimeMinus ) - ltime) );
	Diff_Add = (long)fabs( double(mktime( &TimeAdd ) - ltime) );
	Diff_Nochange = (long)fabs( double(mktime( &TimeNochange ) - ltime) );

	//ѡȡ�뵱ǰʱ�������С��һ��ʱ��ֵ��ΪSOE��ʱ��
	if (Diff_Minus<Diff_Add && Diff_Minus<Diff_Nochange)
	{
		UseTime = &TimeMinus;
	}
	else if (Diff_Add<Diff_Minus && Diff_Add<Diff_Nochange)
	{
		UseTime = &TimeAdd;
	}
	else if(Diff_Nochange<Diff_Minus && Diff_Nochange<Diff_Add)
	{
		UseTime = &TimeNochange;
	}
	else
	{
		UseTime = &TimeNochange;
	}

	Time.milisecond = SoeTime.millsecond % 1000;
	Time.second = UseTime->tm_sec;
	Time.minute = UseTime->tm_min;
	Time.hour = UseTime->tm_hour;
	Time.day = UseTime->tm_mday;
	Time.month = UseTime->tm_mon+1;
	Time.year = UseTime->tm_year+1900;

	return Time;
}
