#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QDockWidget>
#include <QToolButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QTreeWidget>
#include <QHeaderView>
#include <QtAlgorithms>
#include <QActionGroup>
#include <QVariant>
#include <QStringList>
#include <QMessageBox>
#include <QTreeWidgetItemIterator>

#include <QColor>

#include <QDebug>

#include "selectorwidget.h"
#include "datacurvedefine.h"
#include "datacurveapp.h"
#include "uitools/dmessagebox.h"
#define _Q2C(str) ((const char *)(str.toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

SelectorWidget::SelectorWidget( QWidget* parent /*= 0*/)
:filterNullNode(false),currentSelPara(static_cast<DataCurveApp*>(qApp)->selpara)
{
	/*findpromptInfo=tr("[�����ı����س�����,�ٴλس��������ҡ�]");*/
	init();
}


SelectorWidget::~SelectorWidget()
{

}

void SelectorWidget::createActions()
{
	actionRefresh = new QAction(QIcon(":/res/refresh.png"),tr("ˢ��"),this);
    actionRefresh->setObjectName(QString::fromUtf8("actionRefresh"));
	connect(actionRefresh, SIGNAL(activated()), this, SLOT(refresh()));
	
	
	actionStationMeasTree = new QAction(QIcon(":/res/StationMeasType.png"),tr("ң������"),this);
    actionStationMeasTree->setObjectName(QString::fromUtf8("actionStationMeasTree"));
	connect(actionStationMeasTree, SIGNAL(activated()), this, SLOT(stationYcType()));

	actionEnergyMeasTree = new QAction(QIcon(":/res/EnergyMeasType.png"),tr("��������"),this);
    actionEnergyMeasTree->setObjectName(QString::fromUtf8("actionEnergyMeasTree"));
	connect(actionEnergyMeasTree, SIGNAL(activated()), this, SLOT(stationKwhType()));


	actionFind = new QAction(QIcon(":/res/find.png"),tr("����"),this);
    actionFind->setObjectName(QString::fromUtf8("actionFind"));
	actionFind->setToolTip(tr("����"));
	actionFind->setCheckable(true);
	actionFind->setChecked(true);
	connect(actionFind, SIGNAL(toggled(bool)), this, SLOT(showFindWidget(bool)));

	actionShowNodeInfo = new QAction(QIcon(":/res/shownodeinfo.png"),tr("�ڵ���Ϣ"),this);
    actionShowNodeInfo->setObjectName(QString::fromUtf8("actionShowNodeInfo"));
	actionShowNodeInfo->setToolTip(tr("��ʾ/���ؽڵ���ϢС���ڲ���"));
	actionShowNodeInfo->setCheckable(true);
	actionShowNodeInfo->setChecked(true);
	connect(actionShowNodeInfo, SIGNAL(toggled(bool)), this, SLOT(showNodeInfoWidget(bool)));


	actionSingleSel = new QAction(QIcon(":/res/singleselector.png"),tr("��ѡ����"),this);
    actionSingleSel->setObjectName(QString::fromUtf8("actionSingleSel"));
	actionSingleSel->setCheckable(true);
	connect(actionSingleSel, SIGNAL(toggled(bool)), this, SLOT(singleSelector(bool)));

	actionMutilSel = new QAction(QIcon(":/res/multiselector.png"),tr("��ѡ����"),this);
    actionMutilSel->setObjectName(QString::fromUtf8("actionMutilSelactionRefresh"));
	actionMutilSel->setCheckable(true);
	connect(actionMutilSel, SIGNAL(toggled(bool)), this, SLOT(mutilSelector(bool)));

	QActionGroup *selActionGroup=new QActionGroup(this);
	selActionGroup->addAction(actionSingleSel);
	selActionGroup->addAction(actionMutilSel);
}

void SelectorWidget::initToolBars()
{
	selectorTools = new QToolBar(this);
	selectorTools->setIconSize( QSize(20,20) );
	selectorTools->setObjectName(QString::fromUtf8("selector"));
	selectorTools->setFloatable(false);
	selectorTools->setMovable(false);

	QMenu *popMenu = new QMenu(this);
	popMenu->addAction(actionStationMeasTree);
	popMenu->addAction(actionEnergyMeasTree);
	
	QToolButton *btnDevTreeSel = new QToolButton(this);
	btnDevTreeSel->setMenu(popMenu);
	btnDevTreeSel->setPopupMode(QToolButton::MenuButtonPopup);
	btnDevTreeSel->setDefaultAction(actionStationMeasTree);
	connect(popMenu, SIGNAL(triggered(QAction *)), btnDevTreeSel, SLOT(setDefaultAction(QAction *)));
    selectorTools->addWidget(btnDevTreeSel);
	
	selectorTools->addAction(actionRefresh);
    selectorTools->addAction(actionFind);
	selectorTools->addAction(actionShowNodeInfo);
	selectorTools->addSeparator();
    selectorTools->addAction(actionSingleSel);
	selectorTools->addAction(actionMutilSel);
	selectorTools->addSeparator();

		
}

