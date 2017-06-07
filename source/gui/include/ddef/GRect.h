#ifndef GRECT_H_HEADER_INCLUDED_BCB8DC85
#define GRECT_H_HEADER_INCLUDED_BCB8DC85

#include "ddes_comm.h"
#include "GPoint.h"
//##ModelId=4339FF4B032C
//##Documentation
//## ���ܣ�
//##       ������������ݶ�ʧ�ľ�������
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
class GDE_EXPORT_DEF GRect
{
  public:    

    //##ModelId=433A9B800251
    //##Documentation
    //## ���ܣ�
    //##       ����վ���  
    //## ������
    //##      ��
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    GRect();
	//##ModelId=437883BD01E4
    //##Documentation
    //## ���ܣ�
    //##       �趨����      
    //## ������
    //##      const GPoint& lt //���Ͻ�
    //##      const GPoint& rb //���½�
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    GRect(const GPoint& tl, const GPoint& rb);
    //##ModelId=433A9BA403C8
    //##Documentation
    //## ���ܣ�
    //##       ����ָ������   
    //## ������
    //##      FLOAT8 left  //���Ͻ�x����
    //##      FLOAT8 top  //���Ͻ�y����
    //##      FLOAT8 right //���½�x����
    //##      FLOAT8 bottom//���½�y����
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    GRect(FLOAT8 l, FLOAT8 t, FLOAT8 r, FLOAT8 b);
	//##ModelId=433A069202AF
    //##Documentation
    //## ���ܣ�
    //##      ����ָ�����Ƿ��ڵ�ǰ������    
    //## ������
    //##      const GPoint& point//���Ե�
    //## ���أ���uint8��
    //##      if point in curr rect return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    uint8 container(const GPoint& point) const;

    //##ModelId=433A0745006D
    //##Documentation
    //## ���ܣ�
    //##      ����ָ�������Ƿ��ڵ�ǰ������     
    //## ������
    //##      const GRect& rect//���Ծ���
    //## ���أ���uint8��     
    //##       if rect in curr rect return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    uint8 container(const GRect& rect) const;

    //##ModelId=433A9ACA00CB
    //##Documentation
    //## ���ܣ�
    //##      ����ָ�����Ƿ��ڵ�ǰ������     
    //## ������
    //##      FLOAT8  x
    //##      FLOAT8  y//���Ե�
    //## ���أ���uint8��
    //##      if point in curr rect return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    uint8 container(FLOAT8 x, FLOAT8 y) const;
    //##ModelId=433A9BF70128
    //##Documentation
    //## ���ܣ�
    //##       �����������Ƿ��ཻ      
    //## ������
    //##       const GRect& rect
    //## ���أ� ��uint8��
    //##       if success return 1
    //##       else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    uint8 overlap(const GRect& rect) const;

    //##ModelId=433A9C3B0186
    //##Documentation
    //## ���ܣ�
    //##       �����趨����
    //## ������
    //##      FLOAT8 left  //���Ͻ�x����
    //##      FLOAT8 top  //���Ͻ�y����
    //##      FLOAT8 right //���½�x����
    //##      FLOAT8 bottom//���½�y����      
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& set(FLOAT8 l, FLOAT8 t, FLOAT8 r, FLOAT8 b);

    //##ModelId=433A9C9700DA
    //##Documentation
    //## ���ܣ�
    //##       �����趨����      
    //## ������
    //##      const GPoint lt //���Ͻ�
    //##      const GPoint rb//���½�
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& set(const GPoint& lt, const GPoint& rb);

	//##ModelId=433A9C9700DA
    //##Documentation
    //## ���ܣ�
    //##       �����趨����      
    //## ������
    //##      FLOAT8 width //��
    //##      FLOAT8 height//��
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& set(FLOAT8 width, FLOAT8 height);

	//##ModelId=433A9C9700DA
    //##Documentation
    //## ���ܣ�
    //##       �����趨����      
    //## ������
    //##      FLOAT8 width //��
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& setWidth(FLOAT8 width);
	
	//##ModelId=433A9C9700DA
    //##Documentation
    //## ���ܣ�
    //##       �����趨����      
    //## ������
    //##      FLOAT8 height//��
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& setHeight(FLOAT8 height);
	
	
    //##ModelId=433A9CE803D8
    //##Documentation
    //## ���ܣ�
    //##       ���ƽ�ƾ���    
    //## ������
    //##       FLOAT8 cx //����ƶ�x�����
    //##       FLOAT8 cy //����ƶ�y�����
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& offset(FLOAT8 cx, FLOAT8 cy);

    //##ModelId=433A9D67032C
    //##Documentation
    //## ���ܣ�
    //##       �ƶ����ε�ָ��λ��      
    //## ������
    //##       FLOAT8 x //�ƶ���x��λ��
    //##       FLOAT8 y //�ƶ���y��λ��
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& move(FLOAT8 x, FLOAT8 y);

