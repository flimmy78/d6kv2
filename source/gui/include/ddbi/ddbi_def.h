#ifndef __DDBI_DEF_H__
#define __DDBI_DEF_H__

#include <QtCore/QString>
#include <QtCore/QObject>
#include "ddef/ddes_comm.h"

//##ModelId=433F86AB034B
//##Documentation
//## ���ܣ�
//##    ö�����ݿ����ֶ�����
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-10-2
enum  { 
	//##Documentation
    //## δ֪����
	GDB_FIELD_UNKNOWN = -1,
    //##Documentation
    //## �޷����ַ���
    GDB_FIELD_UINT8 , 
    //##Documentation
    //## �з����ַ���
    GDB_FIELD_INT8 , 
    //##Documentation
    //## �޷��Ŷ�������
    GDB_FIELD_UINT16 , 
    //##Documentation
    //## �з��Ŷ�������
    GDB_FIELD_INT16 , 
    //##Documentation
    //## �޷���������
    GDB_FIELD_UINT32, 
    //##Documentation
    //## �з���������
    GDB_FIELD_INT32, 
    //##Documentation
    //## �޷��ų�������(8 byte long)
    GDB_FIELD_UINT64, 
    //##Documentation
    //## �з��ų�������(8 byte long)
    GDB_FIELD_INT64, 
    //##Documentation
    //## �����ȸ�����
    GDB_FIELD_FLOAT4, 
    //##Documentation
    //## ˫���ȸ�����
    GDB_FIELD_FLOAT8, 
    //##Documentation
    //## �ַ�����
    GDB_FIELD_STRING};

//##ModelId=434A76800203
//##Documentation
//## ���壺
//##   ���������Զ���
//## ���ܣ�
//##   ����������
//##   ����GDB_KEY,GDB_NULL
//##   �����ж����Ƿ��ǹؼ��֣��Ƿ����Ϊ��
//##   ��Ӧ��GdbDataFmtField.status��
//## ��ƣ�
//##   ������
//## ���ڣ�
//##   2005-10-10
enum  { 
    //##Documentation
    //## �Ƿ�ؼ���
    GDB_KEY = 0x01, 
    //##Documentation
    //## �Ƿ����Ϊ��
    GDB_NULL = 0x02 };

//##ModelId=433B6CB401B5
//##Documentation
//## ���壺
//##        ��ֵ����
//## ���ܣ�
//##       ʵ�����ݿ���ֵ�����޹���
//##       ͳһ���ݿ���ʱ�������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
union UField
{
    //##ModelId=433B6DB30280
    uint8 uint8_v;

    //##ModelId=433B6E22000F
    char int8_v;

    //##ModelId=433B6DFF0186
    uint16 uint16_v;

    //##ModelId=433B6E2F009C
    int16 int16_v;

    //##ModelId=433B6E1101F4
    uint32 uint32_v;

    //##ModelId=433B6E3803C8
    int32 int32_v;

    //##ModelId=433F8CE0033C
    uint64 uint64_v;

    //##ModelId=433F8CD2002E
    int64 int64_v;

    //##ModelId=433B6E480138
    FLOAT4 float_v;

    //##ModelId=433B6E5E0271
    FLOAT8 double_v;

    //##ModelId=433B6E74029F
    char *string;

};

//##ModelId=433CBBD70175
//##Documentation
//## ���壺
//##        ���ݿ���ֶνṹ����
//## ���ܣ�
//##       ʵ�����ݿ��¼�����޹���
//##       ͳһ���ݿ���ʱ�������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
struct GdbDataFmtField
{
    //##ModelId=434780700251
    //##Documentation
    //## ���ܣ�
    //##   �Ƚ����ṹ�Ƿ����
    //## ������
    //##   const GdbDataFmtField& fmt
    //## ���أ�
    //##   if equal return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    //##   
    uint8 operator==(const GdbDataFmtField& fmt)
	{
		if ( this == &fmt ) return 1 ;

		if ( strcmp( name , fmt.name ) != 0 )	return 0;	
		if ( namelen != fmt.namelen ) 			return 0;	
		if ( datatype != fmt.datatype) 			return 0;		
		if ( format != fmt.format ) 			return 0;
		if ( maxlength != fmt.maxlength ) 		return 0;
		if ( scale != fmt.scale ) 				return 0;
		if ( precision != fmt.precision ) 		return 0;
		if ( status != fmt.status ) 			return 0;
		if ( count != fmt.count ) 				return 0;
		if ( usertype != fmt.usertype )			return 0;
			
		return 1;
	}

    //##ModelId=43479FA90399
    //##Documentation
    //## ���ܣ�
    //##   �Ƚ����ṹ�Ƿ����
    //## ������
    //##   const GdbDataFmtField& fmt
    //## ���أ�
    //##   if not equal return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    //##   
    uint8 operator!=(const GdbDataFmtField& fmt)
	{
		if ( *this == fmt )		return 0;
		
		return 1;
	}

    //##ModelId=433CBC40007B
    //##Documentation
    //## �ֶ�����
    char name[GDB_FMT_LEN];

