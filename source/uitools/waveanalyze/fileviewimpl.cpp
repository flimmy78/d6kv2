#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileInfoList>

#include "fileviewimpl.h"
#include "funutils.h"
#include "fileoperate.h"
#include "mainformimpl.h"
#include "globaldef.h"

extern QWidget* getMainWnd();

QDlgFileViewImpl::QDlgFileViewImpl( QWidget* parent,Qt::WindowFlags f) 
	: QDialog(parent, f)
{
	setupUi(this) ;

	create_actions();
	InitControl();
// 	m_lstFile = new FileList;
}

QDlgFileViewImpl::~QDlgFileViewImpl()
{
// 	 QListIterator<FILEITEM> it(*m_lstFile);
// 	FILEITEM* fi;
// 	while ((fi = it.current()) != 0)
// 	{
// 		++it;
// 		delete fi->file;
// 		delete fi;
// 	}
// 	delete m_lstFile;
	QList<FILEITEM*>::iterator it;
	for (it=m_lstFile.begin();it!=m_lstFile.end();++it)
	{
		delete (*it)->file;
		delete (*it);
	}
	m_lstFile.clear();
}

void QDlgFileViewImpl::create_actions()
{
	action_open = new QAction(_C2Q("��"),this);
	action_open->setObjectName(QString::fromUtf8("action_open"));
	connect(action_open,SIGNAL(activated()),this,SLOT(slot_open()));

	action_open_newform = new QAction(_C2Q("���´����д�"),this);
	action_open_newform->setObjectName(QString::fromUtf8("action_open_newform"));
	connect(action_open_newform,SIGNAL(activated()),this,SLOT(slot_open_newform()));

	action_delete = new QAction(_C2Q("ɾ��"),this);
	action_delete->setObjectName(QString::fromUtf8("action_delete"));
	connect(action_delete,SIGNAL(activated()),this,SLOT(slot_delete()));

	action_delete_all = new QAction(_C2Q("ȫ��ɾ��"),this);
	action_delete_all->setObjectName(QString::fromUtf8("action_delete_all"));
	connect(action_delete_all,SIGNAL(activated()),this,SLOT(slot_delete_all()));
}

void QDlgFileViewImpl::InitControl()
{
	QString path = FILEPATH;
	InitListView(m_lvDevice);
	new QDevItem(m_lvDevice, NULL, path);
	connect(m_lvDevice, SIGNAL(selectionChanged(QTreeWidgetItem*)), 
		SLOT(DevSelectionChange(QTreeWidgetItem*)));	
	
	InitListView(m_lvDir);
	new QDirItem(m_lvDir, NULL, path);
	connect(m_lvDir, SIGNAL(selectionChanged(QTreeWidgetItem*)), 
		SLOT(DirSelectionChange(QTreeWidgetItem*)));

	connect(m_lvFile, SIGNAL(contentsMoving(int, int)), 
		SLOT(FileViewMove(int, int)));
	connect(m_lvFile, SIGNAL(customContextMenuRequested( const QPoint&)), this,
		SLOT(FileItemClick(const QPoint&)));
}

FILEITEM* QDlgFileViewImpl::GetItem(const QString& path)
{
	FILEITEM* file = NULL;
	char* p;
	strcpy(p, _Q2C(path));
// 	 QListIterator<FILEITEM> it(*m_lstFile);
// 	FILEITEM* fi;
// 	while ((fi = it.current()) != 0)
// 	{
// 		++it;
// 		if ((int)strlen(p) == fi->length && memcmp(p, fi->file, fi->length) == 0)
// 		{
// 			file = fi;
// 			break;
// 		}
// 	}
	QList<FILEITEM*>::iterator it;
	for (it=m_lstFile.begin();it!=m_lstFile.end();++it)
	{
		if ((int)strlen(p) == (*it)->length && memcmp(p, (*it)->file,(*it)->length) == 0)
		{
			file = (*it);
			break;
		}
	}
	return file;
}

