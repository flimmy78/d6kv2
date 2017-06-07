/**
@file fileoperate.h
@brief comtrade�ļ�����ͷ�ļ�
��������Զ��ftp������װ����comtrade¼���ļ�
@author lujiashun 
@version 1.0.0
@date 2016-11-24
*/
#ifndef FILEOPERATE_H
#define FILEOPERATE_H

#include "comtrade.h"

#include <qstring.h>

class File_operate
{
public:
	File_operate(Comtrade* data) { m_pComtrade = data; }

private:
	QString		m_strFileName;
	Comtrade*	m_pComtrade;

protected:
	/**
	@brief ��ȡcomtrade¼���ļ�����
	*/
	bool	read_comtrade();
	/**
	@brief ��ȡcomtrade¼�������ļ�����
	����filename.cfg�ļ�
	*/
	bool	read_cfg(const QString& cfg);
	/**
	@brief ����ģ����ͨ������
	*/
	void	set_analog(const QString& anaStr, Analog* ana);
	/**
	@brief ���ÿ�����ͨ������
	*/
	void	set_digtal(const QString& digStr, Digtal* dig);
	/**
	@brief ��ȡ������Ϣ
	*/	
	SAMPLE	get_sample_info(const QString& samStr);
	/**
	@brief ����ļ���С�Ƿ���ȷ
	*/	
	bool	check_data_file(int size, int& nRec, int& nStart);
	/**
	@brief ��ȡ¼�������ļ� filename.dat
	��������Ϊascii�ı���
	*/	
	bool	read_ascii(const QString& dat);
	/**
	@brief ��ȡ¼�������ļ� filename.dat
	��������Ϊbinary ������
	*/	
	bool	read_binary(const QString& dat, int& nError);

	/**
	@brief ����¼�������ļ� filename.cfg
	*/	
	bool	save_cfg_file(const QString& cfgFile);
	/**
	@brief ����¼�������ļ� filename.dat
	*/	
	bool	save_dat_file(const QString& datFile);

	short	BigToLitter(ushort* p, int pos)
	{
		ushort ss = p[pos];
		ss = (ss << 8) | (ss >> 8);
		return (short)ss;
	}

public:
	bool	load_file(const QString& fileName, int fileType);
	bool	save_as_comtrade(const QString& fileName);
	/**
	@brief ���������ļ�
	*/	
	bool	load_config(const QString& fileName);
};

#endif
