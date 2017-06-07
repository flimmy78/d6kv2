#if !defined(AFX_QZoneMainWnd_H__5C652462_DFE1_42D2_8FA7_888DB4606B41__INCLUDED_)
#define AFX_QZoneMainWnd_H__5C652462_DFE1_42D2_8FA7_888DB4606B41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "net/netapi.h"
#include "scd/scdapi.h"
#include "QZonePara.h"
#include <QString>
#include <QtGui>
#include <QVariant>
#include <QDialog>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QPixmap>
#include "ui_QuserWnd.h"

class QListViewItem;
class QTreeWidgetItem;  //libinghong

class QuserWnd : public QMainWindow , public Ui::QuserWnd
{
	Q_OBJECT
public:
    QuserWnd( QWidget* parent = NULL, const char* name = NULL );
    ~QuserWnd();
	
 public slots:
    virtual void onLogin();
    virtual void onExit();
    virtual void onSave();
    virtual void onLoad();
    virtual void onAddRole();
    virtual void onDeleteRole();
    virtual void onApply();   //libinghong 2012-04-12
	virtual void onSelAllRole();
    virtual void onNotSelAllRole();
	virtual void onreadAuth(  );
    virtual void onModifyNode();		// add [4/12/2012 zhoujian]
	virtual void onAddNode();
	virtual void onDelNode();
	virtual void ViewAuth();			//���ݲ���Ա��

	virtual void onAddUser();
    virtual void onDeleteUser();
    virtual void onModifyUser();

	virtual void onSelAllAuth();	//  [5/2/2012 zhoujian]
	virtual void onSelEptAuth();

   virtual void m_roleCfgLVclicked( QListViewItem * );
   virtual void onCurrentPageChanged(QWidget * );

   virtual void GetAuthCheck(QTreeWidgetItem* pItem);

	
private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;
    QPixmap image4;
    QPixmap image5;
    QPixmap image6;
    QPixmap image7;
    QPixmap image8;
    QPixmap image9;
    QPixmap image10;
    QPixmap image11;

//	Ui::QuserWnd ui;

//	ProcMan			m_procMan;
//	NetDbg			m_netDbg;

	QMangerPara		m_zonePara;
	
	//�û��Ƿ��޸�
	bool			m_userChanged;
	//��ɫ�Ƿ��޸�
	bool			m_roleChanged;   /* libinghong */
	bool			m_roleauthChanged; //  [4/26/2012 zhoujian]
	bool			m_rolecfgChanged;
	
	//�û�ҳ���Ƿ�ˢ��
	bool			m_userPageFresh;
	
	//��ɫ����ҳ�Ƿ�Ҫˢ��
	bool			m_RolePageFresh;
	bool			m_rolecfgFresh;  //libinghong
	
	//Ȩ�����ҳ�Ƿ�Ҫˢ��
	bool			m_userAuthChanged;
	bool			m_MangerPageFresh;
	bool			m_bchangedAuth;

	QMap<QTreeWidgetItem*, PNODEPARA> m_item2Node;
	QMap<QTreeWidgetItem*, PNODEPARA> m_userItem2Node;
	QMap<int,QString> roletypemap;

	QList<QTreeWidgetItem *> m_treeItemList;
	QList<PNODEPARA> m_addrolelist;
	QList<PNODEPARA> m_delrolelist;
	QList<PNODEPARA> m_addyuserlist;
	QList<PNODEPARA> m_delyuserlist;
	QList<PUSERPARA> m_addusercfglist;
	QList<PUSERPARA> m_delusercfglist;
	QList<PUSERPARA> m_chageusercfglist;


	int m_roletype;
	bool m_grpleader;
	int				m_beforeSelRow;

	void freshrolePage();		/* libinghong */
	void freshroleCfgPage();
	void freshUserPage();
	void freshMangerCfgPage();
	void SetAuthCheck(QTreeWidgetItem* pItem);	//���ݽڵ�
	void onmodAuth( PNODEPARA pNode);  //����Ȩ��
	void RoleToUser();
	void CheckItem();	//

	void setObjZone( QListViewItem *pItem , uint zone, QString zonedesc, bool selon = TRUE );
	void initStyleSheet() ;
};
#endif
