/**
@file iec104.cpp
@brief 104��Լ����Դ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-08-30
*/

#include "iec104.h"

#include "fes/timer.h"
#include "fes/fesshm.h"
#include "fes/platformwrapper.h"
#include "fes/fesdatashm_allocator.h"
#include "fes/femailstruct.h"
#include "fes/fertfile.h"

#include <ace/OS.h>
#include <ace/Log_Msg.h>
#include <ace/Message_Block.h>
#include <ace/Time_Value.h>
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;

const char* const version_string = "1.0.0";
const char* const desc_string = "the very beginning";

//���Ӳ��ɹ��󣬵ȴ�2���ٳ�������
const int CONN_RETRY_TIME = 2;
const int SECOND_LOOP_TIME = 1;

/**
@brief IEC104������
*/
static IEC104_CONFIG iec104_config[MAX_CHAN_NUM];

//Ԥ�õĿ���֡����
unsigned char TEST_FR_VAL[6]={0x68,0x04,0x43,0x00,0x00,0x00};
unsigned char TEST_FR_ACK[6]={0x68,0x04,0x83,0x00,0x00,0x00};
unsigned char STOP_DT_VAL[6]={0x68,0x04,0x13,0x00,0x00,0x00};
unsigned char START_DT_VAL[6]={0x68,0x04,0x07,0x00,0x00,0x00};
unsigned char S_FRAME_ACK[6]={0x68,0x04,0x01,0x00,0x00,0x00};

FETIME T101Time_To_Realtime(CP24Time2a& SoeTime);	//CP24time����Ϊʵ��ʱ��


Base_protocol* get_protocol(const int& channel)
{
	return new Iec104(channel);
}

void get_config(void)
{
	//todo
	//�����ݿ��ȡ104������Ϣ��д��iec104_config
	struct IEC104_CONFIG_
	{
		unsigned short channel_no; //ͨ����
		unsigned short  yx_type;	//ң������
		unsigned short soe_type;	//SOE����
		unsigned short  yc_type;	//ң������
		unsigned short  kwh_type; //�������

		unsigned short  yk_type;	//ң�����ͣ������˫��


		short call_all_data_gap;///<���ټ����Ϊ-1ʱ������
		short call_kwh_gap;		///<�ٻ���ȼ����Ϊ-1ʱ���ٻ�
		short send_time_gap;	///<��ʱ�����Ϊ-1ʱ����ʱ

		unsigned short yx_begin;	///<ң����ʼ��ַ
		unsigned short yc_begin;	///<ң����ʼ��ַ
		unsigned short yk_begin;	///<ң����ʼ��ַ
		unsigned short kwh_begin;	///<�����ʼ��ַ

		unsigned short t0;		///<t0��ʱ���������ӳ�ʱ
		unsigned short t1;		///<t1��ʱʱ�䣬����һ��I֡��U֡�󣬱�����t1ʱ���ڽ��յ��Է���ȷ�ϣ�������Ϊ���������⣬��Ҫ�ر���������
		unsigned short t2;		///<t2��ʱʱ�䣬����һ��I֡����t2ʱ����δ���յ��µ�I֡������S֡����ȷ��
		unsigned short t3;		///<t3��ʱʱ�䣬���յ��Է�һ֡���ĺ󣬾���t3ʱ��û�н��յ��µı��ģ���Ҫ��Է�����U֡����

		unsigned short test_fr_cnt;	//δ�ظ��Ĳ���֡��ֵ
		unsigned short k;		///kֵ����k��I֡δ�õ��Է�S֡ȷ��ʱ��ֹͣ���䣬ȡֵ��Χ1-32767
		unsigned short w;		///<wֵ����ٽ��յ��Է�w��I֡ʱ����Է�����S֡��ȡֵ��Χ1-32767

		unsigned char check_ns_nr;		///<��鷢�ͽ�����ţ���Ŵ�����Ҫ�ر����ӣ�1Ϊ��飬0Ϊ�����
		unsigned char chg_to_soe;		///<ң��תsoe��1ΪSOEתң�أ�0Ϊ��ת 
		unsigned char soe_to_chg;	///<soeתң�ţ�1ΪSOEתң�أ�0Ϊ��ת
		unsigned char grp_yc_check_chg_flag;	///<���鵥��ң�ż���λ��־��1Ϊ��飬0Ϊ�����
	};
	IEC104_CONFIG_ * special_config = NULL;

	int ret = Db_connector::select("select * from iec104_special_config order by  channel_no asc", (void **)&special_config);
	int rcd_cnt = ret;		//��Լ��������Ԫ������

	if(ret < 0)	//�������ݿ����
	{
		return;
	}

// 	ACE_DEBUG((LM_DEBUG, "read iec104 special \n"));

	for (int nnum = 0; nnum < rcd_cnt; ++nnum)
	{
		unsigned short channel_no = special_config->channel_no;

		iec104_config[channel_no].yx_type = special_config->yx_type;
		iec104_config[channel_no].soe_type = special_config->soe_type;
		iec104_config[channel_no].kwh_type = special_config->kwh_type;

		iec104_config[channel_no].call_all_data_gap = special_config->call_all_data_gap;
		iec104_config[channel_no].call_kwh_gap = special_config->call_kwh_gap;
		iec104_config[channel_no].send_time_gap = special_config->send_time_gap;

		iec104_config[channel_no].yx_begin = special_config->yx_begin;
		iec104_config[channel_no].yc_begin = special_config->yc_begin;
		iec104_config[channel_no].yk_begin = special_config->yk_begin;
		iec104_config[channel_no].kwh_begin = special_config->kwh_begin;

		iec104_config[channel_no].t0 = special_config->t0;
		iec104_config[channel_no].t1 = special_config->t1;
		iec104_config[channel_no].t2 = special_config->t2;
		iec104_config[channel_no].t3 = special_config->t3;

		iec104_config[channel_no].test_fr_cnt = special_config->test_fr_cnt;
		iec104_config[channel_no].k = special_config->k;
		iec104_config[channel_no].w = special_config->w;

		iec104_config[channel_no].check_ns_nr = special_config->check_ns_nr;
		iec104_config[channel_no].chg_to_soe = special_config->chg_to_soe;
		iec104_config[channel_no].soe_to_chg = special_config->soe_to_chg;
		iec104_config[channel_no].grp_yc_check_chg_flag = special_config->grp_yc_check_chg_flag;
		
		special_config++;
	}
}

char* get_version(void)
{
	return const_cast<char*>(version_string);
}

char* get_desc(void)
{
	return const_cast<char*>(desc_string);
}


Iec104::Iec104(const int& channel)
:Base_protocol(channel), m_config_ptr(iec104_config + channel)
{
// 	ACE_DEBUG((LM_DEBUG, "iec104 constructor\n"));

	m_common_addr = m_chanpara->common_addr;	//������ַ
}

Iec104::~Iec104()
{

}

void Iec104::init_protocol(void)
{
// 	ACE_DEBUG((LM_DEBUG, "iec104 init\n"));

	m_t0_timer.restart();
	m_t1_timer.restart();
	m_t2_timer.restart();
	m_t3_timer.restart();

	m_call_all_data_flag = NO_NEED;	
	m_call_kwh_data_flag = NO_NEED;	
	m_sync_time_flag = NO_NEED;
	m_rtu_init_flag = NO_NEED;

	m_bt1_timeout = false;
	m_bt2_timeout = false;
	m_bstartdt_flag = true;
	m_btestfr_flag = false;
	m_bSFrame_flag = false;

	m_call_all_data_timer.restart();	
	m_call_kwh_timer.restart();		
	m_sync_time_timer.restart();

	m_v_recv = 0;
	m_v_send = 0;
	m_v_ack = 0;

	m_current_ykno = -1;
	m_current_ykoder = 0;

	m_yx_all_num = Fes_shm::get().chan_para_ptr[m_channel_no].yx_num;
	m_yc_all_num = Fes_shm::get().chan_para_ptr[m_channel_no].yc_num;
	m_pi_all_num = Fes_shm::get().chan_para_ptr[m_channel_no].kwh_num;

	ACE_OS::memset(&m_read_buf, 0, REC_LEN);
	ACE_OS::memset(&m_write_buf, 0, TRN_LEN);

	ACE_OS::memset(m_wave_file_list,0,sizeof(WAVE_LIST)*WAVE_FILE_MAX);
	ACE_OS::memset(&m_cur_wave_file,0,sizeof(WAVE_LIST_RUN));
	m_Manual_wave = 0;
	m_wave_num_pos = 0;
}

