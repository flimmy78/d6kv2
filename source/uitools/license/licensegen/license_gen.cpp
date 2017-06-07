#include "license_gen.h"
#include "license_gen_def.h"
#include "AES.h"
#include "rwxml.h"

#include <QNetworkInterface>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>

DLicense_gen::DLicense_gen( QWidget* parent /*= 0 */ ) : QMainWindow( parent )
{
	setupUi( this ) ;
	ini_main_window() ;

	connect( m_pSelectList , SIGNAL( currentRowChanged( int ) ) , this , SLOT( on_change_widget( int ) ) ) ;
	connect( m_pOkBtn , SIGNAL( clicked() ) , this , SLOT( on_ok_btn() ) ) ;
	connect( m_pCancelBtn , SIGNAL( clicked() ) , this , SLOT( on_cancel_btn() ) ) ;
	connect( m_pMacBtn , SIGNAL( clicked() ) , this , SLOT( on_get_macaddr() ) ) ;
	connect( m_pGenPathBtn , SIGNAL( clicked() ) , this , SLOT( on_get_genpath() ) ) ;
	connect( m_pCheckPathBtn , SIGNAL( clicked() ) , this , SLOT( on_get_checkpath() ) ) ;
}

void DLicense_gen::ini_main_window( void )
{
	m_pSelectList->addItem( new QListWidgetItem( QIcon( ":/res/point.png" ), tr( "����" ) ) ) ;
	m_pSelectList->addItem( new QListWidgetItem( QIcon( ":/res/point.png" ), tr( "�鿴" ) ) ) ;
	m_pSelectList->item( 0 )->setSelected( true ) ;

	m_pTimeComboBox->addItem( tr( "��  ��" ) , 3 ) ;
	m_pTimeComboBox->addItem( tr( "һ����" ) , 30 ) ;
	m_pTimeComboBox->addItem( tr( "������" ) , 90 ) ;
	m_pTimeComboBox->addItem( tr( "��  ��" ) , 180 ) ;
	m_pTimeComboBox->addItem( tr( "һ  ��" ) , 365 ) ;
	m_pTimeComboBox->addItem( tr( "������" ) , -1 ) ;

	m_pHsplitter->setStretchFactor( 0 , 1 ) ;
	m_pHsplitter->setStretchFactor( 1 , 3 ) ;
}

void DLicense_gen::on_change_widget( int list_index )
{
	if ( list_index == 0 )
	{
		m_pSelectList->item( 0 )->setIcon( QIcon( ":/res/star.png" ) ) ;
		m_pSelectList->item( 1 )->setIcon( QIcon( ":/res/point.png" ) ) ;
		m_pGenLicFrame->show() ;
		m_pCheckLicFrame->hide() ;
	}
	else if ( list_index == 1 )
	{
		m_pSelectList->item( 0 )->setIcon( QIcon( ":/res/point.png" ) ) ;
		m_pSelectList->item( 1 )->setIcon( QIcon( ":/res/star.png" ) ) ;
		m_pGenLicFrame->hide() ;
		m_pCheckLicFrame->show() ;
	}
}

void DLicense_gen::on_ok_btn()
{
	if ( m_pSelectList->currentRow() == 0 )
	{
		gen_license() ;
	}
	else
	{
		check_license() ;
	}
}

void DLicense_gen::gen_license( void )
{
	if ( m_pMacLineEdit->text().isEmpty() )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr( "������Mac��ַ��" ) ) ;
		return;
	}

	if ( m_pGenPathLineEdit->text().isEmpty() )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr( "��ѡ������·����" ) ) ;
		return;
	}

	QString strLicPath = m_pGenPathLineEdit->text() + _C2Q( "/license.lic" ) ;

    QFile f( strLicPath ) ;
    if ( f.exists() )
    {
		int tmp_res = QMessageBox::warning( this , tr("�澯��ʾ") , tr("�ļ����ڣ��Ƿ񸲸ǣ�") ,
			QMessageBox::Ok | QMessageBox::Cancel,
			QMessageBox::Ok ) ;
        switch ( tmp_res )
        {
        case QMessageBox::Ok:
            break ;
        case QMessageBox::Cancel:
            return ;
        }
    }

