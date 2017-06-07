///������յ�����������Ĺ���
///
///									written by xiaowq 2003-05-19
////////////////////////////////////////////////////////////
#ifndef _MANAGEDAEVT_H_XWQ_20030519_
#define _MANAGEDAEVT_H_XWQ_20030519_

#include "rdbapi.h"
#include "GlobalDef.h"

class CDaEvent
{
public:
	CDaEvent();
	~CDaEvent();

//	void GetEvtNum(int& num);
	void MoveBuf(EX_SYS_EVENT** pEvt , int* EvtNum);
	int ReadEvent(
#ifdef _WINDOWS
		CWnd* pWnd
#endif
		);
private:
	BOOL AddEvent(EX_SYS_EVENT& evt);
	void ReleaseBuf();
private:
	BOOL m_bUsing;
	BOOL m_bNeedClearBuf;
	EX_SYS_EVENT * m_pEvt;
	int m_iBufSize;
	int m_iEvtNum;
	EventProc m_eventproc;
public:
	void BroadcastEvt(SYS_EVENT* pEvt,int num);
	SYS_TIME m_timeFirstFault;//���ͬʱ����ʱ���յ���һ�����ϵ�ʱ��
	SYS_TIME m_timeLastFault;//���ͬʱ����ʱ���յ����һ�����ϵ�ʱ��
};
#endif
