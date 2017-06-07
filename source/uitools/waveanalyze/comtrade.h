/**
@file comtrade.h
@brief comtrade�ļ����������ͷ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-10-25
*/
#ifndef COMTRADE_H
#define COMTRADE_H

#include <QList>
#include <QPoint>

#include "viewintf.h"

class Comtrade;

/**
@brief ͨ�������
*/
class Channel : public View_data
{
public:
	Channel(Comtrade* owner);
	// ע����������QChannel��������Ϊ�麯����c++��׼�涨
	virtual ~Channel();

private:
	QString		m_strClassName;		// ����
	Comtrade*	m_pOwner;			// ������
	double*		m_pData;			// ����ָ��
	uint		m_nChannelNo;		// ͨ����
	QString		m_strChannelName;	// ͨ������
	QString		m_strDevice;		// �����ӻ�·Ԫ��
	QString		m_strPhase;			// ��λ����

public:
	Channel &operator=(const Channel &);
	void	copy_data(Channel* src, QPoint range);
	void	set_size(int size);
	virtual	int	switch_num();
	QString	channel_name();
	int		channel_no();
	void	SetChannelNo(int n);
	int		channel_phase();
	double*	get_data();
	QString	get_device();
	QString	Phase();
	
	// Interface
	double* get_analog_data(int minPos, int maxPos, int& count);
	STATUS* get_digtal_data(int& initData, int& count);
	int		get_all_switch(STATUS* pSta);
	virtual double Value(int idx) { return 0.0; }
	virtual QColor get_color() { return Qt::white; }
	virtual int harm_value() { return 0; }
	virtual QString get_channel_unit() {return "";}

	friend class Analog;
	friend class Digtal;
	friend class Comtrade;
	friend class File_operate;

};
/**
@brief ģ����ͨ����
�̳���ͨ������
*/
class Analog : public Channel
{
public:
	Analog(Comtrade* owner);
	~Analog();

private:
	QString		m_strDataUnit;		// ͨ����λ
	double		m_fScale;			// ͨ������
	double		m_fOffset;			// ͨ��ƫ�Ƽ���
	double		m_fSkew;			// �ӳ������ڿ�ʼ���ͨ��ʱ��(us)
	double		m_fMinValue;		// ��С����ֵ
	double		m_fMaxValue;		// �������ֵ
	double		m_fPrimary;			// һ�α��
	double		m_fSecodary;		// ���α��
	QString		m_strPs;			// ��ֵ������
	uint		m_nHarmValue;		// г������ 0-ֱ�� 1-���� 2-����г��

public:
	Analog &operator=(const Analog &);
	uint	AnaType();
	double	Value(int n);
	double	MaxValue();
	QColor	GetColor();
	int		HarmValue()					{ return m_nHarmValue; }

	double* get_analog_data(int minPos, int maxPos, int& count);
	//��ȡͨ����λ
	QString get_channel_unit() {return m_strDataUnit;}

	friend class File_operate;

};
/**
@brief ������ͨ����
�̳���ͨ������
*/
class Digtal : public Channel
{
public:
	Digtal(Comtrade* owner);
	~Digtal();

private:
	uint		m_nSwitchNum;			// Ծ�����
	uint		m_nDefaultState;		// ��ʼ״̬
	STATUS		m_sChangeArray[255];	// ��λ����

public:
	Digtal &operator=(const Digtal &);
	double		Value(int n);
	int		get_all_switch(STATUS* pSta);
	STATUS* get_digtal_data(int& initData, int& count);

	void	Filtrate();
	int		switch_num()					{ return m_nSwitchNum; }

	friend class File_operate;
	
};


/**
@brief comtrade������
*/
class Comtrade : public Gain_data
{
public:
	Comtrade();
	~Comtrade();

private:
	uint				m_nDataCount;		// �����ܵ���
	QString		m_strStartTime;		// ������ʼʱ��
	QString		m_strTriggerTime;	// ���ϴ���ʱ��
	double*		m_pSampleTime;		// ����ʱ��ָ��,������PDWORD 
	QString		m_strFaultFile;		// �����ļ���
	QString		m_strStation;		// ���վ����
	QString		m_strRecorder;		// �豸���
	QString		m_strRevYear;		// COMTRADE�ļ��汾����Ϊ��׼�޸����
	uint		m_nChannelNum;		// ͨ������
	uint		m_nAnaChannelNum;	// ģ����ͨ����
	uint		m_nDigChannelNum;	// ������ͨ����
	double		m_fSysFrequency;	// ϵͳƵ��
	uint		m_nFrequencyNum;	// ����Ƶ����Ŀ
	QString		m_strFileType;		// �ļ����� BINARY-ASCII
	QString		m_strTimeMult;		// ʱ���ǳ���ϵ��
	int			m_nFaultFileType;	//

