/**
@file channelcluser.cpp
@brief ǰ��ͨ�����ʵ��

�ṩ�����ļ��أ�ͨ��ʵ���Ĵ����ȹ���

@author hongxiang 
@version 1.0.0
@date 2013-10-10
*/

#include "fes/channelchuser.h"
#include "fes/platformwrapper.h"
#include "fes/baseprotocol.h"
#include "fes/fesdatashm.h"
#include "fes/fesdatashm_allocator.h"
#include "fes/femailstruct.h"
#include <ace/os.h>
#include <ace/Log_Msg.h>
#include <vector>

using namespace std;

namespace
{
	const char* CHANNEL_FILE_NAME = "log/dat/channel.dat";
	const char* RTU_FILE_NAME = "log/dat/rtu.dat";
	const char* PROTOCAL_FILE_NAME = "log/dat/protocol.dat";
}

/**************************************************************************
���캯��
**************************************************************************/
Channel_chuser::Channel_chuser(ACE_Thread_Mutex& check_watch_load_mutex)
:m_check_watch_load_mutex(check_watch_load_mutex)
{
	m_channel_para = new CHANPARA[MAX_CHAN_NUM];	//����ͨ�������ṹ��
	m_rtu_para = new RTUPARA[MAX_RTU_NUM];			//�����ն˲����ṹ��

 	m_protocol = new Base_protocol*[MAX_CHAN_NUM];

	ACE_OS::memset(m_protocol, 0, MAX_CHAN_NUM*sizeof(Base_protocol*));		//modify by chris

	ACE_OS::memset(m_bnew_protocol, false, MAX_CHAN_NUM);

	generate_protocol_factory();					//������Լ����ʵ��
}


/**************************************************************************
��������
**************************************************************************/
Channel_chuser::~Channel_chuser()
{
	delete[] m_channel_para;
	m_channel_para = NULL;

	delete[] m_rtu_para;
	m_rtu_para = NULL;
}

