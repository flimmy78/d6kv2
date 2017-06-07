/**
@file fesnode.h
@brief ǰ�ýڵ�������
@author zhangyun 
@version 1.0.0
@date 2013-11-06
*/
#ifndef FES_NOED_H_
#define FES_NOED_H_

#include "fes/timer.h"

/**
@brief ǰ�ýڵ���Ϣ
*/
class Fe_node
{
	enum
	{
		DEAD_TIME = 5
	};
public:
	/**
	@brief ���캯��
	@param in const int& host_no�ڵ��
	@param in const int& host_index�ڵ����
	*/
	Fe_node(const int& host_no, const int& host_index);

	/**
	@brief ��ȡ����������״̬
	*/
	int get_stat(void) const;

	/**
	@brief �յ�����

	���ü�ʱ��
	*/
	void got_heartbeat(void){m_live_timer.restart();};

	/**
	@brief ��ȡ�������ĸ���λͼ
	@return ����������
	*/
	unsigned long get_load(void) const {return m_load_map;};

	/**
	@brief ���÷���������λͼ
	@return ����������
	*/
	void set_load(const unsigned long& load){m_load_map = load;};

	/**
	@brief ��ȡ������״̬
	@return ������״̬����������0���쳣����-1
	*/
	int get_srv_stat(void);

	/**
	@brief ͨ������λͼ���㸺��
	@return ����
	*/
	int calc_load(void) const;
	/**
	@brief ��ȡǰ�÷����������
	@return ǰ�÷����������
	*/
	int get_index(void) const {return m_index;};
	/**
	@brief ��ȡ�ڵ��
	@return �ڵ��
	*/
	int get_host_no(void) const {return m_host_no;};

	/**
	@brief �Ƿ���ǰ��
	@return 0��ǰ��
	*/
	int Is_Main_Server(void) const;

	/**
	@brief ��ȡ�ɼ�����
	@return �ɼ�����
	*/
	int get_collect_zone(void) const {return m_collect_zone;};

private:
	const int m_host_no;//������
	const int m_index;	//ǰ�÷����������

	int m_collect_zone;	//�ɼ�����

	unsigned long m_load_map;	//����λͼ

	Timer m_live_timer;			//���м�ʱ�������յ��û��ʼ���ʱ������
};
#endif