void SelectorWidget::refresh()
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	if(DMessageBox::question(this,tr("��ʾ"),tr("ˢ�����Ľڵ���Ϣ��������?"),
		QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes)
	{
		myApp->initCurveTypeDefs();
		myApp->initStationDb();
		myApp->initStationAreaDb();
		myApp->initStationViewDb();
		myApp->initStationYcDb();
		myApp->initStationKwnDb();
		currentSelPara.selType=SingleSelector;
		currentSelPara.dataSrc=YcDataSrc;
		currentSelPara.paras.clear();
		actionSingleSel->setChecked(true);  //emit yc 
		initStationYcTree(false);
		refind = true ;
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void SelectorWidget::init()
{
	createActions();
	initToolBars();
	m_showCheckBox = false ;
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setSpacing(3);
	mainLayout->setMargin(0);

	findEdit=new QLineEdit(this);
	findEdit->setObjectName(QString::fromUtf8("findEdit"));
	findEdit->setText(tr("�����ı����س�����,�ٴλس�������һ��..."));
	connect(findEdit,SIGNAL(returnPressed()),this,SLOT(findEnterPressed()));
	connect(findEdit,SIGNAL(textChanged(QString)),this,SLOT(findTextChanged(QString)));

	devTree=new QTreeWidget(this);
	devTree->setObjectName(QString::fromUtf8("devTree"));

	//devTree->setFrameStyle(QFrame::NoFrame);
	devTree->setIndentation(devTree->indentation() * 9/10);
    devTree->setUniformRowHeights(true);
    devTree->setTextElideMode(Qt::ElideNone);
    devTree->setAttribute(Qt::WA_MacShowFocusRect, false);
	devTree->setHeaderHidden(true);
	// show horizontal scrollbar
	devTree->header()->setResizeMode(QHeaderView::ResizeToContents);
    //devTree->header()->setStretchLastSection(false);
	connect(devTree,SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),this,SLOT(updateNodeInfo(QTreeWidgetItem*)));
	connect(devTree,SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),this,SLOT(updateCurve(QTreeWidgetItem*,QTreeWidgetItem*)));
	connect(devTree,SIGNAL(itemChanged(QTreeWidgetItem *, int)),this, SLOT(updateCurve(QTreeWidgetItem *, int)));
	connect(devTree,SIGNAL(itemExpanded(QTreeWidgetItem *)),this, SLOT(showRtuDetailObj(QTreeWidgetItem *)));

	nodeInfoEdit=new QTextEdit(this);
	nodeInfoEdit->setObjectName(QString::fromUtf8("nodeInfoEdit"));
	nodeInfoEdit->setReadOnly(true);
	nodeInfoEdit->setTextColor(Qt::blue);
	nodeInfoEdit->setTextBackgroundColor(Qt::gray);
//	nodeInfoEdit->setMaximumHeight(200);
	
	mainLayout->setMenuBar(selectorTools);
	mainLayout->addWidget(findEdit);
	
	//mainLayout->addWidget(devTree);
	

	QSplitter *vSplitter=new QSplitter(Qt::Vertical,this);
	vSplitter->addWidget(devTree);
	vSplitter->addWidget(nodeInfoEdit);
	vSplitter->setStretchFactor( 0 , 5 ) ;
	vSplitter->setStretchFactor( 1 , 2 ) ;
	
	mainLayout->addWidget(vSplitter);
	setLayout(mainLayout);
	
	currentSelPara.selType=SingleSelector;
	currentSelPara.dataSrc=YcDataSrc;
	currentSelPara.paras.clear();
	actionSingleSel->setChecked(true);  //emit yc 
}


//////////////////////////////////////////////////////////////////////////
////

void SelectorWidget::showFindWidget( bool checked )
{
	if(checked)
	{
		findEdit->setVisible(true);
	}
	else
	{
		findEdit->setVisible(false);
	}
	layout()->update();
}

void SelectorWidget::showNodeInfoWidget( bool checked )
{
	if(checked)
	{
		nodeInfoEdit->setVisible(true);
	}
	else
	{
		nodeInfoEdit->setVisible(false);
	}
	layout()->update();
}


