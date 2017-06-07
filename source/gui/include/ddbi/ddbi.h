#ifndef DDBI_H_HEADER_INCLUDED_BCB03856
#define DDBI_H_HEADER_INCLUDED_BCB03856

#include "ddbi/ddbi_def.h"

class QLibrary;
class DdbiBase ;

//���ض�̬��ص�����
//symbol = dbiFactoryBuild
//

typedef DdbiBase* (*DBIFB_PROC)();

//##ModelId=434F8CBC034B
//##Documentation
//## ���壺
//##        ���ݿ�ӿ���
//##        �̳���ʵ�־����������      
//## ���ܣ�
//##        ʵ�����ݿ��޹���ӿ�
//##        �ṩ�����ݽṹ��ѯ
//##        �ṩ���ݿ��ѯ��ɾ�������룬���²���
//##        ֧��ֱ��ʹ��SQL���Բ�������
//##       ͳһ���ݿ���ʱ���ݿ��������    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
//## ������
//##   const char*  PASSWDT="�û������";
//## 
//##   ddbi ddbi;
//##   if ( !ddbi.startdb() )
//##     return 0;
//##   
//##   if ( !ddbi.selectFrom(PASSWDT)
//##     return 0;
//##   
//##   GdbRecords dbrecs ;
//## 
//##   if ( !ddbi.selectRecords( dbrecs , 
//##            "where ����Ա=sa") )
//##      return 0;
//##   
//##   ...... 
//##  
//##   return 1;
class GDE_EXPORT_DBI Ddbi
{
  public:
    //##ModelId=434F906A008C
    //##Documentation
    //## ���ݿ�����
    //## ����createDBI
    enum  { 
        //##Documentation
        //## ��ʷ������
        GD_DBI_HIS = 0, 
        //##Documentation
        //## ʵʱ���ݿ�
        GD_DBI_REAL = 1, 
        //##Documentation
        //## ODBC���ݿ�
        GD_DBI_ODBC = 2 };
    
     //##ModelId=434FA17700DA
    enum  { 
        //##Documentation
        //## ֻ����ʽ��
        G_DB_R = 0x01, 
        //##Documentation
        //## ��д��ʽ��
        G_DB_RW = 0x02 };

	//##ModelId=434FA17700DA
	enum{
		G_DBT_ODBC_UNKNOWN	=0x00,
		G_DBT_ODBC_ORACLE	=0x01,
		G_DBT_ODBC_SYBASE	=0x02,//default
		G_DBT_ODBC_SQLSERVER=0x03,
		G_DBT_ODBC_MYSQL	=0x04,
		G_DBT_ODBC_DB2		=0x05 };

    //##ModelId=434F8D17031C
    //##Documentation
    //## ���ܣ�
    //##     �������ݿ⣬Ĭ��Ϊ��ȡ����
    //## ������
    //##     uint32 opmode //�򿪷�ʽ��G_DB_R��G_DB_RW��
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    virtual uint8 startdb(
        //##Documentation
        //## ����ѡ�G_DB_R��G_DB_RW��
        //## Ĭ��ΪG_DB_R
        uint32 opmode = G_DB_R);

    //##ModelId=434F8D1D0109
    //##Documentation
    //## ���ܣ�
    //##     �ر����ݿ�
    //## ������
    //##     ��
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    virtual uint8 closedb();

    //##ModelId=434F8D1F0290
    //##Documentation
    //## ���ܣ�
    //##     �����������ݿ�����
    //## ������
    //##     const char* dbname //���ݿ�����
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    uint8 setDB(
        //##Documentation
        //## ָ�����ݿ���
        const char* dbname);

    //##ModelId=434F8D210222
    //##Documentation
    //## ���ܣ�
    //##     ȡ���������ݿ���
    //## ������
    //##    ��
    //## ���أ���const char* ��
    //##     if success return dbname
    //##     else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const char* getDB() const;

