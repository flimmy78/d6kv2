#include <QEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QToolBar>
#include <QList>
#include <QLineEdit>
#include <QString>
#include <QValidator>
#include <QLabel>
#include <QTimer>
#include <QResizeEvent>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QString>
#include <QDir>
#include <QFile>

#include "msgviewwnd.h"
#include "QtScrollView.h"
#include "msgframe.h"
#include "sysdef.h"
#include "CNetModulaData.h"
#include "QtScrollView.h"
#include "CNetModula.h"
//#include "vld.h"

#include "uitools/dmessagebox.h"




QMsgFrame::QMsgFrame(QWidget *parent)
:QFrame(parent)
{
	QString dir;
	m_pScrollView = new QtScrollView(this);
	m_pTopToolbar = new QToolBar(this);
	m_pNextLayerToolbar = new QToolBar(this);
	m_pTopToolbar->setMovable(false);
	m_pNextLayerToolbar->setMovable(false);
	

	m_pauseAction = new QAction(QIcon(":res/player_pause.png"), tr("��ͣ"), this);
	m_continueAction = new QAction(QIcon(":res/player_play.png"), tr("����"), this);
	m_cleanAction = new QAction(QIcon(":res/clear.png"), tr("����"), this);
	m_saveAction = new QAction(QIcon(":res/filesave.png"), tr("����"), this);


	m_pTopToolbar->addAction(m_pauseAction);
	m_pTopToolbar->addAction(m_continueAction);
    m_pTopToolbar->addSeparator();
	m_pTopToolbar->addAction(m_cleanAction);
	//m_pTopToolbar->addAction(m_saveAction);
    m_pTopToolbar->addSeparator();

	m_pTopToolbar->addAction(m_saveAction);
	m_pTopToolbar->addSeparator();

	m_clearLabel = new QLabel(_C2Q("����"));
	m_begintimelabel = new QLabel(_C2Q("��ʼʱ�� "));
	m_endtimelabel = new QLabel(_C2Q("����ʱ�� "));
	m_search = new QLineEdit();

	m_Validator = new QRegExpValidator(QRegExp("([0-9A-F]{2}\\ {0,9}){1,10}"), this);

	m_clearSearchAction = new QAction(QIcon(":res/clear_search.png"), tr("���ɸѡ��"), this);
	m_pTopToolbar->addWidget(m_clearLabel);
	m_pTopToolbar->addWidget(m_search);
	m_pTopToolbar->addAction(m_clearSearchAction);
    m_pTopToolbar->addSeparator();

	m_checkbox = new QCheckBox(_C2Q("�Ƿ��������"),this);
	m_checkbox->setCheckable(true);  
	m_checkbox->setChecked(false);  
	m_checkbox->hide() ;
// 	m_pTopToolbar->addWidget(m_checkbox);

	//m_combobox = new QComboBox(this);

	m_begintime = new QDateTimeEdit( QDateTime::currentDateTime(), this );
	m_endtime = new QDateTimeEdit( QDateTime::currentDateTime(), this );
	m_begintime->setDisplayFormat(_C2Q("yyyy-MM-dd hh:mm:ss:zzz"));
	m_endtime->setDisplayFormat(_C2Q("yyyy-MM-dd hh:mm:ss:zzz"));
	m_pNextLayerToolbar->addWidget(m_begintimelabel);
	m_pNextLayerToolbar->addWidget(m_begintime);
	m_pNextLayerToolbar->addSeparator();
	m_pNextLayerToolbar->addWidget(m_endtimelabel);
	m_pNextLayerToolbar->addWidget(m_endtime);
	m_pNextLayerToolbar->addSeparator();
	m_autosavecheckbox = new QCheckBox(_C2Q("�Զ��洢����"),this);
	m_autosavecheckbox->setToolTip( tr( "���Ĵ洢��d6000/message�ļ�����" ) ) ;
	m_autosavecheckbox->setCheckable(true);  
	m_autosavecheckbox->setChecked(false); 
	m_pNextLayerToolbar->addWidget(m_autosavecheckbox);
	
	connect(m_checkbox, SIGNAL(toggled(bool)), this, SLOT(onCheck()));
	connect(m_pauseAction, SIGNAL(triggered()), this, SLOT(onPause()));
	connect(m_continueAction, SIGNAL(triggered()), this, SLOT(onContinue()));
	connect(m_cleanAction, SIGNAL(triggered()), this, SLOT(onClean()));
	connect(m_saveAction, SIGNAL(triggered()), this, SLOT(onSave()));
	connect(m_autosavecheckbox, SIGNAL(toggled(bool)), this, SLOT(autoSave()));  //�Զ��洢
	connect(m_clearSearchAction, SIGNAL(triggered()), this, SLOT(onClearSearch()));
	connect(m_search, SIGNAL(textEdited( const QString &)), this, SLOT(onSearchTextChange()));

	connect(m_begintime, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(setTime()));
	connect(m_endtime, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(setTime()));


	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()));  //����ʱ����¹�����
	m_pTimer->start(250);

	UpdataToolBar();
}