	//##ModelId=433A9D67032C
    //##Documentation
    //## ���ܣ�
    //##       �ƶ����ε�ָ��λ��      
    //## ������
    //##       const GPoint& pt
    //## ���أ�
    //##       const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& move(const GPoint& pt);

	//##ModelId=433A9D67032C
    //##Documentation
    //## ���ܣ�
    //##      �ƶ��������ĵ㵽ָ��λ��      
    //## ������
    //##      const GPoint& ptc
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& centerPoint(const GPoint& ptc);

	//##ModelId=433A9D67032C
    //##Documentation
    //## ���ܣ�
    //##      �ƶ��������ĵ㵽ָ���������ĵ�λ��      
    //## ������
    //##      const GRect& rc
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& centerRect(const GRect& rc);

    //##ModelId=433A9E5501D4
    //##Documentation
    //## ���ܣ�
    //##       ���ž���       
    //## ������
    //##       FLOAT8 cx //��Сx��ߴ�
    //##       FLOAT8 cy //��Сy��ߴ�
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& deflate(FLOAT8 cx, FLOAT8 cy);

    //##ModelId=433A9EB901D4
    //##Documentation
    //## ���ܣ�
    //##       ���ž���       
    //## ������
    //##       FLOAT8 l //��Сx��ߴ�
    //##       FLOAT8 t //��Сy��ߴ�
    //##       FLOAT8 r //��Сx��ߴ�
    //##       FLOAT8 b //��Сy��ߴ�
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& deflate(FLOAT8 l, FLOAT8 t, FLOAT8 r, FLOAT8 b);

    //##ModelId=433A9F060128
    //##Documentation
    //## ���ܣ�
    //##       ���ž���       
    //## ������
    //##       FLOAT8 xy //��Сxy��ߴ�
    //## ���أ�
    //##      const GRect&
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& deflate(FLOAT8 xy);
	//##ModelId=4434A34B0261
    //##Documentation
    //## ���ܣ�
    //##   ��ֵ����
    //## ������
    //##   const GRect&
    //## ���أ�
    //##   const GRect&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    const GRect& operator=(const  GRect& copy);
    //##ModelId=433A9F440399
    //##Documentation
    //## ���ܣ�
    //##       �������������ཻ�Ĳ��֣����洢������
    //## ������
    //##       const GRect& rect //�ཻ�ľ���      
    //## ���أ���const GRect&��
    //##       ���ؽ������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& operator &=(const GRect& rect);

    //##ModelId=433A9FD501A5
    //##Documentation
    //## ���ܣ�
    //##       �����ܰ����������ε���С����      
    //## ������
    //##       const GRect& rect //�ཻ�ľ���      
    //## ���أ���const GRect��
    //##       ���ؽ������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& operator |=(const GRect& rect);

    //##ModelId=433AA03300EA
    //##Documentation
    //## ���ܣ�
    //##       ȡ���������ཻ�Ĳ���  
    //## ������
    //##       const GRect& rect //�ཻ�ľ��� 
    //## ���أ���const GRect��
    //##       ���ؽ������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect operator &(const GRect& rect);

    //##ModelId=433AA0B2008C
    //##Documentation
    //## ���ܣ�
    //##       ���ε�x�᷽������������Point.x�����أ�
    //##       ���ε�y�᷽������������Point.y������ 
    //## ������
    //##       const GPoint& point//������
    //## ���أ���const GRect ��
    //##       ���ؽ������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect operator +(const GPoint& point);

    //##ModelId=433AA131029F
    //##Documentation
    //## ���ܣ�
    //##       ���ε�x,y�᷽������������x������
    //## ������
    //##       FLOAT8 x
    //## ���أ���const GRect &��
    //##       ���ؽ������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect &operator +=(FLOAT8 x);

    //##ModelId=433AA137034B
    //##Documentation
    //## ���ܣ�
    //##       ���ε�x,y�᷽������������1������
    //## ������
    //##      ��
    //## ���أ���const GRect&��
    //##       ���ؽ������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& operator++(int);

    //##ModelId=433AA224004E
    //##Documentation
    //## ���ܣ�
    //##       ���ε�x,y�᷽����������Сx�����أ�
    //## ������
    //##       FLOAT8 x
    //## ���أ���const GRect &��
    //##       ���ؽ������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& operator-=(FLOAT8 x);

    //##ModelId=433AA2270109
    //##Documentation
    //## ���ܣ�
    //##       ���ε�x,y�᷽����������С1������
    //## ������
    //##      ��
    //## ���أ���const GRect&��
    //##       ���ؽ������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& operator--(int);

    //##ModelId=433AA2D0030D
    //##Documentation
    //## ���ܣ�
    //##       �Ƚ��������Ƿ����
    //## ������
    //##       const GRect& rect//�Ƚϵľ�������
    //## ���أ���uint8��
    //##       if equ return 1
    //##       else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    uint8 operator==(const GRect& rect) const;

