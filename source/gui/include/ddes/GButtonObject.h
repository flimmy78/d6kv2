#ifndef GBUTTONOBJECT_H_HEADER_INCLUDED_BC7D0C57
#define GBUTTONOBJECT_H_HEADER_INCLUDED_BC7D0C57

#include "gcommobjs.h"
#include "GRectObject.h"

class GScaleFactor;
class GFile;

//##ModelId=4382CF8202EE
//##Documentation
//## ���壺
//##        ͼԪ��ť��
//## ���ܣ�
//##        ����ͼԪ��ť
//## ��ƣ�
//##        ������
//## ���ڣ�
//##       2005-11-22
class GDE_EXPORT_DES GButtonObject : public GRectObject
{
  public:
    //##ModelId=4382D17200DA
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
    //##     2005-11-10
    GButtonObject();

    //##ModelId=4382D17200DB
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     const GButtonObject&
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GButtonObject(const GButtonObject& copy);

    //##ModelId=4382D17200DD
    //##Documentation
    //## ���ܣ�
    //##     ��ֵ����   
    //## ������
    //##     const GButtonObject&
    //## ���أ�
    //##     const GButtonObject&
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    const GButtonObject& operator=(const GButtonObject& copy);
    
	//##ModelId=4340A54901C5
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ���ƹ��ܣ�����ͼԪ��̬ͼ��
    //## ������
    //##   GDC* pDC //���ƻ���
    //##   const GFillStyle& fs//�������
	//##   const GLineStyle& ls//�߿�����
	//##   uint8 bFill=1//�Ƿ����
	//##   uint8 bFrame=1//�Ƿ���Ʊ߿�
	//##   GScaleFactor* pSF//��������
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
	virtual void drawNormal(GDC *pDC ,\
		const GFillStyle& fs,\
		const GLineStyle& ls ,\
		uint8 bFill=1, uint8 bFrame=1 ,\
		GScaleFactor *pSF=NULL );

    //##ModelId=4382D17200E3
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ��������ȡ�������ݰ�������
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

    //##ModelId=4382D17200E5
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      GFile& fFile //�ļ�
    //## ���أ���int32 ��
    //##      if success return write real size
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 saveFile(GFile& fFile);

    //##ModelId=4382D17200EA
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

	//##ModelId=4382D17200EA
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ��ʾ�㺯��
    //## ������
    //##      QDomElement& dom
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
	uint8 readLayers(QDomElement& dom);

    //##ModelId=4382D17200EC
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

	//##ModelId=4382D17200EC
    //##Documentation
    //## ���ܣ�
    //##      ������ʾ�㺯��
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
	uint8 saveLayers(QDomElement& dom);
	//##ModelId=43A0F670035B
    //##Documentation
    //## ���ܣ�
    //##   ���������������
    //## ������
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-16
    virtual int32 onLButtonUp(GDC* pDC, const GPoint& pt, uint32 flags);
    //##ModelId=43A0F671004E
    //##Documentation
    //## ���ܣ�
    //##   ������������
    //## ������
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-16
    virtual int32 onLButtonDown(GDC* pDC, const GPoint& pt, uint32 flags);
    //##ModelId=438313AD02DE
    //##Documentation
    //## ��������
    uint32 m_cmdMask;
    //##ModelId=438313F00119
    //##Documentation
    //## ���
    QString m_cmdString;
	//##ModelId=43A21F0B009C
    //##Documentation
    //## ��ť��ǩ
    QString m_lblString;
	//##ModelId=43A21F0B009C
    //##Documentation
	//## ������ʾ��ͼ���б�
	GListINT32S m_layers;
  protected:
    //##ModelId=4383140F0128
    //##Documentation
    //## ��ť���
    uint32 m_style;

};



#endif /* GBUTTONOBJECT_H_HEADER_INCLUDED_BC7D0C57 */
