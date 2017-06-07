//#include "sybase.h"

#include "db/dbapi.h"
#include "verapi.h"

#include <io.h>

//#include <sys\stat.h>

#include <conio.h>
#include <direct.h>

//#define m_startRtu 40
#define YXNUM	10
#define YCNUM	5
class CDbOperate
{
private:
	int m_StationNum;
	int m_PointNum;
	int m_startChan;
	int m_startRtu;
	DBSingle dbsingle;
private:
	int DBStart();
	int InsertStaData();
	int InsertRtuData( );
	int InsertChanData( );
	int InsertDevData( );
	int InsertYcData( );
	int InsertYxData( );

public:
	CDbOperate(int startChan, int startRtu, int stanum, int pointnum );
	~CDbOperate();
	int InsertData();
};
CDbOperate::CDbOperate(int startChan, int startRtu, int stanum, int pointnum )
{
	m_startChan = startChan;
	m_startRtu = startRtu;
	m_StationNum = stanum;
	m_PointNum = pointnum;
}

CDbOperate::~CDbOperate()
{
	dbsingle.Close();
}

int CDbOperate::InsertRtuData( )
{
	int i, col;
	
	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;
	
	int tmpint;
	char tmpstr[ 100 ];
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "�ն˲�����" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}
	
	func.func = INS_RECORDERS;
	func.roxnum = m_StationNum;
	pt = ( char* )malloc( func.recorderlen * m_StationNum );
	int ptcnt = 0;
	int ptstart = 0;
	
	for( i = 0; i < m_StationNum; i ++ )
	{
		col = 0;
		ptcnt = ptstart;
		//�ն����
		tmpint = i + m_startRtu;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//����
		sprintf( tmpstr, "rtest%03d", i + m_startRtu );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//����
		sprintf( tmpstr, "�����ն�%03d", i + m_startRtu );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//��ȫ��
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//վ����ϵ����
		sprintf( tmpstr, "test%03d", i + m_startRtu );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//��ͨ����
// 		tmpint = i + m_startChan;
// 		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
// 		col ++;
		//ң�����
		tmpint = m_PointNum / YXNUM * YCNUM;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//ң�Ÿ���
		tmpint = m_PointNum;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//��ȸ���
		tmpint = 10;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//״̬����
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//ʹ�ñ�־
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		
		ptstart += func.recorderlen;
	}
	
	retcode = dbsingle.InsertRcds(&func,datafmt,pt);
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );
	
	return retcode;
}

