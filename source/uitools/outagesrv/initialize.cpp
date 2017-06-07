#include "initialize.h"
#include "tpdata.h"
#include "streamdata.h"

extern CStreamData g_stream;
extern CTpData g_topoData;
extern pthread_t pEvtThread;


#ifndef WIN32

void ReadStreamData(int i)
{
	printf( "enter ReadStreamData\n" );
	g_stream.readStreamData();
	printf( "exit ReadStreamData\n" );
}
#endif

CInitialize::CInitialize()
{

}

CInitialize::~CInitialize()
{
	endCleanup();
}
int CInitialize::beginInit()
{

	if( RegProc("ͣ��������") == FALSE )
	{
		printf( "����ע��ʧ�ܣ������г������У�" );
		exit(0);
		return 1;
	}

	netmanage.StartProc("outagesrv","ͣ��������",""
#if defined(WIN32)
		,(HWND)20000
#endif
		);

	if(g_topoData.readTpData() == FALSE)
		return 2;

	//��ʼ��������
#ifdef __unix
	sigset(DATA_CHANGED,ReadStreamData);
	//sigset(SIGUSR1,SIG_IGN);
	sigset(END_PROC_SIGNAL,pwrmngQuit);
	if(g_stream.initStreamPara() != 0)
		return 3;
#else
	if(g_stream.initStreamPara() != 0)
		return 3;
#endif

	//���������߳�
	pthread_attr_t ptha;
	pthread_attr_init(&ptha);
	if(pthread_create(&pEvtThread,&ptha,EvtProc,NULL) != 0)
	 return	4;
	
	return 0;
}

int CInitialize::endCleanup()
{
	pthread_cancel(pEvtThread);
	pthread_detach(pEvtThread);
	g_stream.stopStreamData();
	netmanage.EndProc();
	UnRegProc();
	return 0;
}

int CInitialize::checkParentProc()
{
	if( netmanage.CheckParentProc() == 0)
		return FALSE;
	else 
		return TRUE;
}



