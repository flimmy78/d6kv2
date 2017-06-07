// DistSys.h: interface for the CDS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTSYS_H__8E83BC55_2F28_49D4_B5FC_F76589BE3500__INCLUDED_)
#define AFX_DISTSYS_H__8E83BC55_2F28_49D4_B5FC_F76589BE3500__INCLUDED_

// #include "qptrlist.h"
// #include "qfile.h"
// #include "qtextstream.h"
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMap>

#include "sysdef.h"
#include "rdb/rdbapi.h"
#include "model/scdtp.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define			LENGTH_ERRORMSG		512

/////////////////////////////////// 

class CDSMeas;		// �����࣬��������ʵʱ�����α����

class CDSObject;	// ���������ģ���������豸�Ļ���

class 		CDSRootNode;			// ���ڵ�(��Դ��)
class 			CDSOutlet;			// ���������

class		CDSBus;					// ĸ��(���ڼ���)
class		CDSLine;				// �߶�(���ڼ���)

class		CDSDevice;				// �豸
class			CDSNode;			// �ڵ�
class			CSglTermDSDev;		// �����豸
class			CDSDevBus;			// ĸ���豸
class				CDSCapacitor;	// ������
class					CDSLoad;	// ����
class			CDblTermDSDev;		// ˫���豸
class				CDSFeeder;		// �߶� ///////////////////////////////////////
// ���������ģ���������豸�Ļ���
//
class CDSObject
{
private:

	int m_nObjId;	// CDS ��Ԫ�صı�ʶ(ȫ��Ψһ)

public:
	CDSObject(int nObjId = -1) { m_nObjId = nObjId; }
	~CDSObject() {}

	// ��ȡԪ�ر�ʶ
	int GetObjectId() { return m_nObjId; }
	// ����Ԫ�ر�ʶ
	void SetObjectId(int nObjId) { m_nObjId = nObjId; }
};

///////////////////////////////////////
// ���������ģ���е����豸
// 

class CDSDevice : public CDSObject
{
private:

	// ���������豸������ͨ���豸���ƺ��豸������Ψһ��ʶÿ���豸����ͬ�豸����
	// ������ͬ���豸����(m_nDevId)��������������ͬʵ���ʶ(m_nObjId)��
	char	m_strDevName[CODE_LEN];	// device name

public:	
	int		m_DevType;			//�豸����,��scd/devdef.h����

	CDSDevice(char *strDevName, int nObjId) 
		: CDSObject(nObjId)
	{
		Q_ASSERT(strDevName != NULL); strcpy(m_strDevName, strDevName); 
	}
	~CDSDevice() { }

	// ��ȡ�豸����
	char *GetDevName() { return m_strDevName; }
	void SetDevName(char* strDevName) { strcpy(m_strDevName, strDevName); }
};




///////////////////////////////////////
// �ڵ��� (���ڼ�¼���˽ṹ)
//

class CDSNode : public CDSDevice
{
private:
	//modify by lipeng 20110524
	//CPtrArray				LinkedFeederArray; // ��ýڵ������ӵ�����ָ������
	QList<CDSFeeder*>		LinkedFeederArray;//��ýڵ������ӵ�����ָ������
	BOOL					m_bVisited; // ͨ���˱�־�����жϸýڵ��Ƿ����ڹ���(ͣ��)����

public:

	int								m_nIndex;
	//int							m_nodeType;			//�ڵ������������豸����:-1:���豸;0:�ڵ�,1:ĸ��;2:����;3:������;4:��Դ��;
	uint							m_nLoadNum,m_nCapNum,m_nRootNum,m_nDevbusNum;
	QList<CDSLoad*>				m_pLoad;
	QList<CDSCapacitor*>			m_pCapacitor;
	CDSRootNode*					m_pRoot;
	CDSDevBus*						m_pDevBus;

	CDSNode(char *strDevName, int nObjId);
	~CDSNode(); 

	BOOL m_bOutput;

