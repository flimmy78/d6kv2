#include "event_para.h"
#include <sys/stat.h>

#define BUFF_LEN 1024
char *EventSortName[] = 
{
	"δ����",
	"ϵͳ�е��豸�����������¼�",
	"SCADA�¼�",
	"�ۺ��Զ��������¼�",
	"����¼�",
	"DAӦ���¼�",
	"DA�о�̬Ӧ���¼�",
	"EMSӦ���¼�",
	"�����¼�",
	"֪ͨ�¼�",
	"WEBӦ���¼�",
	"ά����Ӧ���¼�",
};


Event_para::Event_para()
{
	m_rfile = false;
}

Event_para::~Event_para()
{

}

bool Event_para::readPara()
{
	bool ret;

	printf( "readPara start\n" );
	fflush( stdout );

	if( !m_db.Open( DB_OP_R, false , false ) )
	{
		m_rfile = true;
	}

	printf( "readPara end\n" );
	fflush( stdout );

	if( !readEventType() ||!readGroupPara() ||!readStationPara() ||!readRtuPara() ||!readYcPara() ||!readYxPara() ||!readDevPara() ||!readDevTypePara()||!readAreaPara() ) 
	{
		ret = false;
	}
	else
	{
		ret = true;
	}

#ifndef _HISEVENT_VERSION
	if( !readNodeTable() ||!readConfirmEvent() )
	{
		ret = false;
	}
	else
	{
		ret = true;
	}
#endif

	if( !m_rfile )
		m_db.Close();
	else
	{
		m_rfile = false;
	}	

	return ret;
}

uint Event_para::getGroupCount()
{
	uint count;

	count = m_groupPara.count();

	return count;
}

bool Event_para::getGroupDesc( uint id, QString &desc )
{
	bool ret;


	PGROUPPARA pGroupPara = m_groupPara.at( id );
	if( pGroupPara == NULL )
	{
		ret = false;
	}
	else
	{
		ret = true;
		desc = pGroupPara->desc;
	}
	return ret;
}

bool Event_para::getGroupNo( uint id, uint &no )
{
	bool ret;
	PGROUPPARA pGroupPara = m_groupPara.at( id );
	if( pGroupPara == NULL )
	{
		ret = false;
	}
	else
	{
		ret = true;
		no = pGroupPara->no;
	}
	return ret;
}

bool Event_para::getStaCode( uint groupno, QStringList &stations )
{
	bool ret = false;
	uint i;
	PSTATIONPARA pStaPara;

	for( i = 0 ; i < m_stationPara.count(); i ++ )
	{
		pStaPara = m_stationPara.at( i );
		if( groupno != pStaPara->groupno )
			continue;

		stations.append( pStaPara->code );
	}

	if( stations.count() > 0 )
		ret = true;

	return ret;
}


bool Event_para::getStaDesc( const QString &code, QString &desc )
{
	bool ret = false;
	uint i;
	PSTATIONPARA pStaPara;

	for( i = 0 ; i < m_stationPara.count(); i ++ )
	{
		pStaPara = m_stationPara.at( i );
		if( code == pStaPara->code )
		{
			desc = pStaPara->desc;
			ret = true;
			break;
		}
	}

	return ret;
}

bool Event_para::getYcCode( const QString &rtucode, QStringList &yccode )
{
	bool ret = false;
	uint i;

	PYCPARA pYcPara;

	for( i = 0 ; i < m_ycPara.count(); i ++ )
	{
		pYcPara = m_ycPara.at( i );

		if( rtucode == pYcPara->stationcode )
		{
			yccode.append( pYcPara->code );
			ret = true;
		}
	}

	return ret;
}

bool Event_para::getYcDesc( const QString &code, QString &desc )
{
	bool ret = false;
	YCPARA *pYcPara;

	if( m_ycCode2YcParaMap.find( code ) != 
		m_ycCode2YcParaMap.end() )
	{
		pYcPara = m_ycCode2YcParaMap[ code ];
		desc = pYcPara->desc;
		ret = true;
	}

	return ret;
}

