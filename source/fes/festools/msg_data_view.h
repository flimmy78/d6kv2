/**
@file msg_data_view.h
@brief ����չʾ����

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef MSGDATAVIEW_H_
#define MSGDATAVIEW_H_

#include <QAbstractScrollArea>
#include "msg_modular.h"

class msg_data_view : public QAbstractScrollArea
{
	Q_OBJECT
public:
	msg_data_view( QWidget* parent = NULL ) ;
	void draw_modular_data( msg_modular_data* pModula_data, QPainter* pPainter ) ;
	void update_scroll();
	void set_up_down(int up_down) ;

protected:

private:
	void paintEvent( QPaintEvent* pe ) ;
	void resizeEvent( QResizeEvent *re ) ;
	bool match_keyword( unsigned char msg ) ;
	void scroll_contents_by( int dx, int dy ) ;
	int calc_total_line(); 

public:
	int m_total_line;					//��ǰҳ��������
	int m_char;								//m_char = 29  һ����ʾ�ı�����
	int m_scrollview_width;		//scrollview�������ؿ��
	int m_analysis_msg_state;	//��ѡ��״̬��1�������ģ�0������
	int m_desc_char;					//δ֪����
	int m_current_line ;				//��ǰ����
	msg_modular* m_pCurrent_msg_modular;
	QList<unsigned char> m_keyword_list;

private:
	int m_char_width;					//�ַ���ȣ�m_nCharWidth Ϊ5���ַ����
	int m_line_height;					//�ַ��߶�
	int m_up_down;						//0:������ 1:���� 2:����
};

#endif