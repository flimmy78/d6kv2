/**
@file fesdatashm_accessor.h
@brief ǰ�������ݹ����ڴ����ͷ�ļ�

�ṩǰ�������ݹ����ڴ���������

@author zhoujian 
@version 1.0.0
@date 2013-11-18
*/

#include "fes/fesdatashm.h"

/**
@brief ǰ�������ݹ����ڴ������
�ṩ��ǰ�������ݹ����ڴ�ķ���

*/
class FES_DATA_EXPORT Fes_data_shm_accessor : public Fes_data_shm
{
public:
	/**
	@brief ��ȡʵ������ʵ������
	@return Fes_data_shm_accessorʵ��
	*/
	static Fes_data_shm_accessor& get();

	/**
	@brief ���������ݵ�ַ
	*/
	FE_DATA* fe_data() const {return m_fe_data;};


private:
	/**
	@brief ���캯������ʼ����Ա��������ʽ����
	*/
	Fes_data_shm_accessor();
	/**
	@brief ֻ��������ʵ��
	��������������л����Ĭ�Ͽ������캯�������ֽڿ����������ܻ��𻵵���
	�е����ݣ���ֻ������ʵ�ֵķ�������Ĭ�Ͽ������캯���������ʹ���е���
	�������캯����������������
	*/
	Fes_data_shm_accessor(const Fes_data_shm_accessor&);
	/**
	@brief ֻ��������ʵ��
	��������������л����Ĭ�ϸ�ֵ����������ֽڸ�ֵ�������ܻ��𻵵���
	�е����ݣ���ֻ������ʵ�ֵķ�������Ĭ�Ͽ������캯���������ʹ���е���
	�������캯����������������
	*/
	Fes_data_shm_accessor& operator = (const Fes_data_shm_accessor&);

};
