//#include "sybase.h"

#include "db/dbapi.h"
#if defined(__hpux)
#include <sys/termios.h>
#endif

#if defined(WIN32)
#include <io.h>
#elif defined(__unix) || defined(__linux)
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#endif

//#include <sys\stat.h>

#if defined(WIN32)
#include <conio.h>
#include <direct.h>
#elif defined(__unix) || defined(__linux)
#include <ctype.h>
//#include <curses.h>
#include <unistd.h>
long filelength(char *filename )
{
	struct stat rdbfstat;
	int ret=stat(filename,&rdbfstat);
	if(ret!=0)return(-1);
	return(rdbfstat.st_size);
}
#endif

#if defined(__linux)
#include <termios.h>
#endif



#define MAXPARA		10	// ����������
#define MAXPARALEN		100	// ÿ����������󳤶�

#define FLAG_ERROR	-1		// ��������
#define FLAG_NO		0		// �޲���
#define FLAG_COPY	1		// ����
#define FLAG_DEL	2		// ɾ��
#define FLAG_COMPARE	3	// �Ƚ�
#define FLAG_OUTFILE	4	// ��->�ļ�
#define FLAG_INFILE	5		// �ļ�->��
#define FLAG_CLEAN	6		// ��ձ�
#define FLAG_DROP	7		// ɾ����
#define FLAG_DISP	8		// ��ʾ��Ϣ
#define FLAG_ALL	9		// ��ʾ���а���
#define FLAG_SCADA	10		//SCADA����׷��
#define FLAG_DUMP	11		//��־���
#define FLAG_CHECK	12		//������������Լ��
#define FLAG_DFHIS	13		//����������ʷ���ݵ���

char strstdin[255];

#define EX_MAXSTRINGLEN		255
#define MAXCPERRTABNUM  100
#define MAXINROWNUM		1000		//ÿ�ε�����������
#define MAXOUTROWNUM		5000	//ÿ�ε������������
#define MAXSTATIONNUM		512		//ϵͳ��������վ��
#define DF_HIS				1		//�������ӵ���ʷ���ݿ�

DBSingle dbsingle;
typedef  struct {
	char  name[24];				//����
	char  desc[40];				//����
	short type;					//����
	char  devname[24];			//�豸����
	char  logicalflag;			//�Ƿ����
}CHECK_VALUE;
typedef  struct {
	short typevalue;			//����ֵ
	char typedaima[24];			//���ʹ���
}SCD_TYPEVALUE;
FUNC_STR tmpfunc;
CS_DATAFMT *tmpdatafmt;
FILE *tmpstream=NULL;
long  recordnum;
int year,month;
int dbtype=DBTYPE_SYBASE;			//
char stationname[MAXSTATIONNUM][24];
int stationnum,scdtypenum;
SCD_TYPEVALUE *scdtypevalue=NULL;

/***************************************************************************/
char	getpass( char* lpszBuffer )
{
#ifndef WIN32
	char *ptr = NULL ;
	sigset_t  sig , sigsave ;
	struct termios term , termsave;
	FILE 	*fp;
	int	c  ;

	fp = fopen( ctermid( NULL ) , "r+" );

	if ( fp == NULL  ) return FALSE;

	int fno = fileno( fp ) ;

	setbuf( fp , NULL );
	sigemptyset(&sig);
	sigaddset(&sig , SIGINT);
	sigaddset(&sig , SIGTSTP);
	sigprocmask(SIG_BLOCK,&sig,&sigsave );

	tcgetattr( fno, &termsave );
	term = termsave ;
	term.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHONL);
	tcsetattr( fno , TCSAFLUSH, &term );

	ptr = lpszBuffer ;

	while ( (c=getc( fp ))!= EOF && c!= '\n' )
	{
		if ( ptr < lpszBuffer+256 ) *ptr++=c;
	}

	*ptr = 0 ;

	putc( '\n' , fp );

	tcsetattr( fno , TCSAFLUSH , &termsave );
	sigprocmask(SIG_SETMASK , &sigsave , NULL );

	fclose(fp);
#else
		int i = 0 ; 
		char letter ;
		do
		{
			letter=getch();
			lpszBuffer[i]=letter;
			i++;
		}
		while (letter!='\r');

		lpszBuffer[i-1]='\0';

#endif

		return 1 ;
}
/***************************************************************************/
/***************************************************************************/
  /* ���ַ�������Ŀո񡢻س������С�TAB�Ƚص� */
void db_TrimRight(char *p)
{
	char *psz=p;
	char *pszLast=NULL;
	while(*psz != '\0')
	{
		if(isspace((unsigned char)(*psz)))
		{
			if(pszLast==NULL)
				pszLast=psz;
		}
		else pszLast=NULL;
		psz++;
	}
	if(pszLast!=NULL) *pszLast='\0';
}

/***************************************************************************/
  /* ���ַ���ǰ��Ŀո񡢻س������С�TAB�Ƚص� */
void db_TrimLeft(char *p)
{
	int len=strlen(p);
	char *psz=p;
	char *pszLast=NULL;
	while(*psz != '\0')
	{
		if(isspace((unsigned char )(*psz))) pszLast=psz+1;
		else break;
		psz++;
	}
	if(pszLast!=NULL)
		memmove(p,pszLast,len-(pszLast-p)+1);
}



int STDCALL proc(int* row,void* ppt,void* client)//��dbi�Ļص�����
{
	int nRow = * row;
	char* pt = (char*)ppt;
	char strvalue[255];

	int nCol = tmpfunc.colnum;
	VAL_UNION* pdata = (VAL_UNION*)malloc(sizeof(VAL_UNION)*nRow*nCol);
	fprintf(stderr,"-");
	int ptcnt = 0;
	for(int i=0; i<nRow; i++)
	{
		ptcnt = tmpfunc.recorderlen*i;
		for(int j=0; j<nCol; j++)
		{
			if(tmpfunc.col[j].select!=YES)	continue;

			CS_CHAR* charpt = 0;
			switch (tmpdatafmt[j].datatype) 
			{
				case CS_DATETIME_TYPE:
					charpt=(CS_CHAR*)((CS_CHAR*)pt+ptcnt);
					memcpy(&(pdata[i*nCol+j].charvalue),charpt,tmpdatafmt[j].maxlength-1);
					ptcnt+=tmpdatafmt[j].maxlength;
					strncpy(strvalue,pdata[i*nCol+j].charvalue,tmpdatafmt[j].maxlength-1);
					db_TrimLeft(strvalue);
					db_TrimRight(strvalue);
					break;
				case CS_CHAR_TYPE:
					charpt=(CS_CHAR*)((CS_CHAR*)pt+ptcnt);
					memcpy(&(pdata[i*nCol+j].charvalue),charpt,tmpdatafmt[j].maxlength-1);
					ptcnt+=tmpdatafmt[j].maxlength;
					strncpy(strvalue,pdata[i*nCol+j].charvalue,tmpdatafmt[j].maxlength-1);
					if (tmpdatafmt[j].maxlength>1)//add 2004 11
					strvalue[tmpdatafmt[j].maxlength-1]=0;//add 2004 11
					db_TrimLeft(strvalue);
					db_TrimRight(strvalue);
					break;
				case CS_TINYINT_TYPE:
				case CS_BIT_TYPE:
					pdata[i*nCol+j].tinyvalue=*(CS_TINYINT*)((CS_CHAR*)pt+ptcnt);
					ptcnt+=tmpdatafmt[j].maxlength;
					sprintf(strvalue,"%d",pdata[i*nCol+j].tinyvalue);		
					break;
				case CS_SMALLINT_TYPE:
					if ((ptcnt % sizeof(CS_SMALLINT) ) != 0) ptcnt++;
					pdata[i*nCol+j].smallvalue=*(CS_SMALLINT*)((CS_CHAR*)pt+ptcnt);
					ptcnt+=tmpdatafmt[j].maxlength;
					sprintf(strvalue,"%d",pdata[i*nCol+j].smallvalue);		
					break;
				case CS_INT_TYPE:
					if ((ptcnt % sizeof(CS_INT)) != 0)
						ptcnt=(ptcnt/sizeof(CS_INT) +1)*sizeof(CS_INT);
					pdata[i*nCol+j].intvalue=*(CS_INT*)((CS_CHAR*)pt+ptcnt);
					ptcnt+=tmpdatafmt[j].maxlength;
					sprintf(strvalue,"%d",pdata[i*nCol+j].intvalue);		
					break;
				case CS_REAL_TYPE:
					if ((ptcnt % sizeof(CS_REAL)) != 0)
						ptcnt=((ptcnt/sizeof(CS_REAL))+1)*sizeof(CS_REAL);
					pdata[i*nCol+j].realvalue = *(CS_REAL*)((CS_CHAR*)pt+ptcnt);
					ptcnt+=tmpdatafmt[j].maxlength;
					sprintf(strvalue,"%g",pdata[i*nCol+j].realvalue);
					break;
				case CS_FLOAT_TYPE:
					if ((ptcnt % sizeof(CS_FLOAT)) != 0)
						ptcnt=((ptcnt/sizeof(CS_FLOAT))+1)*sizeof(CS_FLOAT);
					pdata[i*nCol+j].floatvalue = *(CS_FLOAT*)((CS_CHAR*)pt+ptcnt);
					ptcnt+=tmpdatafmt[j].maxlength;
					sprintf(strvalue,"%lg",pdata[i*nCol+j].floatvalue);
					break;
				default:
					ptcnt+=tmpdatafmt[j].maxlength;
			}//switch
			fputs(strvalue,tmpstream);
			memset(strvalue,0,strlen(strvalue));//���strvalue
			fputs("\t",tmpstream);
		}//for j
		fputs("\n",tmpstream);
		recordnum++;
	}//for i

	if(pdata)	free(pdata);
	pdata = 0;

	return 1;
}

#define HISDATANUM 5
#define CHECKTABLENUM 5

char *datatype[HISDATANUM]=//��ʷ���ݵĽṹ
{
	"�������͵���ʷ����",
	"��������",
	"��ʷ����Ͳ�����¼",
	"׷������",
	"�뼶��������"
};
char *checktable[CHECKTABLENUM]=//������У��Ľṹ
{
	"��������",
	"SCD�豸������",
	"ң�������",
	"ң�Ų�����",
	"��Ȳ�����"
};

class CDbOperate
{
public:
	char argc;				// ��ڲ�������
	char argv[MAXPARA][MAXPARALEN];	// ��ڲ����ַ���

	int OpeType;		// ��������
	int bHelpFlag;		// ������ʶ
	int bPrompt;		// ����ÿ�����Ƿ���ʾ
	int bAutoOpe;		// ����������ȷ�ϣ�ֱ��ִ��
	int bAllTables;		// =1�����ļ��������б�=0���ֱ���Ҫ����
	int oneallflag;		// =1�Ա������=2�Կ����
	int bInputUser;		// �Ƿ�ָ���û���
	char cUser[20];		// ָ�����û���
	char cPasswd[20];	// ָ�����û�����
	char mode[2];            //ģʽ��0 ׷�� 1 ���׷��
	int HistableNameCount;	// histable����ʷ��ĸ���������oracle�ȵ������б���ʷ��
	int HistableNameLen;
	char *pHistableName;
	int UnchangedTableNameCount;	// ����Ҫ����ı�ĸ���
	int UnchangedTableNameLen;
	char *pUnchangedTableName;
private:
	int get_input_char(char *strdisp,int flag=0);	// ���� Y,N,Q (yes,no,quit)
	int db_start(int serverno,int anotherflag=0);	// �������ݿ�
	int get_serverno();								// ȡserver��
	int get_dbname(char *dbname);					// ȡ���ݿ���
	int get_tablename(char *tablename);				// ȡ����
	int get_filename(char *filename);				// ȡ�ļ���
	int realize_correct_onetable(CS_SMALLINT serverno,CS_SMALLINT sserverno,CS_CHAR *tablename,CS_CHAR *dbname);
	int realize_correct_table(CS_SMALLINT fserverno,CS_SMALLINT sserverno,CS_CHAR *dbname);
	int check_root_password(int srvno);				// �������
	int do_in_hisdata();	/* ��ʷ���� */
	int do_in_para();		/* ���� */
	int realize_in_file(FUNC_STR *func,char *filename,int null_num=0,int id=-1);
	int do_out_hisdata();	/* ��ʷ���� */
	int do_out_para();		/* ���� */
	int realize_out_file(FUNC_STR *func,char *filename,int null_num=0,int id=-1);
	int load_histable_name();	//��histable��ȡ��ʷ����
	int load_unchangedtable_name();	//��ȡ����Ҫ����ı���
	int check_histable_name(char *tbname);	//����Ƿ���ʷ��
	int check_unchangedtable_name(char *tbname);	//����Ƿ�Ϊ����Ҫ�����
	int check_tmptable_name(char *tbname);  //����Ƿ���ʱ����ʱ��Ϊ tmp_????
	int do_cmp_table(CS_CHAR *dbname,CS_CHAR *tablename,int id=-1);	//�Ƚ�˫��������һ����
	int do_cmp_db(CS_CHAR *dbname);		//�Ƚ�˫��������һ����
	int do_disp_table(CS_CHAR *dbname,CS_CHAR *tablename);	//��ʾ�����Ϣ
	int	check_table(CS_CHAR *tablename,int dataflag);	//�Ƚϱ��м�¼����������
	void CheckNameLogic(CHECK_VALUE *value,int checknum,int dataflag);//�����������
public:
	CDbOperate();
	~CDbOperate();
	void DoOperate();		// ����
	int GetOperateType();	// ȡ��������
	void DispHelp();		// ��ʾ������Ϣ
		
		/*
		  ������������� [-P  -NQ -USR�û� -PWD����]
		     -P      ���������������б�ʱ����ÿ���������ʾ");
		     -NQ	 ��ֱ��ִ�У�������������ȷ��ɾ��
		     -USR�û����Դ��û���¼���ݿ⣬�޴�ѡ������modeldb�û���¼ 
		     -PWD����û�����
		*/

	int do_cmp();
		/* �Ƚ�˫������һ����");
		   dbmt cmp -ѡ��");
		     -T����  ���Ƚϵı�
		     -D����  ���ȽϵĿ�
		     
		*/
	int do_copy();
		/* �ڷ�����֮�临��ĳ�����ĳ�����е����б�");
		   dbmt copy -Sn <-T���� | -D����> [-P -debug -NQ -USR�û� -PWD����]");
		     -Sn     ��-S1 �� -S2 , Դ������Ϊ������1���������2��");
		     -T����  ������һ����");
		     -D����  �����ƿ��е����б���-T -Dͬʱ����ʱ��-T��Ч,");
			
			 -debug  ����ʾ������Ϣ
		*/
	int do_delete();
		/* ɾ����ʷ���� 
		   dbmt del -Sn -B��ʼʱ�� -E��ֹʱ�� [-DTn -NQ -USR�û� -PWD����]
		   -Sn			: -S1��-S2 
		   -B��ʼʱ��	: ��ʽΪ���£���2002��1�� -B200201
		   -E��ֹʱ��	:
					 ����Ϊ��ѡ��
		   -DTn			: ��ѡ��ɾ�����������࣬������ѡ������ѡ����ʾ
		*/
	int do_out_file();
		/* ����->�ļ�
		    ��������ļ���Ϊ�����
		    ��ʷ��������Ϊ��λ������Ϊ ����yyyymm ,
			   �����������ʷ�����200201��monyc200201, 
			   �������������µĳ��⣬�� yc200201,dayyc200201
		   ��ʷ���ݣ�
			 dbmt out -HD -Sn -YM���� [-PATHĿ¼ -DTn -debug -NQ -USR�û� -PWD����]
				-HD     : ��ʷ����
				-Sn		: -S1 ��-S2
				-YM		: ���£�yyyymm ��2002��1��Ϊ 200201
				-PATHĿ¼: �ļ��Ĵ��Ŀ¼�������Ѿ�����
			    -DTn	: ��ѡ���������������࣬������ѡ������ѡ����ʾ
		   ������
		 	 dbmt out -PA <-D���� | -T����> [-PATHĿ¼ -P -debug -NQ -USR�û� -PWD����]
				-PA     : ������
				-Sn		: -S1(nbdb) ��-S2(nbdbb)
				-D����  : �������б� modeldb
				-T		: ĳ���� ����.dbo.����
				-PATHĿ¼: �ļ��Ĵ��Ŀ¼�������Ѿ�����
		*/
	int do_in_file();
		/* �ļ�->����
		    ��������ļ���Ϊ�����
		    ��ʷ��������Ϊ��λ������Ϊ ����yyyymm ,
			    �����������ʷ�����200201��monthyc200201, 
			    �������������µĳ��⣬�� yc200201,dayyc200201
		   ��ʷ���ݣ�
		 	 dbmt in -HD -Sn -YM���� [-PATHĿ¼ -DTn -debug -NQ -USR�û� -PWD����]
				-HD     : ��ʷ����
				-Sn		: -S1 ��-S2
				-YM		: ���£�yyyymm ��2002��1��Ϊ 200201
				-PATHĿ¼: �ļ��Ĵ��Ŀ¼�������Ѿ�����
				-DTn	: ��ѡ��������������࣬������ѡ������ѡ����ʾ
		   ������
			 dbmt in -PA <-D���� | -T����> [-PATHĿ¼ -P -debug -NQ -USR�û� -PWD����]
				-PA     : ������
				-Sn		: -S1(nbdb) ��-S2(nbdbb)
				-D����  : �������б� modeldb
				-T		: ĳ���� ����.dbo.����
				-PATHĿ¼: �ļ��Ĵ��Ŀ¼�������Ѿ�����
		*/
	int do_clean();
		/* ���ĳ�����ĳ�����е����б�");
		    dbmt clean -Sn <-T���� | -D����> [-P -debug -NQ -USR�û� -PWD����]");
		     -Sn     ��-S1 �� -S2 , Դ������Ϊ������1���������2��");
		     -T����  �����һ����");
		     -D����  ����տ��е����б���-T -Dͬʱ����ʱ��ֻ��-T��Ч");
		*/
	int do_drop();
		/* ɾ��ĳ�����ĳ�����е����б�");
		    dbmt drop -Sn <-T���� | -D����> [-P -debug -NQ -USR�û� -PWD����]");
		     -Sn     ��-S1 �� -S2 , Դ������Ϊ������1���������2��");
		     -T����  ��ɾ��һ����");
		     -D����  ��ɾ�����е����б���-T -Dͬʱ����ʱ��ֻ��-T��Ч");
		*/
	int do_disp();
		/* ��ʾ���ݿ���Ϣ");
		     dbmt disp -Sn");
		     -Sn     ��-S1 �� -S2 , Դ������Ϊ������1���������2��");
		*/
	int do_scada();
		/*SCADA����׷���ļ��ĵ���
		*/
	int do_dump();
		/*���ݿ���־���
		*/
	int do_check();
		/*������������Լ��
		*/
	int do_dfhis();
		/*
		*/
	int realize_in_file_ex(FUNC_STR *func,char *filename,CHECK_VALUE *change,int null_num=0,int id=-1);
		/*
		*/
};
CDbOperate::CDbOperate()
{
	OpeType=FLAG_ERROR;
	bHelpFlag=0;
	bPrompt=0;
	bAutoOpe=0;
	bAllTables=0;
	oneallflag=-1;
	bInputUser=0;
	cUser[0]=0;
	cPasswd[0]=0;
	HistableNameCount=0;
	pHistableName=NULL;
}

CDbOperate::~CDbOperate()
{
	if(pHistableName) free(pHistableName);
}

void CDbOperate::DoOperate()
{
	if( GetOperateType()==0 )
	{
		DispHelp();
		return;
	}

	switch(OpeType)
	{
	case FLAG_COMPARE:	/*�Ƚ�˫���ݿ�*/
		do_cmp();
		break;
	case FLAG_COPY:	/* ��˫������֮�临������ */
		do_copy();
		break;
	case FLAG_DEL:	/* ɾ����ʷ���� */
		do_delete();
		break;
	case FLAG_OUTFILE:
		do_out_file();
		break;
	case FLAG_INFILE:
		do_in_file();
		break;
	case FLAG_CLEAN:
		do_clean();
		break;
	case FLAG_DROP:
		do_drop();
		break;
	case FLAG_DISP:
		do_disp();
		break;
	case FLAG_SCADA:
		do_scada();
		break;
	case FLAG_DUMP:
		do_dump();
		break;
	case FLAG_CHECK:
		do_check();
		break;
	case FLAG_DFHIS://200508
		do_dfhis();
		break;
	default:
		DispHelp();
		break;
	}
}

int CDbOperate::GetOperateType()
{
	int i;
	int len;
	char cPathName[255];	// �ļ���ŵ�Ŀ¼

	OpeType=FLAG_ERROR;
	bHelpFlag=0;
	bPrompt=0;
	bAutoOpe=0;

	if(argc<2)		//�޲���
	{
		OpeType=0;
		bHelpFlag=1;
		return 0;
	}
	
    for (i=1;i<argc;i++)
		if (strcmp(argv[i],"/?")==0 || strcmp(argv[i],"help")==0 )
		{
			bHelpFlag=1;
			if(i==1) 
			{
				OpeType=FLAG_NO;
				return 0;
			}
		}
    for (i=1;i<argc;i++)
		if (strcmp(argv[i],"helpall")==0 )
		{
			bHelpFlag=2;
			if(i==1) 
			{
				OpeType=FLAG_ALL;
				return 0;
			}
		}
    for (i=1;i<argc;i++)	/* ��ÿ������ʾ��ʾ��Ϣ */
		if(strcmp(argv[i],"-P")==0)
			{	bPrompt=1; break;}
    for (i=1;i<argc;i++)	/* �Ƿ���������ȷ�� */
		if(strcmp(argv[i],"-NQ")==0)
			{	bAutoOpe=1; break;}
    for (i=1;i<argc;i++)	/* �Ƿ�ÿ������������ */
		if(strcmp(argv[i],"-A")==0)
			{	bAllTables=1; break;}
	for (i=1;i<argc;i++)
		if(strncmp(argv[i],"-PATH",5)==0)
		{
			strcpy(cPathName,argv[i]+5);
			len=strlen(cPathName);
			if(len>1)
			{
				if(chdir(cPathName)!=0)
				{
					fprintf(stderr,"\n-PATH������Ŀ¼ %s �����ڡ�\n",cPathName);
					return 0;
				}
			}
			break;
		}

	int bflag=0;
	for (i=1;i<argc;i++)
	{
		if(strncmp(argv[i],"-USR",4)==0)
		{
			if(argv[i][4]!=0)
				strcpy(cUser,argv[i]+4);
			bflag++;
		}
		else if(strncmp(argv[i],"-PWD",4)==0)
		{
			if(argv[i][4]!=0)
				strcpy(cPasswd,argv[i]+4);
			bflag++;
		}
       else if(strncmp(argv[i],"-M",2)==0)
		{
			if(argv[i][2]!=0)
				strcpy(mode,argv[i]+2);
		}
	}
	if(bflag==2) bInputUser=1;

	if(strcmp(argv[1],"cmp")==0) OpeType=FLAG_COMPARE;
	else if(strcmp(argv[1],"copy")==0) OpeType=FLAG_COPY;
	else if(strcmp(argv[1],"del")==0) OpeType=FLAG_DEL;
	else if(strcmp(argv[1],"out")==0) OpeType=FLAG_OUTFILE;
	else if(strcmp(argv[1],"in")==0) OpeType=FLAG_INFILE;
	else if(strcmp(argv[1],"clean")==0) OpeType=FLAG_CLEAN;
	else if(strcmp(argv[1],"drop")==0) OpeType=FLAG_DROP;
	else if(strcmp(argv[1],"disp")==0) OpeType=FLAG_DISP;
	else if(strcmp(argv[1],"scada")==0) OpeType=FLAG_SCADA;
	else if(strcmp(argv[1],"dump")==0) OpeType=FLAG_DUMP;
	else if(strcmp(argv[1],"check")==0) OpeType=FLAG_CHECK;
	else if(strcmp(argv[1],"dfhis")==0) OpeType=FLAG_DFHIS;//200508
	else 
	{
		OpeType=FLAG_ERROR;
		bHelpFlag=1;
		return 0;
	}
	if(bHelpFlag>0) return 0;
	else return 1;
}

