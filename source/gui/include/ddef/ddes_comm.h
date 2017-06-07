#ifndef DDES_COMM_H_HEADER_INCLUDED_BCB88C6E
#define DDES_COMM_H_HEADER_INCLUDED_BCB88C6E

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <QtCore/QtGlobal>

#include "dllinc.h"

//##ModelId=433A2B350222
//##Documentation
//## ���壺
//##      �޷���8λ����
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef quint8 uint8;



//##ModelId=433A2BF70128
//##Documentation
//## �з���8λ����
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef qint8 int8;

//##ModelId=433A2B330242
//##Documentation
//## ���壺
//##      �޷���16λ����
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef quint16 uint16;

//##ModelId=433A2BF5003E
//##Documentation
//## ���壺
//##      �з���16λ����
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef qint16 int16;

//##ModelId=433A2B300148
//##Documentation
//## ���壺
//##      �޷���32λ����
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef quint32 uint32;

//##ModelId=433A2BF0031C
//##Documentation
//## ���壺
//##      �з���32λ����
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef qint32 int32;

//##ModelId=433F8A670186
//##Documentation
//## ���壺
//##      �޷���64λ����
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef quint64 uint64;

//##ModelId=433F8A7501A5
//##Documentation
//## ���壺
//##      �з���64λ����
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef qint64 int64;

//##ModelId=433A2D790290
//##Documentation
//## ���壺
//##      32λ������
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef float FLOAT4;

//##ModelId=433A2D7D009C
//##Documentation
//## ���壺
//##       64λ������
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef double FLOAT8;

//##ModelId=433A2D7D009C
//##Documentation
//## ���壺
//##       ָ������
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef void *GHANDLE, *GWPARAM,*GLPARAM;

//##ModelId=433A32740157
//##Documentation
//## ���壺
//##      λ��ָ�룬�����б��ѯ������
//## ���ܣ�
//##       ʵ�ֿ�ƽ̨���ݶ��壬ͳһ����������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
typedef struct __tagGPOS{} *GPOS;

GDE_EXPORT_DEF uint8 g_check_ptr( uint8 c, const char *n, int l );

//##ModelId=433F8EA802FD
enum  { 
    //##Documentation
    //## ���볤��
    GDB_CODE_LEN = 24, 
    //##Documentation
    //## ��������
    GDB_DESC_LEN = 40, 
    //##Documentation
    //## �ļ�·������
    GDB_PATH_LEN = 256, 
    //##Documentation
    //## �ַ�����������
    GDB_STR_LEN = 1024, 
    //##Documentation
    //## �������Ƴ���
    GDB_FMT_LEN = 132 };

#define r_RGB(rgb)	((uint8)(rgb))
#define g_RGB(rgb)  ((uint8)(((uint16)rgb)>>8))
#define b_RGB(rgb)	((uint8)((rgb)>>16))
//BBGGRR
#define c_RGB(r,g,b) ((uint32)(((uint8)(r)|((uint16)((uint8)(g))<<8)|((uint32)((uint8)(b))<<16))))	
#define q_RGB(rgb) r_RGB(rgb),g_RGB(rgb),b_RGB(rgb) 

#define g_MAKEWORD(low, high)	((uint16)(((uint8)(low)) | ((uint16)((uint8)(high))) << 8))
#define g_MAKELONG(low, high)	((uint32)(((uint16)(low)) | ((uint32)((uint16)(high))) << 16))
#define g_LOWORD(l)	((uint16)(l))
#define g_HIWORD(l)	((uint16)(((uint32)(l) >> 16) & 0xFFFF))
#define g_LOBYTE(w)	((uint8)(w))
#define g_HIBYTE(w)	((uint8)(((uint16)(w) >> 8) & 0xFF))
#define g_MAX(a,b)	(((a)>(b))?(a):(b))
#define g_MIN(a,b)  (((a)>(b))?(b):(a))
#define g_ABS(a)	(((a)>=0)?(a):-(a))
#define g_ROUND(a)  (int(a))
#define g_SQUARE(x) ((x)*(x))
#define g_BETWEEN(c,a,b) (((c)>=g_MIN(a,b))&&((c)<=g_MAX(a,b)))
#define g_GT(a,b) ((a)>(b))
#define g_LT(a,b) ((a)<(b))
#define g_GE(a,b) ((a)>=(b))
#define g_LE(a,b) ((a)<=(b))
#define g_RANGE(c,a,b) (g_BETWEEN(c,a,b)?0:(g_GT(c,g_MAX(a,b))?1:-1))

#define g_Q(str) (QString::fromLocal8Bit(str))	// 	[1/14/2013 ljs116]
#define q_G(qs)  ((const char*)((qs).toLocal8Bit().data())) //  [2/27/2013 ljs]

#define _Q2C(str) ((const char *)((str).toLocal8Bit().data())) //  [1/15/2013 ljs]
#define _C2Q(str) (QString::fromLocal8Bit(str))

#define g_NEPS    3.55271367880050092935562E-15 
#define g_MEPS    3.55271367880050092935562E+15 
#define	g_PI	  3.1415926535897932384626433832795
#define g_RAD2DEG 57.295779513082320876798154814114
#define g_DEG2RAD 0.017453292519943295769236907684883 	// pi/180
#define g_CHECK_PTR(p) g_check_ptr((p==NULL),__FILE__,__LINE__)

#endif /* ddes_COMM_H_HEADER_INCLUDED_BCB88C6E */
