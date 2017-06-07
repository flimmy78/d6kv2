/**
@file fesdatashm_allocator.h
@brief ǰ�������ݹ����ڴ�ͷ�ļ�

�ṩǰ�������ݹ����ڴ����

@author lujiashun 
@version 1.0.0
@date 2016-08-12
*/

#include "fes/fesshm.h"
#include "fes/fesdatashm_allocator.h"
#include "fes/platformwrapper.h"
#include "buddy.h"
#include "math.h"

#include <ace/Log_Msg.h>

Fes_data_shm_allocator::Fes_data_shm_allocator()
{
	double log_2 = log(static_cast<double>(2));

	//���ݿ�Ĵ�С���ָ��һ�̶�ֵ,̫С���ܻᵼ����Ƭ̫��,̫����ܻᵼ�¿ռ���˷�,�ʴ˴��Ĵ�����
	//Ϊȡ������������ң�⣬ң�Ż��ȸ������ֵ�Ƶ�����ֵ���Դ�Ϊ������Ϊ���ݿ��С
	unsigned short* Block_Size = NULL;
	yc_block_size = YC_BLOCK_SIZE;
	yx_block_size = YX_BLOCK_SIZE;
	kwh_block_size = KWH_BLOCK_SIZE;
	int rcd_cnt = Db_connector::select("select ң����� from (select ң�����,dense_rank() over(order by cnt desc) \
									   as rnk from (select ң�����,count(*)as cnt from �ն˲����� group by ң�����)) where rnk = 1",
		(void **)&Block_Size );
	if ( rcd_cnt > 0 && *Block_Size > 0 )
	{
		yc_block_size = *Block_Size;
	}
	rcd_cnt = Db_connector::select("select ң�Ÿ��� from (select ң�Ÿ���,dense_rank() over(order by cnt desc) \
									   as rnk from (select ң�Ÿ���,count(*)as cnt from �ն˲����� group by ң�Ÿ���)) where rnk = 1",
		(void **)&Block_Size );
	if ( rcd_cnt > 0 && *Block_Size > 0 )
	{
		yx_block_size = *Block_Size;
	}

	rcd_cnt = Db_connector::select("select ��ȸ��� from (select ��ȸ���,dense_rank() over(order by cnt desc) \
									   as rnk from (select ��ȸ���,count(*)as cnt from �ն˲����� group by ��ȸ���)) where rnk = 1",
		(void **)&Block_Size );
	if ( rcd_cnt > 0 && *Block_Size > 0 )
	{
		kwh_block_size = *Block_Size;
	}

	
	//����ң��buddy�Ĳ�������������log2 ��ĸ��� + 1
	/*int yc_level = static_cast<int>(log(static_cast<double>(MAX_YC_NUM / yc_block_size)/log_2) + 1);
	int yx_level = static_cast<int>(log(static_cast<double>(MAX_YX_NUM / yx_block_size)/log_2) + 1);
	int kwh_level = static_cast<int>(log(static_cast<double>(MAX_KWH_NUM / kwh_block_size)/log_2) + 1);*/

	int yc_level = static_cast<int>(log(static_cast<double>(MAX_YC_NUM / yc_block_size))/log_2);
	int yx_level = static_cast<int>(log(static_cast<double>(MAX_YX_NUM / yx_block_size))/log_2);
	int kwh_level = static_cast<int>(log(static_cast<double>(MAX_KWH_NUM / kwh_block_size))/log_2);

	m_yc_alloctor = buddy_new(yc_level);
	ACE_ASSERT(m_yc_alloctor != NULL);

	m_yx_alloctor = buddy_new(yx_level);
	ACE_ASSERT(m_yx_alloctor != NULL);

	m_kwh_alloctor = buddy_new(kwh_level);
	ACE_ASSERT(m_kwh_alloctor != NULL);

}


void Fes_data_shm_allocator::inital_para(int channel_no)
{
	m_fe_data->ctrl_seg.channel_yc_begin[channel_no] = -1;
	m_fe_data->ctrl_seg.channel_yx_begin[channel_no] = -1;
	m_fe_data->ctrl_seg.channel_kwh_begin[channel_no] = -1;	
}


