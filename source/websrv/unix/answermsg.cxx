/*
 * --------------------------------------------------
 *   �ļ�����answermsg.cxx 
 *   �������DS3210�������ϵͳWeb������   
 *   �ļ���Ҫ���ݣ��ش�ͻ��˵���Ϣ
 *
 *   Copyright , 2002
 *   �汾�ţ�v1.0.0,	2002/06/03
 *					 
 *   ����ˣ�
 *	 ˵����
 * --------------------------------------------------
 */

#ifdef WIN32
	#include "stdafx.h"
#endif 

#ifdef __unix
	#include <stdlib.h>
	#include <strings.h>
	#include <sys/types.h>
    #include <dirent.h>
#endif

#include "webstruct.h"

#include "externdefine.h"
#include "db.h"
#include "scd/saveflag.h"
#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scd/dev.h"
#include "filestat.h"

THREADMANAGE threadinfo[INITCLIENTNUMS];
char DBNAME[64];
char HISDBNAME[64];
char DBO[64];

#ifdef WIN32
	extern int  clntchangef;
#else
	extern void recvStream(int iii);
#endif

CDB dbo;
USEREVENT *puserevent = NULL;	//ָ��web�¼������
DEVPARATABLE devparatable;		//�豸�������ձ�
extern void readstation(char *groupname);




int strcmpNoCase(const char * string1, const char * string2)
{
#ifdef WIN32
	return _stricmp(string1,string2);
#else
	return strcasecmp(string1,string2);
#endif
}