	QList<Analog*> m_pAnaChannels;
	QList<Digtal*> m_pDigChannels;
	SAMPLE*		m_pSampleInfo;		// ������Ϣ�ṹ����

	bool		m_bResetTime;		// ��֯������Ϣ��־

protected:
	double	GetTime(int n);
	SAMPLE	GetSampleByPos(int pos);
	int		GetSampleIndexByPos(int pos);
	int		MaxAnalogNo();
	int		MaxDigtalNo();

	Channel* AddItem(bool b = true);

public:
	void	Clear();
	void	Delete(int n);
	void	DeleteRef(Channel* chl);
	
	Channel* Add(bool b = true);
	Channel* Insert(int n, bool b = true);
	Channel* channel(int n);
	Channel* ChannelByNo(int n, bool b = true);

	QList<Analog*> AnaChannels()	{ return m_pAnaChannels; }
	QList<Digtal*> DigChannels()	{ return m_pDigChannels; }

	int		CalCount();
	void	SetSampleTime(double* p);
	void	AddSample(SAMPLE sam);

	Analog* analog(int n);
	Digtal* digtal(int n);
	int		IndexOf(Channel* channel);
	bool	CanAddData(Comtrade& src);
	void	Notify(void* ptr, ListNotify action);

	void	ResetSample(bool b = false);
	void	FiltrateDigtals();
	void	SetFaultFile(QString s)		{ m_strFaultFile = s; }

	int		ChannelNum()				{ return m_nChannelNum; }
	int		AnaChannelNum()				{ return m_nAnaChannelNum; }
	int		DigChannelNum()				{ return m_nDigChannelNum; }

	// Interface(QGainData)
	QString fault_file()					{ return m_strFaultFile; }
	// �ļ�����
	QString file_fype();
	// ͨ������
	int Count(int n = 2);
	// ͨ�����ͣ�ģ������
	uint channel_type(int n);
	// ͨ��г��������ģ������
	uint channel_harm_value(int n);
	// ͨ����λ��ģ������
	uint channel_phase(int n);
	QString start_time()					{ return m_strStartTime; }
	QString trigger_time()				{ return m_strTriggerTime; }
	int sys_frequency()					{ return m_fSysFrequency; }
	// ʱ���Ƿ���ȷ
	bool pos_is_correct(double timeData);
	int pos(double timeData);
	int data_count() { return m_nDataCount; } 
	// posλ�ô����ܲ�����
	int cycle_count(int pos);	
	// ���һ���ܲ����ݣ���ʱ�����
	double* get_cycle_data(int n, int pos);
	double* get_analog_data(int n, int pos, int count);
	STATUS* get_digtal_data(int& initData, int& count, int n);
	// ���ͨ����posλ�ÿ�ʼ������ָ��
	double* get_data(int n, int pos);
	int sample_count()					{ return m_nFrequencyNum; }
	int get_samples(int ns, int ne, SAMPLE* sam);
	// ��������Ĳ�����Ϣ
	void get_full_sample(SAMPLE* sam);
	// ���Posλ�õ�ʱ��
	virtual double get_sample_time(int pos);
	virtual double get_sample_time(int pos, int zero);
	// ����������ܲ���
	int max_cycle_count();
	// �������λ��
	int max_switch_num();
	// �����Ϣ�ַ���
	QString get_info_str(int n);
	// ��ù���ʱ��λ��
	int get_fault_pos();
	// ���ָ��λ�õĲ���ʱ������ָ��
	double* get_pos_sample(int pos);
	// ��ȡͨ����λ
	QString get_channel_unit(int index);

	// propertys
	void	SetDataCount(int n);
	void	SetStartTime(QString s);
	void	SetTriggerTime(QString s);
	double	SampleTime(int n);
	double* PSampleTime()				{ return m_pSampleTime; }

	// �Ƿ����
	bool	IsFault();

	friend class Analog;
	friend class Digtal;
	friend class File_operate;

};

#endif