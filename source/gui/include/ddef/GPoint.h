#ifndef GPOINT_H_HEADER_INCLUDED_BCB8A4D3
#define GPOINT_H_HEADER_INCLUDED_BCB8A4D3
#include "ddes_comm.h"
#include "GList.h"

//##ModelId=4339FECE036B
//##Documentation
//## ���ܣ�
//##        ������������ݶ�ʧ�ĵ㶨��
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-9-30
class GDE_EXPORT_DEF GPoint
{
  public:
    //##ModelId=433A015D0138
    //##Documentation
    //## ���ܣ�
    //##       �Ƚ������Ƿ����
    //## ������
    //##       const GPoint& point//�Ƚϵĵ�
    //## ���أ���uint8��
    //##       if not equ return 1
    //##       else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    uint8 operator !=(const GPoint& point) const;

    //##ModelId=433A01D9003E
    //##Documentation
    //## ���ܣ�
    //##       �Ƚ������Ƿ����
    //## ������
    //##       const GPoint& point//�Ƚϵĵ�
    //## ���أ���uint8��
    //##       if equ return 1
    //##       else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-9-28
    uint8 operator==(const GPoint& point) const;

    //##ModelId=433A01F60251
    //##Documentation
    //## ���ܣ�
    //##      ���ز�����+=
    //## ������
    //##      const GPoint& point//�ƶ��ĵ�      
    //## ���أ���const GPoint&��
    //##     ���ؼ�����ĵ�
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-1
    const GPoint& operator+=(const GPoint& point);

    //##ModelId=433A998E038A
    //##Documentation
    //## ���ܣ�
    //##      ���ز�����+
    //## ������
    //##      const GPoint& point//�ƶ��ĵ�      
    //## ���أ���const GPoint��
    //##     ���ؼ�����ĵ㣬���ı�õ�
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-1
    const GPoint operator+(const GPoint& point) const;
    //##ModelId=436CA0000399
    //##Documentation
    //## ���ܣ�
    //##   ���캯��
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    GPoint();

	GPoint(int n);