BOOL IsLittleEndian(void) 
{ 
	unsigned short  wValue = 0x5678; 
	return (*((unsigned char*)&wValue) == 0x78); 
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��convertPasswd()
 * �����������������
 * ����    ��
 *          PWD=����ԭ��ָ��
 * ����ֵ  �� 
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
void  convertPasswd(char* pwd)
{
      unsigned int i;
      for(i=0;i<strlen(pwd);i++)  
		  pwd[i]=pwd[i]-50-i;
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
	NAME *p = (NAME *)malloc(num*sizeof(NAME));
	memset(p,0,sizeof(NAME)*num);
	int len = strlen(srcstr);
	int j = 0,i;
	for( i =0;i<len;i++)
	{
		if(srcstr[i] == ',')
		{
			if(i>j)
			{
				if(count>0 && num/count >=1)
				{
					NAME *temp = p;
					p = (NAME*)malloc((num+count)*sizeof(NAME));
					memset(p,0,sizeof(NAME)*(num+count));
					memcpy(p,temp,num*sizeof(NAME));
					FREE(temp);
					num += count;
				}
				memcpy((char*)(p[count].name),(char*)&srcstr[j],i-j);
				count++;
			}
			j = i+1;
		}
	}
	if(i>j)//���һ������",",���п������һ����","
	{
		memcpy((char*)(p[count].name ),(char*)&srcstr[j],i-j);
		j = i+1;
		count++;
	}
	*desp = (void*)p;
	p = NULL;
	return count;
}

void getTableinfo(char*tablename,void**buf)
{
	dbo.requestDBApi();
	*buf = NULL;

	CS_RETCODE      retcode = 0; 
	FUNC_STR func;
	CS_DATAFMT *datafmt = NULL;

	void* pt = NULL;
	func.func = GET_TABLE_INFO;
	strcpy(func.dbname,DBNAME);
	strcpy(func.table,tablename);
	if(dbo.GetTableInfo(&func,&datafmt)!=CS_FAIL)
	{
		NAME *pval = (NAME *)malloc(func.colnum*sizeof(NAME));
		memset(pval,0,func.colnum*sizeof(NAME));
		for(int i=0; i<func.colnum; i++)
		{
			sprintf((pval+i)->name,"%s",(datafmt+i)->name);//����
		}
		*buf = (void*)pval;
		pval = NULL;
	}
	FREE(datafmt);
	dbo.releaseDBApi();
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��ReadRecorders
 * ���������������ݿ�
 * ����    ��tablename,����;fieldname,����;  where,����;orderfn,��������
 *		   fieldcount,ѡ���������
 *
 * ����ֵ  ��ֵ�ĸ���(���Ǽ�¼�ĸ���),<0ʱ����
 *           buf,Ϊ��ѯ��ֵ������������������
 *
 * --------------------------------------------------
 */
int readRecorders(char* dbname,char* tablename,char* fieldname, char* where,char* orderfn,int bdistinct,int *fieldcount,void **buf)
{
	dbo.requestDBApi();
	CS_RETCODE      retcode = 0; 
	FUNC_STR func;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL;
    *buf = NULL;
	
	func.func = GET_TABLE_INFO;
	strcpy(func.dbname,dbname);
	strcpy(func.table,tablename);
	
	if(fieldname != NULL  && fieldname[0] != '\0')
	{
		if(dbo.GetTableInfo(&func,&datafmt) != CS_FAIL)
		{		
			NAME *parsedfdname = NULL;

			int colcount = parsefieldstring(fieldname,(void**)&parsedfdname);

			int orderf = (orderfn == NULL || orderfn[0] =='\0') ? FALSE : TRUE;
			func.distinctflag = bdistinct;
			if(colcount > 0 && parsedfdname[0].name[0] == '*')//ȫѡ
			{
				colcount = func.colnum;
				for(int i=0; i<func.colnum; i++)
				{
					func.col[i].condition = NO;
					func.col[i].select = YES;
					func.col[i].order_d = i; //order data,used to column order ,must be ��ֵ
					func.col[i].order = -1;
					if(orderf && !strcmpNoCase(datafmt[i].name,orderfn))
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
						if(!strcmpNoCase(datafmt[i].name,parsedfdname[j].name))
						{
							func.col[i].select = YES;
							func.col[i].order_d = j;
						}
					}
					if(orderf && !strcmpNoCase(datafmt[i].name,orderfn))
						func.col[i].order = 0;
				}
			} 
		
			*fieldcount = colcount;
			FREE(parsedfdname);

			func.func = SEL_RECORDERS;
			if(where != NULL && where[0] != '\0')
			{
				strcpy(func.isql,where);
				func.func = SEL_ISQL_RECORDERS;
			}//if(we != NULL)
			if((retcode = dbo.SelectRcds(&func,datafmt,&pt)) == CS_SUCCEED)
			{
				retcode = func.ret_roxnum * colcount;
				if(retcode>0)
				{
					int ptcnt = 0;
					NAME *pval = (NAME *)malloc(retcode*sizeof(NAME));
					memset(pval,0,retcode*sizeof(NAME));
					for(int i =0;i<func.ret_roxnum;i++)//row
					{
						for(int j =0;j<colcount;j++)
						{
							for(int k =0;k<func.colnum;k++)
							{
								if(func.col[k].select!=YES)		continue;

								int od = func.col[k].order_d;
								if(od!=j) continue;
									switch ((datafmt+k)->datatype)
									{
										case CS_BINARY_TYPE:
										{
											break;
										}
										case CS_TINYINT_TYPE:
										{
											unsigned char bb = (unsigned char)dbo.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
											sprintf(pval[i*colcount + j].name,"%d",bb);
											break;
										}
										case CS_SMALLINT_TYPE:
										{
											short sint = (short)dbo.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
											sprintf(pval[i*colcount + j].name ,"%d",sint);
											break;
										}
										case CS_INT_TYPE:
										{
											int  intrtn = (int)dbo.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
											sprintf(pval[i*colcount + j].name ,"%d",intrtn);
											break;
										}
										case CS_LONG_TYPE:
										{
											long longrtn = (long)dbo.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
											sprintf(pval[i*colcount + j].name ,"%d",longrtn);
											break;
										}
										case CS_REAL_TYPE:
										case CS_FLOAT_TYPE:
										{
											double  frtn = (double)dbo.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
											sprintf(pval[i*colcount + j].name ,"%f",frtn);
											break;
										}
										case CS_CHAR_TYPE:
										case CS_VARCHAR_TYPE:
										{
											dbo.GetData(i,j,&func,datafmt,pt,ptcnt,pval[i*colcount + j].name );
											break;
										}
										case CS_DECIMAL_TYPE:
										{
											break;
										}
									}//switch
									break;
								}//for k
							}//for j
						}//for i
					*buf = (void*)pval;
					pval = NULL;
				}//if(retcode>0)
			}//if((retcode = pdb->SelectRcds(&func,datafmt,&pt)) == CS_SUCCEED)
			else
				retcode = -2;
		}
		else
			retcode = -1;
	}	
	dbo.releaseDBApi();
	FREE(datafmt);
	FREE(pt);
	return retcode;
}


/*
 * ��������:ִ��SQL����
 * ����    ��DBName,���ݿ�����sqlStatement��sql���ʽ.
 * ����ֵ  ��true�ɹ�.falseʧ��.
 * --------------------------------------------------
 */
int Execute(char *DBName,char* sqlStatement)
{
	if (sqlStatement == 0 || sqlStatement[0] == '\0')
	{
		return CS_FAIL;
	}

	dbo.requestDBApi();

	FUNC_STR    func;
	CS_INT		retcode;
	
	
	strcpy(func.dbname,DBName);
    func.func = ISQL_COMMAND;
	
	strcpy(func.isql,sqlStatement);
	retcode = dbo.IsqlCommand(&func);
	dbo.releaseDBApi();
   	return (retcode);

}


/*	����˵��
 * --------------------------------------------------
 * ������  ��loadExcludeEvent
 * ������������"web�¼������"
 * ����    ��
 *
 * ����ֵ  ��
 *
 * --------------------------------------------------
 */
void loadExcludeEvent()
{
	//������ֻ����"web�¼������"�����õ�����
	int fieldcount;
	void *buf=NULL;
	int ret = readRecorders(DBNAME,"web�¼������","���ͺ�,����","","����",TRUE,&fieldcount,&buf);
	if(ret>0)
	{
		NAME *pname = NULL;
		pname = (NAME*)buf;
		unsigned char userright = 0;
		USEREVENT *pevent = puserevent;//�¼����û���Ϊ����ࡣ��ȱʡ��2һ��Ҫ��ס
		USEREVENT *poldevent = pevent;
		for(int i=0;i<ret/fieldcount;i++)
		{
			userright = (unsigned char)atoi(pname[2*i+1].name);//����
			int haveuserflag = FALSE;
			pevent = puserevent;
			while(pevent != NULL)
			{
				if(userright == pevent->userright)
				{
					haveuserflag = TRUE;
					pevent->excludeevent[pevent->eventcount] = atoi(pname[2*i].name);
					pevent->eventcount++;
					break;
				}
				poldevent = pevent;
				pevent = pevent->puserevent;
			}
			if(!haveuserflag)
			{
				if(puserevent == NULL)
				{
					puserevent = new USEREVENT();
					puserevent->userright = userright;
					puserevent->excludeevent[puserevent->eventcount] = atoi(pname[2*i].name);
					puserevent->eventcount++;
				}
				else
				{
					poldevent->puserevent = new USEREVENT();//����1������
					poldevent->puserevent->userright = userright;
					poldevent->puserevent->excludeevent[poldevent->puserevent->eventcount] = atoi(pname[2*i].name);
					poldevent->puserevent->eventcount++;
				}
			}
		}
		buf = NULL;
		FREE(pname);
	}
}


//�豸������
void loadDevParaTable()
{
	int fieldcount;
	void *buf=NULL;
	int ret = readRecorders(DBNAME,"web�豸��������ձ�","�豸����,��������,��ʾ����","","�豸����",TRUE,&fieldcount,&buf);
	if(ret>0)
	{
		NAME *pname = NULL;
		pname = (NAME*)buf;
		
		int devcount = ret/fieldcount > DEVTYPENUM ? DEVTYPENUM :ret/fieldcount;
		devparatable.count = devcount;
	    for(int i=0;i<devcount;i++)
		{
			devparatable.devtype[i] = atoi(pname[i*fieldcount].name);
			strncpy(devparatable.paratable[i].name,pname[i*fieldcount+1].name,CODESIZE);
			strncpy(devparatable.showfield[i].name,pname[i*fieldcount+2].name,CODESIZE);
		}
		FREE(pname);
	}
}

//ȡ�豸����
char *getDevParaTable(int devtype,char *showfield)
{
	for(int i=0;i<devparatable.count;i++)
	{
		if(devparatable.devtype[i] == devtype)
		{
			strcpy(showfield,devparatable.showfield[i].name) ;
			return devparatable.paratable[i].name;
		}
	}
	return NULL;
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��specifyDir()
 * �����������ж�ĳһĿ¼�Ƿ���ָ����Ŀ¼
 * ����    ��dir,Ŀ¼��
 * ����ֵ��  true,��
 *			 false,��
 *           
 *
 * --------------------------------------------------
 */
int  specifyDir(char *dir)
{
#ifdef WIN32
	if(!_stricmp(dir,"config") || !_stricmp(dir,"line") || !_stricmp(dir,"normal") || 
		!_stricmp(dir,"station") || !_stricmp(dir,"system"))
		return TRUE;
#else
	if(!strcasecmp(dir,"config") || !strcasecmp(dir,"line") || !strcasecmp(dir,"normal") || 
		!strcasecmp(dir,"station") || !strcasecmp(dir,"system"))
		return TRUE;
#endif
	return FALSE;

}

//����"web�û���"
void  readWebUser(int threadno)
{
	int fieldcount;
	void *valbuf;
	char where[256];
	memset(where,0,256);
	int ret = readRecorders(DBNAME,"web�û���","�û���,����,����","","",FALSE,&fieldcount,&valbuf);
	printf("\n����'web�û����' readRecord ret=%d fieldcnt=%d \n",ret,fieldcount);
	MESSAGE msgbuf;
	msgbuf.type = S_USER;
	char *buf = msgbuf.messagebuf ;

	NAME *pname = NULL;
	
	if(ret>0)
	{
		int usercount = ret/fieldcount;
		
		pro_int2char(&usercount,buf);//�û�����
		buf += sizeof(int);
		
		pname = (NAME*)valbuf;
		unsigned char len = 0;
		char pass[256];
		for(int i=0;i<usercount;i++)
		{
			len = strlen(pname[i*fieldcount].name );//�û�������
			buf[0] = len;
			buf += sizeof(unsigned char);
			
			memcpy(buf,pname[i*fieldcount].name,len);//�û���
			buf += len;

			len = strlen(pname[i*fieldcount+1].name); //�����
			buf[0] = len;
			buf += sizeof(unsigned char);
			
			memset(pass,0,256);
			memcpy(pass,pname[i*fieldcount+1].name,len);//����
			memcpy(buf,pass,len);//����
			buf += len;

			buf[0] = (unsigned char)atoi(pname[i*fieldcount+2].name);//����
			buf += sizeof(unsigned char);
		}
	}
	else
	{
		int usercount = 0;
		pro_int2char(&usercount,buf);
		buf += sizeof(int);
	}
	msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
	writemsg(threadinfo[threadno].sockid,&msgbuf);
	FREE(pname);

}

//����webͼ�η����
void readWebFilename(int threadno)
{
	int fieldcount = 0;
	void *valbuf = NULL;

	//ͼ���û��ĸ���Ҳ�ǿ��Բ���
	int ret = readRecorders(DBNAME,"webͼ�η����","*",NULL,NULL,FALSE,&fieldcount,&valbuf);
		MESSAGE msgbuf;
		msgbuf.type = S_GRAPHGROUP;
		char *buf = msgbuf.messagebuf ;
		int usercount = 0;
		if(ret>0) 
		  usercount = ret/fieldcount;
		
		pro_int2char(&usercount,buf);//ͼ�μ�¼����
		buf += sizeof(int);
		
		NAME *pname = NULL;
		pname = (NAME*)valbuf;
		valbuf = NULL;
		unsigned char len = 0;
		for(int i=0;i<usercount;i++)
		{
			len = strlen(pname[i*fieldcount].name );//ͼ���ļ�������
			buf[0] = len;
			buf += sizeof(unsigned char);
			
			memcpy(buf,pname[i*fieldcount].name,len);//��ֹ���ʵ�ͼ���ļ���
			buf += len;

			buf[0] = (unsigned char)atoi(pname[i*fieldcount+1].name);//����
			buf += sizeof(unsigned char);
		}
		msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
		writemsg(threadinfo[threadno].sockid,&msgbuf);
		FREE(pname);
}

//�����û�Ȩ�޲�����
void readWebUserRight(int threadno)
{
	int fieldcount;
	void *valbuf;
	char where[256];
	memset(where,0,256);
	char fields[512]={"������,����Ա,�鿴��ʷ����,�鿴��ʷ����,��ȫ����0,��ȫ����1,��ȫ����2,��ȫ����3,��ȫ����4,��ȫ����5,��ȫ����6,��ȫ����7,��ȫ����8,��ȫ����9,��ȫ����10,��ȫ����11,��ȫ����12,��ȫ����13,��ȫ����14,��ȫ����15,��ȫ����16,��ȫ����17,��ȫ����18,��ȫ����19,��ȫ����20,��ȫ����21,��ȫ����22,��ȫ����23,��ȫ����24,��ȫ����25,��ȫ����26,��ȫ����27,��ȫ����28,��ȫ����29,��ȫ����30,��ȫ����31"};
	int ret = readRecorders(DBNAME,"webȨ�޲�����",fields,"","",FALSE,&fieldcount,&valbuf);
	printf("\n����'webȨ�޲�����' readRecord ret=%d fieldcnt=%d \n",ret,fieldcount);
	MESSAGE msgbuf;
	msgbuf.type = S_USERRIGHT;
	char *buf = msgbuf.messagebuf ;
	
	NAME *pname = NULL;
	
	if(ret>0)
	{
		int rowcount=ret/fieldcount;
		pro_int2char(&rowcount,buf);  //��¼����
		buf += sizeof(int);

		pname=(NAME*)valbuf;
		unsigned char len=0;
		for(int i=0;i<rowcount;i++)
		{
			len=strlen(pname[i*fieldcount].name);		//����������
			buf[0]=len;
			buf+=sizeof(unsigned char);
			
			memcpy(buf,pname[i*fieldcount].name,len);	//������
			buf+=len;

			len=strlen(pname[i*fieldcount+1].name);		//����Ա����
			buf[0]=len;
			buf+=sizeof(unsigned char);

			memcpy(buf,pname[i*fieldcount+1].name,len);	//����Ա
			buf+=len;
			

			//ǰ���2����ʾ���ߺ�����Ȩ�޵��ֶ�
			//��ȫ���𳤶�,����Ϊ2����ʾ��ȫ���𳤶�2�ֽ�short int�͡�
			len=strlen(pname[i*fieldcount+2].name);
			if(len==1) 
				len=2;
			buf[0]=len;
			buf+=sizeof(unsigned char);
			short int val;
			for(int j=0;j<32+2;j++)
			{
				val=atoi(pname[i*fieldcount+2+j].name);
				pro_short2char(&val,buf);
				buf+=sizeof(short int);
			}
		}
		
	}
	else
	{
		int usercount = 0;
		pro_int2char(&usercount,buf);
		buf += sizeof(int);
	}
	msgbuf.length=abs((int)(msgbuf.messagebuf-buf));
	writemsg(threadinfo[threadno].sockid,&msgbuf);
	FREE(pname);
	return;
}


/*
LiCG : SYS_TIME��linux64�·����������⣬c����ʱ��
*/
//sdateΪ1970����������,objname,������;day���ؽ��ܿͻ��˵�ʱ��
//day��1970������������
void  DayCurve(int threadno,char *objname,unsigned char curFlag,int curType,int day)
{
	time_t datetime = time_t(day);
	//utc seconds to local date time
	//struct tm *when = localtime(&datetime);
	//utc seconds to utc date time
	struct tm *when = gmtime(&datetime);
	when->tm_year += 1900;
	when->tm_mon +=1;

	MESSAGE msgbuf;
	msgbuf.type = S_CURVE; 
	char *psendmsgbuf = msgbuf.messagebuf ;

	char tablename[128];
	memset(tablename,0,128);
	//TimeProc timeproc;
	//SYS_TIME sys_time = timeproc.ConvertTime(day);
	int fieldcount=0;
	int fieldcount1 =0;
	short savetime = 5;		//��������,�ۼӵ��ֵ,����ĳ���Ժ�ȱ��ʱ,ҲӦ����ֵ
	void *valbuf = NULL;
	NAME *pname = NULL;
	int ret = 0;

	int bValid = TRUE;
	if(curFlag == DAYYCCURVE)//��ң������
	{
#ifdef USE_YC_DAYTABLE
		sprintf(tablename,"yc%04d%02d%02d",when->tm_year,when->tm_mon,when->tm_year,when->tm_mday);
#else
		sprintf(tablename,"yc%04d%02d",when->tm_year,when->tm_mon);
#endif
		char con[256];
		memset(con,0,256);
		sprintf(con,"���� = '%s'",objname);
		int fdc;
		if(readRecorders(DBNAME,"ң�������","��������",con,"",TRUE,&fdc,&valbuf))
		{
			pname = (NAME *)valbuf;
			savetime = atoi(pname->name);
			FREE(pname);
		}
		else 
			bValid = FALSE;
	}
	else if(curFlag == MONTHYCCURVE)//��ң������
	{
		sprintf(tablename,"dayyc%04d%02d",when->tm_year,when->tm_mon);
	}
	else if(curFlag == DAYKWHCURVE)//�յ������
	{
		sprintf(tablename,"kwh%04d%02d",when->tm_year,when->tm_mon);
		char con[256];
		memset(con,0,256);
		sprintf(con,"���� = '%s'",objname);
		int fdc;
		if(readRecorders(DBNAME,"��Ȳ�����","��������",con,"",TRUE,&fdc,&valbuf))
		{
			pname = (NAME *)valbuf;
			savetime = atoi(pname->name);
			FREE(pname);
		}
		else 
			bValid = FALSE;
	}
	else if(curFlag == MONTHKWHCURVE)//�µ������
	{
		sprintf(tablename,"daykwh%04d%02d",when->tm_year,when->tm_mon);
	}
	else
	{
		bValid = FALSE;
	}

	if(!bValid)
	{
		psendmsgbuf[0] = 0;//��Ч����
		msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
		writemsg(threadinfo[threadno].sockid,&msgbuf);
		return;
	}

	char where[256];
	memset(where,0,256);

	if(curFlag == DAYYCCURVE || curFlag == MONTHYCCURVE || curFlag == DAYKWHCURVE || curFlag == MONTHKWHCURVE)
	{
		char fieldname[128],orderfdname[64];
		memset(fieldname,0,128);
		memset(orderfdname,0,64);

		if(curFlag == DAYYCCURVE || curFlag == DAYKWHCURVE)	//������
		{
			strcpy(fieldname,"time,data");
			strcpy(orderfdname,"time");
			if(curType>=50)
			{
				sprintf(where ,"name ='%s' and sdate = %d and flag=%d",objname,(int)day/(3600*24),curType);//�ƻ�ֵ������;
			}
			else
				sprintf(where,"name ='%s' and sdate = %d and flag<50",objname,(int)day/(3600*24));//HISDATA_REAL);
		}
		else
		{

			//������,��������µ�һ������µ�һ���sdate
			int fromdate=0,todate=0;
			fromdate = day/(3600*24) - when->tm_mday;
			struct tm newtime;
			newtime.tm_year = when->tm_year -1900;
			newtime.tm_mon = when->tm_mon - 1;
			newtime.tm_mday = 1;
			newtime.tm_hour =  when->tm_hour;
			newtime.tm_min = when->tm_min;
			newtime.tm_sec = when->tm_sec;
			newtime.tm_wday = 0;
			newtime.tm_yday = 0;
			newtime.tm_isdst = -1;
			if(newtime.tm_mon == 11)
			{
				newtime.tm_year ++;
				newtime.tm_mon = 0;
			}
			else
			{
				newtime.tm_mon ++;
			}
			time_t totime = mktime(&newtime);
			todate = totime/(60*60*24);
			strcpy(fieldname,"sdate,data");
			strcpy(orderfdname,"sdate");
			//��ʷ���ݿ���sdate�洢����1970������������
			sprintf(where,"name ='%s' and sdate > %d and sdate <= %d and flag = %d",objname,fromdate,todate,curType);
		}

		ret = readRecorders(HISDBNAME,tablename,fieldname,where,orderfdname,TRUE,&fieldcount1,&valbuf);
		if(ret>0)
		{
			pname = (NAME*)valbuf;
		}

	}
	psendmsgbuf[0] = curFlag;//�������߱�־��1Ϊ������;
	psendmsgbuf += sizeof(unsigned char);
	pro_int2char(&curType,psendmsgbuf);//�������ͱ�־����Ҫ����������//������
	psendmsgbuf += sizeof(int);
	
	pro_int2char(&day,psendmsgbuf);//������
	psendmsgbuf += sizeof(int);

	pro_short2char(&savetime,psendmsgbuf);//��������
	psendmsgbuf += sizeof(short);

	psendmsgbuf[0] = strlen(objname);//���󳤶�
	psendmsgbuf += sizeof(unsigned char);
	memcpy(psendmsgbuf,objname,strlen(objname));//�������
	psendmsgbuf += strlen(objname);
	

	short valnums ;//ֵ����
	if(ret<=0 || savetime <= 0) {ret=0;valnums=0;} //û�д���ʱ��,��������
	else
	{
		valnums = ret/fieldcount1;
	}
	
	char *pvalnumsbuf = psendmsgbuf;//����ֵ����ָ��
	psendmsgbuf += sizeof(short);

	int intval;
	int timepoint,oldtimepoint=-1;
	short count=0;
	for(int i=0;i<valnums;i++)
	{
		timepoint = (int)atof(pname[i*2].name);
		
		if(timepoint == oldtimepoint) continue;
		oldtimepoint =  timepoint;
		//if( (curFlag == DAYYCCURVE || curFlag == DAYKWHCURVE) && (timepoint % savetime != 0)) continue;//�������Ǵ���ʱ�������������ݾͷ���
		
		intval = (int)(atof(pname[i*2+1].name)*100);
		pro_int2char(&timepoint,psendmsgbuf);//ʱ���,�������Ƿ��ӣ���������sdate(1970/01/01������)
		psendmsgbuf += sizeof(int);
		pro_int2char(&intval,psendmsgbuf);//ֵ
		psendmsgbuf += sizeof(int);
		count++;
	}//for i

	pro_short2char(&count,pvalnumsbuf);

	FREE(pname);

	int calVar  = 0;  //����������
	int calType  = 0; //����������
	int val = 0;	  //��������ֵ

	//�����ߺ�Ҫ����ͳ����
	if(curFlag == DAYYCCURVE || curFlag == MONTHYCCURVE)
	{
		
		int dataType = 0; //ң������
		//�õ�ң���������,P��QҪ�㸺���ʣ�UҪ��ϸ���,����Ҫ��ƽ��ֵ
		memset(where,0,256);
		sprintf(where,"����=\'%s\'",objname);
		
		ret = readRecorders(DBNAME,YCPARATABLE,"����",where,NULL,FALSE,&fieldcount,&valbuf);
		if(ret > 0)
		{
		    calVar = 1;
		    pname = (NAME*)valbuf;
		    dataType = atoi(pname[0].name);
		    FREE(pname);
		}
	        
		if(calVar != 0 || dataType != 0)
		{
		    if(dataType == OBJTYPE_P   || dataType == OBJTYPE_Pa  ||  \
		       dataType == OBJTYPE_Pb  || dataType == OBJTYPE_Pc  || \
		       dataType == OBJTYPE_Pab || dataType == OBJTYPE_Pbc || \
		       dataType == OBJTYPE_Pca || dataType == OBJTYPE_Q   || \
		       dataType == OBJTYPE_Qa  || dataType == OBJTYPE_Qb  || \
		       dataType == OBJTYPE_Qc  || dataType == OBJTYPE_Qab || \
		       dataType == OBJTYPE_Qbc || dataType == OBJTYPE_Qca )   //��������
		     
		       calType = HISDATA_DAYS_PQRATE;	//������
		       
		     else if(dataType == OBJTYPE_V   || dataType == OBJTYPE_Va  ||  \
		       	     dataType == OBJTYPE_Vb  || dataType == OBJTYPE_Vc  || \
		             dataType == OBJTYPE_Vab || dataType == OBJTYPE_Vbc || \
		             dataType == OBJTYPE_Vca || dataType == OBJTYPE_V0   || \
		             dataType == OBJTYPE_V1  || dataType == OBJTYPE_V2 )	//��ѹ����

		       calType = HISDATA_DAYS_HGL;	//�ϸ���
		     else 
		     	calType = HISDATA_DAYS_AVERAGE;	//ƽ��ֵ	     		     
		    	
			if(curFlag == MONTHYCCURVE && curType == HISDATA_DAYS_PQRATE)	//�¸�������������ƽ��ֵ
			{
				calType = HISDATA_DAYS_AVERAGE;
			}
		    memset(where,0,256);
		    if(curFlag == DAYYCCURVE)
				sprintf(where,"name='%s' and sdate = %d and flag = %d ",objname,(int)day/(3600*24),calType);
			else
				sprintf(where,"name='%s' and  flag = %d ",objname,calType);

			sprintf(tablename,"dayyc%04d%02d",when->tm_year,when->tm_mon);
 			ret = readRecorders(HISDBNAME,tablename,"data",where,NULL,FALSE,&fieldcount,&valbuf);
 			if(ret > 0)
 			{
 			    pname = (NAME*)valbuf;
			    val = int(atof(pname[0].name) * 1000);
			    FREE(pname);
 			}
		}
	}
		
	pro_int2char(&calVar,psendmsgbuf);//����������
	psendmsgbuf += sizeof(int);
	pro_int2char(&calType,psendmsgbuf);//����������
	psendmsgbuf += sizeof(int);
	
	pro_int2char(&val,psendmsgbuf);//��������ֵ
	psendmsgbuf += sizeof(int);

	msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
	writemsg(threadinfo[threadno].sockid,&msgbuf);

}


//ȡʵʱ��������
void getSendRealCurveData(int threadno)
{
	void *valbuf = NULL;
	NAME *pname = NULL;
	int ret = 0;
	int fieldcount;


	THREADMANAGE *pthread = threadinfo + threadno;
	time_t now;
	time( &now );
	struct tm *when = localtime( &now );
	for(int i = 0; i < pthread->realcurvecount ; i++)
	{
		MESSAGE msgbuf;
		msgbuf.type = S_REALCURVE;
		char *psendmsgbuf = msgbuf.messagebuf ;
		REALCURVE *p = pthread->prealcurve + i;
		if(p->savetime >0 && now - p->lastSendTime > p->savetime*60)//����ʱ��Ҫ����0,������Ҫ���ڴ���ʱ��
		{
			p->lastSendTime = now;
			ret = readRecorders(HISDBNAME,p->hisTable,p->selfname,p->where,p->orderfname,TRUE,&fieldcount,&valbuf);
			if(ret > 0)
			{
				pname = (NAME*)valbuf;
				psendmsgbuf = msgbuf.messagebuf;

				psendmsgbuf[0] = p->curStructNo;//���߽ṹ���;
				psendmsgbuf += sizeof(unsigned char);
				psendmsgbuf[0] = p->cur_no ;//���ߺ�
				psendmsgbuf += sizeof(unsigned char);
				psendmsgbuf[0] = p->cur_type ;//��������
				psendmsgbuf += sizeof(unsigned char);

				pro_short2char(&p->savetime,psendmsgbuf);//��������
				psendmsgbuf += sizeof(short);
				
				psendmsgbuf[0] = strlen(p->objName);//����������
				psendmsgbuf += sizeof(unsigned char);
				strcpy(psendmsgbuf,p->objName);
				psendmsgbuf += strlen(p->objName);//������

				short valnums;//ֵ����
				valnums = ret/fieldcount;

				char *pvalnumsbuf = psendmsgbuf;//����ֵ����ָ��
				psendmsgbuf += sizeof(short);
				
				int intval;
				int timepoint,oldtimepoint=-1;
				short count=0;
				for(int i=0;i<valnums;i++)
				{
					timepoint = (int)atof(pname[i*2].name);
					
					if(timepoint == oldtimepoint) continue;
					oldtimepoint =  timepoint;
					
					intval = (int)(atof(pname[i*2+1].name)*1000);
					pro_int2char(&timepoint,psendmsgbuf);//ʱ���,�������Ƿ��ӣ���������sdate(1970/01/01������)
					psendmsgbuf += sizeof(int);
					pro_int2char(&intval,psendmsgbuf);//ֵ
					psendmsgbuf += sizeof(int);
					count++;
				}//for i
				pro_short2char(&count,pvalnumsbuf);
				FREE(pname);
				msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
				writemsg(threadinfo[threadno].sockid,&msgbuf);
			}
		}		
	}
}




//ʵʱ����Ԥ����
void  RealCurve(int threadno)
{
	THREADMANAGE *pthread = threadinfo + threadno;
	time_t now;
	time( &now );
	struct tm *when = localtime(&now);
	when->tm_year += 1900;
	when->tm_mon += 1;
	for(int i = 0; i < pthread->realcurvecount ; i++)
	{
		REALCURVE *p = pthread->prealcurve + i;
		char con[256];
		memset(con,0,256);
		sprintf(con,"���� = '%s'",p->objName);
		int fdc;
		void *valbuf = NULL;
		if(p->cur_type == REALYCCURVE)
		{
#ifdef USE_YC_DAYTABLE
			sprintf(p->hisTable,"yc%04d%02d%02d",when->tm_year,when->tm_mon,when->tm_mday);
#else
			sprintf(p->hisTable,"yc%04d%02d",when->tm_year,when->tm_mon);
#endif
			if(readRecorders(DBNAME,"ң�������","��������",con,"",TRUE,&fdc,&valbuf)>0)
			{
				NAME *pname = (NAME *)valbuf;
				p->savetime = (short)atoi(pname->name);
				strcpy(p->selfname,"time,data");
				strcpy(p->orderfname,"time");
				FREE(pname);
			}
			else
				continue;
		}
		else
		{
			sprintf(p->hisTable,"kwh%04d%02d",when->tm_year,when->tm_mon);
			if(readRecorders(DBNAME,"��Ȳ�����","��������",con,"",TRUE,&fdc,&valbuf)>0)
			{
				NAME *pname = (NAME *)valbuf;
				p->savetime = (short)atoi(pname->name);
				strcpy(p->selfname,"time,data");
				strcpy(p->orderfname,"time");
				FREE(pname);
			}
			else
				continue;
		}
	//	memset(p->where,0,128);
		sprintf(p->where,"name ='%s' and sdate = %d and flag<50",p->objName,(int)now/(3600*24));//HISDATA_REAL);
	}
	getSendRealCurveData(threadno);
}


#define AnalogID	0
#define StationID	1
#define RtuSn		2
void readAnalogView(int threadno,int paramClassify,char* param)
{
	int fieldcount;
	void *valbuf = NULL;
	char where[256];
	memset(where,0,256);
	if(paramClassify == AnalogID)
	{
		sprintf(where,"����='%s'",param);
	}
	else
	{
		printf("readAnalogView ��ʱδ����Ĳ������ \n");
		return;
	}
	int ret = readRecorders(DBNAME,"ģ������ͼ","����,����,վ����ϵ����,�豸����,�ն����",where,"����",FALSE,&fieldcount,&valbuf);
	
	MESSAGE msgbuf;
	msgbuf.type = S_ANALOGVIEW;
	char *buf = msgbuf.messagebuf ;
	short recCount = ret/fieldcount;
	
	pro_short2char(&recCount,buf);//��¼����
	buf += sizeof(short);
	
	NAME *pname = NULL;
	pname = (NAME*)valbuf;
	valbuf = NULL;
	unsigned char len = 0;
	for(int i=0;i<recCount;i++)
	{
		len = strlen(pname[i*fieldcount].name); //����
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount].name,len);//�ַ��� 
		buf += len;

		len = strlen(pname[i*fieldcount+1].name); //����
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+1].name,len);//�ַ��� 
		buf += len;

		len = strlen(pname[i*fieldcount+2].name); //����
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+2].name,len);//�ַ��� 
		buf += len;

		len = strlen(pname[i*fieldcount+3].name); //����
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+3].name,len);//�ַ��� 
		buf += len;

		int rtusn=atoi(pname[i*fieldcount+4].name);  //�ն����
		pro_int2char(&rtusn,buf);
		buf += sizeof(int);
	}
	msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
	printf("����ģ������ͼ���� \n");
	writemsg(threadinfo[threadno].sockid,&msgbuf);
	FREE(pname);
}