bool Event_para::getYxCode( const QString &rtucode, QStringList &yxcode )
{
	bool ret = false;
	uint i;

	PYXPARA pYxPara;

	for( i = 0 ; i < m_yxPara.count(); i ++ )
	{
		pYxPara = m_yxPara.at( i );

		if( rtucode == pYxPara->stationcode )
		{
			yxcode.append( pYxPara->code );
			ret = true;
		}
	}

	return ret;
}

bool Event_para::getYxDesc( const QString &code, QString &desc )
{
	bool ret = false;
	YXPARA *pYxPara;

	if( m_yxCode2YxParaMap.find( code ) != 
		m_yxCode2YxParaMap.end() )
	{
		pYxPara = m_yxCode2YxParaMap[ code ];
		desc = pYxPara->desc;
		ret = true;
	}

	return ret;
}

bool Event_para::readGroupPara()
{
	qDeleteAll(m_groupPara) ;
	m_groupPara.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "�������" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy( func.table, "�������" );
	strcpy( func.dbname, "modeldb" );
	char * col_name[] = { "���", "����", "����" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/�������3/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return false;
		
		sprintf( filename, "%s/dbmap/�������3/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return false;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/�������3/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return false;
	}
	else
	{
		if( m_db.GetTableInfo( &func,&datafmt ) == CS_FAIL )
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return false;
		}
		if( func.colnum < 1 || !datafmt )
		{
			if( datafmt ) 
				free( datafmt );
			
			m_db.Close();	
			return false;
		}
		int   col_num = sizeof( col_name ) / sizeof( char* );
		
		for( i = 0; i < func.colnum ; i ++ )
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition = NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}
	}
	

	PGROUPPARA pGroup;
	int count ;
	char	tmpbuf[ BUFF_LEN ];

    for( i = 0; i < func.ret_roxnum; i ++ )
	{	
		pGroup = new GROUPPARA;
		count = 0;
		pGroup->no = (int)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pGroup->code = tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pGroup->desc = tr( tmpbuf );
		m_groupPara.append( pGroup );
    }


    if( func.roxnum != 0 ) 
		free( pt );

    if( datafmt ) 
		free( datafmt );

    return true;
}


bool Event_para::readRtuPara()
{
	qDeleteAll(m_rtuPara);
	m_rtuPara.clear();
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;


	printf( "�ն˲�����" );
	fflush( stdout );
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"�ն˲�����");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = {"����","����","�ն����","վ����ϵ����"};

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/�ն˲�����1/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return false;
		
		sprintf( filename, "%s/dbmap/�ն˲�����1/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return false;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/�ն˲�����1/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return false;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return false;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return false;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PRTUPARA pRtu;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pRtu = new RTUPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pRtu->code = tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pRtu->desc = tr( tmpbuf );
		pRtu->no = (int)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);

		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pRtu->rtucode = tr( tmpbuf );

		m_rtuno2StaCode[ pRtu->no ] = pRtu->rtucode;

		m_rtuPara.append( pRtu );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);
	return true;
}


bool Event_para::readYcPara()
{
	qDeleteAll(m_ycPara);
	m_ycPara.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "ң�������" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"ң�������");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "����","����","����","�豸����","�ն����" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/ң�������/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return false;
		
		sprintf( filename, "%s/dbmap/ң�������/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return false;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/ң�������/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return false;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return false;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return false;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PYCPARA pYc;
	uint	rtuno;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pYc = new YCPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYc->code = tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYc->desc = tr( tmpbuf );
		pYc->type = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYc->devcode = tr( tmpbuf );
		rtuno = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		if( m_rtuno2StaCode.find( rtuno ) != m_rtuno2StaCode.end() )
		{
			pYc->stationcode = m_rtuno2StaCode[ rtuno ];
		}
		m_ycPara.append( pYc );
		m_ycCode2YcParaMap[ pYc->code ] = pYc;
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return true;
}

