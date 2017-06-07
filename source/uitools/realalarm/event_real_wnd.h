/**
@file event_real_wnd.h
@brief ���ܸ澯��������ͷ�ļ�

@author lujiashun
@version 1.2.0
@date 2016-9-23
*/
#ifndef EVENT_REAL_WND_H
#define EVENT_REAL_WND_H

#include <QMainWindow>
#include <QList>
#include <QIcon>
#include <QFont>
#include <QProcess>

#include "scd/scdapi.h"
#include "net/netapi.h"
#include "event_table.h"
#include "event_para.h"
#include "event_cfg.h"
#include "event_sound.h"
#include "read_event_proc.h"
#include <qstatusbar.h> 
//#ifdef _WIN32
#include "event_msg.h"
#include "globaldef.h"
//#endif

#include "event_print.h"
#include "title_frame.h"
#include "tool_button_frame.h"


#define DATABUFLEN	10240

class QTabWidget;
class QToolButton;
class QSplitter;
class QFrame;

class Event_real_wnd : public QMainWindow  
{
	Q_OBJECT
public:
	Event_real_wnd( QWidget * parent = 0, const char * name = 0, Qt::WindowFlags f = /*WType_TopLevel | */Qt::WindowStaysOnTopHint );
	virtual ~Event_real_wnd();

// 	void customEvent(QCustomEvent *);

	QTabWidget * m_tab_widget;
	QWidget* m_centre_widget;
	QFrame* m_tab_frame;
	QSplitter  * m_splitter;
	QVBoxLayout* m_vbox_layout;
	QVBoxLayout* m_vtab_layout;
	QLabel* m_status_label;


	Title_frame	*m_title_frame;
	Tool_button_frame* m_tool_button_frame;

	//ҳ���б�
	QList<Event_table*>	m_page_list;

	//�����б�
	QList<Event_table*>	m_wnd_list;

	//�������ð�ť
	QToolButton	*m_ui_set_btn;

	//ϵͳ���ð�ť
	QToolButton *m_sys_set_btn;

	//ϵͳ�˳���ť
	QToolButton *m_sys_exit_btn;

	//����ȷ�ϰ�ť
	QToolButton *m_confirm_event_btn;

	//����ȷ�ϰ�ť
	QToolButton *m_all_confirm_event_btn;

	//����ȷ�ϰ�ť
	QToolButton *m_confirm_snd_btn;

	//����ɾ����ť
	QToolButton *m_delete_btn;

	//��ͣ��ť
	QToolButton *m_pause_btn;

	//�����ö����ڰ�ť
	QToolButton *m_topmost_btn;

	//����������ť
	QToolButton *m_test_wav_btn;
	
	//���ݿ��д��
	Event_para		m_db_para;

	//���ù�����
	Event_cfg		m_event_cfg;

	//��������������
	Event_sound		m_event_sound;

	//���ű�����
//#ifdef _WIN32
	Event_msg		m_event_msg;
//#endif

	//ע���Ƿ�ɹ�
	bool			m_breg_successed;

	//��ǰ�û�
	USEROPERRIGHT_STRU m_cur_user;

	DISPATCHER_STRU    m_cur_oper_man;

	QMutex	m_mutex;

	//����ʵʱ����ָ���б�
	Sys_event_list		m_real_event_list;

	//�����˶��ŵ������б�
	Sys_event_list		m_event_msg_list;

	//��ӡ�б�
	QList<Rsys_event *> m_print_list;
	
	//��ʼ���������Ͱ�ť��
	void init_title_and_toolbtn_frame( void );

	//�������ݶԱ�
	void confirm_msg_event( Sys_event_list & eventlist );

	//����ȷ��
	void confirm_event( Sys_event_list & eventlist );

	//����ɾ��
	void delete_event( Sys_event_list & eventlist );

	//�������ڵ㷢��ȷ��������Ϣ
	void send_msg( Sys_event_list & eventlist );
	
	//���ʵʱҳ
	void add_page( Event_table *table );

	void set_page_name( Event_table *table, const QString &title );

	//��Ӵ���
	void add_wnd( Event_table *table );

	//ɾ��ʵʱҳ
	void del_page( uint index );

	//ɾ������
	void del_wnd( uint index );

	bool m_bnew_event( Rsys_event*eve );

	//ˢ������ҳ
	void refresh_table();

	void save_event();
	void load_event();

	bool show_dock_menu( const QPoint & globalPos );

	void closeEvent( QCloseEvent *e );
	void timerEvent ( QTimerEvent * e );
	void showEvent( QShowEvent * e);

	bool get_pause();

	//��ʵʱ������
	Read_event_proc	m_read_event_proc;

	//��ӡʵʱ������
	Event_print m_print;

	bool m_bclose_wnd;

	void statics_events();

	ProcMan & GetProcMan() {return m_proc_man;}

	int  m_page_count;
	int m_current_table_index;
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
// 	bool winEvent(MSG *message, long *result); 
public slots:

	void slot_exit();

	void slot_sys_set();

	void slot_ui_set();

	void slot_top_most();

	void slot_confirm_event();

	void slot_confirm_all_event();

	void slot_confirm_snd();

	void slot_delete_event();

	void slot_pause_btn();

	void slot_show_page( int );

	void slot_test_wav_btn();

	void slot_stop_thread();

	void slot_show_help();

	void slot_custom_event( Rsys_event * ,int);

	// add by chenkai 2013��3��12�� 11:23:21
// 	void add(SYS_EVENT *e ,int) ;

private:
	//����ʼ���ļ�
	void read_ini( );

	//д��ʼ���ļ�
	void write_ini( );

	bool reg_proc();

	void unreg_proc();

	void create_socket();

	//��ʵʱ���ж���û�б�ȷ�ϵ�����
	void read_unconfirm_event();

	//��������ȷ�ϱ���
	void on_read_data();

	// �����б����
	void roll_event_list();

	// �˳�������Դ
	void onclose();

	Event_table * get_foucs_table();

	void region(const QPoint &cursorPoint);

private:
	ProcMan			m_proc_man;
	NetDbg			m_net_dbg;

	//checkparent��ʱ��
	int				m_check_timer;
	//����ȷ�ϱ��Ķ�ʱ��
	int				m_recv_data_timer;
	//ɾ�����ʱ��
	int				m_del_event_timer;
	//��ӡ��ʱ��
	int				m_print_event_timer;

	//����ȷ�ϱ���socket
	int				m_event_confirm_socket;

	//�Ƿ���ͣ
	bool			m_bpause;

	//����ȷ�ϱ��Ļ���
	char			m_recv_data_buf[ DATABUFLEN ];
	//����ȷ�ϱ��Ļ���дָ��
	uint			m_write_point;
	//����ȷ�ϱ��Ļ����ָ��
	uint			m_read_point;
	SYS_EVENT       m_sub_SYSEVENT;
	QIcon			m_new_event_icon;
	QIcon			m_no_new_event_icon;
	QFont			m_cur_font;

	QPoint drag_position;
	Direction dir;

	bool m_bleft_press_down;

};

#endif // !defined(AFX_QEVENTREALWND_H__562B2EBC_1D29_4AB4_8744_99B943BE2074__INCLUDED_)
