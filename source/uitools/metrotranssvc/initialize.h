///initialize.h
///�������������ĳ�ʼ������
///
///��Ҫ���б�֤Ψһʵ��������ע�ᣬ���ݿ⿪���ȹ��������˳�������
///
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _INITIALIZE_H
#define _INITIALIZE_H
#include <QThread>

#include <net/netapi.h>

class DaThread : public QThread 
{    
	Q_OBJECT	
public:    
	DaThread();    
protected:    
	void run();	
private:    
};

class EvtThread : public QThread 
{    
	Q_OBJECT	
public:    
	EvtThread();    
protected:    
	void run();	
private:    
};

class CInitialize : public NetDbg
{
public:
	CInitialize(){};
	~CInitialize(){};
// #ifdef _WINDOWS
// 	int BeginInit(CWnd* pWnd);
// #else
	int BeginInit();
// #endif
	int EndCleanup();
	int CheckParentProc();
	int IsMainDAServer();

private:
	DaThread *pDaThread; //�߳�
	EvtThread *pEvtThread; //�߳�


// #ifdef WIN32
// 	HANDLE m_hInstance;
// #endif
	NetStatus netstate;
#ifndef __PROGRAM_DEBUG_XXX
	ProcMan netmanage;
#endif
};


#endif