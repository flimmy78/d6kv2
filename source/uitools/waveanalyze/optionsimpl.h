#ifndef OPTIONSIMPL_H
#define OPTIONSIMPL_H

#include "ui_options.h"

const double AUTOPLAYTIME[] =
	{ 0.3, 0.5, 0.7, 1.0 };

// ��ѡ����б��棬ֻ֧�����߲���
class Dlg_option_impl : public QDialog,public Ui::QDlgOption
{
    Q_OBJECT

public:
    Dlg_option_impl( QWidget* parent = 0, Qt::WindowFlags f = 0 );
    ~Dlg_option_impl();

	int filter();
	void set_filter(int n);

protected slots:
	void slot_harm_button_click(bool checked);

};

/* ȫ������ */
class Propertys
{
public:
	Propertys();
	~Propertys();
	bool Config();

	int Filter()				{ return m_nFilter; }
	int DiffValue()				{ return m_nDiffValue; }
	bool FixedHarm()			{ return m_bFixedHarm; }
	int FixedHarmValue()		{ return m_nFixedHarmValue; }
	int FloatFormatValue()		{ return m_nFloatFormatValue; }
	int AutoPlayTime()			{ return m_nAutoPlayTime; }

private:
	int		m_nFilter;				// �˲���ʽ
	int		m_nDiffValue;			// ��ֵ���
	bool	m_bFixedHarm;			// �̶����г��������־
	int		m_nFixedHarmValue;		// г���������ֵ, ������Ϊ50
	int		m_nFloatFormatValue;	// ��������ʽ����ʽ
	int		m_nAutoPlayTime;		// �α��Զ���ʾʱ����

	friend class Dlg_option_impl;

};

#endif