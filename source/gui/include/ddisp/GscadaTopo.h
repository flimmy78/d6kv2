#ifndef _G_SCADATOPO_H
#define _G_SCADATOPO_H

#ifdef _WIN32
	#pragma warning(disable: 4251)
	#pragma warning(disable: 4275)
#endif

#include "rdb/rdbapi.h"
#include "GList.h"

#define CODE_LEN				 24
#define MAX_PORTNUM_DEV   6				//һ���豸�����������ӽڵ�
#define MAX_DEVNUM_POINT 60 			//һ���ڵ��Ͽ����ӱ���豸�������
#define MAX_MESH_DEV   100
#define MAX_MESH_NUM   30


//վ���豸
typedef struct
{
	char  devname[CODE_LEN];				//�豸����
	int   type;											//�豸����
	int   nodeno[MAX_PORTNUM_DEV];	//�׶˽ڵ��,ĩ�˽ڵ��	
																	//��ѹ��ڵ��,��ѹ��ڵ��,
																	//��ѹ��ڵ��,���Ե�ڵ�� etc.
	int   external_device;					//�������豸��һ��վ���豸
	unsigned char	  power_flag;			//�������
	char	  obligate[32];						//�����ֶΣ����ڰ

	//add by lipeng 
	int		MeshFlag;						//������־

} DEVINFO_INTERNAL;						

//վ���豸,վ��ֻ�ж˵��˵ĶԽ��豸,T����Ӧ������վ��һ���ִ����վ����·
typedef struct
{
	char devname[CODE_LEN];	//�豸����
	int	type;									//�豸����
	int	head_station;					//�׶˳�վ��
	int	head_node;						//�׶˽ڵ��	//��ѹ��ڵ��
	int	tail_station;					//ĩ�˳�վ��
	int	tail_node;						//ĩ�˽ڵ��	//��ѹ��ڵ��

	//add by lipeng 
	int		MeshFlag;						//������־

} DEVINFO_EXTERNAL;

//�ڵ�ҽ��豸�嵥
typedef struct _tagNODEINFO
{
	int   devnum;						//�豸����
	//add by lipeng
	int   stindex;						//��վ����
	///////////////////////
	int   nodeno;						//�ڵ����
	int   index[MAX_DEVNUM_POINT];	//�ڵ�ҽ��豸�±�
	int   port[MAX_DEVNUM_POINT];	//�ڵ�ҽ��豸�˿�
	unsigned char   is_bus;						//ʵ��ĸ�߱�־

	_tagNODEINFO()
	{
		devnum	= 0;
		is_bus  = 0;
		nodeno	= -1;
		stindex = -1;

		for( int i = 0 ; i < MAX_DEVNUM_POINT ; i ++ )
		{
			index[i] = -1;
			port[i]= -1;
		}
	}

} NODEINFO;

//����ʹ��
typedef struct 
{
	int stno;//��վ��
	int ndno;//�ڵ��
} NODEPOS;

typedef struct 
{
	int stno  ;//��վ��
	int index ;//�豸�±�
} DEVPOS;

//��վ��Ϣ
class  GStations
{
public:

	GStations()
	{
		memset(stname,0,sizeof(stname));
		m_pDevices	= NULL;
		m_pNodes	= NULL;
		m_deviceCnt = 0;
		m_nodeCnt	= 0;
		MAXNODENO	= 0;
		m_node2Indexs = NULL;
	}

	~GStations()
	{
		if(m_pDevices)		delete[] m_pDevices;
		if(m_pNodes)		delete[] m_pNodes ;
		if(m_node2Indexs)	delete [] m_node2Indexs;
	}

	char stname[CODE_LEN];			//��վ����

	DEVINFO_INTERNAL	*m_pDevices;		//վ���豸�嵥
	int		m_deviceCnt;					//վ���豸����

	NODEINFO*		  m_pNodes;			  	//�ڵ�ҽ��豸�嵥
	int*				m_node2Indexs; 		//��Ŷ�Ӧ�±�

	int m_nodeCnt;									//�ýڵ����ţ����֣����ɱ�ʾ
	int MAXNODENO;									//���ڵ����

	DEVINFO_INTERNAL* devices() { return m_pDevices; }
	DEVINFO_INTERNAL* device(int index) { if(index>m_deviceCnt) return NULL ; return &m_pDevices[index]; }

	NODEINFO* nodes() { return m_pNodes; }
	NODEINFO* node(int index) { if( index > m_nodeCnt) return NULL; return &m_pNodes[index]; }
	
	int nodeCnt() const { return m_nodeCnt; }
	int deviceCnt() const { return m_deviceCnt; }
	int no() const { return MAXNODENO; }
	unsigned char contains(int n) { if((m_node2Indexs==NULL)||(m_node2Indexs[n]==-1)) return 0; return 1; }
	int index(int n) { return m_node2Indexs[n]; }

	void  setNo(int n) { MAXNODENO = n ; }
};

