/*
 * --------------------------------------------------
 *   �ļ�����srvwatchthread.cxx 
 *   �������DS3210�������ϵͳWeb������   
 *   �ļ���Ҫ���ݣ�WEB�������ػ��߳�
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

	#if defined(_AIX) || defined(__linux)
		#include	"sys/signal.h"
	#endif
	
	#ifdef __hpux
		#include <sys/procset.h>
	#endif

    #include    "sys/time.h"
	#include	"sys/types.h"
	#include	"unistd.h"
#endif

#ifdef WIN32
	HANDLE m_hEvenKillWatch;
	HANDLE m_hEventKillCheck;
	extern int answermsg(int threadno,HWND hwnd);
#else
	extern int answermsg(int threadno);
#endif
	
#include "webstruct.h"
#include "externdefine.h"


extern THREADMANAGE threadinfo[INITCLIENTNUMS];
extern USEREVENT *puserevent;
int bEndFlag = FALSE;
int clientnums = INITCLIENTNUMS;//�ͻ�����Ŀ
int  clntchangef = FALSE;

extern void removeStream(THREADMANAGE *threadinfo1,int winno,int threadno,int bwait=FALSE);
extern void delay_t(int n);
extern int readRecorders(char* dbname,char* tablename,char* fieldname, char* where,char* orderfn,int bdistinct,int *fieldcount,void **buf);
extern int Execute(char *DBName,char* sqlStatement);
extern int parsefieldstring(char *srcstr,void **desp);
extern int sendafile(int threadid,SOCKET sockid,char *env,char* path,char* filenamepara,int type,int sendwrongf = FALSE,int bupdatetable=FALSE);
extern void updateClientFile(int threadid);

int CardRcdChgNums = 1;
time_t eventtime = 0;
int SendFileNums = 1;

//lcg 20110905���һ�����浱ǰcardrcd�����ı���
int cardrcdNums=0;

// �����߳�
#ifndef WIN32
int scd_pthread_create(PTHREAD_CALLFUNC  pthfunc,void *argp,int stacksize,	pthread_t *tid)
{

	int ret;

	if (stacksize==0) {
		ret=pthread_create(tid,NULL,pthfunc,argp);
	}
	else {
		pthread_attr_t   tmpattr;
  
		pthread_attr_init(&tmpattr);
		pthread_attr_setstacksize(&tmpattr,stacksize*1024);
		ret=pthread_create(tid,&tmpattr,pthfunc,argp);
	}
	return(ret);
}
#endif

/*
	valcount,�仯�����ݸ���
*/
void getData(int valcount,int threadno,int winno,Rdb_MultiTypeValue& val)
{
	if(valcount>0)//�仯���ݸ���>0
	{
		MULTISTREAM *pmultistream = threadinfo[threadno].pmultistream;
		int i;
		for( i = 0; i < threadinfo[threadno].streamcount; i++)
		{
			if(pmultistream == NULL) 
			{ 
				#ifdef WIN32
				//	LeaveCriticalSection(&threadinfo[threadno].critThread);
				#endif
				return;
			} 
			if(pmultistream->winno == winno)
				break;
			pmultistream = pmultistream->plist;
		}
		if(i >= threadinfo[threadno].streamcount)	
		{ 
			#ifdef WIN32
			//	LeaveCriticalSection(&threadinfo[threadno].critThread);
			#endif
			return;
		}   //û���ҵ�����ƥ�����
				
		CThreadStreamData *pthreadstreamdata = pmultistream->pthreadstreamdata;
		ASSERT(pthreadstreamdata != NULL);
		for(i=0;i<valcount;i++)
		{
			int order = val.RdbGetValOrderno(i);
			if(order >= 0 && order < pthreadstreamdata->datacount)
			{
				if(!pthreadstreamdata->pdatatype[order])//��������û�еõ���
				{
					val.RdbGetValInfo(i, &(pthreadstreamdata->pvalue[order].valuetype));
					pthreadstreamdata->pdatatype[order] = TRUE;
				}//if

				uchar datatype = pthreadstreamdata->pvalue[order].valuetype;
				switch(datatype)
				{
				case RDB_DATATYPE_UCHAR:		//�޷����ַ���,0
				case RDB_DATATYPE_CHAR:         //�з����ַ���,1
				case RDB_DATATYPE_USHORT:       //�޷��Ŷ�������,2
				case RDB_DATATYPE_SHORT:        //�з��Ŷ�������,3
				case RDB_DATATYPE_UINT:         //�޷���������,4
				case RDB_DATATYPE_INT:          //�з���������,5
					{
						pthreadstreamdata->pvalue[order].value.intval = val.RdbGetVal_int(i);
						break;
					}
				case RDB_DATATYPE_ULONG:        //�޷��ų�������,6
					{
						pthreadstreamdata->pvalue[order].value.ulongval = val.RdbGetVal_ulong(i);
						break;
					}
				case RDB_DATATYPE_LONG:         //�з��ų�������(8 unsigned char long),7
					{
						pthreadstreamdata->pvalue[order].value.longval = val.RdbGetVal_long(i);
						break;
					}
				case RDB_DATATYPE_FLOAT:        //�����ȸ�����,8
					{
						//���������ȽϺ�ԭ���Ĵ�С
						pthreadstreamdata->pvalue[order].value.floatval = val.RdbGetVal_float(i);
						break;
					}
				case RDB_DATATYPE_DOUBLE:       //˫���ȸ�����,9
					{
						pthreadstreamdata->pvalue[order].value.doubleval = val.RdbGetVal_double(i);
						break;
					}
				case RDB_DATATYPE_STRING:       //�ַ�����,10
					{
						 char *p = val.RdbGetString(RDB_MAX_STRVAL_LEN,i);
						 strncpy(pthreadstreamdata->pvalue[order].stringval,p,strlen(p));
						 pthreadstreamdata->pvalue[order].stringlen = strlen(p);
						break;
					}

				default:
					break;

				}//switch
				pthreadstreamdata->pdatachangedf[order] = TRUE;;
			}//if
		}//for i
		pthreadstreamdata->bchangef = TRUE;
	}//if(valcount>=0)
	#ifdef WIN32
	//	LeaveCriticalSection(&threadinfo[threadno].critThread);
	#endif
}



