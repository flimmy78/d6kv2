/**
@file event_ui_set_dlg.h
@brief �������öԻ�����ͷ�ļ�

@author lujiashun
@version 1.2.0
@date 2016-9-23
*/
#ifndef EVENT_UI_SET_DLG_H
#define EVENT_UI_SET_DLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ui_qeventuisetbase.h"
#include <QMessageBox>
#include "uitools/dmessagebox.h"

#include <QMap>

class Event_table;


class Event_ui_set_dlg : public QDialog, public Ui::QEventUISetBase  
{
	Q_OBJECT
public:
	Event_ui_set_dlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
	virtual ~Event_ui_set_dlg();

public slots:
	//����ҳ
	void slot_add_page();
	//�޸�ҳ����
	void slot_modify_page();
	//ɾ��ҳ
	void slot_del_page();
	//˫��ҳ������ҳ��ҳ�б�
	void slot_double_clicked_pagelist( QTreeWidgetItem * item);
	//ˢ��ҳ�б�
	void slot_refresh_pagelist( );

	//ˢ������ҳ���ҳ�б�
	void slot_refresh_pageCB( int item );
	//ˢ�������������
	void slot_refresh_filter( int item );

	void slot_event_type_listview_clicked( QTreeWidgetItem * pItem );

	void slot_save_filter();

	void slot_cancel_btn();

	virtual void show_page ( int );
private:
	//void refreshPage2();
	//void refreshPage3();
	void set_wnd_stylesheet();

	//���������������
	void create_filter_tree();
	void init_para();
	void deal_child_items(Event_table *pTable,QTreeWidgetItem *item);

	void set_child_checkstate(QTreeWidgetItem *item,Qt::CheckState cs);
	void set_parent_checkstate(QTreeWidgetItem *item);
	bool is_topitem(QTreeWidgetItem *item);

private:
	QMap<QTreeWidgetItem*,uint>	m_item_to_eventtype_map;
	QMap<QTreeWidgetItem*,QString> m_item_to_rtucode_map;
	QMap<QTreeWidgetItem*,uint>	m_item_to_pageid_map;

	QMap<int , QStringList> m_station_area_map ;

	QList<QStringList> m_station_area_para ;		//����
	QList<QStringList> m_station_para ;			//��վ

};

#endif 