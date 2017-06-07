/**
@file Mirror_Proc.cpp
@brief III��ʵʱ���ݴ����߳�Դ�ļ�

III��ʵʱ���ݴ���

@author lujiashun 
@version 1.0.0
@date 2016-09-12
*/

#include "Mirror_Proc.h"
#include "fes/CMyEFile.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <ace/OS.h>

Mirror_Proc::Mirror_Proc( ) : QThread()
{
	m_data = new FE_REAL_DATA_NODE[MAX_FILE_DATA_NUM];
	m_soe = new FE_DATA_EVENT_NODE[MAX_FILE_DATA_NUM];
	m_channel = new FE_CHANNEL_STAT_MAIL[MAX_FILE_DATA_NUM];
	m_ykcheck = new FE_YK_CHECK_NODE[MAX_FILE_DATA_NUM];


	m_data_mail_struct = (FE_MAIL_STRUCT *)malloc( sizeof(FE_MAIL_STRUCT) + (MAX_FILE_DATA_NUM * sizeof(FE_REAL_DATA_NODE)) );		//����Ϊ��󳤶�

	m_stat_mail_struct = (FE_MAIL_STRUCT *)malloc( sizeof(FE_MAIL_STRUCT) + (MAX_FILE_DATA_NUM * sizeof(FE_CHANNEL_STAT_MAIL)) );		//����Ϊ��󳤶�
	
	m_last_modify_time = QDateTime::currentDateTime();
}

Mirror_Proc::~Mirror_Proc()
{
	delete m_data;
	delete m_soe;
	delete m_channel;
	delete m_ykcheck;
	free(m_data_mail_struct);
	free(m_stat_mail_struct);
}


void Mirror_Proc::run()
{
	QStringList file_list;
	while(true)		//�߳�ѭ����
	{
		if (scan_datafile(file_list))	//�������ļ�
		{
			for (int filenum = file_list.size(); filenum > 0; filenum--)
			{

				convert_file_to_list(file_list[filenum-1]);	
			}
		}
		QThread::sleep(1);
	}//�߳�ѭ����
}

bool Mirror_Proc::scan_datafile(QStringList& file_list)
{
	char dir_path[512];
	sprintf(dir_path,"%s/log/dat/IIIFEP/",getenv("NBENV"));
	
	QDir data_file_dir(g_Q(dir_path));

	file_list.clear();
	file_list = data_file_dir.entryList(QDir::Files, QDir::Time);
	
	if (file_list.isEmpty())
	{
		return false;
	}
	else
		return true;
}

void Mirror_Proc::convert_file_to_list(QString filename)
{
	char file_path[512];
	sprintf(file_path,"%s/log/dat/IIIFEP/%s",getenv("NBENV"),_Q2C(filename));
	QFile file(file_path);
	QFileInfo file_info( file );

	if ((file_info.lastModified()) < m_last_modify_time)	//ʱ�������ϴ��ļ�ʱ�� ������
	{
		file.remove();
		return;
	}
	else //��¼���ļ�������ʱ��
	{
		m_last_modify_time = file_info.lastModified();
	}

	if (!Net_stat::get_host_master_flag(Net_stat::get_host_no()))		//��ǰ�ñ�־,����ǰ�÷��������ʼ�
	{
		QStringList list1 = filename.split("_");
		if (list1[0] == "raw")	//�������ʹ���
		{
			EFILE_DATA* tmp_data = new EFILE_DATA(1);
			tmp_data->set_file_name(file_path);
			memset(m_data,0,sizeof(FE_REAL_DATA_NODE)*MAX_FILE_DATA_NUM);
			int num = tmp_data->read_data_cross_table(m_data);
			Proc_manager::putdbg(201,"raw num = %d\n",num);
			send_data_to_scada(num);
		}
		else if(list1[0] == "soe")//soe���ʹ���
		{
			EFILE_SOE* tmp_data = new EFILE_SOE(1);
			tmp_data->set_file_name(file_path);
			memset(m_soe,0,sizeof(FE_DATA_EVENT_NODE)*MAX_FILE_DATA_NUM);
			int num = tmp_data->read_data_cross_table(m_soe);
			send_soe_to_scada(num);

		}
		else if ((list1[0] == "channel"))//ͨ��״̬����
		{
			EFILE_CHANNEL* tmp_data = new EFILE_CHANNEL(1);
			tmp_data->set_file_name(file_path);
			memset(m_channel,0,sizeof(FE_CHANNEL_STAT_MAIL)*MAX_FILE_DATA_NUM);
			int num  = tmp_data->read_data_cross_table(m_channel);
			send_channel_to_scada(num);
		}
		else if ((list1[0] == "channelevent"))//ͨ�������
		{
			float channel_stat;
			char desdribe[64];
			char namestr[128];

			EFILE_CHANNEL* tmp_data = new EFILE_CHANNEL(1);
			tmp_data->set_file_name(file_path);
			tmp_data->read_event_cross_table(channel_stat, desdribe, namestr);
			
			printf("%f %s %s \n", channel_stat, desdribe, namestr);

			//����ͨ������
			Fe_event::make_channel_event(channel_stat,"", desdribe, namestr);
		}
		if (list1[0] == "ykcheck")	//ң�ط�У����
		{
			EFILE_YKCHHECK* tmp_data = new EFILE_YKCHHECK(1);
			tmp_data->set_file_name(file_path);
			memset(m_ykcheck,0,sizeof(FE_YK_CHECK_NODE)*MAX_FILE_DATA_NUM);
			int num = tmp_data->read_data_cross_table(m_ykcheck);
			send_ykcheck_to_scada(num);
		}

	}

 	file.remove();
}

