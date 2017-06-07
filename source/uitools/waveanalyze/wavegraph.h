/**
@file wavegraph.h
@brief ���߻���ͷ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-11-01
*/
#ifndef WAVEGRAPH_H
#define WAVEGRAPH_H

#include <QColor>
#include <QRect> 
#include <QPoint>
#include <qnamespace.h> 
#include <QFont> 
#include <QCursor>
#include <QList> 
#include <QStringList>
#include <QPixmap>  
#include <QEvent> 
#include <QScrollBar>
#include <QScrollArea>
#include <QWidget>
#include <QFrame>
#include <QToolButton>
#include <QToolTip>

#include "viewintf.h"
#include "mouselinerect.h"
#include "drawchannel.h"

const int MAXPOINTS = 100000;		// �����������
const int MAXCHANGES = 100;			// ���������λ����
const int POINTERHEIGHT = 20;		// ָ��߶�
const int MINMOUSEPOINTSMOVE = 5;	// �������ƶ�viewʱ����С���ظ���
const int MINMOUSEZOOM = 20;		// ѡ������ŵ���С������
const int MAXSPLITTERPOS = 100;		// �ָ������λ��
const int MINSPLITTERPOS = 50;		// �ָ�����Сλ��
const int OPERATEHINT = 1;			// ���-������ʾ

class Wave_graph;
class Draw_channel;
class Draw_analog;
class Draw_digtal;
/**
@brief ����ʱ������

*/
class Time_line : public QObject
{
	Q_OBJECT

public:
	Time_line(Wave_graph* owner, QPaintDevice* d);
	~Time_line();

private:
	QColor	m_Color;		// ָ����ɫ
	QCursor m_Cursor;       // ���ָ��, ֻ���𱣴���Щ��Ϣ����Owner��������ʾ
	QString m_strHint;      // ��ʾ��Ϣ
	int		m_nSize;
	int		m_nPosition;    // λ�ã�����β��ֱ
	bool	m_bVisible;     // �Ƿ���ʾ
	bool	m_bShowPointer; // �Ƿ����ͷ��ָ��
	QString	m_strCaption;
	int		m_nTag;

	QPaintDevice* m_Device;

signals:
	void	OnCursor(int positon);	// �ƶ�

protected:
	QRect	m_rctDrawRect;
	Wave_graph* m_pOwner;
	QPainter* m_Paint;

	void	Change();
	void	draw_device(QPaintDevice* dev);
	
public:
	int		hit_test(QPoint pt);
	void	Draw();
	
	QColor	color() { return m_Color; }
	void	set_color(QColor c);
	int		position() { return m_nPosition; }
	void	set_position(int n);
	bool	visible() { return m_bVisible; }
	void	set_visible(bool b);
	bool	show_pointer() { return m_bShowPointer; }
	void	set_show_pointer(bool b);
	int		size() { return m_nSize; }
	void	set_size(int n);
	QString	caption() { return m_strCaption; }
	void	set_caption(QString s);
	QString	hint() { return m_strHint; }
	void	set_hint(const QString& s) { m_strHint = s; }
	int		tag() { return m_nTag; }
	void	set_tag(int n);
	QCursor get_cursor() { return m_Cursor; }
	void	set_cursor(QCursor& c);

};

typedef enum Digtal_Kind{ dkLine = 0, dkRect };
typedef enum Cursor_Action{ caLDrag, caLClick, caDbClick};
typedef enum Mouse_Mode{mmNone, mmCopy, mmCursor, mmHint};


/**
@brief �������߻�����
*/
class Wave_graph : public QFrame
{
	Q_OBJECT
public:
	Wave_graph(QWidget* parent = 0,  Qt::WindowFlags f=0);
	~Wave_graph();

private:
	QColor		m_backgroud_color;	// ����ɫ
	int		m_channel_count;			// ͨ������ ����ģ�����Ϳ�����
	int		m_analog_count;				// ģ����ͨ������
	int		m_digtal_height;				// �������߶�
	int		m_line_width;					// �߿��
	int		m_lock_count;					// ��������
	int		m_show_count;				// ÿҳ��ʾ���߸���
	int		m_data_count;					// �ܵ���
	bool		m_right_move_flag;			// ����Ҽ��ƶ�view��־
	double	m_fzoom_Xdata;				// X�����ű���
	bool		m_lock_line_flag;					//�����α��־
	bool		m_mouse_zoom_flag;			// ʹ�����������Ų�����־
	bool		m_copying_flog;					// ���ڿ�����־
	bool		m_lock_all_lines_flag;			// ��������ʱ���߱�־ 
	int		m_cursor_step;					// �α�����ƶ�����
	bool		m_auto_set_flag;					// ��������ʱ�Զ����ø߶ȱ�־
	double	m_fzoom_Xstep;					// �������ű���
	bool		m_region_select_flag;			// ѡ�������־
	bool		m_auto_set_copy_flag;		// ��������ʱ�Զ����ñ�־
	QString m_str_hint;
	bool		m_bshow_hint;
	
	bool	m_redraw_flag;						// �Ƿ���Ҫ�ػ��־
	uint	m_time_to_draw;						// ms������ÿ�λ�������ʱ��

