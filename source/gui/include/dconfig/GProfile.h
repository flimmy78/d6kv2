#ifndef GPROFILE_H_HEADER_INCLUDED_BCB8E460
#define GPROFILE_H_HEADER_INCLUDED_BCB8E460
// #include <qstring.h>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>

#include "ddef/ddes_comm.h"
#include "ddef/GList.h"

//##ModelId=4345C2FF0109
//##Documentation
//## ���壺
//##   �����ļ��ؼ��ּ���ֵ
//## ���ܣ�
//##   ���������ļ���ȡ����
//## ��ƣ�
//##   ������
//## ���ڣ�
//##   2005-10-7
struct GDE_EXPORT_CONF GPFS
{
    //##ModelId=434E11F8025C
    //##Documentation
    //## ���ܣ�
    //##   ���캯��
    //## ������
    //##   const GPFS& gpfs
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    GPFS(const GPFS& gpfs)
	{
		key = gpfs.key ;
		value = gpfs.value ;
	}
    //##ModelId=434E122E0365
    //##Documentation
    //## ���ܣ�
    //##   ���캯��
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    GPFS()
	{
	}
    //##ModelId=434E123A00B6
    //##Documentation
    //## ���ܣ�
    //##   ��ֵ
    //## ������
    //##   const GPFS&
    //## ���أ�
    //##   const GPFS&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    const GPFS& operator=(const GPFS& gpfs)
	{
		if ( this == &gpfs )
			return *this ;

		key = gpfs.key ;
		value = gpfs.value ;

		return *this ;
	}
	//##ModelId=435D8257004D
    //##Documentation
    //## ���ܣ�
    //##   ���캯��
    //## ������
    //##   const char* k//�ؼ���
    //##   const char* v//ֵ
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    GPFS(const QString& k, const QString& v)
	{
		key   = k ;
		value = v ;
	}


    //##ModelId=4345C356030D
    //##Documentation
    //## �ؼ���
    QString key;

    //##ModelId=4345C37002AF
    //##Documentation
    //## ֵ
    QString value;

};

class GDE_EXPORT_CONF GListPFS: public GList<GPFS *>{};

//##ModelId=43422ED200CB
//##Documentation
//## ���壺
//##   �����ļ���д
//## ���ܣ�
//##   ����ϵͳ�����ļ���д����
//##   �����ļ��������¸�ʽ���д洢
//##   #------ע�����
//##   [func_block1]
//##   keyword1=value
//##   keyword2=value
//##   [func_block2]
//##   keyword1=value
//##   keyword2=value
//## ��ƣ�
//##   ������
//## ���ڣ�
//##   2005-10-06
class GDE_EXPORT_CONF GProfile
{
  public:
    //##ModelId=434E050B03A7
    //##Documentation
    //## ���ܣ�
    //##   ���캯��
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    GProfile();
	//##ModelId=434E0502029D
    //##Documentation
    //## ���ܣ�
    //##   ��������
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    ~GProfile();
	//##ModelId=434E0AE102FA
    //##Documentation
    //## ���ܣ�
    //##   ���������ļ�
    //## ������
    //##   const char* profile //�����ļ�
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    //##  
    virtual uint8 create(const char* profile);
    //##ModelId=434E0AE102FA
    //##Documentation
    //## ���ܣ�
    //##   �������ļ�
    //## ������
    //##   const char* profile //�����ļ�
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    //##  
    virtual uint8 open(const char* profile);
	//##ModelId=434E0D8B029B
    //##Documentation
    //## ���ܣ�
    //##   �ر������ļ�
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
	virtual void  close();
    //##ModelId=434E0D8B029B
    //##Documentation
    //## ���ܣ�
    //##   �����ļ��Ƿ����ʹ��
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    virtual uint8 isValid();
	//##ModelId=4345C41C0177
    //##Documentation
    //## ���ܣ�
    //##   ���ע��
    //## ������
    //##   const char* comment //ע��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    //##   
	virtual uint8 addComment(const char* comment);
    //##ModelId=4345C41C0177
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�����ļ�
    //## ������
    //##   const char* entry //��ڶ���
    //##   const char* key   //�ؼ���
    //##   char* value       //ֵ
    //##   const char* def   //ȱʡֵ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    //##   
    virtual uint8 getString(const char* entry, const char* key, char* value, const char* def = NULL);

    //##ModelId=4345C42502FD
    //##Documentation
    //## ���ܣ�
    //##   д�������ļ�
    //## ������
    //##   const char* entry //��ڶ���
    //##   const char* key   //�ؼ���
    //##   char* value       //ֵ
    //##   const char* def   //ȱʡֵ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    virtual uint8 writeString(const char* entry, const char* key, const char* value);

	//##ModelId=4345C42502FD
    //##Documentation
    //## ���ܣ�
    //##   ���������ļ�
    //## ������
    //##   GString& strDst    //���
    //##   const char* strSrc //�������������ַ���
	//##   ��������:SYSHOME=c:
	//##   ����:
	//##	strSrc=$(SYSHOME)/bin
	//##	strDst=c:/bin
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
	static uint8 parseString(QString& strDst,const char* strSrc);

