#ifndef   _SCADA_PROC_H
#define   _SCADA_PROC_H
#include  <string.h>

#include    "net/netdef.h"
#include	"rdb/rdbapi.h"
#include	"scd/cmddef.h"
#include    "scd/comnlink.h"
#include	"scadatime.h"
#include    "scadaservice.h"
#include "fes/femailstruct.h"


#define MyUserName	"scada" 
#define MyPassWord	"scada" 

#define	MAX_CHGDATANUM		8192		//�仯������󳤶�


//////////////////////////////////////////////////////////////////////////////////////
Rdb_Tab  *GetSubstationRdbTab(void);
Rdb_Tab  *GetAnalogRdbTab(void);
Rdb_Tab  *GetBreakRdbTab(void);
Rdb_Tab  *GetSwitchRdbTab(void);
Rdb_Tab  *GetProtsigRdbTab(void);
Rdb_Tab  *GetStatusRdbTab(void);
Rdb_Tab  *GetDmsStatusRdbTab(void);
Rdb_Tab  *GetKwhRdbTab(void);

///////////////////////////////  ��ʱ��    ///////////////////////////////////////////

#define  SCDTIMER_INTERV            1
#define  SCDTIMER_ACCURATE          2

// ��ʱ������ָ�붨��
typedef void (*SCDTIMER_CALLBACKPROC)(void);

typedef struct {
	int     time;
	uchar   mode;
	SCDTIMER_CALLBACKPROC   callfunc;
}  SCDTIMER_STRU;


class 
#ifdef WIN32
 _SCD_DLLPORT 
#endif
ScdTimer  {
	static ComnLinkTable<SCDTIMER_STRU>		timertask;
public:
	void SetScdTimer(uchar mode,int time,SCDTIMER_CALLBACKPROC  callfunc);
	void ProcTimerTask(void);
};

/////////////////////////////////////




#define  DAY_TIME_ADD(totaltime,inctime)   ((totaltime+inctime)>=(24*3600-8) ? (24*3600):(totaltime+inctime))


#ifdef WIN32
_SCD_DLLPORT 
#endif
int DataAccord(int groupno,char *tabname);


////////////////////////////////////////////////////////////
//////////// ����ע���  ///////////////////////////////////
////////////////////////////////////////////////////////////

typedef struct {
	char	name[OBJECTNAME_LEN];
	char	describe[OBJDESCRIBE_LEN];
	int		intval;
	float   floatval;
	char    stringval[OBJECTNAME_LEN];
}  PROGAMREG_STRU;

#ifdef WIN32
_SCD_DLLPORT 
#endif
PROGAMREG_STRU	*GetProgramRunPara(char *name);


#ifdef WIN32
_SCD_DLLPORT 
#endif
int					GetProgramRunPara(char *name,int defaultval);

#ifdef WIN32
_SCD_DLLPORT 
#endif
float				GetProgramRunPara(char *name,float defaultval);

//////////////////////////////////////////////////////////
/////////// ʱ�δ��� /////////////////////////////////////
//////////////////////////////////////////////////////////

#define  SCDSDTYPE_ALL		   0
#define  SCDSDTYPE_MONDAY      1
#define  SCDSDTYPE_TUESDAY     2
#define  SCDSDTYPE_WEDNESDAY   3
#define  SCDSDTYPE_THURSDAY    4
#define  SCDSDTYPE_FRIDAY      5
#define  SCDSDTYPE_SATURDAY    6
#define  SCDSDTYPE_SUNDAY      7
#define  SCDSDTYPE_WORKDAY     8
#define  SCDSDTYPE_HOLIDAY     9

/// ʱ�����Ͷ���ṹ 
typedef struct {
   ushort   district;		// ʱ����(����)
   ushort   order;			// ���
   uchar    stmonth;		// ��ʼ��
   uchar    stday;			// ��ʼ��
   uchar    endmonth;		// ��ֹ��
   uchar    endday;			// ��ֹ��
   uchar    type;			// ����
   ushort   daygroupno;	    // ��ʱ����
} SCDSDTYPE_STRU;

typedef struct {
   ushort   daygroupno;	// ��ʱ����
   uchar    sthour;		// ��ʼʱ
   uchar    stminute;	// ��ʼ��
   uchar    endhour;	// ��ֹʱ
   uchar    endminute;	// ��ֹ��
   uchar    timetype;	// ʱ������
} SCDDAYSDTYPE_STRU;
 



class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ScdTimePeriodType {
   static  int      opentabf;
   static  Rdb_Tab  scdtimetab;
   static  Rdb_Tab  dayscdtimetab;
   static  Rdb_Tab  timeparayeartab;
   static  Rdb_Tab  timeparaminutetab;
   static  ushort  *timeparayearp;
   static  uchar   *timeparaminutep;
   static  int		maxdistrictnums;
   static  int		maxdaygroupnums;
public:
   ScdTimePeriodType(void);
   int  UpdateTimePeriodParaYearTab(void);
   int  UpdateTimePeriodParaMinuteTab(void);

#if defined(WIN32)
	friend  int  _SCD_DLLPORT  UpdateTimePeriodParaTab(void);
#else
	friend  int  UpdateTimePeriodParaTab(void);
#endif

#if defined(WIN32)
	friend  int  _SCD_DLLPORT  GetTimePeriodType(ushort district,ushort year,uchar month,uchar day,uchar hour,uchar minute);
#else
	friend  int  GetTimePeriodType(ushort district,ushort year,uchar month,uchar day,uchar hour,uchar minute);
#endif

#if defined(WIN32)
	friend  int  _SCD_DLLPORT  GetCurrentTimePeriodType(ushort district=0);
#else
	friend  int  GetCurrentTimePeriodType(ushort district=0);
#endif

#if defined(WIN32)
	friend  void  _SCD_DLLPORT  FreshCurrentTimePeriodType(void);
#else
	friend  void  FreshCurrentTimePeriodType(void);
#endif
};


//////////////////////////////////////////////////////// 
///////////// �¼����ƽṹ  ////////////////////////////
////////////////////////////////////////////////////////

typedef struct {
		uchar  puteventf;		// 
		int    lasteventno;		//
		uchar  putapeventf;		//
		int    lastapeventno;	//
		uchar  putykquef;		//
		uchar  putrlycmdf;		//
		uchar  addstudentf;		//
} PROG_SHARECTRL_STRU;  

PROG_SHARECTRL_STRU  *GetShareCtrlAddr(void);


//////////////////////////////////////////////////
// FEP������ͨ���¼� 
typedef struct {
	SYS_CLOCK	entclock;
	uchar		type;
	int		    channo;
	uchar		status;
	uchar       relclasstype;
	char		relname[OBJECTNAME_LEN];
	char		reldescribe[OBJDESCRIBE_LEN];
}  ENT_FORECHANENT_STRU;

// ����װ�����ݿ��  
typedef struct {
	char	hostname[HOSTNAME_LEN];
	char	username[USERNAME_LEN];
	char	mtabname[TABNAME_LEN];
} ENT_REQLOADTAB_STRU;

// ���ݿ����޸�
typedef struct {
	char			hostname[HOSTNAME_LEN];
	char			username[USERNAME_LEN];
	char			tabname[TABNAME_LEN];
	char			objectname[OBJECTNAME_LEN];
	char			fieldname[FLDNAME_LEN];
	uchar			datatype;
	VTVALUE_UNION	putvalue;
} ENT_DBPUTFLD_STRU;

// ���ݿ���仯    
typedef struct {
	char			tabname[TABNAME_LEN];
	char			objectname[OBJECTNAME_LEN];
	char			fieldname[FLDNAME_LEN];
	uchar			datatype;
	VTVALUE_UNION	putvalue;
} ENT_FLDCHG_STRU;

// �˹�����    
typedef struct {
	char			hostname[HOSTNAME_LEN];
	char			username[USERNAME_LEN];
	char			tabname[TABNAME_LEN];
	char			objectname[OBJECTNAME_LEN];
    uchar           securegrade;
	uchar			setflag;
	uchar			datatype;
	double			setvalue;
} ENT_MANSET_STRU;

// ���    
typedef struct {
	char			tabname[TABNAME_LEN];
	char			objectname[OBJECTNAME_LEN];
    uchar           securegrade;
	uchar			setflag;
} ENT_WFBS_STRU;

// Խ�� 
typedef struct {
	char	tabname[TABNAME_LEN];
	char	objectname[OBJECTNAME_LEN];
	uchar	olgrade;
	float	olvalue;
} ENT_OVERLIMIT_STRU;


#define  SATATUSCHG_BY_FORE			0	// ��ǰ������,�ɲ�������
#define  SATATUSCHG_BY_SCADA		1	// ��SCADA����,�ɲ��������˫ң�š���ң�ŵ�

#define  SATATUSCHG_MAKEENT_YN		10	// ���������벻�����������͵ķֽ�

#define  SATATUSCHG_SCADA_INSIDE	10	// ��SCADAͨ��ȫ���ݱȽ�����,��Ϊ�¼����䣬����������
#define  SATATUSCHG_SCADA_MANSET	11	// ��SCADAͨ��ȫ���ݱȽ�����,��Ϊ�¼����䣬����������

// ״̬���仯    
typedef struct {
	int         groupno;
	char		tabname[TABNAME_LEN];
	char		objectname[OBJECTNAME_LEN];
	char		substation[SUBSTNAME_LEN];
	SYS_CLOCK	chgclock;
	uchar		oddflag;
	uchar		sgzflag;
	short		staval;
	float       pdata;
	float       qdata;
	float       idata;
	short		zygroupno;
	uchar       maketype;
	uchar       testflag;
} ENT_STATUSCHG_STRU;

// ң��
typedef struct {
	int         groupno;
	char		tabname[TABNAME_LEN];
	char		objectname[OBJECTNAME_LEN];
	int			securegrade;
	uchar		actype;
	uchar		programykf;
	uchar		procsercode;
	char		ophostname[HOSTNAME_LEN];
	char		operatorname[USERNAME_LEN];
	uhlong		optaskid;
	char		confirmhost[HOSTNAME_LEN];
	char		confirmname[USERNAME_LEN];
	uhlong		confirmtaskid;
	char		entname[ENTNAME_LEN];
} ENT_YK_STRU;

// ң������
typedef struct {
	uchar		ykcmd;
	uchar		ykfangshi;
	int         ykgroupno;            //add
	int			ykrtuno;
	int			ykno;
	int			ykaction;
} ENT_YKCMD_STRU;


// ң��
typedef struct {
	short       groupno;
	char		objectname[OBJECTNAME_LEN];
	int			securegrade;
	uchar		outtype;
	short		terminalno;
	short		ytno;
	int			ytsercode;
	int			failcode;
	float		setvalue;
	float		curvalue;
	int			outvalue;
	char		ophostname[HOSTNAME_LEN];
	char		operatorname[USERNAME_LEN];
	uhlong		optaskid;
	char		entname[ENTNAME_LEN];
} ENT_YT_STRU;

    
// ����
typedef struct {
	SYS_CLOCK	entclock;
	char		breakname[OBJECTNAME_LEN];
	uchar		staval;
	uchar		sgf;
	float       pdata;
	float       qdata;
	float       idata;
	uchar       relclasstype;
	char		relname[OBJECTNAME_LEN];
	char		reldescribe[OBJDESCRIBE_LEN];
	uchar       testflag;
} ENT_BREAK_STRU;