QMsgFrame::~QMsgFrame()
{
	printf("\n delete QMsgFrame");
	delete m_pTopToolbar;
	m_pTopToolbar = NULL;
	delete m_pNextLayerToolbar;
	m_pNextLayerToolbar = NULL;
	delete m_pauseAction;
	m_pauseAction = NULL;
	delete m_continueAction;
	m_continueAction = NULL;
	delete m_cleanAction;
	m_cleanAction = NULL;
	delete m_saveAction;
	m_saveAction = NULL;
	delete m_clearSearchAction;
	m_clearSearchAction = NULL;
	delete m_pTimer;
	m_pTimer = NULL;
	delete m_search;
	m_search = NULL;
	delete m_clearLabel;
	m_clearLabel = NULL;
	delete m_Validator;
	m_Validator = NULL;
	
	delete m_checkbox;
	m_checkbox = NULL;
	
	delete m_begintimelabel;
	m_begintimelabel = NULL;
	delete m_endtimelabel;
	m_endtimelabel = NULL;
	delete m_begintime;
	m_begintime = NULL;
	delete m_endtime;
	m_endtime = NULL;
	delete m_autosavecheckbox;
	m_autosavecheckbox = NULL;

	delete m_pScrollView;
	m_pScrollView = NULL;
}

void QMsgFrame::resizeEvent(QResizeEvent *re)
{
m_pTopToolbar->setGeometry(0,
						   1,
						   re->size().width(), 
						   m_pTopToolbar->size().height());
m_pNextLayerToolbar->setGeometry(0,
								 m_pTopToolbar->size().height() + 2,
								 re->size().width(),  
								 m_pNextLayerToolbar->size().height());
//˫������
m_pScrollView->setGeometry(0,
						   m_pTopToolbar->size().height() + m_pNextLayerToolbar->size().height() + 4, 
						   re->size().width(),
						   re->size().height() -  m_pTopToolbar->size().height() -2 - m_pNextLayerToolbar->size().height() - 2);

//��������
//m_pScrollView->setGeometry(0,
//						   m_pTopToolbar->size().height() + 4, 
//						   re->size().width(),
//						   re->size().height() -  m_pTopToolbar->size().height() -2  - 2);
}

void QMsgFrame::showEvent(QShowEvent *)
{
	m_pScrollView->viewport()->update();
}

void QMsgFrame::onPause()
{
	if(m_pScrollView->m_pCurrentModula)
	{
		m_pScrollView->m_pCurrentModula->SetPause(true);
		UpdataToolBar();
	}	
}

void QMsgFrame::onContinue()
{
	if(m_pScrollView->m_pCurrentModula)
	{
		m_pScrollView->m_pCurrentModula->SetPause(false);
		UpdataToolBar();
	}
}

void QMsgFrame::onClean()
{
	if(m_pScrollView->m_pCurrentModula)
	{
		m_pScrollView->m_pCurrentModula->ClearListData();
	}
	m_pScrollView->UpdateScroll();
	m_pScrollView->viewport()->update();
}

