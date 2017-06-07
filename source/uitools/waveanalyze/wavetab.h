/**
@file wavetab.h
@brief tabҳ�����ʾͷ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-11-07
*/
#ifndef WAVETAB_H
#define WAVETAB_H

#include <QTabBar>
#include <QTabWidget>
#include <QStatusBar>
#include <QLineEdit>
#include <QAction>
#include <QMenu>


/* ���ڱ�ǩ */
class QWaveTabbar : public QTabBar
{
	Q_OBJECT
public:
	QWaveTabbar(QWidget * parent = 0);
	~QWaveTabbar() {}

protected:
	void mouseDoubleClickEvent(QMouseEvent* e);    
	void contextMenuEvent(QContextMenuEvent* e);

signals:
	void signal_item_close(int index);
	void signal_item_property(int index);
private slots:
	void close_tab(void);
	void tab_property(void);
private:
	void create_action(QWidget* parent);

private:
	QAction *action_close;		//�ر�
	QAction *action_property;	//�ļ�����
	QMenu *menu_tab;

	int m_current_index;

	
};

/* ��ҳ */
class QWaveTabWidget : public QTabWidget
{
	Q_OBJECT
public:
	QWaveTabWidget(QWidget * parent = 0);
	~QWaveTabWidget();

	QWaveTabbar* WaveTabbar() { return m_pTabbar; }

protected:
	QWaveTabbar* m_pTabbar;
};


/* ״̬�� */
class QWaveBar
{
public:
	QWaveBar(QStatusBar* bar);
	~QWaveBar();

	void Message(int n, const QString& message);
	void Clear();

private:
	QStatusBar* m_pBar;
	QLineEdit* m_pEdit[4];

protected:
	QLineEdit* CreateEdit(int len = -1);

};

#endif