Fes_data_shm_allocator::~Fes_data_shm_allocator()
{
	buddy_delete(m_yc_alloctor);
	buddy_delete(m_yx_alloctor);
	buddy_delete(m_kwh_alloctor);

	m_yc_alloctor = m_yx_alloctor = m_kwh_alloctor = NULL;
}

Fes_data_shm_allocator& Fes_data_shm_allocator::get()
{
	static Fes_data_shm_allocator instance;
	return instance;
}


//������估����
int Fes_data_shm_allocator::alloc_channel_data(const int& channel)
{
	int ret = -1;

	if (channel >= 0 && channel < MAX_CHAN_NUM)
	{
		CHANPARA* para = Fes_shm::get().chan_para_ptr + channel;

		int channel_first_rtu = para->child_rtu;			//��ǰͨ����rtu��
		RTUPARA* rtu_para = Fes_shm::get().rtu_para_ptr + channel_first_rtu;//��ͨ����Ӧ�����ڴ��ָ��

		//�������ɸ�block��ң��
		int block_pos = buddy_alloc(m_yc_alloctor, (para->yc_num + yc_block_size - 1) / yc_block_size);
			
		ACE_ASSERT(block_pos != -1);

		if (block_pos != -1)
		{
			int yc_pos = block_pos;
			m_fe_data->ctrl_seg.channel_yc_begin[channel] = yc_pos * yc_block_size;

			//��ʼ��ң��
			set_yc_default(channel);
		}

		//��������block��ң��
		block_pos = buddy_alloc(m_yx_alloctor, (para->yx_num + yx_block_size - 1) / yx_block_size);
		ACE_ASSERT(block_pos != -1);

		if (block_pos != -1)
		{
			int yx_pos = block_pos;
			m_fe_data->ctrl_seg.channel_yx_begin[channel] = yx_pos * yx_block_size;

			//��ʼ��ң��
			set_yx_default(channel);
		}

		//��������block�ĵ��
		block_pos = buddy_alloc(m_kwh_alloctor, (para->kwh_num + kwh_block_size - 1) / kwh_block_size);
		ACE_ASSERT(block_pos != -1);

		if (block_pos != -1)
		{
			int kwh_pos = block_pos;
			m_fe_data->ctrl_seg.channel_kwh_begin[channel] = kwh_pos * kwh_block_size;

			//��ʼ�����
			set_kwh_default(channel);
		}
		//todo ����ͨ����begin�Ժ���Ҫ�����ٷ����ն˵�begin

		//��ȡyc,yx,kwhƫ������ʼλ��,�׸�rtuƫ��λ��Ϊ0
		int rtu_yc_pos = 0;
		int rtu_yx_pos = 0;
		int rtu_kwh_pos = 0;

		m_fe_data->ctrl_seg.rtu_yc_begin[rtu_para->order] = rtu_yc_pos;
		m_fe_data->ctrl_seg.rtu_yx_begin[rtu_para->order] = rtu_yx_pos;
		m_fe_data->ctrl_seg.rtu_kwh_begin[rtu_para->order] = rtu_kwh_pos;

		//ע�����0���ռ�ʱʵ���Ƿ���1���ռ䣨��C++��newһ����������������ο϶����пռ��
	}//if (channel >= 0 && channel < MAX_CHAN_NUM )

	return ret;
}

