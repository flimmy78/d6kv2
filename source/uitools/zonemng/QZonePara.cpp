// QZonePara.cpp: implementation of the QZonePara class.
//
//////////////////////////////////////////////////////////////////////

#include "QZonePara.h"
#include "globaldef.h"
#include "sysdef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


QZonePara::QZonePara()
{
// 	m_stationList.setAutoDelete( true );
// 	m_userList.setAutoDelete( true );
// 	m_devList.setAutoDelete( true );
// 	m_devTypeList.setAutoDelete( true );
}

QZonePara::~QZonePara()
{

}

bool QZonePara::readPara()
{
	bool ret;
	
	printf( "readPara start\n" );
	fflush( stdout );
	
	if( m_db.Open( DB_OP_R, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}
	
	printf( "readPara end\n" );
	fflush( stdout );
	
	if( !readStationPara() ||
		!readDevPara() ||
		!readDevTypePara() ||
		!readNodeTable() ||
		!readUserPara() ||
		!readZonePara()||!readStationAreaPara() ) 
	{
		ret = false;
	}
	else
	{
		ret = true;
	}
	
	m_db.Close();
	
	return ret;
}

bool QZonePara::savePara()
{
	bool ret;
	
	printf( "savePara start\n" );
	fflush( stdout );
	
	if( m_db.Open( DB_OP_WR, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}
	
	printf( "readPara end\n" );
	fflush( stdout );
	
	if( !saveDevPara() ||
		!saveUserPara() ||
		!saveZonePara() ) 
	{
		ret = false;
	}
	else
	{
		ret = true;
	}
	
	m_db.Close();
	
	return ret;
}

bool QZonePara::readStationPara()
{
	m_stationList.clear();
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;
    m_stanametoid.clear();
	printf( "վ����ϵ������" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"վ����ϵ������");
	strcpy(func.dbname, DB_NAME );
	char * col_name[] = {"����","����","�������" };

	datafmt=0;
	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
    if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}

	int   col_num = sizeof(col_name)/sizeof(char*);
	for(i=0;i<func.colnum;i++)
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition = FALSE;
		for(j=0; j<col_num; j++)
		{
			if(strcasecmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;
			func.col[i].order = 0 ;
		}
	}

	func.func = SEL_RECORDERS;
	void *pt=0;
	if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}

	if(func.ret_roxnum ==0)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}

	int count;
	PSTATIONPARA pSta;
	char tmpbuf[ 256 ];

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pSta = new STATIONPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pSta->code = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pSta->desc = QObject::tr( tmpbuf );
		int areaid= m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_stationList.append( pSta );
		m_stanametoid.insert(pSta->code,areaid);
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);
	return TRUE;
}

bool QZonePara::readStationAreaPara()
{
	/*m_station_area.clear();
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "STATIONAREA" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"STATIONAREA");
	strcpy(func.dbname, DB_NAME );
	char * col_name[] = {"id","name","masterid" };

	datafmt=0;
	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
	if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}

	int   col_num = sizeof(col_name)/sizeof(char*);
	for(i=0;i<func.colnum;i++)
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition = FALSE;
		for(j=0; j<col_num; j++)
		{
			if(strcasecmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;
			func.col[i].order = 0 ;
		}
	}

	func.func = SEL_RECORDERS;
	void *pt=0;
	if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}

	if(func.ret_roxnum ==0)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}

	int count;
	//char tmpbuf[ 256 ];
    STATIONAREAPARA * db_stationarea = NULL;
	for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		STATIONAREAPARA *tmp_db_stationarea = new STATIONAREAPARA;
		
		db_stationarea->id = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,db_stationarea->name);
		db_stationarea->masterid = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
        *tmp_db_stationarea = *(db_stationarea);
		m_station_area.insert(tmp_db_stationarea->id, tmp_db_stationarea );
	}
	if (func.roxnum != 0) free(pt);
	if (datafmt) free(datafmt);
	return TRUE;*/


	int ret = -1;
	m_station_area.clear();

	if (m_db.Open(DB_OP_WR) != CS_SUCCEED)
	{
		printf("�����ݿ�ʧ�ܣ����أ�\n");
		return false;
	}

	CS_DATAFMT * datafmt = NULL;
	FUNC_STR  func;
	func.func = SEL_ISQL_RESULTS;
	strcpy(func.dbname, DB_NAME);
	strcpy( func.table, "STATIONAREA" );
	strcpy(func.isql, "select * from STATIONAREA order by id");
	STATIONAREAPARA * db_stationarea = NULL;

	if (CS_SUCCEED == m_db.SelectIsqlResults(&func, &datafmt, (void **)&db_stationarea))
	{
		ret = func.ret_roxnum;
	}

	if (ret > 0 )
	{
		for (int i = 0; i < ret; i++)
		{
			STATIONAREAPARA *tmp_db_stationarea = new STATIONAREAPARA;
			*tmp_db_stationarea = *(db_stationarea + i);
			m_station_area.insert(tmp_db_stationarea->id,tmp_db_stationarea);
		}
	}

	if (datafmt)
	{
		free(datafmt);
		datafmt = NULL;
	}

	if (db_stationarea)
	{
		free(db_stationarea);
	}
	return ret > 0 ? true : false;

}

