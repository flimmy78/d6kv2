/**
@file encryiec104.cpp
@brief ����104��Լ����Դ�ļ�

@author chenkai 
@version 1.0.0
@date 2015-11-29
*/

#include "encryiec104.h"

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
using namespace std ;

const char* const version_string = "1.0.0" ;
const char* const desc_string = "the very beginning" ;

//���Ӳ��ɹ��󣬵ȴ�2���ٳ�������
const int CONN_RETRY_TIME = 2 ;
const int SECOND_LOOP_TIME = 1 ;

/**
@brief ����IEC104������
*/
static ENCRY_IEC104_CONFIG encry_iec104_config[ MAX_CHAN_NUM ] ;

//Ԥ�õĿ���֡����
unsigned char TEST_FR_VAL[ 6 ]={0x68,0x04,0x43,0x00,0x00,0x00} ;
unsigned char TEST_FR_ACK[ 6 ]={0x68,0x04,0x83,0x00,0x00,0x00} ;
unsigned char STOP_DT_VAL[ 6 ]={0x68,0x04,0x13,0x00,0x00,0x00} ;
unsigned char START_DT_VAL[ 6 ]={0x68,0x04,0x07,0x00,0x00,0x00} ;
unsigned char S_FRAME_ACK[ 6 ]={0x68,0x04,0x01,0x00,0x00,0x00} ;

SGD_UCHAR PRI_KEYWORD[ 16 ] = "12345678" ;

FETIME T101Time_To_Realtime( CP24Time2a& SoeTime ) ;	//CP24time����Ϊʵ��ʱ��


Base_protocol* get_protocol( const int& channel )
{
	return new Encry_Iec104( channel ) ;
}

void get_config( void )
{
	//todo
	//�����ݿ��ȡ104������Ϣ��д��encry_iec104_config
	struct ENCRY_IEC104_CONFIG_
	{
		unsigned short channel_no ; //ͨ����
		unsigned short yx_type ;	//ң������
		unsigned short soe_type ;	//SOE����
		unsigned short yc_type ;	//ң������
		unsigned short kwh_type ; //�������

		unsigned short yk_type ;	//ң�����ͣ������˫��


		short call_all_data_gap ;///<���ټ����Ϊ-1ʱ������
		short call_kwh_gap ;		///<�ٻ���ȼ����Ϊ-1ʱ���ٻ�
		short send_time_gap ;	///<��ʱ�����Ϊ-1ʱ����ʱ

		unsigned short yx_begin ;	///<ң����ʼ��ַ
		unsigned short yc_begin ;	///<ң����ʼ��ַ
		unsigned short yk_begin ;	///<ң����ʼ��ַ
		unsigned short kwh_begin ;	///<�����ʼ��ַ

		unsigned short t0 ;		///<t0��ʱ���������ӳ�ʱ
		unsigned short t1 ;		///<t1��ʱʱ�䣬����һ��I֡��U֡�󣬱�����t1ʱ���ڽ��յ��Է���ȷ�ϣ�������Ϊ���������⣬��Ҫ�ر���������
		unsigned short t3 ;		///<t3��ʱʱ�䣬���յ��Է�һ֡���ĺ󣬾���t3ʱ��û�н��յ��µı��ģ���Ҫ��Է�����U֡����

		unsigned short test_fr_cnt ;	//δ�ظ��Ĳ���֡��ֵ
		unsigned short k ;		///kֵ����k��I֡δ�õ��Է�S֡ȷ��ʱ��ֹͣ���䣬ȡֵ��Χ1-32767
		unsigned short w ;		///<wֵ����ٽ��յ��Է�w��I֡ʱ����Է�����S֡��ȡֵ��Χ1-32767

		unsigned char check_ns_nr ;		///<��鷢�ͽ�����ţ���Ŵ�����Ҫ�ر����ӣ�1Ϊ��飬0Ϊ�����
		unsigned char chg_to_soe ;		///<ң��תsoe��1ΪSOEתң�أ�0Ϊ��ת 
		unsigned char soe_to_chg ;	///<soeתң�ţ�1ΪSOEתң�أ�0Ϊ��ת
		unsigned char grp_yc_check_chg_flag ;	///<���鵥��ң�ż���λ��־��1Ϊ��飬0Ϊ�����
	} ;
	ENCRY_IEC104_CONFIG_ * special_config = NULL ;

	int ret = Db_connector::select( "select * from encry_iec104_special_config order by channel_no asc", ( void ** )&special_config ) ;
	int rcd_cnt = ret ;		//��Լ��������Ԫ������

	if( ret < 0 )	//�������ݿ����
	{
		return ;
	}

// 	ACE_DEBUG( ( LM_DEBUG, "read iec104 special \n" ) ) ;

	for ( int nnum = 0 ; nnum < rcd_cnt ; ++nnum )
	{
		unsigned short channel_no = special_config->channel_no ;

		encry_iec104_config[ channel_no ].yx_type = special_config->yx_type ;
		encry_iec104_config[ channel_no ].soe_type = special_config->soe_type ;
		encry_iec104_config[ channel_no ].kwh_type = special_config->kwh_type ;

		encry_iec104_config[ channel_no ].call_all_data_gap = special_config->call_all_data_gap ;
		encry_iec104_config[ channel_no ].call_kwh_gap = special_config->call_kwh_gap ;
		encry_iec104_config[ channel_no ].send_time_gap = special_config->send_time_gap ;

		encry_iec104_config[ channel_no ].yx_begin = special_config->yx_begin ;
		encry_iec104_config[ channel_no ].yc_begin = special_config->yc_begin ;
		encry_iec104_config[ channel_no ].yk_begin = special_config->yk_begin ;
		encry_iec104_config[ channel_no ].kwh_begin = special_config->kwh_begin ;

		encry_iec104_config[ channel_no ].t0 = special_config->t0 ;
		encry_iec104_config[ channel_no ].t1 = special_config->t1 ;
		encry_iec104_config[ channel_no ].t3 = special_config->t3 ;

		encry_iec104_config[ channel_no ].test_fr_cnt = special_config->test_fr_cnt ;
		encry_iec104_config[ channel_no ].k = special_config->k ;
		encry_iec104_config[ channel_no ].w = special_config->w ;

		encry_iec104_config[ channel_no ].check_ns_nr = special_config->check_ns_nr ;
		encry_iec104_config[ channel_no ].chg_to_soe = special_config->chg_to_soe ;
		encry_iec104_config[ channel_no ].soe_to_chg = special_config->soe_to_chg ;
		encry_iec104_config[ channel_no ].grp_yc_check_chg_flag = special_config->grp_yc_check_chg_flag ;
		
		special_config++ ;
	}
}

char* get_version( void )
{
	return const_cast<char*>( version_string ) ;
}

char* get_desc( void )
{
	return const_cast<char*>( desc_string ) ;
}


Encry_Iec104::Encry_Iec104( const int& channel )
:Base_protocol( channel ), m_config_ptr( encry_iec104_config + channel )
{
// 	ACE_DEBUG( ( LM_DEBUG, "iec104 constructor\n" ) ) ;

	m_common_addr = m_chanpara->common_addr ;	//������ַ
	m_device_handle = NULL ;
}

Encry_Iec104::~Encry_Iec104( )
{
	if ( NULL != m_device_handle )
	{
		if ( 0 != SDF_CloseDevice( m_device_handle ) )
		{
			printf( "SDF_CloseDevice failed\n" ) ;
		}
	}
}

void Encry_Iec104::init_protocol( void )
{
// 	ACE_DEBUG( ( LM_DEBUG, "iec104 init\n" ) ) ;

	// �������豸
	if ( NULL == m_device_handle )
	{
		if ( 0 != SDF_OpenDevice( &m_device_handle ) )
		{
			printf( "open fail\n" ) ;
		}	
	}

	RTUPARA *pRtuPara = Fes_shm::get( ).rtu_para_ptr + m_rtu_no ;
	FILE* file = NULL ;
	char fileName[ MAXFILENAMELEN ] ;
	sprintf( fileName , "%s/log/dat/keyinf/%d.cfg" , ACE_OS::getenv( "NBENV" ) , m_channel_no ) ;
	file = fopen( fileName , "rb" ) ;

	if ( NULL == file )
	{
		printf( "open key info file failed,use default ( rtuNo = %d )\n" , m_rtu_no ) ;

		pRtuPara->encry_key_info.cur_key_no[ 0 ] = 1 ;
		pRtuPara->encry_key_info.cur_key_no[ 1 ] = 2 ;
		pRtuPara->encry_key_info.cur_key_no[ 2 ] = 3 ;
		pRtuPara->encry_key_info.cur_key_no[ 3 ] = 4 ;

		FILE *wFile = fopen( fileName , "wb" ) ;
		if ( wFile )
		{
			fseek( wFile , 0L , SEEK_SET ) ;
			fwrite( ( const void* )&pRtuPara->encry_key_info , sizeof( ENCRYPT_KEYNO ) , 1 , wFile ) ;
			fclose( wFile ) ;
			printf( "sizeof( ENCRYPT_KEYNO ) = %d\n" , sizeof( ENCRYPT_KEYNO ) ) ;
		}
		else
		{
			printf( "write default key info to file( rtuNo = %d ) failed\n" , m_rtu_no ) ;
		}
	}
	else
	{
		fseek( file , 0L , SEEK_SET ) ;
		fread( ( void* )&pRtuPara->encry_key_info , sizeof( ENCRYPT_KEYNO ) , 1 , file ) ;
		fclose( file ) ;
		printf( "keyNo in card: 1( %d ),2( %d ),3( %d ),4( %d )\n" , pRtuPara->encry_key_info.cur_key_no[ 0 ] , pRtuPara->encry_key_info.cur_key_no[ 1 ] , pRtuPara->encry_key_info.cur_key_no[ 2 ] , pRtuPara->encry_key_info.cur_key_no[ 3 ] ) ;
	}

	m_encrypt_msg_type = TYPENULL ;
	for( int i = 0 ; i < SIGNIDLEN ; i++ )//SIGNID��ֵ
	{
		m_104_sign_id[ i ] = 0x01;
	}

	m_t0_timer.restart( ) ;
	m_t1_timer.restart( ) ;
	m_t2_timer.restart( ) ;
	m_t3_timer.restart( ) ;

	m_call_all_data_flag = NO_NEED ;	
	m_call_kwh_data_flag = NO_NEED ;	
	m_sync_time_flag = NO_NEED ;
	m_rtu_init_flag = NO_NEED ;
	m_check_public_key = NO_NEED ;
	m_refresh_public_key = NO_NEED ;

	m_bt1_timeout = false ;
	m_bstartdt_flag = true ;
	m_btestfr_flag = false ;
	m_bSFrame_flag = false ;

	m_call_all_data_timer.restart( ) ;	
	m_call_kwh_timer.restart( ) ;		
	m_sync_time_timer.restart( ) ;

	m_v_recv = 0 ;
	m_v_send = 0 ;
	m_v_ack = 0 ;

	m_current_ykno = -1 ;
	m_current_ykoder = 0 ;

	m_yx_all_num = Fes_shm::get( ).chan_para_ptr->yx_num ;
	m_yc_all_num = Fes_shm::get( ).chan_para_ptr->yc_num ;
	m_pi_all_num = Fes_shm::get( ).chan_para_ptr->kwh_num ;

	ACE_OS::memset( &m_read_buf, 0, REC_LEN ) ;
	ACE_OS::memset( &m_write_buf, 0, TRN_LEN ) ;

}