	Time_line*			m_pdouble_click_line;		// ���˫��ʱȷ��λ�õ�ʱ����
	Draw_channel*	m_selected_channel;			// ��ǰѡ�е�ͨ��
	Mouse_Mode		m_mouse_mode;			// ������ģʽ
	Cursor_Action		m_cursor_move;			// �α��ƶ���ʽ
	Cursor_Action		m_zero_move;				// ����ƶ�
	Digtal_Kind			m_digtal_kind;			// ��������λ��ʽ

	QStringList		m_lstZoom;				//�Ѳ���Ҫ�ı���
	
	QList<Draw_channel*>	m_lst_channel;	// �����б�
	QList<Time_line*>	m_lst_cursor;	// �α��б�
	QList<Time_line*>	m_lst_lock_lines;	// ������ʱ���б�
	 
	QPoint	m_pt_selected_region;				// ѡ������ķ�Χ
	bool	m_bPrinting;						// ��ӡ��
	QPoint	m_pt_print;						// ��ӡ��ʼ�㣬��ִ�����Ų���ʱ�����!	

	QScrollBar*	m_pHbar;
	QScrollBar* m_pVbar;
	QToolButton* m_pbutton;
	QCursor*	m_pleft_hand;
//	QWaveTip*	m_pTip;

signals:
	void signal_move(int n);						// �����ƶ��¼�
	void signal_zoom(double f);					// �����¼�
	void signal_copy();							// ����ͼ���¼�
	void signal_unselect();						// ȡ��ѡ���¼�
	void signal_select(int count, QColor* arr);	// ͨ����ѡ���¼�
	void signal_delete(void* ptr);				// ɾ��ͨ���¼�
	void signal_add_select(void* ptr);			// ����ѡ��ͨ���¼�
	void signal_show(int count, bool* arr);		// ͨ����/���¼�
	void signal_auto_hint(const QString& hint);	// ͨ����ʾ/�����¼�
	void signal_wave_menu();

protected:
	QColor		m_clr_old;							//	
	int			m_last_show_count;			//
	int			m_last_horz_pos;					// �����ϴ�ˮƽ������λ��
	int			m_last_vert_pos;					// �����ϴδ�ֱ������λ��
	QPoint		m_pt_mouse;						// ���λ��
	bool			m_bleft_pressed;					//
	bool			m_bright_pressed;				//
	uint			m_mouse_points_move;		// �ƶ�view��С����
	int			m_lock_delta;						// ��������״̬��ʱ���߼��
	QPixmap	m_pixmap;							// �ڴ�λͼ
	Time_line*		m_cursor_line;				// �α��ߣ���ʱ�̶�Ϊ��һ����ӵ�ʱ����
	Time_line*		m_selected_line;			// ��ǰ��������ʱ����  
	Mouse_line*	m_mouse_line;			// ��Ƥ��

protected:
	QPainter*	get_mem_painter();

	void	load_cursors();
	void	init_data();
	void	move_cursor_line(int x);
	void	move_lock_line();

	bool	locked();
	void	set_locking(bool	updateing);
	void	copy_select_rect();
	void	line_zoom();
	void	on_up_mouse(Wave_graph* p, QRect r);

	virtual void render(QPaintDevice* pd);

	int		footer();						// ʱ���߻��Ƶײ�λ��
	int		max_footer(bool all = true);		// ��Ͳ���λ��
	int		lefter();						// ͼ�λ�����ʼλ��
	int		draw_count();
	int		cursor_range(bool h = false);
	int		analog_height();
	bool		can_zoom_in();
	int		get_default_analog_height();
	void		update_horz(int pos = 0)	;
	virtual void show_channel(int channel, bool show);
	int		channel_position(int channel);
	double	get_scale(int index);
	Mouse_Mode mouse_mode();
	void		set_mouse_mode(Mouse_Mode mm);
	
	inline	int	get_delta();
	bool		move_selected_channels(int delta);
	Time_line* get_select_cursor(QPoint pt);

	inline void update_mouse_mode();

	void	draw_lines();

	/*  ����VertScrollBar������
	   1����������0����������-widget�߶ȣ�
	   2��singleStep: ��λģ�������߸߶ȣ�
	   3��pageStep: widget�߶ȣ�
 	*/
	void	create_scroll_bars();
	void	update_scroll_bars();
	inline	QRect graph_rect();
	int Hbar_height();	// ˮƽ�������߶�
	int Vbar_width();		// ��ֱ���������

protected:
	// mouse
	void	mousePressEvent( QMouseEvent* );
	void	mouseMoveEvent ( QMouseEvent* );
	void	mouseReleaseEvent ( QMouseEvent* );
	void	mouseDoubleClickEvent ( QMouseEvent* );
	void	wheelEvent ( QWheelEvent* );

	// keyboard
	void	keyPressEvent ( QKeyEvent* );

	// draw
	void	paintEvent ( QPaintEvent* );
	void	resizeEvent ( QResizeEvent* );

