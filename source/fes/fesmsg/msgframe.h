#ifndef MSGFRAME_H_
#define MSGFRAME_H_

#include <QFrame>
#include <QTextStream>

class CNetModula;
class QtScrollView;
class QToolBar;
class QAction;
class QTimer;
class QLineEdit;
class QLabel;
class QRegExpValidator;
class QResizeEvent;
class QShowEvent;
class CNetModulaData;
class QDateTimeEdit;
class QCheckBox;
class QComboBox;



#define _Q2C(str) ((const char *)(str.local8Bit().data()))
//ʹ��fromLocal8Bit()������ʵ���˴ӱ����ַ���GB��Unicode��ת������ҪӦ���ڴ�������ʾ���������
#define _C2Q(str) (QString::fromLocal8Bit(str))

class QMsgFrame : public QFrame
{
    Q_OBJECT

public:
	QMsgFrame(QWidget *parent=0);
	~QMsgFrame();
	void UpdataToolBar();
	void OnCurrentModulaChange(CNetModula* pCurrentModula);
	void SaveModulaData(CNetModulaData* pModulaData, QTextStream &output,QString strtime);
	//void AutoSaveModulaData(CNetModulaData* pModulaData, QTextStream &output,QString strtime);  //0731


public:
	QtScrollView*	m_pScrollView;

	
private:
	QToolBar*	m_pTopToolbar;
	QToolBar*	m_pNextLayerToolbar;   //0708
	QAction*	m_pauseAction;
	QAction*	m_continueAction;
	QAction*	m_cleanAction;
	QAction*	m_saveAction;
	QAction*	m_clearSearchAction;
	QTimer*		m_pTimer;
	QLineEdit*		m_search;
	QLabel*			m_clearLabel;
	QLabel*			m_begintimelabel;
	QLabel*			m_endtimelabel;
	QRegExpValidator* m_Validator;
	QCheckBox*        m_checkbox;    //�Ƿ��������  
	
	QCheckBox*        m_autosavecheckbox;    //�Ƿ��������  
	QDateTimeEdit* m_begintime;
	QDateTimeEdit* m_endtime;






protected:
	void resizeEvent(QResizeEvent *re);
	void showEvent(QShowEvent *);

public slots:
	void onPause();
	void onContinue();
	void onClean();
	void onSave();
	void autoSave();
	void onTimer();
	void onSearchTextChange();
	void onClearSearch();
	void onCheck();
	void setTime();

	//void OnSaveLogList();
};

#endif