    //##ModelId=436CA008036B
    //##Documentation
    //## ���ܣ�
    //##   �������캯��
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    GPoint(const GPoint& copy);
    //##ModelId=436CA01F0290
    //##Documentation
    //## ���ܣ�
    //##   �������캯��
    //## ������
    //##   FLOAT8 x
    //##   FLOAT8 y
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    GPoint(FLOAT8 sx, FLOAT8 sy);	
    //##ModelId=436CA08B006D
    //##Documentation
    //## ���ܣ�
    //##   ��ֵ����
    //## ������
    //##   const GPoint&
    //## ���أ�
    //##   const GPoint&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    const GPoint& operator=(const GPoint& copy);
    //##ModelId=436CA7D3033C
    //##Documentation
    //## ���ܣ�
    //##      ��㵽��һ��ľ���
    //## ������
    //##      const GPoint& point//�ƶ��ĵ�      
    //## ���أ�
    //##     FLOAT8
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    FLOAT8 distance(const GPoint& point) const;
    //##ModelId=436CA83D0242
    //##Documentation
    //## ���ܣ�
    //##      ƫ�Ƶ�
    //## ������
    //##      FLOAT8 ox
    //##      FLOAT8 oy
    //## ���أ���const GPoint&��
    //##     ���ؼ�����ĵ�,���ظı�
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-5
    const GPoint& offset(FLOAT8 ox, FLOAT8 oy);
    //##ModelId=436CAA940280
    //##Documentation
    //## ���ܣ�
    //##      ���Ա���Ϊ����ĽǶ�
    //## ������
    //##      const GPoint& pt1//��1
    //##      const GPoint& pt2//��2
    //## ���أ�
    //##     FLOAT8
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    FLOAT8 angle(const GPoint& pt1, const GPoint& pt2) const;
    //##ModelId=436CB9B10186
    //##Documentation
    //## ���ܣ�
    //##      ���ز�����-
    //## ������
    //##      const GPoint& point//�ƶ��ĵ�      
    //## ���أ���const GPoint��
    //##     ���ؼ�����ĵ㣬���ı�õ�
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    const GPoint operator-(const GPoint& point) const;
    //##ModelId=436CBA1101A5
    //##Documentation
    //## ���ܣ�
    //##      ���ز�����-=
    //## ������
    //##      const GPoint& point//�ƶ��ĵ�      
    //## ���أ���const GPoint&��
    //##     ���ؼ�����ĵ㣬�ı�õ�
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    const GPoint& operator-=(const GPoint& point);
    //##ModelId=436CBC9401C5
    //##Documentation
    //## ���ܣ�
    //##      ��õ㵽���������ֱ�ߵľ���
    //## ������
    //##      const GPoint& pt1//��1
    //##      const GPoint& pt2//��2      
    //## ���أ�
    //##     FLOAT8
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    FLOAT8 distance(const GPoint& pt1, const GPoint& pt2) const;
    //##ModelId=436CC00B0128
    //##Documentation
    //## ���ܣ�
    //##      ��õ�������������������ε����
    //## ������
    //##      const GPoint& pt1//��1
    //##      const GPoint& pt2//��2      
    //## ���أ�
    //##     FLOAT8
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    FLOAT8 area(const GPoint& pt1, const GPoint& pt2) const;
    //##ModelId=436CCE680222
    //##Documentation
    //## ���ܣ�
    //##      ��õ��Ƿ������������ڵ�ֱ����
    //## ������
    //##      const  GPoint& pt1//��1
    //##      const  GPoint& pt2//��2
    //##      int32  width      //�߿�
    //##      FLOAT8 range      //��Χ
    //## ���أ�
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    uint8 onLine(const GPoint& pt1, const GPoint& pt2, int32 width, FLOAT8 range) const;
    //##ModelId=436CD1920261
    //##Documentation
    //## ���ܣ�
    //##      ��õ��Ƿ��ڶ������
    //## ������
    //##      GPoint* pts //���������
    //##      int32   ptn //�����
    //## ���أ�
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    uint8 inPolygon(GPoint* pts, int32 ptn) const;
    //##ModelId=436CD33E01A5
    //##Documentation
    //## ���ܣ�
    //##      ��õ��Ƿ��ڶ������
    //## ������
    //##      GPoint* pts //���������
    //##      int32   ptn //�����
    //##      int32  width//�߿�
    //##      FLOAT8 range//��Χ
    //## ���أ�
    //##      if success return edge pos
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    int32 onPolygon(GPoint* pts, int32 ptn, int32 width, FLOAT8 range) const;
    //##ModelId=436CD54F0222
    //##Documentation
    //## ���ܣ�
    //##      ��õ��Ƿ�����Բ��
    //## ������
    //##      const GPoint& c    //��Բ���ĵ�
    //##      FLOAT8 a           //a�᳤
    //##      FLOAT8 b           //b�᳤
    //##      int32  w           //�߿�
    //## ���أ�
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    uint8 inEllipse(const GPoint& c, FLOAT8 a, FLOAT8 b, int32 w) const;
    //##ModelId=436CD5560196
    //##Documentation
    //## ���ܣ�
    //##      ��õ��Ƿ�����Բ��
    //## ������
    //##      const GPoint& c    //��Բ���ĵ�
    //##      FLOAT8 a           //a�᳤
    //##      FLOAT8 b           //b�᳤
    //##      int32  w           //�߿�
    //##      FLOAT8 r           //��Χ
    //## ���أ�
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    uint8 onEllipse(const GPoint& c, FLOAT8 a, FLOAT8 b, int32 w, FLOAT8 r) const;
    //##ModelId=436CD8F7009C
    //##Documentation
    //## ���ܣ�
    //##      ���Ա�������һ������ֱ����X��н�->���߼�����ϵXY��
    //## ������
    //##      const GPoint& pt//��
    //## ���أ�
    //##      FLOAT8
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-5
    FLOAT8 slopeAngle(const GPoint& pt) const;
    //##ModelId=436D604800EA
    //##Documentation
    //## ���ܣ�
    //##      ��õ��Ƿ���Բ��
    //## ������
    //##      const GPoint& c    //Բ���ĵ�
    //##      FLOAT8 r            //�뾶
    //##      int32 w            //�߿�
    //## ���أ�
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    uint8 inCircle(const GPoint& c, FLOAT8 r, int32 w) const;
    //##ModelId=436D605300BB
    //##Documentation
    //## ���ܣ�
    //##      ��õ��Ƿ���Բ��
    //## ������
    //##      const GPoint& c    //Բ���ĵ�
    //##      FLOAT8 r           //�뾶
    //##      int32  w           //�߿�
    //##      FLOAT8 range       //��Χ
    //## ���أ�
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    uint8 onCircle(const GPoint& c, FLOAT8 r, int32 w, FLOAT8 range) const;
    //##ModelId=436D64960232
    //##Documentation
    //## ���ܣ�
    //##      ��õ��Ƿ��ھ�����
    //## ������
    //##      const GPoint& c    //�������ĵ�
    //##      FLOAT8 a           //��/2
    //##      FLOAT8 b           //��/2
    //##      int32  w           //�߿�
    //## ���أ�
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    uint8 inRect(const GPoint& c, FLOAT8 a, FLOAT8 b, int32 w) const;
    //##ModelId=436D655202EE
    //##Documentation
    //## ���ܣ�
    //##      ��õ��Ƿ��ھ�����
    //## ������
    //##      const GPoint& c    //�������ĵ�
    //##      FLOAT8 a           //��/2
    //##      FLOAT8 b           //��/2
    //##      int32  w           //�߿�
    //##      FLOAT8 r           //��Χ
    //## ���أ�
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
    uint8 onRect(const GPoint& c, FLOAT8 a, FLOAT8 b, int32 w, FLOAT8 r) const;