// 	ini_RW* pini = new ini_RW( ( char*) _Q2C( strLicPath ) ) ;
	Xml_RW* pini = new Xml_RW( ( char*) _Q2C( strLicPath ) ) ;

    // AES�����㷨��ʼ��key
    AES aes( aes_key ) ;
    unsigned char key[ 64 ] = { 0 } ;
    unsigned char value[ 64 ] = { 0 } ;
	char hexKey[ 128 ] = { 0 } ;
	char hexValue[ 128 ] = { 0 } ;

    // ����
	QString key_str = tr( "��Ȩ����" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;

    // ����value
    int period = m_pTimeComboBox->itemData( m_pTimeComboBox->currentIndex() ).toInt() ;
    memset( value , 0 , 64 ) ;
	sprintf( ( char* )value , "%d" , period ) ;
	aes.Cipher( ( void* )value ) ;
	ByteToHexStr( value , hexValue , 32 ) ;
	pini->set_string( "license" , ( char* )hexKey , ( char* )hexValue ) ;

	key_str = tr( "�ļ�����ʱ��" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;

	// �ļ�����ʱ��value
	QDateTime file_date_time = QDateTime::currentDateTime() ;
	QString file_date = file_date_time.toString( "yyyy-MM-dd hh:mm:ss" ) ;
	memset( value , 0 , 64 ) ;
	sprintf( ( char* )value , _Q2C( file_date ) ) ;
	aes.Cipher( ( void* )value ) ;
	ByteToHexStr( value , hexValue , 32 ) ;
	pini->set_string( "license" , ( char* )hexKey , ( char* )hexValue ) ;

	key_str = tr( "MAC��ַ" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;

	// �ļ�����ʱ��value
	memset( value , 0 , 64 ) ;
	sprintf( ( char* )value , _Q2C( m_pMacLineEdit->text() ) ) ;
	aes.Cipher( ( void* )value ) ;
	ByteToHexStr( value , hexValue , 32 ) ;
	pini->set_string( "license" , ( char* )hexKey , ( char* )hexValue ) ;

	key_str = tr( "��Ȩ�Ƿ���ʹ��" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;

	// ֤���Ƿ���ʹ��value
	memset( value , 0 , 64 ) ;
	sprintf( ( char* )value , "��" ) ;
	aes.Cipher( ( void* )value ) ;
	ByteToHexStr( value , hexValue , 32 ) ;
	pini->set_string( "license" , ( char* )hexKey , ( char* )hexValue ) ;

	if( pini )
	{
		delete( pini ) ;
		pini = NULL ;
	}

    QMessageBox::information( this , tr( "��ʾ" ) , tr( "������Ȩ�ļ��ɹ���") ) ;
}

void DLicense_gen::check_license( void )
{
	if ( m_pCheckPathLineEdit->text().isEmpty() )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("��ѡ��鿴�ļ���" ) ) ;
		return ;
	}

	QString strFilePath = m_pCheckPathLineEdit->text() ;
	QFile f( strFilePath ) ;
	if ( !f.exists() )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("�ļ������ڣ�" ) ) ;
		return ;
	}

	m_pDetailTable->setRowCount( 4 ) ;

