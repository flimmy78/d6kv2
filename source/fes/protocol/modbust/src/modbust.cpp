/**
@file modbust.cpp
@brief ���ϵ�������modbus��Լ����Դ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-3-23
*/

#include "modbust.h"

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
const char* const desc_string = "the very beginning";

//���Ӳ��ɹ��󣬵ȴ�2���ٳ�������
const int CONN_RETRY_TIME = 2;
const int SECOND_LOOP_TIME = 1;

/**
@brief IEC104������
*/
static MODBUST_CONFIG modbust_config[MAX_CHAN_NUM];

//Ԥ�õĿ���֡����
unsigned char CONF_LK_VAL[4]={0x00,0x00,0xff,0xff};


FETIME ModbustTime_To_Realtime(MODBUST_TIME* mTime);

Base_protocol* get_protocol(const int& channel)
{
	return new Modbust(channel);
}

void get_config(void)
{
	//todo
	//�����ݿ��ȡ104������Ϣ��д��iec104_config
	struct MODBUST_CONFIG_
	{
		unsigned short channel_no; //ͨ����

		unsigned short yx_begin;	///<ң����ʼ��ַ
		unsigned short yc_begin;	///<ң����ʼ��ַ
		unsigned short kwh_begin;	///<�����ʼ��ַ

		unsigned char reply_data_flag;		///<�յ�ң�⡢ң�š�ң�����ݺ�ظ�ȷ��֡��1Ϊ�ظ���0Ϊ���ظ�
	};
	MODBUST_CONFIG_ * special_config = NULL;

	int ret = Db_connector::select("select * from modbust_special_config order by  channel_no asc", (void **)&special_config);
	int rcd_cnt = ret;		//��Լ��������Ԫ������

	if(ret < 0)	//�������ݿ����
	{
		return;
	}

// 	ACE_DEBUG((LM_DEBUG, "read iec104 special \n"));

	for (int nnum = 0; nnum < rcd_cnt; ++nnum)
	{
		unsigned short channel_no = special_config->channel_no;

		modbust_config[channel_no].yx_begin = special_config->yx_begin;
		modbust_config[channel_no].yc_begin = special_config->yc_begin;
		modbust_config[channel_no].kwh_begin = special_config->kwh_begin;

		modbust_config[channel_no].reply_data_flag = special_config->reply_data_flag;
		
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


Modbust::Modbust(const int& channel)
:Base_protocol(channel), m_config_ptr(modbust_config + channel)
{
// 	ACE_DEBUG((LM_DEBUG, "iec104 constructor\n"));

	m_common_addr = m_chanpara->common_addr;	//������ַ

}

Modbust::~Modbust()
{

}

void Modbust::init_protocol(void)
{
// 	ACE_DEBUG((LM_DEBUG, "modbust init\n"));
	m_confirm_link_flag = false;

	m_yx_all_num = Fes_shm::get().chan_para_ptr->yx_num;
	m_yc_all_num = Fes_shm::get().chan_para_ptr->yc_num;
	m_pi_all_num = Fes_shm::get().chan_para_ptr->kwh_num;

	ACE_OS::memset(&m_read_buf, 0, REC_LEN);
	ACE_OS::memset(&m_write_buf, 0, TRN_LEN);
}

void Modbust::protocol_proc(void)
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
			write_ret = modbust_send_proc();
			if (-1 == write_ret)
			{
				ACE_DEBUG((LM_DEBUG, "д�˿�ʧ��\n"));
// 				break;
			}

			//���ձ��ģ����жϸ��ֳ�ʱ
			read_ret  = recv_msg();	
			if (-1 == read_ret)			//���˿�ʧ�ܻᵼ�±���ѭ���˳�
			{
				ACE_DEBUG((LM_DEBUG, "���˿�ʧ��\n"));
				break;
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
					}
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
			ACE_OS::sleep(ACE_Time_Value(0,10*1000));

		}//while (true) ����ѭ��
 	}//while (true)  һ��ѭ��
}

int Modbust::modbust_send_proc(void)
{
	int ret = 0xffff;
	if (m_confirm_link_flag && m_config_ptr->reply_data_flag)
	{
		ret = confirm_link();
		m_confirm_link_flag = false;
	}
	if (m_call_yc_data_flag == NEED_CALL)
	{
		call_yc_data();
	}
	if (m_call_yx_data_flag == NEED_CALL)
	{
		call_yx_data();
	}
	if (m_call_ym_data_flag == NEED_CALL)
	{
		call_ym_data();
	}
	return ret;
}