	//##ModelId=436D655202EE
    //##Documentation
    //## ���ܣ�
    //##      ��ָ���������Բ�����꣬����ֵΪ�뾶
    //## ������
    //##      const GPoint& pts
    //##      const GPoint& ptc
    //##      const GPoint& pte
    //## ���أ�
    //##      return �뾶
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
	FLOAT8  radius( const GPoint& pts , const GPoint& ptc ,const GPoint& pte);

	//##ModelId=436D655202EE
    //##Documentation
    //## ���ܣ�
    //##      ����ָ������ʼ����ֹ����
    //## ������
    //##      const GPoint& pts
    //##      const GPoint& ptc
    //##      const GPoint& pte
	//##      FLOAT8& as
	//##      FLOAT8& alen
    //## ���أ�
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
	uint8	anglese(const GPoint& pts , const GPoint& ptc ,const GPoint& pte,\
		FLOAT8& as , FLOAT8& alen );

	//##ModelId=436D655202EE
    //##Documentation
    //## ���ܣ�
    //##      ����ֱ����ָ����ˮƽ�ߵĽ���X
    //## ������
    //##      const GPoint& pts
    //##      const GPoint& pte
	//##      FLOAT8 y0
    //## ���أ�
    //##      return x����
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
	FLOAT8  lineX( const GPoint& pts , const GPoint& pte , FLOAT8 y0 );

	//##ModelId=436D655202EE
    //##Documentation
    //## ���ܣ�
    //##      ����ֱ����ָ����ˮƽ�ߵĽ���Y
    //## ������
    //##      const GPoint& pts
    //##      const GPoint& pte
	//##      FLOAT8 x0
    //## ���أ�
    //##      return y����
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
	FLOAT8  lineY( const GPoint& pts , const GPoint& pte , FLOAT8 x0 );

	//##ModelId=436D655202EE
    //##Documentation
    //## ���ܣ�
    //##      ������ֱ�ߵĽ���
    //## ������
    //##      const GPoint& ls1
    //##      const GPoint& le1
	//##      const GPoint& ls2
    //##      const GPoint& le2
    //## ���أ�
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
	uint8	intersect(const GPoint& ls1,const GPoint& le1,\
					const GPoint& ls2,const GPoint& le2);

	//##ModelId=436D655202EE
    //##Documentation
    //## ���ܣ�
    //##      ���߶��е�
    //## ������
    //##      const GPoint& pts
    //##      const GPoint& pte
    //## ���أ�
    //##      return �е�
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-5
	const GPoint& center(const GPoint& pts,const GPoint& pte);

    //##ModelId=4339FEDD0261
    FLOAT8 x;

    //##ModelId=4339FF0302CE
    FLOAT8 y;

};

class GDE_EXPORT_DEF GListPOINTS:public GList<GPoint>{};

#endif /* GPOINT_H_HEADER_INCLUDED_BCB8A4D3 */
