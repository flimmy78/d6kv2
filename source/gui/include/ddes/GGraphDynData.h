#ifndef GGRAPHDYNDATA_H_HEADER_INCLUDED_BCB8D865
#define GGRAPHDYNDATA_H_HEADER_INCLUDED_BCB8D865

#include <QtXml/QtXml>
#include <QtCore/QString>

#include "ddef/ddes_comm.h"
#include "ddef/GList.h"
#include "GVersion.h"

class GFile;
class GBaseObject;
class GGraphDynData;
//���ض�̬��ص�����
//symbol = gddFactoryBuild
//typedef GGraphDynData* (*GDDFB_PROC)();

//##ModelId=4340F8B101B5
//##Documentation
//## ���壺
//##   ͼԪ���Ӷ�̬����
//## ���ܣ�
//##   ���ڶ���ͼԪ���ӵĶ�̬����
//## ��ƣ�
//##   ������
//## ���ڣ�
//##   2005-10-3
class GDE_EXPORT_DES GGraphDynData
{
  public:
	//##ModelId=43A63FE501C5
    enum  { 
        //##Documentation
        //## ���ݿ�����
        GDDT_DBNAME, 
        //##Documentation
        //## ����
        GDDT_TBNAME, 
        //##Documentation
        //## �ؼ�������
        GDDT_KEYNAME, 
        //##Documentation
        //## �ؼ�����ֵ
        GDDT_KEYWORD, 
        //##Documentation
        //## ��������-GDDT_TBNAME ����
        GDDT_RTABLE, 
        //##Documentation
        //## ������ؼ�����-GDDT_KEYNAME����
        GDDT_RKEYNAME, 
        //##Documentation
        //## ѡȡ������
        GDDT_RSFIELD,
        //##Documentation
        //## �豸����
        GDDT_DEVTYPE,
        //##Documentation
        //## ��ʼ��վ
		GDDT_HSTATION,
		//##Documentation
        //## ������վ
		GDDT_STATION=GDDT_HSTATION,
        //##Documentation
        //## �ն˳�վ
		GDDT_TSTATION,
        //##Documentation
        //## �豸���
		GDDT_DEVID,
		//�߲��ѹ�ȼ�
		GDDT_VOLH,
		//�в��ѹ�ȼ�
		GDDT_VOLM,
		//�ڲ��ѹ�ȼ�
		GDDT_VOLL};

	//##ModelId=4341029B02AF
    //##Documentation
    //## ���ܣ�
    //##     ���캯��
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    GGraphDynData();
	//##ModelId=43A62A7E01F4
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
    //##       2005-9-28
    virtual ~GGraphDynData();

	//##ModelId=43422C970213
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ��¡����
    //## ������
    //##   ��
    //## ���أ�
    //##   GGraphDynData*
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    virtual GGraphDynData* clone();
	 
    //##ModelId=435314BF02AF
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ͼԪ��̬�ߴ磬���ڴ洢ͼԪ
    //## ������
    //##   ��
    //## ���أ�uint32
    //##   if success return realsize
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    uint32 getSize();

	//##ModelId=4340FD560167
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      GFile& fFile
    //## ���أ���int32��
    //##      if success return write real size
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 saveFile(GFile& fFile);

    //##ModelId=4340FD5900AB
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      GFile& fFile
    //## ���أ���int32��
    //##      if success return read real size
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 loadFile(GFile& fFile);
	//##ModelId=43A9108A036B
    //##Documentation
    //## ���ܣ�
    //##      �����������ж�ȡ����
    //## ������
    //##      GGraphDynData *pObj
    //## ���أ���int32��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 loadObj(GGraphDynData* pObj);
    //##ModelId=43A9108A03C8
    //##Documentation
    //## ���ܣ�
    //##      �����������ж�ȡ����
    //## ������
    //##      const GGraphDynData& obj
    //## ���أ���int32��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 loadObj(const GGraphDynData& obj);
	//##ModelId=435DA0AE01E7
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      QDomElement& dom
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    virtual uint8 readXML(QDomElement& dom);
	//##ModelId=435DA0B1034E
    //##Documentation
    //## ���ܣ�
    //##      ���ر��溯��
    //## ������
    //##      QTextStream& stream
    //##      int32 indent
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    virtual uint8 saveXML(QDomElement& dom);
    //##ModelId=43A61C2703D8
    //##Documentation
    //## ���ܣ�
    //##   �Ƿ�ʵʱ����
    //## ������
    //##   ��
    //## ���أ�
    //##   if is return 1
    //##   else return  0 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    virtual uint8 canReal() const;

	 //##ModelId=4354B56202F9
    //##Documentation
    //## ���ܣ�
    //##   �����µ�ͼԪ
    //## ������
    //##   const QString& src//����Դ
    //## ���أ�
    //##   if success return GGraphDynData* 
    //##   else return NULL
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    GGraphDynData* newImpObject(const QString& src);