    //##ModelId=43486A8201A5
    //##Documentation
    //## ȫ�������ڽ���
    char full_name[GDB_FMT_LEN];

    //##ModelId=433CBC9402DC
    //##Documentation
    //## �ֶ�������
    int16 namelen;

    //##ModelId=433CBCBA0349
    //##Documentation
    //## �ֶ���������
    int32 datatype;

    //##ModelId=433CBCD9004C
    //##Documentation
    //## ��ʽ
    int32 format;

    //##ModelId=433CBCF2031A
    //##Documentation
    //## ������󳤶�
    int32 maxlength;

    //##ModelId=433CBD0703E6
    //##Documentation
    //## ��Χ
    int32 scale;

    //##ModelId=433CBD35027E
    //##Documentation
    //## ����
    uint32 precision;

    //##ModelId=433CBD5101D2
    //##Documentation
    //## ״̬
    int32 status;

    //##ModelId=433CBD640359
    //##Documentation
    //## ����
    int32 count;

    //##ModelId=433CBD7B002C
    //##Documentation
    //## �û���������
    int32 usertype;

    //##ModelId=433CBD9303E6
    void* locale;

    //##ModelId=433CDE610070
    //##Documentation
    //## ��ʾ���ԣ�������������ʾ����
    //## ��� visible = 1 ���û���ѯ���Կ�����ʾ��������ʾ
    uint8 visible;

    //##ModelId=433CDF2003BB
    //##Documentation
    //## λ�����ԣ����������ж�Ӧ��ѯ��λ������
    uint8 position;

};

//##ModelId=433CC6C801FE
//##Documentation
//## ���壺
//##       ���¼��Ϣ����
//## ���ܣ�
//##       ʵ�����ݿ��¼�����޹���
//##       ͳһ���ݿ���ʱ�������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
struct GdbDataFmt
{
    //##ModelId=433CC8360384
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbDataFmt()
	{
		colnum = 0 ;
		cols   = NULL ;	
	}

    //##ModelId=433CC842027A
    //##Documentation
    //## ���ܣ�
    //##      ��������������clear����ڴ�
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    ~GdbDataFmt()
	{
		clear();
	}

    //##ModelId=433CE46E032D
    //##Documentation
    //## ���ܣ�
    //##     ���������ڴ�
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    void clear()
	{
		if ( colnum )
		delete [] cols ;
	
		colnum = 0 ;
		cols   = NULL ;
	}

    //##ModelId=433F8031001F
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      const GdbDataFmt& datafmt
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbDataFmt(const GdbDataFmt& datafmt)
	{
		if ( this == &datafmt )
			return ;
		
		colnum = 0 ;
		cols   = NULL ;
		
		if ( datafmt.colnum == 0 )
			return ;
		
		colnum = datafmt.colnum ;
		cols = new GdbDataFmtField[colnum];
		g_CHECK_PTR(cols);
		
		memcpy( cols , datafmt.cols , datafmt.colnum*sizeof(GdbDataFmtField)); 
	}

    //##ModelId=433F8071030D
    //##Documentation
    //## ���ܣ�
    //##      ��ֵ���غ���
    //## ������
    //##      const GdbDataFmt& datafmt
    //## ���أ�
    //##     const GdbDataFmt&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbDataFmt& operator=(const GdbDataFmt& datafmt)
	{
		if ( this == &datafmt )
			return *this ;
	
		if ( !allocMem( datafmt.colnum ) ) return datafmt ;
		
		memcpy( cols , datafmt.cols , colnum*sizeof(GdbDataFmtField)); 
		
		return *this ;
	}

    //##ModelId=43476E670213
    //##Documentation
    //## ���ܣ�
    //##   �����ڴ�
    //## ������
    //##   uint32 colnum//�и���
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 allocMem(uint32 n)
	{
		if ( n == 0 )      
		{
			clear();
			return 0;
		}
	
		if ( n == colnum ) return 1;
	
		clear();
		
		colnum = n ;
		cols = new GdbDataFmtField[colnum];
		g_CHECK_PTR(cols);
		
		if (cols)	
		{
			memset( cols , 0 , colnum*sizeof(GdbDataFmtField));			
			return 1;
		}
		
		colnum = 0 ;
		
		return 0 ;
	}

    //##ModelId=434778DA034B
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   uint32 index//����
    //## ����(GdbDataFmtField&)
    //##   if success return GdbDataFmtField&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    GdbDataFmtField& operator[](uint32 index)
	{
		if ( index >= colnum )
			assert(0);
	
		return cols[index];
	}

	//##ModelId=4347BCC30222
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   uint32 index//����
    //## ����(GdbDataFmtField&)
    //##   if success return GdbDataFmtField&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    const GdbDataFmtField& operator[](uint32 index) const
	{
		if ( index >= colnum )
			assert(0);
	
		return cols[index];
	}

    //##ModelId=434780F601D4
    //##Documentation
    //## ���ܣ�
    //##   �Ƚϱ�ṹ�Ƿ����
    //## ������
    //##   const GdbDataFmt& fmt
    //## ���أ�
    //##   if equal return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 operator==(const GdbDataFmt& fmt)
	{
		if ( this == &fmt ) return 1 ;

		if ( colnum != fmt.colnum ) return 0;
		
		for ( uint32 col = 0 ; col < colnum ; col ++ )
		{
			if ( cols[col] == fmt[col] )
				continue ;
				
			return 0;			
		}
			
		return 1;
	}

