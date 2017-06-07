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
#include <ace/OS.h>
#include <ace/Log_Msg.h>
#include <vector>

using namespace std;

namespace
{
	const char* CHANNEL_FILE_NAME = "log/dat/channel.dat";
	const char* RTU_FILE_NAME = "log/dat/rtu.dat";
	const char* PROTOCAL_FILE_NAME = "log/dat/protocol.dat";
	const char* YC_FILE_NAME = "log/dat/fesyc.dat";
	const char* YX_FILE_NAME = "log/dat/fesyx.dat";
	const char* KWH_FILE_NAME = "log/dat/feskwh.dat";

}

/**************************************************************************
���캯��
**************************************************************************/
Channel_chuser::Channel_chuser(ACE_Thread_Mutex& check_watch_load_mutex)
:m_check_watch_load_mutex(check_watch_load_mutex)
{
	m_channel_para = new CHANPARA[MAX_CHAN_NUM];	//����ͨ�������ṹ��
	m_rtu_para = new RTUPARA[MAX_RTU_NUM];			//�����ն˲����ṹ��

	Fes_shm& fes_share_memory = Fes_shm::get();		//���������ڴ�
	Fes_data_shm_allocator &fes_data_shm_allocator = Fes_data_shm_allocator::get();	//�����ݹ����ڴ�

 	m_protocol = new Base_protocol*[MAX_CHAN_NUM];

	ACE_OS::memset(m_protocol, 0, MAX_CHAN_NUM*sizeof(Base_protocol*));		//modify by lujiashun

	ACE_OS::memset(m_bnew_protocol, false, MAX_CHAN_NUM);

	ACE_OS::memset(m_channel_para, 0, MAX_CHAN_NUM*sizeof(CHANPARA));
	ACE_OS::memset(m_rtu_para, 0, MAX_CHAN_NUM*sizeof(RTUPARA));


	//�����ڴ��Ҫ�ĳ�ʼ��
	for (int channel_no = 0; channel_no < MAX_CHAN_NUM; channel_no++)		//ǰ�ýڵ��Ϊ0���г�ͻ
	{
		fes_share_memory.chan_para_ptr[channel_no].default_attend_host = -1;
		fes_share_memory.chan_para_ptr[channel_no].next_channel = -1;
		fes_share_memory.chan_data_ptr[channel_no].stat = CHANSTOP;
		fes_share_memory.chan_data_ptr[channel_no].curr_attend_host = -1;
		fes_share_memory.chan_data_ptr[channel_no].master_flag = 0;
		fes_data_shm_allocator.inital_para(channel_no);
	}

	generate_protocol_factory();					//������Լ����ʵ��
}


/**************************************************************************
��������
**************************************************************************/
Channel_chuser::~Channel_chuser()
{

	for (int channel_no = 0; channel_no < MAX_CHAN_NUM; channel_no++)		//�رչ�Լ�߳�
	{
		if (m_protocol[channel_no])
		{
			m_protocol[channel_no]->exit_proc();
			m_protocol[channel_no] = NULL;
		}
	}

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
// 	Db_connector::open();
	int ret = Db_connector::select("select ��Լ��, ��Լ���� from ��Լ������", (void **)&db_protocol);
	int rcd_cnt = ret; //��Լ��Ԫ������
	char path[LONGCHAR_LEN];
	ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV), PROTOCAL_FILE_NAME);

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
		FILE* fp = ACE_OS::fopen(path, "rb");
		if (fp == NULL)
		{
			ACE_ERROR((LM_ERROR, "���ļ�ʧ�ܣ�\n"));
		}
		else
		{
			db_protocol = new PROTOCOL_[100];
			rcd_cnt = ACE_OS::fread(db_protocol, sizeof(PROTOCOL_), 100, fp);
			ACE_OS::fclose(fp);
			for (int i = 0; i < rcd_cnt; i++)
			{
				//������Լʵ��
				PROTOCOL_* protocol_in_db = db_protocol + i;
				Protocol_factory::instance()->add_protocol(protocol_in_db->protocol_name);
			}	
		}
		ACE_DEBUG((LM_DEBUG, "���ļ���ȡ��Լ�����ɹ���\n"));

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

