#ifndef _GLOBLALDEF_H
#define _GLOBLALDEF_H

#define REALDATA_BUF_SIZE	100 //ʵʱ���ݶ������
#define POWERCUTEVENTNUM	50  //�¹�����������

void ReadStreamData(int);
void pwrmngQuit(int);

void* EvtProc(void* pVoid);

#endif
