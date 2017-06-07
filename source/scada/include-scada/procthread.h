#ifndef _PROC_THREAD_H
#define _PROC_THREAD_H

#include  "scd/cmddef.h"

#if defined(WIN32)
_SCD_DLLPORT 
#endif
int	 IsMainServer(int groupno);		//�ж��Ƿ���SCADA��������

//ϵͳ���̹���

#ifdef WIN32
_SCD_DLLPORT 
#endif
int IsProcessOk(void);			//�ж�ĸ�����Ƿ���������

int ExecCmd(char *cmdstr);		//����ϵͳ����

typedef  void *(*PTHREAD_CALLFUNC)(void *);

#if defined(WIN32)
_SCD_DLLPORT HANDLE
#else
int 
#endif
scd_pthread_create(PTHREAD_CALLFUNC pthfunc,void *argp,int stacksize=0);//�����߳�

   
//ϵͳ������
#if defined(WIN32)
_SCD_DLLPORT 
#endif
void ScdPrintError(char *context,int errorcode);	

#if defined(WIN32)
_SCD_DLLPORT 
#endif
void ScdPrintError(int errorcode,char  *errinfo);

#endif