// ���÷��ű�־
typedef struct {
	char		hostname[HOSTNAME_LEN];
	char		username[USERNAME_LEN];
	uchar		setflag;
	CARD_RCD_STRU	cardrcd;
} ENT_SETSPECIALFLAG_STRU;		// ���÷��ű�־����/��ӵط���Σ�շ������޷���

typedef struct {
	char	hostname[HOSTNAME_LEN];
    char	username[USERNAME_LEN];
	char	breakname[OBJECTNAME_LEN];
	char	break_replaced[OBJECTNAME_LEN];
	char	mode;
}ENT_LINEREPLACE_STRU;

/*
typedef struct
{
	SYS_CLOCK eclock;
	int  faulttype;
	char startbreakname[OBJECTNAME_LEN];
	char startftuname[OBJECTNAME_LEN];
	char endbreakname[OBJECTNAME_LEN];
	char endftuname[OBJECTNAME_LEN];
}ENT_DADIAGNOSE_STRU;
*/

typedef struct
{
	SYS_CLOCK clock;
	intertime timeflag;
	char      breakname[OBJECTNAME_LEN];
	uchar     zygrpno;
	uchar     causetype;
	char      cause[OBJDESCRIBE_LEN];
    uchar	  preframenums;		// �¹�ǰ����
    uchar	  postframenums;	// �¹ʼ��¹ʺ����
}ENT_STARTPDR_STRU;



typedef struct
{
	int       groupno;
	char	  ophostname[HOSTNAME_LEN];
	char	  operatorname[USERNAME_LEN];
	hlong	  optaskid;
	char	  tabname[40];
}ENT_CCFORMULA_STRU;



// Ӧ���¼��ṹ����    
typedef struct {
	int			apentcode;
	SYS_CLOCK	apentclock;
	short		apentmsglen;
	char		apentmsg[MAXAPENTMSG_LEN];
} EVENT_STRU;

// ����ָ�붨��
typedef int (*APENT_CALLBACKPROC)(int apentcode,SYS_CLOCK *,void *,void *);

// Ӧ���¼����Ƚṹ����
typedef struct apentdispatch_stru {
	int							apentcode;
	APENT_CALLBACKPROC			callbackproc;
	void						*argp;
	struct apentdispatch_stru	*next;
} ENT_DISPATCH_STRU;



// Ӧ���¼������ඨ��
class  
#if defined(WIN32)
_SCD_DLLPORT 
#endif
APEvent: public NetDbg {
		static PROG_SHARECTRL_STRU	*apentquectrl;		// �¼����ƽṹָ��
		static EVENT_STRU			*apeventaddr;		// Ӧ���¼����׵�ַ
		static int				     maxapentnums;		// ���Ӧ���¼�����
		APENT_CALLBACKPROC		     noentcallback;		// ���¼�ʱ�Ĵ�����
		void					    *noentcallbackargp;	// ���¼�ʱ�Ĵ������Ĳ���ָ��
		ENT_DISPATCH_STRU			*apentdproot;		// �ַ��¼�����������ָ��
		int		curapeventno;						// ��ǰ���¼���
		// ץ�����Ȩ
		int		GrapPutRight(void);				
		// �ͷſ���Ȩ
		void	ReleasePutRight(void);
		// �����¼�������
        void	AddApEventProc(int selcode,APENT_CALLBACKPROC selproc,void *argp);
		// �����¼�������
        int		DispatchApEvent(int apentcode,EVENT_STRU *apentp);
     public:
        APEvent(void);
        ~APEvent(void);
		// ����Ӧ���¼��ص�����
        int		AddApEventCallback(int selcode,APENT_CALLBACKPROC selproc,void *argp,int selcode1,...);
		// ����һ��Ӧ���¼�(�¼����룬�¼����ݣ��¼����ݳ���)
        int		AddApEvent(int apentcode,void *apentmsg,int len);
		// ������ȡ�¼�
        EVENT_STRU	*apentstep(void);
		// �¼�ѭ��
        int		ApEventLoop(int noentretf=0);
		// �¼�ѭ��
        int		ApEventLoop(int *apentcode,SYS_CLOCK *apentclock,void *apmsg);
		// ָ�����µ�ǰ�¼���
        void	ResetApEvent(void);
     };
     
/////////////////////////////////////////////////////////////////
// ���������¹�׷��

void  RequestStartPDR(SYS_CLOCK *clockp,intertime timeflag,char *breakname,
					  short groupno,uchar causetype,char *cause,uchar prefnums,uchar postfnums);


//////////////////////////////////////////////////////////////////

intertime 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
GetSysLastTime(void);


void	  
#if defined(WIN32)
_SCD_DLLPORT 
#endif
FreshSysLastTime(void);


/////////////////////////// ����״̬�� //////////////////////////
typedef struct {
	short   groupno;      //�������
	char	name[24];     //����
	char	describe[40]; //����
	char	hoststatus;   //����״̬
	char	netastatus;   //����A״̬
	char	netbstatus;   //����B״̬
	char	datastream;   //������
	char	ctrlstream;   //������
	char	netaused;     //����Aʹ��
	char	netbused;     //����Bʹ��
	float	cpuload;      //CPU����
	float	diskused;     //RAMʹ����
	float   netload;      //NET����
	float   ramload;      //����ʹ��
} HOSTSTATUS_STRU;

///////////////////////////  ��վ�� /////////////////////////////
typedef struct {
	char	name[SUBSTNAME_LEN];       //����
	char	describe[OBJDESCRIBE_LEN]; //����
//	uchar	type;
//	short	stationno;
	uchar	sgzdeadband;               //�¹�������
	uchar	status;                    //״̬
	short	saferunstartyear;          //��ȫ��ʼ��
	short   saferunstartmonth;         //��ȫ��ʼ��
	short   saferunstartday;           //��ȫ��ʼ��
	short	saferundays;               //��ȫ��������
//	short   ycinconsist_entmode;
//	short   yxinconsist_entmode;
//	short   deaddata_entmode;
	float	deadscale;                 //��������
#ifdef QTGRAPH
	char    graphname[255];//����ͼ��
#else
	char    graphname[OBJDESCRIBE_LEN];//����ͼ��
#endif
	short   groupno;                   //�������

	uchar	overlimitf;             //YCԽ��
	uchar   ycinconsistf;           //YC����
	uchar   yxinconsistf;           //YX����
	uchar   deadflag;               //����
	uchar   chanerror;              //ͨ������
	uchar	terminalerror;          //�ն˹���
	uchar	invalidf;               //YCֵ�Ƿ�

	uchar   oddstate;              //�쳣�źű�
	uchar   sgzstatus;             //�¹����źű�
	uchar   hsgstate;              //�¹��źű�
	uchar   hygstate;              //Ԥ���źű�
	uchar   hpsstate;              //�����źű�
	uchar   reserved;

	uchar   calycflag;               //
	uchar	calyxflag;               //
} SUBSTATION_STRU;
   

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Substation {
		static int       opentabf;
		static Rdb_Tab  substationtab;
	public:
		Substation(void);
		SUBSTATION_STRU  *GetSubStPara(char *substname);
        int  CalSafeDays(void);
		void SetCalYCFlag(char *substname);
		void SetCalYXFlag(char *substname);
		void SetSgFlag(char *substname);
		void SubStManConfirm(char *substname);
		int  CalSubStYCState(void);
		int  CalSubStYXState(void);
};

/************************   �ն˱�   *******************************/
typedef struct {
	short		terminalno;                   //�ն����
	char		name[OBJECTNAME_LEN];         //����
	char		describe[OBJDESCRIBE_LLEN];    //����
	uchar		useflag;                      //ʹ�ñ�־
	uchar		status;                       //״̬
	short		ycnums;                       //ң�����
	short		yxnums;                       //ң�Ÿ���
	short		kwhnums;                      //��ȸ���
    intertime	lastproct;                    //�ϴδ���ʱ��
    intertime	dayruntime;                   //������ʱ��
    intertime	totalruntime;                 //����ʱ��
	ushort		deadf;						  //������־
	short       runchanno;                  //ʵʱͨ����
	int     	breaktimes;	//��ͣ�˴���
} TERMINAL_STRU;
  

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Terminal {
		static	int			opentabf;
		static	Rdb_Tab	    terminaltab;
	public:
		Terminal(void);
        void       CalRunTime();
        void       DayInit();   
        TERMINAL_STRU  *GetTerminalPara(int terminalno);
	    int        GetName(short terminalno,char *name);
	    int        GetNameDes(short terminalno,char *name,char *des);
	    int        EnShareYK(int terminalno);
};

/************************ FTU �� *******************************/
/*
typedef struct {
	char	name[OBJECTNAME_LEN];
	char	describe[OBJDESCRIBE_LEN];
	short	ftuno; 			
	short	terminalno;
	short	channo;
	uchar	status;
	short	ycnums;
	short	yxnums;
	short	kwhnums;
    intertime  lastproct;
    int     dayruntime; //����ʱ�� ����
    int     totalruntime;    //�ۼ�����ʱ��
} FTU_STRU;
   
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Ftu {
		static	int			opentabf;
		static	Rdb_Tab	ftutab;
	public:
		Ftu(void);
        void CalRunTime();
        void DayInit();
		int  GetName(int,int,char*);
};
*/


#define		FDKYCNUMS	16
typedef struct {
	char	name[OBJECTNAME_LEN];
    char	describe[OBJDESCRIBE_LEN];
	short	terminalno; 	
	short	fdkno;
	char	ycname[FDKYCNUMS][OBJECTNAME_LEN];
	} FDK_MODULE_STRU;

/************************ ͨ���� *******************************/
typedef struct {
    short		channelno;      //ͨ�����
	char		name[OBJECTNAME_LEN];//����
	char		describe[OBJDESCRIBE_LLEN];//����
	uchar  	 	useflag;		//ʹ��״̬
	short   	rtuno;       //�ն����
	uchar		status;			//״ֵ̬
	char		mainbak;		//����״̬	
	float 		rateCode;		//������
	int		    lastswitchtime;	//�ϴ��л�ʱ��
    int     	breaktimes;		//�չ��ϴ���
    int  	    lastproct;      //�ϴδ���ʱ��
    int     	dayruntime;     //������ʱ��
    int     	totalruntime;   //�ۼ�����ʱ��
	char		hostname[OBJECTNAME_LEN];//�ڵ���
	uchar       grouppriority;   //�������ȼ�
	int		  lastsynctime;			///<�ϴζ�ʱ�ɹ�ʱ��
	float		recvcount;		///<ͨ�������ֽ���
	float		sendcount;		///<ͨ�������ֽ���
	unsigned char		synctimesuccess;	///<��ʱ�ɹ���־	
} CHANNEL_STRU;
 

typedef struct {
	short       rtu_no;
	short		channel_no;		//��ͨ�����
    char		chname[OBJECTNAME_LEN];//��ͨ������
    short      grouppriority;   //�������ȼ�
	short		last_channel_no;		//�ϴ�������ͨ�����
} SCDTERMINALDATA;


typedef struct
{
	SCDTERMINALDATA terminaldata[MAX_RTU_NUM];
	//SCDCHANDATA	chandata[MAX_CHAN_NUM];
}SCADACHANPARA;

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Channel {
		static int       opentabf;
		static Rdb_Tab   channeltab;
	public:
		Channel(void);
		friend int     GetChanName(int channelno,char *channelname,char *chandes);
		CHANNEL_STRU  *GetChanPara(int channo);
        void           CalRunTime();
        void           DayInit();
		int            ProcChanData(CHANNEL_STRU *,float ,char );
};

