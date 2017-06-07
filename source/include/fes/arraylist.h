/**
@file arraylist.h
@brief ��������ģ��ͷ�ļ�
@author zhangyun 
@version 1.0.0
@date 2012-10-31
*/
#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#include <vector>
#include <list>
#include <algorithm>
using namespace std;

/**
@brief ��������ģ����

�ṩ������ʱ�临�Ӷȵ�Ԫ���������롢ɾ������ȡ�ӿڣ����̰߳�ȫ��
��������ݻ����η������飬������ﵽ��������ʱ����д���������¸�
���飬��������������������ʱ�������µ����顣ɾ��������Ϊ��λ��
ֱ����������飬Ȼ����������ƶ�������β����
*/

template <typename T>
class Array_list
{
	enum
	{
		DEFAULT_ARRAY_SIZE = 4096,
		DEFAULT_LIST_SIZE = 4
	};
public:
	/**
	@brief ���캯��
	@param in const int& array_size ��������ĳ���
	@param in const int& array_size ����ĳ���
	*/
	explicit Array_list(const int& array_size, const int& list_size)
	:ARRAY_SIZE(array_size > 0 ? array_size : DEFAULT_ARRAY_SIZE),
	LIST_SIZE(list_size > 0 ? list_size : DEFAULT_LIST_SIZE)
	{
		for (int i = 0; i < list_size; i++)
		{
			append_an_array();
		}
	};

	/**
	@brief �������캯�� ����Ĭ�Ͽ������캯������ʵ�֣���ֹ����
	*/
	Array_list(const Array_list&);

	/**
	@brief ��ֵ������ ����Ĭ�ϸ�ֵ����������ʵ�֣���ֹ����
	*/
	Array_list operator = (const Array_list&);

	/**
	@brief ��������
	*/
	~Array_list()
	{
		//���ɾ������
		typename std::list<vector<T>*>::iterator end = m_array_list.end();

		for (typename std::list<vector<T>*>::iterator it = m_array_list.begin(); it != end; it++)
		{
			delete *it;
			*it = NULL;
		}

		m_array_list.clear();
	};

	/**
	@brief �ж��Ƿ�Ϊ��
	*/
	bool empty(void) const
	{
		return m_array_list.empty() ? true : m_array_list.front()->empty();
	};

	/**
	@brief ��ȡ�׸������ָ��
	@return �ɹ������׸������ָ�룬ʧ�ܷ���NULL
	*/
	T* get_head_array(void) const
	{
		//vector����ͨ��&v[0]ȡ��Ԫ�صĵ�ַ
		return empty() ? NULL : &(*(m_array_list.front()))[0];
	};

	/**
	@brief ��ȡ�׸�����ĳ���
	@return �ɹ������׸�����ĳ��ȣ�ʧ�ܷ���-1
	*/
	int	get_head_size(void) const
	{
		return empty() ? -1 : m_array_list.front()->size();
	};

	/**
	@brief ��ղ��ƶ��׸����鵽����β����ʹ����ͷ������ʼ��������
	*/
	void clear_head(void)
	{
		if (!empty())
		{
			m_array_list.front()->clear();

			//��һ�����黹û����������£�û�б�Ҫ�ƶ����鵽����β����ֻ��Ҫ�ƶ�array���������׸�array��ͷ��
			if (m_list_it == m_array_list.begin())
			{
				m_array_it = (*m_array_list.begin())->begin();
			}
			else
			{
				//�����һ�������Ѿ����ˣ��������ƶ�����Ӱ�������������е�����һ��
				m_array_list.push_back(m_array_list.front());
				m_array_list.pop_front();
			}
		}
	};

	/**
	@brief �������ݣ����û��ʣ��ռ����Զ�����
	@param in const vector<T>& buf �������������
	*/
	void insert_data(vector<T>& source)
	{
		if (source.empty())
		{
			return;
		}

		typename vector<T>::iterator it = source.begin();
		typename vector<T>::iterator end = source.end();

		while (it != end)
		{
			int data_left = end - it;
			int array_left = (*m_list_it)->empty() ? ARRAY_SIZE : ARRAY_SIZE - (*m_list_it)->size();
			int data_insert = min(data_left, array_left);

			//��������
			(*m_list_it)->insert(m_array_it, it, it + data_insert);
			//�ƶ�����Դ�ĵ�����
			advance(it, data_insert);

			//���뵼��array������ʧЧ����������֮
			m_array_it = (*m_list_it)->begin() += ((*m_list_it)->size()); 

			//��������
			if ((*m_list_it)->size() == ARRAY_SIZE)
			{
				m_list_it++;

				//����Ҳ����
				if (m_list_it == m_array_list.end())
				{
					append_an_array();
				}
				else
				{
					m_array_it = (*m_list_it)->begin();
				}
			}
		}	
	};

	/**
	@brief �������ݣ����û��ʣ��ռ����Զ�����
	@param in T ���������
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int insert_data(T data)
	{
	
// 		vector<T>* element_ptr = *m_list_it;
// 		if (!element_ptr)		//�ж����ڲ����vectorָ�����Ч
// 		{
// 			return -1;
// 		}
// 		else
// 		{
// 			element_ptr->push_back(data);		//ֱ�Ӳ��룬�������Ļ�����������
// 		}
// 
// 		//�˴�list��������Ӧ��ʧЧ������VC�Ե������Ĵ����ã�ֻ������
// 		bool empty_flag = empty();
// 		m_list_it = m_array_list.begin();
// 		if (!empty_flag)
// 		{
// 			advance(m_list_it, m_array_list.size() - 1);
// 		}
// 
// 		m_array_it = (*m_list_it)->begin();
// 
// 		//��������
// 		if ((*m_list_it)->size() == ARRAY_SIZE)
// 		{
// 			m_list_it++;
// 
// 			//����Ҳ����
// 			if (m_list_it == m_array_list.end())
// 			{
// 				append_an_array();
// 			}
// 			else
// 			{
// 				m_array_it = (*m_list_it)->begin();
// 			}
// 		}

		vector<T> vector_insert;
		vector_insert.push_back(data);
		insert_data(vector_insert);


  		return 0;
	};

private:
	/**
	@brief ����һ�����鵽������
	*/
	void append_an_array(void)
	{
		bool empty_flag = empty();

		vector<T>* element_ptr = new vector<T>;
		element_ptr->reserve(ARRAY_SIZE);
		m_array_list.push_back(element_ptr);

		//�˴�list��������Ӧ��ʧЧ������VC�Ե������Ĵ����ã�ֻ������
		m_list_it = m_array_list.begin();

		if (!empty_flag)
		{
			advance(m_list_it, m_array_list.size() - 1);
		}
		
		m_array_it = (*m_list_it)->begin();
	};

private:
	typename list<vector<T>* >::iterator	m_list_it;	///<��һ�β���ʱ�����������
	typename vector<T>::iterator			m_array_it;	///<��һ�β���ʱ�����������

	const int			ARRAY_SIZE;	///<�����С
	const int			LIST_SIZE;	///�����С
	list<vector<T>* >	m_array_list;///<��������

};

#endif







