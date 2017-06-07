#include <QtGui/QBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QHeaderView>
#include "rawviewframe.h"
#include <sys/stat.h>

QRawViewFrame::QRawViewFrame( QWidget* parent /* = NULL */ ) :QFrame( parent )
{
	m_curRTU  = -1;
	m_pTabWidget = new QTabWidget( this );
	m_pYcTable = new QTableWidget( this );
	m_pYxTable = new QTableWidget( this );
	m_pKwhTable = new QTableWidget( this );

	m_pYcTable->setEditTriggers ( QAbstractItemView::NoEditTriggers );
	m_pYxTable->setEditTriggers ( QAbstractItemView::NoEditTriggers );
	m_pKwhTable->setEditTriggers ( QAbstractItemView::NoEditTriggers );

	m_pTabWidget->addTab( m_pYcTable, _C2Q( "ң��" ) );
	m_pTabWidget->addTab( m_pYxTable, _C2Q( "ң��" ) );
	m_pTabWidget->addTab( m_pKwhTable, _C2Q( "���" ) );


	QVBoxLayout *layOut = new QVBoxLayout( this );
	layOut->addWidget( m_pTabWidget );
	setLayout( layOut );
	initTable(  );

	m_tableLock = 0;
	m_rtuChanged = 0;
	m_yc = NULL;
	m_yx = NULL;
	m_kwh = NULL;

	m_ycNum = 0;
	m_yxNum = 0;
	m_kwhNum = 0;
}


QRawViewFrame::~QRawViewFrame(  )
{

}

void QRawViewFrame::onReload(  )
{

}

void QRawViewFrame::initTable(  )		//����仯����ʱ����ʾ
{
	if ( NULL == m_pYcTable || NULL == m_pYxTable || NULL == m_pKwhTable )
	{
		return;
	}

	m_pYcTable->setColumnCount( 7 );
	QStringList ycHeader;
	ycHeader<<_C2Q( "ң���" )
		<<_C2Q( "����" )
		<<_C2Q( "������" )
		<<_C2Q( "��Ч" )
		<<_C2Q( "�仯ʱ����" )
		<<_C2Q( "�仯ʱ�����" )
		<<_C2Q( "������" );
	m_pYcTable->setHorizontalHeaderLabels( ycHeader ); 
	m_pYcTable->verticalHeader(  )->setVisible( false );

	m_pYcTable->setColumnWidth( 0, 50 );
	m_pYcTable->setColumnWidth( 1, 200 );
	m_pYcTable->setColumnWidth( 2, 100 );
	m_pYcTable->setColumnWidth( 3, 50 );
	m_pYcTable->setColumnWidth( 4, 100 );
	m_pYcTable->setColumnWidth( 5, 100 );
	m_pYcTable->setColumnWidth( 6, 50 );


	m_pYxTable->setColumnCount( 9 );
	QStringList yxHeader;
	yxHeader<<_C2Q( "ң�ź�" )
		<<_C2Q( "����" )
		<<_C2Q( "ң��ֵ" )
		<<_C2Q( "ȡ��" )
		<<_C2Q( "��Ч" )
		<<_C2Q( "����λ" )
		<<_C2Q( "�仯ʱ����" )
		<<_C2Q( "�仯ʱ�����" )
		<<_C2Q( "������" );
	m_pYxTable->setHorizontalHeaderLabels( yxHeader );
	m_pYxTable->verticalHeader(  )->setVisible( false );

	m_pYxTable->setColumnWidth( 0, 50 );
	m_pYxTable->setColumnWidth( 1, 200 );
	m_pYxTable->setColumnWidth( 2, 100 );
	m_pYxTable->setColumnWidth( 3, 100 );
	m_pYxTable->setColumnWidth( 4, 50 );
	m_pYxTable->setColumnWidth( 5, 50 );
	m_pYxTable->setColumnWidth( 6, 100 );
	m_pYxTable->setColumnWidth( 7, 100 );
	m_pYxTable->setColumnWidth( 8, 50 );

	m_pKwhTable->setColumnCount( 7 );
	QStringList kwhHeader;
	kwhHeader<<_C2Q( "��Ⱥ�" )
		<<_C2Q( "����" )
		<<_C2Q( "���ֵ" )
		<<_C2Q( "��Ч" )
		<<_C2Q( "�仯ʱ����" )
		<<_C2Q( "�仯ʱ�����" )
		<<_C2Q( "������" );	
	m_pKwhTable->setHorizontalHeaderLabels( kwhHeader );
	m_pKwhTable->verticalHeader(  )->setVisible( false );

	m_pKwhTable->setColumnWidth( 0, 50 );
	m_pKwhTable->setColumnWidth( 1, 200 );
	m_pKwhTable->setColumnWidth( 2, 100 );
	m_pKwhTable->setColumnWidth( 3, 100 );
	m_pKwhTable->setColumnWidth( 4, 100 );
	m_pKwhTable->setColumnWidth( 5, 100 );
	m_pKwhTable->setColumnWidth( 6, 50 );
}