/**************************************************************************
��ȡ��Լ���ͱ�������Լ����
**************************************************************************/
void Channel_chuser::generate_protocol_factory(void)
{
	struct PROTOCOL_
	{
		char protocol_name[CODE_LENTH]; //��Լ��
		char describe[LONG_DESC_LENTH]; //��Լ����
	};

	PROTOCOL_* db_protocol = NULL;
	Db_connector::open();
	int ret = Db_connector::select("select protocol_name, describe from protocol", (void **)&db_protocol);
	int rcd_cnt = ret; //��Լ��Ԫ������

	//��������
	if (ret > 0 && rcd_cnt > 0)
	{
		//�������ݿ��ж����Ĺ�Լ������������Լ����
		for (int i = 0; i < rcd_cnt; i++)
		{
			//������Լʵ��
			PROTOCOL_* protocol_in_db = db_protocol + i;
			Protocol_factory::instance()->add_protocol(protocol_in_db->protocol_name);
		}	

		//����PROTOCOL_�ṹ�嵽�ļ�
		char path[LONGCHAR_LEN];
		ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DEFAULT_ENV), PROTOCAL_FILE_NAME);
		FILE* fp = ACE_OS::fopen (path, "w+b");

		if (fp == NULL)
		{
			ACE_DEBUG((LM_ERROR, "���ļ�ʧ�ܣ�\n"));
		}

		else
		{
			ACE_OS::fwrite (db_protocol, sizeof(PROTOCOL_), rcd_cnt, fp);
			ACE_OS::fclose(fp);

			ACE_DEBUG((LM_DEBUG, "�����Լ�����������ļ��ɹ���\n"));
		}

	}//if (ret>0)
	else if(ret == 0)
	{
		ACE_DEBUG((LM_DEBUG, "���ݿ����޹�Լ������¼��\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "�����ݿ�ʧ�ܣ�\n"));
	}

	if (db_protocol)
	{
		free(db_protocol);
	}

}
/**************************************************************************
���ض�ȡ����
**************************************************************************/
int Channel_chuser::load(void)
{

	clear_para();

	read_protocol_config();	//��ȡ��Լ��������

	//�����ݿ�ʧ��
	if (read_channel_para_from_db() || read_rtu_para_from_db())
	{
		ACE_DEBUG((LM_ERROR, "�����ݿ��ȡ����ʧ�ܣ�ת�ӱ����ļ���ȡ������\n"));
		
		//���ļ�ʧ��
		if (read_channel_para_from_file() || read_rtu_para_from_file())
		{
			ACE_ERROR_RETURN((LM_ERROR, "��ȡ����ʧ�ܣ�\n"), -1);
		}
	}

	calc_channel_priority();	//����ͨ��������ϵ
	calc_rtu_priority();
	calc_rtu_channel();		//�����ն�ͨ����ϵ

	ACE_Guard<ACE_Thread_Mutex> guard (m_check_watch_load_mutex);
	check_channels(); //���ͨ��״��

	return 0;
}

/**************************************************************************
���ʼ�д���Լ����Ϣ����
**************************************************************************/
int Channel_chuser::put_mail(void* mail, const int& lenth)
{
	FE_MAIL_STRUCT* mail_msg = (FE_MAIL_STRUCT*)mail;

	FE_YK_MAIL* yk_mail = (FE_YK_MAIL*)mail_msg->mail_body;

	int yk_rtu_no = yk_mail->rtu_no;

	int yk_channel_no = (m_rtu_para+yk_rtu_no)->last_channel;		//��ȡ��Ӧrtuno��ͨ����

	if (yk_mail->action == 0xCC)		//add by chris ת��
	{
		yk_mail->action = 1;
	}
	else if (yk_mail->action == 0x33)
	{
		yk_mail->action = 0;
	}

	ACE_Message_Block *protocol_msg = 0;
	protocol_msg = new ACE_Message_Block(lenth);
	ACE_OS::memcpy(protocol_msg->wr_ptr(), mail, lenth);

	int current_channel_host = (Fes_shm::get().chan_data_ptr + yk_channel_no)->curr_attend_host; //ң��ͨ����ǰֵ������
	int current_host = Fes_shm::get().fe_sys_data_ptr->host_no;		//�������
	if (current_channel_host == current_host)
	{
		if (m_protocol[yk_channel_no])
		{
			m_protocol[yk_channel_no]->putq(protocol_msg);
		}
	}
	return 0;
}

/**************************************************************************
��ȡ��Լ��������
**************************************************************************/
void Channel_chuser::read_protocol_config(void)
{
	Protocol_factory::instance()->get_all_config();
}

/**************************************************************************
��ȡͨ������
**************************************************************************/
int Channel_chuser::read_channel_para_from_db(void)
{
	struct CHANNEL_
	{
		unsigned short channel_no; //ͨ����
		char describe[SHORT_DESC_LENTH]; //ͨ������
		char protocol_name[CODE_LENTH];  //��Լ��
		short parent_channel; //��ͨ����
		unsigned short common_address; //������ַ		//add by chris����ͨ��������ַ����վ��ַ���ű�����Ӧ�Ķ�
		unsigned short master_address; //��վ��ַ
		unsigned char is_check; //ͨ������־λ
		unsigned char is_gps; //GPS��־λ
		unsigned char initial_stat; //ͨ����ʼ��״̬
		unsigned short stop_gap; //ͨ��ֹͣʱ��
		unsigned short bit_rate_error; //������
		unsigned short default_attend_host; //<Ĭ��ֵ����ڵ㣬�������������д-1������ϵͳ�Զ�����Ĭ��ֵ����ڵ�
		unsigned char dev_type; //�豸����
		char local_dev[DEV_NAME_LEN]; //�����豸��ַ
		unsigned short local_port; //���ض˿ں�
		char peer_dev[DEV_NAME_LEN]; //�����豸��ַ
		unsigned short peer_port; //�����豸�˿�
		char back_peer_dev[DEV_NAME_LEN]; //���籸���豸��ַ
		unsigned short back_peer_port; //���籸���豸�˿�
		unsigned short band_rate; //������
		unsigned char check_bit; //У��λ
		unsigned char stop_bit; //ֹͣλ
		unsigned char data_bit; //����λ
		unsigned char flow; //������
		unsigned char valid; //ͨ��ʹ�ñ�־
	};

	CHANNEL_* db_channel = NULL;
	int rcd_cnt = Db_connector::select("select * from channel order by  channel_no asc", (void **)&db_channel); 

	//��ȡͨ����ɹ���������д�뱾���ļ�
	if (rcd_cnt > 0)
	{	
		//�������ն˺ŵ��ն˲�����ֵ����Ӧ���ն˲����ṹ��Ԫ��
		for (int i = 0; i < rcd_cnt; i++)
		{
			CHANNEL_* db_para = db_channel + i;
			int channel_no = db_channel[i].channel_no; //��ȡͨ����

			if (channel_no >= 0 && channel_no < MAX_CHAN_NUM)
			{
				CHANPARA* para = m_channel_para + channel_no;

				para->order = db_para->channel_no;
				ACE_OS::strcpy(para->desdribe,db_para->describe);
				ACE_OS::strcpy(para->protocol,db_para->protocol_name);
				para->gps = db_para->is_gps;
				para->check = db_para->is_check;
				para->valid = db_para->valid;
				para->last_channel = db_para->parent_channel;
				para->common_addr = db_para->common_address;		//add by chris
				para->master_addr = db_para->master_address;
				para->dev_type = db_para->dev_type;
				ACE_OS::strcpy(para->local_dev,db_para->local_dev);
				para->local_port = db_para->local_port;
				ACE_OS::strcpy(para->dev_name,db_para->peer_dev);
				para->port = db_para->peer_port;
				ACE_OS::strcpy(para->backup_dev_name,db_para->back_peer_dev);
				para->back_up_port = db_para->back_peer_port;
				para->band = db_para->band_rate;
				para->check_bit = db_para->check_bit;
				para->stop_bit = db_para->stop_bit;
				para->data_bit = db_para->data_bit;
				para->flow = db_para->flow;
				para->initial_stat = db_para->initial_stat;
				para->stop_time_limit = db_para->stop_gap;
				para->bit_error_rate = db_para->bit_rate_error;
				para->default_attend_host = db_para->default_attend_host;
			}
		}

		save_channel_para();  //����ͨ�������������ļ�
	}
	else if (rcd_cnt == 0)
	{
		ACE_DEBUG((LM_ERROR, "���ݿ�����ͨ��������¼��\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "��ȡͨ�������ʧ�ܣ�\n"));
	}

	if (db_channel)
	{
		free(db_channel);
	}

	return rcd_cnt > 0 ? 0 : -1;
}

/**************************************************************************
��ȡ�ն˲���
**************************************************************************/
int Channel_chuser::read_rtu_para_from_db(void)
{
	struct RTU_
	{ 
		unsigned short rtu_no; //�ն����
		char rtu_code[CODE_LENTH]; //�ն˴���
		char describe[SHORT_DESC_LENTH]; //�ն���
		unsigned short channel_no; //ͨ����
		unsigned short yc_num; //ң����Ŀ
		unsigned short yx_num;//ң����Ŀ
		unsigned short kwh_num; //�������Ŀ
		unsigned char valid; //ʹ�ñ�־
	};

	RTU_* db_rtu = NULL;
	int rcd_cnt = Db_connector::select("select * from rtu order by rtu_no asc",(void **)&db_rtu);
	
	//��ȡ�ն˱�ɹ���������д�뱾���ļ�
	if (rcd_cnt > 0)
	{
		//���������ն˲��������ն˲����ṹ��
		for (int i = 0; i < rcd_cnt; i++)
		{
			RTU_* db_para = db_rtu + i; 
			int rtu_no = db_rtu[i].rtu_no; //��ȡ�ն˺�

			if (rtu_no >= 0 && rtu_no < MAX_CHAN_NUM)
			{
				//�������ն˺ŵ��ն˲�����ֵ����Ӧ���ն˲����ṹ��Ԫ��
				RTUPARA* para = m_rtu_para + rtu_no;

				para->order = db_para->rtu_no;
				para->last_channel = db_para->channel_no;
				ACE_OS::strcpy(para->rtu_code,db_para->rtu_code);
				ACE_OS::strcpy(para->desdribe,db_para->describe);
				para->yc_num = db_para->yc_num;
				para->yx_num = db_para->yx_num;
				para->kwh_num = db_para->kwh_num;
				para->valid = db_para->valid;
			}

		}//for (int i = 0; i<rcd_cnt; i++)

		save_rtu_para(); //�����ն˲����������ļ� 		
	}
	else if (rcd_cnt == 0)
	{
		ACE_DEBUG((LM_ERROR, "���ݿ������ն˲�����¼��\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "��ȡ�ն˱����ʧ�ܣ�\n"));
	}

	if (db_rtu)
	{
		free(db_rtu);
	}

	return rcd_cnt > 0 ? 0 : -1;
}
/**************************************************************************
��ȡң�����
**************************************************************************/
int Channel_chuser::read_yc_para_from_db(void)
{
	struct YC_PARA_DB
	{

		double slope;	//б��
		double ordinate;//�ؾ�
		unsigned short  ter_no;//�ն����
		unsigned short  yc_no;//ң�����


	};

	YC_PARA_DB* db_yc_para = NULL;
	vector<YC_PARA> chan_buffer; 
	CHANPARA * chan_para = NULL;
	vector<YC_PARA_DB> yc_buffer_db;
	vector<YC_PARA> yc_buffer;
	YC_PARA_DB tem_yc_buffer;
	YC_PARA tem_yc;
	int rtu_no = 0;
	int pos = 0;
	int current_yc_num = 0;
	Fes_data_shm_allocator &m_fes_data_shm_allocator = Fes_data_shm_allocator::get();

	int rcd_cnt = Db_connector::select("select б��/����,�ؾ�,�ն����,ң����� from ң������� order by �ն����, ң����� asc ",(void **)&db_yc_para);
	int rcd_act_num = 0;
	bool isend = false;

	if (rcd_cnt > 0)
	{
		for (int i = 0; i < MAX_CHAN_NUM; i++)
		{
			
			chan_para = m_channel_para + i;
			rtu_no = chan_para->child_rtu;
			if (chan_para->valid == 0)
			{
				continue;
			}
// 			m_fes_data_shm_allocator.alloc_channel_data(i);
			if (chan_para->yc_num <= 0)
			{
				continue;
			}
			chan_buffer.resize(chan_para->yc_num);
			pos = 0;
			while (true)
			{    
				current_yc_num = (m_rtu_para + rtu_no)->yc_num;
				if (current_yc_num <= 0)
				{
					rtu_no = (m_rtu_para + rtu_no)->next_rtu;
					continue;
				}

				isend = false;
				for (int j = 0; j < rcd_cnt; j++)
				{
					if ((db_yc_para + j)->ter_no == rtu_no)
					{
						yc_buffer_db.push_back(*(db_yc_para + j));
						isend = true;

					}
					else if (isend)
					{
						break;

					}
				}
				
				rcd_act_num = yc_buffer_db.size();
				if (rcd_act_num > 0)
				{

					for (int j = 0; j < rcd_act_num ; j++)
					{
						for (int k = rcd_act_num - 1; k > j; k--)
						{

							if (yc_buffer_db[k].yc_no < yc_buffer_db[k-1].yc_no)
							{
								tem_yc_buffer = yc_buffer_db[k];
								yc_buffer_db[k] = yc_buffer_db[k-1];
								yc_buffer_db[k-1] = tem_yc_buffer;
							}
						}
						tem_yc.ordinate = yc_buffer_db[j].ordinate;
						tem_yc.slope = yc_buffer_db[j].slope;
						yc_buffer.push_back(tem_yc);
					}//for (int j = 0; j < rcd_act_num ; j++)
					//�Ե�ǰ�ն����ø���Ϊ׼����ʵ��YC���������ն����ø�����������YC
					if(rcd_act_num > current_yc_num)
					{
						rcd_act_num = current_yc_num;
					}
					memcpy(&chan_buffer[pos],&yc_buffer[0],rcd_act_num * sizeof(YC_PARA));
				}
				rtu_no = (m_rtu_para + rtu_no)->next_rtu;
				pos += current_yc_num;
				yc_buffer.clear();
				yc_buffer_db.clear();
				if (rtu_no == chan_para->child_rtu)
				{
					break;
				}

			}
			m_fes_data_shm_allocator.set_yc_para(i,&chan_buffer[0],chan_para->yc_num * sizeof(YC_PARA));
			chan_buffer.clear();
		}//for (int i = 0; i < MAX_CHAN_NUM; i++)
	}
	else if (rcd_cnt == 0)
	{
		ACE_DEBUG((LM_ERROR, "���ݿ�����ң�������¼��\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "��ȡң����������ʧ�ܣ�\n"));

	}
	return rcd_cnt > 0 ? 0 : -1;



}

/**************************************************************************
��ȡң�Ų���
**************************************************************************/
int Channel_chuser::read_yx_para_from_db(void)
{
	struct YX_PARA_DB
	{
		unsigned char negate;	//ȡ����0Ϊ��ȡ����1Ϊȡ��
		unsigned short ter_no;//�ն����
		unsigned short  yx_no;//ң�����

	};
	YX_PARA_DB* db_yx_para = NULL;
	vector<YX_PARA> chan_buffer; 
	CHANPARA * chan_para = NULL;
	vector<YX_PARA_DB> yx_buffer_db;
	vector<YX_PARA> yx_buffer;
	YX_PARA_DB tem_yx_buffer;
	YX_PARA tem_yx;
	int rtu_no = 0;
	int pos = 0;
	int current_yx_num = 0;
	Fes_data_shm_allocator &m_fes_data_shm_allocator = Fes_data_shm_allocator::get();
	int rcd_cnt = Db_connector::select("select ȡ����־,�ն����,ң����� from ң�Ų����� order by �ն����, ң����� asc ",(void **)&db_yx_para);		//modidfy by chris
	int rcd_act_num = 0;
	bool isend = false;
	if (rcd_cnt > 0)
	{
		for (int i = 0; i < MAX_CHAN_NUM; i++)
		{
			chan_para = m_channel_para + i;
			rtu_no = chan_para->child_rtu;
			if (chan_para->valid == 0)
			{
				continue;
			}
			if (chan_para->yx_num <= 0)
			{
				continue;
			}
			chan_buffer.resize(chan_para->yx_num);
			pos = 0;
			while (true)
			{    
				current_yx_num = (m_rtu_para + rtu_no)->yx_num;
				if (current_yx_num <= 0)
				{
					rtu_no = (m_rtu_para + rtu_no)->next_rtu;
					continue;
				}

				isend = false;
				for (int j = 0; j < rcd_cnt; j++)
				{
					if ((db_yx_para + j)->ter_no == rtu_no)
					{
						yx_buffer_db.push_back(*(db_yx_para + j));
						isend = true;

					}
					else if (isend)
					{
						break;

					}
				}

				rcd_act_num = yx_buffer_db.size();
				if (rcd_act_num > 0)
				{

					for (int j = 0; j < rcd_act_num ; j++)
					{
						for (int k = rcd_act_num - 1; k > j; k--)
						{

							if (yx_buffer_db[k].yx_no < yx_buffer_db[k-1].yx_no)
							{
								tem_yx_buffer = yx_buffer_db[k];
								yx_buffer_db[k] = yx_buffer_db[k-1];
								yx_buffer_db[k-1] = tem_yx_buffer;
							}
						}
						tem_yx.negate = yx_buffer_db[j].negate;
						yx_buffer.push_back(tem_yx);
					}
					//�Ե�ǰ�ն����ø���Ϊ׼����ʵ��YX���������ն����ø�����������YX
					if(rcd_act_num > current_yx_num)
					{
						rcd_act_num = current_yx_num;
					}
					memcpy(&chan_buffer[pos],&yx_buffer[0],rcd_act_num * sizeof(YX_PARA));
				}
				rtu_no = (m_rtu_para + rtu_no)->next_rtu;
				pos += current_yx_num;
				yx_buffer.clear();
				yx_buffer_db.clear();
				if (rtu_no == chan_para->child_rtu)
				{
					break;
				}

			}
  			m_fes_data_shm_allocator.set_yx_para(i,&chan_buffer[0],chan_para->yx_num * sizeof(YX_PARA));
			chan_buffer.clear();
		}//for (int i = 0; i < MAX_CHAN_NUM; i++)
	}
	else if (rcd_cnt == 0)
	{
		ACE_DEBUG((LM_ERROR, "���ݿ�����ң�Ų�����¼��\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "��ȡң�Ų��������ʧ�ܣ�\n"));

	}

	return rcd_cnt > 0 ? 0 : -1;
}

/**************************************************************************
��ȡ���������
**************************************************************************/
int Channel_chuser::read_kwh_para_from_db(void)
{
	struct KWH_PARA_DB
	{
		double full_code;	//����
		double slope;		//б�� = ����/���
		unsigned short  ter_no;//�ն����
		unsigned short  kwh_no;//������

	};
	KWH_PARA_DB* db_kwh_para = NULL;
	vector<KWH_PARA> chan_buffer; 
	CHANPARA * chan_para = NULL;
	vector<KWH_PARA_DB> kwh_buffer_db;
	vector<KWH_PARA> kwh_buffer;
	KWH_PARA_DB tem_kwh_buffer;
	KWH_PARA tem_kwh;
	int rtu_no = 0;
	int pos = 0;
	int current_kwh_num = 0;
	Fes_data_shm_allocator &m_fes_data_shm_allocator = Fes_data_shm_allocator::get();
	int rcd_cnt = Db_connector::select("select ����,����/���,�ն����,������ from ��Ȳ����� order by �ն����, ������ asc ",(void **)&db_kwh_para);
	int rcd_act_num = 0;
	bool isend = false;
	if (rcd_cnt > 0)
	{
		for (int i = 0; i < MAX_CHAN_NUM; i++)
		{
			chan_para = m_channel_para + i;
			rtu_no = chan_para->child_rtu;
			if (chan_para->valid == 0)
			{
				continue;
			}
			if (chan_para->kwh_num <= 0)
			{
				continue;
			}
			chan_buffer.resize(chan_para->kwh_num);
			pos = 0;
			while (true)
			{    
				current_kwh_num = (m_rtu_para + rtu_no)->kwh_num;
				if (current_kwh_num <= 0)
				{
					rtu_no = (m_rtu_para + rtu_no)->next_rtu;
					continue;
				}

				isend = false;
				for (int j = 0; j < rcd_cnt; j++)
				{
					if ((db_kwh_para + j)->ter_no == rtu_no)
					{
						kwh_buffer_db.push_back(*(db_kwh_para + j));
						isend = true;

					}
					else if (isend)
					{
						break;

					}
				}

				rcd_act_num = kwh_buffer_db.size();
				if (rcd_act_num > 0)
				{

					for (int j = 0; j < rcd_act_num ; j++)
					{
						for (int k = rcd_act_num - 1; k > j; k--)
						{

							if (kwh_buffer_db[k].kwh_no < kwh_buffer_db[k-1].kwh_no)
							{
								tem_kwh_buffer = kwh_buffer_db[k];
								kwh_buffer_db[k] = kwh_buffer_db[k-1];
								kwh_buffer_db[k-1] = tem_kwh_buffer;
							}
						}

						tem_kwh.full_code = kwh_buffer_db[j].full_code;
						tem_kwh.slope = kwh_buffer_db[j].slope;
						kwh_buffer.push_back(tem_kwh);
					}

					//�Ե�ǰ�ն����ø���Ϊ׼����ʵ��KWH���������ն����ø�����������KWH
					if(rcd_act_num > current_kwh_num)
					{
						rcd_act_num = current_kwh_num;
					}
					memcpy(&chan_buffer[pos],&kwh_buffer[0],rcd_act_num * sizeof(KWH_PARA));
				}
				rtu_no = (m_rtu_para + rtu_no)->next_rtu;
				pos += current_kwh_num;
				kwh_buffer.clear();
				kwh_buffer_db.clear();
				if (rtu_no == chan_para->child_rtu)
				{
					break;
				}

			}

			m_fes_data_shm_allocator.set_kwh_para(i,&chan_buffer[0],chan_para->kwh_num * sizeof(KWH_PARA));
			chan_buffer.clear();
		}//for (int i = 0; i < MAX_CHAN_NUM; i++)
	}
	else if (rcd_cnt == 0)
	{
		ACE_DEBUG((LM_ERROR, "���ݿ����޵�Ȳ�����¼��\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "��ȡ��Ȳ��������ʧ�ܣ�\n"));

	}
	return rcd_cnt > 0 ? 0 : -1;

}

/**************************************************************************
���ͨ������
**************************************************************************/
void Channel_chuser::check_channels(void)
{
	//	CHANPARA* 
	CHANPARA* m_shm_para = Fes_shm::get().chan_para_ptr;
	Fes_data_shm_allocator &m_fes_data_shm_allocator = Fes_data_shm_allocator::get();

	CHANDATA* m_shm_chandata = Fes_shm::get().chan_data_ptr;
	int channel_no;

	for (channel_no = 0; channel_no < MAX_CHAN_NUM; channel_no++)
	{

		CHANPARA* para = m_channel_para + channel_no;

		CHANPARA* shm_para = m_shm_para + channel_no;
// 		Base_protocol* protocol = m_protocol[channel_no];

		//�빲���ڴ������жԱ�...
		if (memcmp(para,shm_para,sizeof(CHANPARA)) == 0)	//��ͬ��pass
		{
			continue;
		}
		else			//�����ͬ���������˼��أ���д�빲���ڴ�,��ɾ��ͨ��ʵ��
		{
			ACE_OS::memcpy(shm_para,para,sizeof(CHANPARA));
			if (m_protocol[channel_no])
			{
				delete m_protocol[channel_no];
				m_protocol[channel_no] = NULL;
				m_fes_data_shm_allocator.recycle_channel_data(channel_no);
			}
		}

		int rtu_no = para->child_rtu;		//rtu��
		RTUPARA *rtu_para = NULL;		//����rtupara�ڴ�
		RTUPARA *m_shm_rtu_para = Fes_shm::get().rtu_para_ptr;		//����rtupara�ڴ�
		RTUPARA *shm_rtu_para = NULL;		//����rtupara�ڴ�
		while(true)
		{
			rtu_para = m_rtu_para + rtu_no;
			shm_rtu_para = m_shm_rtu_para + rtu_no;
			memcpy(shm_rtu_para,rtu_para,sizeof(RTUPARA));
			rtu_no = rtu_para->next_rtu;
			if (rtu_no == para->child_rtu)		//��������һ���ˣ�����rtupara��������
			{
				break;
			}

		}

		//�Ƿ���Ҫ�����¶���
		if (para->valid == 0)
		{
			continue;
		}
		else
		{
			//д��CHANDATA��ʼֵ,����Ĭ��ֵ��ڵ�
			int attend_host = m_shm_para[channel_no].default_attend_host;
			calc_hostno_enale(attend_host);//���ڵ���Ƿ�ϸ�
// 			m_shm_chandata[channel_no].curr_attend_host = attend_host;

			m_protocol[channel_no] = Protocol_factory::instance()->get_instance(para->protocol,channel_no);
			//���ʵ������
			if (m_protocol[channel_no] != NULL)
			{
				m_fes_data_shm_allocator.alloc_channel_data(channel_no);
				//todo ���ظ�ͨ����YX��YC��KWH����
				m_bnew_protocol[channel_no] = true;
			}	
		}
	}	//end first loop

	read_yc_para_from_db();
	read_yx_para_from_db();
	read_kwh_para_from_db();

	for (channel_no = 0; channel_no < MAX_CHAN_NUM; channel_no++)
	{
		CHANPARA* para = m_channel_para + channel_no;
		if (m_protocol[channel_no] && m_bnew_protocol[channel_no])
		{
			m_protocol[channel_no]->open();
			m_bnew_protocol[channel_no] = false;
		}
	}//end second loop
}
 
/**************************************************************************
���ļ��ж�ȡͨ������
**************************************************************************/
int Channel_chuser::read_channel_para_from_file(void)
{
	char path[LONGCHAR_LEN];
    ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DEFAULT_ENV), CHANNEL_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "rb");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "���ļ�ʧ�ܣ�\n"),-1);
	}

	ACE_OS::fread(m_channel_para, sizeof(CHANPARA), MAX_CHAN_NUM, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_DEBUG, "���ļ���ȡͨ�������ɹ���\n"));

	return 0; 
}

/**************************************************************************
���ļ��ж�ȡ�ն˲���
**************************************************************************/
int Channel_chuser::read_rtu_para_from_file(void)
{
	
	char path[LONGCHAR_LEN];

	ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DEFAULT_ENV), RTU_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "rb");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "���ļ�ʧ�ܣ�\n"), -1);
	}

	ACE_OS::fread(m_rtu_para, sizeof(RTUPARA), MAX_RTU_NUM, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_ERROR, "���ļ���ȡ�ն˲����ɹ���\n"));

	return 0;
}

