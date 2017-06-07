/**
@file protocolhandle.h
@brief ǰ�ù�Լ�����

�ṩ��Լ����ӿڵ������ִ�й���

@author zhangyun 
@version 1.0.0
@date 2013-11-07
*/
#ifndef FES_PROTOCOL_HANDLE_H_
#define FES_PROTOCOL_HANDLE_H_

#include "fes/protocoldef.h"

class Base_protocol;

class Protocol_handle
{
public:
	Protocol_handle();
	Protocol_handle(void* m_get_instance_fp, void* m_get_config_fp);
	Protocol_handle(const Protocol_handle& handle);
	Protocol_handle& operator = (const Protocol_handle&);
	~Protocol_handle();
	
	/**
	@brief ��ȡ��Լʵ���ӿ�
	@param in const int& channel ͨ�����
	@return ��Լʵ��ָ��
	*/
	Base_protocol* get_instance(const int& channel) const;
	/**
	@brief ��ȡ��Լ����
	@param in const int& channel ͨ�����
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int get_config(void) const;
	/**
	@brief ���ù�Լʵ���ӿں�������
	@param in void* �������ĺ���ָ��
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	void set_instance_fp(void* fp);
	/**
	@brief ���û�ȡ���ú�������
	@param in void* �������ĺ���ָ��
	*/
	void set_config_fp(void* fp);
private:
	GET_INSTANCE_FP m_get_instance_fp;
	GET_CONFIG_FP   m_get_config_fp;
};


#endif