    //##ModelId=43479F71036B
    //##Documentation
    //## ���ܣ�
    //##   �Ƚϱ�ṹ�Ƿ����
    //## ������
    //##   const GdbDataFmt& fmt
    //## ���أ�
    //##   if not equal return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 operator!=(const GdbDataFmt& fmt)
	{
		if ( *this == fmt )
			return 0;
		
		return 1;
	}

	//##ModelId=4347BCC30222
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   const char* fldname//����
    //## ����(const GdbDataFmtField&)
    //##   if success return GdbDataFmtField&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    GdbDataFmtField& operator[](const char* fldname)
	{
		uint32 index=exist(fldname);
	
		if ( index >= colnum )
			assert(0);
		
		return cols[index];
	}

    //##ModelId=4347BCC30222
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   const char* fldname//����
    //## ����(const GdbDataFmtField&)
    //##   if success return GdbDataFmtField&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    const GdbDataFmtField& operator[](const char* fldname) const
	{
		uint32 index=exist(fldname);
	
		if ( index >= colnum )
			assert(0);
		
		return cols[index];
	}

    //##ModelId=434DC56A0247
    //##Documentation
    //## ���ܣ�
    //##   ����Ƿ��������
    //## ������
    //##   const char* name;//����
    //## ���أ�
    //##   if exist return pos
    //##   else return -1
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    uint32 exist(const char* name) const
	{
		for ( uint32 col = 0 ; col < colnum ; col ++ )
		{
			if ( !strcmp( cols[col].name , name ) )
			{				
				return col;
			}
		}

		return (uint32)-1 ;
	}

    //##ModelId=433CC6EA02F8
    //##Documentation
    //## �и���
    uint32 colnum;

    //##ModelId=433CC72300A6
    //##Documentation
    //## ����Ϣ����
    GdbDataFmtField* cols;

};

//##ModelId=433B6ECD0399
//##Documentation
//## ���壺
//##        ���ݿ�����ֵ����
//## ���ܣ�
//##       ʵ�����ݿ��¼�����޹���
//##       ͳһ���ݿ���ʱ�������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
struct GdbField
{
    //##ModelId=433B6F530157
    //##Documentation
    //## ���ܣ�
    //##     ��ʼ�����캯��
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField()
	{
		type = GDB_FIELD_UNKNOWN ;
		memset( &value , 0 , sizeof(value) );
		len = 0;
	}

    //##ModelId=4347576701E4
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      const GdbField& v
    //## ���أ�
    //##       ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(const GdbField& v)
	{
		if ( this == &v )
			return ;
		
		type = v.type ;
		
		if ( type != (char)GDB_FIELD_STRING )
		{
			memcpy( this , &v , sizeof(GdbField) );
		}
		else
		{	
			if ( len )
			{
				value.string = new char[len];
				g_CHECK_PTR(value.string);
				strncpy( value.string , v.value.string , len );
			}
		}
	}

    //##ModelId=433B6F6601E4
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     uint8 v
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(uint8 v)
	{
		type = GDB_FIELD_UINT8 ;
		value.uint8_v = v ;
		len = sizeof(v);
	}

    //##ModelId=433B6FC20222
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     uint16 v
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(uint16 v)
	{
		type = GDB_FIELD_UINT16 ;
		value.uint16_v = v ;
		len = sizeof(v);
	}

    //##ModelId=433B6FDB029F
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     uint32 v
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(uint32 v)
	{
		type = GDB_FIELD_UINT32 ;
		value.uint32_v = v ;
		len = sizeof(v);
	}

    //##ModelId=434754580119
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     uint64 v
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(uint64 v)
	{
		value.uint64_v = v ;
		type = GDB_FIELD_UINT64 ;
		len = sizeof(v);
	}

    //##ModelId=433B6FEF0157
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     char v
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(char v)
	{
		type = GDB_FIELD_INT8 ;
		value.int8_v = v ;
		len = sizeof(v);
	}

    //##ModelId=433B700403D8
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     int16 v
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(int16 v)
	{
		type = GDB_FIELD_INT16 ;
		value.int16_v = v ;
		len = sizeof(v);
	}

    //##ModelId=433B70110167
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     int32 v
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(int32 v)
	{
		type = GDB_FIELD_INT32 ;
		value.int32_v = v ;
		len = sizeof(v);
	}

    //##ModelId=4347542F0186
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     int64 v
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(int64 v)
	{
		value.int64_v = v ;
		type = GDB_FIELD_INT64 ;
		len = sizeof(v);
	}

    //##ModelId=433B702402FD
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     FLOAT4 v
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(FLOAT4 v)
	{
		type = GDB_FIELD_FLOAT4 ;
		value.float_v = v ;
		len = sizeof(v);
	}