// 	calc_channel_priority();	//����ͨ��������ϵ
// 	calc_rtu_priority();
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
	int rtu_no = -1;
	int channel_no = -1;

	FE_MAIL_STRUCT* mail_msg = (FE_MAIL_STRUCT*)mail;

	if (mail_msg->mail_type == YK)
	{
		FE_YK_MAIL* yk_mail = (FE_YK_MAIL*)mail_msg->mail_body;
		rtu_no = yk_mail->rtu_no;
		channel_no = yk_mail->channel_no;		//��ȡ��Ӧrtuno��ͨ����

		if (yk_mail->action == 0xCC)		//add by lujiashun ת��
		{
			yk_mail->action = 1;
		}
		else if (yk_mail->action == 0x33)
		{
			yk_mail->action = 0;
		}
	}
	else if (mail_msg->mail_type == CALLING)
	{
		FE_CALLING_MAIL* call_mail = (FE_CALLING_MAIL*)mail_msg->mail_body;
		rtu_no = call_mail->rtu_no;
		channel_no = call_mail->channel_no;

	}
	else if (mail_msg->mail_type == SET_POINT)
	{
		FE_SETPOINT_MAIL* set_mail = (FE_SETPOINT_MAIL*)mail_msg->mail_body;
		rtu_no = set_mail->rtu_no;
		channel_no = set_mail->channel_no;		//��ȡ��Ӧrtuno��ͨ����
	}
	else if (mail_msg->mail_type == PROT)
	{
		FE_PROT_MAIL* prot_mail = (FE_PROT_MAIL*)mail_msg->mail_body;
		rtu_no = prot_mail->rtu_no;
		channel_no = prot_mail->channel_no;
	}

	if (rtu_no < 0 || channel_no < 0)
	{
		return -1;
	}

	Proc_manager::putdbg(MAINDBG, "ң���ն˺� = %d ң���ն�ͨ���� = %d",rtu_no,channel_no);

	ACE_Message_Block *protocol_msg = 0;
	protocol_msg = new ACE_Message_Block(lenth);
	ACE_OS::memcpy(protocol_msg->wr_ptr(), mail, lenth);
	
