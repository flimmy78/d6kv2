#ifndef GPATHOBJECT_H_HEADER_INCLUDED_BB69AAF5
#define GPATHOBJECT_H_HEADER_INCLUDED_BB69AAF5

#include "gcommobjs.h"
#include "GPolylineObject.h"

class GScaleFactor;

//##ModelId=44960A13000F
//##Documentation
//## ���壺
//##        ·����
//## ���ܣ�
//##        ����·����
//## ��ƣ�
//##        ������
//## ���ڣ�
//##       2006-06-19
class GDE_EXPORT_DES GPathObject : public GPolylineObject
{
  public:
    //##ModelId=44960A7B00AB
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
    GPathObject();
    //##ModelId=44960A7B0119
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     const GPathObject&
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GPathObject(const GPathObject& copy);
	
    //##ModelId=44960A7B01F4
    //##Documentation
    //## ���ܣ�
    //##     ��ֵ����   
    //## ������
    //##     const GPathObject&
    //## ���أ�
    //##     const GPathObject&
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    const GPathObject& operator=(const GPathObject& copy);

    //##ModelId=44960A7B0271
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ���ƹ��ܣ�����ͼԪ��̬ͼ��
    //## ������
    //##   GDC* pDC //���ƻ���
    //##   uint32 nFlags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    virtual void drawFocus(GDC* pDC, uint32 nFlags);

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

	 //##ModelId=4496268103B9
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
    //##ModelId=44962682009C
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
    //##ModelId=449626820109
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
	//##ModelId=4382D138007A
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ�㼯����
    //## ������
    //##      QDomElement& dom
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    uint8 readCodes(QDomElement& dom);
    //##ModelId=449626820177
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
	//##ModelId=4382D138007C
    //##Documentation
    //## ���ܣ�
    //##      ����㼯����
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
	uint8 saveCodes(QDomElement& dom);
	//##ModelId=43A60320004E
    //##Documentation
    //## ���ܣ�
    //##   ����ʱ��Ҫ��������
	//##   ���ڲ��ͼԪ����
    //## ������
    //##   ��
    //## ���أ�
    //##   if ��� return 1
    //##   else return  0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-25
	virtual void  setCodes(const GListUINT8& codes);
	
	GListUINT8	m_codes;
};



#endif /* GPATHOBJECT_H_HEADER_INCLUDED_BB69AAF5 */