bool Event_para::readYxPara()
{
	qDeleteAll(m_yxPara);
	m_yxPara.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "ң�Ų�����" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"ң�Ų�����");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "����","����","����","�豸����","�ն����" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/ң�Ų�����/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return false;
		
		sprintf( filename, "%s/dbmap/ң�Ų�����/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return false;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/ң�Ų�����/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return false;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return false;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return false;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PYXPARA pYx;
	uint	rtuno;

	printf( "222222222222" );
	fflush( stdout );

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pYx = new YXPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYx->code = tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYx->desc = tr( tmpbuf );
		pYx->type = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYx->devcode = tr( tmpbuf );
		rtuno = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		if( m_rtuno2StaCode.find( rtuno ) != m_rtuno2StaCode.end() )
		{
			pYx->stationcode = m_rtuno2StaCode[ rtuno ];
		}

		m_yxPara.append( pYx );

		m_yxCode2YxParaMap[ pYx->code ] = pYx;
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return true;
}

bool Event_para::readEventType()
{
	qDeleteAll(m_eventTypePara);
	m_eventTypePara.clear();
	int i;

	printf( "�������ͱ�" );
	fflush( stdout );

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	char * col_name[] = { "�����","���ͺ�","������" };

	memset(&func,0,sizeof(FUNC_STR));
	for(int iOrder = 0 ; iOrder<MAX_COLNUM ;iOrder++)
	{
		func.col[iOrder].order = -1;
		func.col[iOrder].order_d = -1;
	}

	func.func=GET_TABLE_INFO;
	strcpy(func.table,"�������ͱ�");
	strcpy(func.dbname,"modeldb");
	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/�������ͱ�1/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return false;
		
		sprintf( filename, "%s/dbmap/�������ͱ�1/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return false;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/�������ͱ�1/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return false;
	}
	else
	{
	
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return false;
		}

		if (func.colnum<1) 
		{
			free(datafmt);
			m_db.Close();	
			return false;
		}
		
		for ( i=0;i<func.colnum;i++)
		{
			func.col[i].select=NO;
			func.col[i].order=-1;
			func.col[i].condition=NO;
			func.col[i].order_d = -1;
		}

		for (i=0;i<func.colnum;i++)
		{
			if(strcmp(datafmt[ i ].name,"�����")==0)
			{
				func.col[i].select=YES;
				func.col[i].order_d = 0;
			}
			else if(strcmp(datafmt[ i ].name,"���ͺ�")==0) 
			{
				func.col[i].select=YES;
				func.col[i].order=0;
				func.col[i].order_d = 1;
			}
			else if(strcmp(datafmt[ i ].name,"������")==0) 
			{
				func.col[i].select=YES;
				func.col[i].order_d = 2;
			}
		}

		func.func=SEL_RECORDERS;
		
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}
	}

	PEVENTTYPEDEF pEventTypeDef;
	int count;
	char tmpbuf[ BUFF_LEN ];

	for( i = 0; i < func.ret_roxnum; i ++ )
	{
		count = 0;
		pEventTypeDef = new EVENTTYPEDEF ;
		pEventTypeDef->sort = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		pEventTypeDef->type = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		m_eventTypePara.append( pEventTypeDef );

		m_typeDescMap[ pEventTypeDef->type ] = tr( tmpbuf );
		m_descTypeMap[ tr( tmpbuf ) ] = pEventTypeDef->type;
	}

	if(datafmt)
		free(datafmt);
	return true;
}

uint Event_para::getEventSortCount()
{
	return EVENT_SORT_NUM;
}

bool Event_para::getEventSortDesc( uint sort, QString &desc )
{
	bool ret ;

	if( sort >= EVENT_SORT_NUM )
	{
		ret = false;
	}
	else
	{
		desc = tr( EventSortName[ sort ] );
		ret = true;
	}

	return ret;
}

uint Event_para::getEventTypeCount()
{
	uint count;
	count = m_eventTypePara.count();

	return count;
}

bool Event_para::getEventTypeDesc( uint type, QString &desc )
{
	bool ret = false;

	if( m_typeDescMap.find( type ) != m_typeDescMap.end() )
	{
		desc = m_typeDescMap[ type ];
		ret = true;
	}

	return ret;
}

