/**
@file msg_modular.h
@brief ���Ĳ���

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef MSGMODULAR_H_
#define MSGMODULAR_H_

#include <QString>
#include <QDateTime>
#include <QMutex>
#include <QList>
#include <QTextStream>

#include "fes/fesscdshm.h"

#define NETVIEW_MAXBYTE		40960
#define NETVIEW_TOTAL_MAX	40*1024*1024

class QFile ;
class msg_modular_data ;

//���Ĳ�����
class msg_modular
{
public:
	msg_modular() ;
	~msg_modular() ;

	void set_pause( bool bPause ) ;
	bool get_pause() ;
	void set_channel_name( QString channel_name_str ) ;	//����ͨ������
	QString get_channel_name() ;

	void set_max_bytes( int nMaxBytes ) ;
	bool add_data( char* pData, int nLength, int nType, QString qprotocol, QDateTime save_begintime, QDateTime save_endtime,\
		int save_checkbox, QString channel_name, FETIME* msgtime ) ;   //nType��ʾ�����У�pType��ʾ��Լ

	void remove_data( int nTotal ) ;
	void clear_list_data() ;	//�������

	void set_save_to_file( bool b ) ; 	//���汨��
	bool get_save_to_file()  ;
	void set_save_path( QString strPath ) ;	//���ı���·��
	QString get_save_path() { return m_savePath ; }
	void set_end_time( QDateTime t ) { m_tEndSaveTime = t ; }
	QDateTime get_end_time() { return m_tEndSaveTime ; }
	void trace_to_file( char* pdata,int nlength ) ;

	//�Զ��洢����
	QDateTime get_autosave_begintime() {return m_save_begintime ;}
	QDateTime get_autosave_endtime() {return m_save_endtime ;}
	int get_autosave_checkbox_state() {return m_autosave_checkbox_state ;}

	void set_autosave_begintime( QDateTime begintime ) {m_save_begintime = begintime ;}
	void set_autosave_endtime( QDateTime endtime ) {m_save_endtime = endtime ;}
	void set_autosave_checkbox_state( int check_box_type ) {m_autosave_checkbox_state = check_box_type ;}

	void auto_save_message( msg_modular_data* pModulaData, QDateTime save_begintime, QDateTime save_endtime,QTextStream &out ) ;

public:
	//msg_modular_data�洢�ı���ת�浽��,ָ��msg_modular_data�ļ���,���ļ���
	QList<msg_modular_data*> m_List_data ;

private:
	QString m_channel_name_str ;
	int m_nTotalBytes ;
	bool m_bPause ;
	int m_max_bytes ;
	bool m_bSaveToFile ;
	QString m_savePath ;
	QDateTime m_tEndSaveTime ;
	QFile* m_saveFile ;

	QDateTime m_save_begintime ;   //���Ĵ洢��ʼʱ��
	QDateTime m_save_endtime ;     //���Ĵ洢����ʱ��
	int m_autosave_checkbox_state ;
} ;


#endif 