#ifndef GBASEOBJECT_H_HEADER_INCLUDED_BCB8A08D
#define GBASEOBJECT_H_HEADER_INCLUDED_BCB8A08D
#include <QtGui/QPixmap>
#include <QtCore/QString>
#include <QtCore/QMutex>

#include "ddef/ddes_comm.h"
#include "ddef/GRect.h"
#include "GDC.h"
#include "GGraphDynData.h"
#include "GVersion.h"
#include "GLineStyle.h"
#include "GFillStyle.h"
#include "GTextStyle.h"
#include "GRange.h"
#include "GObjectCommon.h"
#include "GScaleFactor.h"
#include "gcomminterface.h"

class GScaleFactor;
class GGraphLayer;
class GFile;

//##ModelId=4339E604009C
//##Documentation
//## ���壺
//##        ͼԪ����
//## ���ܣ�
//##        ����ͼԪ��������
//##        ����ͼԪ���д˼̳�
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
class GDE_EXPORT_DES GBaseObject
{
  public:
	//##ModelId=4341030F0203
    //##Documentation
    //## ���ܣ�
    //##       ���캯��   
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-3
    GBaseObject();

    //##ModelId=4341032E029F
    //##Documentation
    //## ���ܣ�
    //##       ��������    
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-3
    virtual ~GBaseObject();
	//##ModelId=43A601AF0148
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ���ݺ�����
    //##      ��ȡ����������Ϣ
    //## ������
    //##      GFile& fFile
    //## ���أ���int32��
    //##      if success return read real size
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 loadData(GFile& fFile);
    //##ModelId=433FE3130261
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
	//##ModelId=43A6027D0186
    //##Documentation
    //## ���ܣ�
    //##      �����������ж�ȡ����
    //## ������
    //##      GBaseObject *pObj
    //## ���أ���int32��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 loadObj(GBaseObject* pObj );
	//##ModelId=43A602D10128
    //##Documentation
    //## ���ܣ�
    //##      �����������ж�ȡ����
    //## ������
    //##      const GBaseObject& obj
    //## ���أ���int32��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 loadObj(const GBaseObject& obj);
	//##ModelId=43A6024803B9
    //##Documentation
    //## ���ܣ�
    //##      ���ر������ݺ�����
    //##      ��������������Ϣ
    //## ������
    //##      GFile& fFile
    //## ���أ���int32��
    //##      if success return read real size
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 saveData(GFile& fFile);
    //##ModelId=433FE2890128
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

	//##ModelId=433FE2890128
    //##Documentation
    //## ���ܣ�
    //##      �Ƿ��ܻ��ƶ���
    //## ������
    //##      ��
    //## ���أ���uint8 ��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
	virtual uint8 canDrawObj(GDC* pDC) const;

    //##ModelId=4340A54901C5
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

	//##ModelId=4340A54901C5
    //##Documentation
    //## ���ܣ�
    //##   ʵ��ͼԪ���ƹ��ܣ�������̬ͼԪͼ��
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
	virtual void drawAction(GDC *pDC , GScaleFactor *pSF=NULL);

    //##ModelId=43422A6D031C
    //##Documentation
    //## ���ܣ�
    //##   �ṩͼԪ��¡����
    //## ������
    //##   ��
    //## ���أ�
    //##   ����ͼԪ�ڲ�����
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    virtual GBaseObject* clone();

    //##ModelId=4344F67700FA
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

    //##ModelId=4344F6F3038A
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ͼԪID����
    //## ������  
    //##   ��
    //## ���أ�
    //##   uint32 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    uint32 getID() const ;

	//##ModelId=4344F6F3038A
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ͼԪ��Χ
    //## ������  
    //##   ��
    //## ���أ�
    //##   GRect& 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
	GRect& getBox();
    const GRect& getBox() const ;

	//##ModelId=4344F6F3038A
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ͼԪ��Χ
    //## ������  
    //##   ��
    //## ���أ�
    //##   GRect& 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
	GListPOINTS& getBound();

