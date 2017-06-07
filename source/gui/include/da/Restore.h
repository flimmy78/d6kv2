////
///
///
//
//////////////////////////////////////////////////////////////////
#ifndef _DA_RESTORE_H_XWQ20030523_
#define _DA_RESTORE_H_XWQ20030523_

#include "GlobalDef.h"
#include "Isolate.h"
#include "ChainTab.h"

typedef struct _island_info
{
	int islandno;
	int state;//�������
	int isneedrestore;//�Ƿ���Ҫ�ָ�
	BOOL isfault;//�Ƿ��ǹ��ϵ�
	_island_info* pNext;
} ISLANDINFO;

typedef struct _restore_brk_
{
	int brkaddr;
	unsigned char action;
	BOOL linkflag;//��Ǹÿ����������ָ���֮������翪��
	_restore_brk_* pNext;
} RESTOREBRK;

typedef struct
{
	int brkaddr;
	unsigned char action;
} RESTOREBRKINFO;

typedef struct 
{
	int BrkNum;
	RESTOREBRKINFO * m_pBrk;
	int *m_pBeginFlagPerRgn;
} RESTOREPRJ;

class CRestore
{
	//Functions
public:
	CRestore();
	~CRestore();
	int Initialize(CIsolate& Iso);
	virtual void Clear();
	virtual void GenRestorePro(int& prjnum);//��Ϸ���
	int PerformResPrj(int PrjNo, BOOL bForceSerialYk=FALSE);

protected:
	void GetResRgnNum();
	int GetRestoreBrk();//�������Իָ��������Ŀ�������
	int TpAnalysis();
	void SpreadAllIslandNode(int nodeaddr,const int islandno, int& flag);
	//Variables
public:
//	int GetResPrjNum();
	unsigned char *m_pState;
	unsigned char *m_pInvalidf;
	int m_iBrkNum;
protected:
	//����ֵ:FALSE������û�䶯���������ҽڵ㵺�ŷ�����Ч��TRUE�����˱䶯���������ҽڵ㵺�ŷ�����Ч
	BOOL TpSwitch(int brkaddr, int action);
	void WritePrjtoRdb();
	int m_iFaultNum;
	FAULTINFO *m_pFault;
	int m_iIsoBrkNum;
	ISOLATE_BRK *m_pIsoBrk;

	////���˷������
	ISLANDINFO * m_pIslandinfo;
	//////�ָ������еĻ���ͳ����Ϣ
	int m_iResRgnNum;
	int *m_pActBrkNum;
	RESTOREBRK ** m_ppResBrkPerResRgn;
	/////������Ϣ
	int m_iPrjNum;
	RESTOREPRJ* m_pPrj;
};
#endif