#ifndef _TOPO_H
#define _TOPO_H

#include "sysdef.h"

typedef struct
{
	char  name[CODE_LEN];
	unsigned char type;
	char  hstation[CODE_LEN];
	char  tstation[CODE_LEN];
	unsigned char vol1;
	unsigned char vol2;
	unsigned char vol3;
	unsigned char vol4;
	char  node1[CODE_LEN];
	char  node2[CODE_LEN];
	char  node3[CODE_LEN];
	char  node4[CODE_LEN];
	unsigned char power_flag;
	char  obligate[32];
	char servicestatus;//Ͷ��״̬
	int feederno;	//�������
	int nodeActive1;
	int nodeActive2;
	int nodeActive3;
	int nodeActive4;
	int realisland;	//ʵʱ����
	int active;
	int realfeederno; //ʵʱ���ߺ�
	int runstate;	//����״̬
	int statetime;	//����״̬��ʼʱ��
	unsigned char lossreason;	//ʧ��ԭ��
	int colorstate;//��ɫ״̬

}TPRCD,*PTPRCD;

#endif	