void Iec104::protocol_proc(void)
{
	short curr_host_no = Fes_shm::get().fe_sys_data_ptr->host_no;
	while (!m_nEnd)
 	{
// 		ACE_DEBUG((LM_DEBUG, "��һ��ѭ��\n"));
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
		else		//�µ���������,����104��Լ����
		{
			init_protocol();
		}
 
		//����ѭ������
 		while (!m_nEnd)		//��������Ϊ��ͨ��ֵ���Ϊ��ǰǰ��
 		{
// 			ACE_DEBUG((LM_DEBUG, "�ڶ���ѭ��\n"));
			int write_ret = 0;
			int read_ret = 0;
			int proc_ret = 0;
	  		m_write_lenth = 0;  //���ͳ�������

			//����ʼ��������ʼ�������ش���
			check_event();
			write_ret = Iec104_send_proc();
			if (-1 == write_ret)
			{
				ACE_DEBUG((LM_DEBUG, "д�˿�ʧ��\n"));
				break;
			}

			//���ձ��ģ����жϸ��ֳ�ʱ
			read_ret  = recv_msg();	
			if (-1 == read_ret)			//���˿�ʧ�ܻᵼ�±���ѭ���˳�
			{
				ACE_DEBUG((LM_DEBUG, "���˿�ʧ��\n"));
				break;
			}
			else if (0 == read_ret)		//δ���յ����ݣ��жϳ�ʱ
			{
				if (m_t1_timer.elapsed() > m_config_ptr->t1 && m_bt1_timeout  )//����I֡��U֡����ȷ�ϣ�t1��ʱ�ر�����
				{
					ACE_DEBUG((LM_DEBUG, "�������ݳ�ʱ\n"));
					close_dev();
					break;
				}

				if (m_t3_timer.elapsed() > m_config_ptr->t3)				//t3��ʱ

				{
					m_btestfr_flag = true;
					m_t3_timer.restart();
				}
			}
			else	//��������������t3��ʱ��
			{
				m_t1_timer.restart();
				m_bt1_timeout = false;
				m_t3_timer.restart();
/*				printf("m_v_recv = %d  w=%d \n",m_v_recv,m_config_ptr->w);
				fflush(stdout);
				if (!(m_v_recv % m_config_ptr->w))	//���յ��Է�w��I֡ʱ����Է�����S֡
				{
					if (m_v_recv)		//�ѽ��յ�I֡
					{
						m_bSFrame_flag = true;
// 						send_s_frame();
					}
				}
*/
			}
			if (m_t2_timer.elapsed() > m_config_ptr->t2 && m_bt2_timeout )//����I֡�� t2ʱ�����s֡
			{
				m_bSFrame_flag = true;
				m_t2_timer.restart();
				m_bt2_timeout = false;
//				send_s_frame();
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
			if (m_read_lenth > 0)	//���ջ������б����ٽ��д���
			{
// 				ACE_DEBUG((LM_DEBUG, "��������,����������%d\n", m_read_lenth));
				FRAME_TYPE frame_type = judge_frame();
				switch (frame_type)
				{
				case I_FRAME:
					{
						proc_ret = deal_i_frame();
						if (!m_bt2_timeout)
						{
							m_t2_timer.restart();
							m_bt2_timeout = true;
						}
				//		m_t2_timer.restart();
				//		m_bt2_timeout = true;
					}
					break;
				case S_FRAME:
					proc_ret = deal_s_frame();
					break;
				case U_FRAME_TESTFR:
					proc_ret = deal_test_fr();
					break;
				case U_FRAME_TESTFR_ACK:
					proc_ret = deal_test_ack();
					break;
				case U_FRAME_STOPDT:
					proc_ret = deal_stop_dt();
					break;
				case U_FRAME_STARTDT:
					proc_ret = deal_start_dt();
					break;
				case INVALID_FRAME:
					proc_ret = deal_error_frame();
					break;
				default:
					break;
				}
				if (-1 == proc_ret)  //��������ѭ�� 
				{
					break;
				}
				if (!(m_v_recv % m_config_ptr->w))	//���յ��Է�w��I֡ʱ����Է�����S֡
				{
					if (m_v_recv)		//�ѽ��յ�I֡
					{
						m_bSFrame_flag = true;
					// 						send_s_frame();
					}
				}
			}


			if (m_chandata->curr_attend_host != curr_host_no)		//�жϹ黹����
			{
				close_dev();
				break;
			}
			ACE_OS::sleep(ACE_Time_Value(0,5*1000));

		}//while (true) ����ѭ��
 	}//while (true)  һ��ѭ��
}

int Iec104::Iec104_send_proc(void)
{
	int ret = 0xff;
	if (m_bstartdt_flag)
	{
		ret = send_start_DT();
		m_bstartdt_flag = false;
	}
	else if (m_rtu_init_flag)		//�����ն˳�ʼ��
	{
		ACE_DEBUG((LM_DEBUG, "rtu init\n"));
		m_rtu_init_flag = NO_NEED;
	}
	else if (m_btestfr_flag)
	{
// 		ACE_DEBUG((LM_DEBUG, "t3��ʱ,���Ͳ���֡\n"));
		ret = send_test_fr();
		m_btestfr_flag = false;
	}
	else if(m_bSFrame_flag)
	{
		ret = send_s_frame();
		m_bSFrame_flag = false;
	}
	else if (m_call_all_data_flag == NEED_CALL)		//�ʼ��оݵı��ķ���
	{
		if (m_call_kwh_data_flag == ON_PROCESS)	//�ٻ���ȵĹ����У�������������
		{
			//todo
		}
		ret = call_all_data();
	}
	else if (m_sync_time_flag == NEED_CALL)
	{
		if (m_call_all_data_flag != ON_PROCESS)		//���ٹ����в����ж�ʱ
		{
			ret = sync_time();
		}
	}
	else if ( m_pi_all_num && (m_call_kwh_data_flag == NEED_CALL))	//���е��������Ҫ�ٻ�
	{
		if (m_call_all_data_flag != ON_PROCESS)		//���ٹ����в����е���ٻ�
		{
			ret = call_pi_data();
		}
	}


	if (((m_write_buf[2]&0x03) != 0x01) && ret== 0)		//����I֡��U֡
	{
		m_t1_timer.restart();
		m_bt1_timeout = true;
	}
	return ret;
}


Iec104::FRAME_TYPE Iec104::judge_frame()
{
	int cur_position;
	int frame_Len = 0;

	FRAME_TYPE  judge;
	judge = INVALID_FRAME; //��ʼ��Ϊ��Ч֡
	ACE_OS::memset(m_apdu_buf, 0, MAX_BUF_SIZE);

	for (cur_position = 0;cur_position < m_read_lenth; cur_position++)
	{
		if (m_read_buf[cur_position] == FIRST)
		{
			frame_Len = m_read_buf[cur_position + 1] + 2;

			if((cur_position + frame_Len) > m_read_lenth)				//���������ı������´δ���	
			{
				ACE_OS::memmove(m_read_buf, &m_read_buf[cur_position], m_read_lenth - cur_position);
				m_read_lenth -= cur_position;
				frame_Len = 0;
			}
			else
			{
				ACE_OS::memcpy(m_apdu_buf, &m_read_buf[cur_position], frame_Len);   //����һ֡�������Ĵ�����

				//��m_read_buf��δ����ı�����λ
				ACE_OS::memmove(m_read_buf, &m_read_buf[cur_position + frame_Len], m_read_lenth - frame_Len - cur_position);
				m_read_lenth = m_read_lenth - cur_position - frame_Len;
			}
			if( m_read_lenth < 0 || m_read_lenth > REC_LEN )	//���Ļ��������ޣ�ȫ�����
			{
				m_read_lenth = 0;
				ACE_OS::memset(m_read_buf, 0, REC_LEN);	
			}
			ACE_OS::memset(&m_read_buf[m_read_lenth], 0, REC_LEN - m_read_lenth);	//��λ������������ʼ��
			break;
		}
	}
	if (cur_position == m_read_lenth)	//�������������ݶ����ԣ�ȫ�����
	{
		m_read_lenth = 0;
		ACE_OS::memset(m_read_buf, 0, REC_LEN);	
	}
	
	if (frame_Len == 0)		//���ı������´δ���
	{
		return judge;
	}
	if ((m_apdu_buf[0] != FIRST) || (m_apdu_buf[1] < 4))	//��Ч֡����
	{
		return judge;
	}

	P104_APDU *apdu_104;
	apdu_104=(P104_APDU*)&m_apdu_buf[0];
	cur_position += 2;   //�Ƶ������򲿷�

	switch (m_apdu_buf[2]&0x03)
	{
	case 0x00:
	case 0x02:
		{
			m_v_recv++;		//���յ�I֡�󣬽��ռ������ۼ�
			judge = I_FRAME;
		}
		break;
	case 0x01:
		judge = S_FRAME;
		break;
	case 0X03:		//U_FRAME
		switch(apdu_104->apci.type_u.u_type) //�����������ж�
		{
		case STARTDT_CON:  //��վ����������Ӧ
			judge = U_FRAME_STARTDT;
			break;
		case STOPDT_CON:
			judge = U_FRAME_STOPDT; //��վֹͣ������Ӧ
			break;
		case TESTFR_ACT:  //��վ���Ͳ���֡
			judge = U_FRAME_TESTFR;
			break;
		case TESTFR_CON:  //��վ�յ���վ����֡��ȷ��
			judge = U_FRAME_TESTFR_ACK;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	} 
	return judge;
}

int Iec104::deal_i_frame(void)
{
	int ret = 0;


	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	switch (apdu_104->asdu.type)
	{
	case M_SP_NA_1:							//yx����
		ret = deal_sequence_M_SP_NA_1();
		break;
	case M_DP_NA_1:
		ret = deal_sequence_M_DP_NA_1();
		break;
	case M_PS_NA_1:
		ret = deal_sequence_M_PS_NA_1();			
		break;
	case M_SP_TA_1:							//yxsoe����
		ret = deal_sequence_M_SP_TA_1();
		break;
	case M_DP_TA_1:
		ret = deal_sequence_M_DP_TA_1();
		break;
	case M_SP_TB_1:
		ret = deal_sequence_M_SP_TB_1();
		break;
	case M_DP_TB_1:
		ret = deal_sequence_M_DP_TB_1();		
		break;
	case M_ME_NA_1:							//yc����
	case M_ME_NB_1:
		ret = deal_sequence_M_ME_NA_1();
		break;
	case M_ME_TA_1:
		ret = deal_sequence_M_ME_TA_1();
		break;
	case M_ME_ND_1:
		ret = deal_sequence_M_ME_ND_1();
		break;
	case M_ME_NC_1:
		ret = deal_sequence_M_ME_NC_1();			
		break;
	case M_ME_TE_1:
		ret = deal_sequence_M_ME_TE_1();
		break;
	case M_ME_TF_1:
		ret = deal_sequence_M_ME_TF_1();
		break;
	case M_IT_NA_1:							//kwh����
		ret = deal_sequence_M_IT_NA_1();		
		break;
	case M_IT_TA_1:  //����
		break;
	case M_IT_TB_1:
		ret = deal_sequence_M_IT_TB_1();
		break;
	case M_BO_NA_1:		//7:��վԶ���ն�״̬���� ����
		break;
	case C_SC_NA_1:
	case C_DC_NA_1:
		{
			proc_yk_frame();			//yk����
		}
		break;
	case C_RC_NA_1://47://������У
		//		A_DAResult(StaNo,ChNo,DataHead,comp);
		break;
	case C_IC_NA_1:		//����
		{
			ret = 0;
			if ((apdu_104->asdu.cot & 0x3f) == COT_ACTTERM)
			{
				m_call_all_data_flag = NO_NEED;		//���ٻ�����,ȷ�ϲ�������
			}
		}
		break;
	case C_CI_NA_1:		//����ٻ�
		{
			ret = 0;
			if ((apdu_104->asdu.cot & 0x3f) == COT_ACTTERM)
			{
				m_call_kwh_data_flag = NO_NEED;		//����ٻ�����,ȷ�ϲ�������
			}
		}
		break;
	case C_CS_NA_1:		//��ʱȷ��
		{
			ret = 0;
			if ((apdu_104->asdu.cot & 0x3f) == COT_ACTCON)
			{
				m_chandata->sync_time_success = 1;
				m_sync_time_flag = NO_NEED;		//��ʱ����
			}
		}
		break;
	case 141:
	case 145:
		{
			deal_prot_frame();
		}
	default:
		break;
	} // end switch(apdu_104->asdu.type)

	if (ret == -1)
	{
		ACE_DEBUG((LM_DEBUG, "���ݴ������\n"));
	}

	return ret;
}

#pragma pack(1)
int Iec104::deal_s_frame(void)
{
	int ret = 0;

	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	int nrecv_count = apdu_104->apci.type_s.nr >> 1;
	
	if (nrecv_count != m_v_send)		
	{
		ACE_DEBUG((LM_DEBUG, "������վS֡������������\n"));		//�ݲ���S֡��������������ӡ��Ϣ,����ʽҲ�д�����
	}

	return ret;  //�޷���ֵ
}

int Iec104::send_s_frame(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_lenth = sizeof(S_FRAME_ACK);
	ACE_OS::memcpy(m_write_buf, S_FRAME_ACK, m_write_lenth);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.type_s.nr =  m_v_recv << 1;  //�������кţ�2�ֽ�

	return send_msg();
}

#pragma pack()

int Iec104::send_test_fr(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_lenth = sizeof(TEST_FR_VAL);
	ACE_OS::memcpy(m_write_buf, TEST_FR_VAL, m_write_lenth);
	return send_msg();

}

int Iec104::deal_test_fr(void)
{
	//������վ����֡������ȷ��֡
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_lenth = sizeof(TEST_FR_ACK);
	ACE_OS::memcpy(m_write_buf, TEST_FR_ACK, m_write_lenth);
	return send_msg();
}

int Iec104::deal_test_ack(void)
{
	return 0;	//������վȷ��֡����������
}

int Iec104::deal_error_frame(void)
{
 	ACE_DEBUG((LM_DEBUG, "recv invaild frame!\n"));
	return 0;
}

int Iec104::send_start_DT(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_lenth = sizeof(START_DT_VAL);
	ACE_OS::memcpy(m_write_buf, START_DT_VAL, m_write_lenth);

// 	ACE_DEBUG((LM_DEBUG, "send start DT!\n"));
	return send_msg();
}

int Iec104::deal_start_dt(void)
{
	//������·�󣬽������ٵ�
	m_call_all_data_flag = NEED_CALL;	
	m_call_kwh_data_flag = NEED_CALL;	
	m_sync_time_flag = NEED_CALL;

	return 0;
}

int Iec104::send_stop_DT(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_lenth = sizeof(STOP_DT_VAL);
	ACE_OS::memcpy(m_write_buf, STOP_DT_VAL, m_write_lenth);

	ACE_DEBUG((LM_DEBUG, "send stop dt!\n"));
	return send_msg();
}

int Iec104::deal_stop_dt(void)
{
	return 0;
}

#pragma pack(1)
int Iec104::call_all_data(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.apdu_len = VALUE;
	apdu_104->apci.type_i.ns = m_v_send << 1;   //�������кţ�2�ֽ�
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //�������кţ�2�ֽ�
	apdu_104->asdu.type = C_IC_NA_1;
	apdu_104->asdu.vsq = VSQ_SINGLE;
	apdu_104->asdu.cot = COT_ACT;
	apdu_104->asdu.addr = m_common_addr;

	C_STRUCT* pt = (C_STRUCT*)apdu_104->asdu.data.buf;
	pt->index_no = 0;
	pt->pos_holder = 0;
	pt->qdi = QDI_GENERAL_CALL;
	m_write_lenth = apdu_104->apci.apdu_len + 2;
	
	m_call_all_data_flag = ON_PROCESS;	

	m_v_send++; 
	return send_msg();
}

int Iec104::call_pi_data(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.apdu_len = VALUE;
	apdu_104->apci.type_i.ns = m_v_send << 1;   //�������кţ�2�ֽ�
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //�������кţ�2�ֽ�
	apdu_104->asdu.type = C_CI_NA_1;
	apdu_104->asdu.vsq = VSQ_SINGLE;
	apdu_104->asdu.cot = COT_ACT;
	apdu_104->asdu.addr = m_common_addr;

	C_STRUCT* pt = (C_STRUCT*)apdu_104->asdu.data.buf;
	pt->index_no = 0;
	pt->pos_holder = 0;
	pt->qdi = 0x45;
	m_write_lenth = apdu_104->apci.apdu_len + 2;

	m_call_kwh_data_flag = ON_PROCESS;

	m_v_send++; 
	return send_msg();
}

int Iec104::sync_time(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.apdu_len = SYNC_VALUE;
	apdu_104->apci.type_i.ns = m_v_send << 1;   //�������кţ�2�ֽ�
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //�������кţ�2�ֽ�
	apdu_104->asdu.type = C_CS_NA_1;
	apdu_104->asdu.vsq = VSQ_SINGLE;
	apdu_104->asdu.cot = COT_ACT;
	apdu_104->asdu.addr = m_common_addr;

	SYNC_TIME_STRUCT* pt = (SYNC_TIME_STRUCT*)apdu_104->asdu.data.buf;
	pt->index_no = 0;
	pt->pos_holder = 0;

	FETIME tim;
	get_time(&tim);

	pt->cp56time.minute = tim.minute;
	pt->cp56time.hour = tim.hour;
	pt->cp56time.day = tim.day;
	pt->cp56time.month = tim.month;
	pt->cp56time.year = (((tim.year + 1900)-2000)&0xff);
	pt->cp56time.millsecond = tim.milisecond + tim.second*1000;

	m_write_lenth = apdu_104->apci.apdu_len + 2;

	m_v_send++; 

	m_sync_time_flag = ON_PROCESS;

	m_chandata->last_sync_time = ACE_OS::time(NULL);
	m_chandata->sync_time_success = 0;

	return send_msg();
}

int Iec104::send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.apdu_len = YK_FRAME_LENTH;
	apdu_104->apci.type_i.ns = m_v_send << 1;   //�������кţ�2�ֽ�
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //�������кţ�2�ֽ�
	apdu_104->asdu.type = m_config_ptr->yk_type;
	apdu_104->asdu.vsq = VSQ_SINGLE;
	apdu_104->asdu.cot =  (yk_flag == 2) ? COT_DEACT : COT_ACT;		//�Ƿ�ȡ��

	apdu_104->asdu.addr = m_common_addr;

	C_STRUCT* pt = (C_STRUCT*)apdu_104->asdu.data.buf;
	//todo �Ƿ���Ҫ�ж�ң�غ�
	pt->index_no = m_config_ptr->yk_begin + ykno;
	pt->pos_holder = 0;
	
	//�غϿط�,�ɵ����˫��ң��ȷ��
	if (m_config_ptr->yk_type == C_SC_NA_1)		//����ң��
	{
		if(bclosed)
			pt->qdi = STATE_SP_ON;
		else
			pt->qdi = STATE_SP_OFF;
	}
	else if (m_config_ptr->yk_type == C_DC_NA_1)	//˫��ң��
	{
		if(bclosed)
			pt->qdi = STATE_DP_ON;
		else
			pt->qdi = STATE_DP_OFF;
	}

	//ѡ���ִ��
	if (yk_flag == 0)
	{
		pt->qdi |= 0x80;
	}

	m_current_ykno = ykno;				//�洢��ǰ�·�ң����ţ��Ա�У��
	m_current_ykoder = pt->qdi;			//�洢��ǰ�·�ң������Ա�У��

	m_write_lenth = apdu_104->apci.apdu_len + 2;

	m_v_send++;
	return send_msg();
}

int Iec104::send_yt_frame(unsigned char ytno, float set_value, unsigned char yt_flag)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

 	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
 	apdu_104->apci.start_id = FIRST;  
 	apdu_104->apci.apdu_len = SETPOINT_FRAME_LENTH;
 	apdu_104->apci.type_i.ns = m_v_send << 1;   //�������кţ�2�ֽ�
 	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //�������кţ�2�ֽ�
 	apdu_104->asdu.type = C_SE_NC_1;
 	apdu_104->asdu.vsq = VSQ_SINGLE;
 	apdu_104->asdu.cot =  (yt_flag == 3) ? COT_DEACT : COT_ACT;		//�Ƿ�ȡ��
	apdu_104->asdu.addr = m_common_addr;

	SETPOINT_STRUCT* pt = (SETPOINT_STRUCT*)apdu_104->asdu.data.buf;
	//todo �Ƿ���Ҫ�ж�ң�غ�
	pt->index_no = m_config_ptr->setpoint_begin + ytno;
	pt->pos_holder = 0;
	pt->index_value = set_value ;
	//ѡ���ִ��
// 	if (yt_flag == 0)
// 	{
// 		pt->qdi = 0x80;
// 	}
// 	else
// 	{
		pt->qdi = 0x00;
/*	}*/

	m_current_ykno = ytno;				//�洢��ǰ�·�ң����ţ��Ա�У��
	m_current_ykoder = pt->qdi;			//�洢��ǰ�·�ң������Ա�У��

 	m_write_lenth = apdu_104->apci.apdu_len + 2;

 	m_v_send++;
	return send_msg();
}