void SelectorWidget::initStationYcTree(bool showCheckBox)
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);

	QTreeWidgetItem *itemRtu ;
	Terminals::const_iterator itRtu;
	QVariant  Var;
	
	devTree->clear();
	m_mapYcCode_treeItem.clear();


	QTreeWidgetItem *pItem,*pSubItem;
	QMap<int ,QTreeWidgetItem*> m_groupItemMap ;
	QStringList desc_group;
	desc_group.clear() ;
	desc_group << _C2Q("δ���鳧վ")  ;
	QTreeWidgetItem *firstStaItem = new QTreeWidgetItem( devTree, desc_group ) ;
	if (myApp->currentUserRight.type!=0)
	{
	   firstStaItem->setDisabled(true);
	}
	firstStaItem->setData(0,Qt::UserRole,-1);
	m_groupItemMap[0 ] = firstStaItem ;
	StationAreaParas::const_iterator itstaarea;
	for(itstaarea=myApp->stationAreaParas.begin();itstaarea!=myApp->stationAreaParas.end();itstaarea++)
	{
      if ((*itstaarea)->masterid ==NULL)
      {
		  desc_group.clear() ;
		  desc_group.append( (*itstaarea)->name) ;
		  pItem = new QTreeWidgetItem( devTree, desc_group ) ;
		  pItem->setData(0,Qt::UserRole,-1);
		  // 			pItem->setCheckState(0, Qt::Unchecked); // �趨�ɺ�ȡ�ķ��� 
		  m_groupItemMap[(*itstaarea)->id] = pItem ;
		  if (myApp->currentUserRight.type!=0)
		  {
		  if(!myApp->checkLoginUserZone((*itstaarea)->id))
		  {
		  	pItem->setDisabled(true);
		  }
		  }
      }
	}
	StationAreaParas::const_iterator itstaarea2;
	for(itstaarea2=myApp->stationAreaParas.begin();itstaarea2!=myApp->stationAreaParas.end();itstaarea2++)
	{
		if ((*itstaarea2)->masterid !=NULL)
		{
			desc_group.clear() ;
			desc_group.append( (*itstaarea2)->name ) ;
			pItem = new QTreeWidgetItem( m_groupItemMap[(*itstaarea2)->masterid ] , desc_group ) ;
			pItem->setData(0,Qt::UserRole,-1);
			// 			pItem->setCheckState(0, Qt::Unchecked); // �趨�ɺ�ȡ�ķ��� 
			m_groupItemMap[(*itstaarea2)->id ] = pItem ;
		}
	}


	for(itRtu=myApp->terminals.begin();itRtu!=myApp->terminals.end();itRtu++)
	{
		//if(!myApp->checkLoginUserZone((*itRtu)->zone))
		//	continue;
		int areano = myApp->notoareaid[(*itRtu)->no];
		itemRtu = new QTreeWidgetItem(m_groupItemMap[areano],QStringList((*itRtu)->describe));
		itemRtu->setIcon(0, QIcon(":/res/terminal.png"));
		itemRtu->setData(0,Qt::UserRole,QVariant(TerminalNode));
		qVariantSetValue<terminalpara*>(Var,*itRtu);
		itemRtu->setData(0,Qt::UserRole+1,Var);
		//devTree->addTopLevelItem(itemRtu);
		m_rtuItem2RtuNoMap[ itemRtu ] = (*itRtu)->no;
		itemRtu->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );
// 		for(itYC=myApp->ycparas.begin();itYC!=myApp->ycparas.end();itYC++)
// 		{
// 			if((*itYC)->terminalno==(*itRtu)->no)
// 			{
// 				if(!myApp->checkLoginUserZone((*itYC)->zone))
// 					continue;
// 
// 				itemYc=new QTreeWidgetItem(QStringList((*itYC)->describe));
// 				if((*itYC)->plantime>0)
// 					itemYc->setIcon(0, QIcon(":/res/buttonok1.png"));
// 				else
// 					itemYc->setIcon(0, QIcon(":/res/buttonok.png"));
// 				itemYc->setData(0,Qt::UserRole,QVariant(YCParaNode));
// 				qVariantSetValue<ycpara*>(Var,*itYC);
// 				itemYc->setData(0,Qt::UserRole+1,Var);
// 				if(showCheckBox)
// 					itemYc->setCheckState(0,Qt::Unchecked);
// 				itemRtu->addChild(itemYc);
// 
// 				m_mapYcCode_treeItem.insert((*itYC)->name,itemYc);
// 			}
// 		}
// 
// 		if(filterNullNode && itemRtu->childCount()==0)
// 		{
// 			devTree->removeItemWidget(itemRtu,0);
// 			delete itemRtu;
// 			itemRtu=NULL;
// 		}
	}
	devTree->scrollToTop();
}