bool Event_para::getEventTypes( uint sort, uint *types, uint &num )
{
	PEVENTTYPEDEF pEventTypeDef;

	num = 0;

	for( uint i = 0 ; i < m_eventTypePara.count(); i ++ )
	{
		pEventTypeDef = m_eventTypePara.at( i );

		if( pEventTypeDef->sort == sort )
		{
			types[ num ++ ] = pEventTypeDef->type;
		}
	}

	if( num == 0 )
		return false;

	return true;
}

PEVENTTYPEDEF Event_para::getEventType( uint id )
{
	PEVENTTYPEDEF pEventType;

	pEventType = m_eventTypePara.at( id );

	return pEventType;
}

uint Event_para::getEventTypeByDesc( QString &desc )
{
	if( m_descTypeMap.find( desc ) == m_descTypeMap.end() )
		return 0;

	return m_descTypeMap[ desc ];
}

bool Event_para::readDevPara()
{
	qDeleteAll(m_devPara);
	m_devPara.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "SCD�豸������" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"SCD�豸������");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "����","����","վ����ϵ����", "����" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/SCD�豸������1/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return false;
		
		sprintf( filename, "%s/dbmap/SCD�豸������1/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return false;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/SCD�豸������1/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return false;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return false;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return false;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PDEVPARA pDev;
	QStringList *staObjList;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pDev = new DEVPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDev->code = tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDev->desc = tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDev->stationcode = tr( tmpbuf );
		pDev->type = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		
		m_devCode2DevParaMap[ pDev->code ] = pDev;
		m_devPara.append( pDev );
		if( m_staCode2ObjCodesMap.contains( pDev->stationcode ) )
		{
			staObjList = m_staCode2ObjCodesMap[ pDev->stationcode ]; 
		}
		else
		{
			staObjList = new QStringList;
			m_staCode2ObjCodesMap[ pDev->stationcode ] = staObjList;
		}
		staObjList->append( pDev->code );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return true;
}

bool Event_para::getDevCode( const QString &rtucode, QStringList &devcode )
{
	bool ret = false;

	PDEVPARA pDevPara;

	if( m_staCode2ObjCodesMap.contains( rtucode ) )
	{
		devcode = *( m_staCode2ObjCodesMap[ rtucode ] );
		ret = true;
	}

	return ret;
}

bool Event_para::getDevCode( const QString &rtucode, uint devtype, QStringList &devcode )
{
	bool ret = false;

	PDEVPARA pDevPara;

	QStringList *staObjList;
	QString objName;
	uint i;

	if( !m_staCode2ObjCodesMap.contains( rtucode ) )
		return false;

	staObjList = m_staCode2ObjCodesMap[ rtucode ];

	for( i = 0; i < staObjList->count(); i ++ )
	{
		objName = (*staObjList)[ i ];
		pDevPara = getDev( objName );
		if( devtype == pDevPara->type )
		{
			devcode.append( pDevPara->code );
			ret = true;
		}
	}

	return ret;
}

bool Event_para::readDevTypePara()
{
	qDeleteAll(m_devTypePara);
	m_devTypePara.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "SCD�豸���ͱ�" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"SCD�豸���ͱ�");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "����","����" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/SCD�豸���ͱ�/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return false;
		
		sprintf( filename, "%s/dbmap/SCD�豸���ͱ�/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return false;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/SCD�豸���ͱ�/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return false;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return false;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return false;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PDEVTYPEPARA pDevType;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pDevType = new DEVTYPEPARA ;
		pDevType->devtype = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDevType->desc = tr( tmpbuf );
		
		m_devTypePara.append( pDevType );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return true;
}

uint Event_para::getDevTypeNum()
{
	return m_devTypePara.count();
}

bool Event_para::getDevType( uint id, PDEVTYPEPARA devtype )
{
	bool ret  = false;

	if( id >= m_devTypePara.count() )
		return false;

	devtype->desc = m_devTypePara.at( id )->desc;
	devtype->devtype = m_devTypePara.at( id )->devtype;

	return true;
}