    //##ModelId=434F8D23031C
    //##Documentation
    //## ���ܣ�
    //##     �������ñ���
    //##     ���ڶ�ȡ���ݿ����Ϣ
    //##     �ı�����������ȵ��ô˺���
    //## ������
    //##     const char* tblname //��ȡ�ı���
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    uint8 selectFrom(
        //##Documentation
        //## ���ݿ����
        const char* tblname);

    //##ModelId=434F8D2600DA
    //##Documentation
    //## ���ܣ�
    //##     ȡ���ñ�����
    //## ������
    //##     ��
    //## ���أ���const char* ��
    //##     if success return tblname
    //##     else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const char* getTable() const;

    //##ModelId=434F8D660261
    //##Documentation
    //## ���ܣ�
    //##     �����������ݿ��¼�û�����
    //## ������
    //##     const char* user//��¼�û�����
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    uint8 setUser(const char* user = NULL);

    //##ModelId=434F8D6D029F
    //##Documentation
    //## ���ܣ�
    //##     �����������ݿ��¼�û�����
    //## ������
    //##     const char* passwd//��¼�û�����
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    uint8 setPass(const char* passwd = NULL);

    //##ModelId=434F8D7203D8
    //##Documentation
    //## ���ܣ�
    //##     ȡ���õ�¼�û���
    //## ������
    //##     ��
    //## ���أ���const char* ��
    //##     if success return usrname
    //##     else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const char* getUser();

    //##ModelId=434F8D7B0242
    //##Documentation
    //## ���ܣ�
    //##     ȡ���õ�¼�û�����
    //## ������
    //##     ��
    //## ���أ���const char* ��
    //##     if success return passwd
    //##     else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    const char* getPass();

    //##ModelId=434F8D820177
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ������Ϣ
    //## ������
    //##   ��
    //## ���أ�
    //##   const char*
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    const char* getError();


    //##ModelId=434F8D9401D4
    //##Documentation
    //## ���ܣ�
    //##     ��ȡȫ�����¼����Ϣ
    //## ������
    //##     GdbRecords& records//�洢�������м�¼
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-9
    virtual uint8 readRecords(
        //##Documentation
        //## ��¼����Ϣ
        GdbRecords& records);

    //##ModelId=434F8D9C00BB
    //##Documentation
    //## ���ܣ�
    //##     д��ȫ�����¼����Ϣ
    //## ������
    //##     GdbRecords& records//�洢�������м�¼
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-8
    virtual uint8 writeRecords(GdbRecords& records);

    //##ModelId=434F8DE000FA
    //##Documentation
    //## ���ܣ�
    //##     ����ָ��������ѡȡһ����¼��ĳһ�ֶ�
    //## ������
    //##     GdbField& value//��ȡ��ֵ
    //##     const char* keyfield//�ؼ��ֶ���
    //##     const char* keyword //�����ؼ���
    //##     const char* field   //ѡȡ�ֶ���
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 selectAField(GdbField& value, const char* keyfield, const char* keyword, const char* field);

    //##ModelId=434F8DE6005D
    //##Documentation
    //## ���ܣ�
    //##     ��ָ��where�����Ӿ��ȡѡȡ��¼����Ϣ
    //## ������
    //##     GdbRecords& records//�洢ѡ�еļ�¼
    //##     const char* where  //�����Ӿ䣬ѡȡ��¼
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    virtual uint8 selectRecords(GdbRecords& records, const char* where = NULL);

    //##ModelId=434F8DEB006D
    //##Documentation
    //## ���ܣ�
    //##     ���뵥����¼
    //## ������
    //##     const GdbRecord& rec//������¼
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 insertARecord(const GdbRecord& rec);

    //##ModelId=434F8DF00261
    //##Documentation
    //## ���ܣ�
    //##     ���ϼ�¼����Ϊ�¼�¼
    //## ������
    //##     const GdbRecord& oldRec //���м�¼ 
    //##     const GdbRecord& newRec //�¼�¼
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 updateARecord(const GdbRecord& oldRec, const GdbRecord& newRec);

