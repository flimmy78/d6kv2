// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "webstruct.h"
#include "WebManage.h"

#include "MainFrm.h"
#include "ClientInfo.h"
#include "rdb/rdbapi.h"
#include "DB.h"
#include <Iprtrmib.h>
#include "About.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMainFrame
NOTIFYICONDATA baricon;
#define ID_USERICONNID 100;
#define WM_ICONMESSAGE	(WM_USER + 500)
//#define	DATA_MESSAGE		WM_USER+100			/*������׼������Ϣ*/

extern char *Title;
extern THREADMANAGE threadinfo[INITCLIENTNUMS];
extern BOOL isExcludeEvent(int eventType);
extern void loadExcludeEvent();
extern void loadDevParaTable();

extern UINT srvwatchthread(LPVOID pParam );
extern void killAllClient();
extern USEREVENT *puserevent;
extern void updateClienttFileTime();
extern int getLifeThrNums();

extern CDB dbo;
extern BOOL bEndFlag;
extern HANDLE m_hEventKillCheck;
extern HANDLE m_hEvenKillWatch;
extern int CardRcdChgNums;
extern int SendFileNums;
extern UINT  checkEvent(LPVOID pParam);
extern void getData(int valcount,int threadno,int winno,Rdb_MultiTypeValue val);

UINT CheckThread(void *pPara)
{
	while(bEndFlag==FALSE)
	{
		ProcMan procMan;
		if(!procMan.CheckParentProc()) 
		{
			break;
		}
		_delay_time(1000*1000);
	}
	return TRUE;
}

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_SHOW_MAINWINDOW, OnShowMainwindow)
	ON_WM_SIZE()
	ON_COMMAND(ID_HIDE_MAINWINDOW, OnHideMainwindow)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_COMMAND(ID_CLIENT_INFO, OnClientInfo)
	ON_COMMAND(ID_SET, OnSet)
	ON_COMMAND(ID_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ICONMESSAGE,OnBarIconMsg)
	ON_MESSAGE(DATA_MESSAGE, OnDataMsg)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	

	baricon.cbSize = sizeof(NOTIFYICONDATA);
	baricon.hWnd = *this;
	baricon.uID = ID_USERICONNID;
	baricon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	baricon.uCallbackMessage = WM_ICONMESSAGE;
	baricon.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	strcpy(baricon.szTip, Title);
	Shell_NotifyIcon(NIM_ADD, &baricon);

	ProcMan proc;
	proc.StartProc("websrv","Web������","",this->GetSafeHwnd());

	m_netbug.RegProc("websrv");
	CWinThread  *p = AfxBeginThread(CheckThread,GetSafeHwnd());

    dbo.Open(DB_OP_WR);
	loadExcludeEvent();	//����ʧ��ʱ,���ڴ�й¶
	loadDevParaTable();

	p->SuspendThread();
	TerminateThread(p->m_hThread,0);

	m_hEventKillCheck = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEvenKillWatch =  CreateEvent(NULL, TRUE, FALSE, NULL);

	ASSERT(m_hEventKillCheck != NULL);
	ASSERT(m_hEvenKillWatch != NULL);

	AfxBeginThread(srvwatchthread,GetSafeHwnd());//����SOCKET�ػ��߳�
	AfxBeginThread(checkEvent,GetSafeHwnd());	//�¼�����߳�

	m_pdlg = NULL;
	m_pSysConfigDlg = NULL;

	SetTimer(2,1000,NULL);

	//getMacLicense();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style =WS_POPUP;//ʹ�����ڲ��ɼ�
	cs.dwExStyle |=WS_EX_TOOLWINDOW;//����ʾ����ť

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFrameWnd::DestroyWindow();
}

LRESULT CMainFrame::OnBarIconMsg(WPARAM wParam , LPARAM lParam)//NOTIFYICONDATA callback message map
{
	UINT uid = (UINT)wParam;
	UINT msg = (UINT)lParam;
	if(msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN)	
	{
		int xPos = GetSystemMetrics(SM_CXSCREEN);
		int yPos = GetSystemMetrics(SM_CYSCREEN);
		
		SetForegroundWindow();
		CMenu  menu;
		menu.LoadMenu(IDR_BAR_MENU);
		pPopMenu = menu.GetSubMenu(0);
		pPopMenu->TrackPopupMenu(TPM_CENTERALIGN,xPos,yPos,this,NULL);
	}

	return 0;

}

void CMainFrame::OnExit() //�˳�Ӧ�ó���
{
	// TODO: Add your command handler code here
	CString msg;
	int activeThreadCount = getLifeThrNums();
	if(activeThreadCount>0)
		msg = "���пͻ�������,ȷ���˳�websrv?";
	else
		msg = "ȷ���˳�websrv?";
	
	if(AfxMessageBox(msg,MB_YESNOCANCEL|MB_ICONEXCLAMATION) == IDYES)
	{
		PostMessage(WM_CLOSE,0,0);
	}
}