#ifndef WIN32
void recvStream(int kk)
{
	Rdb_QuickStream *stream = NULL;
	Rdb_MultiTypeValue val;
	MULTISTREAM *pmultistream = NULL;
	int winno;
	THREADMANAGE *pthreadinfo = NULL;
	for(int i = 0; i < INITCLIENTNUMS; i++)
	{
		pthreadinfo = threadinfo + i;
		if(pthreadinfo->runf == 1 && pthreadinfo->validf != INVALIDF)
		{
			stream = pthreadinfo->prdb_quickstream;
			if(stream != NULL)
			{
				pmultistream = pthreadinfo->pmultistream;
				while(pmultistream != NULL)
				{
					winno = pmultistream->winno;
					int valcount = stream->RdbGetFieldValue(BASEWINNO * i + winno, &val);
					if(valcount > 0)
					{
						printf("����������,���=%d,���� = %d,%s\n",i,valcount,threadinfo[i].client_addr);
						getData(valcount,i,winno,val);
					}
					pmultistream = pmultistream->plist;
				}
			}
		}
	}
}
#endif





/*	����˵��
 * --------------------------------------------------
 * ������  ��readstation()
 * ������������"վ����ϵ������"�����������еĶ�������� ����������
 * ����    ��
 * ����ֵ��  
 * --------------------------------------------------
 */
void readstation(char *groupname)
{
	Rdb_SQL rdbsql;
	rdbsql.RdbSelectFrom(STATION);
	rdbsql.RdbSelectField("name");
	rdbsql.RdbSelectField("describe");
	int ret = rdbsql.RdbSelCmd("scada","scada");
	if(ret == RDB_OK)
	{
		int nRcdCount = rdbsql.RdbGetRcdCnt();
		if(nRcdCount == 0) return;
		for(int i=0;i<nRcdCount;i++)
		{
			char *name = rdbsql.RdbGetString(i,"name");//����
			if(name == NULL  || name[0] == '\0')
			{
				continue;
			}
			if(!strcmp(name,groupname))
			{
				name = rdbsql.RdbGetString(i,"describe");
				if(name == NULL) return;
				memset(groupname,0,SHORTNAME_LEN);
				strcpy(groupname,name);
				return;
			}			
		}
	}
}

#if defined(_AIX)||defined(__linux)
void all_exit_s (int sid )
{
	printf("==�쳣�˳�==\n");
	exit( 0 );
}
#endif

//��õ�ǰʵʱ���cardrcd��¼����
int getCardRcdNums()
{
	Rdb_SQL rdbsql;
	rdbsql.RdbSelectFrom(CARDRCD);
	rdbsql.RdbSelectField("objtype");
	rdbsql.RdbSelectField("objname");
	rdbsql.RdbSelectField("cardtype");
	rdbsql.RdbSelectField("cardno");
	int ret = rdbsql.RdbSelCmd("scada","scada");
	if(ret == RDB_OK)
	{
		int nRcdCount = rdbsql.RdbGetRcdCnt();
		if(nRcdCount<0) 
			return 0;
		else 
			return nRcdCount;
	}
	return 0;
}


#ifdef WIN32
	UINT  checkEvent(LPVOID pParam)
#else
	void * checkEvent(void *argp)
#endif
{
	EventProc eventproc;
	SYS_EVENT sysevent;
	while(!bEndFlag)
	{	
		int hvaeevent = eventproc.Read(&sysevent);//
		if(hvaeevent)
		{
			short int type = sysevent.type;
			short int sort = sysevent.sort;
			readstation(sysevent.Object);
			USEREVENT *pevent = puserevent;				//�����е�ÿ������Ҫ����������ƶ���Ӧ��ָ��
			while(pevent)
			{
				for(int i=0;i<pevent->eventcount;i++)
				{
					if(sysevent.type == pevent->excludeevent[i])
					{
						memcpy(&(pevent->sysevent[pevent->ptr]),&sysevent,sizeof(SYS_EVENT));
						pevent->ptr++;
						pevent->ptr = pevent->ptr % EVENT_NUM_MAX;
						break;
					}
				}
				pevent = pevent->puserevent;
			}
			if(sysevent.type ==	ENTTYPE_SCADA_SETFLAG)//����
			{
				TRACE("�յ�������Ϣ\n");
				CardRcdChgNums++;
				CardRcdChgNums = CardRcdChgNums % MAXLOOPNUMS;
				if(CardRcdChgNums == 0) CardRcdChgNums = 1;
				time(&eventtime);
				//lcg changed
				cardrcdNums=getCardRcdNums();
				printf("��ǰ���ܸ���%d\r\n",cardrcdNums);
				
			}
			if(sysevent.type ==	ENTTYPE_FILE_SAVE)//�ļ������¼�
			{
				SendFileNums++;
				SendFileNums = SendFileNums % MAXLOOPNUMS;
				if(SendFileNums == 0) SendFileNums = 1;
				time(&eventtime);
			}
			else if(type == ENTTYPE_NULL && sort == ENTCLASS_WEB)//���ƵĶ�������
			{
				printf("�յ�����webʵʱ������\n");
				#ifndef WIN32
				#if defined(_AIX)||defined(__linux)
					signal(SIGTERM, all_exit_s);
  					signal(SIGKILL, all_exit_s);
				#else
					sigsend(P_PID,getpid(),SIGTERM);
					sigsend(P_PID,getpid(),SIGKILL);
				#endif
				#endif
			}

		}
		delay_t(WAIT_TIME/2 *1000);
	}
#ifdef WIN32
	ASSERT(SetEvent(m_hEventKillCheck));
	return TRUE;
#else
	return(NULL);
#endif	
}




