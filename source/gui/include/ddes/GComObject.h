#ifndef GCOMOBJECT_H_HEADER_INCLUDED_BCB20227
#define GCOMOBJECT_H_HEADER_INCLUDED_BCB20227
#include "GBaseObject.h"
#include "gcommobjs.h"

class GScaleFactor;
class GFile;

//##ModelId=4340FF8A008C
//##Documentation
//## ���壺
//##        ͼԪ�����
//## ���ܣ�
//##        ����ͼԪ��������ͼԪ����ΪGD_TYPE_COMOBJ
//##        �л���ͼԪ����Ƕ�����ͼԪ�����
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
class GDE_EXPORT_DES GComObject : public GBaseObject
{
  public:
    //##ModelId=4341043D0157
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
    GComObject();

    //##ModelId=4341045101E4
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
    virtual ~GComObject();
	//##ModelId=437C8C9A0203
    //##Documentation
    //## ���ܣ�
    //##   ���õ�ǰ���ѡ�ж���
    //## ������
    //##   GBaseObject* obj
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    void setObj(GBaseObject* obj);
    //##ModelId=437C8CA50399
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ��ǰ���ѡ�ж���
    //## ������
    //##   ��
    //## ���أ�
    //##   GBaseObject* 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    GBaseObject* getObj();
	 //##ModelId=43799642008D
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ��ǰ�������
    //## ������
    //##      ��
    //## ���أ�
    //##      uint32
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    uint32 count() const;

    //##ModelId=4341047200BB
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

    //##ModelId=4341047C007D
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

	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   �ж��Ƿ�����ճ��
    //## ������
    //##   ��
    //## ���أ�
    //##   if is return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	virtual uint8 canSnap() const ;

	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   �����豸״̬
    //## ������
    //##   uint32 devStatus
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	virtual void setDevStatus(uint32 devStatus) ;

	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   �ж�ָ�����Ƿ�λ��ճ����Χ
    //## ������
    //##   const GPoint& pt
    //## ���أ�
    //##   if is return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	virtual uint8 hitSnap(const GPoint& pt);
   
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

    //##ModelId=4344FC2102BF
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
    //##ModelId=4354783E032C
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
    virtual uint8 delObject(GBaseObject* pObject);


    //##ModelId=43547843008C
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
    virtual GPOS addObject(GBaseObject* pObject);

    //##ModelId=435478490196
    //##ModelId=435478490196
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
    virtual uint8 upObject(GBaseObject* pObject);

    //##Documentation
    //##ModelId=435478500290
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
    virtual uint8 downObject(GBaseObject* pObject);

    //## ͼԪ����
    //##ModelId=435478640128
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
    virtual uint8 chgObject(GBaseObject* pObject1, GBaseObject* pObject2);
	//##ModelId=43788ECA002E
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
    virtual uint8 topObject(GBaseObject* pObject);


    //##ModelId=43788ECD032C
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
    virtual uint8 bottomObject(GBaseObject* pObject);

	//##ModelId=4381550900BB
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
    virtual GBaseObject* firstObject() const;

    //##ModelId=43788F02031C
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
    virtual GBaseObject* prevObject(GBaseObject* pObject) const;


    //##ModelId=43788F0A0203
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
    virtual GBaseObject* nextObject(GBaseObject* pObject) const;
    
    //##ModelId=43547871038A
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
    virtual uint8 remove();

    
    //##ModelId=4354787703D8
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
    virtual uint8 removeAt(GBaseObject* pObject);

	//##ModelId=4354787703D8
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
    virtual uint8 removePos(GPOS gPos);

    
    //##ModelId=435478C803B9
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
    virtual GBaseObject* getPos(GPOS gPos) const;
	//##ModelId=435DA0CA0041
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


    //##ModelId=435DA0D001D7
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
    
    //##ModelId=435478CF033C
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
    virtual GBaseObject* getPos(int32 nIndex) const;	   
	//##ModelId=43780174011C
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


    //##ModelId=437801760060
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
	//##ModelId=437938A203A9
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
    //##ModelId=43793E1F032C
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
    //##ModelId=43793E45030D
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