void CDbOperate::DispHelp()
{
	if(bHelpFlag==0) return;

	switch(OpeType)
	{
	case FLAG_ERROR:
		fprintf(stderr,"\n �޴��������dbmt help ��ѯ����!");
		break;
	case FLAG_NO:
        fprintf(stderr,"ϵͳ����Ա����: ");
		fprintf(stderr,"\n dbmt < copy | del | out | in | cmp | scada | dump | disp | drop | clean | check> [help] ");
		fprintf(stderr,"\n");
		fprintf(stderr,"\n ���");
		fprintf(stderr,"\n dbmt out  : ���Ʋ��������ʷ���� -> �ļ�");
		fprintf(stderr,"\n dbmt in   : ���ļ�װ����������ʷ���ݵ����ݿ�");
		fprintf(stderr,"\n dbmt copy : ��˫������֮�临������");
		fprintf(stderr,"\n dbmt del  : ɾ��ĳʱ�ڵ���ʷ����");
		fprintf(stderr,"\n dbmt cmp  : �Ƚ����ݿ�����ݿ��еı�");
		fprintf(stderr,"\n dbmt scada: ������׷�ϵ��ļ����뵽���ݿ���");
		fprintf(stderr,"\n dbmt clean: ���ĳ�����ĳ�����е����б�");
		fprintf(stderr,"\n dbmt drop : ɾ��ĳ�����ĳ�����е����б�");
		fprintf(stderr,"\n dbmt dump : ������ݿ���־");
		fprintf(stderr,"\n dbmt disp : ��ʾ���ݿ���Ϣ");
		fprintf(stderr,"\n dbmt check : ���SCD��������");
		fprintf(stderr,"\n");
		fprintf(stderr,"\n help    ����ʾ�������ϸʹ�÷������� dbmt copy help");
		fprintf(stderr,"\n");
		break;
	case FLAG_ALL:
        fprintf(stderr,"ϵͳ����Ա����: ");
		fprintf(stderr,"\n dbmt <����> [help] [ -P -NQ -USR�û� -PWD����]");
		fprintf(stderr,"\n");
		fprintf(stderr,"\n ���");
		fprintf(stderr,"\n dbmt out  : ���Ʋ��������ʷ���� -> �ļ�");
		fprintf(stderr,"\n dbmt in   : ���ļ�װ����������ʷ���ݵ����ݿ�");
		fprintf(stderr,"\n dbmt copy : ��˫������֮�临������");
		fprintf(stderr,"\n dbmt del  : ɾ��ĳʱ�ڵ���ʷ����");
		fprintf(stderr,"\n dbmt cmp  : �Ƚ�˫�������ı���");
		fprintf(stderr,"\n dbmt clean: ���ĳ�����ĳ�����е����б�");
		fprintf(stderr,"\n dbmt drop : ɾ��ĳ�����ĳ�����е����б�");
		fprintf(stderr,"\n dbmt disp : ��ʾ���ݿ���Ϣ");
		fprintf(stderr,"\n dbmt dump : ������ݿ���־");
		fprintf(stderr,"\n dbmt scada: ������׷�ϵ��ļ����뵽���ݿ���");
		fprintf(stderr,"\n dbmt check : ���SCD��������");
		fprintf(stderr,"\n");
		fprintf(stderr,"\n help    ����ʾ�������ϸʹ�÷������� dbmt copy help");
		fprintf(stderr,"\n -P      �������в���ʱ������ÿ������������ʾ");
		fprintf(stderr,"\n -NQ     ��ֱ��ִ�У�������ʾ�������ͶԲ�������ȷ��");
		fprintf(stderr,"\n -USR�û����Դ��û���¼���ݿ⣬�޴�ѡ������ϵͳȱʡ�û���¼");
		fprintf(stderr,"\n -PWD����û�����");
		fprintf(stderr,"\n");
		break;
	case FLAG_COPY:
		fprintf(stderr,"\n dbmt copy : �ڷ�����֮�临��ĳ�����ĳ�����е����б�");
		
		if(bHelpFlag==2) 
			fprintf(stderr,"\n      dbmt copy -Snn <-T���� | -D����> [-P -NQ -USR�û� -PWD����]");
		else 
			fprintf(stderr,"\n      dbmt copy -Snn <-T���� | -D����> [-P]");
		fprintf(stderr,"\n      -Snn     ����: -S13  ��������1�����ݿ�����������3��");
		fprintf(stderr,"\n              ��     -S42  ��������4�����ݿ�����������2��");
		fprintf(stderr,"\n      -T����  ������һ����");
		fprintf(stderr,"\n        ������modeldb�б�ʱ,��T��������Ͽ��� ���磺-Thisdb..��ʷ�����");
		fprintf(stderr,"\n      -D����  �����ƿ��е����б���-T -Dͬʱ����ʱ��ֻ��-T��Ч");
		fprintf(stderr,"\n      -P      �����ƿ������б�ʱ����ÿ���������ʾ");
		if(bHelpFlag!=2) break;
		fprintf(stderr,"\n      -NQ     ��ֱ��ִ�У�������ʾ�������ͶԲ�������ȷ��");
		fprintf(stderr,"\n      -USR�û����Դ��û���¼���ݿ⣬�޴�ѡ������ϵͳȱʡ�û���¼");
		fprintf(stderr,"\n      -PWD����û�����");
		break;
	case FLAG_DEL:
		fprintf(stderr,"\n dbmt del : ɾ��ĳʱ�ڵ���ʷ����");
		if(bHelpFlag==2)
			fprintf(stderr,"\n      dbmt del -Sn -B��ʼʱ�� -E��ֹʱ��  [-DTn -NQ -USR�û� -PWD����]");
		else 
			fprintf(stderr,"\n      dbmt del -Sn -B��ʼʱ�� -E��ֹʱ��");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
		fprintf(stderr,"\n      -B��ʼʱ�� ��");
		fprintf(stderr,"\n      -E��ֹʱ�� ��ɾ����ʷ���ݵ�ʱ��Σ�������ʼ����ֹʱ�䣩");
		fprintf(stderr,"\n                   ��ʽΪ�����£���2002��1�£�200201");
		if(bHelpFlag!=2) break;
		fprintf(stderr,"\n      -DTn    ��ɾ������������n��������ѡ������ѡ����ʾ");
		fprintf(stderr,"\n      -NQ     ��ֱ��ִ�У�������ʾ�������ͶԲ�������ȷ��");
		fprintf(stderr,"\n      -USR�û����Դ��û���¼���ݿ⣬�޴�ѡ������ϵͳȱʡ�û���¼");
		fprintf(stderr,"\n      -PWD����û�����");
		break;
	case FLAG_COMPARE:	
		fprintf(stderr,"\n dbmt cmp : �Ƚ�˫�������ı���");
		fprintf(stderr,"\n      dbmt cmp -Snn <-T���� | -D����>");
		fprintf(stderr,"\n      -Snn     ����: -S12  �ȽϷ�����1�ͷ�����2");
		fprintf(stderr,"\n              ��    -S24  �ȽϷ�����2�ͷ�����4");
		fprintf(stderr,"\n      -T����  :  �Ƚϵı�");
		fprintf(stderr,"\n      -D����  :  �Ƚϵı����ڵĿ⣨����д����������ֱ�ӱȽϿ�");
		break;
	case FLAG_SCADA:	
		fprintf(stderr,"\n dbmt scada : ������׷�ϵ��ļ����뵽���ݿ���");
		fprintf(stderr,"\n      dbmt scada -ѡ��");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
		fprintf(stderr,"\n      -F�ļ���	��׷�ϵ��ļ���");
		fprintf(stderr,"\n      -PATHĿ¼: �ļ��Ĵ��Ŀ¼���޴�ѡ����Ϊ��ǰĿ¼");
		fprintf(stderr,"\n      -P      ������׷��֮ǰ�ȶ����ݿ����ɾ������������ɺ�����");
		fprintf(stderr,"\n      ���磺-PATHc:\\\\ds3210\\\\procscript");
		break;
	case FLAG_OUTFILE:
		fprintf(stderr,"\n dbmt out : ���Ʋ������ĳ�µ���ʷ���� --> �ļ�");
		fprintf(stderr,"\n                 �����ݿ���������Ӱ�졣");
		fprintf(stderr,"\n    ������");
		if(bHelpFlag==2)
			fprintf(stderr,"\n    dbmt out  -Sn <-T���� | -D����> [-PATHĿ¼ -P -NQ -USR�û� -PWD����]");
		else	
			fprintf(stderr,"\n    dbmt out  -Sn <-T���� | -D����> [-PATHĿ¼ -P]");
		fprintf(stderr,"\n       ��Ҫ���Ʋ�����-->�ļ����ļ���Ϊ����");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
		fprintf(stderr,"\n      -T����  ������һ����");
		fprintf(stderr,"\n      -D����  �����ƿ��е����б�(���б�������Ϊ�����ڵĿ���)");
		fprintf(stderr,"\n      -PATHĿ¼: �ļ��Ĵ��Ŀ¼���޴�ѡ����Ϊ��ǰĿ¼");
		fprintf(stderr,"\n      -P      �����ƿ������б�ʱ����ÿ���������ʾ");
		fprintf(stderr,"\n    ��ʷ���ݣ�");
		if(bHelpFlag==2)
			fprintf(stderr,"\n    dbmt out -Dhisdb -Sn -YM���� [-PATHĿ¼ -DTn -NQ -USR�û� -PWD����]");
		else
			fprintf(stderr,"\n    dbmt out -Dhisdb -Sn -YM���� [-PATHĿ¼]");
		fprintf(stderr,"\n      ��Ҫ������ʷ���ݱ� --> �ļ�����ɾ�����ݿ��е����ݡ�");
		fprintf(stderr,"\n      �ļ���Ϊ(����+����),�ļ�����Ҫ�Ķ�����Ϊ�����ݿ��лָ�ʹ��");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
		fprintf(stderr,"\n      -YM���� ��ʱ�䣬��ʽΪ YYYYMM,��2002��1��Ϊ -YM200201");
		fprintf(stderr,"\n      -PATHĿ¼: �ļ��Ĵ��Ŀ¼���޴�ѡ����Ϊ��ǰĿ¼");
		if(bHelpFlag!=2) break;
		fprintf(stderr,"\n      -DTn    ��ɾ������������n��������ѡ������ѡ����ʾ");
		fprintf(stderr,"\n      -NQ     ��ֱ��ִ�У�������ʾ�������ͶԲ�������ȷ��");
		fprintf(stderr,"\n      -USR�û����Դ��û���¼���ݿ⣬�޴�ѡ������ϵͳȱʡ�û���¼");
		fprintf(stderr,"\n      -PWD����û�����");
		break;
	case FLAG_INFILE:
		fprintf(stderr,"\n dbmt in : ���ļ�װ��������ĳ�µ���ʷ����");
		fprintf(stderr,"\n    ������");
		if(bHelpFlag==2)
			fprintf(stderr,"\n    dbmt in -Sn <-T���� | -D����> [-PATHĿ¼ -P -NQ -A -USR�û� -PWD����]");
		else
			fprintf(stderr,"\n    dbmt in  -Sn <-T���� | -D����> [-PATHĿ¼ -P]");
		fprintf(stderr,"\n      ���ļ�װ���¼��������");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
		fprintf(stderr,"\n      -T����  ��װ��һ����");
		fprintf(stderr,"\n      -D����  ��װ����е����б�����д����������ֱ��װ���");
        fprintf(stderr,"\n      -Mģʽ  ��-Ma׷��,-Mb �������׷�� ");
		fprintf(stderr,"\n      -PATHĿ¼: �ļ��Ĵ��Ŀ¼���޴�ѡ����Ϊ��ǰĿ¼");
		fprintf(stderr,"\n      -P      ��װ��������б�ʱ����ÿ���������ʾ");
		fprintf(stderr,"\n      -A      ��װ��������б��޴�ѡ�������б�װ���:");
		fprintf(stderr,"\n               (pdb_field��pdb_table��rtdb_field��rtdb_table��topocolor������ע�����־�Ʋ����˵����ñ�SCD�������ͱ��������ͱ���������ѹ�ȼ���)");
		fprintf(stderr,"\n  ");
		fprintf(stderr,"\n    ��ʷ���ݣ�");
		if(bHelpFlag==2)
			fprintf(stderr,"\n    dbmt in -Dhisdb -Sn -YM���� [-PATHĿ¼ -DTn -NQ -USR�û� -PWD����]");
		else
			fprintf(stderr,"\n    dbmt in -Dhisdb -Sn -YM���� [-PATHĿ¼]");
		fprintf(stderr,"\n      װ����ʷ���ݵ����ݿ⣬��ɾ�����ݿ��е�ԭ�����ݡ�");
		//fprintf(stderr,"\n      �����ݿ��и��µ����ݴ���ʱ������Ҫװ�����ݣ�ȷʵҪװ�룬");
		//fprintf(stderr,"\n      ���� dbmt del ������ɾ�����µ���ʷ���ݡ�");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
        fprintf(stderr,"\n      -Mģʽ  ��-Ma׷��,-Mb �������׷�� ");
		fprintf(stderr,"\n      -YM���� ��ʱ�䣬��ʽΪ YYYYMM,��2002��1��Ϊ -YM200201");
		fprintf(stderr,"\n      -PATHĿ¼: �ļ��Ĵ��Ŀ¼���޴�ѡ����Ϊ��ǰĿ¼");
		if(bHelpFlag!=2) break;
		fprintf(stderr,"\n      -DTn    ��ɾ������������n��������ѡ������ѡ����ʾ");
		fprintf(stderr,"\n      -NQ     ��ֱ��ִ�У�������ʾ�������ͶԲ�������ȷ��");
		fprintf(stderr,"\n      -USR�û����Դ��û���¼���ݿ⣬�޴�ѡ������ϵͳȱʡ�û���¼");
		fprintf(stderr,"\n      -PWD����û�����");
		break;
	case FLAG_CLEAN:
		fprintf(stderr,"\n dbmt clean : ���ĳ�����ĳ�����е����б�");
		fprintf(stderr,"\n      dbmt clean -Sn <-T���� | -D����> [-P] -A");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
		fprintf(stderr,"\n      -T����  �����һ����");
		fprintf(stderr,"\n      -D����  ����յı����ڵĿ⣨����д����������ֱ�ӱȽϿ�");
		fprintf(stderr,"\n      -P      ����տ������б�ʱ����ÿ���������ʾ");
		fprintf(stderr,"\n      -A      ����տ������б��޴�ѡ�������б�������գ�");
		fprintf(stderr,"\n               (pdb_field��pdb_table��rtdb_field��rtdb_table��topocolor������ע�����־�Ʋ����˵����ñ�SCD�������ͱ��������ͱ���������ѹ�ȼ���)");
		break;
	case FLAG_DROP:
		fprintf(stderr,"\n dbmt drop : ɾ��ĳ�����ĳ�����е����б�");
		fprintf(stderr,"\n      dbmt drop -Sn <-T���� | -D����> [-P]");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
		fprintf(stderr,"\n      -T����  ��ɾ��һ����");
		fprintf(stderr,"\n      -D����  ��ɾ���ı����ڵĿ⣨����д����������ֱ�ӱȽϿ�");
		fprintf(stderr,"\n      -P      ��ɾ���������б�ʱ����ÿ���������ʾ");
		break;
	case FLAG_DISP:
		fprintf(stderr,"\n dbmt disp : ��ʾ���ݿ���Ϣ");
		fprintf(stderr,"\n      dbmt disp -Sn");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
		fprintf(stderr,"\n      -T����  ����ʾ����Ϣ�ı���");
		break;
	case FLAG_DUMP:
		fprintf(stderr,"\n dbmt dump : ������ݿ���־");
		fprintf(stderr,"\n      dbmt dump -Sn");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
		fprintf(stderr,"\n      -D����  �������־�Ŀ���");
		break;
	case FLAG_CHECK:
		fprintf(stderr,"\n dbmt check : ���SCD��������");
		fprintf(stderr,"\n      dbmt check -Sn");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
		break;
	case FLAG_DFHIS:
		fprintf(stderr,"\n dbmt dfhis : ����������ʷ���ݵ���");
		fprintf(stderr,"\n      dbmt dfhis -Sn -YM����");
		fprintf(stderr,"\n      -Sn     ��-S1 �� -S2 , -S3 , -S4, ������1���������2,������3,������4��");
		fprintf(stderr,"\n      -YM���� ��ʱ�䣬��ʽΪ YYYYMM,��2002��1��Ϊ -YM200201");
		fprintf(stderr,"\n      ע�⣺��Ҫ�ڱ�ϵͳ�ڴ��ڣ�ң���ϵ��Ӧ�� ��ң�Ź�ϵ��Ӧ��");
		break;
	default:
		fprintf(stderr,"\n �޴��������dbmt help ��ѯ����!");
		break;
	}
}

/*
	�ӿ���̨���� Y,N,Q (yes,no,quit)
	strdisp : ��ʾ�����
	flag    : �����ַ���־��=0 Y/N  =1 Y/N/Q
	default_input : ȱʡ�������ַ���
*/
int CDbOperate::get_input_char(char *strdisp,int flag)
{
	int inchar;
	char instr[255];

	if(bAutoOpe==1) return 'Y';
	do
	{
		fprintf(stderr,"%s",strdisp);
		gets(instr);
		inchar=toupper(instr[0]);
	}while( !(inchar=='Y' || inchar=='N' || (flag==1 && inchar=='Q')) );
	return inchar;
}

int CDbOperate::db_start(int serverno,int anotherflag)
{
	LOG_INFO *loginfo;
	int retcode;
	
	if(serverno<0)
	{
		fprintf(stderr,"\n������ָ��������(%d)\n",serverno+1);
		return CS_FAIL;
	}

	loginfo=(LOG_INFO *)malloc (sizeof(LOG_INFO));
	memset(loginfo,0,sizeof(LOG_INFO));

	switch(serverno)
	{
	case 1:
		strcpy(loginfo->servername,"nbdb");
		break;
	case 2:
		strcpy(loginfo->servername,"nbdbb");
		break;
	case 3:
		strcpy(loginfo->servername,"nbdb1");
		break;
	case 4:
		strcpy(loginfo->servername,"nbdbb1");
		break;
	default:
		break;
	}
	if (anotherflag == DF_HIS) 
		strcpy(loginfo->servername,"dfdb");

	strcpy(loginfo->username,"modeldb");
	strcpy(loginfo->password,"supernb");

	if(bInputUser==1 && loginfo!=NULL)
	{
		strcpy(loginfo->username,cUser);
		strcpy(loginfo->password,cPasswd);
	}

	loginfo->useflag=1;

	retcode=dbsingle.Open(0,loginfo,0,1);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n���󣺷�����%s��ʧ��!\n",loginfo->servername);
		fprintf(stderr,"���� 1��������%s�Ƿ�����\n",loginfo->servername);
		fprintf(stderr,"       2���ͻ��������Ƿ���ȷ\n");

		if (loginfo) free (loginfo);
		return retcode;
	}

	
	if (loginfo) free(loginfo);
	return retcode;
}

/* ����������ȡserver��*/
int CDbOperate::get_serverno()
{
	int serverno=-1;
	for (int i=1;i<argc;i++)
	{
		if (!(strcmp(argv[i],"-S1")))
		{	serverno=1; break;}
		if (!(strcmp(argv[i],"-S2")))
		{	serverno=2; break;}
		if (!(strcmp(argv[i],"-S3")))
		{	serverno=3; break;}
		if (!(strcmp(argv[i],"-S4")))
		{	serverno=4; break;}
		if (!(strcmp(argv[i],"-S12")))
		{	serverno=12; break;}
		if (!(strcmp(argv[i],"-S13")))
		{	serverno=13; break;}
		if (!(strcmp(argv[i],"-S14")))
		{	serverno=14; break;}
		if (!(strcmp(argv[i],"-S23")))
		{	serverno=23; break;}
		if (!(strcmp(argv[i],"-S24")))
		{	serverno=24; break;}
		if (!(strcmp(argv[i],"-S34")))
		{	serverno=34; break;}
		if (!(strcmp(argv[i],"-S21")))
		{	serverno=21; break;}
		if (!(strcmp(argv[i],"-S31")))
		{	serverno=31; break;}
		if (!(strcmp(argv[i],"-S41")))
		{	serverno=41; break;}
		if (!(strcmp(argv[i],"-S32")))
		{	serverno=32; break;}
		if (!(strcmp(argv[i],"-S42")))
		{	serverno=42; break;}
		if (!(strcmp(argv[i],"-S43")))
		{	serverno=43; break;}
	}
	return serverno;
}
/* ����������ȡ���� -D����
   =1 �ɹ��� =0 ʧ��
*/
int CDbOperate::get_dbname(char *dbname)
{
    for (int i=1;i<argc;i++)
	{
		if (strncmp(argv[i],"-D",2)==0)
		{
			if (argv[i][2]!=0)
			{
				strcpy(dbname,argv[i]+2);
				return 1;
			}
			fprintf(stderr,"\n-D δ�ṩ����!");
			return 0;
		}
	}
	return 0;
}
/* ����������ȡ���� -T����
   =1 �ɹ��� =0 ʧ��
*/
int CDbOperate::get_tablename(char *tablename)
{   
	int i,k=0,len=0;
    for (i=1;i<argc;i++)
	{
		if (strncmp(argv[i],"-T",2)==0)
		{
			if (argv[i][2]!=0)
			{
				strcpy(tablename,argv[i]+2);
			if (dbtype==DBTYPE_ORACLE)//��Сдת��
			{
				len=strlen(tablename);
				for(k=0;k<len;k++)
				if( tablename[k]>='a' && tablename[k]<='z' )
				tablename[k]=toupper(tablename[k]);
			}
				return 1;
			}
			fprintf(stderr,"\n-T δ�ṩ����!");
			return 0;
		}
	}
	return 0;
}