int CDbOperate::InsertChanData( )
{
	int i, col;
	
	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;
	
	int tmpint;
	char tmpstr[ 100 ];
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "ͨ��������" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}
	
	func.func = INS_RECORDERS;
	func.roxnum = m_StationNum;
	pt = ( char* )malloc( func.recorderlen * m_StationNum );
	int ptcnt = 0;
	int ptstart = 0;
	
	for( i = 0; i < m_StationNum; i ++ )
	{
		col = 0;
		ptcnt = ptstart;
		//ͨ�����
		tmpint = i + m_startChan;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//����
		sprintf( tmpstr, "ctest%03d", i + m_startChan );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//����
		sprintf( tmpstr, "�����ն�%03dͨ��", i + m_startChan );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//��ȫ��
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//��Լ����
		sprintf( tmpstr, "iec104");
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//�ն����
		tmpint = i + m_startRtu;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//�������ȼ�
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//������ַ
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//��վ��ַ
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//�Ƿ���
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//�Ƿ�GPS
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//��ʼ״̬
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//ͨ��ֹͣʱ����
		tmpint = 300;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//�����ʱ�׼
		tmpint = 20;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//Ĭ��ֵ���
		sprintf( tmpstr, "FEP1");
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//�豸����
		tmpint = 2;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//�����豸
		sprintf( tmpstr, "" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//�����豸�˿�
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//�豸��ַ
		sprintf( tmpstr, "172.27.27.170" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//�豸�˿�
		tmpint = 10001+i;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//���豸��ַ
		sprintf( tmpstr, "" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//���豸�˿�
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//������
		tmpint = 1200;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//У��λ
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//ֹͣλ
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//����λ
		tmpint = 8;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//������
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//�Ƿ���ͼ
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//��ͼͼ��
		sprintf( tmpstr, "" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
// 		//��ʼ����������
// 		tmpint = 0;
// 		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
// 		col ++;
		//ͨ��ʹ��
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;


		ptstart += func.recorderlen;
	}
	
	retcode = dbsingle.InsertRcds(&func,datafmt,pt);
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );
	
	return retcode;
}

int CDbOperate::InsertDevData( )
{
	int i, j, col;
	
	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;
	
	int tmpint;
	char tmpstr[ 100 ];
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "SCD�豸������" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}
	
	func.func = INS_RECORDERS;
	func.roxnum = m_PointNum / YXNUM;
	pt = ( char* )malloc( func.recorderlen * func.roxnum );
	int ptcnt = 0;
	int ptstart = 0;
	
	for( i = 0; i < m_StationNum; i ++ )
	{
		ptcnt = 0;
		ptstart = 0;
		memset( pt, 0, func.recorderlen * func.roxnum );
		for( j = 0; j < m_PointNum / YXNUM ; j ++ )
		{
			col = 0;
			ptcnt = ptstart ;
			//SUBID
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			//DTYPEID
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			//DEVID
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;    
			// RCFGID         
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;    
			// ����		
			sprintf( tmpstr, "Ktest%03d%03d", i + m_startRtu, j + 1 );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;
			// ����	
			sprintf( tmpstr, "����վ%03d����%03d", i + m_startRtu, j + 1 );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;
			// վ����ϵ����	
			sprintf( tmpstr, "test%03d", i + m_startRtu );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;
			// ����      
			tmpint = 1;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			// �ߵ�ѹ�ȼ� 
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			// �е�ѹ�ȼ� 
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			// �͵�ѹ�ȼ�  
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			// ���豸��־ 
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			// ĩ��վ����ϵ���� 
			sprintf( tmpstr, ""  );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;
			// �ͺ� 	
			sprintf( tmpstr, ""  );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;
			// ��ע 		
			sprintf( tmpstr, ""  );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;

			ptstart += func.recorderlen;
		}
		retcode = dbsingle.InsertRcds(&func,datafmt,pt);
	}
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );
	
	return retcode;
}

