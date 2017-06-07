#ifndef GOBJECTMANAGE_H_HEADER_INCLUDED_BC868426
#define GOBJECTMANAGE_H_HEADER_INCLUDED_BC868426
#include "GComObject.h"
#include "ddef/ddes_comm.h"
#include "ddef/GPoint.h"
class GBaseObject;
class GPoint;
class GGraphFile;

//##ModelId=43797F090182
//##Documentation
//## ���壺
//##        ͼԪ�����
//## ���ܣ�
//##        ���ڹ���ͼԪ������
//##        ͼԪ��ӣ�ɾ���ָ�������
//##        ���У����ƣ�ճ��
//##        ��ת�������
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-15
class GDE_EXPORT_DES GObjectManage : public GComObject
{
  public:
    //##ModelId=4379807D0124
    //##Documentation
    //## ���ܣ�
    //##     ����/��������    
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    GObjectManage();
	//##ModelId=43798052029B
    //##Documentation
    //## ���ܣ�
    //##     ����/��������    
    //## ������
    //##     ��
    //## ���أ�
    //##     ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-15
    virtual ~GObjectManage();
	//##ModelId=437AB47A0200
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
    //##ModelId=437AB47A025E
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
	virtual void  setAct(LPDA dynAct);

    //##ModelId=4379827C0190
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
    //##ModelId=4379827C0190
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
    //##ModelId=4379827C020D
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
    //##ModelId=4379827C027A
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
    //##ModelId=4379827C02E8
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
    //##ModelId=4379827C0365
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
    //##ModelId=4379827C03D2
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
    //##ModelId=4379827D0067
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
    //##ModelId=4379827D00D4
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
    //##ModelId=4379827D0161
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
    //##ModelId=4379827D01DE
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
    //##ModelId=4379827D025B
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
    virtual void drawFocus(GDC* pDC, uint32 nFlags=0);
    //##ModelId=4379827D02D8
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
    //##ModelId=4379827D0365
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
	//##ModelId=437C7852032C
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
    //##ModelId=4379827D03E2
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
    //##ModelId=4379827E0190
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
    //##ModelId=4379827E022C
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
    //##ModelId=4379827E02B9
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
    //##ModelId=4379827E0345
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
    //##ModelId=4379827F0122
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
    //##ModelId=4379827F01CE
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
    //##ModelId=4379827F026B
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
    //##ModelId=4379827F0307
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
    //##ModelId=437990B2037E
    //##Documentation
    //## ���ܣ�
    //##      ���������ĵ�
    //## ������
    //##      GGraphFile* gFile//ָ�������ļ�
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    void setFile(GGraphFile* gFile);
    //##ModelId=437990B30051
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
    //##       2005-11-15
    GGraphFile* getFile() const;   
    //##ModelId=43799675005E
    //##Documentation
    //## ���ܣ�
    //##      ѡ������Ƿ��������
    //## ������
    //##      ��
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    uint8 canUp();
    //##ModelId=437996F402C0
    //##Documentation
    //## ���ܣ�
    //##      ѡ������Ƿ�����½�
    //## ������
    //##      ��
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    uint8 canDown();
    //##ModelId=4379971A00EA
    //##Documentation
    //## ���ܣ�
    //##      ѡ������Ƿ�����ö�
    //## ������
    //##      ��
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    uint8 canTop();
    //##ModelId=4379972001C5
    //##Documentation
    //## ���ܣ�
    //##      ѡ������Ƿ������β
    //## ������
    //##      ��
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    uint8 canBottom();
    //##ModelId=4379979D01F4
    //##Documentation
    //## ���ܣ�
    //##      ѡ������Ƿ���Զ���
    //## ������
    //##      ��
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    uint8 canAlign();
    //##ModelId=437997B20129
    //##Documentation
    //## ���ܣ�
    //##      ѡ������Ƿ���Ժϲ�
    //## ������
    //##      ��
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    uint8 canUnion();
    //##ModelId=437997D200AC
    //##Documentation
    //## ���ܣ�
    //##      ѡ������Ƿ���Էָ�
    //## ������
    //##      ��
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    uint8 canSplit();
    //##ModelId=437998250177
    //##Documentation
    //## ���ܣ�
    //##      ѡ������Ƿ���Ծ���
    //## ������
    //##      ��
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    uint8 canMirror();
    //##ModelId=4379983801F4
    //##Documentation
    //## ���ܣ�
    //##      ѡ������Ƿ������ת
    //## ������
    //##      ��
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    virtual uint8 canRotate() const;
    //##ModelId=4379985E0281
    //##Documentation
    //## ���ܣ�
    //##      ѡ������Ƿ���Ը���
    //## ������
    //##      ��
    //## ���أ�
    //##     if success return 1
    //##     else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    uint8 canCopy();
	
	//##ModelId=4379B91901A5
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
    //##ModelId=4387FB14002E
    //##Documentation
    //## ���ܣ�
    //##   ͼԪ�������,����ָ�����ܶ���ͼԪ
    //## ������
    //##   uint32 op
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    virtual uint8 alignPos(uint32 op,FLOAT8* rv=NULL);
    //##ModelId=4387FB1400BB
    //##Documentation
    //## ���ܣ�
    //##   ͼԪ�ȴ�С����,����ָ�����ܵȴ�СͼԪ
    //## ������
    //##   uint32 op
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-17
    virtual uint8 sameSize(uint32 op,FLOAT8 *rw=NULL,FLOAT8 *rh=NULL);
	//##ModelId=4388007A0177
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
    //##ModelId=4388007A01F4
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
    void setVis(uint8 visable);

  protected:    
    //##ModelId=437990970264
    GGraphFile* m_graphFile;

};



#endif /* GOBJECTMANAGE_H_HEADER_INCLUDED_BC868426 */
