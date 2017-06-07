/**
@file globalbuf_event.cpp
@brief ǰ��ȫ���������ݻ��������ʵ��

@author hongxiang 
@version 1.0.0
@date 2013-10-28
*/
#include "fes/globalbuf_evevnt.h"
#include "fes/CMyEFile.h"

#include <ace/OS.h>
#include <ace/UUID.h>


using namespace std;


Global_buf_Event::Global_buf_Event()
:m_event_list(Array_list<FE_DATA_EVENT_NODE>(4096, 4))
{

}

/**************************************************************************
����
**************************************************************************/
int Global_buf_Event::open(void *args)
{
	activate();
	return 0;
}

/**************************************************************************
ֹͣ
**************************************************************************/
int Global_buf_Event::close (u_long flags)
{
	return 0;
}

/**************************************************************************
���ݷ����߳���
**************************************************************************/
int Global_buf_Event::svc(void)
{
	static int timecount = 0;
	static int rawevent_count = 0;
	static int time_limit = 0;
	while(true)		//�߳�ѭ����
	{
		lock_event_list();		//������
		if (!m_event_list.empty())	//��������Ҫ����
		{
			while(m_event_list.get_head_array() != NULL)
			{
				//todo III��ǰ�ô��ļ�
				if(Net_stat::get_PUBFEP_flag())
				{
					if (get_head_event_num()>=1000 || time_limit > 250)	//5S��1000�������
					{
						time_limit = 0;
						EFILE_SOE *event_list = new EFILE_SOE(get_head_event_num());

						char file_path[512];
						ACE_Utils::UUID_Generator uuidout;
						uuidout.init();
						ACE_Utils::UUID uuid;
						uuidout.generate_UUID(uuid);///->ace5.4.0 

						char **hosts = new char*[MAX_FE_SRV_NUM];
						for(int i = 0;i<MAX_FE_SRV_NUM;i++)
						{
							hosts[i] = new char[CODE_LENTH];
							memset(hosts[i],0,sizeof(char)*CODE_LENTH);
						}
						int host_num = Net_stat::get_fe_hosts(hosts, MAX_FE_SRV_NUM);//��ȡI��fep������
						for (int j=0;j<host_num;j++)
						{
							ACE_OS::sprintf(file_path,"%s/log/dat/IIIFEP/%s/soe_%s.txt",getenv("NBENV"),hosts[j],uuid.to_string ()->c_str ());

							event_list->set_file_name(file_path);

							event_list->save_data_cross_table(get_head_event());
						}
						m_event_list.clear_head();

						for( int k = 0; k < MAX_FE_SRV_NUM; k ++ )
						{
							delete [] hosts[ k ];
						}
						delete [] hosts;
						hosts = NULL;
					}
					else
						break;
				}
				else
				{
					int ret =Fe_scasrv::fe_send_electric_event(get_head_event(), get_head_event_num());

					
					rawevent_count += get_head_event_num();
// 					if (ret == 0 && (rawevent_count % 100 == 0))
// 					{
// 						ACE_DEBUG((LM_DEBUG,"send event_count =%d!\n",rawevent_count));
// 					}
					//��շ��͹��ı��������飬������������
					m_event_list.clear_head();
				}

			}

		}
		unlock_event_list();		//�ͷ���

		if( timecount > 500 )
		{
			ACE_DEBUG((LM_DEBUG,"send event_count =%d!\n",rawevent_count));
			timecount = 0;
		}
		else
		{
			timecount ++;
		}
		time_limit++;
		ACE_OS::sleep(ACE_Time_Value(0,20*1000));	//�����ݣ�sleep
	}//�߳�ѭ����

	return 0;
}