void QRawViewFrame::setPara( int rtuNo , int chan_no)
{
	if ( rtuNo < 0 || chan_no < 0 )
	{
		return;
	}

	int i;

	while(  m_tableLock  )
		;
	m_tableLock = 1;

	blockSignals(  true  );

	clearrdlist(  );

	Fes_shm& fesshm = Fes_shm::get();

	m_curRTU = rtuNo;
	m_curCHAN = chan_no;
	m_pYcTable->setRowCount( fesshm.rtu_para_ptr[rtuNo].yc_num );
	m_pYxTable->setRowCount( fesshm.rtu_para_ptr[rtuNo].yx_num );
	m_pKwhTable->setRowCount( fesshm.rtu_para_ptr[rtuNo].kwh_num );
	readYcPara(  );
	readYxPara(  );
	readKwhPara(  );
	blockSignals(  false  );
	m_rtuChanged = 1;
	m_tableLock = 0;
}


bool QRawViewFrame::readYcPara(  )
{
	CS_RETCODE retcode = 0; 
	int i;

	FUNC_STR func;
	memset(  &func,0, sizeof(  func  )  );
	CS_DATAFMT *datafmt=NULL;
	func.func = GET_TABLE_INFO;
	//func.groupno = 1;
	strcpy( func.table,"ģ������ͼ" );
	strcpy( func.dbname,DB_NAME );

	char * col_name[] = {"ң�����","����","��������־"};

	char str[100];
	sprintf( str,"�ն����=%d", m_curRTU );
	strcpy( func.isql,str );

	//printf( "readycpara:%s-%s, sizeof( FUNC_STR )=%d\n", func.dbname, func.table, sizeof( FUNC_STR ) );
	//fflush(  stdout  );
	void* pt = 0;


	char filename[256];
	if( !m_db.Open( DB_OP_R,FALSE,FALSE ) )
	{
// 		return false;
		sprintf( filename, "%s/dbmap/ģ������ͼ1/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return FALSE;

		sprintf( filename, "%s/dbmap/ģ������ͼ1/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		m_db.GetTableInfo( &func, NULL, NULL );

		sprintf( filename, "%s/dbmap/ģ������ͼ1/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return FALSE;

		int ptcnt = 0;
		int rtuNo,ycNo, isComputer;
		char desc[DESC_LEN];
		m_yc = new RTDATA[ func.ret_roxnum ];
		int i,j;
		j = 0;
		for(i=0; i<func.ret_roxnum; i++ )
		{
			rtuNo = ( int )m_db.GetData( i,&func,datafmt,"�ն����",pt,ptcnt,NULL );
			ycNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );

			if (rtuNo == m_curRTU)
			{
				m_yc[ j ].no = ycNo;
				m_yc[ j ].desc = _C2Q(  desc  );
				m_yc[ j ].iscomputer = isComputer;
				j++;
			}
		}
		m_ycNum = j;
		printf("yc num = %d\n",m_ycNum);

	}
	else
	{
		if( m_db.GetTableInfo( &func,&datafmt )==CS_FAIL )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			datafmt = NULL;
			return false;
		}

		int col_num = sizeof( col_name )/sizeof( char* );

		for( int i=0;i<func.colnum;i++ )
		{
			func.col[i].select = NO;
			func.col[i].order = -1;
			func.col[i].condition = NO;
			func.col[i].order_d = -1;
			for( int j=0; j<col_num; j++ )
			{
				if( strcasecmp( datafmt[i].name,col_name[j] ) )
				{
					continue;
				}
				if( strcasecmp( datafmt[i].name,"ң�����" )==0 )
				{
					func.col[i].order = 0;
				}
				func.col[i].select = YES;
				func.col[i].order_d = j;
				break;
			}
		}
		func.func = SEL_ISQL_RECORDERS;
		if( ( retcode = m_db.SelectRcds( &func,datafmt,&pt ) ) != CS_SUCCEED )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return false;
		}

		int lognum = func.ret_roxnum;
		if( lognum == 0  ) 
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return false;
		}

		int ptcnt = 0;
		int ycNo, isComputer;
		char desc[DESC_LEN];
		char computer[DESC_LEN];

		m_yc = new RTDATA[ func.ret_roxnum ];
		m_ycNum = func.ret_roxnum;
		for( i=0; i<func.ret_roxnum; i++ )
		{
			ycNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );

			m_yc[ i ].no = ycNo;
			m_yc[ i ].desc = _C2Q(  desc  );
			m_yc[ i ].iscomputer = isComputer;
		}

	}



	if( datafmt )
	{
		free( datafmt );
	}
	if ( pt )
	{
		free( pt );
	}
	datafmt = NULL;
	pt = NULL;

	m_db.Close(  );
	return true;
}

