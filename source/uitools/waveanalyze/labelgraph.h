#ifndef LABELGRAPH_H
#define LABELGRAPH_H

#include <qsize.h>
#include <qwidget.h>
#include <qfontmetrics.h>

#include "vectoritem.h"

const int MINFONTSIZE  = 9;
const int LABEL_BORDERSIZE = 3;
const int DEFAULTFONTSIZE = 12;

// ��Ҫ�����һ������ִ��doublebuffer�Ļ��ƻ��࣬��������ִ��virtual��draw������

// ��ʶ�ؼ�
class QLabelGraph : public QWidget
{
	Q_OBJECT
public:
	QLabelGraph(QWidget* parent);
	~QLabelGraph();

	void Clear(bool bRepaint = true);
	QColor BkColor()			{ return m_Color; }
	QVectorMgr* Items()			{ return m_Items; }
	void SetBkColor(const QColor& clr);

protected:
	int MaxSize();
	QSize FontSize(const QString& str);
	void paintEvent(QPaintEvent* e);

public slots:
	void Change();

private:
	QColor m_Color;
	QVectorMgr* m_Items;

};

#endif