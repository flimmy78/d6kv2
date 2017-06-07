#include <QtCore/QtGlobal>
#include <QtCore/QString>
#include <QtCore/QLatin1String>
#include <QtCore/QTextDecoder>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtGui/QFont>
#include <QtCore/QProcess>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QSettings>
#include <QtGui/QColor>

#include <QtCore/QDebug>

#include "sysdef.h"

#include "datacurveapp.h"
#include "logindialog.h"


DBExtension			DataCurveApp::dbex;
DataCurveDefine		DataCurveApp::dataCurveDef;
bool				DataCurveApp::ycEverdayTable=false;


DataCurveApp::DataCurveApp( int & argc, char ** argv )
:QApplication(argc,argv),nbenv(C2G("NBENV")),isSuperMan(false)
{
	foreach(QString envVar,QProcess::systemEnvironment())
	{
		if(envVar.contains(NBENV,Qt::CaseInsensitive))
		{
			nbenv=envVar.right(envVar.length()-envVar.indexOf(C2G("="))-1);
		}
	}

	settings=new QSettings(C2G("gcurve.ini"),QSettings::IniFormat);
	readSettings();
}

DataCurveApp::~DataCurveApp()
{
	dbex.Close();
	//saveSettings();
	delete settings;
}

//////////////////////////////////////////////////////////////////////////
////

void DataCurveApp::initUserRightPara()
{
	int			 i,j,k,colcnt;
	QString		 sqlword;
	QStringList  content;

	userparas.clear();
#ifdef  APP_VER_1_0
	sqlword=QString(C2G("select * from �û���������� order by ����Ա���"));
#elif APP_VER_2_0
	sqlword=QString(C2G("select * from �û���������� order by ����Ա���"));
#endif
	colcnt=DataCurveApp::dbex.sqlReadRecorderDir(DB_NAME,sqlword,content);
	for(i=0;i<content.size();i+=colcnt)
	{
		consumer   user;
		j=i;
		user.type=content[j++].toUInt();
		user.typedesc=content[j++];
		user.username=content[j++];
		user.userdesc=content[j++];
		user.password=content[j++];
		user.groupname=content[j++];
		user.ismonitor=content[j++].toInt()>0 ? true:false;
		userparas.append(user);
	}

	rightparas.clear();
#ifdef  APP_VER_1_0
	sqlword=QString(C2G("select * from �ڵ�Ȩ�޲����� order by ������,����Ա"));
#elif APP_VER_2_0
	sqlword=QString(C2G("select * from �ڵ�Ȩ�޲����� order by ������,����Ա"));
#endif
	colcnt=DataCurveApp::dbex.sqlReadRecorderDir(DB_NAME,sqlword,content);
	for(i=0;i<content.size();i+=colcnt)
	{
		consumerRight    right;
		j=i;
		right.hostname=content[j++];
		right.username=content[j++];
		foreach(consumer item,userparas)
		{
			if(item.username==right.username)
			{
				right.type=item.type;
				right.userdesc=item.userdesc;
				right.password=item.password;
				break;
			}
		}
		for(k=0;k<USRRIGHT_NUM;k++)
		{
			right.right[k]=content[j++].toInt()>0 ? true:false;
		}
		for(k=0;k<GRADE_NUM;k++)
		{
			right.grade[k]=content[j++].toInt()>0 ? true:false;
		}
		rightparas.append(right);
	}
}

void DataCurveApp::initStationAreaDb()
{
	int i,j;
	QString sqlword;
	StationAreaPara *staareaItem;
	QStringList  content;


	qDeleteAll(stationAreaParas);
	stationAreaParas.clear();
	
//#ifdef  APP_VER_1_0
	sqlword=C2G("select id,name,masterid from STATIONAREA  order by id");
//#elif APP_VER_2_0
//	sqlword=C2G("select no,name,describe,zone,sname,beginstatus,isstat,isused from terminal where isused<>0 order by no");
//#endif
	int colcnt=DataCurveApp::dbex.sqlReadRecorderDir(DB_NAME,sqlword,content);
	for(i=0;i<content.size();i+=colcnt)
	{
		j=i;
		staareaItem=new StationAreaPara;
		staareaItem->id=content[j++].toShort();
		staareaItem->name=content[j++];
		staareaItem->masterid=content[j++].toShort();
		stationAreaParas.append(staareaItem);
	}
}


