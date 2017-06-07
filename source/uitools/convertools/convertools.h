/**
@file convertools.h
@brief ת������ͷ�ļ�

@author ljh
@version 1.0
@date 2017-3-16
*/
#ifndef CONVERTOOLS_H
#define CONVERTOOLS_H

#include <QMainWindow>
#include "ddes/GGraphFile.h"
#include "ddef/GDevFile.h"
#include "ui_convertools.h"

#define _Q2C(str) ((const char *)((str).toLocal8Bit().data())) 
#define _C2Q(str) (QString::fromLocal8Bit(str))

class Convertools : public QMainWindow , public Ui::convertbase
{
	Q_OBJECT
public:
	Convertools( QWidget* parent = NULL , const char* name = NULL);
	~Convertools();

public:
	bool gdf_to_xml( QString &gdfpath , QString &xmlpath ); //gdfתxml���޸�xml
	bool xml_to_bgf( QString &xmlpath , QString &bgfpath );//xmlתbgf
	int  get_file_list( QStringList &resultList , QDir director , QString filter );//��ȡ�ļ��б�

public slots:
	void slot_open_dir(); //���ļ���
	void slot_open_file();//��һ�������ļ�
	void slot_del_filelist();//ɾ��һ�������ļ�
	void slot_search_file();
	void slot_gdf_to_bgf();//gdfתbgf
	void slot_about();
	void slot_exit();

private:
	QStringList m_file_list ; //�ļ��б�
	QString m_file_path;//Ĭ��·��

};
#endif