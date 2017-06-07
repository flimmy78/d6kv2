#ifndef  _PDR_H
#define  _PDR_H

#include	"scadatime.h"
#include	"scd/cmddef.h"
#include    "scd/comnlink.h"
#include	"rdb/rdbapi.h"

#define		MAXPDRGROUPNUMS		128
#define		SNP_GROUPNO			0

///////////////////////////////////////////////////////////////////
//׷������������ԣ�
#define  ZY_SELSUBST_NUMS		6
#define  ZY_SELVOLT_NUMS		6

struct PDRGROUP_STRU
{
    char    groupname[OBJECTNAME_LEN];//����
    char    describe[OBJDESCRIBE_LEN];//����
	short   groupno;   //�������
    uchar   zygrpno;   //׷�����
	uchar   zymode;    //׷�䷽ʽ
    uchar   sampledensity;	//��������
    uchar   prepoints;		//�¹�ǰ����
    uchar   postpoints;		//�¹ʺ����
	uchar   zycondition;    //׷������
	char    selsubst[ZY_SELSUBST_NUMS][SUBSTNAME_LEN];//վ����ϵ
	short   selvolt[ZY_SELVOLT_NUMS];//��ѹ�ȼ�
	uchar   usef;    //ʹ�ñ�־
};


//׷��������ṹ(����ʷ���д洢ʱʹ�ã�һ���¹������׷������ʷ����ֻ��һ��׷��������¼)
struct SPdrDescribe	{
    char		breakername[OBJECTNAME_LEN];	// ׷�俪����
    char		breakername2[OBJECTNAME_LEN]; 
    int			timeflag;			// ʱ���־
	int			ymd;
	int		    hmsms;
	short		zygrpno;
    uchar		causetype;			// ׷��ԭ������(�¹��Զ�׷��1���˹��ֶ�����0)
    uchar		sampledensity;		// �������(��)
    uchar		preframenums;		// �¹�ǰ����
    uchar		postframenums;		// �¹ʼ��¹ʺ����
    char		cause[40];			// ԭ������(��������)	
};

//׷��������ݽṹ(����ʷ���ݿ��д洢ʱ��)
struct SPdrSaveData	{
    short	zygrpno;					// ׷�����
    short	frameno;					// ׷����(�¹�ǰΪ����, �¹ʺ�Ϊ0������)
    intertime	timeflag;				// ʱ���־
    char	objname[OBJECTNAME_LEN];	// ��׷�������
    float	value;						// ֵ(ң����Ϊ������ֵ, ң����Ϊ״ֵ̬)
    short	baseobjtype;				// ��׷���������
    uchar	qualityf;					// ������־
};

//////////////////////////////////////////////////////////////////////////////////
typedef struct {
	char		triggername[OBJECTNAME_LEN];
	intertime	trigtime;
	int			ymd;
	int		    hmsms;
	uchar		zygrpno;
    uchar		causetype;			// ׷��ԭ������(�¹��Զ�׷��1���˹��ֶ�����0)
    char		cause[40];			// ԭ������(��������)
	int			trigframeno;
	int			preframenums;
	int			bakframenums;
	int			lastframeno;
} PDR_SAVETASK;


class PdrSaveTaskMan {
	static ComnLinkTable<PDR_SAVETASK>		savetask;
public:
	PdrSaveTaskMan(void);
	~PdrSaveTaskMan(void);
	void AddPdrSaveTask(uchar zygrpno,int trigtime,char *triggername,
					SYS_CLOCK *entclock,int causetype,char *cause,
					int prefnums,int bakfnums);
	void ProcAllSaveTask(void);
};


///////////////////////////////////////////

#define  PDROBJ_TBLTYPE_ANALOG		0
#define  PDROBJ_TBLTYPE_BREAK		1
#define  PDROBJ_TBLTYPE_SWITCH		2

typedef struct {
	char	objname[OBJECTNAME_LEN];
	short	objtype;
	short	tabtype;
	int		tabrcdno;
} OBJDATA_PARA;

class ObjectDataPdr {
	static int opentabf;
	static Rdb_Tab	anatab,brktab,swtab;
	uchar		zygrpno;
	uchar	lockflag;
	int		pdrobjnums;
	short	maxframenums;
	short	curframeno;
	OBJDATA_PARA *objdatapara;
	float		 *objdata;
	uchar		 *quality;
	void  InitObjDataPdr(void);
public:
	ObjectDataPdr(void);
	~ObjectDataPdr(void);
	short	GetCurFrameNo(void){return(curframeno);}
	int		GetMaxFrameNums(void){return(maxframenums);}
	int		SetObjDataPara(uchar zygrpno,int maxfnums,int objnums,OBJDATA_PARA *inpara);
	void	FreshObjData(void);
	void	SavePdrData(PDR_SAVETASK *pstask,int savemode);
};

/////////////////////////////////////////////

typedef struct {
	SPdrDescribe  pdrdes;
	int			datanums;
	char		savefname[200];
} PDR_DBTASK;


class PdrDBTaskMan {
	static ComnLinkTable<PDR_DBTASK>		pdrdbtask;
	void SaveDataInDB(PDR_DBTASK *dbtaskp);
public:
	PdrDBTaskMan(void);
	~PdrDBTaskMan(void);
	void AddPdrDBTask(SPdrDescribe *pdrdesp,int datanums,char *sfname);
	void ProcAllDBTask(void);
};

////////////////////////////////////////////
typedef struct {
	uchar	availflag;
	uchar	initedflag;
	uchar	lockflag;
	uchar	trigflag;
	char    groupname[OBJECTNAME_LEN]; 
	int		sampdensity;
	int		preframenums;
	int		bakframenums;
	int		maxframenums;
	int		timecount;
	ObjectDataPdr *objpdr;
} PDRGROUP_MANINFO;


class PdrGroupMan {
	void SetPdrPara(PDRGROUP_STRU *spgi,int maxfnums);
public:
	PdrGroupMan(void);
	~PdrGroupMan(void);
	void ProcAllPdrGroup(void);
	void UpdAllPdrPara(void);
};


/////////////////////////////////////////

#endif