//lcg 2011-07-21�޸ķ��ظ��ͻ����ն˲��������ݣ����ڷ���Ȩ�޵�ʵ�ֺͿ��ơ�
//groupnoĿǰ��ʱ��ʹ�á�
void readRtuPara(int threadno,int groupno)
{
	int fieldcount;
	void *valbuf = NULL;
	char where[256];
	memset(where,0,256);
	sprintf(where,"ʹ�ñ�־=1");
	int ret = readRecorders(DBNAME,"�ն˲�����","�������,�ն����,����,����,��ȫ��,վ����ϵ����",where,"�ն����",FALSE,&fieldcount,&valbuf);
	
	MESSAGE msgbuf;
	msgbuf.type = S_RTUNAME;
	char *buf = msgbuf.messagebuf ;
	short rtucount = ret/fieldcount;
	
	pro_short2char(&rtucount,buf);//RTU����
	buf += sizeof(short);
	
	NAME *pname = NULL;
	pname = (NAME*)valbuf;
	valbuf = NULL;
	unsigned char len = 0;
	for(int i=0;i<rtucount;i++)
	{
		int rtugroupno = atoi(pname[i*fieldcount].name );//�����
		pro_int2char(&rtugroupno,buf);
		buf += sizeof(int);
		
		int rtuid=atoi(pname[i*fieldcount+1].name );    //�ն����
		pro_int2char(&rtuid,buf);
		buf += sizeof(int);

		len = strlen(pname[i*fieldcount+2].name); //���볤��
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+2].name,len);//����
		buf += len;
		
		len = strlen(pname[i*fieldcount+3].name); //��������
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+3].name,len);//����
		buf += len;

		int safelev=atoi(pname[i*fieldcount+4].name );  //��ȫ��
		pro_int2char(&safelev,buf);
		buf += sizeof(int);

		len = strlen(pname[i*fieldcount+5].name); //վ����ϵ���볤��
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+5].name,len);//վ����ϵ����
		buf += len;
	}
	msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
	printf("�����ն˲��������\n");
	writemsg(threadinfo[threadno].sockid,&msgbuf);
	FREE(pname);
}

//����վ����ϵ������,group
void  readRTU(int threadno,int groupno)
{
	int fieldcount;
	void *valbuf = NULL;
	char where[256];
	memset(where,0,256);
//	if(groupno != 0xfff)//ȫ�������վ����ϵ
//	{
	sprintf(where,"���� in(select վ����ϵ���� from �ն˲����� where ʹ�ñ�־=1)");
//	}
//	printf("%s\n","�������=%d and ���� in(select վ����ϵ���� from �ն˲����� where ʹ�ñ�־=1)");
	int ret = readRecorders(DBNAME,"վ����ϵ������","����,����",where,"����",FALSE,&fieldcount,&valbuf);
		
	if(ret>0) {
		MESSAGE msgbuf;
		msgbuf.type = S_STATION;
		char *buf = msgbuf.messagebuf ;
		short rtucount = ret/fieldcount;
		
		pro_short2char(&rtucount,buf);//RTU����
		buf += sizeof(short);
		
		NAME *pname = NULL;
		pname = (NAME*)valbuf;
		valbuf = NULL;
		unsigned char len = 0;
		for(int i=0;i<rtucount;i++)
		{
// 			int rtugroupno = atoi(pname[i*fieldcount].name );//�����
// 			pro_int2char(&rtugroupno,buf);
// 			buf += sizeof(int);
			
			len = strlen(pname[i*fieldcount].name); //���볤��
			buf[0] = len;
			buf += sizeof(unsigned char);
			memcpy(buf,pname[i*fieldcount].name,len);//����
			buf += len;

			len = strlen(pname[i*fieldcount+1].name); //��������
			buf[0] = len;
			buf += sizeof(unsigned char);
			memcpy(buf,pname[i*fieldcount+1].name,len);//����
			buf += len;
		}
		msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
		printf("����վ����ϵ����\n");
		writemsg(threadinfo[threadno].sockid,&msgbuf);
		FREE(pname);
	}
}


//����ң�����,���͵�ȴ���
//20110721 lcg�޸Ķ�ȡ�ն˲������ö�Ӧ��ң�����
void  readYCCode(int threadno,char *tablename,char *rtucode)
{
	int fieldcount;
	void *valbuf = NULL;
	char where[256];
	memset(where,0,256);
	
/*
	//WEB���߲鿴�����й��������ն�
	//sprintf(where,"վ����ϵ����=\'%s\'",rtucode);
	sprintf(where,"��ʾ��־=1 and �ն���� in (select �ն���� from �ն˲����� where ʹ�ñ�־=1 and վ����ϵ����=\'%s\')",rtucode);
	printf("%s\n",where);

	int ret = readRecorders(DBNAME,tablename,"����,����",where,"����",FALSE,&fieldcount,&valbuf);
*/
	
	sprintf(where,"��ʾ��־=1 and �ն����=%s",rtucode);
	printf("%s\n",where);
	
	int ret = readRecorders(DBNAME,tablename,"����,����,��ȫ��",where,"����",FALSE,&fieldcount,&valbuf);

	if(ret<0) fieldcount = 2;	
	MESSAGE msgbuf;
	msgbuf.type = S_YCCODE;
	char *buf = msgbuf.messagebuf ;
	short rtucount = ret/fieldcount;
	
	if(ret <= 0) //��վû��ң��
	{
		pro_short2char(&rtucount,buf);
		buf += sizeof(short);
		msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
		printf("û��\"%s\"��ң�����\n",rtucode);
		writemsg(threadinfo[threadno].sockid,&msgbuf);
		return;
	}
	pro_short2char(&rtucount,buf);//�������
	buf += sizeof(short);

	unsigned char len = 0;
	
	len = (unsigned char)strlen(rtucode);
	buf[0] = len;//��վ���볤��
	buf += sizeof(unsigned char);
	memcpy(buf,rtucode,len);//��վ����
	buf += len;
	
	NAME *pname = NULL;
	pname = (NAME*)valbuf;
	valbuf = NULL;
	for(int i=0;i<rtucount;i++)
	{
		len = strlen(pname[i*fieldcount].name); //���볤��
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount].name,len);//����
		buf += len;

		len = strlen(pname[i*fieldcount+1].name); //��������
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+1].name,len);//����
		buf += len;

		int safelev=atoi(pname[i*fieldcount+2].name );  //��ȫ��
		pro_int2char(&safelev,buf);
		buf += sizeof(int);
	}
	msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
	printf("����\"%s\"��ң�����\n",rtucode);
	writemsg(threadinfo[threadno].sockid,&msgbuf);
	FREE(pname);
}


//bwait,��ֹͣˢ����ʱ���Ƿ���Ҫ���߳�Sleep
void removeStream(THREADMANAGE *threadinfo1,int winno,int threadno,int bwait = FALSE)
{
	//����ע����
	if(threadinfo1->streamcount <= 0) return;
	if(winno == 0xff)//ע���ÿͻ��˵�ȫ����
	{
		#ifdef WIN32
			EnterCriticalSection(&threadinfo1->critThread);
		#endif

		MULTISTREAM *pmultistream = threadinfo1->pmultistream;
		MULTISTREAM *poldmultistream = pmultistream;
		for(int i = 0; i < threadinfo1->streamcount; i++)
		{
			ASSERT(pmultistream != NULL);

			threadinfo1->prdb_quickstream->RdbStopFresh( BASEWINNO * threadno + threadinfo1->pmultistream->winno);
			#ifndef WIN32
				delay_t(WAIT_TIME);
			#endif
			pmultistream = pmultistream->plist ;
			
			delete poldmultistream;
			poldmultistream = NULL;

			poldmultistream = pmultistream;
			threadinfo1->streamcount--;
		}
		threadinfo1->pmultistream = NULL;
		threadinfo1->streamcount = 0;
		delete threadinfo1->prdb_quickstream ; threadinfo1->prdb_quickstream = NULL;
		#ifdef WIN32
			LeaveCriticalSection(&threadinfo1->critThread);
		#endif

	}
	else //ע��������
	{
		#ifdef WIN32
			EnterCriticalSection(&threadinfo1->critThread);
		#endif
		MULTISTREAM *pmultistream = threadinfo1->pmultistream;
		MULTISTREAM *poldmultistream = pmultistream;
		for(int i=0;i<threadinfo1->streamcount;i++)
		{
			ASSERT(pmultistream != NULL);

			if(pmultistream->winno == winno)
			{				

				threadinfo1->prdb_quickstream->RdbStopFresh(BASEWINNO * threadno + winno);
				
				#ifndef WIN32
					delay_t(WAIT_TIME);
				#endif

				pmultistream = pmultistream->plist;
				delete poldmultistream; poldmultistream = NULL;
				
				poldmultistream = pmultistream;
				threadinfo1->streamcount--;
				break;
			}
		}
		if(threadinfo1->streamcount == 0)
		{
			delete threadinfo1->prdb_quickstream ; threadinfo1->prdb_quickstream = NULL;
			threadinfo1->pmultistream = NULL;
		}
		#ifdef WIN32
			LeaveCriticalSection(&threadinfo1->critThread);
		#endif
	}
}

#ifdef WIN32
	void acceptStream(int threadno,HWND hwnd,BOOL bwait=false)
#else 
	void acceptStream(int threadno,int bwait=FALSE)
