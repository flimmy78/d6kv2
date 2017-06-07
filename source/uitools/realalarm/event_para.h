/**
@file event_para.h
@brief ���������ͷ�ļ�
�����ȡ���ݲ�����
@author lujiashun
@version 1.2.0
@date 2016-9-23
*/
#ifndef EVENT_PARA_H
#define EVENT_PARA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QString>
#include <QList>
#include <QMap>
#include <QStringList>
#include <QMutex>
#include <QObject>

#include "db/dbapi.h"

#define EVENT_SORT_NUM	12

typedef struct _GroupPara
{
	uint	no;
	QString code;
	QString desc;
}GROUPPARA,*PGROUPPARA;

typedef struct _StationPara
{
	uint	groupno;	//���
	QString	code;		//����
	QString	desc;		//����
	uint	areano;		//�������
}STATIONPARA,*PSTATIONPARA;

typedef struct _AreaPara
{
	int	areano;			//id
	QString	desc;		//name
	int masterareano;	//masterid
}AREAPARA,*PAREAPARA;

typedef struct _RtuPara
{
	uint	no;			//�ն����
	QString	code;		//����
	QString	desc;		//����
	QString	rtucode;	//վ����ϵ����
}RTUPARA,*PRTUPARA;		

typedef struct _YcPara
{
	QString	stationcode;//վ����ϵ����
	QString	code;		//����
	QString	desc;		//����
	QString devcode;	//�豸����
	uint	type;		//����
}YCPARA,*PYCPARA;

typedef struct _YxPara
{
	QString	stationcode;//վ����ϵ����
	QString	code;		//����
	QString	desc;		//����
	QString devcode;	//�豸����
	uint	type;		//����
}YXPARA,*PYXPARA;

typedef struct _EventTypeDef
{
	uint	sort;		//��������
	uint	type;		//�������ͺ�
}EVENTTYPEDEF,*PEVENTTYPEDEF;

typedef struct _DevPara
{
	QString	stationcode;//վ����ϵ����
	QString	code;		//����
	QString	desc;		//����
	uint	type;		//�豸����
}DEVPARA,*PDEVPARA;

typedef struct _DevTypePara
{
	uint	devtype;	//�豸���ͺ�
	QString	desc;		//����
}DEVTYPEPARA,*PDEVTYPEPARA;

typedef struct __tagREPORTNAMEPARA
{
	char  rptname[64] ;
	char  rpttype[64] ;
	uint  webid ;
}REPORTNAMEPARA,*PREPORTNAMEPARA;

class Event_para
{
public:
	Event_para();
	virtual ~Event_para();

	bool read_para();

	uint get_group_count();
	bool get_group_desc( uint id, QString &desc );
	bool get_group_no( uint id, uint &no );

	bool get_station_code( uint groupno, QStringList &stations );
	bool get_station_desc( const QString &code, QString &desc );

	//bool getRtuCode( uint rtuno, QString &code );
	//bool getRtuDesc( const QString code, QString &desc );

	bool get_yc_code( const QString &rtucode, QStringList &yccode );
	bool get_yc_desc( const QString &code, QString &desc );

	bool get_yx_code( const QString &rtucode, QStringList &yxcode );
	bool get_yx_desc( const QString &code, QString &desc );
	bool get_yx_type( const QString &code, uint &type );

	uint get_event_sort_count();
	bool get_event_sort_desc( uint sort, QString &desc );

	uint get_event_type_count();
	PEVENTTYPEDEF get_event_type( uint id );
	bool get_event_type_desc( uint type, QString &desc );
	bool get_event_types( uint sort, uint *types, uint &num );

	uint get_event_type_by_desc( QString &desc );

	bool get_dev_code( const QString &rtucode, QStringList &devcode );
	bool get_dev_code( const QString &rtucode, uint devtype, QStringList &devcode );
	bool get_dev_desc( const QString &devcode, QString &devdesc );
	PDEVPARA get_dev( const QString &devcode );

	uint get_dev_type_num();
	bool get_dev_type( uint id, PDEVTYPEPARA devtype );
	//�õ����豸��ص�ʵʱ��
	bool get_dev_real_code( const QString &devcode, QStringList &codes );

	uint get_ip_num();
	bool get_ip_by_id( uint id, QString &ip );

	bool m_bconfirm_event_type( uint type );

	int get_report_count();

	QStringList get_report_name() ;

	bool get_report_Id( QString name , int& id ) ;

	QList<QStringList> get_station_para() ;
	QList<QStringList> get_area_para() ;


private:
	bool read_area_para() ;

	bool read_group_para();
	bool read_station_para();
	bool read_rtu_para();
	bool read_yc_para();
	bool read_yx_para();
	bool read_dev_para();
	bool read_event_type();
	bool read_dev_type_para();
	bool read_node_table();
	bool read_confirm_event();
	bool read_report_name() ;

	bool read_file( char *filename, void ** buf );
	bool read_file( char *filename, void * buf, int len );

private:

// 	QPtrList<GROUPPARA>		m_groupPara;
// 	QPtrList<STATIONPARA>	m_stationPara;
// 	QPtrList<RTUPARA>		m_rtuPara;
// 	QPtrList<YCPARA>		m_ycPara;
// 	QPtrList<YXPARA>		m_yxPara;
// 	QPtrList<DEVPARA>		m_devPara;
// 	QPtrList<DEVTYPEPARA>	m_devTypePara;
// 	QPtrList<EVENTTYPEDEF>	m_eventTypePara;
	QList<GROUPPARA*>		m_group_para;			//modify by chenkai
	QList<STATIONPARA*>	m_station_para;
	QList<RTUPARA*>		m_rtu_para;
	QList<YCPARA*>		m_yc_para;
	QList<YXPARA*>		m_yx_para;
	QList<DEVPARA*>		m_dev_para;
	QList<DEVTYPEPARA*>	m_dev_type_para;
	QList<EVENTTYPEDEF*>	m_event_type_para;

	//�ն���ŵ�վ����ϵ��������
	QMap<uint,QString>		m_rtuno_to_station_code;

	//�������͵���������
	QMap<uint,QString>		m_type_desc_map;

	//������������������
	QMap<QString,uint>		m_desc_type_map;

	//ң����뵽ң��ṹ����
	QMap<QString, YCPARA*>	m_yccode_to_ycpara_map;

	//ң�Ŵ��뵽ң�Žṹ����
	QMap<QString, YXPARA*>	m_yxcode_to_yxpara_map;

	//�豸���뵽�豸�ṹ����
	QMap<QString, DEVPARA*>	m_devcode_to_devpara_map;

	QStringList				m_host_ip_list;

	DBApi					m_db;

	QList<uint>				m_confirm_event_type;
	int						m_rfile;

	int						m_report_count ;
	QStringList			m_report_name_list ;

	PREPORTNAMEPARA m_report_para ;
	QMap<QString , uint > m_reportname_to_webid ;

	QList<QStringList> m_station_stringlist;
	QList<QStringList>	m_area_stringlist; 
};

#endif 