int CDbOperate::InsertYcData( )
{
	int i, j, k, col;
	
	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;
	
	int tmpint;
	char tmpstr[ 100 ];
	double tmpdouble;

	int ycno;
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "ң�������" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}
	
	func.func = INS_RECORDERS;
	func.roxnum = m_PointNum / YXNUM * YCNUM;
	pt = ( char* )malloc( func.recorderlen * func.roxnum );
	int ptcnt;
	int ptstart = 0;
	
	
	for( i = 0; i < m_StationNum; i ++ )
	{
		ycno = 0;
		ptstart = 0;
		memset( pt, 0, func.recorderlen * func.roxnum );
		for( j = 0; j < m_PointNum / YXNUM ; j ++ )
		{
			for( k = 0; k < YCNUM; k ++ )
			{
				col = 0;
				ptcnt = ptstart;
				//SUBID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				//DTYPEID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				//DEVID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;    
				switch( k )
				{
				case 0:
					{
						// ����
						sprintf( tmpstr, "Ktest%03d%03dP", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						//����
						sprintf( tmpstr, "����վ%03d����%03d�й�", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;	
					}
					break;
				case 1:
					{
						sprintf( tmpstr, "Ktest%03d%03dQ", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "����վ%03d����%03d�޹�", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				case 2:
					{
						sprintf( tmpstr, "Ktest%03d%03dIa", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "����վ%03d����%03dA����", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				case 3:
					{
						sprintf( tmpstr, "Ktest%03d%03dIb", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "����վ%03d����%03dB����", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				case 4:
					{
						sprintf( tmpstr, "Ktest%03d%03dIc", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "����վ%03d����%03dC����", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				default:
					break;
				}
				// ��ȫ��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// �豸����
				sprintf( tmpstr, "Ktest%03d%03d", i + m_startRtu, j + 1 );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// ����
				switch( k )
				{
				case 0:
					{
						tmpint = 20;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				case 1:
					{
						tmpint = 40;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				case 2:
					{
						tmpint = 2;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				case 3:
					{
						tmpint = 3;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				case 4:
					{
						tmpint = 4;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				default:
					break;
				}
				// �ɼ�����
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// �ն����
				tmpint =  i + m_startRtu;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ң�����
				tmpint =  ycno ++ ;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// б��
				tmpdouble = 1;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// �ؾ�
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// ��Ư
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// ����ֵ
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// ����ֵ
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// �������
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// ��������
				tmpint = 5;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// �ƻ����
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ����ʱ����
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// �����ʽ
				sprintf( tmpstr, "event" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// ����Խ�޼���
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ��������
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// ��������
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// ��������
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// ��������
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// ׷��ģʽ
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// �ָ�ϵ��
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// ��������
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// ��ͼ��
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// ��������־
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ת����־
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ����
				tmpint = 1;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ��ʾ��־
				tmpint = 1;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ת���ն˺�
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ת�����
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ת��ϵ��
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++;
				// ת��ң��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// �ϼ�ת��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// VQCת��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ģ����ת��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;

				ptstart +=func.recorderlen;
			}
		}
		retcode = dbsingle.InsertRcds(&func,datafmt,pt);
	}
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );
	
	return retcode;
}

int CDbOperate::InsertYxData( )
{
	int i, j, k, col;
	
	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;
	
	int tmpint;
	char tmpstr[ 100 ];

	int yxno;
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "ң�Ų�����" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}
	
	func.func = INS_RECORDERS;
	func.roxnum = YXNUM;
	pt = ( char* )malloc( func.recorderlen * func.roxnum );
	int ptcnt = 0;
	int ptstart = 0;
	
	
	for( i = 0; i < m_StationNum; i ++ )
	{
		yxno = 0;
		for( j = 0; j < m_PointNum / YXNUM ; j ++ )
		{
			ptstart = 0;
			memset( pt, 0, func.recorderlen * func.roxnum );
			for( k = 0; k < YXNUM; k ++ )
			{
				col = 0;
				ptcnt = ptstart;
				//SUBID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				//DTYPEID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				//DEVID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;    
				// ����
				switch( k )
				{
				case 0:
					{
						sprintf( tmpstr, "Ktest%03d%03dKg", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "����վ%03d����%03d", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;	
					}
					break;
				case 1:
					{
						sprintf( tmpstr, "Ktest%03d%03dDz", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "����վ%03d����%03d��բ", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				default:
					{
						sprintf( tmpstr, "Ktest%03d%03d%04dBh", i + m_startRtu, j + 1, yxno );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "����վ%03d����%03d����%04d", i + m_startRtu, j + 1, yxno );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				}
				// ��ȫ��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;    
				// �豸����
				sprintf( tmpstr, "Ktest%03d%03d", i + m_startRtu, j + 1 );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// ��·���ش���
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// ����
				switch( k )
				{
				case 0:
					{
						tmpint = 300;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++;  
					}
					break;
				case 1:
					{
						tmpint = 500;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				default:
					{
						tmpint = 600;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				}
				// �Ƿ�׷��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// �Ƿ�ң��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// �ն����
				tmpint = i + m_startRtu;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ң�����
				tmpint = yxno ++ ;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ��ң�����
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ˫ң�ű�־
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ��ң�ű�־
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ˫ң�ż��㷽ʽ
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ң�����
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ����ģʽ
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ң�ط�ʽ
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// �����ʽ
				sprintf( tmpstr, "event" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// ��ͼ��
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// SOE��
				sprintf( tmpstr, "SOE" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// �����
				switch( k )
				{
				case 0:
				case 1:
					{
						sprintf( tmpstr, "KgChg" );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				default:
					{
						sprintf( tmpstr, "ProtChg" );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				}
				// �¹���ֵ
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ���¹ʷ�ʽ
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ���¹�����
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ׷�����
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// �¹ʹ�����
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// �غϹ�����
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// �طֹ�����
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// ң�ط�Уʱ����
				tmpint = 30;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ң�ط���ʱ����
				tmpint = 30;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ң��ִ��ʱ����
				tmpint = 30;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// ң�������
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// ��������־
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ȡ����־
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// �Ƿ�ͳ��
				tmpint = 1;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ת����־
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ��չң��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ң��У����
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// ת���ն˺�
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ת�����
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ת��ң��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// �ϼ�ת��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// VQCת��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ģ����ת��
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// ��������
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;

				ptstart += func.recorderlen;
			}
			retcode = dbsingle.InsertRcds(&func,datafmt,pt);
		}
	}
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );
	
	return retcode;
}


int CDbOperate::DBStart()
{
	LOG_INFO *loginfo;
	int retcode;
	
	loginfo=(LOG_INFO *)malloc (sizeof(LOG_INFO));
	memset(loginfo,0,sizeof(LOG_INFO));
	
	strcpy(loginfo->servername,"nbdb");
	
	strcpy(loginfo->username,"modeldb");
	strcpy(loginfo->password,"supernb");
	
	loginfo->useflag=1;
	
	retcode=dbsingle.Open(0,loginfo,0,1);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n���󣺷�����%s��ʧ��!\n",loginfo->servername);
		fprintf(stderr,"���� 1��������%s�Ƿ�����\n",loginfo->servername);
		fprintf(stderr,"       2���ͻ��������Ƿ���ȷ\n");
		
		if (loginfo) free (loginfo);
		return retcode;
	}
	
	
	if (loginfo) free(loginfo);
	return retcode;
}

int CDbOperate::InsertStaData()
{
	int i, col;

	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;

	int tmpint;
	char tmpstr[ 100 ];
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "վ����ϵ������" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}

	func.func = INS_RECORDERS;
	func.roxnum = m_StationNum;
	pt = ( char* )malloc( func.recorderlen * m_StationNum );
	int ptcnt = 0;
	int ptstart = 0;

	for( i = 0; i < m_StationNum; i ++ )
	{
		col = 0;
		ptcnt = ptstart;

		//SUBID
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//����
		sprintf( tmpstr, "test%03d", i + m_startRtu );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//����
		sprintf( tmpstr, "����վ%03d", i + m_startRtu );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//�¹��ܱ�־
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//�¹�������
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//��ȫ��ʼ��
		tmpint = 2012;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//��ȫ��ʼ��
		tmpint = 2;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//��ȫ��ʼ��
		tmpint = 10;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//����ͼ��
		sprintf( tmpstr, "" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//�������
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//��ע
		sprintf( tmpstr, "" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//�������
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//����̬
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//�������
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//�������
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;

		ptstart += func.recorderlen;
	}

	retcode = dbsingle.InsertRcds(&func,datafmt,pt);
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );

	return retcode;
}

int CDbOperate::InsertData()
{
	int retcode = DBStart();
	if( retcode != CS_SUCCEED )
		return retcode;

	retcode = InsertStaData();
	if( retcode != CS_SUCCEED )
		return retcode;
	
	retcode = InsertRtuData();
	if( retcode != CS_SUCCEED )
		return retcode;

	retcode = InsertChanData();
	if( retcode != CS_SUCCEED )
		return retcode;

	retcode = InsertDevData();
	if( retcode != CS_SUCCEED )
		return retcode;


	retcode = InsertYcData();
	if( retcode != CS_SUCCEED )
		return retcode;
	retcode = InsertYxData();

	return retcode;
}

int main(int argc,char *argv[])
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("datagen,1.1");
				printf("datagen, 1.1");
				return 0;
			}
		}
	}
	NetDbg dbg;
	NetStatus cfg;

	if( argc < 5 )
	{
		printf( "datagen ���������������ݹ���\n" );
		printf( "����˵����\n" );
		printf( "datagen [��ʼͨ����] [��ʼ�ն˺�] [��վ����] [���ݵ���]\n" );
		printf( "ʵ����\n" );
		printf( "datagen 10 20 30 40\n" );
		printf( "������ͨ����Ϊ��10 �� 39 ��30��ͨ��\n" );
		printf( "�������ն˺�Ϊ��20 �� 49 ��30���ն�\n" );
		printf( "�������豸��Ϊ��30 * (40 / 10) = 120���豸\n" );
		printf( "������ң����Ϊ��30 * (40 /  2) = 600��ң��\n" );
		printf( "������ң����Ϊ��30 * 40 = 1200��ң��\n" );
		exit(0);
	}

	int startchan = atoi(argv[1]);
	int startRTU = atoi(argv[2]);
	int stanum = atoi(argv[3]);
	int pointnum = atoi(argv[4]);

	if( stanum == 0 || pointnum == 0 )
	{
		return 1;
	}

	CDbOperate dbOperate(startchan, startRTU, stanum, pointnum );

	dbg.RegProc("���ݿ⹤��");

	dbOperate.InsertData();

	dbg.UnRegProc();

	return 1;
}