    //##ModelId=433AA3270251
    //##Documentation
    //## ���ܣ�
    //##       �Ƚ��������Ƿ����
    //## ������
    //##       const GRect& rect//�Ƚϵľ�������
    //## ���أ���uint8��
    //##       if equ return 0
    //##       else return 1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    uint8 operator !=(const GRect& rect) const;

    //##ModelId=433AA36601A5
    //##Documentation
    //## ���ܣ�
    //##       ����ο��
    //## ������
    //##     ��
    //## ���أ���FLOAT8 ��
    //##       ���ο��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    FLOAT8 width() const;

    //##ModelId=433AA36E038A
    //##Documentation
    //## ���ܣ�
    //##       ����θ߶�
    //## ������
    //##     ��
    //## ���أ���FLOAT8 ��
    //##       ���θ߶�
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    FLOAT8 height() const;

    //##ModelId=433AA37002EE
    //##Documentation
    //## ���ܣ�
    //##       ��������ĵ�
    //## ������
    //##      ��
    //## ���أ���const GPoint��
    //##       �������ĵ�
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GPoint center() const;

    //##ModelId=433AA41A0399
    //##Documentation
    //## ���ܣ�
    //##       �������������
    //## ������
    //##      ��
    //## ���أ���const GPoint��
    //##       ������������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GPoint topLeft() const;

    //##ModelId=433AA42E0242
    //##Documentation
    //## ���ܣ�
    //##       �������������
    //## ������
    //##      ��
    //## ���أ���const GPoint��
    //##       ������������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GPoint bottomRight() const;
    //##ModelId=434BDC370268
    //##Documentation
    //## ���ܣ�
    //##       �������������
    //## ������
    //##      ��
    //## ���أ���const GPoint��
    //##       ������������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GPoint topRight() const;
    //##ModelId=434BDC5C00D2
    //##Documentation
    //## ���ܣ�
    //##       �������������
    //## ������
    //##      ��
    //## ���أ���const GPoint��
    //##       ������������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GPoint bottomLeft() const;
	//##ModelId=437E83D002CE
    //##Documentation
    //## ���ܣ�
    //##       �������������
    //## ������
    //##      ��
    //## ���أ���const GPoint��
    //##       ������������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GPoint topCenter() const;
    //##ModelId=437E83E50232
    //##Documentation
    //## ���ܣ�
    //##       �������������
    //## ������
    //##      ��
    //## ���أ���const GPoint��
    //##       ������������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GPoint bottomCenter() const;
    //##ModelId=437E83F40242
    //##Documentation
    //## ���ܣ�
    //##       �������������
    //## ������
    //##      ��
    //## ���أ���const GPoint��
    //##       ������������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GPoint leftCenter() const;
    //##ModelId=437E83FB034B
    //##Documentation
    //## ���ܣ�
    //##       �������������
    //## ������
    //##      ��
    //## ���أ���const GPoint��
    //##       ������������
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GPoint rightCenter() const;
    //##ModelId=434BDD30017D
    //##Documentation
    //## ���ܣ� 
    //##   ���滯����
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-11
    void normal();
	//##ModelId=4378865000FA
    //##Documentation
    //## ���ܣ�
    //##      �������Ƿ�Ϊ��
    //## ������
    //##      ��
    //## ���أ�
    //##       if empty return 1
    //##       else 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    uint8 isEmpty() const;
	//##ModelId=43850C8203A9
    //##Documentation
    //## ���ܣ�
    //##       ���ɾ���
    //## ������
    //##       GPoint* pts 
    //##       int32   npts
    //## ���أ�
    //##       const GRect&      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    const GRect& convFromPoints(GPoint* pts, int32 npts);
	//##ModelId=43850C8203A9
    //##Documentation
    //## ���ܣ�
    //##       ���ɻ�����
    //## ������
    //##       GPoint* pts 
    //##       int32   npts
	//##	   if npts == 2  get topLeft,botRight
	//##	   if npts == 4  get topLeft,topRight,botRight,botLeft
	//##	   if npts == 8  get topLeft,topcenter,
	//##						 topRight,rightcenter
	//##						 botRight,botcenter
	//##						 botLeft,leftcenter
    //## ���أ�
    //##       int32    //ʵ��ת���ĸ���
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    int32 convToPoints(GPoint* pts, int32 npts);
    //##ModelId=4378868F001F
    //##Documentation
    //## ���ܣ�
    //##      ���þ���Ϊ��
    //## ������
    //##      ��
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    void setEmpty();	

    //##ModelId=4339FF5800DA
    FLOAT8 top;

    //##ModelId=4339FF6A02FD
    FLOAT8 left;

    //##ModelId=4339FF6B02FD
    FLOAT8 bottom;

    //##ModelId=4339FF6C02EE
    FLOAT8 right;

};

#endif /* GRECT_H_HEADER_INCLUDED_BCB8DC85 */
