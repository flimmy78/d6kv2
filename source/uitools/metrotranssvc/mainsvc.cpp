#include "mainsvc.h"
#include "fileinfo.h"
#include "scd/scdapi.h"
#include <QSound>
#include <QMessageBox>
#include <QProcess>
int g_count = 0;
int g_flagkilltimer = 0;
extern int g_flagDebug;
extern int gbNeedStart;
extern int bDaQuit ;
extern File_info fileInfo;
extern 	Mng_event Event;
extern int GroupNo;
extern bool isBeginControl;

const int CHARBUFSIZE = 255;

#ifdef  Q_OS_WIN32
#define CPS_BIN "\\bin\\dabat\\"
#else
#define CPS_BIN "/bin/dabat/"
#endif

Action_thread::Action_thread() 
{    
}

void Action_thread::add_content(QString text)
{
	emit update_signal(text);
}

void Action_thread::run() 
{  
	for (;;)
	{
		if (isBeginControl)
		{
			isBeginControl = FALSE;
			int action_no = GroupNo;
			int control_num = fileInfo.get_actionlist_by_actionno(action_no);
			if( control_num<=0 )
			{
				add_content("û�п�������");
				break ;
			}

			ScadaServer YkServer;
			SCADA_YKRESULT_STRU struYkRes ;
			int	tmp = 0;
			int iIndex  = 0;
			QString qstr;
			char tmpstr[256];

			SYS_TIME time;
			TimeProc timeProc;	
			int32s timegap1;
			int32s timegap2;
			SYS_EVENT evt;
			memset(&evt,0,sizeof(SYS_EVENT));
			evt.grpno = 1;
			evt.sort = ENTCLASS_DA;
			evt.DiskOut = 1;
			evt.DispOut = 1;

			int yk_num=0; //ң������
			for( int i = 0 ; i< control_num ; i++)
			{
				//���ͬһ�����а���ͬһ��ң�ص��ε�����
				fileInfo.read_action_realstate();
				if( fileInfo.m_pControlList[i].state == fileInfo.m_pControlList[i].real_state )
				{

					if (g_flagDebug == 1)
					{
						sprintf(tmpstr," %s λ����ȷ,����ң�� !",fileInfo.m_pControlList[i].msg);

						evt.type = ENTTYPE_PROTECT_EVENT;	//���ʹ���
						evt.ymd = time.year*10000 + time.mon*100 + time.day;
						evt.hmsms = time.hour*10000000 + time.min*100000 + time.sec*1000 + time.ms;
						strcpy(evt.text,tmpstr);
						evt.val = 1.0;//��ʾDA������̽�����,�����к�������,Ϊ1ʱ,��ʾDA��������ڴ�������˳�
						evt.val1 = 0.0;
						evt.val2 = 0.0;
						Event.broadcast_event(&evt,1);

						add_content(tmpstr);
					}
					continue;
				}
				yk_num ++;
			}


			SCADA_YKOBJNAME_STRU* ykgrp = new SCADA_YKOBJNAME_STRU[yk_num];
			int ykNo = 0;
			for (int j=0;j<control_num;j++)
			{
				if( fileInfo.m_pControlList[j].state != fileInfo.m_pControlList[j].real_state )
				{
					strcpy((ykgrp+ykNo)->objname,fileInfo.m_pControlList[j].code);
					strcpy((ykgrp+ykNo) -> tablename,fileInfo.m_pControlList[j].table);
					if( fileInfo.m_pControlList[j].state != 0)
						(ykgrp+ykNo)->acttype = ACTTYPE_ON;
					else
						(ykgrp+ykNo)->acttype = ACTTYPE_OFF;

					ykNo++;
				}

			}

// 			unsigned char* pFlag = new unsigned char[yk_num];
// 			int failFlag = FALSE;
// 			memset(pFlag ,0, (yk_num)*sizeof(unsigned char));

			tmp	= YkServer.SendGroupYkCmd(1,"sa","daqo", "",yk_num,ykgrp);
			if(tmp == -1)
			{//����ң������ʧ��
				if (g_flagDebug == 1)
				{
					sprintf(tmpstr,"ң�ز���SCADA�޻�Ӧ!\n");		
					add_content(tmpstr);
					continue;
				}
			}
	/*		void * pRes=0;
			int nResLen;
			int nRes;
			int count = 0;
			int no = 0;
			for(;;)
			{
				nRes = YkServer.ScadaReturn("sa",&pRes,&nResLen);

				if(nRes == SCADA_TYPE_YKRESULT)
				{
					if(nResLen < sizeof(SCADA_YKRESULT_STRU)) 
						continue;
					memcpy(&struYkRes,pRes,sizeof(SCADA_YKRESULT_STRU));

					printf("\n struYkRes.resulttype ==%d !!! \n",struYkRes.resulttype);

					if(struYkRes.resulttype == SCADA_YK_CHECK_OK || \
						struYkRes.resulttype == SCADA_YK_PRESET || \
						struYkRes.resulttype == SCADA_YK_COMFIRM || \
						struYkRes.resulttype == SCADA_YK_EXEC || \
						struYkRes.resulttype == SCADA_YK_NULL )
						continue;

					for(int k = 0; k <yk_num; k++)
					{
						if(!strcmp(ykgrp[k].objname,struYkRes.objectname))
						{
							if(pFlag[k] == 0)
								no++;
							if(struYkRes.resulttype != SCADA_YK_SUCCESS)
							{
								if (g_flagDebug == 1)
								{
									sprintf(tmpstr,"cmdң��ʧ��: %s!\n",struYkRes.objectname);		
									add_content(tmpstr);
								}
							}
							if(struYkRes.resulttype == SCADA_YK_SUCCESS)
							{
								if (g_flagDebug == 1)
								{
									sprintf(tmpstr,"cmdң�سɹ�: %s!\n",struYkRes.objectname);		
									add_content(tmpstr);
									pFlag[k] = 1;
								}
							}
							break;
						}
					}
					if(no == yk_num )
						break;
				}
				else
				{
					if(count++ > 15*(yk_num)) //�ȴ��ʼ�
					{
						break ;
					}
					_delay_time(1000*1000);
				}				
			}*/
			delete[] ykgrp;
		}
		_delay_time(200*1000);		//1s
	 }

}


