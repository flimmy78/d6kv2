//ʵʱ�����࣬��ȡ�������ϵ�ʵʱ�ɼ�����------ң�ţ�����״̬���Ƿ���Ч
//                                        |	
//                                        |---ң�⣺�й����޹�����ѹ������
//�ɼ���ʵʱ���ݽ���ϵ�������ͷ��������ֻ�Թ������ã�������ԭ�����£�
//��SCADA��ȡ��ʵʱ����a,��ң��ĵ���ϵ��Ϊt,����Ϊf(-1,�ҽڵ�������ڵ�;1,��ڵ������ҽڵ�)
//DA�����и�ң��ʵ������Ϊval
//	val=a*t*f								

#ifndef _STREAM_DATA_XWQ_20030515_H_ 
#define _STREAM_DATA_XWQ_20030515_H_

#include "GlobalDef.h"
#include "rdbapi.h"
#include "netapi.h"

class CStreamData
{
	enum POWERDIR
	{
		NEGATIVE = -1,
		POSITIVE = 1,
	};
public:
	CStreamData();
	~CStreamData();
#ifdef _WINDOWS
	int InitStreamPara(CWnd* pWnd);
#else
	int InitStreamPara();
#endif
	void ReadStreamData();
	void StopStreamData();
	void ReadBackupDataYx(unsigned char** pState , unsigned char** pInvalidf ,int * StateNum);
	void ReadBackupDataYc(float** pP ,float** pQ ,float** pU , float** pI ,int * StateNum);
	void ReadRealYxData(unsigned char** pState , unsigned char** pInvalidf , int * StateNum);
	void ReadRealYcData(float** pP ,float** pQ ,float** pU , float** pI ,int *StateNum);
	int RefreshStreamData(
#ifdef _WINDOWS
		CWnd* pWnd
#else
		
#endif		
		);
//	int RefreshStreamData();
//private:
	int BackupingData();
public:
	float* m_pPowerU;//��Դ���ѹ��
private:
	unsigned char *m_pState[REALDATA_BUF_SIZE];
	unsigned char *m_pInvalidf[REALDATA_BUF_SIZE];
	float* m_pAnalogP[REALDATA_BUF_SIZE];
	float* m_pAnalogQ[REALDATA_BUF_SIZE];
	float* m_pAnalogU[REALDATA_BUF_SIZE];
	float* m_pAnalogI[REALDATA_BUF_SIZE];
	unsigned char *m_pStateAtFaultMoment;
	unsigned char *m_pInvalidfAtFaultMoment;
	float* m_pAnalogPAtFaultMoment;
	float* m_pAnalogQAtFaultMoment;
	float* m_pAnalogUAtFaultMoment;
	float* m_pAnalogIAtFaultMoment;
private:
	RDB_FIELD_STRU* m_pStreamData;
//	RDB_FIELD_STRU* m_pBackupData;
	Rdb_QuickStream rdb;
	//��д��־����ֹ�����߳�ͬʱ����
	BOOL bUsing[REALDATA_BUF_SIZE];
	BOOL bBackUsing;
	int m_curWritePointer;
	SYS_TIME inittime;
	SYS_TIME saveRdataTime[REALDATA_BUF_SIZE];
	int m_iParaBaseNum;
	POWERDIR m_dir;
};
#endif