int Fes_data_shm_allocator::recycle_channel_data(const int& channel)
{
	int ret = -1;

	if (channel >= 0 && channel < MAX_CHAN_NUM)
	{
		int pos = m_fe_data->ctrl_seg.channel_yc_begin[channel];

		if (pos != -1)
		{
			buddy_free(m_yc_alloctor, pos / yc_block_size);
		}

		pos =  m_fe_data->ctrl_seg.channel_yx_begin[channel];

		if (pos != -1)
		{
			buddy_free(m_yx_alloctor, pos / yx_block_size);
		}

		pos =  m_fe_data->ctrl_seg.channel_kwh_begin[channel];

		if (pos != -1)
		{
			buddy_free(m_kwh_alloctor, pos / kwh_block_size);
		}

		ret = 0;

		//todo �����꣬��Ҫ��channel��rtu��begin����-1
		CHANPARA* para = Fes_shm::get().chan_para_ptr + channel;

		int channel_first_rtu = para->child_rtu;			//��ǰͨ����rtu��
		RTUPARA* rtu_para = Fes_shm::get().rtu_para_ptr + channel_first_rtu;//��ͨ����Ӧ�����ڴ��ָ��

		//channel��begin��-1
		m_fe_data->ctrl_seg.channel_yc_begin[channel] = -1;
		m_fe_data->ctrl_seg.channel_yx_begin[channel] = -1;
		m_fe_data->ctrl_seg.channel_kwh_begin[channel] = -1;

		m_fe_data->ctrl_seg.rtu_yc_begin[rtu_para->order] = -1;
		m_fe_data->ctrl_seg.rtu_yx_begin[rtu_para->order] = -1;
		m_fe_data->ctrl_seg.rtu_kwh_begin[rtu_para->order] = -1;

	}//if (channel >= 0 && channel < MAX_CHAN_NUM)

	return ret;
}

void Fes_data_shm_allocator::set_yx_default(const int& channel)
{
	int channel_yx_begin = m_fe_data->ctrl_seg.channel_yx_begin[channel];		//ȡ��Ӧͨ��ң�ŵ�ַ
	CHANPARA* para = Fes_shm::get().chan_para_ptr + channel;					//ȡ��Ӧͨ��ң�Ÿ���

	for (int  i = 0; i<para->yx_num; i++)
	{
		m_fe_data->para_seg.yx[channel_yx_begin + i].set_default();
		m_fe_data->data_seg.yx[channel_yx_begin + i].set_default();

	}
}

void Fes_data_shm_allocator::set_yc_default(const int& channel)
{
	int channel_yc_begin = m_fe_data->ctrl_seg.channel_yc_begin[channel];			//ȡ��Ӧͨ��ң���ַ
	CHANPARA* para = Fes_shm::get().chan_para_ptr + channel;					//ȡ��Ӧͨ��ң�����

	for (int  i = 0; i<para->yc_num; i++)
	{
		m_fe_data->para_seg.yc[i + channel_yc_begin].set_default();
		m_fe_data->data_seg.yc[i + channel_yc_begin].set_default();

	}
}

void Fes_data_shm_allocator::set_kwh_default(const int& channel)
{
	int channel_kwh_begin = m_fe_data->ctrl_seg.channel_kwh_begin[channel];		//ȡ��Ӧͨ��ң����ַ
	CHANPARA* para = Fes_shm::get().chan_para_ptr + channel;					//ȡ��Ӧͨ��ң������
	for (int  i = 0; i<para->kwh_num; i++)
	{
		m_fe_data->para_seg.kwh[i + channel_kwh_begin].set_default();
		m_fe_data->data_seg.kwh[i + channel_kwh_begin].set_default();

	}
}

FE_YC* Fes_data_shm_allocator::set_yc_value(const int& channel, const int& data_no,\
										  const float& value, const unsigned char& quality,\
										  FETIME* time /* = NULL */)
{
	FE_YC* fe_yc = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no)		//�ж�channel�Ƿ����
	{
		CHANPARA* channel_para = Fes_shm::get().chan_para_ptr + channel;

		if(data_no <= channel_para->yc_num)		//�Ƿ��ͨ��yc
		{
			fe_yc = channel_yc(channel) + data_no;
			fe_yc->set_default();   //yc��ʼ��

		}
		else
		{
			return NULL;
		}

		fe_yc->raw_yc = value;
		
		//������
		//����ת��ֵ = vlaue * slope + ordinate
		fe_yc->processed_yc = value * const_yc_slope(channel,data_no) + const_yc_ordinate(channel, data_no);

		//�������η����ָ�ֵ
		fe_yc->modifier.quality = quality;
		if(time == NULL)
		{
			fe_yc->modifier.self_time_flag = false;

			get_time(&(fe_yc->modifier.time));//��ȡ��ǰʱ��,����ǰϵͳʱ�����yc��ʱ��
		}
		else
		{
			fe_yc->modifier.self_time_flag = true;		//rtu����ʱ��
			fe_yc->modifier.time = *time;
		}
	}

	return fe_yc;
}

