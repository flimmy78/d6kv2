/**
@file fesdatashm_accessor.h
@brief ǰ�������ݹ����ڴ�ͷ�ļ�

�ṩǰ�������ݹ����ڴ����

@author lujiashun 
@version 1.0.0
@date 2016-08-12
*/

#include "fes/fesdatashm_accessor.h"

Fes_data_shm_accessor::Fes_data_shm_accessor()
{

}

Fes_data_shm_accessor& Fes_data_shm_accessor::get()
{
	static Fes_data_shm_accessor instance;
	return instance;
}
