#ifndef DDBIBASE_H_HEADER_INCLUDED_BCB071E7
#define DDBIBASE_H_HEADER_INCLUDED_BCB071E7

#include "ddef/ddes_comm.h"
#include "ddbi_def.h"

//##ModelId=433CBE8C0174
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
//##   ddbiBase ddbi;
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
//##  
class GDE_EXPORT DdbiBase
{
  public:
    //##ModelId=434734500157
    enum  { 
        //##Documentation
        //## ֻ����ʽ��
        G_DB_R = 0x01, 
        //##Documentation
        //## ��д��ʽ��
        G_DB_RW = 0x02 };

	enum{
		G_DBT_ODBC_UNKNOWN	=0x00,
		G_DBT_ODBC_ORACLE	=0x01,
		G_DBT_ODBC_SYBASE	=0x02,
		G_DBT_ODBC_SQLSERVER=0x03,
		G_DBT_ODBC_MYSQL	=0x04,
		G_DBT_ODBC_DB2		=0x05 };

    //##ModelId=433CC3B801EF
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

    //##ModelId=433CC5550068
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

    //##ModelId=433CC913028A
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

    //##ModelId=433CC98D00C5
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

    //##ModelId=433CCE450304
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

    //##ModelId=433CCF2201BC
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

    //##ModelId=433CC77701FD
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
        GdbDataFmt& colfmt) = 0;

    //##ModelId=433CE2100188
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
    //##       2005-9-30
    virtual uint8 selectAField(
        //##Documentation
        //## ����ֵ
        GdbField& value, 
        //##Documentation
        //## �ؼ�������
        const char* keyfield, 
        //##Documentation
        //## �ؼ���ֵ
        const char* keyword, 
        //##Documentation
        //## ��ѡȡ���ֶ�����
        const char* field) = 0;

    //##ModelId=433CD809011D
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
    virtual uint8 selectRecords(
        //##Documentation
        //## ��¼��
        GdbRecords& recs, 
        //##Documentation
        //## �����Ӿ䣬���м�¼���ˣ�if where=NULL �����ȼ���readRecords
        const char* where = NULL) = 0;

    //##ModelId=433CD2140286
    //##Documentation
    //## ���ܣ�
    //##     ���뵥����¼
    //## ������
    //##      const GdbRecord& rec//������¼
    //## ���أ���uint8��
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-30
    virtual uint8 insertARecord(
        //##Documentation
        //## ��¼��Ϣ
        const GdbRecord& rec) = 0;

    //##ModelId=433CD2F500EF
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
    //##       2005-9-30
    virtual uint8 updateARecord(
        //##Documentation
        //## �ϼ�¼
        const GdbRecord& oldRec, 
        //##Documentation
        //## �¼�¼
        const GdbRecord& newRec) = 0;

    //##ModelId=433CD3000351
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
    //##       2005-9-30
    virtual uint8 removeARecord(
        //##Documentation
        //## ���¼
        const GdbRecord& rec) = 0;

    //##ModelId=433CD7690218
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
    //##       2005-9-30
    virtual uint8 execSQL(
        //##Documentation
        //## SQL���
        const char* sql) = 0;

    //##ModelId=433E9EF300DA
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
    DdbiBase();

    //##ModelId=433E9EF90271
    //##Documentation
    //## ���ܣ�
    //##     ��������
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-1
    virtual ~DdbiBase();

    //##ModelId=4347285101D4
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

    //##ModelId=4347285801C5
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

    //##ModelId=434728FE0232
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

    //##ModelId=43472925030D
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

    //##ModelId=4347618F0196
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

    //##ModelId=434762A20186
    //##Documentation
    //## ���ܣ�
    //##   ���������Ϣ��m_strerr
    //## ������
    //##   const char* format//��ʽ
    //##   ...//�ɱ����
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    void errFormat(const char* format, ...);

    //##ModelId=433CCD62018E
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

    //##ModelId=4349293F0167
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

    //##ModelId=4349D93F034B
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
    virtual uint8 getTbl(GdbTable& tbl) = 0;
	//##ModelId=435C99FB0177
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
    virtual uint8 readSQL(GdbRecords& recs, const char* sql) = 0;

	//##ModelId=435C99FB0177
    //##Documentation
    //## ���ܣ�
    //##     �ж����ݿ�����
    //## ������
    //##     int32 dbType
    //## ���أ�(uint8)
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-24
	virtual uint8 isDBType ( int32 dbType );

	//##ModelId=435C99FB0177
    //##Documentation
    //## ���ܣ�
    //##     �������ݿ�����
    //## ������
    //##     int32 dbType
    //## ���أ�(uint8)
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-24
	void	setDBType( int32 dbType );
	//##ModelId=435C99FB0177
    //##Documentation
    //## ���ܣ�
    //##     ��ȡ���ݿ�����
    //## ������
    //##     
    //## ���أ�(int32)
    //##     
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-24
	int32   getDBType() const;
	
	virtual uint8 isOpen() const;

  protected:
    //##ModelId=433CC8A903E1
    //##Documentation
    //## �������ݿ�����
    //## ���ڶ����ݿ�ϵͳ��sybase
    char m_dbname[GDB_FMT_LEN];

    //##ModelId=433CCE050391
    //##Documentation
    //## �������ݿ�����������ϴ�ѡȡ��ı���
    char m_tblname[GDB_FMT_LEN];

    //##ModelId=434727D80128
    //##Documentation
    //## ��¼����
    char m_passwd[GDB_FMT_LEN];

    //##ModelId=434727DE0242
    //##Documentation
    //## ��¼�û�����
    char m_usrname[GDB_FMT_LEN];

    //##ModelId=4347615102EE
    //##Documentation
    //## ������Ϣ����
    char m_strerr[GDB_STR_LEN];

	int32  m_dbType ;

};

#endif /* DDBIBASE_H_HEADER_INCLUDED_BCB071E7 */