void CMainFrame::OnShowMainwindow() //��ʾ������
{
	// TODO: Add your command handler code here
	ShowWindow(SW_SHOW);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) //��С��ʱ����������
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(nType==SIZE_MINIMIZED)
		ShowWindow(SW_HIDE);
}

void CMainFrame::OnHideMainwindow() //����������
{
	// TODO: Add your command handler code here
	ShowWindow(SW_HIDE);
}

void CMainFrame::OnClose() //�ر�������ʱ�����˳����򣬽������ش���
{
	// TODO: Add your message handler code here and/or call default

	//ShowWindow(SW_HIDE);
	PreExit();
	CFrameWnd::OnClose();
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	ProcMan proc;
	proc.CheckParentProc();
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnClientInfo() //��ʾ�ͻ���Ϣ
{
	// TODO: Add your command handler code here
	if(m_pdlg == NULL )
	{
		m_pdlg = new CClientInfo();
		m_pdlg->Create(IDD_DIALOG_CLIENTINFO,this);
		m_pdlg->ShowWindow(SW_SHOW);
	}
	else if(!m_pdlg->IsWindowVisible())
		m_pdlg->ShowWindow(SW_SHOW );
}

void CMainFrame::OnSet() //ϵͳ����
{
	// TODO: Add your command handler code here
	//��󲢷��ͻ���
	//����ʱ��
	//�����ٶ�
	//ֻ��ʾ�쳣״̬�Ŀͻ���
	//���͵��ͻ��˵�ʵʱ��������,���ݿͻ��˵�½��Ȩ��
	//�޸Ŀͻ��˶��ļ���Ȩ��
/*
	if(m_pSysConfigDlg == NULL )
	{
		m_pSysConfigDlg = new CDlgSysConfig();
		m_pSysConfigDlg->Create(IDD_DLG_SYSCONFIG,this);
		m_pSysConfigDlg->ShowWindow(SW_SHOW);
	}
	else if(!m_pSysConfigDlg->IsWindowVisible())
		m_pSysConfigDlg->ShowWindow(SW_SHOW );
*/
}

LRESULT CMainFrame::OnDataMsg(WPARAM wParam, LPARAM lParam)
{
		int threadno = lParam/BASEWINNO;	//�̺߳�
		int winno = lParam%BASEWINNO;		//���ں�
		if(threadinfo[threadno].runf == 0 || threadinfo[threadno].validf == INVALIDF) return 0;

		EnterCriticalSection(&threadinfo[threadno].critThread);//Ҫ���ж��Ƿ���ס��?????

		Rdb_QuickStream *stream = threadinfo[threadno].prdb_quickstream;

		if(stream)//��ɾ��������߳���ͬ�����⣬�����ڴ涼ɾ���ˣ���β���???,���߳̽�����ҲҪͬ��
		{
			Rdb_MultiTypeValue val;
			int valcount = stream->RdbGetFieldValue(lParam, &val );
			//getData(valcount,threadno,winno,val);
				if(valcount>0)//�仯���ݸ���>0
				{
					MULTISTREAM *pmultistream = threadinfo[threadno].pmultistream;
					int i;
					for( i = 0; i < threadinfo[threadno].streamcount; i++)
					{
						if(pmultistream == NULL) 
						{ 
							LeaveCriticalSection(&threadinfo[threadno].critThread);
							return 0;
						} 
						if(pmultistream->winno == winno)
							break;
						pmultistream = pmultistream->plist;
					}
					if(i >= threadinfo[threadno].streamcount)	
					{ 
						LeaveCriticalSection(&threadinfo[threadno].critThread);
						return 0;
					}   //û���ҵ�����ƥ�����
							
					CThreadStreamData *pthreadstreamdata = pmultistream->pthreadstreamdata;
					ASSERT(pthreadstreamdata != NULL);
					for(i=0;i<valcount;i++)
					{
						int order = val.RdbGetValOrderno(i);
						if(order >= 0 && order < pthreadstreamdata->datacount)
						{
							if(!pthreadstreamdata->pdatatype[order])//��������û�еõ���
							{
								val.RdbGetValInfo(i, &(pthreadstreamdata->pvalue[order].valuetype));
								pthreadstreamdata->pdatatype[order] = TRUE;
							}//if

							uchar datatype = pthreadstreamdata->pvalue[order].valuetype;
							switch(datatype)
							{
							case RDB_DATATYPE_UCHAR:		//�޷����ַ���,0
							case RDB_DATATYPE_CHAR:         //�з����ַ���,1
							case RDB_DATATYPE_USHORT:       //�޷��Ŷ�������,2
							case RDB_DATATYPE_SHORT:        //�з��Ŷ�������,3
							case RDB_DATATYPE_UINT:         //�޷���������,4
							case RDB_DATATYPE_INT:          //�з���������,5
								{
									pthreadstreamdata->pvalue[order].value.intval = val.RdbGetVal_int(i);
									break;
								}
							case RDB_DATATYPE_ULONG:        //�޷��ų�������,6
								{
									pthreadstreamdata->pvalue[order].value.ulongval = val.RdbGetVal_ulong(i);
									break;
								}
							case RDB_DATATYPE_LONG:         //�з��ų�������(8 byte long),7
								{
									pthreadstreamdata->pvalue[order].value.longval = val.RdbGetVal_long(i);
									break;
								}
							case RDB_DATATYPE_FLOAT:        //�����ȸ�����,8
								{
									//���������ȽϺ�ԭ���Ĵ�С
									pthreadstreamdata->pvalue[order].value.floatval = val.RdbGetVal_float(i);
									break;
								}
							case RDB_DATATYPE_DOUBLE:       //˫���ȸ�����,9
								{
									pthreadstreamdata->pvalue[order].value.doubleval = val.RdbGetVal_double(i);
									break;
								}
							case RDB_DATATYPE_STRING:       //�ַ�����,10
								{
									 char *p = val.RdbGetString(RDB_MAX_STRVAL_LEN,i);
									 strncpy(pthreadstreamdata->pvalue[order].stringval,p,strlen(p));
									 pthreadstreamdata->pvalue[order].stringlen = strlen(p);
									break;
								}

							default:
								break;

							}//switch
							pthreadstreamdata->pdatachangedf[order] = TRUE;;
						}//if
					}//for i
					pthreadstreamdata->bchangef = TRUE;
				}//if(valcount>=0)
		}
		LeaveCriticalSection(&threadinfo[threadno].critThread);
	return 0;
}

void CMainFrame::getMacLicense()
{
	typedef DWORD (CALLBACK* LPGETIFTABLE)(PMIB_IFTABLE, PULONG, BOOL);
	
	LPGETIFTABLE get_if_table;
	
	#define IF_LIST_BUF_SIZE 5000

	int wsa_rc = 0;
	int return_code = TRUE;
	char *buffer = NULL;
	DWORD count;
	HINSTANCE ip_help_handle = NULL;
	DWORD getiftable_rc;
	ULONG buf_size;
	MIB_IFROW *ifr;
	MIB_IFTABLE *ift;
	DWORD ii = 0;
	DWORD jj = 0;

	ip_help_handle = LoadLibrary("iphlpapi.dll");
	if (ip_help_handle != NULL)
	{
		get_if_table = (LPGETIFTABLE) GetProcAddress(ip_help_handle, "GetIfTable");
		if (get_if_table != NULL)
		{
			
			/***************************************************************************/
			/* Call GetIfTable with zero buffer size to find out how much memory we    */
			/* need.                                                                   */
			/***************************************************************************/		
			buf_size = 0;
			getiftable_rc = (*get_if_table)((PMIB_IFTABLE) buffer, &buf_size, TRUE);
			if (getiftable_rc == ERROR_INSUFFICIENT_BUFFER)
			{
				buffer = (char *) malloc(buf_size);
				memset(buffer, 0, buf_size);
				/***************************************************************************/
				/* Get the physical interface data for real this time.                     */
				/***************************************************************************/
				getiftable_rc = (*get_if_table)((PMIB_IFTABLE)buffer, &buf_size, TRUE);
				if (getiftable_rc == NO_ERROR)
				{
					ift = (MIB_IFTABLE *) buffer;
					count = ift->dwNumEntries;
					char mac[32];
					memset(mac,0,32);
					char *pmac = mac;
					for (ii = 0; ii < count ; ii++)
					{
						ifr = (ift->table) + ii;
						for (jj = 0; jj < ifr->dwPhysAddrLen; jj++)
						{
						  /***********************************************************************/
						  /* Print out the next byte of the network address                      */
						  /***********************************************************************/
						  sprintf(pmac,"%02x", ifr->bPhysAddr[jj]);
						  pmac += 2;
						}
					}
				}
				FREE(buffer);
			}
		}
		FreeLibrary(ip_help_handle);
	}

}

void CMainFrame::PreExit()
{
	bEndFlag = true;	//���ö˿��ػ��̡߳�������߳̽�����־

	m_netbug.UnRegProc(); 
	if(m_pdlg != NULL )
	{
		m_pdlg->DestroyWindow();
	}
	if(m_pSysConfigDlg != NULL )
	{
		m_pSysConfigDlg->DestroyWindow();
	}
	
	killAllClient();

	WaitForSingleObject(m_hEventKillCheck,INFINITE);
	WaitForSingleObject(m_hEvenKillWatch,INFINITE);

	CloseHandle(m_hEventKillCheck);
	CloseHandle(m_hEvenKillWatch);

	while(puserevent)
	{
		USEREVENT *pevent = puserevent->puserevent;
		delete puserevent;
		puserevent = pevent;
	}
	dbo.Close();
	Shell_NotifyIcon(NIM_DELETE, &baricon);
	ProcMan proc;
	proc.EndProc();
}

void CMainFrame::OnAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}