void DataCurveApp::initStationDb()
{
	int i,j;
	QString sqlword;
	terminalpara *rtuItem;
	QStringList  content;


	qDeleteAll(terminals);
	terminals.clear();

#ifdef  APP_VER_1_0
	sqlword=C2G("select �ն����,����,����,��ȫ��,վ����ϵ����,ʹ�ñ�־ from �ն˲����� where ʹ�ñ�־<>0 order by �ն����");
#elif APP_VER_2_0
	sqlword=C2G("select no,name,describe,zone,sname,beginstatus,isstat,isused from terminal where isused<>0 order by no");
#endif
	int colcnt=DataCurveApp::dbex.sqlReadRecorderDir(DB_NAME,sqlword,content);
	for(i=0;i<content.size();i+=colcnt)
	{
		j=i;
		rtuItem=new terminalpara;
		rtuItem->no=content[j++].toShort();
		rtuItem->name=content[j++];
		rtuItem->describe=content[j++];
		rtuItem->zone=content[j++].toShort();
		rtuItem->sname=content[j++];
		// 		rtuItem->beginstatus=content[j++].toShort();
		// 		rtuItem->isstat=content[j++].toShort();
		rtuItem->isused=content[j++].toShort();
		terminals.append(rtuItem);
	}
}

void DataCurveApp::initStationViewDb()
{
	int i,j;
	QString sqlword;
	stationviewpara *rtuItem;
	QStringList  content;
   notoareaid.clear();

	qDeleteAll(stationviewparas);
	stationviewparas.clear();

#ifdef  APP_VER_1_0
	sqlword=C2G("select �ն����,�ն˴���,������� from subareastationview2  order by �ն����");
#elif APP_VER_2_0
	//sqlword=C2G("select no,name,describe,zone,sname,beginstatus,isstat,isused from terminal where isused<>0 order by no");
#endif
	int colcnt=DataCurveApp::dbex.sqlReadRecorderDir(DB_NAME,sqlword,content);
	for(i=0;i<content.size();i+=colcnt)
	{
		j=i;
		rtuItem=new stationviewpara;
		rtuItem->no=content[j++].toShort();
		rtuItem->name=content[j++];
		rtuItem->areano=content[j++].toShort();
		stationviewparas.append(rtuItem);
		notoareaid.insert(rtuItem->no,rtuItem->areano);
	}
}


void DataCurveApp::initStationYcDb()
{
	int i,j;
	QString sqlword;
	ycpara   *ycItem;
	QStringList     content;
	
	qDeleteAll(ycparas);
	ycparas.clear();

#ifdef  APP_VER_1_0
//	sqlword=C2G("select ����,����,��ȫ��,�豸����,����,�ն����,ң�����,��������,��������,��������,��������,\
		��������־,��������,�ƻ����,��ʾ��־ from ң������� order by �ն����,����"); // 
	sqlword=C2G("select ����,����,��ȫ��,�豸����,����,�ն����,ң�����,��������,��������,��������,��������,\
		��������־,��������,�ƻ����,��ʾ��־ from ң������� order by ң�����"); //
#elif APP_VER_2_0
//	sqlword=C2G("select name,describe,zone,devname,type,terminalno,ycno,warnupperlimit,warnlowerlimit,alertupperlimit,alertlowerlimit,\
		iscomputer,savetime,plantime,isshow from ycpara order by terminalno,type");
	sqlword=C2G("select name,describe,zone,devname,type,terminalno,ycno,warnupperlimit,warnlowerlimit,alertupperlimit,alertlowerlimit,\
		iscomputer,savetime,plantime,isshow from ycpara order by ycno");
#endif
	int colcnt=DataCurveApp::dbex.sqlReadRecorderDir(DB_NAME,sqlword,content);
	for(i=0;i<content.size();i+=colcnt)
	{
		j=i;
		ycItem=new ycpara;
		ycItem->name=content[j++];
		ycItem->describe=content[j++];
		ycItem->zone=content[j++].toShort();
		ycItem->devname=content[j++];
		ycItem->type=content[j++].toShort();
		ycItem->terminalno=content[j++].toShort();
		ycItem->ycno=content[j++].toShort();
		ycItem->warnupperlimit=content[j++].toFloat();
		ycItem->warnlowerlimit=content[j++].toFloat();
		ycItem->alertupperlimit=content[j++].toFloat();
		ycItem->alertlowerlimit=content[j++].toFloat();
		ycItem->iscomputer=content[j++].toShort();
		ycItem->savetime=content[j++].toShort();
		ycItem->plantime=content[j++].toShort();
		ycItem->isshow=content[j++].toShort();

		ycItem->typeDescribe=curveTypeDef(ycItem->type).Desc();
		ycparas.append(ycItem);
	}
}



