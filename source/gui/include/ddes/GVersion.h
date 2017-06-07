#ifndef GVERSION_H_HEADER_INCLUDED_BCACDF1F
#define GVERSION_H_HEADER_INCLUDED_BCACDF1F

#include <QtCore/QString>
#include <QtCore/QObject>
#include "ddef/ddes_comm.h"

class GFile;

//##ModelId=43530F370167
//##Documentation
//## ���壺
//##   ͼ�ΰ汾������
//## ���ܣ�
//##   ʵ��ͼ�ΰ汾������
//## ��ƣ�
//##   ������
//## ���ڣ�
//##   2005-10-17
class GDE_EXPORT_DES GVersion
{
  public:
    //##ModelId=43530FDF02AF
    //##Documentation
    //## ���ܣ�
    //##   �������汾��
    //## ������
    //##   uint8   h //���ֽں�
    //##   uint8   l//���ֽں�
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    void setMajor(uint8 h, uint8 l);
    //##ModelId=4353107D02BF
    //##Documentation
    //## ���ܣ�
    //##   ���ôΰ汾��
    //## ������
    //##   uint8   h //���ֽں�
    //##   uint8   l //���ֽں�
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    void setMinor(uint8 h, uint8 l);
    //##ModelId=435310BE03B9
    //##Documentation
    //## ���ܣ�
    //##   �Ƿ�ָ�����汾��
    //## ������
    //##   uint8   h //���ֽں�
    //##   uint8   l //���ֽں�
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    uint8 isMajor(uint8 h, uint8 l) const ;
    //##ModelId=4353111F02DE
    //##Documentation
    //## ���ܣ�
    //##   �Ƿ�ָ���ΰ汾��
    //## ������
    //##   uint8   h //���ֽں�
    //##   uint8   l //���ֽں�
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    uint8 isMinor(uint8 h, uint8 l) const ;
	//##ModelId=4353111F02DE
    //##Documentation
    //## ���ܣ�
    //##   �Ƿ�ָ�汾��
    //## ������
    //##   uint8   mh //���汾���ֽں�
    //##   uint8   ml //���汾���ֽں�
	//##   uint8   nh //�ΰ汾���ֽں�
    //##   uint8   nl //�ΰ汾���ֽں�
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    uint8 isVers(uint8 mh, uint8 ml,uint8 nh,uint8 nl) const ;
    //##ModelId=43531154000F
    //##Documentation
    //## ���ܣ�
    //##   ȡ���汾��
    //## ������
    //##   uint8&   h //���ֽں�
    //##   uint8&   l //���ֽں�
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    void getMajor(uint8& h, uint8& l);
    //##ModelId=4353122502DE
    //##Documentation
    //## ���ܣ�
    //##   ȡ�ΰ汾��
    //## ������
    //##   uint8&   h //���ֽں�
    //##   uint8&   l //���ֽں�
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    void getMinor(uint8& h, uint8& l);
    //##ModelId=4353125402FD
    //##Documentation
    //## ���ܣ�
    //##   �жϰ汾���Ƿ�һ��
    //## ������
    //##   const  GVersion& ver //�汾��
    //## ���أ�
    //##   if equal return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    uint8 operator==(const GVersion& ver) const;
    //##ModelId=4353125D03B9
    //##Documentation
    //## ���ܣ�
    //##   �жϰ汾���Ƿ�һ��
    //## ������
    //##   const  GVersion& ver //�汾��
    //## ���أ�
    //##   if equal return 0
    //##   else return 1
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    uint8 operator!=(const  GVersion& ver) const;
    //##ModelId=435312CF02FD
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
    //##   2005-10-17
    GVersion();
    //##ModelId=435312F701F4
    //##Documentation
    //## ���ܣ�
    //##   ���캯��
    //## ������
    //##   uint8 mh // major version h
    //##   uint8 ml // major version l
    //##   uint8 nh // minor version h 
    //##   uint8 nl // minor version l
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    GVersion(uint8 mh, uint8 ml, uint8 nh, uint8 nl);
    //##ModelId=4353139C00EA
    //##Documentation
    //## ���ܣ�
    //##   ȡ���汾�Ÿ��ֽ�
    //## ������
    //##   ��
    //## ���أ�
    //##   uint8
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    uint8 getMH() const ;
    //##ModelId=435313BD00BB
    //##Documentation
    //## ���ܣ�
    //##   ȡ���汾�ŵ��ֽ�
    //## ������
    //##   ��
    //## ���أ�
    //##   uint8
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    uint8 getML() const;
    //##ModelId=435313D00232
    //##Documentation
    //## ���ܣ�
    //##   ȡ�ΰ汾�Ÿ��ֽ�
    //## ������
    //##   ��
    //## ���أ�
    //##   uint8
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    uint8 getNH() const;
    //##ModelId=435313E00242
    //##Documentation
    //## ���ܣ�
    //##   ȡ�ΰ汾�ŵ��ֽ�
    //## ������
    //##   ��
    //## ���أ�
    //##   uint8
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    uint8 getNL() const;
    //##ModelId=43531437035B
    //##Documentation
    //## ���ܣ�
    //##      д�뺯��
    //## ������
    //##      GFile& fFile
    //## ���أ���int32��
    //##      if success return write real size
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-17
    int32 saveFile(GFile& fFile);


    //##ModelId=4353143902BF
    //##Documentation
    //## ���ܣ�
    //##      ��ȡ����
    //## ������
    //##      GFile& fFile
    //## ���أ���int32��
    //##      if success return read real size
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-17
    int32 loadFile(GFile& fFile);
    //##ModelId=435315CA01E4
    //##Documentation
    //## ���ܣ�
    //##   �жϰ汾���Ƿ�һ��
    //## ������
    //##   const GVersion& ver //�汾��
    //## ���أ�
    //##   const GVersion&
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    const GVersion& operator=(const GVersion& ver);
    //##ModelId=435315F30157
    //##Documentation
    //## ���ܣ�
    //##   ���캯��
    //## ������
    //##   onst GVersion& ver //�汾��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    GVersion(const GVersion& ver);
    //##ModelId=4353182702BF
    //##Documentation
    //## ���ܣ�
    //##   ȡ�汾��
    //## ������
    //##   ��
    //## ���أ�
    //##   uint32
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    uint32 getVers() const;

	//##ModelId=4353182702BF
    //##Documentation
    //## ���ܣ�
    //##   ���ð汾��
    //## ������
    //##   uint32 ver
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-17
    void setVers(uint32 ver);
	//##ModelId=438C096C0251
    //##Documentation
    //## ���ܣ�
    //##      ת������
    //## ������
    //##      ��
    //## ���أ�
    //##      �ַ���
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##      2005-11-29
    QString toString();

  protected:
    //##ModelId=43530F9A02CE
    //##Documentation
    //## ���汾�Ÿ��ֽ�
    uint8 mvh;
    //##ModelId=43530FC60157
    //##Documentation
    //## ���汾�ŵ��ֽ�
    uint8 mvl;
    //##ModelId=435311C1004E
    //##Documentation
    //## �ΰ汾���ֽ�
    uint8 nvh;
    //##ModelId=43531201031C
    //##Documentation
    //## �ΰ汾���ֽ�
    uint8 nvl;
  private:
};



#endif /* GVERSION_H_HEADER_INCLUDED_BCACDF1F */
