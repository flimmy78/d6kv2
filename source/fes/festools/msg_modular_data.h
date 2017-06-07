/**
@file msg_modular_data.h
@brief ���Ĵ洢

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef MSGMODULARDATA_H_
#define MSGMODULARDATA_H_

#include <QString>
#include <QList>
#include <QDateTime>

#include "fes/fesscdshm.h"

//���Ĵ洢��
class msg_modular_data
{
public:
	msg_modular_data() ;
	~msg_modular_data() ;

public:
	bool set_data( char* pData, int nLength ) ;
	void set_type( int nType ) ;
	void set_protocol_type( QString qprotocol ) ;
	void set_time( QDateTime datetime ) ;
	int get_cdt_frame_cnt() ;
	int get_data_length() ;  //����շ����ĳ���
	int get_type() ;
	int get_exdata_len() ;
	uchar* get_key_flag() ;
	char* get_data() ;  //ȡ�ó��洢���ַ�������ʾ����
	QString get_protocol_type() ;
	QString get_exchange_data() ;  //���ش洢���ַ�����������ʾ
	QString get_time_str() ;

private:
	char*					m_pData ;
	uchar*					m_pKey_flag ;
	int						m_length ;
	int						m_type ;            //�жϱ�����������
	int						m_cdt_frame_cnt ;  //��"*"�ָ�CDT�ַ�����������ʾѭ������
	QString				m_exchange_str ;   //char��ת������洢��QString��
	QString				m_protocol_type ; 
	QDateTime			m_msg_time ;
} ;

#endif