/*
#define  YCOLSAVEMODE_ALLSAVE		0  // ȫ�����
#define  YCOLSAVEMODE_NROLUNSAVE	1  // ��ͨԽ�޲����
#define  YCOLSAVEMODE_ALLUNSAVE		2  // ȫ�������
*/

/***************** ģ���� **************************/
typedef struct  {
        char    name[OBJECTNAME_LEN];//����
        char    describe[OBJDESCRIBE_LEN];//����
        int     securegrade;//��ȫ��
        char    substation[SUBSTNAME_LEN];//վ����ϵ����
		char    devname[OBJECTNAME_LEN];  //�豸����         //add
		ushort  terminal;//���߶���                          //add
//		uchar	samplemode;//�ɼ���ʽ
//		short   ftuno;
		short   groupno;    //�������                       //add
        short   terminalno;//�ն����
        short   ycno;//ң�����
		short	baseobjtype;//ң������
//        int     maxscale;//	
//        float   maxengval;  // �����֪ϵ���ͻ�ֵ������£�������д����:
//        int     minscale;	// minscale=0  maxscale=1  maxengval=ϵ��+��ֵ  minengval=��ֵ
//        float   minengval;//
        uchar  savedensity;//��������
        uchar  polgrade; //����Խ�޼��� 0-no process limit
        float   slope;//б��                           //add
		float   intercept;//�ؾ�                       //add

        float   zeroval;//��Ư
        float   maxinvld;//����ֵ
        float   mininvld;//����ֵ
        float   maxchange;//�������

//		short   deadtime;//����ʱ��                     //add
		char    entdealmode[OBJECTNAME_LEN];//�����ʽ//add
        float   highlimit;//��������
        float   lowlimit;//��������
        float   hihilimit;//��������
        float   lololimit;//��������
        float   recovcoef;      //�ָ�ϵ��
		char    olentname[ENTNAME_LEN];//��������
		char    almgraname[GRAP_LEN];//��ͼ��
        uchar   calcultf;//��������־                          //add
		uchar   changesavef;//�仯�洢��־                            //add
		short   scale;    //����                                //add
//        uchar   delayalmtime;   //������ʱ
//        char    olentname[ENTNAME_LEN];//������
        
        float   rawdata;//ԭʼֵ
		float   maxrawdata;//���ԭʼֵ
		float   minrawdata;//��Сԭʼֵ
        uchar   maxhour;//���ֵСʱ
        uchar   maxminute;//���ֵ����
        uchar   maxsecond;//���ֵ��
        uchar   minhour;//��СֵСʱ
        uchar   minminute;//��Сֵ����
        uchar   minsecond;//��Сֵ��
        float   value;//ֵ
        float   maxvalue;//���ֵ
        float   minvalue;//��Сֵ

        uchar   mansetf; //�����־
        uchar   bypassflag;//��·�����־
        uchar   linezero;//��·��ֵ��־
        uchar	neterror;//������ϱ�־
		uchar	terminalerror;//�ն˹��ϱ�־
		uchar   invalidf;//�Ƿ���־
		uchar	reserved0;

        uchar   puteventf;    //�����������־
        uchar   deadf;		//������־
        uchar   reliabilityf;	//�ɿ���-��Լ�������0-�ɿ���1-���ɿ�
		uchar	lockfertf;    //����FEP����������
		uchar	ycinconsistf; //YC��һ�±�־
		uchar	yxinconsistf; //YX��һ�±�־
		uchar	bypassrevf;   //���ֵȡ����־
		ushort	reserved1;	

		ushort   exflag1;		//��չ��־
		ushort   olgrade;/*Խ�޼���	
							0-- no over limit 
							1-- over high limit
							2-- over low  limit 
							3-- over high-high limit
							4-- over low-low   limit 
						*/
//		uchar	calmode;		//0--�Ǳ�׼���㣬1--
		short	overlimitcounthh;
		short   overlimitcounth;
		short   overlimitcountl;
		short   overlimitcountll;
		int     overlimittimehh;
		int     overlimittimeh;
		int     overlimittimel;
		int     overlimittimell;
		int 	sumtimes;
		double	sumvalue;
		double	daysumvalue;
		int	    daysumtimes;
		int		deadsecs;
		int     deadtime;
		int     dstoptime;	//��ͣ��ʱ��(���Ե�ѹ����)
		intertime  lastproctime;
		short   deadtimelimit;
		short	overinccnt;
		short	olcounter;
		char	bypassname[OBJECTNAME_LEN]; 
		int     procsecs;//����ʱ����
		int 	procmsecs;//����ʱ�����
}  BASEANALOG_STRU;

//�仯����ʱ��ṹ��
typedef struct
{
	int    tv_sec;         /* seconds */
	int    tv_msec;        /* and microseconds */
}TIME_VAL;

typedef struct 
{
	char    name[OBJECTNAME_LEN];//����
	char    desc[OBJDESCRIBE_LEN];
    float   value;//ֵ
	int     procsecs;//����ʱ����
	int 	procmsecs;//����ʱ�����
	char    unit[16];
} CHANGE_YCDATA;


typedef struct 
{
	CHANGE_YCDATA  ycData[MAX_CHGDATANUM];
	short         ycwritep;
	short          ycreadp;
	short         ycTrendDBwp;
	short         ycTrendDBrp;
}SAVE_YCDATA;

typedef struct 
{
	char    db[OBJECTNAME_LEN];
	char    nameserver[OBJDESCRIBE_LEN];
	char    user[OBJECTNAME_LEN];
	char    passwd[SUBSTNAME_LEN];
	int    datahistdays;  
	short  datahistdepth;    
}TRENDDB_STRU;


typedef struct  {
	short   index;			                        
	char    name[OBJECTNAME_LEN];
	short   starttime;
	short   endtime;
	float   highlimit;
	float   lowlimit;
	float   hihilimit;
	float   lololimit;
} YCTIMELIMIT_STRU;


#define   IS_YC_OVER_LIMIT(a) ((a)->olgrade && (a)->puteventf)



//analog table struct
typedef struct {
        BASEANALOG_STRU   baseanalog;
        short   quotatime;
        float   quotadata;  // ��ǰ�ƻ�ֵ
		float   quotaup;	// ��ǰ�ƻ�����
		float   quotadown;  // ��ǰ�ƻ�����
//		uchar	voltgrade; // ��ѹ�ȼ� 
//		short	voltlmt;   //��ѹ��ֵ
		int		updowntimes; // �������ɹ���������������
    } ANALOG_STRU;


//////////////////// ң�⴦�����  ////////////////////////

class  
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BaseAnalog : public NetDbg{
	Rdb_Tab*    pRdbTable;
    uchar       cbOLGradeChangef,cbOLGrade;
    uchar       cbPutEventf;

  private:
//˽�г�Ա��
    void        RawToEngineering(short  rawdata);
    void        Filter(void);			//�������ݹ��ˣ�����ֵ����Խ�޼����������ݷǷ���־���ȡ�
    void        OverLimitCheck(void);	//Խ�޴����ж��Ƿ�Խ�ޣ���Խ�޼��𣬲����ʵ���־��
    void        TimeOverLimit(BASEANALOG_STRU  *banap,int decsecs)	//Խ��ͳ��ʱ��
    		{
	  	switch (banap->olgrade)
	   	  {
	   	   case ANAOL_HIGH:
			banap->overlimittimeh=DAY_TIME_ADD(banap->overlimittimeh,decsecs);
			break;
	   	   case ANAOL_LOW:
			banap->overlimittimel=DAY_TIME_ADD(banap->overlimittimel,decsecs);
			break;
	   	   case ANAOL_HIHI:
			banap->overlimittimehh=DAY_TIME_ADD(banap->overlimittimehh,decsecs);
			break;
	   	   case ANAOL_LOLO:
			banap->overlimittimell=DAY_TIME_ADD(banap->overlimittimell,decsecs);
			break;
	   	  }
    		}
    void        OverLimitApEventProcessing(void);	//����Խ�������Խ�޼���仯��������ʵ���Խ�������֮��Ȼ��

virtual int     YieldOverLimitApEvent(void *rcdp)=0;          //���麯��==������Ϊ������࣬������ʵ��
virtual int     YieldOverMutationApEvent(void *rcdp)=0;          //���麯��==������Ϊ������࣬������ʵ��
	int         SetValue(float  setdata,int setflag);



   protected:
	BASEANALOG_STRU*      pBASEANALOG;
	int				nDecSecs;
	uchar			cbRawChangef,cbCookChangef;
	float			fEngData;
	//short           sRawData;
	virtual void    CalMaxMin(float value,float rawdata);	
	virtual void	Statistics(void);		//ͳ�ƹ������ݵ������Сֵ������ʱ��
	
    void			CountOverLimit(BASEANALOG_STRU  *banap,uchar olgrade);	//Խ�޴���ͳ��

  public:
	BaseAnalog(Rdb_Tab* pTable)
	{
		pRdbTable=pTable;
		m_pYcTimeLimit = NULL;
		ycdatap = NULL;
	};
	~BaseAnalog()
	{
		if (ycdatap!= NULL)
		{
			delete ycdatap;
			ycdatap = NULL;
		}
	}
	SAVE_YCDATA*    ycdatap;
	int GetYcName(short terminalno,short order,char* name);//ȡң����
  
	int     GetAnaValue(char *name,float *data);    //ȡֵ
	int     ManSetAnaVal(char *name,float  setdata,int setflag); //�˹�����
	virtual int     DayInit(void); 
	int     EngTimeDataProcessing(BASEANALOG_STRU *anap,float engdata,TIME_VAL yctime); //����ת��,��ʱ��
	//int     EngDataProcessing(BASEANALOG_STRU *anap,float engdata); //����ת��
	//int     RawDataProcessing(BASEANALOG_STRU *anap,short  rawdata ); //����������
	//int     FloatRawDataProcessing(BASEANALOG_STRU *anap,float  rawdata ); //��������������
	//int     FloatEngDataProcessing(BASEANALOG_STRU *anap,float  engdata,TIME_VAL yctime); //�������͹������� zxm
	//int     PutRawDataToYcRcdNo(int rcdno,short rawdata);  //�������ݷ�����Ӧң���¼
	int     PutTimeData(char *name,float data,TIME_VAL ycmultime);
	void	StatisticsCalVal(BASEANALOG_STRU  *banap);		//2004 05 add ͳ�Ƽ������ݵ������Сֵ������ʱ��
	void    TimerProcess(void);    // ����ʱ�ۼ�����
  public:
	  YCTIMELIMIT_STRU * m_pYcTimeLimit;
	  int CheckYcInTimeLimit(float &hlimit,float &hhlimit,float &llimit,float &lllimit);
  };

/////////////////////// ң�⴦����   ////////////////////////
class  
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Analog : public  BaseAnalog {
    static int        nOpenTablef;
    static Rdb_Tab    AnalogTable;
    int     YieldOverLimitApEvent(void *rcdp);
  public:
    Analog(void);
	ANALOG_STRU  *GetAnalogAddr(char *name,int *rcdno=NULL);
		int     YieldOverMutationApEvent(void *rcdp);
/*
	#if defined(WIN32)
	_SCD_DLLPORT 
	#endif
	friend void  AnaStandardCal(void);
*/
	#if defined(WIN32)
	_SCD_DLLPORT 
	#endif
	friend void   AnaBypassProcessing(void);
  };

#if defined(WIN32)
_SCD_DLLPORT 
#endif
void  SetTimerSavingFlag(int setval);

