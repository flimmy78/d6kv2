#ifndef GELLIPSEOBJECT_H_HEADER_INCLUDED_BC7D7A6E
#define GELLIPSEOBJECT_H_HEADER_INCLUDED_BC7D7A6E
#include "GBaseObject.h"
class GScaleFactor;
class GFile;

//##ModelId=4382CF100280
//##Documentation
//## ���壺
//##        ͼԪ��Բ��
//## ���ܣ�
//##        ����ͼԪ��Բ��
//## ��ƣ�
//##        ������
//## ���ڣ�
//##       2005-11-22
class GDE_EXPORT_DES GEllipseObject:public GBaseObject
{
  public:
    //##ModelId=4382D1640167
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
    GEllipseObject();

    //##ModelId=4382D1640168
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     const GEllipseObject&
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GEllipseObject(const GEllipseObject& copy);

    //##ModelId=4382D164016A
    //##Documentation
    //## ���ܣ�
    //##     ��ֵ����   
    //## ������
    //##     const GEllipseObject&
    //## ���أ�
    //##     const GEllipseObject&
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    const GEllipseObject& operator=(const GEllipseObject& copy);

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

    //##ModelId=4382D164016F
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ����
    //## ������  
    //##   ��
    //## ���أ�
    //##   uint32
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    virtual void calcRect();

    //##ModelId=4382D1640170
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

    //##ModelId=4382D1640172
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

    //##ModelId=4382D1640174
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

    //##ModelId=4382D1640176
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

    //##ModelId=4382D1640179
    //##Documentation
    //## ���ܣ�
    //##   �ṩ�������ż��㣬�޷��أ�ֱ�Ӹı���ƶ���
    //##   Ĭ�Ϻ��ԣ���Ҫ�ڸú����е���pSF->scaleObject
    //## ������ 
    //##   GScaleFactor *pSF //��������
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    virtual void scaleFactor(GScaleFactor* pSF);

    //##ModelId=4382D164017B
    //##Documentation
    //## ���ܣ�
    //##   ���Ե���ͼԪָ����Χ
    //## ������  
    //##   const GPoint& pt
    //## ���أ�
    //##   uint32
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-12
    virtual uint32 hitTest(const GPoint& pt);

    //##ModelId=4382D164017F
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

    //##ModelId=4382D1640182
    //##Documentation
    //## ���ܣ�
    //##     ��ȡ������ֵ�����ڽ��л�������
    //## ������
    //##     ��
    //## ���أ�
    //##     ���������
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-12
    virtual int32 getOPT(GPoint* pts = NULL);
    //##ModelId=4382D1640186
    //##Documentation
    //## ���ܣ�
    //##     ��ȡ��ת�����㣬���ڽ�����ת����
    //## ������
    //##     GPoint*
    //## ���أ�
    //##     �������������
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-12
    virtual int32 getROPT(GPoint* pts);

    //##ModelId=4382D1640188
    //##Documentation
    //## ���ܣ�
    //##   X,Yƫ����
    //## ������
    //##   FLOAT8 x
    //##   FLOAT8 y
    //##   uint32 flags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-14
    virtual void offsetXY(FLOAT8 x, FLOAT8 y, uint32 flags = 0);

    //##ModelId=4382D164018C
    //##Documentation
    //## ���ܣ�
    //##   X,Y����ƫ����
    //## ������
    //##   const GPoint& pt
    //##   FLOAT8 x
    //##   FLOAT8 y
    //##   uint32 flags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-14
    virtual void resizeXY(const GPoint& pt, FLOAT8 x, FLOAT8 y, uint32 flags = 0);

    //##ModelId=4382D1640199
    //##Documentation
    //## ���ܣ�
    //##   ��ͼԪ���о�����
    //## ������
    //##   uint32 op
    //##   uint32 flags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-15
    virtual void mirrorXY(uint32 op, uint32 flags = 0);

    //##ModelId=4382D164019C
    //##Documentation
    //## ���ܣ�
    //##   ��ָ��ԭ���ͼԪ���о�����
    //## ������
    //##   const GPoint& pto
    //##   uint32 op
    //##   uint32 flags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-15
    virtual void mirrorXY(const GPoint& pto, uint32 op, uint32 flags = 0);

    //##ModelId=4382D16401A0
    //##Documentation
    //## ���ܣ�
    //##   ��ָ����ת���ĵ�ptc��תth��
    //## ������
    //##   const GPoint& ptc
    //##   FLOAT8 th
    //##   uint32 flags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-14
    virtual void rotateXY(const GPoint& ptc, FLOAT8 th, uint32 flags = 0);

    //##ModelId=4382D16401A4
    //##Documentation
    //## ���ܣ�
    //##   ��ͼԪ�ڲ���ת���ĵ���תth��
    //## ������
    //##   FLOAT8 th
    //##   uint32 flags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-14
    virtual void rotateXY(FLOAT8 th, uint32 flags = 0);

    //##ModelId=4382D16401A7
    //##Documentation
    //## ���ܣ�
    //##   �������ĵ�
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    virtual void resetRO();
    //##ModelId=4387FAF30138
    //##Documentation
    //## ���ܣ�
    //##   ͼԪ�������,����ָ�����ܶ���ͼԪ
    //## ������
    //##   uint32 op
    //##   FLOAT8 v
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    virtual void alignPos(uint32 op, FLOAT8 v);
    //##ModelId=4387FAF301D4
    //##Documentation
    //## ���ܣ�
    //##   ͼԪ�ȴ�С����,����ָ�����ܵȴ�СͼԪ
    //## ������
    //##   uint32 op
    //##   FLOAT8 w
    //##   FLOAT8 h
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    virtual void sameSize(uint32 op, FLOAT8 w, FLOAT8 h);
    //##ModelId=43830C850177
    //##Documentation
    //## ���ĵ�
    GPoint m_ptc;
    //##ModelId=43830CA20000
    //##Documentation
    //## ����һ��
    FLOAT8 m_al;
    //##ModelId=43830CC70290
    //##Documentation
    //## ����뾶
    FLOAT8 m_bl;
  protected:
	//##ModelId=438479CF0290
    //##Documentation
    //## ��ʱ���������
    GPoint m_points[8];
  private:

};



#endif /* GELLIPSEOBJECT_H_HEADER_INCLUDED_BC7D7A6E */
