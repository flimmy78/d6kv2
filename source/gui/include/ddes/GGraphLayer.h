#ifndef GGRAPHLAYER_H_HEADER_INCLUDED_BCB8F875
#define GGRAPHLAYER_H_HEADER_INCLUDED_BCB8F875

#include "ddef/ddes_comm.h"
#include "ddef/GList.h"
#include "ddef/GRect.h"
#include "GVersion.h"
#include "GRedoundo.h"

class GGraphFile;
class GFile;
class GBaseObject;
class GScaleFactor;

//##ModelId=433D36320000
//##Documentation
//## ���壺
//##        ͼ�δ洢�ṹ
//## ���ܣ�
//##        ͼ�β㶨��
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
class GDE_EXPORT_DES GGraphLayer
{
  public:
    //##ModelId=433FE3C40138
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

    //##ModelId=433FE4310261
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

    //##ModelId=4341037302CE
    //##Documentation
    //## ���ܣ�
    //##       ����/��������    
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-3
    GGraphLayer();

    //##ModelId=4341038E030D
    //##Documentation
    //## ���ܣ�
    //##       ����/��������    
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-3
    virtual ~GGraphLayer();

    //##ModelId=4347199B001F
    //##Documentation
    //## ���ܣ�
    //##      ���������ĵ�
    //## ������
    //##      GGraphFile* pFile//ָ�������ļ�
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-8
    void setFile(GGraphFile* pFile);

    //##ModelId=434719EA0000
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ�����ĵ�
    //## ������
    //##      ��
    //## ���أ�
    //##      GGraphFile*
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-8
    GGraphFile* getFile();

    //##ModelId=43471A0A0000
    //##Documentation
    //## ���ܣ�
    //##      ɾ��ָ��ͼԪ������ͼԪ����
    //## ������
    //##      GBaseObject* pObject//ָ��ͼԪ
    //## ���أ�(uint8)
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-8
    uint8 delObject(GBaseObject* pObject);

    //##ModelId=43471A8D038A
    //##Documentation
    //## ���ܣ�
    //##      ���ָ��ͼԪ
    //## ������
    //##      GBaseObject* pObject//ָ��ͼԪ
    //## ���أ���GPOS��
    //##      if success return GPOS
    //##      else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-8
    GPOS addObject(GBaseObject* pObject);

    //##ModelId=43471ACB006D
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪλ��
    //## ������
    //##   GBaseObject* pObject//ָ��ͼԪ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 upObject(GBaseObject* pObject);

    //##ModelId=43471AD602EE
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪλ��
    //## ������
    //##   GBaseObject* pObject//ָ��ͼԪ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 downObject(GBaseObject* pObject);

    //##ModelId=43471ADB0213
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪλ��
    //## ������
    //##   GBaseObject* pObject1//ָ��ͼԪ
    //##   GBaseObject* pObject2//ָ��ͼԪ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 chgObject(GBaseObject* pObject1, GBaseObject* pObject2);

    //##ModelId=43471D09031C
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ����ָ��λ��
    //## ������  
    //##   GPOS gPos//ָ��ͼԪ����λ��
    //## ���أ�
    //##   GBaseObject* 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    GBaseObject* getPos(GPOS gPos);

    //##ModelId=43471D0D007D
    //##Documentation
    //## ���ܣ�
    //##   ȡͼԪ��Ӧ����λ��
    //## ������
    //##   int32 nIndex//ͼԪ
    //## ���أ�
    //##   GBaseObject*
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
     GBaseObject* getPos(int32 nIndex);

    //##ModelId=43471D2001A5
    //##Documentation
    //## ���ܣ�
    //##   ɾ������ͼԪ������ͼԪ����
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 remove();

	//##ModelId=43471D2001A5
    //##Documentation
    //## ���ܣ�
    //##   ɾ��ɾ�����ͼԪ������ͼԪ����
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
	uint8 removeDels();

    //##ModelId=434720B20213
    //##Documentation
    //## ���ܣ�
    //##   ɾ��ָ��ͼԪ��������ͼԪ����
    //## ������
    //##   GBaseObject* pObject
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 removeAt(GBaseObject* pObject);
    //##ModelId=434DE5650123
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
    //##ModelId=434DF8CF010C
    //##Documentation
    //## ���ܣ�
    //##   ȡͼԪ��Ӧ����λ��
    //## ������
    //##   ��
    //## ���أ�
    //##   GPOS
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    GPOS getPos();


    //##ModelId=434DF8D50060
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ����ָ��λ��
    //## ������
    //##   GPOS gPos//ָ��ͼԪ����λ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    void setPos(GPOS gPos);
    //##ModelId=435DA06401B8
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