void DataCurveApp::initStationKwnDb()
{
	int i,j;
	QString			sqlword;
	kwhpara			*kwhItem;
	QStringList     content;
	
	qDeleteAll(kwhparas);
	kwhparas.clear();
#ifdef  APP_VER_1_0
	sqlword=C2G("select ����,����,��ȫ��,�豸����,����,�ն����,������,����,���,����,����,��������,\
		�ɼ�����,ԭ����ֵ,��������־,���ֱ�־,���̱�־,ɾ����־,��ʼ����ʱ��,��ʼ״̬,�Ƿ�ͳ��,��ʾ��־ \
		from ��Ȳ����� order by ������");
#elif APP_VER_2_0
	sqlword=C2G("select name, describe,zone,devname, type, terminalno, kwhno, basecode, scale, maxcode, times, savetime,\
		collectortype,primitivemaxcode, iscomputer, isintegral, issave, isdel, starttime, startstatus, isstat, isshow \
		from kwhpara order by kwhno");
#endif
	int colcnt=DataCurveApp::dbex.sqlReadRecorderDir(DB_NAME,sqlword,content);
	for(i=0;i<content.size();i+=colcnt)
	{
		j=i;
		kwhItem=new kwhpara;
		kwhItem->name=content[j++];
		kwhItem->describe=content[j++];
		kwhItem->zone=content[j++].toShort();
		kwhItem->devname=content[j++];
		kwhItem->type=content[j++].toShort();
		kwhItem->terminalno=content[j++].toShort();
		kwhItem->kwhno=content[j++].toShort();
		kwhItem->basecode=content[j++].toFloat();
		kwhItem->scale=content[j++].toFloat();
		kwhItem->maxcode=content[j++].toFloat();
		kwhItem->times=content[j++].toFloat();
		kwhItem->savetime=content[j++].toShort();
		kwhItem->collectortype=content[j++].toShort();
		kwhItem->primitivemaxcode=content[j++].toULong();
		kwhItem->iscomputer=content[j++].toShort();
		kwhItem->isintegral=content[j++].toShort();
		kwhItem->issave=content[j++].toShort();
		kwhItem->isdel=content[j++].toShort();
		kwhItem->starttime=content[j++].toShort();
		kwhItem->startstatus=content[j++].toShort();
		kwhItem->isstat=content[j++].toShort();
		kwhItem->isshow=content[j++].toShort();
		
		kwhItem->typeDescribe=curveTypeDef(kwhItem->type).Desc();
		kwhparas.append(kwhItem);
	}
}