bool QRawViewFrame::readYxPara(  )
{
	CS_RETCODE retcode = 0; 
	int i;

	char filename[256];
	FUNC_STR func;
	CS_DATAFMT *datafmt;
	func.func = GET_TABLE_INFO;
	strcpy( func.table,"ң������ͼ" );
	strcpy( func.dbname, DB_NAME );

	char * col_name[] = {"ң�����", "����", "ȡ����־", "��������־"};

	char str[100];
	sprintf( str,"�ն����=%d", m_curRTU );
	strcpy( func.isql,str );
	void* pt = 0;


	if( !m_db.Open( DB_OP_R,FALSE,FALSE ) )
	{
// 		return false;
		sprintf( filename, "%s/dbmap/ң������ͼ1/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return FALSE;

		sprintf( filename, "%s/dbmap/ң������ͼ1/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;

		m_db.GetTableInfo( &func, NULL, NULL );

		sprintf( filename, "%s/dbmap/ң������ͼ1/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return FALSE;

		int ptcnt = 0;
		int rtuNo,yxNo, isComputer, isNegate;
		char desc[DESC_LEN];
		m_yx = new RTDATA[ func.ret_roxnum ];
		int i,j;
		j = 0;
		for(i=0; i<func.ret_roxnum; i++ )
		{
			rtuNo = ( int )m_db.GetData( i,&func,datafmt,"�ն����",pt,ptcnt,NULL );
			yxNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isNegate = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[3],pt,ptcnt,NULL );

			if (rtuNo == m_curRTU)
			{
				m_yx[ j ].no = yxNo;
				m_yx[ j ].desc = _C2Q(  desc  );
				m_yx[ j ].isnegate = isNegate;
				m_yx[ j ].iscomputer = isComputer;
				j++;
			}
		}
		m_yxNum = j;
		printf("yx num = %d\n",m_yxNum);
	}
	else
	{
		if( m_db.GetTableInfo( &func,&datafmt )==CS_FAIL )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			datafmt = NULL;
			return false;
		}

		int col_num = sizeof( col_name )/sizeof( char* );

		for( int i=0;i<func.colnum;i++ )
		{
			func.col[i].select = NO;
			func.col[i].order = -1;
			func.col[i].condition = NO;
			func.col[i].order_d = -1;
			for( int j=0; j<col_num; j++ )
			{
				if( strcasecmp( datafmt[i].name,col_name[j] ) )
				{
					continue;
				}
				if( strcasecmp( datafmt[i].name,"ң�����" )==0 )
				{
					func.col[i].order = 0;
				}
				func.col[i].select = YES;
				func.col[i].order_d = j;
				break;
			}
		}

		func.func = SEL_ISQL_RECORDERS;
		if( ( retcode = m_db.SelectRcds( &func,datafmt,&pt ) ) != CS_SUCCEED )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return false;
		}

		int lognum = func.ret_roxnum;
		if( lognum == 0  ) 
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return false;
		}

		int ptcnt = 0;
		int yxNo, isComputer, isNegate;
		char desc[DESC_LEN];
		char computer[DESC_LEN];
		char negate[DESC_LEN];

		m_yx = new RTDATA[ func.ret_roxnum ];
		m_yxNum = func.ret_roxnum;

		for( i=0; i<func.ret_roxnum; i++ )
		{
			yxNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isNegate = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[3],pt,ptcnt,NULL );

			m_yx[ i ].no = yxNo;
			m_yx[ i ].desc = _C2Q(  desc  );
			m_yx[ i ].isnegate = isNegate;
			m_yx[ i ].iscomputer = isComputer;
		}
	}



	if( datafmt )
	{
		free( datafmt );
	}
	if ( pt )
	{
		free( pt );
	}
	datafmt = NULL;
	pt = NULL;
	m_db.Close(  );

	return true;
}