/*	����˵��
 * --------------------------------------------------
 * ������  ��clearThread
 * ������������������߳�
 * ����    ��i,�̺߳�
 *
 * ����ֵ  ��
 * --------------------------------------------------
 */
void clearThread(int i)
{
	THREADMANAGE *pthread = threadinfo + i;
	//ɾ�������ݵ��ڴ�
	FREE(pthread->psnddata);
	FREE(pthread->prcvdata);

	pthread->invalidtime = 0;
	pthread->starttime = 0;

	pthread->validf = 0;
	pthread->cardrcdchgnums = 0;
	pthread->sendfilenums = 0;
	if(pthread->streamcount >0)
	{
		removeStream(pthread,0xff,i,TRUE);
	}
	
#ifdef WIN32
	closesocket(pthread->sockid);
#else
	close(pthread->sockid);
#endif
	delay_t(10000);
	pthread->sockid = 0;


	pthread->clienttype = 0;
	pthread->right = 0;
	pthread->puserevent = NULL;
	pthread->eventptr = 0;

	pthread->streamcount = 0;
	pthread->prdb_quickstream = NULL;
	pthread->pmultistream = NULL;
	
	pthread->realcurvecount = 0;
	DELETEP(pthread->prealcurve);
	memset(pthread->client_name,0,WEBNODENAMELEN);
	memset(pthread->client_addr,0,WEBNODENAMELEN);
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��sendregresult()
 * �������������Ϳͻ���ע���־
 * ����    ��regfע���־��=trueע��ɹ���=falseʧ��
 *
 * ����ֵ  ��
 * --------------------------------------------------
 */
void sendregresult(SOCKET sockid,int regf)
{
	MESSAGE regmsg;

	regmsg.length=32;
	if(regf)//ע��ɹ�
	{
		regmsg.type = S_SUCCESS;
		strcpy(regmsg.messagebuf,"��¼�ɹ�!");
	}
	else//ע��ʧ��
	{
		regmsg.type = S_FAILLOG;
		strcpy(regmsg.messagebuf,"��¼ʧ��!");
	}
	int ret = writemsg(sockid,&regmsg);
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��err_abort()
 * �����������쳣�˳�
 * ����    ��
 *
 * ����ֵ  ��
 * --------------------------------------------------
 */
void error_abort()
{
	perror("error,and exit!");
	exit(0);
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��acceptclient()
 * �������������̹߳�����У����һ��¼
 * ����    ��sockid=�������˺Ϳͻ��˶�Ӧ��SOCKET
 *			 clientaddr=�ͻ�IP��ַ
 * ����ֵ  ���ɹ�������>=0
 *           ʧ�ܣ�����-1  
 *
 * --------------------------------------------------
 */
static int acceptclient(SOCKET sockid,struct sockaddr_in clientaddr,int *threadno)
{
	register int i = 0;
	for( i = 0; i < clientnums; i++ )
	{
		if(threadinfo[i].runf == 0 && threadinfo[i].validf == 0)
		{
			threadinfo[i].runf = 1;
			break;
		}

	}
	*threadno = i;
	if(i >= clientnums)//�ͻ�����,����
	{
		return FALSE;
	}
	threadinfo[i].sockid = sockid;
	strncpy(threadinfo[i].client_addr, inet_ntoa(clientaddr.sin_addr), WEBNODENAMELEN);
	threadinfo[i].client_name[0] = 0;
	threadinfo[i].invalidtime = time(NULL);
	time(&(threadinfo[i].starttime));
	threadinfo[i].psnddata = (MESSAGE*)calloc(1,sizeof(MESSAGE));
	threadinfo[i].prcvdata = (MESSAGE*)calloc(1,sizeof(MESSAGE));
	printf("�ͻ��˵�ַ%s,%d\n",threadinfo[i].client_addr,i);
	return TRUE;
}


/*	����˵��
 * --------------------------------------------------
 * ������  dealStream(int threadno)
 * ����������
 *          �����̵߳�������
 * ����    ��threadno,�̺߳�
 *
 * ����ֵ  ��
 *
 * --------------------------------------------------
 */

void dealStream(int threadno)
{
	THREADMANAGE *threadinfo1 = &threadinfo[threadno];
	ASSERT(threadinfo1 != NULL);

	MULTISTREAM *pmultistream = threadinfo1->pmultistream;
	
	for(int i=0;i<threadinfo1->streamcount;i++)
	{
		ASSERT(pmultistream);

		CThreadStreamData *pthreadstreamdata = pmultistream->pthreadstreamdata;  //����������һ���е�CThreadStreamData����
		ASSERT(pthreadstreamdata);
		if(pthreadstreamdata->bchangef)			//�����ݱ�ˢ��
		{
			pthreadstreamdata->bchangef = FALSE;

			MESSAGE *sendmsg = threadinfo1->psnddata;
			ASSERT(sendmsg);

			sendmsg->type = S_STREAM;
			char *pchar = sendmsg->messagebuf;
			
			pchar += sizeof(int);//Ԥ���������λ��

			pro_int2char(&(pmultistream->winno),pchar);//���ں�
			pchar += sizeof(int);

			int streamdatacount=0;
			for(int k=0;k<pthreadstreamdata->datacount;k++)		//����CThreadStreamData��ÿ�����ݵ�
			{
				if(pthreadstreamdata->pdatachangedf[k])//����ֵ�ı���
				{
					pthreadstreamdata->pdatachangedf[k] = FALSE;
					pro_int2char(&(pthreadstreamdata->pold_order[k]),pchar);//���

					pchar += sizeof(int);
					pchar[0] = pthreadstreamdata->pvalue[k].valuetype;//����
					pchar += sizeof(char);
					
					streamdatacount++;
					switch(pthreadstreamdata->pvalue[k].valuetype)
					{
						case RDB_DATATYPE_UCHAR:		//�޷����ַ���,0
						case RDB_DATATYPE_CHAR:         //�з����ַ���,1
						case RDB_DATATYPE_USHORT:       //�޷��Ŷ�������,2
						case RDB_DATATYPE_SHORT:        //�з��Ŷ�������,3
						case RDB_DATATYPE_UINT:         //�޷���������,4
						case RDB_DATATYPE_INT:          //�з���������,5
							{
								int intval = (int)pthreadstreamdata->pvalue[k].value.intval;
								pchar[0] = sizeof(int);//��ֵ����
								pchar += sizeof(unsigned char);
								pro_int2char(&intval,pchar);//��ֵ
								pchar += sizeof(int);
								break;
							}
						/*case RDB_DATATYPE_ULONG:        //�޷��ų�������,6
							{
								hlong hlongval = (hlong)pthreadstreamdata->pvalue[k].value.ulongval;
								pchar[0] = sizeof(hlong);//��ֵ����
								pchar += sizeof(unsigned char);
								pro_long2char(&hlongval,pchar);//��ֵ
								pchar += sizeof(hlong);
								break;
							}
						case RDB_DATATYPE_LONG:         //�з��ų�������(8 unsigned char long),7
							{
								hlong hlongval = (hlong)pthreadstreamdata->pvalue[k].value.longval;
								pchar[0] = sizeof(hlong);//��ֵ����
								pchar += sizeof(unsigned char);
								pro_long2char(&hlongval,pchar);//��ֵ
								pchar += sizeof(hlong);
								break;
							}*/
						case RDB_DATATYPE_FLOAT:        //�����ȸ�����,8
							{
								float floatval = (float)pthreadstreamdata->pvalue[k].value.floatval;
								pchar[0] = sizeof(float);//��ֵ����
								pchar += sizeof(unsigned char);
								pro_float2char(&floatval,pchar);//��ֵ
								pchar += sizeof(float);
								break;
							}
						case RDB_DATATYPE_DOUBLE:       //˫���ȸ�����,9
							{
								double floatval = pthreadstreamdata->pvalue[k].value.doubleval;
								pchar[0] = sizeof(double);//��ֵ����
								pchar += sizeof(unsigned char);
								pro_double2char(&floatval,pchar);//��ֵ
								pchar += sizeof(double);
								break;
							}
						case RDB_DATATYPE_STRING:       //�ַ�����,10
							{
								pchar[0] = (unsigned char)pthreadstreamdata->pvalue[k].stringlen;//��ֵ����
								pchar += sizeof(unsigned char);
								strcpy(pchar,pthreadstreamdata->pvalue[k].stringval);//��ֵ
								pchar += pthreadstreamdata->pvalue[k].stringlen;
								break;
							}

						default:
							{
								streamdatacount--;
								break;
							}
						}//switch
				}//if(pthreadstreamdata->pdatachangedf[k])//����ֵ�ı���
			}//for k
			if(streamdatacount>0)
			{
				sendmsg->length = abs(sendmsg->messagebuf - pchar);
				pro_int2char(&streamdatacount,sendmsg->messagebuf);//��Ч�����
				writemsg(threadinfo1->sockid,sendmsg);//����������
				memset(sendmsg,0,sizeof(MESSAGE));
			}
		}//if(pthreadstreamdata->bchangf)//�����ݱ�ˢ��
		pmultistream = pmultistream->plist;
	}//for i
}


//��ϳ������
void combineEvent(char **pbuf,SYS_EVENT *sysevent)
{
		char *psendbuf = *pbuf;
//		short int shortvar = sysevent->grpno;//�����
		short int shortvar = 0;//�����
		pro_short2char(&shortvar,psendbuf);
		psendbuf += sizeof(short int);

		shortvar = sysevent->sort;//�����
		pro_short2char(&shortvar,psendbuf);
		psendbuf += sizeof(short int);

		shortvar = sysevent->type ;//����
		pro_short2char(&shortvar,psendbuf);
		psendbuf += sizeof(short int);

		int intvar = sysevent->ymd;//������
		pro_int2char(&intvar,psendbuf);
		psendbuf += sizeof(int);

		intvar = sysevent->hmsms ;//4��ʱ�������
		pro_int2char(&intvar,psendbuf);
		psendbuf += sizeof(int);

		psendbuf[0] = sysevent->security ;//5����ȫ��
		psendbuf += sizeof(unsigned char);

		shortvar = sysevent->state;//״̬
		pro_short2char(&shortvar,psendbuf);
		psendbuf += sizeof(short int);

		float floatvar = sysevent->val; //��ǰ����ֵ
		pro_float2char(&floatvar,psendbuf);
		psendbuf += sizeof(float);

		unsigned char bytelen = strlen(sysevent->Object);//�¼�����������
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->Object,bytelen);//�¼���������
		psendbuf += bytelen;

		bytelen = strlen(sysevent->member0);//�¼�������0��
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->member0,bytelen);//�¼�������0
		psendbuf += bytelen;

		bytelen = strlen(sysevent->member1);//�¼�������1��
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->member1,bytelen);//�¼�������1
		psendbuf += bytelen;

		bytelen = strlen(sysevent->member2);//�¼�������2��
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->member2,bytelen);//�¼�������2
		psendbuf += bytelen;

		bytelen = strlen(sysevent->member3);//�¼�������3��
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->member3,bytelen);//�¼�������3
		psendbuf += bytelen;

		bytelen = strlen(sysevent->graph);//18����ͼ����
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->graph,bytelen);//18����ͼ��
		psendbuf += bytelen;

		bytelen = strlen(sysevent->tone);//����������Ϣ��
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->tone,bytelen);//����������Ϣ
		psendbuf += bytelen;

		bytelen = strlen(sysevent->text);//����������Ϣ��
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->text,bytelen);//����������Ϣ
		psendbuf += bytelen;

		*pbuf = psendbuf;

}