//add by lipeng for topo
#define MAX_NODE_LINK_NUM	4
#define MAX_NODE_NUM		1000
#define	MAX_POWER_NUM		4
typedef struct devInfo
{
	char	devcode[CODE_LEN];
	int		stindex;					//��վ��Ϊ-1�Ĵ���վ����·
}DEVINFO;

typedef struct NodeLink
{
	NODEINFO					m_NodeInfo;
	NodeLink*					ParentNode[MAX_NODE_LINK_NUM];
	int							ParentNum;							//������΢�������κνڵ�ĸ��ڵ�ֻ��һ��
	NodeLink*					ChildNode[MAX_NODE_LINK_NUM];
	int							ChildNum;

	DEVINFO_INTERNAL			PowerSource[MAX_NODE_LINK_NUM];
	int							PowerNum;
	DEVINFO_INTERNAL			LoadInfo[MAX_NODE_LINK_NUM];
	int							LoadNum;
}NODELINK;

typedef struct TreeInfo
{
	int							m_IslandNo;						//���ĵ��ţ�һ��������һ����
	NODELINK					m_NodeLink[MAX_NODE_NUM];		//����Ľڵ�������1��ʼ
	int							m_NodeNum;						//����Ľڵ���Ŀ
	DEVINFO_INTERNAL			m_DevInfo[MAX_NODE_NUM];		//������豸�б�
	int							m_DevNum;						//������豸��Ŀ
	DEVINFO_INTERNAL			PowerInfo[MAX_POWER_NUM];		//���ĵ�Դ�б�
	int							PowerNum;						//���ĵ�Դ��Ŀ

	int							IsMeshFlag;						//������־
}TREEINFO;
////////////////////////////////////////////////////////
//add by zhangyun
//һ���жϳ����Ļ���
typedef struct  
{
	int meshIndex;
	int devNum;
	DEVINFO dev[MAX_MESH_DEV];
}MESH;

//һ�λ����жϵĽ���ļ���
typedef struct
{
	int meshNum;
	MESH mesh[MAX_MESH_NUM];
}MESHRESULT;
typedef struct  
	{
		//char dev[MAX_MESH_DEV][CODE_LEN];

		DEVINFO devBreakSwitch[MAX_MESH_DEV];
		char meshBSDevVal[MAX_MESH_NUM][MAX_MESH_DEV];

		DEVINFO devLine[MAX_MESH_DEV];
		char LineInMesh[MAX_MESH_NUM][MAX_MESH_DEV];

		int devBSNum;
		int devLineNum;
		int modeNum;
		uchar readMeshModeOK;
	}MESHMODE;
///////////////////////////////////////

//������ȫ����Ϣ
class GPowerNets
{
public:

	GPowerNets()
	{
		m_stationCnt	= 0 ;
		m_lineCnt  = 0 ;
		m_deviceCnt = 0;
		m_pStations = NULL;
		m_pLines = NULL;
		m_pDevices= NULL;
	}

	~GPowerNets()
	{
		if(m_lineCnt )		delete[] m_pLines;
		if(m_stationCnt )	delete[] m_pStations;
		if(m_deviceCnt)		delete [] m_pDevices;

		m_stationCnt = 0;
		m_lineCnt = 0;
		m_deviceCnt= 0;
	}
	
	GStations		 *m_pStations;		//��վ��Ϣ�嵥			
	int			  m_stationCnt;		//��վ����
	DEVINFO_EXTERNAL *m_pLines;			//վ����·�嵥
	int			  m_lineCnt;		//��·����	
	DEVPOS			 *m_pDevices;		//��ʼ�����豸�嵥
	int			  m_deviceCnt;		//�豸����
	//add by zhangyun
	MESHRESULT	  m_meshResult;		//�����жϵĽ��

	//add by lipeng for topo
	TREEINFO			*m_NodeTree;
	int					m_TreeNum;
	///////////////////////////////


	GStations*  station(int index) { if( index > m_stationCnt ) return NULL ; return &m_pStations[index]; }
	DEVINFO_EXTERNAL* line(int index) { if( index > m_lineCnt ) return NULL ; return &m_pLines[index]; }
	DEVPOS&	device(int index){ return m_pDevices[index]; }

	GStations* stations() const { return m_pStations; }
	DEVINFO_EXTERNAL* lines() const { return m_pLines; }
	DEVPOS* devices() const { return m_pDevices; }


	int		deviceCnt() const { return m_deviceCnt; }
	int		lineCnt() const { return m_lineCnt;     }
	int		stationCnt() const { return m_stationCnt; }

};

//����һ����GPowerNets���������׵Ľṹ,
typedef struct 
{
	int   *pLnIndex;	//��·�豸��ʵʱ��"scdtp"�е��±�
	unsigned char  **pLnState;	//�豸��ʵʱ��ķֺ�״̬
	int  **pDevIndex;	//��վ�е��豸��ʵʱ��"scdtp"�е��±�	
	unsigned char ***pDevState;	//�豸��ʵʱ��ķֺ�״̬
} Rdb_LOCATE_STRU;


