/**
@file config.h
@brief �����ļ����������ͷ�ļ�
�����дqtwave.xml�����ļ�����
@author lujiashun 
@version 1.0.0
@date 2016-10-25
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include "globaldef.h"


const QString CONFIG[] =
{
	_C2Q("ftpservername"),
	_C2Q("ftpserverport"),
	_C2Q("ftpusername"),
	_C2Q("ftpuserpassword"),
	_C2Q("localwavefilepath")
};
typedef struct tagConfigSet 
{
	QString m_ftpserver_name;	///<ftp��������
	QString m_ftpserver_port;	///<ftp�������˿�
	QString m_user_name;	///<�û���
	QString m_password;		///<����
	QString m_local_wavefile_path;	///<����¼���ļ�·��
} CONFIG_SET;

class Config
{
public:
	Config();
	virtual ~Config();

private:
	QString m_ftpserver_name;	///<ftp��������
	QString m_ftpserver_port;	///<ftp�������˿�
	QString m_user_name;	///<�û���
	QString m_password;		///<����
	QString m_local_wavefile_path;	///<����¼���ļ�·��
	CONFIG_SET m_config;

public:
	void set_ftpserver_name(QString servername);
	QString get_ftpserver_name();
	void set_ftpserver_port(QString serverport);
	QString get_ftpserver_port();
	void set_user_name(QString username);
	QString get_user_name();
	void set_password(QString password);
	QString get_password();
	void set_local_wavefile_path(QString wavefilepath);
	QString get_local_wavefile_path();
	
	CONFIG_SET get_config_set(void);

	bool read_config_file();
	bool write_config_file();

};


#endif