	//##ModelId=4344F6F3038A
    //##Documentation
    //## ���ܣ�
    //##   ȡ��ͼԪ��Χ
    //## ������  
    //##   ��
    //## ���أ�
    //##   GRect& 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
	const GListPOINTS& getBound() const ;

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

	//##ModelId=43A0C35201B5
    //##Documentation
    //## ���ܣ�
    //##   �����ڲ�ʵʱֵ
    //## ������
    //##   const char* v
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-15
    virtual void setValue(const char* value);
    //##ModelId=43A0C3590119
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�ڲ�ʵʱֵ
    //## ������
    //##   ��
    //## ���أ�
    //##   FLOAT8
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-12-15
    FLOAT8 getValue() const;

    //##ModelId=4344F7770128
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ���Ͳ���
    //## ������  
    //##   ��
    //## ���أ�
    //##   uint32
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    uint32 getType() const;

    //##ModelId=4344F81300EA
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

    //##ModelId=4344F84F0232
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ��������
    //## ������  
    //##   ��
    //## ���أ�
    //##   const GFillStyle&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    const GFillStyle& getFillStyle() const;

    //##ModelId=4344F88203A9
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ��������
    //## ������  
    //##   ��
    //## ���أ�
    //##   GFillStyle&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    GFillStyle& getFillStyle();

    //##ModelId=4344F8C5006D
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ�߷��
    //## ������  
    //##   ��
    //## ���أ�
    //##   GLineStyle&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    GLineStyle& getLineStyle() ;

    //##ModelId=4344F8F102EE
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

    //##ModelId=4344F92D0148
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ�߷��
    //## ������  
    //##   ��
    //## ���أ�
    //##   const GLineStyl&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    const GLineStyle& getLineStyle() const;

    //##ModelId=4344F95200BB
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ��ת�Ƕ�
    //## ������  
    //##   FLOAT8 ra//��ת�Ƕ�
    //## ���أ�
    //##   void
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    virtual void setRotate(FLOAT8 ra);

    //##ModelId=4344F998035B
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ��ת�Ƕ�
    //## ������  
    //##   ��
    //## ���أ�
    //##   FLOAT8
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    FLOAT8 getRotate() const;

    //##ModelId=4344F9B800CB
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
    virtual void calcRect(){};

    //##ModelId=4344F9EF0119
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ�汾��
    //## ������  
    //##   const GVersion& v
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    void setVers(const GVersion& v);


    //##ModelId=4344FA5E03C8
    //##Documentation
    //## ���ܣ�
    //##  ��ȡͼԪ���汾��
    //## ������  
    //##   ��
    //## ���أ�
    //##   const GVersion&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    const GVersion& getVers() const;

    //##ModelId=4344FA8703A9
    //##Documentation
    //## ���ܣ�
    //##  ��ȡͼԪ�ΰ汾��
    //## ������  
    //##   ��
    //## ���أ�
    //##   GVersion&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    GVersion& getVers();

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

    //##ModelId=4344FB160399
    //##Documentation
    //## ���ܣ�
    //##  ��ȡͼԪ���ڲ�
    //## ������  
    //##   ��
    //## ���أ�
    //##   GGraphLayer*
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    GGraphLayer* getLayer() const;

    //##ModelId=43471C1C0213
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

    //##ModelId=43471C230290
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
    GPOS getPos() const;
    //##ModelId=434DC77E011E
    //##Documentation
    //## ���ܣ�
    //##   �����Ƿ���ʾ��ͼԪ
    //## ������
    //##   uint8 visable//��ʾ��־
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    //##   
    void setVis(uint8 bVis=1);
    //##ModelId=434DC7DD010E
    //##Documentation
    //## ���ܣ�
    //##   ����Ƿ���ʾ��ͼԪ
    //## ������
    //##   ��
    //## ���أ�uint8
    //##   if visable return 1
    //##   else return 0 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    uint8 isVis() const;
	 //##ModelId=437C11C10280
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
    //##ModelId=437C11F2032C
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
	//##ModelId=434DCD9402F0
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ���������ӱ�־
    //## ������
    //##   uint32 uFlag
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    void setRunFlag(uint32 uFlag );

	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ���������ӱ�־
    //## ������
    //##   ��
    //## ���أ�
    //##   if is return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	uint32 getRunFlag() const;

