/**
@file mainformimpl.h
@brief ����������ͷ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-11-01
*/
#ifndef MAINFORMIMPL_H
#define MAINFORMIMPL_H

#include <QTimer>
 #include <QDockWidget>
#include <QGridLayout>
#include <QSpinBox>

#include "comtrade.h"
#include "wavegraph.h"
#include "analyzeutils.h"
#include "wavetab.h"
#include "valueformimpl.h"
// #include "vectorsimpl.h"

#include "ui_mainform.h"

class Wave_caption;

class Main_form_impl : public QMainWindow,public Ui::QMainForm
{
    Q_OBJECT

public:
    Main_form_impl( QWidget* parent = 0);
    ~Main_form_impl();

protected:
	Dlg_value_impl* m_value;	///< ������Ϣ��
// 	QDlgVectorImpl* m_pVectors;	///<ʸ��ͼ
	QTimer*		m_timer_ptr;
	QWaveTabWidget*	m_tab_wave;	///<���Ի���tab
	QWaveBar*	m_status_bar;		///<״̬��
	int		m_auto_play_interval;		///<�Զ���ʾʱ���� ��λ����
	QPixmap*	m_pix_lock[2];

	QDockWidget* m_dock_value;	
	QDockWidget* m_dock_vector;

// 	QGridLayout *QMainFormLayout;
	QVBoxLayout  *QMainFormLayout;

protected:
	/**
	@brief �����α���
	*/
	void add_lines(Wave_graph* graph, int pos,Wave_caption* caption);
	/**
	@brief ��comtrade�ļ���������comtrade����
	*/
	Comtrade* create_comtrade(const QString& fileName, bool bshow = true);
	/**
	@brief ����ͼ����Ϣ������ͼ�κͱ���
	*/
    Wave_graph* create_graph(const QString& caption = QString::null);
	/**
	@brief ��ȡ��ǰtabҳ��ͼ��
	*/
	Wave_graph*	current_wave();
	/**
	@brief ��ȡ��ǰtabҳ�ķ���ģ����Ϣ
	*/
	QAnalyze* current_analyze();
	/**
	@brief ��ȡ��ǰtabҳ��comtrade�ļ���Ϣ
	*/
	Comtrade* current_comtrade();
	/**
	@brief ��ȡ��ǰtabҳ��¼������ͷ��Ϣ
	*/
	Wave_caption* current_caption();

	void set_zoom_box();

	/**
	@brief ����action��ť�Ƿ����ʹ��
	*/
	void set_action_enable(bool enable = true);
	/**
	@brief ��ǰѡ�е�ͨ����
	*/
	int	selected_channel();
	/**
	@brief ���������α�
	*/
	void update_lockline(Wave_graph* graph);
	/**
	@brief ����Data������Analyze
	*/
	void update_analyze(Comtrade* c);

	/**
	@brief ��ʾ�㷨
	*/
	void show_math_type();
	/**
	@brief ��ʾ��ʼʱ��
	*/
	void show_start_time();
	/**
	@brief ��ʾ�α���DeltaT֮��Ĳ�ֵ
	*/
	void update_time_delta(int pos1, int pos2);
	/**
	@brief ��ʾ/���� ʸ��ͼ 
	������ʱδʵ��
	*/
	void view_vectors(int page);
	/**
	@brief ����dockwidget
	@para in str dockwidget��name
	@return �����ɹ���dockwidget
	*/
	QDockWidget* create_dock_widget(const QString& str,QMainWindow* mainWin);

	/**
	@brief ��ʾ/���� ������
	@para in bar ����������
	@para in b	 true:��ʾ  false:����
	*/
	void view_toolbar(QToolBar* bar, bool b);
	/**
	@brief ��дresizeEvent() ������߿�ı�ʱ�����
	*/
	virtual void resizeEvent( QResizeEvent * event );
public:
	Gain_data*	Data();

