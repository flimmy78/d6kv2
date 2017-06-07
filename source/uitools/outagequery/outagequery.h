#ifndef POWERCUTMNG_H
#define POWERCUTMNG_H
#include <QTreeWidgetItem>
#include <QDateTimeEdit>
#include <QToolButton>

#include <QEventPara.h>
#include "net/netapi.h"

#include "ui_outagequery.h"

class QPowerCutQuery : public QMainWindow ,public Ui::WindowBase 
{
	Q_OBJECT
public:
	QPowerCutQuery( QWidget* parent = 0 );	
	void iniAreaPage();
	void iniStPage();
	void iniFeederPage();
	void fillTable();
	bool timeChecked(uint time);

public slots:
	void onStQueryBtn() ;
	void onAreaQueryBtn();
	void onFeederQueryBtn();
	void onAllAreaBtn();
	void onAllStBtn();
	void onAllFeederBtn() ;
	void onRead();
	void onExit();
	void onQueryBtnClicked();

private:
	void findRecordByStation();
	void findRecordByArea() ;
	void findRecordByFeederLine() ;
	bool readPowerCutPara();

	bool getCutTypeDesc(uint &cutType, QString &breakName, uint &beginTime, QString &cutTypeDesc);
	bool readUserTypeFile();


private:
	//���ݿ��д��
	QEventPara		m_dbPara;
	QList<POWERCUTPARA *>		m_powerCutParaList;
	QList<POWERCUTPARA *>		m_initPowerCutParaList;

	//ѡ�еĳ�վ����
	QMap<QTreeWidgetItem*,QString> m_item2StCodeMap;

	//ѡ�е�������Ϣ����
	QMap<QTreeWidgetItem*,QString> m_item2AreaCodeMap;

	//ѡ�е������豸����
	QMap<QTreeWidgetItem*,QString> m_item2FeederLineMap;

	QStringList			m_AreaCodeList;
	QStringList			m_stCodeList;
	QStringList			m_feederNameList;

	//�ͻ����͵��ͻ���������
	QMap<uint, QString> m_userType2userDescMap;

	//���ҿ�ʼʱ��
	QDateTimeEdit *m_beginDateTime;

	//���ҽ���ʱ��
	QDateTimeEdit *m_endDateTime;

	//��ѯ��ť
	QToolButton *m_queryBtn;

	SYS_TIME			m_beginTime;

	SYS_TIME			m_endTime;


};
#endif 