/**
@file drawchannnel.h
@brief ����ͨ��ͷ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-11-08
*/
#ifndef DRAWCHANNEL_H
#define DRAWCHANNEL_H

#include <QFont>
#include <QRect>
#include <QFontMetrics>

#include "viewintf.h"
#include "wavegraph.h"
#include "comtrade.h"

const int AMARGIN = 2;				// ��Ե
// const int DIGTALHEIGHT = 15;		// ������Ĭ�ϸ߶�	 
const int DIGTALHEIGHT = 40;		// ������Ĭ�ϸ߶�	 
const QString DRAWCLASS[] = {"QDrawChannel", "QDrawAnalog", "QDrawDigtal"};

class Wave_graph;

/**
@brief ����ͨ�����߻���

*/
class Draw_channel
{
public:
	Draw_channel(Wave_graph* owner);
	virtual ~Draw_channel();

private:
	View_data*		m_pdata;				///< �������ݽӿ�
	Comtrade*	m_pcomtrade;
	bool		m_bselected;					///< ��ѡ��
	int		m_left_margin;				///< ���Ե
	bool		m_bshow_border;			///< ��ʾ�߿�
	bool		m_bshow_caption;			///< ��ʾ����
	bool		m_bvisible;						///< �Ƿ���ʾ
	QString	m_caption;					///< ����
	QFont		m_font;						///< ��������
	bool			m_bshow_pointer;		///< ��ʾָʾ��, ָ��Ӧ�ÿ����м��ֻ��Ʒ�����ָ����ǰ��״̬
	QColor		m_color;						///< ��ɫ
	QRect		m_draw_rect;				///< ��������
	double		m_max_value;				///< ���ֵ
	int			m_position;					///< λ�ã����ݻ����������
	QString	m_str_hint;					///<
	bool			m_bauto_size;				///< �Զ��������������־

	QString	m_str_class_name;	///< ����

protected:
	Wave_graph* m_owner_ptr;
	QRect		m_caption_rect;

	virtual void change();
	virtual void rect_update() = 0;
	virtual QRect get_select_rect(int rightPos) = 0;
	/**
	@brief ����ֱ�߷�װ����
	@para in  move  moveTo/lineTo     true:moveTo  false:lineTo
	*/
	void	draw_line(QPainter* p, QPainterPath &pa,QPointF pt, bool print, bool move);
	/**
	@brief �ı����ͷrect�Ĵ�С
	*/
	void	change_caption_rect();
	/**
	@brief ����¼�����߻�������
	@para in  xref  x�����ϵ��  
	@para in  lefter  x�����ϵ��  
	*/
	virtual void draw_wave(QPainter* p, double xref, int lefter, int drawCount, bool print = FALSE) = 0;

public:
	/**
	@brief ���Ʊ���ͷ����
	@para in p ����
	@para in  rightPos �����������Ҷ�
	@para in  index ���Ƶ�ͨ����
	@para in  currentPos ��ǰ�������λ��x������
	*/
	void   draw_caption(QPainter* p, int rightPos,int index,int currentPos=0);
	/**
	@brief ���Ʊ���ͷ�Ҳ��ͷ
	@para in p ����
	@para in  rightPos �����������Ҷ�
	*/
	void   draw_pointer(QPainter* p, int rightPos);
	/**
	@brief �ж�pt�Ƿ��ڱ���ͷ���ο��ڲ�
	*/
	int    hit_test(QPoint pt);
	QPoint caption_size();
	void   copy_font(QFont& ft);
	void   set_max_value(double f);
	void   set_view_data(View_data* data, double maxData = 0);
	/**
	@brief ��ȡ����
	*/
	QString class_name();
	/**
	@brief ����comtrade����
	*/
	void   set_comtrade_data(Comtrade* c);		///<����comtrade

public:
	int		position();
	void	set_position(int n );
	QColor	color(); 
	void	set_color(QColor c);
	bool	show_pointer();
	void	set_show_pointer(bool b);
	bool	visible();
	void	set_visible(bool b);
	QString caption();
	void	set_caption(const QString& s);
	QRect	caption_rect();
	QFont&	font();
	void	set_font(QFont& f);
	bool	show_border();
	void	set_show_border(bool b);
	bool	show_caption();
	void	set_show_caption(bool b);
	int		left_margin();
	void	set_left_margin(int n);
	QRect	draw_rect();
	void	set_draw_rect(QRect& r);
	bool	selected();
	void    set_selected(bool b);
	QString hint();
	void	set_hint(QString s);
	bool	auto_size();
	void	set_auto_size(bool b);

