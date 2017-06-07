/**
@file protocoldef.h
@brief ��Լ�������Ҫʵ�ֵ�C���ӿ���

@author zhangyun 
@version 1.0.0
@date 2013-11-07
*/
#ifndef FES_PROTOCOL_DEF_H_
#define FES_PROTOCOL_DEF_H_

class Base_protocol;

//�����Ǿ����Լ����Ҫʵ�ֵļ���C���ӿڵĺ���ָ�붨��
typedef Base_protocol* (*GET_INSTANCE_FP)(const int& channel_no);	///<��ȡ��Լʵ��
typedef int (*GET_CONFIG_FP)(void);									///<��ȡ��������ݿ��еĹ�Լ��������
typedef char* (*GET_VERSION_FP)(void);								///<��ȡ��Լ��汾
typedef char* (*GET_DESC_FP)(void);									///<��ȡ��Լ����������

//�����Ǿ����Լ����Ҫʵ�ֵļ���C���ӿڵĽӿ���
const char* const GET_INSTANCE_INTERFACE = "get_protocol";	///<��ȡʵ���ӿ���
const char* const GET_CONFIG_INTERFACE = "get_config";		///<��ȡ���ýӿ���
const char* const GET_VERSION_INTERFACE = "get_version";	///<��ȡ���ýӿ���
const char* const GET_DESC_INTERFACE = "get_desc";			///<��ȡ���ýӿ���

#endif