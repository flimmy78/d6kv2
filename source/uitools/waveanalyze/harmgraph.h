#ifndef HARMGRAPH_H
#define HARMGRAPH_H

#include <qstring.h>

const double HARMLIMIT = 0.05;
const int MINHARMSIZE = 9;		// ��С��г�����߶�/���
const QString HARMVALUE[] =
	{"123.24AB", "123.45��", "123.45AB-123.45��"};
const QString HARMFORMAT[] =
	{"%5.2f%s", "%5.2f��", "%5.2f%s-%5.2f��", "%5.2f"};

typedef enum HARMSTYLE{hsVert, hsHorz};

#include <qwidget.h>
#include <qpainter.h>
#include "vectoritem.h"


// г��ͼ�ؼ�
class QHarmGraph : public QWidget
{
	Q_OBJECT
public:
	QHarmGraph(QWidget* parent);
	~QHarmGraph();

	int Count()				{ return m_Items->Count(); }
	void Clear(bool bRepaint = true);
	QVectorMgr* Items()		{ return m_Items; }

	QColor Color()			{ return m_Color; }
	QString Unit()			{ return m_strUnit; }
	QString Caption()		{ return m_strCaption; }
	QColor ItemColor()		{ return m_clrItem; }
	QColor CaptionColor()	{ return m_clrCaption; }
	HARMSTYLE HarmStyle()	{ return m_HarmStyle; }

	void SetColor(const QColor& clr);
	void SetUnit(const QString& str);
	void SetCaption(const QString& str);
	void SetBaseValue(double val);
	void SetItemColor(const QColor& clr);
	void SetCaptionColor(const QColor& clr);
	void SetHarmStyle(HARMSTYLE hs);
	void Change()			{ repaint(); }

protected:
	int MaxSize();
	QSize FontSize(const QString& str);
	void paintEvent(QPaintEvent* e);
	void DrawLine(QPainter* paint, const QPoint& pt, int margin);
	void contextMenuEvent(QContextMenuEvent* e);

signals:
// 	void HarmMenu();

private:
	QColor		m_Color;
	QString		m_strUnit;
	QString		m_strCaption;		// ����
	double		m_fBaseValue;		// ��׼ֵ
	QColor		m_clrItem;			// ������ɫ
	QColor		m_clrCaption;		// ������ɫ
	HARMSTYLE	m_HarmStyle;		// ��ʾ��ʽ����ֱ/ˮƽ

	QVectorMgr* m_Items;
	QPoint		m_ptZero;

};

#endif