/*	����˵��
 * --------------------------------------------------
 * ������  dealSysEvent(threadno)
 * ����������
 *          1 ����ϵͳ����
 * ����    ��
 *
 * ����ֵ  ��
 *
 * --------------------------------------------------
 */

void dealSysEvent(int threadno)
{
	//1����෢��50�����������������ֶ�����ķ�
	THREADMANAGE *threadinfo1 = &threadinfo[threadno];
	ASSERT(threadinfo1 != NULL);

	if(threadinfo1->validf == -1 || threadinfo1->puserevent == NULL) return;
	MESSAGE *psendmsg = threadinfo1->psnddata;
	ASSERT(psendmsg != NULL);

	char *psendbuf = psendmsg->messagebuf ;
	int syseventptr = threadinfo1->puserevent->ptr;//threadinfo1->puserevent->ptr��ֵȡ����������ֵ�Ƕ�̬�仯��
	psendmsg->type = S_REALEVENT;//��������
	short int eventnums;
	int count = 0;
	short count1 = 0;
	if(threadinfo1->eventptr >  syseventptr)
	{
		eventnums = syseventptr + EVENT_NUM_MAX - threadinfo1->eventptr;
		count = eventnums / 50 ;//���ͻ������������ޣ���������
		count1 = eventnums % 50;
	}
	else if(threadinfo1->eventptr < syseventptr)
	{
		eventnums = syseventptr - threadinfo1->eventptr;
		
		count = eventnums / 50 ;//���ͻ������������ޣ���������
		count1 = eventnums % 50;
	}
	else
		return;

	for(int i=0;i<count;i++)
	{		
		psendbuf = psendmsg->messagebuf ;
		eventnums = 50;
		pro_short2char(&eventnums,psendbuf);//�������  >50ʱ
		psendbuf += sizeof(short int);
		for(int ptr=threadinfo1->eventptr;ptr<threadinfo1->eventptr+50;ptr++)
		{
			SYS_EVENT *sysevent =   &threadinfo1->puserevent->sysevent[ptr%EVENT_NUM_MAX];
			combineEvent(&psendbuf,sysevent);
		}
		psendmsg->length = abs(psendmsg->messagebuf - psendbuf);//���ĳ���

		writemsg(threadinfo1->sockid,psendmsg);

		threadinfo1->eventptr = (threadinfo1->eventptr+50) % EVENT_NUM_MAX;
	}
	if(count1<=0 || count1>EVENT_NUM_MAX) return;
		//����50���������
	psendbuf = psendmsg->messagebuf ;
	pro_short2char(&count1,psendbuf);//������� 
	psendbuf += sizeof(short int);
	for(int ptr=threadinfo1->eventptr;ptr<threadinfo1->eventptr+count1;ptr++)
	{
		SYS_EVENT *sysevent =   &threadinfo1->puserevent->sysevent[ptr%EVENT_NUM_MAX];
		combineEvent(&psendbuf,sysevent);
	}
	psendmsg->length = abs(psendmsg->messagebuf - psendbuf);//���ĳ���
	writemsg(threadinfo1->sockid,psendmsg);
	threadinfo1->eventptr = (threadinfo1->eventptr+count1) % EVENT_NUM_MAX;
	return;
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��sendCardRCD()
 * �������������¿ͻ���"��־�Ƽ�¼��"
 * ����    ��threadid,�̺߳�
 * ����ֵ��  
 * --------------------------------------------------
 */
void sendCardRCD(int threadid)
{
	Rdb_SQL rdbsql;

	rdbsql.RdbSelectFrom(CARDRCD);
	rdbsql.RdbSelectField("groupno");
	rdbsql.RdbSelectField("firststate");
	rdbsql.RdbSelectField("secondstate");
	rdbsql.RdbSelectField("objtype");
	rdbsql.RdbSelectField("objname");
	rdbsql.RdbSelectField("cardtype");
	rdbsql.RdbSelectField("cardno");
	rdbsql.RdbSelectField("startx");
	rdbsql.RdbSelectField("starty");
	rdbsql.RdbSelectField("metaid");
	rdbsql.RdbSelectField("hostname");
	rdbsql.RdbSelectField("username");
	rdbsql.RdbSelectField("context");
	rdbsql.RdbSelectField("graphpath");
	rdbsql.RdbSelectField("metapath");
	rdbsql.RdbSelectField("devdescribe");


// 	rdbsql.RdbSelectFrom(CARDRCD);	
// 	rdbsql.RdbSelectField("bitmap");
// 	rdbsql.RdbSelectField("cardno");
// 	rdbsql.RdbSelectField("cardtype");
// 	rdbsql.RdbSelectField("context");
// 	rdbsql.RdbSelectField("firststate");
// 	rdbsql.RdbSelectField("groupno");
// 	rdbsql.RdbSelectField("height");
// 	rdbsql.RdbSelectField("hostname");
// 	rdbsql.RdbSelectField("imagename");
// 	rdbsql.RdbSelectField("imagetype");
// 	rdbsql.RdbSelectField("objname");
// 	rdbsql.RdbSelectField("objtype");
// 	rdbsql.RdbSelectField("secondstate");
// 	rdbsql.RdbSelectField("startx");
// 	rdbsql.RdbSelectField("starty");
// 	rdbsql.RdbSelectField("userdefgr");
// 	rdbsql.RdbSelectField("username");
// 	rdbsql.RdbSelectField("width");
	int ret = rdbsql.RdbSelCmd("scada","scada");
	if(ret == RDB_OK)
	{
		int nRcdCount = rdbsql.RdbGetRcdCnt();
		if(nRcdCount < 0) return;
		MESSAGE msgCardrcd;
		char *msgbuf = msgCardrcd.messagebuf;
		char *ptmp;
		msgCardrcd.type = S_CARDRCD;
		pro_int2char(&nRcdCount,msgbuf);
		msgbuf += sizeof(int);
		int len;
		for(int i=0;i<nRcdCount;i++)
		{
			
			//��ʵʱ���ü�¼�����浽��������.
			len = rdbsql.RdbGetVal_int(i,"groupno");//�������
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);
			
			len = rdbsql.RdbGetVal_int(i,"firststate");//��һ̬
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"secondstate");//�ڶ�̬
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"objtype");//��������
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			ptmp = rdbsql.RdbGetString(i,"objname");//������(�������)��
			len = strlen(ptmp);						//������볤��
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);				//������(�������)��
			msgbuf += len;

			len = rdbsql.RdbGetVal_int(i,"cardtype");//��־������
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);
			
			len = rdbsql.RdbGetVal_int(i,"cardno");//��־��
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"startx");//����X��
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);
			
			len = rdbsql.RdbGetVal_int(i,"starty");//����Y
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			
			len = rdbsql.RdbGetVal_int(i,"metaid");//����Y
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			ptmp = rdbsql.RdbGetString(i,"hostname");//������
			len = strlen(ptmp);//����������
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//������
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"username");//�û���
			len = strlen(ptmp);//�û�������
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//�û���
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"context");//ע��
			len = strlen(ptmp);//ע�ͳ���
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//ע��
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"graphpath");
			len = strlen(ptmp);					
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);				
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"metapath");
			len = strlen(ptmp);					
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);				
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"devdescribe");
			len = strlen(ptmp);					
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);				
			msgbuf += len;
			
