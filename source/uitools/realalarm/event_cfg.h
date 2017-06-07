/**
@file event_cfg.h
@brief  ����������ͷ�ļ�

@author lujiashun
@version 1.2.0
@date 2016-9-22
*/
#ifndef EVENT_CFG_H
#define EVENT_CFG_H

#include <QString>
#include <QColor>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QStringList>
#include <QList>

#include "sysstr.h"

enum EmColoring
{
	EVENTSORTCOLOR = 0,//����������������ɫ
	EVENTTYPECOLOR,//���������ͽ�����ɫ
	EVENTRTUCOLOR,//�����վ������ɫ
	EVENTLEVELCOLOR
};

//�ж���
enum
{
	LIST_COL_SORT,
	LIST_COL_TYPE,
	LIST_COL_TIME,
	LIST_COL_DES,
	LIST_COL_FLOAT_VAL,
	LIST_COL_STATUS_VAL,
	LIST_COL_GROUP_NAME,
	LIST_COL_MEMBER_NAME0,
	LIST_COL_MEMBER_NAME1,
	LIST_COL_MEMBER_NAME2,
	LIST_COL_MEMBER_NAME3,
	LIST_COL_GRAPH_NAME,
	LIST_COL_TONE_INFO,
	LIST_COL_SECU_LEV,
	LIST_COL_PRINT_FLAG,
	LIST_COL_SAVE_FLAG,
	LIST_COL_GRAPH_FLAG,
	LIST_COL_WAV_FLAG,
	LIST_COL_NUM
};

class Event_cfg  
{
public:
	Event_cfg();
	virtual ~Event_cfg();
	
	/**
	@brief ������ɫ��ʽ
	*/
	void set_color_type( EmColoring type );

	EmColoring get_color_type();
	/**
	@brief �õ���Ӧ�е�����
	*/
	bool get_colname_by_id( uint index, QString &colname );
	/**
	@brief ����������ȡID
	*/
	int get_colid_by_colname( const QString &colname );

	//����ʾ���
	bool get_col_disp_no( uint index, uint &dispno );
	bool get_col_disp_order( uint index, uint &disporder );
	void set_col_disp_no( uint index, uint dispno );
	bool colIs_disp( uint index );
	void set_col_disp( uint index, bool show = true );
	void set_col_disp( const QString &colname, bool show = true );
	uint get_disp_col_num();
	QString get_col_name( uint dispno );
	bool get_col_id( uint dispno, uint &index );

	//����������ɫ
	void add_sort_color( uint sort, const QColor &clr );
	void add_type_color( uint type, const QColor &clr );
	void add_rtu_color( QString &rtucode, const QColor &clr );

	//�õ�������ɫ
	bool get_sort_color( uint sort, QColor &clr );
	bool get_type_color( uint type, QColor &clr );
	bool get_rtu_color( const QString &rtucode, QColor &clr );

	//�õ�������ɫ
	bool get_level_color( uint level, QColor &clr );

	bool set_level_color( uint level, QColor &clr );

	void set_tone( bool b = true );

	bool get_tone();

	void set_tone_dev( const QString & dev );
	
	QString get_tone_dev();

	void set_keep_unconfirmed_event( bool b = true );

	bool get_keep_unconfirmed_event();

	void set_print( bool b = true );
	
	bool get_print();

	void set_font_size( uint size );

	uint get_font_size();

	void set_event_sound_count( uint type, int soundcount );

	int get_event_sound_count( uint type );

	void set_phone_no( uint type, QString  phoneno );

	QString get_type_phone_no( uint type );

	void set_phone_no_desc(  uint type, QString phoneno, QString desc );
	QString get_send_phone_no( SYS_EVENT* event );

	typedef QMap<QString, QString> PhoneDescMap;
	QMap<int,PhoneDescMap> get_type_to_no_map();


	void set_send_msg( bool b = true );

	bool get_send_msg();

	void set_rcv_msg( bool b);

	bool get_rcv_msg();

	void set_serial_no( QString no );

	QString get_serial_no();