#endif
{
	THREADMANAGE *threadinfo1 = threadinfo + threadno;
	ASSERT(threadinfo1 != NULL);
	char *pbuf = threadinfo1->prcvdata->messagebuf;
	ASSERT(pbuf != NULL);

	int paranums;//��Ч�����
	pro_char2int((char *)(pbuf),&paranums);
	pbuf += sizeof(int);
	

	int winno;////Winno���ں�
	pro_char2int((char *)(pbuf),&winno);
	pbuf += sizeof(int);

	//ע��������
	if(winno == 0xff)
	{
		//printf("�յ�ע������Ϣ!\n");
		removeStream(threadinfo1,winno,threadno,bwait);
		return;
	}
	else if(winno<0)
	{
		removeStream(threadinfo1,abs(winno),threadno,bwait);
		return;
	}

	if(paranums<=0) 
	{
		MESSAGE *msg = threadinfo1->psnddata;
		msg->type = S_STREAM;
		msg->length = sizeof(int) * 2;
		paranums = 0;
		pro_int2char(&paranums, (char *)(msg->messagebuf));//��Ч�����
		pro_int2char(&winno, (char *)(msg->messagebuf + sizeof(int)));//Winno���ں�
		writemsg(threadinfo1->sockid, msg);
		return;
	}//������Ч��������Ե�ʧ����Ϣ

	int freshperiod;//Freshperiodˢ������
	pro_char2int((char *)(pbuf), &freshperiod);
	pbuf += sizeof(int);
	
	char username[32],password[32];
	memset(username,0,32);
	memset(password,0,32);
	strncpy(username,USERNAME,strlen(USERNAME));
	strncpy(password,PASSWORD,strlen(PASSWORD));
	
	RDB_FIELD_STRU *fldaddr = (RDB_FIELD_STRU *)calloc(1,sizeof(RDB_FIELD_STRU)*paranums);
	ASSERT(fldaddr);

	unsigned char len = 0;
    
	int *oldorder = (int*)calloc(1,sizeof(int)*paranums);

	NetStatus netstatus;
	int grooupnum = netstatus.GetHostGrpNum();
	int *grpEnm = netstatus.GetHostGrpEnm();
    int validnum = 0;//��Ч�����
   
	int i;
	for( i = 0; i < paranums; i++)
	{ 

		int j = 0;
		int groupno=0;
		pro_char2int(pbuf,&groupno);//���
		pbuf += sizeof(int);

		for(j = 0; j < grooupnum; j++)
		{
			if(groupno == grpEnm[j] && (netstatus.IsMirrorServer(groupno,SCD_SERVER) || netstatus.IsServer(groupno,SCD_SERVER)))
				break;
		}

		if(j < grooupnum)
		{
			oldorder[validnum] = i;
			fldaddr[validnum].groupno = groupno;
			
			len = pbuf[0];//tablename�ĳ���
			pbuf += sizeof(unsigned char);
            if(len > RDB_TABNAME_LEN) len = RDB_TABNAME_LEN;
			memcpy(fldaddr[validnum].tabname,pbuf,len);//tablename������
			pbuf += len;

			len = pbuf[0];//objname�ĳ���
			pbuf += sizeof(unsigned char);
			if(len > RDB_OBJNAME_LEN) len = RDB_OBJNAME_LEN;
			memcpy(fldaddr[validnum].objname,pbuf,len);//objname������
			pbuf += len;

			len = pbuf[0];//fldname�ĳ���
			pbuf += sizeof(unsigned char);
			if(len > RDB_COLNAME_LEN) len = RDB_COLNAME_LEN;
			memcpy(fldaddr[validnum].fldname,pbuf,len);//fldname������
			pbuf += len;
			validnum++;
		}
		else
		{
			int invalidno = paranums-(i-validnum)-1;
			oldorder[invalidno] = i;
			fldaddr[invalidno].groupno = groupno;
			
			len = pbuf[0];//tablename�ĳ���
			pbuf += sizeof(unsigned char);
			if(len > RDB_TABNAME_LEN) len = RDB_TABNAME_LEN;
			memcpy(fldaddr[invalidno].tabname,pbuf,len);//tablename������
			pbuf += len;

			len = pbuf[0];//objname�ĳ���
			pbuf += sizeof(unsigned char);
			if(len > RDB_OBJNAME_LEN) len = RDB_OBJNAME_LEN;
			memcpy(fldaddr[invalidno].objname,pbuf,len);//objname������
			pbuf += len;

			len = pbuf[0];//fldname�ĳ���
			pbuf += sizeof(unsigned char);
			if(len > RDB_COLNAME_LEN) len = RDB_COLNAME_LEN;
			memcpy(fldaddr[invalidno].fldname,pbuf,len);//fldname������
			pbuf += len;
		}
		if(abs((int)(pbuf - threadinfo1->prcvdata->messagebuf )) > (MAXBUFSIZE - sizeof(RDB_FIELD_STRU))) //�п����ڴ����
		{
			paranums = i + 1;
			break;
		}
	}
	MULTISTREAM *pmultistream = threadinfo1->pmultistream;
	MULTISTREAM *poldmultistream = pmultistream;
	i=0;
	if(threadinfo1->streamcount>0)//��������
	{
		for(i=0;i<threadinfo1->streamcount;i++)
		{
			ASSERT(pmultistream != NULL );
			if(pmultistream->winno == winno)//���ںź���������ͬ
			{
				#ifdef WIN32
					EnterCriticalSection(&threadinfo1->critThread );
				#endif

				threadinfo1->prdb_quickstream->RdbStopFresh(BASEWINNO*threadno + winno);				
				#ifndef WIN32
					delay_t(WAIT_TIME);
				#endif

				while(pmultistream->pthreadstreamdata)
				{
					delete pmultistream->pthreadstreamdata ;//ɾ�����������
					pmultistream->pthreadstreamdata = NULL;
					break;
				}

				pmultistream->pthreadstreamdata = new CThreadStreamData(paranums);
				ASSERT(pmultistream->pthreadstreamdata != NULL );
				memcpy(pmultistream->pthreadstreamdata->pold_order,oldorder,sizeof(int)*paranums);
				#ifdef WIN32
					LeaveCriticalSection(&threadinfo1->critThread );
				#endif
				break;
			}
			poldmultistream = pmultistream;
			pmultistream = pmultistream->plist ;
		}
	}
	
	if(i >= threadinfo1->streamcount || threadinfo1->streamcount == 0)//û���ҵ���ͬwinno����,��û��������
	{
		pmultistream = new MULTISTREAM();
		ASSERT(pmultistream);

		pmultistream->plist = NULL;
		pmultistream->winno = winno;
		pmultistream->pthreadstreamdata = new CThreadStreamData(paranums);
		ASSERT(pmultistream->pthreadstreamdata);

		memcpy(pmultistream->pthreadstreamdata->pold_order,oldorder,sizeof(int)*paranums);
		if(threadinfo1->streamcount == 0)
		{
			threadinfo1->prdb_quickstream = new Rdb_QuickStream();
			ASSERT(threadinfo1->prdb_quickstream);

			threadinfo1->pmultistream = pmultistream;
		}
		else
		{
			poldmultistream->plist = pmultistream;
		}
		threadinfo1->streamcount++;
	}
#ifdef WIN32
	int setresult = threadinfo1->prdb_quickstream->RdbSetStreamPara(username,password,freshperiod,validnum,fldaddr,BASEWINNO * threadno + winno,hwnd);//���ں���threadnoȡ������threadnoΨһ,�Ա�ӳ��.
#else
	sigset(SIGUSR1,recvStream);
	int setresult = threadinfo1->prdb_quickstream->RdbSetStreamPara(username,password,freshperiod,validnum,fldaddr,BASEWINNO * threadno + winno);//���ں���threadnoȡ������threadnoΨһ,�Ա�ӳ��.
	printf("ע������Ϣ,����%d,ע�᷵�ؽ��%d\n",validnum,setresult);
#endif
	if(setresult != RDB_OK)
	{//��ע�������Ϣ
		MESSAGE *msg = threadinfo1->psnddata;
		msg->type = S_STREAM;
		msg->length = sizeof(int)*2;
		paranums = -1;
		pro_int2char(&paranums,(char *)(msg->messagebuf));//��Ч�����
		pro_int2char(&winno,(char *)(msg->messagebuf+sizeof(int)));//Winno���ں�
		writemsg(threadinfo1->sockid,msg);

		removeStream(threadinfo1,abs(winno),threadno,FALSE);
	}
	FREE(oldorder);
	FREE(fldaddr);

	return;
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��sendafile()
 * ��������������һ���ļ�
 * ����    ��sockid=�ͻ�socket��;env,�������������治��"\"��path,�ļ������·����ǰ�󶼲��ܴ�\,filenamepara�ļ���
 *			 type,���͵��ļ�����.	sendwrongf,�Ƿ��ʹ�����Ϣ
 * ����ֵ��  -1,���ļ�ʧ��;
 *			 -2,���ļ����󣻳ɹ�ʱ�����ط��͵��ļ��ĳ���
 *           -3,���ʹ���
 *
 * --------------------------------------------------
 */
int sendafile(int threadid,SOCKET sockid,char *env,char* path,char* filenamepara,int type,int sendwrongf = FALSE,int bupdatetable=FALSE)
{

	if(path == NULL || path[0] == '\0') return 0;
	char fn[256];
	memset(fn,0,256); 

	if(filenamepara == NULL && filenamepara[0] == '\0') return 0;
	strcpy(fn,env);
	strcat(fn,"/");
	strcat(fn,path);//�ļ������·��
	strcat(fn,"/");
	strcat(fn,filenamepara);
	printf("�����ļ�%s��%s\n",fn,threadinfo[threadid].client_addr);
	FStat filestat;
	int fileexist = filestat.isExist(fn);
	int length = 0;
	short fnlength = strlen(filenamepara);
	//							��ʶ         �ļ�·������    �ļ�·��        �ļ�������(n)  + �ļ���
	int prefixlen = MSGHEADLEN + sizeof(unsigned char) + sizeof(short) +strlen(path) + sizeof(short) + fnlength;
	int rtn = 0;
	if(fileexist)
	{
		int openf = filestat.openfile();
		if(openf == TRUE)
		{
			long filelen = filestat.getFileLen();
			if(filelen > 0)
			{
				char *pfilebuf = (char*)malloc(filelen + prefixlen);
				memset(pfilebuf,0,filelen + prefixlen);
				char *pbuf = pfilebuf;
				int realcount = filestat.read(pfilebuf+prefixlen,filelen);
				if(realcount!=filelen)
				{
					rtn = -2;//���ļ�����!
				}
				else//�����ļ�
				{
					length = filelen + prefixlen -  MSGHEADLEN;//���ĳ���
					pro_int2char(&type,pbuf);//����
					pbuf += sizeof(int);
					pro_int2char(&length,pbuf);//����
					pbuf += sizeof(int);
					pbuf += sizeof(int);//��ͷ����

					pbuf[0] = 0;//��ʶ
					pbuf += sizeof(unsigned char);

					short pathlen = (short)strlen(path);//�ļ�·������
					pro_short2char(&pathlen,pbuf);
					pbuf += sizeof(short);

					strcpy(pbuf,path);//�ļ�·��
					pbuf += pathlen;

					pro_short2char(&fnlength,pbuf);//�ļ�������
					pbuf += sizeof(short);

					memcpy(pbuf,filenamepara,fnlength);//�ļ���

					if(send_data(sockid,pfilebuf,filelen + prefixlen) == -1)
					{
						rtn = -3;//���ʹ���
					}
					else
					{
						rtn = filelen;
						//����Զ���Ҫ���ֿͻ����ļ���һ��
						char sql[MAXSQLWORDLEN];
						memset(sql,0,MAXSQLWORDLEN);
						//IP��ַ        char(40)        not null,
						//Ŀ¼			char(128)       not null,
						//�ļ���		char(128)       not null,
						//������		int             not null,
						//ʱ����		int				not null
						int yearmonthday,hourminsecond;
						filestat.getmtime(yearmonthday,hourminsecond);

						if(bupdatetable)
						{
							sprintf(sql,"delete %s.%s%s where ip��ַ='%s' and Ŀ¼='%s' and �ļ���='%s'",
								DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,filenamepara);
							Execute(DBNAME,sql);

							sprintf(sql,"delete %s.%s%s where ip��ַ='%s' and Ŀ¼='%s' and �ļ���='%s'",
								DBNAME,DBO,WEBVISITTABLE,threadinfo[threadid].client_addr,path,filenamepara);
							Execute(DBNAME,sql);
							memset(sql,0,MAXSQLWORDLEN);
						}
						sprintf(sql,"insert into %s.%s%s values('%s','%s','%s',%d,%d)",
							 DBNAME,DBO,WEBVISITTABLE,threadinfo[threadid].client_addr,path,filenamepara,yearmonthday,hourminsecond);
						Execute(DBNAME,sql);
					//	_delay_time(2000*1000);
					//	sprintf(sql,"delete from %s.%s%s where ip��ַ='%s' and Ŀ¼='%s' and �ļ���='%s'",
						//		DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,filenamepara);
					//	Execute(DBNAME,sql);
					}
				}
				FREE(pfilebuf);
			}
			filestat.close();
			return rtn;
		}
	}
	rtn = -1;//���ļ�ʧ��

	if(sendwrongf && rtn<0)
	{
		//�ͻ���������ĳһ�ļ�ʱ��Ҫ��������Ϣ
		char *pbuf = (char*)malloc(prefixlen*sizeof(char));
		memset(pbuf,0,prefixlen*sizeof(char));
		char *pbuf1 = pbuf;
		length = prefixlen -  MSGHEADLEN;//���ĳ���
		pro_int2char(&type,pbuf1);//����
		pbuf1 += sizeof(int);
		pro_int2char(&length,pbuf1);//����
		pbuf1 += sizeof(int);
		pbuf1 += sizeof(int);//��ͷ����

		pbuf1[0] = rtn;//��ʶ
		pbuf1 += sizeof(unsigned char);

		short pathlen = (short)strlen(path);//�ļ�·������
		pro_short2char(&pathlen,pbuf1);
		pbuf1 += sizeof(short);

		strcpy(pbuf1,path);//�ļ�·��
		pbuf1 += pathlen;

		pro_short2char(&fnlength,pbuf1);//�ļ�������
		pbuf1 += sizeof(short);

		memcpy(pbuf1,filenamepara,fnlength);//�ļ���

		send_data(sockid,pbuf,prefixlen);
		FREE(pbuf);
	}
	return rtn;
}


#ifndef WIN32
void addPathList(DIRLIST **dirlist,char *pathname)
{
	if(*dirlist == NULL)
	{
		*dirlist = new DIRLIST;
		(*dirlist)->dirlist = NULL;
		memset((*dirlist)->path,0,256);
		strcpy((*dirlist)->path,pathname);
		(*dirlist)->bsendf = FALSE;
	}
	else
	{
		DIRLIST *pdirlisthead = *dirlist;
		DIRLIST *pdirlistend = (*dirlist)->dirlist;
		while(pdirlistend)
		{
			pdirlisthead = pdirlistend;
			pdirlistend = pdirlistend->dirlist;
		}
		pdirlisthead->dirlist = new DIRLIST;
		pdirlisthead->dirlist->dirlist = NULL;
		memset(pdirlisthead->dirlist->path,0,256);
		strcpy(pdirlisthead->dirlist->path,pathname);
		pdirlisthead->dirlist->bsendf = FALSE;
	}
	return;
}


void dealDirSendFile(DIRLIST **dirlist,char *filen,FStat fstat,char *path,char *env,int threadid,int sockid)
{
	DIR *dirp = opendir(filen);
	struct dirent *dp;
	while (dirp) {
	 errno = 0;
	 if ((dp = readdir(dirp)) != NULL) {
		 char filetmp[256];
		 memset(filetmp,0,256);
		 strcpy(filetmp,filen);
		 strcat(filetmp,"/");
		 strcat(filetmp,dp->d_name);
		 if(fstat.isExist(filetmp))
		 {
			 if(fstat.isDir())//��Ŀ¼
			 {
				 if(specifyDir(dp->d_name))
				 {
					 char pathtmp[256];
					 memset(pathtmp,0,256);
					 strcpy(pathtmp,path);
					 strcat(pathtmp,"/");
					 strcat(pathtmp,dp->d_name);
					 addPathList(dirlist,pathtmp);
				 }
			 }
			 else
			 {
				 sendafile(threadid,sockid,env,path,dp->d_name,S_ALLFILE,FALSE);
			 }
		 }//if(fstat.isExist(filetmp))
	 }//if ((dp = readdir(dirp)) != NULL) 
	 else
	 {
		if (errno == 0) {
			 closedir(dirp);
			 return ;
		 }//if (errno == 0) 
		 closedir(dirp);
		 return ;
		}//else
	}//while
	return;
}




void dealDir(DIRLIST **dirlist,FILELIST **ppfilelist,FILELIST **ppfilelist1,char *filen,FStat fstat,char *path,char *env)
{
	DIR *dirp = opendir(filen);
	struct dirent *dp;
	while (dirp) {
	 errno = 0;
	 if ((dp = readdir(dirp)) != NULL) {
		 char filetmp[256];
		 memset(filetmp,0,256);
		 strcpy(filetmp,filen);
		 strcat(filetmp,"/");
		 strcat(filetmp,dp->d_name);
		 if(fstat.isExist(filetmp))
		 {
			 if(fstat.isDir())//��Ŀ¼
			 {
				 if(specifyDir(dp->d_name))
				 {
					 char pathtmp[256];
					 memset(pathtmp,0,256);
					 strcpy(pathtmp,path);
					 strcat(pathtmp,"/");
					 strcat(pathtmp,dp->d_name);
					 addPathList(dirlist,pathtmp);
				 }
			 }
			 else
			 {
				 if(*ppfilelist == NULL)
				 {
					 *ppfilelist = new FILELIST();
					 *ppfilelist1 = *ppfilelist;
					 strcpy((*ppfilelist)->filename,dp->d_name);
					 strcpy((*ppfilelist)->path,path);
					 fstat.getmtime((*ppfilelist)->yearmonday,(*ppfilelist)->hourminsec);					 
				 }
				 else
				 {
					 FILELIST *p = *ppfilelist;
					 while(p->pfilelist != NULL)
					 {
						 p = p->pfilelist ;
					 }
					p->pfilelist = new FILELIST();
					strcpy(p->pfilelist->path,path);
					strcpy(p->pfilelist->filename,dp->d_name);
					fstat.getmtime(p->pfilelist->yearmonday,p->pfilelist->hourminsec);
					
				}
			 }
		 }//if(fstat.isExist(filetmp))
	 }//if ((dp = readdir(dirp)) != NULL) 
	 else
	 {
		if (errno == 0) {
			 closedir(dirp);
			 return ;
		 }//if (errno == 0) 
		 closedir(dirp);
		 return ;
		}//else
	}//while
	return;
}

#endif

/*	����˵��
 * --------------------------------------------------
 * ������  ��sendfile()
 * ��������������ĳһĿ¼�µ������ļ�
 * ����    ��sockid=�ͻ�socket��;
 *           env,���л�������;path,�ļ������·������󶼲��ܴ�\
 *			 filenamepara,����·�����ļ���
 * ����ֵ��  -1,���ļ�ʧ��;
 *			 -2,���ļ����󣻳ɹ�ʱ�����ط��͵��ļ��ĳ���
 *			 
 *           
 *
 * --------------------------------------------------
 */
int sendfile(int threadid,SOCKET sockid,char *env,char *path,char *filenamepara)
{
#ifdef WIN32
	WIN32_FIND_DATA FileData; 
	
	HANDLE hSearch = NULL; 
	BOOL fFinished = FALSE; 
	
	//ȡ�ô�ȫ·�����ļ���
	char filen[256];
	memset(filen,0,256);
	strcpy(filen,env);
	strcat(filen,"/");
	strcat(filen,path);
	strcat(filen,"/");
	strcat(filen,filenamepara);

	hSearch = FindFirstFile(filen, &FileData); 
	if(hSearch == INVALID_HANDLE_VALUE) 
	{ 
		return 0;
	}
	else
	{
		if((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)//��Ŀ¼�ļ�
		{
			if(specifyDir(FileData.cFileName))
			{
				char path1[256];
				memset(path1,0,256);
				strcpy(path1,path);
				strcat(path1,"/");
				strcat(path1,FileData.cFileName);
				sendfile(threadid,sockid,env,path1,"*");//�ݹ����Ŀ¼�����ļ�����
			}
		}
		else
		{
			sendafile(threadid,sockid,env,path,FileData.cFileName,S_ALLFILE,false);
		}
	}

	while (!fFinished) 
	{ 
		if (!FindNextFile(hSearch, &FileData)) 
		{
			fFinished = TRUE;
		}
		else
		{
			if((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)//��Ŀ¼�ļ�
			{
				if(specifyDir(FileData.cFileName))
				{
					char path1[256];
					memset(path1,0,256);
					strcpy(path1,path);
					strcat(path1,"/");
					strcat(path1,FileData.cFileName);
					sendfile(threadid,sockid,env,path1,"*");//�ݹ����Ŀ¼�����ļ�����
				}
			}
			else
			{
				if(sendafile(threadid,sockid,env,path,FileData.cFileName,S_ALLFILE,false)>=0)
				{
				char sql[256];
				memset(sql,0,256);
				sprintf(sql,"delete %s.%s%s where ip��ַ='%s' and Ŀ¼='%s' and �ļ���='%s'",
				DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,FileData.cFileName);
				Execute(DBNAME,sql);
				//	break;
				}
				else 
					break;
			/*	char sql[256];
				memset(sql,0,256);
				sprintf(sql,"delete from %s.%s%s where ip��ַ='%s' and Ŀ¼='%s' and �ļ���='%s'",
				DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,FileData.cFileName);
				Execute(DBNAME,sql);*/
			}
		}
	} 
	if (!FindClose(hSearch)) 
	{ 
	} 
	return 0;
#else
	//ȡ�ô�ȫ·�����ļ���
	char filen[256];
	memset(filen,0,256);
	strcpy(filen,env);
	strcat(filen,"/");
	strcat(filen,path);
	DIRLIST *dirlist = NULL;
	if(strcasecmp(filenamepara,"*"))//����"*"ʱ������Ϊһ���ļ�
	{
		strcat(filen,"/");
		strcat(filen,filenamepara);
	}
	FStat fstat;
	if(fstat.isExist(filen))//�ļ���Ŀ¼����
	{
		if(fstat.isDir())//��Ŀ¼
		{
			dealDirSendFile(&dirlist,filen,fstat,path,env,threadid,sockid);
		}//if(fstat.isDir())//��Ŀ¼
		else
		{
			sendafile(threadid,sockid,env,path,filenamepara,S_ALLFILE,FALSE);

		/*	char ssql1[256];
			memset(ssql1,0,256);
			sprintf(ssql1,"delete from %s.%s.%s where ip��ַ='%s' and Ŀ¼='%s' and �ļ���='%s'",
			DBNAME,"dbo",WEBUPDATETABLE,threadinfo[threadid].client_addr,path,filenamepara);
			Execute(DBNAME,ssql1);*/

			char osql1[256];
			memset(osql1,0,256);
			sprintf(osql1,"delete %s.%s%s where ip��ַ='%s' and Ŀ¼='%s' and �ļ���='%s'",
			DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,filenamepara);
			Execute(DBNAME,osql1);
		}
		DIRLIST *pdirlist = dirlist;
		while(pdirlist)
		{
			if(pdirlist->bsendf == FALSE)
			{
				memset(filen,0,256);
				strcpy(filen,env);
				strcat(filen,"/");
				strcat(filen,pdirlist->path);
				if(fstat.isExist(filen))
				{
					if(fstat.isDir())//��Ŀ¼
					{
						dealDirSendFile(&dirlist,filen,fstat,pdirlist->path,env,threadid,sockid);
					}//if(fstat.isDir())//��Ŀ¼
				}
				pdirlist->bsendf == TRUE;
			}
			pdirlist = pdirlist->dirlist;
		}

		//��ʼ�ͷ��ڴ�
		pdirlist = dirlist;
		while(pdirlist)
		{
			DIRLIST *dirlistend = pdirlist->dirlist;
			delete pdirlist;
			pdirlist = NULL;
			pdirlist = dirlistend;
		}
	}//if(fstat.isExist(filen))//�ļ���Ŀ¼����
	return 0;

#endif
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��getPathFile()
 * ������������ȡĿ¼�µ������ļ�
 * ����    ��env,��������;path��·��
 * ����ֵ��  
 *			 
 *           
 *
 * --------------------------------------------------
 */
FILELIST * getPathFile(char *env,char *path)
{
#ifdef WIN32
	//ȡ�ô�ȫ·�����ļ���
	char filen[256];
	memset(filen,0,256);
	strcpy(filen,env);
	strcat(filen,"\\");
	strcat(filen,path);
	strcat(filen,"\\");
	strcat(filen,"*");

	WIN32_FIND_DATA FileData; 
	
	HANDLE hSearch = NULL; 
	BOOL fFinished = FALSE; 

	SYSTEMTIME lastwritetime,localtime;

    FILELIST *pfilelist = NULL;
    FILELIST *pfilelist1 = NULL;
	hSearch = FindFirstFile(filen, &FileData); 
	if(hSearch == INVALID_HANDLE_VALUE) 
	{ 
		return NULL;
	}
	else
	{
		if((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)//��Ŀ¼�ļ�
		{
			if(specifyDir(FileData.cFileName))
			{
				char path1[256];
				memset(path1,0,256);
				strcpy(path1,path);
				strcat(path1,"\\");
				strcat(path1,FileData.cFileName);
				if(pfilelist == NULL) pfilelist = getPathFile(env,path1);//�ݹ����Ŀ¼�����ļ�����
				else
				{
					while(pfilelist->pfilelist != NULL)
						pfilelist = pfilelist->pfilelist;
					pfilelist->pfilelist = getPathFile(env,path1);//�ݹ����Ŀ¼�����ļ�����
				}
				if(pfilelist1 == NULL) pfilelist1 = pfilelist;
			}
		}
		else
		{
			
			if(pfilelist == NULL) pfilelist = (FILELIST*)new FILELIST();
			else 
			{
				while(pfilelist->pfilelist != NULL)
					pfilelist = pfilelist->pfilelist;
				pfilelist->pfilelist = (FILELIST*)new FILELIST();
				pfilelist = pfilelist->pfilelist ;
			}
			strcpy(pfilelist->filename,FileData.cFileName);
			strcpy(pfilelist->path,path);
			pfilelist->pfilelist = NULL;
			FileTimeToSystemTime(&FileData.ftLastWriteTime,&lastwritetime);
			SystemTimeToTzSpecificLocalTime(NULL,&lastwritetime,&localtime);
			pfilelist->yearmonday = localtime.wYear*10000 + localtime.wMonth *100 + localtime.wDay ;
			pfilelist->hourminsec = localtime.wHour*10000 + localtime.wMinute*100 + localtime.wSecond ;
			if(pfilelist1 == NULL)
				pfilelist1 = pfilelist;			
		}
	}

	while (!fFinished) 
	{ 
		if (!FindNextFile(hSearch, &FileData)) 
		{
			fFinished = TRUE;
		}
		else
		{
			if((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)//��Ŀ¼�ļ�
			{
				if(specifyDir(FileData.cFileName))
				{
					char path1[256];
					memset(path1,0,256);
					strcpy(path1,path);
					strcat(path1,"\\");
					strcat(path1,FileData.cFileName);
					if(pfilelist == NULL) pfilelist = getPathFile(env,path1);//�ݹ����Ŀ¼�����ļ�����
					else
					{
						while(pfilelist->pfilelist != NULL)
							pfilelist = pfilelist->pfilelist;
						pfilelist->pfilelist = getPathFile(env,path1);//�ݹ����Ŀ¼�����ļ�����
					}
					if(pfilelist1 == NULL) pfilelist1 = pfilelist;
				}
			}
			else
			{
				if(pfilelist == NULL) pfilelist = (FILELIST*)new FILELIST();
				else 
				{
					while(pfilelist->pfilelist != NULL)
						pfilelist = pfilelist->pfilelist;
					pfilelist->pfilelist = (FILELIST*)new FILELIST();
					pfilelist = pfilelist->pfilelist ;
				}
				strcpy(pfilelist->filename,FileData.cFileName);
				strcpy(pfilelist->path,path);
				pfilelist->pfilelist = NULL;
				FileTimeToSystemTime(&FileData.ftLastWriteTime,&lastwritetime);
				SystemTimeToTzSpecificLocalTime(NULL,&lastwritetime,&localtime);
				pfilelist->yearmonday = localtime.wYear*10000 + localtime.wMonth *100 + localtime.wDay ;
				pfilelist->hourminsec = localtime.wHour*10000 + localtime.wMinute*100 + localtime.wSecond ;
				if(pfilelist1 == NULL)
					pfilelist1 = pfilelist;
			}
		}
	} 
	if (!FindClose(hSearch)) 
	{ 
	} 
	return pfilelist1;
#else
	//ȡ�ô�ȫ·�����ļ���
	char filen[256];
	memset(filen,0,256);
	strcpy(filen,env);
	strcat(filen,"/");
	strcat(filen,path);
    FILELIST *pfilelist = NULL;
    FILELIST *pfilelist1 = NULL;
	DIRLIST *dirlist = NULL;
	FStat fstat;
	if(fstat.isExist(filen))//�ļ���Ŀ¼����
	{
		if(fstat.isDir())//��Ŀ¼
		{
			dealDir(&dirlist,&pfilelist,&pfilelist1,filen,fstat,path,env);
		}//if(fstat.isDir())//��Ŀ¼
		DIRLIST *pdirlist = dirlist;
		while(pdirlist)
		{
			if(pdirlist->bsendf == FALSE)
			{
				memset(filen,0,256);
				strcpy(filen,env);
				strcat(filen,"/");
				strcat(filen,pdirlist->path);
				if(fstat.isExist(filen))
				{
					if(fstat.isDir())//��Ŀ¼
					{
						dealDir(&dirlist,&pfilelist,&pfilelist1,filen,fstat,pdirlist->path,env);
					}//if(fstat.isDir())//��Ŀ¼
				}
				pdirlist->bsendf == TRUE;
			}
			pdirlist = pdirlist->dirlist;
		}

		//��ʼ�ͷ��ڴ�
		pdirlist = dirlist;
		while(pdirlist)
		{
			DIRLIST *dirlistend = pdirlist->dirlist;
			delete pdirlist;
			pdirlist = NULL;
			pdirlist = dirlistend;
		}
	}//if(fstat.isExist(filen))//�ļ���Ŀ¼����
	return pfilelist1;
#endif
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��updateClientFile()
 * �������������¿ͻ����ļ�
 * ����    ��threadid,�̺߳�
 * ����ֵ��  
 * --------------------------------------------------
 */
void updateClientFile(int threadid)
{
	char where[64];
	memset(where,0,64);
	sprintf(where,"ip��ַ='%s'",threadinfo[threadid].client_addr);
	int fieldcount;
	void *buf = NULL;
	int retcount = readRecorders(DBNAME,WEBVISITTABLE,"ip��ַ",where,"ip��ַ",TRUE,&fieldcount,&buf);
	if(buf != NULL)
	{
		free(buf);
		buf = NULL;
	} 
	if(retcount <= 0)
	{
		char sql[256];
		memset(sql,0,256);
		
		sprintf(sql,"insert into %s.%s%s values('%s','graph','graph.lib',19700101,101010)",DBNAME,DBO,WEBVISITTABLE,threadinfo[threadid].client_addr);
		Execute(DBNAME,sql);
	}
	else 
	{
		retcount = readRecorders(DBNAME,WEBUPDATETABLE,"Ŀ¼,�ļ���,ip��ַ",where,"Ŀ¼,�ļ���,ip��ַ",FALSE,&fieldcount,&buf);
		//retcount = readRecorders(DBNAME,WEBUPDATETABLE,"ip��ַ",where,"ip��ַ",FALSE,&fieldcount,&buf);
		if(retcount>0)
		{
			
		//	char sql[256];
		//	memset(sql,0,256);
		//	sprintf(sql,"delete from %s.%s%s where ip��ַ='%s'",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr);
		//	Execute(DBNAME,sql);

			char envvar[256];//��������,������Զ�ȥ�������"\"
			memset(envvar,0,256);
			getEnv(envvar);
			int row = retcount/fieldcount;
			NAME *pname = (NAME*)buf;
			for(int i=0;i<row;i++)
			{
				sendafile(threadid,threadinfo[threadid].sockid,envvar,pname[i*fieldcount].name,pname[i*fieldcount+1].name,S_FILE,FALSE,TRUE);						
			//	sprintf(sql,"delete from %s.%s%s where ip��ַ='%s' and Ŀ¼='%s' and �ļ���='%s'",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,pname[i*fieldcount].name,pname[i*fieldcount+1].name);
			//	Execute(DBNAME,sql);
			}
			//FREE(pname);
		}
		if(buf != NULL)
		{
			free(buf);
			buf = NULL;
		} 
	}
}


int getLineHyperlink(const char* lineID,char** hyperlinkFile)
{
	int linkfileCnt=0;
	int rcdcnt,fieldCnt;	
	char chsWhere[255];
	void   *valbuf1,*valbuf2;
	NAME *pname1=NULL,*pname2=NULL;
	//
	char chsStations[255];
	char *pChBuf=(char*)hyperlinkFile;
	
	sprintf(chsWhere,"name='%s'",lineID);
	rcdcnt=readRecorders(DBNAME,"DEV_LINE","name,sname",chsWhere,"",TRUE,&fieldCnt,&valbuf1); //���ܽ����᷵�ض�����¼
	if(rcdcnt>0)
	{
		pname1 = (NAME*)valbuf1;
		int rcdval=rcdcnt;
		for(int i=0;i<rcdval;i++)
		{
			strcpy(chsStations,pname1[i].name);
			sprintf(chsWhere,"station='%s'",chsStations);
			
			rcdcnt=readRecorders(DBNAME,"FILEINFO","filename",chsWhere,"",TRUE,&fieldCnt,&valbuf2); 
			if(rcdcnt>0)
			{
				pname2 = (NAME*)valbuf2;
				for(int j=0;j<rcdcnt/fieldCnt;j++)
				{
					strcpy(pChBuf,pname2[j].name);
					pChBuf+=CHAR_BUF_LEN;
					linkfileCnt++;
				}
				FREE(pname2);
			}
		}
		FREE(pname1);
	}
	return linkfileCnt;
}

void SetPropToBuff(char* &msgBuf,const char* propname,const char* propval)
{
	short fieldlen;
	
	fieldlen=strlen(propname);
	pro_short2char(&fieldlen,msgBuf);
	msgBuf+=sizeof(short int);
	strncpy(msgBuf,propname,fieldlen);     //���Ե�ͷ����
	
	msgBuf+=fieldlen;
	
	fieldlen=strlen(propval);
	pro_short2char(&fieldlen,msgBuf);
	msgBuf+=sizeof(short int);
	strncpy(msgBuf,propval,fieldlen);      //���Ե�����
	msgBuf+=fieldlen;
}

/************************************************************************/
/* ���豸�����ͱ�Ż���豸���͵�������Ϣ                               */
/* ������scd�豸���ͱ��л����Ϣ                                        */
/************************************************************************/
int getDevTypeDesc(const char* devtypeno,char* devTypeDesc){
	int succ=0;
	int fieldCnt;
	int rcdcnt;
	void *valbuf;
	char chsWhere[256];
	sprintf(chsWhere,"type='%s'",devtypeno);
	rcdcnt=readRecorders(DBNAME,"DEVTYPE","describe",chsWhere,"",false,&fieldCnt,&valbuf);
	if(rcdcnt>0){
		NAME* pname=(NAME*)valbuf;
		strcpy(devTypeDesc,pname[0].name);
		succ=1;
		FREE (pname);
	}
	return succ;
}

char *yc_yx_type[]={"1*����","2*A�����","3*B�����","4*C�����",
"300*����","301*С������","302*ĸ��С������","303*ĸ������","304*��ĸ����","305*��ĸ��ĸ������","400*״̬",
"500*��բ","501*��·���뵶","502*Iĸ���뵶","503*IIĸ���뵶","504*IIIĸ���뵶","505*IVĸ���뵶",
"506*��ĸ���뵶","507*���䵶բ","508*վ�䵶բ","509*PT��բ","510*ĸ����բ","541*��·�ӵص�","542*����ӵص�",
"543*վ��ӵص�","544*PT�ӵص�","571*Iĸ�ӵص�","572*IIĸ�ӵص�","573*IIIĸ�ӵص�","574*IV ĸ�ӵص�","575*��ĸ�ӵص�"};

char *break_type[]={"������բ","��֧��բ","�ӵص�բ"};
/************************************************************************/
/*�ӡ���ң������.txt���л������������Ϣ,�ŵ��ṹ�й���ѯ��             */
/************************************************************************/
int getYXTypeDesc(int type,char* YXTypeDesc){
	//char* chPos;
	//char* chtype;
	int   nTmp;
	char *token;
	char chsBuf[255];
	char seps[]   = "*";
	for(int i=0;i<sizeof(yc_yx_type)/sizeof(char*);i++)
	{
		strcpy(chsBuf,yc_yx_type[i]);
		token = strtok(chsBuf, seps );
		nTmp=atoi(token);
		if(nTmp==type)
		{
			strcpy(YXTypeDesc,token = strtok( NULL, seps ));
			return TRUE;
		}
	}
	return FALSE;
}


/************************************************************************/
/*�豸��������������+����ֵ�ķ�ʽ��
/*@devtypedesc �豸�����Ĳ�������ʾ�豸���͡�
/*@codedesc �豸����������Ψһ��
/*svg��ѯ�������豸��������ͻ���
/*��ʱ��ʹ�ã�����ƽ̨�󣬱������ֶζ��������ġ�
/*��Ҫע��������ﴫ�ݵ����豸��������
/*
/************************************************************************/
int sendDevParaTable(int threadno,char* devtypedesc ,char* codedesc){
	MESSAGE msgbuf;
	msgbuf.type = S_DEVPARA;
	char *psendmsgbuf = msgbuf.messagebuf;
	
	short  fieldlen;
	char   chsTmp[256];

	int    fieldCnt;
	int    rcdcnt;
	void   *valbuf;
	char   chsWhere[256];
	
	short proppagecnt;
	char  *proppagecntpos;   //���ڱ����ڻ����������Ը�����λ�ã����ڱ�ʶ���������е�����ҳ�ĸ���

	char chsdevid[CODESIZE];
	
	
	/*
	memset(chsdevid,0,sizeof(chsdevid));
	strcpy(chsdevid,codedesc);
	readstation(chsdevid);
	if(chsdevid[0]=='\0'){
	return 0;					//��ʵʱ��scddevide����Ϊ�����Ӧ�����Ĵ��롣
	}
	*/
	strcpy(chsdevid,codedesc);
	if(chsdevid[0]=='\0') return 0;
	

	//����ң��������ͻ���
	if(strcmpNoCase(devtypedesc,DEV_MEAS)==0 ){
		//�����豸֡����
		fieldlen=DEV_FRAME_STYLE_DEVPROP;
		pro_short2char(&fieldlen,psendmsgbuf);
		psendmsgbuf += sizeof(short int);
		
		proppagecntpos=psendmsgbuf; //����λ��
		proppagecnt=1;
		pro_short2char(&proppagecnt,proppagecntpos);
		psendmsgbuf += sizeof(short int);
		
		sprintf(chsWhere,"����='%s'",chsdevid);
		printf("�����ݿ��ж�ȡ��Ϣ:[%s]\n",chsWhere);

		rcdcnt=readRecorders(DBNAME,"ң�������","����,����,�豸����,ң�����,��������,�����ʽ,��������,��������,��������,��������,��������־,ת����־",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		
		printf("\n readcord rcdcnt=%d  fieldcnt=%d",rcdcnt,fieldCnt);
		if(rcdcnt>0){
			NAME* pname=NULL;
			pname = (NAME*)valbuf;
			
			strcpy(chsTmp,"ң�����");
			fieldlen=strlen(chsTmp);
			
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);              //���Ե�ͷ����
			psendmsgbuf+=fieldlen;
			
			fieldlen = fieldCnt;
			pro_short2char(&fieldlen,psendmsgbuf);     //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);
			
			SetPropToBuff(psendmsgbuf,"����",pname[0].name);
			SetPropToBuff(psendmsgbuf,"����",pname[1].name);
			sprintf(chsTmp,"%s",pname[2].name);
			readstation(chsTmp);
			SetPropToBuff(psendmsgbuf,"����վ��",chsTmp);
			SetPropToBuff(psendmsgbuf,"ң�����",pname[3].name);
			SetPropToBuff(psendmsgbuf,"��������",pname[4].name);
			SetPropToBuff(psendmsgbuf,"�����ʽ",pname[5].name);
			SetPropToBuff(psendmsgbuf,"��������",pname[6].name);
			SetPropToBuff(psendmsgbuf,"��������",pname[7].name);
			SetPropToBuff(psendmsgbuf,"��������",pname[8].name);
			SetPropToBuff(psendmsgbuf,"��������",pname[9].name);
			SetPropToBuff(psendmsgbuf,"��������־",pname[10].name);
			SetPropToBuff(psendmsgbuf,"ת����־",pname[11].name);
			//pro_short2char(&proppagecnt,proppagecntpos);
			FREE (pname);
			msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
			return writemsg(threadinfo[threadno].sockid,&msgbuf);
		}		
	}




	//�����ض��ĳ����Ӳ���,Ŀǰʹ�����ݿ��й��������������·�����еĶ�Ӧͼ���ļ�����

	if(strcmpNoCase(devtypedesc,DEV_LINE_HYPERLINK)==0){
		short  filecnt=0;
		char file[HYPERLINK_MAX_FILENAME][CHAR_BUF_LEN];  //���������ļ�Ϊ10����ÿ���ļ�������Ϊ255���ַ���
		//�����豸֡����
		fieldlen=DEV_FRAME_STYLE_HYPERLINK;
		pro_short2char(&fieldlen,psendmsgbuf);
		psendmsgbuf += sizeof(short int);
		filecnt=getLineHyperlink(chsdevid,(char**)file);

		pro_short2char(&filecnt,psendmsgbuf);
		psendmsgbuf += sizeof(short int);

		for(int i=0;i<filecnt;i++)
		{
			fieldlen=strlen(file[i]);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,file[i],fieldlen);              //�ļ���
			psendmsgbuf+=fieldlen;
		}
		msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
		return writemsg(threadinfo[threadno].sockid,&msgbuf);
	}

	//�����豸֡����
	fieldlen=DEV_FRAME_STYLE_DEVPROP;
	pro_short2char(&fieldlen,psendmsgbuf);
	psendmsgbuf += sizeof(short int);
		
	proppagecntpos=psendmsgbuf; //����λ��
	proppagecnt=0;
	pro_short2char(&proppagecnt,proppagecntpos);
	psendmsgbuf += sizeof(short int);
	
	sprintf(chsWhere,"����='%s'",chsdevid);
	rcdcnt=readRecorders(DBNAME,"scd�豸��ͼ","����,����,վ����ϵ����,����",chsWhere,"",TRUE,&fieldCnt,&valbuf);
	if(rcdcnt>0){
		NAME* pname=NULL;
		pname = (NAME*)valbuf;
		
		strcpy(chsTmp,"SCD�豸");
		fieldlen=strlen(chsTmp);
		
		pro_short2char(&fieldlen,psendmsgbuf);
		psendmsgbuf += sizeof(short int);
		strncpy(psendmsgbuf,chsTmp,fieldlen);              //���Ե�ͷ����
		psendmsgbuf+=fieldlen;
		
		fieldlen = fieldCnt;
		pro_short2char(&fieldlen,psendmsgbuf);     //һ����ʾ���Եĸ�����ע��Ҫ*2
		psendmsgbuf+=sizeof(short int);
		
		SetPropToBuff(psendmsgbuf,"����",pname[0].name);
		SetPropToBuff(psendmsgbuf,"����",pname[1].name);
		sprintf(chsTmp,"%s",pname[2].name);
		readstation(chsTmp);
		SetPropToBuff(psendmsgbuf,"����վ��",chsTmp);
		
		memset(chsTmp,0,sizeof(chsTmp));
		if(getDevTypeDesc(pname[3].name,chsTmp)){
			SetPropToBuff(psendmsgbuf,"����",chsTmp);
		}
		else{
			SetPropToBuff(psendmsgbuf,"����",pname[3].name);
		}
		//SetPropToBuff(psendmsgbuf,"�ͺ�","");
		proppagecnt++;
		pro_short2char(&proppagecnt,proppagecntpos);
		FREE (pname);
	}
		
	if(strcmpNoCase(devtypedesc,DEV_BREAK)==0 || strcmpNoCase(devtypedesc,DEV_SWITCH)==0)
	{
		BOOL bIsBreak; 
		if(strcmpNoCase(devtypedesc,DEV_BREAK)==0)bIsBreak=TRUE;else bIsBreak=FALSE;
		if(bIsBreak)
			sprintf(chsWhere,"����='%s%s'",chsdevid,BREAK_POSTFIX);
		else
			sprintf(chsWhere,"����='%s%s'",chsdevid,SWITCH_POSTFIX);

		rcdcnt=readRecorders(DBNAME,"ң�Ų�����","����,����,����,�Ƿ�׷��,�Ƿ�ң��,�ն����,ң�����,�����ʽ,��ͼ��,SOE��,�����",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;

			strcpy(chsTmp,"SCDң��");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);              //���Ե�ͷ����
			psendmsgbuf+=fieldlen;

			fieldlen = fieldCnt;
			pro_short2char(&fieldlen,psendmsgbuf);                //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);
			
			SetPropToBuff(psendmsgbuf,"����",pname[0].name);
			SetPropToBuff(psendmsgbuf,"����",pname[1].name);
			
			memset(chsTmp,0,sizeof(chsTmp));
			getYXTypeDesc(atoi(pname[2].name),chsTmp);
			SetPropToBuff(psendmsgbuf,"����",chsTmp);
		
			SetPropToBuff(psendmsgbuf,"�Ƿ�׷��",atoi(pname[3].name)!=0 ? "��" : "��");
			SetPropToBuff(psendmsgbuf,"�Ƿ�ң��",atoi(pname[4].name)!=0 ? "��" : "��");
			SetPropToBuff(psendmsgbuf,"�ն����",pname[5].name);
			SetPropToBuff(psendmsgbuf,"ң�����",pname[6].name);
			SetPropToBuff(psendmsgbuf,"�����ʽ",pname[7].name);
			SetPropToBuff(psendmsgbuf,"��ͼ��",pname[8].name);
			SetPropToBuff(psendmsgbuf,"SOE��",pname[9].name);
			SetPropToBuff(psendmsgbuf,"�����",pname[10].name);

			proppagecnt++;
			pro_short2char(&proppagecnt,proppagecntpos);

			FREE (pname);	
		}
/*		
		sprintf(chsWhere,"name='%s'",chsdevid);
		if(bIsBreak)
			rcdcnt=readRecorders(DBNAME,"DEV_BREAKER","DEVID,NAME,STATUS,STYLE,ISZERO,ENGCODE",
				chsWhere,"",TRUE,&fieldCnt,&valbuf);
		else
			rcdcnt=readRecorders(DBNAME,"DEV_SWITCH","DEVID,NAME,STATUS,STYLE,	ENGCODE",
				chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;
			if(bIsBreak)
				strcpy(chsTmp,"EMS����");
			else
				strcpy(chsTmp,"EMS��բ");
			fieldlen=strlen(chsTmp);
			
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);              //���Ե�ͷ����
			psendmsgbuf+=fieldlen;

			pro_short2char((short*)&fieldCnt,psendmsgbuf);                //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);
			
			SetPropToBuff(psendmsgbuf,"DEVID",pname[0].name);
			SetPropToBuff(psendmsgbuf,"��ʶ��",pname[1].name);
			SetPropToBuff(psendmsgbuf,"����״̬",atoi(pname[2].name)==0 ? "��" : "��");
			SetPropToBuff(psendmsgbuf,"��բ����",break_type[(atoi(pname[3].name) <2 ? atoi(pname[3].name) : 2)]);
			if(bIsBreak)
			{
				SetPropToBuff(psendmsgbuf,"�Ƿ���ֵ",pname[3].name);
				SetPropToBuff(psendmsgbuf,"����",pname[4].name);
			}
			else
			{
				SetPropToBuff(psendmsgbuf,"����",pname[3].name);
			}
			
			proppagecnt++;
			pro_short2char(&proppagecnt,proppagecntpos);

			FREE (pname);	
			
		}
*/
	}
	else if(strcmpNoCase(devtypedesc,DEV_LINE)==0) {
		int nCol=0;
		sprintf(chsWhere,"name='%s'",chsdevid);
		rcdcnt=readRecorders(DBNAME,"DEV_LINE",
			"devid,name,maxmva,resist,react,halfcap,safecurrent,zeroresist,zeroreact,zerohalfcap,length,type,paralinenumber,linemj, \
			ismoniset,isteleline,belongtl,isresistvalue,headpoint,tailpoint",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;
			
			strcpy(chsTmp,"EMS��·");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //���Ե�ͷ����
			psendmsgbuf+=fieldlen;
			
			fieldlen = fieldCnt ;
			pro_short2char(&fieldlen,psendmsgbuf);                //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);
			
			int devid=0;
			SetPropToBuff(psendmsgbuf,"DEVID",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"��ʶ��",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"��������(MVA)",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"��ô����(��100)",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"��ô�翹(��100)",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"��ô������(��100)",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"��ȫ����",pname[devid++].name);

			SetPropToBuff(psendmsgbuf,"�ӵص���",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"�ӵص翹",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"�ӵذ�����",pname[devid++].name);

			SetPropToBuff(psendmsgbuf,"��·����",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"��·����",pname[devid++].name); //Ӧ�ô�linetype���л��

			SetPropToBuff(psendmsgbuf,"�����ߺ�[paralinenumber]",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"linemj",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"ismoniset",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"isteleline",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"belongtl",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"isresistvalue",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"headpoint",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"tailpoint",pname[devid++].name);
			
			proppagecnt++;
			pro_short2char(&proppagecnt,proppagecntpos);
			
			FREE (pname);	
		}
	}
	else if(strcmpNoCase(devtypedesc,DEV_BUS)==0) {
		sprintf(chsWhere,"name='%s'",chsdevid);
		rcdcnt=readRecorders(DBNAME,
			"DEV_BUS",
			"devid,name,realbus,iscentrumbus,maxvol,minvol,normalvol,engcode",
		chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;
			
			strcpy(chsTmp,"EMSĸ��");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //���Ե�ͷ����
			psendmsgbuf+=fieldlen;
			
			fieldlen = fieldCnt ;
			pro_short2char(&fieldlen,psendmsgbuf);                //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);
			
			int nCol=0;

			SetPropToBuff(psendmsgbuf,"DEVID",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ʶ��",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"ĸ������",atoi(pname[nCol].name)==1 ? "��·ĸ��" : "��ͨĸ��");
			nCol++;
			SetPropToBuff(psendmsgbuf,"����ĸ��",atoi(pname[nCol].name)==0 ? "��" : "��");
			nCol++;
			SetPropToBuff(psendmsgbuf,"��ѹ����",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ѹ����",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"������ѹ",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"����",pname[nCol++].name);

			proppagecnt++;
			pro_short2char(&proppagecnt,proppagecntpos);
			
			FREE (pname);	
		}
	}
/*
����ȥ���˺����ishtapesti��ʼ��3���ֶΣ���9����
devid,name,hvl,mvl,lvl,isresistvalue,it,mpt,
hmmva,hmtapty,hmresist,hmreact,hmextdg,hmextdb,hmnormaltap,hmisitcx_f0,hmisitcx_f1,hmconnected,hmzeroreact,hmzeroresist,hus,lmps,ishtapesti,hscadapoint,mtappoint
mlmva,mltapty,mlresist,mlreact,mlextdg,mlextdb,mlnormaltap,mlisitcx_f0,mlisitcx_f1,mlconnected,mlzeroreact,mlzeroresist,mus,mmps,ismtapesti,htappoint,lscadapoint
hlmva,hltapty,hlresist,hlreact,hlextdg,hlextdb,hlnormaltap,hlisitcx_f0,hlisitcx_f1,hlconnected,hlzeroreact,hlzeroresist,lus,hmps,isltapesti,mscadapoint,ltappoint
	*/
	else if(strcmpNoCase(devtypedesc,DEV_TRANSFORMER3LAYER)==0) {
		int nCol=0;
		short nPagePropCnt;
		sprintf(chsWhere,"name='%s'",chsdevid);
		rcdcnt=readRecorders(DBNAME,
			"DEV_TRANSTHREE",
			"devid,name,hvl,mvl,lvl,isresistvalue,it,mpt,hmmva,hmtapty,hmresist,hmreact,hmextdg,hmextdb,hmnormaltap,hmisitcx_f0,hmisitcx_f1,hmconnected,hmzeroreact,hmzeroresist,hus,lmps,mlmva,mltapty,mlresist,mlreact,mlextdg,mlextdb,mlnormaltap,mlisitcx_f0,mlisitcx_f1,mlconnected,mlzeroreact,mlzeroresist,mus,mmps,hlmva,hltapty,hlresist,hlreact,hlextdg,hlextdb,hlnormaltap,hlisitcx_f0,hlisitcx_f1,hlconnected,hlzeroreact,hlzeroresist,lus,hmps",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;


			strcpy(chsTmp,"EMS�������ѹ��");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //���Ե�ͷ����
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=8;

			pro_short2char(&nPagePropCnt,psendmsgbuf);                //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;

			SetPropToBuff(psendmsgbuf,"DEVID",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ʶ��",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ѹ�˶��ѹ(KV)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ѹ�˶��ѹ(KV)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ѹ�˶��ѹ(KV)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ôֵ��Ч",atoi(pname[nCol].name)==0 ? "��" : "��");
			nCol++;
			SetPropToBuff(psendmsgbuf,"���ص���",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�������",pname[nCol++].name);


			strcpy(chsTmp,"��ѹ��");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //���Ե�ͷ����
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=14;
			pro_short2char(&nPagePropCnt,psendmsgbuf);                //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;

			SetPropToBuff(psendmsgbuf,"�����",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�ֽ�ͷ����",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"*��ô����(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ô�翹(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ŵ絼",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ŵ���",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"������λ",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ص�ѹ��ѹ��",atoi(pname[nCol].name)==0 ? "��" : "��");
			nCol++;
			SetPropToBuff(psendmsgbuf,"��ȹ���",atoi(pname[nCol].name)==0 ? "��" : "��");
			nCol++;
			SetPropToBuff(psendmsgbuf,"�������",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�������(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"����翹(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��·���(��-��)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��·��ѹ(��-��)",pname[nCol++].name);
			

			strcpy(chsTmp,"��ѹ��");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //���Ե�ͷ����
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=14;
			pro_short2char(&nPagePropCnt,psendmsgbuf);                //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;

			SetPropToBuff(psendmsgbuf,"�����",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�ֽ�ͷ����",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"*��ô����(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ô�翹(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ŵ絼",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ŵ���",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"������λ",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ص�ѹ��ѹ��",atoi(pname[nCol].name)==0 ? "��" : "��");
			nCol++;
			SetPropToBuff(psendmsgbuf,"��ȹ���",atoi(pname[nCol].name)==0 ? "��" : "��");
			nCol++;
			SetPropToBuff(psendmsgbuf,"�������",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�������(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"����翹(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��·���(��-��)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��·��ѹ(��-��)",pname[nCol++].name);

			strcpy(chsTmp,"��ѹ��");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //���Ե�ͷ����
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=14;
			pro_short2char(&nPagePropCnt,psendmsgbuf);                //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;
			
			SetPropToBuff(psendmsgbuf,"�����",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�ֽ�ͷ����",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"*��ô����(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ô�翹(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ŵ絼",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ŵ���",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"������λ",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ص�ѹ��ѹ��",atoi(pname[nCol].name)==0 ? "��" : "��");
			nCol++;
			SetPropToBuff(psendmsgbuf,"��ȹ���",atoi(pname[nCol].name)==0 ? "��" : "��");
			nCol++;
			SetPropToBuff(psendmsgbuf,"�������",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�������(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"����翹(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��·���(��-��)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��·��ѹ(��-��)",pname[nCol++].name);


			pro_short2char(&proppagecnt,proppagecntpos);
			FREE (pname);	
		}
	}
	else if(strcmpNoCase(devtypedesc,DEV_SUBTRANSFORMER)==0) {
		int nCol=0;
		short nPagePropCnt;
		sprintf(chsWhere,"name='%s'",chsdevid);
		rcdcnt=readRecorders(DBNAME,
			"DEV_TRANSTWO",
			"devid,name,mva,mvl,zvl,istapside,tapty,normaltap,isitcx_f0,isitcx_f1,istapesti,connected,isresistvalue,us,mps,mpt,it,resist,react,extdg,extdb,zeroreact,zeroresist,hscadapoint,lscadapoint,tappoint",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;
			
			strcpy(chsTmp,"EMS��ѹ������I");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //���Ե�ͷ����
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=11;
			pro_short2char(&nPagePropCnt,psendmsgbuf);                //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;
			
			SetPropToBuff(psendmsgbuf,"DEVID",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ʶ��",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�����",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ѹ�˶��ѹ",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ѹ�˶��ѹ",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�ֽ�ͷ��",atoi(pname[nCol++].name)==0 ? "��ѹ��" : "��ѹ��");
			SetPropToBuff(psendmsgbuf,"�ֽ�ͷ����",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"������λ",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ص�ѹ",atoi(pname[nCol++].name)==0 ? "��" : "��");
			SetPropToBuff(psendmsgbuf,"��ȹ���",atoi(pname[nCol++].name)==0 ? "��" : "��");
			SetPropToBuff(psendmsgbuf,"���õ�λ����",atoi(pname[nCol++].name)==0 ? "��" : "��");
		
			
			strcpy(chsTmp,"EMS��ѹ������II");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //���Ե�ͷ����
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=15;
			pro_short2char(&nPagePropCnt,psendmsgbuf);                //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;
			
			SetPropToBuff(psendmsgbuf,"��ô����Ч",atoi(pname[nCol++].name)==0 ? "��" : "��");
			SetPropToBuff(psendmsgbuf,"��·��ѹ",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��·���",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�������",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ص���",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"*��ô����(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"*��ô�翹(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ŵ絼(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ŵ���(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�������(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"����翹(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"����翹(X100)",pname[nCol++].name);

			SetPropToBuff(psendmsgbuf,"hscadapoint",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"lscadapoint",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"tappoin",pname[nCol++].name);

			pro_short2char(&proppagecnt,proppagecntpos);
			FREE (pname);	
		}
	}
	else if(strcmpNoCase(devtypedesc,DEV_LOAD)==0) {
		sprintf(chsWhere,"name='%s'",chsdevid);
		rcdcnt=readRecorders(DBNAME,
			"DEV_LOAD",
			"devid,name,type,snom,dtstype,constreact,mwnom,mrnom,wmin,wmax,rmin,rmax,engpoint",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;
			strcpy(chsTmp,"EMS����");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //���Ե�ͷ����
			psendmsgbuf+=fieldlen;
			fieldlen = fieldCnt ;
			pro_short2char(&fieldlen,psendmsgbuf);                //һ����ʾ���Եĸ�����ע��Ҫ*2
			psendmsgbuf+=sizeof(short int);

			int nCol=0;
			SetPropToBuff(psendmsgbuf,"DEVID",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��ʶ��",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"���ɾ������ͺ�",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"�����",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"DTS��������",atoi(pname[nCol++].name)==0 ? "�綯��" : "���迹");
			SetPropToBuff(psendmsgbuf,"���迹�ٷֱ�",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��й�",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��޹�",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��С�й�",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"����й�",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"��С�޹�",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"����޹�",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"engpoint",pname[nCol++].name);

			proppagecnt++;
			pro_short2char(&proppagecnt,proppagecntpos);
			FREE (pname);		
		}
	}

	else if(strcmpNoCase(devtypedesc,DEV_MLBREAK)==0) {
		
	}

	else if(strcmpNoCase(devtypedesc,DEV_CAPACITOR)==0) {
		
	}
	else if(strcmpNoCase(devtypedesc,DEV_COIL)==0) {
		
	}
	else if(strcmpNoCase(devtypedesc,DEV_GNDSWITCH)==0) {
		
	}

	msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
	return writemsg(threadinfo[threadno].sockid,&msgbuf);
}



/*
//�����豸������
int sendDevParaTable(int threadno,int devtype,char*code)
{
	//���룬��������ע��վ����ϵ
	MESSAGE msgbuf;
	msgbuf.type = S_DEVPARA;
	char showfield[CODESIZE];
	memset(showfield,0,CODESIZE);
	//char *tb = getDevParaTable(devtype,showfield);
	char *tb = "SCD�豸������";
	strcpy(showfield,"վ����ϵ����,����,�ͺ�,��ע");
	//��������豸����
	if( tb != NULL && showfield[0] != '\0')
	{
		void *fieldbuf = NULL;
		
		if(!strcmp(showfield,"*")) {//=="*"
			getTableinfo(tb,&fieldbuf);
		}
		else
		{
			parsefieldstring(showfield,&fieldbuf);
		}
		NAME *pfield = (NAME*)fieldbuf;
		if(fieldbuf != NULL)
		{
			char where[128];
			memset(where,0,128);
			sprintf(where,"����='%s'",code);//��ʱ,�п��ܱ����޴����ֶ�
			int fieldcount;
			void *buf = NULL;
			int ret = readRecorders(DBNAME,tb,showfield,where,"",FALSE,&fieldcount,&buf);
			NAME *pval = (NAME*)buf;
			char *psendmsgbuf = msgbuf.messagebuf;
			pro_int2char(&fieldcount,psendmsgbuf);//��ĸ���
			psendmsgbuf += sizeof(int);

			short fieldlen;
			for(int i=0;i<fieldcount;i++)
			{
				if(i == 0)
				{
					memset((pfield+i)->name,0,sizeof((pfield+i)->name));
					strcpy((pfield+i)->name,"�����");
				}
				else if(i == 1)
				{
					memset((pfield+i)->name,0,sizeof((pfield+i)->name));
					strcpy((pfield+i)->name,"�豸����");
				}
				fieldlen = strlen((pfield+i)->name);//��������
				pro_short2char(&fieldlen,psendmsgbuf);
				psendmsgbuf += sizeof(short int);

				strncpy(psendmsgbuf,(pfield+i)->name,fieldlen);//����
				psendmsgbuf += fieldlen;

				if(ret>0)
				{
					if(i == 0)//վ����ϵ���룬Ҫ���������
					{
						readstation((char *)(pval+i)->name);
					}
					fieldlen = strlen((pval+i)->name);//��ֵ����
					pro_short2char(&fieldlen,psendmsgbuf);
					psendmsgbuf += sizeof(short int);

					strncpy(psendmsgbuf,(pval+i)->name,fieldlen);//��ֵ
					psendmsgbuf += fieldlen;
				}
				else
				{
					fieldlen = 0;//��ֵ����
					pro_short2char(&fieldlen,psendmsgbuf);
					psendmsgbuf += sizeof(short int);
				}
			}//for i
			fieldbuf = NULL;
			FREE(pval);
			FREE(pfield);
			msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
			printf("����\"%s\"���豸����\n",code);
			return writemsg(threadinfo[threadno].sockid,&msgbuf);
		}//if(fieldbuf != NULL)
		FREE(pfield);
	}//if( tb!= NULL)

	//����ʧ��
	int num = 0;
	pro_int2char(&num,(char*)msgbuf.messagebuf);
	msgbuf.length = sizeof(int);
	printf("û��\"%s\"���豸����\n",code);
	return writemsg(threadinfo[threadno].sockid,&msgbuf);
}

*/



//CODE two-dimensional char array,fixed char length equal code_len;
//return code count
int sendTopologyTable(int threadno,int codecnt,char** code)
{
	short charlen;

	MESSAGE msgbuf;
	msgbuf.type = S_TOPOLOGY;

	int  matchCodeCnt=0;
	Rdb_SQL rdbsql;
	rdbsql.RdbSelectFrom(TOPO);
	rdbsql.RdbSelectField("name");
	rdbsql.RdbSelectField("colorstate");  //����״̬
	int ret = rdbsql.RdbSelCmd("scada","scada");
	if(ret == RDB_OK)
	{
		int nRcdCount = rdbsql.RdbGetRcdCnt();
		if(nRcdCount == 0) return 0;
		char *psendmsgbuf = msgbuf.messagebuf;

		pro_int2char(&matchCodeCnt,psendmsgbuf);//��ĸ���
		psendmsgbuf += sizeof(int);
		
		char *pChBuf=(char*)code;
		for(int i=0;i<codecnt;i++)
		{
			for(int j=0;j<nRcdCount;j++)
			{
				char *name = rdbsql.RdbGetString(j,"name");
				if(!strcmpNoCase(name,pChBuf)) //find it in objname collection
				{
					//comb frame
					charlen=strlen(name);
					pro_short2char(&charlen,psendmsgbuf);
					psendmsgbuf += sizeof(short int);
					strncpy(psendmsgbuf,name,charlen);//����
					psendmsgbuf += charlen;
					
					short val=rdbsql.RdbGetVal_int(j,"active");
					pro_short2char(&val,psendmsgbuf);
					psendmsgbuf+=sizeof(short int);
					matchCodeCnt++;
					break;
				}
			}
			pChBuf+=CHAR_BUF_LEN_DEF;  //next code name
		}
		pro_int2char(&matchCodeCnt,(char*)msgbuf.messagebuf);
		msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));

		printf("�������˽������\n");
		return writemsg(threadinfo[threadno].sockid,&msgbuf);
	}
	return 0;
}

//send table all records to client
//
int sendTopoColorTable(int threadno)
{
	MESSAGE msgbuf;
	msgbuf.type = S_TOPOCOLOR;
	char *psendmsgbuf = msgbuf.messagebuf;
	
	int		fieldCnt;
	short   rcdcnt;
	void	*valbuf=NULL;
	char	chsWhere[256];
	
	sprintf(chsWhere,"%s","\0");
	rcdcnt=readRecorders(DBNAME,TOPOCOLORTABLE,"NO,RED,GREEN,BLUE",chsWhere,"",TRUE,&fieldCnt,&valbuf);
	printf("\n ����������ɫ����topcolor�� readcord rcdcnt=%d fieldcnt=%d   \n",rcdcnt,fieldCnt);
	if(rcdcnt>0)
	{	
		int topcolorCount=rcdcnt/fieldCnt;

		pro_int2char(&topcolorCount,psendmsgbuf);              //record count
		psendmsgbuf += sizeof(int);
	
		NAME* pname=NULL;
		pname = (NAME*)valbuf;
		unsigned char len = 0;
		short nTmp;
		for(int i=0;i<topcolorCount;i++)
		{
			nTmp=atoi(pname[i*fieldCnt].name);   //no
			pro_short2char(&nTmp,psendmsgbuf);
			psendmsgbuf+=sizeof(short int);

			nTmp=atoi(pname[i*fieldCnt+1].name);   //red
			pro_short2char(&nTmp,psendmsgbuf);
			psendmsgbuf+=sizeof(short int);

			nTmp=atoi(pname[i*fieldCnt+2].name);   //green
			pro_short2char(&nTmp,psendmsgbuf);
			psendmsgbuf+=sizeof(short int);

			
			nTmp=atoi(pname[i*fieldCnt+3].name);   //blue
			pro_short2char(&nTmp,psendmsgbuf);
			psendmsgbuf+=sizeof(short int);
		}
		
		FREE (pname);
		msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
		return writemsg(threadinfo[threadno].sockid,&msgbuf);
	}
	return 0;
}

#ifdef WIN32
	int answermsg(int threadno,HWND hwnd)
#else
	int answermsg(int threadno)
#endif
{
	int ret = 0;
	THREADMANAGE *pthreadinfo = threadinfo + threadno;
	ASSERT(pthreadinfo);
	printf( "��Ϣ����=%d\n",pthreadinfo->prcvdata->type );
	switch(pthreadinfo->prcvdata->type)//��Ϣ���� 
	{
	case S_STREAM:	//10,������
		{	
			#ifdef WIN32
				acceptStream(threadno,hwnd,true);
			#else
				acceptStream(threadno,TRUE);
			#endif
			break;
		}
	case S_DEVPARA://�豸����,14
		{
/* old code
			char *prcvbuf = pthreadinfo->prcvdata->messagebuf;
			int devtype  = 0;
			pro_char2int(prcvbuf,&devtype);//�豸����
			prcvbuf += sizeof(int);

			unsigned char namelen = prcvbuf[0];//�豸���볤��
			
			if(namelen > CODE_LEN) namelen = CODE_LEN;

			prcvbuf += sizeof(unsigned char);
			char objname[128];
			memset(objname,0,128);
			memcpy(objname,prcvbuf,namelen);

			sendDevParaTable(threadno,devtype,objname);
*/
			char *prcvbuf = pthreadinfo->prcvdata->messagebuf;
			char objnamedesc[128],objtypedesc[128];
			unsigned char namelen;
			memset(objnamedesc,0,sizeof(objnamedesc));
			memset(objtypedesc,0,sizeof(objtypedesc));
			
			namelen = prcvbuf[0];//��������
			prcvbuf += sizeof(unsigned char);
			memcpy(objtypedesc,prcvbuf,namelen);
			prcvbuf += namelen;
			namelen = prcvbuf[0];//��������
			prcvbuf += sizeof(unsigned char);
			memcpy(objnamedesc,prcvbuf,namelen);
			printf("�����豸�������ͻ���:����[%s] ����[%s]\r\n",objtypedesc,objnamedesc);
			sendDevParaTable(threadno,objtypedesc,objnamedesc);
			break;
		}
	case S_TOPOLOGY: //���˽ṹ��,18
		{
			//char objname[TOPU_MAX_COUNT][CHAR_BUF_LEN];
			char *objBuf=(char*)malloc(TOPU_MAX_COUNT*CHAR_BUF_LEN_DEF);
			if(objBuf==NULL)
				break;
			char *prcvbuf = pthreadinfo->prcvdata->messagebuf;

			memset(objBuf,0,TOPU_MAX_COUNT*CHAR_BUF_LEN_DEF);
			char *objname=objBuf;

			int objcnt;
			pro_char2int(prcvbuf,&objcnt);
			prcvbuf += sizeof(int);

			if(objcnt>=TOPU_MAX_COUNT)
				break;

			for(int i=0;i<objcnt;i++)
			{
				unsigned char namelen = prcvbuf[0];   //�豸���볤��
				if(namelen > CODE_LEN) 
					namelen = CODE_LEN;
				prcvbuf += sizeof(unsigned char);
				memcpy(objname,prcvbuf,namelen);
				prcvbuf+=namelen;
				objname+=CHAR_BUF_LEN_DEF;
			}
			sendTopologyTable(threadno,objcnt,(char**)objBuf);
			free(objBuf);
			break;
		}
	case S_RTUNAME://�ն˲�����,21
		{
			int groupno;
			pro_char2int(pthreadinfo->prcvdata->messagebuf,&groupno);
			//2011-07-21 lcg�޸ķ����ն˲���������
			readRtuPara(threadno,groupno);
			//readRTU(threadno,groupno);
			break;
		}
	case S_STATION:  //վ����ϵ��
		{
			int groupno;
			pro_char2int(pthreadinfo->prcvdata->messagebuf,&groupno);
			//2012-05-03 lcg�޸ķ����ն�վ����ϵ������
			readRTU(threadno,groupno);
			break;
		}
	case S_ANALOGVIEW:
		{
			char *pbuf = pthreadinfo->prcvdata->messagebuf;
			int			    paramClassify;
			unsigned char   paramLength;
			char			param[CODE_LEN];
			paramClassify = pbuf[0];
			pbuf += sizeof(unsigned char);
			paramLength = pbuf[0];
			if(paramLength > CODE_LEN) paramLength = CODE_LEN;
			pbuf += sizeof(unsigned char);
			memset(param,0,CODE_LEN);
			memcpy(param,pbuf,paramLength);
			readAnalogView(threadno,paramClassify,param);
			break;
		}
	case S_YCCODE://ң�����,22
	case S_DDCODE://��ȴ���,24
		{
			char *pbuf = pthreadinfo->prcvdata->messagebuf;
			int rtunums;//RTU����
			pro_char2int(pbuf,&rtunums);
			pbuf += sizeof(int);
			unsigned char codelen = pbuf[0];

			if(codelen > CODE_LEN) codelen = CODE_LEN;

			pbuf += sizeof(unsigned char);
			char rtucode[64];
			memset(rtucode,0,64);
			memcpy(rtucode,pbuf,codelen);
			if(pthreadinfo->prcvdata->type == S_YCCODE)
			 	readYCCode(threadno,"ģ������ͼ",rtucode);
		    else
				readYCCode(threadno,"�����ͼ",rtucode);
			break;
		}
	case S_CURVE:////��������,23
		{
			char *prcvbuf = pthreadinfo->prcvdata->messagebuf;
			unsigned char curFlag = prcvbuf[0];
			prcvbuf += sizeof(unsigned char);
			int curType;
			pro_char2int(prcvbuf,&curType);//��������
			prcvbuf += sizeof(int);
			int daytime;
			pro_char2int(prcvbuf,&daytime);//������,1970����������
			prcvbuf += sizeof(int);
			unsigned char namelen = prcvbuf[0];//���볤��

			if(namelen > CODE_LEN) namelen = CODE_LEN;

			prcvbuf += sizeof(unsigned char);
			char objname[128];
			memset(objname,0,128);
			memcpy(objname,prcvbuf,namelen);
			DayCurve(threadno,objname,curFlag,curType,daytime);
			break;
		}
	case S_REALCURVE://ʵʱ������������
		{
			char *prcvbuf = pthreadinfo->prcvdata->messagebuf;
			unsigned char curveNums = prcvbuf[0];///������Ŀ
			prcvbuf += sizeof(unsigned char);
			if(curveNums>0)
			{
				pthreadinfo->realcurvecount = curveNums;
				pthreadinfo->prealcurve =(REALCURVE*) new REALCURVE[curveNums];
				for(int i = 0; i < curveNums; i++)
				{
					REALCURVE *p = pthreadinfo->prealcurve + i;
					p->curStructNo = prcvbuf[0];
					p->cur_type = prcvbuf[1];
					p->cur_no = prcvbuf[2];
					prcvbuf += 3*sizeof(unsigned char);

					unsigned char len;
					len = prcvbuf[0];//��������
					prcvbuf += sizeof(unsigned char);
					strncpy(p->libName,prcvbuf,len);
					prcvbuf += len;

					len = prcvbuf[0];
					prcvbuf += sizeof(unsigned char);
					strncpy(p->objName,prcvbuf,len);
					prcvbuf += len;

					len = prcvbuf[0];
					prcvbuf += sizeof(unsigned char);
					strncpy(p->fieldName,prcvbuf,len);
					prcvbuf += len;					
				}
				RealCurve(threadno);
			}
			else
			{
				pthreadinfo->realcurvecount = 0;
				DELETEP(pthreadinfo->prealcurve);
			}
			break;
		}
	case S_FILE://�����ļ�,30
		{
			char *pbuf = pthreadinfo->prcvdata->messagebuf;
			char envvar[256];//����������������Զ�ȥ�������"\"
			memset(envvar,0,256);
			getEnv(envvar);

			char path[256],filen[128];
			memset(path,0,256); memset(filen,0,128);
			short pathlen;
			pro_char2short(pbuf,&pathlen);//·������
			pbuf += sizeof(short);
			memcpy(path,pbuf,pathlen);
			pbuf += pathlen;
			short filenamelen;
			pro_char2short(pbuf,&filenamelen);//�ļ������� 
			pbuf += sizeof(short int);
			memcpy(filen,pbuf,filenamelen);
			sendafile(threadno,pthreadinfo->sockid,envvar,path,filen, S_FILE,TRUE,TRUE);
			break;
		}
	case S_ALLFILE:////��ȫ���ļ�,31
		{
			char envvar[256];//����������������Զ�ȥ�������"\"
			memset(envvar,0,256);
			getEnv(envvar);
			
			char sql[256];
			memset(sql,0,256);
			sprintf(sql,"delete %s.%s%s where ip��ַ='%s'",DBNAME,DBO,WEBVISITTABLE,threadinfo[threadno].client_addr);
			Execute(DBNAME,sql);


		#ifdef WIN32
			CFileFind finder;
			CFileFind finderbmp;
			CFileFind finderbmpson;
			CFileFind finderson;
//			static const TCHAR szFileToFindbmp[] = _T("C:\\ds3000\\bmp\\*.*");
//			static const TCHAR szFileToFindgraph[] = _T("C:\\ds3000\\graph\\*.*");
			char szFileToFindbmp[256];
			strcpy(szFileToFindbmp,getenv("NBENV"));
			strcat(szFileToFindbmp,"\\bmp\\*.*");
			char szFileToFindgraph[256];
			strcpy(szFileToFindgraph,getenv("NBENV"));
			strcat(szFileToFindgraph,"\\graph\\*.*");

			BOOL bResultbmp = finderbmp.FindFile(szFileToFindbmp);
			BOOL bResultgraph = finder.FindFile(szFileToFindgraph);
			CString str;
			char Directory[256];
			CString direct;
			while (bResultgraph)//(bResultbmp || 
			{
			//   bResultbmp = finder.FindNextFile();
			  bResultgraph = finder.FindNextFile();
			//	str = 	(LPCTSTR) finder.GetRoot();
			  // str =  (LPCTSTR) finder.GetFileTitle();
			// str =  (LPCTSTR)  finder.GetFilePath();
			 //  str =  (LPCTSTR)  finder.GetFileURL();
			  str =  (LPCTSTR) finder.GetFileName();
			  if (str!="." && str!="..")
			  {
			    BOOL IsDirectory = finder.IsDirectory();
				direct = "graph";
				while (IsDirectory)
				{		
					if (str!="." && str!="..")
						direct = str;
					{
						sprintf(Directory,"%s\\graph\\%s\\*.*",envvar,str);
					}
					BOOL bResultgraphSon = finderson.FindFile(Directory);
					while(bResultgraphSon)
					{
						bResultgraphSon = finderson.FindNextFile();
						str =  (LPCTSTR) finderson.GetFileName();
						if (str!="." && str!="..")
						{
							sprintf(sql,"insert into %s.%s%s values('%s','graph/%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,direct,str);
							Execute(DBNAME,sql);
						}
					}
				break;
				}
			//	else
			//	{
				//	direct = "graph";
			//	}
			    sprintf(sql,"insert into %s.%s%s values('%s','%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,direct,str);
				Execute(DBNAME,sql);
			 }
			}
			while (bResultbmp)
			{
			   bResultbmp = finderbmp.FindNextFile();
			   str =  (LPCTSTR) finderbmp.GetFileName();
			 if (str!="." && str!="..")
			 {
			    BOOL IsDirectory = finderbmp.IsDirectory();
				direct = "bmp";
				while (IsDirectory)
				{		
					if (str!="." && str!="..")
						direct = str;
					{
						sprintf(Directory,"%s\\bmp\\%s\\*.*",envvar,str);
					}
					BOOL bResultbmpSon = finderbmpson.FindFile(Directory);
					while(bResultbmpSon)
					{
						bResultbmpSon = finderbmpson.FindNextFile();
						str =  (LPCTSTR) finderbmpson.GetFileName();
						if (str!="." && str!="..")
						{
							sprintf(sql,"insert into %s.%s%s values('%s','%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,direct,str);
							Execute(DBNAME,sql);
						}
					}
					break;
				}
			//	else
			//	{
				//	direct = "bmp";
			//	}
			    sprintf(sql,"insert into %s.%s%s values('%s','%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,direct,str);
				Execute(DBNAME,sql);
			 }
			}
			finder.Close();
			finderbmp.Close();
			finderbmpson.Close();
			finderson.Close();
		#else
			char filen[256];
			memset(filen,0,256);
			char files[256];
			memset(files,0,256);
			char env[256];
			strcpy(env,getenv("NBENV"));
			char path[256] = "graph";
			strcpy(filen,env);
			strcat(filen,"/");
			strcat(filen,path);

			FILELIST *pfilelist = NULL;
			FILELIST *pfilelist1 = NULL;

			DIRLIST *dirlist = NULL;
			FStat fstat;

			if(fstat.isExist(filen))//�ļ���Ŀ¼����
			{
				dealDir(&dirlist,&pfilelist,&pfilelist1,filen,fstat,path,env);
				FILELIST *ppfilelist = pfilelist;
				while(ppfilelist)//pdirlist &&
				{
					if(fstat.isExist(filen))
						{
							char osql[256];
							memset(osql,0,256);
							sprintf(osql,"insert into %s.%s%s values('%s','%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,path,ppfilelist->filename);
							printf("%s\n",osql);
							Execute(DBNAME,osql);
						}
					ppfilelist=ppfilelist->pfilelist;
				}
				ppfilelist=pfilelist;
			while(ppfilelist)
			{
				FILELIST *filelistend = ppfilelist->pfilelist;
				delete ppfilelist;
				ppfilelist = NULL;
				dirlist = NULL;
				ppfilelist = filelistend;
			}
			}



			char bfilen[256];
			memset(bfilen,0,256);
			char bfiles[256];
			memset(bfiles,0,256);
			char bpath[256] = "bmp";
			strcpy(bfilen,env);
			strcat(bfilen,"/");
			strcat(bfilen,bpath);

			FILELIST *bfilelist = NULL;
			FILELIST *bfilelist1 = NULL;

			DIRLIST *bdirlist = NULL;

			if(fstat.isExist(bfilen))//�ļ���Ŀ¼����
			{
				dealDir(&bdirlist,&bfilelist,&bfilelist1,bfilen,fstat,bpath,env);
				FILELIST *bbfilelist = bfilelist;
				while(bbfilelist)//pdirlist &&
				{
					if(fstat.isExist(bfilen))
						{
							char bosql[256];
							memset(bosql,0,256);
							sprintf(bosql,"insert into %s.%s%s values('%s','%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,bpath,bbfilelist->filename);
							printf("%s\n",bosql);
							Execute(DBNAME,bosql);
						}
					bbfilelist=bbfilelist->pfilelist;
				}
				bbfilelist=bfilelist;
			while(bbfilelist)
			{
				FILELIST *bfilelistend = bbfilelist->pfilelist;
				delete bbfilelist;
				bbfilelist = NULL;
				bdirlist = NULL;
				bbfilelist = bfilelistend;
			}
			}
		#endif
		//	return;
			MESSAGE msg;
			msg.type = S_ALLFILE;
			msg.length = 1;
			msg.messagebuf[0] =10;
			writemsg(pthreadinfo->sockid,&msg);
			if (threadinfo->runf==1)
			{
				sendfile(threadno,pthreadinfo->sockid,envvar,"graph","*");
				sendfile(threadno,pthreadinfo->sockid,envvar,"bmp","*");
			}
			
		//	sprintf(sql,"delete from %s.%s%s where ip��ַ='%s' and Ŀ¼='%s' and �ļ���='%s'",
			//		DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,filenamepara);
		//	Execute(DBNAME,sql);

			//���ͽ���
			msg.type = S_ALLFILE;
			msg.length = 1;
			msg.messagebuf[0] =1;
			writemsg(pthreadinfo->sockid,&msg);
			break;
		}
	case S_USER://���û���
		{
			readWebUser(threadno);
			break;
		}
	case   S_ACK://ƽ������,99
		{
			pthreadinfo->invalidtime = time(NULL);//���¼���ʱ��
			if(pthreadinfo->validf != INVALIDF)	// == -1 ������֪ͨ����ֹͣ����
				pthreadinfo->validf = 1;//��Ч��־

			break;
		}
	case S_TYPE://�ͻ�������
		{
			pthreadinfo->clienttype = pthreadinfo->prcvdata->messagebuf[0];
			#ifdef WIN32
				clntchangef = true;
			#endif

			if(pthreadinfo->clienttype == WEBSCADA)//�����webscada�û���Ҫ�����û����ͼ�η����
			{
				int userright;
				pro_char2int(pthreadinfo->prcvdata->messagebuf+2,&userright);
				USEREVENT *p = puserevent;
				while(p)
				{
					if(p->userright == userright)
					{
						pthreadinfo->puserevent = p;							//Ҫ�����û���¼��Ȩ������ֵ
						pthreadinfo->eventptr = pthreadinfo->puserevent->ptr;//��ǰ����ָ��
						break;
					}
					p = p->puserevent ;
				}
				if(pthreadinfo->prcvdata->messagebuf[1] == 1)//��һ�ε�¼
				{
					readWebUser(threadno);	
					readWebUserRight(threadno);
					sendTopoColorTable(threadno);

					int groupno;
					pro_char2int(pthreadinfo->prcvdata->messagebuf,&groupno);
					//2012-05-03 lcg�޸ķ����ն�վ����ϵ������
					readRTU(threadno,groupno);
					//readRtuPara(threadno,groupno);
//					readWebFilename(threadno);   //����WEBͼ�����
//					updateClientFile(threadno);
				}
				else
				{
					char where[64];
					memset(where,0,64);
					sprintf(where,"ip��ַ='%s'",threadinfo->client_addr);
					int fieldcount;
					void *buf = NULL;
					int	retcount = readRecorders(DBNAME,WEBUPDATETABLE,"Ŀ¼,�ļ���,ip��ַ",where,"Ŀ¼,�ļ���,ip��ַ",FALSE,&fieldcount,&buf);
					if (retcount>0)
					{
						readWebUser(threadno);
						readWebUserRight(threadno);
//						readWebFilename(threadno);
//						updateClientFile(threadno);
					}
				}
			}
			break;
		}
	default:
		{
			break;
		}
	}

	return ret;
}