Modbust::FRAME_TYPE Modbust::judge_frame()
{
	int cur_position;
	int frame_Len = 0;

	FRAME_TYPE  judge;
	judge = INVALID_FRAME; //��ʼ��Ϊ��Ч֡
	ACE_OS::memset(m_apdu_buf, 0, MAX_BUF_SIZE);

	for (cur_position = 0;cur_position < m_read_lenth; cur_position++)
	{
		if (m_read_buf[cur_position] == m_common_addr && (m_read_buf[cur_position+1]==0x10 || m_read_buf[cur_position+1]==0x0f))	//��һ���ֽ���װ�õ�ַ,�ڶ�λ��0x10��0x0f
		{
			frame_Len = m_read_buf[cur_position + 8]*256+m_read_buf[cur_position+9] + 18;//�����ֽ���+18

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
	if ((m_apdu_buf[0] != m_common_addr))	//��Ч֡����
	{
		return judge;
	}
	unsigned short crc = CRC16(m_apdu_buf,frame_Len-2);
	if (LOBYTE(crc)!=m_apdu_buf[frame_Len-2] || HIBYTE(crc)!=m_apdu_buf[frame_Len-1])	//У�������
	{
		return judge;
	}

	switch (m_apdu_buf[1])
	{
	case 0x10:
	case 0x0f:
		{
			m_v_recv++;		//���յ�I֡�󣬽��ռ������ۼ�
			judge = I_FRAME;
		}
		break;
	default:
		break;
	} 
	return judge;
}

int Modbust::deal_i_frame(void)
{
	int ret = 0;


	MODBUST_APDU *apdu_modbust = (MODBUST_APDU*)m_apdu_buf;
	unsigned short int data_begin_addr = apdu_modbust->apci.data_begin_addr_h*256+apdu_modbust->apci.data_begin_addr_l;
	switch (apdu_modbust->apci.type)
	{
	case M_ME_TA_1:							
		if (data_begin_addr >= m_config_ptr->yc_begin && data_begin_addr < m_config_ptr->kwh_begin)//yc����
		{
			ret = deal_sequence_M_ME_NA_1();
			m_confirm_link_flag = true;
		}
		else if (data_begin_addr == m_config_ptr->kwh_begin)//ym����
		{
			ret = deal_sequence_M_IT_NA_1();
			m_confirm_link_flag = true;
		}
		break;
	case M_SP_TA_1:			//yx����
		ret = deal_sequence_M_SP_NA_1();
		m_confirm_link_flag = true;
		break;
	default:
		break;
	} // end switch(apdu_104->asdu.type)

	if (ret == -1)
	{
		ACE_DEBUG((LM_DEBUG, "���ݴ������\n"));
	}

	return ret;
}

int Modbust::deal_error_frame(void)
{
 	ACE_DEBUG((LM_DEBUG, "recv invaild frame!\n"));
	return 0;
}

#pragma pack(1)

//���º���ΪI֡�������͵Ĵ���///////////////////////////////////////////////////////////////////////////////////////

//YX���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Modbust::deal_sequence_M_SP_NA_1(void)
{
	MODBUST_APDU *pMbDataUnit = (MODBUST_APDU*)m_apdu_buf;
	unsigned short int data_begin_addr = pMbDataUnit->apci.data_begin_addr_h*256+pMbDataUnit->apci.data_begin_addr_l;

	if(pMbDataUnit->apci.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}
	if(pMbDataUnit->apci.type != M_SP_TA_1)		//�ж�����
	{
		return -1;
	}
	if(data_begin_addr != m_config_ptr->yx_begin)	//ң����ʼ��ַΪ0x0000
	{
		return -1;
	}

	int datanum = pMbDataUnit->apci.data_num_h*256+pMbDataUnit->apci.data_num_l; //��ȡ��ǰ֡��ң�����ݸ���
	int data_bytenum = pMbDataUnit->apci.data_byte_num_h*256+pMbDataUnit->apci.data_byte_num_l;	//ң����ռ�ֽ���
	int data_no = 0;		//ң����
	unsigned char value = 0;

	MODBUST_TIME *data_time = (MODBUST_TIME*)(pMbDataUnit->buf+data_bytenum); 
	FETIME time;			//ң�ⷢ��ʱ��
	time = ModbustTime_To_Realtime(data_time);
	for (int i=0;i<data_bytenum;i++)
	{
		unsigned char tmpdata= pMbDataUnit->buf[i];
		for (int j=0;j<8; j++)
		{
			value = (tmpdata>>j) & 0x01;
			if (data_no > datanum )	//��������ң����
			{
				break;
			}
			make_yx_real_data(data_no,value,1,&time);
			data_no++;
		}
	}
	return 0;
}

//YX���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//YC���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Modbust::deal_sequence_M_ME_NA_1()		//0x10��ʶYC����
{
	MODBUST_APDU *apdu_104 = (MODBUST_APDU*)m_apdu_buf;
	unsigned short int data_begin_addr = apdu_104->apci.data_begin_addr_h*256+apdu_104->apci.data_begin_addr_l;

	if(apdu_104->apci.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}
	if(apdu_104->apci.type != M_ME_TA_1)		//�ж�����
	{
		return -1;
	}
// 	if(data_begin_addr != m_config_ptr->yc_begin)	//ң����ʼ��ַΪ0x0000
// 	{
// 		return -1;
// 	}

	int datanum = apdu_104->apci.data_num_h*256+apdu_104->apci.data_num_l; //��ȡ��ǰ֡��ң�����ݸ���
	int data_bytenum = apdu_104->apci.data_byte_num_h*256+apdu_104->apci.data_byte_num_l;	//ң����ռ�ֽ���

	int data_no = 0;		//ң����

	MODBUST_TIME *data_time = (MODBUST_TIME*)(apdu_104->buf+data_bytenum); 
	FETIME time;			//ң�ⷢ��ʱ��
	time = ModbustTime_To_Realtime(data_time);
	for (int i=0;i<datanum;i++)
	{
		MODBUST_YC* tmpdata= (MODBUST_YC*)(apdu_104->buf+i*2);
		unsigned short int yc_data = tmpdata->data_yc_h*256+tmpdata->data_yc_l;
		make_yc_real_data(i+data_begin_addr,yc_data,1,&time);
	}
	return 0;
}


//YC���ʹ���end///////////////////////////////////////////////////////////////////////////////////////

//KWH���ʹ���begin///////////////////////////////////////////////////////////////////////////////////////
int Modbust::deal_sequence_M_IT_NA_1()		//0x10���
{

	MODBUST_APDU *apdu_104 = (MODBUST_APDU*)m_apdu_buf;
	unsigned short int data_begin_addr = apdu_104->apci.data_begin_addr_h*256+apdu_104->apci.data_begin_addr_l;

	if(apdu_104->apci.addr != m_common_addr)		//�жϹ�����ַ
	{
		return -1;
	}
	if(apdu_104->apci.type != M_ME_TA_1)		//�ж�����
	{
		return -1;
	}
	if(data_begin_addr != m_config_ptr->kwh_begin)	//ң����ʼ��ַΪ3000
	{
		return -1;
	}

	int datanum = (apdu_104->apci.data_num_h*256+apdu_104->apci.data_num_l)/2; //��ȡ��ǰ֡�ĵ�����ݸ���=�Ĵ�������/2
	int data_bytenum = apdu_104->apci.data_byte_num_h*256+apdu_104->apci.data_byte_num_l;	//�������ռ�ֽ���
	int data_no = 0;		//ң����

	MODBUST_TIME *data_time = (MODBUST_TIME*)(apdu_104->buf+data_bytenum); 
	FETIME time;			//ң�ⷢ��ʱ��
	time = ModbustTime_To_Realtime(data_time);
	for (int i=0;i<datanum;i++)
	{
		MODBUST_KWH* tmpdata= (MODBUST_KWH*)(apdu_104->buf+i*4);
		unsigned long ym_data = tmpdata->data_kwh_hh*256*256*256+tmpdata->data_kwh_hl*256*256+tmpdata->data_kwh_lh*256+tmpdata->data_kwh_ll;
		make_kwh_real_data(i,ym_data,1,&time);
	}
	return 0;
}
//KWH���ʹ���end///////////////////////////////////////////////////////////////////////////////////////


//����Լ���͵�ʱ��ת��Ϊ��ʵʱ��
FETIME ModbustTime_To_Realtime(MODBUST_TIME* mTime)
{
	FETIME Time ;
	Time.milisecond = 0;
	Time.second = mTime->secend;
	Time.minute = mTime->minite;
	Time.hour = mTime->hour;
	Time.day = mTime->day;
	Time.month = mTime->month;
	Time.year = mTime->year+100;	//��Լ���͵�ʱ����0f����2015��  FETIME��ʹ��115����2015

	return Time;
}

int Modbust::confirm_link()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = m_common_addr ;
	m_write_buf[ 1 ] = C_CG_TA_1 ;
	MODBUST_APDU *apdu_modbust = (MODBUST_APDU*)m_apdu_buf;
	m_write_buf[ 2 ] = apdu_modbust->apci.send_num_h;
	m_write_buf[ 3 ] = apdu_modbust->apci.send_num_l;
	ACE_OS::memcpy(m_write_buf+4,CONF_LK_VAL,sizeof(CONF_LK_VAL));//4���ֽڹ̶���

	FETIME tim;
	get_time(&tim);
	m_write_buf[8] = tim.year-100;	//FETIME.year��ʵ�����1900�꣬�·���ȥ������2000��
	m_write_buf[9] = tim.month;
	m_write_buf[10] = tim.day;
	m_write_buf[11] = tim.hour;
	m_write_buf[12] = tim.minute;
	m_write_buf[13] = tim.second;

	unsigned short crc = CRC16( m_write_buf , 14 ) ;
	m_write_buf[14] = HIBYTE(crc);
	m_write_buf[15] = LOBYTE(crc);

	m_write_lenth = CONFIRM_LINK_LEN;

	return send_msg();
}

int Modbust::call_yc_data()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = m_common_addr ;
	m_write_buf[ 1 ] = M_ME_TA_1 ;
	m_write_buf[ 2 ] = HIBYTE(m_config_ptr->yc_begin);
	m_write_buf[ 3 ] = LOBYTE(m_config_ptr->yc_begin);
	m_write_buf[ 4 ] = HIBYTE(m_yc_all_num);
	m_write_buf[ 5 ] = LOBYTE(m_yc_all_num);

	unsigned short crc = CRC16( m_write_buf , 6 ) ;
	m_write_buf[6] = LOBYTE(crc);
	m_write_buf[7] = HIBYTE(crc);

	m_write_lenth = CALL_DATA_LEN;

	return send_msg();
}

