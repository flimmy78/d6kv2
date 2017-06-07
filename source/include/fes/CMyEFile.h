/**
@file CMyEFile.h
@brief E���Զ�дͷ�ļ�
Ŀǰ��ʹ�ú��ʽд��
@author zhoujian 
@version 1.0.0
@date 2014-09-17
*/
#ifndef PLATFORM_E_LANG_FILE_H
#define PLATFORM_E_LANG_FILE_H 

#ifdef WIN32
#ifdef PLATFORM_E_LANG_FILE_DLL
#define	PLATFORM_E_LANG_FILE_EXPORT __declspec(dllexport) 
#else
#define	PLATFORM_E_LANG_FILE_EXPORT __declspec(dllimport) 
#endif
#else
#define	PLATFORM_E_LANG_FILE_EXPORT
#endif

#include "fes/fertfile.h"
#include "fes/fesscdshm.h"
#include "fes/femailstruct.h"

#include <stdlib.h>

#define		CLASS_NAME_LEN	40
#define		ENTITY_NAME_LEN	64
#define		DATA_LEN		512


#define		CORSS_TABLE		1
#define		SINGLE_COL		2
#define		MULTI_COL		3

#include "stdio.h"
#include <string.h>

class PLATFORM_E_LANG_FILE_EXPORT EFILE_BaseData
{
public:
	/**
	@brief ���캯��
	@param in int input_num Ԫ�ظ���
	*/
	EFILE_BaseData( int input_num );
	~EFILE_BaseData( );

	/**
	@brief �����ݴ����ļ�
	@param in unsigned char save_type �洢��ʽ
	*/
	virtual void save_data_to_file(unsigned char save_type);

	/**
	@brief ��������������Ը���
	@param in int num ���Ը���ID
	*/
// 	void	set_property_num(int num){ m_property_num = num };

	/**
	@brief ���ø����ݴ洢���ļ���
	@param in char* file_name	�ļ���
	*/
	void	set_file_name(char* file_name);

	/**
	@brief ���ݿ鿪ʼ���
	*/
	void entity_begin();

	/**
	@brief ���ݿ�������
	*/
	void	entity_end();

	/**
	@brief ����ע��
	@param in const char* pNotes ע������
	*/
	void add_notes(const char* pNotes);

	/**
	@brief ����ϵͳ����
	@param in const char* systemname Ӧ��ϵͳ����
	@param in float version E���԰汾
	@param in const char* code �ַ���
	@param in float data_version ���ݰ汾
	*/
	void set_system_declare(const char* systemname, const char* version, const char* code, const char* data_version);

	/**
	@brief ���ݿ���ʽ�洢
	*/
	virtual void save_data_cross_table( );

	/**
	@brief ���ݿ鵥��ʽ�洢
	*/
	virtual	void save_data_single_col( );

	/**
	@brief ����ʽ�洢
	*/
	virtual void save_data_multi_col( );

	void	convert_FETIME_systime(FETIME*, int& procs);

	void	convert_systime_FETIME(FETIME* fetime, int& procs);

protected:
	int		m_property_num;
	char	class_name[CLASS_NAME_LEN];
	char	entity_name[ENTITY_NAME_LEN];
	int		data_group[512];

	int		m_entity_num;

	FertFile* m_pfertfile;		//�ļ���дָ��
};


class PLATFORM_E_LANG_FILE_EXPORT EFILE_DATA : public EFILE_BaseData
{
public:
	EFILE_DATA(int input_num);
	~EFILE_DATA(void);

	/**
	@brief ���ݿ���ʽ�洢
	*/
	virtual void save_data_cross_table( FE_REAL_DATA_NODE* );

	/**
	@brief ���ݿ鵥��ʽ�洢
	*/
	virtual	void save_data_single_col( );

	/**
	@brief ����ʽ�洢
	*/
	virtual void save_data_multi_col( );

	virtual int read_data_cross_table(	FE_REAL_DATA_NODE* data_point );


};

class PLATFORM_E_LANG_FILE_EXPORT EFILE_SOE : public EFILE_BaseData
{
public:
	EFILE_SOE(int input_num);
	~EFILE_SOE(void);

	/**
	@brief ���ݿ���ʽ�洢
	*/
	virtual void save_data_cross_table( FE_DATA_EVENT_NODE* );

	virtual int read_data_cross_table(	FE_DATA_EVENT_NODE* data_point );

};

class PLATFORM_E_LANG_FILE_EXPORT EFILE_CHANNEL : public EFILE_BaseData
{
public:
	EFILE_CHANNEL(int input_num);
	~EFILE_CHANNEL(void);

	/**
	@brief ���ݿ���ʽ�洢
	*/
	virtual void save_data_cross_table( FE_CHANNEL_STAT_MAIL* );

	virtual int read_data_cross_table(	FE_CHANNEL_STAT_MAIL* data_point );

	virtual void save_event_cross_table( float channel_event_val, char* desdribe, char*  namestr);

	virtual void read_event_cross_table( float& channel_event_val, char* desdribe, char*  namestr);

};


class PLATFORM_E_LANG_FILE_EXPORT EFILE_YKCHHECK : public EFILE_BaseData
{
public:
	EFILE_YKCHHECK(int input_num);
	~EFILE_YKCHHECK(void);

	/**
	@brief ���ݿ���ʽ�洢
	*/
	virtual void save_data_cross_table( FE_YK_CHECK_NODE* );

	virtual int read_data_cross_table(	FE_YK_CHECK_NODE* data_point );

};

#endif