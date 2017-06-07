#include "feskeyinfo.h" 

QFesKeyInfo::QFesKeyInfo( KeyOperType oper_type , ENCRYPT_KEYNO encry_key_info , QWidget *parent ) : QDialog( parent )
{
	m_key_oper_type = oper_type ;
	memcpy( &m_encry_key_info , &encry_key_info , sizeof( ENCRYPT_KEYNO ) ) ;
	memcpy( &m_encry_key_info.new_key_no , &encry_key_info.cur_key_no , 4 * sizeof( uchar ) ) ;

	this ->setModal( true ) ;

	QString strTitle ;
	switch( oper_type )
	{
	case keyCheck://��鹫Կ
		{
			strTitle = tr( " ��鹫Կ" ) ;
			break ;
		}
	case keyRefresh://���¹�Կ
		{
			strTitle = tr( " ���¹�Կ" ) ;
			break ;
		}
	case keyExport://������Կ�ļ�
		{
			strTitle = tr( " ������Կ�ļ�" ) ;
			break ;
		}
	default:
		{
			break ;
		}
	}
	this ->setWindowTitle( strTitle ) ;

	m_key_no_label.setText( tr( " ��Կ�ţ�" ) ) ;
	m_key_no_in_card_label.setText( tr( " ���ܿ��й�Կ�ţ�" ) ) ;

	for ( int i = 0 ; i < 4 ; i++ )
	{
		m_key_no_combo.addItem( QString::number( i + 1 ) ) ;
	}
	
	for ( int i = 0 ; i < 50 ; i++ )
	{
		m_key_no_in_card_combo.addItem( QString::number( i + 1 ) ) ;
	}
	m_key_no_combo.setCurrentIndex( 0 ) ;
	m_key_no_in_card_combo.setCurrentIndex( m_encry_key_info.cur_key_no[ 0 ] - 1 ) ;
	
	connect( &m_key_no_combo,SIGNAL( currentIndexChanged( int ) ),this,SLOT( key_no_changed( int ) ) ) ;

	m_key_info_layout.addWidget( &m_key_no_label,0 ) ;
	m_key_info_layout.addWidget( &m_key_no_combo,1 ) ;

	m_key_info_layout.addWidget( &m_key_no_in_card_label,2 ) ;
	m_key_info_layout.addWidget( &m_key_no_in_card_combo,3 ) ;

	m_ok_btn.setText( tr( " ȷ��" ) ) ;
	connect( &m_ok_btn,SIGNAL( released( ) ),this,SLOT( accept( ) ) ) ;
	m_cancel_btn.setText( tr( " ȡ��" ) ) ;
	connect( &m_cancel_btn,SIGNAL( released( ) ),this,SLOT( reject( ) ) ) ;
	m_btn_layout.addWidget( &m_ok_btn,0 ) ;
	m_btn_layout.addWidget( &m_cancel_btn,0 ) ;

	m_main_layout.addLayout( &m_key_info_layout,0 ) ;
	m_main_layout.addLayout( &m_btn_layout,1 ) ;

	this ->setLayout( &m_main_layout ) ;
}

QFesKeyInfo::~QFesKeyInfo( )
{
}

bool QFesKeyInfo::getKeyInfo( ENCRYPT_KEYNO &keyInfo )
{
	int currentIndex = m_key_no_combo.currentIndex( ) ;	
	int currentIndexInCard = m_key_no_in_card_combo.currentIndex( ) ;

	keyInfo.oper_key_no = currentIndex + 1 ;
	switch( m_key_oper_type )
	{
	case keyCheck://��鹫Կ
		{
			break ;
		}
	case keyRefresh://���¹�Կ
		{
			keyInfo.new_key_no[ currentIndex ] = currentIndexInCard + 1 ;
			break ;
		}
	case keyExport://������Կ�ļ�
		{			
			break ;
		}
	default:
		{
			break ;
		}
	}
	return true ;
}

void QFesKeyInfo::key_no_changed( int index )
{
	int currentIndex = m_key_no_combo.currentIndex( ) ;	

	m_key_no_in_card_combo.setCurrentIndex( m_encry_key_info.cur_key_no[ currentIndex ] - 1 ) ;
}

void QFesKeyInfo::key_no_in_card_changed( int index )
{

}