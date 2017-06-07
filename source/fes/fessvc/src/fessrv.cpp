#include "fes/protocolfactory.h"
#include "fes/statchecker.h"

#include "fes/portwatch.h"
#include "fes/fesshm.h"
#include "fes/fesdatashm_allocator.h"
#include "fes/channelchuser.h"
#include "fes/femailstruct.h"
#include "fes/platformwrapper.h"

#include "fes/globalbuf_evevnt.h"
#include "fes/globalbuf_raw.h"
#include "fes/globalbuf_yk_check.h"
#include "fes/timer.h"

class ACE_Time_Value;
class ACE_Thread_Mutex;
//class Femail;

class CheckProcTask: public ACE_Task<ACE_MT_SYNCH>
{
public:
	CheckProcTask(){};
	~CheckProcTask(){};

	int open(void *)
	{
		ACE_DEBUG((LM_DEBUG,"(%t) CheckProcTask open !\n")); 
		//Activate the object with a thread in it. 
		activate(); 
		return 0; 
	};
	int svc(void)
	{
		Proc_manager::start_proc("fessvc", "��ǰ��ͨѶ����","supernb");
		while (true)
		{	
			if ( Proc_manager::check_parent() < 0 )		//2S����һ��checkparent
			{
				Proc_manager::end_proc();
				break;
			}
			ACE_OS::sleep(ACE_Time_Value(2,0));
		}
		exit(0);
	};
};

int main(int argc, char **argv)
{
	ACE_Thread_Mutex ace_t_m;
	Timer check_parent_timer;

	//�ж��Ƿ�������nbsvc
	int a = Net_stat::get_host_no();
	int n=Net_stat::get_host_stat(a);

	if(a<0)
	{
	
		ACE_ERROR((LM_ERROR, "�ڵ�û������nbsvc\n"));
		exit(0);
	}
	if (!Proc_manager::proc_exist("fessvc"))
	{
		ACE_ERROR((LM_ERROR, "֪ͨ����������� exit\n"));
		exit(0);
	}

	/*Proc_manager::start_proc("fessvc", "��ǰ��ͨѶ����","supernb");*/

// 	Fes_shm& fes_share_memory = Fes_shm::get();		//���������ڴ�
// 	Fes_data_shm_allocator &fes_data_shm_allocator = Fes_data_shm_allocator::get();	//�����ݹ����ڴ�
	
	/*Proc_manager::check_parent();*/
	if ( Db_connector::open( ) < 0)
	{
		ACE_ERROR((LM_ERROR, "���ݿ��ʧ��\n"));
	}
	/*Proc_manager::check_parent();*/

	Port_watch tem_port_watch(ace_t_m);		//�˿�ֵ��
	Stat_checker tem_stat_checker(ace_t_m);		//ͨ�����

	Channel_chuser ch_ch(ace_t_m);			//ǰ�ò������أ�����Լʵ������
	Global_buf_Event::get().open();		//��������ͽ���
	Global_buf_Raw::get().open();		//ʵʱ���ݷ��ͽ���
	Global_buf_yk_check::get().open();		//ң�ط�����Ϣ���ͽ���
	Proc_manager::putdbg(MAINDBG, "������Լ�߳�");
	ch_ch.load();						//�������أ�������Լʵ��

	printf("fessvc ch_ch.load ok!\n");
	fflush(stdout);

	Proc_manager::putdbg(MAINDBG, "�����˿�ֵ���߳�");
	tem_port_watch.open();		//�˿�ֵ���߳�����
	Proc_manager::putdbg(MAINDBG, "����״ֵ̬���߳�");
	tem_stat_checker.open();	//״̬����߳�����
	Db_connector::close( );


	CheckProcTask checktask;
	checktask.open(0);


//�ʼ����գ��ַ�����
	FE_MAIL_STRUCT *buff = NULL;
	int recv_len = 0;
	ACE_Message_Block *msg_stu = 0;
	while (true)
	{	
// 		if (check_parent_timer.elapsed() > 2)		//2S����һ��checkparent
// 		{
// 			int check_ret = Proc_manager::check_parent();
// 			if ( !check_ret )
// 			{
// 				check_parent_timer.restart();
// 			}
// 			else
// 			{
// 				break;
// 			}
// 		}

		int ret = Fe_mail::receive((void **)&buff,&recv_len);
		if (ret == -1) //���ʼ�
		{
			ACE_OS::sleep(ACE_Time_Value(0,10*1000));
		}
		else
		{
			switch (buff->mail_type)
			{
			case YK:		//ң�ء��ٻ��ʼ�ͳһ����
			case CALLING:
			case SET_POINT:
			case PROT:
				{
					ACE_DEBUG((LM_DEBUG, "ǰ���յ�ң������\n"));

					ch_ch.put_mail(buff, recv_len);		//����
				}
				break;
			case PORT_WATCH:
				{
					msg_stu = new ACE_Message_Block(recv_len);
					msg_stu->copy((char*)buff,recv_len);
 //					ACE_OS::memcpy(msg_stu->wr_ptr(), buff, recv_len);
					tem_port_watch.putq(msg_stu);
				}
				break;
			case LOAD_PARA:		//���������ʼ������½��м��ع���
				{
					Proc_manager::putdbg(MAINDBG, "�յ����������ʼ�");
					if ( !Db_connector::open( ) )
					{
						ch_ch.load();
					}
					Db_connector::close();
				}
				break;
			case CHANNEL_STAT:	//����ͨ��״̬�ʼ�
			case REAL_DATA:		//����ʵʱ�����ʼ�
			case INVALID:
			default:
				break;
			}
		}
	}

	Proc_manager::end_proc();

	return 0;
}