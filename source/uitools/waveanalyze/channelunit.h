/**
@file channelunit.h
@brief ͨ����ͷ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-10-25
*/
#ifndef CHANNELUNIT_H
#define CHANNELUNIT_H

#include <QList>

#include "comtrade.h"
/**
@brief ȡ��ͨ����
��װ���������DataObject�л��ͨ����Ϣ
*/
class Channels
{
public:
	Channels();
	~Channels();
	/**
	@brief ��������б�
	*/
	void clear();
	/**
	@brief �����б�����
	*/
	int count();
	/**
	@brief ѡ�������б�����
	*/
	int check_count();
	/**
	@brief ��ȡ���Ϊindex������
	*/
	View_data* channel(int index);
	/**
	@brief ��ȡ���Ϊindex�������Ƿ�ѡ��
	*/
	bool checked(int index);
	/**
	@brief �������Ϊindex�������Ƿ�ѡ��
	@para in bCheck  true:checked  false:unchecked
	*/
	void set_checked(int index, bool bCheck);
	/**
	@brief ȫѡ
	*/
	void check_all(bool bCheck = false);
	/**
	@brief ����һ��ͨ��
	*/
	void add(Channel* obj);
	/**
	@brief ����һ��ͨ��
	*/
	void insert(int index, Channel* obj);
	/**
	@brief ɾ��ͨ��
	*/
	void Delete(int index);
	/**
	@brief ���ָ��ͨ���ŵ�ͨ��λ��
	*/
	int Indexof(int channelNo);
	/**
	@brief ��ʼ��
	*/
	void init_by_comtrade(Comtrade* obj, bool ana);
	
protected:
	/**
	@brief ���CheckList���Ƿ�����Ѿ���ɾ����ͨ��
	*/
	void reset_check_list();

private:
	 QList<View_data*> m_lst_data;	///<ȫ�������б�
	 QList<View_data*> m_lst_check;	///<ѡ�������б�
	
};

#endif