void DataCurveApp::initCurveTypeDefs()
{
	DataCurveApp *appSettings=this;
	curveTypeDefs.clear();
	curveTypeDefs.append(curvetypepara(1,QObject::tr("����"),appSettings->defaultColor,QObject::tr("A")));
	curveTypeDefs.append(curvetypepara(2,QObject::tr("A�����"),appSettings->aPhaseColor,QObject::tr("A")));
	curveTypeDefs.append(curvetypepara(3,QObject::tr("B�����"),appSettings->bPhaseColor,QObject::tr("A")));
	curveTypeDefs.append(curvetypepara(4,QObject::tr("C�����"),appSettings->cPhaseColor,QObject::tr("A")));
	curveTypeDefs.append(curvetypepara(5,QObject::tr("AB�����"),appSettings->aPhaseColor,QObject::tr("A")));
	curveTypeDefs.append(curvetypepara(6,QObject::tr("BC�����"),appSettings->bPhaseColor,QObject::tr("A")));
	curveTypeDefs.append(curvetypepara(7,QObject::tr("CA�����"),appSettings->cPhaseColor,QObject::tr("A")));
	curveTypeDefs.append(curvetypepara(8,QObject::tr("�������"),appSettings->defaultColor,QObject::tr("A")));
	curveTypeDefs.append(curvetypepara(9,QObject::tr("�������"),appSettings->defaultColor,QObject::tr("A")));
	
	curveTypeDefs.append(curvetypepara(20,QObject::tr("�й�����"),appSettings->defaultColor,QObject::tr("KW")));
	curveTypeDefs.append(curvetypepara(21,QObject::tr("A���й�"),appSettings->aPhaseColor,QObject::tr("KW")));
	curveTypeDefs.append(curvetypepara(22,QObject::tr("B���й�"),appSettings->bPhaseColor,QObject::tr("KW")));
	curveTypeDefs.append(curvetypepara(23,QObject::tr("C���й�"),appSettings->cPhaseColor,QObject::tr("KW")));
	curveTypeDefs.append(curvetypepara(24,QObject::tr("AB���й�"),appSettings->aPhaseColor,QObject::tr("KW")));
	curveTypeDefs.append(curvetypepara(25,QObject::tr("BC���й�"),appSettings->bPhaseColor,QObject::tr("KW")));
	curveTypeDefs.append(curvetypepara(26,QObject::tr("CA���й�"),appSettings->cPhaseColor,QObject::tr("KW")));
	
	curveTypeDefs.append(curvetypepara(40,QObject::tr("�޹�����"),appSettings->defaultColor,QObject::tr("KVar")));
	curveTypeDefs.append(curvetypepara(41,QObject::tr("A���޹�"),appSettings->aPhaseColor,QObject::tr("KVar")));
	curveTypeDefs.append(curvetypepara(42,QObject::tr("B���޹�"),appSettings->bPhaseColor,QObject::tr("KVar")));
	curveTypeDefs.append(curvetypepara(43,QObject::tr("C���޹�"),appSettings->cPhaseColor,QObject::tr("KVar")));
	curveTypeDefs.append(curvetypepara(44,QObject::tr("AB���޹�"),appSettings->aPhaseColor,QObject::tr("KVar")));
	curveTypeDefs.append(curvetypepara(45,QObject::tr("BC���޹�"),appSettings->bPhaseColor,QObject::tr("KVar")));
	curveTypeDefs.append(curvetypepara(46,QObject::tr("CA���޹�"),appSettings->cPhaseColor,QObject::tr("KVar")));
	
	curveTypeDefs.append(curvetypepara(60,QObject::tr("���ڹ���"),appSettings->defaultColor,QObject::tr("KW")));
	curveTypeDefs.append(curvetypepara(61,QObject::tr("���ʣ�����������"),appSettings->defaultColor,QObject::tr("%")));
	
	curveTypeDefs.append(curvetypepara(100,QObject::tr("�Ƕ�"),appSettings->defaultColor,QObject::tr("")));
	curveTypeDefs.append(curvetypepara(101,QObject::tr("Ƶ��"),appSettings->defaultColor,QObject::tr("Hz")));
	curveTypeDefs.append(curvetypepara(102,QObject::tr("�¶�"),appSettings->defaultColor,QObject::tr("��")));
	curveTypeDefs.append(curvetypepara(103,QObject::tr("ѹ��"),appSettings->defaultColor,QObject::tr("Pa")));
	curveTypeDefs.append(curvetypepara(104,QObject::tr("ˮλ"),appSettings->defaultColor,QObject::tr("m")));
	curveTypeDefs.append(curvetypepara(105,QObject::tr("����"),appSettings->defaultColor,QObject::tr("m3/h")));
	curveTypeDefs.append(curvetypepara(106,QObject::tr("ʪ��"),appSettings->defaultColor,QObject::tr("g/m3")));
	curveTypeDefs.append(curvetypepara(107,QObject::tr("��ѹ����λ"),appSettings->defaultColor,QObject::tr("")));
	curveTypeDefs.append(curvetypepara(108,QObject::tr("����ң��"),appSettings->defaultColor,QObject::tr("")));
	
	curveTypeDefs.append(curvetypepara(150,QObject::tr("��ѹ"),appSettings->defaultColor,QObject::tr("KV")));
	curveTypeDefs.append(curvetypepara(151,QObject::tr("KV���ѹ"),appSettings->aPhaseColor,QObject::tr("KV")));
	curveTypeDefs.append(curvetypepara(152,QObject::tr("B���ѹ"),appSettings->bPhaseColor,QObject::tr("KV")));
	curveTypeDefs.append(curvetypepara(153,QObject::tr("C���ѹ"),appSettings->cPhaseColor,QObject::tr("KV")));
	curveTypeDefs.append(curvetypepara(154,QObject::tr("AB���ѹ"),appSettings->aPhaseColor,QObject::tr("KV")));
	curveTypeDefs.append(curvetypepara(155,QObject::tr("BC���ѹ"),appSettings->bPhaseColor,QObject::tr("KV")));
	curveTypeDefs.append(curvetypepara(156,QObject::tr("CA���ѹ"),appSettings->cPhaseColor,QObject::tr("KV")));
	curveTypeDefs.append(curvetypepara(157,QObject::tr("�����ѹ"),appSettings->defaultColor,QObject::tr("KV")));
	curveTypeDefs.append(curvetypepara(158,QObject::tr("�����ѹ"),appSettings->defaultColor,QObject::tr("KV")));
	curveTypeDefs.append(curvetypepara(159,QObject::tr("�����ѹ"),appSettings->defaultColor,QObject::tr("KV")));
	
	curveTypeDefs.append(curvetypepara(200,QObject::tr("�й����"),appSettings->defaultColor,QObject::tr("KWh")));
	curveTypeDefs.append(curvetypepara(201,QObject::tr("�޹����"),appSettings->defaultColor,QObject::tr("KWh")));
	curveTypeDefs.append(curvetypepara(202,QObject::tr("���й����"),appSettings->defaultColor,QObject::tr("KWh")));
	curveTypeDefs.append(curvetypepara(203,QObject::tr("���й����"),appSettings->defaultColor,QObject::tr("KWh")));
	curveTypeDefs.append(curvetypepara(204,QObject::tr("���޹����"),appSettings->defaultColor,QObject::tr("KWh")));
	curveTypeDefs.append(curvetypepara(205,QObject::tr("���޹����"),appSettings->defaultColor,QObject::tr("KWh")));
	curveTypeDefs.append(curvetypepara(206,QObject::tr("�����ۻ���"),appSettings->defaultColor,QObject::tr("KWh")));
}