int Modbust::call_yx_data()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = m_common_addr ;
	m_write_buf[ 1 ] = M_SP_TA_1 ;
	m_write_buf[ 2 ] = HIBYTE(m_config_ptr->yx_begin);
	m_write_buf[ 3 ] = LOBYTE(m_config_ptr->yx_begin);
	m_write_buf[ 4 ] = HIBYTE(m_yx_all_num);
	m_write_buf[ 5 ] = LOBYTE(m_yx_all_num);

	unsigned short crc = CRC16( m_write_buf , 6 ) ;
	m_write_buf[6] = LOBYTE(crc);
	m_write_buf[7] = HIBYTE(crc);

	m_write_lenth = CALL_DATA_LEN;

	return send_msg();

}

int Modbust::call_ym_data()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = m_common_addr ;
	m_write_buf[ 1 ] = M_ME_TA_1 ;
	m_write_buf[ 2 ] = HIBYTE(m_config_ptr->kwh_begin);
	m_write_buf[ 3 ] = LOBYTE(m_config_ptr->kwh_begin);
	m_write_buf[ 4 ] = HIBYTE(m_pi_all_num);
	m_write_buf[ 5 ] = LOBYTE(m_pi_all_num);

	unsigned short crc = CRC16( m_write_buf , 6 ) ;
	m_write_buf[6] = LOBYTE(crc);
	m_write_buf[7] = HIBYTE(crc);

	m_write_lenth = CALL_DATA_LEN;

	return send_msg();
}

unsigned short Modbust::CRC16( unsigned char *arr_buff, int len)
{
	unsigned short crc =0xFFFF;
	unsigned short i, j;
	for ( j=0; j<len;j++)
	{
		printf("j=%d\n",j);
		crc=crc ^*arr_buff++;
		for ( i=0; i<8; i++)
		{
			if( ( crc&0x0001) >0)
			{
				crc=crc>>1;
				crc=crc^ 0xa001;
			}
			else
				crc=crc>>1;
		}
	}
	return ( crc);
}

int Modbust::send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag)
{
	return 1;
}