/**************************************************************************
����ͨ�����ȼ�
**************************************************************************/
void Channel_chuser::calc_channel_priority(void)
{
	
	vector<int> same_channel_pos;
	vector<int> exist_parent_channel;
	vector<int> total_cope_channel;
	int parent_channel = 0;
	bool is_exist = false;
	bool is_marked = false;		//�Ƿ��Ѵ����
	int same_size = 0;
	int exist_size = 0;

	CHANDATA* chandata = Fes_shm::get().chan_data_ptr;
	GPS_INFO* gps_info = Fes_shm::get().gps_info_ptr;
	for (int i = 0; i < MAX_CHAN_NUM; i++)
	{
		(m_channel_para + i)->next_channel = -1;	//����ͨ�����ȼ�֮ǰ,����next_channelȫ����-1���ɼ������д
		if ((m_channel_para + i)->initial_stat == 1)//��ʼ��ͨ����ʼstat
		{
			chandata[i].stat = CHANRUN;
		}
		else
		{
			chandata[i].stat = CHANSTOP;	
		}

		//(m_channel_para+ i)->child_rtu = -1;//��ʼ��ͨ�����׸��ն˺�,-1��ʾû���ն�
		if ((m_channel_para + i)->valid == 0)
		{
			continue;

		}
//todo
// 		chandata[i].auto_switch = 1;
		if ((m_channel_para + i)->last_channel == -1)	//��ͨ��Ϊ��ͨ��
		{
			chandata[i].master_flag = 1;
		}

		//ͨ����ΪGPSͨ����ͨ���Ĵ���ʽ todo
// 		if ((m_channel_para + i)->gps == 1)
// 		{
// 			
// 		}

		/*�жϸ�ͨ���Ƿ񱻴���*/
		is_marked = false;
		for (vector<int>::iterator j = total_cope_channel.begin(); j != total_cope_channel.end(); j++)
		{
			if (i == *j)
			{
				total_cope_channel.erase(j);
				is_marked = true;
				break;
			}
		}
		if (is_marked)
		{
			continue;
		}


		exist_size = exist_parent_channel.size();
		parent_channel = (m_channel_para + i)->last_channel;

		/*��ʼ�����ȼ���ߵ�ͨ����ʹ��next_channel = -1,�ٶ���ͨ��û�б���ͨ��*/
		if (parent_channel == -1)
		{
// 			(m_channel_para + i)->next_channel = -1;
			continue;
		}

		/*�жϸ�ͨ���ǲ������ȼ���ߵ�ͨ��*/
		is_exist = false;
		for (int j = 0; j < exist_size; j++)
		{
			if (parent_channel == exist_parent_channel.at(j)) 
			{
				is_exist = true;
				break;

			}

		}
		if (is_exist)//�������ȼ���ߵ�ͨ��
		{
			continue;
		}
		/*�����ȼ���ߵ�ͨ������¼��ͨ��*/
		exist_parent_channel.push_back(parent_channel);
        
		/*��¼��ͨ���ı���ͨ��*/
		for (int j = i; j < MAX_CHAN_NUM; j++)
		{
			if ((m_channel_para + j)->valid == 0)
			{
				continue;
			}
			if ((m_channel_para + j)->last_channel == parent_channel)
			{
				same_channel_pos.push_back(j);
				total_cope_channel.push_back(j);
			}

		}

		/*�����ȼ���ߵ�ͨ�����䱸��ͨ������������*/
		same_size = same_channel_pos.size();
		if (same_size == 1)
		{
			(m_channel_para + parent_channel)->last_channel = -1;
			(m_channel_para + parent_channel)->next_channel = same_channel_pos.at(0);
			(m_channel_para + same_channel_pos.at(0))->last_channel = parent_channel;
			(m_channel_para + same_channel_pos.at(0))->next_channel = -1;

		}
		else if(same_size > 1)
		{
			(m_channel_para + parent_channel)->last_channel = -1;
			(m_channel_para + parent_channel)->next_channel = same_channel_pos.at(0);
			(m_channel_para + same_channel_pos.at(0))->last_channel = parent_channel;
			(m_channel_para + same_channel_pos.at(0))->next_channel = same_channel_pos.at(1);
			for (int j = 1; j < same_size - 1; j++)
			{
				(m_channel_para + same_channel_pos.at(j))->last_channel = same_channel_pos.at(j - 1);
				(m_channel_para + same_channel_pos.at(j))->next_channel = same_channel_pos.at(j + 1);

			}

			(m_channel_para + same_channel_pos.at(same_size - 1))->last_channel = same_channel_pos.at(same_size - 2);
			(m_channel_para + same_channel_pos.at(same_size - 1))->next_channel = -1;

		}
		else
		{
			(m_channel_para + parent_channel)->last_channel = -1;
			(m_channel_para + parent_channel)->next_channel = -1;
		}
		
		same_channel_pos.clear();
	}//for (int i = 0; i < MAX_CHAN_NUM; i++)
}