bool QRawViewFrame::readKwhPara(  )
{
	CS_RETCODE retcode = 0; 
	int i;
	char filename[256];
	FUNC_STR func;
	CS_DATAFMT *datafmt;
	func.func = GET_TABLE_INFO;
	strcpy( func.table,"�����ͼ" );
	strcpy( func.dbname, DB_NAME );

	char * col_name[] = {"������","����","��������־"};

	char str[100];
	sprintf( str,"�ն����=%d", m_curRTU );
	strcpy( func.isql,str );
	void* pt = 0;


	if( !m_db.Open( DB_OP_R,FALSE,FALSE ) )
	{
// 		return false;
		sprintf( filename, "%s/dbmap/�����ͼ1/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return FALSE;

		sprintf( filename, "%s/dbmap/�����ͼ1/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;

		m_db.GetTableInfo( &func, NULL, NULL );

		sprintf( filename, "%s/dbmap/�����ͼ1/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return FALSE;

		int ptcnt = 0;
		int rtuNo,kwhNo, isComputer;
		char desc[DESC_LEN];
		char computer[DESC_LEN];
		int j = 0;

		m_kwh = new RTDATA[ func.ret_roxnum ];
		for( i=0; i<func.ret_roxnum; i++ )
		{
			rtuNo = ( int )m_db.GetData( i,&func,datafmt,"�ն����",pt,ptcnt,NULL );
			kwhNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );
			if (rtuNo == m_curRTU)
			{
				m_kwh[ j ].no = kwhNo;
				m_kwh[ j ].desc = _C2Q(  desc  );
				m_kwh[ j ].iscomputer = isComputer;
				j++;
			}
		}
		m_kwhNum = j;
		printf("m_kwhNum = %d\n",m_kwhNum);

	}
	else
	{
		if( m_db.GetTableInfo( &func,&datafmt )==CS_FAIL )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			datafmt = NULL;
			return false;
		}

		int col_num = sizeof( col_name )/sizeof( char* );

		for( int i=0;i<func.colnum;i++ )
		{
			func.col[i].select = NO;
			func.col[i].order = -1;
			func.col[i].condition = NO;
			func.col[i].order_d = -1;
			for( int j=0; j<col_num; j++ )
			{
				if( strcasecmp( datafmt[i].name,col_name[j] ) )
				{
					continue;
				}
				if( strcasecmp( datafmt[i].name,"������" )==0 )
				{
					func.col[i].order = 0;
				}
				func.col[i].select = YES;
				func.col[i].order_d = j;
				break;
			}
		}

		func.func = SEL_ISQL_RECORDERS;
		if( ( retcode = m_db.SelectRcds( &func,datafmt,&pt ) ) != CS_SUCCEED )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return false;
		}

		int lognum = func.ret_roxnum;
		if( lognum == 0  ) 
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return true;
		}
		int ptcnt = 0;
		int kwhNo, isComputer;
		char desc[DESC_LEN];
		char computer[DESC_LEN];


		m_kwh = new RTDATA[ func.ret_roxnum ];
		m_kwhNum = func.ret_roxnum;

		for( i=0; i<func.ret_roxnum; i++ )
		{
			kwhNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );

			m_kwh[ i ].no = kwhNo;
			m_kwh[ i ].desc = _C2Q(  desc  );
			m_kwh[ i ].iscomputer = isComputer;
		}
	}



	if( datafmt )
	{
		free( datafmt );
	}
	if ( pt )
	{
		free( pt );
	}
	datafmt = NULL;
	pt = NULL;
	m_db.Close(  );

	return true;
}