	// ���ڵ㣨���ڻ���������
	CDSNode *m_pParent;
	// ���ø��ڵ�
	void SetParentNode(CDSNode *pParentNode) { m_pParent = pParentNode; }
	// ��ȡ���ڵ�
	CDSNode *GetParentNode() { return m_pParent; }


	// �жϸýڵ��Ƿ��и���
	BOOL HasPowerLoad() { return (m_nLoadNum>0) ? TRUE : FALSE; }


	// �ýڵ�ӳ���ĸ��
	CDSBus *m_pMapedBus; 

	// ��ȡ�ڵ����
	int GetDegree() { return LinkedFeederArray.count(); }
	// ��ȡָ���Ľڵ�
	CDSNode* GetLinkedNode(uint i);
	// ��ȡָ��������
	CDSFeeder* GetLinkedFeeder(uint i);
	// ��ȡ��ָ���ڵ���ص�����
	CDSFeeder* GetLinkedFeeder(CDSNode *pNode);

	// ���ýڵ����Ϊ��ʱ��������һ������
	CDSFeeder* GetAnotherLinkedFeeder(CDSFeeder *pFeeder);
	// ���غ�ָ����·��Ӧ������
	CDSFeeder* GetLinkedFeeder(CDSLine *pLine);
	// ����һ�����ߵ���̬����ָ������
	void AddLinkedFeeder(CDSFeeder *pFeeder);
	// ��һ�����ߴӶ�̬����ָ���������Ƴ�
	void RemoveFeeder(CDSFeeder *pFeeder);
	
	// ���ط��ʱ��
	BOOL GetVisitingFlag() { return m_bVisited; }
	void SetVisitingFlag(BOOL bFlag) { m_bVisited = bFlag; }

	//close by lipeng for topo
	// �ж��Ƿ�Ϊ���ӵ�
	BOOL IsJointPoint(); 

	// ���ѵ㣨��������������/״̬���ƣ�
	CDSNode* m_pBreakNode;
	BOOL IsBreakpoint() { return (m_pBreakNode != NULL) ? TRUE : FALSE; }
	
	int m_nLayerId;
	void SetLayerId(int nLayerId) { m_nLayerId = nLayerId; }
	int GetLayerId() { return m_nLayerId; }

};

// CDSRootNode
//
// ��Դ��
class CDSRootNode : public CDSDevice
{
public:

	CDSNode *m_pNode;
	//BOOL m_bAllocateLoad;

	CDSRootNode(int nObjId, CDSNode *pNode, char* name) 
		: CDSDevice(name, nObjId)
	{
		m_pNode = pNode; 
		m_bActive = TRUE; 
		//m_bAllocateLoad = FALSE;
	}
	~CDSRootNode()
	{
		//OutletArray.RemoveAll();
	}
	
	// ��Դ�����б�־(��Ծ/����)
	BOOL m_bActive;
	void SetActiveFlag(BOOL bActive) { m_bActive = bActive; }
	BOOL IsActiveRootNode() { return m_bActive; }

	//ɾ�����õķ�֧
	//void RemoveTwigs();
	
	// �õ�Դ��ĳ�������
	QList<CDSOutlet*>		OutletArray;
	void AddOutlet(CDSOutlet *pOutlet);
	CDSOutlet* GetOutlet(uint i);
	int GetNumOfOutlets();

	//friend ostream& operator << (ostream& os, CDSRootNode* pRootNode);
};


/////
// CSglTermDSDev �����豸
//
class CSglTermDSDev : public CDSDevice
{
private:

	int m_nLinkId;
	CDSNode* m_pNode; // �����ӵĽڵ�ָ��

public:
	CSglTermDSDev(CDSNode *pNode, char *strDevName)
		: CDSDevice(strDevName, pNode->GetObjectId())
	{
		m_nLinkId = pNode->GetObjectId(); 
		m_pNode = pNode;
	}
	~CSglTermDSDev() { }
	
	int GetLinkId() { return m_nLinkId;	}
	// ��ȡ���ӵĽڵ�ָ��
	CDSNode* GetLinkedNode() { Q_ASSERT(m_pNode != NULL); return m_pNode; }
	// ���������ӵĽڵ�ָ��
	void SetLinkedNode(CDSNode* pNode) { Q_ASSERT(pNode != NULL); m_pNode = pNode; }
};

