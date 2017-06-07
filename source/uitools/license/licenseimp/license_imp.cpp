#include "license_imp.h"
#include "AES.h"
#include <QNetworkInterface>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>

// AES ���ܽ����㷨key
unsigned char aes_key[] = \
{ \
0x2b, 0x7e, 0x15, 0x16, \
0x28, 0xae, 0xd2, 0xa6, \
0xab, 0xf7, 0x15, 0x88, \
0x09, 0xcf, 0x4f, 0x3c \
};


License_imp::License_imp( QWidget* parent /*= 0 */ ) : QMainWindow( parent )
{
	setupUi( this ) ;
	m_hdbi.setDB( "modeldb" ) ;
	m_hdbi.setUser( "modeldb" ) ;
	m_hdbi.setPass( "supernb" ) ;
	m_hdbi.createDBI( Ddbi::GD_DBI_HIS ) ;
	get_mac_addrs() ;

	connect( m_pFile_path_button ,  SIGNAL( clicked() ) , this , SLOT( slot_choose_file() ) ) ;
	connect( m_pCheck_button ,  SIGNAL( clicked() ) , this , SLOT( slot_license_check() ) ) ;
	connect( m_pImp_button ,  SIGNAL( clicked() ) , this , SLOT( slot_license_imp() ) ) ;
	connect( m_pClose_button ,  SIGNAL( clicked() ) , this , SLOT( close() ) ) ;
}

void License_imp::get_mac_addrs( void )
{
	m_mac_addrs.clear() ;
	QString strMac ;
	QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces() ;
	for ( int i = 0 ; i < ifaces.count() ; i++ )
	{
		QNetworkInterface iface = ifaces.at( i ) ;

		strMac = iface.hardwareAddress();
		if ( m_mac_addrs.indexOf( strMac ) == -1 )
		{
			m_mac_addrs.append( strMac ) ;
		}
	}
}

void License_imp::slot_license_imp( void )
{
	if ( m_pFile_path_lineedit->text().isEmpty() )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("��ѡ��鿴�ļ���" ) ) ;
		return ;
	}

	QString strFilePath = m_pFile_path_lineedit->text() ;
	QFile f( strFilePath ) ;
	if ( !f.exists() )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("�ļ������ڣ�" ) ) ;
		return ;
	}

	LICENSE_STRU* pLicense_stru = new LICENSE_STRU ;
	pLicense_stru->no = 0 ;

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
	pLicense_stru->period = _C2Q( ( char* )( value ) ).toInt() ;

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
	strcpy( pLicense_stru->file_gen_time , ( char* )value ) ;

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
	if( m_mac_addrs.indexOf( ( char* )( value ) ) == -1 )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("��Ȩ�ļ��뱾��MAC��ַ����!" ) ) ;
		return ;
	}
	else
	{
		strcpy( pLicense_stru->mac_addr , ( char* )value ) ;
	}

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
	if( tr( "��") == _C2Q( ( char* )( value ) ) )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("��Ȩ�ļ��ѱ�ʹ�ã�" ) ) ;
		return ;
	}
	else
	{
		QDateTime file_imp_time = QDateTime::currentDateTime() ;
		QString file_date = file_imp_time.toString( "yyyy-MM-dd hh:mm:ss" ) ;
		sprintf( pLicense_stru->file_imp_time , _Q2C( file_date ) ) ;
		pLicense_stru->expired_flag = 0 ;

		if( save_imp_info( pLicense_stru ) )
		{
			// ֤���Ƿ���ʹ��value
			memset( value , 0 , 64 ) ;
			sprintf( ( char* )value , "��" ) ;
			aes.Cipher( ( void* )value ) ;
			ByteToHexStr( value , hexValue , 32 ) ;
			pini->set_string( "license" , ( char* )hexKey , ( char* )hexValue ) ;
		}
		else
		{
			QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("��Ȩ�ļ�����ʧ�ܣ�" ) ) ;
			return ;
		}
	}

	if( pini )
	{
		delete( pini ) ;
		pini = NULL ;
	}
	QMessageBox::information( this , tr( "��ʾ" ) , tr("��Ȩ�ļ�����ɹ���" ) ) ;
}

