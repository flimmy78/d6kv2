/**
@file title_frame.h
@brief ��װ������ͷ�ļ�

@author lujiashun
@version 1.0.0
@date 2016-09-18
*/

#ifndef TITLE_FRAME_H
#define TITLE_FRAME_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QSpacerItem>

/**
@brief ��������

�������࣬�̳���QFrame����װ�ñ�����������������ֱ�ӵ���
*/
class Title_frame : public QFrame
{
	Q_OBJECT
public:
	Title_frame( QWidget* parent = 0 ) ;

private:
	/**
	@brief ��ʼ�������Ʊ�����
	*/
	void init_title_frame( void ) ;

protected:
	void mouseDoubleClickEvent( QMouseEvent * event ) ;

private slots:
	/**
	@brief ��С���ۺ���
	*/
	void slot_min_widget( void ) ;
	/**
	@brief ��󻯲ۺ���
	*/
	void slot_max_widget( void ) ;
	/**
	@brief �رղۺ���
	*/
	void slot_close_widget( void ) ;
	/**
	@brief ���ڲۺ���
	*/
	void slot_about_dlg(void);

public:
	QLabel* m_plogo_label ;	///< ͼ���ǩ
	QLabel* m_ptext_label ;		///< �����ǩ
	QPushButton*	m_pmin_button ;		///< ��С����ť
	QPushButton*	m_pmax_button ;	///< ��󻯰�ť
	QPushButton*	m_pclose_button ;	///< �رհ�ť
	QPushButton* m_psetting_button;	///< ���ð�ť
	QPushButton* m_pabout_button;	///< ���ڰ�ť
	QGroupBox*  m_pbutton_group;	///< ��ť��
	QHBoxLayout* m_pbutton_layout;
	QHBoxLayout* m_phbox_layout;
	QSpacerItem* m_pspace;
	bool is_max;		///< �Ƿ���󻯱�־
};

#endif