	void set_msg_confirm_time( uint msgconfirmtime);
	
	uint get_msg_confirm_time();

	void set_max_event_num( uint eventnum );

	uint get_max_event_num();

	void set_show_unconfirm( bool b );

	bool get_show_unconfirm();

	// ȷ�Ϻ��Զ�ɾ��
	void set_auto_confirm_del( bool b );

	bool get_auto_confirm_del();

	// �Զ�ƥ��
	void set_auto_match( bool b );

	bool get_auto_match();

	QString get_printer();

	void set_printer(const QString & printer);

	void set_dbg_out( bool b );

	bool get_dgb_out();
	
	/**
	@brief ������ʾ��������
	*/
	void set_show_overhaul_event( bool b );

	bool get_show_overhaul_event();

	void set_report_print( bool b = true );

	bool get_report_print();

	void set_report_print_type( bool b = false );

	int get_report_print_type();

	QMap<QString , QStringList> get_report_map();

	int get_station_count() ;

	void set_report_event_type( QString station, QStringList  eventTypeList ) ;

	QStringList get_report_event_type( QString station ) ;

	void set_report_name( QString station, QStringList  nameList ) ;

	QStringList get_report_name( QString station ) ;

private:
	void create_col_name_map();

private:
	//��������������ɫʱ��ÿ�ַ�����ɫ�б�
	QMap<uint,QColor>		m_event_sort_color;

	//���������ͽ�����ɫʱ��ÿ��������ɫ�б�
	QMap<uint,QColor>		m_event_type_color;

	//����վ��ɫʱ��ÿ����վ������ɫ�б�
	QMap<QString,QColor>	m_event_rtu_color;

	//��������ɫʱ��ÿ�����������ɫ
	QColor					m_level_color[ 8 ];

	//���������������Ͷ�Ӧ��������
	QMap<int,int>			m_event_sound_count_map;

	//���ű����������Ͷ�Ӧ�ֻ�����
	QMap<int,QString>		m_phone_no_map;

	//���ű����������Ͷ�Ӧ����������
	QMap<int,PhoneDescMap>		m_type_to_no_map;


	//������ɫ����
	EmColoring				m_color_type;

	//����ʾ˳���б�
	uint					m_col_id_to_dispno_map[ LIST_COL_NUM ];
	uint					m_col_dispno_to_id_map[ LIST_COL_NUM ];

	QMutex					m_mutex;

	//����ʾ��־
	bool					m_col_disp_flags[ LIST_COL_NUM ];

	//����ŵ���������
	QMap<uint,QString>		m_id_to_col_name;
	//���������������
	QMap<QString, uint>		m_col_name_to_id;
	
	//�Ƿ���������
	bool					m_is_tone;

	//�������������豸���
	QString					m_tone_dev;
	//�Ƿ������ӡ
	bool					m_is_print;
	//�Ƿ�����ȷ������
	bool					m_keep_unconfirmed_event;

	//�����С
	uint					m_font_size;

	//����������
	uint					m_event_num;

	//ֻ��ʾ��ȷ������
	bool					m_is_show_unconfirm_event;

	//�Ƿ���ű���
	bool					m_is_send_msg;

	//  �Ƿ����ȷ��[7/10/2012 ljs116] 
	bool					m_is_rcv_msg;
	//  ����ȷ��ʱ��[7/10/2012 ljs116]
	uint					m_msg_confirm_time;

	QString					m_serial_no;

	//ȷ�Ϻ��Զ�ɾ��
	bool					m_bauto_confirm_del;

	//��Ҫ�Զ�ƥ��			
	bool					m_bauto_math;

	//��ӡ��
	QString					m_printer;

	bool					m_dgb_out;
	
	bool					m_show_overhaul_event; ///< �Ƿ���ʾ��������

	//�Ƿ񱨱��ӡ
	bool					m_is_report_print ;

	int					m_is_vprint ;	///< �Ƿ�����ӡ
	//�����ӡ�б�
	QMap<QString , QStringList>			m_report_event_type ;
	QMap<QString , QStringList>			m_report_name ;

};

#endif