/**************************************************************************
�����ն����ȼ�
**************************************************************************/
void Channel_chuser::calc_rtu_priority( void )
{
	vector<int> same_rtu_pos;			//����ͳһͨ���������ն˵��ն˺�
	vector<int> dealt_channel;			//�Ѵ���ͨ������
	int parent_channel = 0;
	bool already_dealt = false;
	int same_size = 0;
	int exist_size = 0;
	for (int i = 0; i < MAX_RTU_NUM; i++)		//��rtu����Ϊ����,ͨ������С��Ϊĳͨ���ĵ�һ���ն�
	{
		if ((m_rtu_para + i)->valid == 0)
		{
			continue;
		}

		exist_size = dealt_channel.size();

		parent_channel = (m_rtu_para + i)->last_channel;
		already_dealt = false;
		for (int j = 0; j < exist_size; j++)
		{
			if (parent_channel == dealt_channel.at(j)) 
			{
				already_dealt = true;
				break;

			}

		}
		if (already_dealt)//�Ѿ��������rtu�Ĺ�ϵ������
		{
			continue;
		}

        /*δ�����rtu����rtuӦΪ��ͨ���ĵ�һ���նˣ�����ͨ�������Ѵ������*/

		dealt_channel.push_back(parent_channel);
		/*��¼���ڸ�ͨ���������ն�*/
		for (int j = i; j < MAX_RTU_NUM; j++)
		{
			if ((m_rtu_para + j)->valid == 0)
			{
				continue;
			}
			if ((m_rtu_para + j)->last_channel == parent_channel)
			{
				same_rtu_pos.push_back(j);
			}


		}

		if (same_rtu_pos.empty())
		{
			continue;

		}

		same_size = same_rtu_pos.size();
		/*���ն���������*/
		for (int j = 0; j < same_size - 1; j++)
		{

			(m_rtu_para + same_rtu_pos.at(j))->next_rtu = same_rtu_pos.at(j + 1);

		}

		(m_rtu_para + same_rtu_pos.at(same_size - 1))->next_rtu = same_rtu_pos.at(0);
		same_rtu_pos.clear();
	}//for (int i = 0; i < MAX_RTU_NUM; i++)
}

