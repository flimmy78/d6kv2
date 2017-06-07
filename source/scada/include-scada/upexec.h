#ifndef _USERPROC_EXEC_H
#define _USERPROC_EXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


#include "uplang.h"
#include "scadaproc.h"

#define  PCOK    0

#define  PCEXECERR_OTHER           -999999
#define  PCEXECERR_FILE            -999998
#define  PCEXECERR_INPARA          -999997
#define  PCEXECERR_ARRAY           -999996

typedef struct {
      int stackp; //ջָ�� 
      void *stackpt[100]; //ջ���� voidָ������
   } PARASTACK;


typedef struct {
     char  tabname[TABNAME_LEN]; //����
     char  objectname[OBJECTNAME_LEN];//������
     char  fieldname[FLDNAME_LEN];  //����
     RDB_VALUETYPE_STRU  mnvalue;          //��ͬ����ֵ����
   } PCMNOP_VALUE;

#define  MAXPCMNOPNUMS  10

typedef struct {
      void * point;
      DATATYPE datatype;
   } OBJECT;


class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Pcexec {
   char  procname[256]; //������
   PARASTACK     *parastack;    //��ջ
   OBJECT		 object;
   PCINSTRUMENT  *pcinstru;     //ָ��
   char          *dataaddr;     //����ָ��
   uchar         crparastackf;  //��������ջ��־
   CALLPARA      externinpara;  //�ⲿ�������
   int           mnopvalnums;   //ģ�����ֵ����
   PCMNOP_VALUE  mnopvalue[MAXPCMNOPNUMS]; //ģ�����ֵ�ṹ
   APEvent       *apeventp;     //Ӧ���¼�ָ��
   uchar         crapeventf;    //����Ӧ���¼���־
   int           apentcode;     //Ӧ���¼�����
   SYS_CLOCK     apentclock;    //Ӧ���¼�ʱ��
   uhlong        apentmsg[MAXAPENTMSG_LEN/sizeof(uhlong)+1]; //Ӧ���¼���Ϣ
   int  loadpcinstrument(FILE *fp,PROC_HEAD *phd);//���ع���ָ��
   int  callfunction(int  funcno,CALLPARA  *cpp,double *raddr); //���ù��ܺ�
   int  get_apevent(void); //ȡ�¼�
   void reset_apevent(void); //��λ�¼�ָ��
   int  callbasisfunc(int  funcno,CALLPARA  *cpp,double *raddr);
   int  call_hiscomnfunc(int  funcno,CALLPARA  *cpp,double *raddr);
   int  call_ykfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr); //��YK����
   int  call_rsqlfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr);//��SQL��ѯ����
   int  call_memtblfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr);//��ʵʱ�⺯��
   int  call_apentfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr); //��Ӧ���¼��ص�����
   int  call_hisdataopfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr); //����ʷ���ݴ�����
   int  call_histableopfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr); //����ʷ���ݴ�����
   int  call_timetypefunc(int  funcno,CALLPARA  *cpp,double *raddr);//ʱ�����Ժ���
   int  call_adduserentfunc(int  funcno,CALLPARA  *cpp,double *raddr,int groupno); //����û��¼�����
   int  call_fileopfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr);
 public:
   Pcexec(PARASTACK *psp=NULL,APEvent  *apep=NULL);  //���캯��
  ~Pcexec(void);
   void push_parastack(void *pt);
   void *pop_parastack(void);
   void *get_parastack(int orderno);
virtual int  callappfunc(int  funcno,CALLPARA  *cpp,double *raddr);//
virtual int  callprocedure(PARASTACK *,CALLPARA  *cpp,double *raddr);
   int  exec_pcexecfile(char *pcexecfname,CALLPARA *callparap=NULL,double *resultaddr=NULL);
   int  exec_instrument(int start_instruno,PCINSTRUMENT *insp,char  *database,double *resultaddr);
   int  push_inpara(DATATYPE datatype,void *paraaddr);
   int  call_pcexecfile(char *pcexecfname,double *resultaddr);
   int  call_pcexecfile(char *pcexecfname,double *resultaddr,int datatype,...);
   int  set_pcmnopvalue(int mnopnums,PCMNOP_VALUE *mnopvalp);
   int  set_pcmnopvalue(char *tabname,char *objectname,char *fieldname,uchar valtype,void *valaddr);
   int  get_pcmnopvalue(char *tabname,char *objectname,char *fieldname,RDB_VALUETYPE_STRU *mnval);
   int  get_pcmnopvalue(int *mnopnums,PCMNOP_VALUE **mnopvalp);
virtual   void PrintOutString(char *s);
   };
   

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Userpcexec : public Pcexec {
   char          *hostname;
   char          *username;
   char          *password;
   long          taskid;
public:
   Userpcexec(char *hm,char *um, char *pw, long tid,PARASTACK *psp=NULL,APEvent  *apep=NULL);
virtual int  callappfunc(int  funcno,CALLPARA  *cpp,double *raddr);
virtual int  callprocedure(PARASTACK *,CALLPARA  *cpp,double *raddr);
   };



#endif