bool QZonePara::readDevPara()
{
	m_devList.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "SCD�豸������" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"SCD�豸������");
	strcpy(func.dbname, DB_NAME );
	char * col_name[] = { "����","����","վ����ϵ����", "����", "��ȫ��" };

	datafmt=0;
	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
    if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}

	int   col_num = sizeof(col_name)/sizeof(char*);
	for(i=0;i<func.colnum;i++)
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=FALSE;
		for(j=0; j<col_num; j++)
		{
			if(strcasecmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;
			func.col[i].order = 0;
		}

// 		if( !strcasecmp( datafmt[i].name, "վ����ϵ����" ) )
// 		{
// 			func.col[i].order = 1;
// 		}
// 
// 		if( !strcasecmp( datafmt[i].name, "����" ) )
// 		{
// 			func.col[i].order = 0;
// 		}
	}
	func.func = SEL_RECORDERS;
	void *pt=0;
	if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}

	if(func.ret_roxnum ==0)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}

	int count;
	char tmpbuf[ 256 ];
	PNODEPARA pNode;

	QStringList *staObjList ;		//add by chenkai 2014.05.27
    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pNode = new NODEPARA ;
		pNode->type = NODETYPE_DEV;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pNode->code = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pNode->desc = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pNode->sname = QObject::tr( tmpbuf );
		pNode->devtype = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		pNode->zone = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		pNode->oldzone = pNode->zone;

		m_devCode2DevParaMap[ pNode->code ] = pNode ;
		m_devList.append( pNode );

		// add by chenkai 2014.05.27
		if( m_staCode2ObjCodesMap.contains( pNode->sname) )
		{
			staObjList = m_staCode2ObjCodesMap[ pNode->sname ]; 
		}
		else
		{
			staObjList = new QStringList;
			m_staCode2ObjCodesMap[ pNode->sname ] = staObjList;
		}
		staObjList->append( pNode->code );

    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return TRUE;
}

bool QZonePara::readNodeTable()
{
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	NetStatus		netstatus;

	QString			hostname;

	hostname = QObject::tr( netstatus.GetHostName( netstatus.GetHostNo() ) ) ;

	printf( "����ڵ������" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy( func.table, "����ڵ������" );
	strcpy( func.dbname, DB_NAME );
	char * col_name[] = { "������", "�ڵ���" };

	datafmt=0;
	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
    if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}

	int   col_num = sizeof(col_name)/sizeof(char*);
	for( i = 0; i < func.colnum; i ++ )
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=FALSE;
		for(j=0; j<col_num; j++)
		{
			if( strcasecmp(datafmt[i].name,col_name[j]) )
				continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;	
		}

		if( !strcasecmp( datafmt[i].name, "no" ) )
		{
			func.col[i].order = 0;
		}
	}
	func.func = SEL_RECORDERS;
	void *pt=0;
	if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}

	if(func.ret_roxnum ==0)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}

	int count;
	char tmpbuf[ 256 ];
	QString desc, code;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
		code = QObject::tr( tmpbuf );

		m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
		desc = QObject::tr( tmpbuf );
		
		m_hostmap[ code ] = desc;
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return TRUE;
}
bool QZonePara::readDevTypePara()
{
	int i,j;
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;
	
	printf( "DEVTYPE" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"SCD�豸���ͱ�");
	strcpy(func.dbname, DB_NAME );
	char * col_name[] = {"����","����", "������" };
	
	datafmt=0;
	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
    if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}
	
	int   col_num = sizeof(col_name)/sizeof(char*);
	for(i=0;i<func.colnum;i++)
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition = FALSE;
		for(j=0; j<col_num; j++)
		{
			if(strcasecmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;
		}
		
		if( !strcasecmp( datafmt[i].name, "����" ) )
		{
			func.col[i].order = 0;
		}
	}
	
	func.func = SEL_RECORDERS;
	void *pt=0;
	if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if(func.ret_roxnum ==0)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	int count;
	char tmpbuf[ 256 ];
	PDEVTYPEPARA pDevType;
	
    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pDevType = new DEVTYPEPARA;
		pDevType->devtype = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDevType->devname = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDevType->tablename = QObject::tr( tmpbuf );
		for( j = m_devTypeList.count(); j < pDevType->devtype; j ++ )
		{
			m_devTypeList.append( NULL );
		}
		m_devTypeList.append( pDevType );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);
	return TRUE;
}

