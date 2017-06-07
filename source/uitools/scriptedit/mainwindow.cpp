#include <QApplication>
#include <QtCore>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QToolButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QDockWidget>
#include <QPrintPreviewDialog>
#include <QDateTime>
#include <QVariant>
#include <QTreeWidget>
#include <QTextEdit>
#include <QTimerEvent>
#include <QDir>


#include "mainwindow.h"

#include "sysdef.h"

#include "appdef.h"
#include "functionwidget.h"
#include "userscriptedit.h"
#include "rdbparadialog.h"
#include "uitools/dmessagebox.h"
//scada defined
#include "upctrl.h"

extern DBExtension  dbex;
//static call for compile proc
void PrintInfoLine(const char *buf)
{
	foreach (QWidget *widget, qApp->topLevelWidgets()) {
		qDebug()<< widget->objectName();
		if(widget->inherits("QMainWindow"))
		{
			MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
			mainWin->appendOutput(C2G(buf));
		}
    }
}


MainWindow::MainWindow(QWidget *parent) 
: QMainWindow(parent),print_paper_size(QPrinter::A4),printer_orientation(QPrinter::Landscape)
{
 	
#ifdef DBG_REGPROC
	QString title(MAINWINDOWTITLE);
	netDbg.RegProc((char*)G2C(title));
	procMan.StartProc("scriptedit",(char*)G2C(title),"");
	procimerID=startTimer(PROCINTERVAL);
#endif

	init();
}

MainWindow::~MainWindow()
{
	killTimer(mainTImerID);
	
#ifdef DBG_REGPROC
	netDbg.UnRegProc();
	procMan.EndProc();
	killTimer(procimerID);
#endif
}


void MainWindow::init( bool factorySettings /*= false*/ )
{
#ifndef Q_OS_WIN32
 	QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/res/scriptedit_16.png")));
    setWindowIcon(icon);
#endif
	createActions(this);
	initMainMenu(this);
	initToolBars(this);
	initStatusBars(this);
	initCentralWidget();
	CreateDockWidget(this);
	setWindowTitle(MAINWINDOWTITLE);
	initializeRecentActions(this);
	initStyleSheet() ;
	mainTImerID=startTimer(MAINTIMERINTERVAL);
}