Metro_trans_svc::Metro_trans_svc ()
{
	GroupNo = 0;
	isBeginControl = FALSE;
}


void Metro_trans_svc::Init()
{
	QString msgtemp;

	if(fileInfo.Init() < 0)
	{
		msgtemp = _C2Q("���ݿ����Ӵ���!");
// 		m_initInfo->setText(msgtemp);

		return;
	}
	if(fileInfo.read_triggercond_para() < 0)
	{
		msgtemp = _C2Q("��ȡ��·���������!");
// 		m_initInfo->setText(msgtemp);
		return;
	}
	if(fileInfo.read_action_para() < 0)
	{
		msgtemp = _C2Q("��ȡ�����б����!");
// 		m_initInfo->setText(msgtemp);
		return;
	}
	pMainThread = new Main_thread(this); //�߳�
	pEvtThread = new Evt_thread(this); //�߳�
	pControlThread = new Action_thread(); //�߳�

	pMainThread->start();
	pEvtThread->start();
	pControlThread->start();
	if (!pMainThread->isRunning())
	{
		msgtemp = _C2Q("�����߳�����ʧ��!");
		return;
	}
	if (!pEvtThread->isRunning())
	{
		msgtemp = _C2Q("�����߳�����ʧ��!");
		return;
	}
	msgtemp = _C2Q("ң��ת�����������ɹ�!");
}

void Evt_thread::run()
{
	while(bDaQuit == FALSE)
	{
		if(Event.read_event(fileInfo) != 0)
			bDaQuit = TRUE;
		_delay_time(200*1000);
	}
	return;
}

