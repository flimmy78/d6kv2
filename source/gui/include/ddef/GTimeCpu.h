#ifndef GTIMECPU_H_HEADER_INCLUDED_BCAB1FEE
#define GTIMECPU_H_HEADER_INCLUDED_BCAB1FEE
#include "ddes_comm.h"
#include <time.h>

//##ModelId=433FC90C00BB
//##Documentation
//## ���壺
//##   ����Ч�ʲ�����
//## ���ܣ�  
//##   �ṩ����ʵЧ���Թ���
//## ��ƣ�
//##   ������
//## ���ڣ�
//##   2005-10-2
class GDE_EXPORT_DEF GTimeCpu
{
public:
	//##ModelId=433FCF3F03B9
    //##Documentation
    //## ���ܣ�
    //##      ���캯��
    //## ������
    //##      uint8 autostart  �Զ���ʼ��ʱ
	//##	  uint8 outresult  ����������
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##    2005-10-2
	GTimeCpu(uint8 autostart=0,uint8 outresult=0);
	
	//##ModelId=433FD1E402EE
    //##Documentation
    //## ���ܣ�
    //##    ��������
    //## ������
    //##    ��
    //## ���أ�
    //##    ��
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-2
	~GTimeCpu();

	//##ModelId=433FD1E402EE
    //##Documentation
    //## ���ܣ�
    //##    ȡ��CPUʱ�Ӽ��
    //## ������
    //##    ��
    //## ���أ�
    //##    clock_t
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-2
	clock_t cpuTick() const;

	//##ModelId=433FD1E402EE
    //##Documentation
    //## ���ܣ�
    //##    ȡ��CPUʱ�䣨�룩
    //## ������
    //##    ��
    //## ���أ�
    //##    FLOAT8
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-2
	FLOAT8 cpuTime() const;

	//##ModelId=433FD1E402EE
    //##Documentation
    //## ���ܣ�
    //##    ����CPU��ʱ
    //## ������
    //##    ��
    //## ���أ�
    //##    clock_t
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-2
	clock_t start();

	//##ModelId=433FD1E402EE
    //##Documentation
    //## ���ܣ�
    //##    ��ֹCPU��ʱ
    //## ������
    //##    ��
    //## ���أ�
    //##    clock_t
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-2
	clock_t stop();

	//##ModelId=433FD1E402EE
    //##Documentation
    //## ���ܣ�
    //##    ����CPU��ʱ
    //## ������
    //##    ��
    //## ���أ�
    //##    ��
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-2
	void	reset();

	//##ModelId=433FD1E402EE
    //##Documentation
    //## ���ܣ�
    //##    ����������Խ��
    //## ������
    //##    uint8 enable
    //## ���أ�
    //##    ��
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-2
	void	outEnable(uint8 enable=1);

protected:

	//clock start rec
	clock_t	m_cs;

	//clock end rec
	clock_t m_ce;

	//clock freq
	clock_t m_cf;

	//auto start
	uint8   m_as;

	//out result
	uint8	m_or;

};

#endif
