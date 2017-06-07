/**
@file protocolfactory.h
@brief ǰ�ù�Լ������

�ṩ��Լ����Ĵ򿪡����ù���

@author zhangyun 
@version 1.0.0
@date 2013-11-07
*/
#ifndef FES_PROTOCOL_FACTORY_H_
#define FES_PROTOCOL_FACTORY_H_

#include <map>
#include <string>
using std::map;
using std::string;

#include <ace/Singleton.h>
#include <ace/Synch.h>

class Protocol_handle;
class Base_protocol;
class Protocol_factory_i;

//����һ��ACE�ṩ�ĵ����ӿ�
typedef ACE_Singleton<Protocol_factory_i, ACE_SYNCH_MUTEX> Protocol_factory;

/**
@brief ��Լ������
*/
class Protocol_factory_i
{
	friend class ACE_Singleton<Protocol_factory_i, ACE_SYNCH_MUTEX>;
	typedef map<string, Protocol_handle*>::iterator protocol_iterator;
public:
	~Protocol_factory_i();
	/**
	@brief ����һ�ֹ�Լ
	@param in const char* protocol_name ��Լ��
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int add_protocol(const char* protocol_name);
	/**
	@brief ��ȡ��Լʵ��
	@param in const char* protocol_name ��Լ��
	@param in const int& channel ͨ�����
	@return ��Լʵ��ָ��
	*/
	Base_protocol* get_instance(const char* protocol_name, const int& channel_no);
	/**
	@brief ��ȡ���й�Լ����
	*/
	void get_all_config(void);
	/**
	@brief ��Լ�����Ƿ�Ϊ��
	@return �շ���ture����Ϊ�շ���false
	*/
	bool empty(void);

private:

	Protocol_factory_i();

private:
	map<string, Protocol_handle*> m_handle_map;
};



#endif