class GscadaTopo
{
	typedef struct
	{
		char  name[CODE_LEN];
		unsigned char type;
		char  hstation[CODE_LEN];
		char  tstation[CODE_LEN];
		unsigned char vol1;
		unsigned char vol2;
		unsigned char vol3;
		unsigned char vol4;
		unsigned char vol5;
		unsigned char vol6;
		char  node1[CODE_LEN];
		char  node2[CODE_LEN];
		char  node3[CODE_LEN];
		char  node4[CODE_LEN];
		char  node5[CODE_LEN];
		char  node6[CODE_LEN];
		unsigned char power_flag;
		char  obligate[32];
		int busno1;
		int busno2;
		int busno3;
		int busno4;
		int busno5;
		int busno6;
		int islno1;
		int islno2;
		int islno3;
		int islno4;
		int islno5;
		int islno6;
		int active1;
		int active2;
		int active3;
		int active4;
		int active5;
		int active6;
		int active;

	}TPRCD,*PTPRCD;

public:

	GscadaTopo();
	~GscadaTopo();

	unsigned char initTP();
	void  clear();

	//��ĳ����ѹ�������
	void  runActiveAnalysis(int stno, int ndno, int nIslno);
	void  runActiveAnalysis(int lnno, int nIslno);

	//ȫ�ִ������
	void  runActiveAnalysis();

protected:

	//����������Ƿ����Խ�����豸�����������жϺ���
	unsigned char isActiveByVol(int stno, int devno);
	unsigned char isActiveBySt(int stno, int devno);

	//��ָ��������������ؽڵ�
	void  travelNode(NODEPOS pos, int nIslno,TREEINFO *TreeInfo);

	//����
	int			m_bInit;			//��ʼ���ķ�ʽ
	int			m_bMode;			//0 - ��ĳ����ѹ������� 
										//1 - ȫ�ִ������
	GPowerNets			m_nets;				//ȫ�����˽ṹ��Ϣ
	PTPRCD					m_tpRcd;			//ʵʱ�����˱�
	
public:

	int		  **m_pResult    ;	//�������˽��

	//add by lipeng for topo  
	GList<TREEINFO>				m_Tree;
	DEVINFO GetDevBetweenParent(NODELINK node,int*  DevNum,DEVINFO** singleDev);	//��ȡ�븸�ڵ�֮����豸,singleDev���Ǹ��ڵ��ϵĵ���Ԫ��
	int  GetDevBetweenChild(NODELINK node,DEVINFO** Dev);	//��ȡ���ӽڵ�֮����豸�б�
	int  GetDevAtNode(NODELINK node,DEVINFO** Dev);			//��ȡ�ڵ��ϵ��豸�б�
	int  GetPowerPath(char* devname,DEVINFO** path) ;			//��ȡ����Դ���·��,ע������·������ֻ�Ը�����Ч
	int  GetLoadPath(char* devname,DEVINFO** path) {return 1;};
	int	 GetPowerPathFromTree(DEVINFO* device,NODEPOS pos,DEVINFO* path,TREEINFO* queryTree);
	int  GetLoadPathOfLine(char* devname,DEVINFO** path);			//��ȡ���߹���·�������豸���µĹ��緶Χ,ע�����緶Χ����ֻ��������Ч
																	//����-1���������ڻ����ڣ�����-2���������ڶ��Դ����
	int  GetLoadPathOfBreaker(char* devname,DEVINFO** path);			//��ȡ�������������б�
	int	 GetLoadPathFromTree(DEVINFO* device,NODELINK node,DEVINFO** path,TREEINFO* queryTree);//���ȫ�Ǹ��ɣ��򷵻������豸��Ŀ��
	int	 JudgeDoublePower(char* devname,DEVINFO* path);		//�жϸ��豸�Ƿ���˫�˹���
	int GetPowerPathFromDev(const char *devName, char ***devList);
	int GetLoadPathFromDev(const char *devName, char ***devList);
	///////////////*/
	
	MESHMODE   m_meshMode;
	//add by zhangyun 
		//���ļ��ж�ȡ����ģʽ
	uchar readMeshMode();
	//���ļ��ж�ȡ����ģʽ�����߲���
	uchar readMeshModeLine();
		//�ж������Ƿ���ڻ���
	int MeshJudge();
	//�ж�һ���豸�Ƿ��ڻ�����
	int  JudgeMesh(char *devName);
	//����ĳ���豸���ڻ����������豸
	//int GetMeshFromDev(char* devName,DEVINFO_INTERNAL* meshDev){return 0;};
	int GetMeshFromDev(char* devName,DEVINFO* meshDev);
	//////////////////////////////////////////
};

class GstreamMsg
{
public:

	GstreamMsg(){};
	~GstreamMsg();

	unsigned char setStream(void (*func)(void *buf,int ord,int len,int num) = NULL);
	
	void stopStream();
	void readStream();

private:
	Rdb_QuickStream rdb_stream;

};

#endif