void Encry_Iec104::protocol_proc( void )
{
	short curr_host_no = Fes_shm::get( ).fe_sys_data_ptr->host_no ;
	while ( !m_nEnd )
 	{
// 		ACE_DEBUG( ( LM_DEBUG, "��һ��ѭ��\n" ) ) ;
		short curr_attend_host = m_chandata->curr_attend_host ;
		if ( curr_attend_host != curr_host_no || curr_attend_host == -1 )	//�Ǳ�ǰ�ýӹ�ͨ��,ֻ�������ѭ��
		{
			ACE_OS::sleep( ACE_Time_Value( 0,10*1000 ) ) ;
			continue ;
		}
		
		if ( m_dev_ptr )
		{
			m_dev_ptr->close_dev( ) ;
		}
 		if ( open_dev( ) < 0 )		//����˿�δ��
 		{
			ACE_OS::sleep( CONN_RETRY_TIME ) ;
			ACE_DEBUG( ( LM_DEBUG, "��·��ʧ��!\n" ) ) ;
 			continue ;
 		}
		else		//�µ���������,����104��Լ����
		{
			init_protocol( ) ;
		}
 
		//����ѭ������
 		while ( !m_nEnd )		//��������Ϊ��ͨ��ֵ���Ϊ��ǰǰ��
 		{
// 			ACE_DEBUG( ( LM_DEBUG, "�ڶ���ѭ��\n" ) ) ;
			int write_ret = 0 ;
			int read_ret = 0 ;
			int proc_ret = 0 ;
	 		m_write_lenth = 0 ; //���ͳ�������

			//����ʼ��������ʼ�������ش���
			check_event( ) ;
			write_ret = Encry_Iec104_send_proc( ) ;
			if ( -1 == write_ret )
			{
				ACE_DEBUG( ( LM_DEBUG, "д�˿�ʧ��\n" ) ) ;
				break ;
			}

			//���ձ��ģ����жϸ��ֳ�ʱ
			read_ret = recv_msg( ) ;	
			if ( -1 == read_ret )			//���˿�ʧ�ܻᵼ�±���ѭ���˳�
			{
				ACE_DEBUG( ( LM_DEBUG, "���˿�ʧ��\n" ) ) ;
				break ;
			}
			else if ( 0 == read_ret )		//δ���յ����ݣ��жϳ�ʱ
			{
				if ( m_t1_timer.elapsed( ) > m_config_ptr->t1 && m_bt1_timeout  )//����I֡��U֡����ȷ�ϣ�t1��ʱ�ر�����
				{
					ACE_DEBUG( ( LM_DEBUG, "�������ݳ�ʱ\n" ) ) ;
					close_dev( ) ;
					break ;
				}

				if ( m_t2_timer.elapsed( ) > m_config_ptr->t2 )	
				{
// 						send_s_frame( ) ;
				}

				if ( m_t3_timer.elapsed( ) > m_config_ptr->t3 )				//t3��ʱ

				{
					m_btestfr_flag = true ;
					m_t3_timer.restart( ) ;
				}
			}
			else	//��������������t3��ʱ��
			{
				m_t1_timer.restart( ) ;
				m_bt1_timeout = false ;
				m_t3_timer.restart( ) ;
				if ( !( m_v_recv % m_config_ptr->w ) )	//���յ��Է�w��I֡ʱ����Է�����S֡
				{
					if ( m_v_recv )		//�ѽ��յ�I֡
					{
						m_bSFrame_flag = true ;
// 						send_s_frame( ) ;
					}
				}
			}

			//�������١��ٵ�ȼ���ʱ��ʱ��
			if ( m_config_ptr->call_all_data_gap != -1 && m_call_all_data_timer.elapsed( ) >= m_config_ptr->call_all_data_gap )
			{
				m_call_all_data_flag = NEED_CALL ;
				m_call_all_data_timer.restart( ) ;
			}

			if ( m_config_ptr->call_kwh_gap != -1 && m_call_kwh_timer.elapsed( ) >= m_config_ptr->call_kwh_gap )
			{
				m_call_kwh_data_flag = NEED_CALL ;
				m_call_kwh_timer.restart( ) ;
			}

			if ( m_config_ptr->send_time_gap != -1 && m_sync_time_timer.elapsed( ) >= m_config_ptr->send_time_gap )
			{
				m_sync_time_flag = NEED_CALL ;
				m_sync_time_timer.restart( ) ;
			}

			//����ȷ�ϼ��ظ�
			if ( m_read_lenth > 0 )	//���ջ������б����ٽ��д���
			{
// 				ACE_DEBUG( ( LM_DEBUG, "��������,����������%d\n", m_read_lenth ) ) ;
				FRAME_TYPE frame_type = Encry_Iec104_receive_proc( ) ;
				switch ( frame_type )
				{
				case ENCRY_FRAME:
					proc_ret = deal_encry_msg() ;
					break;
				case I_FRAME:
					{
						proc_ret = deal_i_frame( ) ;
						m_t2_timer.restart( ) ;
					}
					break ;
				case S_FRAME:
					proc_ret = deal_s_frame( ) ;
					break ;
				case U_FRAME_TESTFR:
					proc_ret = deal_test_fr( ) ;
					break ;
				case U_FRAME_TESTFR_ACK:
					proc_ret = deal_test_ack( ) ;
					break ;
				case U_FRAME_STOPDT:
					proc_ret = deal_stop_dt( ) ;
					break ;
				case U_FRAME_STARTDT:
					proc_ret = deal_start_dt( ) ;
					break ;
				case INVALID_FRAME:
					proc_ret = deal_error_frame( ) ;
					break ;
				default:
					break ;
				}
				if ( -1 == proc_ret ) //��������ѭ�� 
				{
					break ;
				}
			}

			if ( m_chandata->curr_attend_host != curr_host_no )		//�жϹ黹����
			{
				close_dev( ) ;
				break ;
			}
			ACE_OS::sleep( ACE_Time_Value( 0,10*1000 ) ) ;

		}//while ( true ) ����ѭ��
 	}//while ( true ) һ��ѭ��
}

int Encry_Iec104::Encry_Iec104_send_proc( void )
{
	int ret = 0xff ;
	if ( m_bstartdt_flag )
	{
		ret = send_start_DT( ) ;
		m_bstartdt_flag = false ;
	}
	else if ( m_rtu_init_flag )		//�����ն˳�ʼ��
	{
		ACE_DEBUG( ( LM_DEBUG, "rtu init\n" ) ) ;
		m_rtu_init_flag = NO_NEED ;
	}
	else if ( m_btestfr_flag )
	{
// 		ACE_DEBUG( ( LM_DEBUG, "t3��ʱ,���Ͳ���֡\n" ) ) ;
		ret = send_test_fr( ) ;
		m_btestfr_flag = false ;
	}
	else if( m_bSFrame_flag )
	{
		ret = send_s_frame( ) ;
		m_bSFrame_flag = false ;
	}
	else if ( m_call_all_data_flag == NEED_CALL )		//�ʼ��оݵı��ķ���
	{
		if ( m_call_kwh_data_flag == ON_PROCESS )	//�ٻ���ȵĹ����У�������������
		{
			//todo
		}
		ret = call_all_data( ) ;
	}
	else if ( m_sync_time_flag == NEED_CALL )
	{
		if ( m_call_all_data_flag != ON_PROCESS )		//���ٹ����в����ж�ʱ
		{
			ret = sync_time( ) ;
		}
	}
	else if ( m_check_public_key == NEED_CALL )
	{
		if ( m_call_all_data_flag != ON_PROCESS )		//���ٹ����в����ж�ʱ
		{
			ret = send_check_key_frame( ) ;
		}
	}
	else if ( m_refresh_public_key == NEED_CALL )
	{
		if ( m_call_all_data_flag != ON_PROCESS )		//���ٹ����в����ж�ʱ
		{
			ret = send_update_key_frame( ) ;
		}
	}
	else if ( m_pi_all_num && ( m_call_kwh_data_flag == NEED_CALL ) )	//���е��������Ҫ�ٻ�
	{
		if ( m_call_all_data_flag != ON_PROCESS )		//���ٹ����в����е���ٻ�
		{
			ret = call_pi_data( ) ;
		}
	}


	if ( ( ( m_write_buf[ 2 ]&0x03 ) != 0x01 ) && ret== 0 )		//����I֡��U֡
	{
		m_t1_timer.restart( ) ;
		m_bt1_timeout = true ;
	}
	return ret ;
}

Encry_Iec104::FRAME_TYPE Encry_Iec104::Encry_Iec104_receive_proc( )
{
	int cur_position ;
	int frame_Len = 0 ;

	FRAME_TYPE frame_type ;
	frame_type = INVALID_FRAME ; //��ʼ��Ϊ��Ч֡
	ACE_OS::memset( m_receive_buf, 0, MAX_BUF_SIZE ) ;

	if ( m_read_lenth < 4 ) //֡��С��5
	{
		return frame_type ;
	}

	for ( cur_position = 0 ;cur_position < m_read_lenth ; cur_position++ )
	{
		if ( m_read_buf[ cur_position ] == FIRST )
		{
			frame_Len = m_read_buf[ cur_position + 1 ] + 2 ;

			if( ( cur_position + frame_Len ) > m_read_lenth )				//���������ı������´δ���	
			{
				ACE_OS::memmove( m_read_buf, &m_read_buf[ cur_position ], m_read_lenth - cur_position ) ;
				m_read_lenth -= cur_position ;
				frame_Len = 0 ;
			}
			else
			{
				ACE_OS::memcpy( m_receive_buf, &m_read_buf[ cur_position ], frame_Len ) ;  //����һ֡�������Ĵ�����

				//��m_read_buf��δ����ı�����λ
				ACE_OS::memmove( m_read_buf, &m_read_buf[ cur_position + frame_Len ], m_read_lenth - frame_Len - cur_position ) ;
				m_read_lenth = m_read_lenth - cur_position - frame_Len ;
			}
			if( m_read_lenth < 0 || m_read_lenth > REC_LEN )	//���Ļ��������ޣ�ȫ�����
			{
				m_read_lenth = 0;
				ACE_OS::memset(m_read_buf, 0, REC_LEN);	
			}
			ACE_OS::memset( &m_read_buf[ m_read_lenth ], 0, REC_LEN - m_read_lenth ) ;	//��λ������������ʼ��
			break ;
		}
		else if ( m_read_buf[ cur_position ] == ENCRY_FIRST && m_read_buf[ cur_position + 3 ] == ENCRY_FIRST )
		{
			frame_Len = m_read_buf[ cur_position + 1 ] + 6 ;

			if( ( cur_position + frame_Len ) > m_read_lenth )				//���������ı������´δ���	
			{
				ACE_OS::memmove( m_read_buf, &m_read_buf[ cur_position ], m_read_lenth - cur_position ) ;
				m_read_lenth -= cur_position ;
				frame_Len = 0 ;
			}
			else
			{
				ACE_OS::memcpy( m_receive_buf, &m_read_buf[ cur_position ], frame_Len ) ;  //����һ֡�������Ĵ�����

				//��m_read_buf��δ����ı�����λ
				ACE_OS::memmove( m_read_buf, &m_read_buf[ cur_position + frame_Len ], m_read_lenth - frame_Len - cur_position ) ;
				m_read_lenth = m_read_lenth - cur_position - frame_Len ;
			}
			if( m_read_lenth < 0 || m_read_lenth > REC_LEN )	//���Ļ��������ޣ�ȫ�����
			{
				m_read_lenth = 0;
				ACE_OS::memset(m_read_buf, 0, REC_LEN);	
			}
			ACE_OS::memset( &m_read_buf[ m_read_lenth ], 0, REC_LEN - m_read_lenth ) ;	//��λ������������ʼ��
			break ;
		}
	}
	if ( cur_position == m_read_lenth )	//�������������ݶ����ԣ�ȫ�����
	{
		m_read_lenth = 0 ;
		ACE_OS::memset( m_read_buf, 0, REC_LEN ) ;	
	}

	if ( frame_Len == 0 )		//���ı������´δ���
	{
		return frame_type ;
	}
	if ( m_receive_buf[ 0 ] == FIRST )
	{
		return judge_frame( ) ;
	}
	else if ( m_receive_buf[ 0 ] == ENCRY_FIRST )
	{
		return judge_encry_frame( ) ;
	}
	else
	{
		return frame_type ;
	}
}

Encry_Iec104::FRAME_TYPE Encry_Iec104::judge_frame( )
{
	FRAME_TYPE frame_type ;
	frame_type = INVALID_FRAME ;

	if ( m_receive_buf[ 1 ] < 4 )
	{
		return frame_type ;
	}

	P104_APDU *apdu_104 ;
	apdu_104=( P104_APDU* )&m_receive_buf[ 0 ] ;
//	cur_position += 2 ;  //�Ƶ������򲿷�

	switch ( m_receive_buf[ 2 ]&0x03 )
	{
	case 0x00:
	case 0x02:
		{
			m_v_recv++ ;		//���յ�I֡�󣬽��ռ������ۼ�
			frame_type = I_FRAME ;
		}
		break ;
	case 0x01:
		frame_type = S_FRAME ;
		break ;
	case 0X03:		//U_FRAME
		switch( apdu_104->apci.type_u.u_type ) //�����������ж�
		{
		case STARTDT_CON: //��վ����������Ӧ
			frame_type = U_FRAME_STARTDT ;
			break ;
		case STOPDT_CON:
			frame_type = U_FRAME_STOPDT ; //��վֹͣ������Ӧ
			break ;
		case TESTFR_ACT: //��վ���Ͳ���֡
			frame_type = U_FRAME_TESTFR ;
			break ;
		case TESTFR_CON: //��վ�յ���վ����֡��ȷ��
			frame_type = U_FRAME_TESTFR_ACK ;
			break ;
		default:
			break ;
		}
		break ;
	default:
		break ;
	} 
	return frame_type ;
}

Encry_Iec104::FRAME_TYPE Encry_Iec104::judge_encry_frame( void )
{
	FRAME_TYPE frame_type ;
	frame_type = INVALID_FRAME ;

	if ( m_receive_buf[ 1 ] < 6 )
	{
		return frame_type ;
	}
	else
	{
		frame_type = ENCRY_FRAME ;
		return frame_type ;
	}
}