void MainWindow::createActions( QWidget* parent )
{
	actionNewFile = new QAction(QIcon(":/res/filenew.png"),tr("�½�..."),parent);
    actionNewFile->setObjectName(QString::fromUtf8("actionNewFile"));
	actionNewFile->setToolTip(tr("����һ���µĽű�"));
	actionNewFile->setStatusTip(actionNewFile->toolTip());
	actionNewFile->setShortcut( tr("Ctrl+N") );
	connect(actionNewFile, SIGNAL(activated()), parent, SLOT(newFile()));

	actionOpenFile = new QAction(QIcon(":/res/fileopen.png"),tr("��..."),parent);
    actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
	actionOpenFile->setToolTip(tr("��һ�����ڵĽű��ļ�"));
	actionOpenFile->setStatusTip(actionOpenFile->toolTip());
	actionOpenFile->setShortcut( tr("Ctrl+O") );
	connect(actionOpenFile, SIGNAL(activated()), parent, SLOT(open()));

	actionSave = new QAction(QIcon(":/res/filesave.png"),tr("����"),parent);
    actionSave->setObjectName(QString::fromUtf8("actionSave"));
	actionSave->setToolTip(tr("���浱ǰ�༭�Ľű����ļ���"));
	actionSave->setStatusTip(actionSave->toolTip());
	actionSave->setShortcut( tr("Ctrl+S") );
	connect(actionSave, SIGNAL(activated()), parent, SLOT(save()));

	actionSaveAs = new QAction(QIcon(":/res/saveas.png"),tr("���Ϊ..."),parent);
    actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
	actionSaveAs->setToolTip(tr("��ǰ�༭�Ľű���浽�����ļ���"));
	actionSaveAs->setStatusTip(actionSaveAs->toolTip());
	//actionSaveAs->setShortcut( tr("Ctrl+S") );
	connect(actionSaveAs, SIGNAL(activated()), parent, SLOT(saveAs()));
		
	actionPrint = new QAction(QIcon(":/res/fileprint.png"),tr("��ӡ"),parent);
    actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
	actionPrint->setToolTip(tr("���͵�ǰ�ű�����ӡ���ϴ�ӡ"));
	actionPrint->setStatusTip(actionPrint->toolTip());
	actionPrint->setShortcut( tr("Ctrl+P") );
	connect(actionPrint, SIGNAL(activated()), parent, SLOT(print()));
	
	actionPrintpreview = new QAction(QIcon(":/res/printpreview.png"),tr("��ӡԤ��"),parent);
    actionPrintpreview->setObjectName(QString::fromUtf8("actionPrintpreview"));
	actionPrintpreview->setToolTip(tr("��ӡǰԤ����ǰ�ű�"));
	actionPrintpreview->setStatusTip(actionPrintpreview->toolTip());
	actionPrintpreview->setShortcut( tr("Ctrl+Alt+P") );
	connect(actionPrintpreview, SIGNAL(activated()), parent, SLOT(printPreview()));
	
	actionExitApp = new QAction(QIcon(":/res/exit.png"),tr("�˳�"),parent);
    actionExitApp->setObjectName(QString::fromUtf8("actionExitApp"));
	actionExitApp->setToolTip(tr("�˳�Ӧ�ó���"));
	actionExitApp->setStatusTip(actionExitApp->toolTip());
	actionExitApp->setShortcut( tr("Alt+X") );
	connect(actionExitApp, SIGNAL(activated()), parent, SLOT(exit()));


	actionUndo = new QAction(QIcon(":/res/undo.png"),tr("����"),parent);
    actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
	actionUndo->setToolTip(tr("���ĵ����޸Ľ��г�������"));
	actionUndo->setStatusTip(actionUndo->toolTip());
	actionUndo->setShortcut( tr("Ctrl+Z") );
	//connect(actionUndo, SIGNAL(activated()), parent, SLOT(undo()));

	actionRedo = new QAction(QIcon(":/res/redo.png"),tr("����"),parent);
    actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
	actionRedo->setToolTip(tr("���ĵ����޸Ľ�����������"));
	actionRedo->setStatusTip(actionRedo->toolTip());
	actionRedo->setShortcut( tr("Ctrl+Y") );
	//connect(actionRedo, SIGNAL(activated()), parent, SLOT(redo()));

	actionViewOutput=new QAction(tr("��Ŀ���"),parent);
	actionViewOutput->setObjectName(QString::fromUtf8("actionViewOutput"));
	actionViewOutput->setToolTip(tr("��ʾ/����������ĿС����"));
	actionViewOutput->setStatusTip(actionViewOutput->toolTip());
	actionViewOutput->setCheckable(true);
	actionViewOutput->setChecked(false);
	connect(actionViewOutput, SIGNAL(triggered(bool)), parent, SLOT(viewOutput(bool)));

	actionCopy = new QAction(QIcon(":/res/editcopy.png"),tr("����"),parent);
    actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
	actionCopy->setToolTip(tr("���Ƶ�ǰѡ����ĵ�����������"));
	actionCopy->setStatusTip(actionCopy->toolTip());
	actionCopy->setShortcut( tr("Ctrl+C") );
	//connect(actionCopy, SIGNAL(activated()), parent, SLOT(copy()));


	actionPaste = new QAction(QIcon(":/res/editpaste.png"),tr("ճ��(&P)"),parent);
    actionPaste->setObjectName(QString::fromUtf8("actionCopy"));
	actionPaste->setToolTip(tr("�Ӽ������н��ĵ�ճ������ǰλ��"));
	actionPaste->setStatusTip(actionPaste->toolTip());
	actionPaste->setShortcut( tr("Ctrl+V") );
	//connect(actionPaste, SIGNAL(activated()), parent, SLOT(paste()));

	actionCut = new QAction(QIcon(":/res/editcut.png"),tr("����(&T)"),parent);
    actionCut->setObjectName(QString::fromUtf8("actionCut"));
	actionCut->setToolTip(tr("���е�ǰѡ����ĵ����������У���ǰѡ���ĵ�������"));
	actionCut->setStatusTip(actionCut->toolTip());
	actionCut->setShortcut( tr("Ctrl+X") );
	//connect(actionCopy, SIGNAL(activated()), parent, SLOT(cut()));


	actionInsertFunc = new QAction(QIcon(":/res/insertfunc.png"),tr("���뺯��..."),parent);
    actionInsertFunc->setObjectName(QString::fromUtf8("actionInsertFunc"));
	actionInsertFunc->setToolTip(tr("���뺯������ǰ����λ��"));
	actionInsertFunc->setStatusTip(actionInsertFunc->toolTip());
	//actionInsertFunc->setShortcut( tr("Ctrl+X") );
	connect(actionInsertFunc, SIGNAL(activated()), parent, SLOT(insertFunc()));

	actionInsertSCDFunc = new QAction(QIcon(":/res/insertscdfunc.png"),tr("ʵʱ�������..."),parent);
    actionInsertSCDFunc->setObjectName(QString::fromUtf8("actionInsertSCDFunc"));
	actionInsertSCDFunc->setToolTip(tr("�ڵ�ǰ�༭λ�ò���ʵʱ�������"));
	actionInsertSCDFunc->setStatusTip(actionInsertSCDFunc->toolTip());
	//actionInsertSCDFunc->setShortcut( tr("") );
	connect(actionInsertSCDFunc, SIGNAL(activated()), parent, SLOT(insertSCDFunc()));




	actionToolBar = new QAction(tr("������"),parent);
    actionToolBar->setObjectName(QString::fromUtf8("actionToolBar"));
	actionToolBar->setToolTip(tr("�Ƿ���ʾϵͳ�Ĺ�����"));
	actionToolBar->setStatusTip(actionToolBar->toolTip());
	actionToolBar->setCheckable(true);
	actionToolBar->setChecked(true);
	connect(actionToolBar, SIGNAL(activated()), parent, SLOT(viewToolBar()));

	actionStatusBar = new QAction(tr("״̬��"),parent);
    actionStatusBar->setObjectName(QString::fromUtf8("actionStatusBar"));
	actionStatusBar->setToolTip(tr("�Ƿ���ʾϵͳ��״̬��"));
	actionStatusBar->setStatusTip(actionStatusBar->toolTip());
	actionStatusBar->setCheckable(true);
	actionStatusBar->setChecked(true);
	connect(actionStatusBar, SIGNAL(activated()), parent, SLOT(viewStatusBar()));

	
	actionCompile = new QAction(QIcon(":/res/compile.png"),tr("����Ŀ�����"),parent);
    actionCompile->setObjectName(QString::fromUtf8("actionCompile"));
	actionCompile->setToolTip(tr("���뵱ǰ�Ľű��ļ���������upo�ļ���ָ����Ŀ¼��"));
	actionCompile->setStatusTip(actionCompile->toolTip());
	actionCompile->setShortcut( tr("F7") );
	connect(actionCompile, SIGNAL(activated()), parent, SLOT(compile()));

#ifdef HANDEXECPROC
	actionExecproc = new QAction(QIcon(":/res/execproc.png"),tr("�ֶ�ִ�й���"),parent);
    actionExecproc->setObjectName(QString::fromUtf8("actionExecproc"));
	actionExecproc->setToolTip(tr("�˹�ִ��ѡ��Ľű��ļ���"));
	actionExecproc->setStatusTip(actionExecproc->toolTip());
	actionExecproc->setShortcut( tr("F5") );
	connect(actionExecproc, SIGNAL(activated()), parent, SLOT(execProc()));
#endif

	actionAbout = new QAction(tr("����..."),parent);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
	actionAbout->setToolTip(tr("������İ�Ȩ˵�����򵥰���֧����Ϣ"));
	actionAbout->setStatusTip(actionAbout->toolTip());
	connect(actionAbout, SIGNAL(activated()), parent, SLOT(about()));
}