// 	CHANPARA *para_ptr = Fes_shm::get().chan_para_ptr + yk_channel_no;
	CHANDATA *data_ptr = Fes_shm::get().chan_data_ptr + channel_no;

	int current_channel_host = data_ptr->curr_attend_host; //ң��ͨ����ǰֵ������
	int current_host = Fes_shm::get().fe_sys_data_ptr->host_no;		//�������
	if (current_channel_host == current_host)
	{
		if (m_protocol[channel_no])
		{
			m_protocol[channel_no]->putq(protocol_msg);
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
		char channel_code[CODE_LENTH]; //�ն˴���
		char describe[SHORT_DESC_LENTH]; //ͨ������
		unsigned short zone_level;		//��ȫ��
		char protocol_name[CODE_LENTH];  //��Լ��
		short rtu_no; //����rtu��,��ͨ�����
		unsigned char group_priority;	//�������ȼ�,0Ϊ������ȼ�
		unsigned short common_address; //������ַ		//add by lujiashun����ͨ��������ַ����վ��ַ���ű�����Ӧ�Ķ�
		unsigned short master_address; //��վ��ַ
		unsigned char is_check; //ͨ������־λ
		unsigned char is_gps; //GPS��־λ
		unsigned char initial_stat; //ͨ����ʼ��״̬
		unsigned short stop_gap; //ͨ��ֹͣʱ��
		unsigned short bit_rate_error; //������
// 		unsigned short default_attend_host; //<Ĭ��ֵ����ڵ㣬�������������д-1������ϵͳ�Զ�����Ĭ��ֵ����ڵ�
		char default_attend_host[CODE_LENTH];	//<<Ĭ��ֵ����ڵ���
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
		unsigned char graph_push; //�Ƿ���ͼ
		char grName[40];	//��ͼ��	
		unsigned char valid; //ͨ��ʹ�ñ�־
	};

	CHANNEL_* db_channel = NULL;
	int rcd_cnt = Db_connector::select("select * from ͨ�������� order by  ͨ����� asc", (void **)&db_channel); 

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
// 				ACE_OS::strcpy(para->channel_code,db_para->channel_code);
				ACE_OS::strcpy(para->desdribe,db_para->describe);
				ACE_OS::strcpy(para->protocol,db_para->protocol_name);
				para->gps = db_para->is_gps;
				para->check = db_para->is_check;
				para->valid = db_para->valid;
				para->child_rtu = db_para->rtu_no;
				para->common_addr = db_para->common_address;		//add by lujiashun
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

// 				para->default_attend_host = db_para->default_attend_host;
				//todo �ҳ��ýڵ����ڴ������ڵ����
				para->default_attend_host = Net_stat::get_host_no(db_para->default_attend_host);
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
		unsigned short zone_level; //��ȫ��
		char station_code[CODE_LENTH];
// 		unsigned short channel_no; //ͨ����
		unsigned short yc_num; //ң����Ŀ
		unsigned short yx_num;//ң����Ŀ
		unsigned short kwh_num; //�������Ŀ
		unsigned short status_num; //״̬����Ŀ
		unsigned char valid; //ʹ�ñ�־
	};

	RTU_* db_rtu = NULL;
	int rcd_cnt = Db_connector::select("select * from �ն˲����� order by �ն���� asc",(void **)&db_rtu);
	
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
// 				para->last_channel = db_para->channel_no;
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

	YC_PARA_DB* db_yc_para = NULL;
	vector<YC_PARA> chan_buffer; 
	CHANPARA * chan_para = NULL;
	vector<YC_PARA_DB> yc_buffer_db;
	vector<YC_PARA> yc_buffer;
	YC_PARA tem_yc;
	int rtu_no = 0;
	int pos = 0;
	int current_yc_num = 0;
	Fes_data_shm_allocator &m_fes_data_shm_allocator = Fes_data_shm_allocator::get();

	int rcd_cnt = Db_connector::select("select  ң�����,(case when б�� = 0 then 1 else б�� end)/(case when ���� = 0 then 1  else ���� end),�ؾ�,�ն���� from ң������� order by �ն����, ң����� asc ",(void **)&db_yc_para);
	int rcd_act_num = 0;
	bool isend = false;

	if (rcd_cnt < 0)
	{
		ACE_DEBUG((LM_ERROR, "��ȡ���ݿ�ң����������ʧ�ܣ�\n"));
		db_yc_para = new YC_PARA_DB[MAX_YC_NUM];
		rcd_cnt = read_yc_para_from_file(db_yc_para);
	}
	else
	{
		save_yc_para(db_yc_para, rcd_cnt);
	}

	if (rcd_cnt > 0)
	{
		for (int i = 0; i < MAX_CHAN_NUM; i++)
		{
			
			chan_para = m_channel_para + i;
			rtu_no = chan_para->child_rtu;
			if (chan_para->valid == 0 || chan_para->yc_num <= 0)
			{
				continue;
			}

			chan_buffer.resize(chan_para->yc_num);
			pos = 0;

			current_yc_num = (m_rtu_para + rtu_no)->yc_num;
			if (current_yc_num <= 0)
			{
				continue;
			}

			isend = false;
			for (int j = 0; j < rcd_cnt; j++)
			{
				// 					printf("slope = %lf ordinate = %lf ter_no = %d ycno = %d j = %d \n",(db_yc_para + j)->slope,(db_yc_para + j)->ordinate,(db_yc_para + j)->ter_no,(db_yc_para + j)->yc_no, j);
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
				for (int j = 0; j < rcd_act_num; j++)
				{
					if (yc_buffer_db[j].yc_no < yc_buffer.size())
					{
						continue;
					}
					while (yc_buffer_db[j].yc_no != yc_buffer.size() && yc_buffer_db[j].yc_no != 9999)		//��ȫ����жϵ�yc
					{
						tem_yc.ordinate = 0.0;
						tem_yc.slope = 1.0;
						yc_buffer.push_back(tem_yc);
					}
					tem_yc.ordinate = yc_buffer_db[j].ordinate;
					tem_yc.slope = yc_buffer_db[j].slope;
					yc_buffer.push_back(tem_yc);
				}//for (int j = 0; j < rcd_act_num ; j++)
				//�Ե�ǰ�ն����ø���Ϊ׼����ʵ��YC���������ն����ø�����������YC

				rcd_act_num = yc_buffer.size();

				if(rcd_act_num > current_yc_num)
				{
					rcd_act_num = current_yc_num;
				}
				memcpy(&chan_buffer[pos],&yc_buffer[0],rcd_act_num * sizeof(YC_PARA));
			}
			yc_buffer.clear();
			yc_buffer_db.clear();
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
	YX_PARA_DB* db_yx_para = NULL;
	vector<YX_PARA> chan_buffer; 
	CHANPARA * chan_para = NULL;
	vector<YX_PARA_DB> yx_buffer_db;
	vector<YX_PARA> yx_buffer;
	YX_PARA tem_yx;
	int rtu_no = 0;
	int pos = 0;
	int current_yx_num = 0;
	Fes_data_shm_allocator &m_fes_data_shm_allocator = Fes_data_shm_allocator::get();
	int rcd_cnt = Db_connector::select("select ȡ����־,�ն����,ң����� from ң�Ų����� order by �ն����, ң����� asc ",(void **)&db_yx_para);		//modidfy by libinghong
	int rcd_act_num = 0;
	bool isend = false;

	if (rcd_cnt < 0)
	{
		ACE_DEBUG((LM_ERROR, "��ȡ���ݿ�ң�Ų��������ʧ�ܣ�\n"));
		db_yx_para = new YX_PARA_DB[MAX_YX_NUM];
		rcd_cnt = read_yx_para_from_file(db_yx_para);
	}
	else
	{
		save_yx_para(db_yx_para, rcd_cnt);
	}

	if (rcd_cnt > 0)
	{
		for (int i = 0; i < MAX_CHAN_NUM; i++)
		{
			chan_para = m_channel_para + i;
			rtu_no = chan_para->child_rtu;
			if (chan_para->valid == 0 || chan_para->yx_num <= 0)
			{
				continue;
			}
			chan_buffer.resize(chan_para->yx_num);

			pos = 0;
			current_yx_num = (m_rtu_para + rtu_no)->yx_num;
			if (current_yx_num <= 0)
			{
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
					if (yx_buffer_db[j].yx_no < yx_buffer.size())
					{
						continue;
					}
					while (yx_buffer_db[j].yx_no != yx_buffer.size() && yx_buffer_db[j].yx_no != 9999)		//��ȫ��ȫ����жϵ�yx
					{
						tem_yx.negate = 0;
						yx_buffer.push_back(tem_yx);
					}
					tem_yx.negate = yx_buffer_db[j].negate;
					yx_buffer.push_back(tem_yx);
				}
				//�Ե�ǰ�ն����ø���Ϊ׼����ʵ��YX���������ն����ø�����������YX
				rcd_act_num = yx_buffer.size();		
				if(rcd_act_num > current_yx_num)
				{
					rcd_act_num = current_yx_num;
				}
				memcpy(&chan_buffer[pos],&yx_buffer[0],rcd_act_num * sizeof(YX_PARA));
			}
			yx_buffer.clear();
			yx_buffer_db.clear();
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
	KWH_PARA_DB* db_kwh_para = NULL;
	vector<KWH_PARA> chan_buffer; 
	CHANPARA * chan_para = NULL;
	vector<KWH_PARA_DB> kwh_buffer_db;
	vector<KWH_PARA> kwh_buffer;
	KWH_PARA tem_kwh;
	int rtu_no = 0;
	int pos = 0;
	int current_kwh_num = 0;
	Fes_data_shm_allocator &m_fes_data_shm_allocator = Fes_data_shm_allocator::get();
	int rcd_cnt = Db_connector::select("select ������,����,(case when ���� = 0 then 1 else ���� end)/(case when ��� = 0 then 1  else ��� end),�ն���� from ��Ȳ����� order by �ն����, ������ asc ",(void **)&db_kwh_para);
	int rcd_act_num = 0;
	bool isend = false;

	if (rcd_cnt < 0)
	{
		ACE_DEBUG((LM_ERROR, "��ȡ���ݿ��Ȳ��������ʧ�ܣ�\n"));
		db_kwh_para = new KWH_PARA_DB[MAX_KWH_NUM];
		rcd_cnt = read_kwh_para_from_file(db_kwh_para);
	}
	else
	{
		save_kwh_para(db_kwh_para, rcd_cnt);
	}

	if (rcd_cnt > 0)
	{
		for (int i = 0; i < MAX_CHAN_NUM; i++)
		{
			chan_para = m_channel_para + i;
			rtu_no = chan_para->child_rtu;
			if (chan_para->valid == 0 || chan_para->kwh_num <= 0)
			{
				continue;
			}

			chan_buffer.resize(chan_para->kwh_num);
			pos = 0;
			current_kwh_num = (m_rtu_para + rtu_no)->kwh_num;
			if (current_kwh_num <= 0)
			{
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
					if (kwh_buffer_db[j].kwh_no < kwh_buffer.size())
					{
						continue;
					}
					while (kwh_buffer_db[j].kwh_no != kwh_buffer.size() && kwh_buffer_db[j].kwh_no != 9999)		//��ȫ����жϵ�kwh
					{
						tem_kwh.full_code = 1;
						tem_kwh.slope = 1;
						kwh_buffer.push_back(tem_kwh);
					}
					tem_kwh.full_code = kwh_buffer_db[j].full_code;
					tem_kwh.slope = kwh_buffer_db[j].slope;
					kwh_buffer.push_back(tem_kwh);
				}

				rcd_act_num = kwh_buffer.size();

				//�Ե�ǰ�ն����ø���Ϊ׼����ʵ��KWH���������ն����ø�����������KWH
				if(rcd_act_num > current_kwh_num)
				{
					rcd_act_num = current_kwh_num;
				}
				memcpy(&chan_buffer[pos],&kwh_buffer[0],rcd_act_num * sizeof(KWH_PARA));
			}
			kwh_buffer.clear();
			kwh_buffer_db.clear();
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
//  				m_protocol[channel_no]->close();
				m_protocol[channel_no]->exit_proc();
				m_protocol[channel_no] = NULL;
				m_fes_data_shm_allocator.recycle_channel_data(channel_no);
			}
		}

		int rtu_no = para->child_rtu;		//rtu��
		RTUPARA *rtu_para = NULL;		//����rtupara�ڴ�
		RTUPARA *m_shm_rtu_para = Fes_shm::get().rtu_para_ptr;		//����rtupara�ڴ�
		RTUPARA *shm_rtu_para = NULL;		//����rtupara�ڴ�
// 		while(true)
// 		{
			rtu_para = m_rtu_para + rtu_no;
			shm_rtu_para = m_shm_rtu_para + rtu_no;
			ACE_OS::memcpy(shm_rtu_para,rtu_para,sizeof(RTUPARA));
// 			rtu_no = rtu_para->next_rtu;
// 			if (rtu_no == para->child_rtu)		//��������һ���ˣ�����rtupara��������
// 			{
// 				break;
// 			}
// 
// 		}
//		

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
				m_bnew_protocol[channel_no] = true;
			}	
		}
	}	//end first loop


	read_yc_para_from_db();
	read_yx_para_from_db();
	read_kwh_para_from_db();

	for (channel_no = 0; channel_no < MAX_CHAN_NUM; channel_no++)
	{
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
    ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV), CHANNEL_FILE_NAME);
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

	ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV), RTU_FILE_NAME);
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
���ļ��ж�ȡң�����
**************************************************************************/
int Channel_chuser::read_yc_para_from_file(YC_PARA_DB* p_data)
{

	int ret = 0;
	char path[LONGCHAR_LEN];

	ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV), YC_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "rb");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "���ļ�ʧ�ܣ�\n"), -1);
	}

	ret = ACE_OS::fread(p_data, sizeof(YC_PARA_DB), MAX_YC_NUM, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_ERROR, "���ļ���ȡң������ɹ���\n"));

	return ret;
}


