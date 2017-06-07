// dataselectortoolbar.cpp: implementation of the DataSelectorToolBar class.
//
//////////////////////////////////////////////////////////////////////

#include <QAction>
#include <QDateEdit>
#include <QLabel>
#include <QComboBox>
#include <QDateTime>
#include <QWidgetAction>
#include <QMessageBox>
#include <QMenu>
#include <QToolButton>
#include <QSpinBox>
#include <QLineEdit>

#include <QDebug>
#include "datacurveapp.h"
#include "dataselectortoolbar.h"

#include "qwt_plot_curve.h"
#include "uitools/dmessagebox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataSelectorToolBar::DataSelectorToolBar( QMainWindow *parent )
: QToolBar((QWidget *)parent),currentSelPara(static_cast<DataCurveApp*>(qApp)->selpara)
{
	this->parent=(QWidget *)parent;
	init();
}

DataSelectorToolBar::~DataSelectorToolBar()
{

}

void DataSelectorToolBar::init()
{
	//setObjectName(QString::fromUtf8("dataSelectorTools"));
	setWindowTitle(tr("����ʱ��ѡ�񹤾�"));
	setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	//setAllowedAreas(Qt::TopToolBarArea);
	//dataSelectorTools->setMovable(true);
	//dataSelectorTools->setFloatable(true);
	
	//2.0scada�汾��֧������������  chenkai 
// 	actionVariantDayCycle=new QAction(QIcon(":/res/day.png"),tr("�仯������"),parent);
// 	actionVariantDayCycle->setObjectName(QString::fromUtf8("actionVariantDayCycle"));
// 	actionVariantDayCycle->setCheckable(true);
// 	actionVariantDayCycle->setToolTip(tr("�仯��������������ͻ���"));
// 	connect(actionVariantDayCycle, SIGNAL(toggled(bool)), this, SLOT(variantCycleDayTriggered(bool)));
// 	addAction(actionVariantDayCycle);
// 
// 	activeTrendDayCycle=new QAction(QIcon(":/res/day.png"),tr("��������"),parent);
// 	activeTrendDayCycle->setObjectName(QString::fromUtf8("actionVariantDayCycle"));
// 	activeTrendDayCycle->setCheckable(true);
// 	activeTrendDayCycle->setToolTip(tr("����������������ͻ���"));
// 	connect(activeTrendDayCycle, SIGNAL(toggled(bool)), this, SLOT(trendCycleDayTriggered(bool)));
// 	addAction(activeTrendDayCycle);

	actionDayCycle=new QAction(QIcon(":/res/day.png"),tr("������"),parent);
	actionDayCycle->setObjectName(QString::fromUtf8("actionDayCycle"));
	actionDayCycle->setCheckable(true);
	actionDayCycle->setToolTip(tr("��������������ͻ���"));
	connect(actionDayCycle, SIGNAL(toggled(bool)), this, SLOT(cycleDayTriggered(bool)));
	addAction(actionDayCycle);
	

	actionMonCycle=new QAction(QIcon(":/res/month.png"),tr("������"),parent);
	actionMonCycle->setObjectName(QString::fromUtf8("actionMonCycle"));
	actionMonCycle->setCheckable(true);
	actionMonCycle->setToolTip(tr("������ͳ����������ͻ���"));
	connect(actionMonCycle, SIGNAL(toggled(bool)), this, SLOT(cycleMonthTriggered(bool)));
	addAction(actionMonCycle);


	actionYearCycle=new QAction(QIcon(":/res/year.png"),tr("������"),parent);
	actionYearCycle->setObjectName(QString::fromUtf8("actionYearCycle"));
	actionYearCycle->setCheckable(true);
	actionYearCycle->setToolTip(tr("������ͳ����������ͻ���"));
	connect(actionYearCycle, SIGNAL(toggled(bool)), this, SLOT(cycleYearTriggered(bool)));
	addAction(actionYearCycle);
	
	QActionGroup *cycleGroup=new QActionGroup(parent);
// 	cycleGroup->addAction(actionVariantDayCycle);
// 	cycleGroup->addAction(activeTrendDayCycle);
	cycleGroup->addAction(actionDayCycle);
	cycleGroup->addAction(actionMonCycle);
	cycleGroup->addAction(actionYearCycle);
	
// 	addSeparator();
	
	actionGeneralDate=new QAction(QIcon(":/res/generaldate.png"),tr("��׼"),parent);
	actionGeneralDate->setObjectName(QString::fromUtf8("segmentdate.png"));
	actionGeneralDate->setCheckable(true);
	actionGeneralDate->setToolTip(tr("��ѡ������������������"));
	connect(actionGeneralDate, SIGNAL(toggled(bool)), this, SLOT(generalDateTriggered(bool)));
	addAction(actionGeneralDate);

	actionSegmentDate=new QAction(QIcon(":/res/segmentdate.png"),tr("�ֶ�"),parent);
	actionSegmentDate->setObjectName(QString::fromUtf8("actionSegmentDate"));
	actionSegmentDate->setCheckable(true);
	actionSegmentDate->setToolTip(tr("����ʱ�ֶη�ʽ�������"));
	connect(actionSegmentDate, SIGNAL(toggled(bool)), this, SLOT(segmentDateTriggered(bool)));
	addAction(actionSegmentDate);
	
	QActionGroup *segmentGroup=new QActionGroup(parent);
	segmentGroup->addAction(actionGeneralDate);
	segmentGroup->addAction(actionSegmentDate);

// 	addSeparator();
	

	actionDateEdit_1Lbl=new QWidgetAction(parent);
	actionDateEdit_1Lbl->setObjectName(QString::fromUtf8("actionDateEdit_1Lbl"));
	dateEdit_1Lbl=new QLabel(tr("��ʼ����:"),parent);
	dateEdit_1Lbl->setObjectName(QString::fromUtf8("dateEdit_1Lbl"));
	actionDateEdit_1Lbl->setDefaultWidget(dateEdit_1Lbl);
	addAction(actionDateEdit_1Lbl);

	actionDateEdit_1=new QWidgetAction(parent);
	actionDateEdit_1->setObjectName(QString::fromUtf8("actionDateEdit_1"));
	dateEdit_1=new QDateEdit(parent);
	dateEdit_1->setObjectName(QString::fromUtf8("dateEdit_1"));
	dateEdit_1->setDisplayFormat(tr("yyyy��MM��dd��"));
	dateEdit_1->setCalendarPopup(true);
	//dateEdit_1->setMinimumWidth(111);
	dateEdit_1->setDate(currentSelPara.beginDate);
	connect(dateEdit_1, SIGNAL(dateChanged(const QDate &)), this, SLOT(dateEdit_1Changed(const QDate &)));
	actionDateEdit_1->setDefaultWidget(dateEdit_1);
	addAction(actionDateEdit_1);

	actionsdateLel=new QWidgetAction(parent);
	actionsdateLel->setObjectName(QString::fromUtf8("actionsdateLel"));
	sdateEdit=new QLineEdit("sdate:",parent);
	sdateEdit->setMaximumWidth(100);
	sdateEdit->setObjectName(QString::fromUtf8("sdateEdit"));
	sdateEdit->setFocusPolicy(Qt::NoFocus);
	actionsdateLel->setDefaultWidget(sdateEdit);
	actionsdateLel->setVisible(false);
	addAction(actionsdateLel);
	
	
	actionTimeEdit_1Lbl=new QWidgetAction(parent);
	actionTimeEdit_1Lbl->setObjectName(QString::fromUtf8("actionTimeEdit_1Lbl"));
	timeEdit_1Lbl=new QLabel(tr(" ʱ��:"),parent);
	timeEdit_1Lbl->setObjectName(QString::fromUtf8("timeEdit_1Lbl"));
	actionTimeEdit_1Lbl->setDefaultWidget(timeEdit_1Lbl);
	addAction(actionTimeEdit_1Lbl);

	
	actionTimeEdit_1=new QWidgetAction(parent);
	actionTimeEdit_1->setObjectName(QString::fromUtf8("actionTimeEdit_1"));
	timeEdit_1=new QTimeEdit(parent);
	timeEdit_1->setObjectName(QString::fromUtf8("timeEdit_1"));
	timeEdit_1->setDisplayFormat(tr("HH:mm"));
	//timeEdit_1->setMinimumWidth(61);
	timeEdit_1->setTime(currentSelPara.beginTime);
	actionTimeEdit_1->setDefaultWidget(timeEdit_1);
	connect(timeEdit_1, SIGNAL(editingFinished()), this, SLOT(timeEdit_1Changed()));
	addAction(actionTimeEdit_1);
	
	
	actionDateEdit_2Lbl=new QWidgetAction(parent);
	actionDateEdit_2Lbl->setObjectName(QString::fromUtf8("actionDateEdit_2Lbl"));
	dateEdit_2Lbl=new QLabel(tr("��������:"),parent);
	dateEdit_2Lbl->setObjectName(QString::fromUtf8("dateEdit_2Lbl"));
	actionDateEdit_2Lbl->setDefaultWidget(dateEdit_2Lbl);
	addAction(actionDateEdit_2Lbl);
	
	
	actionDateEdit_2=new QWidgetAction(parent);
	actionDateEdit_2->setObjectName(QString::fromUtf8("actionDateEdit_2"));
	dateEdit_2=new QDateEdit(parent);
	dateEdit_2->setObjectName(QString::fromUtf8("dateEdit_2"));
	dateEdit_2->setDisplayFormat(tr("yyyy��MM��dd��"));
	dateEdit_2->setCalendarPopup(true);
	//dateEdit_2->setMinimumWidth(111);
	//dateEdit_2->setMaximumWidth(111);
	dateEdit_2->setDate(currentSelPara.endDate);
	actionDateEdit_2->setDefaultWidget(dateEdit_2);
	connect(dateEdit_2, SIGNAL(dateChanged(const QDate &)), this, SLOT(dateEdit_2Changed(const QDate &)));
	addAction(actionDateEdit_2);
	
	
	actionTimeEdit_2Lbl=new QWidgetAction(parent);
	actionTimeEdit_2Lbl->setObjectName(QString::fromUtf8("actionTimeEdit_2Lbl"));
	timeEdit_2Lbl=new QLabel(tr(" ʱ��:"),parent);
	timeEdit_2Lbl->setObjectName(QString::fromUtf8("timeEdit_2Lbl"));
	actionTimeEdit_2Lbl->setDefaultWidget(timeEdit_2Lbl);
	addAction(actionTimeEdit_2Lbl);
	

	
	actionTimeEdit_2=new QWidgetAction(parent);
	actionTimeEdit_2->setObjectName(QString::fromUtf8("actionTimeEdit_2"));
	timeEdit_2=new QTimeEdit(parent);
	timeEdit_2->setObjectName(QString::fromUtf8("timeEdit_2"));
	timeEdit_2->setDisplayFormat(tr("HH:mm"));
	//timeEdit_2->setMinimumWidth(61);
	timeEdit_2->setTime(currentSelPara.endTime);
	actionTimeEdit_2->setDefaultWidget(timeEdit_2);
	connect(timeEdit_2, SIGNAL(editingFinished()), this, SLOT(timeEdit_2Changed()));
	addAction(actionTimeEdit_2);
	
	
	actionBoxDataTypeLbl=new QWidgetAction(parent);
	actionBoxDataTypeLbl->setObjectName(QString::fromUtf8("actionBoxDataTypeLbl"));
	boxDataTypeLbl=new QLabel(tr(" ����:"),parent);
	boxDataTypeLbl->setObjectName(QString::fromUtf8("boxDataTypeLbl"));
	actionBoxDataTypeLbl->setDefaultWidget(boxDataTypeLbl);
	addAction(actionBoxDataTypeLbl);
	
	
	actionBoxDataType=new QWidgetAction(parent);
	actionBoxDataType->setObjectName(QString::fromUtf8("actionBoxDataType"));
	boxDataType=new QComboBox(parent);
	boxDataType->setEditable(false);
	boxDataType->addItem(tr("������"),QVariant(HISDATA_DAYS_PQRATE));
	boxDataType->addItem(tr("���ֵ"),QVariant(HISDATA_DAYS_MAX));
	boxDataType->addItem(tr("��Сֵ"),QVariant(HISDATA_DAYS_MIN));
	boxDataType->addItem(tr("ƽ��ֵ"),QVariant(HISDATA_DAYS_AVERAGE));
	boxDataType->addItem(tr("�ϸ���"),QVariant(HISDATA_DAYS_HGL));
	//boxDataType->setMinimumWidth(81);
	actionBoxDataType->setDefaultWidget(boxDataType);
	connect(boxDataType, SIGNAL(currentIndexChanged(int)), this, SLOT(boxDataTypeIndexChanged(int)));
	addAction(actionBoxDataType);
	
	//////////////////////////////////////////////////////////////////////////
	dayMinCycleLbl=new QLabel(tr("��С����:"),parent);
	dayMinCycleLbl->setObjectName(QString::fromUtf8("dayCycleLbl"));
	actionDayMinCycleLbl=new QWidgetAction(parent);
	actionDayMinCycleLbl->setObjectName(QString::fromUtf8("actionDayCycleLbl"));
	actionDayMinCycleLbl->setDefaultWidget(dayMinCycleLbl);
	addAction(actionDayMinCycleLbl);
	
	boxDayMinCycle=new QComboBox(parent);
	boxDayMinCycle->setObjectName(QString::fromUtf8("boxDayCycle"));
	boxDayMinCycle->setEditable(false);
	boxDayMinCycle->addItem(tr("�Զ�ѡ��"),QVariant(0));
	boxDayMinCycle->addItem(tr("1����"),QVariant(1));
	boxDayMinCycle->addItem(tr("5����"),QVariant(5));
	boxDayMinCycle->addItem(tr("15����"),QVariant(15));
	boxDayMinCycle->addItem(tr("30����"),QVariant(30));
	boxDayMinCycle->addItem(tr("60����"),QVariant(60));
	actionBoxDayMinCycle=new QWidgetAction(parent);
	actionBoxDayMinCycle->setObjectName(QString::fromUtf8("actionBoxDayCycle"));
	actionBoxDayMinCycle->setDefaultWidget(boxDayMinCycle);
	connect(boxDayMinCycle, SIGNAL(currentIndexChanged(int)), this, SLOT(boxDayMinCycleIndexChanged(int)));
	addAction(actionBoxDayMinCycle);
	
// 	actionSepComp=addSeparator();

	actionInsComp=new QAction(QIcon(":/res/inscomp.png"),tr("��ͬ��"),parent);
	actionInsComp->setObjectName(QString::fromUtf8("actionInsComp"));
	actionInsComp->setToolTip(tr("���ͬ������"));
	connect(actionInsComp, SIGNAL(activated()), this, SLOT(insComp()));
	addAction(actionInsComp);

	//
	btnDelComp=new QToolButton(parent);
	btnDelComp->setPopupMode(QToolButton::MenuButtonPopup);
	btnDelComp->setObjectName(QString::fromUtf8("btnDelComp"));
	btnDelComp->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	btnDelComp->setVisible(false);
	actionDelComp=addWidget(btnDelComp);


	actionDayCycle->setChecked(true);
	cycleDayTriggered(true);
	actionGeneralDate->setChecked(true);
}