/*
			ptmp = rdbsql.RdbGetString(i,"bitmap");//λͼ��
			len = strlen(ptmp);
			msgbuf[0] = len;//λͼ������
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//λͼ��
			msgbuf += len;

			len = rdbsql.RdbGetVal_int(i,"cardno");//��־��
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"cardtype");//��־������
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			ptmp = rdbsql.RdbGetString(i,"context");//ע��
			len = strlen(ptmp);//ע�ͳ���
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//ע��
			msgbuf += len;

			len = rdbsql.RdbGetVal_int(i,"firststate");//��һ̬
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"groupno");//�������
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"height");// �߶ȣ�
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			ptmp = rdbsql.RdbGetString(i,"hostname");//������
			len = strlen(ptmp);//����������
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//������
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"imagename");//ͼ����
			len = strlen(ptmp);//ͼ��������
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//ͼ����
			msgbuf += len;
			
			len = rdbsql.RdbGetVal_int(i,"imagetype");//ͼ������
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			ptmp = rdbsql.RdbGetString(i,"objname");//������(�������)��
			len = strlen(ptmp);//������볤��
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//������(�������)��
			msgbuf += len;
			
			len = rdbsql.RdbGetVal_int(i,"objtype");//��������
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"secondstate");//�ڶ�̬
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"startx");//����X��
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"starty");//����Y
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);
			
			ptmp = rdbsql.RdbGetString(i,"userdefgr");//�û�����ͼԪ
			len = strlen(ptmp);//�û�����ͼԪ����
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//�û�����ͼԪ
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"username");//�û���
			len = strlen(ptmp);//�û�������
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//�û���
			msgbuf += len;
			
			len = rdbsql.RdbGetVal_int(i,"width");//��
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);
*/
		}
		msgCardrcd.length = abs(msgbuf - msgCardrcd.messagebuf);
		writemsg(threadinfo[threadid].sockid,&msgCardrcd);
	}
}