	//##ModelId=434DCD9402F0
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ���ӱ�־
    //## ������
    //##   uint32 uFlag
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    void setFlag(uint32 uFlag );

	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ���ӱ�־
    //## ������
    //##   ��
    //## ���أ�
    //##   if is return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	uint32 getFlag() const;
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
	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ�Ƿ񼤻�
    //## ������
    //##   ��
    //## ���أ�
    //##   if is return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	int32 getActive() const;    
    //##ModelId=434DE2D101C1
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
    uint32 getSize() ;
	 //##ModelId=434DE2D101C1
    //##Documentation
    //## ���ܣ�
    //##   ȡ�����ݳ���
    //## ������
    //##   ��
    //## ���أ�uint32
    //##   if success return realsize
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
	uint32 getDataSize();	
    //##ModelId=4354C073011F
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ���ӷ�Χ
    //## ������  
    //##   const GRange&
    //## ���أ�
    //##   void
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    void setRange(const GRange& range );
    //##ModelId=4354C0A4013E
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ���ӷ�Χ
    //## ������  
    //##   ��
    //## ���أ�
    //##   const GRange&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    const GRange& getRange() const;
    //##ModelId=435D919900D4
    //##Documentation
    //## ���ܣ�
    //##   ������ʾ��Χ
    //## ������
    //##   FLOAT8 vMin //��С��ʾ��Χ
    //##   FLOAT8 vMax //�����ʾ��Χ
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-25
    void setRange(FLOAT8 vm, FLOAT8 vn);
    //##ModelId=435D922101DD
    //##Documentation
    //## ���ܣ�
    //##   ����ָ��ֵ�Ƿ�����ʾ��Χ��
    //## ������
    //##   FLOAT8 v
    //## ���أ�
    //##   if is return 1
    //##   else return  0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-25
    uint8 canView(FLOAT8 v) const;
	//##ModelId=43A60320004E
    //##Documentation
    //## ���ܣ�
    //##   ����ʱ��Ҫ�������
	//##   ���ڲ��ͼԪ����
    //## ������
    //##   mode
	//##   if mode == 0 
	//##		if ��Ҫ�����߻��� return 1
	//##		else return 0
	//##   if mode == 1
	//##		if ����CTRL��һ������ return 1
	//##		else return 0
	//##   if mode == 2
	//##		if ����SHIFT��һ������ return 1
	//##		else return 0
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
	//##ModelId=43A60320004E
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
    //##ModelId=435DA0F202A2
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