void QMsgFrame::onSave()
{	
	if(m_pScrollView->m_pCurrentModula)
	{
		QString fileName = QFileDialog::getSaveFileName(this,
									_C2Q("���Ϊ"),  //����
									"message",    //�洢�ļ��� 	QString(),
									tr("Text Files (*.txt);;Text Files (*.txt)"));  //�洢��ʽ
		if (!fileName.isEmpty()) 
		{
			QFile file(fileName);
			if (!file.open(QFile::Truncate | QFile::WriteOnly | QFile::Text))
			{
				DMessageBox::information(this, tr("���汨��"), tr("�����ļ� ")+ fileName +tr(" ʧ��!"));
				return;
		    }

			if(!m_pScrollView->m_pCurrentModula->m_ListData.isEmpty())
			{
				QTextStream out(&file);
				out << _C2Q("ͨ��: ");
				out << m_pScrollView->m_pCurrentModula->GetName();  //��ȡͨ������
				out << QString("    ");
				out << _C2Q("����ʱ��: ");
			    QDateTime datetime = QDateTime::currentDateTime();
				out << datetime.toString("yyyy-MM-dd hh:mm:ss (zzz 'ms')");
				out << QString("\n");
				//QList<CNetModulaData*> m_ListData;
				//����ֻ��������

				QListIterator<CNetModulaData*> saveIterator(m_pScrollView->m_pCurrentModula->m_ListData);
				saveIterator.toFront();

				while(saveIterator.hasNext())
				{
					CNetModulaData* pModulaData = saveIterator.next();
				
						if(pModulaData)
						{
							SaveModulaData(pModulaData, out, pModulaData->GetStrTime());  //�洢����
						}
				}
			}

			file.close();
			DMessageBox::information(this, tr("���汨��"), tr("�����ļ� ")+ fileName +tr(" �ɹ�!"));
		}
	}
}

void QMsgFrame::autoSave()
{
	QDateTime auto_save_begintime = m_begintime->dateTime();   //��ʼʱ��
	QDateTime auto_save_endtime = m_endtime->dateTime();

		if (m_autosavecheckbox->isChecked())   //�洢ʱ����д���
		{
			m_pScrollView->m_pCurrentModula->Set_autosave_begintime(auto_save_begintime);
			m_pScrollView->m_pCurrentModula->Set_autosave_endtime(auto_save_endtime);
			m_pScrollView->m_pCurrentModula->Set_autosave_checkbox_state(1);

		}
		else
			m_pScrollView->m_pCurrentModula->Set_autosave_checkbox_state(0);
}


void QMsgFrame::setTime()
{
	QDateTime auto_save_begintime = m_begintime->dateTime();   //��ʼʱ��
	QDateTime auto_save_endtime = m_endtime->dateTime();

	if ( m_pScrollView->m_pCurrentModula )
	{
		m_pScrollView->m_pCurrentModula->Set_autosave_begintime(auto_save_begintime);
		m_pScrollView->m_pCurrentModula->Set_autosave_endtime(auto_save_endtime);
	}
}



void QMsgFrame::onTimer()
{
	if(m_pScrollView->m_pCurrentModula)
	{
		if(m_pScrollView->m_pCurrentModula->GetPause())
		{
			return;
		}
		m_pScrollView->UpdateScroll();
		m_pScrollView->viewport()->update();
	}
}



void QMsgFrame::UpdataToolBar()
{
	if(m_pScrollView->m_pCurrentModula)
	{
		//�Ƿ�����ͣ��ť
		//&& m_pScrollView->checkbox_state == 1 
		if(m_pScrollView->m_pCurrentModula->GetPause())
		{
			m_pauseAction->setEnabled(false);
			m_continueAction->setEnabled(true);
		}
		else
		{
			m_pauseAction->setEnabled(true);
			m_continueAction->setEnabled(false);
		}
		if (m_pScrollView->checkbox_state == 0)  //20130717,chengzhu
		{
			m_cleanAction->setEnabled(true);
			m_saveAction->setEnabled(true);
			m_search->setEnabled(true);
		}
		if (m_pScrollView->checkbox_state == 1)
		{
			m_cleanAction->setEnabled(false);
			m_saveAction->setEnabled(false);
			m_search->setEnabled(false);
		}

	}
	else
	{
		m_pauseAction->setEnabled(false);
		m_continueAction->setEnabled(false);
		m_cleanAction->setEnabled(false);
		m_saveAction->setEnabled(false);
	}
}


