///initialize.h
///�������������ĳ�ʼ������
///
///��Ҫ���г���ע�ᣬ�̴߳��������ݿ⿪�� ���˳�����ȹ���
///
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _INITIALIZE_H_XWQ_20030514
#define _INITIALIZE_H_XWQ_20030514
#include <net/netapi.h>

class CInitialize : public NetDbg
{
public:
	CInitialize();
	~CInitialize();
	int beginInit();	
	int endCleanup();	//���������̡߳�ͣ�������̡�ע������ֹͣ������ˢ��
	int checkParentProc();	

private:

#ifdef _WINDOWS
	HANDLE m_hInstance;
#endif
	NetStatus netstate;
	ProcMan netmanage;
};
#endif