/*	����˵��
 * --------------------------------------------------
 * ������  answerclient(LPVOID pParam)
 * ����������
 *          1 ��Ӧ�ͻ�������߳�
 * ����    ��LPVOID pParam�����ͷ���˵�Socket
 *
 * ����ֵ  ��
 *
 * --------------------------------------------------
 */
#ifdef WIN32
	UINT answerclient(LPVOID pParam)
#else
	void * answerclient(void *pParam)
#endif
	{
	int threadno = ((int*)pParam)[0];
	#ifdef WIN32
		HWND hwnd =	   (HWND)(((int*)pParam)[1]);
	#endif
	delete pParam;


	fd_set				readset;
    struct timeval		readtv;
	int					nums;

	readtv.tv_sec  = 0;
	readtv.tv_usec = 100*1000;//0.1��,��Ϊ0����CPU����̫��20*100;//

	THREADMANAGE *threadinfo1 = threadinfo + threadno;
	ASSERT(threadinfo1);

	sendregresult(threadinfo1->sockid ,TRUE);   //����ע��ɹ���Ϣ
	
	threadinfo1->runf = 1;
	clntchangef	= TRUE;
	time_t  threadtime;
	
	while(TRUE) {
		readtv.tv_sec  = 0;
		readtv.tv_usec = 100*1000;
		if(threadinfo1->validf == INVALIDF){
			printf("����������ֹ!\n");
			//ƽ�����ĳ�ʱ 
			break;
		}
	    FD_ZERO(&readset);
		FD_CLR(threadinfo1->sockid, &readset);
		FD_SET(threadinfo1->sockid,&readset);
		int fd_isset = FD_ISSET(threadinfo1->sockid,&readset);
		nums = select(FD_SETSIZE,&readset,0,0,&readtv);
		FD_CLR((SOCKET)threadinfo1->sockid,&readset);
        if(nums>0) {//�յ����籨��
			if(readmsg(threadinfo1->sockid,threadinfo1->prcvdata) == 0)
			{
				#ifdef WIN32
					answermsg(threadno,hwnd);//���Ĵ���
					//updateClientFile(threadno);
				#else
					answermsg(threadno);//���Ĵ���
					//	if(sysevent.type ==	ENTTYPE_FILE_SAVE)//�ļ������¼�
				//	updateClientFile(threadno);
					//updateClientsFile(threadno);
				#endif
			}
			else//�����Ĵ��жϴ���
			{
				printf("�����Ĵ�,����������ֹ!\n");
				threadinfo1->validf = INVALIDF;
				break;//��������Ϣ����û�ж������ݣ�˵���ͻ����˳�
			}
        }                    
        else if(nums == SOCKET_ERROR) {        //SELECT���ź��жϣ��������˳�
			if(errno != EINTR)
			{
				threadinfo1->validf = INVALIDF;
				break;
			}
		}
        else {//�߳̿���ʱ,��ʱ����ɨ��ʵʱ���߳��˳���־
			if(threadinfo1->streamcount >0)//ע������
			{
				dealStream(threadno);
			}

			if(threadinfo1->clienttype == WEBSCADA)//�û�������webscada
			{
				dealSysEvent(threadno);//����ʵʱ����
				
				if(threadinfo1->cardrcdchgnums !=  CardRcdChgNums )     //���ǵ�,�յ����ʵʱ��ı䣬��һ���ӳ�
				{
					time(&threadtime);
					//printf("threadtime = %d,eventtime = %d",threadtime,eventtime);
					//10s�ӳ�,ע�⣬�������������������ġ�10��ʾ10�����˼
					//���ڴ��ڿͻ��˽��չ��������Ϣ��ԭ�򣬸�Ϊ30
					if(threadtime - eventtime > 10)
					{
						//TRACE1("���͹�����Ϣ��%s\n",threadinfo1->client_addr );
						printf("10�볬ʱ,���͹�����Ϣ��%s ?\r\n",threadinfo1->client_addr);
						sendCardRCD(threadno);
						threadinfo1->cardrcdchgnums = threadinfo1->cardrcdchgnums + 1 ;
						threadinfo1->cardrcdchgnums = threadinfo1->cardrcdchgnums % MAXLOOPNUMS;
						time(&eventtime);  //reinit eventtime variable

					}
					else
					{
						int cardrcdnums=getCardRcdNums();
						if(cardrcdnums!=cardrcdNums)   //������ı�����У�ʵʱ����Ʊ��¼�����ı��ˡ�
						{
							printf("��־�Ƽ�¼�ı�,���͹�����Ϣ,��ǰ���ܸ���%d \r\n",cardrcdnums);
							sendCardRCD(threadno);
							threadinfo1->cardrcdchgnums = threadinfo1->cardrcdchgnums + 1 ;
							threadinfo1->cardrcdchgnums = threadinfo1->cardrcdchgnums % MAXLOOPNUMS;
							cardrcdNums = cardrcdnums;
						}
					}
				}

				/*if(threadinfo1->sendfilenums !=  SendFileNums )//��ͻ��˷��͸����ļ�
				{
					
					time(&threadtime);
					if(threadtime - eventtime > 3)//1s�ӳ�
					{
						updateClientFile(threadno);
						threadinfo1->sendfilenums = threadinfo1->sendfilenums + 1 ;
						threadinfo1->sendfilenums = threadinfo1->sendfilenums % MAXLOOPNUMS;
					}
				}*/

			}
			//��ʱ����
			//CLIENTSTOPTIME��û�յ�ƽ����,���ýڵ��־validfΪ��Ч-��֪ͨ��Ӧ�ķ����߳��˳�
			{
				time_t curtime = time(NULL);
				if(curtime - threadinfo1->invalidtime > CLIENTSTOPTIME)
				{
					threadinfo1->validf = INVALIDF;
				}
			}
		}//�߳̿���ʱ,��ʱ����ɨ��ʵʱ���߳��˳���־
	}//while
	//�����߳������߳̿��ٵ��ڴ棬�ر�socket
	clearThread(threadno);
	threadinfo1->runf = 0;
	clntchangef	= TRUE;
	printf("��������˳�!\n");
	#ifdef WIN32
	//	AfxEndThread(TRUE);
		return TRUE;
	#else
		return(NULL);
	#endif
}