int Encry_Iec104::deal_encry_msg()
{
	int ret = -1 ;

	printf( "received encrypt responsed msg \n" ) ;
	unsigned char bal_undecrypted[ SAFEZONELEN ] ;// �ԳƼ��� -�����ܻ�����
	unsigned char bal_decrypted[ SAFEZONELEN ] ;// �ԳƼ��� -���ܻ�����
	unsigned char unbal_undecrypted[ SAFEZONELEN ] ;// ���ԳƼ��� -�����ܻ�����
	unsigned char unbal_decrypted[ SAFEZONELEN ] ;// ���ԳƼ��� -���ܻ�����
	int bal_undecrypted_len = 0 ;
	unsigned int bal_decrypted_len = 0 ;
	int unbal_undecrypted_len = 0 ;
	unsigned int unbal_decrypted_len = 0 ;

	memset( bal_undecrypted, 0, SAFEZONELEN ) ;
	memset( bal_decrypted, 0, SAFEZONELEN ) ;
	memset( unbal_undecrypted, 0, SAFEZONELEN ) ;
	memset( unbal_decrypted, 0, SAFEZONELEN ) ;

	int msg_len = m_receive_buf[ 1 ] ;//��֤���ĳ���
	int sign_len = m_receive_buf[ 2 ] ;//ǩ�����ĳ���
	RTUPARA *pRtu = Fes_shm::get( ).rtu_para_ptr + m_rtu_no;
	unsigned char key_index = pRtu->encry_key_info.oper_key_no - 1 ;
	if( key_index < 0 || key_index > 3 )
	{
		printf( "keyNo invalid,keyNo = %d\n" , key_index + 1 ) ;
	}

	unsigned char sign_key_index = 0;//ǩ��ʹ�õĹ�Կ����
	unsigned char pub_key_index = 0;//���»���Ĺ�Կ����
	unsigned char cur_key_in_card = pRtu->encry_key_info.cur_key_no[ key_index ] ;
	unsigned char newkey_incard = pRtu->encry_key_info.new_key_no[ key_index ] ;

	if ( TYPENULL == m_encrypt_msg_type )
	{
		printf("TYPE_NULL,δ֪�ļ��ܱ���\n");
		return ret ;
	}
	else if ( CHECKKEY == m_encrypt_msg_type || UPDATEKEY == m_encrypt_msg_type )// ��վΪ���������Կ����
	{
		bal_undecrypted_len = CHECKBALLEN ;
		memcpy( bal_undecrypted , m_receive_buf + 4 , bal_undecrypted_len ) ;
		unbal_undecrypted_len = msg_len - bal_undecrypted_len ;
		memcpy( unbal_undecrypted , m_receive_buf + 4 + bal_undecrypted_len, unbal_undecrypted_len ) ;
	}
	else if ( YKOPER == m_encrypt_msg_type )
	{
		bal_undecrypted_len = YKBALLEN ;
		memcpy( bal_undecrypted , m_receive_buf + 4 , bal_undecrypted_len ) ;
	}

	unsigned char check_sum = m_receive_buf[ msg_len + 4 ] ; //֡У���CS
	encrypt_epri( bal_undecrypted , bal_undecrypted_len , bal_decrypted, &bal_decrypted_len , check_sum + 0xffffff00 , check_sum % 3 ) ;
	printf( "balance decrypt done\n" ) ;
	printf( "undecrypt len = %d, decrypt len = %d\n" , bal_undecrypted_len , bal_decrypted_len ) ;

	printf( "decrypt: " ) ;
	for ( int i = 0 ; i < bal_decrypted_len ; i++ )
	{
		printf( "%02X " , bal_decrypted[ i ] ) ;
	}
	printf( "\n" ) ;

	//������C
	if( ( bal_decrypted[ 0 ] & 0x40) == 0x40 )
	{
		return ret ;
	}
	if( ( bal_decrypted[ 0 ] & 0x1C ) >> 2 != m_encrypt_msg_type )
	{
		printf( "���ܺ��뷢�ͱ��Ĳ�ƥ��, %d, %d\n" , ( bal_decrypted[ 0 ] & 0x1C ) >> 2 , m_encrypt_msg_type ) ;
		return ret ;
	}

	sign_key_index = bal_decrypted[ 0 ] & 0x03;
	pub_key_index = bal_decrypted[ 1 ] & 0x03;

	unsigned char bAck = bal_decrypted[0] & 0x40 ;//Ӧ���ʶ

	printf( "ǩ��ʹ�õĹ�Կ��(%d)\n" , sign_key_index + 1 ) ;
	if( CHECKKEY == m_encrypt_msg_type )
	{
		printf( "���Ĺ�Կ��(%d)\n" , pub_key_index + 1 ) ;
		if (bAck)
		{
			printf( "Ӧ���ʶΪ1, ����\n  ����£�%d���Ź�Կ��\n" , sign_key_index + 1 ) ;
		}
	}
	else if ( UPDATEKEY == m_encrypt_msg_type )
	{
		printf( "���µĹ�Կ��(%d)\n ", pub_key_index + 1 ) ;
		if (bAck)
		{
			printf("Ӧ���ʶΪ1, ����\n  ���ȸ��£�%d���Ź�Կ��\n" , sign_key_index + 1);
		}
	}

	if( CHECKKEY == m_encrypt_msg_type )
	{
		if ( ( pub_key_index != pRtu->encry_key_info.oper_key_no - 1 ) )
		{
			printf( "���Ĺ�Կ���뷢��ʱ��һ��\n" ) ;
			return ret;
		}
	}
	else if ( UPDATEKEY == m_encrypt_msg_type )
	{
		if ( ( pub_key_index != pRtu->encry_key_info.oper_key_no - 1 ) )
		{
			printf( "���µĹ�Կ���뷢��ʱ��һ��\n" ) ;
			return ret ;
		}
	}

	int msg_rtu_addr = bal_decrypted[ 2 ] | bal_decrypted[ 3 ] << 8 | bal_decrypted[ 4 ] << 16 | bal_decrypted[ 5 ] << 24 ;
	if ( msg_rtu_addr != m_common_addr )
	{
		printf( "վ��ַ��һ��\n" ) ;
		return ret ;
	}

	time_t time_sec_tmp = 0;//seconds
	time_sec_tmp += (time_t)bal_decrypted[ 6 ];
	time_sec_tmp += (time_t)bal_decrypted[ 7 ] * 256;
	time_sec_tmp += (time_t)bal_decrypted[ 8 ] * 256 * 256;
	time_sec_tmp += (time_t)bal_decrypted[ 9 ] * 256 * 256 * 256;

	printf( "timesend = %d, timerecv = %d, timespan = %d\n" , m_send_encrypt_time , time_sec_tmp , abs( int( time_sec_tmp - m_send_encrypt_time ) ) ) ;
	if( abs( int( time_sec_tmp - m_send_encrypt_time ) ) > 60 )//�����ļ�Ҫ��ң�س�ʱ�ж�ʱ����Ϊ1����
	{
		printf( "��Ӧʱ�䳬ʱ\n" ) ;
		return ret ;
	}

	unsigned char calc_check_sum = 0 ;
	printf( "num= %02x %02x %02x %02x" , bal_decrypted[ 10 ] , bal_decrypted[ 11 ] , bal_decrypted[ 12 ] , bal_decrypted[ 13 ] ) ;

	if( YKOPER == m_encrypt_msg_type )	//ң�����б����޼��ܣ����ƿ���ȥ��
	{
		char str[NORMALCHAR_LEN];
		for ( int i = 0 ; i < bal_decrypted_len ; i++ )
		{
			calc_check_sum += bal_decrypted[ i ] ;
		}

		if( calc_check_sum != check_sum )
		{
			printf("У�����\n");
		}
		printf( "YKoper responsed OK! \n" ) ;
		printf( "balance encrypt check ok \n" ) ;
		ACE_OS::sprintf(str,"ң����ȷ!");
		send_yk_ret( m_rtu_no , 0 , 0 , str ) ;
	}
	else//���Գƽ���
	{
		SGD_RV sgd_ret = 0;
		SGD_HANDLE sessionHandle;

		if ( NULL == m_device_handle )
		{
			printf("encrypt device not open\n");
			return ret ;
		}
		printf("open session\n");
		sgd_ret = SDF_OpenSession( m_device_handle, &sessionHandle ) ;// �򿪻Ự

		if( sgd_ret != 0 )
		{
			printf( "session open fail err = %d\n", sgd_ret ) ;
			return ret ;
		}

		unsigned char keyno_incard = pRtu->encry_key_info.cur_key_no[ pub_key_index ];//signKeyIndex;//currentKeyNoIncard;
		if( UPDATEKEY == m_encrypt_msg_type )
		{
			keyno_incard = newkey_incard ;
		} 

		printf( "operKeyNo is (%d) ,keyNoIncard(S) is (%d)\n" , pRtu->encry_key_info.oper_key_no , keyno_incard ) ;
		sgd_ret = SDF_GetPrivateKeyAccessRight( sessionHandle , keyno_incard , PRI_KEYWORD , PASSWORDLEN ) ;

		if( sgd_ret != 0 )
		{
			printf( "get private key right fail err = %d", sgd_ret ) ;
			return ret ;
		}
		else
		{
			printf("get private key access right succeed\n");
		}

		ECCCipher ipher ;
		memset( &ipher , 0 , sizeof( ECCCipher ) ) ;
		memcpy( &ipher , unbal_undecrypted , sizeof( ECCCipher ) ) ;

		printf( "sizeof(ECCCipher) = %d \n" , sizeof( ECCCipher ) ) ;
		printf( "unbanance descrypt len =%d: \n" , unbal_undecrypted_len ) ;

		printf( "Internal decrypting...\n" ) ;
		unbal_decrypted_len = 0 ;
		sgd_ret = SDF_InternalDecrypt_ECC( sessionHandle , keyno_incard , SGD_SM2_1 , &ipher, unbal_decrypted , &unbal_decrypted_len ) ;

		printf( "unbal_decrypted_len = %d \n",unbal_decrypted_len ) ;
		if( sgd_ret != 0 )
		{
			printf( "�ڲ�˽Կ����ʧ�ܡ�Internal descrypt failed. err = %X\n" , sgd_ret ) ;
			printf("\n");
		}
		else
		{
			printf( "Internal decrypt succeed\n" ) ;
			printf( "random number is: %02X %02X %02X %02X\n" , unbal_decrypted[ 0 ] , unbal_decrypted[ 1 ] , unbal_decrypted[ 2 ] , unbal_decrypted[ 3 ] ) ;
			printf( "raw random number is: %02X %02X %02X %02X\n" , m_random_num[ 0 ] , m_random_num[ 1 ] , m_random_num[ 2 ] , m_random_num[ 3 ] ) ;
		}

		if ( memcmp( unbal_decrypted , m_random_num , 4 ) )
		{
			printf("ramdom num err\n");
		}

		sgd_ret = SDF_ReleasePrivateKeyAccessRight( sessionHandle , keyno_incard ) ;
		sgd_ret = SDF_CloseSession( sessionHandle ) ;

		if ( UPDATEKEY == m_encrypt_msg_type )
		{
			pRtu->encry_key_info.cur_key_no[ pRtu->encry_key_info.oper_key_no - 1 ] = pRtu->encry_key_info.new_key_no[ pRtu->encry_key_info.oper_key_no - 1 ] ;
			printf( "keyNo(%d),new keyNo in card is (%d) \n" , pRtu->encry_key_info.oper_key_no,pRtu->encry_key_info.cur_key_no[ pRtu->encry_key_info.oper_key_no - 1 ] ) ;
			FILE *wFile = NULL;
			char fileName[MAXFILENAMELEN];
			sprintf( fileName , "%s/log/dat/keyinf/%d.cfg" , ACE_OS::getenv( "NBENV" ) , m_channel_no ) ;
			wFile = fopen( fileName , "wb" ) ;
			printf("file name = %s !\n" , fileName);
			if (wFile)
			{
				fseek( wFile , 0L ,SEEK_SET ) ;
				fwrite( ( const void* )&pRtu->encry_key_info , sizeof( ENCRYPT_KEYNO ) , 1 , wFile ) ;
				fclose( wFile ) ;
				printf( "save new keyinf to file!\n" ) ;
			}
			else
			{
				printf( "open file to save failed!\n" ) ;
			}
		}
		else if ( CHECKKEY == m_encrypt_msg_type )
		{
			printf( "keyNo(%d),check succeed \n" , pRtu->encry_key_info.oper_key_no,pRtu->encry_key_info.cur_key_no[ pRtu->encry_key_info.oper_key_no - 1 ] ) ;
		}
	}

	m_encrypt_msg_type = TYPENULL ;
	ret = 0 ;
	return ret ;
}