bool QZonePara::readUserPara()
{
	m_userList.clear();
	
	int i,j;
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;
	
	printf( "�û����������" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"�û����������");
	strcpy(func.dbname, DB_NAME );
	char * col_name[] = { "����Ա","����" };
	
	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
    if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}
	
	int   col_num = sizeof(col_name)/sizeof(char*);
	for(i=0;i<func.colnum;i++)
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=FALSE;
		for(j=0; j<col_num; j++)
		{
			if(strcasecmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;
		}
	}
	func.func = SEL_RECORDERS;
	void *pt=0;
	if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if(func.ret_roxnum ==0)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	int count;
	char tmpbuf[ 256 ];
	PNODEPARA pNode;
	QString code, desc;
	QMap< QString, QString > usermap;
	
    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		code = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		desc = QObject::tr( tmpbuf );

		usermap[ code ] = desc;
    }
    if (func.roxnum != 0) 
	{
		free(pt);
		pt = 0;
	}

    if (datafmt) 
	{
		free(datafmt);
		datafmt = NULL;
	}

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"�ڵ�Ȩ�޲�����");
	strcpy(func.dbname, DB_NAME );
	char * usrcolname[] = {"������","����Ա", "��ȫ����0","��ȫ����1","��ȫ����2","��ȫ����3","��ȫ����4","��ȫ����5","��ȫ����6", \
		"��ȫ����7","��ȫ����8","��ȫ����9","��ȫ����10","��ȫ����11","��ȫ����12","��ȫ����13","��ȫ����14","��ȫ����15","��ȫ����16", \
		"��ȫ����17","��ȫ����18","��ȫ����19","��ȫ����20","��ȫ����21","��ȫ����22","��ȫ����23","��ȫ����24","��ȫ����25", \
		"��ȫ����26","��ȫ����27","��ȫ����28","��ȫ����29","��ȫ����30","��ȫ����31"};
	
	datafmt=0;
	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
    if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}
	
	col_num = sizeof(usrcolname)/sizeof(char*);
	for(i=0;i<func.colnum;i++)
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition = FALSE;
		for(j=0; j<col_num; j++)
		{
			if(strcasecmp(datafmt[i].name,usrcolname[j]))	continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;
		}
	}
	
	func.func = SEL_RECORDERS;
	pt=0;
	if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if(func.ret_roxnum ==0)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	uint zone;
	
    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pNode = new NODEPARA;
		pNode->zone = 0;
		m_db.GetData(i,&func,datafmt,usrcolname[count++],pt,ptcnt,tmpbuf);
		pNode->sname = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,usrcolname[count++],pt,ptcnt,tmpbuf);
		pNode->code = QObject::tr( tmpbuf );

		for( j = 0; j < 32; j ++ )
		{
			zone = m_db.GetData(i,&func,datafmt,usrcolname[count+j],pt,ptcnt,NULL);
			if( zone )
			{
				pNode->zone |= 0x1 << j;
			}
		}

		pNode->oldzone = pNode->zone;

		if( !usermap.contains( pNode->code ) )
		{
			delete pNode;
		}
		else
		{
			pNode->desc = usermap[ pNode->code ];
			m_userList.append( pNode );
		}
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);
	
	return TRUE;
}

bool QZonePara::readZonePara()
{
	int i,j;
	
	m_zoneNames.clear();
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;
	
	printf( "ZONE" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"�����������");
	strcpy(func.dbname, DB_NAME );
	char * col_name[] = {"���","����" };
	
	datafmt=0;
	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
    if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}
	
	int   col_num = sizeof(col_name)/sizeof(char*);
	for(i=0;i<func.colnum;i++)
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition = FALSE;
		for(j=0; j<col_num; j++)
		{
			if(strcasecmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;
			
		}
		
		if( !strcasecmp( datafmt[i].name, "���" ) )
		{
			func.col[i].order = 0;
		}
	}
	
	func.func = SEL_RECORDERS;
	void *pt=0;
	if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if(func.ret_roxnum ==0)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	int count;
	char tmpbuf[ 256 ];
	int zoneno;
	
    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		
		zoneno = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		for( j = m_zoneNames.count(); j < zoneno; j ++ )
		{
			m_zoneNames.append( "" );
		}
		m_zoneNames.append( QObject::tr( tmpbuf ) );
		m_zoneNo2ZoneDescMap[ zoneno ] = QObject::tr( tmpbuf ) ;
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);
	return TRUE;
}