    //##ModelId=435DA068008F
    //##Documentation
    //## ���ܣ�
    //##      ���ر��溯��
    //## ������
    //##      QDomElement&  dom
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    virtual uint8 saveXML(QDomElement& dom);
    //##ModelId=436C6D3902FD
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ���ƹ��ܣ�����ͼԪ��̬ͼ��
    //## ������
    //##   GDC* pDC         //���ƻ���
    //##   GRect* pRect     //��������
    //##   GScaleFactor* pSF//��������
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    void onDraw(GDC* pDC, const GRect& rc, GScaleFactor* pSF = NULL);
	//##ModelId=4372E0FF031C
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪλ�õ���
    //## ������
    //##   GBaseObject* pObject//ָ��ͼԪ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 topObject(GBaseObject* pObject);
    //##ModelId=4372E10701F4
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪλ�õ�β
    //## ������
    //##   GBaseObject* pObject//ָ��ͼԪ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    uint8 bottomObject(GBaseObject* pObject);
	 //##ModelId=43815690001F
    //##Documentation
    //## ���ܣ�
    //##   ȡ��һ��ͼԪ
    //## ������
    //##   ��
    //## ���أ�
    //##   GBaseObject*
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    GBaseObject* firstObject() const;
    //##ModelId=4372E1620271
    //##Documentation
    //## ���ܣ�
    //##   ȡָ��ͼԪλ��ǰһͼԪ
    //## ������
    //##   GBaseObject* pObject//ָ��ͼԪ
    //## ���أ�
    //##   if success return Object
    //##   else return NULL
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    GBaseObject* prevObject(GBaseObject* pObject) const;
    //##ModelId=4372E16900DA
    //##Documentation
    //## ���ܣ�
    //##   ȡָ��ͼԪλ�ú�һͼԪ
    //## ������
    //##   GBaseObject* pObject//ָ��ͼԪ
    //## ���أ�
    //##   if success return Object
    //##   else return NULL
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-8
    GBaseObject* nextObject(GBaseObject* pObject) const;
	    //##ModelId=437BD09E009C
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ��һ��λ��
    //## ������
    //##      ��
    //## ���أ�
    //##      GPOS
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    virtual GPOS firstPos() const;
    //##ModelId=437BD09E0128
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ��ǰλ�ö���ǰ������һ������
    //## ������
    //##      GPOS&
    //## ���أ�
    //##      GBaseObject*
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    virtual GBaseObject* nextPos(GPOS& gPos) const;
    //##ModelId=437BD09E0186
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ���һ��λ��
    //## ������
    //##      ��
    //## ���أ�
    //##      GPOS
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    virtual GPOS lastPos() const;
    //##ModelId=437BD09E01E4
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ��ǰλ�ö���ǰ������һ������
    //## ������
    //##      GPOS&
    //## ���أ�
    //##      GBaseObject*
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    virtual GBaseObject* prevPos(GPOS& gPos) const;
    //##ModelId=437BD09E0232
    //##Documentation
    //## ���ܣ�
    //##      ��ȡָ������λ��
    //## ������
    //##      GBaseObject* 
    //## ���أ�
    //##      GPOS
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    virtual GPOS findPos(GBaseObject* obj) const;
    //##ModelId=437BD0A501B5
    //##Documentation
    //## ���ܣ�
    //##      ��ȡָ������ID��Ӧλ��
    //## ������
    //##      uint nID
    //## ���أ�
    //##      GPOS
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    GPOS findPos(uint32 nID) const;
	//##ModelId=437BD0A501B5
    //##Documentation
    //## ���ܣ�
    //##      ��ȡָ��ID��Ӧ����
    //## ������
    //##      uint nID
    //## ���أ�
    //##      GBaseObject*
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    GBaseObject* findObj(uint32 nID) const;

	//##ModelId=437BD0A501B5
    //##Documentation
    //## ���ܣ�
    //##     ��ʷ������¼
    //## ������
    //##     ��
    //## ���أ�
    //##     GCommandHistory&
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
	//
	GCommandHistory& history();
	//##ModelId=438017F60167
    //##Documentation
    //## ���ܣ�
    //##   �Ƿ�Ϊɾ��״̬
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    uint8 isDel() const;
    //##ModelId=438017F601F4
    //##Documentation
    //## ���ܣ�
    //##   ����ɾ��״̬
    //## ������
    //##   uint8 bDel
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    void setDel(uint8 bDel = 1);

	//##ModelId=438017F60167
    //##Documentation
    //## ���ܣ�
    //##   �Ƿ�Ϊ��ʾ״̬
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    uint8 isVis() const;
    //##ModelId=438017F601F4
    //##Documentation
    //## ���ܣ�
    //##   ������ʾ״̬
    //## ������
    //##   uint8 bDel
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    void setVis(uint8 bVis=1);

    //##ModelId=433D369B005D
    //##Documentation
    //## ������
    uint32 m_lyrType;

    //##ModelId=433D36C0036B
    //##Documentation
    //## ������
    QString m_lyrName;

    //##ModelId=433D36E801C5
    //##Documentation
    //## �㷶Χ
    GRect m_lyrRect;

    //##ModelId=433FC5FC0251
    //##Documentation
    //## ͼԪ�б�
    GListOBJECTS m_lstElems;
    //##ModelId=43531B750213
    //##Documentation
    //## �汾��
    GVersion m_vers;
	//##ModelId=4381297B0271
    //##Documentation
    //## ɾ��ͼԪ���������洢
    uint32 m_cntDel;
	
  protected:
    //##ModelId=4347197B0128
    //##Documentation
    GGraphFile* m_graphFile;

    //##ModelId=43471CC60000
    //##Documentation
    //## ��ǰ��Ӧ������ָ��
    GPOS m_curPos;
	//##ModelId=43471CC60000
    //##Documentation
	//�ò���������ʷ������¼
	GCommandHistory m_cmdHistory ;

	//##ModelId=438017DD038A
    //##Documentation
    //## ��ͼ���Ƿ�Ϊɾ��״̬�����Ϊɾ��״̬
    //## �洢ʱ�����д洢������ʱ�����л���
    uint8 m_del;

	//##ModelId=438017DD038A
    //##Documentation
    //## ��ͼ���Ƿ�Ϊ��ʾ״̬����������ʱ����ͼ���л�
    //## ���Ϊ��ʾ״̬������ʱ���л��ƣ����򲻻���
    uint8 m_vis;

  private:
};


#endif /* GGRAPHLAYER_H_HEADER_INCLUDED_BCB8F875 */
