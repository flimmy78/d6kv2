#ifndef GGRAPHPLUGIN_H_HEADER_INCLUDED_BC7D63CB
#define GGRAPHPLUGIN_H_HEADER_INCLUDED_BC7D63CB
#include "GBaseObject.h"
#include "GRectObject.h"

const static int GD_DEVTYPE_HMI = 101;
const static int GD_DEVTYPE_PAINTER = 102;

class GScaleFactor;
class GFile;

//���ض�̬��ص�����
//symbol = gdoFactoryBuild
//
//typedef GBaseObject* (*GDOFB_PROC)();
//##ModelId=4382CFEF0128
//##Documentation
//## ���壺
//##        ͼԪ�����
//## ���ܣ�
//##        ����ͼԪ�����
//## ��ƣ�
//##        ������
//## ���ڣ�
//##       2005-11-22
class GDE_EXPORT_DES GGraphPlugin : public GRectObject
{
  public:
    //##ModelId=4382D15A037A
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
    GGraphPlugin();

    //##ModelId=4382D15A038A
    //##Documentation
    //## ���ܣ�
    //##     ���캯��   
    //## ������
    //##     const GGraphPlugin&
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    GGraphPlugin(const GGraphPlugin& copy);

	//##ModelId=439EABD202DE
    //##Documentation
    //## ���ܣ�
    //##     ��������   
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-12-13
    virtual ~GGraphPlugin();

	//##ModelId=439EA9E701B5
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
    //##ModelId=439EA9ED0177
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
    //##ModelId=439EAA2A035B
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
    //##ModelId=439EAA3703A9
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪID����
    //## ������  
    //##   uint32 uID //ָ��ͼԪID
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    virtual void setID(uint32 uID);
	//##ModelId=43A0C3E1002E
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
	//##ModelId=43A607EC036B
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
	//##ModelId=43A60320004E
    //##Documentation
    //## ���ܣ�
    //##   ����ʱ��Ҫ�������
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
    virtual uint8 needPoints(int32 mode=0) const;
	//##ModelId=43A60320004E
    //##Documentation
    //## ���ܣ�
    //##   ����ʱ��Ҫ�������
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
	virtual void  setPoints(const GListPOINTS& pts);

    //##ModelId=4382D15A038C
    //##Documentation
    //## ���ܣ�
    //##     ��ֵ����   
    //## ������
    //##     const GGraphPlugin&
    //## ���أ�
    //##     const GGraphPlugin&
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-10
    const GGraphPlugin& operator=(const GGraphPlugin& copy);

    //##ModelId=4382D15A038E
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

    //##ModelId=4382D15A0391
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

    //##ModelId=4382D15A0392
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

    //##ModelId=4382D15A0394
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

    //##ModelId=4382D15A0396
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

    //##ModelId=4382D15A0398
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

    //##ModelId=4382D15A039B
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

    //##ModelId=4382D15A039D
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

    //##ModelId=4382D15A039F
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ�Ƿ���ָ����Χ
    //## ������  
    //##   const GRect& rc
    //## ���أ�
    //##   uint32
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-12
    virtual uint32 hitTest(const GRect& rc);

    //##ModelId=4382D15A03A1
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

    //##ModelId=4382D15A03A4
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

    //##ModelId=4382D15A03A6
    //##Documentation
    //## ���ܣ�
    //##     ��ȡ��ת���ĵ㣬���ڽ�����ת����
    //## ������
    //##     ��
    //## ���أ�
    //##     ��ת���ĵ�
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-12
    virtual GPoint getRO();

    //##ModelId=4382D15A03A9
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

    //##ModelId=4382D15A03AB
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

    //##ModelId=4382D15A03AF
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

    //##ModelId=4382D15A03B4
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

    //##ModelId=4382D15A03B9
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

    //##ModelId=4382D15A03BD
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

    //##ModelId=4382D15A03C1
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

    //##ModelId=4382D15A03C4
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
    //##ModelId=4387FB020138
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
    //##ModelId=4387FB0201E4
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
    //##ModelId=439E6ACA0110
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
    //##ModelId=439E6ACA01AD
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
    //##ModelId=439E6ACA022A
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
    //##ModelId=439E6ACA02A7
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
    //##ModelId=439E6ACA0333
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
    //##ModelId=439E6ACA03B0
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
    //##ModelId=439E6ACB0045
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
    //##ModelId=439E6ACB00D2
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�ؼ���ʾ��Ϣ
    //## ������
    //##   QString& tip
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    virtual void getToolTip(QString& tip);