int Encry_Iec104::deal_i_frame( void )
{
	int ret = 0 ;


	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;
	switch ( apdu_104->asdu.type )
	{
	case M_SP_NA_1:							//yx����
		ret = deal_sequence_M_SP_NA_1( ) ;
		break ;
	case M_DP_NA_1:
		ret = deal_sequence_M_DP_NA_1( ) ;
		break ;
	case M_PS_NA_1:
		ret = deal_sequence_M_PS_NA_1( ) ;			
		break ;
	case M_SP_TA_1:							//yxsoe����
		ret = deal_sequence_M_SP_TA_1( ) ;
		break ;
	case M_DP_TA_1:
		ret = deal_sequence_M_DP_TA_1( ) ;
		break ;
	case M_SP_TB_1:
		ret = deal_sequence_M_SP_TB_1( ) ;
		break ;
	case M_DP_TB_1:
		ret = deal_sequence_M_DP_TB_1( ) ;		
		break ;
	case M_ME_NA_1:							//yc����
	case M_ME_NB_1:
		ret = deal_sequence_M_ME_NA_1( ) ;
		break ;
	case M_ME_TA_1:
		ret = deal_sequence_M_ME_TA_1( ) ;
		break ;
	case M_ME_ND_1:
		ret = deal_sequence_M_ME_ND_1( ) ;
		break ;
	case M_ME_NC_1:
		ret = deal_sequence_M_ME_NC_1( ) ;			
		break ;
	case M_IT_NA_1:							//kwh����
		ret = deal_sequence_M_IT_NA_1( ) ;		
		break ;
	case M_IT_TA_1: //����
	case M_IT_TB_1:
		break ;
	case M_BO_NA_1:		//7:��վԶ���ն�״̬���� ����
		break ;
	case C_SC_NA_1:
	case C_DC_NA_1:
		{
			proc_yk_frame( ) ;			//yk����
		}
		break ;
	case C_RC_NA_1://47://������У
		//		A_DAResult( StaNo,ChNo,DataHead,comp ) ;
		break ;
	case C_IC_NA_1:		//����
		{
			ret = 0 ;
			if ( ( apdu_104->asdu.cot & 0x3f ) == COT_ACTTERM )
			{
				m_call_all_data_flag = NO_NEED ;		//���ٻ�����,ȷ�ϲ�������
			}
		}
		break ;
	case C_CI_NA_1:		//����ٻ�
		{
			ret = 0 ;
			if ( ( apdu_104->asdu.cot & 0x3f ) == COT_ACTTERM )
			{
				m_call_kwh_data_flag = NO_NEED ;		//����ٻ�����,ȷ�ϲ�������
			}
		}
		break ;
	case C_CS_NA_1:		//��ʱȷ��
		{
			ret = 0 ;
			if ( ( apdu_104->asdu.cot & 0x3f ) == COT_ACTCON )
			{
				m_chandata->sync_time_success = 1 ;
				m_sync_time_flag = NO_NEED ;		//��ʱ����
			}
		}
		break ;
	default:
		break ;
	} // end switch( apdu_104->asdu.type )

	if ( ret == -1 )
	{
		ACE_DEBUG( ( LM_DEBUG, "���ݴ������\n" ) ) ;
	}

	return ret ;
}

#pragma pack( 1 )
int Encry_Iec104::deal_s_frame( void )
{
	int ret = 0 ;

	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;

	int nrecv_count = apdu_104->apci.type_s.nr >> 1 ;
	
	if ( nrecv_count != m_v_send )		
	{
		ACE_DEBUG( ( LM_DEBUG, "������վS֡������������\n" ) ) ;		//�ݲ���S֡��������������ӡ��Ϣ,����ʽҲ�д�����
	}

	return ret ; //�޷���ֵ
}

int Encry_Iec104::send_s_frame( void )
{
	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;
	m_write_lenth = sizeof( S_FRAME_ACK ) ;
	ACE_OS::memcpy( m_write_buf, S_FRAME_ACK, m_write_lenth ) ;

	P104_APDU *apdu_104 = ( P104_APDU* )m_write_buf ;
	apdu_104->apci.type_s.nr = m_v_recv << 1 ; //�������кţ�2�ֽ�

	return send_msg( ) ;
}

#pragma pack( )

int Encry_Iec104::send_test_fr( void )
{
	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;
	m_write_lenth = sizeof( TEST_FR_VAL ) ;
	ACE_OS::memcpy( m_write_buf, TEST_FR_VAL, m_write_lenth ) ;
	return send_msg( ) ;

}

int Encry_Iec104::deal_test_fr( void )
{
	//������վ����֡������ȷ��֡
	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;
	m_write_lenth = sizeof( TEST_FR_ACK ) ;
	ACE_OS::memcpy( m_write_buf, TEST_FR_ACK, m_write_lenth ) ;
	return send_msg( ) ;
}

int Encry_Iec104::deal_test_ack( void )
{
	return 0 ;	//������վȷ��֡����������
}

int Encry_Iec104::deal_error_frame( void )
{
 	ACE_DEBUG( ( LM_DEBUG, "recv invaild frame!\n" ) ) ;
	return 0 ;
}

int Encry_Iec104::send_start_DT( void )
{
	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;
	m_write_lenth = sizeof( START_DT_VAL ) ;
	ACE_OS::memcpy( m_write_buf, START_DT_VAL, m_write_lenth ) ;

// 	ACE_DEBUG( ( LM_DEBUG, "send start DT!\n" ) ) ;
	return send_msg( ) ;
}

int Encry_Iec104::deal_start_dt( void )
{
	//������·�󣬽������ٵ�
	m_call_all_data_flag = NEED_CALL ;	
	m_call_kwh_data_flag = NEED_CALL ;	
	m_sync_time_flag = NEED_CALL ;

	return 0 ;
}

int Encry_Iec104::send_stop_DT( void )
{
	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;
	m_write_lenth = sizeof( STOP_DT_VAL ) ;
	ACE_OS::memcpy( m_write_buf, STOP_DT_VAL, m_write_lenth ) ;

	ACE_DEBUG( ( LM_DEBUG, "send stop dt!\n" ) ) ;
	return send_msg( ) ;
}

int Encry_Iec104::deal_stop_dt( void )
{
	return 0 ;
}

#pragma pack( 1 )
int Encry_Iec104::call_all_data( void )
{
	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;

	P104_APDU *apdu_104 = ( P104_APDU* )m_write_buf ;
	apdu_104->apci.start_id = FIRST ; 
	apdu_104->apci.apdu_len = VALUE ;
	apdu_104->apci.type_i.ns = m_v_send << 1 ;  //�������кţ�2�ֽ�
	apdu_104->apci.type_i.nr = m_v_recv << 1 ; //�������кţ�2�ֽ�
	apdu_104->asdu.type = C_IC_NA_1 ;
	apdu_104->asdu.vsq = VSQ_SINGLE ;
	apdu_104->asdu.cot = COT_ACT ;
	apdu_104->asdu.addr = m_common_addr ;

	C_STRUCT* pt = ( C_STRUCT* )apdu_104->asdu.data.buf ;
	pt->index_no = 0 ;
	pt->pos_holder = 0 ;
	pt->qdi = QDI_GENERAL_CALL ;
	m_write_lenth = apdu_104->apci.apdu_len + 2 ;
	
	m_call_all_data_flag = ON_PROCESS ;	

	m_v_send++ ; 
	return send_msg( ) ;
}

int Encry_Iec104::call_pi_data( void )
{
	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;

	P104_APDU *apdu_104 = ( P104_APDU* )m_write_buf ;
	apdu_104->apci.start_id = FIRST ; 
	apdu_104->apci.apdu_len = VALUE ;
	apdu_104->apci.type_i.ns = m_v_send << 1 ;  //�������кţ�2�ֽ�
	apdu_104->apci.type_i.nr = m_v_recv << 1 ; //�������кţ�2�ֽ�
	apdu_104->asdu.type = C_CI_NA_1 ;
	apdu_104->asdu.vsq = VSQ_SINGLE ;
	apdu_104->asdu.cot = COT_ACT ;
	apdu_104->asdu.addr = m_common_addr ;

	C_STRUCT* pt = ( C_STRUCT* )apdu_104->asdu.data.buf ;
	pt->index_no = 0 ;
	pt->pos_holder = 0 ;
	pt->qdi = 0 ;
	m_write_lenth = apdu_104->apci.apdu_len + 2 ;

	m_call_kwh_data_flag = ON_PROCESS ;

	m_v_send++ ; 
	return send_msg( ) ;
}

int Encry_Iec104::sync_time( void )
{
	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;

	P104_APDU *apdu_104 = ( P104_APDU* )m_write_buf ;
	apdu_104->apci.start_id = FIRST ; 
	apdu_104->apci.apdu_len = SYNC_VALUE ;
	apdu_104->apci.type_i.ns = m_v_send << 1 ;  //�������кţ�2�ֽ�
	apdu_104->apci.type_i.nr = m_v_recv << 1 ; //�������кţ�2�ֽ�
	apdu_104->asdu.type = C_CS_NA_1 ;
	apdu_104->asdu.vsq = VSQ_SINGLE ;
	apdu_104->asdu.cot = COT_ACT ;
	apdu_104->asdu.addr = m_common_addr ;

	SYNC_TIME_STRUCT* pt = ( SYNC_TIME_STRUCT* )apdu_104->asdu.data.buf ;
	pt->index_no = 0 ;
	pt->pos_holder = 0 ;

	FETIME tim ;
	get_time( &tim ) ;

	pt->cp56time.minute = tim.minute ;
	pt->cp56time.hour = tim.hour ;
	pt->cp56time.day = tim.day ;
	pt->cp56time.month = tim.month ;
	pt->cp56time.year = ( ( ( tim.year + 1900 )-2000 )&0xff ) ;
	pt->cp56time.millsecond = tim.milisecond + tim.second*1000 ;

	m_write_lenth = apdu_104->apci.apdu_len + 2 ;

	m_v_send++ ; 

	m_sync_time_flag = ON_PROCESS ;

	m_chandata->last_sync_time = ACE_OS::time( NULL ) ;
	m_chandata->sync_time_success = 0 ;

	return send_msg( ) ;
}

int Encry_Iec104::send_yk_frame( unsigned char ykno, bool bclosed, unsigned char yk_flag )
{
	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;

	P104_APDU *apdu_104 = ( P104_APDU* )m_write_buf ;
	apdu_104->apci.start_id = FIRST ; 
	apdu_104->apci.apdu_len = YK_FRAME_LENTH ;
	apdu_104->apci.type_i.ns = m_v_send << 1 ;  //�������кţ�2�ֽ�
	apdu_104->apci.type_i.nr = m_v_recv << 1 ; //�������кţ�2�ֽ�
	apdu_104->asdu.type = m_config_ptr->yk_type ;
	apdu_104->asdu.vsq = VSQ_SINGLE ;
	apdu_104->asdu.cot = ( yk_flag == 2 ) ? COT_DEACT : COT_ACT ;		//�Ƿ�ȡ��

	apdu_104->asdu.addr = m_common_addr ;

	C_STRUCT* pt = ( C_STRUCT* )apdu_104->asdu.data.buf ;
	//todo �Ƿ���Ҫ�ж�ң�غ�
	pt->index_no = m_config_ptr->yk_begin + ykno ;
	pt->pos_holder = 0 ;
	
	//�غϿط�,�ɵ����˫��ң��ȷ��
	if ( m_config_ptr->yk_type == C_SC_NA_1 )		//����ң��
	{
		if( bclosed )
			pt->qdi = STATE_SP_ON ;
		else
			pt->qdi = STATE_SP_OFF ;
	}
	else if ( m_config_ptr->yk_type == C_DC_NA_1 )	//˫��ң��
	{
		if( bclosed )
			pt->qdi = STATE_DP_ON ;
		else
			pt->qdi = STATE_DP_OFF ;
	}

	//ѡ���ִ��
	if ( yk_flag == 0 )
	{
		pt->qdi |= 0x80 ;
	}

	m_current_ykno = ykno ;				//�洢��ǰ�·�ң����ţ��Ա�У��
	m_current_ykoder = pt->qdi ;			//�洢��ǰ�·�ң������Ա�У��

	m_write_lenth = apdu_104->apci.apdu_len + 2 ;

	m_v_send++ ;

	add_yk_encry_data() ;

	return send_msg( ) ;
}

int Encry_Iec104::send_yt_frame( int rtuno, unsigned char ykno, bool bclosed, unsigned char yk_flag )
{
	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;

	P104_APDU *apdu_104 = ( P104_APDU* )m_write_buf ;
	apdu_104->apci.start_id = FIRST ; 
	apdu_104->apci.apdu_len = YK_FRAME_LENTH ;
	apdu_104->apci.type_i.ns = m_v_send << 1 ;  //�������кţ�2�ֽ�
	apdu_104->apci.type_i.nr = m_v_recv << 1 ; //�������кţ�2�ֽ�
	apdu_104->asdu.type = C_RC_NA_1 ;
	apdu_104->asdu.vsq = VSQ_SINGLE ;
	apdu_104->asdu.cot = ( yk_flag == 3 ) ? COT_DEACT : COT_ACT ;		//�Ƿ�ȡ��

	if ( rtuno != m_common_addr )		//�ж��·���ַ�Ƿ����
	{
		ACE_ERROR( ( LM_ERROR,"rtuno error!" ) ) ;
		return 0 ;
	}
	apdu_104->asdu.addr = rtuno ;

	C_STRUCT* pt = ( C_STRUCT* )apdu_104->asdu.data.buf ;
	//todo �Ƿ���Ҫ�ж�ң�غ�
	pt->index_no = m_config_ptr->yk_begin + ykno ;
	pt->pos_holder = 0 ;

	//�غϿط�,�ɵ����˫��ң��ȷ��
	if ( m_config_ptr->yk_type == C_SC_NA_1 )		//����ң��
	{
		if( bclosed )
			pt->qdi = STATE_SP_ON ;
		else
			pt->qdi = STATE_SP_OFF ;
	}
	else if ( m_config_ptr->yk_type == C_DC_NA_1 )	//˫��ң��
	{
		if( bclosed )
			pt->qdi = STATE_DP_ON ;
		else
			pt->qdi = STATE_DP_OFF ;
	}

	//ѡ���ִ��
	if ( yk_flag == 0 )
	{
		pt->qdi |= 0x80 ;
	}

	m_current_ykoder = pt->qdi ;			//�洢��ǰ�·�ң������Ա�У��
	m_write_lenth = apdu_104->apci.apdu_len + 2 ;

	m_v_send++ ;

	add_yk_encry_data() ;

	return send_msg( ) ;
}