void MainWindow::initMainMenu( QWidget * parent)
{
	menubar=menuBar();
	menubar->setObjectName(QString::fromUtf8("menubar"));

	fileMenu = new QMenu(menubar);
	fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
	fileMenu->setTitle(tr("�ļ�(&F)"));
	
	editMenu = new QMenu(menubar);
    editMenu->setObjectName(QString::fromUtf8("editMenu"));
	editMenu->setTitle(tr("�༭(&E)"));
	
    viewMenu = new QMenu(menubar);
    viewMenu->setObjectName(QString::fromUtf8("viewMenu"));
	viewMenu->setTitle(tr("�鿴(&V)"));
	
	funcMenu = new QMenu(menubar);
    funcMenu->setObjectName(QString::fromUtf8("funcMenu"));
	funcMenu->setTitle(tr("����(&I)"));

	compileMenu = new QMenu(menubar);
    compileMenu->setObjectName(QString::fromUtf8("compileMenu"));
	compileMenu->setTitle(tr("����(&B)"));
	
    helpMenu = new QMenu(menubar);
    helpMenu->setObjectName(QString::fromUtf8("helpMenu"));
	helpMenu->setTitle(tr("����(&H)"));
	
	menubar->addAction(fileMenu->menuAction());
    menubar->addAction(editMenu->menuAction());
	menubar->addAction(viewMenu->menuAction());
    menubar->addAction(funcMenu->menuAction());
	menubar->addAction(compileMenu->menuAction());
    menubar->addAction(helpMenu->menuAction());

	fileMenu->addAction(actionNewFile);
	fileMenu->addAction(actionOpenFile);
	fileMenu->addSeparator();
	fileMenu->addAction(actionSave);
	fileMenu->addAction(actionSaveAs);
	fileMenu->addSeparator();
	fileMenu->addAction(actionPrint);
	fileMenu->addAction(actionPrintpreview);
	fileMenu->addSeparator();
	fileMenu->addAction(actionExitApp);

	editMenu->addAction(actionUndo);
	editMenu->addAction(actionRedo);
	editMenu->addSeparator();
	editMenu->addAction(actionCopy);
	editMenu->addAction(actionPaste);
	editMenu->addAction(actionCut);

	viewMenu->addAction(actionToolBar);
	viewMenu->addAction(actionStatusBar);
	viewMenu->addSeparator();
	viewMenu->addAction(actionViewOutput);

	//funcMenu->addAction(actionInsertFunc);
	funcMenu->addAction(actionInsertSCDFunc);
	
	compileMenu->addAction(actionCompile);
#ifdef HANDEXECPROC
	compileMenu->addAction(actionExecproc);
#endif
	helpMenu->addAction(actionAbout);

}

void MainWindow::initToolBars( QWidget * parent )
{	
	fileTools = new QToolBar(tr("�ļ�����"),parent);
    fileTools->setObjectName(QString::fromUtf8("fileTools"));
	fileTools->setIconSize( QSize( 20 , 20 ) ) ;
    ((QMainWindow*)parent)->addToolBar(Qt::TopToolBarArea, fileTools);
	
// 	editTools = new QToolBar(tr("�༭����"),parent);
//     editTools->setObjectName(QString::fromUtf8("editTools"));
//     ((QMainWindow*)parent)->addToolBar(Qt::TopToolBarArea, editTools);
	
// 	viewTools = new QToolBar(tr("�鿴����"),parent);
//     viewTools->setObjectName(QString::fromUtf8("viewTools"));
//     ((QMainWindow*)parent)->addToolBar(Qt::TopToolBarArea, viewTools);

// 	funcTools = new QToolBar(tr("���ܹ���"),parent);
//     funcTools->setObjectName(QString::fromUtf8("funcTools"));
//     ((QMainWindow*)parent)->addToolBar(Qt::TopToolBarArea, funcTools);

// 	compileTools = new QToolBar(tr("���빤��"),parent);
//     compileTools->setObjectName(QString::fromUtf8("compileTools"));
//     ((QMainWindow*)parent)->addToolBar(Qt::TopToolBarArea, compileTools);

	
	fileTools->addAction(actionNewFile);
    fileTools->addAction(actionOpenFile);
	fileTools->addSeparator();
    fileTools->addAction(actionSave);
	fileTools->addSeparator();
	fileTools->addAction(actionPrint);
	fileTools->addAction(actionPrintpreview);
	fileTools->addSeparator();
	fileTools->addAction(actionExitApp);

	fileTools->addAction(actionUndo);
	fileTools->addAction(actionRedo);
	fileTools->addSeparator();
	fileTools->addAction(actionCopy);
	fileTools->addAction(actionPaste);
	fileTools->addAction(actionCut);
	
// 	viewTools->addAction(actionToolBar);
// 	viewTools->addAction(actionStatusBar);
	
//	funcTools->addAction(actionInsertFunc);
	fileTools->addAction(actionInsertSCDFunc);
	fileTools->addSeparator();

	
	fileTools->addAction(actionCompile);
}