    //##ModelId=435DA1010031
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
    //##ModelId=436C451A032C
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ���ӷ�Χ
    //## ������  
    //##   ��
    //## ���أ�
    //##   const GRange&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-6
    GRange& getRange();
    //##ModelId=436C46F50399
    //##Documentation
    //## ���ܣ�
    //##      ��ȡͼԪ�ı������Ϣ
    //## ������
    //##    ��
    //## ���أ�
    //##     const GTextStyle&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    const GTextStyle& getTextStyle() const;
    //##ModelId=436C4712033C
    //##Documentation
    //## ���ܣ�
    //##      ��ȡͼԪ�ı������Ϣ
    //## ������
    //##    ��
    //## ���أ�
    //##     GTextStyle&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    GTextStyle& getTextStyle();
    //##ModelId=436C47290148
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
    //##ModelId=436C476400BB
    //##Documentation
    //## ���ܣ�
    //##      ȡͼԪ�ڲ���������
    //## ������
    //##     ��
    //## ���أ���uint32��
    //##     �����ڲ���������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    uint32 getStyleMask() const;
    //##ModelId=436C47B302AF
    //##Documentation
    //## ���ܣ�
    //##      ����ͼԪ�ڲ���������
    //## ������
    //##     uint32 mask
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    virtual void setStyleMask(uint32 mask);
    //##ModelId=436C6DF600DA
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
    //##ModelId=436C781902BF
    //##Documentation
    //## ���ܣ�
    //##   ��ȡͼԪ�ڲ��������Ӳ���
    //## ������  
    //##   ��
    //## ���أ�
    //##   const GScaleFactor&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    FLOAT8 getScale() const;    
    //##ModelId=436C785E0251
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ�ڲ��������Ӳ���
    //## ������  
    //##   ��
    //## ���أ�
    //##   const GScaleFactor&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    void setScale(FLOAT8 scale);
    //##ModelId=436C970A0213
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
	//##ModelId=4375D0CC01D4
    //##Documentation
    //## ���ܣ�
    //##   ���Ե�ǰ����ͼԪ���λ��
    //## ������  
    //##   const GPoint& pt
    //## ���أ�
    //##   uint32
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-12
    virtual uint32 hitTest(const GPoint& pt);
    //##ModelId=4375D1AB02CE
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
    //##ModelId=4375DAD7031C
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
    virtual void drawFocus(GDC* pDC, uint32 nFlags = 0);
    //##ModelId=4375DB6B03A9
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
	//##ModelId=437C7800000F
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
	//##ModelId=437AB085004C
    //##Documentation
    //## ���ܣ�
    //##      ������ת���ĵ�
    //## ������
    //##      const GPoint& po
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-16
    virtual void setRO(const GPoint& po);
    //##ModelId=4375DC69002E
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
    //##ModelId=4375DCC301B5
    //##Documentation
    //## ���ܣ�
    //##     ��ȡ��ת�����㣬���ڽ�����ת����
    //## ������
    //##     ��
    //## ���أ�
    //##     ����������
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-11-12
    virtual int32 getROPT(GPoint *);
	
	//##ModelId=4377E06900DA
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
    //##ModelId=4377F8980034
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
	 //##ModelId=43793D6C0177
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
    //##ModelId=43788BA2032C
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
    //##ModelId=43793752036B
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
    //##ModelId=43793DC00177
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
	//##ModelId=43771C78002E
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
    //##ModelId=43771C9A02BF
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
	//##ModelId=437B2584031C
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
    //##ModelId=437B261000FA
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
    //##ModelId=437B2676000F
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
    //##ModelId=437B26810290
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
    //##ModelId=437B27590251
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
	//##ModelId=43814F200196
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
	//##ModelId=438690240232
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
    //##ModelId=438690A002FD
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

