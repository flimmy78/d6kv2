#include <QScrollBar>
#include <QEvent>
#include <QPainter>
#include <QFile>
#include <QDebug>
#include <QResizeEvent>
#include <iostream>
#include <stdio.h>
#include <QString>
#include <cstring>
#include <string>
#include <QDebug>
#include <QByteArray>
#include <QDir>
#include "qglobal.h"
//#include "vld.h"



#include "QtScrollView.h"
#include "CNetModulaData.h"
#include "msgframe.h"


extern float g_LogFileSize ;
extern int   g_LogFileNum  ;
extern int g_LogFileSaveTime ;

using namespace std;



QtScrollView::QtScrollView( QWidget *parent)
: QAbstractScrollArea(parent)
{
	m_nChar = 0;
	scrollview_width = 0;
	m_nDescChar = 0;
	m_nCurrentLine = 0;
	m_nTotalLine = 0;
	checkbox_state = 0;
	m_pCurrentModula = NULL;
    QFontMetrics fm = fontMetrics();
	m_nCharWidth = fm.averageCharWidth();   //ƽ��������
	m_nLineheight = fm.lineSpacing() * 1.2;   //�о�,20130708�޸�
	setUpdatesEnabled(true);
	viewport()->setBackgroundRole(QPalette::Dark);   //���ñ�������ɫ
	viewport()->setAutoFillBackground(true);
}

QtScrollView::~QtScrollView()   //0731����,chengzhu
{
	delete m_pCurrentModula;
	m_pCurrentModula = NULL;



}

void QtScrollView::resizeEvent(QResizeEvent *e)
{
	if(m_nCharWidth == 0)
		return;

	 //e->size().width()Ϊ537������ֵ,
     //m_nCharWidth Ϊ5���ַ����
	 //m_nChar = 29  һ����ʾ�ı�����
	 //m_nDescChar = 88
	m_nChar = (e->size().width() / m_nCharWidth) / 3; 
	scrollview_width = e->size().width();   //��ʾ���ڿ��
	
	if(m_nChar < 0)
		m_nChar = 0;
	m_nDescChar = (e->size().width() / m_nCharWidth) - 1;
	if(m_nDescChar < 0)
		m_nDescChar = 0;
	m_nTotalLine = CalcTotalLine();
	if(m_nTotalLine == 0)
		return;
    verticalScrollBar()->setPageStep(viewport()->height());
	//verticalScrollBar()->setSingleStep(m_nLineheight);   //0814,chengzhu
    verticalScrollBar()->setRange(0, qMax(0, (m_nTotalLine+1)*m_nLineheight - viewport()->height()));
	QAbstractScrollArea::resizeEvent(e);
}

//void QtScrollView::AutoSaveModulaData(CNetModulaData* pModulaData, QTextStream &output, QString strtime)
//{
//
//	if(!pModulaData)
//		return;
//
//	
//	QString strTitle;
//
//	switch(pModulaData->GetType())
//	{
//	case 1:
//		strTitle.sprintf("RECV(%d)", pModulaData->GetDataLength());
//		strTitle += _C2Q("        ��ǰʱ��:");
//		strTitle += strtime;
//		strTitle += _C2Q("\n");
//		break;	
//	case 2:
//		strTitle.sprintf("SEND(%d)", pModulaData->GetDataLength());
//		strTitle += _C2Q("        ��ǰʱ��:");
//		strTitle += strtime;
//		strTitle += _C2Q("\n");
//		break;
//	default:
//		strTitle.sprintf("UNKNOW(%d)\n", pModulaData->GetDataLength());
//		/*strTitle += _C2Q("        ��ǰʱ��:");
//		strTitle += strtime;
//		strTitle += _C2Q("\n");*/
//		break;
//	}
//
//	if (1 == pModulaData->GetType() || 2 == pModulaData->GetType())
//	{
//		output << strTitle;
//	}
//	
//	
//	if (1 == pModulaData->GetType() || 2 == pModulaData->GetType())
//	{
//		QString strTemp;
//		int nCount = 0;
//		
//		while(nCount < pModulaData->GetDataLength())
//		{
//			for(int i = 0; i < 16; i++)
//			{
//				if(nCount + i < pModulaData->GetDataLength())
//				{
//					strTemp.sprintf("%02X ", (BYTE)(*(pModulaData->GetData()+nCount+i)));
//					output << strTemp;
//				}
//			}
//			output << QString("   ");
//			output << QString("\n");
//			nCount += 16;
//		}
//	}
//}