void MainWindow::initStatusBars( QWidget * parent )
{
	statusBar()->showMessage(tr("����..."));
	statusbar=statusBar();
	statusbar->setObjectName(QString::fromUtf8("statusbar"));

}

void MainWindow::initCentralWidget()
{
	m_editor=new UserScriptEdit(this);
	m_editor->setBackgroundVisible(false);
#if defined(Q_OS_WIN32)
	QFont font("simsun",10,QFont::Bold,false);  
	m_editor->setFont(font);
#endif
	 setCentralWidget(m_editor);
}

void MainWindow::CreateDockWidget( QMainWindow* mainWin )
{
	dockFunctionWidget = new QDockWidget(tr("���ܹ���"),mainWin);
	dockFunctionWidget->setObjectName(QString::fromUtf8("dockWidget"));
	dockFunctionWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::NoDockWidgetFeatures);
	dockFunctionWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	funcWidget=new FunctionWidget(dockFunctionWidget);
	funcWidget->setObjectName(QString::fromUtf8("funcWidget"));
	dockFunctionWidget->setWidget(funcWidget);
	mainWin->addDockWidget(Qt::LeftDockWidgetArea, dockFunctionWidget);
	//////////////////////////////////////////////////////////////////////////
	//dock information output
	dockInfoTip = new QDockWidget(tr("�����ʾ"),mainWin);
	dockInfoTip->setObjectName(QString::fromUtf8("dockInfoTip"));
	dockInfoTip->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::NoDockWidgetFeatures);
	dockInfoTip->setAllowedAreas(Qt::BottomDockWidgetArea);
	dockInfoTip->setVisible(actionViewOutput->isChecked());
	compileOut=new QTextEdit(dockInfoTip);
	compileOut->setObjectName(QString::fromUtf8("compileOut"));
	compileOut->setReadOnly(true);
	dockInfoTip->setWidget(compileOut);
	mainWin->addDockWidget(Qt::BottomDockWidgetArea, dockInfoTip);
	dockFunctionWidget->setMinimumWidth( 300 ) ;
}


void MainWindow::initializeRecentActions(QWidget* parent)
{
	QAction *beforAct=actionPrintpreview;
	m_separatorAct=new QAction(parent);
	m_separatorAct->setSeparator(true);
	fileMenu->insertAction(beforAct,m_separatorAct);
	beforAct=m_separatorAct;
	for (int i = 0; i < MaxRecentFiles; ++i) {
        m_recentFileActs[i] = new QAction(parent);
        m_recentFileActs[i]->setVisible(false);
        connect(m_recentFileActs[i], SIGNAL(triggered()),
			this, SLOT(openRecentFile()));
		fileMenu->insertAction(beforAct,m_recentFileActs[i]);
		beforAct=m_recentFileActs[i];
    }
}

//////////////////////////////////////////////////////////////////////////
//mainwindow solt

void MainWindow::newFile()
{
	if (maybeSave())
	{
		m_editor->clear();
		m_editor->setPlainText(QString("proc procnameXXX\r\n{\r\n}\r\n"));
	    setCurrentFile("");
	}
}

void MainWindow::open()
{
	if (maybeSave()) {
		QString pathDir=getEnvVar()+QString("/procscript");
		QString fileName=QFileDialog::getOpenFileName(this,tr("�򿪽ű��ļ�"),pathDir,tr("�ű��ļ� (*.ups *.txt);;ȫ���ļ�(*.*)"));
		if (!fileName.isEmpty())
			loadFile(fileName);
    }
}

void MainWindow::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		loadFile(action->data().toString());
}


bool MainWindow::save()
{
	if (m_curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(m_curFile);
    }
}

bool MainWindow::saveAs()
{
	QString pathDir=getEnvVar()+QString("/procscript");
	QString fileName = QFileDialog::getSaveFileName(this,tr("���Ϊ�ű��ļ�"),pathDir,tr("�ű��ļ� (*.ups *.txt);;ȫ���ļ�(*.*)"));
    if (fileName.isEmpty())
        return false;
    return saveFile(fileName);
}

bool MainWindow::saveAll()
{
	return true;	
}