	//##ModelId=439EA8B601C5
    //##Documentation
    //## ���ܣ�
    //##    ��������ڲ�����,�ڲ�ʹ��
    //## ������
    //##     GFile& fFile
    //##     uint8  bLoad
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##     ������
    //## ���ڣ�
    //##     2005-12-13
	virtual void transPluginData(GFile& fFile,uint8 bload=1);
	//##ModelId=439FC62A00F3
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
	//##ModelId=43A0C0A70290
    //##Documentation
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
	//##ModelId=439FA91D01E7
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
	//##ModelId=43A0DD2A000F
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
	//##ModelId=43A6035702DE
    //##Documentation
    //## ���ܣ�
    //##   ���ñ�������
    //## ������
    //##   uint32 mask
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    virtual void setMask(uint32 mask);
    //##ModelId=43A603870271
    //##Documentation
    //## ���ܣ�
    //##   �����������
    //## ������
    //##   uint32 mask
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    virtual void clearMask(uint32 mask=0);
    //##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   �ж��Ƿ����ָ��������
    //## ������
    //##   uint32 mask
    //## ���أ�
    //##   if is return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    virtual uint8 isMask(uint32 mask) const;

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
	virtual void setDevType(uint32 devType) ;

	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�豸״̬
    //## ������
    //##   uint32
    //## ���أ�
    //##   uint8 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	virtual uint8 isDevStatus(uint32 devStatus) const;
	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�豸״̬
    //## ������
    //##   ��
    //## ���أ�
    //##   uint32 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	virtual uint32 getDevStatus() const ;
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
    //##   �����豸״̬
    //## ������
    //##   uint32
    //## ���أ�
    //##   uint8 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	virtual void addDevStatus(uint32 devStatus);

	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   ����豸״̬
    //## ������
    //##   uint32
    //## ���أ�
    //##   uint8 
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	virtual void clrDevStatus(uint32 devStatus);
	
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

	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ���ֱ�־
    //## ������
    //##   ��
    //## ���أ�
    //##   if is return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	uint8  getFlash() const ;
	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   �������ֱ�־
    //## ������
    //##   uint8 f
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	void   setFlash(uint8 f=1) ;

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
    //##   ��ȡ��Ϊ
    //## ������
    //##   ��
    //## ���أ�
    //##   LPDA
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	//LPDA  getAct() const ;
	uint32 countAct() const;
	GPOS   firstAct() const;
	LPDA   nextAct(GPOS& gPos) const;
	GPOS   lastAct( ) const;
	//##ModelId=43FFCF4A009C
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
    //##ModelId=43FFCFFC030D
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

	//##ModelId=43FFCFFC030D
    //##Documentation
    //## ���ܣ�
    //##   ȡͼԪ�еĶ�̬��������
    //## ������
    //##   ��
    //## ���أ�
    //##   GGraphDynDataList*
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2006-2-27
	virtual GGraphDynDataList* getDynData() const;

	//##ModelId=43FFCFFC030D
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ�еĸ�������ָ��
    //## ������
    //##   GHANDLE 
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2006-2-27
	void setAttachPtr(GHANDLE data);

	//##ModelId=43FFCFFC030D
    //##Documentation
    //## ���ܣ�
    //##   ȡͼԪ�еĸ�������ָ��
    //## ������
    //##   ��
    //## ���أ�
    //##   GHANDLE
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2006-2-27
	GHANDLE getAttachPtr() const;

	//##ModelId=43FFCFFC030D
    //##Documentation
    //## ���ܣ�
    //##   ����ͼԪ�еĸ�������
    //## ������
    //##   uint32 
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2006-2-27
	void setAttachData(uint32 data);

		//##ModelId=43FFCFFC030D
    //##Documentation
    //## ���ܣ�
    //##   ȡͼԪ�еĸ�������
    //## ������
    //##   ��
    //## ���أ�
    //##   GHANDLE
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2006-2-27
	uint32 getAttachData() const;

	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ��ͼԪ
    //## ������
    //##   ��
    //## ���أ�
    //##   GBaseObject*
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	GBaseObject* getParent() const;

	//##ModelId=43A603AC0280
    //##Documentation
    //## ���ܣ�
    //##   ���ø�ͼԪ
    //## ������
    //##   GBaseObject*
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
	void setParent(GBaseObject* parent);
	
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

    //##ModelId=433D3DC30000
    //##Documentation
    //## ͼԪID�ţ���ͼ��Ψһ��ʶ
    uint32 m_nID;

    //##ModelId=433D3DF703B9
    //##Documentation
    //## ͼԪ����
    uint32 m_nType;

    //##ModelId=433D3FDF02AF
    //##Documentation
    //## ͼԪ��Χ
    GRect m_boundBox;