void License_imp::slot_license_check( void )
{
	if ( m_pFile_path_lineedit->text().isEmpty() )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("��ѡ��鿴�ļ���" ) ) ;
		return ;
	}

	QString strFilePath = m_pFile_path_lineedit->text() ;
	QFile f( strFilePath ) ;
	if ( !f.exists() )
	{
		QMessageBox::warning( this , tr( "�澯��ʾ" ) , tr("�ļ������ڣ�" ) ) ;
		return ;
	}

	m_pLicense_info_table->setRowCount( 4 ) ;

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
	m_pLicense_info_table->setItem( 0 , 0 , new QTableWidgetItem( _C2Q( "��Ȩ����" ) ) ) ;
	m_pLicense_info_table->setItem( 0 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) + _C2Q( "��" ) ) ) ;

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

	m_pLicense_info_table->setItem( 1 , 0 , new QTableWidgetItem( _C2Q( "�ļ�����ʱ��" ) ) ) ;
	m_pLicense_info_table->setItem( 1 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) ) ) ;

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
	m_pLicense_info_table->setItem( 2 , 0 , new QTableWidgetItem( _C2Q( "MAC��ַ" ) ) ) ;
	m_pLicense_info_table->setItem( 2 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) ) ) ;

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
	m_pLicense_info_table->setItem( 3 , 0 , new QTableWidgetItem( _C2Q( "��Ȩ�Ƿ���ʹ��" ) ) ) ;
	m_pLicense_info_table->setItem( 3 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) ) ) ;

	if( pini )
	{
		delete( pini ) ;
		pini = NULL ;
	}
}

void License_imp::slot_choose_file( void )
{
	char filename[ 255 ] ;
	sprintf( filename , "%s/log/dat" , getenv("NBENV") ) ;
	QString str = QFileDialog::getOpenFileName( this , tr("��Ҫ������ļ�") , tr( filename ) ,"license files(*.lic)" ) ;
	m_pFile_path_lineedit->setText( str ) ;
}

bool License_imp::save_imp_info( LICENSE_STRU* pLicense_stru )
{
	int success = m_hdbi.startdb( Ddbi::G_DB_RW ) ;
	if ( !success )
	{
		return false ;
	}
	int i = 0,col = 0;
	QString strCode = "" ;
	GdbRecord record ;
	record.allocMem( 6 ) ;
	char sqlw[ 128 ] ;
	sprintf( sqlw, "select ���,��Ȩ����,�ļ�����ʱ��,\
				   �ļ�����ʱ��,MAC��ַ,�Ƿ���� from ��ȨȨ�޲����� where ���=0" ) ;

	m_hdbi.selectFrom( "��ȨȨ�޲�����" ) ;

	GdbRecords recs ;
	if( !m_hdbi.readSQL( recs , sqlw ) || recs.recnum == 0 )
	{
		col = 0 ;
		FillGdbRecord( record,col,pLicense_stru->no ) ;
		FillGdbRecord( record,col,pLicense_stru->period ) ;
		FillGdbRecord( record,col,pLicense_stru->file_gen_time ) ;
		FillGdbRecord( record,col,pLicense_stru->file_imp_time ) ;
		FillGdbRecord( record,col,pLicense_stru->mac_addr ) ;
		FillGdbRecord( record,col,pLicense_stru->expired_flag ) ;

		if( m_hdbi.insertARecord( record )  )
		{
			return true ;
		}
		else
		{
			return false ;
		}
	}
	else
	{
		col = 0 ;
		FillGdbRecord( record,col,pLicense_stru->no ) ;
		FillGdbRecord( record,col,pLicense_stru->period ) ;
		FillGdbRecord( record,col,pLicense_stru->file_gen_time ) ;
		FillGdbRecord( record,col,pLicense_stru->file_imp_time ) ;
		FillGdbRecord( record,col,pLicense_stru->mac_addr ) ;
		FillGdbRecord( record,col,pLicense_stru->expired_flag ) ;

		if( m_hdbi.updateARecord( recs[0] , record ) )
		{
			return true ;
		}
		else
		{
			return false ;
		}
	}
}
