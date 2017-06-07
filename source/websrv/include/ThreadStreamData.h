// ThreadStreamData.h: interface for the ThreadStreamData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADSTREAMDATA_H__5776CA4D_97F4_4976_9843_652980078DB5__INCLUDED_)
#define AFX_THREADSTREAMDATA_H__5776CA4D_97F4_4976_9843_652980078DB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdb/rdbdef.h"//ʵʱ���ͷ����

//�߳������ݹ���
class CThreadStreamData  
{
public:
	RDB_VALUETYPE_STRU * pvalue;//�����ݵ�ֵ
	int * pdatachangedf;//ÿ�����ݵĸı��־
	int * pdatatype;//�Ƿ��Ѿ��õ�������������,Ϊ�ӿ��ٶȣ�����ÿ�ζ�������������
	int bchangef;//�Ƿ������ݱ�ˢ��
	int datacount;//�����ݵĸ���
	int * pold_order;//ԭʼ���͵��������
	CThreadStreamData(int datanums);
	virtual ~CThreadStreamData();

};

#endif // !defined(AFX_THREADSTREAMDATA_H__5776CA4D_97F4_4976_9843_652980078DB5__INCLUDED_)