void SelectorWidget::initStationKwhTree( bool showCheckBox )
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);

	QTreeWidgetItem			   *itemRtu,*itemKwh;
	Terminals::const_iterator  itRtu;
	KwhParas::const_iterator   itKwh;
	QVariant				   Var;
	
	devTree->clear();

	QTreeWidgetItem *pItem,*pSubItem;
	QMap<int ,QTreeWidgetItem*> m_groupItemMap ;
	QStringList desc_group;
	desc_group.clear() ;
	desc_group << _C2Q("δ���鳧վ")  ;
	QTreeWidgetItem *firstStaItem = new QTreeWidgetItem( devTree, desc_group ) ;
	firstStaItem->setData(0,Qt::UserRole,-1);
	m_groupItemMap[0 ] = firstStaItem ;
	if (myApp->currentUserRight.type!=0)
	{
	   firstStaItem->setDisabled(true);
	}
	StationAreaParas::const_iterator itstaarea;
	for(itstaarea=myApp->stationAreaParas.begin();itstaarea!=myApp->stationAreaParas.end();itstaarea++)
	{
		if ((*itstaarea)->masterid ==NULL)
		{
			desc_group.clear() ;
			desc_group.append( (*itstaarea)->name) ;
			pItem = new QTreeWidgetItem( devTree, desc_group ) ;
			pItem->setData(0,Qt::UserRole,-1);
			// 			pItem->setCheckState(0, Qt::Unchecked); // �趨�ɺ�ȡ�ķ��� 
			m_groupItemMap[(*itstaarea)->id] = pItem ;
			if (myApp->currentUserRight.type!=0)
			{
			if(!myApp->checkLoginUserZone((*itstaarea)->id))
			{
				pItem->setDisabled(true);
			}
			}
		}
	}
	StationAreaParas::const_iterator itstaarea2;
	for(itstaarea2=myApp->stationAreaParas.begin();itstaarea2!=myApp->stationAreaParas.end();itstaarea2++)
	{
		if ((*itstaarea2)->masterid !=NULL)
		{
			desc_group.clear() ;
			desc_group.append( (*itstaarea2)->name ) ;
			pItem = new QTreeWidgetItem( m_groupItemMap[(*itstaarea2)->masterid ] , desc_group ) ;
			pItem->setData(0,Qt::UserRole,-1);
			// 			pItem->setCheckState(0, Qt::Unchecked); // �趨�ɺ�ȡ�ķ��� 
			m_groupItemMap[(*itstaarea2)->id ] = pItem ;
		}
	}

	for(itRtu=myApp->terminals.begin();itRtu!=myApp->terminals.end();itRtu++)
	{
		//if(!myApp->checkLoginUserZone((*itRtu)->zone))
		//	continue;
		int areano = myApp->notoareaid[(*itRtu)->no];
		itemRtu = new QTreeWidgetItem(m_groupItemMap[areano],QStringList((*itRtu)->describe));
		itemRtu->setIcon(0, QIcon(":/res/terminal.png"));
		itemRtu->setData(0,Qt::UserRole,QVariant(TerminalNode));
		qVariantSetValue<terminalpara*>(Var,*itRtu);
		itemRtu->setData(0,Qt::UserRole+1,Var);
		//devTree->addTopLevelItem(itemRtu);
		
		for(itKwh=myApp->kwhparas.begin();itKwh!=myApp->kwhparas.end();itKwh++)
		{
			if((*itKwh)->terminalno==(*itRtu)->no)
			{
				//if(!myApp->checkLoginUserZone((*itKwh)->zone))
				//	continue;

				itemKwh=new QTreeWidgetItem(QStringList((*itKwh)->describe));
				itemKwh->setIcon(0, QIcon(":/res/buttonok.png"));
				itemKwh->setData(0,Qt::UserRole,QVariant(KWHParaNode));
				qVariantSetValue<kwhpara*>(Var,*itKwh);
				itemKwh->setData(0,Qt::UserRole+1,Var);
				if(showCheckBox)
					itemKwh->setCheckState(0,Qt::Unchecked);
				itemRtu->addChild(itemKwh);
			}
		}

		if(filterNullNode && itemRtu->childCount()==0)
		{
			devTree->removeItemWidget(itemRtu,0);
			delete itemRtu;
			itemRtu=NULL;
		}
	}
	devTree->scrollToTop();
}