// 	ini_RW* pini = new ini_RW( ( char*) _Q2C( strFilePath ) ) ;
	Xml_RW* pini = new Xml_RW( ( char*) _Q2C( strFilePath ) ) ;

	AES aes( aes_key ) ;
	unsigned char key[ 64 ] = { 0 } ;
	unsigned char value[ 64 ] = { 0 } ;
	char hexKey[ 128 ] = { 0 } ;
	char *hexValue = NULL ;

	// ����
	QString key_str = tr( "��Ȩ����" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;

	if ( SUCCESS != pini->get_string( "license" , ( char* )hexKey , hexValue ) )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("��Ȩ�ļ�����" ) ) ;
		return ;
	}
	else
	{
		HexStrToByte( hexValue , value , strlen( hexValue ) ) ;
		aes.InvCipher( ( void* )value , strlen( hexValue ) )  ;
	}
	m_pDetailTable->setItem( 0 , 0 , new QTableWidgetItem( _C2Q( "��Ȩ����" ) ) ) ;
	m_pDetailTable->setItem( 0 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) + _C2Q( "��" ) ) ) ;

	key_str = tr( "�ļ�����ʱ��" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;
	hexValue = NULL ;
	if ( SUCCESS != pini->get_string( "license" , ( char* )hexKey , hexValue ) )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("��Ȩ�ļ�����" ) ) ;
		return ;
	}
	else
	{
		HexStrToByte( hexValue , value , strlen( hexValue ) ) ;
		aes.InvCipher( ( void* )value , strlen( hexValue ) )  ;
	}

	m_pDetailTable->setItem( 1 , 0 , new QTableWidgetItem( _C2Q( "�ļ�����ʱ��" ) ) ) ;
	m_pDetailTable->setItem( 1 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) ) ) ;

	key_str = tr( "MAC��ַ" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;
	hexValue = NULL ;
	if ( SUCCESS != pini->get_string( "license" , ( char* )hexKey , hexValue ) )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("��Ȩ�ļ�����" ) ) ;
		return ;
	}
	else
	{
		HexStrToByte( hexValue , value , strlen( hexValue ) ) ;
		aes.InvCipher( ( void* )value , strlen( hexValue ) )  ;
	}
	m_pDetailTable->setItem( 2 , 0 , new QTableWidgetItem( _C2Q( "MAC��ַ" ) ) ) ;
	m_pDetailTable->setItem( 2 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) ) ) ;

	key_str = tr( "��Ȩ�Ƿ���ʹ��" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;
	hexValue = NULL ;
	if ( SUCCESS != pini->get_string( "license" , ( char* )hexKey , hexValue ) )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("��Ȩ�ļ�����" ) ) ;
		return ;
	}
	else
	{
		HexStrToByte( hexValue , value , strlen( hexValue ) ) ;
		aes.InvCipher( ( void* )value , strlen( hexValue ) )  ;
	}
	m_pDetailTable->setItem( 3 , 0 , new QTableWidgetItem( _C2Q( "��Ȩ�Ƿ���ʹ��" ) ) ) ;
	m_pDetailTable->setItem( 3 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) ) ) ;

	if( pini )
	{
		delete( pini ) ;
		pini = NULL ;
	}
}

void DLicense_gen::on_get_macaddr( void )
{
	QString strMac ;
	bool flag = false ;
	//��ȡ����������Ϣ
	QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces() ;
	for ( int i = 0 ; i < ifaces.count() ; i++ )
	{
		QNetworkInterface iface = ifaces.at( i ) ;
		if ( iface.flags().testFlag(QNetworkInterface::IsUp) &&
			iface.flags().testFlag(QNetworkInterface::IsRunning)
			&& !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
		{ 
			//��ȡ��ǰ��Ч����
			for (int j = 0; j < iface.addressEntries().count(); j++)
			{
				strMac = iface.hardwareAddress();
				flag = true ;
				break ;
			}

			if ( flag )
			{
				break ;
			}
		}
	}

	m_pMacLineEdit->setText(strMac);
}

void DLicense_gen::on_get_genpath( void )
{
	char filename[ 255 ] ;
	sprintf( filename , "%s/log/dat" , getenv("NBENV") ) ;
	QString str = QFileDialog::getExistingDirectory( this , tr( "ѡ�������ļ�·��" ) , _C2Q( filename ) ) ;
	m_pGenPathLineEdit->setText( str ) ;
}

void DLicense_gen::on_get_checkpath( void )
{
	char filename[ 255 ] ;
	sprintf( filename , "%s/log/dat" , getenv("NBENV") ) ;
	QString str = QFileDialog::getOpenFileName( this , tr( "��Ҫ�鿴���ļ�" ) , _C2Q( filename ) , "license files(*.lic)" ) ;
	m_pCheckPathLineEdit->setText( str ) ;
}

void DLicense_gen::on_cancel_btn( void )
{
	close() ;
}