    //##ModelId=43A61E180148
    //##Documentation
    //## ���ܣ�
    //##   ����ڲ�ʵ��
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-13
    void clearImpObject();
    //##ModelId=43A61E790261
    //##Documentation
    //## ���ܣ�
    //##   �����ڲ�ʵ��
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1 
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-13
    uint8 createImpObject();
	//##ModelId=43A631660119
    //##Documentation
    //## ���ܣ�
    //##   ȡ����������
    //## ������
    //##   ��
    //## ���أ�
    //##   uint32  
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    uint32 getType() const;
	//##ModelId=43A631660119
    //##Documentation
    //## ���ܣ�
    //##   ȡ����������
    //## ������
    //##   ��
    //## ���أ�
    //##   uint32  
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    uint32 getDevID() const;
	//##ModelId=43A631660119
    //##Documentation
    //## ���ܣ�
    //##   ȡ����������
    //## ������
    //##   ��
    //## ���أ�
    //##   uint32  
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    void setDevID(uint32 devID);
	//##ModelId=43A9365C0177
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ͼԪ�����е�ָ������������
    //## ������
    //##   int32 pos
    //## ���أ�
    //##   const char*   
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    virtual const char* getDataName(int32 pos) const;
	//##ModelId=43A63A78032C
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ͼԪ�����еĲ�������
    //## ������
    //##   ��
    //## ���أ�uint32
    //##   if success return realsize
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    virtual int32 getDataCount() const;
    //##ModelId=43A63AC50251
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ͼԪ�����е�ָ����������������
    //## ������
    //##   int32 pos
    //## ���أ�int32     
    //##     
    //##     GDB_FIELD_UINT8 = 0, //�޷����ַ���    
    //##     GDB_FIELD_INT8 = 1, //�з����ַ���
    //##     GDB_FIELD_UINT16 = 2, //�޷��Ŷ�������  
    //##     GDB_FIELD_INT16 = 3, //�з��Ŷ�������
    //##     GDB_FIELD_UINT32 = 4, //�޷���������
    //##     GDB_FIELD_INT32 = 5, //�з���������
    //##     GDB_FIELD_UINT64 = 6, //�޷��ų�������
    //##     GDB_FIELD_INT64 = 7, //�з��ų�������
    //##     GDB_FIELD_FLOAT4 = 8, //�����ȸ�����
    //##     GDB_FIELD_FLOAT8 = 9, //˫���ȸ�����
    //##     GDB_FIELD_STRING = 10, //�ַ�����
    //##     GDB_FIELD_UNKNOWN=-1   //δ֪����
    //##   if success return datatype
    //##   else return -1
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    virtual int32 getDataType(int32 pos) const;
    //##ModelId=43A63D320203
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ͼԪ�����е�ָ������������
    //## ������
    //##   int32 pos
    //## ���أ�
    //##   QString   
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    virtual QString getDataString(int32 pos);
	//##ModelId=443CF13900DA
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ�����е�ָ������������
    //## ������
    //##   int32 pos,const char* strValue
    //## ���أ�
    //##   QString   
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
	virtual void  setDataString(int32 pos,const char* strValue);
    //##ModelId=43A63E2E02AF
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ͼԪ�����е�ָ���ض�����λ��
    //## ������
    //##   GDDT_DBNAME //���ݿ�����
    //##   GDDT_TBNAME //����
    //##   GDDT_KEYNAME//�ؼ�������
    //##   GDDT_KEYWORD//�ؼ�����ֵ
    //##   GDDT_RTABLE //��������-GDDT_TBNAME ����
    //##   GDDT_RKEYNAME//������ؼ�����-GDDT_KEYNAME����
    //##   GDDT_RSFIELD //ѡȡ������
    //##   int32 type
    //## ���أ� int32
    //##   if exist return pos
    //##   else return -1
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    virtual int32 getDataPos(int32 type) const;
	//##ModelId=43A6C9C402CE
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ��������
    //## ������
    //##   ��
    //## ���أ�
    //##   const char*
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-17
    virtual QString getPluginName() const;

	//��ȡ����keyword
	virtual QString getKeyWord(void) const;

	//��ȡ���ݵ����ͣ������豸����������豸���ͣ�������ʵ���ݲ�����������ݵ�P��Q��I
	virtual int32 getKeyType(void) const;

    //##ModelId=4340F917032C
    //##Documentation
    //## �������Դ
    //##  
    QString m_impSrc;
    //##ModelId=43A61CBC02FD
    //##Documentation
    //## ����Դʵ��
    GGraphDynData* m_impObj;
	//##ModelId=43A6272002FD
    //##Documentation
    //## ��������
    uint32 m_nType;

	//##ModelId=43A6272002FD
    //##Documentation
    //## ���ݰ汾��
	GVersion m_vers;
};

//##ModelId=4340FB28009C
class GDE_EXPORT_DES GGraphDynDataList:public GList<GGraphDynData*>{};

#endif /* GGRAPHDYNDATA_H_HEADER_INCLUDED_BCB8D865 */
