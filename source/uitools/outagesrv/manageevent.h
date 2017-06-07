///manageevent.h
///��ȡʵʱ����
//////////////////////////////////////////////////////////////////////////////////////	

#ifndef _MANAGEDAEVT_H_
#define _MANAGEDAEVT_H_

#include "rdb/rdbapi.h"
#include "globaldef.h"

#include <QList>

enum{
	FAULTPOWERCUT = 0	//�¹�ͣ��
};
typedef struct
{
	char devName[CODE_LEN];	//ͣ���豸
	int	 type;				//ͣ������
	int	 starttime;			//ͣ�翪ʼʱ��
	int	 cutType;			//�豸ʧ��ԭ��
} CUTEVENT;

class CManageEvent
{
public:
	CManageEvent();
	~CManageEvent();

	int readEvent();						//��ȡʵʱ����		
	int getEvent( CUTEVENT & event );		//��ȡһ��ͣ������
	int addEvent( CUTEVENT & event );		//���ͣ������
private:
	int m_bUsing;		//��д��ʶ
	CUTEVENT m_cutEvents[ POWERCUTEVENTNUM ]; //ͣ�������б�
	unsigned int m_curEventRP;				  //ͣ�������ָ��
	unsigned int m_curEventWP;				  //ͣ������дָ��
	EventProc m_eventproc;
};
#endif