void MainWindow::exit()
{
	if(DMessageBox::question(this, tr("��ʾ"),tr("�Ƿ��˳�?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::No)
	{
		return;
	}
	qApp->closeAllWindows();
}

QString MainWindow::getEnvVar()
{
	QString nbenv;
	foreach(QString envVar,QProcess::systemEnvironment())
	{
		if(envVar.contains("NBENV",Qt::CaseInsensitive))
		{
			nbenv=envVar.right(envVar.length()-envVar.indexOf(C2G("="))-1);
			break;
		}
	}
	return nbenv;
}

void MainWindow::undo()
{
}

void MainWindow::redo()
{
}

void MainWindow::cut()
{
	
}

void MainWindow::copy()
{
	
}

void MainWindow::paste()
{
	
}

void MainWindow::about()
{
	
}

//////////////////////////////////////////////////////////////////////////




bool MainWindow::saveFile( const QString &fileName )
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        DMessageBox::warning(this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << m_editor->toPlainText();
    QApplication::restoreOverrideCursor();
	
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("%1�ļ��ѱ���").arg(fileName), 2000);
	file.close();
    return true;	
}


void MainWindow::loadFile( const QString& fileName )
{
	QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        DMessageBox::warning(this, tr("����ļ�"),tr("���ļ�ʧ�� %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_editor->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
	
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("�ļ���װ��"), 2000);
	file.close();
}


void MainWindow::setCurrentFile(const QString &fileName)
{
	m_curFile = fileName;
    if (m_curFile.isEmpty())
        setWindowTitle(tr("Recent Files"));
    else
        setWindowTitle(tr("%1 - %2").arg(strippedName(m_curFile))
		.arg(tr("Recent Files")));
// 	
//     QSettings settings("snr", "Recent UserScript");
//     QStringList files = settings.value("recentFileList").toStringList();
//     files.removeAll(fileName);
//     files.prepend(fileName);
//     while (files.size() > MaxRecentFiles)
//         files.removeLast();
// 	
//     settings.setValue("recentFileList", files);
// 	
//     foreach (QWidget *widget, QApplication::topLevelWidgets()) {
//         MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
//         if (mainWin)
//             mainWin->updateRecentFileActions();
//     }
}


void MainWindow::updateRecentFileActions()
{
    QSettings settings("snr", "Recent UserScript");
    QStringList files = settings.value("recentFileList").toStringList();
	
    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);
	
    for (int i = 0; i < numRecentFiles ; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        m_recentFileActs[numRecentFiles-i-1]->setText(text);
        m_recentFileActs[numRecentFiles-i-1]->setData(files[i]);
        m_recentFileActs[numRecentFiles-i-1]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        m_recentFileActs[j]->setVisible(false);
	
    m_separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

MainWindow *MainWindow::findMainWindow(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
	
    foreach (QWidget *widget, qApp->topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin && mainWin->m_curFile == canonicalFilePath)
            return mainWin;
    }
    return 0;
}

bool MainWindow::maybeSave()
{
	if (m_editor->document()->isModified()) {
		QMessageBox::StandardButton ret;
		ret = DMessageBox::warning(this, tr("�ļ�"),
			tr("�ĵ����޸�.\n"
			"���뱣��ı���?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
	if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::timerEvent( QTimerEvent *event )
{
	 //qDebug() << "Timer ID:" << event->timerId();
#ifdef DBG_REGPROC
	if(event->timerId()==procimerID)
	{
		procMan.CheckParentProc();
	}
#endif
	if(event->timerId()==mainTImerID)
	{
		return;
	}
	else if(event->timerId()==removeUserProcDelayID)
	{
		killTimer(removeUserProcDelayID);  
		//remove hand procscript
#ifdef HANDEXECPROC
		proc_hand_stop();
#endif
	}
}

void MainWindow::readSettings()
{
	
}

void MainWindow::writeSettings()
{
	
}

void MainWindow::compile()
{
	if (maybeSave()) {
		if (m_curFile.isEmpty()) 
		{
			saveAs();
		} 
		else
		{
			saveFile(m_curFile);
		}
		UserProc userproc;
		qDebug()<<m_curFile;
		if(m_curFile=="")
			return;
		actionViewOutput->setChecked(true);
		dockInfoTip->setVisible(true);
		//check userproc dir is exist if none create it
		QString pathDir=getEnvVar()+QString("/procscript/procscript");
		QDir dir(pathDir);
		if(!dir.exists())
		{
			dir.mkpath(pathDir);
		}
		appendOutput(QString(tr("��ʼ�����ļ�\"%1\" ")).arg(m_curFile));
		if(userproc.userprocfile(const_cast<char*>(G2C(m_curFile)))==USERPROCOK)
		{
			appendOutput(QString(tr("\"%1\"�ļ�����ɹ�!").arg(m_curFile)));
			return;
		}
		else
		{
			DMessageBox::information(this, tr("������ʾ"),tr("%1�ļ�����ʧ��!").arg(m_curFile));
		}
    }
}

//////////////////////////////////////////////////////////////////////////
/*
 *	
 */
void MainWindow::execProc()
{
	//int Pcexec::call_pcexecfile(char *pcexecfname,double *resultaddr)
	QString pathDir=getEnvVar()+QString("/procscript/procscript");
	QStringList fileNames=QFileDialog::getOpenFileNames(this,tr("�򿪽ű�ִ���ļ�"),pathDir,tr("�ű�ִ���ļ� (*.upo)"));
	foreach(QString filename,fileNames)
	{
		QFileInfo fileInf(filename);
		QString basename=fileInf.baseName();
		qDebug()<<basename;
		if (!basename.isEmpty() && fileInf.isFile())
		{
			//proc_formula_exec(basename);
			proc_hand_exec(basename);
		}
	}
}

void MainWindow::proc_hand_exec( const QString &execfile )
{	
	RtdbProc rtdbproc;
	QString sqlword;
	QStringList context;
	bool bSucc;
	
	sqlword=QString(C2G("select * from �û����̲����� where �û�������='%1'"))
		.arg(execfile);
	qDebug()<<sqlword;
	//exist the execfile
	if(dbex.sqlReadRecorderDir(DB_NAME,sqlword,m_existUserProcRec))
	{
		m_insRecSerial=m_existUserProcRec[0].toInt();
		//update it
		sqlword=QString(C2G("update �û����̲����� set ִ��ģʽ=1,�������=0,�����=%1,ʹ�ñ�־=1 where �û�������='%2'"))
			.arg(HANDPROC_INTERVAL)
			.arg(execfile);
		qDebug()<<sqlword;
		bSucc=dbex.Execute(DB_NAME,sqlword);
	}
	else
	{
		sqlword=QString(C2G("select max(���) from �û����̲�����"));
		bSucc=dbex.sqlReadRecorderDir(DB_NAME,sqlword,context);
		if(bSucc && context.size()==1)
			m_insRecSerial=context[0].toInt()+1;
		else
			m_insRecSerial=1;
		//insert it
		sqlword=QString(C2G("insert into �û����̲����� (���,�������,�û�������,ִ��ģʽ,�������,�����,��ʱ��,��ʱ��, ��ʱСʱ,��ʱ����,ʹ�ñ�־) values (%1,1,'%2',1,0,%3,0,0,0,0,1);"))
 			.arg(m_insRecSerial)
 			.arg(execfile)
 			.arg(HANDPROC_INTERVAL);
		qDebug()<<sqlword;
		bSucc=dbex.Execute(DB_NAME,sqlword);
	}
	if(bSucc)
	{
		actionViewOutput->setChecked(true);
		dockInfoTip->setVisible(true);
		appendOutput(QString(C2G("��ʼ�ֶ�ִ��[%1]�û����̣�")).arg(execfile));
		//reload rtdb
		int nResult;
		nResult=rtdbproc.ReloadMode(Rdb_UserName,Rdb_Password,0);
		nResult=rtdbproc.Recall(Rdb_UserName,Rdb_Password,Rdb_Server,Rdb_Name);
		//timing proc
		removeUserProcDelayID=startTimer(HANDPROC_REMOVEDELAY*1000);
	}
	return;
}

void MainWindow::proc_hand_stop()
{
	RtdbProc rtdbproc;
	QStringList fieldname;
	QString sqlword;
	bool bSucc;
	//remove it
	
	qDebug()<<sqlword;

	
	//insert old rec
	if(m_existUserProcRec.size()!=0)
	{
		fieldname.append(C2G("*"));
		sqlword=QString(C2G("update �û����̲����� set ִ��ģʽ=%1,�������=%2,�����=%3,ʹ�ñ�־=%4 where �û�������='%5'"))
			.arg(m_existUserProcRec[3])
			.arg(m_existUserProcRec[4])
			.arg(m_existUserProcRec[5])
			.arg(m_existUserProcRec[23])
			.arg(m_existUserProcRec[2]);
		qDebug()<<sqlword;
		bSucc=dbex.Execute(DB_NAME,sqlword);
		//bSucc=dbex.InsertRecords(DB_NAME,C2G("�û����̲�����"),fieldname,m_existUserProcRec);
	}
	else
	{
		sqlword=QString(C2G("delete from �û����̲����� where ���= %1;")).arg(m_insRecSerial);
		bSucc=dbex.Execute(DB_NAME,sqlword);
	}
	
	//reload rtdb
	actionViewOutput->setChecked(true);
	dockInfoTip->setVisible(true);
	appendOutput(QString(C2G("�����ֶ�ִ���û����̣�")));
	int nResult;
	nResult=rtdbproc.ReloadMode(Rdb_UserName,Rdb_Password,0);
	nResult=rtdbproc.Recall(Rdb_UserName,Rdb_Password,Rdb_Server,Rdb_Name);
}


uint MainWindow::proc_formula_exec(QString& execfile)
{
	USERPROCPARA_STRU   *calp;
	Rdb_Tab             calcParaTab;

	bool				bfind;
	//Rdb_SqlCli      rdbSQL;
	int					ret,execReturnCode=0;
	
	if((ret=calcParaTab.RdbLogin(MyUserName,MyPassWord))!=RDB_OK)
	{
	   return execReturnCode;
	}
	if((ret=calcParaTab.RdbOpenTab(USERPROC_TABNAME))!=RDB_OK)
	{
	   return execReturnCode;
	}
	
	bfind=false;
	for(calp=(USERPROCPARA_STRU*)calcParaTab.rdbgetfirstrcdaddr();
	calp!=NULL;calp=(USERPROCPARA_STRU *)calcParaTab.rdbgetnextrcdaddr()) {
		QString procname=QString(calp->callprocname);
		qDebug()<<procname;
		qDebug()<<execfile;
		if(execfile.compare(procname)==0)
		{
			bfind=true;
			break;
		}
	}

	if(bfind)
	{
		execReturnCode=ExecOneProc(DefaultGroupNo,calp);
	}
	
	calcParaTab.RdbCloseTab();
	return execReturnCode;
}

int MainWindow::ExecOneProc( int groupno,USERPROCPARA_STRU *calp )
{
	//Userpcexec  pce("",MyUserName,MyPassWord,0);
	
	Userpcexec *pce=new Userpcexec("",Rdb_UserName,Rdb_Password,0);

	static int  HDBTIMER_EXEC_MODE=GetProgramRunPara("hdtexemode",0);
	GetInterTime(&calp->lastproctime);
	
	if(HDBTIMER_EXEC_MODE==0 && calp->startmode==CALSTMODE_HDBACCURATE) {
		if(!IsMainServer(groupno)) {
			calp->returncode=0;
			return(calp->returncode);
		}
	}
	
	
	int  i,dataargno=0;
	double inval[16];
	
	for(i=0;i<CALINPARANUMS;i++) {
		if(calp->inargv[i][0]=='\0') break;
		if(isdigit(calp->inargv[i][0])) {
			inval[dataargno]=atof(calp->inargv[i]);
			pce->push_inpara(doubledata,&inval[dataargno]);
			dataargno++;
		}
		else if(calp->inargv[i][0]=='+' || calp->inargv[i][0]=='-')	{
			if(isdigit(calp->inargv[i][1])) {
				inval[dataargno]=atof(calp->inargv[i]);
				pce->push_inpara(doubledata,&inval[dataargno]);
				dataargno++;
			}
			else {
				pce->push_inpara(stringdata,calp->inargv[i]);
			}
		}
		else if(iswalpha(calp->inargv[i][0]) || ishanzhi(calp->inargv[i][0])) {
			pce->push_inpara(stringdata,calp->inargv[i]);
		}
		else  break;
	}
	
	calp->returncode=pce->call_pcexecfile(calp->callprocname,&calp->returnvalue);
	//	printf("\ncalp->callprocname=%s calp->returncode=%d\n",calp->callprocname,calp->returncode);
	delete pce;
	return(calp->returncode);
}

int MainWindow::IsMainServer(int groupno)
{
	NetStatus  netstat;
	int ret=netstat.IsMainServer(groupno,SCD_SERVER);
	if(ret)	return 1;
	return(0);
}

//////////////////////////////////////////////////////////////////////////


void MainWindow::viewToolBar(bool checked)
{
	
}

void MainWindow::viewStatusBar(bool checked)
{
	
}

void MainWindow::viewOutput( bool checked )
{
	dockInfoTip->setVisible(checked);
}


void MainWindow::insertFunc()
{
	
}

void MainWindow::insertSCDFunc()
{
	rdbParaDlg=new RdbParaDialog(this);
	rdbParaDlg->show();
}

void MainWindow::insertCustomFunc()
{
	
}

void MainWindow::deleteCustomFunc()
{
	
}

void MainWindow::editCustomFunc()
{
	
}


void MainWindow::setPrintPreviewOptions( QPrinter* printer )
{
	if (!printer)
		return;
	print_paper_size = printer->paperSize();
	printer_orientation = printer->orientation();
}



void MainWindow::printPreview()
{
	QPrinter p;
	p.setPaperSize(print_paper_size);
	p.setOrientation(printer_orientation);
	
	QPrintPreviewDialog *preview = new QPrintPreviewDialog(&p, this, Qt::Window);
	preview->setWindowTitle(MAINWINDOWTITLE + " - " + tr("��ӡԤ������: "));
	connect(preview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(setPrintPreviewOptions(QPrinter *)));
	connect(preview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(print(QPrinter *)));
	preview->exec();
}

void MainWindow::print( QPrinter* printer/*=NULL*/ )
{
	if(printer==NULL)
	{
		QPrinter p;
		p.setPaperSize(print_paper_size);
		p.setOrientation(printer_orientation);
		m_editor->print(&p);
	}
	else
	{
		m_editor->print(printer);
	}
	return;
}

void MainWindow::appendOutput( QString outText,bool additionDate/*=true*/ )
{
	QString  info;
	if(additionDate)
	{
		info=QString("%1 %2:%3")
			.arg(QDate::currentDate().toString("yyyy-MM-dd"))
			.arg(QTime::currentTime().toString("hh:mm:ss"))
		.arg(outText);
	}
	else
	{
		info=QString("%3\r\n")			
		.arg(outText);
	}
	compileOut->append(info);
}


void MainWindow::insertText( const QString& text )
{
	m_editor->textCursor().insertText(text);
}


MainWindow* MainWindow::mainWindow()
{
    foreach (QWidget *widget, qApp->topLevelWidgets()) {
		if(widget->inherits("QMainWindow"))
		{
			return  qobject_cast<MainWindow *>(widget);
		}
    }
    return NULL;
}

void MainWindow::initStyleSheet()
{
	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QWidget\
						   {\
						   background: #FAFAFA;\
						   }\
						   QToolBar\
						   {\
						   spacing:10px;\
						   min-height:35px;\
						   background:#FAFAFA;\
						   }\
						   QToolButton\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   min-height:25px;\
						   }\
						   QToolButton:checked\
						   {\
						   background: #FFFFFF;\
						   border: 1px solid #4A90E2;\
						   }\
						   QTableWidget \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 12px;\
						   color:#4A4A4A;\
						   background: #FAFAFA;\
						   border: 1px solid #E5E5E5;\
						   }\
						   QTableWidget::item\
						   {\
						   text-align:center;\
						   }\
						   QScrollBar\
						   {\
						   background:white;\
						   height:8px;\
						   }\
						   QScrollBar::handle\
						   {\
						   background:lightgray;\
						   border:2px solid transparent;\
						   border-radius:4px;\
						   }\
						   QScrollBar::handle:hover\
						   {\
						   background:gray;\
						   }\
						   QScrollBar::sub-line\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar::add-line\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar:vertical\
						   {\
						   width:8px;\
						   background:white;\
						   padding-top:9px;\
						   padding-bottom:9px;\
						   }\
						   QScrollBar::handle:vertical\
						   {\
						   background:lightgray; \
						   width:8px;\
						   min-height:20;\
						   border:2px solid transparent; \
						   border-radius:4px;\
						   }\
						   QScrollBar::handle:vertical:hover\
						   {\
						   background:gray;\
						   }\
						   QScrollBar::sub-line:vertical\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar::add-line:vertical\
						   {\
						   background:transparent;\
						   }\
						   QPushButton\
						   {\
						   background:#4A90E2;\
						   border-radius: 2px;\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: white;\
						   min-width:75px;\
						   min-height:25px;\
						   }\
						   QPushButton:hover\
						   {\
						   background:#0C2543;\
						   }\
						   QCheckBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QCheckBox::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QCheckBox::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
						   }\
						   QComboBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QComboBox::drop-down\
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: top right;width: 28px;\
						   border-top-right-radius: 2px;\
						   border-bottom-right-radius: 2px;\
						   }\
						   QComboBox::down-arrow\
						   {\
						   image: url(:/res/drop_down.png);\
						   }\
						   QListView,QTreeView\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   border: 1px solid #E5E5E5;\
						   }\
						   QListView::item:hover,QTreeView::item:hover \
						   {\
						   background-color: wheat;\
						   }\
						   QListView::item:selected,QTreeView::item:selected \
						   {\
						   background-color: lightblue;\
						   }\
						   QListView::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QListView::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
						   }\
						   QSpinBox \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QSpinBox::up-button \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: right;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QSpinBox::up-arrow \
						   {\
						   image: url(:/res/spinup.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QSpinBox::down-button \
						   {\
						   subcontrol-origin: border;\
						   subcontrol-position: left;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QSpinBox::down-arrow \
						   {\
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QDateTimeEdit \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QDateTimeEdit::drop-down \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: top right;width: 28px;\
						   border-top-right-radius: 2px;\
						   border-bottom-right-radius: 2px;\
						   }\
						   QDateTimeEdit::down-arrow\
						   {\
						   image: url(:/res/drop_down.png);\
						   }\
						   QToolButton#qt_calendar_prevmonth,QToolButton#qt_calendar_nextmonth,QToolButton#qt_calendar_monthbutton,QToolButton#qt_calendar_yearbutton\
						   {\
						   border:0px solid #000000;\
						   border-radius:3px;\
						   margin:3px 3px 3px 3px;\
						   padding:3px;\
						   background-color:rgba(0,0,0,0);\
						   color:#FFFFFF;\
						   }\
						   QToolButton#qt_calendar_prevmonth:hover,QToolButton#qt_calendar_nextmonth:hover,QToolButton#qt_calendar_monthbutton:hover,QToolButton#qt_calendar_yearbutton:hover,QToolButton#qt_calendar_prevmonth:pressed,QToolButton#qt_calendar_nextmonth:pressed,QToolButton#qt_calendar_monthbutton:pressed,QToolButton#qt_calendar_yearbutton:pressed\
						   {\
						   border:1px solid #D8D8D8;\
						   }\
						   QCalendarWidget QSpinBox#qt_calendar_yearedit\
						   {\
						   margin:3px 3px 3px 3px;\
						   padding:0px 0px 0px 0px;\
						   font-family: Microsoft YaHei;\
						   font-size: 9px;\
						   background-color:rgba(0,0,0,0);\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 25px;\
						   max-height: 25px;\
						   min-width:70px;\
						   max-width:70px;\
						   }\
						   QCalendarWidget QToolButton::menu-indicator\
						   {\
						   image:None;\
						   }\
						   QCalendarWidget QTableView\
						   {\
						   border-width:0px;\
						   }\
						   QCalendarWidget QWidget#qt_calendar_navigationbar\
						   {\
						   border:1px solid #575757;\
						   border-width:1px 1px 0px 1px;\
						   background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #4D4D4D,stop:1 #292929);\
						   }\
						   QTabBar::tab\
						   {\
						   color:#0F1923;\
						   font-family:Microsoft YaHei;\
						   font-size: 13px;\
						   min-width:100px;\
						   min-height:30px;\
						   border:0px;\
						   }\
						   QTabBar::tab:hover\
						   {\
						   background:rgb(255,255, 255, 100);\
						   }\
						   QTabBar::tab:selected\
						   {\
						   border-color:white;\
						   background:white;\
						   color:#5D9CE5;\
						   border-bottom: 2px solid #4A90E2;\
						   }\
						   QLineEdit\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   background: #FFFFFF;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 25px;\
						   max-height: 25px;\
						   }\
						   QRadioButton::indicator:unchecked\
						   {\
						   image:url(:/res/radio.png);\
						   }\
						   QRadioButton::indicator:checked\
						   {\
						   image:url(:/res/radio_checked.png);\
						   }\
						   QLabel\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#3E464E;\
						   min-height: 25px;\
						   max-height: 25px;\
						   }\
						   QGroupBox,QRadioButton,QDockWidget\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#3E464E;\
						   }\
						   QMenuBar\
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:14px;\
						   color:#3E464E;\
						   background: #FAFAFA;\
						   min-width:60px;\
						   min-height:32px;\
						   border:1px solid #D5D5D5;\
						   }\
						   QMenuBar::item:selected\
						   {\
						   background:#4A90E2;\
						   color:white;\
						   }\
						   QMenu\
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color:black;\
						   background-color:#FAFAFA;\
						   }\
						   QMenu::item:selected\
						   {\
						   background:#4A90E2;\
						   color:white;\
						   }\
						   QTimeEdit \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QTimeEdit::up-button \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: right;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QTimeEdit::up-arrow \
						   {\
						   image: url(:/res/spinup.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QTimeEdit::down-button \
						   {\
						   subcontrol-origin: border;\
						   subcontrol-position: left;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QTimeEdit::down-arrow \
						   {\
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QHeaderView::section \
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color:#0F1923;\
						   min-width:50px;\
						   height:30px;\
						   background:#F1F1F1;\
						   border: 0px solid #567dbc;\
						   border-right:1px solid #B5B5B5;\
						   border-bottom:1px solid #B5B5B5;\
						   }\
						   ") ;

	setStyleSheet( buttonStyleSheet ) ;
}