////////////////  ��ѹ�ȼ� �� ��ѹ��ֵ /////////////////////////////////////////
/*
typedef struct {
	char   name[OBJECTNAME_LEN];
	uchar  voltgrade;
    char   describe[OBJDESCRIBE_LEN];
    float  ratingvolt;
    float  stdvolt;
 	uchar  units;	//��λ��0--KV��1--V
	uchar  ac_dc;    //��ֱ��: 0-- AC 1-- DC
} VOLTGRADE_STRU;
  
typedef struct {
	short  voltlmtno;
    char   describe[OBJDESCRIBE_LEN];
    float  upvolt;
    float  downvolt;
	float  almhlimit;
	float  almllimit;
	float  almhhlimit;
	float  almlllimit;
} VOLTLIMIT_STRU;
  
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
VoltLimit {
	static int  opentabf;
	static Rdb_Tab  voltlimittab;
	static Rdb_Tab  voltgradetab;
  public:
	VoltLimit(void);
	VOLTGRADE_STRU *GetVoltGrade(uchar voltgrade);
	VOLTLIMIT_STRU *GetVoltLimit(short voltlmt);
};  
*/


/*********************** ��������� ************************/     

//     �˱�Ķ�����ҪΪ�˱���һЩ���������м������������
//	ģ��������״̬��������Ҫ��������ֻ�豣��һЩֵ��
/*
// ��������ṹ
typedef struct {
		char    name[OBJECTNAME_LEN];			// ����
		int     describe[OBJDESCRIBE_LEN];		// ����
		int     securegrade;					// ��ȫ��
		float   value;							// ֵ
		uchar	state;							// ״̬
} CALCULATE_STRU;


class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Calculate {
		static int			opentabf;
		static Rdb_Tab		calculttab;
	public:
		Calculate(void);  
		int GetCalValue(char *name, float *data);  
  };
*/  

/*********************** ״̬��Ϣ�� ************************/
// ���������ʽ
typedef struct {
		uchar  printf;	// ��ӡ
		uchar  logf;		// ��¼	
		uchar  alarmf;	// ���챨��
		uchar  popgraphf;	// ��ͼ
} ENTOUTMODE_STRU;


typedef struct {
		char		name[OBJECTNAME_LEN];		// ����
		char		describe[OBJDESCRIBE_LEN];	// ����
		short		staval;						// ״ֵ̬
		ENTOUTMODE_STRU	entmode;				// ���������ʽ
} STATUSMESG_STRU;
          
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
StatusMesg {
		static	int			opentabf;
		static	Rdb_Tab	    stamsgtab;
	public:
		StatusMesg(void); 
		// ���ݶ�������״ֵ̬ȡ��״̬��Ϣ��ṹ��ַ
		STATUSMESG_STRU	*getmsgaddr(char *,short);
};




typedef	struct {
		char	name[OBJECTNAME_LEN];			// ����
		char	describe[OBJDESCRIBE_LEN];		// ����
		int	    securegrade;				    // ��ȫ��
        char    substation[SUBSTNAME_LEN];
		ushort	objtype;				        // �豸����
		uchar   sampletype;
		short   groupno;
		short   terminalno;
		short   orderno;
		short	maxinvld;						// �����Ч��ֵ
		short	mininvld;						// ��С��Ч��ֵ
		char	chgentname[ENTNAME_LEN];		// �������
		char	stamsgname[OBJECTNAME_LEN];     // ״̬��Ϣ��
		char	almgraname[GRAP_LEN];		    // ͼ��
		short	value;							// ״ֵ̬
		uchar	mansetf;						//����λ
		uchar	invalidf;						// ��Ч��־
		uchar   calcultf;
} MULTISTATUS_STRU;        


class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
MultiStatus {
		static	int			opentabf;
		static	Rdb_Tab	    multstatab;
		//  ������̬���仯Ӧ���¼�
		int		YieldChgApEvent(MULTISTATUS_STRU *,short);
		//	�����ֵ̬
		int		MultiStadataProcessing(MULTISTATUS_STRU *,short);
	public:
		MultiStatus(void);
		//  ���ݶ���������״ֵ̬
		int		SetMultiStadataByName(char *name,short staval);
		//  ���ݶ�����ȡ״ֵ̬
		int		GetData(char *name,short *stadata);
		void    ProcRawData(MULTISTATUS_STRU *mp,short mstaval)  {
			MultiStadataProcessing(mp,mstaval);
		}
};

////////////////////////////////////////////////////////////////
typedef struct {
        SYS_CLOCK   eclock;		//	ʱ��
        uchar     source;		//	������Դ 
        ushort    terminalno;	//	
        ushort    yxno;			//
        uchar     status;		//
		uchar     testflag;
}  RAWENT_STRU;



// �ṹ
typedef struct	{
		short terminalno;
		short yxno;
}YXSORT_STRU;

typedef	struct   {
	    short   groupno;
	    short	terminalno;					// �ն˺�
		short	yxno;						// ң�����
		char	objname[OBJECTNAME_LEN];	// ������
		short	objtype;					// ��������
} TERMINALYXSORT_STRU;

typedef struct	{
	short terminalno;
	short ycno;
}YCSORT_STRU;

typedef	struct   {
	short   groupno;
	short	terminalno;					// �ն˺�
	short	ycno;						// ң�����
	char	objname[OBJECTNAME_LEN];	// ������
	short	objtype;					// ��������
} TERMINALYCSORT_STRU;

typedef struct	{
	short channelno;
	char	objname[OBJECTNAME_LEN];	// ������
}CHANNELSORT_STRU;

typedef struct	{
	short terminalno;
	short kwhno;
}KWHSORT_STRU;

typedef	struct   {
	short   groupno;
	short	terminalno;					// �ն˺�
	short	kwhno;						// ң�����
	char	objname[OBJECTNAME_LEN];	// ������
	short	objtype;					// ��������
} TERMINALKWHSORT_STRU;


typedef	struct   {
		short	no;						    // ���
		char	objname[OBJECTNAME_LEN];	// ������
} WFBS_STRU;

//״̬�������ṹ
typedef struct {
        char    name[OBJECTNAME_LEN];		//  ������
        char    describe[OBJDESCRIBE_LEN];	//	����
        int     securegrade;				//	��ȫ��
//		uchar   voltgrade;					//  ��ѹ�ȼ�
        char    substation[SUBSTNAME_LEN];	//	վ����ϵ����
		char    devcode[OBJECTNAME_LEN];	//  �豸����                     //add
		ushort	baseobjtype;				//  ң������
//		uchar	samplemode;					//	�ɼ���ʽ
//		short	ftuno;						//	ftu��
		short	groupno;					//	�������                     //add
		short	terminalno;					//	�ն����
		short	yxno;						//  ң�����
		short	yxno1;						//  ��ң�����
		uchar	twinyxflag;					//  ˫ң�ű�־��˫λ��־��
		uchar	mainyxflag;					//  ��ң�ű�־                   //add
		uchar   twinyxcalmode;              //  ˫ң�ż��㷽ʽ               //add
        char    entdealmode[OBJECTNAME_LEN];//  �����ʽ                 //add
		uchar   slowyx;						//  ��ң��
//		uchar   confirmmode;				//  ȷ�Ϸ�ʽ
//      uchar	oddproctype;				// 0--- no process     
											// 1--- ��ֱ���  
		uchar	zyf;						//	׷���־
		uchar	ykf;						//	�ɷ�ң��
		uchar	calcultf;					//����������־
		ushort	reserved0;
		uchar	changesavef;					//���仯�洢��־

		uchar	maskchgentf;				//  ������λ�����־
		uchar	mansetf;					//���˹�����
		uchar	lockfertf;					//  ����FEP����������
		uchar	putdangerf;					//  ���Ʊ�־
		uchar	putgroundf;					//  ĸ�߲�ӵر�־
		uchar	sndgroundf;					//  ��·��ӵر�־
		uchar   hsgflag;
		ushort	reserved1;

		uchar	invalidf;					//����Ч
		uchar	changef;					//����λ
		uchar	oddstate;					//���쳣
		uchar	yktakef;					//	ң��ִ�б�־
		uchar   linerunf;					//  ��·�����־
		uchar	revval0;
		uchar	revval1;
		uchar	initedf;					//  ��ʼ����־

		uchar	neterror;					//���������
		uchar	terminalerror;				//��TERMINAL����
		uchar   alarmf;						//  ��ʾ��־
		uchar   yxinconsistf;				//  ң�Ų�ƥ��
		uchar	reserved3;

        uchar   value;						//����ǰֵ
        uchar   revval;						//������ֵ(yxno)
		uchar	fiveflag;					//  �����־

		ushort clearchgtime;				//  ��λ��־Ϊ1ʱ����ָ��ʱ�����־���������ڴ��У�
		ushort clearalmtime;				//  ��ʾ��־Ϊ1ʱ����ָ��ʱ�����־���������ڴ��У�
        char    soeentname[ENTNAME_LEN];	//  SOE��
        char    chgentname[ENTNAME_LEN];	//  �����
        char    almgraname[GRAP_LEN];	    //  ��ͼ��
		intertime lastproct;				//	�ϴδ���ʱ��
		int		yktimes;					//  ң�سɹ������ۼ�ֵ, ��������
		int     procsecs;                    //����ʱ����
		int     procmsecs;                    //����ʱ�����
} BASESTATUS_STRU;
   

typedef struct 
{
	char    name[OBJECTNAME_LEN];		//  ������
	char    desc[OBJDESCRIBE_LEN];
    uchar   value;						//��ֵ
	int     procsecs;                    //����ʱ����
	int     procmsecs;                    //����ʱ�����
} CHANGE_YXDATA;


typedef struct 
{
	CHANGE_YXDATA  yxData[MAX_CHGDATANUM];
	short         yxwritep;
	short          yxreadp;
	short         yxTrendDBwp;
	short         yxTrendDBrp;       
}SAVE_YXDATA;

// ״̬�����ࣨstatus process base class��

class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BaseStatus : public NetDbg{
		char		     objtabname[RDB_COLNAME_LEN];
		Rdb_Tab	        *objtable;
		BASESTATUS_STRU	*statusp;
		uchar            changeflag;
		// �����ն���ź�TERMINAL����ţ�ȡ�ü�¼�ţ��˼�¼��Ӧ�Ǽ���ֵ��
//		int			getrcdno(short terminalno, short order); 
		// �þ�ʾ��־
		virtual int		SetAlarmFlag(BASESTATUS_STRU *p); 
	public:
		SAVE_YXDATA *yxdatap;
	
		
		BaseStatus(char *tabname,Rdb_Tab *tabp)
		{
			strcpy(objtabname,tabname);
		    objtable = tabp;
			yxdatap = NULL;
		};
		~BaseStatus()
		{
			if (yxdatap != NULL)
			{
				delete yxdatap;
				yxdatap = NULL;
			}
		}
		int GetYxName(short terminalno,short order,char* name);//ȡң����
		int GetSubYxName(short terminalno,short order,char* name);//ȡ��ң�ź�ң����	
		//��λʱ��С�����ص�״̬
		int  CarbreakStatusProcessing(BASESTATUS_STRU  *p,uchar &uCarBreakStat);