int Iec104::send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.type_i.ns = m_v_send << 1;   //�������кţ�2�ֽ�
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //�������кţ�2�ֽ�

	apdu_104->apci.apdu_len = cmd_len + 6 + 4;	//4:���շ�����ų��ȣ�6��ASDU���̶ֹ����� cmd_len:prot��֯���ĳ���

	apdu_104->asdu.addr = m_common_addr;

	unsigned char *Buff = apdu_104->asdu.data.buf;

	switch(protocol_type)
	{
	case FE_PROTTYPE_IEC103://����,��â104Ƕ�ױ�׼103����
		{
			apdu_104->asdu.type =  145;//���ͱ�ʶ
			apdu_104->asdu.vsq = VSQ_SINGLE; //�ɱ�ṹ�޶���(VSQ)
			apdu_104->asdu.cot =  COT_REQ;		//����ԭ��

			apdu_104->apci.apdu_len += 3;//��Ϣ������ַ����
			*Buff++ = 0;//��Ϣ������ַ 3�ֽ�
			*Buff++ = 0;
			*Buff++ = 0;

			for( int i=0; i<cmd_len ; i++)
			{
				*Buff++ = cmd[i];
			}
		}	//add end
		break;
	default:
		{
			for( int i=0; i<cmd_len ; i++)
			{
				*Buff++ = cmd[i];
			}
		}
		break;
	}

	m_write_lenth = apdu_104->apci.apdu_len + 2;
	m_v_send++;
	return send_msg();
}

