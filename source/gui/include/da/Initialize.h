///dainitialize.h
///�������������ĳ�ʼ������
///
///��Ҫ���б�֤Ψһʵ��������ע�ᣬ���ݿ⿪���ȹ��������˳�������
///
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _INITIALIZE_H_XWQ_20030514
#define _INITIALIZE_H_XWQ_20030514
#include <netapi.h>
class CInitialize : public NetDbg
{
public:
//	void PutDbg(int type,char *format,...);
	CInitialize(){};

	~CInitialize(){};
#ifdef _WINDOWS
	int BeginInit(CWnd* pWnd);
#else
	int BeginInit();
#endif

	int EndCleanup();
	BOOL CheckParentProc();
	BOOL IsMainDAServer();
	//variables
private:
//	int GrpNo;
#ifdef _WINDOWS
	HANDLE m_hInstance;
#endif
//	NetDbg dbg;
	NetStatus netstate;
#ifndef __PROGRAM_DEBUG_XXX
	ProcMan netmanage;
#endif
};
#endif