bool QZonePara::getStationDesc( const QString &sname, QString & desc )
{
	bool ret = false;

	uint i;
	PSTATIONPARA pSta;

	for( i = 0; i < m_stationList.count(); i ++ )
	{
		pSta = m_stationList.at( i );

		if( pSta->code == sname )
		{
			desc = pSta->desc;
			ret = true;
			break;
		}
	}

	return ret;
}

bool QZonePara::saveUserPara()
{
	int i,j;
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt;
	PNODEPARA		pNode;
// 	QPtrList<NODEPARA> nodeList;
	QList<PNODEPARA> nodeList;
	void *pt = NULL;
	int keyid1, keyid2;
	int valueid[ 32 ];
	int level;

// 	nodeList.setAutoDelete( false );
	
	ptcnt = 0;
	nodeList.clear();
	

	for( j = 0; j < m_userList.count(); j ++ )
	{
		pNode = m_userList.at( j );
		if( pNode->oldzone != pNode->zone )
		{
			nodeList.append( pNode );
			pNode->oldzone = pNode->zone;
		}
	}

	if( nodeList.count() == 0 )
		return TRUE;

	printf( "%s", "�ڵ�Ȩ�޲�����" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy( func.table, "�ڵ�Ȩ�޲�����" );
	strcpy( func.dbname, DB_NAME );
	
	datafmt=0;
	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
	if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}

	for( i = 0; i < 32; i ++ )
	{
		valueid[ i ] = 0;
	}

	for( j = 0; j < func.colnum; j ++ )
	{
		func.col[j].select = FALSE;
		func.col[j].order_d = -1;
		func.col[j].order = -1;
		func.col[j].condition=FALSE;
		
		if( !strcasecmp( datafmt[j].name, "������" ) )
		{
			func.col[j].condition = TRUE;
			func.col[j].order_d = 1;
			keyid1 = j;
		}

		if( !strcasecmp( datafmt[j].name, "����Ա" ) )
		{
			func.col[j].condition = TRUE;
			func.col[j].order_d = 1;
			keyid2 = j;
		}

		if( strstr( datafmt[j].name, "��ȫ����" ) )
		{
			level = atoi( datafmt[j].name + 8 );
			func.col[j].select = TRUE;
			func.col[j].order_d = 1;
			valueid[ level ] = j;
		}
	}
	func.func = UPD_RECORDERS;
	func.roxnum = nodeList.count();
	uint zone;
	char isset;
	
	pt= malloc( func.recorderlen * func.roxnum );
	memset( pt, 0, sizeof( func.recorderlen * func.roxnum ) );
	for( j = 0; j < nodeList.count(); j ++ )
	{
		pNode = nodeList.at( j );
		m_db.FillData( &datafmt[ keyid1 ], pt, ptcnt, (void*)(_Q2C( pNode->sname ) ) );
		m_db.FillData( &datafmt[ keyid2 ], pt, ptcnt, (void*)(_Q2C( pNode->code ) ) );
		for( i = 0; i < 32; i ++ )
		{
			zone = ( pNode->zone >> i ) & 0x1;
			isset = (char)zone;
			m_db.FillData( &datafmt[ valueid[ i ] ], pt, ptcnt, &(isset) );
		}
	}
	
	if((retcode = m_db.UpdateRcds(&func,datafmt,pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if( pt )
	{
		free(pt);
		pt = NULL;
	}
	if( datafmt )
	{
		free(datafmt);
		datafmt = NULL;
	}

	return TRUE;
}

bool QZonePara::saveZonePara()
{
	int i;
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;
	
	printf( "ZONEPARA" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy( func.table, "�����������" );
	strcpy( func.dbname, DB_NAME );
	
	datafmt=0;
	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
    if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}
	
	func.func = ISQL_EXCOMMAND;
	sprintf( func.isql, "delete �����������" );
	if((retcode = m_db.IsqlCommand( &func, func.isql ) ) != CS_SUCCEED )
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}

	for( i = 0; i < func.colnum; i ++ )
	{
		func.col[i].select = TRUE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
	}
	func.func = INS_RECORDERS;
	func.roxnum = m_zoneNames.count();
	
	void *pt= malloc( func.recorderlen * func.roxnum );
	memset( pt, 0, func.recorderlen * func.roxnum );

	for( i = 0; i < m_zoneNames.count(); i ++ )
	{	
		if( m_zoneNames[ i ].isEmpty() )
			continue;
		m_db.FillData( datafmt, pt, ptcnt, &i );
		m_db.FillData( &datafmt[ 1 ], pt, ptcnt, (void*)(_Q2C( m_zoneNames[ i ] ) ) );
	}
	
	if((retcode = m_db.InsertRcds(&func,datafmt,pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();
		return FALSE;
	}
	
	
    free(pt);
    free(datafmt);
	return TRUE;
}

bool QZonePara::saveDevPara()
{
	int j;
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt;
	PNODEPARA		pNode;
// 	QPtrList<NODEPARA> nodeList;
	QList<PNODEPARA> nodeList;
	void *pt = NULL;
	int keyid;
	int valueid;

// 	nodeList.setAutoDelete( false );
	
	ptcnt = 0;
	nodeList.clear();
	

	for( j = 0; j < m_devList.count(); j ++ )
	{
		pNode = m_devList.at( j );
		if( pNode->oldzone != pNode->zone )
		{
			nodeList.append( pNode );
			pNode->oldzone = pNode->zone;
		}
	}

	if( nodeList.count() == 0 )
		return TRUE;

	printf( "%s", "SCD�豸������" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy( func.table, "SCD�豸������" );
	strcpy( func.dbname, DB_NAME );
	
	datafmt=0;
	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
	if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}

	for( j = 0; j < func.colnum; j ++ )
	{
		func.col[j].select = FALSE;
		func.col[j].order_d = -1;
		func.col[j].order = -1;
		func.col[j].condition=FALSE;
		
		if( !strcasecmp( datafmt[j].name, "����" ) )
		{
			func.col[j].condition = TRUE;
			func.col[j].order_d = 1;
			keyid = j;
		}
		if( !strcasecmp( datafmt[j].name, "��ȫ��" ) )
		{
			func.col[j].select = TRUE;
			func.col[j].order_d = 1;
			valueid = j;
		}
	}
	func.func = UPD_RECORDERS;
	func.roxnum = nodeList.count();
	
	pt= malloc( func.recorderlen * func.roxnum );
	memset( pt, 0, sizeof( func.recorderlen * func.roxnum ) );
	for( j = 0; j < nodeList.count(); j ++ )
	{
		pNode = nodeList.at( j );
		m_db.FillData( &datafmt[ keyid ], pt, ptcnt, (void*)(_Q2C( pNode->code ) ) );
		m_db.FillData( &datafmt[ valueid ], pt, ptcnt, &(pNode->zone) );
	}
	
	if((retcode = m_db.UpdateRcds(&func,datafmt,pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if( pt )
	{
		free(pt);
		pt = NULL;
	}
	if( datafmt )
	{
		free(datafmt);
		datafmt = NULL;
	}

	return TRUE;
}


bool QZonePara::getDevCode( const QString &rtucode, uint devtype, QStringList &devcode )
{
	bool ret = false;

	PNODEPARA pDevPara;

	QStringList *staObjList;
	QString objName;
	uint i;

	if( !m_staCode2ObjCodesMap.contains( rtucode ) )
		return FALSE;

	staObjList = m_staCode2ObjCodesMap[ rtucode ];

	for( i = 0; i < staObjList->count(); i ++ )
	{
		objName = (*staObjList)[ i ];
		pDevPara = getDev( objName );
		if( devtype == pDevPara->devtype )
		{
			devcode.append( pDevPara->code );
			ret = true;
		}
	}

	return ret;
}

PNODEPARA QZonePara::getDev( const QString &devcode )
{
	if( !m_devCode2DevParaMap.contains( devcode ) )
		return NULL;

	return m_devCode2DevParaMap[ devcode ];
}

bool QZonePara::getDevDesc( const QString &devcode, QString &devdesc )
{
	bool ret = false;
	NODEPARA *pDev;

	if( m_devCode2DevParaMap.contains( devcode ) )
	{
		pDev = m_devCode2DevParaMap[ devcode ];
		devdesc = pDev->desc;
		ret = true;
	}

	return ret;
}

bool QZonePara::getZoneDesc( const QString &devcode, QString &zonedesc )
{
	bool ret = false;
	NODEPARA *pDev;

	if( m_devCode2DevParaMap.contains( devcode ) )
	{
		pDev = m_devCode2DevParaMap[ devcode ];
		zonedesc = m_zoneNo2ZoneDescMap[ pDev->zone ];
		ret = true;
	}

	return ret;
}