int Iec104::send_WaveCommand(unsigned char Type, unsigned char* cmd_buf)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.type_i.ns = m_v_send << 1;   //�������кţ�2�ֽ�
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //�������кţ�2�ֽ�

	apdu_104->apci.apdu_len = 6 + 4;	//4:���շ�����ų��ȣ�6��ASDU���̶ֹ�����

	apdu_104->asdu.addr = m_common_addr;

	unsigned char *buf = apdu_104->asdu.data.buf;

	char tempstr[32];
	memset(tempstr,0,32);
	unsigned int index = 0;

	

	if (Type == CONFIRM_NODE || Type == CONFIRM_FILE)
	{
		apdu_104->asdu.type = F_AF_NA_1;
	}
	else
	{
		apdu_104->asdu.type = F_SC_NA_1;
	}
	apdu_104->asdu.vsq = 1;
	apdu_104->asdu.cot = COT_FILE;	//�ļ�����ԭ��
	if(Type == REQUTST_DIR)
		apdu_104->asdu.cot = COT_REQ;	//�ٻ�Ŀ¼ԭ��

	// 	t104.DSD[ChNo].wave_file.file_type = 1;
	*buf++ = m_cur_wave_file.file_type;	//type

	//��Ϣ���ַ  6902H~7000H
	if (Type == REQUTST_DIR)
	{
		*buf++ = 0x00;
		*buf++ = 0x00;
		*buf++ = 0x00;
		*buf++ = 0x00;
	}
	else
	{
		*buf++ = m_cur_wave_file.file_name[0];
		*buf++ = m_cur_wave_file.file_name[1];

		if (Type == SEL_FILE || Type == REQUTST_FILE)
		{
			*buf++ = 0x00;
		}
		else
			*buf++ = m_cur_wave_file.cur_nodename[0];
	}

	switch(Type)
	{
	case REQUTST_DIR:	//����Ŀ¼
		*buf++ = 0x00;
		break;
	case SEL_FILE:	//ѡ���ļ�
		*buf++ = 0x01;
		break;
	case REQUTST_FILE:	//�����ļ�
		*buf++ = 0x02;
		break;
	case REQUTST_NODE:	//�����
		*buf++ = 0x06;
		break;
	case CONFIRM_NODE:	//���Ͽ�
		{
			if (m_cur_wave_file.cur_node_lenth == m_cur_wave_file.sum_node_lenth)
			{
				*buf++ = 0x03;//�Ͽ�
			}
			else
				*buf++ = 0x04;//��
		}
		break;
	case CONFIRM_FILE:	//�ļ��Ͽ�
		{
			if (m_cur_wave_file.file_lenth == m_cur_wave_file.sum_file_lenth)
			{
				*buf++ = 0x01;//�Ͽ�

				if (m_cur_wave_file.file_type == 0x02 || m_cur_wave_file.file_type == 0x04)
				{
					break;
				}
				else		//�����ٻ�
				{
					m_Manual_wave = 2;
					m_cur_wave_file.file_type ++;	//����һ�����ļ�
					send_WaveCommand(Type,NULL);
				}

			}
			else
				*buf++ = 0x02;//��
		}
		break;
	default:
		break;
	}


	return 0;
}

