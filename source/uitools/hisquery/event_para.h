/**
@file event_para.h
@brief 

@author chenkai
@version 1.2.0
@date 2016-11-7
*/
#ifndef EVENT_PARA_H
#define EVENT_PARA_H

#include <QString>
#include <QList>
#include <QMap>
#include <QStringList>

#include "db/dbapi.h"

#define EVENT_SORT_NUM	12
#define MAX_EVENT_COUNT 1500 

#define _Q2C(str) ((const char *)(str.toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

typedef struct _GroupPara
{
	uint	no;
	QString code;
	QString desc;
}GROUPPARA,*PGROUPPARA;

typedef struct _AreaPara
{
	int	areano;			//id
	QString	desc;		//name
	int masterareano;	//masterid
}AREAPARA,*PAREAPARA;

typedef struct _StationPara
{
	QString	code;		//����
	QString	desc;		//����
	uint	groupno;	//���
	uint	areano;		//�������
}STATIONPARA,*PSTATIONPARA;

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


class Event_para : public QObject
{
public:
	Event_para();
	virtual ~Event_para();

	bool readPara();

	uint getGroupCount();
	bool getGroupDesc( uint id, QString &desc );
	bool getGroupNo( uint id, uint &no );

	bool readAreaPara() ;
	QList<QStringList> getAreaPara() ;

	//��վ���� �漰��վ����ϵ������(�������)�����������
	bool readStationPara();
	QList<QStringList> getStationPara() ;

	bool getStaCode( uint groupno, QStringList &stations );
	bool getStaDesc( const QString &code, QString &desc );

	bool getYcCode( const QString &rtucode, QStringList &yccode );
	bool getYcDesc( const QString &code, QString &desc );

	bool getYxCode( const QString &rtucode, QStringList &yxcode );
	bool getYxDesc( const QString &code, QString &desc );
	bool getYxType( const QString &code, uint &type );

	uint getEventSortCount();
	bool getEventSortDesc( uint sort, QString &desc );

	uint getEventTypeCount();
	PEVENTTYPEDEF getEventType( uint id );
	bool getEventTypeDesc( uint type, QString &desc );
	bool getEventTypes( uint sort, uint *types, uint &num );

	uint getEventTypeByDesc( QString &desc );

	bool getDevCode( const QString &rtucode, QStringList &devcode );
	bool getDevCode( const QString &rtucode, uint devtype, QStringList &devcode );
	bool getDevDesc( const QString &devcode, QString &devdesc );
	PDEVPARA getDev( const QString &devcode );

	uint getDevTypeNum();
	bool getDevType( uint id, PDEVTYPEPARA devtype );
	//�õ����豸��ص�ʵʱ��
	bool getDevRealCode( const QString &devcode, QStringList &codes );

	uint getIPNum();
	bool getIPByID( uint id, QString &ip );

	bool isConfirmEventType( uint type );

private:
	bool readGroupPara();
	bool readRtuPara();
	bool readYcPara();
	bool readYxPara();
	bool readDevPara();
	bool readEventType();
	bool readDevTypePara();
	bool readNodeTable();
	bool readConfirmEvent();

	bool readFile( char *filename, void ** buf );
	bool readFile( char *filename, void * buf, int len );

private:
	QList<GROUPPARA *>		m_groupPara;
	QList<STATIONPARA *>	m_stationPara;
	QList<RTUPARA *>		m_rtuPara;
	QList<YCPARA *>			m_ycPara;
	QList<YXPARA *>			m_yxPara;
	QList<DEVPARA *>		m_devPara;
	QList<DEVTYPEPARA *>	m_devTypePara;
	QList<EVENTTYPEDEF *>	m_eventTypePara;

	//�ն���ŵ�վ����ϵ��������
	QMap<uint,QString>		m_rtuno2StaCode;

	//�������͵���������
	QMap<uint,QString>		m_typeDescMap;

	//������������������
	QMap<QString,uint>		m_descTypeMap;

	//������������������
	QMap<QString,QStringList*>	m_staCode2ObjCodesMap;

	//ң����뵽ң��ṹ����
	QMap<QString, YCPARA*>	m_ycCode2YcParaMap;

	//ң�Ŵ��뵽ң�Žṹ����
	QMap<QString, YXPARA*>	m_yxCode2YxParaMap;

	//�豸���뵽�豸�ṹ����
	QMap<QString, DEVPARA*>	m_devCode2DevParaMap;

	QStringList				m_hostIPList;

	DBApi					m_db;

	QList<uint>		m_confirmEventType;
	int						m_rfile;

	QList<QStringList> m_stationStringList;
	QList<QStringList>	m_areaStringList; 
};

#endif
