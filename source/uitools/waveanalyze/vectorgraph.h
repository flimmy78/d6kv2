#ifndef VECTORGRAPH_H
#define VECTORGRAPH_H


// #include "globaldef.h"
#include <QObject>
const double VECTORLIMIT = 0.1;		// ʸ����С�ż�ֵ
//const int V_BORDERSIZE = 5;
const int ARROWLENGHT = 10;
const int ANGLESIZE[] = {30, 45};
const QString ANGLECAPTION[] = {QObject::tr("+90��"), QObject::tr("0��"), QObject::tr("-90��"), QObject::tr("180��")};

#include <math.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qfontmetrics.h>
#include "vectoritem.h"
#include "mathutils.h"

// ʸ��ͼ�ؼ�
class QVectorGraph : public QWidget
{
	Q_OBJECT
public:
	QVectorGraph(QWidget* parent);
	~QVectorGraph();

	int ItemIndex()				{ return m_nItemIndex; }
	QColor Color()				{ return m_Color; }
	bool IsShowIndex()			{ return m_bShowIndex; }
	QColor ScaleColor()			{ return m_clrScale; }
	int ScaleCount()			{ return m_nScale; }
	QColor AngleColor()			{ return m_clrAngle; }
	int AngleCount()			{ return m_nAngle; }

	void SetItemIndex(int n);
	void SetColor(const QColor& clr);
	void SetScaleColor(const QColor& clr);
	void SetScaleCount(int n);
	void SetAngleColor(const QColor& clr);
	void SetAngleCount(int n);
	void SetShowIndex(bool b);

	void Clear();
	int Count()					{ return m_Items->Count(); }		
	QVectorMgr* Items()			{ return m_Items; }

	void Change();

signals:
// 	void VectorMenu();

protected:
	void DrawVector(QPainter* paint, int idx, double radis);
	QSize FontSize(const QString& str);
	void paintEvent(QPaintEvent* e);
	void contextMenuEvent(QContextMenuEvent* e);

private:
	int		m_nItemIndex;		// ʸ����׼
	QColor	m_Color;
	bool	m_bShowIndex;		// ��ʾ����
	QColor	m_clrScale;			// �̶���ɫ
	int		m_nScale;			// �̶ȷָ���
	QColor	m_clrAngle;			// �Ƕ���ɫ
	int		m_nAngle;			// �Ƕȷָ���

	QVectorMgr* m_Items;// Ҫ��: TVeItem��Re��Im�Ƿ�ֵ+���
	QPoint m_ptZero;

};


#endif