#pragma pack()

#pragma pack(1)

//���º���ΪI֡�������͵Ĵ���///////////////////////////////////////////////////////////////////////////////////////

//YX���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::deal_sequence_M_SP_NA_1(void)
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}
	int info_num = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң�ŵ��
	unsigned char yx_quelity = 1;		//yx����λ
	if (apdu_104->asdu.vsq & 0x80)		//ֻ��һ����Ϣ���ַ
	{

		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;

		unsigned char* pt = (unsigned char*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		int i;
		for (i = 0; i < info_num; i++)
		{
			//����λΪ1 ������
			if ((*pt) & 0x80)
			{
				yx_quelity = 0;
			}
			else
			{
				yx_quelity = 1;
			}
			make_yx_real_data(data_no, *pt, yx_quelity);
			data_no++;
			pt++;
		}		
	}
	else			//ÿ֡����Ϣ���ַ
	{
		SEQ_M_NA_1* yx_struct = (SEQ_M_NA_1*)(apdu_104->asdu.data.buf);
		int i;
		for (i = 0; i < info_num; i++)
		{
			data_no = yx_struct->addr1 - m_config_ptr->yx_begin;
			if ((yx_struct->yx_value) & 0x80)	//����λΪ1 ������
			{
				yx_quelity = 0;
			}
			else
			{
				yx_quelity = 1;
			}
			make_yx_real_data(data_no, yx_struct->yx_value, yx_quelity);
			yx_struct++;
		}		
	}
	
	return 0;
}

int Iec104::deal_sequence_M_DP_NA_1(void)
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}
	int info_num = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң�ŵ��
	unsigned char value = 0;

	if (apdu_104->asdu.vsq & 0x80)		//ֻ��һ����Ϣ���ַ
	{

		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;
	
		unsigned char* pt = (unsigned char*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
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
		SEQ_M_NA_1* yx_struct = (SEQ_M_NA_1*)(apdu_104->asdu.data.buf);
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

	return 0;
}

int Iec104::deal_sequence_M_PS_NA_1(void)			//need for check �ô���ʽ�Բ���ȫ������֤
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}
	int info_num = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;
	unsigned char value = 0;

	if (apdu_104->asdu.vsq & 0x80)		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;

		SEQ_M_PS_NA_1* yx_struct = (SEQ_M_PS_NA_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
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

		SEQ_M_PS_NA_1_B* yx_struct = (SEQ_M_PS_NA_1_B*)(apdu_104->asdu.data.buf);

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

	return 0;

}
//YX���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//YXSOE���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::deal_sequence_M_SP_TA_1()		//M_SP_TA_1
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int info_num = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң�ŵ��
	FETIME soe_time;
	SEQ_M_TA_1* seq_struct = (SEQ_M_TA_1*)(apdu_104->asdu.data.buf);

	for (int i = 0; i < info_num; i++)
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin;  //����yx���
		soe_time = T101Time_To_Realtime(seq_struct->short_time);	

// 		make_yx_real_data(data_no, seq_struct->value, 1, &soe_time);
		//make event
		make_event(soe_time, data_no, seq_struct->value);

		seq_struct++;
	}

	return 0;
}

int Iec104::deal_sequence_M_DP_TA_1()		//M_DP_TA_1
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int info_num = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң�ŵ��
	unsigned char value = 0;

	FETIME soe_time;
	SEQ_M_TA_1* seq_struct = (SEQ_M_TA_1*)(apdu_104->asdu.data.buf);

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

// 		make_yx_real_data(data_no, seq_struct->value, 1, &soe_time);
		//make event
		make_event(soe_time, data_no, seq_struct->value);

		seq_struct++;
	}


	return 0;
}


int Iec104::deal_sequence_M_SP_TB_1()	//M_SP_TB_1
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int info_num = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң�ŵ��
	FETIME soe_time;
	SEQ_M_TB_1* seq_struct = (SEQ_M_TB_1*)(apdu_104->asdu.data.buf);

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


		//make event
		make_event(soe_time, data_no, seq_struct->value);
// 		make_yx_real_data(data_no, seq_struct->value, 1, &soe_time);

		seq_struct++;
	}

	return 0;
}

int Iec104::deal_sequence_M_DP_TB_1()	//M_DP_TB_1
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int info_num = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң�ŵ��
	unsigned char value = 0;
	FETIME soe_time;
	SEQ_M_TB_1* seq_struct = (SEQ_M_TB_1*)(apdu_104->asdu.data.buf);

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

// 		make_yx_real_data(data_no, seq_struct->value, 1, &soe_time);

		//make event
		make_event(soe_time, data_no, seq_struct->value);


		seq_struct++;
	}

		return 0;
}
//YXSOE���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//YC���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::deal_sequence_M_ME_NA_1()		//9��11��ʶYC����
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң����

	if (apdu_104->asdu.vsq & 0x80)		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//����ң����

		SEQ_M_ME_NA_1 * seq_data = (SEQ_M_ME_NA_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
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
		SEQ_M_ME_NA_1_B * yc_struct = (SEQ_M_ME_NA_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//����ң����
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity);
			yc_struct ++;
		}

	}
	return 0;
}

int Iec104::deal_sequence_M_ME_TA_1()	//10��ʱ��ң��ֵ
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң����
	FETIME time;			//ң�ⷢ��ʱ��
	SEQ_M_ME_TA_1 * yc_struct = (SEQ_M_ME_TA_1*)(apdu_104->asdu.data.buf);
	for (int i =0; i< datanum; ++i)
	{
		time = T101Time_To_Realtime(yc_struct->short_time);
		data_no = yc_struct->info_addr.addr1 - m_config_ptr->yc_begin;
		make_yc_real_data(data_no, yc_struct->value, yc_struct->quanity, &time);
		yc_struct++;
	}
	return 0;

}