//
// CDSCapacitor
//
class CDSCapacitor : public CSglTermDSDev
{
private:
	double Cap; // capacity under the normal voltage
	
public:
	CDSCapacitor(char *strDevName, CDSNode *pNode)
		: CSglTermDSDev(pNode, strDevName) {}
	~CDSCapacitor() { }
	
};


//
// CDSTrans
// 
class CDSLoad : public CSglTermDSDev
{
public:

	
	int m_nIndex;	

	CDSLoad(char *strDevName, CDSNode *pNode)
		: CSglTermDSDev(pNode, strDevName)
	{
		m_nIndex = 0;
	}
	~CDSLoad() {}
	
};

class CDSDevBus : public CSglTermDSDev
{
private:
	
public:
	CDSDevBus(char *strDevName, CDSNode *pNode)
		: CSglTermDSDev(pNode, strDevName) {}
	~CDSDevBus() { }
	
};


// CDblTermDSDev
//
class CDblTermDSDev : public CDSDevice
{
private:
	
	int m_nLinkId1, m_nLinkId2;
	CDSNode *m_pNode1, *m_pNode2;

public:

	CDblTermDSDev(int nObjId, char *strDevName, CDSNode *pNode1, CDSNode *pNode2)
	: CDSDevice(strDevName, nObjId)
	{
		m_pNode1 = pNode1; m_pNode2 = pNode2;
		m_nLinkId1 = pNode1->GetObjectId(); m_nLinkId2 = pNode2->GetObjectId();
	}

	~CDblTermDSDev() 
	{
		if (m_pNode1 != NULL) delete m_pNode1;
		if (m_pNode2 != NULL) delete m_pNode2;
	}

	void SetLinkedNode1(CDSNode *pNode1) 
	{ Q_ASSERT(pNode1 != NULL); m_pNode1 = pNode1; m_nLinkId1 = m_pNode1->GetObjectId(); }
	void SetLinkedNode2(CDSNode *pNode2)
	{ Q_ASSERT(pNode2 != NULL); m_pNode2 = pNode2; m_nLinkId2 = m_pNode2->GetObjectId(); }

	void _SetLinkedNode1() { m_pNode1 = NULL; }
	void _SetLinkedNode2() { m_pNode2 = NULL; }
	
	void GetLinkId(int& nLinkId1, int& nLinkId2) 
	{ nLinkId1 = m_nLinkId1; nLinkId2 = m_nLinkId2; }
	void SetLinkId(int nLinkId1, int nLinkId2) 
	{ m_nLinkId1 = nLinkId1; m_nLinkId2 = nLinkId2; }

	CDSNode *GetLinkedNode1() { Q_ASSERT(m_pNode1 != NULL); return m_pNode1; }
	CDSNode *GetLinkedNode2() { Q_ASSERT(m_pNode2 != NULL); return m_pNode2; }
};

///////////////////////////////////////
// CDSOutlet
//

class CDSOutlet : public CDSDevice
{
public:
	CDSRootNode *m_pRootNode;
	
	BOOL m_bActive;
	void SetActiveFlag(BOOL bActive) { m_bActive = bActive; }
	BOOL IsActiveOutlet() { return m_bActive; }

	// this flag is only valid in radial network
	BOOL m_bRadial;
	BOOL IsRadialNetwork() { return m_bRadial; }
	void SetRadialFlag(BOOL bRadial) { m_bRadial = bRadial; }

	CDSOutlet *m_pAnotherOutlet;

	CDSOutlet(int nObjId, char *strDevName, CDSRootNode *pRootNode);
	~CDSOutlet() {}

	CDSBus* m_pHeadBus;
	CDSBus* m_pTailBus;

	BOOL m_bAllocateLoad;

	//Ϊ�������ж���
	CDSFeeder*	m_pMapFeeder;

};


class CDSFeeder : public CDblTermDSDev
{
private:

public:
	
	int m_nOperStatus;

	int m_nIndex;

