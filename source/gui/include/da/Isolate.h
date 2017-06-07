

#ifndef _DA_ISOLATE_H_XWQ20030521_
#define _DA_ISOLATE_H_XWQ20030521_

#include "GlobalDef.h"
#include "scdapi.h"

class CIsolate
{
	///Functions
public:
	CIsolate();
	~CIsolate();
	void Clear();
	int LocateFaultPos();
	int IsolateFault(BOOL bFlag);
	int ExIsolateFault();
	int HaveSubDealMode();
	int AnalysisReport();
	unsigned char GetFaultType(int BrkNo);
private:
	void LocateTpSearch(int BrkAddr , int NodeAddr , const int searchno , int& flag);
	
	int GetDevName(SYS_EVENT& evt,char* devname);
	///variables
public:
	EX_SYS_EVENT *m_pEvt;
	int m_iEvtNum;
	unsigned char *m_pState;
	unsigned char *m_pInvalidf;
	int m_iBrkNum;
	//���ָ��Ľӿ�
	FAULTINFO* m_pFault;
	int m_iFaultNum;
	ISOLATE_BRK * m_pIsoBrk;//���Ҫ������Χ���и���Ļ��������������
	int m_iIsoBrkNum;
private:
//	SCADA_YKRESULT_STRU *m_pIsoYkRes;
//	int m_iYkResNum;

private:

};
#endif