    //##ModelId=43793E48008C
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
	//##ModelId=4379BAAB0128
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
    //##ModelId=4379BAAB01D4
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
    //##ModelId=4379BAAB029F
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
    //##ModelId=4379BAAB034B
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
    //##ModelId=4379BACE0251
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
	//##ModelId=43771D470157
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


    //##ModelId=43771D49032C
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
    //##ModelId=437B2814000F
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
    //##ModelId=437B281400CB
    //##Documentation
    //## ���ܣ�
    //##   ��������̸�����
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
    //##ModelId=437B28140177
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
    //##ModelId=437B28140251
    //##Documentation
    //## ���ܣ�
    //##   ������Ҽ�̸�����
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
    //##ModelId=437B2814032C
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
	 //##ModelId=437F17630203
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
     //##ModelId=4387FAE80128
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
    //##ModelId=4387FAE801A5
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
	//##ModelId=43A6064003B9
    //##Documentation
    //## ���ܣ�
    //##   ���Ըö����Ƿ������ת
    //## ������
    //##   ��
    //## ���أ�
    //##   if is return 1
    //##   else return  0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-25
    virtual uint8 canRotate() const;
	    //##ModelId=43D313DD034B
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
    //##ModelId=43D313DE009C
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
    //##ModelId=43D313DE0186
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
	//##ModelId=440522BA013A
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ��ʱ��Ӧ����
    //## ������
    //##   uint32 ymd//������=yyyymmdd
    //##   uint32 hms//ʱ����=hhmmss
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2006-2-27
    virtual uint8 onTimer(uint32 ymd, uint32 hmsm);
    //##ModelId=440522BA01F5
    //##Documentation
    //## ���ܣ�
    //##   ͼԪ�Ƿ�����ʱ��Ӧ����
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1
    //##   return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2006-2-27
    virtual uint8 canTimer() const;
	//##ModelId=43A211A4033C
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�ؼ�����
    //## ������
    //##   ��
    //## ���أ�
    //##   const char*
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-17
    virtual QString getPluginName() const;
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

	//##ModelId=4344FAC702DE
    //##Documentation
    //## ���ܣ�
    //##  ����ͼԪ��ǰ��
    //## ������  
    //##   GGraphLayer *pLayer//��ǰ��
    //## ���أ�
    //##   void
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
	virtual void setLayer(GGraphLayer* pLayer);
	
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
	//##ModelId=437984020383
    //##Documentation
    //## ���ܣ�
    //##   �����Զ�ɾ�����еĶ���
    //## ������
    //##   uint8 bAutoDelete
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-14
    void setAutoDelete(uint8 bAutoDelete);
	//##ModelId=434DCD9402F0
    //##Documentation
    //## ���ܣ�
    //##   ͼԪ�Ƿ񼤻�
    //## ������
    //##   uint8 active
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    virtual void setActive(int32 active );
    //##ModelId=4379844B01CD
    //##Documentation
    //## ���ܣ�
    //##  �ж��Ƿ��Զ�ɾ�����еĶ���
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1 
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-14
    uint8 getAutoDelete() const;
	//##ModelId=43A210A70280
    //##Documentation
    //## ���Զ���ͼԪ���ƣ�����Ϊ����ʾ
    //## ͨ��getPluginName����
    QString m_customName;
	//##ModelId=43410072035B
    //##Documentation
    //## Ƕ�������
	uint8	m_bLink;
	//##ModelId=43410072035B
    //##Documentation
    //## ��·��
	QString	m_libPath;
	//##ModelId=43410072035B
    //##Documentation
    //## ���ʶ
	uint32 m_libID  ;
    //##ModelId=43410072035B
    //##Documentation
    //## ͼԪ����
    GListOBJECTS m_objList;
protected:
    //##ModelId=4379AF4F0138
    //##Documentation
    //## ��ǰ��������
    GBaseObject* m_curObj;
	
	//##ModelId=437983C40316
	//##Documentation
    //## ��ǰ��������
    uint8 m_bAutoDelete;

};


#endif /* GCOMOBJECT_H_HEADER_INCLUDED_BCB20227 */
