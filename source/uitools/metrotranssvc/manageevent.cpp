#include "manageevent.h"
#include "fileinfo.h"

extern int gbNeedStart;
extern int bDaQuit;
extern int GroupNo;


Mng_event::Mng_event()
{
	m_bUsing = FALSE;
	m_pEvt = NULL;
	m_iEvtNum = 0;
	m_iBufSize = 0;
	m_bNeedClearBuf = TRUE;
}

Mng_event::~Mng_event()
{
	//m_bUsing = FALSE;
	if(m_pEvt != NULL)
		delete m_pEvt;
	m_iEvtNum = 0;
}

int Mng_event::add_event(EX_SYS_EVENT& evt)
{
	while (m_bUsing == TRUE)
		;
	m_bUsing = TRUE;
	///////////���ڴ���й���
	//ɾ����ʱ�������̬��������

	/////�����ʱ������
	//#ifdef _WINDOWS
	SYS_TIME Origintime;
	TimeProc timeProc;

	if(gbNeedStart != FALSE && m_bNeedClearBuf != FALSE)
	{
// 		if(bDebug == FALSE)
			Origintime = m_timeFirstFault;
// 		else
// 			Origintime = Mail.m_timeFirstFault;
	}
	else//��Ϊ��ǰ��ʱ��
		timeProc.GetTime(Origintime);
	//#else

	//#endif
	if(m_bNeedClearBuf != FALSE)
	{
		for(int i = 0 ; i < m_iEvtNum ; i++)
		{
			int32s gap = Origintime - (m_pEvt+i)->recvTime;
// 			if( (int) gap > (DaSetting.m_pDasettingAddr->BeforeFaultSecs * 1000))
			{
				*(m_pEvt + i) = *(m_pEvt + m_iEvtNum - 1);
				m_iEvtNum --;
				i -- ;
			}
		}
		if(gbNeedStart != FALSE)
			m_bNeedClearBuf = FALSE;
	}
	/////��̬�����ڴ棬������������
	if(m_iEvtNum == m_iBufSize)
	{
		if(m_pEvt == NULL)
		{
			m_pEvt = new EX_SYS_EVENT[DINAMIC_BUF_BLOCK_SIZE];
			if(m_pEvt == NULL)
			{
#ifdef _WINDOWS
				TRACE("�ڴ����ʧ��\n");
#else
				printf("�ڴ����ʧ��\n");
#endif
				bDaQuit = TRUE;
				return FALSE;
			}
		}
		else
		{
			EX_SYS_EVENT *m_ptmpEventBuf = new EX_SYS_EVENT[m_iEvtNum];
			if(m_ptmpEventBuf == NULL)
			{
#ifdef _WINDOWS
				TRACE("�ڴ����ʧ��\n");
#else
				printf("�ڴ����ʧ��\n");
#endif
				bDaQuit = TRUE;
				return FALSE;
			}
			memcpy(m_ptmpEventBuf,m_pEvt,m_iEvtNum*sizeof(EX_SYS_EVENT));
			delete[] m_pEvt;
			m_pEvt = new EX_SYS_EVENT[m_iBufSize + DINAMIC_BUF_BLOCK_SIZE];
			if(m_pEvt == NULL)
			{
#ifdef _WINDOWS
				TRACE("�ڴ����ʧ��\n");
#else
				printf("�ڴ����ʧ��\n");
#endif
				bDaQuit = TRUE;
				return FALSE;
			}
			memcpy(m_pEvt,m_ptmpEventBuf,m_iEvtNum*sizeof(EX_SYS_EVENT));
			delete [] m_ptmpEventBuf;
			m_ptmpEventBuf = NULL;		
		}
		m_iBufSize += DINAMIC_BUF_BLOCK_SIZE;
	}
	memcpy((m_pEvt + m_iEvtNum),&evt,sizeof(EX_SYS_EVENT)); 
	m_iEvtNum = m_iEvtNum+1;
	m_bUsing = FALSE;
	return TRUE;
}