void SelectorWidget::singleSelector( bool checked )
{
	if(checked)
	{
		currentSelPara.selType=SingleSelector;
		currentSelPara.paras.clear();
		if(currentSelPara.dataSrc==YcDataSrc)
		{
			m_showCheckBox = false ;
			initStationYcTree(false);
		}
		else if(currentSelPara.dataSrc==DdDataSrc)
		{
			m_showCheckBox = false ;
			initStationKwhTree(false);
		}
		refind = true ;

		Q_EMIT(selectorChanged(currentSelPara));
	}
	
}

void SelectorWidget::mutilSelector( bool checked )
{
	if(checked)
	{
		currentSelPara.selType=MutilSelector;
		currentSelPara.paras.clear();
		currentSelPara.clearCompDate();
		currentSelPara.clearCompDate();  //clear comp status and content
		if(currentSelPara.dataSrc==YcDataSrc)
		{
			m_showCheckBox = true ;
			initStationYcTree(true);
		}
		else if(currentSelPara.dataSrc==DdDataSrc)
		{
			m_showCheckBox = true ;
			initStationKwhTree(true);
		}
		refind = true ;

		Q_EMIT(selectorChanged(currentSelPara));
	}
}

void SelectorWidget::stationYcType()
{
	if(currentSelPara.dataSrc!=YcDataSrc)
	{
		currentSelPara.paras.clear();
		currentSelPara.dataSrc=YcDataSrc;
		m_showCheckBox = currentSelPara.selType==SingleSelector ? false : true ;
		initStationYcTree(currentSelPara.selType==SingleSelector ? false : true);
		Q_EMIT(dataSrcChanged());
	}
}

void SelectorWidget::stationKwhType( )
{
	if(currentSelPara.dataSrc!=DdDataSrc)
	{
		currentSelPara.paras.clear();
		currentSelPara.dataSrc=DdDataSrc;
		m_showCheckBox = currentSelPara.selType==SingleSelector ? false : true ;
		initStationKwhTree(currentSelPara.selType==SingleSelector ? false : true);
		Q_EMIT(dataSrcChanged());
	}
}

//ѡ��ı�
void SelectorWidget::updateCurve( QTreeWidgetItem * current, QTreeWidgetItem * previous )
{
	if (current==NULL || currentSelPara.selType==MutilSelector )
        return;
	if(current->data(0,Qt::UserRole).isNull()) return;
	if(current->data(0,Qt::UserRole+1).isNull()) return;
	
// 	if(previous!=NULL)
// 		previous->setIcon(0, QIcon(":/res/buttonok.png"));
// 	current->setIcon(0, QIcon(":/res/buttoncancel.png"));
// 	current->setSelected(true);

	if(current->data(0,Qt::UserRole).toUInt()==YCParaNode)
	{
		ycpara *ycitem=qVariantValue<ycpara*>(current->data(0,Qt::UserRole+1));
		Q_ASSERT(ycitem!=NULL);
		currentSelPara.dataSrc=YcDataSrc;
		currentSelPara.selType=SingleSelector;
		currentSelPara.paras.clear();
		currentSelPara.paras.append(ycitem->name);
		currentSelPara.setPlantime(ycitem->plantime);
		Q_EMIT(selectorChanged(currentSelPara));
	}
	else if(current->data(0,Qt::UserRole).toUInt()==KWHParaNode)
	{
		kwhpara *kwnitem=qVariantValue<kwhpara*>(current->data(0,Qt::UserRole+1));
		Q_ASSERT(kwnitem!=NULL);
		currentSelPara.dataSrc=DdDataSrc;
		currentSelPara.selType=SingleSelector;
		currentSelPara.paras.clear();
		currentSelPara.paras.append(kwnitem->name);
		Q_EMIT(selectorChanged(currentSelPara));
	}
}

