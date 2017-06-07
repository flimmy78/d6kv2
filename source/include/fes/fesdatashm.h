/**
@file fesdatashm.h
@brief ǰ�������ݹ����ڴ�ͷ�ļ�

�ṩǰ�������ݹ����ڴ�Ĵ��������롢���ʽӿ�

@author zhangyun 
@version 1.0.0
@date 2013-11-08
*/
#ifndef FES_DATA_SHM_H_
#define FES_DATA_SHM_H_

#ifdef WIN32
#ifdef FESSHM_DLL
#define	FES_DATA_EXPORT __declspec(dllexport) 
#else
#define	FES_DATA_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_DATA_EXPORT
#endif 

#include "fes/fesshare.h"

struct buddy;
class Fes_shm;



/**
@brief ң�����
*/
typedef struct
{
	float slope;	//б��
	float ordinate;	//�ؾ� 

	void set_default(void)
	{
		slope = 1;
		ordinate = 0;
	};
}YC_PARA;

/**
@brief ң�Ų���
*/
typedef struct
{
	unsigned char negate;	//ȡ����0Ϊ��ȡ����1Ϊȡ��
	void set_default(void)
	{
		negate = 0;
	};
}YX_PARA;

/**
@brief ��Ȳ���
*/
typedef struct
{
	float full_code;	//����
	float slope;		//б�� = ����/���

	void set_default(void)
	{
		slope = 1;
		full_code = FLOAT_DEFAULT;
	};
}KWH_PARA;

/**
@brief ǰ�����ݿ��ƶΣ�ÿ��ͨ���������ݵ���ʼ��ַ
*/
typedef struct 
{
	int channel_yc_begin[MAX_CHAN_NUM];
	int channel_yx_begin[MAX_CHAN_NUM];
	int channel_kwh_begin[MAX_CHAN_NUM];
	
	int rtu_yc_begin[MAX_RTU_NUM];
	int rtu_yx_begin[MAX_RTU_NUM];
	int rtu_kwh_begin[MAX_RTU_NUM];
}FES_DATA_CRL_SEG;

/**
@brief ǰ�����������ݶΣ�����ң�⡢ң�š����
*/
typedef struct  
{
	FE_YC yc[MAX_YC_NUM];
	FE_YX yx[MAX_YX_NUM];
	FE_KWH kwh[MAX_KWH_NUM];
}FES_DATA_SEG;

/**
@brief ǰ�����ݲ�����
*/
typedef struct  
{
	YC_PARA yc[MAX_YC_NUM];
	YX_PARA yx[MAX_YX_NUM];
	KWH_PARA kwh[MAX_KWH_NUM];
}FES_DATA_PARA_SEG;



/**
@brief ǰ�����ݹ����ڴ�ṹ
*/
typedef struct
{
	FES_DATA_CRL_SEG ctrl_seg;
	FES_DATA_PARA_SEG para_seg;
	FES_DATA_SEG data_seg;
}FE_DATA;

/**
@brief ǰ�������ݹ����ڴ������

�ṩ��ǰ�������ݹ����ڴ�����롢���գ���������յ�ʱ�򲻻��ƶ�����ͨ��������
���ǿռ䲻�������Դ˴��ռ䶼���õĺܴ�

���Ƿ������С��λ����ʹֻ����0��ң�⣬Ҳ�����1�����

���ǵ��Ժ�Ŀ��ܻ��зǻ������͵�������ΪFE_DATA�ĳ�Ա�ˣ��˴�����ʹ��ģ������
��һ����װ
*/
class FES_DATA_EXPORT Fes_data_shm
{
public:
	enum
	{
		FES_DATA_SHM_KEY = 6666777,		///<�����ڴ�key
		FES_DATA_SHM_LEN = 0x8000000,	///<�����ڴ泤��
		YC_BLOCK_SIZE = 100,				///<ң����С��
		YX_BLOCK_SIZE = YC_BLOCK_SIZE,	///<ң�ſ��С
		KWH_BLOCK_SIZE = 20,				///<��ȿ��С
	};
public:
	~Fes_data_shm();
	/**
	@brief ���캯������ʼ����Ա��������ʽ����
	*/
	Fes_data_shm();

	/**
	@brief ��ȡͨ����ң����ʼ
	*/
	FE_YC* channel_yc(const int& channel) const;
	const FE_YC* const_channel_yc(const int& channel) const;
	/**
	@brief ��ȡͨ����ң����ʼ
	*/
	FE_YX* channel_yx(const int& channel) const;
	const FE_YX* const_channel_yx(const int& channel) const;
	/**
	@brief ��ȡͨ���ĵ����ʼ
	*/
	FE_KWH* channel_kwh(const int& channel) const;
	const FE_KWH* const_channel_kwh(const int& channel) const;
	/**
	@brief ��ȡ�ն˵�ң����ʼ
	@return ͨ�������ƫ�Ƶ�ַ
	*/
	int rtu_yc(const int& rtu) const;
	const int const_rtu_yc(const int& rtu) const;
	/**
	@brief ��ȡ�ն˵�ң����ʼ
	@return ͨ�������ƫ�Ƶ�ַ
	*/
	int rtu_yx(const int& rtu) const;
	const int const_rtu_yx(const int& rtu) const;
	/**
	@brief ��ȡ�ն˵ĵ����ʼ
	@return ͨ�������ƫ�Ƶ�ַ
	*/
	int rtu_kwh(const int& rtu) const;
	const int const_rtu_kwh(const int& rtu) const;

	/**
	@brief ��ȡYC��б��
	*/
	const float const_yc_slope(const int& channel, const int& datano) const;
	/**
	@brief ��ȡYC�Ľ�ȡ
	*/
	const float const_yc_ordinate(const int& channel, const int& datano) const;
	/**
	@brief ��ȡYX��ȡ����־
	*/
	const unsigned char const_yx_negate(const int& channel, const int& datano) const;
	/**
	@brief ��ȡKWH������ֵ
	*/
	const float const_kwh_full_code(const int& channel, const int& datano) const;
	/**
	@brief ��ȡKWH��б��
	*/
	const float const_kwh_slope(const int& channel, const int& datano) const;

private:
	Fes_data_shm(const Fes_data_shm&);
	Fes_data_shm& operator = (const Fes_data_shm&);

protected:
	FE_DATA*	m_fe_data;		//�����ݵ�ַ
};

#endif