int CDbOperate::load_histable_name()
{
    HistableNameLen=50;
	int MaxTableName=50;
	pHistableName=(char *)malloc(MaxTableName*HistableNameLen);
	if(pHistableName==NULL) 
	{
		HistableNameLen=0;
		HistableNameCount=0;
		return CS_FAIL;
	}
	HistableNameCount=0;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"yc");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"yx");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"kwh");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"bk");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"dayyc");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"daykwh");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"monthyc");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"monthkwh");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"yearyc");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"yearkwh");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"sdata");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"������ʷ�����");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"������ʷ�����");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"�豸����ͳ�Ʊ�");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"pdrdescribe");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"pdrdata");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"���ر�λͳ�Ʊ�");
	HistableNameCount++;
	return CS_SUCCEED;
}

int CDbOperate::load_unchangedtable_name()
{
	UnchangedTableNameLen=50;
	int MaxTableName=50;
	pUnchangedTableName=(char *)malloc(MaxTableName*UnchangedTableNameLen);
	if(pUnchangedTableName==NULL) 
	{
		UnchangedTableNameLen=0;
		UnchangedTableNameCount=0;
		return CS_FAIL;
	}
	UnchangedTableNameCount=0;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"pdb_field");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"pdb_table");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"rtdb_field");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"rtdb_table");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"����ע���");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"��־�Ʋ����˵����ñ�");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"SCD�������ͱ�");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"�������ͱ�");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"������");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"��ѹ�ȼ���");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"topocolor");
	UnchangedTableNameCount++;

	return CS_SUCCEED;
}

int CDbOperate::check_histable_name(char *tbname)
{
	char tmpname[50];
	char *p;
	int len;
	if(pHistableName==NULL) return CS_FAIL;
	
	p=strrchr(tbname,'.');
	if(p!=NULL) strcpy(tmpname,p+1);
	else strcpy(tmpname,tbname);

	for(int i=0;i<HistableNameCount;i++)
	{
		/* �Ƚ��Ƿ���ʷ�� yc????,kwh????...
		   ��Ϊ pHistableName�п���ֻ��¼��ʷ������ǰһ���֣���һ����Ϊ���£� */
		len=strlen(pHistableName+i*HistableNameLen);
#if defined(__unix)
		if(strncasecmp(tmpname,pHistableName+i*HistableNameLen,len)==0)
#else 
		if(strnicmp(tmpname,pHistableName+i*HistableNameLen,len)==0)
#endif
			return CS_SUCCEED;	
	}
	return CS_FAIL;
}

int CDbOperate::check_unchangedtable_name(char *tbname)
{
	char tmpname[50];
	char *p;
	int len;
	if(pUnchangedTableName==NULL) return CS_FAIL;
	
	p=strrchr(tbname,'.');
	if(p!=NULL) strcpy(tmpname,p+1);
	else strcpy(tmpname,tbname);

	for(int i=0;i<UnchangedTableNameCount;i++)
	{
		len=strlen(pUnchangedTableName+i*UnchangedTableNameLen);
#if defined(__unix)
		if(strncasecmp(tmpname,pUnchangedTableName+i*UnchangedTableNameLen,len)==0)
#else 
		if(strnicmp(tmpname,pUnchangedTableName+i*UnchangedTableNameLen,len)==0)
#endif
			return CS_SUCCEED;	
	}
	return CS_FAIL;
}


int CDbOperate::check_tmptable_name(char *tbname)
{
#if defined(__unix)
	if(strncasecmp(tbname,"tmp_",4)==0)
#else 
	if(strnicmp(tbname,"tmp_",4)==0)
#endif
	return CS_SUCCEED;
	return CS_FAIL;
}

/* ���Ƶ����� */
int CDbOperate::realize_correct_onetable(CS_SMALLINT fserverno,CS_SMALLINT sserverno,CS_CHAR *tablename,CS_CHAR *dbname)
{
    FUNC_STR        func;
	memset(&func,0,sizeof(FUNC_STR));
    CS_RETCODE      retcode;
	char filename[255];

    strcpy(func.table,tablename);
	strcpy(func.dbname,dbname);
	strcpy(filename,tablename);

	retcode=db_start(fserverno);
	if (retcode!=CS_SUCCEED) 
		return (retcode);

	retcode=realize_out_file(&func,filename);
	if(retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n   ��ȡԴ���������ݴ���!");
		remove(filename);
	    return (retcode);
	}
	dbsingle.Close();
	//����һ��������
	retcode=db_start(sserverno);
	if (retcode!=CS_SUCCEED) 
	{
		remove(filename);
		return retcode;
	}

	strcpy(func.table,tablename);
	strcpy(func.dbname,dbname);
	retcode=realize_in_file(&func,filename);
	if (retcode!=CS_SUCCEED) 
	{
		fprintf(stderr,"\n   ��Ŀ��������������ݴ���!");
		remove(filename);
		return retcode;
	}
	remove(filename);
	dbsingle.Close();
	//if (p6) free (p6);
    return (retcode);
}

/* �����������е����б� */
int CDbOperate::realize_correct_table(CS_SMALLINT fserverno,CS_SMALLINT sserverno,CS_CHAR *dbname)
{   
    FUNC_STR        func;
	memset(&func,0,sizeof(FUNC_STR));
    CS_RETCODE      retcode;
	int inchar;
	int tbcount=0;
	CS_DATAFMT *datafmt;
	char *pt=NULL;
    if ((dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) || (dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) ) //modify by chenkai ����mysql
	if(pHistableName==NULL) load_histable_name();


if (dbtype==DBTYPE_DB2)
	sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='T'");
else if(dbtype==DBTYPE_ORACLE)
	sprintf(func.isql,"select table_name from user_tables");
else if(dbtype==DBTYPE_DMSQL)
	sprintf(func.isql,"select table_name from user_tables");
else if (dbtype==DBTYPE_MYSQL)					//modify by chenkai ����mysql
{
	sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
}
else // SYBASE_DB MSSQL_DB
	sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U'",dbname);

	retcode=db_start(fserverno);
	if (retcode!=CS_SUCCEED) 
		return (retcode);

	func.func=SEL_ISQL_RESULTS;
	retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
	if(retcode!=CS_SUCCEED)
	{
		fprintf(stderr," �ӿ���ȡ����ʧ��!\n");
		if (datafmt) free(datafmt);
		if (pt) free(pt);
		return CS_FAIL;
	}

	int tbnum=func.ret_roxnum;
	int i;
	int hisflag=0;
	int reclen=func.recorderlen;
	if(strcmp(dbname,"hisdb")==0)
		hisflag=1;
	tbcount=0;
	for(i=0;i<tbnum;i++)
	{
		if(check_tmptable_name(pt+i*reclen)==1)
		{	//��ʱ����������Ϊ��
			*(pt+i*reclen)=0;
			continue;
		}
		if ((dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE ) || ( dbtype == DBTYPE_MYSQL) || (dbtype==DBTYPE_DMSQL ) )  //modify by chenkai ����mysql
		{
			retcode=check_histable_name(pt+i*reclen);
				// �����⣺ֻ���������; ��ʷ�⣺ֻ������ʷ��
			if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
			{
				*(pt+i*reclen)=0;
				continue;
			}
		}
		tbcount++;
	}
	fprintf(stderr," �Ѵӿ���ȡ������: %d ����\n",tbcount);
	dbsingle.Close();

	tbcount=0;
	for(i=0;i<tbnum;i++)
	{
		if(*(pt+i*reclen)==0) continue;
		tbcount++;
		sprintf(func.table,"%s",pt+i*reclen);
		fprintf(stderr,"\n\n---------------------------------------------------");
		

		if(bPrompt==1)
		{
			sprintf(strstdin,"\n%d: �Ƿ��޲��� [ %s ] ��/��/�˳� (Y/N/Q)? ",tbcount,func.table);
			inchar=get_input_char(strstdin,1);
			if(inchar==(int)'Q') break;
			if (inchar==(int)'N') 
				continue;
		}

		fprintf(stderr,"\n%d: �����޲��� [ %s ] ...",tbcount,func.table);
		retcode=realize_correct_onetable(fserverno,sserverno,func.table,dbname);
	}
	if (datafmt) free(datafmt);
	if(pt) free(pt);
    return 1;
}

/* ���ϵͳ����Ա���� 	*/
int CDbOperate::check_root_password(int srvno)
{
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_DATAFMT *datafmt;
	char *pt=NULL;
	char inpasswd[255];
	int i=0;

	if (bAutoOpe) return CS_SUCCEED;
	//inpasswd[0]=0;
	fprintf(stderr,"\n������ϵͳ����Ա����: ");
	if(bAutoOpe==1) strcpy(inpasswd,"dbtools");
	else 
	{
		getpass( inpasswd );
		/*do
		{
			letter=getch();
			inpasswd[i]=letter;
			i++;
		}
		while (letter!='\n');
		*/
	}
	inpasswd[i-1]='\0';
	if(strcmp(inpasswd,"dbtools")==0)	
		return CS_SUCCEED;
	
	func.func=SEL_ISQL_RESULTS;
	strcpy(func.isql,"select ���� from �û���������� where ����Ա='sa' ");
	int ret=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
	if(ret!=CS_SUCCEED)
	{
		fprintf(stderr,"\nȡϵͳ����Ա����ʧ��!");

		if (datafmt) free (datafmt);
		if (pt) free (pt);

		return CS_FAIL;
	}
	if(func.ret_roxnum==0) 
	{
		fprintf(stderr,"\nδ����ϵͳ����Ա!");

		if (datafmt) free (datafmt);
		if (pt) free (pt);

		return CS_FAIL;
	}
	char rootpasswd[24];
	strncpy(rootpasswd,pt,24);

	if (datafmt) free(datafmt);
	if(pt) free(pt);

	if(strcmp(inpasswd,rootpasswd)==0)	
		return CS_SUCCEED;
	fprintf(stderr,"\n�������!");
	return CS_FAIL;
}

int CDbOperate::do_cmp()
{
    int objflag=0;
	char tablename[100],dbname[100];

	if(get_tablename(tablename)==1) 
	{
		objflag=1;
		if (!get_dbname(dbname))
		{
			fprintf(stderr,"\n   δ�ṩ����������dbmt cmp help ����!");
			return 0;
		}
	}
	else if(get_dbname(dbname)==1) objflag=2;
	

	if(objflag==0) 
	{
	    fprintf(stderr,"\n�����ʽ����! ���� dbmt cmp help ������\n");
		return 0;
	}
	
	if (objflag==1)	do_cmp_table(dbname,tablename);
	else if (objflag==2) do_cmp_db(dbname);

	return 1;
}
int CDbOperate::do_cmp_table(CS_CHAR *dbname,CS_CHAR *tablename,int id)
{
	CS_RETCODE retcode;
	CS_SMALLINT serverno,firstserverno,secondserverno;
	int i=0;

	serverno=get_serverno();
	if(serverno<12||serverno>43)
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩ��������������������ڡ����� dbmt in help ������\n");
	    return 0;
	}else
	{
		firstserverno = (int)(serverno/10);
		secondserverno = (int)(serverno%10);
	}

	if (id==-1)
	fprintf(stderr,"\n		������%s",tablename);
	else
	fprintf(stderr,"\n%d		������%s",id,tablename);

	retcode=db_start(firstserverno);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			���ݿ��ʧ��!");
		return retcode;
	}
	
	CS_DATAFMT *datafmt1,*datafmt2;
	FUNC_STR func1,func2;
	void *pt1,*pt2;

	memset(&func1,0,sizeof(FUNC_STR));
	memset(&func2,0,sizeof(FUNC_STR));

	func1.func=GET_TABLE_INFO;
	strcpy(func1.table,tablename);
	strcpy(func1.dbname,dbname);
	retcode=dbsingle.GetTableInfo(&func1,&datafmt1);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			ȡ������(%d)��%s����Ϣʧ��!",firstserverno,tablename);
		if (datafmt1) free(datafmt1);
		return retcode;
	}
	for (i=0;i<func1.colnum;i++)
	{
		func1.col[i].condition = NO;
		func1.col[i].select = YES;
		func1.col[i].order  = -1;
		func1.col[i].order_d = i;		
	}
	func1.func=SEL_RECORDERS;
	retcode=dbsingle.SelectRcds(&func1,datafmt1,&pt1);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			ȡ������(%d)��%s���¼����ʧ��!",firstserverno,tablename);
		if (datafmt1) free(datafmt1);
		return retcode;
	}

	dbsingle.Close();

	retcode=db_start(secondserverno);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			���ݿ��ʧ��!");
		return retcode;
	}
	
	func2.func=GET_TABLE_INFO;
	strcpy(func2.table,tablename);
	strcpy(func2.dbname,dbname);
	retcode=dbsingle.GetTableInfo(&func2,&datafmt2);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			ȡ������(&d)�� %s ����Ϣʧ��!",secondserverno,tablename);
		if (datafmt1) free(datafmt1);
		if (datafmt2) free(datafmt2);
		return retcode;
	}
	for ( i=0;i<func2.colnum;i++)
	{
		func2.col[i].condition = NO;
		func2.col[i].select = YES;
		func2.col[i].order  = -1;
		func2.col[i].order_d = i;		
	}
	func2.func=SEL_RECORDERS;
	retcode=dbsingle.SelectRcds(&func2,datafmt2,&pt2);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			ȡ������(%d)��%s���¼����ʧ��!",secondserverno,tablename);
		if (datafmt2) free(datafmt2);
		if (datafmt1) free(datafmt1);
		return retcode;
	}

	for(i=0;i<func1.colnum;i++)//Ϊ�˱Ƚ�datafmt ����
	{
		(datafmt1+i)->locale=0;
		(datafmt2+i)->locale=0;
	}
	if (func1.colnum!=func2.colnum) fprintf(stderr,"\n		�����������һ��!");
	else if (func1.ret_roxnum!=func2.ret_roxnum) fprintf(stderr,"\n		�������¼������һ��!");
	else if (memcmp(datafmt1,datafmt2,(sizeof(CS_DATAFMT)*func1.colnum))!=0) fprintf(stderr,"\n		���������Ϣ��һ��!");
	else if (memcmp(pt1,pt2,func1.recorderlen*func1.ret_roxnum)!=0) fprintf(stderr,"\n		�������¼���ݲ�һ��!");
	else
	{
		fprintf(stderr,"\n		�����һ��!");
	}

	dbsingle.Close();
	
	if (datafmt1) free(datafmt1);
	if (datafmt2) free(datafmt2);

	if (pt1) free(pt1);
	if (pt2) free(pt2);
	return retcode;
}