		virtual int      RawStatusdataProcessing(BASESTATUS_STRU  *stap,int yxno,uchar  statusval,int proc_slowyxf,uchar makeentf=1);
		virtual int		 StatusdataObjectProcessing(BASESTATUS_STRU  *stap,uchar  statusval,uchar makeentf=1);//����������
		// ����λ�¼�����Ӧ���¼�����
		virtual void     PutStaChgEvent(SYS_CLOCK *entclockp,int chgtype,BASESTATUS_STRU *stap,uchar staval,uchar maketype,uchar testflag=0);
		// �������ֽ�״ֵ̬�������
		virtual  int     SetStadataByName(char *name,uchar stadata);
		// ��������ȡ״ֵ̬
		int				 GetYxData(char *name,uchar *stadata);//ȡֵ
		// ��������ȡ�ü�¼�ṹ��ַ
		BASESTATUS_STRU	*GetAddrByName(char *name,int *rcdno=NULL);
		// ����������
		int				 RaweventProcessing(char *name, RAWENT_STRU *raweventp,int proc_slowyxf); 
		// �������ֽ���¼��Ϊ�˹����뷽ʽ������¼״̬��Ϊ����ֵ ���� ���˹������־������¼״̬��Ϊʵʱֵ
		virtual  int     MansetYxVal(char *name,uchar setdata,uchar setflag); //�˹�����
};



/******************** ״̬�� *********************/
// �ṹ
typedef struct {
        BASESTATUS_STRU	    basestatus;
	    int			        runtime;		//����ʱ��
		unsigned short		chgcounter;		//��λ����
		unsigned int		second_yxchg;		//  ������ zl20110805
		unsigned int		msecond_yxchg;		//  �������
		unsigned int		second_soe;			//  ������
		unsigned int		msecond_soe;		//  �������
} STATUS_STRU;

// �ࣨstatus process class��
class
#if defined(WIN32)
_SCD_DLLPORT 
#endif
StatusProcess : public  BaseStatus {
		static int         opentabf;
		static Rdb_Tab    statustab;
	public:
		StatusProcess(void) ;
};

/******************** �����ϼ��״̬�� *********************/
// �ṹ
typedef struct {
        BASESTATUS_STRU	    basestatus;
	    int			        runtime;		//����ʱ��
		unsigned short		chgcounter;		//��λ����
		unsigned int		second_yxchg;		//  ������ zl20110805
		unsigned int		msecond_yxchg;		//  �������
		unsigned int		second_soe;			//  ������
		unsigned int		msecond_soe;		//  �������
} DMSSTATUS_STRU;

// �ࣨdmsstatus process class��
class
#if defined(WIN32)
_SCD_DLLPORT 
#endif
DmsStatusProcess : public  BaseStatus {
		static int         opentabf;
		static Rdb_Tab     dmsstatustab;
	public:
		DmsStatusProcess(void) ;
};

/*****************  �����ź� ********************/
// �ṹ
typedef struct   {
		BASESTATUS_STRU	basestatus;
		ushort		actcounter;
		unsigned int		second_yxchg;		//  ������ zl20110805
		unsigned int		msecond_yxchg;		//  �������
		unsigned int		second_soe;			//  ������
		unsigned int		msecond_soe;		//  �������
} PROTSIG_STRU;


// �ࣨProtsig process class��

class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Protsig : public BaseStatus  {
		static int         opentabf;
		static Rdb_Tab    protsigtab;
	public:
		Protsig(void);
};


/***************** ��·�� ���� ********************/
// �ṹ
typedef	struct  {
		BASESTATUS_STRU	basestatus;
		int		        tchgcounter;	//	�ܱ�λ����
		int		        dayruntime;		//	������ʱ��
		int		        totalruntime;	//	������ʱ��
		unsigned short	dchgcounter;	//	�ձ�λ����
		unsigned short	tsgcounter;		//	���¹���բ����
		short		    maxchgnums;     //  ��λ������
		short		    maxsgnums;		//	�¹���
		short		    zygroup;		//	׷�����
		char		    bypassbreak[OBJECTNAME_LEN];
		char		    sglogic[PROCNAME_LEN];		// �¹��߼�������
		uchar		    sgprocmode;		//	���¹�ģʽ
		uchar		    sgprocband;		//	���¹�����
		uchar		    linestat;		//  ��·״̬ ���������ޡ��޵��
		uchar           bypassflag;     //  ��·��־
		unsigned int		second_yxchg;		//  ������ zl20110805
		unsigned int		msecond_yxchg;		//  �������
		unsigned int		second_soe;			//  ������
		unsigned int		msecond_soe;		//  �������
} BREAK_STRU;


// �ࣨBreak process class��
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Break : public BaseStatus  {
		static int			opentabf;
		static Rdb_Tab		breaktab;
	    //static intertime	lastcaltime;	//	�ϴ�ʱ��
	public:
		Break(void);
		// �ۼ�����ʱ��
		void	CalRunTime(void);
		// �ۼ�ֵ����
		void	DayInit(void);
		// �������ֽ���¼��Ϊָ���Ĳ�����־
	    int		SetSpecialFlag(char *name,uchar setflag,short flagtype,short flagno); 
};


/**********************  ��բ  *******************/
// �ṹ
typedef	struct   {
		BASESTATUS_STRU	basestatus;
	    int			runtime;					// ����ʱ��
		uchar		linestat;		//  ��·״̬ ���������ޡ��޵��
		unsigned int		second_yxchg;		//  ������ zl20110805
		unsigned int		msecond_yxchg;		//  �������
		unsigned int		second_soe;			//  ������
		unsigned int		msecond_soe;		//  �������
} SWITCH_STRU;



// �ࣨswitch process class��
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Switch : public BaseStatus  {
		static int         opentabf;
		static Rdb_Tab     switchtab;
	public:
		Switch(void);
		//
	    int   SetSpecialFlag(char *name,uchar setflag,short flagtype,short flagno);
};


#ifdef WIN32
_SCD_DLLPORT 
#endif
BASESTATUS_STRU   *GetYXTableNameForRdb(char *objname,char *tabname);

#ifdef WIN32
_SCD_DLLPORT 
#endif
BASESTATUS_STRU   *GetYXTableTypeForRdb(char *objname,int objtype);

#ifdef WIN32
_SCD_DLLPORT 
#endif
int PutForeRaweventByObjtype(char *name,int objtype, RAWENT_STRU *rawentp,int proc_slowyxf);

#ifdef WIN32
_SCD_DLLPORT 
#endif
int PutStatusDataByObjtype(char *name,int objtype,uchar staval);


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class	
#if defined(WIN32)
_SCD_DLLPORT  
#endif
BreakYCRelation {
	static Rdb_Tab					brktable;
	static Rdb_Tab					anatable;
	static int						breaknums;
	static intertime				brkupdtime;
	static intertime				anaupdtime;
	static BASEANALOG_STRU				**ana_addr;
public :
	BreakYCRelation(void);
	void              CheckAllBreakYCRelation(void);
	BASEANALOG_STRU **GetYCRelation(int brkno,int *relnums);
	BASEANALOG_STRU **GetYCRelation(char *breakname,int *relnums);
};



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct  {
	uchar   oldstate;
	uchar   timecount;
} BREAKSTATE_DELAY_STRU;
class	
#if defined(WIN32)
_SCD_DLLPORT  
#endif
BreakYCInconsist {
	static Rdb_Tab			brktable;
	static BREAKSTATE_DELAY_STRU  *brkstate_addr;
public :
	BreakYCInconsist(void);
	void SetLineZero(int brkno,int flag);
	void SetYXInconsist(int brkno,BASESTATUS_STRU *breakp,int flag);
	void LoopProcBreakYCInconsist(void);
};



#define  SLOWYX_TYPE_CHANG        0
#define  SLOWYX_TYPE_SOE		  1
#define  SLOWYX_TYPE_STATUS       2

typedef struct {
	uchar       type;
	short		objtype;					// ��������
	char		objname[OBJECTNAME_LEN];	// ������
	short       yxno;
	short		lifetime;
	short       bandtime;
	intertime	recvtime;
	unsigned short		recvms;
	uchar		fstatus;	// �״�״̬
	short		nstatus;    // ����״̬
	 RAWENT_STRU    rawevent;
}  SLOW_LINK_DATA_STRU;

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
SlowYXLinkTable {
	static ComnLinkTable<SLOW_LINK_DATA_STRU>  clinktable;
public:
	int AddYXEvent(short objtype,char *objname, RAWENT_STRU  *rawentp,int slowyx);
	int AddYXStatus(short objtype,char *objname,uchar status,int slowyx);
	void SlowYXProc(void);
};

/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------//
// �����

#define	SDTYPENUMS	4	// ʱ��������
// �ṹ
typedef	struct {
		char    name[OBJECTNAME_LEN];		// ������
		char    describe[OBJDESCRIBE_LEN];	// ����
		int     securegrade;				// ��ȫ��
		char    substation[SUBSTNAME_LEN];	// ��վ��
		char    devname[OBJECTNAME_LEN];	// �豸����
		ushort	baseobjtype;				// �������
//		uchar	samplemode;					// �ɼ���ʽ
//		short	ftuno;						// FTU��
		short   groupno;                    // �������
		short	terminalno;					// �ն����	
		short	kwhno;						// ������
		float	basecode;					// ����
		float   scale;						// ���
		float   fullcode;					// ����
		float   factor;						// ����
		float	sdbasecode[SDTYPENUMS];
		float	sdfullcode[SDTYPENUMS];
									//           2--20bit,3--24bit
									//           4--32bit,5--�Զ���
		unsigned int     rawdatafull;       // ԭ����ֵ
		unsigned int     rawdata;			// ������
		float   tabcode;	
		float	sdtabcode[SDTYPENUMS];		// ʱ�α���	
		float   curkwh;						// �������ֵ
		short   savedensity;				// ���̼��
		uchar	sampletype;					// ��������  0--16bit,1--19bit,
		uchar    exflag1;					//��չ��־
		double	integralkwh;				// ���ֵ��ֵ	
		double  savekwh;					// ����ֵ
		double  daykwh;						// ���ۼ�
		double  monthkwh;					// ���ۼ�
		double  yearkwh;					// ���ۼ�
		double  daysdkwh[SDTYPENUMS];       // ʱ�����ۼ�(ƽ���塢�ȡ����)
		double  monthsdkwh[SDTYPENUMS];		// ʱ�����ۼ�
		double  yearsdkwh[SDTYPENUMS];		// ʱ�����ۼ�
		ushort  district;					// ����(ʱ����)

		uchar	mansetf;					// �˹������־
		uchar	calcultf;					// �����־
		uchar	integralf;					// ���ֱ�־
		uchar	savediskf;					// ���̱�־
		uchar	initedf;					// ��ʼ����־
		uchar	neterror;					// �������
		uchar	terminalerror;				// TERMINAL����
		uchar	reserved0;

		uchar	lockfertf;					// ����FEP����������			
		uchar	bypassflag;					// ��·��־
		int	reserved1;

		char    bypassname[OBJECTNAME_LEN];	// ��·��ȴ���
		char    integycname[OBJECTNAME_LEN];// ����ң�����
		float   lastbasecode;				// �ϴλ���
		double	abskwh;						// ���Ե��
		double	bykwh;						// ��·���
		double  daysdkwh5;                  // ����ʱ�����ۼ�
		double  monthsdkwh5;		        // ����ʱ�����ۼ�
		double  yearsdkwh5;		            // ����ʱ�����ۼ�
} KWH_STRU;