#pragma pack( )

#pragma pack( 1 )

//���º���ΪI֡�������͵Ĵ���///////////////////////////////////////////////////////////////////////////////////////

//YX���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Encry_Iec104::deal_sequence_M_SP_NA_1( void )
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;
	if( apdu_104->asdu.addr != m_common_addr )		//�жϹ�����ַ
	{
		return -1 ;
	}
	int info_num = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0 ;		//ң�ŵ��

	if ( apdu_104->asdu.vsq & 0x80 )		//ֻ��һ����Ϣ���ַ
	{

		INFO_ADDR *pt_addr = ( INFO_ADDR* )( apdu_104->asdu.data.buf ) ;
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin ;

		unsigned char* pt = ( unsigned char* )( apdu_104->asdu.data.buf + sizeof( INFO_ADDR ) ) ;
		int i ;
		for ( i = 0 ; i < info_num ; i++ )
		{
			make_yx_real_data( data_no, *pt, 1 ) ;
			data_no++ ;
			pt++ ;
		}		
	}
	else			//ÿ֡����Ϣ���ַ
	{
		SEQ_M_NA_1* yx_struct = ( SEQ_M_NA_1* )( apdu_104->asdu.data.buf ) ;
		int i ;
		for ( i = 0 ; i < info_num ; i++ )
		{
			data_no = yx_struct->addr1 - m_config_ptr->yx_begin ;
			make_yx_real_data( data_no, yx_struct->yx_value, 1 ) ;
			yx_struct++ ;
		}		
	}
	
	return 0 ;
}

int Encry_Iec104::deal_sequence_M_DP_NA_1( void )
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;
	if( apdu_104->asdu.addr != m_common_addr )		//�жϹ�����ַ
	{
		return -1 ;
	}
	int info_num = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0 ;		//ң�ŵ��
	unsigned char value = 0 ;

	if ( apdu_104->asdu.vsq & 0x80 )		//ֻ��һ����Ϣ���ַ
	{

		INFO_ADDR *pt_addr = ( INFO_ADDR* )( apdu_104->asdu.data.buf ) ;
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin ;
	
		unsigned char* pt = ( unsigned char* )( apdu_104->asdu.data.buf + sizeof( INFO_ADDR ) ) ;
		int i ;
		for ( i = 0 ; i < info_num ; i++ )
		{
			//˫��ң�ż���״̬
			value = *pt & 0x03 ;			//����״̬����
			if ( value == 1 )
			{
				value = 0 ;	//״̬��
			}
			else if ( value == 2 )
			{
				value = 1 ;	//״̬��
			}
			else
			{
				value = 2 ;	//״̬��Ч
			}
			make_yx_real_data( data_no, value, 1 ) ;
			data_no++ ;
			pt++ ;
		}		
	}
	else			//ÿ֡����Ϣ���ַ
	{
		SEQ_M_NA_1* yx_struct = ( SEQ_M_NA_1* )( apdu_104->asdu.data.buf ) ;
		int i ;
		for ( i = 0 ; i < info_num ; i++ )
		{ 
			data_no = yx_struct->addr1 - m_config_ptr->yx_begin ;
		
			//˫��ң�ż���״̬
			value = yx_struct->yx_value & 0x03 ;			//����״̬����
			if ( value == 1 )
			{
				value = 0 ;	//״̬��
			}
			else if ( value == 2 )
			{
				value = 1 ;	//״̬��
			}
			else
			{
				value = 2 ;	//״̬��Ч
			}
			make_yx_real_data( data_no, value, 1 ) ;

			yx_struct++ ;
		}		
	}

	return 0 ;
}

int Encry_Iec104::deal_sequence_M_PS_NA_1( void )			//need for check �ô���ʽ�Բ���ȫ������֤
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;
	if( apdu_104->asdu.addr != m_common_addr )		//�жϹ�����ַ
	{
		return -1 ;
	}
	int info_num = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0 ;
	unsigned char value = 0 ;

	if ( apdu_104->asdu.vsq & 0x80 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = ( INFO_ADDR* )( apdu_104->asdu.data.buf ) ;
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin ;

		SEQ_M_PS_NA_1* yx_struct = ( SEQ_M_PS_NA_1* )( apdu_104->asdu.data.buf + sizeof( INFO_ADDR ) ) ;
		for ( int j = 0 ; j< info_num ; j++ )		//��Ϣ��ѭ��
		{
			for ( int i=0 ; i<16 ; i++ )		//16��Ϊһ��
			{
				if ( ( yx_struct->statuscheck>>i ) &0x01 )		//״̬λ���ټ����һ��״̬�仯
				{
					data_no ++ ;
					if ( data_no > m_chanpara->yx_num )		//yxnum����
					{
						break ;
					}
					value = ( yx_struct->value>>i ) & 0x01 ;

					make_yx_real_data( data_no, value, yx_struct->quanity ) ;
				}
				else
				{
					ACE_DEBUG( ( LM_DEBUG,"yxno = %d yx has no change", data_no ) ) ;
				}
			}	//�������һ��
			yx_struct ++ ;	
		}

	}
	else
	{
		struct SEQ_M_PS_NA_1_B
		{
			INFO_ADDR addr ;
			SEQ_M_PS_NA_1 seq_data ;
		} ;

		SEQ_M_PS_NA_1_B* yx_struct = ( SEQ_M_PS_NA_1_B* )( apdu_104->asdu.data.buf ) ;

		for ( int j = 0 ; j< info_num ; j++ )
		{
			data_no = yx_struct->addr.addr1 - m_config_ptr->yx_begin ;

			for ( int i=0 ; i<16 ; i++ )		//16��Ϊһ��
			{
				if ( ( yx_struct->seq_data.statuscheck>>i ) &0x01 )		//״̬λ���ټ����һ��״̬�仯
				{
					data_no ++ ;
					if ( data_no > m_chanpara->yx_num )		//yxnum����
					{
						break ;
					}
					value = ( yx_struct->seq_data.value>>i ) & 0x01 ;

					make_yx_real_data( data_no, value, yx_struct->seq_data.quanity ) ;

				}
				else
				{
					ACE_DEBUG( ( LM_DEBUG,"yxno = %d yx has no change", data_no ) ) ;
				}
			}		
		}
	}

	return 0 ;

}
//YX���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//YXSOE���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Encry_Iec104::deal_sequence_M_SP_TA_1( )		//M_SP_TA_1
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;
	int info_num = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0 ;		//ң�ŵ��
	FETIME soe_time ;
	SEQ_M_TA_1* seq_struct = ( SEQ_M_TA_1* )( apdu_104->asdu.data.buf ) ;

	for ( int i = 0 ; i < info_num ; i++ )
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin ; //����yx���
		soe_time = T101Time_To_Realtime( seq_struct->short_time ) ;	

// 		make_yx_real_data( data_no, seq_struct->value, 1, &soe_time ) ;
		//make event
		make_event( soe_time, data_no, seq_struct->value ) ;

		seq_struct++ ;
	}

	return 0 ;
}

int Encry_Iec104::deal_sequence_M_DP_TA_1( )		//M_DP_TA_1
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;
	int info_num = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0 ;		//ң�ŵ��
	unsigned char value = 0 ;

	FETIME soe_time ;
	SEQ_M_TA_1* seq_struct = ( SEQ_M_TA_1* )( apdu_104->asdu.data.buf ) ;

	for ( int i = 0 ; i < info_num ; i++ )
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin ; //����yx���
		soe_time = T101Time_To_Realtime( seq_struct->short_time ) ;	
	
		//˫��ң�ż���״̬
		value = seq_struct->value & 0x03 ;
		if ( value == 1 )
		{
			value = 0 ;	//״̬��
		}
		else if ( value == 2 )
		{
			value = 1 ;	//״̬��
		}
		else
		{
			value = 2 ;	//״̬��Ч
		}

// 		make_yx_real_data( data_no, seq_struct->value, 1, &soe_time ) ;
		//make event
		make_event( soe_time, data_no, seq_struct->value ) ;

		seq_struct++ ;
	}


	return 0 ;
}


int Encry_Iec104::deal_sequence_M_SP_TB_1( )	//M_SP_TB_1
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;
	int info_num = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0 ;		//ң�ŵ��
	FETIME soe_time ;
	SEQ_M_TB_1* seq_struct = ( SEQ_M_TB_1* )( apdu_104->asdu.data.buf ) ;

	for ( int i = 0 ; i < info_num ; i++ )
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin ; //����yx���

		//ʱ��ת��ΪFETIME
		soe_time.year = seq_struct->long_time.year + 100 ;
		soe_time.month = seq_struct->long_time.month ;
		soe_time.day = seq_struct->long_time.day ;
		soe_time.hour = seq_struct->long_time.hour ;
		soe_time.minute = seq_struct->long_time.minute ;
		soe_time.second = seq_struct->long_time.millsecond / 1000 ;
		soe_time.milisecond = seq_struct->long_time.millsecond %1000 ;


		//make event
		make_event( soe_time, data_no, seq_struct->value ) ;
// 		make_yx_real_data( data_no, seq_struct->value, 1, &soe_time ) ;

		seq_struct++ ;
	}

	return 0 ;
}

int Encry_Iec104::deal_sequence_M_DP_TB_1( )	//M_DP_TB_1
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;
	int info_num = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0 ;		//ң�ŵ��
	unsigned char value = 0 ;
	FETIME soe_time ;
	SEQ_M_TB_1* seq_struct = ( SEQ_M_TB_1* )( apdu_104->asdu.data.buf ) ;

	for ( int i = 0 ; i < info_num ; i++ )
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin ; //����yx���
		//ʱ��ת��ΪFETIME
		soe_time.year = seq_struct->long_time.year + 100 ;
		soe_time.month = seq_struct->long_time.month ;
		soe_time.day = seq_struct->long_time.day ;
		soe_time.hour = seq_struct->long_time.hour ;
		soe_time.minute = seq_struct->long_time.minute ;
		soe_time.second = seq_struct->long_time.millsecond / 1000 ;
		soe_time.milisecond = seq_struct->long_time.millsecond %1000 ;

		//˫��ң�ż���״̬
		value = seq_struct->value & 0x03 ;
		if ( value == 1 )
		{
			value = 0 ;	//״̬��
		}
		else if ( value == 2 )
		{
			value = 1 ;	//״̬��
		}
		else
		{
			value = 2 ;	//״̬��Ч
		}

// 		make_yx_real_data( data_no, seq_struct->value, 1, &soe_time ) ;

		//make event
		make_event( soe_time, data_no, seq_struct->value ) ;


		seq_struct++ ;
	}

		return 0 ;
}
//YXSOE���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//YC���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Encry_Iec104::deal_sequence_M_ME_NA_1( )		//9��11��ʶYC����
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;

	if( apdu_104->asdu.addr != m_common_addr )		//�жϹ�����ַ
	{
		return -1 ;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0 ;		//ң����

	if ( apdu_104->asdu.vsq & 0x80 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = ( INFO_ADDR* )( apdu_104->asdu.data.buf ) ;
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin ;//����ң����

		SEQ_M_ME_NA_1 * seq_data = ( SEQ_M_ME_NA_1* )( apdu_104->asdu.data.buf + sizeof( INFO_ADDR ) ) ;
		for ( int i =0 ; i< datanum ; ++i )
		{
			make_yc_real_data( data_no, seq_data->value, seq_data->quanity ) ;
			data_no++ ;		//ң������������
			seq_data ++ ;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		struct SEQ_M_ME_NA_1_B
		{
			INFO_ADDR addr ;
			SEQ_M_ME_NA_1 seq_data ;
		} ;

		int sizen = sizeof( SEQ_M_ME_NA_1_B ) ;
		SEQ_M_ME_NA_1_B * yc_struct = ( SEQ_M_ME_NA_1_B* )( apdu_104->asdu.data.buf ) ;
		for ( int i =0 ; i< datanum ; ++i )
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin ;//����ң����
			make_yc_real_data( data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity ) ;
			yc_struct ++ ;
		}

	}
	return 0 ;
}