    //##ModelId=433B7038034B
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##    FLOAT8 v
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(FLOAT8 v)
	{
		type = GDB_FIELD_FLOAT8 ;
		value.double_v = v ;
		len = sizeof(v);
	}

    //##ModelId=433B70480399
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##    const char* v
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbField(const char *v)
	{
		type = GDB_FIELD_UNKNOWN ;
		memset( &value , 0 , sizeof(value) );
		len = 0;
		
		if ( v == NULL ) return ;
			
		type = GDB_FIELD_STRING ;	
		len = strlen(v);
		
		if ( len == 0 )
			return ;
			
		value.string = new char[len+1];
		g_CHECK_PTR(value.string);

		memset( value.string , 0 , len+1 );
		strncpy( value.string , v , len );
		
	}

    //##ModelId=433B708A00CB
    //##Documentation
    //## ���ܣ�
    //##      ��������������clear����ڴ�
    //## ������
    //##      ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    ~GdbField()
	{
		clear();
	}
    //##ModelId=433B769303B9
    //##Documentation
    //## ���ܣ�
    //##     ������¼��������ڴ�
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    void clear()
	{
		if ( type == (char)GDB_FIELD_STRING )
		{
			if ( len && value.string )
			{
				delete [] value.string;
			}
			
			value.string = NULL ;
			len = 0;
		}
			
		type = (char)GDB_FIELD_UNKNOWN ;
		memset( &value , 0 , sizeof(value) );
	}

    //##ModelId=433E92C8036B
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      const GdbField& v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(const GdbField& v)
	{
		if ( &v == this )
			return v ;
			
		clear();
		
		type = v.type ;
		len  = v.len  ;
		
		if ( type != (char)GDB_FIELD_STRING )
		{
			memcpy( this , &v , sizeof(GdbField) );
		}
		else
		{	
			if ( len )
			{
				value.string = new char[len+1];
				g_CHECK_PTR(value.string);
				memset( value.string , 0 , len+1 );	
				strncpy( value.string , v.value.string , len );
			}
		}
		
		return *this ;
	}

    //##ModelId=433E931300FA
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      uint8 v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(uint8 v)
	{
		clear();
		
		value.uint8_v = v ;
		type = (char)GDB_FIELD_UINT8 ;
		len = sizeof(v);
		
		return *this ;
	}

    //##ModelId=433E93FF0109
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      uint16 v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(uint16 v)
	{
		clear();
		
		value.uint16_v = v ;
		type = (char)GDB_FIELD_UINT16 ;
		len = sizeof(v);
		
		return *this ;
	}

    //##ModelId=433E942601F4
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      uint32 v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(uint32 v)
	{
		clear();
		
		value.uint32_v = v ;
		type = (char)GDB_FIELD_UINT32 ;
		len = sizeof(v);
		
		return *this ;
	}

    //##ModelId=433E944D00DA
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      char v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(char v)
	{
		clear();
		
		value.int8_v = v ;
		type = (char)GDB_FIELD_INT8 ;
		len = sizeof(v);
		
		return *this ;
	}

    //##ModelId=433E946C036B
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      int16 v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(int16 v)
	{
		clear();
		
		value.int16_v = v ;
		type = (char)GDB_FIELD_INT16 ;
		len = sizeof(v);
		
		return *this ;
	}

    //##ModelId=433E949203B9
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      int32 v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(int32 v)
	{
		clear();
		
		value.int32_v = v ;
		type = (char)GDB_FIELD_INT32 ;
		len = sizeof(v);
		
		return *this ;
	}

    //##ModelId=433E94B300EA
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      FLOAT4 v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(FLOAT4 v)
	{
		clear();
		
		value.float_v = v ;
		type = (char)GDB_FIELD_FLOAT4 ;
		len = sizeof(v);
		
		return *this ;
	}

    //##ModelId=433E94DF00DA
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      FLOAT8 v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(FLOAT8 v)
	{
		clear();
		
		value.double_v = v ;
		type = (char)GDB_FIELD_FLOAT8 ;
		len = sizeof(v);
		
		return *this ;
	}

    //##ModelId=433E9511008C
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      const char* v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(const char* v)
	{
		clear();
		
		if ( v == NULL ) 
			return *this ;
			
		type = (char)GDB_FIELD_STRING ;
		len = strlen( v );
		
		if ( len == 0 )
			return *this ;
			
		value.string = new char[len+1];
		memset( value.string , 0 , len+1 );	
		strncpy( value.string , v , len );
		
		return *this ;
	}

    //##ModelId=4347548B0177
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      int64 v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(int64 v)
	{
		clear();
		
		value.int64_v = v ;
		type = (char)GDB_FIELD_INT64 ;
		len = sizeof(v);
		
		return *this ;
	}

    //##ModelId=434754BF02EE
    //##Documentation
    //## ���ܣ�
    //##      �������غ���
    //## ������
    //##      uint64 v
    //## ���أ�
    //##     const GdbField& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbField& operator=(uint64 v)
	{
		clear();
		
		value.uint64_v = v ;
		type = (char)GDB_FIELD_UINT64 ;
		len = sizeof(v);
		
		return *this ;
	}