int Iec104::deal_sequence_M_ME_NC_1()		//13��ʶYC����
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң����

	if (apdu_104->asdu.vsq & 0x80)		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//����ң����

		SEQ_M_ME_NC_1 * seq_data = (SEQ_M_ME_NC_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		for (int i =0; i< datanum; ++i)
		{
			make_yc_real_data(data_no, seq_data->value, seq_data->quanity);

			data_no++;		//ң������������
			seq_data ++;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		struct SEQ_M_ME_NC_1_B
		{
			INFO_ADDR addr;
			SEQ_M_ME_NC_1 seq_data;
		};

		SEQ_M_ME_NC_1_B * yc_struct = (SEQ_M_ME_NC_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//����ң����
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity);
			yc_struct ++;
		}

	}
	return 0;

}

int Iec104::deal_sequence_M_ME_ND_1()		//21:��Ʒ������ң��ֵ
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң����

	if (apdu_104->asdu.vsq & 0x80)		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//����ң����

		int16s* yc_data = (int16s*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
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

		SEQ_M_ME_ND_1_B * yc_struct = (SEQ_M_ME_ND_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//����ң����
			make_yc_real_data(data_no, yc_struct->yc_data, 1);
			yc_struct ++;
		}

	}
	return 0;
}

int Iec104::deal_sequence_M_ME_TE_1()//35 <��ʱ��CP56Time2a�ĵĲ���ֵ����Ȼ�ֵ
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң����

	FETIME yc_time;

	if (apdu_104->asdu.vsq & 0x80)		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//����ң����

		SEQ_M_ME_TE_1 * seq_data = (SEQ_M_ME_TE_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		//ʱ��ת��ΪFETIME
		yc_time.year = seq_data->long_time.year + 100;
		yc_time.month = seq_data->long_time.month;
		yc_time.day = seq_data->long_time.day;
		yc_time.hour = seq_data->long_time.hour;
		yc_time.minute = seq_data->long_time.minute;
		yc_time.second = seq_data->long_time.millsecond / 1000;
		yc_time.milisecond = seq_data->long_time.millsecond %1000;

		for (int i =0; i< datanum; ++i)
		{
			make_yc_real_data(data_no, seq_data->value, seq_data->quanity,&yc_time);
			data_no++;		//ң������������
			seq_data ++;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		struct SEQ_M_ME_TE_1_B
		{
			INFO_ADDR addr;
			SEQ_M_ME_TE_1 seq_data;
		};

		int sizen = sizeof(SEQ_M_ME_TE_1_B);
		SEQ_M_ME_TE_1_B * yc_struct = (SEQ_M_ME_TE_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//����ң����
			yc_time.year = yc_struct->seq_data.long_time.year + 100;
			yc_time.month = yc_struct->seq_data.long_time.month;
			yc_time.day = yc_struct->seq_data.long_time.day;
			yc_time.hour = yc_struct->seq_data.long_time.hour;
			yc_time.minute = yc_struct->seq_data.long_time.minute;
			yc_time.second = yc_struct->seq_data.long_time.millsecond / 1000;
			yc_time.milisecond = yc_struct->seq_data.long_time.millsecond %1000;
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity,&yc_time);
			yc_struct ++;
		}

	}
	return 0;
}

int Iec104::deal_sequence_M_ME_TF_1()//36 <��ʱ��CP56Time2a�ĵĲ���ֵ���̸�����
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡��ң�����ݸ���
	int data_no = 0;		//ң����

	FETIME yc_time;

	if (apdu_104->asdu.vsq & 0x80)		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//����ң����

		SEQ_M_ME_TF_1 * seq_data = (SEQ_M_ME_TF_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		//ʱ��ת��ΪFETIME
		yc_time.year = seq_data->long_time.year + 100;
		yc_time.month = seq_data->long_time.month;
		yc_time.day = seq_data->long_time.day;
		yc_time.hour = seq_data->long_time.hour;
		yc_time.minute = seq_data->long_time.minute;
		yc_time.second = seq_data->long_time.millsecond / 1000;
		yc_time.milisecond = seq_data->long_time.millsecond %1000;

		for (int i =0; i< datanum; ++i)
		{
			make_yc_real_data(data_no, seq_data->value, seq_data->quanity,&yc_time);
			data_no++;		//ң������������
			seq_data ++;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		struct SEQ_M_ME_TF_1_B
		{
			INFO_ADDR addr;
			SEQ_M_ME_TF_1 seq_data;
		};

		int sizen = sizeof(SEQ_M_ME_TF_1_B);
		SEQ_M_ME_TF_1_B * yc_struct = (SEQ_M_ME_TF_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//����ң����
			yc_time.year = yc_struct->seq_data.long_time.year + 100;
			yc_time.month = yc_struct->seq_data.long_time.month;
			yc_time.day = yc_struct->seq_data.long_time.day;
			yc_time.hour = yc_struct->seq_data.long_time.hour;
			yc_time.minute = yc_struct->seq_data.long_time.minute;
			yc_time.second = yc_struct->seq_data.long_time.millsecond / 1000;
			yc_time.milisecond = yc_struct->seq_data.long_time.millsecond %1000;
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity,&yc_time);
			yc_struct ++;
		}

	}
	return 0;
}
//YC���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//KWH���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::deal_sequence_M_IT_TB_1()		//0x25���
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡�ĵ�����ݸ���
	int data_no = 0;		//ң�����
	FETIME kwh_time;

	if (apdu_104->asdu.vsq & 0x80)		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->kwh_begin;//����ң�����

		SEQ_M_IT_TB_1 * seq_data = (SEQ_M_IT_TB_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));

		//ʱ��ת��ΪFETIME
		kwh_time.year = seq_data->long_time.year + 100;
		kwh_time.month = seq_data->long_time.month;
		kwh_time.day = seq_data->long_time.day;
		kwh_time.hour = seq_data->long_time.hour;
		kwh_time.minute = seq_data->long_time.minute;
		kwh_time.second = seq_data->long_time.millsecond / 1000;
		kwh_time.milisecond = seq_data->long_time.millsecond %1000;

		for (int i =0; i< datanum; ++i)
		{
			make_kwh_real_data(data_no, seq_data->value, seq_data->quanity,&kwh_time);
			data_no++;		//ң�������������
			seq_data ++;
		}
	}
	else		//ÿ֡����Ϣ���ַ
	{
		struct SEQ_M_IT_TB_1_B
		{
			INFO_ADDR addr;
			SEQ_M_IT_TB_1 seq_data;
		};

		SEQ_M_IT_TB_1_B * kwh_struct = (SEQ_M_IT_TB_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = kwh_struct->addr.addr1 - m_config_ptr->kwh_begin;//����ң�����

			kwh_time.year = kwh_struct->seq_data.long_time.year + 100;
			kwh_time.month = kwh_struct->seq_data.long_time.month;
			kwh_time.day = kwh_struct->seq_data.long_time.day;
			kwh_time.hour = kwh_struct->seq_data.long_time.hour;
			kwh_time.minute = kwh_struct->seq_data.long_time.minute;
			kwh_time.second = kwh_struct->seq_data.long_time.millsecond / 1000;
			kwh_time.milisecond = kwh_struct->seq_data.long_time.millsecond %1000;

			make_kwh_real_data(data_no, kwh_struct->seq_data.value, kwh_struct->seq_data.quanity,&kwh_time);
			kwh_struct ++;
		}

	}
	return 0;
}