/*	����˵��
 * --------------------------------------------------
 * ������  srvwatchthread()
 * ����������
 *          1 ����SOCKET
 *          2 ���ü���WEBPORT�˿�
 * ����    ��LPVOID pParam,����δ��
 *
 * ����ֵ  ��0,���������Socketʧ�ܣ�
 *
 * --------------------------------------------------
 */
#ifdef WIN32
	UINT srvwatchthread(LPVOID pParam)
#else
	void * srvwatchthread(void *argp)
#endif
{ 
	SOCKET	listensock,srvsock/*�ܵ����ӹ����ķ�������Socket*/;
	int		maxbufsize = MAXBUFSIZE;
	int		reuseaddr=1,keepvalue=1;
    struct	sockaddr_in localaddr/*������������ַ*/,clientaddr/*�ͻ�����ַ*/;
    struct	timeval     selecttv;
    fd_set  readfds;
	int		threadno=-1;//��ǰ�߳��ڴ������߳����е����
#ifdef _AIX
	unsigned long len;
#elif __linux
	socklen_t    len;
#else
	int len;
#endif

	#ifdef WIN32
		HWND    hwnd = (HWND)pParam;//CMainFrame�Ĵ��ھ��
	#endif
	

    localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(WEBPORT);
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY); 

	selecttv.tv_sec =  0;       //1
	selecttv.tv_usec = 500*1000;		//0
 	
	//��������Socket
	listensock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//socket(int af,int type,int protocol); 
	if(listensock == SOCKET_ERROR) {
	     //AfxMessageBox("Socket����˹���ʧ��!",MB_OK|MB_ICONHAND);
		printf("Socket����˹���ʧ��\n");
	     return 0;
	}

	//����socket option
	if(setsockopt(listensock,SOL_SOCKET,SO_RCVBUF,(char *)&maxbufsize,sizeof(maxbufsize))<0)  error_abort(); //SO_RCVBUF means to Specify buffer size for receives 
	if(setsockopt(listensock,SOL_SOCKET,SO_SNDBUF,(char *)&maxbufsize,sizeof(maxbufsize))<0)  error_abort(); //SO_SNDBUF means to Specify buffer size for sends 
	if(setsockopt(listensock,SOL_SOCKET,SO_REUSEADDR,(char *)&reuseaddr,sizeof(reuseaddr))<0)   error_abort(); //SO_REUSEADDR means to Allow the socket to be bound to an address that is already in use
    if(setsockopt( listensock, SOL_SOCKET, SO_KEEPALIVE, (char *)&keepvalue, sizeof(keepvalue)) < 0 )   error_abort(); //SO_KEEPALIVE means to Send keepalives (���ӻ�Ծ)