int Encry_Iec104::deal_sequence_M_ME_TA_1( )	//10��ʱ��ң��ֵ
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;

	if( apdu_104->asdu.addr != m_common_addr )		//�жϹ�����ַ
	{
		return -1 ;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0 ;		//ң����
	FETIME time ;			//ң�ⷢ��ʱ��
	SEQ_M_ME_TA_1 * yc_struct = ( SEQ_M_ME_TA_1* )( apdu_104->asdu.data.buf ) ;
	for ( int i =0 ; i< datanum ; ++i )
	{
		time = T101Time_To_Realtime( yc_struct->short_time ) ;
		data_no = yc_struct->info_addr.addr1 - m_config_ptr->yc_begin ;
		make_yc_real_data( data_no, yc_struct->value, yc_struct->quanity, &time ) ;
		yc_struct++ ;
	}
	return 0 ;

}

int Encry_Iec104::deal_sequence_M_ME_NC_1( )		//13��ʶYC����
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;

	if( apdu_104->asdu.addr != m_common_addr )		//�жϹ�����ַ
	{
		return -1 ;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0 ;		//ң����

	if ( apdu_104->asdu.vsq & 0x80 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = ( INFO_ADDR* )( apdu_104->asdu.data.buf ) ;
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin ;//����ң����

		SEQ_M_ME_NC_1 * seq_data = ( SEQ_M_ME_NC_1* )( apdu_104->asdu.data.buf + sizeof( INFO_ADDR ) ) ;
		for ( int i =0 ; i< datanum ; ++i )
		{
			make_yc_real_data( data_no, seq_data->value, seq_data->quanity ) ;

			data_no++ ;		//ң������������
			seq_data ++ ;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		struct SEQ_M_ME_NC_1_B
		{
			INFO_ADDR addr ;
			SEQ_M_ME_NC_1 seq_data ;
		} ;

		SEQ_M_ME_NC_1_B * yc_struct = ( SEQ_M_ME_NC_1_B* )( apdu_104->asdu.data.buf ) ;
		for ( int i =0 ; i< datanum ; ++i )
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin ;//����ң����
			make_yc_real_data( data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity ) ;
			yc_struct ++ ;
		}

	}
	return 0 ;

}