	//modify by lipeng 20110524
	//BOOL m_bIsCB;
	int		m_feedType;		//-1:δȷ������;0:����;1:����;2:��բ;3:����ѹ��;(4:��������ʱ������һ��);

	CDSFeeder(char *strDevName,  int nObjId, 
		CDSNode *pNode1, CDSNode *pNode2);
	~CDSFeeder() {}
	
	CDSNode *GetFarNode(CDSNode *pNode);

	CDSLine* m_pMapedLine;


	// ��·��
	int m_nGroupId, m_nIndivId; // group and individual identification
	void SetGroupId(int nGroupId) { m_nGroupId = nGroupId; }
	void SetIndivId(int nIndivId) { m_nIndivId = nIndivId; }
	int GetGroupId() { return m_nGroupId; }
	int GetIndivId() { return m_nIndivId; }

	// ��·����״̬
	enum { DSF_OPEN, DSF_CLOSE };
	int GetOperStatus() { return m_nOperStatus; }
	void SetOperStatus(int nOperStatus) { m_nOperStatus = nOperStatus; }
	
	// Since we don't know the direction of feeder before the WFS, we 
	// must revise this flag during topological analysis.
	enum { DSF_POSITIVE_DIR = 1, DSF_NEGATIVE_DIR = -1 };

	int m_nDirFlag; // ��·������ 1 or -1
	int GetDirectionalFlag() { return m_nDirFlag; }
	void SetDirectionalFlag(int nDirFlag) { m_nDirFlag = nDirFlag; }

	uint	MeshFlag;		//������־
	uint	GetMeshFlag(){return MeshFlag;}
	void	SetMeshFlag(uint nMeshFlag){MeshFlag = nMeshFlag;}

};


// CDSBus is a abstract concept for analysis of distrubition system.

class CDSBus 
{
private:
	int m_nLayerId;
	
public:
	CDSBus* m_pNextSibling;
	CDSBus* m_pPrevSibling;
	CDSBus* m_pParent;
	CDSBus* m_pFirstChild;

	CDSBus* m_pPrevBus; // in WFS
	CDSBus* m_pNextBus; // 

	CDSLine* m_pLine; // this line connects its parent and itself

	CDSNode* m_pOrigNode;

	CDSBus* m_pHeadBus;
	CDSBus* GetHeadBus() { Q_ASSERT(m_pHeadBus != NULL); return m_pHeadBus; }
	CDSBus* m_pTailBus;
	CDSBus* GetTailBus() { Q_ASSERT(m_pHeadBus != NULL); return m_pHeadBus->m_pTailBus; }

	
	CDSBus(CDSBus *pHeadBus = NULL, int nLayerId = 1);
	~CDSBus() {}

	int GetLayerId() { return m_nLayerId; }

};
	
// CDSLine ��(���ڼ���)
// 

class CDSLine : public CDSObject
{
private:
	CDSBus* m_pStartBus; // ��ĩĸ��
	CDSBus* m_pEndBus;
	
	int m_nNumOfNodes; // ���߶��������Ľڵ���Ŀ

public:
	CDSLine(CDSBus* pStartBus, CDSBus* pEndBus);
	~CDSLine() {}
	
	// ��ȡ��ĩĸ��
	CDSBus* GetStartBus() {	Q_ASSERT(m_pStartBus != NULL); return m_pStartBus; }
	CDSBus* GetEndBus() { Q_ASSERT(m_pEndBus != NULL); return m_pEndBus; }


	// ��ȡ���߶��������Ľڵ���Ŀ
	int GetNumOfNodes() { return m_nNumOfNodes; }
	// ���ø��߶��������Ľڵ���Ŀ
	void SetNumOfNodes(int nNumOfNodes) { m_nNumOfNodes = nNumOfNodes; }

	CDSOutlet *m_pOutlet; // ����·���ڵĳ���
	void SetOutlet(CDSOutlet *pOutlet) // ���ø���·���ڵĳ���
	{ Q_ASSERT(pOutlet != NULL); m_pOutlet = pOutlet; }
	CDSOutlet *GetOutlet() // ��ȡ����·���ڵĳ���
	{ Q_ASSERT(m_pOutlet != NULL); return m_pOutlet; } 

