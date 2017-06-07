/**
@file globalbuf_raw.h
@brief ǰ��ȫ�������ݻ�����������
@author zhangyun 
@version 1.0.0
@date 2012-11-02
*/
#ifndef GLOBAL_BUF_H_
#define GLOBAL_BUF_H_

#ifdef WIN32
#ifdef FES_PROTOCOL_API_DLL
#define	FES_GLOBALBUF_EXPORT __declspec(dllexport) 
#else
#define	FES_GLOBALBUF_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_GLOBALBUF_EXPORT
#endif

#include "fes/arraylist.h"
#include "fes/fesdatashm.h"
#include "fes/femailstruct.h"
#include "fes/platformwrapper.h"
#include "ace/Thread_Mutex.h"
#include "ace/Task.h"

/**
@brief ȫ�����ݻ�������

����������scada���͵����ݣ����������ݡ�������͸�scada������һ��������
�����ж���̷߳��ʣ���Ҫ���û���
*/
class FES_GLOBALBUF_EXPORT Global_buf_Raw : ACE_Task<ACE_MT_SYNCH>
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
	static Global_buf_Raw& get(void)
	{
		static Global_buf_Raw instance;
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
	Global_buf_Raw(const Global_buf_Raw&);

	/**
	@brief �����������ֻ������ʵ��
	
	��������������л����Ĭ�ϸ�ֵ����������ֽڸ�ֵ�������ܻ��𻵵����е����ݣ�
	��ֻ������ʵ�ֵķ�������Ĭ�ϸ�ֵ������������ʹ���е����˸�ֵ����������
	���������
	*/
	Global_buf_Raw& operator = (const Global_buf_Raw&);

	/**
	@brief ���������ݻ���
	*/
	void lock_raw_list(void){m_raw_mutex.acquire();};

	/**
	@brief ���������ݻ���
	*/
	void unlock_raw_list(void){m_raw_mutex.release();};

	/**
	@brief �ѵ���������д��ȫ�ֻ���
	@param in const FE_RAW& raw ������
	*/
	void put_raw(const FE_REAL_DATA_NODE& raw)
	{

		ACE_Guard<ACE_Thread_Mutex> guard(m_raw_mutex);
		m_raw_list.insert_data(raw);
	};


	/**
	@brief �Ѷ��������д��ȫ�ֻ���
	@param in vector<FE_RAW>& raws ����������
	*/
	void put_raw(vector<FE_REAL_DATA_NODE>& raws)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_raw_mutex);
		m_raw_list.insert_data(raws);
	};

	/**
	@brief �����һ������������
	*/
	void clear_head_raw(void){m_raw_list.clear_head();};

	/**
	@brief ��ȡ�׸������������ָ��
	@return �ɹ�����ָ�룬ʧ�ܷ���NULL
	*/
	FE_REAL_DATA_NODE*	  get_head_raw(void){return m_raw_list.get_head_array();};

	/**
	@brief ��ȡ�׸�����������ĳ���
	@return �ɹ����س��ȣ�ʧ�ܷ���-1
	*/
	int	get_head_raw_num(void){return m_raw_list.get_head_size();};

private:
	explicit Global_buf_Raw();

private:
	Array_list<FE_REAL_DATA_NODE>			m_raw_list;			///<�����ݻ���

	ACE_Thread_Mutex			m_raw_mutex;		///<�����ݻ�����
	
};

#endif