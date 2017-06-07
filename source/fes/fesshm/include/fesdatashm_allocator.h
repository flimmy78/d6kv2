/**
@file fesdatashm_allocator.h
@brief ǰ�������ݹ����ڴ� ����ͷ�ļ�

�ṩǰ�������ݹ����ڴ�����롢��ʼ������������

@author zhoujian 
@version 1.0.0
@date 2013-11-18
*/

#include "fes/fesdatashm.h"

/**
@brief ǰ�������ݹ����ڴ������

�ṩ��ǰ�������ݹ����ڴ�Ļ��գ���������յ�ʱ�򲻻��ƶ�����ͨ��������
���ǿռ䲻�������Դ˴��ռ䶼���õĺܴ�

*/
class FES_DATA_EXPORT Fes_data_shm_allocator : public Fes_data_shm
{
public:
	/**
	@brief ��ȡʵ��,��ʵ������
	@return Fes_data_shm_allocatorʵ��
	*/
	static Fes_data_shm_allocator& get();

	/**
	@brief ����ͨ���������ݿռ�
	@param in const int& channel ͨ����
	*/
	int alloc_channel_data(const int& channel);
	/**
	@brief ����ͨ���������ݿռ�
	*/
	int recycle_channel_data(const int& channel);
    /**
	@brief ���ò�������ֵ
	*/
	int set_yc_para(const int& channel, void *data, int len);
	int set_yx_para(const int& channel, void *data, int len);
	int set_kwh_para(const int& channel, void *data, int len);

	~Fes_data_shm_allocator();

protected:
	/**
	@brief ��ʼ��ң����صĸ�������
	*/
	void set_yc_default(const int& channel);
	/**
	@brief ��ʼ��ң����صĸ�������
	*/
	void set_yx_default(const int& channel);
	/**
	@brief ��ʼ�������صĸ�������
	*/
	void set_kwh_default(const int& channel);
	/**
	@brief ����ң��ֵ
	@para in data_no Ϊͨ���е�ƫ�Ƶ�ַ������Ӧң����

	ͬʱ���й��̼��㣬���timeΪNULL��˵���ǲ���ʱ������ݣ���Ҫ��������ʱ��
	�������ɵ�FE_DATA_MSG_NODEд��ȫ�ֻ��棨�����ʱ����ʵ�֣�
	*/
	void set_yc_value(const int& channel, const int& data_no, const float& value,
					  const unsigned char& quality, FETIME* time = NULL);
	/**
	@brief ����ң��ֵ
	@para in data_no Ϊͨ���е�ƫ�Ƶ�ַ������Ӧң�ŵ��

	ͬʱ���й��̼��㣬���timeΪNULL��˵���ǲ���ʱ������ݣ���Ҫ��������ʱ��
	�������ɵ�FE_DATA_MSG_NODEд��ȫ�ֻ��棨�����ʱ����ʵ�֣�
	*/
	void set_yx_value(const int& channel, const int& data_no, const unsigned char& value,
		const unsigned char& quality, FETIME* time = NULL);
	/**
	@brief ���õ��ֵ
	@para in data_no Ϊͨ���е�ƫ�Ƶ�ַ������Ӧң�����

	ͬʱ���й��̼��㣬���timeΪNULL��˵���ǲ���ʱ������ݣ���Ҫ��������ʱ��
	�������ɵ�FE_DATA_MSG_NODEд��ȫ�ֻ��棨�����ʱ����ʵ�֣�
	*/
	void set_kwh_value(const int& channel, const int& data_no, const unsigned long& value,
		const unsigned char& quality, FETIME* time = NULL);

private:
	/**
	@brief ���캯������ʼ����Ա��������ʽ����
	*/
	Fes_data_shm_allocator();
	/**
	@brief ֻ��������ʵ��
	��������������л����Ĭ�Ͽ������캯�������ֽڿ����������ܻ��𻵵���
	�е����ݣ���ֻ������ʵ�ֵķ�������Ĭ�Ͽ������캯���������ʹ���е���
	�������캯����������������
	*/
	Fes_data_shm_allocator(const Fes_data_shm_allocator&);
	/**
	@brief ֻ��������ʵ��
	��������������л����Ĭ�ϸ�ֵ����������ֽڸ�ֵ�������ܻ��𻵵���
	�е����ݣ���ֻ������ʵ�ֵķ�������Ĭ�Ͽ������캯���������ʹ���е���
	�������캯����������������
	*/
	Fes_data_shm_allocator& operator = (const Fes_data_shm_allocator&);

private:
	buddy*		m_yc_alloctor;	//ң�������
	buddy*		m_yx_alloctor;	//ң�ŷ�����
	buddy*		m_kwh_alloctor;	//��ȷ�����

};