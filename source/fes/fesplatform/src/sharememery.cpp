/**
@file sharememery.cpp
@brief ǰ��ƽ̨��װʵ��֮�����ڴ�ʵ��

@author chenkai 
@version 1.0.0
@date 2016-07-18
*/
#include "fes/platformwrapper.h"
#include "net/netapi.h"

void* Share_memery::create_share_memory(int shm_key, int shm_lenth)
{
	ShareMem shm;
	void* ret = NULL;

	if (0 == shm.CreateShm(shm_key, shm_lenth))
	{
		ret = shm.MappingShm(shm_key, shm_lenth);
	}

	return ret;
}