// void DataCurveApp::initParaTypeDef()
// {
// 	QString line;
// 	QStringList splitLine;
// 	QFile file(NBENV+YCTYPEDEFINED);
// 	if(file.open(QIODevice::ReadOnly))
// 	{
// 		QTextStream stream(&file);
// 		do 
// 		{
// 			line=stream.readLine();
// 			splitLine=line.split(C2G("*"));
// 			if(splitLine.size()!=2) continue;
// 			ycTypeDef.insert(splitLine[0].toUInt(),splitLine[1]);
// 
// 		} while (!line.isNull());
// 	}
// 	file.close();
// 	file.setFileName(NBENV+KWHTYPEDEFINED);
// 	if(file.open(QIODevice::ReadOnly))
// 	{
// 		QTextStream stream(&file);
// 		do 
// 		{
// 			line=stream.readLine();
// 			splitLine=line.split(C2G("*"));
// 			if(splitLine.size()!=2) continue;
// 			kwhTypeDef.insert(splitLine[0].toUInt(),splitLine[1]);
// 			
// 		} while (!line.isNull());
// 	}
// 	file.close();
// }

void DataCurveApp::readSettings()
{
	//settings->setPath(QSettings::IniFormat,QSettings::UserScope,NBENV+C2G("/ini/"));
	settings->beginGroup(tr("AppSettings"));
	settings->contains(C2G("MaxCurveNumber")) ? maxCurveNumber=settings->value(C2G("MaxCurveNumber")).toInt() : 
	maxCurveNumber=5;
	settings->contains(C2G("MaxTableRowNumber")) ? maxTableRowNumber=settings->value(C2G("MaxTableRowNumber")).toInt() : 
	maxTableRowNumber=1000;
	settings->contains(C2G("DefaultMinCycle")) ? defaultMinCycle=settings->value(C2G("DefaultMinCycle")).toInt() : 
	defaultMinCycle=1;
	settings->contains(C2G("TimeSpecValue")) ?  timeSpecValue=static_cast<Qt::TimeSpec>(settings->value(C2G("TimeSpecValue")).toInt()) : 
	timeSpecValue=Qt::UTC;
	settings->contains(tr("ToolsIconSize")) ? toolsIconSize=qVariantValue<QSize>(settings->value(tr("ToolsIconSize"))) : 
	toolsIconSize=QSize(18,18);

	settings->endGroup();

	settings->beginGroup(tr("ColorDefined"));
	settings->contains(tr("APhaseColor")) ? aPhaseColor=qVariantValue<QColor>(settings->value(tr("APhaseColor"))) : 
	aPhaseColor=QColor(255,0,0);
	settings->contains(tr("BPhaseColor")) ? bPhaseColor=qVariantValue<QColor>(settings->value(tr("BPhaseColor"))) :
	bPhaseColor=QColor(0,255,0);
	settings->contains(tr("CPhaseColor")) ? cPhaseColor=qVariantValue<QColor>(settings->value(tr("CPhaseColor"))) :
	cPhaseColor=QColor(0,0,255);
	settings->endGroup();

	settings->beginGroup(tr("CurveDefined"));
	settings->contains(tr("CurveLineWidth")) ? curveLineWidth=settings->value(tr("CurveLineWidth")).toInt() :
	curveLineWidth=2;
	settings->contains(tr("CurveSymbolStyle")) ? curveSymbolStyle=settings->value(tr("CurveSymbolStyle")).toInt():
	curveSymbolStyle= 0;//QwtSymbol::Ellipse
	settings->contains(tr("CurveSymbolSize")) ? curveSymbolSize=qVariantValue<QSize>(settings->value(tr("CurveSymbolSize"))) : 
	curveSymbolSize=QSize(8,8);
	settings->endGroup();


}

void DataCurveApp::saveSettings()
{
	settings->setPath(QSettings::IniFormat,QSettings::UserScope,nbenv+C2G("/ini/"));
	settings->beginGroup(tr("AppSettings"));
	settings->setValue(C2G("MaxCurveNumber"),maxCurveNumber);
	settings->setValue(C2G("MaxTableRowNumber"),maxTableRowNumber);
	settings->setValue(C2G("DefaultMinCycle"),defaultMinCycle);
	settings->setValue(C2G("TimeSpecValue"),timeSpecValue);
	settings->endGroup();

	settings->beginGroup(tr("ColorDefined"));
 	settings->setValue(tr("APhaseColor"),aPhaseColor);
 	settings->setValue(tr("BPhaseColor"),bPhaseColor);
 	settings->setValue(tr("CPhaseColor"),cPhaseColor);
	settings->endGroup();

	settings->beginGroup(tr("CurveDefined"));
	settings->setValue(tr("CurveLineWidth"),curveLineWidth);
	settings->setValue(tr("CurveSymbolStyle"),curveSymbolStyle);
	settings->setValue(tr("CurveSymbolSize"),curveSymbolSize);
	settings->endGroup();

}

