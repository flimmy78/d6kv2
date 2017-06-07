#ifndef HISDATAMODEL_H
#define HISDATAMODEL_H

#include <QAbstractTableModel>

#include "datacurvedefine.h"
#include "datacurveapp.h"

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
QT_END_NAMESPACE

#define REAL_PRECISION  3

#define REAL_MIN_DESC				tr("ʵʱ��Сֵ")
#define STAT_MIN_DESC				tr("ͳ����Сֵ")
#define REAL_MAX_DESC				tr("ʵʱ���ֵ")
#define STAT_MAX_DESC				tr("ͳ�����ֵ")
#define REAL_AVG_DESC				tr("ʵʱƽ��ֵ")
#define STAT_AVG_DESC				tr("ͳ��ƽ��ֵ")

#define STAT_TIME_POINT_DESC		tr("����ʱ��")

#define STAT_DAY_INVALID_COUNT_DESC	tr("����Ч���ݵ����")
#define STAT_DAY_INVALID_TIME_DESC	tr("����Чʱ��")

class MainWindow;

class HisDataModel : public QAbstractTableModel  
{
	Q_OBJECT
public:
	HisDataModel(QObject *parent=0);
	virtual ~HisDataModel();
public:
	virtual int rowCount ( const QModelIndex & parent = QModelIndex()) const;
	virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
	virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
	virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
	virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	virtual bool setHeaderData ( int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole );
	virtual bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
	virtual bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
	virtual bool insertColumns ( int column, int count, const QModelIndex & parent = QModelIndex() );
	virtual bool removeColumns ( int column, int count, const QModelIndex & parent = QModelIndex() );


//////////////////////////////////////////////////////////////////////////
////my defined
	//bool	        isModified(){return beModified;}           //model data is modified?
protected:
// 	bool			showCycleAllData;	//�Ƿ���ģ���з���ѡ�������е�ȫ������
// 	bool			showNullValue;		//��ʾ��ֵ��


	selectorpara	  lastSelector;					    //��������������ѡ����
	selectorcurvepara currentCurveSelector;				//
public slots:
	void updateModelData(selectorpara& selPara);			 //main slot,selector changed emit
	void selectorChanged(selectorpara & selpara);            //ui main selector changed solt,process min day min;
	void selectorCurveChanged(selectorcurvepara& selpara);   //ui curve selector changed slot,process null show;
	void updateModelDataToDB();							     //update model modified data to database,only day curve table
	
	MainWindow  *mainWindow(){return (MainWindow*)QObject::parent();}
public:
	bool curveDataBound(double& min,double& max);			//get current model data max min value(not include empty and null)
private:
//	QDateTime modelTimeSeqComp(int modelTimeIdx,int compIdx) const;   //get modeltime mapping comp datetime
	mCurveData		modelCurveData;
	
	//////////////////////////////////////////////////////////////////////////
	int             modelSelCycle;					//current choose day min cycle in model 
	//bool            beModified;                     //curve data be modified
	int             CurveMinCycle;                  //current curves for min cycle
	int             curveBeginRow;                  //except static curve begin row position
	DbCurveData		dbCurveData;
	//////////////////////////////////////////////////////////////////////////
private:
	////model data operation
	//use db data to init all model curve data
	void  synchModelData(const selectorpara& selpara,int curveMinCycle,const DbCurveData& dbCurveData,mCurveData& modelCurveData,bool clear=true);
	//use db data to init one model curve data struct
	bool initOneCurveModelData(QString keyname,CurveCycleType cycle,int curveMinCycle,QDateTime begin,QDateTime end,int curveStatType,const DbCurveData& dbCurveData,mOneCurveData& modelData);
	//if GeneralDate get it from db,else get it model data,don't change modeldata curve value;
	bool initOneStatModelData(QString keyname,CurveCycleType cycle,CurveSegmentType segment,QDate date,int statType,mOneCurveData& modelData);
	bool getModelStatVal(CurveCycleType statCycle,int statType,mOneCurveData& mOneCurve,bool clean=true);
	void  initModelTimeSeq(CurveCycleType cycleType,QDateTime& begin,QDateTime& end,int minCycle,mTimeSequence& timeSeq);
	//find para and datetime in onecurvedata(db) return find vector index
	bool   findIn(CurveCycleType cycleType,QString keyname,QDateTime findDatetime,int findStatType,const DbCurveData& dbCurveData,hisDataValue& findValue);
	
	void updateModelNullVal(NullValueStyle nullValStyle,mCurveData& modelData);  //use ui curve selector nullvaluestyle update model empty data;
	bool isModifyed(const mCurveData& modelData) const;
	bool isModifyed(const mOneCurveData& modelOneData) const;
	bool isModifyed(const mCurveData& modelData,QString keyname,QDateTime begin) const;

	friend class DataEditCellcommand;
};

#endif