void DataSelectorToolBar::variantCycleDayTriggered( bool checked )
{
	if(checked)
	{
		currentSelPara.cycle=VariantDayCycle;
		currentSelPara.segment=GeneralDate;
		actionGeneralDate->setChecked(true);
		actionSegmentDate->setChecked(false);
		currentSelPara.beginTime=QTime(0,0,0,0);
		currentSelPara.endDate=currentSelPara.beginDate.addDays(1);
		currentSelPara.endTime=QTime(0,0,0,0);
		currentSelPara.statType=HISDATA_REAL;
		currentSelPara.dayCurveMinCycle=0;
		modifyCurveDataToolBars(currentSelPara.cycle,currentSelPara.segment);
		updateSelPara(currentSelPara,true);

		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void DataSelectorToolBar::trendCycleDayTriggered( bool checked )
{
	if(checked)
	{
		currentSelPara.cycle=TrendDayCycle;
// 		currentSelPara.cycle=VariantDayCycle;
		currentSelPara.segment=GeneralDate;
		actionGeneralDate->setChecked(true);
		actionSegmentDate->setChecked(false);
		currentSelPara.beginTime=QTime(0,0,0,0);
		currentSelPara.endDate=currentSelPara.beginDate.addDays(1);
		currentSelPara.endTime=QTime(0,0,0,0);
		currentSelPara.statType=HISDATA_REAL;
		currentSelPara.dayCurveMinCycle=0;
		modifyCurveDataToolBars(currentSelPara.cycle,currentSelPara.segment);
		updateSelPara(currentSelPara,true);

		Q_EMIT(selectorChanged(currentSelPara));
	}
}


void DataSelectorToolBar::cycleDayTriggered( bool checked )
{
	if(checked)
	{
		currentSelPara.cycle=DayCycle;
		currentSelPara.segment=GeneralDate;
		actionGeneralDate->setChecked(true);
		actionSegmentDate->setChecked(false);
		currentSelPara.beginTime=QTime(0,0,0,0);
		currentSelPara.endDate=currentSelPara.beginDate.addDays(1);
		currentSelPara.endTime=QTime(0,0,0,0);
		currentSelPara.statType=HISDATA_REAL;
		currentSelPara.dayCurveMinCycle=0;
		modifyCurveDataToolBars(currentSelPara.cycle,currentSelPara.segment);
		updateSelPara(currentSelPara,true);

		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void DataSelectorToolBar::cycleMonthTriggered( bool checked )
{
	if(checked)
	{
		currentSelPara.clearCompDate();
		currentSelPara.cycle=MonthCycle;
		currentSelPara.segment=GeneralDate;
		actionGeneralDate->setChecked(true);
		actionSegmentDate->setChecked(false);
		currentSelPara.beginTime=QTime(0,0,0,0);
		currentSelPara.endDate=currentSelPara.beginDate.addMonths(1);
		currentSelPara.endTime=QTime(0,0,0,0);
		currentSelPara.statType=HISDATA_DAYS_PQRATE;
		currentSelPara.dayCurveMinCycle=0;
		modifyCurveDataToolBars(currentSelPara.cycle,currentSelPara.segment);
		updateSelPara(currentSelPara,true);
		dataSrcChanged();   //modify month datatype 
		Q_EMIT(selectorChanged(currentSelPara));
	}
	return;
}

void DataSelectorToolBar::cycleYearTriggered( bool checked )
{
	if(checked)
	{
		currentSelPara.clearCompDate();
		currentSelPara.cycle=YearCycle;
		currentSelPara.segment=GeneralDate;
		actionGeneralDate->setChecked(true);
		actionSegmentDate->setChecked(false);
		currentSelPara.beginTime=QTime(0,0,0,0);
		currentSelPara.endDate=currentSelPara.beginDate.addYears(1);
		currentSelPara.endTime=QTime(0,0,0,0);
		currentSelPara.statType=HISDATA_DAYS_PQRATE;
		currentSelPara.dayCurveMinCycle=0;
		modifyCurveDataToolBars(currentSelPara.cycle,currentSelPara.segment);
		updateSelPara(currentSelPara,true);

		Q_EMIT(selectorChanged(currentSelPara));
	}
	return;
}

void DataSelectorToolBar::generalDateTriggered( bool checked )
{
	if(checked)
	{
		currentSelPara.segment=GeneralDate;
		if(currentSelPara.cycle==DayCycle)
			cycleDayTriggered(true);
		else if(currentSelPara.cycle==MonthCycle)
			cycleMonthTriggered(true);
		else if(currentSelPara.cycle==YearCycle)
			cycleYearTriggered(true);
	}
	return;
}

void DataSelectorToolBar::segmentDateTriggered( bool checked )
{
	if(checked)
	{
		currentSelPara.segment=SegmentDate;
		if(currentSelPara.beginDate<currentSelPara.endDate)
		{
			if(currentSelPara.cycle==DayCycle)
				currentSelPara.endDate=currentSelPara.beginDate.addDays(1);
			else if(currentSelPara.cycle==MonthCycle)
				currentSelPara.endDate=currentSelPara.beginDate.addMonths(1);
			else if(currentSelPara.cycle==YearCycle)
				currentSelPara.endDate=currentSelPara.beginDate.addYears(1);
		}
		currentSelPara.endTime=currentSelPara.beginTime;
		modifyCurveDataToolBars(currentSelPara.cycle,currentSelPara.segment);
		updateSelPara(currentSelPara,true);
	}
	return;
}

void DataSelectorToolBar::dateEdit_1Changed( const QDate & date )
{
	if(date!=currentSelPara.beginDate)
	{
		if(currentSelPara.segment==SegmentDate && date>currentSelPara.endDate)
		{
			DMessageBox::warning(this,tr("�����������"),tr("�������ʼ����С�ڽ������ڣ����������룡"),
				QMessageBox::Ok,QMessageBox::NoButton);
			return;
		}
		else if(currentSelPara.segment==GeneralDate)
		{
			currentSelPara.endDate=date.addDays(1);
		}
		currentSelPara.beginDate=date;
		
		//if general segment emit update model 
		if(currentSelPara.segment==GeneralDate)
		{
			Q_EMIT(selectorChanged(currentSelPara));
		}

		//show sdate
		int sdate;
		DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
		if(currentSelPara.cycle==YearCycle)
			sdate=myApp->dataCurveDef.ConvertDateToMons(date);
		else
			sdate=myApp->dataCurveDef.convertDateToDays(date);
		sdateEdit->setText(QString("sdate:[%1]").arg(sdate));
		//currentSelPara.debugOut();
	}
}

void DataSelectorToolBar::dateEdit_2Changed( const QDate & date )
{
	if(date!=currentSelPara.endDate)
	{
		if(date<currentSelPara.beginDate)
		{
			DMessageBox::warning(this,tr("�����������"),tr("����Ľ�������С�ڿ�ʼ���ڣ����������룡"),
				QMessageBox::Ok,QMessageBox::NoButton);
			return;
		}
		currentSelPara.endDate=date;
		//currentSelPara.debugOut();
		
	}
}

void DataSelectorToolBar::timeEdit_1Changed()
{
	if(timeEdit_1->time()!=currentSelPara.beginTime)
	{
		currentSelPara.beginTime=timeEdit_1->time();
		//currentSelPara.debugOut();
	}
}

void DataSelectorToolBar::timeEdit_2Changed()
{
	if(timeEdit_2->time()!=currentSelPara.endTime)
	{
		currentSelPara.endTime=timeEdit_2->time();
		//currentSelPara.debugOut();
	}
}

void DataSelectorToolBar::boxDataTypeIndexChanged( int index )
{
	if(index==-1) return;
	int staticType=boxDataType->itemData(index).toUInt();
	if(currentSelPara.statType!=staticType)
	{
		currentSelPara.statType=staticType;
		Q_EMIT(selectorChanged(currentSelPara));
	}
	
}


void DataSelectorToolBar::boxDayMinCycleIndexChanged( int index )
{
	if(index==-1) return;
	int dayCurveMinCycle=boxDayMinCycle->itemData(index).toUInt();
	if(currentSelPara.dayCurveMinCycle!=dayCurveMinCycle)
	{
		currentSelPara.dayCurveMinCycle=dayCurveMinCycle;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}


void DataSelectorToolBar::insComp()
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	if(currentSelPara.paras.size()==0)
	{
		DMessageBox::warning(this,tr("��ʾ"),
			QString(tr("��ǰδѡ�����ӵ��豸����\r\n�����豸��������ѡ�����������ԡ�")),
				QMessageBox::Ok,QMessageBox::NoButton);
		return;
	}
	if(currentSelPara.selType==SingleSelector)
	{
		if(currentSelPara.compDateCount() >= appSettings->maxCurveNumber)	
		{
			DMessageBox::warning(this,tr("��ʾ"),
				QString(tr("ѡ��ͬ�ȵ����߸����ﵽ������(%1)����")).arg(appSettings->maxCurveNumber),
				QMessageBox::Ok,QMessageBox::NoButton);
			return;
		}
		if(currentSelPara.isExistCompDate(currentSelPara.beginDate))
		{
			DMessageBox::warning(this,tr("��ʾ"),
				QString(tr("ѡ��������Ѵ��ڣ�������ѡ��Ա����ں�����ӡ�")),
				QMessageBox::Ok,QMessageBox::NoButton);
			return;
		}
	}
	currentSelPara.appendCompDate(currentSelPara.beginDate);
	updCompPopmenu();
	Q_EMIT(selectorChanged(currentSelPara));
}

void DataSelectorToolBar::delComp()
{
	QObject *src=this->sender();
	if(src->inherits("QAction"))
	{
		QDateTime date=((QAction*)src)->data().toDateTime();
		currentSelPara.removeCompDate(date.date());
		updCompPopmenu();
		Q_EMIT(selectorChanged(currentSelPara));
	}
// 
// 	if(currentSelPara.isExistCompDate(currentSelPara.beginDate))
// 	{
// 		currentSelPara.removeCompDate(currentSelPara.beginDate);
// 	}
// 	else
// 	{
// 		QMessageBox::warning(this,tr("��ʾ"),
// 			QString(tr("ѡ������ڲ�������ͬ���б��У�������ѡ��ͬ�����ں���ɾ����")),
// 			QMessageBox::Ok,QMessageBox::NoButton);
// 		return;
// 	}
}

//////////////////////////////////////////////////////////////////////////
////

void DataSelectorToolBar::modifyCurveDataToolBars(CurveCycleType curveCycle,CurveSegmentType curveSeg )
{
	switch(curveCycle)
	{
	case VariantDayCycle:
	case TrendDayCycle:
		{
			dateEdit_1Lbl->setText(tr("����:"));
			dateEdit_1->setDisplayFormat(tr("yyyy��MM��dd��"));

			actionDateEdit_1Lbl->setVisible(true);
			actionDateEdit_1->setVisible(true);

			actionTimeEdit_1Lbl->setVisible(false);
			actionTimeEdit_1->setVisible(false);

			actionDateEdit_2Lbl->setVisible(false);
			actionDateEdit_2->setVisible(false);

			actionTimeEdit_2Lbl->setVisible(false);
			actionTimeEdit_2->setVisible(false);

			actionBoxDataTypeLbl->setVisible(false);
			actionBoxDataType->setVisible(false);

			actionDayMinCycleLbl->setVisible(false);
			actionBoxDayMinCycle->setVisible(false);

			actionSegmentDate->setVisible(false);
			actionGeneralDate->setVisible(false);


//			actionSepComp->setVisible(false);
			actionInsComp->setVisible(false);
			actionDelComp->setVisible(false);

		}
		break;
	case DayCycle:
		actionSegmentDate->setVisible(true);
		actionGeneralDate->setVisible(true);

		if(curveSeg==GeneralDate)
		{
			dateEdit_1Lbl->setText(tr("����:"));
			dateEdit_1->setDisplayFormat(tr("yyyy��MM��dd��"));
			
			actionDateEdit_1Lbl->setVisible(true);
			actionDateEdit_1->setVisible(true);

			actionTimeEdit_1Lbl->setVisible(false);
			actionTimeEdit_1->setVisible(false);

			actionDateEdit_2Lbl->setVisible(false);
			actionDateEdit_2->setVisible(false);
			
			actionTimeEdit_2Lbl->setVisible(false);
			actionTimeEdit_2->setVisible(false);

			actionBoxDataTypeLbl->setVisible(false);
			actionBoxDataType->setVisible(false);
			
			actionDayMinCycleLbl->setVisible(true);
			actionBoxDayMinCycle->setVisible(true);
			
//			actionSepComp->setVisible(true);
			actionInsComp->setVisible(true);
			if(currentSelPara.compDateCount()>0)
				actionDelComp->setVisible(true);
			else 
				actionDelComp->setVisible(false);
		}
		else
		{
			dateEdit_1Lbl->setText(tr("��ʼ����"));
			dateEdit_2Lbl->setText(tr("��������"));
			dateEdit_1->setDisplayFormat(tr("yyyy��MM��dd��"));
			dateEdit_2->setDisplayFormat(tr("yyyy��MM��dd��"));
			
			actionDateEdit_1Lbl->setVisible(true);
			actionDateEdit_1->setVisible(true);
			
			actionTimeEdit_1Lbl->setVisible(true);
			actionTimeEdit_1->setVisible(true);
			
			actionDateEdit_2Lbl->setVisible(true);
			actionDateEdit_2->setVisible(true);
			
			actionTimeEdit_2Lbl->setVisible(true);
			actionTimeEdit_2->setVisible(true);
			
			actionBoxDataTypeLbl->setVisible(false);
			actionBoxDataType->setVisible(false);

			actionDayMinCycleLbl->setVisible(true);
			actionBoxDayMinCycle->setVisible(true);

//			actionSepComp->setVisible(false);
			actionInsComp->setVisible(false);
			actionDelComp->setVisible(false);
		}
		break;
	case MonthCycle:
		actionSegmentDate->setVisible(true);
		actionGeneralDate->setVisible(true);
		if(curveSeg==GeneralDate)
		{
			dateEdit_1Lbl->setText(tr("����:"));
			dateEdit_1->setDisplayFormat(tr("yyyy��MM��"));

			actionDateEdit_1Lbl->setVisible(true);
			actionDateEdit_1->setVisible(true);
			

			actionTimeEdit_1Lbl->setVisible(false);
			actionTimeEdit_1->setVisible(false);
			
			actionDateEdit_2Lbl->setVisible(false);
			actionDateEdit_2->setVisible(false);
			
			actionTimeEdit_2Lbl->setVisible(false);
			actionTimeEdit_2->setVisible(false);
			
			actionBoxDataTypeLbl->setVisible(true);
			actionBoxDataType->setVisible(true);

			actionDayMinCycleLbl->setVisible(false);
			actionBoxDayMinCycle->setVisible(false);
			
//			actionSepComp->setVisible(false);
			actionInsComp->setVisible(false);
			actionDelComp->setVisible(false);
			currentSelPara.clearCompDate();
			
		}
		else
		{
			dateEdit_1Lbl->setText(tr("��ʼ����:"));
			dateEdit_2Lbl->setText(tr("��������:"));
			dateEdit_1->setDisplayFormat(tr("yyyy��MM��dd��"));
			dateEdit_2->setDisplayFormat(tr("yyyy��MM��dd��"));
			
			actionDateEdit_1Lbl->setVisible(true);
			actionDateEdit_1->setVisible(true);
			
			actionTimeEdit_1Lbl->setVisible(false);
			actionTimeEdit_1->setVisible(false);
			
			actionDateEdit_2Lbl->setVisible(true);
			actionDateEdit_2->setVisible(true);
			
			actionTimeEdit_2Lbl->setVisible(false);
			actionTimeEdit_2->setVisible(false);
			
			actionBoxDataTypeLbl->setVisible(true);
			actionBoxDataType->setVisible(true);
			actionDayMinCycleLbl->setVisible(false);
			actionBoxDayMinCycle->setVisible(false);

//			actionSepComp->setVisible(false);
			actionInsComp->setVisible(false);
			actionDelComp->setVisible(false);
			currentSelPara.clearCompDate();
		}
		break;
	case YearCycle:
		actionSegmentDate->setVisible(true);
		actionGeneralDate->setVisible(true);
		if(curveSeg==GeneralDate)
		{
			dateEdit_1Lbl->setText(tr("����:"));
			dateEdit_1->setDisplayFormat(tr("yyyy��"));
			
			actionDateEdit_1Lbl->setVisible(true);
			actionDateEdit_1->setVisible(true);
			
			
			actionTimeEdit_1Lbl->setVisible(false);
			actionTimeEdit_1->setVisible(false);
			
			actionDateEdit_2Lbl->setVisible(false);
			actionDateEdit_2->setVisible(false);
			
			actionTimeEdit_2Lbl->setVisible(false);
			actionTimeEdit_2->setVisible(false);
			
			actionBoxDataTypeLbl->setVisible(true);
			actionBoxDataType->setVisible(true);

			actionDayMinCycleLbl->setVisible(false);
			actionBoxDayMinCycle->setVisible(false);

//			actionSepComp->setVisible(false);
			actionInsComp->setVisible(false);
			actionDelComp->setVisible(false);
			currentSelPara.clearCompDate();
			
		}
		else
		{
			dateEdit_1Lbl->setText(tr("��ʼ����:"));
			dateEdit_2Lbl->setText(tr("��������:"));
			dateEdit_1->setDisplayFormat(tr("yyyy��MM��"));
			dateEdit_2->setDisplayFormat(tr("yyyy��MM��"));
			
			actionDateEdit_1Lbl->setVisible(true);
			actionDateEdit_1->setVisible(true);
			
			actionTimeEdit_1Lbl->setVisible(false);
			actionTimeEdit_1->setVisible(false);
			
			actionDateEdit_2Lbl->setVisible(true);
			actionDateEdit_2->setVisible(true);
			
			actionTimeEdit_2Lbl->setVisible(false);
			actionTimeEdit_2->setVisible(false);
			
			actionBoxDataTypeLbl->setVisible(true);
			actionBoxDataType->setVisible(true);

			actionDayMinCycleLbl->setVisible(false);
			actionBoxDayMinCycle->setVisible(false);

//			actionSepComp->setVisible(false);
			actionInsComp->setVisible(false);
			actionDelComp->setVisible(false);
			currentSelPara.clearCompDate();
		}
		break;
	}
}


CurveCycleType DataSelectorToolBar::getCycle()
{
	return currentSelPara.cycle;
}

CurveSegmentType DataSelectorToolBar::getSegment()
{
	return currentSelPara.segment;
}

int DataSelectorToolBar::getDataType()
{
	return boxDataType->itemData(boxDataType->currentIndex()).toUInt();
}

QDateTime DataSelectorToolBar::getBeginDateTime()
{
	QDateTime bDate=QDateTime(dateEdit_1->date(),timeEdit_1->time() , ((DataCurveApp*)qApp)->timeSpecValue);;
	if(currentSelPara.cycle==DayCycle)
	{
		if(currentSelPara.segment==GeneralDate)
			bDate=QDateTime(dateEdit_1->date(),QTime(0,0,0) , ((DataCurveApp*)qApp)->timeSpecValue);
	}
	else if(currentSelPara.cycle==MonthCycle)
	{
		bDate=QDateTime(dateEdit_1->date(),QTime(0,0,0) , ((DataCurveApp*)qApp)->timeSpecValue);
	}
	else if(currentSelPara.cycle==YearCycle)
	{
		bDate=QDateTime(dateEdit_1->date(),QTime(0,0,0) , ((DataCurveApp*)qApp)->timeSpecValue);
	}
	return bDate;
}

QDateTime DataSelectorToolBar::getEndDateTime()
{
	QDateTime eDate=QDateTime(dateEdit_2->date(),timeEdit_2->time() , ((DataCurveApp*)qApp)->timeSpecValue);
	if(currentSelPara.cycle==DayCycle)
	{
		if(currentSelPara.segment==GeneralDate)
			eDate=QDateTime(dateEdit_1->date(),QTime(0,0,0) , ((DataCurveApp*)qApp)->timeSpecValue);
	}
	else if(currentSelPara.segment==MonthCycle)
	{
		eDate=QDateTime(dateEdit_2->date(),QTime(0,0,0) , ((DataCurveApp*)qApp)->timeSpecValue );
	}
	else if(currentSelPara.cycle==YearCycle)
	{
		eDate=QDateTime(dateEdit_2->date(),QTime(0,0,0) , ((DataCurveApp*)qApp)->timeSpecValue);
	}
	return eDate;
}

void DataSelectorToolBar::updateSelPara( selectorpara& selpara,bool isset/*=false*/ )
{	
	if(isset)
	{
// 		if(selpara.cycle==DayCycle)
// 			cycleDayTriggered(true);
// 		else if(selpara.cycle==MonthCycle)
// 			cycleMonthTriggered(true);
// 		else if(selpara.cycle==YearCycle)
// 			cycleYearTriggered(true);
// 		if(selpara.segment==GeneralDate)
// 			generalDateTriggered(true);
// 		else if(selpara.segment==SegmentDate)
// 			segmentDateTriggered(true);
			
		dateEdit_1->setDate(selpara.beginDate);
		timeEdit_1->setTime(selpara.beginTime);
		dateEdit_2->setDate(selpara.endDate);
		timeEdit_2->setTime(selpara.endTime);
		for(int i=0;i<boxDataType->count();i++)
		{
			int staticType=boxDataType->itemData(i).toUInt();
			if(selpara.statType==staticType)
			{
				boxDataType->setCurrentIndex(i);
				break;
			}
		}
	}
	else
	{
		if(actionDayCycle->isChecked())
			selpara.cycle=DayCycle;
		else if(actionMonCycle->isChecked())
			selpara.cycle=MonthCycle;
		else if(actionYearCycle->isChecked())
			selpara.cycle=YearCycle;
		if(actionGeneralDate->isChecked())
			selpara.segment=GeneralDate;
		else if(actionSegmentDate->isChecked())
			selpara.segment=SegmentDate;

		selpara.beginDate=dateEdit_1->date();
		selpara.beginTime=timeEdit_1->time();
		selpara.endDate=dateEdit_2->date();
		selpara.endTime=timeEdit_2->time();
		if(boxDataType->currentIndex()>=0)
			selpara.statType=boxDataType->itemData(boxDataType->currentIndex()).toUInt();

	}
}

void DataSelectorToolBar::showSDateText(bool show)
{
	actionsdateLel->setVisible(show);
}

void DataSelectorToolBar::updCompPopmenu()
{
	//dynamic create qaction comp menu
	QMenu *popMenu=btnDelComp->menu();
	if(popMenu==0)
	{
		popMenu = new QMenu(this);
	}
	popMenu->clear();
	if(currentSelPara.isCompType())	
	{
		for(int i=0;i<currentSelPara.compDateCount();i++)
		{
			QString  tmpStr;
			if(currentSelPara.selType==DayCycle)
				tmpStr=QString(tr("�Ƴ�ͬ��:%1")).arg(currentSelPara.compBeginDate(i).toString("yyyy-MM-dd"));
			else if(currentSelPara.selType==MonthCycle)
				tmpStr=QString(tr("�Ƴ�ͬ��:%1")).arg(currentSelPara.compBeginDate(i).toString("yyyy-MM"));
			else if(currentSelPara.selType==YearCycle)
				tmpStr=QString(tr("�Ƴ�ͬ��:%1")).arg(currentSelPara.compBeginDate(i).toString("yyyy"));
			QAction *tmpAction=new QAction(QIcon(":/res/delcomp.png"),tmpStr,popMenu);
			tmpAction->setData(QVariant(currentSelPara.compBeginDate(i)));
			connect(tmpAction, SIGNAL(activated()), this, SLOT(delComp()));
			popMenu->addAction(tmpAction);
			if(i==0)
				btnDelComp->setDefaultAction(tmpAction);
		}
		btnDelComp->setMenu(popMenu);
		connect(popMenu, SIGNAL(triggered(QAction *)), btnDelComp, SLOT(setDefaultAction(QAction *)));
	
	}
	if(currentSelPara.compDateCount()>0)
	{
		actionDelComp->setVisible(true);
	}
	if(!currentSelPara.isCompType() || currentSelPara.compDateCount()==0)
	{
		currentSelPara.clearCompDate();
		actionDelComp->setVisible(false);
	}
}

void DataSelectorToolBar::dataSrcChanged()
{
	boxDataType->clear();
	if(currentSelPara.dataSrc==YcDataSrc && currentSelPara.cycle==MonthCycle)
	{
		boxDataType->addItem(tr("������"),QVariant(HISDATA_DAYS_PQRATE));
		boxDataType->addItem(tr("���ֵ"),QVariant(HISDATA_DAYS_MAX));
		boxDataType->addItem(tr("��Сֵ"),QVariant(HISDATA_DAYS_MIN));
		boxDataType->addItem(tr("ƽ��ֵ"),QVariant(HISDATA_DAYS_AVERAGE));
		boxDataType->addItem(tr("�ϸ���"),QVariant(HISDATA_DAYS_HGL));
		boxDataTypeIndexChanged(0);
	}
	else if(currentSelPara.dataSrc==DdDataSrc && currentSelPara.cycle==MonthCycle)
	{
		boxDataType->addItem(tr("1ʱ���յ���(ƽ)"),QVariant(HISDATA_DAYKWHSD1));
		boxDataType->addItem(tr("2ʱ���յ���(��)"),QVariant(HISDATA_DAYKWHSD2));
		boxDataType->addItem(tr("3ʱ���յ���(��)"),QVariant(HISDATA_DAYKWHSD3));
		boxDataType->addItem(tr("4ʱ���յ���(��)"),QVariant(HISDATA_DAYKWHSD4));
		boxDataType->addItem(tr("5ʱ���յ���(����)"),QVariant(HISDATA_DAYKWHSD5));
		boxDataType->addItem(tr("6ʱ���յ���"),QVariant(HISDATA_DAYKWHSD6));
		boxDataType->addItem(tr("���ۼƵ���"),QVariant(HISDATA_DAYKWHTTL));
		boxDataTypeIndexChanged(0);
	}
	return;
}

void DataSelectorToolBar::setDate( QString strDate )
{
	QDate date = QDate::fromString(strDate,"yyyyMMd");
	dateEdit_1->setDate(date);
}







//////////////////////////////////////////////////////////////////////////
///CurveSelectorToolBar ʵ��
CurveSelectorToolBar::CurveSelectorToolBar( QMainWindow *parent )
:QToolBar((QWidget *)parent),currentSelPara(static_cast<DataCurveApp*>(qApp)->selcurvepara)
{
	this->parent=(QWidget *)parent;
	init();
}

CurveSelectorToolBar::~CurveSelectorToolBar()
{
	
}



void CurveSelectorToolBar::init()
{
	setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	setWindowTitle(tr("���߻�ͼ����"));
	
	actionAttrAntialiased=new QAction(QIcon(":/res/Antialias.png"),tr("ƽ��"),parent);
	actionAttrAntialiased->setObjectName(QString::fromUtf8("actionAttrAntialiased"));
	actionAttrAntialiased->setToolTip(tr("��/�ر�ͼ�����߻��Ʒ����"));
	actionAttrAntialiased->setCheckable(true);
	actionAttrAntialiased->setChecked(currentSelPara.attrAntialiased);
	connect(actionAttrAntialiased, SIGNAL(toggled(bool)), this, SLOT(attrAntialiased(bool)));
	
	actionAttrFitted=new QAction(QIcon(":/res/fitted.png"),tr("���"),parent);
	actionAttrFitted->setObjectName(QString::fromUtf8("actionAttrFitted"));
	actionAttrFitted->setToolTip(tr("��/�ر�ͼ�����߻������ƽ��"));
	actionAttrFitted->setCheckable(true);
	actionAttrFitted->setChecked(currentSelPara.attrFitted);
	connect(actionAttrFitted, SIGNAL(toggled(bool)), this, SLOT(attrFitted(bool)));
	
	actionShowGrid=new QAction(QIcon(":/res/grid.png"),tr("����"),parent);
	actionShowGrid->setObjectName(QString::fromUtf8("actionShowGrid"));
	actionShowGrid->setToolTip(tr("��ʾ/��������ͼ���е�����"));
	actionShowGrid->setCheckable(true);
	actionShowGrid->setChecked(currentSelPara.showgrid);
	connect(actionShowGrid, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
	
	actionShowSymbol=new QAction(QIcon(":/res/symbol.png"),tr("���"),parent);
	actionShowSymbol->setObjectName(QString::fromUtf8("actionShowSymbol"));
	actionShowSymbol->setToolTip(tr("��ʾ/��������ͼ���е����ݵ���"));
	actionShowSymbol->setCheckable(true);
	actionShowSymbol->setChecked(currentSelPara.showsymbol);
	connect(actionShowSymbol, SIGNAL(toggled(bool)), this, SLOT(showSymbol(bool)));
	
	//////////////////////////////////////////////////////////////////////////
	actionNullValZero=new QAction(QIcon(":/res/curvenullstyle.png"),tr("��ֵ->0"),parent);
	actionNullValZero->setObjectName(QString::fromUtf8("actionNullValZero"));
	actionNullValZero->setToolTip(tr("ʹ��0������������������ݿ�Ϊ�յĵ�"));
	actionNullValZero->setCheckable(true);
	actionNullValZero->setChecked(currentSelPara.nullValueStyle==NullValZero);
	connect(actionNullValZero, SIGNAL(toggled(bool)), this, SLOT(nullValZero(bool)));
	
	actionNullValAvg=new QAction(QIcon(":/res/curvenullstyle.png"),tr("��ֵ->ƽ��"),parent);
	actionNullValAvg->setObjectName(QString::fromUtf8("actionNullValAvg"));
	actionNullValAvg->setToolTip(tr("ʹ�����ߵ��ƽ��ֵ������������������ݿ�Ϊ�յĵ�"));
	actionNullValAvg->setCheckable(true);
	actionNullValAvg->setChecked(currentSelPara.nullValueStyle==NullValAvg);
	connect(actionNullValAvg, SIGNAL(toggled(bool)), this, SLOT(nullValAvg(bool)));
	

	actionNullFix=new QAction(QIcon(":/res/curvenullstyle.png"),tr("��ֵ->�̶�"),parent);
	actionNullFix->setObjectName(QString::fromUtf8("actionNullFix"));
	actionNullFix->setToolTip(tr("ʹ�������п�ֵ֮ǰ��֮��Ĺ̶�ֵ������������������ݿ�Ϊ�յĵ�"));
	actionNullFix->setCheckable(true);
	actionNullFix->setChecked(currentSelPara.nullValueStyle==NullValFix);
	connect(actionNullFix, SIGNAL(toggled(bool)), this, SLOT(nullValFix(bool)));

	QMenu *popMenuNullVal = new QMenu(parent);
	popMenuNullVal->addAction(actionNullValZero);
	popMenuNullVal->addAction(actionNullValAvg);
	popMenuNullVal->addAction(actionNullFix);
	
	QToolButton *btnNullVal = new QToolButton(parent);
	btnNullVal->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	btnNullVal->setMenu(popMenuNullVal);
	btnNullVal->setPopupMode(QToolButton::MenuButtonPopup);
	btnNullVal->setDefaultAction(actionNullValZero);
	QString styleSheet = tr( "QToolButton\
							 {\
							 font-family: Microsoft YaHei;\
							 font-size: 13px;\
							 color: #3E464E;\
							 min-height:25px;\
							 border: 1px solid #C6CFD9;\
							 border-radius: 2px;\
							 }\
							 QToolButton::drop-down\
							 {\
							 subcontrol-origin: padding;\
							 subcontrol-position: top right;width: 28px;\
							 border-top-right-radius: 2px;\
							 border-bottom-right-radius: 2px;\
							 }\
							 QToolButton::down-arrow\
							 {\
							 image: url(:/res/drop_down.png);\
							 }") ;
	btnNullVal->setMinimumWidth( 110 ) ;
	btnNullVal->setStyleSheet( styleSheet ) ;

	connect(popMenuNullVal, SIGNAL(triggered(QAction *)), btnNullVal, SLOT(setDefaultAction(QAction *)));
	QActionGroup *actionNullValGroup=new QActionGroup(parent);
	actionNullValGroup->addAction(actionNullValZero);
	actionNullValGroup->addAction(actionNullValAvg);
	actionNullValGroup->addAction(actionNullFix);

	//////////////////////////////////////////////////////////////////////////
	actionYAxisAutoScale=new QAction(QIcon(":/res/axisautoscale.png"),tr("���"),parent);
	actionYAxisAutoScale->setObjectName(QString::fromUtf8("actionYAxisAutoScale"));
	actionYAxisAutoScale->setToolTip(tr("��/�ر�ͼ�������е�Y�����Զ�����"));
	actionYAxisAutoScale->setCheckable(true);
	actionYAxisAutoScale->setChecked(currentSelPara.yAxisAutoScale);
	connect(actionYAxisAutoScale, SIGNAL(toggled(bool)), this, SLOT(yAxisAuto(bool)));
	

	actionYMinSpinLbl=new QWidgetAction(parent);
	actionYMinSpinLbl->setObjectName(QString::fromUtf8("actionYMinSpinLbl"));
	
	yMinSpinLbl=new QLabel(tr("��Сֵ"),parent);
	yMinSpinLbl->setObjectName(QString::fromUtf8("yMinSpinLbl"));
	actionYMinSpinLbl->setDefaultWidget(yMinSpinLbl);
	actionYMinSpinLbl->setVisible(false);

	actionYMinSpin=new QWidgetAction(parent);
	actionYMinSpin->setObjectName(QString::fromUtf8("actionYMinSpin"));
	yMinSpin=new QSpinBox(parent);
	yMinSpin->setMinimum(0);
	yMinSpin->setObjectName(QString::fromUtf8("yMinSpin"));
	actionYMinSpin->setDefaultWidget(yMinSpin);
	actionYMinSpin->setVisible(!currentSelPara.yAxisAutoScale);
	
	actionYMaxSpinLbl=new QWidgetAction(parent);
	actionYMaxSpinLbl->setObjectName(QString::fromUtf8("actionYMaxSpinLbl"));
	yMaxSpinLbl=new QLabel(tr("���ֵ"),parent);
	yMaxSpinLbl->setObjectName(QString::fromUtf8("yMaxSpinLbl"));
	actionYMaxSpinLbl->setDefaultWidget(yMaxSpinLbl);
	actionYMaxSpinLbl->setVisible(!(currentSelPara.yAxisAutoScale));
	
	actionYMaxSpin=new QWidgetAction(parent);
	actionYMaxSpin->setObjectName(QString::fromUtf8("actionYMaxSpin"));
	yMaxSpin=new QSpinBox(parent);
	yMaxSpin->setMinimum(0);
	yMaxSpin->setObjectName(QString::fromUtf8("yMaxSpin"));
	actionYMaxSpin->setDefaultWidget(yMaxSpin);
	actionYMaxSpin->setVisible(!currentSelPara.yAxisAutoScale);

	connect(yMinSpin, SIGNAL(valueChanged(int)), this, SLOT(yAxisRangeChanged(int)));
	connect(yMaxSpin, SIGNAL(valueChanged(int)), this, SLOT(yAxisRangeChanged(int)));
	

	//////////////////////////////////////////////////////////////////////////
	actionStyleLine=new QAction(QIcon(":/res/lPlot.png"),tr("ֱ��"),parent);
	actionStyleLine->setObjectName(QString::fromUtf8("actionStyleLine"));
	actionStyleLine->setToolTip(tr("���û�ͼ��������Ϊֱ��"));
 	actionStyleLine->setCheckable(true);
 	actionStyleLine->setChecked(currentSelPara.curvestyle==QwtPlotCurve::Lines);
	connect(actionStyleLine, SIGNAL(toggled(bool)), this, SLOT(setLineStyle(bool)));
	
	actionStyleDots=new QAction(QIcon(":/res/pPlot.png"),tr("��"),parent);
	actionStyleDots->setObjectName(QString::fromUtf8("actionStyleDots"));
	actionStyleDots->setToolTip(tr("���û�ͼ��������Ϊ��"));
 	actionStyleDots->setCheckable(true);
 	actionStyleDots->setChecked(currentSelPara.curvestyle==QwtPlotCurve::Dots);
	connect(actionStyleDots, SIGNAL(toggled(bool)), this, SLOT(setDotsStyle(bool)));
	
	actionStyleSetp=new QAction(QIcon(":/res/stepsplot.png"),tr("������"),parent);
	actionStyleSetp->setObjectName(QString::fromUtf8("actionStyleSetp"));
	actionStyleSetp->setToolTip(tr("���û�ͼ��������Ϊˮƽ������"));
 	actionStyleSetp->setCheckable(true);
 	actionStyleSetp->setChecked(currentSelPara.curvestyle==QwtPlotCurve::Steps);
	connect(actionStyleSetp, SIGNAL(toggled(bool)), this, SLOT(setSetpStyle(bool)));
	
	actionStyleSticks=new QAction(QIcon(":/res/sticksplot.png"),tr("��ֱ��"),parent);
	actionStyleSticks->setObjectName(QString::fromUtf8("actionStyleSticks"));
	actionStyleSticks->setToolTip(tr("���û�ͼ��������Ϊ��ֱ��"));
 	actionStyleSticks->setCheckable(true);
 	actionStyleSticks->setChecked(currentSelPara.curvestyle==QwtPlotCurve::Sticks);
	connect(actionStyleSticks, SIGNAL(toggled(bool)), this, SLOT(SetSticksStyle(bool)));
	
	QMenu *popMenu = new QMenu(parent);
	popMenu->addAction(actionStyleLine);
	popMenu->addAction(actionStyleDots);
	popMenu->addAction(actionStyleSetp);
	popMenu->addAction(actionStyleSticks);

	QToolButton *btnStyle = new QToolButton(parent);
	btnStyle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	btnStyle->setMenu(popMenu);
	btnStyle->setPopupMode(QToolButton::MenuButtonPopup);
	btnStyle->setDefaultAction(actionStyleLine);
	btnStyle->setMinimumWidth( 80 ) ;
	btnStyle->setStyleSheet( styleSheet ) ;
	connect(popMenu, SIGNAL(triggered(QAction *)), btnStyle, SLOT(setDefaultAction(QAction *)));
 	QActionGroup *actionStyleGroup=new QActionGroup(parent);
 	actionStyleGroup->addAction(actionStyleLine);
 	actionStyleGroup->addAction(actionStyleDots);
 	actionStyleGroup->addAction(actionStyleSetp);
 	actionStyleGroup->addAction(actionStyleSticks);

	addWidget(btnStyle);
// 	addSeparator();
	addWidget((btnNullVal));
// 	addSeparator();
	addAction(actionShowGrid);
	addAction(actionShowSymbol);
// 	addSeparator();
	addAction(actionAttrAntialiased);
	//lbh 20130318 function Invalid comment this line
	//addAction(actionAttrFitted);               
// 	addSeparator();
	addAction(actionYAxisAutoScale);
// 	actionYAxisSparator=addSeparator();
//	actionYAxisSparator->setVisible(!currentSelPara.yAxisAutoScale);
	addAction(actionYMinSpinLbl);
	addAction(actionYMinSpin);
	addAction(actionYMaxSpinLbl);
	addAction(actionYMaxSpin);
}

//////////////////////////////////////////////////////////////////////////
////plot implement

void CurveSelectorToolBar::attrAntialiased( bool checked )
{
	if(currentSelPara.attrAntialiased!=checked)
	{
		currentSelPara.attrAntialiased=checked;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::attrFitted( bool checked )
{
	if(currentSelPara.attrFitted!=checked)
	{
		currentSelPara.attrFitted=checked;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::showGrid( bool checked )
{
	if(currentSelPara.showgrid!=checked)
	{
		currentSelPara.showgrid=checked;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::showSymbol( bool checked )
{
	if(currentSelPara.showsymbol!=checked)
	{
		currentSelPara.showsymbol=checked;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::setLineStyle( bool checked )
{
	if(checked && currentSelPara.curvestyle!=(int)QwtPlotCurve::Lines)
	{
		currentSelPara.curvestyle=(int)QwtPlotCurve::Lines;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::setDotsStyle( bool checked )
{
	if(checked && currentSelPara.curvestyle!=(int)QwtPlotCurve::Dots)
	{
		currentSelPara.curvestyle=(int)QwtPlotCurve::Dots;
		//same time set symbol is enabled
		//LiCG 20130318 disable show symbol uncomment two line
		//currentSelPara.showsymbol=true;
		//actionShowSymbol->setChecked(true);

		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::setSetpStyle( bool checked )
{
	if(checked && currentSelPara.curvestyle!=(int)QwtPlotCurve::Steps)
	{
		currentSelPara.curvestyle=(int)QwtPlotCurve::Steps;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::SetSticksStyle( bool checked )
{
	if(checked && currentSelPara.curvestyle!=(int)QwtPlotCurve::Sticks)
	{
		currentSelPara.curvestyle=(int)QwtPlotCurve::Sticks;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::nullValZero( bool checked )
{
	if(checked && currentSelPara.nullValueStyle!=NullValZero)
	{
		currentSelPara.nullValueStyle=NullValZero;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::nullValAvg( bool checked )
{
	if(checked && currentSelPara.nullValueStyle!=NullValAvg)
	{
		currentSelPara.nullValueStyle=NullValAvg;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::nullValFix( bool checked )
{
	if(checked && currentSelPara.nullValueStyle!=NullValFix)
	{
		currentSelPara.nullValueStyle=NullValFix;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::yAxisAuto( bool checked )
{
	if(currentSelPara.yAxisAutoScale!=checked)
	{
		currentSelPara.yAxisAutoScale=checked;
// 		actionYAxisSparator->setVisible(!checked);
		actionYMinSpinLbl->setVisible(!checked);
		actionYMinSpin->setVisible(!checked);
		actionYMaxSpinLbl->setVisible(!checked);
		actionYMaxSpin->setVisible(!checked);
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void CurveSelectorToolBar::yAxisRangeChanged( int val )
{
	currentSelPara.yAxisScaleMin=yMinSpin->value();
	currentSelPara.yAxisScaleMax=yMaxSpin->value();
	Q_EMIT(selectorChanged(currentSelPara));
}

//////////////////////////////////////////////////////////////////////////
////public implemention
void CurveSelectorToolBar::setYAsisBoxRange( double& min,double& max ) const
{
	int val;
	val=qIntCast(min-min*0.05);
	yMinSpin->setMinimum(val>0 ? 0 : val);
	yMinSpin->setMaximum(qIntCast(max));
	yMinSpin->setValue(val);
	
	val=qIntCast(max+max*0.05);
	yMaxSpin->setMinimum(qIntCast(min));
	yMaxSpin->setMaximum(val);
	yMaxSpin->setValue(val);
}