	//##ModelId=438C096C0251
    //##Documentation
    //## ���ܣ�
    //##      ת������
    //## ������
    //##      ��
    //## ���أ�
    //##      �ַ���
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-29
    QString toString() const
	{
		char av[GDB_STR_LEN];
		memset( av , 0 , GDB_STR_LEN );

		switch( type )
		{
		case GDB_FIELD_UINT8:
			{
				sprintf(av,"%u",value.uint8_v);
			}
			break;
		case GDB_FIELD_INT8:
			{
				sprintf(av,"%d",value.int8_v);
			}
			break;
		case GDB_FIELD_UINT16:
			{
				sprintf(av,"%u",value.uint16_v);
			}
			break;
		case GDB_FIELD_INT16:
			{
				sprintf(av,"%d",value.int16_v);
			}
			break;
		case GDB_FIELD_UINT32:
			{
				sprintf(av,"%u",value.uint32_v);
			}
			break;
		case GDB_FIELD_INT32:
			{
				sprintf(av,"%d",value.int32_v);
			}
			break;
		case GDB_FIELD_UINT64:
			{
				sprintf(av,"%lu",value.uint64_v);
			}
			break;
		case GDB_FIELD_INT64:
			{
				sprintf(av,"%ld",value.int64_v);
			}
			break;
		case GDB_FIELD_FLOAT4:
			{
				sprintf(av,"%0.3f",value.float_v);
			}
			break;
		case GDB_FIELD_FLOAT8:
			{
				sprintf(av,"%0.6lf",value.double_v);
			}
			break;
		case GDB_FIELD_STRING:
			{
				if ( len )
					sprintf(av,"%s",value.string);
			}
			break;
		}
		
		return g_Q(av);
	}

    //##ModelId=438C09A402DE
    //##Documentation
    //## ���ܣ�
    //##      ת������
    //## ������
    //##      ��
    //## ���أ�
    //##      ������
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-29
    int32 toInt()
	{
		char av[GDB_STR_LEN];
		memset( av , 0 , GDB_STR_LEN );

		int32 rv = 0 ;
		switch( type )
		{
		case GDB_FIELD_UINT8:
			{
				rv = (int32)value.uint8_v ;
			}
			break;
		case GDB_FIELD_INT8:
			{
				rv = (int32)value.int8_v ;
			}
			break;
		case GDB_FIELD_UINT16:
			{
				rv = (int32)value.uint16_v ;
			}
			break;
		case GDB_FIELD_INT16:
			{
				rv = (int32)value.int16_v;
			}
			break;
		case GDB_FIELD_UINT32:
			{
				rv = (int32)value.uint32_v;
			}
			break;
		case GDB_FIELD_INT32:
			{
				rv = value.int32_v;
			}
			break;
		case GDB_FIELD_UINT64:
			{
				rv = (int32)value.uint64_v;
			}
			break;
		case GDB_FIELD_INT64:
			{
				rv = (int32)value.int64_v;
			}
			break;
		case GDB_FIELD_FLOAT4:
			{
				rv = (int32)value.float_v;
			}
			break;
		case GDB_FIELD_FLOAT8:
			{
				rv = (int32)value.double_v;
			}
			break;
		case GDB_FIELD_STRING:
			{
				if ( len )
				{
					rv = (int32)atol(value.string);					
				}
			}
			break;
		}

		return rv ;
	}
    //##ModelId=438C09C8038A
    //##Documentation
    //## ���ܣ�
    //##      ת������
    //## ������
    //##      ��
    //## ���أ�
    //##      ������
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-29
    FLOAT8 toDouble()
	{
		char av[GDB_STR_LEN];
		memset( av , 0 , GDB_STR_LEN );

		FLOAT8 rv = 0 ;
		switch( type )
		{
		case GDB_FIELD_UINT8:
			{
				rv = value.uint8_v ;
			}
			break;
		case GDB_FIELD_INT8:
			{
				rv = (FLOAT8)value.int8_v ;
			}
			break;
		case GDB_FIELD_UINT16:
			{
				rv = (FLOAT8)value.uint16_v ;
			}
			break;
		case GDB_FIELD_INT16:
			{
				rv = (FLOAT8)value.int16_v;
			}
			break;
		case GDB_FIELD_UINT32:
			{
				rv = (FLOAT8)value.uint32_v;
			}
			break;
		case GDB_FIELD_INT32:
			{
				rv = (FLOAT8)value.int32_v;
			}
			break;
		case GDB_FIELD_UINT64:
			{
				rv = (FLOAT8)(uint32)value.uint64_v;
			}
			break;
		case GDB_FIELD_INT64:
			{
				rv = (FLOAT8)(int32)value.int64_v;
			}
			break;
		case GDB_FIELD_FLOAT4:
			{
				rv = (FLOAT8)value.float_v;
			}
			break;
		case GDB_FIELD_FLOAT8:
			{
				rv = value.double_v;
			}
			break;
		case GDB_FIELD_STRING:
			{
				if ( len )
				{
					rv = atof(value.string);					
				}
			}
			break;
		}

		return rv ;
	}

    //##ModelId=433B6F1E001F
    char type;

    //##ModelId=433B6F3B00EA
    UField value;