// ���ࣨKWH process base class��
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BaseKwh : public NetDbg{
		Rdb_Tab	*objtable;
		void        ProcRawData_Counter(KWH_STRU *,unsigned int  rawdata);
		void        ProcRawData_Tabcode(KWH_STRU *,unsigned int  rawdata);
	public:
		BaseKwh(Rdb_Tab *tabp) {	objtable = tabp;	};
		//
		int GetKwhName(short terminalno,short order,char* name);//ȡ�����
		void        ProcIncData(KWH_STRU *,double incdata); 
        // ȡ�����̵��ֵ��������ֵ���㣬���½����ۼ�
		float       GetSaveKwhVal(KWH_STRU  *kwhp);
        // ȡ�����ֵ��ֵ��������ֵ���㣬���½����ۼ�
		float       GetIntegralKwhVal(KWH_STRU  *kwhp);
		//
		int			GetKwhValueByName(char *name, float *value);
		// ���ݼ�¼�Ž������ݷ���ʵʱ����
		int         PutRawDataByRcdNo(int rcdno,unsigned int rawdata);
		// ����������
		void        RawKwhProcessing(KWH_STRU *,unsigned int  rawdata);
		void        ProcRawData_HourClean(KWH_STRU *,unsigned int  rawdata);
		//
		KWH_STRU   *GetKwhAddr(const char *name,int *rcdno=NULL);
};


// �ࣨKwh  process  class��
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Kwh  : public BaseKwh {
		static int         opentabf;
		static Rdb_Tab     kwhtab;
	public:
		Kwh(void);
		
		// �ۼƵ��������
		#if defined(WIN32)
		_SCD_DLLPORT 
		#endif
		friend void         KwhDayInit(void);
		// ��Ȼ���	
		#if defined(WIN32)
		_SCD_DLLPORT 
		#endif
		friend void         CalIntegralKwh(void);
		// ��·���	
		#if defined(WIN32)
		_SCD_DLLPORT 
		#endif
		friend void         KwhBypassProcessing(void);
};



////////////////////////////////ң���Դ����//////////////////////////////

#define		MAX_YCSRC_NUMS	4


#define		MULTSRC_SAMPLING_TERMINAL	1
#define		MULTSRC_SAMPLING_OBJ	2

typedef struct {
	uchar   samplemode;
	short   sampgrpno;
	short   samporderno;
	float   coef;
	float	base;
}  YC_SRCPARA_STRU;

typedef struct {
	float   value;
	uchar	quality;
	uchar	initedf;
	uchar	invalidf;
	uchar	deadf;
	uchar	deverror;
	uchar   reserved;
	int		deadsecs;
	int     procsecs;
	int     procmsecs;
}  YC_SRCDATA_STRU;

typedef struct {
	char	objname[OBJECTNAME_LEN];
	uchar	selectmode;
	uchar	cursrcno;
	uchar	setflag;
	uchar	reserved;
	YC_SRCPARA_STRU  ycsrcpara[MAX_YCSRC_NUMS];
	YC_SRCDATA_STRU  ycsrcdata[MAX_YCSRC_NUMS];
} YC_MULTSRC_STRU;

class YCMultiSource {
    static int         opentabf;
    static Rdb_Tab    ycmultsrctab;
  public:
    YCMultiSource(void);

	#if defined(WIN32)
	_SCD_DLLPORT 
	#endif
	friend void  YCMultiSourceDataProcessing(void);
};


////////////////////////////////////////////////////////////////////////////////////////////

// ���ݿ�����ȡP��Q��I
#if defined(WIN32)
_SCD_DLLPORT 
#endif
int  GetAnalogDataByBreak(char *breakname,float *pdata,float *qdata=NULL,float *idata=NULL);


 
//------------------------ ���ر�����ϵ�� --------------------------//
typedef	struct {
		char   breakname[OBJECTNAME_LEN];
		char   protname[OBJECTNAME_LEN];
		uchar  sgjudgef;
} BREAK_PROT_REL_STRU;

class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BreakProtRelation {
		static int        opentabf;
		static Rdb_Tab   breakprottab;
		static Rdb_Tab   breakprotmdltab;
	public:
		BreakProtRelation(void);
		int  CheckProtRelate(char *,char *,uchar);
		int  MaskBreakProtEnt(char *breakname,int flag);
};


//------------------------ ���������ϵ�� --------------------------//

typedef	struct {
		char   breakreplaced[OBJECTNAME_LEN];
		char   breakname[OBJECTNAME_LEN];
		short  replacetype;
		char   plswitch[OBJECTNAME_LEN];
		uchar  replacemode;
} BREAK_REPLACE_REL_STRU;


class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BreakReplace : public NetDbg{
		static int        opentabf;
		static Rdb_Tab    breakreplacetab;
	public:
		BreakReplace(void);
		#if defined(WIN32)
		_SCD_DLLPORT 
		#endif
		friend int AutoCheckBreakReplace(void);

		#if defined(WIN32)
		_SCD_DLLPORT 
		#endif
		friend int Break_Replace(char *breakreplaced,char *breakname,uchar replacemode,char *hostname,char *username);
};

/////////////////////////  �������ӵ�բ�� ////////////////////////////////
typedef	struct {
		char   breakname[POWDEVNAME_LEN];
		char   switchname[POWDEVNAME_LEN];
		short  swtype;
		short  distance;
		uchar  sgmode;
} BREAK_SWITCH_REL_STRU;


class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BreakSwitchRelation {
		static int        opentabf;
		static Rdb_Tab    breakswitchtab;
	public:
		BreakSwitchRelation(void);
		int CheckSGCond_Switch(char *breakname);
		int CheckYKCond_Switch(char *breakname);
};


/***************************  �¹ʴ����� ******************************/

// �¹ʴ���
#define  ESOURCE_SGZ			1			// �¹�Դ���¹����ź�
#define  ESOURCE_PROT			2			//         �����ź�
#define  ESOURCE_PROTMODULE		3			//         ����ģ���¼��ź�

// ���صȴ����� 
typedef	struct {
		intertime  recvtime;					// �յ�ʱ��
		unsigned short recvms;
		uchar	   yieldentf;
		int        lifetime;					// ��������
		uchar      sgcondition;					// �¹��ж�����
		uchar      haveprotf;					// 
		SYS_CLOCK  entclock;					// �����ʱ��
		char       substation[SUBSTNAME_LEN];	// ��վ��	
		char       breakname[OBJECTNAME_LEN];	// ������
		uchar      staval;						// ״ֵ̬
		int        relatecond;					// ��ϵ����
		float      pdata;
		float      qdata;
		float      idata;
		short	   zygroupno;
		uchar       relclasstype;
		char		relname[OBJECTNAME_LEN];
		char		reldescribe[OBJDESCRIBE_LEN];
} BRKWAITQUE_STRU;

// ��ϵ����ȴ����� 
typedef	struct {
		intertime  recvtime;					// �յ�ʱ��
		unsigned short recvms;
		int			lifetime;					// ��������
		uchar      classtype;					// 
		char       substation[SUBSTNAME_LEN];		// ��վ��
		char       objname[OBJECTNAME_LEN];		// ������
		char	   objdescribe[OBJDESCRIBE_LEN];
} RELATEWAITQUE_STRU;


// �¹ʴ�����
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
AccidentProc : public NetDbg{
		static ComnLinkTable<BRKWAITQUE_STRU>	    brkwaittab;		// ���صȴ���
		static ComnLinkTable<RELATEWAITQUE_STRU>	relatetab;		// ��ϵ��
		// ����<��������>Ӧ���¼�����������δ���¹�ǰ��<��λ>Ӧ���¼�
		int		YieldBreakApEveent(SYS_CLOCK *,char *,uchar,uchar,short,float,float,
			float,uchar relclasstype=0,char *relname=NULL,char *reldes=NULL,uchar testflag=0);
	public:
		// �����صȴ������в���һ���µĿ��ر�λ
		int		InsertBreakEvent(SYS_CLOCK *entclock,char *substation,char *breakname,
			    uchar staval,short zygroupno,float pdata,float qdata,float idata,uchar testflag);
		// ��������ϵ�ȴ������в����¹����ź�
		int		InsertSgzEvent(char *substation,char *sgzname,uchar staval);
		// ��������ϵ�ȴ������в��뱣������
		int		InsertProtEvent(char *substation,char *protname,uchar staval);
		int		InsertProtModuleEvent(char *substation,char *protmodulename,uchar staval);
		// �������
		int		ClearBreakEvent(void);
		// �п��ر�λ�Ƿ��¹�
		int		CheckBreakEvent(void);
};
  

/////////////////// ״̬��������   //////////////////////////////////
#define  MAX_YKCONDITION_NUMS     10

typedef struct {
	char  name[OBJECTNAME_LEN];		// ����
	uchar yktype;//condtype;��������
	uchar condgroupno;	//�������
	struct {
		char  condname[OBJECTNAME_LEN];
		uchar condstate;
	} ctrlcond[MAX_YKCONDITION_NUMS];
}  YKCONDITION_STRU;


int  
#if defined(WIN32)
_SCD_DLLPORT 
#endif
CheckYKCondition(char *name,uchar  state);


typedef	struct {
		char    name[OBJECTNAME_LEN];		//  ����
		char    describe[OBJDESCRIBE_LEN];	//	����                         //add
		int     securegrade;				//  ��ȫ��
        char    substation[SUBSTNAME_LEN];	//	վ����ϵ����                 //add
		char    devcode[OBJECTNAME_LEN];	//  �豸����                     //add
		ushort	baseobjtype;				//  ң������                     //add
        short	groupno;					//	�������                     //add
		short   ykterminalno;				//  �ն����
		short   ykno;						//  ң�����
        uchar   monflag;					//  ����ģʽ �Ƿ�ල��
		uchar   ykmode;                     //  ң�ط�ʽ
		char    entdealmode[OBJECTNAME_LEN];//  �����ʽ                 //add
		char    almgraname[GRAP_LEN];	    //  ��ͼ��                       //add
		char    khprocname[OBJECTNAME_LEN]; //  �غϹ�����                   //add
		char    kfprocname[OBJECTNAME_LEN]; //  �طֹ�����                   //add
//		uchar   monflag;					//  �ල��־
		uchar   exykflag;					//  ��չң�أ�1--������ң��״̬
		uchar   waitchktime;				//  ң�ط�Уʱ����
		uchar   sendcmdtime;				//  ң�ط���ʱ����
		uchar   execcmdtime;				//	ң��ִ��ʱ����
		char    ykentname[ENTNAME_LEN];	    //  ң�������
		uchar   lockykf;					//  ����ң�ر�־
		
//		char    ykactionname[OBJECTNAME_LEN];	// ң�ض�������
} YKPARA_STRU;


// ��������
typedef	struct {
		char   name[OBJECTNAME_LEN];		// ����
		char   describe[OBJDESCRIBE_LEN];	//	����                         //add
		int    securegrade;				    //  ��ȫ��
        char   substation[SUBSTNAME_LEN];	//	վ����ϵ����                 //add
		char   devcode[OBJECTNAME_LEN];	    //  �豸����                     //add
		short	groupno;					//	�������                     //add
		short  terminalno;			        //  �ն����
		uchar  monflag;					    //  ����ģʽ
		short  upykno;						//  ��ң�غ�
		uchar  upykaction;					//  ������
		uchar  upykcmd;					    //  ������
		short  downykno;					//  ��ң��
		uchar  downykaction;				//  ������
		uchar  downykcmd;					//  ������
		short  pauseykno;					//  ��ͣң�غ�
		uchar  pauseykaction;				//  ��ͣ����
		uchar  pauseykcmd;					//  ��ͣ����
//		uchar  monflag;					    //  �ල��־
		uchar  waitchktime;				    //  ��Уʱ����
        uchar  sendcmdtime;				    //  ����ʱ����
		uchar  execcmdtime;				    //  ִ��ʱ����
        char    entdealmode[OBJECTNAME_LEN];//  �����ʽ                 //add
		char   entname[ENTNAME_LEN];	    //  �������
		char   upctrlogic[PROCNAME_LEN];	//  �����߼�
		char   downctrlogic[PROCNAME_LEN];	//  �ؽ��߼�
		uchar  updownmode;					//  ������ʽ
		uchar  lockupdownf;				    //  ����������־
} UPDOWNPARA_STRU;