    //##ModelId=434F8DF902DE
    //##Documentation
    //## ���ܣ�
    //##     ɾ��һ����¼
    //## ������
    //##     const GdbRecord& rec//ɾ���ļ�¼
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 removeARecord(const GdbRecord& rec);

    //##ModelId=434F8DFF0399
    //##Documentation
    //## ���ܣ�
    //##     ִ���޷���SQL���
    //## ������
    //##     const char* sql//��ִ�е�SQL���
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 execSQL(const char* sql);

    //##ModelId=434F8E060128
    //##Documentation
    //## ���ܣ�
    //##     ȡ������Ϣ������ɹ�����Ϣ�洢��colfmt��
    //## ������
    //##     GdbDataFmt& colfmt //����Ϣ
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    virtual uint8 getFmt(
        //##Documentation
        //## ������Ϣ
        GdbDataFmt& colfmt);

    //##ModelId=434F8E0F00CB
    //##Documentation
    //## ���ܣ�
    //##      ȡ�ñ���Ϣ
    //## ������
    //##      GdbTable& tbl //����Ϣ
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-10-10
    virtual uint8 getTbl(GdbTable& tbl);

    //##ModelId=434F8E9B0157
    //##Documentation
    //## ���ܣ�
    //##   ����DBI�ӿ�
    //##   Ŀǰ֧��GD_DBI_HIS
    //##           GD_DBI_REAL
    //##           GD_DBI_ODBC
	//##   ����ǰ�������ýӿ�·��
    //## ������
    //##    uint8 dbiType//���ݿ�ӿ�����
    //## ���أ�
    //##    if success return 1;
    //##    else return 0;
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-14
    virtual uint8 createDBI(uint8 dbi);
	//##ModelId=434F8E9B0157
    //##Documentation
    //## ���ܣ�
    //##   ����DBI�ӿ�
    //## ������
    //##    const char* libpath//���ݿ�ӿ��ļ�·��
    //## ���أ�
    //##    if success return 1;
    //##    else return 0;
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-14
	virtual uint8 createDBI(const QString& libpath);
	//##ModelId=435C9AD90138
    //##Documentation
    //## ���ܣ�
    //##     ִ���з���select-SQL���
    //## ������
    //##     GdbRecords& recs//������ݻ�����
    //##     const char* sql//��ִ�е�SQL���
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-24
    virtual uint8 readSQL(GdbRecords& recs, const char* sql);

    //##ModelId=434F8F5902CE
	//##Documentation
    //## ���ܣ�
    //##     ���캯��
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-10-24
    Ddbi();

    //##ModelId=434F8F610213
	//##Documentation
    //## ���ܣ�
    //##     ��������
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-10-24
    ~Ddbi();

	//##ModelId=434F8F610213
	//##Documentation
    //## ���ܣ�
    //##    �ж����ݿ�����
    //## ������
    //##    int32 dbType;//���ݿ�����
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-10-24
	virtual uint8 isDBType ( int32 dbType ) const;

	//##ModelId=434F8F610213
	//##Documentation
    //## ���ܣ�
    //##    �������ݿ�����
    //## ������
    //##    int32 dbType;//���ݿ�����
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-10-24
	void	setDBType( int32 dbType );

	//##ModelId=434F8F610213
	//##Documentation
    //## ���ܣ�
    //##    ��ȡ���ݿ�����
    //## ������
    //##    ��
    //## ���أ�int32
    //##    ���ݿ�����
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##     2005-10-24
	int32   getDBType() const;

	//##ModelId=434F8F610213
	//##Documentation
    //## ���ܣ�
    //##    ���ݿ��Ƿ��
    //## ������
    //##    ��
    //## ���أ�(uint8)
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##     2005-10-24
	uint8   isOpen() const;

	//##ModelId=434F8F610213
	//##Documentation
    //## ���ܣ�
    //##    �������ݿ���·��
    //## ������
    //##    const char* dbiPath//���·��
    //## ���أ�
    //##    ��
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##     2005-10-24
	void	setDBIPath(const QString& dbiPath);

