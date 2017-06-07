#ifndef VECTORITEM_H
#define VECTORITEM_H

#include <QColor>
#include <QString>
#include <QList>

class QLabelGraph;
class QVectorGraph;
class QHarmGraph;


// ʸ����г��������
class QVectorItem
{
public:
	QVectorItem()
	{
		m_fRe = 0;
		m_fIm = 0;
		m_nTag = 0;
		m_Color = Qt::red;
		m_strCaption = "Item";
		m_bShow = true;
	}
	QVectorItem(const QVectorItem& src)
	{
		m_fRe = 0;
		m_fIm = 0;
		m_nTag = 0;
		m_Color = Qt::red;
		m_strCaption = "Item";
		m_bShow = true;
	}
	
	void SetRe(double f)	{ m_fRe = f; }
	void SetIm(double f)	{ m_fIm = f; }
	void SetCaption(const QString& str) { m_strCaption = str; }
	double Re()				{ return m_fRe; }
	double Im()				{ return m_fIm; }
	QColor Color()			{ return m_Color; }
	int Tag()				{ return m_nTag; }
	QString Caption()		{ return m_strCaption; }
	bool IsShow()			{ return m_bShow; }

private:
	double	m_fRe;			// ʵ��
	double	m_fIm;			// �鲿
	int		m_nTag;			//
	QColor	m_Color;		// ��ɫ
	QString m_strCaption;	// ����
	bool	m_bShow;		// ��ʾ

	friend class QLabelGraph;
	friend class QVectorGraph;
	friend class QHarmGraph;
	friend class QVectorMgr;

};


// ʸ�����������

class QVectorMgr
{
public:
	QVectorMgr();
	~QVectorMgr();

	int Count()					{ return m_lstVector.count(); }
	void Clear()				{ m_lstVector.clear(); }
	void Delete(int index);
	QVectorItem* Item(int index);
	QVectorItem* Add(QColor clr, QString str);
	void Connect(QObject* receiver, const char* member);
	
protected:
	void Change()				{ /*emit m_pSig(); */}

private:
	 QList<QVectorItem*>  m_lstVector;
signals:
// 	 void m_pSig();
	
};


#endif