// ���������ļ�Item���������п���Item����Ϣ
void QDlgFileViewImpl::FileViewMove(int x, int y)
{
	const char* FAULTSTATE[] =
	{  "�޹���", "����" };
	QTreeWidgetItem* item ;
	Comtrade c;
	File_operate fo(&c);
	QRect rct = m_lvFile->geometry();
	while (item != NULL)
	for (int i=0;i<m_lvFile->topLevelItemCount();i++)
	{
		item = m_lvFile->topLevelItem(i);

		QFileItem* fitem = (QFileItem*)item;
		char* p ;
		strcpy(p, _Q2C(fitem->Path()));

		FILEITEM* fi = GetItem(fitem->Path());
		if (fi == NULL)
		{
// 			QRect itemRct = m_lvFile->itemRect(item);
			QRect itemRct = m_lvFile->visualItemRect(item);	//��ȡitem��rectangle
			itemRct.adjust(1, 1, -1, -1);
			// ��������������ļ���Ϣ
			if (itemRct.isValid() && rct.contains(itemRct, true))	
			{
				// ����������Ϣ
				if (fo.load_config(fitem->Path()))
				{
					fi = new FILEITEM;
					fi->fault = c.IsFault();
					const char* p = fitem->Path().toLatin1();
					fi->length = strlen(p);
					// ע�⣺���һ���ֽڷŽ�����
					fi->file = new char[fi->length+1];
					strcpy(fi->file, p);
					fi->time = c.start_time();
					m_lstFile.append(fi);
				}
			}
		}
		if (fi != NULL)
		{
			item->setText(1, fi->time);
			int n = (int)fi->fault;
			item->setText(4, tr(FAULTSTATE[n]));
		}
	}
}

void QDlgFileViewImpl::FileItemClick( const QPoint& pos )
{
	QMenu menu(this);
// 	int ID_OPEN = menu.insertItem(tr("��(&O)"));
// 	int ID_NEWFORM = menu.insertItem(tr("���´����д�(&N)"));
// 	menu.insertSeparator();
// 	int ID_DELETE = menu.insertItem(tr("ɾ��(&D)"));
// 	int ID_DELALL = menu.insertItem(tr("ȫ��ɾ��"));
	
	menu.addAction(action_open);
	menu.addAction(action_open_newform);
	menu.addSeparator();
	menu.addAction(action_delete);
	menu.addAction(action_delete_all);

	int count = 0;
//     QListViewItemIterator it(m_lvFile);
//     while (it.current() != NULL)
// 	{
//         if (it.current()->isSelected())
// 		{
// 			count++;
// 			if (count >= 2)
// 				break;
// 		}
//         ++it;
//     }
	QTreeWidgetItem *item;
	for (int i=0;i<m_lvFile->topLevelItemCount();i++)
	{
		item = m_lvFile->topLevelItem(i);
		if (item->isSelected())
		{
			count++;
			if (count >=2)
			{
				break;
			}
		}
	}
	bool b1 = count == 1;
	bool b2 = count > 0;
	m_current_item = (QFileItem*)m_lvFile->itemAt(pos);

	action_open->setEnabled(b1);
	action_open_newform->setEnabled(b1);
	action_delete->setEnabled(b2);
	action_delete_all->setEnabled(b2);
	menu.exec(QCursor::pos());


/*	QMainFormImpl* main = (QMainFormImpl*)qApp->mainWidget();
	QFileItem* i = (QFileItem*)item;
	if (id == ID_OPEN)			// ���ļ�
	{
		main->OpenFile(i->Path());
		close();
	}
	else if (id == ID_NEWFORM)	// ���´����д��ļ�
	{
		main->NewFile(i->Path());
		close();
	}
	else if (id == ID_DELETE)	// ɾ��ѡ���ļ�
	{
		DeleteFiles();
	}
	else if (id == ID_DELALL)	// ɾ�������ļ�
	{
		DeleteFiles(true);
	}*/
}
	
