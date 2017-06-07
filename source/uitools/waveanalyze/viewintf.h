/**
@file viewintf.h
@brief ���ݽӿ���ͷ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-10-25
*/

#ifndef VIEWINTF_H
#define VIEWINTF_H

#include <QString>
#include <QColor>

const QString CHANNELCLASS[] = {"Channel", "Analog", "Digtal"};


typedef	enum ListNotify{lnAdded, lnDeleted, lnExtracted};

/** 
@brief ��������λ�ṹ
	�м���״̬���м���ֵ��֮ǰ���б�λ����ֵ
	Ϊ���ڻ��ƿ���������ʱ������ƽ�βλ��
*/
typedef struct tagStatus 
{
	int position;		// ��λλ��
	bool change;		// ״̬��True-��, False-��
	int endPosition;	//��λ����λ��
} STATUS;

typedef struct tagSample
{
	int count;
	int begin;
	int end;
	double frequency;
} SAMPLE;

/**
@brief ͨ����ӿ���
*/
class View_data
{
public:
	// ���ģ��������
	virtual double* get_analog_data(int minPos, int maxPos, int& count) = 0;
	// ��ÿ���������
	virtual STATUS* get_digtal_data(int& initData, int& count) = 0;
	// ���ͨ������
	virtual QString channel_name() = 0;
	// ���ͨ��������Ψһ��
	virtual int channel_no() = 0;
	// ���ͨ�����
	virtual int channel_phase() = 0;
	// ���ͨ����λ����
	virtual int switch_num() = 0;
	// ������б�λ��Ϣ
	virtual int get_all_switch(STATUS* pSta) = 0;
	// ���ͨ��ĳ���ֵ
	virtual double Value(int idx) = 0;
	// ���ͨ����ɫ
	virtual QColor get_color() = 0;
	// ���ͨ��г������
	virtual int harm_value() = 0;
	// ��ȡͨ����λ
	virtual QString get_channel_unit() = 0;
};


/**
@brief ���ݻ�ȡ�ӿ���
*/
class Gain_data
{
public:
	// �����ļ�����
	virtual QString fault_file() = 0;
	// �ļ�����
	virtual QString file_fype() = 0;
	// ͨ������
	virtual int Count(int n = 2) = 0;
	// ͨ�����ͣ�ģ������
	virtual uint channel_type(int n) = 0;
	// ͨ��г��������ģ������
	virtual uint channel_harm_value(int n) = 0;
	// ͨ����λ��ģ������
	virtual uint channel_phase(int n) = 0;
	virtual QString start_time() = 0;
	virtual QString trigger_time() = 0;
	virtual int sys_frequency() = 0;

	// ʱ���Ƿ���ȷ
	virtual bool pos_is_correct(double timeData) = 0;
	virtual	int pos(double timeData) = 0;
	virtual int data_count() = 0;
	// posλ�ô����ܲ�����
	virtual int cycle_count(int pos) = 0;
	
	// ���һ���ܲ����ݣ���ʱ�����
	virtual double* get_cycle_data(int n, int pos) = 0;
	virtual double* get_analog_data(int n, int pos, int count) = 0;
	virtual STATUS* get_digtal_data(int& initData, int& count, int n) = 0;
	// ���ͨ����posλ�ÿ�ʼ������ָ��
	virtual double* get_data(int n, int pos) = 0;

	virtual int sample_count() = 0;
	virtual int get_samples(int ns, int ne, SAMPLE* sam) = 0;
	// ��������Ĳ�����Ϣ
	virtual void get_full_sample(SAMPLE* sam) = 0;
	// ���Posλ�õ�ʱ��
	virtual double get_sample_time(int pos) = 0;
	// ����������ܲ���
	virtual int max_cycle_count() = 0;
	// �������λ��
	virtual int max_switch_num() = 0;
	// �����Ϣ�ַ���
	virtual QString get_info_str(int n) = 0;
	// ��ù���ʱ��λ��
	virtual int get_fault_pos() = 0;
	// ���ָ��λ�õĲ���ʱ������ָ��
	virtual double* get_pos_sample(int pos) = 0;
	
};

// ��ǰ���ݶ���
Gain_data* DataObject();

#endif