// QEventPara.h: interface for the QEventPara class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QEVENTPARA_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_)
#define AFX_QEVENTPARA_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QString>
#include <QList>
#include <QMap>
#include <QMutex>
#include <QStringList>

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
}STATIONPARA,*PSTATIONPARA;

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

typedef struct _FeederLinePara
{
	uint	no;			//���
	QString	name;		//����
	QString	desc;		//����
}FEEDERLINEPARA,*PFEEDERLINEPARA;

typedef struct _PowerCutPara	//ͣ��������
{
	QString	code;			//����
	uint	type;			//�豸����
	QString	stationcode;	//վ����ϵ����
	uint	feeder;			//�������ߺ�
	uint	starttime;		//ͣ�翪ʼʱ��
	uint	endtime;		//ͣ�����ʱ��
	QString	breakname;		//�¹ʿ���
	uint	cutType;		//ͣ������
	float	loadValue;		//��ʧ��������
}POWERCUTPARA,*PPOWERCUTPARA;

typedef struct _PlanCutPara	//�ƻ�ͣ��
{
	QString	code;		//����
	QString	desc;		//����
	uint	beginYmd;	//ͣ�翪ʼ����
	uint	beginHms;	//ͣ�翪ʼʱ����
	uint	endYmd;		//ͣ���������
	uint	endHms;		//ͣ�����ʱ����
}PLANCUTPARA,*PPLANCUTPARA;

typedef struct _AreaInfoPara	//���������Ϣ
{
	QString	code;		//����
	QString	desc;		//����
}AREAINFOPARA,*PAREAINFOPARA;

typedef struct _UserInfoPara	//���ͻ���Ϣ
{
	QString	code;			//����
	QString	desc;			//����
	uint	type;			//�ͻ�����
	QString	belongsLoad;	//��������
	QString	belongsArea;	//��������
}USERINFOPARA,*PUSERINFOPARA;

class QEventPara
{
public:
	QEventPara();
	~QEventPara();

	bool readPara();

	bool fineDevByFeederLine( QStringList& devList , QStringList& feederLineList ) ;

	uint getGroupCount();
	bool getGroupDesc( uint id, QString &desc );
	bool getGroupNo( uint id, uint &no );

	bool getStaCode( uint groupno, QStringList &stations );
	bool getStaDesc( const QString &code, QString &desc );

	bool getDevDesc( const QString &devcode, QString &devdesc );

	uint getDevTypeNum();
	bool getDevType( uint id, PDEVTYPEPARA devtype );

	bool getDevTypeNo( uint& devType, QString desc );
	bool getDevTypeDesc( uint devType, QString &desc );
	bool getDevTypeDescs( QStringList &devtypedescs );

	bool getFeederLineDesc( uint &feederNo, QString &feederDesc );

	QList<FEEDERLINEPARA *>	m_feederLinePara;
	QList<PLANCUTPARA *>		m_planCutPara;

	//�������ɵ��ͻ���Ϣ����
	QMap<QString, USERINFOPARA*>	m_belongsLoad2userInfoMap;
	
	//���ߺŵ���������
	QMap<uint, FEEDERLINEPARA*>	m_feederLineNo2FeederLineMap;

	//��������������������
	QMap<QString, QString>		m_areaName2descMap;


private:
	bool readGroupPara();		
	bool readStationPara();
	bool readDevPara();
	bool readDevTypePara();
	bool readFeederLinePara();
	bool readPlanCutPara();
	bool readAreaPara();
	bool readUserPara();

private:

	QList<GROUPPARA *>			m_groupPara;
	QList<STATIONPARA *>		m_stationPara;
	QList<DEVPARA *>				m_devPara;
	QList<DEVTYPEPARA *>		m_devTypePara;

	//�������͵���������
	QMap<uint,QString>		m_typeDescMap;

	//������������������
	QMap<QString,uint>		m_descTypeMap;

	//�豸���뵽�豸�ṹ����
	QMap<QString, DEVPARA*>	m_devCode2DevParaMap;

	//�豸���ͺŵ��豸��������
	QMap<uint, DEVTYPEPARA*>	m_devTypeNo2DevTypeMap;

	//�豸�����������豸��������
	QMap<QString, DEVTYPEPARA*>	m_devTypeDesc2DevTypeMap;


	DBApi	m_db;
};

#endif // !defined(AFX_QEVENTPARA_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_)
