#ifndef GROUNDRECTOBJECT_H_HEADER_INCLUDED_BC7A2345
#define GROUNDRECTOBJECT_H_HEADER_INCLUDED_BC7A2345
#include "GRectObject.h"
class GScaleFactor;
class GFile;

//##ModelId=4385B4190177
//##Documentation
//## ���壺
//##        ͼԪԲ�Ǿ�����
//## ���ܣ�
//##        ����ͼԪԲ�Ǿ�����
//## ��ƣ�
//##        ������
//## ���ڣ�
//##       2005-11-22
class GDE_EXPORT_DES GRoundRectObject : public GRectObject
{
  public:
    //##ModelId=4385B4EB0232
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
    GRoundRectObject();

    //##ModelId=4385B4EB0242
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     const GRoundRectObject&
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GRoundRectObject(const GRoundRectObject& copy);

    //##ModelId=4385B4EB0244
    //##Documentation
    //## ���ܣ�
    //##     ��ֵ����   
    //## ������
    //##     const GRoundRectObject&
    //## ���أ�
    //##     const GRoundRectObject&
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    const GRoundRectObject& operator=(const GRoundRectObject& copy);

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

    //##ModelId=4385B4EB0264
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

    //##ModelId=4385B4EB0271
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

    //##ModelId=4385B4EB0280
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

    //##ModelId=4385B4EB0290
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

    //##ModelId=4385B4EB0292
    //##Documentation
    //## ���ܣ�
    //##      ���ر��溯��
    //## ������
    //##      QDomElement& dom
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-25
    virtual uint8 saveXML(QDomElement& dom);

    //##ModelId=4385B4EB029F
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

    //##ModelId=4385B4EB02AF
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

    //##ModelId=4385B4EB02CF
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

    //##ModelId=4385B4EB02DE
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

    //##ModelId=4385B4EB034B
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

    //##ModelId=4385B4EB035B
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

    //##ModelId=4385B4EB036B
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

    //##ModelId=4385B4EB037A
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

    //##ModelId=4385B4EB03A9
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

    //##ModelId=4385B4EB03C8
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

    //##ModelId=4385B4EC0000
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

    //##ModelId=4385B4EC001F
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
    //##ModelId=4387FB2A030D
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
    //##ModelId=4387FB2A03A9
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

	FLOAT8	m_rndX ;
	FLOAT8	m_rndY ;
	GPoint  m_rndPX ;
	GPoint  m_rndPY ;

};



#endif /* GROUNDRECTOBJECT_H_HEADER_INCLUDED_BC7A2345 */