int Mng_event::read_event(File_info fileInfo)
{
	int flag;
	SYS_EVENT evt;
	for( ; ; )
	{
		flag = m_eventproc.Read(&evt);

		if(flag == 0) 
		{
			break;
		}

		int type = (int)evt.type ;
/*		if( (type >= ENTTYPE_POWERSYS) && \
			(type <= ENTTYPE_PROTECT) && \
			(evt.state == 1) && \
			(!strcmp(evt.member1,"SOE")))
		{
#ifdef __PROGRAM_DEBUG_
			TRACE("�յ�DA��������ϼ������!\n");
#endif
			/////////
			TimeProc timeProc;
			SYS_TIME time_recv;
			timeProc.GetTime(time_recv);
			SYS_TIME time_create;
			time_create.year = evt.ymd/10000;
			time_create.mon = (evt.ymd%10000)/100;
			time_create.day = evt.ymd%100;
			time_create.hour = evt.hmsms/10000000;
			time_create.min = (evt.hmsms%10000000)/100000;
			time_create.sec = (evt.hmsms%100000)/1000;
			time_create.ms = evt.hmsms%1000;
			int32s gap = time_recv - time_create;
			if(abs((int)gap) <= EVENT_VALID_GAP * 1000)
			{

				EX_SYS_EVENT exevt;
				exevt.evt = evt;
				exevt.recvTime = time_recv;
				if(add_event(exevt) == FALSE)
					return 1;
			}
		}
*/

// 		if( type == ENTTYPE_DIAG_S_GND)
		if( (type >= ENTTYPE_POWERSYS) && (type < ENTTYPE_PROTECT))
		{
#ifdef __PROGRAM_DEBUG_
			TRACE("�յ�DA������¹���բ����!\n");
#endif
			TimeProc timeProc;
			SYS_TIME time_recv;
			timeProc.GetTime(time_recv);
			SYS_TIME time_create;
			time_create.year = evt.ymd/10000;
			time_create.mon = (evt.ymd%10000)/100;
			time_create.day = evt.ymd%100;
			time_create.hour = evt.hmsms/10000000;
			time_create.min = (evt.hmsms%10000000)/100000;
			time_create.sec = (evt.hmsms%100000)/1000;
			time_create.ms = evt.hmsms%1000;
			int32s gap = time_recv - time_create;
			int iGap = (int)gap;
			if(abs((int)gap) <= EVENT_VALID_GAP * 1000)
			{				
				int i;
				for(i = 0 ; i < fileInfo.m_triggercond_num ; i ++)
				{
					if(!strcmp(evt.member0,fileInfo.m_faultcond_list[i].code) && (evt.val==fileInfo.m_faultcond_list[i].state))
						break;
				}

				if(i != fileInfo.m_triggercond_num)
				{
// 					if(bDebug == FALSE)
// 					{
						if(gbNeedStart == FALSE)
						{
							timeProc.GetTime(m_timeFirstFault);
							m_timeLastFault = m_timeFirstFault;

// 							Stream.BackupingData();

							gbNeedStart = TRUE;
						}
						else
						{
							timeProc.GetTime(m_timeLastFault);
						}
// 					}

					EX_SYS_EVENT exevt;
					exevt.evt = evt;
					exevt.recvTime = time_recv;

					if(add_event(exevt) == FALSE)
						return 1;
				}
			}
		}
	}
	return 0;
}

void Mng_event::release_buf()
{
	while (m_bUsing == TRUE)
		;
	m_bUsing = TRUE;


	m_bUsing = FALSE;
}

void Mng_event::move_buf(EX_SYS_EVENT** pEvt , int* pEvtNum)//����ָ����ָ���ָ������ڴ��ͷ��ɵ��������
//�β���ָ��ָ���ʼΪһ��NULLָ�룬��ȷ�����뺯��֮ǰ�ڴ����ͷ�
{
	if(*pEvt != NULL)
		return;
	while (m_bUsing == TRUE)
		;
	m_bUsing = TRUE;

	*pEvtNum = m_iEvtNum;
	if(m_iEvtNum == 0)
	{
		m_bUsing = FALSE;
		return;
	}
	*pEvt = new EX_SYS_EVENT[m_iEvtNum];
	memcpy(*pEvt,m_pEvt,sizeof(EX_SYS_EVENT)*m_iEvtNum);
	//������ڴ��ͷŵ�
	delete[] m_pEvt;
	m_pEvt = NULL;
	m_iBufSize = 0;
	m_iEvtNum = 0;
	//
	m_bNeedClearBuf = TRUE;
	m_bUsing = FALSE;
}

void Mng_event::broadcast_event(SYS_EVENT *pEvt, int num)
{
// 	if(init.IsMainDAServer())
		m_eventproc.Save(GroupNo,pEvt,num);
}
