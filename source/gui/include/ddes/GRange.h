#ifndef GRANGE_H_HEADER_INCLUDED_BC93C755
#define GRANGE_H_HEADER_INCLUDED_BC93C755
#include "ddef/ddes_comm.h"
class GFile;

//##ModelId=436C3DB1004E
//##Documentation
//## ���壺
//##        ��Ұ��Χ
//## ���ܣ�
//##        ����ͼԪ��Ұ��Χ
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-5
class GDE_EXPORT_DES GRange
{
  public:
    //##ModelId=436C3E2401E4
    //##Documentation
    //## ���ܣ�
    //##   �ж��Ƿ��ڷ�Χ��
    //## ������
    //##   FLOAT8 fRange//�жϷ�Χ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    uint8 in(
        //##Documentation
        //## �Ƿ��ڷ�Χ��
        FLOAT8 fRange) const ;

    //##ModelId=436C3E5F0213
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
    GRange();

    //##ModelId=436C3E7C03D8
    //##Documentation
    //## ���ܣ�
    //##   ��ֵ����
    //## ������
    //##   const GRange&
    //## ���أ�
    //##   const GRange&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    const GRange& operator=(const GRange& copy);

    //##ModelId=436C3EC201C5
    //##Documentation
    //## ���ܣ�
    //##   ���캯��
    //## ������
    //##   FLOAT8 fMin
    //##   FLOAT8 fMax
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    void setRange(FLOAT8 fMin, FLOAT8 fMax);

    //##ModelId=436C3EF90271
    //##Documentation
    //## ���ܣ�
    //##   ȡ�����Χ����
    //## ������
    //##   ��
    //## ���أ�
    //##   FLOAT8
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    FLOAT8 getMax() const;

    //##ModelId=436C3EFE0271
    //##Documentation
    //## ���ܣ�
    //##   ȡ��С����
    //## ������
    //##   ��
    //## ���أ�
    //##   FLOAT8
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    FLOAT8 getMin() const;

    //##ModelId=436C3F940232
    //##Documentation
    //## ���ܣ�
    //##   �������캯��
    //## ������
    //##   const GRange&
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-11-5
    GRange(const GRange& copy);
    //##ModelId=436C40700148
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


    //##ModelId=436C407201C5
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

    //##ModelId=436C3DDE02EE
    //## �����Ұ��Χ
    //##Documentation
    //## �����Ұ��Χ
    FLOAT8 maxRange;

    //##ModelId=436C3E0100FA
    //##Documentation
    //## ��С��Ұ
    FLOAT8 minRange;

};



#endif /* GRANGE_H_HEADER_INCLUDED_BC93C755 */