//checked״̬�ı�
void SelectorWidget::updateCurve( QTreeWidgetItem *item, int column )
{
//	qDebug()<<"SelectorWidget::itemChanged";
	if (!item || column != 0)
	{
//		qDebug()<<"item:"<<item<<"  "<< "column:"<<column << "  return solt";
        return;
	}
	Qt::CheckState currentState = item->checkState(0);
	if(item->data(0,Qt::UserRole).isNull()) 
	{
		return;
	}
	if(item->data(0,Qt::UserRole+1).isNull()) 
	{
		return;
	}
	disconnect(devTree,SIGNAL(itemChanged(QTreeWidgetItem *, int)),this, SLOT(updateCurve(QTreeWidgetItem *, int)));
	
	if(item->data(0,Qt::UserRole).toUInt()==YCParaNode)
	{
		ycpara *ycitem=qVariantValue<ycpara*>(item->data(0,Qt::UserRole+1));
		Q_ASSERT(ycitem!=NULL);
		currentSelPara.dataSrc=YcDataSrc;
		currentSelPara.selType=MutilSelector;
//		qDebug()<<"find item"<<ycitem->name;
//		qDebug()<<"currentSelPara.paras:"<<currentSelPara.paras;
		//û�ж���,hpux������size�жϣ������ڼ��Ͽ�ʱ�����ؼ١�
		if(qFind(currentSelPara.paras.begin(),currentSelPara.paras.end(),ycitem->name)==currentSelPara.paras.end())
		{
			if(currentState==Qt::Checked && currentSelPara.paras.size() < ((DataCurveApp*)qApp)->maxCurveNumber)
			{
				currentSelPara.paras.append(ycitem->name);
//				qDebug()<<"Q_EMIT(selectorChanged(currentSelPara))";
				Q_EMIT(selectorChanged(currentSelPara));
			}
			else if(currentState==Qt::Checked && currentSelPara.paras.size() >= ((DataCurveApp*)qApp)->maxCurveNumber)
			{
				DMessageBox::critical(this,tr("����"),
					tr("��ǰѡ���ѡ������Ŀ����ϵͳ������������%1��ǰ��ѡ����%2�����ߣ�ϵͳ����������߸���%3")
					.arg("\r\n").arg(currentSelPara.paras.size()).arg(((DataCurveApp*)qApp)->maxCurveNumber),
					QMessageBox::Ok,QMessageBox::NoButton);
				item->setCheckState(0,Qt::Unchecked);
			}
		}
		else	//��������Ӧ�Ķ���
		{
//			qDebug()<<tr("��������Ӧ�Ķ���");
			if(currentState==Qt::Unchecked)
			{
				bool ret=currentSelPara.paras.removeOne(ycitem->name);
//				qDebug()<<tr("remove:")<<ycitem->name << "result:"<<ret;
				Q_EMIT(selectorChanged(currentSelPara));
			}
		}
	}
	else if(item->data(0,Qt::UserRole).toUInt()==KWHParaNode)
	{
		kwhpara *kwhitem=qVariantValue<kwhpara*>(item->data(0,Qt::UserRole+1));
		Q_ASSERT(kwhitem!=NULL);
		currentSelPara.dataSrc=DdDataSrc;
		currentSelPara.selType=MutilSelector;
		//QString findItem = *qFind(currentSelPara.paras,kwnitem->name);
		if(qFind(currentSelPara.paras.begin(),currentSelPara.paras.end(),kwhitem->name)==currentSelPara.paras.end()) //û�ж���
		{
			if(currentState==Qt::Checked && currentSelPara.paras.size() < ((DataCurveApp*)qApp)->maxCurveNumber)
			{
				currentSelPara.paras.append(kwhitem->name);
				//				qDebug()<<"Q_EMIT(selectorChanged(currentSelPara))";
				Q_EMIT(selectorChanged(currentSelPara));
			}
			else if(currentState==Qt::Checked && currentSelPara.paras.size() >= ((DataCurveApp*)qApp)->maxCurveNumber)
			{
				DMessageBox::critical(this,tr("����"),
					tr("��ǰѡ���ѡ������Ŀ����ϵͳ������������%1��ǰ��ѡ����%2�����ߣ�ϵͳ����������߸���%3")
					.arg("\r\n").arg(currentSelPara.paras.size()).arg(((DataCurveApp*)qApp)->maxCurveNumber),
					QMessageBox::Ok,QMessageBox::NoButton);
				item->setCheckState(0,Qt::Unchecked);
			}
		}
		else	//��������Ӧ�Ķ���
		{
			if(currentState==Qt::Unchecked)
			{
				currentSelPara.paras.removeOne(kwhitem->name);
				Q_EMIT(selectorChanged(currentSelPara));
			}
		}
	}
	connect(devTree,SIGNAL(itemChanged(QTreeWidgetItem *, int)),this, SLOT(updateCurve(QTreeWidgetItem *, int)));
	return;
}