	/**
	@brief ��¼���ļ�
	*/
	void open_comtrade_file(const QString& fileName);
	/**
	@brief �½�¼���ļ�
	���Ѿ���һ��¼���ļ�������£����´�һ��¼���ļ�
	*/
	void new_comtrade_file(const QString& fileName);
		/**
	@brief �ر�¼���ļ�
	*/
void close_comtrade_file(int idx, bool ask = true);

public slots:
	// file
    void slot_file_open();	///<���ļ�
	void slot_file_new();	///<�½��ļ�
	void slot_file_close();	///<�ر��ļ�
	void slot_file_print();	///<��ӡ�ļ�
	void slot_exit();	///<�˳�����
	void slot_file_property();		///<�ļ�����
	void slot_file_view();		///<�ļ��б�
	void slot_down_load_file();		///< ����¼���ļ� FTP�Ի���

	// view
	void slot_view_fullscreen();			///<ȫ��
	void slot_view_show_all();			///<��ʾ��������
	void slot_view_auto_set();			///<����ͼ�Զ�����
	void slot_view_draw1();				///<�����ػ���
	void slot_view_draw2();				///<ѹ��/������һҳ
	void slot_view_auto_play();			///<�Զ���ʾ
	void slot_view_to_here();			///<��ʱ�������

	// toolbar
	void slot_view_mainbar(bool isToggled);		///<�鿴��������
	void slot_view_anabar(bool isToggled);			///<�鿴����������
	void slot_view_editbar(bool isToggled);		///<�鿴ͼ�α༭������
	void slot_visible_changed1(bool visible);		///<��������״̬�ı�
	void slot_visible_changed2(bool visible);		///<����������״̬�ı�
	void slot_visible_changed3(bool visible);		///<ͼ�α༭������״̬�ı�

	// analyze
	void slot_analyze_value();			///<������Ϣ
	void slot_analyze_harm();			///<г����ͼ 
	void slot_analyze_vector();			///<����ͼ
	void slot_analyze_delete();			///<ɾ������
	void slot_analyze_harm_wave();			///<г����ͼ

	// setting
	void slot_set_zero();		///<���ʱ�����
	void slot_set_fault();		///<��ʱ���
	void slot_set_property();		///<����ͼ����
	void slot_set_options();		///<ѡ��

	// help
	void slot_help_about();			///<����

    // edit
	void slot_edit_zoom_YIn();			///<����Ŵ�
	void slot_edit_zoom_Yout();		///<������С
	void slot_edit_lock();		///<�����α�
	void slot_edit_home();	///<�α��ƶ��������ײ�
	void slot_edit_end();	///<�α��ƶ�������β��
	void slot_edit_move_up();	///<ǰ��һ��
	void slot_edit_move_down();	///<����һ��
	void slot_edit_page_up();	///<ǰ��1/3ҳ
	void slot_edit_page_down();	///<����1/3ҳ
	void slot_edit_show_all();	///<����ͨ����ʾ��һҳ

	// timeline	
	void slot_SG_time_line1(int position);
	void slot_SG_time_line2(int position);
	void slot_SG_time_line3(int position);

	// popup menu
	void slot_wave_menu();
	void slot_caption_menu();

	// caption menu
	void slot_caption_delete();
	void slot_caption_auto_position();
	void slot_caption_auto_scale();
	void slot_caption_harm();
	void slot_caption_harm_wave();
	void slot_caption_property();
	void slot_caption_same_position();
	void slot_caption_same_scale();

	void slot_zoom_actived(int index);
	void slot_wave_zoom(double f);
	void slot_time_out();
	void slot_select(int count, QColor* arr);

	void slot_table_changed(QWidget* w);
	void slot_item_close(int index);
	void slot_item_property(int index);

	void slot_value_dock(bool visible);
	void slot_vector_dock(bool visible);

	void slot_resize_widget(int,int);

private:
		QSpinBox* m_x_edit_zoom;		///<x�����ϵ��spinbox

};

#endif