// ң�ض���
typedef struct {
		char      tabname[TABNAME_LEN];		    // ����
		char      name[OBJECTNAME_LEN];			// ������
		char      describe[OBJDESCRIBE_LEN];	// ����
		char	  groupname[OBJECTNAME_LEN];    //  ����
		int       securegrade;					// ��ȫ��
		uchar     yktype;						// ң������
		uchar     actype;						// ��������
		ushort    ykgroupno;                    // �������
		ushort    ykterminalno;					// �ն˺�
		ushort    ykno;							// yk ��
		uchar     monflag;						// �ල��־
		uchar     exykflag;					    // ��չң�أ�1--������ң��״̬
		short     waitchktime;					// �ȴ���Уʱ��	
		short     sendcmdtime;					// ��������ʱ��
		short     execcmdtime;					// ִ������ʱ��
		uchar     ykmode;						// ykģʽ
		char      entname[ENTNAME_LEN];		    // �����
		char      opmachine[HOSTNAME_LEN];		// ����������
		char      operatorname[USERNAME_LEN];	// ����Ա��
		uhlong     optaskid;					// ���������
		char      confirmhost[HOSTNAME_LEN];	// ���������
		char      confirmname[USERNAME_LEN];	// ��ɲ���Ա��
		uhlong    confirmtaskid;				// ��������  
		uchar     firstcmdf;					// 
		uchar     programykf;					// 
		uchar     intercmdf;					// 
		uchar	  ykfangshi;					//  ң�ط�ʽ 0:��׼ 1:������������
		intertime yktime;					    //
		uchar     yksercode;					//
		uchar     sendcmdf;						//
		uchar     ykaction;						//
		int       updownval;					//
} YKQUEUE_STRU;



typedef	struct  {
		uchar   lockf;	// ������־
		uchar   getshareykf;  //  ����ȡ����ң�ر�־
		uchar   shareykf;   //  ����ң�ر�־
		YKQUEUE_STRU  *lockykquep;  // ��Ȩң�صĶ���
} YK_LOCK_STRU;



// ң�ض�����
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Ykqueue : public NetDbg{
		static	PROG_SHARECTRL_STRU	*ykqueuectrl;
		static	Rdb_Tab		ykqueuetab;
		static  int             initlockf;
		static	YK_LOCK_STRU		terminalyklock[SCADA_MAX_TERMINALNUMS];

		//  ץ�����Ȩ
		int		GrapPutRight(void);
		//  �ͷſ���Ȩ
		void	ReleasePutRight(void);
		//	����ң������  
		int		YkCmdProcessing(YKPARA_STRU  *ykparap,char *tablename,uchar action,int ykcmd,uchar ykmode,uchar  programykf,
					char *opmachine,char *operatorname,uhlong optaskid,uchar eflag,char *groupname=NULL);
		//  ������������
		int		UpdownCmdProcessing(UPDOWNPARA_STRU  *updownp,uchar action,int ykcmd,uchar ykmode,
							uchar  programykf,char *opmachine,char *operatorname,uhlong optaskid);
		//  ����Ӧ���¼�
		int		YieldYkApEvent(int yksercode,YKQUEUE_STRU *ykqueuep);		//  ����ң������;
		int		SendYkCmd(int ykcmd,int groupno,int ykrtuno,int ykno,uchar ykaction);
		//  ��������������
		int     CancelYkPreset(YKQUEUE_STRU *ykqueuep,uchar entf=0);
		//  ����ң�ض���
		int	    YkQueueProcessing(YKQUEUE_STRU *ykqueuep);
		//	����TERMINALң��
		int		LockTerminalYk(YKQUEUE_STRU *ykqueuep);
		//  ����TERMINALң��
		int		UnlockTerminalYk(YKQUEUE_STRU *ykqueuep);
		//  ����TERMINALң��
		void    ClearTerminalYk(void);
		// �ж�ң������
		int     CheckYkCond(YKPARA_STRU  *ykparap,char *tabname,BASESTATUS_STRU *bp,int actype,uchar eflag);
		// �ж���������
		int     CheckUpdownCond(UPDOWNPARA_STRU  *updownp,int actype);
	public:
		Ykqueue(void);
		char	errorinfo[100];  
		int		YkCmd(char *name,char *tablename,uchar actype,int ykcmd,uchar ykmode,char *opmachine,
							char *operatorname,uhlong optaskid,uchar eflag);

		int		AutoYkCmd(char *name,char *tablename,uchar actype,int ykcmd,
					char *opmachine,char *operatorname,uhlong optaskid,char *groupname=NULL);

		int		ProgYk(char *name,char *tablename,uchar actype,int ykcmd,char *programname,uchar eflag=0);

		int		UpdownCmd(char *name,uchar actype, int ykcmd,uchar ykmode,char *opmachine,
							char *operatorname,uhlong optaskid);

		int		ProgUpdown(char *name,uchar actype,int ykcmd,char *programname);
		int		YkCheck(int ykrtuno,int ykno,uchar chkcode);
		int		YkResult(char  *tabname,char *objname,int action);
		int		YkQueueLoop(void);
		int     StopYk(char *name);		
		int     StopGroupYk(char *groupname);		
		int     StopAllYk(void);			
		int     CheckStopYk(char *name);
};

///////////////////////////////// ң�� ///////////////////////////////////////////////////
// ң������
typedef	struct {
		char    name[OBJECTNAME_LEN];		// ����
		int     securegrade;				// ��ȫ��
		short   terminalno;				// 
		short   groupno;				// 
		short   ytno;						//
		uchar   outtype;					// �������
		float   maxengval;					
		int		maxoutval;
		float   minengval;
		int     minoutval;
		float   highlimit;
		float   lowlimit;
		float   tolerance;
		short   execcmdtime;				//
		char    realhlmname[OBJECTNAME_LEN];
		char    realllmname[OBJECTNAME_LEN];
		char    condname1[OBJECTNAME_LEN];
		uchar   condstate1;
		char    condname2[OBJECTNAME_LEN];
		uchar   condstate2;
		char    entname[ENTNAME_LEN];	// �������
		char	ytlogic[PROCNAME_LEN];	// �����߼�
		uchar   lockytf;				// ����������־
} YTPARA_STRU;


// ң������
typedef struct {
		char      name[OBJECTNAME_LEN];			// ������
		char      describe[OBJDESCRIBE_LEN];	// ����
		int 	  securegrade;				    // ��ȫ��
		short     groupno;
		short	  terminalno;
		short	  ytno;
		uchar     outtype;
		short     execcmdtime;					// ִ������ʱ��
		float     tolerance;
		char      entname[ENTNAME_LEN];		    // �����
		char      opmachine[HOSTNAME_LEN];		// ����������
		char      operatorname[USERNAME_LEN];	// ����Ա��
		uhlong    optaskid;					    // ���������
		int			cmdcode;
		intertime yttime;					    //
		uchar     sendcmdf;						//
		float     setvalue;				     	//
		float	  curvalue;
		int		  outvalue;
} YTQUEUE_STRU;

class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Ytqueue : public NetDbg{
	static ComnLinkTable<YTQUEUE_STRU>	ytqueuetab;		// ң�����б�
	int  SearchYtQueue(char *name);
	int  CheckYtCond(YTPARA_STRU *ytparap,int cmdcode,float setvalue);
	int  YtCmdProcessing(YTPARA_STRU *ytparap,int cmdcode,float setvalue,char *opmachine,
							char *operatorname,uhlong optaskid);
	int  YieldYtApEvent(int ytsercode,YTQUEUE_STRU *ytqueuep,int failcode);
public:
	//Ytqueue(void);
	int	 YtCmd(char *name,int cmdcode,float setvalue,char *opmachine,
							char *operatorname,uhlong optaskid);
	int  YtQueueLoop(void);
};




///////////////////////////  ������״̬��� /////////////////////////////////
/*
typedef struct {
	short	terminalno;
	short	yxno;
	char	name[OBJECTNAME_LEN];
	char	describe[OBJDESCRIBE_LEN];
	int		chkenttype;  //����¼�����
	char	brkname[OBJECTNAME_LEN]; //��Ӧ���ش���
}  DMS_FAULTENTTYPE_STRU;


class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
DMSFaultEventType {
	static int  opentabflag;
	static Rdb_Tab  dmsfaulttable;
public:
	DMSFaultEventType(void);
	DMS_FAULTENTTYPE_STRU  *GetDMSFaultEntType(short terminalno,short yxno);
};
*/
/********************************* ����� **********************************/


   
// ������

class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Event : public ScdComnEvent {
	public:
		// Խ�������������������ʱ�䣬��ȫ����ֵ��
		int		 AddOverlimitEvent(char *tabname,char *objname,SYS_CLOCK *,int olgrade,float  olvalue);
		// ��̬���仯�����������������ʱ�䣬״̬��
		int		 AddMultiStaChgEvent(char *tabname,char *objname,SYS_CLOCK *,int  staval);
		// ״̬���仯�����������������ʱ�䣬�����־��״̬��
		int		 AddStateChangeEvent(char *tabname,char *objname,SYS_CLOCK  *chgclockp,int entflag,
					int  staval,float pdata=0.0,float qdata=0.0,float idata=0.0,
					uchar relclasstype=0,char *relname=NULL,char *reldes=NULL,uchar testflag=0);
		// ң�ء��������ң��/������־��ʱ�䣬����������������ȫ����  ��
		int		 AddYkandUpdownEvent(int ykupdownf,SYS_CLOCK  *ykclockp,char *tabname,
									char *objname,int securegrade,char *actbasemsg,
									int		 actype,char *procbasemsg,int  procserode,char *ophostname,
									char	 *operatorname,char  *entname);
		int 	 AddYtEvent(SYS_CLOCK  *ytclockp,char *name,uchar securegrade,
				 	int ytsercode,int failcode,float setvalue,float curvalue,
					int outvalue,char *ophostname,char *operatorname,char *entname);
		//�˹���������
		int      AddMansetEvent(char* hostname,char *username,char* tabname,char* objname,uchar securegrade,
							uchar setflag,double setval,SYS_CLOCK* clockp);
		//�������
		int      AddWfbsEvent(char* tabname,char* objname,uchar securegrade,uchar setflag, SYS_CLOCK* apentclock);

		int      AddSetSpecialflagEvent(char* hostname,char* username,uchar setflag,CARD_RCD_STRU *cardrcdp,SYS_CLOCK *clockp);
		//������·�������
		int      AddBreakReplaceEvent(char* hostname,char* username,char* breakname,char * break_replaced,uchar mode,SYS_CLOCK *clockp);
		int      AddDmsDiagnoseEvent(int faulttype,char* startbreakname,char* startftuname,char* endbreakname,char* endftuname,SYS_CLOCK* clockp);
		//�����������������
//		int      AddDmsFaultEvent(DMS_FAULTENTTYPE_STRU *dmsfltentypep,SYS_CLOCK *clockp);
		//int      AddTerminalEvent(char *substation,char *terminalname,char *terminaldes,int state,int security);
		int      AddTerminalEvent(char *terminalname,char *terminaldes,int state,short channelno);
		int      AddChannelEvent(char *channelname,char *channeldes,int channel_no, bool main_back);
//		int      AddSubstationEvent(char *substation,int almtype);
		int      AddYxTrembleEvent(char *objname,short objtype,int tremblenums);//����ң�Ŷ�������
};