bool Event_para::getDevDesc( const QString &devcode, QString &devdesc )
{
	bool ret = false;
	DEVPARA *pDev;

	if( m_devCode2DevParaMap.contains( devcode ) )
	{
		pDev = m_devCode2DevParaMap[ devcode ];
		devdesc = pDev->desc;
		ret = true;
	}

	return ret;
}

bool Event_para::getDevRealCode( const QString &devcode, QStringList &codes )
{
	bool ret = false;

	PYCPARA pYcPara;
	uint i;

	for( i = 0 ; i < m_ycPara.count(); i ++ )
	{
		pYcPara = m_ycPara.at( i );

		if( devcode == pYcPara->devcode )
		{
			codes.append( pYcPara->code );
			ret = true;
		}
	}

	PYXPARA pYxPara;

	for( i = 0 ; i < m_yxPara.count(); i ++ )
	{
		pYxPara = m_yxPara.at( i );

		if( devcode == pYxPara->devcode )
		{
			codes.append( pYxPara->code );
			ret = true;
		}
	}
	return ret;
}

PDEVPARA Event_para::getDev( const QString &devcode )
{
	if( !m_devCode2DevParaMap.contains( devcode ) )
		return NULL;

	return m_devCode2DevParaMap[ devcode ];
}

bool Event_para::readNodeTable()
{
	m_hostIPList.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	NetStatus		netstatus;

	QString			hostname;

	hostname = tr( netstatus.GetHostName( netstatus.GetHostNo() ) ) ;

	printf( "����ڵ������" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"����ڵ������");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "������","A��IP��ַ","B��IP��ַ" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/����ڵ������1/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return false;
		
		sprintf( filename, "%s/dbmap/����ڵ������1/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return false;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/����ڵ������1/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return false;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return false;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return false;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for( i = 0; i < func.colnum; i ++ )
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcasecmp(datafmt[i].name,col_name[j]))	
					continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		tmpbuf[ 0 ] = '\0';
		m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );

		if( hostname == tr( tmpbuf ) )
		{
			m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
			m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
			continue;
		}

		tmpbuf[ 0 ] = '\0';
		m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
		if( !tr( tmpbuf ).isEmpty() )
		{
			m_hostIPList.append( tr( tmpbuf ) );
		}
		
		tmpbuf[ 0 ] = '\0';
		m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
		if( !tr( tmpbuf ).isEmpty() )
		{
			m_hostIPList.append( tr( tmpbuf ) );
		}
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return true;
}

uint Event_para::getIPNum()
{
	return m_hostIPList.count();
}

bool Event_para::getIPByID( uint id, QString &ip )
{
	if( id >= m_hostIPList.count() )
		return false;

	ip = m_hostIPList[ id ];

	return true;
}

bool Event_para::readConfirmEvent()
{
	m_confirmEventType.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "����ȷ�����ͱ�" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"����ȷ�����ͱ�");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "���ͺ�" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/����ȷ�����ͱ�1/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return false;
		
		sprintf( filename, "%s/dbmap/����ȷ�����ͱ�1/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return false;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/����ȷ�����ͱ�1/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return false;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return false;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return false;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}
	}

	int count;
	uint	type;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		type = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_confirmEventType.append( type );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return true;
}

bool Event_para::isConfirmEventType( uint type )
{
	if( m_confirmEventType.indexOf( type ) != 
		m_confirmEventType.last() )
		return true;

	return false;
}

bool Event_para::getYxType( const QString &code, uint &type )
{
	bool ret = false;
	YXPARA *pYxPara;

	if( m_yxCode2YxParaMap.find( code ) != 
		m_yxCode2YxParaMap.end() )
	{
		pYxPara = m_yxCode2YxParaMap[ code ];
		type = pYxPara->type;
		ret = true;
	}

	return ret;
}

bool Event_para::readFile( char *filename, void ** buf )
{
	struct stat sf;
	
	if( stat( filename, &sf )!=0 )
	{
		return false;
	}
	
	FILE *fp;
	int ret;
	
	if( sf.st_size == 0 )
		return TRUE;
	
	fp = fopen( filename, "rb" );
	if( fp == NULL )
		return false;
	*buf =  (char*)malloc( sf.st_size );
	ret = fread( *buf, sf.st_size, 1, fp );
	
	fclose( fp );
	fp = NULL;
	if( ret != 1 )
	{
		return false;
	}
	
	return true;
	
}