void Main_thread::run()
{
	///��������

	SYS_TIME time;
	TimeProc timeProc;	
	int32s timegap1;
	int32s timegap2;
	SYS_EVENT evt;
	memset(&evt,0,sizeof(SYS_EVENT));
	evt.grpno = 1;
	evt.sort = ENTCLASS_DA;
	evt.DiskOut = 1;
	evt.DispOut = 1;
	timeProc.GetTime(time);

	timegap1 = Event.m_timeLastFault - Event.m_timeFirstFault;
	timegap2 = time - Event.m_timeLastFault;
	///ѭ���崦�����
	for(;;)
	{
		if(gbNeedStart == TRUE)
		{
			//��Ҫ�������

			int iFaultNumOfSub = 0;
			int iRet = 0;
			int iFaultNum = 0;
			int iIsoBrkNum = 0;
			gbNeedStart = FALSE;

			//----------------------
			evt.type = ENTTYPE_DIAG_SGZ;	//���ʹ���
			evt.ymd = time.year*10000 + time.mon*100 + time.day;
			evt.hmsms = time.hour*10000000 + time.min*100000 + time.sec*1000 + time.ms;
			strcpy(evt.text,"�յ���λ�źţ����Կ�ʼת��ң�ع���!");
			evt.val = 1.0;//��ʾDA������̽�����,�����к�������,Ϊ1ʱ,��ʾDA��������ڴ�������˳�
			evt.val1 = 0.0;
			evt.val2 = 0.0;
			Event.broadcast_event(&evt,1);
			//----------------------
#ifdef __PROGRAM_DEBUG_
			TRACE("ת��ң�ش���ʼ!\n");
#endif
			m_pEvt = NULL;
			Event.move_buf(&m_pEvt,&m_iEvtNum);
			for(int i=0;i<m_iEvtNum;i++)
			{
				if (m_pEvt[i].evt.type>=ENTTYPE_POWERSYS && m_pEvt[i].evt.type< ENTTYPE_PROTECT)  //��������
				{
					for(int j=0;j<fileInfo.m_triggercond_num;j++)
					{
						if( !strcmp(m_pEvt[i].evt.member0, fileInfo.m_faultcond_list[j].code) && m_pEvt[i].evt.val== fileInfo.m_faultcond_list[j].state)
						{
							evt.type = ENTTYPE_DIAG_SGZ;
							evt.ymd = time.year*10000 + time.mon*100 + time.day;
							evt.hmsms = time.hour*10000000 + time.min*100000 + time.sec*1000 + time.ms;
							int result = deal_block_data(j);
							if (result==0)	//��·��ʼ״̬����
							{
								sprintf(evt.text,"ң�ű�λ�������������㣬����ң��!");
								evt.val = 0;//��ʾDA������̽�����,�����к�������,Ϊ1ʱ,��ʾDA��������ڴ�������˳�
								GroupNo = fileInfo.m_faultcond_list[j].action_no;
								isBeginControl = TRUE;
								// 								ControlProc();
							}
							else if (result==1)
							{
								strcpy(evt.text,"��ȡң�ر�����������ң�ش�������˳�!");
								evt.val = 1.0;//��ʾDA������̽�����,�����к�������,Ϊ1ʱ,��ʾDA��������ڴ�������˳�
							}
							else if (result==2)
							{
								sprintf(evt.text,"ң�ر������������㣬ң�ع����˳�!");
								// 								strcpy(evt.text,"��·�Ϸ�������,��·��ʼ״̬���󣬹��ϴ�������˳�!");
								evt.val = 1.0;//��ʾDA������̽�����,�����к�������,Ϊ1ʱ,��ʾDA��������ڴ�������˳�
							}
							evt.val1 = 0.0;
							evt.val2 = 0.0;
							add_content(evt.text);
							Event.broadcast_event(&evt,1);
						}

					}
				}
			}
		}
		_delay_time(200*1000);
	}

	return;
}

void Main_thread::add_content(QString text)
{
	emit update_signal(text);
}

int Main_thread::deal_block_data(int no)
{
	int blockcondno = fileInfo.m_faultcond_list[no].blockcond_no;
	if (fileInfo.read_blockcond_para(blockcondno)<0)
	{
		return 1;	//��ȡ��·���ݳ���
	}
	fileInfo.read_blockcond_realstate();

	for (int i=0;i<fileInfo.m_blockcond_num;i++)
	{
		if (fileInfo.m_blockcond_list[i].state!=fileInfo.m_blockcond_list[i].real_state)
		{
			return 2; //��·��ʼ״̬������
		}
	}
	return 0;
}


int Main_thread::deal_da_file(QString filename)
{
	char path[CHARBUFSIZE]= {0};
	char* env = getenv("NBENV");
	if (env) {
		strncat(path, env, CHARBUFSIZE);
	}
	strncat(path, CPS_BIN, CHARBUFSIZE);

	QString dir = path;
	QString str = dir + filename;
	QProcess process;
	process.execute(str);	
	return 0;
}