bool DataCurveApp::doLogin()
{
	bool dlgRet=false;
	USEROPERRIGHT_STRU tmpRight;
	DISPATCHER_STRU    tmpOperInfo;
	loginDlg=new Login_widget();
	int result=loginDlg->exec();
	
	if(result==QDialog::Accepted)
	{
		dlgRet=true;
		loginDlg->get_user_right( tmpRight );
		loginDlg->get_oper_info( tmpOperInfo );
		if( tr( tmpOperInfo.password ).compare(MANPWD,Qt::CaseInsensitive)==0)
		{
			isSuperMan=true;
		}
		else
		{
			foreach(consumerRight item,rightparas)
			{
				if(item.username.compare(  tr( tmpOperInfo.username ) ,Qt::CaseInsensitive)==0 
					&& item.password.compare(  tr( tmpOperInfo.password ) ,Qt::CaseInsensitive)==0)
				{
					currentUserRight=item;
					break;
				}
			}
		}
		EventProc evtproc;
		SYS_EVENT	sysevent;
		SYS_TIME	tmpTime;
		NetStatus ns;
		memset( &sysevent, 0, sizeof( SYS_EVENT ) );
		sysevent.grpno = ns.GetHostGrpNo( ns.GetHostNo() );

		sysevent.sort  = ENTCLASS_OPERATION ;
		sysevent.type  = ENTTYPE_SCADA_LOGIN ;
		sysevent.DiskOut = 1;
		sysevent.DispOut = 1;
		TimeProc tmpTimeProc;
		tmpTimeProc.GetTime( tmpTime );
		sysevent.ymd = tmpTime.year * 10000 + tmpTime.mon * 100 + tmpTime.day;
		sysevent.hmsms = tmpTime.hour * 10000000 + tmpTime.min * 100000 + 
			tmpTime.sec * 1000 + tmpTime.ms;

		strcpy( sysevent.Object , tmpRight.username );
		sprintf(sysevent.text,"%s��%s�ڵ��¼��ʷ����",tmpOperInfo.describe, 
			ns.GetNodeName( sysevent.grpno, ns.GetHostName( ns.GetHostNo() ) ) );
		evtproc.Save( &sysevent, 1 );
	}
	delete loginDlg;
	return dlgRet;
}

//////////////////////////////////////////////////////////////////////////
//login user operatoration

bool DataCurveApp::checkLoginUserZone( int zone,consumerRight* right/*=NULL*/ )
{
	if(isSuperMan) return true;
	consumerRight tmpRight;
	if(right==NULL)
		tmpRight=currentUserRight;
	else
		tmpRight=*right;
	if(zone>=GRADE_NUM)
		return false;
	else if(tmpRight.grade[zone])
		return true;
	return false;
}

bool DataCurveApp::checkLoginUserRight( int rightIdx,consumerRight* right/*=NULL*/ )
{
	if(isSuperMan) return true;
	consumerRight tmpRight;
	if(right==NULL)
		tmpRight=currentUserRight;
	else
		tmpRight=*right;

	if(rightIdx>=USRRIGHT_NUM)
		return false;
	else if(tmpRight.right[rightIdx])
		return true;
	return false;
}

QString DataCurveApp::loginUser() const
{
	if(isSuperMan)
		return C2G("�����û�");
	else
		return currentUserRight.userdesc;
}

QString DataCurveApp::loginUserStyle() const
{
	if(isSuperMan)
		return C2G("ϵͳ����Ա");
	foreach(consumer item,userparas)
	{
		if(item.username.compare(currentUserRight.username)==0)
		{
			return item.typedesc;
		}
	}
	return C2G("");
}

QString DataCurveApp::getEnvVar() const
{
	return nbenv;
}

//////////////////////////////////////////////////////////////////////////
////	selectorpara class

void selectorpara::debugOut() const
{
	QString tmpOut;
		tmpOut=QString("datasrc=%1 seltype=%2 cycle=%3 segment=%4 begindate=%5\
			begintime=%6 enddate=%7 endtime=%8 statictype=%9")
			.arg(dataSrc).arg(selType).arg(cycle).arg(segment)
			.arg(beginDate.toString("yyyy-MM-dd"))
			.arg(beginTime.toString("hh:mm:ss"))
			.arg(endDate.toString("yyyy-MM-dd"))
			.arg(endTime.toString("hh:mm:ss"))
			.arg(statType);
	qDebug()<<tmpOut;
}