void QDlgFileViewImpl::DeleteFiles(bool all)
{
	const QString DELETESTR[] = 
	{ 
		_C2Q("ȷ��Ҫɾ��ѡ��������?"),
		_C2Q("ȷ��Ҫɾ������������?")
	};
	int n = (int)all;
	if (QMessageBox::question(this, _C2Q("���η�������"), DELETESTR[n], 
		QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			return;
  
// 	QListViewItemIterator it(m_lvFile);
//     QListViewItem* item = NULL;
// 	while ((item = it.current()) != NULL)
// 	{
//         ++it;
// 		if (all || item->isSelected())
// 		{
// 			QFileItem* i = (QFileItem*)item;
// 			QFile::remove(i->Path());
// 			QString file = ChangeFileExt(i->Path(), ".dat");
// 			QFile::remove(file);
// 			m_lvFile->takeItem(item);
// 		}
//     }
	QTreeWidgetItem* item;
	for (int i=0;i<m_lvFile->topLevelItemCount();i++)
	{
		item = m_lvFile->topLevelItem(i);
		if (all || item->isSelected())
		{
			QFileItem* fileitem = (QFileItem*)item;
			QFile::remove(fileitem->Path());
			QString file = change_file_ext(fileitem->Path(),".dat");
			QFile::remove(file);
			m_lvFile->takeTopLevelItem(i);
		}
	}
}

void QDlgFileViewImpl::InitListView(QTreeWidget* lv)
{
// 	lv->addColumn("", 200);
// 	lv->setColumnWidthMode(0, QListView::Maximum);
	lv->setColumnCount(1);
	lv->setColumnWidth(0,200);
	lv->header()->hide();
	lv->setSortingEnabled(false);

}

void QDlgFileViewImpl::DirSelectionChange(QTreeWidgetItem* i)
{
	m_lvFile->clear();
	TravelDirItem((QDirItem*)i);
	FileViewMove(0, 0);
}

// ����Ƿ���ƥ��cfg�ļ���dat�ļ�
bool QDlgFileViewImpl::HasDatFile(const QString& cfg)
{
	return QFile::exists(change_file_ext(cfg, ".dat"));
}

void QDlgFileViewImpl::TravelDirItem(QDirItem* it, bool all)
{
	QDirItem* item = it;

	if (item != NULL)
	{
		if (all)
		{
			QDirItem* parentitem = NULL;
			parentitem = (QDirItem*)(item->parent());
			if (parentitem !=NULL)
			{
				for(int i=0;i<parentitem->childCount();i++)
				{
					item = (QDirItem*)(parentitem->child(i));
					QDir dir(item->Path());
					QStringList filters;
					filters << "*.cfg";
					QFileInfoList files = dir.entryInfoList(filters);
					for (int j=0;j<files.size();j++)
					{
						QFileInfo file_info = files.at(j);
						QFileItem* fileItem = NULL;
						if (file_info.isFile() && HasDatFile(file_info.filePath()))
						{
							fileItem = new QFileItem(m_lvFile,fileItem,file_info.filePath(),
								item->Device(),item->Sector(),&file_info);
						}
					}
					if (item->childCount() > 0)
					{
						TravelDirItem((QDirItem*)item->child(0), true);
					}
				}
			}		
		}
		else
		{
			QDir dir(item->Path());
			QStringList filters;
			filters << "*.cfg";
			QFileInfoList files = dir.entryInfoList(filters);
			for (int j=0;j<files.size();j++)
			{
				QFileInfo file_info = files.at(j);
				QFileItem* fileItem = NULL;
				if (file_info.isFile() && HasDatFile(file_info.filePath()))
				{
					fileItem = new QFileItem(m_lvFile,fileItem,file_info.filePath(),
						item->Device(),item->Sector(),&file_info);
				}
			}
			if (item->childCount() > 0)
			{
				TravelDirItem((QDirItem*)item->child(0), true);
			}
		}
	}
}

void QDlgFileViewImpl::DevSelectionChange(QTreeWidgetItem* item)
{
	m_lvFile->clear();
	if (item->childCount() > 0)
	{
		for (int i=0;i<item->childCount();i++)
		{
			QTreeWidgetItem* item = item->child(i);
			ListDevices(item);
		}
	}
	else
	{
		ListDevices(item);
	}
	
	FileViewMove(0, 0);
}

void QDlgFileViewImpl::ListDevices(QTreeWidgetItem* i)
{
	QString dev = ((QDevItem*)i)->Device();
	QString path = FILEPATH;
	FILEINFO fo;
	fo.device = "";
	fo.sector = "";
	fo.year = 0;
	TravelFileDir(path, dev, 0);
}

void QDlgFileViewImpl::TravelFileDir(const QString& path, const QString& dev, int depth)
{
	QDir dir(path);
	QFileInfoList filelist = dir.entryInfoList();
	for (int i=0;i<filelist.size();i++)
	{
		QFileInfo file = filelist.at(i);
		if (file.fileName()=="." || file.fileName() == "..")
		{
			continue;
		}
		if (file.isDir())
		{
			TravelFileDir(file.absoluteFilePath(),dev,++depth);
			--depth;
		}
		else if (file.isFile())
		{
			if (file.absoluteFilePath().indexOf(dev) >= 0 &&
				file.completeSuffix().toLower()=="cfg" && 
				HasDatFile(file.filePath()))
			{
				FILEINFO fo = GetFileInfo(file.absoluteFilePath(),depth);
				int childcount = m_lvFile->topLevelItemCount();
				QFileItem* fileItem = new QFileItem(m_lvFile, m_lvFile->takeTopLevelItem(childcount-1), 
					file.filePath(), fo.device, fo.sector, &file);
				fileItem->setSelected(false);
			}
		}
	}

/*	if (filelist) 
	{
		QFileInfoListIterator it(*filelist);
		QFileInfo* fi;
		while((fi = it.current()) != 0) 
		{
			++it;
			if (fi->fileName() == "." || fi->fileName() == "..")
				continue;
			if (fi->isDir())
			{
				TravelFileDir(fi->absFilePath(), dev, ++depth);
				--depth;
			}
			else if (fi->isFile())
			{
				if (fi->absFilePath().find(dev) >= 0 && 
					fi->extension(false).lower() == "cfg" && 
					HasDatFile(fi->filePath()))
				{
					FILEINFO fo = GetFileInfo(fi->absFilePath(), depth);
					QFileItem* fileItem = new QFileItem(m_lvFile, m_lvFile->lastItem(), 
						fi->filePath(), fo.device, fo.sector, fi);
					fileItem->setCheckState()
				}
			}
		}
	}*/
}

// ��·����·����λ���ļ���Ϣ
FILEINFO QDlgFileViewImpl::GetFileInfo(const QString& path, int depth)
{
	// recordfile(0)\2005(1)\WBH801A(2)\sector1(3)
	QString str = path.right(path.length() - path.indexOf("recordfile"));
// 	const char* p = str.ascii();
	char* p ;
	strcpy(p,_Q2C(str));
	FILEINFO fo;
	char ch = '/';
	if (depth > 0)
		fo.year = cut_char(str, 1, ch).toInt();
	if (depth > 1)
		fo.device = cut_char(str, 2, ch);
	if (depth > 2)
		fo.sector = cut_char(str, 3, ch);
	return fo;
}

void QDlgFileViewImpl::slot_open()
{
// 	QMainFormImpl* main = (QMainFormImpl*)qApp->mainWidget();
	Main_form_impl* main = (Main_form_impl*)getMainWnd();
	main->open_comtrade_file(m_current_item->Path());
	close();
}

void QDlgFileViewImpl::slot_open_newform()
{
// 	QMainFormImpl* main = (QMainFormImpl*)qApp->mainWidget();
	Main_form_impl* main = (Main_form_impl*)getMainWnd();
	main->new_comtrade_file(m_current_item->Path());
	close();
}

void QDlgFileViewImpl::slot_delete()
{
	DeleteFiles();
}

void QDlgFileViewImpl::slot_delete_all()
{
	DeleteFiles(true);
}


/*=== QFileItem ===*/

QFileItem::QFileItem(QTreeWidgetItem* parent, QTreeWidgetItem* after, const QString& path)
	: QTreeWidgetItem(parent, after)
{
	m_strPath = path;
	Init();
}
	
QFileItem::QFileItem(QTreeWidget* parent, QTreeWidgetItem* after, const QString& path)
	: QTreeWidgetItem(parent, after)
{
	m_strPath = path;
	Init();
}

QFileItem::QFileItem(QTreeWidget* parent, QTreeWidgetItem* after, const QString& path, 
		const QString& dev, const QString& sec, QFileInfo* fi)
	: QTreeWidgetItem(parent, after)
{
	m_strPath = path;
	m_strDevice = dev;
	m_strSector = sec;

	/* ���������״̬ͨ�����ļ������	*/
	setText(0, fi->baseName());							// �ļ���
	setText(2, m_strDevice);				// װ��
	setText(3, m_strSector);				// ����
	setText(5, QString::number(fi->size()) + "bytes");	// �ļ���С
}
	
QFileItem::~QFileItem()
{
}

void QFileItem::Init()
{
	m_nYear = 0;
	m_strDevice = "";
	m_strSector = "";
}

void QFileItem::SetDirText()
{
	setText(0, relative_path(Path()));
}


/*=== Ŀ¼ ===*/
	
QDirItem::QDirItem(QTreeWidgetItem* parent, QTreeWidgetItem* after, QString path)
	: QFileItem(parent, after, path)
{
	SetDirText();
	
	QFileItem* p = (QFileItem*)parent;
	if (depth() == 1)		// ���
	{
		SetYear(text(0).toInt());
	}
	else if (depth() == 2)	// װ��
	{
		SetYear(p->Year());
		SetDevice(text(0));
	}
	else if (depth() == 3)	// ����
	{
		SetYear(p->Year());
		SetDevice(p->Device());
		SetSector(text(0));
	}

	CreateChild();
}
	
QDirItem::QDirItem(QTreeWidget* parent, QTreeWidgetItem* after, QString path)
	: QFileItem(parent, after, path)
{
	SetDirText();
	CreateChild();
}

void QDirItem::CreateChild()
{
	QDir dir(Path());
	QFileInfoList filelist = dir.entryInfoList();
	for (int i=0; i<filelist.size();i++)
	{
		QFileInfo fileinfo = filelist.at(i);
		QDirItem* item = NULL;
		if (fileinfo.fileName()=="." || fileinfo.fileName()=="..")
		{
			continue;
		}
		if (fileinfo.isDir())
		{
			item = new QDirItem(this,item,fileinfo.absoluteFilePath());
		}
	}
	setExpanded(true);
	/*
	if (filelist) 
	{
	    QFileInfoListIterator it(*filelist);
	    QFileInfo* fi;
		QDirItem* item = NULL;
	    while((fi = it.current()) != 0) 
		{
			++it;
			if (fi->fileName() == "." || fi->fileName() == "..")
				continue;
			if (fi->isDir())
			{
				item = new QDirItem(this, item, fi->absFilePath());
			}
		}
	}
	setOpen(true);*/
}

QDirItem::~QDirItem()
{
}

int QDirItem::depth()
{
	QTreeWidgetItem* parentItem = NULL;
	int depth = 1;
	parentItem = parent();
	while(parentItem != NULL)
	{
		depth++;
		parentItem = parentItem->parent();
	}
	return depth;
}


/*=== QDevItem ===*/

QDevItem::QDevItem(QTreeWidgetItem* parent, QTreeWidgetItem* after, QString path)
	: QFileItem(parent, after, path)
{
	SetDirText();
	SetDevice(text(0));
}

QDevItem::QDevItem(QTreeWidget* parent, QTreeWidgetItem* after, QString path)
	: QFileItem(parent, after, path)
{
	setText(0, QObject::tr("¼��װ��"));
	QDir dir(path);
	QFileInfoList filelist = dir.entryInfoList();
	for (int i=0;i<filelist.size();i++)
	{
		QFileInfo fileinfo = filelist.at(i);
		if (fileinfo.fileName() == "." || fileinfo.fileName() == "..")
		{
			continue;
		}
		if (fileinfo.isDir())
		{
			QDir devDir(fileinfo.absoluteFilePath());
			QFileInfoList devFilesList = devDir.entryInfoList();
			for (int j=0;j<devFilesList.size();j++)
			{
				QDevItem* item = NULL;
				QFileInfo devFileInfo = devFilesList.at(j);
				if (devFileInfo.fileName() == "." || devFileInfo.fileName() == "..")
					continue;
				if (devFileInfo.isDir())
				{
					if (HasDevice(devFileInfo.absoluteFilePath()))
						continue;
					item = new QDevItem(this, item, devFileInfo.absoluteFilePath());
				}
			}
		}
	}
/*	if (filelist) 
	{
	    QFileInfoListIterator it(*filelist);
	    QFileInfo* fi;
	    while ((fi = it.current()) != 0) 
		{
			++it;
			if (fi->fileName() == "." || fi->fileName() == "..")
				continue;
			if (fi->isDir())
			{
				QDir devDir(fi->absFilePath());
				const QFileInfoList* devFiles = devDir.entryInfoList();
				if (devFiles)
				{
					QFileInfoListIterator devIt(*devFiles);
					QFileInfo* devFi;
					QDevItem* item = NULL;
					while ((devFi = devIt.current()) != 0)
					{
						++devIt;
						if (devFi->fileName() == "." || devFi->fileName() == "..")
							continue;
						if (devFi->isDir())
						{
							if (HasDevice(devFi->absFilePath()))
								continue;
							item = new QDevItem(this, item, devFi->absFilePath());
						}
					}
				}
			}
		}
	}*/
	setExpanded(true);
}

QDevItem::~QDevItem()
{
}

// ָ��װ���Ƿ��Ѿ����
bool QDevItem::HasDevice(const QString& dev)
{
	bool b = false;
	QString str = relative_path(dev);
	
	QTreeWidgetItem *item = NULL;
	for (int i=0;i<childCount();i++)
	{
		item = child(i);
		if (((QDevItem*)item)->Device() == str)
		{
			b = true;
			break;
		}
	}
	return b;
}
