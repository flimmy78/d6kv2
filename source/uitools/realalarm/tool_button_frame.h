/**
@file tool_button_frame.h
@brief ��װ�˵���ͷ�ļ�

@author lujiashun
@version 1.0.0
@date 2016-09-19
*/
#ifndef TOOL_BUTTON_FRAME_H
#define TOOL_BUTTON_FRAME_H

#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>

#include "tool_button.h"

class Tool_button_frame : public QFrame
{
	Q_OBJECT
public:
	Tool_button_frame( QWidget* parent = 0 ) ;
	
	/**
	@brief ������ͣ��ť״̬
	*/
	void set_pause_checked(bool);

protected:

private:
	void init_tool_button_frame( void ) ;

	private slots:

public:
	Tool_button*	m_pwin_set_button ;				///< ��������
	Tool_button*	m_pwin_top_button ;				///< �����ö�
	Tool_button*	m_ppause_button ;					///< ��ͣˢ��
	Tool_button* m_pdelete_button;					///< ɾ������
	Tool_button* m_pconfirm_sound_button;		///< ����ȷ��
	Tool_button* m_pconfirm_all_button;			///< ȷ������
	Tool_button* m_pconfirm_event_button;		///< ����ȷ��

	QHBoxLayout *m_phorizontal_layout;
	QSpacerItem *m_phorizontal_spacer;

private:
	bool m_bpause;
};

#endif