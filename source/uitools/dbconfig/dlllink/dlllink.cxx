#ifdef WIN32
	#include "stdafx.h"
	#include "dlllink.h"
#endif

#include "com_daqo_dbinfo_Dlllink.h"
#include "db/dbapi.h"
#include "rdb/rdbapi.h"//���Ը�����Ҫ���Ժ����ȥ��
#include "net/netapi.h"
#include "fes/femailstruct.h"



#ifdef WIN32
	#ifdef _DEBUG
		#define new DEBUG_NEW
	#undef THIS_FILE
		static char THIS_FILE[] = __FILE__;
	#endif

	BEGIN_MESSAGE_MAP(CDlllinkApp, CWinApp)
		//{{AFX_MSG_MAP(CDlllinkApp)
			// NOTE - the ClassWizard will add and remove mapping macros here.
			//    DO NOT EDIT what you see in these blocks of generated code!
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

	CDlllinkApp theApp;

#endif
/////////////////////////////////////////////////////////////////////////////
// CDlllinkApp construction

#define DBNAMESIZE		64//���ݿ�������
#define FIELDNAMESIZE	64//�����ĳ���

//�Ƿ����emsӦ�ò������֣�����ѡ��Ϊ���ֺ���ǰ�汾���ݶ����
//unix������makefile����Ҫ��2��һ����ͨ�汾��һ���Ǽ�ems�����İ汾��
//��ͬ�ĵط���lib����Ҫ���ems�õ����ӿ⣬ͬʱ�����ļ�Ҳ��Ҫ���ems�ð����ļ�
//������ϵͳϰ��Ӧ�÷���d6000��include�С����ǵ����ݣ�ֱ�ӷ��ڱ�����Ŀ¼�µ�include_ems���ˡ�
//#define INCEMS          1 
#ifdef  INCEMS
	char *emsrdbtbls[]={"tblname1","tblname2","tblname3"};
	//���ú���ǰ��Ҫgettableinfo����
	bool IsUpdEmsRdb(){
		CS_CHAR tblName[128];
		strcpy(tblName,func.table);
		for(int i=0;i<sizeof(emsrdbtbls);i++)
	}
#endif


DBApi db;

FUNC_STR func;
CS_DATAFMT *datafmt = NULL;
void* pt = NULL;
int colcount = 0;//�е���Ŀ

char *errdbmsg = NULL;//���ݿ������Ϣ
int errdbmsglen;//��Ϣ����
NetDbg	netdbg;
typedef struct _tagfdname
{
	char fdname[FIELDNAMESIZE];
}FDNAME;


#ifdef WIN32
	CDlllinkApp::CDlllinkApp()
	{
		// TODO: add construction code here,
		// Place all significant initialization in InitInstance
	}
#endif

//�����ó��� begin
void RdbDebugMesg(char *fmt,...)
{
	char *daqohomepath;
	Rdb_Time  rdbtime;
	RDB_DATETIME_MS_STRU  curclock;
	char  filename[128];
	char  debugmsg[1024];
	va_list  args;



	if((daqohomepath=getenv("NBENV"))==NULL)return;
#ifdef RDB_PLATFORM_UNIX
	strcpy(filename,daqohomepath);
	strcat(filename,"/log/dbconfig.txt");
#else
	strcpy(filename,daqohomepath);
	strcat(filename,"\\log\\dbconfig.txt");
#endif
	int len=RdbGetFileLen(filename);
#ifdef RDB_PLATFORM_SUNUNIX
	int debugfd;
	if(len>=(600 * 1000)) {
		unlink(filename);
		debugfd=open(filename,O_CREAT | O_APPEND | O_RDWR, 0666);
	}
	else if(len<0) {
		debugfd=open(filename,O_CREAT | O_APPEND | O_RDWR, 0666);
	}
	else {
		debugfd=open(filename,O_APPEND | O_RDWR);
	}
	if(debugfd<0)return;

 	rdbtime.RdbGetCurClock(&curclock);
 
 	sprintf(debugmsg,"%.4d-%.2d-%.2d  %.2d:%.2d:%.2d.%.3d  ",curclock.year,curclock.month,
      curclock.day,curclock.hour,curclock.minute,curclock.second,curclock.msecond);
	write(debugfd,debugmsg,strlen(debugmsg));

	va_start(args,fmt);
	vsprintf(debugmsg,fmt,args);
	va_end(args);
	write(debugfd,debugmsg,strlen(debugmsg));
	close(debugfd);
#else
	FILE *fp;
	if(len>=(200 * 1000)) {
		fp=fopen(filename,"w");
	}
	else {
		fp=fopen(filename,"a");
	}
	if(fp==NULL)return;

	rdbtime.RdbGetCurClock(&curclock);

 	fprintf(fp,"%.4d-%.2d-%.2d  %.2d:%.2d:%.2d.%.3d  ",curclock.year,curclock.month,
      curclock.day,curclock.hour,curclock.minute,curclock.second,curclock.msecond);

	va_start(args,fmt);
	vsprintf(debugmsg,fmt,args);
	va_end(args);
	fprintf(fp,"%s",debugmsg);
	fclose(fp);
#endif
	
}

void FromDataToStr (CS_DATAFMT *datafmt, char *cmdbuf1, void *datapt)
{
    CS_DATAFMT      destfmt; 
    CS_CHAR         dtstr[600]; 
    int i; 
    
    memset (&destfmt, 0, sizeof (destfmt)); 
    switch (datafmt->datatype) {
		case CS_TINYINT_TYPE:
		case CS_BIT_TYPE:
			sprintf (cmdbuf1, "%d", *((unsigned char *)datapt)); 
			break; 
		case CS_BINARY_TYPE:
			strcpy (cmdbuf1, "0x"); 
			for (i=0; i<datafmt->maxlength; i++) {
				sprintf (dtstr, "%02x", *((unsigned char *)datapt+i)); 
				strcat (cmdbuf1, dtstr); 
			}
			break; 
		case CS_SMALLINT_TYPE:
			sprintf (cmdbuf1, "%d", *((short int *)datapt)); 
			break; 
		case CS_INT_TYPE:
			sprintf (cmdbuf1, "%d", *((CS_INT *)datapt)); 
			break; 
		case CS_REAL_TYPE:
			sprintf (cmdbuf1, "%f", *((float *)datapt)); 
			break; 
		case CS_FLOAT_TYPE:
			sprintf (cmdbuf1, "%f", *((double *)datapt)); 
			break; 
		case CS_CHAR_TYPE:
			strcpy (cmdbuf1, "\""); 
			i=int (MIN ((int) (strlen ((char *)datapt)), datafmt->maxlength)); 
			memcpy (& (cmdbuf1[1]), datapt, i); 
			cmdbuf1[i+1]='\0'; 
			strcat (cmdbuf1, "\""); 
			break; 
		case CS_TEXT_TYPE:
		case CS_IMAGE_TYPE:
			sprintf (cmdbuf1, "0x44"); 
			break; 
		default:
			strcpy (cmdbuf1, ""); 
			break; 
    }
}
// ���Գ��� end