/**************************************************************************
�����ն�ͨ����ϵ
**************************************************************************/
void Channel_chuser::calc_rtu_channel(void)
{
	
	vector<int> exist_parent_channel;
	int current_channel = 0;
	bool is_exist = false;
	int exist_size = 0;
	CHANPARA* chanpara = NULL;

	for (int i = 0; i < MAX_RTU_NUM; i++)
	{
		if ((m_rtu_para + i)->valid == 0)
		{
			continue;
		}

        /*�ж�����ĳ��ͨ���ĵ�һ���ն˺�*/
		exist_size = exist_parent_channel.size();
		current_channel = (m_rtu_para + i)->last_channel;
		is_exist = false;
		for (vector<int>::iterator j = exist_parent_channel.begin(); j != exist_parent_channel.end(); j++)
		{
			if (current_channel == *j) 
			{
				is_exist = true;
				break;
			}

		}
		/*���ǵ�һ���ն˺�*/
		if (is_exist)
		{
			continue;
		}

		(m_channel_para + current_channel)->child_rtu = i;//����ͨ����child_rtu��ֵΪ��һ���ն˺�

		chanpara = m_channel_para + current_channel; 
		while(true)	//�б�ͨ������ͨ������ͨ��rtu��ϵ��д��
		{
			chanpara->child_rtu = i;
			if (chanpara->next_channel == -1)
			{
				break;
			}
			chanpara = m_channel_para + chanpara->next_channel;
		}

	exist_parent_channel.push_back(current_channel);
	}//for (int i = 0; i < MAX_RTU_NUM; i++)
	int total_yc = 0;
	int total_yx = 0;
    int total_kwh = 0;
	RTUPARA *tem_rtu_para = NULL;
	/*����ͨ���ܵ�ң�ţ�ң�⣬�����*/
	for (int i = 0; i < MAX_CHAN_NUM; i++)
	{
		if ((m_channel_para + i)->valid == 0)
		{
			continue;
		}
		tem_rtu_para = m_rtu_para + (m_channel_para + i)->child_rtu;
		total_yc = 0;
		total_yx = 0;
		total_kwh = 0;

		while (true)
		{
			total_yc += tem_rtu_para->yc_num;
			total_yx += tem_rtu_para->yx_num;
			total_kwh += tem_rtu_para->kwh_num;
			if(tem_rtu_para->next_rtu == (m_channel_para + i)->child_rtu)
			{
				break;
			}
			tem_rtu_para = m_rtu_para + tem_rtu_para->next_rtu;

		}//while (true)
		(m_channel_para + i)->yc_num = total_yc;
		(m_channel_para + i)->yx_num = total_yx;
		(m_channel_para + i)->kwh_num = total_kwh;

	}//for (int i = 0; i < MAX_CHAN_NUM; i++)


}