void SelectorWidget::updateNodeInfo( QTreeWidgetItem* item )
{
	ycpara  *ycitem;
	kwhpara *kwhitem;
	terminalpara *terminal;
	QVariant Var;
	if(item==NULL) return;
	if(item->data(0,Qt::UserRole).isNull()) return;
	if(item->data(0,Qt::UserRole).toUInt()==TerminalNode)
	{
		Var=item->data(0,Qt::UserRole+1);
		terminal=qVariantValue<terminalpara*>(Var);
		Q_ASSERT(terminal!=NULL);
		nodeInfoEdit->setText(
		tr("<body style=\"color: #00\">\
			�ն����:<b>%1</b><br/>\
			����:<b>%2</b><br/>\
			����:<b>%3</b><br/>\
			�����:<b>%4</b><br/>\
			��վ����:<b>%5</b><br/>\
			��ʼ״̬:<b>%6</b><br/>\
			�Ƿ�ͳ��:<b>%7</b><br/>\
			ʹ�ñ�־:<b>%8</b><br/>\
			</style>")
			.arg(terminal->no).arg(terminal->name).arg(terminal->describe).arg(terminal->zone).arg(terminal->sname)
			.arg(terminal->beginstatus==0 ? tr("ֹͣ"):tr("����"))
			.arg(terminal->isstat==0 ? tr("��ͳ��") : tr("ͳ��"))
			.arg(terminal->isused==0 ? tr("��ʹ��") : tr("ʹ��")));

	}
	else if(item->data(0,Qt::UserRole).toUInt()==YCParaNode)
	{
		Var=item->data(0,Qt::UserRole+1);
		ycitem=qVariantValue<ycpara*>(Var);
		Q_ASSERT(ycitem!=NULL);
		nodeInfoEdit->setHtml(
		tr("<body style=\"color: #00\">\
			����:<b>%1</b><br/>\
			����:<b>%2</b><br/>\
			�����:<b>%3</b><br/>\
			�豸����:<b>%4</b><br/>\
			����:<b>%5</b><br/>\
			�ն����:<b>%6</b><br/>\
			ң�����:<b>%7</b><br/>\
			��������:<b>%8</b><br/>\
			��������:<b>%9</b><br/>\
			�������� :<b>%10</b><br/>\
			��������:<b>%11</b><br/>\
			��������־:<b>%12</b><br/>\
			��������:<b>%13</b>����<br/>\
			�ƻ����:<b>%14</b>����<br/>\
			��ʾ��־:<b>%15</b><br/>\
			 </style> ")
			.arg(ycitem->name)
			.arg(ycitem->describe)
			.arg(ycitem->zone)
			.arg(ycitem->devname)
			.arg(ycitem->typeDescribe)
			.arg(ycitem->terminalno)
			.arg(ycitem->ycno)
			.arg(ycitem->warnupperlimit)
			.arg(ycitem->warnlowerlimit)
			.arg(ycitem->alertupperlimit)
			.arg(ycitem->alertlowerlimit)
			.arg(ycitem->iscomputer==0 ? tr("��"):tr("��"))
			.arg(ycitem->savetime)
			.arg(ycitem->plantime)
			.arg(ycitem->isshow==0 ? tr("��") : tr("��")));

	}
	else if(item->data(0,Qt::UserRole).toUInt()==KWHParaNode)
	{
		Var=item->data(0,Qt::UserRole+1);
		kwhitem=qVariantValue<kwhpara*>(Var);
		Q_ASSERT(kwhitem!=NULL);
		nodeInfoEdit->setHtml(
		tr("<body style=\"color: #00\">\
		����:<b>%1</b><br/>\
		����:<b>%2</b><br/>\
		�����:<b>%3</b><br/>\
		�豸����:<b>%4</b><br/>\
		����:<b>%5</b><br/>\
		�ն����:<b>%6</b><br/>\
		������:<b>%7</b><br/>\
		����:<b>%8</b><br/>\
		���:<b>%9</b><br/>\
		���� :<b>%10</b><br/>\
		����:<b>%11</b><br/>\
		��������:<b>%12</b>����<br/>\
		�ɼ�����:<b>%13</b><br/>\
		ԭʼ����:<b>%14</b><br/>\
		��������־:<b>%15</b><br/>\
		���ֱ�־:<b>%16</b><br/>\
		���̱�־:<b>%17</b><br/>\
		ɾ����־:<b>%18</b><br/>\
		��ʼ����ʱ��:<b>%19</b><br/>\
		��ʼ״̬:<b>%20</b><br/>\
		�Ƿ�ͳ��:<b>%21</b><br/>\
		�Ƿ���ʾ:<b>%22</b><br/>\
		 </style> ")
		.arg(kwhitem->name)
		.arg(kwhitem->describe)
		.arg(kwhitem->zone)
		.arg(kwhitem->devname)
		.arg(kwhitem->typeDescribe)
		.arg(kwhitem->terminalno)
		.arg(kwhitem->kwhno)
		.arg(kwhitem->basecode)
		.arg(kwhitem->scale)
		.arg(static_cast<ulong>(kwhitem->maxcode))
		.arg(kwhitem->times)
		.arg(kwhitem->savetime)
		.arg(kwhitem->collectortype)
		.arg(kwhitem->primitivemaxcode)
		.arg(kwhitem->iscomputer==0 ? tr("��"):tr("��"))
		.arg(kwhitem->isintegral==0 ? tr("��"):tr("��"))
		.arg(kwhitem->issave==0 ? tr("��"):tr("��"))
		.arg(kwhitem->isdel==0 ? tr("��"):tr("��"))
		.arg(kwhitem->starttime)
		.arg(kwhitem->startstatus==0 ? tr("��"):tr("��"))
		.arg(kwhitem->isstat==0 ? tr("��"):tr("��"))
		.arg(kwhitem->isshow==0 ? tr("��") : tr("��")));
	}
}


