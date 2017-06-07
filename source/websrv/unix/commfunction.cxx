/*
 * --------------------------------------------------
 *   �ļ�����commfunction.cxx 
 *   �������DS3210�������ϵͳWeb������   
 *   �ļ���Ҫ���ݣ����ú���
 *
 *   Copyright , 2002
 *   �汾�ţ�v1.0.0,	2002/06/03
 *					 
 *   ����ˣ�
 *	 ˵��
 * --------------------------------------------------
 */


#ifdef WIN32
#include "stdafx.h"
#endif

#include "message.h"


/*	����˵��
 * --------------------------------------------------
 * ������  ��delay_t()
 * ������������ʱ����
 * ����    ��
 *		   n=1/1000 ms(����)unix,windows�ĵ�λ����
 *
 * ����ֵ  ��
 *
 *
 * --------------------------------------------------
 */


void delay_t(int n)
{
	_delay_time( n );
/*
#if defined (WIN32)
        Sleep(n/1000);
#elif defined (__unix)
		struct timeval tt;
		int    ret;
		while(TRUE)
		{			
			tt.tv_sec=n/1000000;
			tt.tv_usec=n%1000000;
			ret=select(0,0,0,0,&tt);
			if(ret<0){
				if(errno==EINTR)
				{
						n=n/2;
						if(n>=50000)	continue;
				}
			}
			break;
		}
#endif*/
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��pro_char2short()
 * �������������ֽڴ�ת��Ϊshort
 * ����    ��
 *           charp=�ֽڴ�ָ��(Դ)
 *           shortp=���ָ��(Ŀ��)
 *
 * ����ֵ  �� 
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */

void  pro_char2short(char *charp,short *shortp)
{
	char *tmpcharp;
	tmpcharp=(char *)shortp;
#if !defined(WIN32) && !defined(__linux)
	tmpcharp[0]=charp[1];
	tmpcharp[1]=charp[0];
#else
	tmpcharp[0]=charp[0];
	tmpcharp[1]=charp[1];
#endif
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��pro_char2int()
 * �������������ֽڴ�ת��Ϊint
 * ����    ��
 *           charp=�ֽڴ�ָ��
 *           inttp=���ָ��
 *
 * ����ֵ  ��  
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
void pro_char2int(char *charp,int *intp)
{
	char *tmpcharp;
	int i;
 
	tmpcharp=(char *)intp;
	int intsize = sizeof(int);
	for(i=0;i<intsize;i++)  {
#if !defined(WIN32) && !defined(__linux)
		tmpcharp[i]=charp[intsize-1-i]; 
#else
		tmpcharp[i]=charp[i]; 
#endif
	}
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��pro_char2long()
 * �������������ֽڴ�ת��Ϊlong
 * ����    ��
 *           charp=�ֽڴ�ָ��
 *           longp=���ָ��
 *
 * ����ֵ  �� 
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
/*void pro_char2long(char *charp,__int64 *longp)
{
	char *tmpcharp;
	int i;
 
	tmpcharp=(char *)longp;
	int longsize = sizeof(__int64);
	for(i=0;i<longsize;i++)  {
#ifdef ALPHA_PLATFORM
		tmpcharp[i]=charp[longsize-1-i]; 
#else
		tmpcharp[i]=charp[i]; 
#endif
	}
}*/

/*	����˵��
 * --------------------------------------------------
 * ������  ��pro_char2float()
 * �������������ֽڴ�ת��Ϊfloat
 * ����    ��
 *           charp=�ֽڴ�ָ��
 *           floatp=���ָ��
 *
 * ����ֵ  ��  
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
void  pro_char2float(char *charp,float *floatp)
{
	char *tmpcharp;
	int i;

	tmpcharp=(char *)floatp;
	int floatsize = sizeof(float);
	for(i=0;i<floatsize;i++){
#if !defined(WIN32) && !defined(__linux)
		tmpcharp[i]=charp[floatsize-1-i]; 
#else
		tmpcharp[i]=charp[i]; 
#endif
	}
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��pro_char2double()
 * �������������ֽڴ�ת��Ϊfloat
 * ����    ��
 *           charp=�ֽڴ�ָ��
 *           doublep=���ָ��
 *
 * ����ֵ  ��  
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
void  pro_char2double(char *charp,double *doublep)
{
	char *tmpcharp;
	int i;

	tmpcharp=(char *)doublep;
	int doublesize = sizeof(double);
	for(i=0;i<doublesize;i++){
#if !defined(WIN32) && !defined(__linux)
		tmpcharp[i]=charp[doublesize-1-i]; 
#else
		tmpcharp[i]=charp[i]; 
#endif
	}
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��pro_short2char()
 * ������������shortת��Ϊ�ֽڴ�
 * ����    ��
 *           shortp=Դָ��
 *           charp=�ֽڴ�ָ��(Ŀ��)
 *
 * ����ֵ  ��  
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
void  pro_short2char(short *shortp,char *charp)
{
	char *tmpcharp;

	tmpcharp=(char *)shortp;
#if !defined(WIN32) && !defined(__linux)
	charp[0]=tmpcharp[1];
	charp[1]=tmpcharp[0];
#else
	charp[0]=tmpcharp[0];
	charp[1]=tmpcharp[1];
#endif

}

/*	����˵��
 * --------------------------------------------------
 * ������  ��pro_int2char()
 * ������������intת��Ϊ�ֽڴ�
 * ����    ��
 *           intp=Ŀ��ָ��
 *           charp=�ֽڴ�ָ��
 *
 * ����ֵ  ��  
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
void pro_int2char(int *intp,char *charp)
{
	char *tmpcharp;
	int i;

	tmpcharp=(char *)intp;
	int intsize = sizeof(int);
	for(i=0;i<intsize;i++)  {
#if !defined(WIN32) && !defined(__linux)
           charp[i]=tmpcharp[intsize-1-i];
#else
           charp[i]=tmpcharp[i];
#endif
	}
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��pro_long2char()
 * ������������longת��Ϊ�ֽڴ�
 * ����    ��
 *           longp=Ŀ��ָ��
 *           charp=�ֽڴ�ָ��
 *
 * ����ֵ  ��  
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
/*void pro_long2char(__int64 *longp,char *charp)
{
	char *tmpcharp;
	int i;

	tmpcharp=(char *)longp;
	int longsize = sizeof(__int64);
	for(i=0;i<longsize;i++){
#ifdef ALPHA_PLATFORM
           charp[i]=tmpcharp[longsize-1-i];
#else
           charp[i]=tmpcharp[i];
#endif
	}
}*/

/*	����˵��
 * --------------------------------------------------
 * ������  ��pro_float2char()
 * ������������floatת��Ϊ�ֽڴ�
 * ����    ��
 *           floatp=Ŀ��ָ��
 *           charp=�ֽڴ�ָ��
 *
 * ����ֵ  ��  

 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
void  pro_float2char(float *floatp,char *charp)
{
     char *tmpcharp;
     int i;

	tmpcharp=(char *)floatp;

	int floatsize = sizeof(float);
	for(i=0;i<floatsize;i++){
#if !defined(WIN32) && !defined(__linux)
          charp[i]=tmpcharp[floatsize-1-i];
#else
          charp[i]=tmpcharp[i];
#endif
	}
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��pro_double2char()
 * ������������floatת��Ϊ�ֽڴ�
 * ����    ��
 *           doublep=Ŀ��ָ��
 *           charp=�ֽڴ�ָ��
 *
 * ����ֵ  ��  

 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
void  pro_double2char(double *doublep,char *charp)
{
     char *tmpcharp;
     int i;

	tmpcharp=(char *)doublep;

	int doublesize = sizeof(double);
	for(i=0;i<doublesize;i++){
#if !defined(WIN32) && !defined(__linux)
          charp[i]=tmpcharp[doublesize-1-i];
#else
          charp[i]=tmpcharp[i];
#endif
	}
}
      
/*	����˵��
 * --------------------------------------------------
 * ������  ��GetDateString()
 * ������������ȡ�����ַ���
 * ����    ��
 *
 * ����ֵ  �� 
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
char*  GetDateString()
{
	time_t ntim;
	struct tm *tt;
    time(&ntim);
    tt=localtime(&ntim);
    return(asctime(tt));
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��GetDate()
 * ����������ȡ����ָ���ֶε�ֵ
 * ����    ��
 *			mode=ָ���ֶ�
 * ����ֵ  �� 
 *          ָ���ֶε�ֵ   
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
int  GetDate(char *mode)
{
	time_t ntim;
	struct tm *tt;

    time(&ntim);
    tt=localtime(&ntim);
  
    if(strcmp(mode,"year")==0)
       return  tt->tm_year;
    else if(strcmp(mode,"mon")==0)
       return  tt->tm_mon+1;
    else if(strcmp(mode,"day")==0)
       return  tt->tm_mday;
    else if(strcmp(mode,"hour")==0)
       return  tt->tm_hour;
    else if(strcmp(mode,"min")==0)
       return  tt->tm_min;
    else if(strcmp(mode,"sec")==0)
       return  tt->tm_sec;
    else if(strcmp(mode,"week")==0)
       return  tt->tm_wday;
	return TRUE;
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��get_date()
 * ����������ȡ����ֵ
 * ����    ��
 *			retv=���������ָ��
 * ����ֵ  �� 
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
void  get_date(int* retv)
{
	    time_t ntim;
    	struct tm *tt;

        time(&ntim);
        tt=localtime(&ntim);
  
        *retv= tt->tm_year;
        *(++retv)= tt->tm_mon+1;
        *(++retv)= tt->tm_mday;
        *(++retv)= tt->tm_hour;
        *(++retv)= tt->tm_min;
        *(++retv)= tt->tm_sec;
        *(++retv)=tt->tm_wday;
}



/*	����˵��
 * --------------------------------------------------
 * ������  ��deleteBlanks()
 * ����������ɾ���ַ���ͷ���Ŀո�
 * ����    ��
 *           str=Ŀ���ַ���
 * ����ֵ  �� 
 *           ����ַ��� 
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
char* deleteBlanks(char *str)
{
	char *tmpp;
	tmpp=str;
	if(tmpp==NULL)
		return NULL;
	while(*tmpp==' ' || *tmpp=='\t') tmpp++;
	return tmpp;
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��startsWith()
 * ��������������ַ���str�Ƿ����ַ���prefix��ͷ
 * ����    ��
 *           str=Ŀ���ַ���
 *           prefix=�����ַ���
 * ����ֵ  �� 
 *           1=�� 
 *           0=�� 
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
char startsWith(char *str,char *prefix)
{
	int i;
	int len=strlen(prefix);
    for(i=0;i<len;i++){
		if(str[i]!=prefix[i])
			break;
	}
	return i==len ? 1:0;
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
 * ������  ��indexOf()
 * ��������������ַ���index���ַ���str�г��ֵ�λ��
 * ����    ��
 *           str=Ŀ���ַ���
 *           index=�����ַ���
 * ����ֵ  �� 
 *             
 *           NULL=��Ч 
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
char* indexOf(char *str,char *index) /*�޸����������*/
{
	char *tmp=strstr(str,index);
	char *val=deleteBlanks((char*)(tmp+strlen(index)));
	if(val==NULL)
		return NULL;
	if(val[strlen(val)-1]=='\n')
		val[strlen(val)-1]='\0';
	else if((tmp=strchr(val,' '))!=NULL)
		*tmp='\0';
	return val;
}


/*	����˵��
 * --------------------------------------------------
 * ������  ��getStringValue()
 * ������������ȡһ�����ַ���
 * ����    ��
 *
 * ����ֵ  �� 
 *            
 * ��ע    �� 
 *
 * --------------------------------------------------
 */
char* getStringValue(char *str,char *prefix,char *suffix) /*�޸����������*/
{
	char *prep,*sufp;
	
	prep=strstr(str,prefix);
	if(prep==NULL)
		return NULL;
	sufp=strstr(str,suffix);
	if(sufp==NULL)
		return NULL;
	if(sufp<prep)
		return NULL;

	*sufp='\0';
	return deleteBlanks((char*)(prep+1));
}



/*	����˵��
 * --------------------------------------------------
 * ������  ��proc_head()
 * ������������ͷ����
 * ����    ��
 *		   rd=���ݻ�����
 *		   opt=ģʽ('r'=���� 'w'=����)
 *		   id=�ͻ������߳�ID��
 *
 * ����ֵ  ��
 *
 *
 * --------------------------------------------------
 */
int  proc_head(char *rd,char opt,MESSAGE *msgbuf) 
{
     if(opt=='r') 
	 {
		pro_char2int(&(rd[0]),&(msgbuf->type));
		pro_char2int(&(rd[4]),&(msgbuf->length));
		msgbuf->registerf=rd[8];
	  }
      else if(opt=='w') 
	  {
		pro_int2char(&(msgbuf->type),&(rd[0]));
		pro_int2char(&(msgbuf->length),&(rd[4]));
		rd[8]=msgbuf->registerf;
	  }
	  return TRUE;
} 


/*	����˵��
 * --------------------------------------------------
 * ������  ��recv_data()
 * ������������SOCKET��ȡָ�����ȵ�����
 *           
 * ����    ��
 *         sockid=SOCKET��ID�� 
 *         buff= ���ܻ�����ָ��
 *         len=  ָ����ȡ���ݳ���
 *
 * ����ֵ  ���ɹ�������>=0
 *           ʧ�ܣ�����-1
 * --------------------------------------------------
 */
 int  recv_data(SOCKET sockid,char *buff,int len) 
 {
	int retnums=-1,rettimes=0;
	int max_count = 10*(1+len/MAXBUFSIZE);//40*(1+len/MAXBUFSIZE);
    char *tmpbuf;
    int tmplen=0;  

    struct timeval  delay;
    delay.tv_sec=0;
    delay.tv_usec=1000;
	

    tmpbuf=buff;
	if(buff == NULL) 
		return -1;
    while(TRUE) { 
		delay.tv_sec=0;
		delay.tv_usec=1000;
		retnums = recv(sockid,tmpbuf,len,0); //If the connection has been gracefully closed, the return value is zero
		if(retnums>0) { 
             tmplen+=retnums;
             len-=retnums;     
             tmpbuf+=retnums;
             if(len<=0)
				 break;
             else
		         select(0,0,0,0,&delay);
		} 
        else if(retnums == SOCKET_ERROR) { 
			if((errno == EINTR)||(errno == EWOULDBLOCK)||(errno == 0))
			{ 
				if((rettimes++)>=max_count) break;
				delay_t(50000);
				continue;
			 } 
             else {
                  return -1;
			 } 
		}
        else {
             return -1;
		} 
	} /*while*/
  return tmplen;
}

/*	����˵��
 * --------------------------------------------------
 * ������  ��send_data()
 * �����������ڲ�����
 *           ��SOCKETд��ָ�����ȵ�����
 * ����    ��
 *         sockid=SOCKET��ID�� 
 *         buff= ���ͻ�����ָ��
 *         len=  ָ���������ݳ���
 *
 * ����ֵ  ���ɹ�������>=0
 *           ʧ�ܣ�����-1
 *
 * --------------------------------------------------
 */
int send_data(SOCKET sockid,char *buf,int len) 
{
    int retn=-1;
	if(sockid<0) return -1;
	char *ptr = (char *)buf;
	int max_count = 10*(1+len/MAXBUFSIZE);//40*(1+len/MAXBUFSIZE);
	int txn = 0;
	if(len<=0) return 0;
	if(buf==NULL) return -1;
	int txntime = 0;
	while(TRUE)
	{
		txn = send(sockid,ptr,len,0);
		if(txn==SOCKET_ERROR)
		{
			if((errno == EINTR)||(errno == EWOULDBLOCK)||(errno == 0))
			{
				if((txntime++)<max_count){
					delay_t(50000);
					continue;
				}
			}
			return -1;
		}
		else if(txn==0){
			return -1;
		}
		txntime=0;
		len -= txn;
		ptr += txn;
		if(len<=0) return 0;
	}
	return retn;
}

/*	����˵��
 * --------------------------------------------------
 * ������  writemsg()
 * ��������������һ��������(��ͷ+����),(���������ڷ��ͻ�����).
 * ����    ��id=�ͻ������߳�ID��
 *
 * ����ֵ  ���ɹ�������0
 *           ʧ�ܣ�����-1
 *int readmsg(SOCKET sockid,MESSAGE *msgbuf)
 * --------------------------------------------------
 */
int  writemsg(SOCKET sockid,MESSAGE *msgbuf) 
{
	  
	   //��ͷ�ͱ���һ����
	   int    ret;
       char   tmpbuf[MSGHEADLEN];

	   int length = msgbuf->length+MSGHEADLEN;
	   memset(tmpbuf,0,MSGHEADLEN);
       proc_head(tmpbuf,'w',msgbuf);
	   memcpy((char*)msgbuf,tmpbuf,MSGHEADLEN);
       ret=send_data(sockid,(char *)msgbuf,length);
       if(ret==-1) {
             printf("���ݷ���ʧ��");
             return -1 ;
	   }
	   return ret;
}



/*	����˵��
 * --------------------------------------------------
 * ������  ��readmsg()
 * ������������ȡһ��������(��ͷ+����),������ܻ�������
 * ����    ��id=�ͻ������߳�ID��
 *
 * ����ֵ  ���ɹ�,����0
 *           ʧ��,����-1
 *
 * --------------------------------------------------
 */
int readmsg(SOCKET sockid,MESSAGE *msgbuf)
{
       int    ret;
       char		tmpbuf[MSGHEADLEN];

	   memset(tmpbuf,0,MSGHEADLEN);
       ret=recv_data(sockid,tmpbuf, MSGHEADLEN);//�ڿͻ����ж�ʱ������0
	   if(ret <= 0)
		   return -1;
       ret=proc_head(tmpbuf,'r',msgbuf);
       if(ret==-1) 
     	    return -1;
       if(msgbuf->length>0 && msgbuf->length<MAXBUFSIZE) {
           ret=recv_data(sockid,(char *)msgbuf->messagebuf,msgbuf->length);
		   if(ret <= 0)
	           return -1;;
	   }
	   else if(msgbuf->length >= MAXBUFSIZE)
	   {
		   msgbuf->hugebuf = (char*)malloc(msgbuf->length);
		   if(msgbuf->hugebuf == NULL) 
			   return -1;
           ret=recv_data(sockid,(char *)msgbuf->hugebuf,msgbuf->length);
		   if(ret <= 0)
	           return -1;;
	   }
      return 0;
}

#ifndef WIN32
int GetEnvironmentVariable(char *env,char *envvar,int size)
{
	char * envtmp = getenv(env);
	if(envtmp == NULL || envtmp[0] == '\0') 
	{
		strcpy(envvar,"/export/home/ds3100");
	}
	else
		strcpy(envvar,envtmp);
	return strlen(envvar);
}
#endif
//ȡ��������
//����������������Զ�ȥ�������"\"
void getEnv(char *envvar)
{
	int num = GetEnvironmentVariable("NBENV", envvar, 256-1);

	if(num == 0)
	{
		strcpy(envvar,"c:/ds3210");
	}
	else
	{
		char post[4];
		memset((char*)post,0,4);
		strcpy(post,"/");
		char rtn = endsWith((char*)envvar,(char*)post);
		if(rtn)
			envvar[strlen(envvar)-1]='\0';//ȥ�������"\"
	}
	return;
}
