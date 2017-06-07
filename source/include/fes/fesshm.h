/**
@file fesshm.h
@brief ǰ�ù����ڴ�ͷ�ļ�

�ṩǰ�ù����ڴ洴�������ʽӿ�

@author zhangyun 
@version 1.0.0
@date 2012-10-22
*/
#ifndef FES_SHM_H_
#define FES_SHM_H_

#ifdef WIN32
#ifdef FESSHM_DLL
#define	FESSHM_EXPORT __declspec(dllexport) 
#else
#define	FESSHM_EXPORT __declspec(dllimport) 
#endif
#else
#define	FESSHM_EXPORT
#endif

#include "fes/fesshare.h"

/**
@brief ǰ�ù����ڴ���ʷ�����

�ṩ��ǰ�ù����ڴ浥�����ʵĽӿڣ�������Fes_shm::get().����
��Ա ���ʹ����ڴ�����ṹ
*/
class FESSHM_EXPORT Fes_shm
{
	enum 
	{
		SHM_ALIGN = 16,			///<��16�ֽڶ���
		FES_SHM_KEY = 5555666,	///<�����ڴ�key
		FES_SHM_LEN = 0x1000000	///<�����ڴ泤��
	};

public:
	/**
	@brief ��ȡʵ��
	@return Fes_shmʵ��

	@verbatim
	Fes_shm::get().fe_sys_data_ptr;
	Fes_shm::get().chan_para_ptr;
	@endverbatim
	*/
	static Fes_shm& get();

	/**
	@brief ֵ����л�

	������old_hostֵ���ͨ�����new_hostֵ��
	@param in const int& defalut_host Ĭ��ֵ���
	@param in const int& new_host ��ֵ���
	*/
	void attend_host_switch(const int& old_host, const int& new_host);

	/**
	@brief ����ֵ��Ӧ���ɱ���ֵ���ͨ��
	@param in const int& host_no �ڵ��
	*/
	void attend_self(const int& host_no);

private:
	/**
	@brief ���캯������ʼ����Ա��������ʽ����
	*/
	Fes_shm();

	/**
	@brief �������캯����ֻ������ʵ��
	
	��������������л����Ĭ�Ͽ������캯�������ֽڿ����������ܻ��𻵵���
	�е����ݣ���ֻ������ʵ�ֵķ�������Ĭ�Ͽ������캯���������ʹ���е���
	�������캯����������������
	*/
	Fes_shm(const Fes_shm&);

	/**
	@brief �����������ֻ������ʵ��
	
	��������������л����Ĭ�ϸ�ֵ����������ֽڸ�ֵ�������ܻ��𻵵����е����ݣ�
	��ֻ������ʵ�ֵķ�������Ĭ�ϸ�ֵ������������ʹ���е����˸�ֵ����������
	���������
	*/
	Fes_shm& operator = (const Fes_shm&);

	/**
	@brief ����ǰ�ù����ڴ�
	
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	void create_fes_shm(void);

	//������ʵ�ʵ�����
public:
	char*			shm_addr_ptr;		///<�����ڴ�ָ��
	FE_SYS_DATA*	fe_sys_data_ptr;	///<����������ǰ�ýڵ����Ϣ
	CHANPARA*		chan_para_ptr;		///<ͨ������
	RTUPARA*		rtu_para_ptr;		///<�ն˲���
	CHANDATA*		chan_data_ptr;		///<ͨ������ 
	GPS_INFO*		gps_info_ptr;		///<GPS����
};

#endif