int Iec104::deal_sequence_M_IT_NA_1()		//0x15���
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //��ȡ��ǰ֡�ĵ�����ݸ���
	int data_no = 0;		//ң�����

	if (apdu_104->asdu.vsq & 0x80)		//ֻ��һ����Ϣ���ַ
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->kwh_begin;//����ң�����

		SEQ_M_IT_NA_1 * seq_data = (SEQ_M_IT_NA_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
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

		SEQ_M_IT_NA_1_B * kwh_struct = (SEQ_M_IT_NA_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = kwh_struct->addr.addr1 - m_config_ptr->kwh_begin;//����ң�����
			make_kwh_real_data(data_no, kwh_struct->seq_data.value, kwh_struct->seq_data.quanity);
			kwh_struct ++;
		}

	}
	return 0;
}
//KWH���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//YK���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::proc_yk_frame()
{
	char str[NORMALCHAR_LEN];


	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int yk_ret = -1;
	C_STRUCT * yk_struct = (C_STRUCT*)(apdu_104->asdu.data.buf);
	int ret_ykno = yk_struct->index_no - m_config_ptr->yk_begin;

	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		ACE_OS::sprintf(str,"ң�ع�����ַ��,��ȷ��ַ:%d,��У��ַ:%d",m_common_addr,apdu_104->asdu.addr);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if (ret_ykno != m_current_ykno)	//ң�غų���
	{
		ACE_ERROR((LM_ERROR, "�·���ң�غźͷ�У��ң�غŲ�һ��!"));
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}
	if ((apdu_104->asdu.cot & 0x3f) != COT_ACTCON && (apdu_104->asdu.cot & 0x3f) != COT_DEACTCON)	//����ԭ���
	{
		ACE_OS::sprintf(str,"ң�ط�У�Ĵ���ԭ����Ч,cot = %02x",apdu_104->asdu.cot & 0x3f);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}
	if (yk_struct->qdi != m_current_ykoder)	//ң���������
	{
		ACE_OS::sprintf(str,"ң�������,�·�����:%02x ��У����:%02x",m_current_ykoder,yk_struct->qdi);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if((apdu_104->asdu.cot & 0x40) == 0 )		//�жϷ�У���
	{
		ACE_OS::sprintf(str,"ң�ط�У��ȷ!");
		yk_ret  = 0;
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
	}
	else if((apdu_104->asdu.cot & 0x40) == 0x40 )
	{
		ACE_OS::sprintf(str,"ң�ط�У������!");
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if (!(yk_struct->qdi & 0x80))		//ң��ִ�л�ȡ����ɺ�
	{
		ACE_DEBUG((LM_DEBUG, "ң�ض������!"));
		m_current_ykoder = 0;	//���ң�غ�ң��״̬��λ
	}

	//������ȷ��Ϣ
	yk_ret = 0;
	return yk_ret;
}
//YK���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//YT���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::proc_yt_frame()
{
	char str[NORMALCHAR_LEN];


	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int yk_ret = -1;
	C_STRUCT * yk_struct = (C_STRUCT*)(apdu_104->asdu.data.buf);
	int ret_ykno = yk_struct->index_no - m_config_ptr->setpoint_begin;

	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		ACE_OS::sprintf(str,"��㹫����ַ��,��ȷ��ַ:%d,��У��ַ:%d",m_common_addr,apdu_104->asdu.addr);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if (ret_ykno != m_current_ykno)	//ң�غų���
	{
		ACE_ERROR((LM_ERROR, "�·������źͷ�У�����Ų�һ��!"));
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}
	if ((apdu_104->asdu.cot & 0x3f) != COT_ACTCON && (apdu_104->asdu.cot & 0x3f) != COT_DEACTCON)	//����ԭ���
	{
		ACE_OS::sprintf(str,"��㷵У�Ĵ���ԭ����Ч,cot = %02x",apdu_104->asdu.cot & 0x3f);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}


	if((apdu_104->asdu.cot & 0x40) == 0 )		//�жϷ�У���
	{
		ACE_OS::sprintf(str,"��㷵У��ȷ!");
		yk_ret  = 0;
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
	}
	else if((apdu_104->asdu.cot & 0x40) == 0x40 )
	{
		ACE_OS::sprintf(str,"��㷵У������!");
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}


	if ((apdu_104->asdu.cot & 0x3f) == COT_ACTTERM)		//ң��ִ�л�ȡ����ɺ�
	{
		// ACE_DEBUG((LM_DEBUG, "ң�ض������!"));
		m_current_ykoder = 0;	//���ң�غ�ң��״̬��λ
	}

	//������ȷ��Ϣ
	yk_ret = 0;
	return yk_ret;
}
//YT���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//����֡���ʹ���begin/////////////////////////////////////////////////////////////////////////////////
int Iec104::deal_prot_frame(void)
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	unsigned char sendflag = 0;
	unsigned char protocoltype = 0;
	if(apdu_104->asdu.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}

	char* mailbuf;
	unsigned char* Buff;
	mailbuf = static_cast<char*>(ACE_OS::malloc(1+256));
	int mail_lenth = 0;

	switch(apdu_104->asdu.type)
	{
	case 140:
	case 141://��â��������
		{
			sendflag = 1;
			protocoltype = FE_PROTTYPE_IEC103;
			Buff = apdu_104->asdu.data.buf + 6;
			mail_lenth = ASDU_DATA_LEN - 6; //����Ϣ�����ַ��ʼΪ��������
		}
		break;
	case 145:	//IEC103 hex103  ������Ҫ
		{
			*mailbuf = FE_PROTTYPE_IEC103;		
			Buff = apdu_104->asdu.data.buf + 6;
			mail_lenth = ASDU_DATA_LEN - 6; //����Ϣ�����ַ��ʼΪ��������
		}
		break;
	default:
		break;
	}

	if(sendflag==1)//��������
	{
		FEP_PROT_STRU protect;
		ACE_OS::memset(&protect,0,sizeof(protect));
		protect.terminalno = m_rtu_no;
		protect.protocoltype = protocoltype;
		protect.gram_len = mail_lenth;
		ACE_OS::memcpy((char *)protect.gram,Buff,mail_lenth);

		Fe_scasrv::fe_send_prot_event(&protect);
		Proc_manager::putdbg(G101DBG,"��SCADA���������ͱ��������ʼ�");
	}
	else if (sendflag == 0)//��ֵ����
	{

		ACE_OS::memcpy((char *)(mailbuf+1),Buff,mail_lenth);
		Fe_mail::send_prot_ack((void*)mailbuf,mail_lenth+1,m_rtu_no);
		Proc_manager::putdbg(G101DBG,"��ͻ������ʼ�");
	}

	return 0;
}
//����֡���ʹ���end/////////////////////////////////////////////////////////////////////////////////

//¼������begin////////////////////////////////////////////////////////////////////////
int Iec104::proc_wave_frame()
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	unsigned char * ptr = apdu_104->asdu.data.buf;

	unsigned char * pFileCon = NULL;
	FertFile* myfile;
	static char filepath[256];
	int sub_rec = 0;
	FILE_SOF file_sof;
	unsigned short info_addr;	//����װ�õ�ַ
	unsigned short node_length;
	unsigned char file_type = 0;//�ļ�����
	char NOF[IEC_101_NOF];
	unsigned char NOS;
	unsigned char	ReadyQ;

	struct tm time_span;
	time_t time_change;
	int mseconds;
	int ngroup = 0;//��·���
	int nstart_way = 0;//������ʽ

	int wave_minute = 0;	//¼���ļ�����ʱ�����
	int wave_msec = 0;	//¼���ļ�����ʱ����

	int file_lenth = 0;
	unsigned char order_type = 0xff;

	unsigned char frame_pos = 0;

	if (apdu_104->asdu.vsq == 1)
	{
		info_addr = ptr[frame_pos]+ptr[frame_pos + 1]*256;
		frame_pos += 2;
		file_type = ptr[frame_pos++];	
		if (ptr[frame_pos++] != m_cur_wave_file.file_name[0] || ptr[frame_pos++] != m_cur_wave_file.file_name[1])
		{
			printf("NOF error!\n");
			fflush(stdout);
		}
		if (info_addr != m_cur_wave_file.info_addr)
		{
			printf("rtu address error!\n");
			fflush(stdout);
		}
		if (file_type != m_cur_wave_file.file_type)
		{
			printf("file type error!\n");
			fflush(stdout);
		}
	}
	else	//����Ŀ¼����
	{
		if (apdu_104->asdu.type == F_DR_TA_1)		//Ŀ¼
		{
			if (m_wave_num_pos == 0)
			{
				ACE_OS::memset(&(m_wave_file_list), 0, sizeof(WAVE_LIST));
			}

			if ( !apdu_104->asdu.vsq )
			{
				m_Manual_wave = 0;//��Ŀ¼���ֶ�λ��0
				return 0;
			}

			for (int i = m_wave_num_pos; i < apdu_104->asdu.vsq + m_wave_num_pos; i++)
			{
				if (frame_pos > ASDU_DATA_LEN || m_wave_num_pos > WAVE_FILE_MAX)
				{
					break;
				}
				//ѭ������Ŀ¼
				m_wave_file_list[i].info_addrL = (ptr[frame_pos]) + (ptr[frame_pos+1]*256);
				frame_pos += 2;
				m_wave_file_list[i].file_type = ptr[frame_pos++];	

				m_wave_file_list[i].file_name[0] = ptr[frame_pos++];
				m_wave_file_list[i].file_name[1] = ptr[frame_pos++];

				m_wave_file_list[i].len = ptr[frame_pos] + ptr[frame_pos+1]*256 + ptr[frame_pos+2]*256*256;
				frame_pos += 3;

				ACE_OS::memcpy(&(m_wave_file_list[i].sof), &(ptr[frame_pos++]), sizeof(FILE_SOF));
				// 				m_wave_file_list[i].sof = (FILE_SOF)ptr[frame_pos++];

				mseconds = ptr[frame_pos]+ptr[frame_pos+1]*256;	//ȡ�ú���ʱ��
				frame_pos += 2;
				time_span.tm_sec =	mseconds/1000;
				time_span.tm_min = ptr[frame_pos++];
				time_span.tm_hour = ptr[frame_pos++];
				time_span.tm_mday = ptr[frame_pos++];
				time_span.tm_mon  = ptr[frame_pos++] - 1;
				time_span.tm_year = ptr[frame_pos++] + 2000 - 1900;

				time_change =  mktime(&time_span);

				m_wave_file_list[i].time_msec = mseconds%1000;
				m_wave_file_list[i].time_sec = time_change;
			}

			if (m_wave_file_list[apdu_104->asdu.vsq - 1].sof.LFD == 0)	//��֡Ŀ¼�ļ�δ�����꣬�����ļ�
			{
				m_wave_num_pos += apdu_104->asdu.vsq;
				return 0;
			}
			else
			{
				m_wave_num_pos = 0;
			}


			if (m_Manual_wave == 0)	//����������ͣ�ѡ���һ��¼���ļ�
			{
				m_cur_wave_file.info_addr = m_wave_file_list[0].info_addrL;
				m_cur_wave_file.file_name[0] = m_wave_file_list[0].file_name[0];
				m_cur_wave_file.file_name[1] = m_wave_file_list[0].file_name[1];

				// 				m_cur_wave_file.file_lenth = m_wave_file_list[0].len;   �ļ��������ļ�׼������ȷ��
				m_cur_wave_file.file_type = m_wave_file_list[0].file_type;
				//ѡ��¼���ļ�
				order_type = SEL_FILE;
			}
			else if (m_Manual_wave == 1)	//�ֶ��ٻ�
			{
				//todo ��Ŀ¼�������ٻ�����
				char	*mailbuf;
				mailbuf = (char *)malloc( 512 );
				if(mailbuf==NULL)
				{	
					printf("�����ڴ�ʧ��!\n");
					return 0;
				}
				*mailbuf = 0;
				*(mailbuf+1) = apdu_104->asdu.vsq;
				int mail_lenth = sizeof(WAVE_LIST)*apdu_104->asdu.vsq;
				ACE_OS::memcpy((char *)(mailbuf+2),(char *)m_wave_file_list,sizeof(WAVE_LIST)*apdu_104->asdu.vsq);

				int ret = Fe_mail::send_prot_ack(mailbuf,mail_lenth+2,m_rtu_no);			

				if(mailbuf) free(mailbuf);
				mailbuf = NULL;
			}
			else if (m_Manual_wave == 2)	//�����ٻ���������
			{
				order_type = SEL_FILE;
			}

			m_Manual_wave = 0;//Ŀ¼���ܳɹ�����0
		}
	}

	switch(apdu_104->asdu.type)
	{
	case F_FR_NA_1:	//�ļ�׼������
		{
			file_lenth =  ptr[frame_pos] + ptr[frame_pos+1]*256 + ptr[frame_pos+2]*256*256;	//�ļ�����
			frame_pos += 3;

			ReadyQ = ptr[frame_pos++];


			m_cur_wave_file.file_lenth = file_lenth;

			//�����ļ�
			if ( (ReadyQ & 0x80) == 0)
			{
				order_type = REQUTST_FILE;
			}
		}
		break;
	case F_SR_NA_1:	//��׼������
		{
			m_cur_wave_file.cur_nodename[0] =  ptr[frame_pos++];

			file_lenth =  ptr[frame_pos] + ptr[frame_pos+1]*256 + ptr[frame_pos+2]*256*256;	//�ڳ���
			frame_pos += 3;

			ReadyQ = ptr[frame_pos++];

			m_cur_wave_file.cur_node_lenth = file_lenth; 

			//������ļ�
			if ( (ReadyQ & 0x80) == 0)
			{
				order_type = REQUTST_NODE;
			}
		}
		break;
	case F_LS_NA_1:	//���Ľ�,���Ķ�
		{
			m_cur_wave_file.cur_nodename[0] =  ptr[frame_pos++];

			ReadyQ = ptr[frame_pos++];

			if (ReadyQ == 0x01 || ReadyQ == 0x02)//�ļ��Ͽ�
			{
				order_type = CONFIRM_FILE;

				myfile->SaveToSys();

			}
			else if (ReadyQ == 0x03 || ReadyQ == 0x04)//���Ͽ�
			{
				//todo У��
				order_type = CONFIRM_NODE;
			}
		}
		break;
	case F_SG_NA_1:	//��
		{
			ACE_OS::memset(filepath,0,256);
			ACE_OS::sprintf(filepath,"%s/log/dat/wave/",getenv("NBENV"));

			m_cur_wave_file.cur_nodename[0] =  ptr[frame_pos++];

			m_cur_wave_file.cur_phase_lenth =  ptr[frame_pos++];//��ǰ�γ���

			pFileCon = ptr + frame_pos;

			//////////////////////////////////////////////////////////////////////////l¼������
			if (m_cur_wave_file.sum_node_lenth == 0)		//��ǰ�ڳ���Ϊ0�������ļ�
			{

				unsigned short nfile_name = m_cur_wave_file.file_name[0] + m_cur_wave_file.file_name[1] * 256;

				ACE_OS::sprintf(m_cur_wave_file.save_name,"%02d_%02d_%03d",m_rtu_no, nfile_name/1000, nfile_name % 1000);	//վ�ţ��ٻ���վrtu�ţ�����

				switch(file_type)
				{
				case 1:
					ACE_OS::strcat(m_cur_wave_file.save_name,".cfg");
					break;
				case 2:
					ACE_OS::strcat(m_cur_wave_file.save_name,".dat");
					break;
				case 3:
					ACE_OS::strcat(m_cur_wave_file.save_name,".hdr");
					break;
				case 4:
					ACE_OS::strcat(m_cur_wave_file.save_name,".cfg+dat+hdr");
					break;
				}

				ACE_OS::strcat(filepath,m_cur_wave_file.save_name);

				myfile = new FertFile(filepath);

				int write_num = myfile->Save((char*)pFileCon,m_cur_wave_file.cur_phase_lenth,m_cur_wave_file.sum_file_lenth,FILE_WRITE_NEW,MYCHAR_BIT);
			}
			else
			{
				ACE_OS::strcat(filepath,m_cur_wave_file.save_name);

				int write_num = myfile->Save((char*)pFileCon,m_cur_wave_file.cur_phase_lenth,m_cur_wave_file.sum_file_lenth,FILE_WRITE_CON,MYCHAR_BIT);
			}

			m_cur_wave_file.sum_node_lenth += m_cur_wave_file.cur_phase_lenth;//��ǰ�ڳ��ȱ��
			m_cur_wave_file.sum_file_lenth += m_cur_wave_file.cur_phase_lenth;
		}
		break;

	default:
		break;
	}


	//�Զ��ٻ�ʣ��֡
	if(order_type != 0xff)
	{
		return send_WaveCommand(order_type, NULL);
	}
	return 0;
}

//¼������end////////////////////////////////////////////////////////////////////////

#pragma pack()

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