    //##ModelId=433B6FA50290
    uint16 len;

};

//##ModelId=433CCC5A0121
//##Documentation
//## ���ݿ���¼�洢�ṹ
//## ���ܣ�
//##       ʵ�����ݿ��¼�����޹���
//##       ͳһ���ݿ���ʱ�������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
struct GdbRecord
{
    //##ModelId=433CCD0B00B3
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     ��
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbRecord()
	{
		fldnum = 0 ;
		fields = NULL ;
		delf   = 0 ;
		recRef = NULL ;
	}

    //##ModelId=433CCD160111
    //##Documentation
    //## ���ܣ�
    //##      ��������������clear����ڴ�
    //## ������
    //##     ��
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    ~GdbRecord()
	{
		clear();
	}

    //##ModelId=433CCD22014F
    //##Documentation
    //## ���ܣ�
    //##     ��ռ�¼��Ϣ
    //## ���أ�
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    void clear()
	{
		if ( fldnum )
		{
			delete [] fields ;
		}
		
		fldnum = 0 ;
		fields = NULL ;
		delf   = 0 ;
		recRef = NULL ;
	}

    //##ModelId=433F81EB0399
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     const GdbRecord& rec
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbRecord(const GdbRecord& rec)
	{
		if ( this == &rec )
			return ;
		
		fields = NULL ;
		delf   = 0 ;
		recRef = NULL ;
		fldnum = rec.fldnum ;
		
		if ( fldnum == 0 )
			return ;
			
		delf = rec.delf ;
		recRef = rec.recRef ;
		fields = new GdbField[fldnum];
		g_CHECK_PTR(fields);

		for ( uint32 i = 0 ; i < fldnum ; i ++ )
		{
			fields[i] = rec.fields[i];
		}
	}

    //##ModelId=433F8220038A
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##      const GdbRecord& rec
    //## ���أ�
    //##      const GdbRecord&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbRecord& operator=(
        //##Documentation
        //## const GdbRecord&
        const GdbRecord& rec)
	{
		if ( this == &rec )
			return *this ;
			
		if ( !allocMem( rec.fldnum ) )
			return rec ;
			
		delf = rec.delf ;
		recRef = rec.recRef ;
		
		for ( uint32 col = 0 ; col < fldnum ; col ++ )
		{
			fields[col] = rec[col];
		}
		
		return *this ;
	}

    //##ModelId=434770BC0196
    //##Documentation
    //## ���ܣ�
    //##   �����ڴ�
    //## ������
    //##   uint32 colnum//�и���
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 allocMem(uint32 n)
	{
		if ( n == 0 )      
		{
			clear();
			return 0;
		}
		
		if ( n == fldnum ) return 1;
		
		clear();
		
		fldnum = n ;
		fields = new GdbField[fldnum];
		g_CHECK_PTR(fields);
		
		if (fields) return 1;
		
		fldnum = 0 ;
		
		return 0 ;
	}

    //##ModelId=434778F100DA
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   uint32 index//����
    //## ����(GdbField&)
    //##   if success return GdbField&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    GdbField& operator[](uint32 index)
	{
		if ( index >= fldnum )
			assert(0);
		
		return fields[index];
	}

	//##ModelId=4347BC1F037A
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   uint32 index//����
    //## ����(const GdbField&)
    //##   if success return GdbField&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    const GdbField& operator[](uint32 index) const
	{
		if ( index >= fldnum )
			assert(0);
		
		return fields[index];
	}

    //##ModelId=4347AA40002E
    //##Documentation
    //## ���ܣ�
    //##   �Ƚ�������¼������Ƿ���ȣ������Ƿ�һ��
    //## ������
    //##   const GdbRecord& rec
    //## ���أ�
    //##   if equal return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    //##   
    uint8 operator==(const GdbRecord& rec)
	{
		if ( this == &rec ) return 1 ;

		if ( fldnum != rec.fldnum ) return 0;
		
		for ( uint32 col = 0 ; col < fldnum ; col ++ )
		{
			if( fields[col].type != rec[col].type )
				return 0;
		}
		
		return 1 ;
	}

    //##ModelId=4347AAE1002E
    //##Documentation
    //## ���ܣ�
    //##   �Ƚ�������¼������Ƿ���ȣ������Ƿ�һ��
    //## ������
    //##   const GdbRecord& rec
    //## ���أ�
    //##   if not equal return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 operator!=(const GdbRecord& rec)
	{
		if ( this == &rec ) return 0 ;

		if ( fldnum != rec.fldnum ) return 1;
			
		for ( uint32 col = 0 ; col < fldnum ; col ++ )
		{
			if( fields[col].type != rec[col].type )
				return 1;
		}
		
		return 0 ;
	}

    //##ModelId=433CCC8E0085
    //##Documentation
    //## �����
    uint32 fldnum;

    //##ModelId=433CCCCD03A1
    //##Documentation
    //## ��������Ϣ
    GdbField* fields;

    //##ModelId=433CD53F01BA
    //##Documentation
    //## ɾ����־�����ɾ���� delf=1������ delf=0
    uint8 delf;

    //##ModelId=433CD5CE0276
    //##Documentation
    //## ��¼�ο�ָ�룬����ά�����¼�¼��
    GdbRecord* recRef;

};