	// menu
	void contextMenuEvent ( QContextMenuEvent* e );

protected slots:
	// ScrollBar and ToolButton
	void	slot_Hscroll(int);
	void	slot_Vscroll(int);
	void	slot_btn_clicked();

	// elastic����Ƥ��
	void	slot_elastic_move(QRect rct);
	void	slot_elastic_up(QRect rct);
	
public:
	// lock design pattern
	void	lock();
	void	unlock();

	// channel
	int	indexOf(Draw_channel* channel);
	Draw_channel* add(bool b = true);
	void	insert(int index, Draw_channel* channel);
	void	delete_channel(int index);
	void	Clear();
	void	notify(void* ptr, ListNotify action);

	// lines
	Time_line*	add_cursor(bool key = false);	// ����α�
	void	delete_cursor(int index);			// ɾ���α�
	int		cursor_index(Time_line* line);
	int		line_count();
	void	add_lock_line(Time_line* line);		// ��ӱ������α�
	void	delete_lock_line(Time_line* line);	// ɾ���������α�
	void	lock_lines();						// �����α�
	void	set_line_pos(Time_line* line, int x);	// �����α�λ��
	int	position_to_dot(int pos);				// ����λ��ת��Ϊ����
	void	update_line_pos(Time_line* line);

	// draw and arrange
	void	set_client_rect(QRect r);
	virtual void render();
	void	print(QPainter* p, int left, int ncount);
	void	alloc_rect();
	void	realloc_rect();
	void	auto_set_rect();
	virtual void auto_position(int channel);
	virtual void auto_position(Draw_channel* channel);
	void	render_caption(bool full = true);
	void	add_select_channel(Draw_channel* channel);
	bool	is_selected(int index);
	void	do_selected(int index, bool b);
	virtual bool locate_channel(int index);
	virtual bool locate_channel(Draw_channel* channel);
	void	select_channel();
	virtual void show_channel();
	void	select_region(QPainter* p);
	void	do_select_region(QPoint pt);

	int	wave_height();
	void	set_item_color(QColor c, int index = 1);	// ����Ⱥ��ɫ
	void	set_selected_item_scale(double y);
	void	scrollY(int delta);						// ���߻���λ��ƫ��
	void	clear_intf();							// ��ն����ݽӿ�ָ�������
	void	update_vert(bool all = true, int h = 0);
    void	zoom_config();							// �������������б�

	// operating
	void	fill_in();
	void	expand();
	void	cursor_left(int delta = 1);
	void	cursor_right(int delta = 1);
	void	cursor_home();
	void	cursor_end();
	void	cursor_up();
	void	cursor_down();
	void	page_up();
	void	page_down();
	void	page_home();
	void	page_end();

	void	move_channel(Draw_channel* channel, int pos);
	bool	channel_move(Draw_channel* channel, int delta, bool bLock = false);
	void	unselect_all();
	void	show_all(bool b = true);
	void	copy_graph(bool b = true);
//	void	EraseMemLine();

	// tooltip
	QRect	tip(const QPoint& pos, Time_line* line);

	// propertys
	Draw_channel*	channels(int index);
//	void	SetChannel(QDrawChannel* channel);
	Time_line*	cursors(int index);
//	void	SetCursors(QTimeLine* line);
	Time_line*	lock_cursor(int index);
	Draw_channel*	 selected_channel();
	void	set_selected_channel(Draw_channel* channel);
	bool	checked(int index);
	void	set_checked(int index, bool b);
	int		count();
	int		analog_count();
	int		cursor_count();
	int		lock_cursor_count();
	int		digtal_height();
	void		set_digtal_height(int n);
	int		line_width();
	void		set_line_width(int n);
	double		zoom_Xdata();
	void		set_zoom_Xdata(double f);
	bool		mouse_zoom();
	void		set_mouse_zoom(bool b);
	Digtal_Kind	digtal_kind();
	void		set_digtal_kind(Digtal_Kind dk);
	bool		lock_line();
	QStringList* zoom_list();
	bool		copying();
	void		set_copying(bool b);
	int		default_analog_height();
	bool		lock_all_lines();
	void		set_lock_all_lines(bool b);
	Time_line* cursor_line();
	void		set_cursor_line(Time_line* line);
	Time_line* double_click_line();
	void	set_double_click_line(Time_line* line);
	int		cursor_step();
	void		set_cursor_step(int n);
	double	zoom_Xstep();
	void	set_zoom_Xstep(double f);
	bool	auto_set();
	void	set_auto_set(bool b);
	bool	region_select();
	void	set_region_select(bool b);
	double	min_selected_scale();
	double	max_selected_scale();
	double	min_selected_scale1();
	double	max_selected_scale1();
	bool	auto_set_copy();
	void	set_auto_set_copy(bool b);
	bool	printing();
	void	set_printing(bool b);
	QPoint	print_point();
	void	set_print_point(QPoint pt);
	QPoint	selected_region();
	QColor	get_color();
	void	set_color(QColor c);
	bool	show_hint();
	void	set_show_hint(bool b);
    
	int		show_count();
	void		set_show_count(int n);
	int		data_count();
	void		set_data_count(int n);

	friend class Draw_channel;
	friend class Time_line;	

};

#endif