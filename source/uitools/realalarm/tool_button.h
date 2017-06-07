/**
@file tool_button.h
@brief ��װ�˵���ťͷ�ļ�
���·�װ��ť���Դﵽ�Լ���Ҫ��Ч��
@author lujiashun
@version 1.0.0
@date 2016-09-18
*/

#ifndef TOOL_BUTTON_H
#define TOOL_BUTTON_H

#include <QPushButton>

class Tool_button : public QPushButton
{
	Q_OBJECT
public:
/**
@para in  QString button_name ��ť����
@para in  QString button_desc	��ť����
@para in  QString button_img	ͼ����
@para in  QWidget * parent ������
*/
Tool_button( QString button_name , QString button_desc , QString button_img = 0 , QWidget* parent = 0 ) ;


private:
	void init_tool_button( void ) ;

private:
	QPushButton* m_ptool_button ;
	QString  m_button_name ;		///< ��ť����
	QString  m_button_desc ;		///< ��ť����
	QString  m_button_img ;			///< ��ťͼ��
};

#endif