    //##ModelId=4340FC7402EE
    //##Documentation
    //## ��̬�����б�
	GGraphDynDataList m_lstDynData;
    //##ModelId=434DC6710312
    //##Documentation
    //## �Ƿ���ʾ��ͼԪ 1=vis,0=not vis
    uint8 m_vis;
    //##ModelId=43531B9C0213
    //##Documentation
    //## �汾��
    GVersion m_vers;
    //##ModelId=436C42490157
    //##Documentation
    //## �߷��
    GLineStyle m_lineStyle;
    //##ModelId=436C42B1036B
    //##Documentation
    //## �����
    GFillStyle m_fillStyle;
    //##ModelId=436C42C70148
    //##Documentation
    //## �ı����
    GTextStyle m_textStyle;
    //##ModelId=436C42DC035B
    //##Documentation
    //## ���ӷ�Χ
    GRange m_range;
    //##ModelId=436C479E0128
	//##Documentation
    //## ��������
    uint32 m_styleMask;  
	//##ModelId=436C779D00EA
    //##Documentation
    //## �ڲ��������ӣ������ڲ����ݹ̻�����
    FLOAT8 m_scale;
    //##ModelId=437AB0600230
    //##Documentation
    //## ��ת���ĵ�
    GPoint m_pointO;
    //##ModelId=4340A28500FA
    //##Documentation
    //## ��ת�Ƕ�
    FLOAT8 m_angle;
	//##ModelId=4340A28500FA
    //##Documentation
    //## ��ת��Ϊִ��
	FLOAT8 m_rotate;
	//##ModelId=43A0C32E036B
    //##Documentation
    //## �ڲ�ʵʱֵ
    FLOAT8 m_fValue;
	//##ModelId=43A0C32E036B
    //##Documentation
    //## �豸����
    uint32 m_devType;
	//##ModelId=43A0C32E036B
    //##Documentation
    //## �豸״̬
    uint32 m_devStatus;
	
  protected:
    //##ModelId=4340EFE00261
    //##Documentation
    //## ͼԪ���ڲ�
    GGraphLayer* m_curLayer;
    //##ModelId=43471BEA005D
    //##Documentation
    //## ��¼��ǰͼԪ��Ӧ����������ָ��
    GPOS m_curPos;
	//##ModelId=437C11460167
    //##Documentation
    //## ��ͼԪ�Ƿ�Ϊɾ��״̬�����Ϊɾ��״̬
    //## �洢ʱ�����д洢������ʱ�����л���
    uint8 m_del;
	//##ModelId=437C11460167
    //##Documentation
    //## ͼԪ��̬��Ϊ�б�
	//LPDA  m_dynAct ;s
	GDAL  m_dynAct ;
	//##ModelId=437C11460167
    //##Documentation
    //## ��˸��־
	uint8 m_flash  ;
	//##ModelId=434DCD5D014B
    //##Documentation
    //## ͼԪ���ӱ�־
    uint32 m_topoFlag;
    //##ModelId=434DCD5D014B
    //##Documentation
    //## ͼԪ����ɫ״̬,���Ϊ0ʱ��ʾ��ͼԪΪ��ʵʱ��ͼԪ,����Ҫ����״̬���ı���ɫ
    int32 m_active;
	//##ModelId=437C11460167
    //##Documentation
    //## ��������ָ��
	GHANDLE m_attachDataPtr;
	//##ModelId=437C11460167
    //##Documentation
    //## ��������,�����û���Ϣ����
	uint32  m_attachData;
	//##ModelId=437C11460167
    //##Documentation
    //## ���������ӱ�־
	uint32 m_runFlag;
	//##ModelId=43A625C30203
    //##Documentation
	//## ��ͼԪ���븸ͼԪ����ʹ��
	GBaseObject* m_parentObj;
	//##ModelId=43A625C30204
	//##Documentation
	//## �����壬�븸���彻��ʹ��
	QWidget* m_parentWidget;


	QMutex		m_actionMutex;
	QMutex		m_setValueMutex;

  protected:
	//##ModelId=437C11460167
    //##Documentation
    //## ��ȡָ������λ�õĵ�λ��
	virtual const GPoint getPoint(uint32 size) const;
};

#endif /* GBASEOBJECT_H_HEADER_INCLUDED_BCB8A08D */
