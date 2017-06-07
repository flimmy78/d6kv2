/**
@file globalbuf_yk_check.h
@brief ǰ��ȫ��ң�ط�У���ݻ�����������
@author lujs 
@version 1.0.0
@date 2016-03-02
*/
#ifndef GLOBAL_BUF_YKCHECK_H_
#define GLOBAL_BUF_YKCHECK_H_

#ifdef WIN32
#ifdef FES_PROTOCOL_API_DLL
#define	FES_GLOBALBUF_YKCHECK_EXPORT __declspec(dllexport) 
#else
#define	FES_GLOBALBUF_YKCHECK_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_GLOBALBUF_YKCHECK_EXPORT
#endif

#include "fes/arraylist.h"
#include "fes/fesdatashm.h"
#include "fes/femailstruct.h"
#include "fes/platformwrapper.h"
#include "ace/Thread_Mutex.h"
#include "ace/Task.h"

/**
@brief ң�ط�У���ݻ�������

����������scada���͵����ݣ����������ݡ�������͸�scada������һ��������
�����ж���̷߳��ʣ���Ҫ���û���
*/
class FES_GLOBALBUF_YKCHECK_EXPORT Global_buf_yk_check : ACE_Task<ACE_MT_SYNCH>
{
	enum
	{
		ARRAY_EVENT_NUM = 8192,	///<ȫ�ֻ�������������ĳ���
		ARRAY_RAW_NUM = 8192,	///<ȫ�ֻ���������������ĳ���
		EVENT_LIST_SIZE = 8,	///<ȫ�ֻ�����������������ĳ���
		RAW_LIST_SIZE = 8		///<ȫ�ֻ�������������������ĳ���
	};
public:
	/**
	@brief ��ʵ����ȡ����
	*/
	static Global_buf_yk_check& get(void)
	{
		static Global_buf_yk_check instance;
		return instance;
	};

	/**
	@brief ����
	*/
	virtual int open (void *args = 0);

	/**
	@brief ֹͣ
	*/
	virtual int close (u_long flags = 0);

	/**
	@brief ���ݷ����߳���
	*/
	virtual int svc(void);

	/**
	@brief �������캯����ֻ������ʵ��
	
	��������������л����Ĭ�Ͽ������캯�������ֽڿ����������ܻ��𻵵���
	�е����ݣ���ֻ������ʵ�ֵķ�������Ĭ�Ͽ������캯���������ʹ���е���
	�������캯����������������
	*/
	Global_buf_yk_check(const Global_buf_yk_check&);

	/**
	@brief �����������ֻ������ʵ��
	
	��������������л����Ĭ�ϸ�ֵ����������ֽڸ�ֵ�������ܻ��𻵵����е����ݣ�
	��ֻ������ʵ�ֵķ�������Ĭ�ϸ�ֵ������������ʹ���е����˸�ֵ����������
	���������
	*/
	Global_buf_yk_check& operator = (const Global_buf_yk_check&);

	/**
	@brief ����ң�ط�У���ݻ���
	*/
	void lock_ykcheck_list(void){m_ykcheck_mutex.acquire();};

	/**
	@brief ����ң�ط�У���ݻ���
	*/
	void unlock_ykcheck_list(void){m_ykcheck_mutex.release();};

	/**
	@brief �ѵ���ң�ط�У����д��ȫ�ֻ���
	@param in const FE_YK_CHECK_NODE& ykcheck ң�ط�У����
	*/
	void put_yk_check(const FE_YK_CHECK_NODE& ykcheck)
	{

		ACE_Guard<ACE_Thread_Mutex> guard(m_ykcheck_mutex);
		m_ykcheck_list.insert_data(ykcheck);
	};


	/**
	@brief �Ѷ��ң�ط�У����д��ȫ�ֻ���
	@param in vector<FE_YK_CHECK_NODE>& ykchecks ң�ط�У��������
	*/
	void put_yk_check(vector<FE_YK_CHECK_NODE>& ykchecks)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_ykcheck_mutex);
		m_ykcheck_list.insert_data(ykchecks);
	};

	/**
	@brief �����һ������������
	*/
	void clear_head_raw(void){m_ykcheck_list.clear_head();};

	/**
	@brief ��ȡ�׸�ң�ط�У���������ָ��
	@return �ɹ�����ָ�룬ʧ�ܷ���NULL
	*/
	FE_YK_CHECK_NODE*	  get_head_ykcheck(void){return m_ykcheck_list.get_head_array();};

	/**
	@brief ��ȡ�׸�ң�ط�У��������ĳ���
	@return �ɹ����س��ȣ�ʧ�ܷ���-1
	*/
	int	get_head_ykcheck_num(void){return m_ykcheck_list.get_head_size();};

private:
	explicit Global_buf_yk_check();

private:
	Array_list<FE_YK_CHECK_NODE>			m_ykcheck_list;			///<ң�ط�У���ݻ���

	ACE_Thread_Mutex			m_ykcheck_mutex;		///<ң�ط�У���ݻ�����
	
};

#endif