void QMsgFrame::onClearSearch(void)
{
	m_search->clear();
	m_pScrollView->m_keyWordList.clear();
}


//���ұ���
void QMsgFrame::onSearchTextChange()
{
	m_pScrollView->m_keyWordList.clear();  //�����������������
	QString text = m_search->text().toUpper();
	QString temp;
	int pos = 0;
	//m_Validator = new QRegExpValidator(QRegExp("([0-9A-F]{2}\\ {0,9}){1,10}"), this);
	if (m_Validator->validate(text, pos) != QValidator::Acceptable)
	{
		printf("\nstring unaccept!");
		return;
	}

	pos = text.indexOf(_C2Q(" "));

	while (pos != -1)
	{
		text.remove(pos, 1);
		pos = text.indexOf(_C2Q(" "));
	}

	QByteArray ba = text.toAscii();
	char* str = ba.data();
	printf("\n%s",str);

	if (text.length() < 2)
	{
		return;
	}

	while (text.length() >= 2)
	{
		bool ok;
		BYTE tmp = text.mid(0,2).toInt(&ok, 16);
		if (true == ok)
		{
			printf("\nthe key word is %02X",tmp);
			m_pScrollView->m_keyWordList.push_back(tmp);
			text = text.remove(0,2);
		}
	}
}

void QMsgFrame::OnCurrentModulaChange(CNetModula* pCurrentModula)
{
	if (NULL == pCurrentModula)
	{
		return;
	}
	m_pScrollView->m_pCurrentModula = pCurrentModula;
	UpdataToolBar();
	m_pScrollView->UpdateScroll();
	m_pScrollView->viewport()->update();
}


void QMsgFrame::SaveModulaData(CNetModulaData* pModulaData, QTextStream &output, QString strtime)
{
	if(!pModulaData)
		return;

	//QString strtime;
	//QDateTime datetime = QDateTime::currentDateTime();
	//strtime = datetime.toString("yyyy-MM-dd hh:mm:ss (zzz 'ms')");
	
	QString strTitle;

	//QListIterator<QString> message_time_show(m_pScrollView->m_pCurrentModula->message_time);
	//message_time_show.toFront();


	switch(pModulaData->GetType())
	{
	case 1:
		strTitle.sprintf("RECV(%d)", pModulaData->GetDataLength());
		strTitle += _C2Q("        ��ǰʱ��:");
		strTitle += strtime;
		strTitle += _C2Q("\n");
		break;	
	case 2:
		strTitle.sprintf("SEND(%d)", pModulaData->GetDataLength());
		strTitle += _C2Q("        ��ǰʱ��:");
		strTitle += strtime;
		strTitle += _C2Q("\n");
		break;
	default:
		strTitle.sprintf("UNKNOW(%d)\n", pModulaData->GetDataLength());
		/*strTitle += _C2Q("        ��ǰʱ��:");
		strTitle += strtime;
		strTitle += _C2Q("\n");*/
		break;
	}

	if (1 == pModulaData->GetType() || 2 == pModulaData->GetType())
	{
		output << strTitle;
	}
	
	
	if (1 == pModulaData->GetType() || 2 == pModulaData->GetType())
	{
		QString strTemp;
		int nCount = 0;
		
		while(nCount < pModulaData->GetDataLength())
		{
			for(int i = 0; i < 16; i++)
			{
				if(nCount + i < pModulaData->GetDataLength())
				{
					strTemp.sprintf("%02X ", (BYTE)(*(pModulaData->GetData()+nCount+i)));
					output << strTemp;
				}
			}
			output << QString("   ");
			output << QString("\n");
			nCount += 16;
		}
	}
}

//void QMsgFrame::AutoSaveModulaData(CNetModulaData* pModulaData, QTextStream &output, QString strtime)
//{
//	QDateTime auto_save_begintime = m_begintime->dateTime();
//	QDateTime auto_save_endtime = m_endtime->dateTime();
//
//
//}

void QMsgFrame::onCheck()
{
	if(m_checkbox->isChecked())
	{
		m_pScrollView->checkbox_state = 1;	
	}
	else
		m_pScrollView->checkbox_state = 0;	

	
}