void QtScrollView::paintEvent(QPaintEvent *pe)
{
	
 	QPainter painter;
 	painter.begin(viewport());
 	painter.setPen(Qt::black);
 	painter.fillRect(pe->rect(), QColor(214,214,214));

	m_nCurrentLine = 0;

	if (NULL == m_pCurrentModula)
	{
		return;
	}

	if (m_pCurrentModula->m_ListData.isEmpty())
	{
		return;
	}

	QListIterator<CNetModulaData*> showIterator(m_pCurrentModula->m_ListData);
	showIterator.toFront();

	while(showIterator.hasNext())
	{
	
		CNetModulaData* pModulaData = showIterator.next();
		if(NULL != pModulaData)
		{		
			DrawModulaData(pModulaData, &painter);

		}
	}

     painter.end();
}

//��ʾ
void QtScrollView::DrawModulaData(CNetModulaData* pModulaData, QPainter* pPainter)
{
	if ((pModulaData == NULL ) || (pPainter == NULL) || (m_nLineheight == 0) || (m_nChar == 0) || (m_nDescChar == 0))
	{
		return;
	}

	int iScrollBarValue = verticalScrollBar()->value();   //���鵱ǰֵ
	int iStartLine = iScrollBarValue/m_nLineheight;      //0-30��һҳ
 	int iEndLine = ((iScrollBarValue + viewport()->height())/m_nLineheight) - 1;  //434/14 -1

	//��������
	if (1 == pModulaData->GetType() || 2 == pModulaData->GetType())
	{
		if((((m_nCurrentLine + (pModulaData->GetDataLength() + m_nChar - 1 ) / m_nChar + 1) < iStartLine) 
			|| (m_nCurrentLine > iEndLine)) && checkbox_state == 0 )
		{
			m_nCurrentLine += ((pModulaData->GetDataLength() + m_nChar - 1 ) / m_nChar + 1);   //��ǰ��������
			return;
		}

		if (checkbox_state == 1)   //δ��
		{
			;
		}
	}

	QString strTitle;   //�շ�����ͷSEND&RECV
	QPen pen;
	QFont font;
	font.setBold(false);  //����ͷ�Ӵ�
	font.setPixelSize(14);

	QString mexchange = pModulaData->GetExchange();
	int m_Strlength =  pModulaData->GetExchange().length();  //U֡����52,��ʱ����199

	int i = 0;
	int j = 0;

	int nCount = 0;
	QString strTemp;
	QString strtime;
	
	//���Ϸ��ͽ���ͷ
	switch(pModulaData->GetType())
	{
	case 1:
		pPainter->setFont(font);
		pPainter->setPen( Qt::darkGreen ) ;
		pPainter->setBackground(Qt::green);
		strTitle.sprintf("RECV(%d)", pModulaData->GetDataLength());
		strTitle += _C2Q("        ��ǰʱ��:");
		strtime = pModulaData->GetStrTime();
		strTitle += strtime;
		break;
	case 2:
		pPainter->setFont(font);
		pPainter->setPen(Qt::blue);
		strTitle.sprintf("SEND(%d)", pModulaData->GetDataLength());
		strTitle += _C2Q("        ��ǰʱ��:");
		strtime = pModulaData->GetStrTime();
		strTitle += strtime;
		break;
	default:
		strTitle.sprintf("UNKNOW(%d)", pModulaData->GetDataLength());
		/*strTitle += _C2Q("        ��ǰʱ��:");
		strTitle += strtime;*/
		break;
	}

	int str_count = m_Strlength / 80;   //QString�ֵ�����������drawtext��ʾ

	if (pModulaData->GetProtolType() == "iec104")
	{
		
		if ((m_Strlength % 80) > 0)
		{
			str_count ++;
		}

		//80�ַ�һ����ʾ
		for (int i = 0 ;i < str_count; i++)
		{
			mexchange.insert(80*(i+1)+j, ",");
			j++;
		}

		//�ָ��ַ���
		QStringList str_list = mexchange.split(",");

		//���д���
		for (i = 1; i < str_count; i++)
		{

			//mid()��remove()��0��ʼ
			if (str_list[i].mid(0,1) == "0"  && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "0";
				str_list[i].remove(0,1);  
			}

			if (str_list[i].mid(0,1) == "1" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "1";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "2" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "2";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "3" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "3";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "4" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "4";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "5" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "5";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "6" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "6";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "7" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "7";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "8" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "8";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "9" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "9";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "A" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "A";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "B" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "B";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "C" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "C";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "D" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "D";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "E" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "E";
				str_list[i].remove(0,1);
			}

			if (str_list[i].mid(0,1) == "F" && (str_list[i].mid(1,1) == " " || str_list[i].mid(1,1) == "("))
			{
				str_list[i-1] += "F";
				str_list[i].remove(0,1);
			}

		}

		//��������
		if ((1 == pModulaData->GetType() || 2 == pModulaData->GetType()) && checkbox_state == 1)
		{

			//��ʾ����
			if ((m_nCurrentLine >= iStartLine) && (m_nCurrentLine < iEndLine ))
			{		
				//������ͷ
				pPainter->drawText(0, m_nLineheight*(m_nCurrentLine-iStartLine+1), strTitle);
			}
			m_nCurrentLine++;

			font.setBold(false);
			font.setPixelSize(14);
			pPainter->setFont(font);
			pPainter->setPen(Qt::black);

			//������
			for(i = 0; i < str_count; i++ )
			{
				pPainter->drawText(0, m_nLineheight*(m_nCurrentLine-iStartLine+1), str_list[i]);
				m_nCurrentLine++;
			}

		}


	}

	if (pModulaData->GetProtolType() == "CDT")
	{
		//�ָ��ַ���
		QStringList str_list = mexchange.split("*");

		if ((1 == pModulaData->GetType() || 2 == pModulaData->GetType()) && checkbox_state == 1)
		{

			//��ʾ����
			if ((m_nCurrentLine >= iStartLine) && (m_nCurrentLine < iEndLine ))
			{		
				//������ͷ
				pPainter->drawText(0, m_nLineheight*(m_nCurrentLine-iStartLine+1), strTitle);
			}
			m_nCurrentLine++;

			font.setBold(false);
			font.setPixelSize(14);
			pPainter->setFont(font);
			pPainter->setPen(Qt::black);

			//������
			for(i = 0; i < pModulaData->GetCDTFramecount() + 1; i++ )
			{
				pPainter->drawText(0, m_nLineheight*(m_nCurrentLine-iStartLine+1), str_list[i]);
				m_nCurrentLine++;
			}

		}

	}


if ((1 == pModulaData->GetType() || 2 == pModulaData->GetType()) && checkbox_state ==0)
{
		//��������ͷ
		if ( (m_nCurrentLine >= iStartLine) && (m_nCurrentLine < iEndLine ))
		{
			pPainter->drawText(0, m_nLineheight*(m_nCurrentLine-iStartLine+1), strTitle);
		}
		    m_nCurrentLine++;

			font.setBold(false);
			pPainter->setFont(font);
			pPainter->setPen(Qt::black);

		while(nCount < pModulaData->GetDataLength())
	   {
		for(i = 0; i < m_nChar; i++)
		{
			//ѭ����ӡ����
			if(nCount + i < pModulaData->GetDataLength())
			{
				//��CNetModulaData��ȡ����
				BYTE msg = (BYTE)(*(pModulaData->GetData()+nCount+i));
				strTemp.sprintf("%02X", msg);  //��ӡ���ַ�

				if((m_nCurrentLine >= iStartLine) && (m_nCurrentLine < iEndLine))
				{
					//���������ַ�
					if (matchkeyWord(msg))
					{
						pPainter->setPen(Qt::red);
						pPainter->drawText((1+i*3)*m_nCharWidth, m_nLineheight*(m_nCurrentLine-iStartLine+1), strTemp);
						pPainter->setPen(Qt::black);
					}
					else
					{
						//��ͨ��ʾ
						pPainter->drawText((1+i*3)*m_nCharWidth, m_nLineheight*(m_nCurrentLine-iStartLine+1), strTemp);
					}		
				}
			}

		}

		m_nCurrentLine++;
		nCount += m_nChar;
	}
}
}