	friend class Draw_analog;
	friend class Draw_digtal;
	friend class Wave_graph;
	
};

/**
@brief ����ģ����������
�̳���Draw_channel
*/
class Draw_analog : public Draw_channel
{
public:
	Draw_analog(Wave_graph* owner);
	~Draw_analog() {}

private:
	double	m_fYref;		///< �������

protected:
	double	m_fmin_Yref;		///< ������С����
	double	m_fmax_Yref;		///< ����������
	virtual void rect_update();
	virtual QRect get_select_rect(int rightPos);
	/**
	@brief �Զ����ݷ���߶Ⱥ����ֵ��������������������ñ�������ֵ 
	*/
	void	auto_set_Yref();

public:
	/**
	@brief ��������
	@para in xref x�����ϵ��
	@para in lefter x��ƫ����
	@para in drawCount �������ߵ����ݵ���
	*/
	virtual void draw_wave(QPainter* p, double xref, int lefter, 
		int drawCount, bool print = FALSE);
	/**
	@brief ��ȡY�����
	*/
	double  Yref();
	/**
	@brief ����Y�����
	*/
	void	set_Yref(double f);
	/**
	@brief ��ȡY����С����
	*/
	double	min_Yref();
	/**
	@brief ��ȡY��������
	*/
	double	max_Yref();
	/**
	@brief ���� �Զ��������������־
	*/
	void	set_auto_size(bool b);
	/**
	@brief ��ȡ�߿�
	*/
	int get_margin(double* pp, double& fmax, double& fmin, 
		int& counter, int nunit, int count);
};


/**
@brief ���ƿ�����������
�̳���Draw_channel
*/
class Draw_digtal : public Draw_channel
{
public:
	Draw_digtal(Wave_graph* owner);
	~Draw_digtal() {}

private:
	int		m_nheight;			///< ����״̬��1ʱ�ĸ߶�
	bool		m_is_rect_flag;		///< ���Ʊ�λ���α�־

protected:
	virtual void rect_update();
	virtual QRect get_select_rect(int rightPos);
	QRectF	 print_rect(QRectF r, bool print);
	/**
	@brief ����pos���ȡ�õ��״̬�����
	������ȡһ�����������ε���ʼ���״̬�Ϳ��ر�λ�����
	�Լ��յ��״̬�����
	@para in pos Ϊ��ǰ���ĵ��
	@para in nInit ��ʼ״̬ 0 or 1
	@para in sta ��ǰ��������״̬��Ϣ
	@para in count ״̬������
	@para in state ��ȡ�ĵ�ǰpos���״ֵ̬ 0 or 1
	@para in index ��ȡ�ĵ�ǰ״ֵ̬��sts�е����
	*/
	void switch_info(int pos, int nInit, const STATUS* sta, 
		int count,int& state, int& index);

public:
	/**
	@brief ���ƿ�����������
	*/
	virtual void draw_wave(QPainter* p, double xref, int lefter, 
		int drawCount, bool print = FALSE);
	/**
	@brief ��ȡ����1�ĸ߶�
	*/
	int		height();
	/**
	@brief ��ȡ�Ƿ���ƾ���
	*/
	bool		is_rect();
	/**
	@brief �����Ƿ���ƾ���
	*/
	void	set_is_rect(bool b);
};



#endif