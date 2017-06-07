#ifndef MASINFO_DBG_PARA_H_
#define MASINFO_DBG_PARA_H_

#include <QMutex>
#include "dqinfo_dbgclnt.h"
#include "net/netapi.h"
#include <iostream>
using namespace std;

class CDbgPara
{
public:
	CDbgPara();
	int getDbg(int* typeNo, char* type, char* desc);
	int setRemote(bool flag, int hostNo, char* ipAddr);
	int getRenmoteTask(DbgProcInfo* &procInfo);
	void setDbgID(int ID);
	bool isRemote(void){return m_remoteFlag;};

public:
	bool			 m_remoteFlag;
	NetDbg			 m_netDbg;
	REMOTE_DBG_PROC  m_remoteDbg;
	CDbgClnt		 m_remoteDbgClient;	
	REMOTE_DBG_SEL	 m_remoteSel;
	int				 m_remoteHost;
	int				 m_dbgID;
	QMutex			 m_remoteLock;
	bool			 m_remoteStopFlag;	//�˱�־������Զ���л��ر���״̬ʱ����Զ�̶�ȡ���һ����Ϣ��رգ���ֱֹ�ӹرջῨ�ܳ�ʱ��
};
#endif