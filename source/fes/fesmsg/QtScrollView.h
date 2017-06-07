#ifndef QT_SCOLL_VIEW_H_
#define QT_SCOLL_VIEW_H_


#include <QAbstractScrollArea>
#include <QToolBar>
#include <QAction>
#include <QDateTime>
#include <QPainter>


#include "CNetModula.h"

class CNetLogFile;

class QtScrollView : public QAbstractScrollArea
{
public:
    QtScrollView(QWidget *parent=0);
	~QtScrollView();
	void DrawModulaData(CNetModulaData* pModulaData, QPainter* pPainter);
	void UpdateScroll();
	int CalcTotalLine(); 
	void setUpDown(int upDown) {m_upDown = upDown;};
	//void AutoSaveModulaData(CNetModulaData* pModulaData, QTextStream &output,QString strtime);


public:
	int m_nCurrentLine;             //��ǰ����
	int m_nTotalLine;               //��ǰҳ��������
	int m_nChar;                    //m_nChar = 29  һ����ʾ�ı�����
	int scrollview_width;           //scrollview�������ؿ��
	int checkbox_state;   //��ѡ��״̬��1�������ģ�0������
	int m_nDescChar;      //δ֪����
	CNetModula* m_pCurrentModula;
	QList<unsigned char> m_keyWordList;



private:
	bool matchkeyWord(unsigned char msg);

private:
	int m_nCharWidth;   //�ַ���ȣ�m_nCharWidth Ϊ5���ַ����
	int m_nLineheight;  ////�ַ��߶�
	int m_upDown;//0:������ 1:���� 2:����

protected:
	 void resizeEvent(QResizeEvent *re);
     void paintEvent(QPaintEvent *pe);  
     void scrollContentsBy(int dx, int dy);

};


#endif
