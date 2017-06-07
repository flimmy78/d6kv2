/**
@file virtualallocor.h
@brief �ڴ����������ͷ�ļ�

@author zhangyun 
@version 1.0.0
@date 2013-11-01
*/
#ifndef FES_VIRTUAL_ALLOCTOR_H_
#define FES_VIRTUAL_ALLOCTOR_H_

/**
@brief ���������

��һ���������ڴ�ռ��а������������ڴ棬ֻ����ƫ��������ʵ�ʷ����ڴ�
��ǰֻ�Ǽ򵥵ķ��䡢���գ�������κη�������㷨���п����ںܶ�β�ͬ��
С�ķ���ͻ��պ󣬻��γ���Ƭ�����пռ���޷����䣬ֻ����չ�������Ϊ��
ʵ�ּ���ʱ���ԣ���ʱ��ȥ�о�buddy֮����㷨��
*/
class Virtual_allocor
{
	typedef map<int, int>::iterator MAP_IT;
	typedef map<int, int>::reverse_iterator MAP_RIT;

	enum
	{
		DEFAULT_ALLOCOR_SIZE = 0x40000
	};
public:
	Virtual_allocor();
	explicit Virtual_allocor(const int& in_size);
protected:

private:
	/**
	@brief ���������ڴ�

	�ɹ�������ţ�ʧ�ܷ���-1
	*/
	int alloc(const int& lenth);

	/**
	@brief ��������ڴ�

	�ɹ�����0��ʧ�ܷ���-1
	*/
	int recycle(const int& position);

	/**
	@brief �ı䳤��

	�ɹ�����0��ʧ�ܷ���-1
	*/
	int resize(void);
private:
	int m_size;
	
	map<int, int>	m_used_mem;	//��ʹ�õ��ڴ������<��ʼԪ�����, ����>
	map<int, int>	m_free_mem;	//δʹ�õ��ڴ������<��ʼԪ�����, ����>
};



#endif