/**************************************************************************
���ļ��ж�ȡң�Ų���
**************************************************************************/
int Channel_chuser::read_yx_para_from_file(YX_PARA_DB* p_data)
{
	int ret = 0;
	char path[LONGCHAR_LEN];

	ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV), YX_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "rb");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "���ļ�ʧ�ܣ�\n"), -1);
	}

	ret = ACE_OS::fread(p_data, sizeof(YX_PARA_DB), MAX_YX_NUM, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_ERROR, "���ļ���ȡң�Ų����ɹ���\n"));

	return ret;
}


/**************************************************************************
���ļ��ж�ȡ��Ȳ���
**************************************************************************/
int Channel_chuser::read_kwh_para_from_file(KWH_PARA_DB* p_data)
{
	int ret = 0;
	char path[LONGCHAR_LEN];

	ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV), KWH_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "rb");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "���ļ�ʧ�ܣ�\n"), -1);
	}

	ret = ACE_OS::fread(p_data, sizeof(KWH_PARA_DB), MAX_KWH_NUM, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_ERROR, "���ļ���ȡ��Ȳ����ɹ���\n"));

	return ret;
}

/**************************************************************************
����ͨ�����ȼ� 
**************************************************************************/
void Channel_chuser::calc_channel_priority(void)
{
	//��������
}