	//##ModelId=439EAA6800EA
    //##Documentation
    //## ���ܣ�
    //##   ����ʵ��ͼԪ
    //## ������
    //##   const QString& pluginsrc
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-13
	GBaseObject* newImpObject(const QString& pluginsrc);

	//##ModelId=439EAAE502FD
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

	//##ModelId=43A1744B0000
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
	//##ModelId=439EA9C40271
    //##Documentation
    //## ���ܣ�
    //##    ��������ڲ�����,�ڲ�ʹ��
    //## ������
    //##     GFile& fFile
    //##     uint8  bload
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-12-13
	virtual void transPluginData(GFile& fFile,uint8 bload=1);
	//##ModelId=439FC6B80180
    //##Documentation
    //## ���ܣ�
    //##    �������λ����Ϣ
	//##    �ļ���ʽ
    //##       GPoint  ptc���ĵ�
    //##       FLOAT8  width ���ο��һ��
    //##       FLOAT8  height���θߵ�һ��
    //## ������
    //##     GFile& fFile
    //##     uint8  bload
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-12-14
    virtual void transPluginPos(GFile& fFile, uint8 bload = 1);
	//##ModelId=43A0C20C0177
    //## ���ܣ�
    //##    �������ֵ��Ϣ 	
    //## ������
	//##	int npos
	//##	FLOAT8* pvalue
	//##	int vcnt
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-12-14
    virtual void transPluginValue(int npos,FLOAT8* pvalue,int vcnt=1);
	//##ModelId=43A0C0A70290
    //##Documentation
    //## ���ܣ�
    //##    ����Ƿ���Ҫ����ֵ��Ϣ 	
    //## ������
	//##	 ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-12-14
	virtual uint8 needPluginTransValue() const;
	//##ModelId=439FA9900013
    //##Documentation
    //## ���ܣ�
    //##     ͼԪ��������
    //## ������
    //##     QWidget *parent
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-12-14
    virtual uint8 showProperty(QWidget *parent);
	//##ModelId=43A0DA9C035B
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
	//##ModelId=43A0DA9C035B
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�ؼ�λͼ
    //## ������
    //##   ��
    //## ���أ�
    //##   λͼ����
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-17
	virtual QPixmap    getPixmap() const;
	//##ModelId=440522DF0224
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ��ʱ��Ӧ����
    //## ������
    //##   uint32 ymd //������=yyyymmdd
    //##   uint32 hmsm//ʱ�������=hhmmssmmm
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2006-2-27
    virtual uint8 onTimer(uint32 ymd, uint32 hmsm);
    //##ModelId=440522E00001
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

	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�豸����
    //## ������
    //##   ��
    //## ���أ�
    //##   uint32 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	virtual uint32 getDevType() const ;
	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   �����豸����
    //## ������
    //##   uint32 devType
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	virtual void setDevType(uint32 devType);
	
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

	//##ModelId=43A603AC0280
	//##Documentation
	//## ���ܣ�
	//##   ��ȡ������
	//## ������
	//##   ��
	//## ���أ�
	//##   QWidget*
	//## ��ƣ�
	//##   lbh
	//## ���ڣ�
	//##   2016-07-28
	virtual QWidget* getParentWidget() const;

	//##ModelId=43A603AC0280
	//##Documentation
	//## ���ܣ�
	//##   ���ø�����
	//## ������
	//##   QWidget*
	//## ���أ�
	//##   ��
	//## ��ƣ�
	//##   lbh
	//## ���ڣ�
	//##   2016-07-28
	virtual void setParentWidget(QWidget* widget);

	
    //##ModelId=4383130A0177
    //##Documentation
    //## ���Դ����ӿ�
    QString m_impSrc;
    //##ModelId=43A625C30203
    //##Documentation
    //## �ڲ�ͼԪʵ��
    GBaseObject* m_impObj;
};



#endif /* GGRAPHPLUGIN_H_HEADER_INCLUDED_BC7D63CB */