	//##ModelId=4345C42502FD
    //##Documentation
    //## ���ܣ�
    //##   ���������ļ�
    //## ������
    //##   GString& strDst    //���
    //##   const char* strSrc //�������������ַ���
	//##   const char* env	  //��������
	//##   ��������:SYSHOME=c:
	//##   ����:
	//##	env="SYSHOME"
	//##	strSrc=c:/bin
	//##	strDst=$(SYSHOME)/bin
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
	static uint8 makeString(QString& strDst,const char* strSrc,const char* env);
	
    //##ModelId=4345C59D02CE
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�����ļ���������
    //## ������
    //##   const char* entry //��ڶ��� 
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    virtual uint8 readBlock(const char* entry);
	//##ModelId=4345C59D02CE
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�����ļ����ж�
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    virtual uint8 readEntries();
	//##ModelId=4345C74E02FD
    //##Documentation
    //## ���ܣ�
    //##     ȡ�׽ڵ�λ��
    //## ������
    //##    ��
    //## ���أ���GPOS��
    //##     if success return pos
    //##     else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
	GListPFS& getPFS();
	//##ModelId=4345C74E02FD
    //##Documentation
    //## ���ܣ�
    //##     ȡ�׽ڵ�λ��
    //## ������
    //##    ��
    //## ���أ���GPOS��
    //##     if success return pos
    //##     else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
	const GListPFS& getPFS() const;
    //##ModelId=4345C74E02FD
    //##Documentation
    //## ���ܣ�
    //##     ȡ�׽ڵ�λ��
    //## ������
    //##    ��
    //## ���أ���GPOS��
    //##     if success return pos
    //##     else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    GPOS firstPos() const;

    //##ModelId=4345C7820119
    //##Documentation
    //## ���ܣ�
    //##     ȡ��һ���ڵ�λ��
    //## ������
    //##    GPOS& pos //��ȡ��һ��λ��
    //## ���أ���GPFS&��
    //##     if success return GPFS&
    //##     else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    GPFS& nextPos(GPOS& pos);
	//##ModelId=4345C7820119
    //##Documentation
    //## ���ܣ�
    //##     ȡ��һ���ڵ�λ��
    //## ������
    //##    GPOS& pos //��ȡ��һ��λ��
    //## ���أ���GPFS&��
    //##     if success return GPFS&
    //##     else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GPFS& nextPos(GPOS& pos) const;
    //##ModelId=434E04A101D3
    //##Documentation
    //## ���ܣ�
    //##   ����m_block��������������
    //## ������
    //##   ��
    //## ���أ�
    //##   uint32
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    uint32 count() const;
    //##ModelId=43508BB70157
    //##Documentation
    //## ���ܣ�
    //##   д�������ļ���������
    //## ������
    //##   const char* entry //��ڶ��� 
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    uint8 writeBlock(const char* entry);
    //##ModelId=435325E001B5
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�����ļ�
    //## ������
    //##   const char* entry //��ڶ���
    //##   const char* key   //�ؼ���
    //##   int32 def         //ȱʡֵ
    //## ���أ�
    //##   if exists return get
    //##   else return def
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    int32 getInt(const char* entry, const char* key, int32 def);
    //##ModelId=4353273F03D8
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�����ļ�
    //## ������
    //##   const char* entry //��ڶ���
    //##   const char* key   //�ؼ���
    //##   FLOAT8 def         //ȱʡֵ
    //## ���أ�
    //##   if exists return get
    //##   else return def
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    FLOAT8 getDouble(const char* entry, const char* key, FLOAT8 def);
    //##ModelId=435327A80232
    //##Documentation
    //## ���ܣ�
    //##   д�������ļ�
    //## ������
    //##   const char* entry //��ڶ���
    //##   const char* key   //�ؼ���
    //##   int32 value       //ֵ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    uint8 writeInt(const char* entry, const char* key, int32 value);
    //##ModelId=4353283403D8
    //##Documentation
    //## ���ܣ�
    //##   д�������ļ�
    //## ������
    //##   const char* entry //��ڶ���
    //##   const char* key   //�ؼ���
    //##   FLOAT8 value       //ֵ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    uint8 writeDouble(const char* entry, const char* key, FLOAT8   value);
	//##ModelId=434E0D8B029B
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�����ļ�·����
    //## ������
    //##   ��
    //## ���أ�
    //##   const QString&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
	const QString& getFilePath() const;
  protected:
    //##ModelId=435D835F01D3
    //##Documentation
    //## ���ܣ�
    //##   ɾ�������ļ���block����
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    void remove();
  protected:
    //##ModelId=43508C58005E
    //##Documentation
    //## ����������	
	GListPFS m_block;
	//##ModelId=43508C58005E
    //##Documentation
    //##�洢�ļ�·��
	QString	 m_filePath;
    //##ModelId=434E0B4B0377
    //##Documentation
    //## �����ļ����
    QFile* m_fFile;
	QDomDocument*	m_pDoc ;
  private:


};

#endif /* GPROFILE_H_HEADER_INCLUDED_BCB8E460 */
