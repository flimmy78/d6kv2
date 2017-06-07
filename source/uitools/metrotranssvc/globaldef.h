#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#include "sysstr.h"
#include "netapi.h"
#include "sysdef.h"


#define _Q2C(str) ((const char *)(str.toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

#define  DES_LEN	64
#define EVENT_VALID_GAP			15 ////�յ������ʱ��������ķ���ʱ�����Ч���
#define DINAMIC_BUF_BLOCK_SIZE	100 ///��100��Ϊһ���ڴ浥λ���ж�̬�����ڴ�


typedef struct
{
	SYS_TIME recvTime;//DA�����յ������ʱ�䣡
	SYS_EVENT evt;
} EX_SYS_EVENT;


#endif