int Encry_Iec104::deal_sequence_M_ME_ND_1( )		//21:��Ʒ������ң��ֵ
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;

	if( apdu_104->asdu.addr != m_common_addr )		//�жϹ�����ַ
	{
		return -1 ;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0 ;		//ң����

	if ( apdu_104->asdu.vsq & 0x80 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = ( INFO_ADDR* )( apdu_104->asdu.data.buf ) ;
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin ;//����ң����

		int16s* yc_data = ( int16s* )( apdu_104->asdu.data.buf + sizeof( INFO_ADDR ) ) ;
		for ( int i =0 ; i< datanum ; ++i )
		{
			make_yc_real_data( data_no, *yc_data, 1 ) ;
			data_no++ ;		//ң������������
			yc_data ++ ;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		struct SEQ_M_ME_ND_1_B
		{
			INFO_ADDR addr ;
			int16s yc_data ;
		} ;

		SEQ_M_ME_ND_1_B * yc_struct = ( SEQ_M_ME_ND_1_B* )( apdu_104->asdu.data.buf ) ;
		for ( int i =0 ; i< datanum ; ++i )
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin ;//����ң����
			make_yc_real_data( data_no, yc_struct->yc_data, 1 ) ;
			yc_struct ++ ;
		}

	}
	return 0 ;
}
//YC���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//KWH���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Encry_Iec104::deal_sequence_M_IT_NA_1( )		//0x15���
{
	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;

	if( apdu_104->asdu.addr != m_common_addr )		//�жϹ�����ַ
	{
		return -1 ;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F ; //��ȡ��ǰ֡�ĵ�����ݸ���
	int data_no = 0 ;		//ң�����

	if ( apdu_104->asdu.vsq & 0x80 )		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = ( INFO_ADDR* )( apdu_104->asdu.data.buf ) ;
		data_no = pt_addr->addr1 - m_config_ptr->kwh_begin ;//����ң�����

		SEQ_M_IT_NA_1 * seq_data = ( SEQ_M_IT_NA_1* )( apdu_104->asdu.data.buf + sizeof( INFO_ADDR ) ) ;
		for ( int i =0 ; i< datanum ; ++i )
		{
			make_kwh_real_data( data_no, seq_data->value, seq_data->quanity ) ;
			data_no++ ;		//ң�������������
			seq_data ++ ;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		struct SEQ_M_IT_NA_1_B
		{
			INFO_ADDR addr ;
			SEQ_M_IT_NA_1 seq_data ;
		} ;

		SEQ_M_IT_NA_1_B * kwh_struct = ( SEQ_M_IT_NA_1_B* )( apdu_104->asdu.data.buf ) ;
		for ( int i =0 ; i< datanum ; ++i )
		{
			data_no = kwh_struct->addr.addr1 - m_config_ptr->kwh_begin ;//����ң�����
			make_kwh_real_data( data_no, kwh_struct->seq_data.value, kwh_struct->seq_data.quanity ) ;
			kwh_struct ++ ;
		}

	}
	return 0 ;
}
//KWH���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//YK���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Encry_Iec104::proc_yk_frame( )
{
	char str[ NORMALCHAR_LEN ] ;


	P104_APDU *apdu_104 = ( P104_APDU* )m_receive_buf ;
	int yk_ret = -1 ;
	C_STRUCT * yk_struct = ( C_STRUCT* )( apdu_104->asdu.data.buf ) ;
	int ret_ykno = yk_struct->index_no - m_config_ptr->yk_begin ;

	if( apdu_104->asdu.addr != m_common_addr )		//�жϹ�����ַ
	{
		ACE_OS::sprintf( str,"ң�ع�����ַ��,��ȷ��ַ:%d,��У��ַ:%d",m_common_addr,apdu_104->asdu.addr ) ;
		send_yk_ret( m_rtu_no, ret_ykno, yk_ret, str ) ;
		return yk_ret ;
	}

	if ( ret_ykno != m_current_ykno )	//ң�غų���
	{
		ACE_ERROR( ( LM_ERROR, "�·���ң�غźͷ�У��ң�غŲ�һ��!" ) ) ;
		send_yk_ret( m_rtu_no, ret_ykno, yk_ret, str ) ;
		return yk_ret ;
	}
	if ( ( apdu_104->asdu.cot & 0x3f ) != COT_ACTCON && ( apdu_104->asdu.cot & 0x3f ) != COT_DEACTCON )	//����ԭ���
	{
		ACE_OS::sprintf( str,"ң�ط�У�Ĵ���ԭ����Ч,cot = %02x",apdu_104->asdu.cot & 0x3f ) ;
		send_yk_ret( m_rtu_no, ret_ykno, yk_ret, str ) ;
		return yk_ret ;
	}
	if ( yk_struct->qdi != m_current_ykoder )	//ң���������
	{
		ACE_OS::sprintf( str,"ң�������,�·�����:%02x ��У����:%02x",m_current_ykoder,yk_struct->qdi ) ;
		send_yk_ret( m_rtu_no, ret_ykno, yk_ret, str ) ;
		return yk_ret ;
	}

	if( ( apdu_104->asdu.cot & 0x40 ) == 0 )		//�жϷ�У���
	{
		ACE_OS::sprintf( str,"ң�ط�У��ȷ!" ) ;
		yk_ret = 0 ;
		send_yk_ret( m_rtu_no, ret_ykno, yk_ret, str ) ;
	}
	else if( ( apdu_104->asdu.cot & 0x40 ) == 0x40 )
	{
		ACE_OS::sprintf( str,"ң�ط�У������!" ) ;
		send_yk_ret( m_rtu_no, ret_ykno, yk_ret, str ) ;
		return yk_ret ;
	}

	if ( !( yk_struct->qdi & 0x80 ) )		//ң��ִ�л�ȡ����ɺ�
	{
		ACE_DEBUG( ( LM_DEBUG, "ң�ض������!" ) ) ;
		m_current_ykoder = 0 ;	//���ң�غ�ң��״̬��λ
	}

	//������ȷ��Ϣ
	yk_ret = 0 ;
	return yk_ret ;
}

//YK���ʹ���end///////////////////////////////////////////////////////////////////////////////////////
#pragma pack( )

//////////////////////////////////////////////////////////////////////////
//����Լ�ϴ��Ķ�ʱ��ʱ�䣬ת��Ϊ��ʵʱ��
FETIME T101Time_To_Realtime( CP24Time2a& SoeTime )
{
	//�˺�����101��ʱ��ת����ʵ��ʱ�䣬��ʱ��ֻ����һСʱ�����Ͳſɼ���
	struct tm TimeMinus,TimeAdd,TimeNochange ;//�ֱ���Сʱ��1��Сʱ��1��Сʱ�����ʱ����������ʱ��
	struct tm *UseTime ;
	struct tm *CurTime ;
	FETIME Time ;
	time_t ltime,tmptime ;
	long Diff_Minus,Diff_Add,Diff_Nochange ;

	time( &ltime ) ;
	tmptime = ltime - 3600 ;
	CurTime = localtime( &tmptime ) ;
	//��Сʱ�ֱ��ȥ�����ϡ����䣬�Ա�Ƚ�
	TimeMinus.tm_year = CurTime->tm_year ;
	TimeMinus.tm_mon = CurTime->tm_mon ;
	TimeMinus.tm_mday = CurTime->tm_mday ;
	TimeMinus.tm_hour = CurTime->tm_hour ;
	TimeMinus.tm_min = SoeTime.minutes ;
	TimeMinus.tm_sec = SoeTime.millsecond / 1000 ;

	tmptime = ltime + 3600 ;
	CurTime = localtime( &tmptime ) ;
	TimeAdd.tm_year = CurTime->tm_year ;
	TimeAdd.tm_mon = CurTime->tm_mon ;
	TimeAdd.tm_mday = CurTime->tm_mday ;
	TimeAdd.tm_hour = CurTime->tm_hour ;
	TimeAdd.tm_min = SoeTime.minutes ;
	TimeAdd.tm_sec = SoeTime.millsecond / 1000 ;

	CurTime = localtime( &ltime ) ;
	TimeNochange.tm_year = CurTime->tm_year ;
	TimeNochange.tm_mon = CurTime->tm_mon ;
	TimeNochange.tm_mday = CurTime->tm_mday ;
	TimeNochange.tm_hour = CurTime->tm_hour ;
	TimeNochange.tm_min = SoeTime.minutes ;
	TimeNochange.tm_sec = SoeTime.millsecond / 1000 ;

	Diff_Minus = ( long )fabs( double( mktime( &TimeMinus ) - ltime ) ) ;
	Diff_Add = ( long )fabs( double( mktime( &TimeAdd ) - ltime ) ) ;
	Diff_Nochange = ( long )fabs( double( mktime( &TimeNochange ) - ltime ) ) ;

	//ѡȡ�뵱ǰʱ�������С��һ��ʱ��ֵ��ΪSOE��ʱ��
	if ( Diff_Minus<Diff_Add && Diff_Minus<Diff_Nochange )
	{
		UseTime = &TimeMinus ;
	}
	else if ( Diff_Add<Diff_Minus && Diff_Add<Diff_Nochange )
	{
		UseTime = &TimeAdd ;
	}
	else if( Diff_Nochange<Diff_Minus && Diff_Nochange<Diff_Add )
	{
		UseTime = &TimeNochange ;
	}
	else
	{
		UseTime = &TimeNochange ;
	}

	Time.milisecond = SoeTime.millsecond % 1000 ;
	Time.second = UseTime->tm_sec ;
	Time.minute = UseTime->tm_min ;
	Time.hour = UseTime->tm_hour ;
	Time.day = UseTime->tm_mday ;
	Time.month = UseTime->tm_mon+1 ;
	Time.year = UseTime->tm_year+1900 ;

	return Time ;
}

int Encry_Iec104::send_check_key_frame( void )
{
	int ret = -1  ;

	unsigned char un_encrypted[ SAFEZONELEN ] ;//������
	unsigned char encrypted[ SAFEZONELEN ] ; //����
	unsigned char safe_zone[ SAFEZONELEN ] ;//��ȫ��
	unsigned char signed_sigment[ SIGNLEN ] ;//ǩ��
	unsigned char unhashed_sigment[SAFEZONELEN];//��ǩ����-hashǰ
	unsigned char unsigned_sigment[ SAFEZONELEN ] ;//��ǩ����-hash��
	unsigned char *pUn_encrypt = un_encrypted;
	unsigned char check_sum = 0 ;
	unsigned int unencrypted_len = 0;
	unsigned int encrypted_len = 0;
	unsigned int head_len = 0;
	unsigned int safe_zone_len = 0;
	unsigned int unsigned_len = 0;
	unsigned int unhashed_len = 0;
	unsigned int signed_len = 0;
	SGD_RV sgd_ret = 0;

	printf( "\n" );
	printf( "check public key\n" ) ;

	RTUPARA *pRtu = Fes_shm::get( ).rtu_para_ptr + m_rtu_no;
	ECCrefPublicKey pubKey;
	unsigned char key_index = pRtu->encry_key_info.oper_key_no - 1 ;
	unsigned char check_keyno_incard = pRtu->encry_key_info.cur_key_no[ key_index ] ;

	printf( "keyNo(%d),keyNo in card is (%d)\n" , key_index + 1 , check_keyno_incard ) ;

	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;

	if( NULL == m_device_handle )
	{
		printf("encrypt device not open\n");
		return ret ;
	}

	SGD_HANDLE session_handle ;
	sgd_ret = SDF_OpenSession( m_device_handle , &session_handle ) ;
	if (sgd_ret != 0)
	{
		printf( "session open fail err = %X\n" , sgd_ret ) ;
		return ret ;
	}
	printf( "open session succeed\n" ) ;
	sgd_ret = SDF_ExportSignPublicKey_ECC( session_handle , check_keyno_incard , &pubKey ) ;
	if (sgd_ret != 0)
	{
		printf( "export public key fail err = %X\n" , sgd_ret ) ;
		return ret ;
	}
	printf( "export sign public key succeed\n" ) ;

	memset( un_encrypted , 0, SAFEZONELEN ) ;
	memset( encrypted , 0 , SAFEZONELEN ) ;
	memset( safe_zone , 0 , SAFEZONELEN ) ;
	memset( unsigned_sigment , 0 , SAFEZONELEN ) ;
	memset( signed_sigment , 0 , SIGNLEN ) ;
	memset( unhashed_sigment , 0 , SAFEZONELEN ) ;

	time_t curTime = time( NULL ) ;

	m_check_or_update_keyno = pRtu->encry_key_info.oper_key_no;//CHECKORUODATEKEY;
	m_encrypt_msg_type = CHECKKEY ;
	m_send_encrypt_time = curTime ;

	*pUn_encrypt++ = (0x003 << 2) | key_index;//encrypt104.LLD[ChNo].keyIndex;		//������
	*pUn_encrypt++ = key_index;//CHECKORUODATEKEY;								//����״̬S

	unsigned short rtu_addr = m_common_addr ;
	*pUn_encrypt++ = ( unsigned char )( rtu_addr & 0x00FF ) ;				//RTU��ַ
	*pUn_encrypt++ = ( unsigned char )( ( rtu_addr & 0xFF00) >> 8 ) ;
	*pUn_encrypt++ = 0x00 ;
	*pUn_encrypt++ = 0x00 ;

	*pUn_encrypt++ = ( unsigned char )(curTime & 0x000000FF ) ;		//ʱ���
	*pUn_encrypt++ = ( unsigned char )( ( curTime & 0x0000FF00 ) >> 8 ) ;
	*pUn_encrypt++ = ( unsigned char )( ( curTime & 0x00FF0000 ) >> 16 ) ;
	*pUn_encrypt++ = ( unsigned char )( ( curTime & 0xFF000000 ) >> 24 ) ;

	sgd_ret = SDF_GenerateRandom( session_handle , RANDOMLEN , m_random_num ) ;

	if (sgd_ret != 0)
	{
		printf( "generate random fail err = %d\n" , sgd_ret ) ;
		return ret ;
	}
	printf( "generate random number succeed\n" ) ;
	printf( "random number is: %02X %02X %02X %02X\n" , m_random_num[ 0 ] , m_random_num[ 1 ] , m_random_num[ 2 ] , m_random_num[ 3 ] ) ;

	*pUn_encrypt++ = m_random_num[ 0 ] ;
	*pUn_encrypt++ = m_random_num[ 1 ] ;
	*pUn_encrypt++ = m_random_num[ 2 ] ;
	*pUn_encrypt++ = m_random_num[ 3 ] ;

	unencrypted_len = CHECKBALLEN ;

	safe_zone[ 0 ] = ENCRY_FIRST ;
	safe_zone[ 1 ] = unencrypted_len + SIGNLEN;		//��ȫ����������	
	safe_zone[ 2 ] = SIGNLEN;						//ǩ������
	safe_zone[ 3 ] = ENCRY_FIRST ;

	head_len = 4;
	memcpy( safe_zone + head_len , un_encrypted , unencrypted_len ) ;

	unhashed_len = head_len + unencrypted_len ;
	memcpy( unhashed_sigment , safe_zone , unhashed_len ) ;

	printf( "SIGNID=%s\n" , m_104_sign_id ) ;
	printf( "SIGIDLEN=%d\n" , SIGNIDLEN ) ;
	sgd_ret = SDF_HashInit( session_handle , SGD_SM3 , &pubKey , m_104_sign_id , SIGNIDLEN ) ;
	if (sgd_ret != 0)
	{
		printf( "hash init fail err = %d", sgd_ret ) ;
		return ret ;
	}

	sgd_ret = SDF_HashUpdate( session_handle , unhashed_sigment , unhashed_len ) ;
	if (sgd_ret != 0)
	{
		printf( "hash update fail err = %d" , sgd_ret ) ;
		return ret ;
	}

	sgd_ret = SDF_HashFinal( session_handle , unsigned_sigment , &unsigned_len ) ;
	if (sgd_ret != 0)
	{
		printf( "hash final fail err = %d" , sgd_ret ) ;
		return ret ;
	}

	printf("hash succeed \n");

	sgd_ret = SDF_GetPrivateKeyAccessRight( session_handle , check_keyno_incard , PRI_KEYWORD , PASSWORDLEN ) ;

	if (sgd_ret != 0)
	{
		printf( "get private key right fail err = %d" , sgd_ret ) ;
		return ret ;
	}
	printf("get private key access right succeed\n");

	ECCSignature sig;
	sgd_ret = SDF_InternalSign_ECC( session_handle , check_keyno_incard , unsigned_sigment , unsigned_len , &sig ) ;
	memcpy( signed_sigment , &sig , sizeof( ECCSignature ) ) ;

	if (sgd_ret != 0)
	{
		printf( "sign fail err = %d", sgd_ret ) ;
		return ret ;
	}
	printf( "internal sign succeed\n" ) ;

	sgd_ret = SDF_ReleasePrivateKeyAccessRight( session_handle , check_keyno_incard ) ;
	sgd_ret = SDF_CloseSession( session_handle ) ;
	memcpy( safe_zone + head_len + unencrypted_len , signed_sigment , SIGNLEN ) ;

	for ( int i = 0 ; i < unencrypted_len + SIGNLEN ; i++ )
	{
		check_sum += safe_zone[ i + head_len ] ;
	}

	safe_zone[ head_len + unencrypted_len + SIGNLEN ] = check_sum;
	safe_zone[ head_len + unencrypted_len + SIGNLEN + 1 ] = ENCRY_FIRST ;

	encrypt_epri( un_encrypted , unencrypted_len , encrypted , &encrypted_len , check_sum + 0xffffff00 , check_sum % 3 ) ;
	memcpy( safe_zone + head_len , encrypted , encrypted_len ) ;

	safe_zone_len = head_len + encrypted_len + SIGNLEN + 2 ;

	memcpy( m_write_buf , safe_zone , safe_zone_len ) ;
	m_write_lenth = safe_zone_len;

	m_check_public_key = NO_NEED ;
	return send_msg() ;
}

int Encry_Iec104::send_update_key_frame( void )
{
	int ret = -1 ;

	unsigned char un_encrypted[ SAFEZONELEN ] ;//������
	unsigned char encrypted[ SAFEZONELEN ] ; //����
	unsigned char safe_zone[ SAFEZONELEN ] ;//��ȫ��
	unsigned char signed_sigment[ SIGNLEN ] ;//ǩ��
	unsigned char unhashed_sigment[SAFEZONELEN];//��ǩ����-hashǰ
	unsigned char unsigned_sigment[ SAFEZONELEN ] ;//��ǩ����-hash��
	unsigned char *pUn_encrypt = un_encrypted;
	unsigned char check_sum = 0 ;
	unsigned int unencrypted_len = 0;
	unsigned int encrypted_len = 0;
	unsigned int head_len = 0;
	unsigned int safe_zone_len = 0;
	unsigned int unsigned_len = 0;
	unsigned int unhashed_len = 0;
	unsigned int signed_len = 0;
	SGD_RV sgd_ret = 0;

	printf( "\n" );
	printf( "update public key\n" ) ;

	RTUPARA *pRtu = Fes_shm::get( ).rtu_para_ptr + m_rtu_no;

	ECCrefPublicKey sign_pub_key;
	ECCrefPublicKey new_pub_key;
	unsigned char key_index = pRtu->encry_key_info.oper_key_no - 1 ;
	if ( key_index < 0 || key_index > 3 )
	{
		printf( "keyNo invalid,keyNo = %d\n", key_index + 1) ;
	} 
	unsigned char signkey_index = (key_index + 3)%4;
	unsigned char sign_keyno_incard = pRtu->encry_key_info.cur_key_no[ signkey_index ] ;//ǩ����Կ
	unsigned char cur_keyno_incard = pRtu->encry_key_info.cur_key_no[ key_index ] ;//�����¹�Կ
	unsigned char new_keyno_incard = pRtu->encry_key_info.new_key_no[ key_index ] ;//�¹�Կ

	printf( "operkeyNo = %d,currentNo in card is (%d),newNo in card is (%d)\n" , pRtu->encry_key_info.oper_key_no , cur_keyno_incard , new_keyno_incard ) ;

	ACE_OS::memset( m_write_buf,0,REC_LEN ) ;

	if ( NULL == m_device_handle )
	{
		printf("encrypt device not open\n");
		return ret ;
	}

	SGD_HANDLE session_handle;
	sgd_ret = SDF_OpenSession( m_device_handle , &session_handle ) ;
	if (sgd_ret != 0)
	{
		printf( "session open fail err = %d\n" , sgd_ret ) ;
		return ret ;
	}

	sgd_ret = SDF_ExportSignPublicKey_ECC( session_handle , sign_keyno_incard , &sign_pub_key ) ;
	if (sgd_ret != 0)
	{
		printf( "export public key fail err = %X,signKeyNo = (%d),signkeyNoIncard = (%d)\n" , sgd_ret,signkey_index + 1 , sign_keyno_incard ) ;
		return ret ;
	}
	printf( "export sign public key succeed,signKeyNo = (%d),signkeyNoIncard = (%d)\n" , signkey_index + 1 , sign_keyno_incard ) ;

	m_encrypt_msg_type = UPDATEKEY ;

	memset( un_encrypted , 0, SAFEZONELEN ) ;
	memset( encrypted , 0 , SAFEZONELEN ) ;
	memset( safe_zone , 0 , SAFEZONELEN ) ;
	memset( unsigned_sigment , 0 , SAFEZONELEN ) ;
	memset( signed_sigment , 0 , SIGNLEN ) ;
	memset( unhashed_sigment , 0 , SAFEZONELEN ) ;

	time_t curTime = time(NULL);
	m_send_encrypt_time = curTime ;

	*pUn_encrypt++ = (0x004 << 2) | signkey_index;		//������
	*pUn_encrypt++ = key_index;			//����״̬S

	unsigned short rtu_addr = m_common_addr ;
	*pUn_encrypt++ = ( unsigned char )( rtu_addr & 0x00FF ) ;				//RTU��ַ
	*pUn_encrypt++ = ( unsigned char )( ( rtu_addr & 0xFF00) >> 8 ) ;
	*pUn_encrypt++ = 0x00 ;
	*pUn_encrypt++ = 0x00 ;

	*pUn_encrypt++ = ( unsigned char )(curTime & 0x000000FF ) ;		//ʱ���
	*pUn_encrypt++ = ( unsigned char )( ( curTime & 0x0000FF00 ) >> 8 ) ;
	*pUn_encrypt++ = ( unsigned char )( ( curTime & 0x00FF0000 ) >> 16 ) ;
	*pUn_encrypt++ = ( unsigned char )( ( curTime & 0xFF000000 ) >> 24 ) ;

	sgd_ret = SDF_GenerateRandom( session_handle , RANDOMLEN , m_random_num ) ;

	if (sgd_ret != 0)
	{
		printf( "generate random fail err = %d\n" , sgd_ret ) ;
		return ret ;
	}
	printf( "generate random number succeed\n" ) ;
	printf( "random number is: %02X %02X %02X %02X\n" , m_random_num[ 0 ] , m_random_num[ 1 ] , m_random_num[ 2 ] , m_random_num[ 3 ] ) ;

	*pUn_encrypt++ = m_random_num[ 0 ] ;
	*pUn_encrypt++ = m_random_num[ 1 ] ;
	*pUn_encrypt++ = m_random_num[ 2 ] ;
	*pUn_encrypt++ = m_random_num[ 3 ] ;

	sgd_ret = SDF_ExportSignPublicKey_ECC( session_handle , new_keyno_incard , &new_pub_key ) ;

	if ( sgd_ret != 0 )
	{
		printf( "export new public key fail err = %d,keyNoIncard=%d\n ", sgd_ret , new_keyno_incard ) ;
		return ret ;
	}

	memcpy( pUn_encrypt , &new_pub_key , sizeof( ECCrefPublicKey ) ) ;

	unencrypted_len = CHECKBALLEN + sizeof( ECCrefPublicKey ) ;

	safe_zone[ 0 ] = ENCRY_FIRST ;
	safe_zone[ 1 ] = unencrypted_len + SIGNLEN;		//��ȫ����������	
	safe_zone[ 2 ] = SIGNLEN;						//ǩ������
	safe_zone[ 3 ] = ENCRY_FIRST ;

	head_len = 4;
	memcpy( safe_zone + head_len , un_encrypted , unencrypted_len ) ;

	sgd_ret = SDF_HashInit( session_handle , SGD_SM3 , &sign_pub_key , m_104_sign_id , SIGNIDLEN ) ;
	if (sgd_ret != 0)
	{
		printf( "hash init fail err = %d", sgd_ret ) ;
		return ret ;
	}
	
	printf( "safe zone len = %d\n" , head_len + unencrypted_len ) ;

	unhashed_len = head_len + unencrypted_len;
	memcpy( unhashed_sigment , safe_zone , unhashed_len ) ;

	sgd_ret = SDF_HashUpdate( session_handle , unhashed_sigment , unhashed_len ) ;

	if (sgd_ret != 0)
	{
		printf("hash update fail err = %d", sgd_ret);
	}

	sgd_ret = SDF_HashFinal( session_handle , unsigned_sigment , &unsigned_len ) ;

	if (sgd_ret != 0)
	{
		printf( "hash final fail err = %d ", sgd_ret ) ;
	}
	else
	{
		printf( "hash succeed\n" ) ;
	}

	sgd_ret = SDF_GetPrivateKeyAccessRight( session_handle , sign_keyno_incard , PRI_KEYWORD , PASSWORDLEN ) ;

	if (sgd_ret != 0)
	{
		printf( "get private key right fail err = %d" , sgd_ret ) ;
	}

	ECCSignature sig;
	sgd_ret = SDF_InternalSign_ECC( session_handle , sign_keyno_incard , unsigned_sigment , unsigned_len , &sig ) ;
	
	memcpy( signed_sigment , &sig , sizeof( ECCSignature ) ) ;

	if ( sgd_ret != 0 )
	{
		printf( "sign fail err = %d" , sgd_ret ) ;
	}

	sgd_ret = SDF_ReleasePrivateKeyAccessRight( session_handle , sign_keyno_incard ) ;
	sgd_ret = SDF_CloseSession( session_handle ) ;

	memcpy( safe_zone + head_len + unencrypted_len , signed_sigment , SIGNLEN ) ;

	for( int i = 0 ; i < unencrypted_len + SIGNLEN ; i++ )
	{
		check_sum += safe_zone[ i + head_len ] ;
	}

	safe_zone[ head_len + unencrypted_len + SIGNLEN ] = check_sum ;
	safe_zone[ head_len + unencrypted_len + SIGNLEN + 1 ] = ENCRY_FIRST ;

	encrypt_epri( un_encrypted , unencrypted_len , encrypted , &encrypted_len , check_sum + 0xffffff00 , check_sum % 3 ) ;
	memcpy( safe_zone + head_len , encrypted , encrypted_len ) ;

	safe_zone_len = head_len + encrypted_len + SIGNLEN + 2 ;

	memcpy( m_write_buf , safe_zone , safe_zone_len ) ;
	m_write_lenth = safe_zone_len;

	m_refresh_public_key = NO_NEED ;

	return send_msg() ;
}

int Encry_Iec104::add_yk_encry_data( void )
{
	int ret = -1 ;

	unsigned char un_encrypted[ SAFEZONELEN ] ;//������
	unsigned char encrypted[ SAFEZONELEN ] ; //����
	unsigned char safe_zone[ SAFEZONELEN ] ;//��ȫ��
	unsigned char signed_sigment[ SIGNLEN ] ;//ǩ��
	unsigned char unhashed_sigment[SAFEZONELEN];//��ǩ����-hashǰ
	unsigned char unsigned_sigment[ SAFEZONELEN ] ;//��ǩ����-hash��
	unsigned char *pUn_encrypt = un_encrypted;
	unsigned char check_sum = 0 ;
	unsigned int unencrypted_len = 0;
	unsigned int encrypted_len = 0;
	unsigned int head_len = 0;
	unsigned int safe_zone_len = 0;
	unsigned int unsigned_len = 0;
	unsigned int unhashed_len = 0;
	unsigned int signed_len = 0;
	SGD_RV sgd_ret = 0;

	printf( "\n" );
	printf( "add yk encry data\n" ) ;

	RTUPARA *pRtuPara = Fes_shm::get( ).rtu_para_ptr + m_rtu_no;

	ECCrefPublicKey pub_key ;
	int key_index = pRtuPara->encry_key_info.oper_key_no % 4;
	int keyno_incard = pRtuPara->encry_key_info.cur_key_no[ key_index ] ;

	m_encrypt_msg_type = YKOPER ;

	if ( NULL == m_device_handle )
	{
		printf( "encrypt device not open\n" ) ;
		return ret ;
	}

	SGD_HANDLE session_handle;
	sgd_ret = SDF_OpenSession( m_device_handle , &session_handle ) ;
	if (sgd_ret != 0)
	{
		printf( "session open fail err = %d\n" , sgd_ret ) ;
		return ret ;
	}

	sgd_ret = SDF_ExportSignPublicKey_ECC( session_handle , keyno_incard , &pub_key ) ;
	if (sgd_ret != 0)
	{
		printf( "export public key fail err = %X,signKeyNo = (%d),signkeyNoIncard = (%d)\n" , sgd_ret,keyno_incard + 1 , keyno_incard ) ;
		return ret ;
	}
	printf( "export sign public key succeed,signKeyNo = (%d),signkeyNoIncard = (%d)\n" , keyno_incard + 1 , keyno_incard ) ;

	m_encrypt_msg_type = UPDATEKEY ;

	memset( un_encrypted , 0, SAFEZONELEN ) ;
	memset( encrypted , 0 , SAFEZONELEN ) ;
	memset( safe_zone , 0 , SAFEZONELEN ) ;
	memset( unsigned_sigment , 0 , SAFEZONELEN ) ;
	memset( signed_sigment , 0 , SIGNLEN ) ;
	memset( unhashed_sigment , 0 , SAFEZONELEN ) ;

	time_t curTime = time(NULL);
	m_send_encrypt_time = curTime ;

	*pUn_encrypt++ = (0x001 << 2) | key_index;//encrypt104.LLD[ChNo].keyIndex;		//������
	*pUn_encrypt++ = 0x00;											//����״̬S

	unsigned short rtu_addr = m_common_addr ;
	*pUn_encrypt++ = ( unsigned char )( rtu_addr & 0x00FF ) ;				//RTU��ַ
	*pUn_encrypt++ = ( unsigned char )( ( rtu_addr & 0xFF00) >> 8 ) ;
	*pUn_encrypt++ = 0x00 ;
	*pUn_encrypt++ = 0x00 ;

	*pUn_encrypt++ = ( unsigned char )(curTime & 0x000000FF ) ;		//ʱ���
	*pUn_encrypt++ = ( unsigned char )( ( curTime & 0x0000FF00 ) >> 8 ) ;
	*pUn_encrypt++ = ( unsigned char )( ( curTime & 0x00FF0000 ) >> 16 ) ;
	*pUn_encrypt++ = ( unsigned char )( ( curTime & 0xFF000000 ) >> 24 ) ;

	sgd_ret = SDF_GenerateRandom( session_handle , RANDOMLEN , m_random_num ) ;

	unencrypted_len = 10;

	safe_zone[ 0 ] = ENCRY_FIRST ;
	safe_zone[ 1 ] = unencrypted_len + SIGNLEN;		//��ȫ���������� L2	
	safe_zone[ 2 ] = SIGNLEN;						//ǩ������ L3
	safe_zone[ 3 ] = ENCRY_FIRST ;

	head_len = 4;

	memcpy( safe_zone + head_len , un_encrypted , unencrypted_len ) ;

	/************************************************************************/
	printf( "safe zone len = %d\n" , head_len + unencrypted_len ) ;

	for( int i = 0 ; i < safe_zone_len + head_len + unencrypted_len ; i++ )
	{
		printf( "%02X " , safe_zone[ i ] ) ;
	}
	printf("\n");

	/************************************************************************/

	memcpy( unhashed_sigment, m_write_buf , m_write_lenth ) ;	//����ң�ر���׼��ǩ����
	memcpy( unhashed_sigment + m_write_lenth , safe_zone , unencrypted_len + head_len ) ;	//������ȫ��������׼��ǩ����

	/************************************************************************/
	printf( "unsignedSigment len = %d\n", head_len + unencrypted_len + m_write_lenth ) ;

	for( int i = 0 ; i < head_len + unencrypted_len + m_write_lenth ; i++ )
	{
		printf( "%02X " , unhashed_sigment[ i ] ) ;
	}

	printf("\n");

	/************************************************************************/

	printf("hash init\n");
	sgd_ret = SDF_HashInit( session_handle,  SGD_SM3, &pub_key , m_104_sign_id , SIGNIDLEN ) ;

	for( int i = 0 ; i < SIGNLEN ; i++ )
	{
		printf( "%c" , m_104_sign_id[ i ] ) ;
	}
	printf("\n");

	if (sgd_ret != 0)
	{
		printf( "hash init fail err = %d" , sgd_ret ) ;
		return ret ;
	}

	printf("hash update\n");
	sgd_ret = SDF_HashUpdate( session_handle , unhashed_sigment , m_write_lenth + unencrypted_len + head_len ) ;

	if (sgd_ret != 0)
	{
		printf( "hash update fail err = %d" , sgd_ret ) ;
		return ret ;
	}

	printf("hash final\n");
	sgd_ret = SDF_HashFinal( session_handle , unsigned_sigment , &unsigned_len ) ;

	printf( "hash len = %d " , unsigned_len ) ;
	for( int i = 0 ; i < unsigned_len ; i++ )
	{
		printf( "%02X " , unsigned_sigment[ i ] ) ;
	}
	printf("\n");


	if (sgd_ret != 0)
	{
		printf( "hash final fail err = %d" , sgd_ret ) ;
		return ret ;
	}

	printf( "get private key access right\n" ) ;
	sgd_ret = SDF_GetPrivateKeyAccessRight( session_handle , keyno_incard , PRI_KEYWORD , PASSWORDLEN ) ;

	if (sgd_ret != 0)
	{
		printf( "get private key right fail err = %d" , sgd_ret ) ;
		return ret ;
	}

	ECCSignature sig ;

	printf("internal sign\n");
	sgd_ret = SDF_InternalSign_ECC( session_handle , keyno_incard, unsigned_sigment , unsigned_len , &sig ) ;

	printf("internal sign ret = %X\n", sgd_ret);
	memcpy( signed_sigment , &sig , sizeof( ECCSignature ) ) ;

	/************************************************************************/
	printf("signedSigment: ");
	for( int i = 0 ; i < sizeof( ECCSignature ) ; i++ )
	{
		printf( "%02X " , signed_sigment[ i ] ) ;
	}
	printf("\n");
	/************************************************************************/

	if (sgd_ret != 0)
	{
		printf( "sign fail err = %d" , sgd_ret ) ;
		return ret ;
	}

	printf("release private key access right\n");
	sgd_ret = SDF_ReleasePrivateKeyAccessRight( session_handle , keyno_incard ) ;

	if (sgd_ret != 0)
	{
		printf("release private key right fail err = %d", sgd_ret);
		return ret ;
	}

	SDF_CloseSession( session_handle ) ;

	memcpy( safe_zone + head_len + unencrypted_len , signed_sigment , SIGNLEN ) ;

	for( int i = 0 ; i < unencrypted_len + SIGNLEN ; i++ )
	{
		check_sum += safe_zone[ i + head_len ] ;
	}

	printf( "checksum = %02X\n" , check_sum ) ;

	safe_zone[ head_len + unencrypted_len + SIGNLEN ] = check_sum ;
	safe_zone[ head_len + unencrypted_len + SIGNLEN + 1 ] = ENCRY_FIRST ;

	// �ԳƼ��� ��һ��һ��
	printf("balance encrypt\n");
	encrypt_epri( un_encrypted , unencrypted_len , encrypted , &encrypted_len , check_sum + 0xffffff00 , check_sum % 3 ) ;
	printf("encryptLen = %d\n", encrypted_len);

	for( int i = 0 ; i < encrypted_len ; i++ )
	{
		printf( "%02X ", encrypted[ i ] ) ;
	}
	printf("\n");
	/************************************************************************/

	memcpy( safe_zone + head_len, encrypted, encrypted_len ) ;

	safe_zone_len = head_len + encrypted_len + SIGNLEN + 2 ;

	memcpy( m_write_buf + m_write_lenth , safe_zone , safe_zone_len ) ;
	m_write_lenth += safe_zone_len;
}
