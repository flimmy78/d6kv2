/**
@file globalbuf_event.h
@brief ǰ��ȫ���������ݻ�����������
@author zhangyun 
@version 1.0.0
@date 2012-11-02
*/
#ifndef GLOBAL_BUF_EVENT_H_
#define GLOBAL_BUF_EVENT_H_

#ifdef WIN32
#ifdef FES_PROTOCOL_API_DLL
#define	FES_GLOBALBUF_EVENT_EXPORT __declspec(dllexport) 
#else
#define	FES_GLOBALBUF_EVENT_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_GLOBALBUF_EVENT_EXPORT
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
class FES_GLOBALBUF_EVENT_EXPORT Global_buf_Event : ACE_Task<ACE_MT_SYNCH>
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
	static Global_buf_Event& get(void)
	{
		static Global_buf_Event instance;
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
	Global_buf_Event(const Global_buf_Event&);

	/**
	@brief �����������ֻ������ʵ��
	
	��������������л����Ĭ�ϸ�ֵ����������ֽڸ�ֵ�������ܻ��𻵵����е����ݣ�
	��ֻ������ʵ�ֵķ�������Ĭ�ϸ�ֵ������������ʹ���е����˸�ֵ����������
	���������
	*/
	Global_buf_Event& operator = (const Global_buf_Event&);

	/**
	@brief ���������
	*/
	void lock_event_list(void){m_event_mutex.acquire();};

	/**
	@brief ���������
	*/
	void unlock_event_list(void){m_event_mutex.release();};

	/**
	@brief �ѵ�������д��ȫ�ֻ���
	@param in FE_EVENT& event ����
	*/
	void put_event(const FE_DATA_EVENT_NODE& event)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_event_mutex);
		m_event_list.insert_data(event);
	};

	/**
	@brief �Ѷ������д��ȫ�ֻ���
	@param in vector<FE_EVENT>& events ��������
	*/
	void put_event(vector<FE_DATA_EVENT_NODE>& events)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_event_mutex);
		m_event_list.insert_data(events);
	};

	/**
	@brief �����һ����������
	*/
	void clear_head_event(void){m_event_list.clear_head();};

	/**
	@brief ��ȡ�׸����������ָ��
	@return �ɹ�����ָ�룬ʧ�ܷ���NULL
	*/
	FE_DATA_EVENT_NODE* get_head_event(void){return m_event_list.get_head_array();};

	/**
	@brief ��ȡ�׸�ͨ��״̬����ĳ���
	@return �ɹ����س��ȣ�ʧ�ܷ���-1
	*/
	int	get_head_event_num(void){return m_event_list.get_head_size();};

private:
	explicit Global_buf_Event();

private:
	Array_list<FE_DATA_EVENT_NODE>		m_event_list;		///<�����

	ACE_Thread_Mutex			m_event_mutex;		///<�������
	
};

#endif