void QRawViewFrame::fillData( )
{
	int i;
	short ycValue;
	short  ycCode;
	unsigned char  ycValid;

	unsigned char  yxValue;
	unsigned char  yxValid;  

// 	uint  kwhValue;
	float kwhValue = 0.f;
	unsigned char  kwhValid;

	if ( -1 == m_curRTU )
	{
		return;
	}

	printf("fill data\n");

	while(  m_tableLock  )
		;
	m_tableLock = 1;
	blockSignals(  true  );

	PRTDATA pRD;
	QString computer;
	QString negate;
	if(  m_rtuChanged  )
	{
		m_pYcTable->clearContents(  );
		m_pYxTable->clearContents(  );
		m_pKwhTable->clearContents(  );
		for(  i = 0; i < m_ycNum; i ++  )
		{
			pRD = &m_yc[ i ];

			if(  pRD->no >= m_pYcTable->rowCount(  )  )
			{
				continue;
			}

			if (  1 == pRD->iscomputer  )
			{
				computer = _C2Q( "��" );
			}
			else 
			{
				computer = _C2Q( "��" );
			}

			m_pYcTable->setItem(  pRD->no, 0, new QTableWidgetItem( QString::number( pRD->no ) )  );
			m_pYcTable->setItem(  pRD->no, 1, new QTableWidgetItem( pRD->desc )  );
			m_pYcTable->setItem(  pRD->no, 6, new QTableWidgetItem(  computer  )  );
		}

		for(  i = 0; i < m_yxNum; i ++  )
		{
			pRD = &m_yx[ i ];

			if(  pRD->no >= m_pYxTable->rowCount(  )  )
			{
				continue;
			}

			if ( 1 == pRD->isnegate )
			{
				negate = _C2Q(  "ȡ��"  );
			}
			else 
			{
				negate = _C2Q(  "��ȡ��" );
			}

			if (  1 == pRD->iscomputer  )
			{
				computer = _C2Q( "��" );
			}
			else 
			{
				computer = _C2Q( "��" );
			}

			m_pYxTable->setItem( pRD->no, 0, new QTableWidgetItem( QString::number( pRD->no ) ) );
			m_pYxTable->setItem( pRD->no, 1, new QTableWidgetItem( pRD->desc ) );
			m_pYxTable->setItem( pRD->no, 3, new QTableWidgetItem( negate ) );
			m_pYxTable->setItem( pRD->no, 8, new QTableWidgetItem( computer ) );
		}

		for(  i = 0; i < m_kwhNum; i ++  )
		{
			pRD = &m_kwh[ i ];

			if(  pRD->no >= m_pKwhTable->rowCount(  )  )
			{
				continue;
			}

			if (  1 == pRD->iscomputer  )
			{
				computer = _C2Q( "��" );
			}
			else 
			{
				computer = _C2Q( "��" );
			}

			m_pKwhTable->setItem( pRD->no, 0, new QTableWidgetItem( QString::number( pRD->no ) ) );
			m_pKwhTable->setItem( pRD->no, 1, new QTableWidgetItem( pRD->desc ) );
			m_pKwhTable->setItem( pRD->no, 6, new QTableWidgetItem( computer ) );
		}

		clearrdlist(  );
		m_rtuChanged = 0;
	}



	Fes_shm& fesshm = Fes_shm::get();
	Fes_data_shm_accessor&	fesdatashm = Fes_data_shm_accessor::get();


	float fYcValue = 0.f;
	unsigned int uiYcCode = 0;
	tm procsec ;
	unsigned int procmsec = 0;
	int rtuyc = fesdatashm.rtu_yc( m_curRTU ) ;
	int rtuyx = fesdatashm.rtu_yx( m_curRTU ) ;
	int rtukwh = fesdatashm.rtu_kwh( m_curRTU ) ;



	FE_YC* yc_ptr = fesdatashm.channel_yc(m_curCHAN) + rtuyc;
	FE_YX* yx_ptr = fesdatashm.channel_yx(m_curCHAN) + rtuyx;
	FE_KWH* kwh_ptr = fesdatashm.channel_kwh(m_curCHAN) + rtukwh;

	if (!yc_ptr || !yx_ptr || !kwh_ptr)
	{
		m_tableLock = 0;
		return;
	}

	for ( i = 0 ; i < fesshm.rtu_para_ptr[ m_curRTU ].yc_num ; i++ )
	{
		fYcValue = yc_ptr->raw_yc ;
		m_pYcTable->setItem( i, 2,  new QTableWidgetItem( QString::number( fYcValue ) ) );

		// 		ycValid = fesdatashm.fe_data()->data_seg.yc[ rtuyc + i ].modifier.quality ;
		if ( FLOAT_DEFAULT == fYcValue )
		{
			m_pYcTable->setItem( i, 3, new QTableWidgetItem( _C2Q( "��Ч" ) ) );
		}
		else 
		{
			m_pYcTable->setItem( i, 3, new QTableWidgetItem( _C2Q( "��Ч" ) ) );
		}

		procsec.tm_sec = yc_ptr->modifier.time.second;
		procsec.tm_min = yc_ptr->modifier.time.minute ;
		procsec.tm_hour = yc_ptr->modifier.time.hour ;
		procsec.tm_mday = yc_ptr->modifier.time.day ;
		procsec.tm_mon = yc_ptr->modifier.time.month ;
		procsec.tm_year = yc_ptr->modifier.time.year ;
// 		m_pYcTable->setItem( i, 4, new QTableWidgetItem( QString::number( mktime( &procsec ) ) ) );
		char str1[ 128 ] ;
		sprintf( str1 , "%04d/%02d/%02d/ %02d:%02d:%02d" , procsec.tm_year + 1900 , procsec.tm_mon + 1 , procsec.tm_mday , procsec.tm_hour , procsec.tm_min , procsec.tm_sec ) ;
		m_pYcTable->setItem( i, 4, new QTableWidgetItem( tr( str1 ) ) ) ;
		procmsec = fesdatashm.fe_data()->data_seg.yc[ rtuyc + i ].modifier.time.milisecond ;
		m_pYcTable->setItem( i, 5, new QTableWidgetItem( QString::number( procmsec ) ) );

		yc_ptr++;
	} //for ( i = 0 ; i < fesshm.rtu_para_ptr[ m_curRTU ].yc_num ; i++ )


	for ( i = 0 ; i < fesshm.rtu_para_ptr[ m_curRTU ].yx_num ; i++ )
	{
		yxValue = yx_ptr->raw_yx ;
		if ( 1 == yxValue  )
		{
			QTableWidgetItem* newitem = new QTableWidgetItem( _C2Q( "��" ) );
			newitem->setTextColor( QColor( 255, 0, 0 ) );
			m_pYxTable->setItem( i, 2, newitem );
		}
		else
		{
			m_pYxTable->setItem( i, 2,  new QTableWidgetItem( _C2Q( "��" ) ) );
		}

		if ( UCHAR_DEFAULT == yxValue )
		{
			m_pYxTable->setItem( i, 4, new QTableWidgetItem( _C2Q( "��Ч" ) ) );
		}
		else 
		{
			m_pYxTable->setItem( i, 4, new QTableWidgetItem( _C2Q( "��Ч" ) ) );
		}
		yxValid = yx_ptr->modifier.quality ;
		if(yxValid)
		{
			m_pYxTable->setItem( i, 5, new QTableWidgetItem( _C2Q( "��Ч" ) ) );
		}
		else
		{
			m_pYxTable->setItem( i, 5, new QTableWidgetItem( _C2Q( "��Ч" ) ) );
		}


		procsec.tm_sec = yx_ptr->modifier.time.second ;
		procsec.tm_min = yx_ptr->modifier.time.minute ;
		procsec.tm_hour = yx_ptr->modifier.time.hour ;
		procsec.tm_mday = yx_ptr->modifier.time.day ;
		procsec.tm_mon = yx_ptr->modifier.time.month ;
		procsec.tm_year = yx_ptr->modifier.time.year ;
// 		m_pYxTable->setItem( i, 6, new QTableWidgetItem( QString::number( mktime( &procsec ) ) ) );
		char str1[ 128 ] ;
		sprintf( str1 , "%04d/%02d/%02d/ %02d:%02d:%02d" , procsec.tm_year + 1900 , procsec.tm_mon + 1 , procsec.tm_mday , procsec.tm_hour , procsec.tm_min , procsec.tm_sec ) ;
		m_pYxTable->setItem( i, 6, new QTableWidgetItem( tr( str1 ) ) ) ;
		procmsec = yx_ptr->modifier.time.milisecond ;
		m_pYxTable->setItem( i, 7, new QTableWidgetItem( QString::number( procmsec ) ) );

		yx_ptr++;
	} //for ( i = 0 ; i < fesshm.rtu_para_ptr[ m_curRTU ].yx_num ; i++ )


	for ( i = 0 ; i < fesshm.rtu_para_ptr[ m_curRTU ].kwh_num ; i++ )
	{
		kwhValue = kwh_ptr->raw_kwh ;
		m_pKwhTable->setItem( i, 2,  new QTableWidgetItem( QString::number( kwhValue ) ) );

		kwhValid = kwh_ptr->modifier.quality ;
		if ( ULONG_DEFAULT == kwhValue )
		{
			m_pKwhTable->setItem( i, 3, new QTableWidgetItem( _C2Q( "��Ч" ) ) );
		}
		else 
		{
			m_pKwhTable->setItem( i, 3, new QTableWidgetItem( _C2Q( "��Ч" ) ) );
		}

		procsec.tm_sec = kwh_ptr->modifier.time.second ;
		procsec.tm_min = kwh_ptr->modifier.time.minute ;
		procsec.tm_hour = kwh_ptr->modifier.time.hour ;
		procsec.tm_mday = kwh_ptr->modifier.time.day ;
		procsec.tm_mon = kwh_ptr->modifier.time.month ;
		procsec.tm_year = kwh_ptr->modifier.time.year ;
// 		m_pKwhTable->setItem( i, 4, new QTableWidgetItem( QString::number( mktime( &procsec ) ) ) );
		char str1[ 128 ] ;
		sprintf( str1 , "%04d/%02d/%02d/ %02d:%02d:%02d" , procsec.tm_year + 1900 , procsec.tm_mon + 1 , procsec.tm_mday , procsec.tm_hour , procsec.tm_min , procsec.tm_sec ) ;
		m_pKwhTable->setItem( i, 4, new QTableWidgetItem( tr( str1 ) ) ) ;
		procmsec = kwh_ptr->modifier.time.milisecond ;
		m_pKwhTable->setItem( i, 5, new QTableWidgetItem( QString::number( procmsec ) ) );

		kwh_ptr++;
	} //for ( i = 0 ; i < fesshm.rtu_para_ptr[ m_curRTU ].kwh_num ; i++ )

	blockSignals(  false  );
	m_tableLock = 0;

	printf("end fill  data\n");

}