#ifndef WIN32
 	struct timeval tv;
	tv.tv_sec = SOCKTIMEOUT;
	tv.tv_usec = 0;
	setsockopt(listensock,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv,sizeof(tv));
	setsockopt(listensock,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv,sizeof(tv));//ֻ��BSD֧��������ѡ��
#endif
	if(bind(listensock,(struct sockaddr *)&localaddr,sizeof(localaddr))<0)  error_abort(); //bind fail,exit.
    
//	int listen ( SOCKET s,int backlog );  SOMAXCONN=5;
	if(listen(listensock,SOMAXCONN) != 0) error_abort(); //places a socket a state where it is listening for an incoming connection.
	
	printf("web����������,���ڷ���״̬\n");
	int count = 0;
	/*���߳��������ӷ������˿�*/
	while(!bEndFlag) {
   		FD_ZERO(&readfds);
		FD_SET(listensock,&readfds);
		selecttv.tv_sec =  0;       //1
		selecttv.tv_usec = 500*1000;
        int characternum = select(FD_SETSIZE,&readfds,NULL,NULL,&selecttv);//&selecttv);//if &selecttv is NULL ,for blocking operation,����select����������������socket����Ϊ������
 		if(characternum>0){//characternum>1ʱ��������
			len = sizeof(clientaddr);
            srvsock = accept(listensock,(struct sockaddr *)&clientaddr,&len);
			if(srvsock == SOCKET_ERROR)
				continue;
            else {
				int ret = acceptclient(srvsock,clientaddr,&threadno);
				if(ret)
				{
					#ifdef WIN32
						int *p = new int[2];
						p[0] = threadno;
						p[1] = (int)(void*)hwnd;
						AfxBeginThread(answerclient,(LPVOID)p);
					#else
						int *p = new int[1];
						p[0] = threadno;
						pthread_t pthread_id;
						scd_pthread_create(answerclient,(void*)p,2048,&pthread_id);
					#endif
				}
				else
					sendregresult(srvsock,FALSE);//��ע��ʧ����Ϣ
			}
		}
	}
#ifdef WIN32
	ASSERT(SetEvent(m_hEvenKillWatch));
	return TRUE;
#else
	return (NULL);
#endif
}



/*	����˵��
 * --------------------------------------------------
 * ������  ��getLifeThrNums()
 * �����������õ���ͻ�����Ŀ
 * ����    ��
 *
 * ����ֵ  �� ��ͻ��˵���Ŀ
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */

int getLifeThrNums()
{
	int count = 0;
	for(int threadno =0;threadno<clientnums;threadno++)
	{
		if(threadinfo[threadno].runf == 1 && threadinfo[threadno].validf == 1)
		{
			count++;
		}//if 
	}//for
	return count;
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��killAllClient()
 * ������������������ֹͣ��Ϣ���ͻ���,ֹͣ���еĿͻ��˷����߳�
 * ����    ��
 *
 * ����ֵ  �� 
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
void killAllClient()
{
	int count = 0;
	MESSAGE msg;
	msg.type = S_SRVEXIT;//������Ϣ����
	msg.length = 0;//������Ϣ�ĳ���
	int bFirst = TRUE;
	while(bFirst || count>0)
	{
		count = 0;
		for(int threadno =0;threadno<clientnums;threadno++)
		{
			if(threadinfo[threadno].runf == 1 )
			{
				count++;
				if(bFirst)
					//��������,�ڴ˷��ǲ���ȫ�ģ������ͬһ���ͻ��ˣ������߳�ͬʱ����������Ļ���
					writemsg(threadinfo[threadno].sockid,&msg);
				threadinfo[threadno].validf = INVALIDF;
			}//if 
		}//for		
		delay_t(count*2*1000);
		bFirst = FALSE;
	}
	return;
}