  protected:
    //##ModelId=434F8E5B02FD
    //##Documentation
    //## ���ݿ�����ڲ��ӿ�
    DdbiBase* m_ddbi;

	//##ModelId=434F8E5B02FD
    //##Documentation
    //## ���ݿ������ؽӿ�
	QLibrary* m_hLibrary;

	//##ModelId=434F8E5B02FD
    //##Documentation
    //## ���ݿ���·��
	QString	  m_dbiPath;

};


//##ModelId=433B75FE0399
//##Documentation
//## ���壺
//##        ȡ��ʷ����ĳһ����ֵ
//## ���ܣ�
//##       ʵ�����ݿ��¼�����޹���
//##       ͳһ���ݿ���ʱ�������Ͷ���    
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
class GdbFieldData
{
  public:
    //##ModelId=433CE0EC0224
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
    //##       2005-10-1
    GdbFieldData()
	{
	}

    //##ModelId=433B772F00FA
    //##Documentation
    //## ���ܣ�
    //##    ȡ���ݹ��죬���캯���е���get
    //## ������
    //##    ddbi* ddbi    //���ݿ�
    //##    const char* tblname //����
    //##    const char* keyfield//�ؼ�������
    //##    const char* keyword //�ؼ���
    //##    const char* field   //ѡȡ��
    //## ���أ�
    //##    ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    GdbFieldData(
        //##Documentation
        //## ���ݿ�ӿ�
        Ddbi* ddbi, 
        //##Documentation
        //## ��ȡ����
        const char* tblname, 
        //##Documentation
        //## �ؼ�������
        const char* keyfield, 
        //##Documentation
        //## �ؼ�����ֵ
        const char* keyword, 
        //##Documentation
        //## ��������
        const char* field)
	{
		get( ddbi , tblname , keyfield , keyword , field );
	}

    //##ModelId=433B785100CB
    //##Documentation
    //## ���ܣ�
    //##     ִ��ȡ���ݹ���
    //## ������
    //##    ddbi* ddbi    //���ݿ�
    //##    const char* tblname //����
    //##    const char* keyfield//�ؼ�������
    //##    const char* keyword //�ؼ���
    //##    const char* field   //ѡȡ��
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    //##   
    uint8 get(Ddbi* ddbi, 
        //##Documentation
        //## ��ȡ����
        const char* tblname, 
        //##Documentation
        //## �ؼ�������
        const char* keyfield, 
        //##Documentation
        //## �ؼ�����ֵ
        const char* keyword, 
        //##Documentation
        //## ��������
        const char* field)
	{
		if ( ddbi == NULL ) 
			return 0;
		
		if ( !ddbi->selectFrom( tblname ) )
			return 0 ;
		
		return ddbi->selectAField( value , keyfield , keyword , field );
	}

    //##ModelId=433D2F2F0148
    //##Documentation
    //## ���ܣ�
    //##    ��������������value.clear�������
    //## ������
    //##    ��
    //## ���أ�
    //##    ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    ~GdbFieldData()
	{
	}

    //##ModelId=433F8106031C
    //##Documentation
    //## ���ܣ�
    //##      ��ֵ���غ���
    //## ������
    //##      const GdbFieldData& field
    //## ���أ�
    //##     const GdbFieldData& 
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
	const GdbFieldData& operator=(const GdbFieldData& field)
	{
		if ( this == &field )
			return *this ;
			
		value = field.value ;
		
		return *this ;
	}

    //##ModelId=433F816700DA
    //##Documentation
    //## ���ܣ�
    //##     ��ʼ�����캯��
    //## ������
    //##     const GdbFieldData& field
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-1
	GdbFieldData(const GdbFieldData& field )
	{
		if ( this == &field )
			return ;
			
		value = field.value ;
	}

    //##ModelId=433B763F0186
    GdbField value;
};

#endif /* DDBI_H_HEADER_INCLUDED_BCB03856 */

