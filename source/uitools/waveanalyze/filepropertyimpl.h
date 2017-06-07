/**
@file filepropertyimpl.h
@brief �ļ����ԶԻ�����ͷ�ļ�
@author lujiashun 
@version 1.0.0
@date 2016-10-25
*/
#ifndef FILEPROPERTYIMPL_H
#define FILEPROPERTYIMPL_H

#include "ui_fileproperty.h"

#include "comtrade.h"

class File_property_impl : public QDialog, public Ui::QFileProperty
{
    Q_OBJECT

public:
    File_property_impl(Comtrade* c, QWidget* parent = 0);
    ~File_property_impl();

protected:
	void init();

private:
	Comtrade* m_pData;

};

#endif