/**************************************************************************
�����ն����ȼ�
**************************************************************************/
void Channel_chuser::calc_rtu_priority( void )
{
	//��������
}

/**************************************************************************
�����ն�ͨ����ϵ
**************************************************************************/
/*
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

        //�ж�����ĳ��ͨ���ĵ�һ���ն˺�
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
		//���ǵ�һ���ն˺�
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
	//����ͨ���ܵ�ң�ţ�ң�⣬�����
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
*/

void Channel_chuser::calc_rtu_channel(void)		//�޸�ͨ�����ն�Ϊ���һ�Ĺ�ϵ
{

	int channel_rtuno = -1;
	RTUPARA *tem_rtu_para = NULL;
	CHANDATA* chandata = Fes_shm::get().chan_data_ptr;

	GPS_INFO* gps_info = Fes_shm::get().gps_info_ptr;
	int gps_priority = 0;

	/*����ͨ����ң�ţ�ң�⣬�����*/
	for (int i = 0; i < MAX_CHAN_NUM; i++)
	{
		if ((m_channel_para + i)->valid == 0)
		{
			continue;
		}

		if ((m_channel_para + i)->initial_stat == 1)//��ʼ��ͨ����ʼstat,Ĭ��״̬ΪCHANSTOP
		{
	//		chandata[i].stat = CHANRUN;
		}
		//chandata[i].last_recv_time = 0;

		if ((m_channel_para + i)->gps == 1)
		{
			(m_channel_para + i)->gps_priority = gps_priority;
			(gps_info + gps_priority)->channel = (m_channel_para + i)->order;
			gps_priority++;
		}

		channel_rtuno = (m_channel_para + i)->child_rtu;
		tem_rtu_para = m_rtu_para + channel_rtuno;

		(m_channel_para + i)->yc_num = tem_rtu_para->yc_num;
		(m_channel_para + i)->yx_num = tem_rtu_para->yx_num;
		(m_channel_para + i)->kwh_num = tem_rtu_para->kwh_num;

	}//for (int i = 0; i < MAX_CHAN_NUM; i++)


}

