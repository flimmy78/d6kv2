#include "alarmwnd.h"
#include "alarmthread.h"
#include <QtGui/QApplication>
#include <QTextCodec>

#include "net/netapi.h"

NetDbg		net_dbg;
ProcMan		proc_man;
int live_flag = 1 ;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	if ( proc_man.IsProcExist( "authalarm" ) )
	{
		exit( 0 ) ;
	}

	if(net_dbg.RegProc( "��Ȩ�澯" )==false)
	{
		live_flag = 0;
		exit(0);
	}

	net_dbg.RegType( 0, "��Ȩ�澯" );
#if defined (__unix)
	proc_man.StartProc( "authalarm", "��Ȩ�澯", "" );
#elif defined (WIN32)
	HWND main_win = (HWND)10180;
	proc_man.StartProc( "authalarm", "��Ȩ�澯", "", main_win);
#endif

	AlarmThread *pCheckThread = new AlarmThread( NULL ) ;
	pCheckThread->start() ;

	alarm_wnd window;

	return app.exec() ;
}