//##ModelId=433CD0380219
//##Documentation
//## ���壺
//##        ��¼�����ڴ洢���¼����
//##        �����Ҫ��̬�����ýṹ
//##        �뽫���ݶ����Ϊ����ṹ
//##        ����Ϊ GList<GdbRecord*> recs ��
//## ���ܣ�
//##       ʵ�����ݿ��¼�����޹���
//##       ͳһ���ݿ���ʱ�������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
struct GdbRecords
{
    //##ModelId=433CD07E01CB
    //##Documentation
    //## ���ܣ�
    //##      ɾ����¼��������ڴ�
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    void clear()
	{
		if ( recnum )
		{
			delete [] recs ;
		}
		
		recs = NULL ;
		recnum = 0 ;
	}

    //##ModelId=433CD08B03DE
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbRecords()
	{
		recs = NULL ;
		recnum = 0 ;
	}

    //##ModelId=433CD0930073
    //##Documentation
    //## ���ܣ�
    //##      ��������������clear����ڴ�
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    ~GdbRecords()
	{
		clear();
	}

    //##ModelId=433F828F03C8
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     const GdbRecords& recs
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    GdbRecords(const GdbRecords& rs)
	{
		if ( this == &rs )
			return ;
			
		recs = NULL ;
		recnum = rs.recnum ;
		
		if( recnum == 0 )
			return ;
		
		recs = new GdbRecord[recnum];
		g_CHECK_PTR(recs);
		
		for ( uint32 i = 0; i < recnum ; i ++ )
		{
			recs[i] = rs.recs[i];
		}
		
		datafmt = rs.datafmt ;
	}

    //##ModelId=433F82CC01C5
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     const GdbRecords& recs
    //## ���أ�
    //##     const GdbRecords&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const GdbRecords& operator=(const GdbRecords& rs)
	{
		if ( this == &rs )
			return *this ;
		
		clear();
		
		if( !allocMem(rs.recnum))
			return rs ;
		
		for ( uint32 i = 0; i < recnum ; i ++ )
		{
			recs[i] = rs.recs[i];
		}	
		
		datafmt = rs.datafmt ;
		
		return *this ;
	}

    //##ModelId=434770940157
    //##Documentation
    //## ���ܣ�
    //##   �����ڴ�
    //## ������
    //##   uint32 recnum//��¼����
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 allocMem(uint32 n)
	{
		if ( n == 0 )      
		{
			clear();
			return 0;
		}
		
		if ( n == recnum ) return 1;
		
		clear();
		
		recnum = n ;
		recs = new GdbRecord[recnum];
		g_CHECK_PTR(recs);

		if (recs) return 1;
		
		recnum = 0 ;
		
		return 0 ;
	}

    //##ModelId=434777CB0242
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   uint32 row//����
    //## ����(GdbRecord&)
    //##   if success return GdbRecord&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    GdbRecord& operator[](uint32 row)
	{
		if ( row >= recnum )
			assert(0);
		
		return recs[row];
	}

    //##ModelId=4347BC7F01B5
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   uint32 row//����
    //## ����(const GdbRecord&)
    //##   if success return GdbRecord&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    const GdbRecord& operator[](uint32 row) const
	{
		if ( row >= recnum )
			assert(0);
		
		return recs[row];
	}

	//##ModelId=4347BC7F01B5
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   uint32 row//����
    //## ����(const GdbRecord&)
    //##   if success return GdbRecord&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8	
	GdbField& at(uint32 row,uint32 col)
	{
		if ( row >= recnum )
			assert(0);

		if ( col >= datafmt.colnum)
			assert(0);
		
		return recs[row][col];
	}

	//##ModelId=4347BC7F01B5
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   uint32 index//����
    //## ����(const GdbRecord&)
    //##   if success return GdbRecord&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
	const GdbField& at(uint32 row,uint32 col) const
	{
		if ( row >= recnum )
			assert(0);

		if ( col >= datafmt.colnum)
			assert(0);
		
		return recs[row][col];
	}

	//##ModelId=4347BC7F01B5
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   uint32 row//����
    //## ����(const GdbRecord&)
    //##   if success return GdbRecord&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8	
	GdbField& at(uint32 row,const char* fldname)
	{
		uint32 col = datafmt.exist(fldname);

		return at(row,col);
	}

	//##ModelId=4347BC7F01B5
    //##Documentation
    //## ���ܣ�
    //##   ���ؽű�������ţ����ڲ�����¼
    //## ������
    //##   uint32 index//����
    //## ����(const GdbRecord&)
    //##   if success return GdbRecord&
    //##   else exit
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
	const GdbField& at(uint32 row,const char* fldname) const
	{
		uint32 col = datafmt.exist(fldname);

		return at(row,col);
	}

    //##ModelId=433CD0E301F9
    //##Documentation
    //## �洢��¼����
    uint32 recnum;

    //##ModelId=433CD10A0257
    //##Documentation
    //## ��¼���ݼ�
    GdbRecord* recs;