void QRawViewFrame::clearrdlist(  )
{
	if(  m_yc  )
	{
		delete [] m_yc;
		m_yc = NULL;
	}
	m_ycNum = 0;

	if(  m_yx  )
	{
		delete [] m_yx;
		m_yx = NULL;
	}
	m_yxNum = 0;

	if(  m_kwh  )
	{
		delete [] m_kwh;
		m_kwh = NULL;
	}
	m_kwhNum = 0;
}


bool QRawViewFrame::readFile( char *filename, void ** buf )
{
	struct stat sf;

	if( stat( filename, &sf )!=0 )
	{
		return FALSE;
	}

	FILE *fp;
	int ret;

	if( sf.st_size == 0 )
		return TRUE;

	fp = fopen( filename, "rb" );
	if( fp == NULL )
		return FALSE;
	*buf =  (char*)malloc( sf.st_size );
	ret = fread( *buf, sf.st_size, 1, fp );

	fclose( fp );
	fp = NULL;
	if( ret != 1 )
	{
		return FALSE;
	}

	return TRUE;

}

bool QRawViewFrame::readFile( char *filename, void * buf, int len )
{
	struct stat sf;

	if( stat( filename, &sf )!=0 )
	{
		return FALSE;
	}

	FILE *fp;
	int ret;

	fp = fopen( filename, "rb" );
	if( fp == NULL )
		return FALSE;
	ret = fread( buf, len, 1, fp );

	fclose( fp );
	fp = NULL;
	if( ret != 1 )
	{
		return FALSE;
	}

	return TRUE;
}