bool Event_para::readFile( char *filename, void * buf, int len )
{
	struct stat sf;
	
	if( stat( filename, &sf )!=0 )
	{
		return false;
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
		return false;
	}
	
	return true;
}



bool Event_para::readAreaPara()
{	
	m_areaStringList.clear();
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;


	printf( "STATIONAREA" );
	fflush( stdout );
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"STATIONAREA");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = {"ID","NAME","MASTERID"};

	datafmt = 0;
	void *pt=0;

	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return false;
	}
	if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return false;
	}

	int   col_num = sizeof(col_name)/sizeof(char*);
	for(i=0;i<func.colnum;i++)
	{
		func.col[i].select = NO;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=NO;
		for(j=0; j<col_num; j++)
		{
			if(strcmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = YES;
			func.col[i].order_d = j;
		}
		if(strcmp(datafmt[i].name,col_name[0])==0)
			func.col[i].order = 0;
	}
	func.func = SEL_RECORDERS;
	if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return false;
	}

	if(func.ret_roxnum ==0)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return false;
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PAREAPARA pAreaPara;

	for( i = 0; i < func.ret_roxnum; i++  )
	{	
		QStringList areaList;
		count = 0;
		pAreaPara = new AREAPARA ;

		pAreaPara->areano = (int)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		areaList.append(QString::number(pAreaPara->areano));

		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pAreaPara->desc = tr( tmpbuf );
		areaList.append(pAreaPara->desc);

		pAreaPara->masterareano = (int)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		areaList.append(QString::number(pAreaPara->masterareano));

		m_areaStringList.append(areaList);
	}
	if (func.roxnum != 0) free(pt);
	if (datafmt) free(datafmt);
	return true;
}

QList<QStringList> Event_para::getAreaPara()
{
	return m_areaStringList ;
}


bool Event_para::readStationPara()
{
	m_stationStringList.clear();
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	char * station_colName[] = {"����","����","�������","�������"};

	printf( "վ����ϵ������" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"վ����ϵ������");
	strcpy(func.dbname,"modeldb");

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/վ����ϵ������1/datafmt", getenv( NBENV ) );
		if( !readFile(filename, (void**)(&datafmt) ) )
			return false;

		sprintf( filename, "%s/dbmap/վ����ϵ������1/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return false;

		m_db.GetTableInfo( &func, NULL, NULL );

		sprintf( filename, "%s/dbmap/վ����ϵ������1/pt", getenv( NBENV ) );
		if( !readFile(filename, &pt ) )
			return false;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return false;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return false;
		}

		int   col_num = sizeof(station_colName)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,station_colName[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
			if(strcmp(datafmt[i].name,station_colName[1])==0)
				func.col[i].order = 0;
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return false;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PSTATIONPARA pSta;

	for( i = 0; i < func.ret_roxnum; i++  )
	{	
		QStringList stationPara;		
		count = 0;
		pSta = new STATIONPARA ;
		m_db.GetData(i,&func,datafmt,station_colName[count++],pt,ptcnt,tmpbuf);
		pSta->code = tr( tmpbuf );
		stationPara.append(tr( tmpbuf ));
		m_db.GetData(i,&func,datafmt,station_colName[count++],pt,ptcnt,tmpbuf);
		pSta->desc = tr( tmpbuf );
		stationPara.append(tr( tmpbuf ));
		pSta->groupno = (uint)m_db.GetData(i,&func,datafmt,station_colName[count++],pt,ptcnt,NULL);
		stationPara.append(QString::number( pSta->groupno ));
		pSta->areano = (uint)m_db.GetData(i,&func,datafmt,station_colName[count++],pt,ptcnt,NULL);
		stationPara.append(QString::number( pSta->areano ));
		m_stationStringList.append(stationPara);
	}
	if (func.roxnum != 0) free(pt);
	if (datafmt) free(datafmt);
	return true;
}

QList<QStringList> Event_para::getStationPara()
{
	return m_stationStringList ;
}