void Mirror_Proc::send_data_to_scada( int data_num )
{
	if (data_num < 1)
	{
		return;
	}

	int datalen = data_num * sizeof(FE_REAL_DATA_NODE);		//����������

	ACE_OS::memset(m_data_mail_struct, 0, m_data_mail_struct->get_lenth()+datalen);

	m_data_mail_struct->mail_type = REAL_DATA;
	m_data_mail_struct->mail_lenth = m_data_mail_struct->get_lenth() + datalen;		//�ʼ��ܳ���
	m_data_mail_struct->sub_information1 = data_num;

	ACE_OS::memcpy(m_data_mail_struct->mail_body, (unsigned char*)m_data, datalen);

// 	FE_REAL_DATA_NODE* real_data_src = get_head_raw();
// 	FE_REAL_DATA_NODE* real_data_dest = (FE_REAL_DATA_NODE*)mail_struct->mail_body;

	int ret = Fe_mail::send_to_scada( (void*)m_data_mail_struct, m_data_mail_struct->mail_lenth, 2 );		//����-1ʧ��
	
	if(ret)
		printf("send raw faile\n");
}

void Mirror_Proc::send_soe_to_scada( int soe_num )
{
	if (soe_num < 1)
	{
		return;
	}
	int ret =Fe_scasrv::fe_send_electric_event(m_soe, soe_num);
	if(ret)
		printf("send soe faile\n");
}

void Mirror_Proc::send_channel_to_scada( int channel_num )
{
	if (channel_num < 1)
	{
		return;
	}
	int datalen = channel_num * sizeof(FE_CHANNEL_STAT_MAIL);		//����������

	ACE_OS::memset(m_stat_mail_struct, 0, m_stat_mail_struct->get_lenth()+datalen);

	m_stat_mail_struct->mail_type = CHANNEL_STAT;
	m_stat_mail_struct->mail_lenth = m_stat_mail_struct->get_lenth() + datalen;		//�ʼ��ܳ���
	m_stat_mail_struct->sub_information1 = channel_num;

	ACE_OS::memcpy(m_stat_mail_struct->mail_body, (unsigned char*)m_channel, datalen);

	int ret = Fe_mail::send_to_scada(m_stat_mail_struct, m_stat_mail_struct->mail_lenth, 2);

	if(ret)
		printf("send channel faile\n");

}


void Mirror_Proc::send_ykcheck_to_scada( int ykcheck_num )
{
	if (ykcheck_num < 1)
	{
		return;
	}
	for (int i=0;i<ykcheck_num;i++)
	{
		int ret = Fe_scasrv::fe_send_ykret(m_ykcheck->rtu_no,m_ykcheck->yk_no,m_ykcheck->yk_ret,m_ykcheck->msg );
		m_ykcheck++;
	}

// 	int ret =Fe_scasrv::fe_send_ykret(m_ykcheck, ykcheck_num);
// 	if(ret)
// 		printf("send ykcheck faile\n");
}