/**************************************************************************
����ͨ������
**************************************************************************/
int Channel_chuser::save_channel_para(void)
{
	char path[LONGCHAR_LEN];
    ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DEFAULT_ENV), CHANNEL_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "w+b");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "���ļ�ʧ�ܣ�\n"), -1);
	}

	ACE_OS::fwrite(m_channel_para, sizeof(CHANPARA), MAX_CHAN_NUM, fp);
	ACE_OS::fclose(fp);
		
	ACE_DEBUG((LM_ERROR, "����ͨ�������������ļ��ɹ���\n"));
	return 0; 
}

/**************************************************************************
�����ն˲���
**************************************************************************/
int Channel_chuser::save_rtu_para(void)
{
	char path[LONGCHAR_LEN];
    ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DEFAULT_ENV), RTU_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "w+b");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "���ļ�ʧ�ܣ�\n"), -1);
	}

	ACE_OS::fwrite(m_rtu_para, sizeof(RTUPARA), MAX_RTU_NUM, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_ERROR, "�����ն˲����������ļ��ɹ���\n"));
	return 0; 
}

/**************************************************************************
����GPS����
**************************************************************************/
void Channel_chuser::set_gps_para(void)
{
	//todo

}

/**************************************************************************
����Ĭ�ϵ�ͨ������
**************************************************************************/
void Channel_chuser::clear_para(void)
{
	ACE_OS::memset(m_channel_para, 0, sizeof(CHANPARA) * MAX_CHAN_NUM);
	for(int channel_no = 0; channel_no < MAX_CHAN_NUM; channel_no++)	//��ɽṹ�嶨���ʼ��Ҫ��
	{
		m_channel_para[channel_no].default_attend_host = -1;
	}
	ACE_OS::memset(m_rtu_para, 0, sizeof(RTUPARA) * MAX_RTU_NUM);
}



void Channel_chuser::calc_hostno_enale(int& hostno)
{
	int ncnt;
	int nodes[MAX_FE_SRV_NUM];
	int node_num = Net_stat::get_fe_nodes(nodes, MAX_FE_SRV_NUM);
	int min_node = nodes[0];
	for( ncnt = 0; ncnt<node_num; ncnt++)
	{

		if (hostno == nodes[ncnt])
		{
			break;
		}
		else if (nodes[ncnt] < min_node)		//�Ƚ���С���
		{
			min_node = nodes[ncnt];
		}
	}

	if (ncnt == node_num)		//����ǰ����ڵ�,�������С��ǰ�ýӹ�
	{
		hostno = min_node;
	}
}