FE_YX* Fes_data_shm_allocator::set_yx_value(const int& channel, const int& data_no,\
										  const unsigned char& value, const unsigned char& quality,\
										  FETIME* time /* = NULL */)
{
	FE_YX* fe_yx = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no)		//�ж�channel�Ƿ����
	{
		CHANPARA* channel_para = Fes_shm::get().chan_para_ptr + channel;

		if(data_no <= channel_para->yx_num)		//�Ƿ��ͨ��yx
		{
			fe_yx = channel_yx(channel) + data_no;
			fe_yx->set_default();   //yx��ʼ��

		}
		else
		{
			return NULL;
		}

		fe_yx->raw_yx = value;							//������
		//����ת��ֵ = value ^ negate
		fe_yx->processed_yx = value ^ (const_yx_negate(channel,data_no));


		//�������η����ָ�ֵ
		fe_yx->modifier.quality = quality;
		if(time == NULL)
		{
			fe_yx->modifier.self_time_flag = false;

			get_time(&(fe_yx->modifier.time));//��ȡ��ǰʱ��,����ǰϵͳʱ�����yx��ʱ��
		}
		else
		{
			fe_yx->modifier.self_time_flag = true;		//rtu����ʱ��
			fe_yx->modifier.time = *time;
		}
	}
	return fe_yx;
}

FE_KWH* Fes_data_shm_allocator::set_kwh_value(const int& channel, const int& data_no,\
										   const unsigned long& value, const unsigned char& quality,\
										   FETIME* time /* = NULL */)
{
	FE_KWH* fe_kwh = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no)		//�ж�channel�Ƿ����
	{
		CHANPARA* channel_para = Fes_shm::get().chan_para_ptr + channel;

		if(data_no <= channel_para->kwh_num)		//�Ƿ��ͨ��yx
		{
			fe_kwh = channel_kwh(channel) + data_no;
			fe_kwh->set_default();   //kwh��ʼ��
		}
		else
		{
			return NULL;
		}

		fe_kwh->raw_kwh = value;							//������
		//����ת��ֵ = (vlaue/full_code) *slope
		fe_kwh->processed_kwh = (value / const_kwh_full_code(channel,data_no)) * const_kwh_slope(channel,data_no);


		//�������η����ָ�ֵ
		fe_kwh->modifier.quality = quality;
		if(time == NULL)
		{
			fe_kwh->modifier.self_time_flag = false;

			get_time(&(fe_kwh->modifier.time));//��ȡ��ǰʱ��,����ǰϵͳʱ�����kwh��ʱ��
		}
		else
		{
			fe_kwh->modifier.self_time_flag = true;		//rtu����ʱ��
			fe_kwh->modifier.time = *time;
		}
	}

	return fe_kwh;
}



int Fes_data_shm_allocator::set_yc_para(const int& channel, void *data, int len)
{
	YC_PARA* p_yc_para = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no 
		&& m_fe_data->ctrl_seg.channel_yc_begin[channel] != -1)
	{
		p_yc_para = m_fe_data->para_seg.yc + m_fe_data->ctrl_seg.channel_yc_begin[channel];
	}
	else
	{
		return -1;
	}
	memcpy(p_yc_para,data,len);
	return 0;

}

int Fes_data_shm_allocator::set_yx_para(const int& channel, void *data, int len)
{
	YX_PARA* p_yx_para = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no 
		&& m_fe_data->ctrl_seg.channel_yx_begin[channel] != -1)
	{
		p_yx_para = m_fe_data->para_seg.yx + m_fe_data->ctrl_seg.channel_yx_begin[channel];
	}
	else
	{
		return -1;
	}
	memcpy(p_yx_para,data,len);
	return 0;

}

int Fes_data_shm_allocator::set_kwh_para(const int& channel, void *data, int len)
{
	KWH_PARA* p_kwh_para = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no 
		&& m_fe_data->ctrl_seg.channel_kwh_begin[channel] != -1)
	{
		p_kwh_para = m_fe_data->para_seg.kwh + m_fe_data->ctrl_seg.channel_kwh_begin[channel];
	}
	else
	{
		return -1;
	}
	memcpy(p_kwh_para,data,len);
	return 0;

}