selectorpara::selectorpara()
{
	dataSrc=YcDataSrc;
	cycle=DayCycle;
	segment=GeneralDate;
	selType=SingleSelector;
	beginDate=QDate::currentDate();
	beginTime=QTime(0,0,0,0);
	endDate=beginDate.addDays(1);
	endTime=beginTime;
	paras.clear();
	comps.clear();
	statType=HISDATA_REAL;
}

bool selectorpara::normalizationDateTime( int cycleMinute )
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	switch(cycle)
	{
	case DayCycle:
		if(segment==GeneralDate) 
		{
			beginTime=QTime(0,0,0,0);
			endTime=QTime(0,0,0,0);
			endDate=beginDate.addDays(1);
		}
		else if(segment==SegmentDate)
		{
			if((beginTime.minute()/cycleMinute+1)*cycleMinute>=60)
			{
				beginDate=beginDate.addDays(1);
				beginTime=QTime(0,0,0,0);
			}
			else if(!(beginTime.hour()==0 && beginTime.minute()==0))
			{
				if(beginTime.minute()%cycleMinute==0)
					beginTime=QTime(beginTime.hour(),beginTime.minute()/cycleMinute*cycleMinute,0,0);
				else
					beginTime=QTime(beginTime.hour(),(beginTime.minute()/cycleMinute+1)*cycleMinute,0,0);
			}
			endTime=QTime(endTime.hour(),endTime.minute()/cycleMinute*cycleMinute,0,0);
		}
		break;
	case MonthCycle:
		if(segment==GeneralDate)
		{
			beginDate=QDate(beginDate.year(),beginDate.month(),1);
			endDate=beginDate.addMonths(1);
			endDate=endDate.addDays(-1);			//only this month
			beginTime=QTime(0,0,0,0);
			endTime=QTime(0,0,0,0);
		}
		else if(segment==SegmentDate)
		{
			beginTime=QTime(0,0,0,0);
			endTime=QTime(0,0,0,0);
		}
		break;
	case YearCycle:
		if(segment==GeneralDate)
		{
			beginDate=QDate(beginDate.year(),1,1);
			endDate=beginDate.addYears(1);
			endDate = endDate.addMonths(-1);             //only this year
			beginTime=QTime(0,0,0,0);
			endTime=QTime(0,0,0,0);
		}
		else if(segment==SegmentDate)
		{
			beginTime=QTime(0,0,0,0);
			endTime=QTime(0,0,0,0);
		}
		break;
	}

	//process cmps normalization
// 	foreach(selectorpara selpara,comps)
// 	{
// 		selpara.normalizationDateTime(cycleMinute);
// 	}
	return true;
}

bool selectorpara::refreshAll( const selectorpara& cmpPara )
{
	if(dataSrc!=cmpPara.dataSrc || cycle!=cmpPara.cycle 
		|| selType!=cmpPara.selType || segment!=cmpPara.segment)
		return true;
	else if((cycle==MonthCycle || cycle==YearCycle) && statType!=cmpPara.statType)
		return true;
// 	else if(cycle==DayCycle && dayCurveMinCycle!=cmpPara.dayCurveMinCycle)
// 		return true;
	return false;
}

bool selectorpara::datetimeComp( const selectorpara& cmpPara )
{
	if(cycle==DayCycle)
	{
		if(segment==GeneralDate 
			&& beginDate==cmpPara.beginDate)
			return false;
		else if(segment==SegmentDate 
			&& beginDate==cmpPara.beginDate && endDate==cmpPara.endDate 
			&& beginTime==cmpPara.beginTime && endTime==cmpPara.endTime)
			return false;
	}
	else if(cycle==MonthCycle)
	{
		if(segment==GeneralDate 
			&& beginDate.year()==cmpPara.beginDate.year() && beginDate.month()==cmpPara.beginDate.month())
			return false;
		else if(segment==SegmentDate 
			&& beginDate==cmpPara.beginDate && endDate==cmpPara.endDate)
			return false;
	}
	else if(cycle==YearCycle)
	{
		if(segment==GeneralDate && beginDate.year()==cmpPara.beginDate.year())
			return false;
		else if(segment==SegmentDate 
			&& beginDate.year()==cmpPara.beginDate.year() && beginDate.month()==cmpPara.beginDate.month() 
			&& endDate.year()==cmpPara.endDate.year() && endDate.month()==cmpPara.endDate.month())
			return false;
	}
	return true;
}

int selectorpara::segmentMaxCount( int cycleMinute )
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	if(cycle==DayCycle)
	{
		if(segment==GeneralDate)
		{
			return 24*60 / cycleMinute+1;
		}
		else if(segment==SegmentDate)
		{
			QDateTime bDatetime(beginDate,beginTime,appSettings->timeSpecValue);
			QDateTime eDatetime(endDate,endTime,appSettings->timeSpecValue);
			int dltSecs=eDatetime.toTime_t()-bDatetime.toTime_t();
			return dltSecs/60/cycleMinute+1;
		}
	}
	else if(cycle==MonthCycle)
	{
		if(segment==GeneralDate)
			return beginDate.daysInMonth();
		else if(segment==SegmentDate)
			return beginDate.daysTo(endDate);
	}
	else if(cycle==YearCycle)
	{
		if(segment==GeneralDate)
			return 12;
		else if(segment==SegmentDate)
			return (endDate.year()*12+endDate.month())-(beginDate.year()*12+beginDate.month());
	}
	return -1;
}

