#ifndef GCUSTOMOBJECT_H_HEADER_INCLUDED_BC5DBFB4
#define GCUSTOMOBJECT_H_HEADER_INCLUDED_BC5DBFB4
#include "GComObject.h"

class GScaleFactor;
class GFile;
class GBaseObject;

//##ModelId=43A2103A0109
//##Documentation
//## ���壺
//##        �Զ���ͼԪ��
//## ���ܣ�
//##        ����ͼԪ��������ͼԪ
//##        ����ΪGD_TYPE_CUSTOM
//##        �У����ͼԪ�����
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-12-16
class GDE_EXPORT_DES GCustomObject : public GComObject
{
  public:
    //##ModelId=43A211790261
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
    GCustomObject();

    //##ModelId=43A211790271
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
    virtual ~GCustomObject();

    //##ModelId=43A211790272
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

    //##ModelId=43A211790274
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

	//##ModelId=43A0C35201B5
    //##Documentation
    //## ���ܣ�
    //##   �����ڲ�ʵʱֵ
    //## ������
    //##   FLOAT8 v
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-15
    virtual void setValue(FLOAT8 value);

    //##ModelId=43A211790280
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ���ƹ��ܣ�����ͼԪ��̬ͼ��
    //## ������
    //##   GDC* pDC //���ƻ���
    //##   GScaleFactor* pSF//��������
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    virtual uint8 drawObj(GDC* pDC, GScaleFactor* pSF = NULL);
	
	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   ������Ϊ
    //## ������
    //##   LPDA dynAct
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	virtual void setAct(LPDA dynAct);
	virtual void addAct( LPDA dynAct );
	virtual void rmvAct( LPDA dynAct );
	virtual void clrAct();

    //##ModelId=43A211790283
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

    //##ModelId=43A211790284
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

    //##ModelId=43A211790290
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

    //##ModelId=43A2117902CE
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

    //##ModelId=43A2117902D2
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

    //##ModelId=43A2117902D7
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

    //##ModelId=43A2117902DE
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

    //##ModelId=43A2117902E2
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

    //##ModelId=43A2117902EE
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

    //##ModelId=43A2117902F2
    //##Documentation
    //## ���ܣ�
    //##   ���������
    //## ������
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    virtual int32 onMouseEnter(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A2117902FD
    //##Documentation
    //## ���ܣ�
    //##   ����ڶ������ƶ�����
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
    virtual int32 onMouseMove(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A21179030D
    //##Documentation
    //## ���ܣ�
    //##   ����뿪����
    //## ������
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    virtual int32 onMouseLeave(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A211790311
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

    //##ModelId=43A21179031C
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

    //##ModelId=43A211790320
    //##Documentation
    //## ���ܣ�
    //##   ������Ҽ��������
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
    virtual int32 onRButtonUp(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A21179032C
    //##Documentation
    //## ���ܣ�
    //##   ������Ҽ�����
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
    virtual int32 onRButtonDown(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A21179033C
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
	//##ModelId=43A6064003B9
    //##Documentation
    //## ���ܣ�
    //##   �������ö�������ID
    //## ������
    //##   reset 
	//##	 if =0 ����OwnerID
	//##	 else  ���OwnerID
	//##   nID  
	//##	 if =-1 ��ǰIDΪ�Ӷ��ӵ�����
	//##	 else	ָ��IDΪ����
	//##   pObj
	//##	 if =NULL ��ǰ����Ϊ�Ӷ��ӵ�����
	//##	 else	  ָ������Ϊ����	
    //## ���أ�
    //##   if is return 1
    //##   else return  0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-25
	virtual void setPortOwner(uint8 reset=0,uint32 nID=(uint32)-1,\
		GBaseObject* pObj=NULL);

    //##ModelId=43A21179033E
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

    //##ModelId=43A21179034B
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
    //##ModelId=43A2122E008C
    //##Documentation
    //## ���ܣ�
    //##      ��ȡָ��״̬ͼԪ���
    //## ������
    //##      FLOAT8 vstatus
    //## ���أ�
    //##      GBaseObject*
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-16
    GBaseObject* findObj(FLOAT8 vstatus = 0) const;
	    //##ModelId=43D314070196
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ��������
    //## ������  
    //##   const GFillStyle& //�����
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    virtual void setFillStyle(const GFillStyle& style);
    //##ModelId=43D31407037A
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ�߷��
    //## ������  
    //##   const GLineStyle& style //�߷��
    //## ���أ�
    //##   void
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    virtual void setLineStyle(const GLineStyle& style);
    //##ModelId=43D31408005D
    //##Documentation
    //## ���ܣ�
    //##      ����ͼԪ�ı������Ϣ
    //## ������  
    //##     const GTextStyle&
    //## ���أ�
    //##    ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    virtual void setTextStyle(const GTextStyle& style);
    //##ModelId=43D431F6035B
    //##Documentation
    //## ���ܣ�
    //##   �ж��Ƿ�����ճ��
    //## ������
    //##   ��
    //## ���أ�
    //##   if is return 1
    //##   else return  0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-25
    virtual uint8 canSnap() const;
    //##ModelId=43D431F7001F
    //##Documentation
    //## ���ܣ�
    //##   �ж�ָ�����Ƿ�λ��ճ����Χ
    //## ������  
    //##   const GPoint& pt
    //## ���أ�
    //##   uint8
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    virtual uint8 hitSnap(const GPoint& pt);    

	//##ModelId=43A211A4033C
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ����ͼԪ����
    //## ������
    //##   libPath ����
	//##   libID   ��ʶ
    //## ���أ�
    //##   if success return 1
	//##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-17
	uint8 loadElems(const char* libPath,uint32 libID);
};



#endif /* GCUSTOMOBJECT_H_HEADER_INCLUDED_BC5DBFB4 */