    //##ModelId=43476CE0034B
    //##Documentation
    //## ��ṹ
    GdbDataFmt datafmt;

};

//##ModelId=4349D5D8035B
//##Documentation
//## ���壺
//##        ���ݿ����Ϣ�ṹ����
//## ���ܣ�
//##       ʵ�����ݿ��¼�����޹���
//##       ͳһ���ݿ���ʱ�������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-10-10
struct GdbTableInfo
{
    //##ModelId=4349D81D02BF
    //##Documentation
    //## ������
    char name[GDB_FMT_LEN];

    //##ModelId=4349D8440399
    //##Documentation
    //## ������
    char full_name[GDB_FMT_LEN];

    //##ModelId=4349D86F005D
    //##Documentation
    //## ���¼����
    uint32 reclen;

    //##ModelId=4349D89A030D
    //##Documentation
    //## ���¼����
    uint32 recrcn;

    //##ModelId=4349D8F50213
    //##Documentation
    //## ����
    uint32 colnum;

    //##ModelId=4349D91103D8
    //##Documentation
    //## ������
    uint32 attr;

    //##ModelId=434A19380232
    //##Documentation
    //## ����������
    char srvname[GDB_FMT_LEN];

};

//##ModelId=4349D60800AB
//##Documentation
//## ���壺
//##       ����Ϣ����
//## ���ܣ�
//##       ʵ�����ݿ��¼�����޹���
//##       ͳһ���ݿ���ʱ�������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-10-10
struct GdbTable
{
  public:
    //##ModelId=4349D70A0232
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-10
    GdbTable()
	{
		tblnum  = 0 ;
		tblinfo = NULL ;
	}

    //##ModelId=4349D72B0271
    //##Documentation
    //## ���ܣ�
    //##      ��������
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-10
    virtual ~GdbTable()
	{
		clear();
	}

    //##ModelId=4349D748031C
    //##Documentation
    //## ���ܣ�
    //##      �����ݺ���
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-10
    void clear()
	{
		if ( tblinfo )
		{
			delete [] tblinfo ;
		}

		tblnum  = 0 ;
		tblinfo = NULL ;
	}

    //##ModelId=4349D75C037A
    //##Documentation
    //## ���ܣ�
    //##      ��ֵ����
    //## ������
    //##     const GdbTable& tbl //Դֵ
    //## ���أ�
    //##     const GdbTable&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-10
    const GdbTable& operator=(const GdbTable& tbl)
	{
		if ( this == &tbl )
			return *this ;

		if ( !allocMem( tbl.tblnum ) )
			return tbl ;

		memcpy( tblinfo , tbl.tblinfo , tbl.tblnum * sizeof(GdbTableInfo));

		return *this ;

	}

    //##ModelId=4349D7AB03D8
    //##Documentation
    //## ���ܣ�
    //##      ���ر�������
    //## ������
    //##     uint32 index //λ��
    //## ���أ�
    //##     GdbTableInfo&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-10
    GdbTableInfo& operator[](uint32 index)
	{
		if ( index >= tblnum )
			assert(0);
		
		return tblinfo[index];
	}

    //##ModelId=4349DC26035B
    //##Documentation
    //## ���ܣ�
    //##   �����ڴ�
    //## ������
    //##   uint32 colnum//�и���
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 allocMem(uint32 n)
	{
		if ( n == 0 )
		{
			clear();

			return 0;
		}

		if ( tblnum == n ) 
			return 1;	
		
		clear();

		tblinfo = new GdbTableInfo[n];
		g_CHECK_PTR(tblinfo);

		if ( !tblinfo )  return 0;

		memset( tblinfo , 0 , n*sizeof(GdbTableInfo));

		tblnum = n ;

		return 1 ;
	}

    //##ModelId=4349DD8B038A
    //##Documentation
    //## ���ܣ�
    //##      ��ʼ�����캯��
    //## ������
    //##     const GdbTable& tbl //��ֵ
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-10
    GdbTable(const GdbTable&  tbl)
	{
		if ( this == &tbl )
			return ;
			
		tblnum = 0  ;
		tblinfo= NULL ;
		
		if ( tbl.tblnum  == 0 )
			return ;
		
		tblinfo = new GdbTableInfo[tblnum];
		g_CHECK_PTR(tblinfo);
		
		if ( !tblinfo ) return ;
		
		tblnum = tbl.tblnum ;
		memcpy( tblinfo , tbl.tblinfo , tbl.tblnum * sizeof(GdbTableInfo));
	}

    //##ModelId=434A0374009C
    //##Documentation
    //## ���ܣ�
    //##      ���ر�������
    //## ������
    //##     uint32 index //λ��
    //## ���أ�
    //##     const GdbTableInfo&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-10
    const GdbTableInfo& operator[](uint32  index) const
	{
		if ( index >= tblnum )
			assert(0);
		
		return tblinfo[index];
	}

    //##ModelId=4349D6C20290
    //##Documentation
    //## �����
    uint32 tblnum;

    //##ModelId=4349D6DB008C
    //##Documentation
    //## ����Ϣ
    GdbTableInfo* tblinfo;

};

#endif