void SelectorWidget::findTextChanged( const QString & text )
{
	refind=true;
}

void SelectorWidget::findEnterPressed()
{

	QTreeWidgetItemIterator treeNodeIt(devTree);
	QString treeNodeText;
	findText=findEdit->text().trimmed();
	if(refind)
	{
		treeNodeIt=QTreeWidgetItemIterator(devTree);
	}
	else
		treeNodeIt=QTreeWidgetItemIterator(lastTreeItem);
	
	while(*treeNodeIt)
	{
		treeNodeText=(*treeNodeIt)->text(0); 
		if(treeNodeText.contains(findText,Qt::CaseInsensitive))
		{
			devTree->scrollToItem(*treeNodeIt);
			devTree->setItemSelected(*treeNodeIt,true);
			devTree->setCurrentItem(*treeNodeIt,0);
// 			devTree->setItemExpanded(*treeNodeIt,true);  //������ʱ�ˣ�����Ī������Ĵ���
			(*treeNodeIt)->setExpanded(true);
			
			treeNodeIt++; //assurance find from next item start 
			lastTreeItem=*treeNodeIt;
			if(lastTreeItem)
			{
				treeNodeText=(*treeNodeIt)->text(0);
			}
			refind=false;
			return ;
		}
		treeNodeIt++;
	}

	if(findprompt)
	{
		if(DMessageBox::question(this,tr("��ʾ"),tr("�����ѵ���ͼĩβ��\r\n�´��Ƿ�رձ���ʾ��"),
			QMessageBox::Yes|QMessageBox::No,QMessageBox::NoButton)==QMessageBox::Yes)
			findprompt=false;
	}
	refind=true;
	return;
}

void SelectorWidget::setCurrentTreeItem( QString ycCode)
{
	if (m_mapYcCode_treeItem.contains(ycCode) )
	{
		QTreeWidgetItem *pItem = m_mapYcCode_treeItem[ycCode];
	//	devTree->setCurrentItem(pItem,0,QItemSelectionModel::Select);

		devTree->setCurrentItem(pItem);

	}
	else
	{
/*
		QMessageBox::information(NULL,tr("����"),tr("ң�ⶨλʧ�ܡ�"),
			QMessageBox::Yes|QMessageBox::No,QMessageBox::NoButton);
*/
	}

}

void SelectorWidget::showRtuDetailObj(  QTreeWidgetItem * item )
{
	if ( item->parent() != NULL )
	{
		if (item->data(0,Qt::UserRole).toInt()==-1)
		{
           return ;
		}
	
	}
	if( item->childCount() > 0 )
	{
		return;
	}
	if (item->data(0,Qt::UserRole).toInt()==-1)
	{
		return;
	}

	int rtuNo;
	if( !m_rtuItem2RtuNoMap.contains( item ) )
	{
		return;
	}

	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);

	rtuNo = m_rtuItem2RtuNoMap[ item ];

	QTreeWidgetItem* itemYc ;
	YcParas::const_iterator   itYC;
	QVariant  Var;

	for( itYC = myApp->ycparas.begin() ; itYC != myApp->ycparas.end() ; itYC++ )
	{
		if( ( *itYC )->terminalno == rtuNo )
		{
			//if( !myApp->checkLoginUserZone( ( *itYC )->zone ) )
			//	continue;
	
			itemYc=new QTreeWidgetItem( QStringList( ( *itYC )->describe ) ) ;
			if( ( *itYC )->plantime > 0 )
				itemYc->setIcon( 0 , QIcon( ":/res/buttonok1.png" ) ) ;
			else
				itemYc->setIcon( 0 , QIcon( ":/res/buttonok.png" ) ) ;
			itemYc->setData( 0 , Qt::UserRole , QVariant( YCParaNode ) ) ;
			qVariantSetValue<ycpara*>( Var , *itYC ) ;
			itemYc->setData( 0 , Qt::UserRole+1 , Var ) ;
			if( m_showCheckBox )
				itemYc->setCheckState( 0 , Qt::Unchecked ) ;
			item->addChild( itemYc ) ;
	
			m_mapYcCode_treeItem.insert( ( *itYC )->name , itemYc ) ;
		}
	}
	
}