int selectorpara::getMinCycle( int parasIdx/*=-1*/ )
{
	int minVal = 0; //
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	for(int i=0;i<paras.size();i++)
	{
		if(parasIdx != i && parasIdx >= 0) continue;
		QString &paraname=paras[i];
		if(dataSrc==YcDataSrc)
		{
			YcParas::ConstIterator itFind=findItem<YcParas>(myApp->ycparas,paraname);
			if(itFind!=myApp->ycparas.constEnd())
			{
				if((*itFind)->savetime > 0 && i == 0)
					minVal = (*itFind)->savetime;
				else
					minVal=qMin<int>((*itFind)->savetime,minVal);
			}
		}
		else
		{
			KwhParas::ConstIterator itFind=findItem<KwhParas>(myApp->kwhparas,paraname);
			if(itFind!=myApp->kwhparas.constEnd())
			{
				if((*itFind)->savetime>0 && i == 0 )
					minVal = (*itFind)->savetime;
				else
					minVal=qMin<int>((*itFind)->savetime,minVal);
			}
		}
	}
	if(minVal == 0 )
		minVal = ((DataCurveApp*)qApp)->defaultMinCycle;
	return minVal;
}

QString selectorpara::getDescript( int parasIdx/*=0*/ )
{
	QString tmpStr=C2G("");
	if(paras.size()==0 || parasIdx>=paras.size()) return tmpStr;
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	QString &paraname=paras[parasIdx];
	if(dataSrc==YcDataSrc)
	{
		YcParas::ConstIterator itFind=findItem<YcParas>(myApp->ycparas,paraname);
		if(itFind!=myApp->ycparas.constEnd())
			tmpStr=(*itFind)->describe;
	}
	else
	{
		KwhParas::ConstIterator itFind=findItem<KwhParas>(myApp->kwhparas,paraname);
		if(itFind!=myApp->kwhparas.constEnd())
			tmpStr=(*itFind)->describe;
	}
	return tmpStr;
}


bool selectorpara::isCompType() const 
{
	if(selType==SingleSelector && comps.size() > 0)
		return true;
	else
		return false;
}

QDate selectorpara::compBeginDate( int compIdx ) const
{
	if(compIdx<comps.size())
	{
		return comps[compIdx];
	}
	return QDate::currentDate();
}

QTime selectorpara::compBeginTime() const
{
	return firstbeginTime;
}

QDate selectorpara::compEndDate(int compIdx) const
{
	int days=firstbeginDate.daysTo(firstendDate);
	return comps[compIdx].addDays(days);
}

QTime selectorpara::compEndTime() const
{
	return firstendTime;
}

void selectorpara::appendCompDate( QDate date )
{
	if(comps.size()==0)  //first append
	{
		normalizationDateTime(getMinCycle());
		firstbeginDate=date;
		firstbeginTime=beginTime;
		firstendDate=endDate;
		firstendTime=endTime;
	}
	comps.append(date);
}

bool selectorpara::removeCompDate( QDate date )
{
	for(int i=0;i<comps.size();i++)
	{
		if(comps[i]==date)		
		{
			comps.remove(i);
			return true;
		}
	}
	return false;
}

void selectorpara::clearCompDate()
{
	comps.clear();
}

bool selectorpara::isExistCompDate( QDate& date )
{
	for(int i=0;i<comps.size();i++)
	{
		if(comps[i]==date)		
		{
			return true;
		}
	}
	return false;
}

int selectorpara::compDateCount() const
{
	return comps.size();
}

bool selectorpara::isPlanType() const
{
	if(dataSrc == YcDataSrc && selType == SingleSelector && cycle == DayCycle && segment == GeneralDate && comps.size()<=0 && plantime>0)
	{
		return true;
	}
	return false;
}

void selectorpara::setPlantime(int plantime )
{
	this->plantime=plantime;
}





//////////////////////////////////////////////////////////////////////////
//// selectorcurvepara class

selectorcurvepara::selectorcurvepara()
{
	curvestyle=1;	//QwtPlotCurve::Lines
	attrAntialiased=true;
	attrFitted=false;
	showsymbol=false;
	showgrid=true;
// 	shownullval=true;
// 	showalignscales=false;
	nullValueStyle=NullValZero;
	yAxisAutoScale=true;
	yAxisScaleMin=0;
	yAxisScaleMax=1000;
}