int CDbOperate::do_cmp_db(CS_CHAR *dbname)
{
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_RETCODE retcode;
	CS_DATAFMT *datafmt;
	int inchar;

	CS_SMALLINT serverno,firstserverno,secondserverno;

	serverno=get_serverno();
	if(serverno<12||serverno>43)
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩ��������������������ڡ����� dbmt in help ������\n");
	    return 0;
	}else
	{
		firstserverno = (int)(serverno/10);
		secondserverno = (int)(serverno%10);
	}
	retcode=db_start(firstserverno);//�Ե�һ��������Ϊ��׼ȡ��ȫ���ı���
	if (retcode!=CS_SUCCEED) return retcode;

	if (dbtype==DBTYPE_ORACLE)
		sprintf(func.isql,"select table_name from user_tables");
	else if (dbtype==DBTYPE_DMSQL)
		sprintf(func.isql,"select table_name from user_tables");	
	else if (dbtype==DBTYPE_MYSQL)			// add by chenkai ����mysql
	{
		sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
	}
	else
		sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U' order by name",dbname);
	
	char *pt=NULL;
	int tbnum,tbcount;
	int reclen,i,hisflag;
	func.func=SEL_ISQL_RESULTS;
	retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
	if(retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n �ӿ���ȡ����ʧ��!\n");
		if (datafmt) free (datafmt);
		if (pt) free (pt);
		return retcode;
	}
	tbnum=func.ret_roxnum;
	reclen=func.recorderlen;
	tbcount=0;
	if (strcmp(dbname,"modeldb")==0)
		hisflag=0;
	else hisflag =1;
	load_histable_name();
	for(i=0;i<tbnum;i++)
	{
		if(check_tmptable_name(pt+i*reclen)==1)
		{	//��ʱ����������Ϊ�ս�oracle��
			*(pt+i*reclen)=0;
			continue;
		}
		if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) || (dbtype == DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) )
		{		
			retcode=check_histable_name(pt+i*reclen);
					// �����⣺ֻ���������; ��ʷ�⣺ֻ������ʷ��
			if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
			{
					*(pt+i*reclen)=0;
					continue;
			}
		}
		tbcount++;
	}
	fprintf(stderr,"\n �Ѵӿ���ȡ������: %d ����\n",tbcount);
	tbcount=0;
	
	dbsingle.Close();//�رշ�����1

	for(i=0;i<tbnum;i++)
	{
		strcpy(func.table,pt+i*reclen);
		if(func.table[0]==0) continue;
		tbcount++;
		if(bPrompt==1)
		{
			sprintf(strstdin,"\n%d: �Ƿ�Ƚϱ� [ %s ]  : ��/��/�˳� (Y/N/Q)? ",tbcount,func.table);
			inchar=get_input_char(strstdin,1);
			if(inchar==(int)'Q') break;
			if (inchar==(int)'N') continue;
		}
		do_cmp_table(dbname,func.table,tbcount);
	}
	if(pt) free(pt);

	
	fprintf(stderr,"\n");
	dbsingle.Close();
	return 1;
}
/* ���Ʊ��� */
int CDbOperate::do_copy()
{
    CS_RETCODE      retcode;
	CS_SMALLINT	serverno,firstserverno,secondserverno;
	char tablename[100];
	char dbname[100];
    int         inchar;
	time_t		nowtime;
	struct tm	*ptm;

	if(get_tablename(tablename)==1) oneallflag=1;
	else if(get_dbname(dbname)==1) oneallflag=2;
	else
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩ��������������� dbmt copy help ������\n");
	    return 0;
	}

	serverno=get_serverno();
	if(serverno<12||serverno>43)
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩԴ��������������������ڡ����� dbmt copy help ������\n");
	    return 0;
	}else
	{
		firstserverno = (int)(serverno/10);
		secondserverno = (int)(serverno%10);
	}

	if(db_start(firstserverno)!=CS_SUCCEED) return 0;
		/* ���ϵͳ����Ա���� */	
	int dbflag=0;
	retcode=check_root_password(firstserverno);
	if(retcode!=CS_SUCCEED)
	{
			dbsingle.Close();
			return 0;
	}
	dbsingle.Close();

	if(oneallflag==1) fprintf(stderr,"\n���Ʊ� %s :\n",tablename);
	else if(oneallflag==2) fprintf(stderr,"\n���ƿ� %s :\n",dbname);

	
	if(bPrompt==1) 
	{
		if (serverno==1)
			sprintf(strstdin,"\n�Ƿ��÷�����1(nbdb) ���Ƶ� ������2(nbdbb) (Y/N)?");
		else if (serverno==2)
			sprintf(strstdin,"\n�Ƿ��÷�����2(nbdbb) ���Ƶ� ������1(nbdb) (Y/N)?");
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
		if (serverno==1)
			sprintf(strstdin,"\nȷ���÷�����1(nbdb) ���Ƶ� ������2(nbdbb) (Y/N)?");
		else if (serverno==2)
			sprintf(strstdin,"\nȷ���÷�����2(nbdbb) ���Ƶ� ������1(nbdb) (Y/N)?");
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
	}
	time(&nowtime);
	ptm=localtime(&nowtime);
    fprintf(stderr,"\n\nBegin time:  %04d-%02d-%02d  %02d:%02d:%02d\n",
		ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,
		ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
	
	if (oneallflag==1)
    {
        strcpy(dbname,"modeldb");
		fprintf(stderr,"\n�����޲��� [ %s ] ...",tablename);
		realize_correct_onetable(firstserverno,secondserverno,tablename,dbname);
    }
	else if(oneallflag==2)
    {
        fprintf(stderr,"\n�����޲��� [ %s ] ...",dbname);
		realize_correct_table(firstserverno,secondserverno,dbname);
    }
	
	time(&nowtime);
	ptm=localtime(&nowtime);
    fprintf(stderr,"\nEnd time:  %04d-%02d-%02d  %02d:%02d:%02d\n",
		ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,
		ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
	return 1;
}

/* ɾ����ʷ���� 
   dbmt del -Sn -B��ʼʱ�� -E��ֹʱ�� [-Dn -NQ -USR�û� -PWD����]
   -Sn			: -S1��-S2 
   -B��ʼʱ��	: ��ʽΪ���£���2002��1�� -B200201
   -E��ֹʱ��	:
             ����Ϊ��ѡ��
   -Dn			: ��ѡ��ɾ�����������࣬������ѡ������ѡ����ʾ
   -NQ			: ֱ��ִ�У�����������ȷ��ɾ��
   -USR�û�		: �޴�ѡ������modeldb�û���¼ 
   -PWD����		:
*/
int CDbOperate::do_delete()
{  
	int i,k,inchar;
	int deltype=-1;
	int err;
	int startdate=0,enddate=0;//��ʼʱ�䣬��ֹʱ��
	int syear,smon,eyear,emon;//��ʼ�꣬��ʼ�£���ֹ�꣬��ֹ��
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_RETCODE retcode;
	int serverno=-1;
	char *pt;
	int recnum,ret;
	time_t time1,time2;
	struct tm tmtime1,tmtime2;
	CS_DATAFMT *datafmt;

	serverno=get_serverno();
	if(serverno<1 ||serverno>4 )
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩ��������������������ڡ����� dbmt del help ������\n");
	    return 0;
	}

	for(i=1;i<argc;i++)		// -DTn nΪҪɾ������������
		if(strncmp(argv[i],"-DT",3)==0)
		{
			if(argv[i][3]!=0)
			{
				if(sscanf(argv[i]+3,"%d",&deltype)<=0) deltype=-1;
				break;
			}
		}
	if(deltype<1 || deltype>HISDATANUM) deltype=-1;

	err=0;
	for (i=1;i<argc;i++)
		if (strncmp(argv[i],"-B",2)==0)
		{
			if(argv[i][2]!=0)
			{
				if(sscanf(argv[i]+2,"%d",&startdate)<=0) err=2;
				break;
			}
		}
	if(i>=argc) err=1;
	if(err==0)
	{
		syear=startdate/100;
		smon=startdate%100;
		if(syear<1998 || syear>2999 || smon<1 || smon>12 ) err=2;
	}
	if(err>0)
	{
		if(err==1) fprintf(stderr,"δ�ṩ��ʼʱ��! ");
		else fprintf(stderr,"-B�ṩ����ʼʱ�����! ");
		fprintf(stderr,"���� dbmt del help ����\n");
		return 0;
	}

	err=0;
	for (i=1;i<argc;i++)
		if (strncmp(argv[i],"-E",2)==0)
		{
			if(argv[i][2]!=0)
			{
				if(sscanf(argv[i]+2,"%d",&enddate)<=0) err=2;
				break;
			}
		}
	if(i>=argc) err=1;
	if(err==0)
	{
		eyear=enddate/100;
		emon=enddate%100;
		if(eyear<1998 || eyear>2999 || emon<1 || emon>12 ) err=2;
		else if(eyear<syear || (eyear==syear && emon<smon) ) err=3;
	}
	if(err>0)
	{
		if(err==1) fprintf(stderr,"δ�ṩ��ֹʱ��! ");
		else if(err==2) fprintf(stderr,"-E�ṩ����ֹʱ�����! ");
		else if(err==3) fprintf(stderr,"-E�ṩ����ֹʱ��С����ʼʱ��! ");
		fprintf(stderr,"���� dbmt del help ����\n");
		return 0;
	}

	if(db_start(serverno)!=CS_SUCCEED) return 0;
		/* ���ϵͳ����Ա���� */	
	int dbflag=0;

	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}

	LOG_INFO *loginfo=NULL;
	int srvnum=0;

	fprintf(stderr,"\n");
	for(i=0;i<HISDATANUM;i++)
		fprintf(stderr,"    %d��%s\n",i+1,datatype[i]);
	fprintf(stderr,"    0���˳�\n");

	int dataflag;
	if(deltype==-1)
	{
		do
		{
			fprintf(stderr,"��ѡ��Ҫɾ������ʷ�������ͣ�[");
			for(i=1;i<=HISDATANUM;i++) fprintf(stderr,"%d,",i);
			fprintf(stderr,"0] : ");
			gets(strstdin);
			inchar=strstdin[0];
		}while(inchar<'0' || inchar>('0'+HISDATANUM) );
		
		if(inchar=='0') return 1;

		dataflag=inchar-'0';
	}
	else dataflag=deltype;
	if (bPrompt==1)
	{
		sprintf(strstdin,"\n�Ƿ�ɾ�� %04d��%02d�� -- %04d��%02d�� ֮���%s [Y/N]: ",
				syear,smon,eyear,emon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin,0);
		if(inchar=='N') return 1;

		sprintf(strstdin,"\nȷ��ɾ�� %04d��%02d�� -- %04d��%02d�� ֮���%s [Y/N]: ",
				syear,smon,eyear,emon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin,0);
		if(inchar=='N') return 1;
	}


	int datayear,datamon,dataday;
	int monnum=(eyear-syear)*12+(emon-smon)+1;
	char *pcmd;
	char tmpstr[255],tmpstr1[50];

	pcmd=(char *)malloc(EX_BUFSIZE*2);
	if(pcmd==NULL) 
	{
		fprintf(stderr,"malloc() failed!");
		dbsingle.Close();
		return 0;
	}
	memset(pcmd,0,EX_BUFSIZE*2);
	
	memset(&tmtime1,0,sizeof(tmtime1));
	dataday=0;
	tmpstr1[0]=0;
	if(dataflag==1 || dataflag==2)	/* ɾ���������� */
	{
if(dbtype==DBTYPE_DB2)
{	for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			pcmd[0]=0;

			tmtime1.tm_isdst=-1;
			tmtime1.tm_sec=0; tmtime1.tm_min=0; tmtime1.tm_hour=8;
			tmtime1.tm_mday=1; tmtime1.tm_mon=datamon-1; tmtime1.tm_year=datayear-1900;
			time1=mktime(&tmtime1);
			dataday=time1/(24*3600);

			for(k=0;k<7;k++)
			{
				sprintf(tmpstr1,"%s where date>=%d and date<=%d",tmpstr,dataday+k*5,dataday+(k+1)*5);
				sprintf(func.isql,"delete from yc%s;delete from kwh%s;\
					delete from dayyc%s;delete from daykwh%s;",
					tmpstr1,tmpstr1,tmpstr1,tmpstr1);
				strcat(pcmd,func.isql);

			}
			sprintf(func.isql,"delete from yc%s;delete from kwh%s;\
				delete from dayyc%s;delete from daykwh%s;",
				tmpstr,tmpstr,tmpstr,tmpstr);
			strcat(pcmd,func.isql);
			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ��������... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"��������ȷ ");
			else fprintf(stderr,"�����ݴ��� ");

			func.func=ISQL_COMMAND;
			sprintf(pcmd,"delete from monyc%04d where date=%d;\
				delete from monkwh%04d where date=%d",
				datayear,(datayear-1970)*12+datamon,
				datayear,(datayear-1970)*12+datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"��������ȷ ");
			else fprintf(stderr,"�������ޱ��ɾ������ ");
			
			func.func=ISQL_COMMAND;
			sprintf(pcmd,"delete from �豸����ͳ�Ʊ� where ��������>=%04d%02d01 and ��������<=%04d%02d31;",datayear,datamon,datayear,datamon);
			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ��������... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"�豸����ͳ�Ʊ� ��ȷ ");
			else fprintf(stderr," �豸����ͳ�Ʊ� ���� ");
		}
}
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
{	for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			sprintf(pcmd,"truncate table yc%s;truncate table kwh%s;\
				truncate table dayyc%s;truncate table daykwh%s;",
				tmpstr,tmpstr,tmpstr,tmpstr);
			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ��������... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"��������ȷ ");
			else fprintf(stderr,"�����ݴ��� ");

			func.func=ISQL_COMMAND;
			sprintf(pcmd,"delete from monyc%04d where date=%d;\
				delete from monkwh%04d where date=%d",
				datayear,(datayear-1970)*12+datamon,
				datayear,(datayear-1970)*12+datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"��������ȷ ");
			else fprintf(stderr,"�������ޱ��ɾ������ ");
			
			func.func=ISQL_COMMAND;
			sprintf(pcmd,"delete from �豸����ͳ�Ʊ� where ��������>=%04d%02d01 and ��������<=%04d%02d31",
				datayear,datamon,datayear,datamon);
			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ��������... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"�豸����ͳ�Ʊ� ��ȷ ");
			else fprintf(stderr," �豸����ͳ�Ʊ� ���� ");
		}			
}
else // SYBASE_DB MSSQL_DB
{		for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			sprintf(pcmd,"use hisdb\
				\r\ngo\r\n if exists(select name from sysobjects where name='yc%s') truncate table yc%s\
				\r\ngo\r\n if exists(select name from sysobjects where name='kwh%s') truncate table kwh%s\
				\r\ngo\r\n if exists(select name from sysobjects where name='dayyc%s') truncate table dayyc%s\
				\r\ngo\r\n if exists(select name from sysobjects where name='daykwh%s') truncate table daykwh%s",
				tmpstr,tmpstr,tmpstr,tmpstr,tmpstr,tmpstr,tmpstr,tmpstr);
			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ��������... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"��������ȷ ");
			else fprintf(stderr,"�����ݴ��� ");

			func.func=ISQL_COMMAND;
			sprintf(pcmd,"use hisdb\
				\r\ngo\r\n if exists(select name from sysobjects where name='monthyc%04d')\
				delete from monthyc%04d where sdate=%d \
				\r\ngo\r\n dump tran hisdb with truncate_only\
				\r\ngo\r\n if exists(select name from sysobjects where name='monthkwh%04d')\
				delete from monthkwh%04d where sdate=%d \
				\r\ngo\r\n dump tran hisdb with truncate_only",
				datayear,datayear,(datayear-1970)*12+datamon,
				datayear,datayear,(datayear-1970)*12+datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"��������ȷ ");
			else fprintf(stderr,"�������ޱ��ɾ������ ");
			

			func.func=ISQL_COMMAND;
			sprintf(pcmd,"use hisdb\
				\r\ngo\r\n delete from �豸����ͳ�Ʊ� where ��������>=%04d%02d01 and ��������<=%04d%02d31\
				\r\ngo\r\n dump transaction hisdb with truncate_only\r\ngo\r\n ",
				datayear,datamon,datayear,datamon);
			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ��������... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"�豸����ͳ�Ʊ� ��ȷ ");
			else fprintf(stderr," �豸����ͳ�Ʊ� ���� ");
		}
}
	}
	
	if(dataflag==1 || dataflag==3)	/* ɾ������Ͳ�����¼ */
	{
		for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			//func.serverno=serverno;
			pcmd[0]=0;
if ( (dbtype==DBTYPE_SYBASE) || (dbtype==DBTYPE_SQLSERVER) )
			strcpy(pcmd,"use hisdb\r\ngo\r\n");

			
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE)  || (dbtype==DBTYPE_DMSQL ) )
{			sprintf(func.isql,"truncate table ������ʷ�����%04d%02d;",
					datayear,datamon);
				strcat(pcmd,func.isql);
}
else // SYBASE_DB MSSQL_DB
{				sprintf(func.isql,"truncate table ������ʷ�����%04d%02d\r\ngo\r\n",
					datayear,datamon);
				strcat(pcmd,func.isql);
				
}			

if ( (dbtype==DBTYPE_SYBASE) || (dbtype==DBTYPE_SQLSERVER) )
			strcat(pcmd,"dump tran hisdb with truncate_only\r\ngo\r\n");

			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ������ʷ����... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"��ȷ ");
			else fprintf(stderr,"���� ");

			func.func=ISQL_COMMAND;
			pcmd[0]=0;
if ( (dbtype==DBTYPE_SYBASE) || (dbtype==DBTYPE_SQLSERVER) )
			strcpy(pcmd,"use hisdb\r\ngo\r\n");
			
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE)  || (dbtype==DBTYPE_DMSQL ) )
{sprintf(func.isql,"delete from ������ʷ�����%04d%02d;",
					datayear,datamon);
strcat(pcmd,func.isql);}
else // SYBASE_DB MSSQL_DB
{				sprintf(func.isql,"truncate table ������ʷ�����%04d%02d\r\ngo\r\n",
					datayear,datamon);
				strcat(pcmd,func.isql);
				}
			
if ( (dbtype==DBTYPE_SYBASE) ||(dbtype==DBTYPE_SQLSERVER) )
			strcat(pcmd,"dump tran hisdb with truncate_only\r\ngo\r\n");

			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ������¼... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"��ȷ ");
			else fprintf(stderr,"���� ");
		}
	}

	memset(&tmtime1,0,sizeof(tmtime1));
	memset(&tmtime2,0,sizeof(tmtime2));
	if(dataflag==1 || dataflag==4)	/* ɾ��׷������ */
	{
		for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ׷������... ",
				serverno,datayear,datamon);

			func.func=SEL_ISQL_RESULTS;
			func.recorderlen=0;
			sprintf(func.isql,"select ʱ�� from hisdb.dbo.pdrdescribe where ������>=%04d%02d%02d and ������<=%04d%02d%02d",
				datayear,datamon,1,datayear,datamon,31);
			
			retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
			if(retcode!=CS_SUCCEED) 
			{
				fprintf(stderr,"����");
				continue;
			}
		
			recnum=func.ret_roxnum;
			int *pint;
			ret=CS_SUCCEED;
			for(k=0;k<recnum;k++)
			{
				func.func=ISQL_COMMAND;
				pcmd[0]=0;
				pint=(int*)(pt+func.recorderlen*k);
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE)  || (dbtype==DBTYPE_DMSQL ) )
				sprintf(func.isql,"delete from pdrdata where ʱ��=%d;\
					delete from pdrdescribe where ʱ��=%d;",
					*pint,*pint);
else //SYBASE_DB MSSQL_DB
{				strcpy(pcmd,"use hisdb\r\ngo\r\n");
				sprintf(func.isql,"delete from pdrdata where ʱ��=%d\r\ngo\r\n\
					delete from pdrdescribe where ʱ��=%d\r\ngo\r\n",
					*pint,*pint);
				strcat(func.isql,"dump tran hisdb with truncate_only\r\n");
}
				strcat(pcmd,func.isql);
				retcode=dbsingle.IsqlCommand(&func,pcmd);
				if(retcode!=CS_SUCCEED) ret=CS_FAIL;
			}
			if (datafmt) free(datafmt);
			if(pt) free(pt);
	
			tmtime1.tm_isdst=-1; tmtime2.tm_isdst=-1;
			tmtime1.tm_sec=0; tmtime1.tm_min=0; tmtime1.tm_hour=0;
			tmtime1.tm_mday=1; tmtime1.tm_mon=datamon-1; tmtime1.tm_year=datayear-1900;
			tmtime2.tm_sec=0; tmtime2.tm_min=0; tmtime2.tm_hour=0;
			tmtime2.tm_mday=1;
			if(datamon<12) { tmtime2.tm_mon=datamon; tmtime2.tm_year=datayear-1900; }
			else { tmtime2.tm_mon=0; tmtime2.tm_year=datayear+1-1900; }
			time1=mktime(&tmtime1);
			time2=mktime(&tmtime2);
			func.func=ISQL_COMMAND;
			pcmd[0]=0;
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE)  || (dbtype==DBTYPE_DMSQL ) )
			sprintf(func.isql,"delete from pdrdata where ʱ��>=%d and ʱ��<=%d;",
				time1,time2);
else // SYBASE_DB MSSQL_DB
{			strcpy(pcmd,"use hisdb\r\ngo\r\n");
			sprintf(func.isql,"delete from pdrdata where ʱ��>=%d and ʱ��<=%d\r\ngo\r\n",
				time1,time2);
			strcat(func.isql,"dump tran hisdb with truncate_only\r\n");
}
			strcat(pcmd,func.isql);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode!=CS_SUCCEED) ret=CS_FAIL;
			if(ret==CS_SUCCEED) fprintf(stderr,"��ȷ ");
			else fprintf(stderr,"���� ");
		}
	}

	memset(&tmtime1,0,sizeof(tmtime1));
	dataday=0;
	tmpstr1[0]=0;
	if(dataflag==1 || dataflag==5)	/* ɾ���뼶�������� */
	{
if (dbtype==DBTYPE_DB2)
{	for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			pcmd[0]=0;

			tmtime1.tm_isdst=-1;
			tmtime1.tm_sec=0; tmtime1.tm_min=0; tmtime1.tm_hour=8;
			tmtime1.tm_mday=1; tmtime1.tm_mon=datamon-1; tmtime1.tm_year=datayear-1900;
			time1=mktime(&tmtime1);
			dataday=time1/(24*3600);

			for(k=0;k<7;k++)
			{
				sprintf(tmpstr1,"%s where sdate>=%d and sdate<=%d",tmpstr,dataday+k*5,dataday+(k+1)*5);
				sprintf(func.isql,"delete from sdata%s;",tmpstr1);
				strcat(pcmd,func.isql);

			}
			sprintf(func.isql,"delete from sdata%s;",tmpstr);
			strcat(pcmd,func.isql);
			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ��������... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"�뼶����������ȷ ");
			else fprintf(stderr,"�뼶�������ݴ��� ");
		}
}
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
{	for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			sprintf(pcmd,"truncate table sdata%s;",tmpstr);
			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ��������... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"�뼶����������ȷ ");
			else fprintf(stderr,"�뼶�������ݴ��� ");
		}			
}
else // SYBASE_DB MSSQL_DB
{		for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			sprintf(pcmd,"use hisdb\
				\r\ngo\r\n if exists(select name from sysobjects where name='sdata%s') truncate table sdata%s",
				tmpstr,tmpstr);
			fprintf(stderr,"\n\nɾ��������(%d)  %04d��%02d�� ��������... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"�뼶����������ȷ ");
			else fprintf(stderr,"�뼶�������ݴ��� ");
		}
}
	}

	free(pcmd);
	dbsingle.Close();
	return 1;
}

/*  �ļ�->����
��������ļ���Ϊ�����
��ʷ��������Ϊ��λ������Ϊ ����yyyymm ,
	�����������ʷ�����200201��monthyc200201��
	�������������µĳ��⣬�� yc200201,dayyc200201
��ʷ���ݣ�
    dbmt in -Dhisdb -Sn -YM���� [-debug]
		-HD     : ��ʷ����
		-Sn		: -S1 ��-S2
		-YM		: ���£�yyyymm ��2002��1��Ϊ 200201 
������
	dbmt in  <-D���� | -T����> [-P] [-debug]
		-Sn		: -S1(nbdb) ��-S2(scadbb)
		-D����  : �������б� modeldb,hisdb
		-T		: ĳ���� ����.dbo.����
		-P		: ͬ-D���ʹ�ã���ÿ������ʾ�Ƿ������
*/
int CDbOperate::do_in_file()
{   
	char dbname[100];
	if (get_dbname(dbname)==1)
	{
		if (strcmp(dbname,"hisdb")==0)
			return do_in_hisdata();	/* ��ʷ���� */
		else if(strcmp(dbname,"modeldb")==0)
		{
			return do_in_para();		/* ���� */
		}
	}
	
	fprintf(stderr,"\n�����ʽ����!  δ�ṩ���� ������ dbmt in help ������\n");
	return 0;
}

