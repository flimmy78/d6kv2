#if !defined(WEBSTRUCT)
#define WEBSTRUCT

#include "rdb/rdbapi.h"

#include "webconstant.h"
#include "message.h"
#include "wfa.h"


struct USEREVENT{
	unsigned char userright;//web�¼�������е�����
	int eventcount;//������������
	int excludeevent[256];//���ͺ�
	int ptr;//��ǰ�����ָ��
	SYS_EVENT sysevent[EVENT_NUM_MAX];
	struct USEREVENT *puserevent;
	USEREVENT()
	{
		userright = 0;//web�¼�������е�����
		eventcount = 0;//������������
		ptr = 0;
		memset(sysevent,0,sizeof(SYS_EVENT)*EVENT_NUM_MAX);
		puserevent = NULL;
	}
};

typedef struct _realcurve{	//ʵʱ���߽ṹ
	unsigned char curStructNo;		//���߽ṹ���
	unsigned char cur_type;			//��������
	unsigned char cur_no;			//���ߺ�
	char libName[CODE_LEN];//����
	char objName[CODE_LEN];//������
	char fieldName[CODE_LEN];//����
	
	int lastSendTime;//�ϴη�������ʱ��,����
	short savetime;//���̼��,�Ǵ����һ��������=0������
	char hisTable[64];//��ʷ�����
	char selfname[64];//selected field name
	char orderfname[64];//order field name
	char where[128];
	_realcurve()
	{
		memset(libName,0,CODE_LEN);
		memset(objName,0,CODE_LEN);
		memset(fieldName,0,CODE_LEN);
		memset(hisTable,0,64);
		memset(selfname,0,64);
		memset(orderfname,0,64);
		memset(where,0,128);
		lastSendTime = 0;
		savetime = 0;
	}
}REALCURVE;


//�̹߳���
typedef struct _threadmanage{
	time_t	invalidtime;				/*ƽ����ʱ*/
	time_t  starttime;					/*����ʱ�����*/
	char	validf;						/*-1,������Ч;-2,�����쳣*/
	char	runf;						/*�߳����б�־,1-->run,0-->stop*/
	/*             runf                         validf
	*��ʼ״̬       0							0
	*����̬			1							1
	*�����ж�		1							-1
	*�ͻ��쳣		1							-1
	*�߳̿�ʼֹͣ	0							-1
	*�߳�������Դ	0							-1
	*������Դ���	0							0
	*�óɳ�ʼ̬		0							0
	*/
	int	cardrcdchgnums;    /*�����ƿ�Ĵ���*/
	int sendfilenums;
	SOCKET		sockid;				/*�̶߳�Ӧ�Ŀͻ�socket id*/
	MESSAGE		*psnddata;			/*������/��������*/
	MESSAGE		*prcvdata;
	
	unsigned char		clienttype;		//�ͻ�������
	unsigned char        right;			//�û�Ȩ��
	USEREVENT   *puserevent;	//ָ���û����������
	int			eventptr;//�����ָ��
	#ifdef WIN32
		CRITICAL_SECTION critThread;//�ٽ��
	#endif
	int			streamcount;		//������Ŀ
	Rdb_QuickStream *prdb_quickstream;	//ע����
	MULTISTREAM *pmultistream;		//������
	
	int			realcurvecount;		//ʵʱ���ߵ���Ŀ,ָSOneCurve����Ŀ
	REALCURVE	*prealcurve;


	char		client_name[WEBNODENAMELEN];/*�ͻ�������*/
	char		client_addr[WEBNODENAMELEN];/*�ͻ���IP��ַ*/
	char reserved[8];//��������
	_threadmanage(){

		invalidtime = 0;
		starttime = 0;

		validf = 0;
		runf = 0;

		sockid = 0;
		psnddata = NULL;
		prcvdata = NULL;

		clienttype = 0;
		right = 0;
		puserevent = NULL;
		eventptr = 0;
		#ifdef WIN32
				InitializeCriticalSection(&critThread);
		#endif

		streamcount = 0;
		prealcurve = NULL;
		prdb_quickstream = NULL;
		pmultistream = NULL;

		realcurvecount = 0;
		memset(client_name,0,WEBNODENAMELEN);
		memset(client_addr,0,WEBNODENAMELEN);
	}
	~_threadmanage()
	{
		#ifdef WIN32
				DeleteCriticalSection(&critThread);
		#endif
	}

}THREADMANAGE;



typedef struct _devparatable{
	int count;//��¼����
	int devtype[DEVTYPENUM];//�豸����
	NAME paratable[DEVTYPENUM];//��������
	NAME showfield[DEVTYPENUM];//��ʾ����
	_devparatable()
	{
		memset(devtype,0,sizeof(int)*DEVTYPENUM);
		memset(paratable,0,sizeof(NAME)*DEVTYPENUM);
		memset(showfield,0,sizeof(NAME)*DEVTYPENUM);
	}
}DEVPARATABLE;

#endif // !defined(WEBSTRUCT)
