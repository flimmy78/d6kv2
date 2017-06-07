#ifndef SELECTORWIDGET_H
#define SELECTORWIDGET_H

#include <QMap>
#include <QWidget>

#include "appdef.h"
#include "datacurveapp.h"


QT_BEGIN_NAMESPACE
class QToolBar;
class QTextEdit;
class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QTreeWidgetItemIterator;
QT_END_NAMESPACE

class SelectorWidget : public QWidget
{
	Q_OBJECT
public:
	SelectorWidget(QWidget* parent = 0);
	~SelectorWidget();

	void setCurrentTreeItem(QString ycCode);
signals:
	void selectorChanged(selectorpara&);
	void dataSrcChanged();
private slots:
	void refresh();
	void stationYcType();
	void stationKwhType();
	
	void singleSelector(bool checked);
	void mutilSelector(bool checked);
	void updateCurve(QTreeWidgetItem * current, QTreeWidgetItem * previous);  //��ѡ״̬ѡ����ĳ�ڵ��
	void updateCurve(QTreeWidgetItem *item, int column);					  //�ڶ�ѡcheckedbox�ı��
	void updateNodeInfo(QTreeWidgetItem* item);

	void showFindWidget(bool checked);
	void showNodeInfoWidget(bool checked);

	void findEnterPressed();
	void findTextChanged(const QString & text);

	void showRtuDetailObj(  QTreeWidgetItem * item );			//add by chenkai 2014.05.28
private:
	void init();
	void createActions();
	void initToolBars();

	void initStationYcTree(bool showCheckBox);
	void initStationKwhTree(bool showCheckBox);

	
private:
	QToolBar   *selectorTools;
	QAction    *actionRefresh,*actionFind,*actionShowNodeInfo,*actionFilter;
	QAction    *actionStationMeasTree,*actionEnergyMeasTree,*actionSingleSel,*actionMutilSel;
	QTreeWidget *devTree;
	QLineEdit   *findEdit;
	QTextEdit   *nodeInfoEdit;
		
	selectorpara       &currentSelPara;      //��ǰ��ϵͳѡ�����
	//UI������
	bool               filterNullNode;
	bool				m_showCheckBox ;

	//CurveTreeType      treeType;			//��ǰ��ʼ�����ͣ�վң������վ��������EMS�������
//	QString				  findpromptInfo;
	QString               findText;         //user enter find text
	bool				  refind;           //refind on find to tree buttom
	bool                  findprompt;
	QTreeWidgetItem		  *lastTreeItem;    //itor last treeitme
	QMap<QString,QTreeWidgetItem*> m_mapYcCode_treeItem;
	QMap<QTreeWidgetItem* , int> m_rtuItem2RtuNoMap ;		//add by chenkai 2014.05.28
};

#endif