int CDbOperate::realize_in_file(FUNC_STR *func,char *filename,int null_num,int id)
{
	CS_RETCODE retcode;
	int i,numrows;
	void *p6;
	FILE *stream=NULL;
	if(id>=0) fprintf(stderr,"\n%d ����װ���¼����[%s] ...",id,func->table);
	else fprintf(stderr,"\n ����װ���¼����[%s] ...",func->table);

	if (func->his.flag==2)
	{
		func->func=ISQL_COMMAND;
		sprintf(func->isql,"truncate table %s",func->table);
		retcode=dbsingle.IsqlCommand(func,func->isql);
		if (retcode==CS_SUCCEED) fprintf(stderr,"\n		ԭ��¼����ɹ�");
		else {
			fprintf(stderr,"\n		ԭ��¼���ʧ��");
			//return retcode;
		}
	}	
	CS_DATAFMT *datafmt;
	func->func=GET_TABLE_INFO;
	retcode = dbsingle.GetTableInfo(func,&datafmt);
	if (retcode!=CS_SUCCEED)
	{

		if (datafmt) free (datafmt);

		fprintf (stderr,"\n %s ��ʧ��!",func->table);
		return CS_FAIL;
	}
	
	CS_DATAFMT** fmt = &datafmt;
	void**  ptt = (void**)fmt;
	stream=fopen(filename,"rt");
	if(stream==NULL)
	{

		if (datafmt) free(datafmt);

		fprintf(stderr,"\n���ļ�[%s] ����ļ�����!",filename);
		return CS_FAIL;
	}

	fseek(stream,0,SEEK_SET);//��λ���ļ�ͷ
	
	numrows=1;
	char * pstr=(char*)malloc(EX_BUFSIZE*2+10);

	fgets(pstr,EX_BUFSIZE*2,stream);//����һ��Ϊ��ṹ�����˵�һ��
	if(strncmp(pstr,"--DATAFMT: ",11)!=0)
	fseek(stream,0,SEEK_SET);//��λ���ļ�ͷ

	char *pcur;
	VAL_UNION *value;
	int tmpint,j;
	int shortvalue =0;
	int recnum=0;
	value=(VAL_UNION *) malloc (sizeof(VAL_UNION)*MAXINROWNUM*func->colnum); //Ϊ�˷�ֹ�ڴ治�����ֻ��Ϊ5000*func->colnum

	while(1)
	{
		memset(value,0,sizeof(VAL_UNION)*MAXINROWNUM*func->colnum); 
		for(i=0;i<MAXINROWNUM;i++)
		{
			if (fgets(pstr,EX_BUFSIZE*2,stream)==NULL)
				break;
			else 
				pcur=pstr;

			for(j=0;j<func->colnum;j++)
			{
				if(j>0)
				{
					if( (pcur=strchr(pcur,'\t'))==NULL )	/* ÿ������TAB������ */
					{
						retcode=CS_FAIL;
						break;
					}
					pcur+=1;
				}
				tmpint=i*func->colnum+j;
				switch(datafmt[j].datatype)
				{
					case CS_SMALLINT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->smallvalue=0;
						else
							if(sscanf(pcur,"%d",&shortvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else (value+tmpint)->smallvalue=shortvalue;
						break;
					case CS_TINYINT_TYPE:
					case CS_BIT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->tinyvalue=0;
						else	
							if(sscanf(pcur,"%d",&shortvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else 
								(value+tmpint)->tinyvalue=(unsigned char)shortvalue;
						break;
					case CS_USHORT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->usmallvalue=0;
						else
							if(sscanf(pcur,"%d",&value)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else (value+tmpint)->usmallvalue=shortvalue;
						break;
					case CS_INT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->intvalue=0;
						else
							if(sscanf(pcur,"%d",&((value+tmpint)->intvalue))<=0)
							{
								retcode=CS_FAIL;
								break;
							}
						break;
					case CS_CHAR_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') strcpy((value+tmpint)->charvalue," ");
						else 
							if(sscanf(pcur,"%[^\t\r\n]s",(value+tmpint)->charvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else (value+tmpint)->charvalue[datafmt[j].maxlength-1]=0;//2006.2.10 ������ֳ����ݵ���������¼������������
						break;
					case CS_REAL_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->realvalue=0;
						else
							if(sscanf(pcur,"%g",&((value+tmpint)->realvalue))<=0)
							{
								retcode=CS_FAIL;
								break;
							}
						break;
					case CS_FLOAT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->floatvalue=0;
						else
						if(sscanf(pcur,"%lg",&((value+tmpint)->floatvalue))<=0)
						{
							retcode=CS_FAIL;
							break;
						}
						break;
					case CS_DATETIME_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') 
						{
							retcode=CS_FAIL;
							break;
						}
						if(sscanf(pcur,"%[^\t\r\n]s",(value+tmpint)->charvalue)<=0)
						{
							retcode=CS_FAIL;
							break;
						}
						break;
					default:
						break;
				}//switch
			}//for (j)
		}//for (i)
		p6=value;
		fprintf(stderr,"-");
		if(i==0) break;
		retcode=dbsingle.DbTools(DB_IN_FILE,(void*)func->dbname,(void*)func->table,(void*)(&func->colnum),ptt,&i,&p6);
		if(retcode==CS_SUCCEED)			recnum+=i;

		if(i<MAXINROWNUM) break;
	}
//	}//while

	//for(j=0;j<null_num;j++) putchar(' ');
	putchar('\t');
	if(retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"	ʧ��!  ");
		fprintf(stderr,"�������ݴ���!",func->table);
		if (pstr) free(pstr);

		if (datafmt) free(datafmt);

		if (p6) free(p6);
		fclose(stream);
		return 0;
	}
	else 
	{
		fprintf(stderr,"	�ɹ�!  ");
		fprintf(stderr,"����%d������!",recnum);
		if (pstr) free(pstr);

		if (datafmt) free(datafmt);

		if (p6) free(p6);
		fclose(stream);
		return 1;
	}
}

int CDbOperate::do_in_para()		/* ���� */
{   
    CS_RETCODE      retcode;
	CS_SMALLINT	serverno,i;
	CS_DATAFMT *datafmt;
	char tablename[100];
	char dbname[100];
	int tbcount=0;
    int  inchar,hisflag=0;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));

	serverno=get_serverno();
	if(serverno<1||serverno>4)
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩ��������������������ڡ����� dbmt in help ������\n");
	    return 0;
	}

	if(get_tablename(tablename)==1) 
	{
		oneallflag=1;
		if (get_dbname(dbname)!=1)
		{
			fprintf(stderr,"\n   δ�ṩ����������dbmt in help ����!");
			return 0;
		}
	}
	else if(get_dbname(dbname)==1) oneallflag=2;
	

	if(strcmp(mode,"a")==0)
		func.his.flag=0;	/* ׷�Ӽ�¼�������Ǳ����Ƿ��м�¼ Ĭ��*/
		else
        if(strcmp(mode,"b")==0)
        func.his.flag=2;	/* ����� */
        else
        if(strcmp(mode,"c")==0)
        func.his.flag=3;	/*��ִ��isql */
	/*if(oneallflag==1)
	{
		dbname[0]=0;
		char *pstr=strchr(tablename,'.');
		if(pstr!=NULL) strncpy(dbname,tablename,pstr-tablename);
	}*/

	if(db_start(serverno)!=CS_SUCCEED) return 0;
		/* ���ϵͳ����Ա���� */	
	int dbflag=0;
	retcode=check_root_password(serverno);

	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		fprintf(stderr,"\n ���ݿ��ʧ��!");
		return 0;
	}

	if(oneallflag==1) fprintf(stderr,"\nװ���¼���� %s :\n",tablename);
	else if(oneallflag==2) fprintf(stderr,"\nװ���¼���� %s :\n",dbname);

	if (bPrompt==1)
	{
		if(oneallflag==1)
			sprintf(strstdin,"\n�Ƿ�װ���¼���� %s  (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n�Ƿ�װ���¼���� %s  (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }

		if(oneallflag==1)
			sprintf(strstdin,"\nȷ��װ���¼���� %s  (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\nȷ��װ���¼���� %s  (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
	}

	char filename[255];
	char *pstr;

	strcpy(func.dbname,dbname);
	if(oneallflag==1)//һ����
	{
		pstr=strrchr(tablename,'.');
		if(pstr==NULL) strcpy(filename,tablename);
		else strcpy(filename,pstr+1);
		strcpy(func.table,tablename);
		if(strcmp(mode,"a")==0)
		func.his.flag=0;	/* ׷�Ӽ�¼�������Ǳ����Ƿ��м�¼ */
		else
        if(strcmp(mode,"b")==0)
        func.his.flag=2;	/* ����� */
        else
        if(strcmp(mode,"c")==0)
        func.his.flag=3;	/*��ִ��isql */

		retcode=realize_in_file(&func,filename);
		fprintf(stderr,"\n");
		return retcode;
	}
	else
	{
if ( (dbtype==DBTYPE_DB2) ||(dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) ) //modify by chenkai ����mysql
		if(pHistableName==NULL) load_histable_name();
// �������û�� [-A]
if (!bAllTables)
		load_unchangedtable_name();

if (dbtype==DBTYPE_DB2)
		sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='T' order by TABNAME");
else if (dbtype==DBTYPE_ORACLE || dbtype == DBTYPE_DMSQL )
		sprintf(func.isql,"select table_name from user_tables");
else if (dbtype == DBTYPE_MYSQL)
{
	sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
}
else // SYBASE_DB MSSQL_DB
		sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U' order by name",dbname);

		char *pt=NULL;
		int tbnum;
		int reclen;
		func.func=SEL_ISQL_RESULTS;
		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
		if(retcode!=CS_SUCCEED)
		{
			fprintf(stderr,"\n �ӿ���ȡ����ʧ��!\n");

			if (datafmt) free (datafmt);
			if (pt) free (pt);

			return retcode;
		}
		tbnum=func.ret_roxnum;
		reclen=func.recorderlen;

		tbcount=0;

		char *tabName;//��¼����
		tabName = (char*) malloc(tbnum * reclen);
		memcpy(tabName,pt,tbnum*reclen);

		for(i=0;i<tbnum;i++)
		{
			if(check_tmptable_name(tabName+i*reclen)==1)
			{	//��ʱ����������Ϊ�ս�oracle��
				*(tabName+i*reclen)=0;
				continue;
			}
			if (!bAllTables)
			{
				if (check_unchangedtable_name(tabName+i*reclen)==1)
				{
					*(tabName+i*reclen)=0;
					continue;
				}
			}
			if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL )  ) //modify by chenkai ����mysql
			{			
				retcode=check_histable_name(tabName+i*reclen);
				// �����⣺ֻ���������; ��ʷ�⣺ֻ������ʷ��
				if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
				{
					*(tabName+i*reclen)=0;
					continue;
				}
			}
			tbcount++;
		}
		fprintf(stderr,"\n �Ѵӿ���ȡ������: %d ����\n",tbcount);
		tbcount=0;
		int maxnamelen=0;
		int tmplen;
		for(i=0;i<tbnum;i++) 
		{
			tmplen=strlen(tabName+i*reclen);
			if(maxnamelen<tmplen) maxnamelen=tmplen;
		}
		maxnamelen+=10+strlen(dbname);

		for(i=0;i<tbnum;i++)
		{
			strcpy(func.table,tabName+i*reclen);
			if(func.table[0]==0) continue;
			tbcount++;
			strcpy(filename,func.table);
			sprintf(func.table,"%s",filename);
			if(bPrompt==1)
			{
				sprintf(strstdin,"\n%d: �Ƿ�װ���¼���� [ %s ]  : ��/��/�˳� (Y/N/Q)? ",tbcount,func.table);
				inchar=get_input_char(strstdin,1);
				if(inchar==(int)'Q') break;
				if (inchar==(int)'N') continue;
			}

			tmplen=maxnamelen-strlen(func.table);
			realize_in_file(&func,filename,tmplen,tbcount);
		}

		if (!bAllTables)
		{
			fprintf(stderr,"\n \n  ����%d����δ�������ݿ⣬���赼����Ӳ���[-A]�����dbmt in help: \n",UnchangedTableNameCount);

			char tmpname[50];

			for(int i=0;i<UnchangedTableNameCount;i++)
			{
				strcpy(tmpname,pUnchangedTableName+i*UnchangedTableNameLen);
				fprintf(stderr,"  %d  %s \n",i+1,tmpname);
			}

		}

		if(pt) free(pt);

	}
	
	fprintf(stderr,"\n");
	dbsingle.Close();
	return 1;
}

int CDbOperate::do_in_hisdata()	/* ��ʷ���� */
{
	int serverno=-1;
	int err=0,deltype=-1;
	int dataYM=-1,datayear,datamon;
	int i,retcode,inchar;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));

	char filename[255];
    if(strcmp(mode,"a")==0)
		func.his.flag=0;	/* ׷�Ӽ�¼�������Ǳ����Ƿ��м�¼ */
		else
        if(strcmp(mode,"b")==0)
        func.his.flag=2;	/* ����� */
        else
        if(strcmp(mode,"c")==0)
        func.his.flag=3;	/*��ִ��isql */
	serverno=get_serverno();
	if(serverno<0)
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩ�������������� dbmt in help ������\n");
	    return 0;
	}

	for(i=1;i<argc;i++)		// -DTn nΪҪװ�����������
		if(strncmp(argv[i],"-DT",3)==0)
		{
			if(argv[i][3]!=0)
			{
				if(sscanf(argv[i]+3,"%d",&deltype)<=0) deltype=-1;
				break;
			}
		}
	if(deltype<1 || deltype>HISDATANUM) deltype=-1;

	for(i=1;i<argc;i++)
	{
		if(strncmp(argv[i],"-YM",3)==0)
				dataYM=atoi(argv[i]+3);
	}

	if(dataYM<=0)
	{
		fprintf(stderr,"\n�����ʽ���ԣ�δ�ṩ���ڡ����� dbmt in help ������\n");
		return 0;
	}
	datayear=dataYM/100;
	datamon=dataYM%100;
	if(datayear<1998 || datayear>2999 || datamon<1 || datamon>12)
	{
		fprintf(stderr,"\n�ṩ�����ڲ��ԡ����� dbmt in help ������\n");
		return 0;
	}

	if(db_start(serverno)!=CS_SUCCEED) return 0;

		/* ���ϵͳ����Ա���� */	
	int dbflag=0;

	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}

	fprintf(stderr,"\n��װ����ʷ����֮ǰ��ȷ����ȥ�ηݵ��ļ��ڵ�ǰ(��ָ��)Ŀ¼��");
	fprintf(stderr,"\n�������ݿ���û��%d��%d�µ����ݡ�",datayear,datamon);
	fprintf(stderr,"\n\n---------------------------\n");
	for(i=0;i<HISDATANUM;i++)
		fprintf(stderr,"    %d��%s\n",i+1,datatype[i]);
	fprintf(stderr,"    0���˳�\n");

	int dataflag;
	if(deltype==-1)
	{
		do
		{
			fprintf(stderr,"\n��ѡ��Ҫװ�����ʷ�������ͣ�[");
			for(i=1;i<=HISDATANUM;i++) fprintf(stderr,"%d,",i);
			fprintf(stderr,"0] : ");
			gets(strstdin);
			inchar=strstdin[0];
		}
		while(inchar<'0' || inchar>('0'+HISDATANUM) );
		
		if(inchar=='0') return 1;

		dataflag=inchar-'0';
	}
	else dataflag=deltype;
	if (bPrompt==1)
	{
		sprintf(strstdin,"\n�Ƿ�װ�� %04d��%02d�� ��%s [Y/N]: ",
			datayear,datamon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin);
		if(inchar=='N') return 1;
		sprintf(strstdin,"\nȷ��װ�� %04d��%02d�� ��%s [Y/N]: ",
			datayear,datamon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin);
		if(inchar=='N') return 1;
	}
	if (dbtype==DBTYPE_ORACLE || dbtype==DBTYPE_DMSQL )
		strcpy(func.dbname,"modeldb");
	else
		strcpy(func.dbname,"hisdb");
	if(dataflag==1 || dataflag==2)	//�������ݡ�ͳ�ƽ��
	{
		// yc199901 -> file yc199901
		sprintf(func.table,"yc%04d%02d",datayear,datamon);
		sprintf(filename,"yc%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// kwh199901 -> file kwh199901
		sprintf(func.table,"kwh%04d%02d",datayear,datamon);
		sprintf(filename,"kwh%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// dayyc199901 -> dayyc199901
		sprintf(func.table,"dayyc%04d%02d",datayear,datamon);
		sprintf(filename,"dayyc%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// daykwh199901 ->daykwh199901
		sprintf(func.table,"daykwh%04d%02d",datayear,datamon);
		sprintf(filename,"daykwh%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// monthyc1999 -> file monthyc199901,monthyc199902...
		sprintf(func.table,"monthyc%04d",datayear);
		sprintf(filename,"monthyc%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// monthkwh1999 -> file monthkwh199901,monthkwh199902...
		sprintf(func.table,"monthkwh%04d",datayear);
		sprintf(filename,"monthkwh%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
			
		// �豸����ͳ�Ʊ� -> file �豸����ͳ�Ʊ�199901,...
		sprintf(func.table,"�豸����ͳ�Ʊ�");
		sprintf(filename,"�豸����ͳ�Ʊ�%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);

		
	}

	if(dataflag==1 || dataflag==3)	//����Ͳ�����¼
	{
		// ������ʷ�����199901 -> file ������ʷ�����199901,...
		sprintf(func.table,"������ʷ�����%04d%02d",datayear,datamon);
		sprintf(filename,"������ʷ�����%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);

		// ������ʷ�����199901 -> file ������ʷ�����199901,...
		sprintf(func.table,"������ʷ�����%04d%02d",datayear,datamon);
		sprintf(filename,"������ʷ�����%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
	}
	if(dataflag==1 || dataflag==4)	//׷������
	{

		// ׷�������� -> file ׷��������199901,...
		sprintf(func.table,"pdrdescribe");
		sprintf(filename,"pdrdescribe%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// ׷�����ݱ� -> file ׷�����ݱ�199901,...
		sprintf(func.table,"pdrdata");
		sprintf(filename,"pdrdata%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
	}

	if(dataflag==1 || dataflag==5)	//�뼶��������
	{
		// sdata199901 -> file sdata199901
		sprintf(func.table,"sdata%04d%02d",datayear,datamon);
		sprintf(filename,"sdata%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);

		//רΪ����ˮ����� begin
		sprintf(func.table,"���ر�λͳ�Ʊ�%04d%02d",datayear,datamon);
		sprintf(filename,"���ر�λͳ�Ʊ�%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		//רΪ����ˮ����� end
	}

	fprintf(stderr,"\n");
	return 1;
}

/*  ����->�ļ�
��������ļ���Ϊ�����
��ʷ��������Ϊ��λ������Ϊ ����yyyymm ,
	�����������ʷ�����200201��monyc200201, 
	�������������µĳ��⣬�� yc200201,dayyc200201
��ʷ���ݣ�
    dbmt out  -Sn -YM���� [-debug]
		-Sn		: -S1 ��-S2
		-YM		: ���£�yyyymm ��2002��1��Ϊ 200201 
������
	dbmt out  <-D���� | -T����> [-P] [-debug]
		-Sn		: -S1(sybase11) ��-S2(sybase22)
		-D����  : �������б� modeldb,hisdb
		-T		: ĳ���� ����.dbo.����
		-P		: ͬ-D���ʹ�ã���ÿ������ʾ�Ƿ������
*/
int CDbOperate::do_out_file()
{
	char dbname[100];
	if (get_dbname(dbname)==1)
	{
		if (strcmp(dbname,"hisdb")==0)
		{
			int ret = do_out_hisdata();	/* ��ʷ���� */
			return ret;
		}
		else if(strcmp(dbname,"modeldb")==0)
		{
			int ret = do_out_para();		/* ���� */
			return ret;
		}
	}
	fprintf(stderr,"\n�����ʽ����! δ�ṩ���� ������ dbmt out help ������\n");
	return 0;
}

int CDbOperate::realize_out_file(FUNC_STR *func,char *filename,int null_num,int id)
{
	CS_RETCODE retcode;
	CS_DATAFMT *datafmt=0;
	int i;
	FILE *stream=NULL;

	if(id>=0) fprintf(stderr,"\n%d ���������[%s] ...",id,func->table);
	else fprintf(stderr,"\n ���������[%s] ...",func->table);
	
	func->func=GET_TABLE_INFO;
	retcode = dbsingle.GetTableInfo(func,&datafmt);

	if (retcode!=CS_SUCCEED)
	{

		if (datafmt) free (datafmt);

		fprintf (stderr,"\n %s ��ʧ��!",func->table);
		return CS_FAIL;
	}

	stream=fopen(filename,"wt");
	if(stream==NULL)
	{
		

		free(datafmt);

		return CS_FAIL;
	}

	/* �ηݱ�ṹ */
	fputs("--DATAFMT: ",stream);
	for(i=0;i<func->colnum;i++)
	{
		func->col[i].condition = NO;
		func->col[i].select = YES;
		func->col[i].order_d = i;
		func->col[i].order = -1;
		fputs(datafmt[i].name,stream);
		fputs("\t",stream);
	}
	fputs("\n",stream);

	/////////////add begin 20150310

	tmpfunc=*func;
	tmpdatafmt=datafmt;
	tmpstream=stream;
	recordnum=0;
	void *client=0;
	retcode=dbsingle.SetBlkOutProcFunc(proc,client,MAXOUTROWNUM);//���ûص�����������

	func->func = BLK_OUT;
	void* pt;
	retcode = dbsingle.BlkCopyOut(func,datafmt,&pt);

	retcode=dbsingle.SetBlkOutProcFunc(0,client,MAXOUTROWNUM);//ȡ���ص�����

	/////////////add  end 20150310


	//for(i=0;i<null_num;i++) putchar(' ');
	putchar('\t');
	if(recordnum<0)
	{
		fprintf(stderr,"ʧ��!  ");
	}
	else 
	{
		fprintf(stderr,"�ɹ� ���� %d ����¼. ",recordnum);

	}

	fclose(stream);

	if (datafmt) free(datafmt);

	return 1;
}

int CDbOperate::do_out_para()
{   
    CS_RETCODE      retcode;
	CS_SMALLINT	serverno,i;
	char tablename[100];
	char dbname[100];
	int tbcount=0;
    int  inchar,hisflag=0;
	FUNC_STR func;
	CS_DATAFMT *datafmt;

	serverno=get_serverno();
	if(serverno<0)
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩ�������������� dbmt out help ������\n");
	    return 0;
	}

	if(get_tablename(tablename)==1) 
	{
		oneallflag=1;
		if (!get_dbname(dbname))
		{
			fprintf(stderr,"\n   δ�ṩ����������dbmt out help ����!");
			return 0;
		}
	}
	else if(get_dbname(dbname)==1) oneallflag=2;
	else
	{
		fprintf(stderr,"\n�����ʽ����! δ�ṩ��������������� dbmt out help ������\n");
		return 0;
	}

	
	if(db_start(serverno)!=CS_SUCCEED) return 0;
		/* ���ϵͳ����Ա���� */	
	int dbflag=0;
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}
	
	if(oneallflag==1) fprintf(stderr,"\n����� %s :\n",tablename);
	else if(oneallflag==2) fprintf(stderr,"\n����� %s :\n",dbname);

	if (bPrompt==1)
	{
		if(oneallflag==1)
			sprintf(strstdin,"\n�Ƿ������ %s ���ļ� (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n�Ƿ������ %s �е����б��ļ� (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
		if(oneallflag==1)
			sprintf(strstdin,"\nȷ������� %s ���ļ� (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\nȷ������� %s �е����б��ļ� (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
	}

	char filename[255];
	char *pstr;

	if(oneallflag==1)
	{
		pstr=strrchr(tablename,'.');
		if(pstr==NULL) strcpy(filename,tablename);
		else strcpy(filename,pstr+1);

		strcpy(func.table,tablename);
		strcpy(func.dbname,dbname);
		sprintf(func.isql,"select * from %s ",tablename);
		retcode=realize_out_file(&func,filename);
		return retcode;
	}
	else
	{
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) || (dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL )  )
		if(pHistableName==NULL) load_histable_name();

if (dbtype==DBTYPE_DB2)
{
		sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='T' order by TABNAME");
}
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
{
		sprintf(func.isql,"select table_name from user_tables");
}
else if (dbtype==DBTYPE_MYSQL)
{
	sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
}
else // SYBASE_DB MSSQL_DB
{
		sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U' order by name",dbname);
}

		char *pt=NULL;
		int tbnum;
		int reclen;
		func.func=SEL_ISQL_RESULTS;
		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
		if(retcode!=CS_SUCCEED)
		{
			fprintf(stderr,"\n �ӿ���ȡ����ʧ��!\n");

			if (datafmt) free (datafmt);
			if (pt) free (pt);

			return CS_FAIL;
		}
		tbnum=func.ret_roxnum;
		reclen=func.recorderlen;
		tbcount=0;
		
		char *tabName;//��¼����
		tabName = (char*) malloc(tbnum * reclen);
		memcpy(tabName,pt,tbnum*reclen);
		for(i=0;i<tbnum;i++)
		{
			if(check_tmptable_name(tabName+i*reclen)==1)
			{	//��ʱ����������Ϊ��
				*(tabName+i*reclen)=0;
				continue;
			}
			if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) || (dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL )  )
			{		
				retcode=check_histable_name(tabName+i*reclen);
						// �����⣺ֻ���������; ��ʷ�⣺ֻ������ʷ��
				if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
				{
						*(tabName+i*reclen)=0;
						continue;
				}
			}
		tbcount++;
		}
		fprintf(stderr,"\n �Ѵӿ���ȡ������: %d ����\n",tbcount);
		tbcount=0;		
		int maxnamelen=0;
		int tmplen;
		for(i=0;i<tbnum;i++) 
		{
			tmplen=strlen(tabName+i*reclen);
			if(maxnamelen<tmplen) maxnamelen=tmplen;
		}
		maxnamelen+=10+strlen(dbname);

		for(i=0;i<tbnum;i++)
		{
			strcpy(func.table,tabName+i*reclen);
			if(func.table[0]==0) continue;
			tbcount++;
			strcpy(filename,tabName+i*reclen);
			strcpy(func.dbname,dbname);
			sprintf(func.table,"%s",filename);

			if(bPrompt==1)
			{
				sprintf(strstdin,"\n%d: �Ƿ������ [ %s ] ���ļ� [%s] : ��/��/�˳� (Y/N/Q)? ",tbcount,func.table,filename);
				inchar=get_input_char(strstdin,1);
				if(inchar==(int)'Q') break;
				if (inchar==(int)'N') 
					continue;
			}
			sprintf(func.isql,"select * from %s ",func.table);
			tmplen=maxnamelen-strlen(func.table);
			realize_out_file(&func,filename,tmplen,tbcount);
		}

		if (datafmt) free ( datafmt);
		if(pt) free(pt);

	}
	
	fprintf(stderr,"\n");
	dbsingle.Close();
	return 1;
}

int CDbOperate::do_out_hisdata()
{
	int serverno=-1;
	int err=0,deltype=-1;
	int dataYM=-1,datayear,datamon;
	int i,retcode,inchar,mon_num;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	char filename[255];

	serverno=get_serverno();
	if(serverno<0)
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩ�������������� dbmt out help ������\n");
	    return 0;
	}

	for(i=1;i<argc;i++)		// -DTn nΪҪ�ηݵ���������
		if(strncmp(argv[i],"-DT",3)==0)
		{
			if(argv[i][3]!=0)
			{
				if(sscanf(argv[i]+3,"%d",&deltype)<=0) deltype=-1;
				break;
			}
		}
	if(deltype<1 || deltype>HISDATANUM) deltype=-1;

	for(i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-P")==0)
			bPrompt=1;
		else if(strncmp(argv[i],"-YM",3)==0)
				dataYM=atoi(argv[i]+3);
	}

	if(dataYM==-1)
	{
		fprintf(stderr,"\n�����ʽ���ԣ�δ�ṩ���ڡ����� dbmt out help ������\n");
		return 0;
	}
	datayear=dataYM/100;
	datamon=dataYM%100;
	year=datayear;
	month=datamon;
	if(datayear<1998 || datayear>2999 || datamon<1 || datamon>12)
	{
		fprintf(stderr,"\n�ṩ�����ڲ��ԡ����� dbmt out help ������\n");
		return 0;
	}

	if(db_start(serverno)!=CS_SUCCEED) return 0;

		/* ���ϵͳ����Ա���� */	
	int dbflag=0;
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}

	fprintf(stderr,"\n");
	for(i=0;i<HISDATANUM;i++)
		fprintf(stderr,"    %d��%s\n",i+1,datatype[i]);
	fprintf(stderr,"    0���˳�\n");

	int dataflag;
	if(deltype==-1)
	{
		do
		{
			fprintf(stderr,"\n��ѡ��Ҫ�ηݵ���ʷ�������ͣ�[");
			for(i=1;i<=HISDATANUM;i++) fprintf(stderr,"%d,",i);
			fprintf(stderr,"0] : ");
			gets(strstdin);
			inchar=strstdin[0];
		}while(inchar<'0' || inchar>('0'+HISDATANUM) );
		
		if(inchar=='0') return 1;

		dataflag=inchar-'0';
	}
	else dataflag=deltype;

	if (bPrompt==1)
	{
		sprintf(strstdin,"\n�Ƿ��η� %04d��%02d�� ��%s [Y/N]: ",
			datayear,datamon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin);
		if(inchar=='N') return 1;
		sprintf(strstdin,"\nȷ���η� %04d��%02d�� ��%s [Y/N]: ",
			datayear,datamon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin);
		if(inchar=='N') return 1;
	}

	if (dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
		strcpy(func.dbname,"modeldb");
	else
		strcpy(func.dbname,"hisdb");

	if(dataflag==1 || dataflag==2)	//�������ݡ�ͳ�ƽ��
	{
		// yc199901 -> file yc199901
		sprintf(func.table,"yc%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"yc%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// kwh199901 -> file kwh199901
		sprintf(func.table,"kwh%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"kwh%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// dayyc199901 -> dayyc199901
		sprintf(func.table,"dayyc%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"dayyc%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// daykwh99901 ->daykwh199901
		sprintf(func.table,"daykwh%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"daykwh%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// monthyc1999 -> file monthyc199901,monyc199902...
		sprintf(func.table,"monthyc%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"monthyc%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);
		
		// monthkwh1999 -> file monkwh199901,monthkwh199902...
		sprintf(func.table,"monthkwh%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"monthkwh%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);
		
		// �豸����ͳ�Ʊ� -> file �豸����ͳ�Ʊ�199901,...
		sprintf(func.table,"�豸����ͳ�Ʊ�");
		sprintf(func.isql,"select * from %s where ��������>=%04d%02d01 and ��������<=%04d%02d31",
			func.table,dataYM/100,dataYM%100,dataYM/100,dataYM%100);
		sprintf(filename,"�豸����ͳ�Ʊ�%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

	}

	if(dataflag==1 || dataflag==3)	//����Ͳ�����¼
	{
		// ������ʷ�����199901 -> file ������ʷ�����199901,...
		sprintf(func.table,"������ʷ�����%04d%02d",year,month);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"������ʷ�����%04d%02d",year,month);
		realize_out_file(&func,filename);
		
				
		// ������ʷ�����199901 -> file ������ʷ�����199901,...
		sprintf(func.table,"������ʷ�����%04d%02d",year,month);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"������ʷ�����%04d%02d",year,month);
		realize_out_file(&func,filename);

	}
	if(dataflag==1 || dataflag==4)	//׷������
	{
		// pdrdescribe -> file pdrdescribe199901,...
		sprintf(func.table,"pdrdescribe");
		sprintf(func.isql,"select * from %s where ������>=%04d%02d01 and ������<=%04d%02d31",
			func.table,dataYM/100,dataYM%100,dataYM/100,dataYM%100);
		sprintf(filename,"pdrdescribe%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// pdrdata -> file pdrdata199901,...

		time_t time1,time2;
		struct tm tmtime1,tmtime2;
		memset(&tmtime1,0,sizeof(tmtime1));
		memset(&tmtime2,0,sizeof(tmtime2));
		tmtime1.tm_isdst=-1;
		tmtime2.tm_isdst=-1;
		tmtime1.tm_sec=0; tmtime1.tm_min=0; tmtime1.tm_hour=0;
		tmtime1.tm_mday=1; tmtime1.tm_mon=datamon-1; tmtime1.tm_year=datayear-1900;
		tmtime2.tm_sec=0; tmtime2.tm_min=0; tmtime2.tm_hour=0;
		tmtime2.tm_mday=1;
		if(datamon<12) { tmtime2.tm_mon=datamon; tmtime2.tm_year=datayear-1900; }
		else { tmtime2.tm_mon=0; tmtime2.tm_year=datayear+1-1900; }
		time1=mktime(&tmtime1);
		time2=mktime(&tmtime2);

		sprintf(func.table,"pdrdata");
		sprintf(func.isql,"select * from %s where ʱ��>=%d and ʱ��<=%d",
			func.table,time1,time2);
		sprintf(filename,"pdrdata%04d%02d",datayear,datamon);
		realize_out_file(&func,filename);
	}

	if(dataflag==1 || dataflag==5)	//�뼶��������
	{
		// sdata199901 -> file sdata199901
		sprintf(func.table,"sdata%04d%02d",datayear,datamon);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"sdata%04d%02d",datayear,datamon);
		realize_out_file(&func,filename);

		//רΪ����ˮ����� begin
		sprintf(func.table,"���ر�λͳ�Ʊ�%04d%02d",datayear,datamon);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"���ر�λͳ�Ʊ�%04d%02d",datayear,datamon);
		realize_out_file(&func,filename);
		//רΪ����ˮ����� end
	}

	fprintf(stderr,"\n");
	return 1;
}

int CDbOperate::do_clean()
{   
    CS_RETCODE      retcode;
	CS_DATAFMT *datafmt;
	CS_SMALLINT	serverno,i,j;
	char tablename[100];
	char dbname[100];
	int tbcount=0;
    int  inchar,hisflag=0;
	FUNC_STR func;

	serverno=get_serverno();
	if(serverno<1 || serverno>4)
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩ��������������������ڡ����� dbmt clean help ������\n");
	    return 0;
	}

	if(get_tablename(tablename)==1) 
	{
		oneallflag=1;
		if (!get_dbname(dbname))
		{
			fprintf(stderr,"\n   δ�ṩ����������dbmt clean help ����!");
			return 0;
		}
	}
	else if(get_dbname(dbname)==1) oneallflag=2;
	else
	{
		fprintf(stderr,"\n�����ʽ����! δ�ṩ��������������� dbmt clean help ������\n");
		return 0;
	}
	if(oneallflag==2)
		for(i=1;i<argc;i++)
		{
			if(strcmp(argv[i],"-P")==0)
				bPrompt=1;
		}

	if(oneallflag==2)
	{
		if(strcmp(dbname,"hisdb")==0)
		{
			fprintf(stderr,"\n %sΪ��ʷ�⣬���������ϴ󣬽������ dbmt del ��ɾ�����ݡ�",dbname);
			fprintf(stderr,"\n ��ϸ�������� dbmt del help ����");
			sprintf(strstdin,"\n �Ƿ����? (Y/N)");
			inchar=get_input_char(strstdin);
			if(inchar=='N') return 0;
			hisflag=1;
		}
	}

	if(db_start(serverno)!=CS_SUCCEED) return 0;
		/* ���ϵͳ����Ա���� */	
	int dbflag=0;
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}

	if(oneallflag==1) fprintf(stderr,"\n��ձ� %s :\n",tablename);
	else if(oneallflag==2) fprintf(stderr,"\n��տ� %s �����б�:\n",dbname);

	if (bPrompt==1)
	{
		if(oneallflag==1)
			sprintf(strstdin,"\n�Ƿ���ձ� %s (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n�Ƿ���տ� %s �е����б� (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
		if(oneallflag==1)
			sprintf(strstdin,"\nȷ����ձ� %s (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\nȷ����տ� %s �е����б� (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
	}

	if(oneallflag==1)
	{
		strcpy(func.table,tablename);

if (dbtype==DBTYPE_DB2)
		sprintf(func.isql,"delete from %s where 1=1",tablename);
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
		sprintf(func.isql,"truncate table %s ",tablename);
else if (dbtype==DBTYPE_MYSQL)
		sprintf(func.isql,"delete from %s where 1=1",tablename);
else // SYBASE_DB MSSQL_DB
		sprintf(func.isql,"truncate table %s.dbo.%s ",dbname,tablename);

		fprintf(stderr,"\n ������ձ�[%s] ...",tablename);

		func.func=ISQL_COMMAND;
		strcpy(func.dbname,dbname);
		retcode=dbsingle.IsqlCommand(&func,func.isql);
		if(retcode==CS_SUCCEED) fprintf(stderr,"��ȷ");
		else fprintf(stderr,"����");
		return retcode;
	}
	else
	{
if ((dbtype==DBTYPE_DB2) ||(dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) )		//modify by chenkai ����mysql
		if(pHistableName==NULL) load_histable_name();
// �������û�� [-A]
if (!bAllTables)
		load_unchangedtable_name();

if (dbtype==DBTYPE_DB2)
		sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='T' order by TABNAME");
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
		sprintf(func.isql,"select table_name from user_tables");
else if (dbtype==DBTYPE_MYSQL)
//		sprintf(func.isql,"SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
		sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
else // SYBASE_DB MSSQL_DB
		sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U' order by name",dbname);


		char *pt=NULL;
		int tbnum;
		int reclen;
		func.func=SEL_ISQL_RESULTS;

		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
		if(retcode!=CS_SUCCEED)
		{
			fprintf(stderr,"\n �ӿ���ȡ����ʧ��!\n");
			if (datafmt) free (datafmt);
			if (pt) free (pt);
			return CS_FAIL;
		}
		tbnum=func.ret_roxnum;
		reclen=func.recorderlen;

		tbcount=0;
		for(i=0;i<tbnum;i++)
		{
			if(check_tmptable_name(pt+i*reclen)==1)
			{	//��ʱ����������Ϊ��
				*(pt+i*reclen)=0;
				continue;
			}
			if (!bAllTables)
			{
				if (check_unchangedtable_name(pt+i*reclen)==1)
				{
					*(pt+i*reclen)=0;
					continue;
				}
			}
if ((dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) )		//modify by chenkai ����mysql
{	
	retcode=check_histable_name(pt+i*reclen);
				// �����⣺ֻ���������; ��ʷ�⣺ֻ������ʷ��
			if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
			{
				*(pt+i*reclen)=0;
				continue;
			}
}
			tbcount++;
		}
		fprintf(stderr,"\n �Ѵӿ���ȡ������: %d ����\n",tbcount);
		tbcount=0;		
		int maxnamelen=0;
		int tmplen;
		for(i=0;i<tbnum;i++) 
		{
			tmplen=strlen(pt+i*reclen);
			if(maxnamelen<tmplen) maxnamelen=tmplen;
		}
		maxnamelen+=10+strlen(dbname);

		for(i=0;i<tbnum;i++)
		{
			strcpy(func.table,pt+i*reclen);
			if(func.table[0]==0) continue;
			tbcount++;

			sprintf(func.table,"%s",pt+i*reclen);

			if(bPrompt==1)
			{
				sprintf(strstdin,"\n%d: �Ƿ���ձ� [ %s ] : ��/��/�˳� (Y/N/Q)? ",tbcount,func.table);
				inchar=get_input_char(strstdin,1);
				if(inchar==(int)'Q') break;
				if (inchar==(int)'N') 
					continue;
			}

if (dbtype==DBTYPE_DB2)
			sprintf(func.isql,"delete from %s where 1=1",func.table);
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL  )
			sprintf(func.isql,"truncate table %s ",func.table);
else if (dbtype == DBTYPE_MYSQL)
{
	sprintf(func.isql,"delete from %s where 1=1",func.table);
}
else // SYBASE_DB MSSQL_DB
			sprintf(func.isql,"truncate table %s ",func.table);

			func.func=ISQL_COMMAND;
			tmplen=maxnamelen-strlen(func.table);

			fprintf(stderr,"\n%d: ������ձ� [ %s ] ...",tbcount,func.table);
			for(j=0;j<tmplen;j++) fprintf(stderr," ");
			retcode=dbsingle.IsqlCommand(&func,func.isql);
			if(retcode==CS_SUCCEED) fprintf(stderr,"�ɹ�");
			else fprintf(stderr,"ʧ��");

		}
		if (!bAllTables)
		{
			fprintf(stderr,"\n \n  ����%d����δ��������������Ӳ��� [-A]���dbmt clear help: \n",UnchangedTableNameCount);

			char tmpname[50];

			for(int i=0;i<UnchangedTableNameCount;i++)
			{
				strcpy(tmpname,pUnchangedTableName+i*UnchangedTableNameLen);
				fprintf(stderr,"  %d  %s \n",i+1,tmpname);
			}

		}

		if (datafmt) free (datafmt);
		if(pt) free(pt);
	}
	
	fprintf(stderr,"\n");
	dbsingle.Close();
	return 1;
}

int CDbOperate::do_drop()
{   
    CS_RETCODE      retcode;
	CS_SMALLINT	serverno,i,j;
	CS_DATAFMT *datafmt;
	char tablename[100];
	char dbname[100];
	int tbcount=0;
    int  inchar,hisflag=0;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));

	serverno=get_serverno();
	if(serverno<1||serverno>4)
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩ��������������������ڡ����� dbmt drop help ������\n");
	    return 0;
	}

	if(get_tablename(tablename)==1) 
	{
		oneallflag=1;
		if (!get_dbname(dbname))
		{
			fprintf(stderr,"\n   δ�ṩ����������dbmt drop help ����!");
			return 0;
		}
	}
	else if(get_dbname(dbname)==1) oneallflag=2;
	else
	{
		fprintf(stderr,"\n�����ʽ����! δ�ṩ��������������� dbmt drop help ������\n");
		return 0;
	}
	if(oneallflag==2)
		for(i=1;i<argc;i++)
		{
			if(strcmp(argv[i],"-P")==0)
				bPrompt=1;
		}

	if(oneallflag==2)
	{
		if(strcmp(dbname,"hisdb")==0)
		{
			fprintf(stderr,"\n %sΪ��ʷ�⣬������� dbmt del ��ɾ�����ݡ�",dbname);
			fprintf(stderr,"\n ��ϸ�������� dbmt del help ����");
			sprintf(strstdin,"\n �Ƿ����? (Y/N)");
			inchar=get_input_char(strstdin);
			if(inchar=='N') return 0;
			hisflag=1;
		}
	}

	if(db_start(serverno)!=CS_SUCCEED) return 0;
		/* ���ϵͳ����Ա���� */	
	int dbflag=0;
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}

	if(oneallflag==1) fprintf(stderr,"\nɾ���� %s :\n",tablename);
	else if(oneallflag==2) fprintf(stderr,"\nɾ���� %s �����б�:\n",dbname);

	if (bPrompt==1)
	{
		if(oneallflag==1)
			sprintf(strstdin,"\n�Ƿ�ɾ���� %s (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n�Ƿ�ɾ���� %s �е����б� (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
		if(oneallflag==1)
			sprintf(strstdin,"\nȷ��ɾ���� %s (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\nȷ��ɾ���� %s �е����б� (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
	}
	int maxnamelen=0;
	int tmplen;
	char *pt=NULL;
	int tbnum;
	int reclen;

	if(oneallflag==1)
	{
		strcpy(func.table,tablename);

		sprintf(func.isql,"drop table %s ",tablename);
		fprintf(stderr,"\n ����ɾ����[%s] ...",tablename);

		func.func=ISQL_COMMAND;
		retcode=dbsingle.IsqlCommand(&func,func.isql);
		if(retcode==CS_SUCCEED) fprintf(stderr,"��ȷ");
		else fprintf(stderr,"����");
		return retcode;
	}
	else
	{
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) )		//modify by chenkai ����mysql
		if(pHistableName==NULL) load_histable_name();

		if(hisflag==0)
		{
if (dbtype==DBTYPE_DB2)
			sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='V' order by TABNAME");
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL  )
		sprintf(func.isql,"select table_name from user_tables");
else // SYBASE_DB MSSQL_DB
			sprintf(func.isql,"select name from %s.dbo.sysobjects where type='V' order by name",dbname);

			func.func=SEL_ISQL_RESULTS;
			retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
			if(retcode!=CS_SUCCEED)
			{
				fprintf(stderr,"\n �ӿ���ȡ��ͼ��ʧ��!\n");
				if (datafmt) free (datafmt);
				if (pt) free (pt);
				return CS_FAIL;
			}
			tbnum=func.ret_roxnum;
			reclen=func.recorderlen;
			fprintf(stderr,"\n �Ѵӿ���ȡ����ͼ��: %d ����ͼ��\n",tbnum);
			
			for(i=0;i<tbnum;i++) 
			{
				tmplen=strlen(pt+i*reclen);
				if(maxnamelen<tmplen) maxnamelen=tmplen;
			}
			maxnamelen+=10+strlen(dbname);

			for(i=0;i<tbnum;i++)
			{
				strcpy(func.table,pt+i*reclen);

				if(bPrompt==1)
				{
					sprintf(strstdin,"\n%d: �Ƿ�ɾ����ͼ [ %s ] : ��/��/�˳� (Y/N/Q)? ",tbcount,func.table);
					inchar=get_input_char(strstdin,1);
					if(inchar==(int)'Q') break;
					if (inchar==(int)'N') 
						continue;
				}

				sprintf(func.isql,"use %s\r\ngo\r\ndrop view %s ",dbname,func.table);
				func.func=ISQL_COMMAND;
				tmplen=maxnamelen-strlen(func.table);

				fprintf(stderr,"\n%d: ����ɾ����ͼ [ %s ] ...",i,func.table);
				for(j=0;j<tmplen;j++) fprintf(stderr," ");
				retcode=dbsingle.IsqlCommand(&func,func.isql);
				if(retcode==CS_SUCCEED) fprintf(stderr,"�ɹ�");
				else fprintf(stderr,"ʧ��");

			}
			if (datafmt) free (datafmt);
			if(pt) free(pt);
		}

if (dbtype==DBTYPE_DB2)
		sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='T' order by TABNAME");
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
		sprintf(func.isql,"select table_name from user_tables");
else if (dbtype==DBTYPE_MYSQL)
{
	sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
}
else // SYBASE_DB MSSQL_DB
		sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U' order by name",dbname);

		func.func=SEL_ISQL_RESULTS;
		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
		if(retcode!=CS_SUCCEED)
		{
			fprintf(stderr,"\n �ӿ���ȡ����ʧ��!\n");
			if (datafmt) free (datafmt);
			return CS_FAIL;
		}
		tbnum=func.ret_roxnum;
		reclen=func.recorderlen;

		tbcount=0;
		for(i=0;i<tbnum;i++)
		{
			if(check_tmptable_name(pt+i*reclen)==1)
			{	//��ʱ����������Ϊ��
				*(pt+i*reclen)=0;
				continue;
			}
if ((dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) )		//modify by chenkai ����mysql
{	retcode=check_histable_name(pt+i*reclen);
				// �����⣺ֻ���������; ��ʷ�⣺ֻ������ʷ��
			if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
			{
				*(pt+i*reclen)=0;
				continue;
			}
}
			tbcount++;
		}
		fprintf(stderr,"\n �Ѵӿ���ȡ������: %d ����\n",tbcount);
		tbcount=0;
		for(i=0;i<tbnum;i++) 
		{
			tmplen=strlen(pt+i*reclen);
			if(maxnamelen<tmplen) maxnamelen=tmplen;
		}
		maxnamelen+=10+strlen(dbname);

		for(i=0;i<tbnum;i++)
		{
			strcpy(func.table,pt+i*reclen);
			if(func.table[0]==0) continue;
			tbcount++;
			sprintf(func.table,"%s",pt+i*reclen);
			
			if(bPrompt==1)
			{
				sprintf(strstdin,"\n%d: �Ƿ�ɾ���� [ %s ] : ��/��/�˳� (Y/N/Q)? ",tbcount,func.table);
				inchar=get_input_char(strstdin,1);
				if(inchar==(int)'Q') break;
				if (inchar==(int)'N') 
					continue;
			}

			sprintf(func.isql,"drop table %s ",func.table);
			func.func=ISQL_COMMAND;

			tmplen=maxnamelen-strlen(func.table);

			fprintf(stderr,"\n%d: ����ɾ���� [ %s ] ...",tbcount,func.table);
			for(j=0;j<tmplen;j++) fprintf(stderr," ");
			retcode=dbsingle.IsqlCommand(&func,func.isql);
			if(retcode==CS_SUCCEED) fprintf(stderr,"�ɹ�");
			else fprintf(stderr,"ʧ��");

		}
		if (datafmt) free (datafmt);
		if(pt) free(pt);
	}
	
	fprintf(stderr,"\n");
	dbsingle.Close();
	return 1;
}

int CDbOperate::do_disp()
{   

    CS_RETCODE      retcode;
	CS_SMALLINT	serverno;
	char dbname[20];
	char tablename[100];

	serverno=get_serverno();
	if(serverno<0)
	{
	    fprintf(stderr,"\n�����ʽ����! δ�ṩ�������������� dbmt disp help ������\n");
	    return 0;
	}
	if(db_start(serverno)!=CS_SUCCEED) return 0;

	if(get_dbname(dbname))
	{
		if(get_tablename(tablename))
		{
		return do_disp_table(dbname,tablename);		
		}		
	}

	if ( dbtype==DBTYPE_SYBASE) 
	{
		FUNC_STR tmpfunc;
		CS_DATAFMT *datafmt;
		tmpfunc.func=SEL_ISQL_RESULTS;
		sprintf(tmpfunc.isql,"declare @size1 int,@fsize1 int,@size2 int,@fsize2 int,@segm int,@segbit int,@segsize float,@freesize float\r\n\
			select @segm=segment from hisdb.dbo.syssegments where name='sacsegment'\r\n\
			select @segbit=power(2,@segm)\r\n\
			select @segsize=sum(size),@freesize=sum(curunreservedpgs(db_id('hisdb'),lstart,unreservedpgs)) from master.dbo.sysusages\
			 where segmap&@segbit=@segbit and dbid=db_id('hisdb')\r\n\
			select @size1=@segsize*2048.0/1048576.0\r\n\
			select @fsize1=@freesize*2048.0/1048576.0\r\n\
			select @segm=segment from hisdb.dbo.syssegments where name='pdrsegment'\r\n\
			select @segbit=power(2,@segm)\r\n\
			select @segsize=sum(size),@freesize=sum(curunreservedpgs(db_id('hisdb'),lstart,unreservedpgs)) from master.dbo.sysusages\
			 where segmap&@segbit=@segbit and dbid=db_id('hisdb')\r\n\
			select @size2=@segsize*2048.0/1048576.0\r\n\
			select @fsize2=@freesize*2048.0/1048576.0\r\n\
			select @size1,@fsize1,@size2,@fsize2\r\ngo\r\n");
		typedef struct _dbsize_
		{ 
			int size;
			int freesize;
			int pdrsize;
			int pdrfreesize;
		} dbsize;
		dbsize *pt;
		retcode=dbsingle.SelectIsqlResults(&tmpfunc,&datafmt,(void **)&pt);
		if(retcode!=CS_SUCCEED || tmpfunc.ret_roxnum<=0 ||pt->size==0)
		{
			retcode=CS_FAIL;
			if (datafmt) {
				free(datafmt);datafmt=NULL;}
			if (pt) {
				free (pt);pt=NULL;}
			fprintf(stderr,"\n������ʾ!\n");
			dbsingle.Close();
			return 0;
		}
		
		int dbspace,dbfreespace,pdrspace,pdrfreespace;
		dbspace=pt->size;
		dbfreespace= pt->freesize;
		pdrspace=pt->pdrsize;
		pdrfreespace=pt->pdrfreesize;
		fprintf(stderr,"\n----------------------------------------------------");
		if(dbspace>0)
			fprintf(stderr,"\n��ʷ����:  ����=%6d M , ʣ��=%6d M , ʣ��%3d%%",
				dbspace,dbfreespace,dbfreespace*100/dbspace);
		else 
			fprintf(stderr,"\n��ʷ����:  ����=%6d M , ʣ��=%6d M , ʣ��%3d%%",
				dbspace,dbfreespace,0);
		if(pdrspace>0)
			fprintf(stderr,"\n׷������:  ����=%6d M , ʣ��=%6d M , ʣ��%3d%%",
				pdrspace,pdrfreespace,pdrfreespace*100/pdrspace);
		else
			fprintf(stderr,"\n׷������:  ����=%6d M , ʣ��=%6d M , ʣ��%3d%%",
				pdrspace,pdrfreespace,0);
		fprintf(stderr,"\n----------------------------------------------------\n");

		sprintf(tmpfunc.isql,"declare @value1 int,@value2 int,@value3 int,@value4 int,@value5 int,@value6 int\r\n\
			select @value1=value from master.dbo.sysconfigures where name='number of user connections'\r\n\
			select @value2=value from master.dbo.sysconfigures where name='number of devices'\r\n\
			select @value3=value from master.dbo.sysconfigures\
			where name='procedure cache percent'\r\n\
			select @value4=value from master.dbo.sysconfigures where name='total memory'\r\n\
			select @value5=value from master.dbo.sysconfigures where name='default data cache' and comment='4K I/O Buffer Pool'\r\n\
			select @value6=value from master.dbo.sysconfigures where name='default data cache' and comment='16K I/O Buffer Pool'\r\n\
			select @value1,@value2,@value3,@value4,@value5,@value6\r\ngo\r\n");
		typedef struct _dboptimize
		{ 
			int value1;//����û���������
			int value2;//����豸������
			int value3;//���̻��������
			int value4;//���ڴ�����
			int value5;//���ݻ��� 4K
			int value6;//���ݻ��� 16K
		} dboptimize;
		dboptimize *ptt;
		retcode=dbsingle.SelectIsqlResults(&tmpfunc,&datafmt,(void **)&ptt);
		if(retcode!=CS_SUCCEED || tmpfunc.ret_roxnum<=0 )
		{
			retcode=CS_FAIL;
			if (datafmt) {
				free(datafmt);datafmt=NULL;}
			if (ptt) {
				free (ptt);ptt=NULL;}
			fprintf(stderr,"\n������ʾ!\n");
			dbsingle.Close();
			return 0;
		}
		float v4k,v16k;
		v4k=(float)ptt->value5/1024;
		v16k=(float)ptt->value6/1024;
		fprintf(stderr,"\n----------------------------------------------------");
		fprintf(stderr,"\n���ݿ�����:  ����û��������� %d ����",ptt->value1);
		fprintf(stderr,"\n���ݿ�����:  ����豸������ %d ����",ptt->value2);
		fprintf(stderr,"\n���ݿ�����:  ���̻�������� %d %%��",ptt->value3);
		fprintf(stderr,"\n���ݿ�����:  ���ڴ����� %d M��",ptt->value4/500);
		if(ptt->value5)
			fprintf(stderr,"\n���ݿ�����:  ���ݻ��� 4K: %0.2f M��",v4k);
		else
			fprintf(stderr,"\n���ݿ�����:  ���ݻ��� 4K: δ���䡣");
		if (ptt->value6)
			fprintf(stderr,"\n���ݿ�����:  ���ݻ��� 16K: %0.2f M��",v16k);
		else
			fprintf(stderr,"\n���ݿ�����:  ���ݻ��� 16K: δ���䡣");
		fprintf(stderr,"\n----------------------------------------------------\n");

		if (datafmt) {
			free(datafmt);datafmt=NULL;}
		if (ptt) {
			free (ptt);ptt=NULL;}
		if (pt) {
			free(pt);pt=NULL;}

	}//end if (dbtype==DBTYPE_SYBASE)
	else if (dbtype ==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
	{
		FUNC_STR func;
		CS_DATAFMT *datafmt=NULL;
		void *pt;
		int ptcnt;

		int totalnum[3],freenum[3];
		int maxconnectnum,i;
		float speedstorerate;
		char *tablespace[3]={"USER_DATA","TEMP_DATA","RB_DATA"};
		func.func=SEL_ISQL_RESULTS;
		strcpy(func.dbname,"modeldb");

		for (i=0;i<3;i++)
		{
			//��ȡ��ռ���ܴ�С
			sprintf(func.isql,"select sum(bytes)/1024 from dba_data_files where tablespace_name='%s'",tablespace[i]);
			retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void **)&pt);
			if(retcode!=CS_SUCCEED || func.ret_roxnum<=0 )
			{
				if (datafmt) {
					free(datafmt);datafmt=NULL;}
				if (pt) {
					free (pt);pt=NULL;}
				fprintf(stderr,"\n������ʾ!\n");
				dbsingle.Close();
				return retcode;
			}
			totalnum[i] = (int)dbsingle.GetData(0,&func,datafmt,pt,ptcnt,NULL);
			if (datafmt) {
				free(datafmt);datafmt=NULL;}
			if (pt) {
				free (pt);pt=NULL;}

			//��ȡ��ռ��ʣ���С
			sprintf(func.isql,"select sum(bytes)/1024 from dba_free_space where tablespace_name='%s'",tablespace[i]);
			retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void **)&pt);
			if(retcode!=CS_SUCCEED || func.ret_roxnum<=0 )
			{
				if (datafmt) free(datafmt);
				if (pt) free (pt);
				fprintf(stderr,"\n������ʾ!\n");
				dbsingle.Close();
				return retcode;
			}
			freenum[i] = (int)dbsingle.GetData(0,&func,datafmt,pt,ptcnt,NULL);
			if (datafmt) {
				free(datafmt);datafmt=NULL;}
			if (pt) {
				free (pt);pt=NULL;}
		}
		//��ȡ���ݿ����������
		strcpy(func.isql,"select to_number(value) from v$parameter where name='processes'");
		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void **)&pt);
		if(retcode!=CS_SUCCEED || func.ret_roxnum<=0 )
		{
			if (datafmt) free(datafmt);
			if (pt) free (pt);
			fprintf(stderr,"\n������ʾ!\n");
			dbsingle.Close();
			return retcode;
		}
		maxconnectnum=(int)dbsingle.GetData(0,&func,datafmt,pt,ptcnt,NULL);

		if (datafmt) {
			free(datafmt);datafmt=NULL;}
		if (pt) {
			free (pt);pt=NULL;}

		//��ȡ���ݿ⻺�������ٻ���������
		strcpy(func.isql,"select (a.value-b.value+c.value)*100/a.value from v$sysstat a,v$sysstat b,v$sysstat c where a.name='session logical reads' and b.name='physical reads' and c.name='physical reads direct'");
		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void **)&pt);
		if(retcode!=CS_SUCCEED || func.ret_roxnum<=0 )
		{
			if (datafmt) free(datafmt);
			if (pt) free (pt);
			fprintf(stderr,"\n������ʾ!\n");
			dbsingle.Close();
			return retcode;
		}
		speedstorerate=(float)dbsingle.GetData(0,&func,datafmt,pt,ptcnt,NULL);

		if (datafmt) {
			free(datafmt);datafmt=NULL;}
		if (pt) {
			free (pt);pt=NULL;}
		fprintf(stderr,"\n----------------------------------------------------");
	
		if(freenum[0]>0)
			fprintf(stderr,"\n\n�û���ռ�:  ����=%6d M , ʣ��=%6d M , ʣ��%3.2f%%",
				(int)(totalnum[0]/1024),(int)(freenum[0]/1024),((float)freenum[0]*100/(float)totalnum[0]));
		else 
			fprintf(stderr,"\n�û���ռ�:  ����=%6d M , ʣ��=%6d M , ʣ��%3.2f%%",
				(int)(totalnum[0]/1024),(int)(freenum[0]/1024),0);
		
		if(freenum[1]>0)
			fprintf(stderr,"\n��ʱ��ռ�:  ����=%6d M , ʣ��=%6d M , ʣ��%3.2f%%",
				(int)(totalnum[1]/1024),(int)(freenum[1]/1024),((float)freenum[1]*100/(float)totalnum[1]));
		else 
			fprintf(stderr,"\n��ʱ��ռ�:  ����=%6d M , ʣ��=%6d M , ʣ��%3.2f%%",
				(int)(totalnum[1]/1024),(int)(freenum[1]/1024),0);
		if(freenum[2]>0)
			fprintf(stderr,"\n�ع��οռ�:  ����=%6d M , ʣ��=%6d M , ʣ��%3.2f%%",
				(int)(totalnum[2]/1024),(int)(freenum[2]/1024),((float)freenum[2]*100/(float)totalnum[2]));
		else 
			fprintf(stderr,"\n�ع��οռ�:  ����=%6d M , ʣ��=%6d M , ʣ��%3.2f%%",
				(int)(totalnum[2]/1024),(int)(freenum[2]/1024),0);
		fprintf(stderr,"\n\n----------------------------------------------------\n");
		
		fprintf(stderr,"\n----------------------------------------------------\n");
		fprintf(stderr,"\n���ݿ�����:  ����û��������� %d ����",maxconnectnum);
		fprintf(stderr,"\n���ݿ�����:  ���ٻ��������ʣ� %2.2f%%��",speedstorerate);
		fprintf(stderr,"\n\n----------------------------------------------------\n");
	
	}//end if (dbtype ==DBTYPE_ORACLE)
	else 
	{
		fprintf(stderr,"\n������ʾ!\n");
	}
	dbsingle.Close();
	return 1;
}
int CDbOperate::do_disp_table(CS_CHAR *dbname,CS_CHAR *tablename)
{
	CS_RETCODE retcode;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_DATAFMT *datafmt;
	int flag=0,i=0;
	
	func.func=GET_TABLE_INFO;
	strcpy(func.table,tablename);
	strcpy(func.dbname,dbname);
	retcode=dbsingle.GetTableInfo(&func,&datafmt);
	if (retcode!=CS_SUCCEED) 
	{
		fprintf(stderr,"\n  ��ȡ%s����Ϣʧ��",tablename);
		return retcode;
	}
	fprintf(stderr,"\n%s	����			����",tablename);
	for (i=0;i<func.colnum;i++)
	{
		switch((datafmt+i)->datatype)
		{
		case CS_SMALLINT_TYPE:
			if (dbtype==DBTYPE_SYBASE)
				fprintf(stderr,"\n		%s:		smallint",(datafmt+i)->name);
			else 
				fprintf(stderr,"\n		%s:		number(5)",(datafmt+i)->name);
			break;
		case CS_TINYINT_TYPE:
			if (dbtype==DBTYPE_SYBASE)
				fprintf(stderr,"\n		%s:		tinyint",(datafmt+i)->name);
			else 
				fprintf(stderr,"\n		%s:		number(3)",(datafmt+i)->name);
			break;
		case CS_USHORT_TYPE:
			fprintf(stderr,"\n		%s:		uchar",(datafmt+i)->name);
			break;
		case CS_INT_TYPE:
			if (dbtype==DBTYPE_SYBASE)
				fprintf(stderr,"\n		%s:		int",(datafmt+i)->name);
			else 
				fprintf(stderr,"\n		%s:		number(10)",(datafmt+i)->name);
			break;
		case CS_CHAR_TYPE:
			fprintf(stderr,"\n		%s:		char[%02d]",(datafmt+i)->name,(datafmt+i)->maxlength);
			break;
		case CS_REAL_TYPE:
			fprintf(stderr,"\n		%s:		real",(datafmt+i)->name);
			break;
		case CS_FLOAT_TYPE:
			fprintf(stderr,"\n		%s:		float",(datafmt+i)->name);
			break;
		default:
			break;
		}
	}
	fprintf(stderr,"\n----------------------------------------------------");

	fprintf(stderr,"\n\n������	");
	for (i=0;i<func.colnum;i++)
		if (((datafmt+i)->status)&CS_KEY)
		{
			fprintf(stderr,"%s	",(datafmt+i)->name);
			flag++;
		}
	if (flag==0) 	fprintf(stderr,"������");
	fprintf(stderr,"\n----------------------------------------------------");

	fprintf(stderr,"\n\n������	\n");
	func.func=SEL_ISQL_RESULTS;
	strcpy(func.dbname,"modeldb");
	char *pt;
	char indexname[30];
	if (dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
		sprintf(func.isql,"select index_name from user_indexes where table_name='%s'",func.table);
	else//DBTYPE_SYBASE
		sprintf(func.isql,"select name from sysindexes where id=Object_id('%s')",func.table);


	retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void **)&pt);
	
	strncpy(indexname,pt,30);
	if (strcmp(indexname,tablename)==0)
		fprintf(stderr,"������!");
	else
	{
		for (int tt=0;tt<func.ret_roxnum;tt++)
		{
			memset(indexname,0,sizeof(char)*30);
			strncpy(indexname,pt+tt*30,30);
			fprintf(stderr,"\t%s\n",indexname);
		}
	}

	dbsingle.Close();
	return retcode;
}
int CDbOperate::do_scada()
{
	FILE *stream;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	int serverno;
	int pluseflag,daystatflag,devflag;
	int year,month,Sec;
	CS_RETCODE retcode;
	struct tm *Time;
	char filename[24];

	retcode=get_filename(filename);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n		δ�ṩ�ļ������ļ���·������ȷ!");
		fprintf(stderr,"\n		���� dbmt scada help ������");
		return retcode;
	}
	if (strstr(filename,"YC")==NULL)
		pluseflag=1;
	else	pluseflag=0;
	if (strstr(filename,"DAY")==NULL)
		daystatflag=0;
	else daystatflag=1;
	if (strstr(filename,"DEV")==NULL)
		devflag=0;
	else devflag=1;
	//ȡ���ļ��Ĵ�С
	long filesize;
	#if defined(WIN32)
	int file_handle;
	if ((file_handle=open(filename,1)) ==-1)
	{
		printf("failed");
	}
	else
	{
		filesize = filelength(file_handle);
		close(file_handle);
	}
	#elif defined(__unix)
	filesize = filelength(filename);
	#endif

	stream = fopen(filename,"rb");
	if (stream==NULL)
	{
		fprintf(stderr,"\n		�ļ�%s��ʧ��!",filename);
		return 0;
	}
	serverno=get_serverno();
	retcode=db_start(serverno);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n		���ݿ��ʧ��!");
		return retcode;
	}
	int i=0;
	//////////////////////////////////
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}
	HIS_DATA *hisdata;
	hisdata = (HIS_DATA *) malloc(filesize);
	memset(hisdata,0,filesize);
	int retvalue=fread(hisdata,sizeof(HIS_DATA),filesize/sizeof(HIS_DATA),stream);
	
	//add 2014 11 begin
	func.his.date= hisdata->date;
	func.his.time= hisdata->time;
	//add 2015 11 end
    Sec=(hisdata->date)*24*60*60;
	time_t stime;
	stime=Sec;
	Time=localtime(&stime);
	year=Time->tm_year+1900;//��
	month=Time->tm_mon+1;//��
	if(devflag) sprintf(func.table,"�豸����ͳ�Ʊ�");
	else if (daystatflag) 
		{	
			if (pluseflag) 	sprintf(func.table,"daykwh%04d%02d",year,month);
			else 	sprintf(func.table,"dayyc%04d%02d",year,month);
		}
		else 
		{
			if (pluseflag) 	sprintf(func.table,"kwh%04d%02d",year,month);
			else 	sprintf(func.table,"yc%04d%02d",year,month);
		}

	func.func=ISQL_COMMAND;
	char pSqlCmd[100];
	sprintf(pSqlCmd,"use hisdb\r\ngo\r\ndelete from %s where sdate=%d and time=%d \r\ngo\r\n",func.table,func.his.date,func.his.time);
	if(bPrompt==1)
	{
		retcode = dbsingle.IsqlCommand(&func,pSqlCmd);
		if (retcode==CS_SUCCEED)
		{
			fprintf(stderr,"\n			ɾ���ɹ�!");
		}
		else
			fprintf(stderr,"\n			ɾ��ʧ��!");
	}
	//////////////////////����ʱ�������
	for(i=1;i<(int)(filesize/sizeof(HIS_DATA));i++)
	{
		(hisdata+i)->date=hisdata->date;
		(hisdata+i)->time=hisdata->time;
	}

	func.func = INS_MIN_HISDATA;
	func.roxnum = filesize/sizeof(HIS_DATA);
	func.his.date = hisdata->date;
	func.his.time = hisdata->time;
	func.his.pulseflag=pluseflag;
	strcpy(func.dbname,"hisdb");
	retcode = dbsingle.InsertHisdata(&func,hisdata);
	if (retcode==CS_SUCCEED)
	{
		fprintf(stderr,"\n			�ļ�����ɹ�!");
	}
	else
		fprintf(stderr,"\n			�ļ�����ʧ��!");

	if (hisdata) free(hisdata);
	fclose(stream);
	dbsingle.Close();
	return retcode;
}
int CDbOperate::get_filename(char *filename)
{
	for (int i=1;i<argc;i++)
	{
		if (strncmp(argv[i],"-F",2)==0)
		{
			if (argv[i][2]!=0)
			{
				strcpy(filename,argv[i]+2);
				return 1;
			}
			fprintf(stderr,"\n-F δ�ṩ�ļ���!");
			return 0;
		}
	}
	return 0;
}
int CDbOperate::do_dump()
{
	CS_RETCODE retcode;
	int serverno=0;
	char dbname[100];
	char pSqlCmd[100];
	serverno=get_serverno();
	if (serverno<1||serverno>4)
	{
		fprintf(stderr,"\n		δָ��������!�������������!");
		fprintf(stderr,"\n		���� dbmt dump help ������");
		return 0;
	}
	if (get_dbname(dbname)==0)
	{
		fprintf(stderr,"\n		δָ�����ݿ�!�����ݿ����!");
		fprintf(stderr,"\n		���� dbmt dump help ������");
		return 0;
	}

	retcode=db_start(serverno);
	if (retcode!=CS_SUCCEED) return retcode;

	/* ���ϵͳ����Ա���� */	
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return retcode;
	}
	FUNC_STR func;
	func.func=ISQL_COMMAND;
	strcpy(func.dbname,"modeldb");
	if (dbtype==DBTYPE_SYBASE)
	{
		sprintf(pSqlCmd,"use master \r\ngo\r\n dump transaction %s with truncate_only\r\ngo\r\n",dbname);
	}
	else//DBTYPE_ORACLE
	{
		fprintf(stderr,"\n		����־�������");
		return retcode;
	}
	
	retcode=dbsingle.IsqlCommand(&func,pSqlCmd);
	if (retcode==CS_SUCCEED) fprintf(stderr,"\n		%s����־����ɹ�",dbname);
	dbsingle.Close();
	return retcode;
}
int CDbOperate::do_check()
{
	CS_RETCODE retcode;
	int serverno=0;
	int inchar,i;
	serverno=get_serverno();
	if (serverno<1||serverno>4)
	{
		fprintf(stderr,"\n		δָ��������!�������������!");
		fprintf(stderr,"\n		���� dbmt check help ������");
		return 0;
	}
	retcode=db_start(serverno);
	if (retcode!=CS_SUCCEED) return retcode;
	for(i=0;i<CHECKTABLENUM;i++)
		fprintf(stderr,"    %d��%s\n",i+1,checktable[i]);
	fprintf(stderr,"    0���˳�\n");
	do
	{
		fprintf(stderr,"\n��ѡ��Ҫ���Ĳ�����[");
		for(i=1;i<=CHECKTABLENUM;i++) fprintf(stderr,"%d,",i);
		fprintf(stderr,"0] : ");
		gets(strstdin);
		inchar=strstdin[0];
	}while(inchar<'0' || inchar>('0'+CHECKTABLENUM) );
		
	if(inchar=='0') return 1;
	int dataflag=inchar-'0';
	
	switch(dataflag)
	{
	case 1:
		check_table("SCD�豸������",1);
		check_table("ң�������",2);
		check_table("ң�Ų�����",3);
		check_table("��Ȳ�����",4);
		break;
	case 2:
		check_table("SCD�豸������",dataflag-1);
		break;
	case 3:
		check_table("ң�������",dataflag-1);
		break;
	case 4:
		check_table("ң�Ų�����",dataflag-1);
		break;
	case 5:
		check_table("��Ȳ�����",dataflag-1);
		break;
	default:
		break;
	}
	dbsingle.Close();
	return retcode;
}
int CDbOperate::check_table(CS_CHAR *tablename,int dataflag)
{
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_DATAFMT *datafmt;
	int j1,j2,j3,j4,ptcnt;
	int checknum,i;

	/////////////////////////////////////////////////////////////
	func.func = GET_TABLE_INFO;
	strcpy(func.table,tablename);
	strcpy(func.dbname,"modeldb");
	if(dbsingle.GetTableInfo(&func,&datafmt)==CS_FAIL)	
		return FALSE;

	for (i=0;i<func.colnum;i++)
	{
		func.col[i].condition = NO;
		func.col[i].select = NO;
		func.col[i].order  = -1;
		if(strcmp(datafmt[i].name,"����")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 0;
			j1=i;
		}
		if(strcmp(datafmt[i].name,"����")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 1;
			j2=i;
		}
		if(strcmp(datafmt[i].name,"����")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 2;
			j3=i;
		}
		if (dataflag!=1){
		if(strcmp(datafmt[i].name,"�豸����")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 3;
			j4=i;
		}
		}
	}
	void *pt;
	func.func=SEL_RECORDERS;
	if ((dbsingle.SelectRcds(&func,datafmt,&pt))!=CS_SUCCEED)
		return FALSE;
	if (func.ret_roxnum<=0) return FALSE;
	checknum=func.ret_roxnum;
	CHECK_VALUE *check_value;
	check_value = (CHECK_VALUE *) malloc (sizeof(CHECK_VALUE)*func.ret_roxnum);
	memset(check_value,0,sizeof(CHECK_VALUE)*func.ret_roxnum);
	for(i=0;i<func.ret_roxnum;i++)
	{
		dbsingle.GetData(i,&func,datafmt+j1,pt,ptcnt,check_value[i].name);
		dbsingle.GetData(i,&func,datafmt+j2,pt,ptcnt,check_value[i].desc);
		check_value[i].type=(short)dbsingle.GetData(i,&func,datafmt+j3,pt,ptcnt,NULL);
		if (dataflag!=1)
			dbsingle.GetData(i,&func,datafmt+j4,pt,ptcnt,check_value[i].devname);
	}
	if (pt) 
	{
		free (pt);
		pt=NULL;
	}
	if (datafmt) 
	{
		free (datafmt);
		datafmt=NULL;
	}
	/////////////////////////////////////////////////////////////////////////
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"վ����ϵ������");
	strcpy(func.dbname,"modeldb");
	if(dbsingle.GetTableInfo(&func,&datafmt)==CS_FAIL)	
		return FALSE;

	for (i=0;i<func.colnum;i++)
	{
		func.col[i].condition = NO;
		func.col[i].select = NO;
		func.col[i].order  = -1;
		if(strcmp(datafmt[i].name,"����")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 0;
			j1=i;
		}
	}
	func.func=SEL_RECORDERS;
	if ((dbsingle.SelectRcds(&func,datafmt,&pt))!=CS_SUCCEED)
		return FALSE;
	if (func.ret_roxnum<=0) return FALSE;
	stationnum=func.ret_roxnum;
	for(i=0;i<func.ret_roxnum;i++)
	{
		dbsingle.GetData(i,&func,datafmt+j1,pt,ptcnt,stationname[i]);
	}
	if (pt) 
	{
		free (pt);
		pt=NULL;
	}
	if (datafmt) 
	{
		free (datafmt);
		datafmt=NULL;
	}
	////////////////////////////////////////////////////////////////////////
	if (dataflag!=1){
		func.func = GET_TABLE_INFO;
		strcpy(func.table,"SCD�������ͱ�");
		strcpy(func.dbname,"modeldb");
		if(dbsingle.GetTableInfo(&func,&datafmt)==CS_FAIL)	
			return FALSE;

		for (i=0;i<func.colnum;i++)
		{
			func.col[i].condition = NO;
			func.col[i].select = NO;
			func.col[i].order  = -1;
			if(strcmp(datafmt[i].name,"����ֵ")==0)
			{
				func.col[i].select = YES;
				func.col[i].order_d = 0;
				j1=i;
			}
			if(strcmp(datafmt[i].name,"���ʹ���")==0)
			{
				func.col[i].select = YES;
				func.col[i].order_d = 1;
				j2=i;
			}
		}
		func.func=SEL_RECORDERS;
		if ((dbsingle.SelectRcds(&func,datafmt,&pt))!=CS_SUCCEED)
			return FALSE;
		if (func.ret_roxnum<=0) return FALSE;
		scdtypenum=func.ret_roxnum;
		scdtypevalue = (SCD_TYPEVALUE *) malloc (sizeof(SCD_TYPEVALUE)*scdtypenum);
		memset(scdtypevalue,0,sizeof(SCD_TYPEVALUE)*scdtypenum);
		for(i=0;i<func.ret_roxnum;i++)
		{
			scdtypevalue[i].typevalue=(short)dbsingle.GetData(i,&func,datafmt+j1,pt,ptcnt,NULL);
			dbsingle.GetData(i,&func,datafmt+j2,pt,ptcnt,scdtypevalue[i].typedaima);
			if (strlen(scdtypevalue[i].typedaima)==1)
				scdtypevalue[i].typedaima[1]=scdtypevalue[i].typedaima[0];
			if (!strncmp(scdtypevalue[i].typedaima,"Cb",2))
				strcpy(scdtypevalue[i].typedaima,"Kg");
		}
	}
	if (pt) 
	{
		free (pt);
		pt=NULL;
	}
	if (datafmt) 
	{
		free (datafmt);
		datafmt=NULL;
	}
	int badnum=0;

	CheckNameLogic(check_value,checknum,dataflag);
	for(i=0;i<checknum;i++)	{
	if (check_value[i].logicalflag) 	badnum++;
	}
	
	fprintf(stderr,"\n----------------------------------------------------");
	fprintf(stderr,"\n%s		������빲��%d��",tablename,badnum);
	if(badnum>0)
	{
		int ii=1;
		fprintf(stderr,"\n----------------------------------------------------");
		if (dataflag==1)
				fprintf(stderr,"\n%3s %24s %40s	%3s","���","����","����","����");
		else	fprintf(stderr,"\n%3s %24s %40s	%3s %24s","���","����","����","����","�豸����");
		for(i=0;i<checknum;i++)
		{
			if (dataflag!=1){
				if (check_value[i].logicalflag) 
				fprintf(stderr,"\n%3d %24s %40s	%3d %24s",ii++,check_value[i].name,check_value[i].desc,check_value[i].type,check_value[i].devname);
			}
			else
			{
				if (check_value[i].logicalflag) 
				fprintf(stderr,"\n%3d %24s %40s	%3d",ii++,check_value[i].name,check_value[i].desc,check_value[i].type);
			}
		}
	}
	if (check_value)
	{
		free (check_value);
		check_value=NULL;
	}
	if (scdtypevalue)
	{
		free (scdtypevalue);
		scdtypevalue=NULL;
	}
	return 1;
}
void CDbOperate::CheckNameLogic(CHECK_VALUE *value,int checknum,int dataflag)
{
	char first[1],last[2];
	char *tempflag=NULL;
	int i,j,strlength;
	int cmpflag=-1;

	if (dataflag==1)
	{
		for(i=0;i<checknum;i++)
		{
			strncpy(first,value[i].name,1);
			switch(first[0])
			{
			case 'K'://����
				if (value[i].type!=1) value[i].logicalflag=1;
				break;
			case 'D'://��բ
				if (value[i].type!=2&&value[i].type!=4) value[i].logicalflag=1;
				break;
			case 'B'://��ѹ��
				if (value[i].type!=5&&value[i].type!=6) value[i].logicalflag=1;
				break;
			case 'M'://ĸ��
				if (value[i].type!=16) value[i].logicalflag=1;
				break;
			case 'X'://��·
				if (value[i].type!=14&&value[i].type!=15&&value[i].type!=19) value[i].logicalflag=1;
				break;
			case 'F'://�����
				if (value[i].type!=17) value[i].logicalflag=1;
				break;
			case 'I'://����������
				if (value[i].type!=11) value[i].logicalflag=1;
				break;
			case 'U'://��ѹ������
				if (value[i].type!=10) value[i].logicalflag=1;
				break;
			case 'C'://������
				if (value[i].type!=7) value[i].logicalflag=1;
				break;
			case 'R'://�翹��
				if (value[i].type!=8) value[i].logicalflag=1;
				break;
			case 'L'://������
				if (value[i].type!=12) value[i].logicalflag=1;
				break;
			case 'P'://����װ��
				if (value[i].type!=22) value[i].logicalflag=1;
				break;
			case 'Q'://������Ȧ
				if (value[i].type!=9) value[i].logicalflag=1;
				break;
			case 'H'://����
				if (value[i].type!=21) value[i].logicalflag=1;
				break;
			case 'O'://�����豸
				if (value[i].type!=51) value[i].logicalflag=1;
				break;
			case 'S'://��վ
				if (value[i].type!=52) value[i].logicalflag=1;
				break;
			default:
				value[i].logicalflag=1;
				break;
			}//switch
			if (!strcmp(value[i].name,"sys")) value[i].logicalflag=0;
			for (j=0;j<stationnum;j++)
			{
				tempflag=strstr(value[i].name,stationname[j]);
				if (tempflag) break;
			}
			if (!tempflag) value[i].logicalflag=1;
		}
	}
	else if (dataflag==2||dataflag==3||dataflag==4)
	{
		//�Ƚ�����������
		for(i=0;i<checknum;i++)
		{
			int existflag=0;
			for(j=0;j<scdtypenum;j++)
			{
				if (value[i].type == (int)scdtypevalue[j].typevalue)
				{
					existflag=1;
					strlength=strlen(value[i].name);
					if (strlength<=4) {
						value[i].logicalflag=1;
						break;
					}
					last[0]=value[i].name[strlength-2];
					last[1]=value[i].name[strlength-1];
					if (last[1]<='Z'&&last[1]>='A')//�����ĸΪ��д��ĸ
						last[0]=last[1];
					cmpflag=strncmp(last,scdtypevalue[j].typedaima,2);
					if (cmpflag!=0)
						value[i].logicalflag=1;
					break;
				}
			}

			if (!existflag) value[i].logicalflag=1;
			//�Ƚ���������Ƿ������վ����
			tempflag=NULL;
			for (j=0;j<stationnum;j++)
			{
				tempflag=strstr(value[i].name,stationname[j]);
				if (tempflag) break;
			}
			if (!tempflag) value[i].logicalflag=1;
			//�Ƚ�������ͷһ���ַ��Ƿ������豸��ͷһ���ַ���ͬ
			last[0]=value[i].name[0];
			last[1]=value[i].devname[0];
			if (last[0]!=last[1]&&last[0]!=last[1]-32)
				value[i].logicalflag=1;
		}
	}
}
//201508 add begin
int CDbOperate::realize_in_file_ex(FUNC_STR *func,char *filename,CHECK_VALUE *change,int changenum,int id)
{
	CS_RETCODE retcode;
	int i,numrows;
	void *p6;
	FILE *stream=NULL;
	if(id>=0) fprintf(stderr,"\n%d ����װ���¼����[%s] ...",id,func->table);
	else fprintf(stderr,"\n ����װ���¼����[%s] ...",func->table);

	if (change==NULL) return 0;

	if (func->his.flag==2)
	{
		func->func=ISQL_COMMAND;
		sprintf(func->isql,"truncate table %s",func->table);
		retcode=dbsingle.IsqlCommand(func,func->isql);
		if (retcode==CS_SUCCEED) fprintf(stderr,"\n		ԭ��¼����ɹ�");
		else {
			fprintf(stderr,"\n		ԭ��¼���ʧ��");
			//return retcode;
		}
	}	
	CS_DATAFMT *datafmt;
	func->func=GET_TABLE_INFO;
	retcode = dbsingle.GetTableInfo(func,&datafmt);
	if (retcode!=CS_SUCCEED)
	{
		if (datafmt) free (datafmt);
		fprintf (stderr,"\n %s ��ʧ��!",func->table);
		return CS_FAIL;
	}
	
	CS_DATAFMT** fmt = &datafmt;
	void**  ptt = (void**)fmt;
	stream=fopen(filename,"rt");
	if(stream==NULL)
	{
		if (datafmt) free(datafmt);
		fprintf(stderr,"\n���ļ�[%s] ����ļ�����!",filename);
		return CS_FAIL;
	}

	fseek(stream,0,SEEK_SET);//��λ���ļ�ͷ
	
	numrows=1;
	char * pstr=(char*)malloc(EX_BUFSIZE*2+10);

	fgets(pstr,EX_BUFSIZE*2,stream);//����һ��Ϊ��ṹ�����˵�һ��
	if(strncmp(pstr,"--DATAFMT: ",11)!=0)
	fseek(stream,0,SEEK_SET);//��λ���ļ�ͷ

	char *pcur;
	VAL_UNION *value;
	int tmpint,j,tt;
	int shortvalue =0;
	int recnum=0;
	value=(VAL_UNION *) malloc (sizeof(VAL_UNION)*MAXINROWNUM*func->colnum); //Ϊ�˷�ֹ�ڴ治�����ֻ��Ϊ5000*func->colnum

	while(1)
	{
		memset(value,0,sizeof(VAL_UNION)*MAXINROWNUM*func->colnum); 
		for(i=0;i<MAXINROWNUM;i++)
		{
			if (fgets(pstr,EX_BUFSIZE*2,stream)==NULL)
				break;
			else 
				pcur=pstr;

			for(j=0;j<func->colnum;j++)
			{
				if (j==5) continue;//������Ϊrawdata 
				if(j>0)
				{
					if( (pcur=strchr(pcur,'\t'))==NULL )	/* ÿ������TAB������ */
					{
						retcode=CS_FAIL;
						break;
					}
					pcur+=1;
				}
				tmpint=i*func->colnum+j;
				switch(datafmt[j].datatype)
				{
					case CS_SMALLINT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->smallvalue=0;
						else
							if(sscanf(pcur,"%d",&shortvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else (value+tmpint)->smallvalue=shortvalue;
						break;
					case CS_TINYINT_TYPE:
					case CS_BIT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->tinyvalue=0;
						else	
							if(sscanf(pcur,"%d",&shortvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else 
								(value+tmpint)->tinyvalue=(unsigned char)shortvalue;
						break;
					case CS_USHORT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->usmallvalue=0;
						else
							if(sscanf(pcur,"%d",&value)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else (value+tmpint)->usmallvalue=shortvalue;
						break;
					case CS_INT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->intvalue=0;
						else
							if(sscanf(pcur,"%d",&((value+tmpint)->intvalue))<=0)
							{
								retcode=CS_FAIL;
								break;
							}
						break;
					case CS_CHAR_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') strcpy((value+tmpint)->charvalue," ");
						else 
							if(sscanf(pcur,"%[^\t\r\n]s",(value+tmpint)->charvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
						for (tt=0;tt<changenum;tt++)
						{
							if (strcmp((value+tmpint)->charvalue,(change+tt)->name)==0)
							{
								strcpy((value+tmpint)->charvalue,(change+tt)->devname);
								break;
							}
						}
						break;
					case CS_REAL_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->realvalue=0;
						else
							if(sscanf(pcur,"%g",&((value+tmpint)->realvalue))<=0)
							{
								retcode=CS_FAIL;
								break;
							}
						break;
					case CS_FLOAT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->floatvalue=0;
						else
						if(sscanf(pcur,"%lg",&((value+tmpint)->floatvalue))<=0)
						{
							retcode=CS_FAIL;
							break;
						}
						break;
					case CS_DATETIME_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') 
						{
							retcode=CS_FAIL;
							break;
						}
						if(sscanf(pcur,"%[^\t\r\n]s",(value+tmpint)->charvalue)<=0)
						{
							retcode=CS_FAIL;
							break;
						}
						break;
					default:
						break;
				}//switch
			}//for (j)
		}//for (i)
		p6=value;
		fprintf(stderr,"-");
		if(i==0) break;
		retcode=dbsingle.DbTools(DB_IN_FILE,(void*)func->dbname,(void*)func->table,(void*)(&func->colnum),ptt,&i,&p6);
		if(retcode==CS_SUCCEED)			recnum+=i;

		if(i<MAXINROWNUM) break;
	}
//	}//while

	//for(j=0;j<null_num;j++) putchar(' ');
	putchar('\t');
	if(retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"	ʧ��!  ");
		fprintf(stderr,"�������ݴ���!",func->table);
		if (pstr) free(pstr);
		if (datafmt) free(datafmt);
		if (p6) free(p6);
		fclose(stream);
		return 0;
	}
	else 
	{
		fprintf(stderr,"	�ɹ�!  ");
		fprintf(stderr,"����%d������!",recnum);
		if (pstr) free(pstr);
		if (datafmt) free(datafmt);
		if (p6) free(p6);
		fclose(stream);
		return 1;
	}

}

int CDbOperate::do_dfhis()
{
	int retcode=0;
	int serverno;
	FUNC_STR func;
	CS_DATAFMT *datafmt;
	int i,j1,j2,ptcnt;
	int changeycnum,changekwhnum;	
	int dataYM,datayear,datamon;
	CHECK_VALUE *change_yc,*change_kwh;
	void *pt;

	serverno=get_serverno();
	if (serverno<1||serverno>4)
	{
		fprintf(stderr,"\n		δָ��������!�������������!");
		fprintf(stderr,"\n		���� dbmt check help ������");
		return 0;
	}
	retcode=db_start(serverno);
	if (retcode!=CS_SUCCEED) return retcode;
	
	for(i=1;i<argc;i++)
	{
		if(strncmp(argv[i],"-YM",3)==0)
				dataYM=atoi(argv[i]+3);
	}

	if(dataYM<=0)
	{
		fprintf(stderr,"\n�����ʽ���ԣ�δ�ṩ���ڡ����� dbmt in help ������\n");
		return 0;
	}
	datayear=dataYM/100;
	datamon=dataYM%100;
	if(datayear<1995 || datayear>2999 || datamon<1 || datamon>12)
	{
		fprintf(stderr,"\n�ṩ�����ڲ��ԡ����� dbmt in help ������\n");
		return 0;
	}


	/////////////////////////////////////////////////////////////
	memset(&func,0,sizeof(FUNC_STR));
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"ң���ϵ��Ӧ��");
	strcpy(func.dbname,"modeldb");
	if(dbsingle.GetTableInfo(&func,&datafmt)==CS_FAIL)	
		return FALSE;

	for (i=0;i<func.colnum;i++)
	{
		func.col[i].condition = NO;
		func.col[i].select = NO;
		func.col[i].order  = -1;
		if(strcmp(datafmt[i].name,"df")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 0;
			j1=i;
		}
		if(strcmp(datafmt[i].name,"sac")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 1;
			j2=i;
		}
	}
	func.func=SEL_RECORDERS;
	if ((dbsingle.SelectRcds(&func,datafmt,&pt))!=CS_SUCCEED)
		return FALSE;
	if (func.ret_roxnum<=0) return FALSE;

	changeycnum=func.ret_roxnum;
	change_yc = (CHECK_VALUE *) malloc (sizeof(CHECK_VALUE)*func.ret_roxnum);
	memset(change_yc,0,sizeof(CHECK_VALUE)*func.ret_roxnum);
	
	for(i=0;i<func.ret_roxnum;i++)
	{
		dbsingle.GetData(i,&func,datafmt+j1,pt,ptcnt,change_yc[i].name);
		dbsingle.GetData(i,&func,datafmt+j2,pt,ptcnt,change_yc[i].devname);
	}


	memset(&func,0,sizeof(FUNC_STR));
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"��ȹ�ϵ��Ӧ��");
	strcpy(func.dbname,"modeldb");
	if(dbsingle.GetTableInfo(&func,&datafmt)==CS_FAIL)	
		return FALSE;

	for (i=0;i<func.colnum;i++)
	{
		func.col[i].condition = NO;
		func.col[i].select = NO;
		func.col[i].order  = -1;
		if(strcmp(datafmt[i].name,"df")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 0;
			j1=i;
		}
		if(strcmp(datafmt[i].name,"sac")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 1;
			j2=i;
		}
	}
	func.func=SEL_RECORDERS;
	if ((dbsingle.SelectRcds(&func,datafmt,&pt))!=CS_SUCCEED)
		return FALSE;
	if (func.ret_roxnum<0) return FALSE;

	changekwhnum=func.ret_roxnum;
	change_kwh = (CHECK_VALUE *) malloc (sizeof(CHECK_VALUE)*func.ret_roxnum);
	memset(change_yc,0,sizeof(CHECK_VALUE)*func.ret_roxnum);
	
	for(i=0;i<func.ret_roxnum;i++)
	{
		dbsingle.GetData(i,&func,datafmt+j1,pt,ptcnt,change_kwh[i].name);
		dbsingle.GetData(i,&func,datafmt+j2,pt,ptcnt,change_kwh[i].devname);
	}


	if (pt) 
	{
		free (pt);
		pt=NULL;
	}
	if (datafmt) 
	{
		free (datafmt);
		datafmt=NULL;
	}
	dbsingle.Close();

	//�򿪶������ӵ���ʷ���ݿ�
	retcode=db_start(serverno,DF_HIS);
	if (retcode!=CS_SUCCEED) 
	{
		fprintf(stderr,"\n�������ӵ���ʷ���ݿ��ʧ�ܡ�\n");
		return retcode;
	}
	char filename[255];
	int mon_num;
	{
		memset(&func,0,sizeof(FUNC_STR));
		sprintf(func.dbname,"xopenshdb");

		// data199901 -> file data199901
		sprintf(func.table,"data%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"data%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// pdata199901 -> file pdata199901
		sprintf(func.table,"pdata%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"pdata%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// daydata199901 -> daydata199901
		sprintf(func.table,"daydata%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"daydata%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// daypdata199901 ->daypdata199901
		sprintf(func.table,"daypdata%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"daypdata%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// mondata1999 -> file mondata199901,mondata199902...
		sprintf(func.table,"mondata%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"mondata%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// monpdata1999 -> file monpdata199901,monpdata199902...
		sprintf(func.table,"monpdata%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"monpdata%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		dbsingle.Close();
	}	

	//��ʼ���ݵ���
	retcode=db_start(serverno);
	if (retcode!=CS_SUCCEED) return retcode;

	{
		memset(&func,0,sizeof(FUNC_STR));
		sprintf(func.dbname,"hisdb");

		//  file data199901 -> yc199901
		sprintf(func.table,"yc%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"data%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_yc,changeycnum,-1);
		remove(filename);

		//  file pdata199901 -> kwh199901
		sprintf(func.table,"kwh%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"pdata%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_kwh,changekwhnum,-1);
		remove(filename);

		//  file daydata199901 -> dayyc199901
		sprintf(func.table,"dayyc%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"daydata%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_yc,changeycnum,-1);
		remove(filename);

		//  file daypdata199901 -> daykwh199901
		sprintf(func.table,"daypdata%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"daypdata%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_kwh,changekwhnum,-1);
		remove(filename);

		// file mondata199901 -> monthyc1999
		sprintf(func.table,"monthyc%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"mondata%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_yc,changeycnum,-1);
		remove(filename);

		// file monpdata199901 -> monthkwh1999
		sprintf(func.table,"monthkwh%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"monpdata%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_kwh,changekwhnum,-1);
		remove(filename);

		dbsingle.Close();
	}

	if (change_yc)
	{
		free(change_yc);
		change_yc = NULL;
	}
	
	if (change_kwh)
	{
		free(change_kwh);
		change_kwh = NULL;
	}
	return retcode;

}
//201508 add ends

int
main(int argc,char *argv[])
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				printf("dbmt, 1.1");
				return 0;
			}
		}
	}
	NetDbg dbg;
	dbg.RegProc("���ݿ⹤��");

//#if defined(__Edition2001)
	if(dbsingle.isDBType(DBTYPE_SYBASE))  //Sybase���ݿ�
	{
		dbtype = DBTYPE_SYBASE;
	}
	else if(dbsingle.isDBType(DBTYPE_ORACLE))  //oracle���ݿ�
	{
		dbtype = DBTYPE_ORACLE;
	}
	else if(dbsingle.isDBType(DBTYPE_DMSQL))  //�������ݿ�
	{
		dbtype = DBTYPE_DMSQL;
	}
	else if(dbsingle.isDBType(DBTYPE_SQLSERVER)) //   SQLSERVER
	{
		dbtype = DBTYPE_SQLSERVER;
	}
	else if(dbsingle.isDBType(DBTYPE_MYSQL)) //   SQLSERVER
	{
		dbtype = DBTYPE_MYSQL;
	}
//#endif

	int i;
	CDbOperate dbOperate;

	dbOperate.argc=argc;
	if(dbOperate.argc>MAXPARA) dbOperate.argc=MAXPARA;
	for (i=0;i<dbOperate.argc;i++) strncpy(dbOperate.argv[i],argv[i],MAXPARALEN);


	dbOperate.DoOperate();

	dbg.UnRegProc();

	printf("\n");
	return 1;

}
