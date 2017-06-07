#ifndef MSG_SERVICE_ACCEPTOR_H_
#define MSG_SERVICE_ACCEPTOR_H_

#include <ace/Thread_Mutex.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Task.h>
#include <map>
#include <utility>
#include <vector>
using std::multimap;
using std::pair;
using std::make_pair;
using std::vector;

class Msg_service;
class ACE_SOCK_Stream;

#include "fes/fesshare.h"

class Msg_service_acceptor : public ACE_Task<ACE_MT_SYNCH>
{
public:
	enum
	{
		MAX_THREAD_SIZE = 16,
		PORT_NO = 9005			//��취�ƶ������õĵط�ȥ
	};

	typedef multimap<int, Msg_service*>::iterator IT;		//linux��const_iterator�޷�erase��
	typedef pair<IT, IT> Range_NOCONST;
	typedef multimap<int, Msg_service*>::const_iterator CIT;
	typedef pair<CIT, CIT> Range;
public:
	/**
	�������ʽӿ�
	Msg_service_acceptor::get().open....
	*/
	static Msg_service_acceptor* get(void);

	static Msg_service_acceptor* instance;

	~Msg_service_acceptor();

	/**
	@brief �̳���Task��������ʵ�������麯��
	*/
	int open(void *);
	int close(u_long flags = 0);

	/**
	@brief �߳���

	���ڲ���һ��ѭ��

	Msg_service* free_service = get_free_service();

	while(true)
	{

	����accept

	m_acceptor.accept(free_service->get_peer(), ....)
	
	accept�ɹ��ߵ��⣬��Ȼcontinue

	���ӳɹ���Ȼ�Ͳ�free��
	free_service = get_free_service();
	}
	*/
	int svc(void);

	/**
	@brief ��ʼ�����򣬽����̳߳أ������캯������
	*/
	void init();

	/**
	@brief �ѱ���д����Ҫ�ķ���,������ʵ�ֵ�����
	*/
	void put_msg(const FE_MSG& msg);

	/**
	@brief ɾ��ͨ���ͷ����ӳ���ϵ
	*/
	void remove_service(const int& channel_no, const Msg_service* service);

	/**
	@brief ���ͨ���ͷ����ӳ���ϵ
	*/
	void add_service(const int& channel_no, Msg_service* service);

private:

	Msg_service_acceptor();

	Msg_service* get_free_service(void);

private:
	//ͨ����������߳�֮���ӳ�䣬����ͨ���ſ����ж����������multimap
	multimap<int, Msg_service*> m_active_task;

	ACE_Thread_Mutex	 task_list_mutex;

	vector<Msg_service *>	m_service_list;		//service��
	ACE_SOCK_Acceptor	m_acceptor;					//TCP������
};

#endif