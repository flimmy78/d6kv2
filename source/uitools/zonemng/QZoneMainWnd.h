// QZoneMainWnd.h: interface for the QZoneMainWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QZoneMainWnd_H__5C652462_DFE1_42D2_8FA7_888DB4606B41__INCLUDED_)
#define AFX_QZoneMainWnd_H__5C652462_DFE1_42D2_8FA7_888DB4606B41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ui_qzonewndbase.h"
#include "net/netapi.h"
#include "scd/scdapi.h"
#include "QZonePara.h"
#include <QtCore/QList>

class QTreeWidgetItem;


class QZoneMainWnd : public QMainWindow ,public Ui::QZoneWndBase
{
	Q_OBJECT
public:
	QZoneMainWnd( QWidget* parent, const char* name, Qt::WindowFlags fl );
	virtual ~QZoneMainWnd();
public slots:
	virtual void onLogin();
    virtual void onExit();
    virtual void onSave();
    virtual void onLoad();
    virtual void onAddZone();
    virtual void onDeleteZone();
    virtual void onModifyZone();
    virtual void onSelAllZone();
    virtual void onNotSelAllZone();
    virtual void onFuzzySelObj();
    virtual void onSelAllObj();
    virtual void onNotSelAllObj();
    virtual void onAppSel();
    virtual void onAppAll();
	virtual void onUserAppSel();
	virtual void onUserAppAll();
	virtual void onZoneCfgLVclicked( QTreeWidgetItem * pItem );
	virtual void onCurrentPageChanged( QWidget * );
	virtual void showRtuObj(  QTreeWidgetItem * item ) ;

private:

	ProcMan			m_procMan;
	NetDbg			m_netDbg;
	QZonePara		m_zonePara;

	//�û��Ƿ��޸�
	bool			m_userChanged;
	//������Ƿ��޸�
	bool			m_zoneChanged;
	//���������Ƿ��޸�
	bool			m_objChanged;

	//�û�ҳ���Ƿ�ˢ��
	bool			m_userPageFresh;

	//���������ҳ�Ƿ�Ҫˢ��
	bool			m_zoneCfgPageFresh;

	//��������ҳ�Ƿ�Ҫˢ��
	bool			m_zoneViewPageFresh;
// 	QValueList<uint> m_zoneNoList;
	QList<uint> m_zoneNoList;
	QMap<QTreeWidgetItem*, PNODEPARA> m_item2Node;

	QMap<QTreeWidgetItem*, PNODEPARA> m_userItem2Node;

	QMap<QTreeWidgetItem*,QString> m_devPageItem2RtuCodeMap;

private:

	void freshUserPage();
	void freshZoneDefinePage();
	void freshZoneCfgPage();
	void freshZoneViewPage();

	void setObjZone( QTreeWidgetItem *pItem , uint zone, QString zonedesc, bool selon = TRUE );
	void initStyleSheet( void ) ;
};

#endif // !defined(AFX_QZoneMainWnd_H__5C652462_DFE1_42D2_8FA7_888DB4606B41__INCLUDED_)
