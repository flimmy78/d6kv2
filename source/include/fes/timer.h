
/**
@file timer.h
@brief ��ʱ��
@author zhangyun 
@version 1.0.0
@date 2013-10-11
*/
#ifndef FES_TIMER_H_
#define FES_TIMER_H_

#include <ace/OS.h>

/**
@brief �뼶��ʱ��

����Qt��Boost��ʱ���һ����ʵ�֣�����elapsed����ϴ�restart���ʱ��
����restart���ü�ʱ������ǰʱ��
*/
class Timer
{
public:
	/**
	@brief ���캯��
	*/
	Timer(){restart();};
	/**
	@brief ���ü�ʱ������ǰʱ��
	*/
	void restart(void)
	{
		m_time = ACE_OS::time(NULL);
		m_stop_flag=false;
	};
	/**
	@brief ֹͣ��ʱ
	*/
	void stop(void){m_stop_flag=true;};
	/**
	@brief ����ϴ�restart���ȥ��ʱ��
	@return ��
	*/
	int elapsed(void) const 
	{
		if ( !m_stop_flag )
		{
			return static_cast<int>(ACE_OS::time(NULL) - m_time);
		}
		else
		{
			return 0 ;
		}
	};
	/**
	@brief ����ϴμ�ʱ�����õ�ʱ��
	*/
	time_t get_start_time(void) const {return m_time;};
private:
	time_t m_time;
	bool m_stop_flag;
};
#endif