	/*CDSNode **GetNodeArray(); // ��ȡ�ڵ�����*/

	//feeder�б�
	QList<CDSFeeder*>		m_pFeederArray;
	void		AddFeeder(CDSFeeder*  pFeeder);
	CDSFeeder **GetFeederArray(); // ��ȡ��������

};

////////////////////////////////////////////////////////////

//ĸ�ߺͽڵ�Ķ��ձ�
typedef struct  
{
	char	BusName[CODE_LEN];
	char	NodeName[CODE_LEN];

	//char*	GetBusName(char* nodeName);
}BUS2NODE;

class CDS  
{

public:
	char					m_beginDevName[CODE_LEN];	//��ʼ�豸����
	int						m_bInit;			//��ʼ����־

	//��ȡʵʱ����
	//Rdb_Tab				lsd;				//���ر�
	Rdb_SqlCli				m_rdbTbl;			//��������
	PTPRCD					m_tpRcd;			//ʵʱ�����˱�
	uint					m_scdtpnum;			//scdtp���¼��
	//////////////////////////////////////////////////////////

	// �����ǹ��ڽڵ㣬���ߣ��û����ɣ���Դ�㡢���ص�բ�����͵������Ķ�ָ̬������
	uint nNumOfNds, nNumOfSgs, nNumOfLds, nNumOfGens, nNumOfCbs, nNumOfLns, nNumOfDisTrans,nNumOfCps;
	QList<CDSNode*>			NodeArray;
	QList<CDSFeeder*>			FeederArray;
	QList<CDSLoad*>			LoadArray;
	QList<CDSRootNode*>		RootArray;
	QList<CDSCapacitor*>		CPArray;
	
	QList<CDSFeeder*>			CBArray;
	QList<CDSFeeder*>			LnArray;
	QList<CDSFeeder*>			DisTransArray;

	uint						nNumOfBus;
	QList<BUS2NODE*>			Bus2NodeArray;
	char*						GetBusnameFromNode(char* nodename);
	char*						GetNodeFromBusname(char* busname);


	QList<CDSNode*>			BreakpointArray; // ���ڷ��ѵ�Ķ�ָ̬������(��������������/״̬����)
	QList<CDSFeeder*>			pmeshArray;		//��Դ�����豸�б�
	QList<CDSFeeder*>			h2hArray;		//�����ֻ����豸�б�


	// ������Ϣ�ļ�(���ڵ���/����)
	char  errormsg[LENGTH_ERRORMSG];
	QFile	m_ofsCore;
	QTextStream m_topostream;
	void PrintMsgToFile(char* strMsg) {m_topostream<<strMsg;}



public:
	CDS();
	virtual ~CDS();
	void			ClearMem();							//����ڴ�


	// ģ��1. ��ʼ��CDS 
	void			setBeginDevName(const char* devName);		//������ʼ�豸����
	int				InitDS();							//��ʼ��CDS
	int				InitBefDS( QMap<QString, int> &devActiveMap );							//��ʼ��CDS
	uint 			ReadTopo();							//��ȡ���˽ṹ
	uint			ReadBefTopo( QMap<QString, int> &devActiveMap );
	uint			PutRcdToCDS(PTPRCD  onescdrcd);		//�����˼�¼ת��CDS���˽ṹ
	CDSNode*		IsNodeExist(char*	nodeName);		//�Ƿ���nodearray�д������нڵ�
	char*			GetBusNameOfNode(char* nodename);	//���ݽڵ���ȡ��Ӧĸ������
	// ϵͳ����״̬ ������/������
	enum { DS_RADIAL, DS_WEAKLY_MESH };

	// ģ��2.  ����բ����״̬
	void			ReadCBStatus();						//��ȡң��״̬
	void			ReadCBBefStatus( QMap<QString, int> &devStatusMap );						//��ȡң��״̬