void QtScrollView::UpdateScroll()
{
	m_nTotalLine = CalcTotalLine();
	if(m_nTotalLine == 0)
		return;
	verticalScrollBar()->setPageStep(viewport()->height());   //ʹ��pageup��pagedown����ҳ�ķ���
	verticalScrollBar()->setRange(0, qMax(0, (m_nTotalLine+1)*m_nLineheight - viewport()->height()));  //���ù�����Χ
	verticalScrollBar()->setSingleStep(m_nLineheight);   //0814,chengzhu����������ͷ��������
	verticalScrollBar()->setValue((m_nTotalLine+1)*m_nLineheight - viewport()->height());  //����һ�ι����������ķ���
}

void QtScrollView::scrollContentsBy(int dx, int dy)
{
	viewport()->update();
}

int QtScrollView::CalcTotalLine()
{
	if((m_nChar == 0) || (m_nDescChar == 0))
	{
		return 0;
	}

	if(NULL == m_pCurrentModula)
	{
		return 0;
	}

	int iTotalLine = 0;

	QListIterator<CNetModulaData*> calcIterator(m_pCurrentModula->m_ListData);  //�洢�ı��ģ���һ��ʼ������
	calcIterator.toFront();


	while(calcIterator.hasNext())
	{
		CNetModulaData* pModulaData = calcIterator.next();

		if(pModulaData)
		{
			//if(1 == pModulaData->GetType() || 2 == pModulaData->GetType())
			//{
			//	if (checkbox_state == 1)
			//	{
			//		iTotalLine +=  (pModulaData->GetexDataLength() + 80)/80 + 1;
			//	}
			//	else
			//		iTotalLine += ( pModulaData->GetDataLength() + m_nChar - 1 ) / m_nChar + 1;
			//}
			//else
			//{
			//	if (checkbox_state == 1)
			//	{
			//		iTotalLine +=  (pModulaData->GetexDataLength() + 80)/80 + 1;   //����������
			//	}
			//	else
			//		iTotalLine += ( pModulaData->GetDataLength() + m_nDescChar - 1 ) / m_nDescChar;

			//}
			


			//ԭ�������������104��CDT����ʱ������������
			
			if((1 == pModulaData->GetType() || 2 == pModulaData->GetType()) && (checkbox_state == 0))
			{
				//һ������ռ3�У�һ��LIST���Ԫ��*3=iTotalLine������
				iTotalLine += ( pModulaData->GetDataLength() + m_nChar - 1 ) / m_nChar + 1;
			}
			/*else
			{
				iTotalLine += ( pModulaData->GetDataLength() + m_nDescChar - 1 ) / m_nDescChar;
			}*/


			//����CDT����
			if((1 == pModulaData->GetType() || 2 == pModulaData->GetType()) && (checkbox_state == 1) && (pModulaData->GetProtolType() == "CDT"))
			{
				iTotalLine += ( pModulaData->GetDataLength() + m_nChar - 1 ) / m_nChar + 1;
			}
			/*else
			{
				iTotalLine += ( pModulaData->GetDataLength() + m_nDescChar - 1 ) / m_nDescChar;
			}*/

			//����104����
			if((1 == pModulaData->GetType() || 2 == pModulaData->GetType()) && (checkbox_state == 1) && (pModulaData->GetProtolType() == "iec104"))
			{
				iTotalLine += ( pModulaData->GetDataLength() + m_nChar - 1 ) / m_nChar + 1;
			}
			/*else
			{
				iTotalLine += ( pModulaData->GetDataLength() + m_nDescChar - 1 ) / m_nDescChar;
			}*/


		}
	}//while(calcIterator.hasNext())
	//qDebug() << "iTotalLine + 1 = " << iTotalLine+1;
	return iTotalLine + 1;
}


bool QtScrollView::matchkeyWord(unsigned char msg)
{
	if (m_keyWordList.isEmpty())
	{
		return false;
	}

	QListIterator<BYTE> findIterator(m_keyWordList);
	findIterator.toFront();
	while(findIterator.hasNext())
	{
		BYTE key = findIterator.next();

		if (key == msg)
		{
			return true;
		}
	}

	return false;
}



