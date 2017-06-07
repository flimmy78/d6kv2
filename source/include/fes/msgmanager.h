#ifndef MSG_MANAGER_H_
#define MSG_MANAGER_H_

#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <ace/Task.h>
#include <vector>
#include <map>
using std::vector;
using std::map;

#include "fes/fesshare.h"

#define  MAX_MSG_FRAME	10

#define  MAX_RECV_SIZE	5120		//ÿ�ν��յ�����
#define  MAX_CACHE_SIZE	102400		//����ռ�Ĵ�С

#define  START_CODE		0xEB
#define  SYNC_CODE		0x90


/**
@brief ����ǰ�ñ��ģ��洢���ģ�������Ҫ�ҷַ�����  
*/
class Msg_manage : public ACE_Task<ACE_MT_SYNCH>
{
public:
	typedef vector<FE_MSG*> Msg_Save_List;

	/**
	�������ʽӿ�

	Msg_manage::get().open....
	*/
	static Msg_manage* get(void);

	static Msg_manage* m_instance;

	~Msg_manage(void);

private:
	Msg_manage(); 
	
public: 
	int open(void*);

	int close(u_long);

	/**
	@brief ���Ĺ����߳���

	���ڲ���һ��˫��ѭ��

	while(true)

	����accept

	accept�ɹ��ߵ��⣬��Ȼcontinue
	while(true)
	{
		���ձ��ģ�ʧ��break���ɹ�����

		�õ��ı���Ӧ���кܶ������ִ��save��deliver��save��ʱ��Ҫʵ�ְɣ���Ϊ��ô������ǧ���ļ�����Ҫ�ú�����
	}
	*/
	int svc(void);

public:

	/**
	@brief ���汨��
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int save(const FE_MSG& msg);

	/**
	@brief �����ķַ������ķ���,����acceptor��put_msg����
	*/
	int deliver(const FE_MSG& msg);

	/**
	@brief ���ı������ļ�
	*/
	void save_to_file(Msg_Save_List *save_list);
	/**
	@brief ���ı������ļ�
	@para in unsigned char* recv_pointer  tcp���ջ�������ָ��
	@para in int recv_pointer  tcp���ճɹ���Ľ����ֽ���
	*/
	void proc_recv_buf(unsigned char* recv_poniter, int nrecv_count );

private:
	ACE_SOCK_Acceptor m_acceptor;	//TCP������
	ACE_SOCK_Stream	  m_peer;		//TCP��

	map<int, Msg_Save_List*> m_channel_msg_list;	//ͨ����Ӧ�Ļ�������

	unsigned char  cache_buf[MAX_CACHE_SIZE];		//���մ�������
	int write_pos;


};

/**
@brief ���ǰ�÷��͵ı��ĵ�ͬ��ͷ
@para in unsigned char* buf  ����ı��������ָ��
@return �ɹ�����true��ʧ�ܷ���false
*/
bool Check_msg_head(unsigned char* buf);


#endif