/////////////////////////////////////////////////////////////////////////////
// The one and only CDlllinkApp object

/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_startdb()
 * �����������������ݿ�
 * ����    ��
 *		   ��
 *
 * ����ֵ  ���ɹ�=1��ʧ��=0
 *
 *
 * --------------------------------------------------
 */

JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_startdb
  (JNIEnv *jEnv, jobject jthis)
{
	netdbg.RegProc("���ݿ������");
	//��emsʵʱ��
	
	return db.Open(DB_OP_WR,FALSE);
	
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_closedb()
 * �����������ر����ݿ�
 * ����    ��
 *		   ��
 *
 * ����ֵ  ��
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_closedb
  (JNIEnv *jEnv, jobject jthis)
{
	netdbg.UnRegProc();
	if(datafmt) 
	{
		free(datafmt);
		datafmt = 0;
	}
	if(pt) 
	{
		free(pt);
		pt = 0;
	}
	return db.Close();
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��endsWith()
 * ��������������ַ���str�Ƿ����ַ���postfix��β
 * ����    ��
 *           str=Ŀ���ַ���
 *           postfix=�����ַ���
 * ����ֵ  �� 
 *           1=�� 
 *           0=�� 
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
char endsWith(char *str,char *postfix)
{
	int i;
	int lenpost=strlen(postfix),lenstr=strlen(str);
    for(i=0;i<lenpost;i++){
		if(str[lenstr-1-i]!=postfix[lenpost-1-i])
			break;
	}
	return i==lenpost ? 1:0;
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��parsefieldstring()
 * ��������������ѡ�����ַ���
 * ����    ��
 *           srcstr=Ŀ���ַ���
 *           desp�����������ָ��
 * ����ֵ  �� 
 *           ��ĸ���
 *            
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
int parsefieldstring(char *srcstr,void **desp)
{

	int num = 10,count=0;//num,�ݽ�����;  count,�����
	FDNAME *p = (FDNAME *)malloc(num*sizeof(FDNAME));
	memset(p,0,sizeof(FDNAME)*num);
	int len = strlen(srcstr);
	int j = 0;
	int i;
	for(i =0;i<len;i++)
	{
		if(srcstr[i] == ',')
		{
			if(i>j)
			{
				if(count>0 && num/count >=1)
				{
					FDNAME *temp = p;
					p = (FDNAME*)malloc((num+count)*sizeof(FDNAME));
					memset(p,0,sizeof(FDNAME)*(num+count));
					memcpy(p,temp,num*sizeof(FDNAME));
					free(temp);
					num += count;
				}
				memcpy((char*)(p[count].fdname),(char*)&srcstr[j],i-j);
				count++;
			}
			j = i+1;
		}
	}
	if(i>j)//���һ������",",���п������һ����","
	{
		memcpy((char*)(p[count].fdname),(char*)&srcstr[j],i-j);
		j = i+1;
		count++;
	}
	*desp = (void*)p;
	return count;
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_getenvvar
 * �������������ϵͳ���л�������
 * ����    ��
 *		   ��
 *
 * ����ֵ  ������������ֵ
 *
 * 
 * --------------------------------------------------
 */
JNIEXPORT jbyteArray JNICALL Java_com_daqo_dbinfo_Dlllink_getenvvar
  (JNIEnv * jEnv, jobject jthis)
{

	char envvar[512];
	memset(envvar,0,512);
	char *name1 = getenv("NBENV");
	if(name1==NULL)
	{
		#if defined(WIN32)
			strcpy(envvar,"c:\\");
		#else
			name1 = getenv("HOME");
			strcpy(envvar,name1);
		#endif
	}
	else
		strcpy(envvar,name1);

	char post[4];
	memset((char*)post,0,4);
	strcpy(post,"\\");
	char rtn = endsWith((char*)envvar,(char*)post);
	if(rtn)
		envvar[strlen(envvar)-1]=0;


	int num = strlen((char*)envvar);
	jbyteArray rtnarray = jEnv->NewByteArray(num);
	
	jEnv->SetByteArrayRegion(rtnarray,0,num,(jbyte*)envvar); 

	return rtnarray;

}

/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_getTableInfo
 * �����������õ�����Ϣ
 * ����    ��tablename,����;tableinfo,���صı���Ϣ,��������������;rownum,��¼����maxlength,���ݵ���󳤶�(byte)
 *		     typeType,�������"U" or "V"
 *
 * ����ֵ  �������Ŀ��<=0ʱ������
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_getTableInfo
  (JNIEnv *jEnv, jobject jthis, jbyteArray dbname, jbyteArray tablename, jobjectArray tableinfo, jintArray maxlength, jbyteArray tableType)
{
    char prtbuf[64];
	memset(prtbuf,0,64);
	jint fieldnums = 0;
	char tn[128],tmpbuf[128],curdb[128];
	memset(tn,0,128);
	memset(curdb,0,128);

	int arrlen = jEnv->GetArrayLength(tablename); //����
	jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tn);


	if(datafmt) 
	{
		free(datafmt);
		datafmt=NULL;
	}
	memset(&func,0,sizeof(FUNC_STR));
	func.func = GET_TABLE_INFO;
	strcpy(func.table,tn);

	arrlen = jEnv->GetArrayLength(dbname); //��������
	jEnv->GetByteArrayRegion(dbname,0,arrlen,(jbyte*)curdb);
	strcpy(func.dbname,curdb);
	


	char tabletype;
	if(db.GetTableInfo(&func,&datafmt,&tabletype)!=CS_FAIL)
	{

		fieldnums = func.colnum;
		int collen = -9999;
		jbyteArray field;
		for(int i=0; i<fieldnums; i++)
		{

			if((datafmt+i)->maxlength > collen)
				collen = (datafmt+i)->maxlength;
			memset(tmpbuf,0,128);
			sprintf(tmpbuf,"%s",(datafmt+i)->name);//����
			field = (jbyteArray)jEnv->GetObjectArrayElement(tableinfo,i*3);
			jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
			memset(tmpbuf,0,128);
			sprintf(tmpbuf,"%d",(datafmt+i)->datatype);//��������
			field = (jbyteArray)jEnv->GetObjectArrayElement(tableinfo,i*3+1);
			jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);

			memset(tmpbuf,0,128);
			if(datafmt[i].status & CS_KEY)//�Ƿ�Ϊ����
			{
				sprintf(tmpbuf,"%d",1);
			}
			else
			{
				sprintf(tmpbuf,"%d",0);
			}
			field = (jbyteArray)jEnv->GetObjectArrayElement(tableinfo,i*3+2);
			jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);				

		}
		jEnv->SetIntArrayRegion(maxlength,0,1,(jint*)&collen);
		jEnv->SetByteArrayRegion(tableType,0,1,(jbyte*)&tabletype);
	}
	else
	{
		fieldnums = -1;
	}

	if(datafmt) {free(datafmt);datafmt=NULL;}

	return fieldnums;
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_openTableInfo
 * �����������õ�����Ϣ
 * ����    ��tablename,����
 *		     
 *
 * ����ֵ  �������Ŀ��<=0ʱ������
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_openTableInfo
  (JNIEnv *jEnv, jobject jthis, jbyteArray dbname, jbyteArray tablename)
{
	jint fieldnums = 0;
	char tn[128],curdb[128];
	memset(tn,0,128);
	memset(curdb,0,128);

	int arrlen = jEnv->GetArrayLength(tablename); //����
	jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tn);

	if(datafmt) {free(datafmt);datafmt=NULL;}
	memset(&func,0,sizeof(FUNC_STR));
	func.func = GET_TABLE_INFO;
	strcpy(func.table,tn);
	jEnv->GetByteArrayRegion(dbname,0,jEnv->GetArrayLength(dbname),(jbyte*)curdb);
	strcpy(func.dbname,curdb);

	if(db.GetTableInfo(&func,&datafmt)!=CS_FAIL)
	{
		fieldnums = func.colnum;
	}
	else
	{
		fieldnums = -1;
	}
	return fieldnums;
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_closeTableInfo
 * �����������������Ϣ
 * ����    ��
 *		     
 *
 * ����ֵ  ��
 *
 * 
 * --------------------------------------------------
 */

JNIEXPORT void JNICALL Java_com_daqo_dbinfo_Dlllink_closeTableInfo
  (JNIEnv *jEnv, jobject jthis)
{
	memset(&func,0,sizeof(FUNC_STR));
	if(datafmt) {free(datafmt);datafmt=NULL;}
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_readdata
 * �����������õ���¼
 * ����    ��buf,��¼���ݴ�ŵĻ�����,1��1������������
 *		     
 *
 * ����ֵ  ��ֵ�ĸ���
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_readdata
  (JNIEnv *jEnv, jobject jthis, jobjectArray buf)
{

	if((datafmt==NULL) || (pt==NULL))
		return -1;
	if(func.ret_roxnum == 0) return 0;
	char tmpbuf[256];
	int ptcnt = 0;
	jint count = 0;
	for(int i =0;i<func.ret_roxnum;i++)//row
	{

		for(int j =0;j<colcount;j++)
		{
			for(int k =0;k<func.colnum;k++)
			{
				if(func.col[k].select!=YES)		continue;

				int od = func.col[k].order_d;
				if(od!=j) continue;
				count++;
				memset(tmpbuf,0,256);
				jbyteArray field = (jbyteArray)jEnv->GetObjectArrayElement(buf,i*colcount + j);
				switch ((datafmt+k)->datatype)
				{
					case CS_TINYINT_TYPE:
					{
						unsigned char bb = (unsigned char)db.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
						sprintf(tmpbuf,"%d",bb);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_SMALLINT_TYPE:
					{
						short sint = (short)db.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
						sprintf(tmpbuf,"%d",sint);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_INT_TYPE:
					{
						int  intrtn = (int)db.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
						sprintf(tmpbuf,"%d",intrtn);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_LONG_TYPE:
					{
						long longrtn = (long)db.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
						sprintf(tmpbuf,"%d",longrtn);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_REAL_TYPE:
					/*{
						break;
					}*/
					case CS_FLOAT_TYPE:
					{
						double  frtn = (double)db.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
						sprintf(tmpbuf,"%f",frtn);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_BINARY_TYPE:
					case CS_CHAR_TYPE:
					case CS_VARCHAR_TYPE:
					{
						db.GetData(i,j,&func,datafmt,pt,ptcnt,tmpbuf);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_DECIMAL_TYPE:
					{
						break;
					}
				}//switch
				break;//new add
			}//for k
		}//for j
	}//for i
	if(datafmt) {	free (datafmt);datafmt=NULL;}
	if(pt)	{	free(pt);pt=NULL;	}
	return count;

}



/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_ReadRecorders
 * ���������������ݿ�
 * ����    ��tablename,����;fieldname,����;  where,����;orderfn,��������
 *		   fieldcount,ѡ���������;bdistinct,�Ƿ�ѡ��Ψһֵ
 *
 * ����ֵ  ��ֵ�ĸ���,<0ʱ����
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_readRecorders
  (JNIEnv *jEnv, jobject jthis, jbyteArray dbname, jbyteArray tablename, jbyteArray fieldname, jbyteArray where, jbyteArray orderfn, jintArray fieldcount, jboolean bdistinct)
{

	char tn[128],ofn[128],curdb[128];//����,��������
	memset(tn,0,128);
	memset(ofn,0,128);
	memset(curdb,0,128);
	char *fn = NULL,*we = NULL;//ѡ������������
	int arrlen = 0;
	arrlen = jEnv->GetArrayLength(fieldname);//ѡ������
	if(arrlen>0)
	{
		fn = (char*)malloc(arrlen+1);
		memset(fn,0,arrlen+1);
		jEnv->GetByteArrayRegion(fieldname,0,arrlen,(jbyte*)fn);
	}
	else
	{
		fn = (char*)malloc(1+1);
		memset(fn,0,2);
		fn[0] = '*';
	}

	arrlen = jEnv->GetArrayLength(where);//����
	if(arrlen>0)
	{
		we = (char*)malloc(arrlen+1);
		memset(we,0,arrlen+1);
		jEnv->GetByteArrayRegion(where,0,arrlen,(jbyte*)we); 
	}

	arrlen = jEnv->GetArrayLength(tablename); //����
	jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tn);
	
	arrlen = jEnv->GetArrayLength(orderfn);//��������
	if(arrlen>0)	
		jEnv->GetByteArrayRegion(orderfn,0,arrlen,(jbyte*)ofn);

	CS_RETCODE      retcode = 0; 

	func.func = GET_TABLE_INFO;
	strcpy(func.table,tn);

	jEnv->GetByteArrayRegion(dbname,0,jEnv->GetArrayLength(dbname),(jbyte*)curdb);
	strcpy(func.dbname,curdb);
	
	if(datafmt) {free(datafmt);datafmt=NULL;}
	if(pt) {free(pt);pt = NULL;}
     
	if(db.GetTableInfo(&func,&datafmt)!=CS_FAIL)
	{		
		FDNAME *parsedfdname = NULL;

		colcount = parsefieldstring(fn,(void**)&parsedfdname);
		int orderf = (ofn[0]==0) ? FALSE : TRUE;
		func.distinctflag = bdistinct;
		if(colcount > 0 && parsedfdname[0].fdname[0] == '*')//ȫѡ
		{
			colcount = func.colnum;
			for(int i=0; i<func.colnum; i++)
			{
				func.col[i].condition = NO;
				func.col[i].select = YES;
				func.col[i].order_d = i; //order data,used to column order ,must be ��ֵ
				func.col[i].order = -1;
			#ifdef WIN32
				if(orderf && !stricmp(datafmt[i].name,ofn))
			#else
				if(orderf && !strcasecmp(datafmt[i].name,ofn))
			#endif
				func.col[i].order = 0;	//used to  order by,��Ҫ�͸�ֵ				
			}
		}
		else
		{
			for(int i=0; i<func.colnum; i++)
			{
				func.col[i].condition = NO;
				func.col[i].select = NO;
				func.col[i].order = -1;
				func.col[i].order_d = -1;
				for(int j=0; j<colcount; j++)
				{
				#ifdef WIN32
					if(!stricmp(datafmt[i].name,parsedfdname[j].fdname))
				#else
					if(!strcasecmp(datafmt[i].name,parsedfdname[j].fdname))
				#endif
					{
						func.col[i].select = YES;
						func.col[i].order_d = j;
					}
				}
			#ifdef WIN32
				if(orderf && !stricmp(datafmt[i].name,ofn))
			#else
				if(orderf && !strcasecmp(datafmt[i].name,ofn))
			#endif
					func.col[i].order = 0;
			}
		} 

		jEnv->SetIntArrayRegion(fieldcount,0,1,(jint*)&colcount);

		if(parsedfdname) free(parsedfdname);

		func.func = SEL_RECORDERS;
		if(we != NULL)
		{
			strcpy(func.isql,we);
			func.func = SEL_ISQL_RECORDERS;
		}
		//printf("%s step 1\n",tn);
		if((retcode = db.SelectRcds(&func,datafmt,&pt)) == CS_SUCCEED)
		{
			retcode = func.ret_roxnum * colcount;
		}
		else
			retcode = -2;
		//printf("%s step 2\n",tn);
	}
	else
		retcode = -1;

	if(fn) free(fn);
	if(we) free(we);
	if(retcode<0)
	{
		if(datafmt) {free(datafmt);datafmt=NULL;}
		if(pt) {free(pt);pt = NULL;}
	}
	return retcode;
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_insertARecord
 * ��������������һ����¼���ݿ�
 * ����    ��jobjectArray,��ż�¼ֵ������byte[][]
 *		   
 *
 * ����ֵ  ��ֵ�ĸ���,<0ʱ����
 *
 *��ʹ�ô˺���ǰ��Ҫ��ʹ��Java_com_daqo_dbinfo_Dlllink_openTableInfo��
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_insertARecord
  (JNIEnv *jEnv, jobject jthis, jobjectArray colvalue)
{
	jint rtn = -1;
	int fieldnums = func.colnum;
	int arrlen = jEnv->GetArrayLength(colvalue);
	if((fieldnums != arrlen) || (datafmt==NULL))
	{
		rtn = -2;//����ļ�¼���Ȳ���
	}
	else
	{
		func.func = INS_ONE_RECORDER;

		int ptcnt = 0;
		char tmp[256];
		for(int i =0;i<fieldnums;i++)
		{
			jbyteArray colarr = (jbyteArray)jEnv->GetObjectArrayElement(colvalue,i);
			arrlen = jEnv->GetArrayLength(colarr);
			memset(tmp,0,256);
			jEnv->GetByteArrayRegion(colarr,0,arrlen,(jbyte*)tmp);
			switch ((datafmt+i)->datatype)
			{
				case CS_BINARY_TYPE:
				{
					break;
				}
				case CS_TINYINT_TYPE:
				{
					func.col[i].val.tinyvalue = atoi(tmp);
					break;
				}
				case CS_SMALLINT_TYPE:
				{
					func.col[i].val.smallvalue = atoi(tmp);
					break;
				}
				case CS_INT_TYPE:
				{
					func.col[i].val.intvalue = atoi(tmp);
					break;
				}
				case CS_LONG_TYPE:
				{
					func.col[i].val.intvalue = atoi(tmp);
					break;
				}
				case CS_REAL_TYPE:
				{
					func.col[i].val.realvalue = (float)atof(tmp);
					break;
				}
				case CS_FLOAT_TYPE:
				{
					func.col[i].val.floatvalue = atof(tmp);
					break;
				}
				case CS_CHAR_TYPE:
				case CS_VARCHAR_TYPE:
				{
					strcpy(func.col[i].val.charvalue,tmp);
					break;
				}
				case CS_DECIMAL_TYPE:
				{
					break;
				}
			}//switch
		}//for
	}//if

	if(db.InsertOneRcd(&func,datafmt)==CS_SUCCEED)
		rtn = CS_SUCCEED;
	
#ifdef INCEMS
	//���뵽�߼�Ӧ��ʵʱ��
	
#endif 


	return rtn;
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_updateARecord
 * ��������������һ����¼
 * ����    ��colvalue,���µļ�¼ֵ��ǰ����Ϊ����ԭֵ;field,Ϊ��¼���кţ�ǰ����ֵΪ�������к�
 *		     keycolnums,��������Ŀ
 *
 * ����ֵ  ��>0,�ɹ�������ʧ��
 *
 *��ʹ�ô˺���ǰ��Ҫ��ʹ��Java_com_daqo_dbinfo_Dlllink_openTableInfo��
 * --------------------------------------------------
 */

JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_updateARecord
  (JNIEnv *jEnv, jobject jthis, jobjectArray colvalue, jintArray field, jint keycolnums)
{
	CS_RETCODE      retcode = 0; 

	func.roxnum = 1;
	int fieldArrLen = jEnv->GetArrayLength(field);
	
	int *pfield = new int[fieldArrLen];

	int i,j;
	char tmp[256];
	for(i=0;i<fieldArrLen;i++)
	{
		jEnv->GetIntArrayRegion(field,i,1,(jint *)(pfield+i));//�����ȡ����
	}



	func.func = UPD_ONE_RECORDER;
	
	for(i=0; i<func.colnum; i++)
	{
		func.col[i].condition = NO;
		func.col[i].select = NO;

		for( j =0;j<keycolnums;j++)//�ؼ��ֶ�
		{
			if(i==pfield[j])
			{
				func.col[i].condition = YES;
				jbyteArray colarr = (jbyteArray)jEnv->GetObjectArrayElement(colvalue,j);
				int arrlen = jEnv->GetArrayLength(colarr);
				memset(tmp,0,256);
				jEnv->GetByteArrayRegion(colarr,0,arrlen,(jbyte*)tmp);

				switch ((datafmt+i)->datatype)
				{
					case CS_BINARY_TYPE:
					{
						break;
					}
					case CS_TINYINT_TYPE:
					{
						func.col[i].val_o.tinyvalue = atoi(tmp);
						break;
					}
					case CS_SMALLINT_TYPE:
					{
						func.col[i].val_o.smallvalue = atoi(tmp);
						break;
					}
					case CS_INT_TYPE:
					{
						func.col[i].val_o.intvalue = atoi(tmp);
						break;
					}
					case CS_LONG_TYPE:
					{
						func.col[i].val_o.intvalue = atoi(tmp);
						break;
					}
					case CS_REAL_TYPE:
					case CS_FLOAT_TYPE:
					{
						func.col[i].val_o.floatvalue = atof(tmp);
						break;
					}
					case CS_CHAR_TYPE:
					case CS_VARCHAR_TYPE:
					{
						strcpy(func.col[i].val_o.charvalue,tmp);
						break;
					}
					case CS_DECIMAL_TYPE:
					{
						break;
					}
				}//switch

			}//if
		}//for j //�ؼ��ֶ�

		for(j=keycolnums;j<fieldArrLen;j++)
		{
			if(i==pfield[j])
			{
				func.col[i].select = YES;
				jbyteArray colarr = (jbyteArray)jEnv->GetObjectArrayElement(colvalue,j);
				int arrlen = jEnv->GetArrayLength(colarr);
				memset(tmp,0,256);
				jEnv->GetByteArrayRegion(colarr,0,arrlen,(jbyte*)tmp);
				switch ((datafmt+i)->datatype)
				{
					case CS_BINARY_TYPE:
					{
						break;
					}
					case CS_TINYINT_TYPE:
					{
						func.col[i].val.tinyvalue = atoi(tmp);
						break;
					}
					case CS_SMALLINT_TYPE:
					{
						func.col[i].val.smallvalue = atoi(tmp);
						break;
					}
					case CS_INT_TYPE:
					{
						func.col[i].val.intvalue = atoi(tmp);
						break;
					}
					case CS_LONG_TYPE:
					{
						func.col[i].val.intvalue = atoi(tmp);
						break;
					}
					case CS_REAL_TYPE:
					{
						func.col[i].val.realvalue = (float)atof(tmp);
						break;
					}
					case CS_FLOAT_TYPE:
					{
						func.col[i].val.floatvalue = (float)atof(tmp);
						break;
					}
					case CS_CHAR_TYPE:
					case CS_VARCHAR_TYPE:
					{
						strcpy(func.col[i].val.charvalue,tmp);
						break;
					}
					case CS_DECIMAL_TYPE:
					{
						break;
					}
				}//switch
			}
		}//for j

	}
	if(pfield) free(pfield);
	int ret = db.UpdateOneRcd(&func,datafmt);

#ifdef INCEMS
	//���¸߼�Ӧ��ʵʱ��

#endif 


	//�ڽ��ֳ������޸�ң��������е�һ��ϵ��ֵʱ��������վ�⣬���еļ�¼�������ͬ����ֵ
	//Ϊ�����Ӽ�¼������Ϣ�Ĵ����
	//debug begin
	retcode = 0; 
    CS_CHAR *cmdbuf, *cmdbuf1,*cmdbufc,*cmdbufc1; 
    CS_INT          beginflag,beginflagc; 
    CS_INT          affectrow = 0; 
    CS_CHAR        tempstr[600]; 
    

    if ((func.func != UPD_ONE_RECORDER)|| (func.colnum<1)) return (ret); 
    
	cmdbuf = (CS_CHAR *) malloc (EX_BUFSIZE); 
    cmdbuf1 = (CS_CHAR *) malloc (EX_BUFSIZE); 
    if ((cmdbuf == (CS_CHAR *)NULL)|| (cmdbuf1 == (CS_CHAR *)NULL))        return ret; 
    memset (cmdbuf, 0, EX_BUFSIZE); 
    memset (cmdbuf1, 0, EX_BUFSIZE); 

	cmdbufc = (CS_CHAR *) malloc (EX_BUFSIZE); 
    cmdbufc1 = (CS_CHAR *) malloc (EX_BUFSIZE); 
    
    beginflag = YES; 
	beginflagc = YES; 
    sprintf (cmdbuf, UPDATE_ANY_ONE_RECORDER, func.table); 

  
    for (i=0; i<func.colnum; i++) {
        if (func.col[i].select != YES && func.col[i].condition != YES) continue;

		if (func.col[i].select == YES){
            if (beginflag!= YES)
                strcat (cmdbuf, ","); 
            beginflag = NO; 
			FromDataToStr (datafmt+i, tempstr, & (func.col[i].val)); 
            sprintf (cmdbuf1, "%s=%s", datafmt[i].name, tempstr); 
            strcat (cmdbuf, cmdbuf1); 
        }

		if (func.col[i].condition == YES)	{
            if (beginflagc == YES) {
                sprintf (cmdbufc, " where "); 
                beginflagc = NO; 

            }
            else strcat (cmdbufc, " and "); 

			FromDataToStr (datafmt+i, tempstr, & (func.col[i].val_o)); 
			sprintf (cmdbufc1, "%s=%s", datafmt[i].name, tempstr); 
			strcat (cmdbufc, cmdbufc1); 
		}
    }
//    sprintf (cmdbuf1, UPDATE_ANY_ONE_RECORDER_CONDITION, func->his.sname);
    strcat (cmdbuf, cmdbufc); 
    free (cmdbuf1); 
	free (cmdbufc); 
	free (cmdbufc1); 
    
    affectrow = 0; 

    if (affectrow<1) retcode = CS_FAIL; 
	
	RdbDebugMesg("%s\n",cmdbuf);

    free (cmdbuf); 
    return ret; 
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_removeARecord
 * ����������ɾ��һ����¼
 * ����    ��where,����
 *		   
 *
 * ����ֵ  ��
 *
 *��ʹ�ô˺���ǰ��Ҫ��ʹ��Java_com_daqo_dbinfo_Dlllink_openTableInfo��
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_removeARecord
  (JNIEnv *jEnv, jobject jthis, jbyteArray where)
{
	CS_RETCODE      retcode = 0; 

	char tmp[MAX_CHAR_BUF];
	memset(tmp,0,MAX_CHAR_BUF);

	int arrlen = jEnv->GetArrayLength(where);
	jEnv->GetByteArrayRegion(where,0,arrlen,(jbyte*)tmp);

	func.func = DEL_ISQL_RECORDERS;
	func.roxnum = 1;
	func.distinctflag = 0;
	strcpy(func.isql,tmp);

	char pt;
#ifdef INCEMS
	//ɾ���߼�Ӧ��ʵʱ��
	//��Ҫ��where����ȡ�ؼ��ֵ�ֵ�����ڱ��м�����
	//���������������У�����Ҫ�µĽӿ���ʵ�֡�
#endif 

	return db.DeleteRcds(&func,datafmt,&pt);

}

/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_reCall
 * ��������������ʵʱ��
 * ����    ��server,�����������飻name����������(��������)
 *		   ���������
 *
 * ����ֵ  ��=0,����ʧ��
 *
 *
 * --------------------------------------------------
 */

JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_reCall
  (JNIEnv *jEnv, jobject jthis, jobjectArray serverpara, jobjectArray namepara)
{
	int size = jEnv->GetArrayLength(serverpara);
	jint rtn = 1;
	if(size == 0) return 0;
	RtdbProc rtdbproc;
	char user[64];
	memset(user,0,64);
	strcpy(user,"scada");

	char password[64];
	memset(password,0,64);
	strcpy(password,"scada");

	char server[64];
	char name[64];
	for(int i=0;i<size;i++)
	{
		memset(server,0,64);
		memset(name,0,64);

		jbyteArray serverarr = (jbyteArray)jEnv->GetObjectArrayElement(serverpara,i);
		jbyteArray namearr = (jbyteArray)jEnv->GetObjectArrayElement(namepara,i);
		jEnv->GetByteArrayRegion(serverarr,0,jEnv->GetArrayLength(serverarr),(jbyte*)server);//������
		jEnv->GetByteArrayRegion(namearr,0,jEnv->GetArrayLength(namearr),(jbyte*)name);//����
		rtn = rtn*rtdbproc.Recall(user,password,server,name);
	}
	return rtn;
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_reloadRtdb
 * ��������������ʵʱ��
 * ����    ��server,�����������飻
 *		   ��������������
 *
 * ����ֵ  ��=0,����ʧ��
 *
 *
 * --------------------------------------------------
 */

JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_reloadRtdb
  (JNIEnv *jEnv, jobject jthis, jobjectArray serverpara)
{
	int size = jEnv->GetArrayLength(serverpara);
	jint rtn = 1;
	if(size == 0) return 0;
	RtdbProc rtdbproc;
	char user[64];
	memset(user,0,64);
	strcpy(user,"scada");

	char password[64];
	memset(password,0,64);
	strcpy(password,"scada");

	char server[64];
	for(int i=0;i<size;i++)
	{
		memset(server,0,64);
		jbyteArray serverarr = (jbyteArray)jEnv->GetObjectArrayElement(serverpara,i);
		jEnv->GetByteArrayRegion(serverarr,0,jEnv->GetArrayLength(serverarr),(jbyte*)server);//������
		rtn = rtn*rtdbproc.ReloadRtdb(user,password,server);
	}
	return rtn;	
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_reloadMode
 * ��������������ʵʱ��
 * ����    ��load_type,δ�ã�
 *		   ȫ������
 *
 * ����ֵ  ��=0,����ʧ��
 *
 *
 * --------------------------------------------------
 */



JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_reloadMode
  (JNIEnv *jEnv, jobject jthis, jint load_type)
{
	jint rtn = 0;
	RtdbProc rtdbproc;
	char user[64];
	memset(user,0,64);
	strcpy(user,"scada");

	char password[64];
	memset(password,0,64);
	strcpy(password,"scada");

	rtn = rtdbproc.ReloadMode(user,password,load_type);
	return rtn;	

}


/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_makeBcp
 * ��������������bcp�ļ�
 * ����    �� groupno,��ţ�type,������ʹ���(ǰ�û�����),tablename,����
 *
 * ����ֵ  �������ļ���λ�ú�����
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_sendMsgToFert
  (JNIEnv *jEnv, jobject jthis, jint groupno, jbyte type, jbyteArray tablename)
{
	char tb[128];
	memset(tb,0,128);
	int arrlen = jEnv->GetArrayLength(tablename);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tb);
	else
		return -1;

	MailBox mb(groupno,"FEP");
	MailBox pub_mb(groupno,"PUBFEP"); //���Ӽ���ʵʱ����III�������ʼ����ܡ�
	char buf[128];
	FE_MAIL_STRUCT* mail_struct = (FE_MAIL_STRUCT*)buf;
	memset(mail_struct, 0, sizeof(FE_MAIL_STRUCT));	//��ʼ��

	mail_struct->mail_type = LOAD_PARA;
	mail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT);
// 	Fe_mail::send_to_fe(m_mail_struct, m_mail_struct->mail_lenth);

// 	memset(buf,0,128);
// 	buf[0] = type;
// 	strcpy(buf+1,tb);
	int len = mail_struct->mail_lenth;
	int result = mb.SendSrvRequest("123456",buf,len,FALSE);	//send mail to client,return TRUE if OK,called by server when result is to be sent
	pub_mb.SendPubSrvRequest("123456",buf,len,FALSE);	//����III��ǰ��
	return result;											//�޸�Ϊ���Է�������ǰ�û�
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_makeBcp
 * ��������������bcp�ļ�
 * ����    �� groupno,��ţ�type,������ʹ���(ǰ�û�����),tablename,����
 *
 * ����ֵ  �������ļ���λ�ú�����
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_sendMsgToWeb
  (JNIEnv *jEnv, jobject jthis)
{
	EventProc event;
	SYS_EVENT sysevent;
	memset(&sysevent,0,sizeof(SYS_EVENT));
	sysevent.PrintOut = 0;
	sysevent.DiskOut = 0;
	sysevent.ToneOut = 0;
	sysevent.GraphOut = 0;
	sysevent.DispOut = 1;

	sysevent.sort = ENTCLASS_WEB;
	sysevent.type = ENTTYPE_NULL;//
	sysevent.grpno = 1;
	return event.Save(&sysevent, 1);	
}
/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_makeBcp
 * ��������������bcp�ļ�
 * ����    �� groupno,��ţ�type,������ʹ���(ǰ�û�����),tablename,����
 *
 * ����ֵ  �������ļ���λ�ú�����
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_sendEventMsg
  (JNIEnv *jEnv, jobject jthis, jbyteArray username,jbyteArray tablename,jbyteArray hostname)
{
	
	char tb[128];
	memset(tb,0,128);
	int arrlen = jEnv->GetArrayLength(tablename);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tb);
	else
		return -1;

	char usr[128];
	memset(usr,0,128);
	arrlen = jEnv->GetArrayLength(username);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(username,0,arrlen,(jbyte*)usr);
	else
		jEnv->GetByteArrayRegion(username,0,arrlen,(jbyte*)"");


	char htn[24];
	memset(htn,0,24);
	arrlen = jEnv->GetArrayLength(hostname);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(hostname,0,arrlen,(jbyte*)htn);
	else
		jEnv->GetByteArrayRegion(hostname,0,arrlen,(jbyte*)"");
	NetStatus cfg;
	
	if(strcmp(usr,"")==0)
		sprintf(usr,"%s","�����û�");

	char * hostdesc=cfg.GetNodeName(1,htn);

	sprintf(usr,"%s �� %s ���޸���%s",usr,hostdesc,tb);

	EventProc event;
	SYS_EVENT sysevent;
	memset(&sysevent,0,sizeof(SYS_EVENT));
	sysevent.PrintOut = 1;
	sysevent.DiskOut = 1;
	sysevent.ToneOut = 1;
	sysevent.GraphOut = 1;
	sysevent.DispOut = 1;

//	time_t  cur_time;
//	time(&cur_time);//�õ�1970���������������
    //cur_time -= (24*60*60);//��ȥһ����������õ�����ͽ�����ͬ��ʱ����
//	CTime etime(cur_time);


	Rdb_Time  rdbtime;
	RDB_DATETIME_MS_STRU  curclock;

	rdbtime.RdbGetCurClock(&curclock);

//	sprintf(debugmsg,"%.4d-%.2d-%.2d  %.2d:%.2d:%.2d.%.3d  ",curclock.year,curclock.month,
  //   curclock.day,curclock.hour,curclock.minute,curclock.second,curclock.msecond);
 	sysevent.ymd=curclock.year*10000+curclock.month*100+curclock.day;
 	sysevent.hmsms=curclock.hour*10000000+curclock.minute*100000+curclock.second*1000;

//	sysevent.ymd=etime.GetYear()*10000+etime.GetMonth()*100+etime.GetDay();
//	sysevent.hmsms=etime.GetHour()*10000000+etime.GetMinute()*100000+etime.GetSecond()*1000;

	sysevent.sort = ENTCLASS_OPERATION;
	sysevent.type = ENTTYPE_USERDEF;//
	sysevent.grpno = 1;
	strcpy(sysevent.text,usr);

	return event.Save(&sysevent, 1);
	return 1;
}

/*	����˵��
 * --------------------------------------------------
 * ������  Java_com_daqo_dbinfo_Dlllink_getAMaxMinVal
 * ����������ִ��select ��Ƕ����
 * ����    �� tablename,������fieldname���ֶ�����val����ֵ 
 *
 * ����ֵ ==0, ʧ�ܡ�����ɹ���
 *
 *
 * --------------------------------------------------
 */

JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_getAMaxMinVal
  (JNIEnv *jEnv, jobject jthis, jbyteArray dbname,jbyteArray tablename, 
	jbyteArray fieldname, jbyte flag)
{
	char dbn[64],tb[128],fd[128];

	memset(dbn,0,64);
	int arrlen = jEnv->GetArrayLength(dbname);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(dbname,0,arrlen,(jbyte*)dbn);
	else
		return false;

	memset(tb,0,128);
	arrlen = jEnv->GetArrayLength(tablename);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tb);
	else
		return false;

	memset(fd,0,128);
	arrlen = jEnv->GetArrayLength(fieldname);
	if(arrlen > 0)
		 jEnv->GetByteArrayRegion(fieldname,0,arrlen,(jbyte*)fd);
	else return false;

	CS_RETCODE		retcode;

	pt = NULL;
	if(dbn[0] == '\0') return false;
	strcpy(func.dbname,dbn);
	func.func = SEL_ISQL_RESULTS;
	char SQLWord[256];
	memset(SQLWord,0,256);
	if(flag == 0)
	{
	sprintf(SQLWord,"select distinct %s from %s where %s = \
		(select max(%s) from %s)",fd,tb,fd,fd,tb);
	}
	else if(flag == 1)
	{
	sprintf(SQLWord,"select distinct %s from %s where %s = \
		(select min(%s) from %s)",fd,tb,fd,fd,tb);
	}
	sprintf(func.isql,"%s",SQLWord);
	
	retcode = db.SelectIsqlResults(&func,&datafmt,&pt);
    if ((retcode != CS_SUCCEED) || (func.ret_roxnum<1))
	{
	    if(datafmt) 
		{
			free(datafmt);
			datafmt = NULL;
		}
	    if(pt)
		{
			free(pt);
			pt = NULL;
		}
		return false;
	}

	func.col[0].order_d = 0;
	func.col[0].select=YES;

	int ptcnt = 0;
	int  frtn = (int)db.GetData(0,0,&func,datafmt,pt,ptcnt,NULL);
	
	if (datafmt) {free(datafmt);;datafmt=NULL;}
	if(pt) {free(pt);pt=NULL;}
	return frtn;		
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_makeBcp
 * ��������������bcp�ļ�
 * ����    �� binoutf=true,����in�ļ�����֮��Ϊout�ļ�
 *		   dbname,���ݿ���;srvname,��������
 *         filename,�ļ���;username,�û���
 *		   pass,����
 *
 * ����ֵ  �������ļ���λ�ú�����
 *
 *
 * --------------------------------------------------
 */
  JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_execproc
  (JNIEnv *jEnv, jobject jthis, jint flag, jint devid, jint devtype,jint olddevtype)
  {
		FUNC_STR    func1;
		func1.func=ISQL_COMMAND;
		if(flag == 0)//INSERT
		{
			sprintf(func1.isql,"exec insertdevice %d ,%d",devid,devtype);
		}
		else if(flag == 1)//DELETE
		{
			sprintf(func1.isql,"exec deletedevice %d, %d",devid,devtype);
		}
		else if(flag == 2)//UPDATE
		{
			sprintf(func1.isql,"exec updatedevice %d, %d,%d",devid,olddevtype,devtype);
		}
		strcpy(func1.dbname,"modeldb");
		return db.IsqlCommand(&func1);
  }


/*	����˵��
 * --------------------------------------------------
 * ������  ��Java_com_daqo_dbinfo_Dlllink_makeBcp
 * ��������������bcp�ļ�
 * ����    �� binoutf=true,����in�ļ�����֮��Ϊout�ļ�
 *		   dbname,���ݿ���;srvname,��������
 *         filename,�ļ���;username,�û���
 *		   pass,����
 *
 * ����ֵ  �������ļ���λ�ú�����
 *
 *
 * --------------------------------------------------
 */

JNIEXPORT jbyteArray JNICALL Java_com_daqo_dbinfo_Dlllink_makeBcp
  (JNIEnv *jEnv, jobject jthis, jbyteArray dbnamearr, jbyteArray srvname, jbyteArray filename, jbyteArray username, jbyteArray pass, jboolean binoutf)
{

#ifdef WIN32
    char dbname[64],srv[64],file[256],user[64],pwd[64];
	memset(dbname,0,64);//���ݿ���
	memset(srv,0,64);
	memset(file,0,256);
    memset(user,0,64);
	memset(pwd,0,64);

	BOOL bcreatefile = false;

	int arrlen = jEnv->GetArrayLength(dbnamearr);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(dbnamearr,0,arrlen,(jbyte*)dbname);
	else
		strcpy(dbname,"modeldb");

	arrlen = jEnv->GetArrayLength(srvname);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(srvname,0,arrlen,(jbyte*)srv);
	else
		strcpy(srv,"nbdb");

	arrlen = jEnv->GetArrayLength(filename);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(filename,0,arrlen,(jbyte*)file);
	else
	{
		if(binoutf)
		  strcpy(file,"c:\\bcpin.bat");
		else
			strcpy(file,"c:\\bcpout.bat");
	}

	arrlen = jEnv->GetArrayLength(username);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(username,0,arrlen,(jbyte*)user);
	else
		strcpy(user,"sa");

	arrlen = jEnv->GetArrayLength(pass);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(pass,0,arrlen,(jbyte*)pwd);
	else
		strcpy(pwd,"");

	
	int retcode = 0;

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"sysobjects");

	strcpy(func.dbname,dbname);
	
	if(datafmt) {free(datafmt);datafmt=NULL;}
	if(pt) {free(pt);pt = NULL;}

	int i;
	int j;

	if(db.GetTableInfo(&func,&datafmt)!=CS_FAIL)
	{		
		func.distinctflag = false;
		for( i=0; i<func.colnum; i++ )
		{
			func.col[i].condition = NO;
			func.col[i].select = NO;
			func.col[i].order = -1;
			func.col[i].order_d = -1;
	#ifdef WIN32
			if(!stricmp(datafmt[i].name,"name"))
	#else 
			if(!strcasecmp(datafmt[i].name,"name"))
	#endif
			{
				func.col[i].select = YES;
				func.col[i].order_d = 0;
			}
		}

		func.func = SEL_ISQL_RECORDERS;
		strcpy(func.isql," type = 'U' ");

		int ptcnt = 0;

		if(db.SelectRcds(&func,datafmt,&pt) == CS_SUCCEED)
		{
			//bcp modeldb.dbo.pdb_field in  pdb_field -Usa -P  -Snbdb  -c
			char *buf = (char*)malloc(func.ret_roxnum*128);
			memset(buf,0,func.ret_roxnum*128);
			int count = 0;
			char tablebuf[256];
			char tmpbuf[256];
			for( i =0;i<func.ret_roxnum;i++)
			{
				memset(tablebuf,0,256);
				memset(tmpbuf,0,256);
				db.GetData(i,0,&func,datafmt,pt,ptcnt,tablebuf);//����
				sprintf(tmpbuf,"bcp %s.dbo.%s",func.dbname,tablebuf);
				int len = strlen(tmpbuf);
				for( j= len;j<50;j++)
				{
					strcat(tmpbuf," ");
				}
				strcat(buf,tmpbuf);
				memset(tmpbuf,0,256);
				if(binoutf)
					sprintf(tmpbuf,"in  %s",tablebuf);
				else
					sprintf(tmpbuf,"out %s",tablebuf);
				len = strlen(tmpbuf);
				for(j = len;j<35;j++)//��������󳤶�Ϊ30�����
					strcat(tmpbuf," ");
				strcat(buf,tmpbuf);
				memset(tmpbuf,0,256);
				sprintf(tmpbuf," -U%s -P%s -S%s -c\r\n",user,pwd,srv);
				strcat(buf,tmpbuf);
				if(++count%5 ==0)
					strcat(buf,"\r\n");
			}
			CFile cfile;
			bcreatefile = cfile.Open(file,CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone);
			if(bcreatefile)			
				cfile.Write(buf,strlen(buf));

			cfile.Close();
			if(buf) 
				free(buf);
		}
		else
			retcode = -1;
		if(datafmt) {free(datafmt);datafmt=NULL;}
		if(pt) {free(pt);pt = NULL;}
	}
	else
		retcode = -1;
	jbyteArray rtnarray;
	if(retcode == -1 || !bcreatefile)
	{
		memset(file,0,256);
		rtnarray = jEnv->NewByteArray(256);		
		jEnv->SetByteArrayRegion(rtnarray,0,0,(jbyte*)file); 
	}
	else
	{
		rtnarray = jEnv->NewByteArray(strlen(file));		
		jEnv->SetByteArrayRegion(rtnarray,0,strlen(file),(jbyte*)file); 
	}
	return rtnarray;
#else
	return 0;
#endif
}