/**************************************************************************
����ͨ������
**************************************************************************/
int Channel_chuser::save_channel_para(void)
{
	char path[LONGCHAR_LEN];
    ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV), CHANNEL_FILE_NAME);
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
    ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV), RTU_FILE_NAME);
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
����ң�����
**************************************************************************/
int Channel_chuser::save_yc_para(YC_PARA_DB* p_data, int rcd_cnt)
{
	char path[LONGCHAR_LEN];
	ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV), YC_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "w+b");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "���ļ�ʧ�ܣ�\n"), -1);
	}

	ACE_OS::fwrite(p_data, sizeof(YC_PARA_DB), rcd_cnt, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_ERROR, "����ң������������ļ��ɹ���\n"));
	return 0; 
}

/**************************************************************************
����ң�Ų���
**************************************************************************/
int Channel_chuser::save_yx_para(YX_PARA_DB* p_data, int rcd_cnt)
{
	char path[LONGCHAR_LEN];
	ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV),YX_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "w+b");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "���ļ�ʧ�ܣ�\n"), -1);
	}

	ACE_OS::fwrite(p_data, sizeof(YX_PARA_DB), rcd_cnt, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_ERROR, "����ң�Ų����������ļ��ɹ���\n"));
	return 0; 
}

/**************************************************************************
�����Ȳ���
**************************************************************************/
int Channel_chuser::save_kwh_para(KWH_PARA_DB* p_data, int rcd_cnt)
{
	char path[LONGCHAR_LEN];
	ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV),KWH_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "w+b");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "���ļ�ʧ�ܣ�\n"), -1);
	}

	ACE_OS::fwrite(p_data, sizeof(KWH_PARA_DB), rcd_cnt, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_ERROR, "�����Ȳ����������ļ��ɹ���\n"));
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
		m_channel_para[channel_no].next_channel = -1;
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