	// ģ��3. ���˷���
	void			TopologicalAnalysis();
	// ����������
	void AnalysisOfWeaklyMeshedNetwork();
	// �����������
	CDSNode *WidthFirstSearch(CDSNode *pNode, CDSOutlet* pOutlet);
	// ��·�㱨
	CDSNode* LoopReport(CDSNode *pNode1, CDSNode *pNode2);
	// ����������������ڵ�
	void			LinkTwoNodes(CDSNode *pNode1, CDSNode *pNode2, CDSFeeder *pFeeder);
	// ����˫�����������ڵ�����ĸ��
	void LinkTwoBuses(CDSBus* pPrevBus, CDSBus* pNextBus);
	// ����ĸ�ߺͶ�Ӧ�Ľڵ�
	void LinkBusAndNode(CDSBus *pBus, CDSNode *pNode);
	// �ڵ����
	void SplitNode(CDSNode *pNode, CDSNode *pSplittedNode);
	// ɾ��֧·
	void			RemoveFeeder(CDSFeeder *pObsoleteFeeder);
	// �ж�ָ���Ľڵ��Ƿ�Ϊ��Դ��
	BOOL IsRootNode(CDSNode *pNode);


	// ģ��6. ������ͳ��
	// ���������������Ϣ
	void WriteFnlRpt();
	//��������ɫ��Ϣ�����ʵʱ��scdtp
	void UpdateScdtpRcd();
	// ��������ļ�(���ڵ���)
	void WriteCoreFile();


	//·����ѯ����
	int GetPowerPathFromDev(const char *devName, char **devList);			//-1:ʧ��;-2:�����ֻ���
	uint GetLoadPathFromDev(const char *devName, uint	devType,char **devList );
	void GetNodeDev(CDSNode* pNode,char**  devlist,uint*  count);			//ȡ�ڵ��ϵ����豸
	void GetDSLineDev(CDSLine* pLine,char**  devlist,uint*  count);			//ȡLine�ϵ��豸�б�
	void GetChildFeedersOfLineFromFeeder(CDSFeeder*  begFeeder,char** pDevList,uint* devCnt);	//ȡfeder����line��feeder�����豸
	void GetParentFeedersOfLineFromFeeder(CDSFeeder*  begFeeder,char** pDevList,uint* devCnt);	//ȡfeder����line��feeder�ĸ��豸

};

// the return feeder may be null if some nodes are merged between the pNode1 & pNode2
inline CDSFeeder* GetDSFeeder(CDSNode *pNode1, CDSNode *pNode2)
{  Q_ASSERT(pNode1 != NULL && pNode2 != NULL);  return pNode1->GetLinkedFeeder(pNode2); }
// �ж�ָ���������ڵ��Ƿ�Ϊͬһ���ڵ�
inline BOOL IsSameNode(CDSNode *pNode1, CDSNode *pNode2)
{  Q_ASSERT(pNode1 != NULL && pNode2 != NULL); return (pNode1->GetObjectId() == pNode2->GetObjectId()); }
// �ж�ָ��������ĸ���Ƿ�Ϊͬһ��ĸ��
inline BOOL IsSameBus(CDSBus *pBus1, CDSBus *pBus2)
{  Q_ASSERT(pBus1 != NULL && pBus2 != NULL); return IsSameNode(pBus1->m_pOrigNode, pBus2->m_pOrigNode); }
inline BOOL IsSameFeeder(CDSFeeder *pFeeder1, CDSFeeder *pFeeder2)
{  Q_ASSERT(pFeeder1 != NULL && pFeeder2 != NULL); return (pFeeder1->GetObjectId() == pFeeder2->GetObjectId()); }
// �ж�ָ����������·�Ƿ�Ϊͬһ����·
inline BOOL IsSameLine(CDSLine *pLine1, CDSLine *pLine2)
{  
	Q_ASSERT(pLine1 != NULL && pLine2 != NULL);

	return IsSameBus(pLine1->GetStartBus(), pLine2->GetStartBus()) 
		&& IsSameBus(pLine1->GetEndBus(), pLine2->GetEndBus());
}

#endif // !defined(AFX_DISTSYS_H__8E83BC55_2F28_49D4_B5FC_F76589BE3500__INCLUDED_)
