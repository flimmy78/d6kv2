/**
@file Mirror_Proc.h
@brief III��ʵʱ���ݴ����߳�ͷ�ļ�

III��ʵʱ���ݴ���

@author lujiashun 
@version 1.0.0
@date 2016-09-12
*/
#ifndef MIRROR_PROC_H_
#define MIRROR_PROC_H_

#include <QtCore/QThread>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QDebug>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>

#include "fes/platformwrapper.h"
#include "fes/femailstruct.h"

#define  MAX_FILE_DATA_NUM 4096

#define _Q2C(str) ((const char *)((str).toLocal8Bit().data())) //  [1/15/2013 ljs]
#define g_Q(str) (QString::fromLocal8Bit(str))

/**
@brief III��ʵʱ���ݴ����߳���

�����ȡIII��ǰ�÷�����һ����ʵʱ�����ļ�����ȡ��
���ݺ�ˢ�빲���ڴ�
*/
	/**
	@brief ����ǰ�ù����ڴ�
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
class  Mirror_Proc: public QThread
{
public:
	Mirror_Proc();
	~Mirror_Proc( );
	/**
	@brief �߳������ļ���ѭ����ȡ�����ļ�����д�빲���ڴ�
	*/
	void run(void); //�߳����壬���ڽ����ݻ����Ϊ�ļ�

	void stop();
private:

	/**
	@brief ɨ��ָ���ļ������Ƿ����µ������ļ�
	@return �ɹ�����1��ʧ�ܷ���0
	*/
	bool scan_datafile(QStringList& file_list);

	/**
	@brief ��ȡ�ļ����е����ݲ�����list
	*/
	void convert_file_to_list(QString filename);

	void send_data_to_scada( int data_num );

	void send_soe_to_scada( int soe_num );

	void send_channel_to_scada( int channel_num );

	void send_ykcheck_to_scada( int ykcheck_num );


private:
	FE_REAL_DATA_NODE*		m_data;
	FE_DATA_EVENT_NODE*		m_soe;
	FE_CHANNEL_STAT_MAIL*	m_channel;
	FE_YK_CHECK_NODE*		m_ykcheck;

	FE_MAIL_STRUCT* m_data_mail_struct;
	FE_MAIL_STRUCT* m_stat_mail_struct;

	QDateTime	m_last_modify_time;
};

#endif
