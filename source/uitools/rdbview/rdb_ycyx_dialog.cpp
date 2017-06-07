#include "rdb_ycyx_dialog.h"
#include "rwxml.h"
#include "uitools/dmessagebox.h"

#include <QMessageBox>

extern SHOWCFG* ShowCfg;  //������ʾ���õĽṹ

Rdb_ycyx_dialog::Rdb_ycyx_dialog( QWidget* parent /*= 0 */ ) : QDialog( parent )
{
	setupUi( this ) ;
	m_pTime_spinbox->setValue( ShowCfg->FreshTime ) ;
	m_pRefresh_checkbox->setChecked( ShowCfg->FreshFlag ) ;
	m_pYccal_checkbox->setChecked( ShowCfg->ShowYcCalcult ) ;
	m_pYxcal_checkbox->setChecked( ShowCfg->ShowYxCalcult ) ;
	m_pYxmanset_checkbox->setChecked( ShowCfg->ShowYxManset ) ;
	m_pYcmanset_checkbox->setChecked( ShowCfg->ShowYcManset ) ;
	m_pYcxs_checkbox->setChecked( ShowCfg->ShowYcxs ) ;
	m_pYcky_checkbox->setChecked( ShowCfg->ShowYcKy ) ;
	m_pKwhshow_checkbox->setChecked( ShowCfg->ShowKwhTableCode ) ;
	m_pStation_checkbox->setChecked( ShowCfg->OrderByStation ) ;

	connect( m_pOk_button , SIGNAL( clicked() ), this, SLOT( slot_ok() ) ) ;
	connect( m_pCancel_button , SIGNAL( clicked() ), this, SLOT( slot_cancel() ) ) ;

	QString styleSheet = tr( "QLabel \
						  {\
						  font-family: Microsoft YaHei;\
						  font-size: 13px;\
						  color:#4A4A4A;\
						  }\
						  ") ;
	setStyleSheet( styleSheet ) ;
}

void Rdb_ycyx_dialog::slot_ok()
{
	ShowCfg->FreshTime = m_pTime_spinbox->value() ;
	ShowCfg->FreshFlag = m_pRefresh_checkbox->isChecked() ;
	ShowCfg->ShowYcCalcult = m_pYccal_checkbox->isChecked() ;
	ShowCfg->ShowYxCalcult = m_pYxcal_checkbox->isChecked() ;
	ShowCfg->ShowYxManset = m_pYxmanset_checkbox->isChecked() ;
	ShowCfg->ShowYcManset = m_pYcmanset_checkbox->isChecked() ;
	ShowCfg->ShowYcxs = m_pYcxs_checkbox->isChecked() ;
	ShowCfg->ShowYcKy = m_pYcky_checkbox->isChecked() ;
	ShowCfg->ShowKwhTableCode = m_pKwhshow_checkbox->isChecked() ;
	ShowCfg->OrderByStation = m_pStation_checkbox->isChecked() ;

	char file_name[ 256 ] ;
// 	sprintf( file_name ,"%s/ini/rdbview.ini" , getenv( "NBENV" ) ) ;
// 	ini_RW* pini  =  new ini_RW( file_name ) ;
	sprintf( file_name ,"%s/resource/xml/rdbview.xml" , getenv( "NBENV" ) ) ;
	Xml_RW* pini  =  new Xml_RW( file_name ) ;

	if( SUCCESS  !=  pini->set_int( "ϵͳ����" , "ˢ��ʱ��" , ShowCfg->FreshTime ) )
	{
		DMessageBox::information( this , tr( "����" ) , tr( "����ʧ�ܣ�" ) ) ;
		return ;
	}
	if( SUCCESS  !=  pini->set_bool( "ϵͳ����" , "�Ƿ�ˢ��" , ShowCfg->FreshFlag ) )
	{
		DMessageBox::information( this , tr( "����" ) , tr( "����ʧ�ܣ�" ) ) ;
		return ;
	}
	if( SUCCESS  !=  pini->set_bool( "ϵͳ����" , "�Ƿ���ʾң������־" , ShowCfg->ShowYcCalcult ) )
	{
		DMessageBox::information( this , tr( "����" ) , tr( "����ʧ�ܣ�" ) ) ;
		return ;
	}
	if( SUCCESS  !=  pini->set_bool( "ϵͳ����" , "�Ƿ���ʾң�ż����־" , ShowCfg->ShowYxCalcult ) )
	{		
		DMessageBox::information( this , tr( "����" ) , tr( "����ʧ�ܣ�" ) ) ;
		return ;
	}
	if( SUCCESS  !=  pini->set_bool( "ϵͳ����" , "�Ƿ���ʾң������" , ShowCfg->ShowYxManset ) )
	{
		DMessageBox::information( this , tr( "����" ) , tr( "����ʧ�ܣ�" ) ) ;
		return ;
	}
	if( SUCCESS  !=  pini->set_bool( "ϵͳ����" , "�Ƿ���ʾң������" , ShowCfg->ShowYcManset ) )
	{
		DMessageBox::information( this , tr( "����" ) , tr( "����ʧ�ܣ�" ) ) ;
		return ;
	}
	if( SUCCESS  !=  pini->set_bool( "ϵͳ����" , "�Ƿ���ʾң��ϵ��" , ShowCfg->ShowYcxs ) )
	{
		DMessageBox::information( this , tr( "����" ) , tr( "����ʧ�ܣ�" ) ) ;
		return ;
	}
	if( SUCCESS  !=  pini->set_bool( "ϵͳ����" , "�Ƿ���ʾң�����" , ShowCfg->ShowYcKy ) )
	{
		DMessageBox::information( this , tr( "����" ) , tr( "����ʧ�ܣ�" ) ) ;
		return ;
	}
	if( SUCCESS  !=  pini->set_bool( "ϵͳ����" , "�Ƿ���ʾ����" , ShowCfg->ShowKwhTableCode ) )
	{
		DMessageBox::information( this , tr( "����" ) , tr( "����ʧ�ܣ�" ) ) ;
		return ;
	}
	if( SUCCESS  !=  pini->set_bool( "ϵͳ����" , "�Ƿ񰴳�վ����" , ShowCfg->OrderByStation ) )
	{
		DMessageBox::information( this , tr( "����" ) , tr( "����ʧ�ܣ�" ) ) ;
		return ;
	}
	if( pini )
	{
		delete pini ;
		pini  =  NULL ;
	}
	DMessageBox::information( this , tr( "��ʾ" ) , tr( "����ɹ�" ) ) ;
}

void Rdb_ycyx_dialog::slot_cancel()
{
	reject() ;
}