/******************************* �û����̱� **********************************/

#define  CALSTMODE_NULL              0
#define  CALSTMODE_INTERV            1
#define  CALSTMODE_ACCURATE          2
#define  CALSTMODE_DBCHANGE          3
#define  CALSTMODE_STACHANGE         4
#define  CALSTMODE_SGALARM           5
#define  CALSTMODE_HDBACCURATE       6
#define  CALSTMODE_HISDBCHANGE       7

#define  CALSTMODE_FIELDUPD          11

#define  CALSTMODE_FOREVERRUN        127



#define  CALTRIGGERNUMS              3
#define  CALINPARANUMS               10

typedef struct {
		short       no;//���                         //add
		short       groupno;//�������                //add
	    char		callprocname[PROCNAME_LEN];//�û�������
		uchar		startmode;//ִ��ģʽ
		short		everyminute;//�������
		uchar		everysecond;//�����
		char		atmonth;//��ʱ��
		char		atday;//��ʱ��
		char		athour;//��ʱСʱ
		char		atminute;//��ʱ����
		char		trigger[CALTRIGGERNUMS][48];//������
		char		inargv[CALINPARANUMS][30];//�������
		uchar       usef;//ʹ�ñ�־                   //add
		int			returncode;//������
		double		returnvalue;//����ֵ
		double		lastvalue[CALTRIGGERNUMS];//����ֵ
		intertime	lastproctime;//ִ��ʱ�� 
} USERPROCPARA_STRU;
    
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
User_Procedure {
		static  int       opentabf;
		static  Rdb_Tab  calcultparatab;	
		int		CheckTimeCond(USERPROCPARA_STRU *calp);
		int		CheckHdbTimeCond(USERPROCPARA_STRU *calp);
		int		CheckDbChgCond(USERPROCPARA_STRU *calp);
		int		SplitDbFldName(char *dbfieldname,char *tabname,char *objname,char *fldname);
		int		CheckFldUpdCond(USERPROCPARA_STRU *calp,char *tabname,char *objname,char *fldname);
		int		CheckStaChgCond(USERPROCPARA_STRU *calp,char *tabname,char *objname);
		int		CheckSgAlarm(USERPROCPARA_STRU *calp,char *objname);
		int		ExecOneProc(int groupno,USERPROCPARA_STRU *calp);
	public:
		User_Procedure(void);
		int		InitUserprocTab(void);//��ʼ��
		int		ExecTimerProc(void);//ִ�ж�ʱ����
		int		ExecHdbTimerProc(void);
		int		ExecDbChgProc(void);//ִ�п�ı䴥������
		int		ExecFldUpdProc(char *tabname,char *objname,char *fldname);//ִ����ı䴥������
		int		ExecStaChgProc(char *tabname,char *objname);//ִ��״̬�ı䴥������
		int		ExecAccidentAlarmProc(char *objname);//ִ���¹ʸ澯����
    };





/************************* �ڵ�Ȩ�ޱ� ********************/
int  CheckUserPassword(char *um,char *password);


typedef  struct {
	char	hostname[HOSTNAME_LEN];		//������
	char	username[USERNAME_LEN];		//�û���
	uchar   modifyjhcurve;              //�޸ļƻ�����
	uchar   modifyrealdata;             //�޸�ʵʱ����
	uchar   modifyhiscurve;             //�޸���ʷ����
	uchar   modifyhisent;               //�޸���ʷ����
	uchar   modifygraph;                //�޸�ͼ��
	uchar   modifynetpara;              //�޸��������
	uchar   modifydbpara;               //�޸����ݿ����
	uchar   modifytable;                //�޸ı���
	uchar   viewjhcurve;                //�鿴�ƻ�����
	uchar   viewrealdata;               //�鿴ʵʱ����
	uchar   viewhiscurve;               //�鿴��ʷ����
	uchar   viewhisent;                 //�鿴��ʷ����
	uchar   viewgraph;                  //�鿴ͼ��
	uchar   viewnetpara;                //�鿴�������
	uchar   viewdbpara;                 //�鿴���ݿ����
	uchar   viewtable;                  //�鿴����
	uchar   viewxbdata;                 //�鿴г������
	uchar   viewlbdata;                 //�鿴¼������
	uchar   viewdebuginfo;              //�鿴������Ϣ
	uchar   viewrawcode;                //�鿴ͨѶԭ��
	uchar   ykright;                    //ң��
	uchar   updownright;                //ң��
	uchar   superykright;               //����ң��
	uchar   stopfresh;                  //ͣ��
	uchar   putcard;                    //����
	uchar   entconfirm;                 //����ȷ��
	uchar   netchecktime;               //�����ʱ
	uchar   switchhost;                 //�л�����
	uchar   switchchannel;              //�л�ͨ��
	uchar   computerprot;               //΢������
	uchar   modifyprotvalue;            //�޸Ķ�ֵ
	uchar   protrunstop;                //����Ͷ��
	uchar   modifysetvalnum;            //�޸Ķ�ֵ����
	uchar   protreserverd1;             //΢������Ԥ��1
	uchar   protreserverd2;             //΢������Ԥ��2
	uchar   protreserverd3;             //΢������Ԥ��3
	uchar   stoprtu;                    //ͣ��RTU
	uchar   controlmnp;                 //����ģ����
	uchar   lockforedata;               //����ǰ������
	uchar   paradownload;               //��������
	uchar   paraupload;                 //��������
	uchar   guangzipai;                 //������
	uchar	opright[MAXSECUREGRADE];	// ����Ȩ�ޣ�=1Ϊ����Ӧ��ȫ������Ȩ��// ��securegrade ��λȡ��
} USEROPERMODE_STRU;
   
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
UserOperMode : public NetDbg{
		static  int  opentabf;
		static  Rdb_Tab useropmodetab;
	public:
		UserOperMode(void);
		// �������������û����������ָ����ȫ���Ĳ���Ȩ��
		USEROPERMODE_STRU  *GetUserOperMode(char *hm, char *um);
		int CheckSecureGrade(USEROPERMODE_STRU *uopm, int securegrade);
		int CheckSecureGrade(char *hm, char *um, int securegrade);
};


//���п����������ṹ
typedef struct {
       char serialname[OBJECTNAME_LEN];
       char describe[OBJDESCRIBE_LEN];
	   short groupno;
       uchar ykmode;
       uchar interval;                       //add
} SERIALYKGRP_STRU;

//���п��ƶ��������ṹ
typedef struct {
       char  serialname[OBJECTNAME_LEN];
       char  breakname[OBJECTNAME_LEN];
       short ykorder;
	   uchar actype;
       uchar condition;
//     uchar ykstyle;
//     short interval;
}SERIALYKOBJ_STRU;



typedef struct  {
	int   groupno;
	char  tablename[OBJECTNAME_LEN];
	char  objname[OBJECTNAME_LEN];
	uchar actype;
	uchar condition;
    short interval;
	uchar sendcmdf;
}  SERIALYKOBJ_UNIT_STRU;


typedef struct  {
	char		groupname[OBJECTNAME_LEN];
	char		ophostname[HOSTNAME_LEN];
	char		operatorname[USERNAME_LEN];
	hlong		optaskid;
	uchar		stepflag;
	uchar		contflag;
	int			objnums;
	int			curserno;
	intertime	ctrltime;
	SERIALYKOBJ_UNIT_STRU	*serialykobj;
}  SERIAL_GROUP_CTRL_STRU;


#define  MAX_SERIAL_GROUP_NUMS  30

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
SerialYK {
	static SERIAL_GROUP_CTRL_STRU  ykgroup[MAX_SERIAL_GROUP_NUMS];
	static unsigned char  lockflag;
	void ClearSerialYK(SERIAL_GROUP_CTRL_STRU *,char notsendserialendmesg=0);
public:
	//SerialYK(void)
	int AddGroup(char *ophostname,char *operatorname,uhlong taskid,
			char *groupname,int serialtype,int mode,int nums,SERIALYKOBJ_UNIT_STRU *serobjp);
	int StopGroup(char *ophostname,char *operatorname,uhlong taskid,char *groupname);
	int PauseGroup(char *ophostname,char *operatorname,uhlong taskid,char *groupname);
	int ContinueGroup(char *ophostname,char *operatorname,uhlong taskid,char *groupname);
	int ProcYkResult(char *objname,int okf);
	int ExecSerialYK(void);
	int StopAllGroup();
	int SendSerialYKMsg(char *hm,char *um,uhlong tid,char *groupname,int curstate,char *pauseobjname,int errorcode=0);

};

/////////////////////////////////////////
// �����־�Ƽ�¼��
/////////////////////////////////////////

#ifdef WIN32
_SCD_DLLPORT 
#endif
int SetBusCheckFlag(int cardtype,char *busname,int flag);

#ifdef WIN32
_SCD_DLLPORT 
#endif
int CheckDevCardCond(int devtype,int cardtype,char *devname,int flag);


class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ProcCard {
	static int  opentabflag;
	static Rdb_Tab  cardtable;
	Rdb_PreMalloc	cardmalloc;
public:
	ProcCard(void);
	~ProcCard(void) {cardmalloc.freespace();}
	int GetOneCard(short objtype,char *objname,int cardtype,CARD_RCD_STRU *cardp);
	int GetAllCard(short objtype,char *objname,int *nums,CARD_RCD_STRU **cardp);
	int InitBusCheckFlag(void);
};

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// ��վͬYC��YX�Ĺ�ϵ ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	char substname[SUBSTNAME_LEN];
	int  ana_stno;
	int  ana_nums;
	int  brk_stno;
	int  brk_nums;
	int  sw_stno;
	int  sw_nums;
	int  prot_stno;
	int  prot_nums;
	int  sta_stno;
	int  sta_nums;
	int  dmssta_stno;
	int  dmssta_nums;
}  SUBSTATION_RELATION_STRU;

class	
#if defined(WIN32)
_SCD_DLLPORT  
#endif
SubstationRelation {
	static SUBSTATION_RELATION_STRU	*substrelidx;
	static intertime				substupdtime;
	static intertime				brkupdtime;
	static intertime				swupdtime;
	static intertime				protupdtime;
	static intertime				staupdtime;
	static intertime				anaupdtime;
	static int						substnums;
	static int						ananums;
	static int						brknums;
	static int						swnums;
	static int						protnums;
	static int						stanums;
	static BASEANALOG_STRU			**ana_addr;
	static BASESTATUS_STRU			**brk_addr;
	static BASESTATUS_STRU			**sw_addr;
	static BASESTATUS_STRU			**prot_addr;
	static BASESTATUS_STRU			**sta_addr;
	static BASESTATUS_STRU			**dmssta_addr;
public :
	SubstationRelation(void);
	void InitAllIndex(void);
	void InitBreakIndex(void);
	void InitSwitchIndex(void);
	void InitProtsigIndex(void);
	void InitStatusIndex(void);
	void InitDmsStatusIndex(void);
	void InitAnalogIndex(void);
	void CheckAllRelation(void);
	BASEANALOG_STRU **GetAnalogRelation(int substrcdno,int *relnums);
	BASESTATUS_STRU **GetBreakRelation(int substrcdno,int *relnums);
	BASESTATUS_STRU **GetSwitchRelation(int substrcdno,int *relnums);
	BASESTATUS_STRU **GetProtsigRelation(int substrcdno,int *relnums);
	BASESTATUS_STRU **GetStatusRelation(int substrcdno,int *relnums);
	BASESTATUS_STRU **GetDmsStatusRelation(int substrcdno,int *relnums);
};




#endif
