#ifndef FILEVIEWIMPL_H
#define FILEVIEWIMPL_H

#include <QDir>
#include <QListWidget>

#include "ui_fileview.h"

#ifdef WIN32
	const QString FILEPATH = "D:/ICS8000/MyTask/faultrecord/bin/recordfile";
#else
	const QString FILEPATH = "/ics8000/cbin/recordfile";
#endif

struct FILEINFO
{
	int year;
	QString device;
	QString sector;
	FILEINFO() { year = 0; device = ""; sector = ""; }
};

class QFileItem : public QTreeWidgetItem
{
public:
	QFileItem(QTreeWidgetItem* parent, QTreeWidgetItem* after, const QString& path);
	QFileItem(QTreeWidget* parent, QTreeWidgetItem* after, const QString& path);
	QFileItem(QTreeWidget* parent, QTreeWidgetItem* after, const QString& path, 
		const QString& dev, const QString& sec, QFileInfo* fi);
	~QFileItem();

	void SetDevice(const QString& s)	{ m_strDevice = s; }
	void SetSector(const QString& s)	{ m_strSector = s; }
	void SetYear(int n)					{ m_nYear = n; }

	QString Path()						{ return m_strPath; }
	QString Device()					{ return m_strDevice; }
	QString Sector()					{ return m_strSector; }
	int Year()							{ return m_nYear; }

protected:
	void Init();
	void SetDirText();

private:
	int m_nYear;
	QString m_strDevice;
	QString m_strSector;
	QString m_strPath;
	
};


class QDirItem : public QFileItem
{
public:
	QDirItem(QTreeWidgetItem* parent, QTreeWidgetItem* after, QString path);
	QDirItem(QTreeWidget* parent, QTreeWidgetItem* after, QString path);
	~QDirItem();

protected:
	void CreateChild();
	int depth();

};

class QDevItem : public QFileItem
{
public:
	QDevItem(QTreeWidgetItem* parent, QTreeWidgetItem* after, QString path);
	QDevItem(QTreeWidget* parent, QTreeWidgetItem* after, QString path);
	~QDevItem();

protected:
	bool HasDevice(const QString& dev);

};

typedef struct tagFileItem
{
	char* file;		// �ļ���
	int length;		// �ļ�������
	QString time;	// ����
	bool fault;		// ���ϱ�־
}FILEITEM;

typedef  QList<FILEITEM*> FileList;

class QDlgFileViewImpl : public QDialog,public Ui::QDlgFileView
{
    Q_OBJECT

public:
    QDlgFileViewImpl( QWidget* parent = 0, Qt::WindowFlags f= 0 );
    ~QDlgFileViewImpl();

protected:
	void InitControl();
	void InitListView(QTreeWidget* lv);
	void TravelDirItem(QDirItem* i, bool all = false);
	void TravelFileDir(const QString& path, const QString& dev, int depth);
	void ListDevices(QTreeWidgetItem* i);
	FILEINFO GetFileInfo(const QString& path, int depth);
	FILEITEM* GetItem(const QString& path);
	bool HasDatFile(const QString& cfg);
	void DeleteFiles(bool all = false);

	void create_actions();

protected slots:
	void DirSelectionChange(QTreeWidgetItem* i);
	void DevSelectionChange(QTreeWidgetItem* i);
	void FileViewMove(int x, int y);
	void FileItemClick( const QPoint& pos );

	void slot_open();					///< ���ļ�
	void slot_open_newform();	///< ���´����д��ļ�
	void slot_delete();		///< ɾ��ѡ���ļ�
	void slot_delete_all();	///< ɾ�������ļ�
protected:
	FileList m_lstFile;

private:
	QAction *action_open;	///<��
	QAction *action_open_newform;	///<���´����д�
	QAction *action_delete;	///< ɾ��
	QAction *action_delete_all;	///< ȫ��ɾ��

	QFileItem* m_current_item;	//��ǰ��
};

#endif