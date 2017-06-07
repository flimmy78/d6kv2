#ifndef GFILE_H_HEADER_INCLUDED_BC8A4290
#define GFILE_H_HEADER_INCLUDED_BC8A4290
#include "ddes_comm.h"
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QByteArray>

class GRect;
class GPoint;

//##ModelId=4375A13B02AF
//##Documentation
//## ���壺
//##   ͼ���ļ��洢������
//## ���ܣ�  
//##   ���ļ�������ڴ������з�װ
//##   ���ڴ洢�����Ƹ�ʽ�ļ�
//##   �洢�ֽ���ΪLittleEndian
//##   ���ΪBigEndian�ģ�������Զ�ת��
//##   �ṩ��̬�ж��ֽ���ĺ���
//## ��ƣ�
//##   ������
//## ���ڣ�
//##   2005-10-2
class GDE_EXPORT_DEF GFile
{
  public:
    //##ModelId=4375A53D0399
    //##Documentation
    //## �����д������ʽ����
    enum  { 
        //##Documentation
        //## �����λ��Ѱַ
        begin = SEEK_SET, 
        //##Documentation
        //## ��Ե�ǰλ��Ѱַ
        current = SEEK_CUR, 
        //##Documentation
        //## ���βλ��Ѱַ
        end = SEEK_END };

    //##ModelId=4375A5960531
    //##Documentation
    //## �ļ��򿪷�ʽ
    enum  { 
        //##Documentation
        //## ֻ����ʽ
        modeRead = 0x01, 
        //##Documentation
        //## ֻд��ʽ
        modeWrite = 0x02, 
        //##Documentation
        //## ���ӷ�ʽ
        modeAppend = 0x04, 
        //##Documentation
        //## ��д��ʽ
        modeReadWrite = 0x08, 
        //##Documentation
        //## ����������modeReadWrite���ʹ��
        modeCreate = 0x10, 
        //##Documentation
        //## ����������modeReadWrite���ʹ��
        modeTruncate = 0x20 };

    //##ModelId=4375A5A70186
    //##Documentation
    //## �ļ�������
    enum  { 
        //##Documentation
        //## �ı���ʽ
        typeText = 0x40, 
        //##Documentation
        //## �����Ʒ�ʽ
        typeBinary = 0x80 };

    //##ModelId=433FD67A000F
    //##Documentation
    //## ���ܣ�
    //##      ��������ڴ��ֽ����Ƿ�ΪBigEndian
    //## ������
    //##      ��
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    static uint8 isBig();

    //##ModelId=434236C9036B
    //##Documentation
    //## ���ܣ�
    //##   �����ļ�����
    //## ������
    //##   const char* oldName //������
    //##   const char* newName //������
    //## ���أ���uint8��
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    static uint8 rename(const char* oldName, const char* newName);

    //##ModelId=4375A67400BB
    //##Documentation
    //## ���ܣ�
    //##   ���ļ�
    //## ������
    //##   const char* fName//�ļ����ƣ����԰���·��
    //##   uint32 uOpenMode //��ģʽ
    //## ���أ���uint8��
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    virtual uint8 open(const char* fName, uint32 uOpenMode = GFile::modeRead|GFile::typeBinary);

    //##ModelId=4375A6740157
    //##Documentation
    //## ���ܣ�
    //##   ���ļ�
    //## ������
    //##   int32 fDesc//�ļ�������
    //##   uint32 uOpenMode //��ģʽ
    //## ���أ���uint8��
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    virtual uint8 open(int32 fDesc, uint32 uOpenMode = GFile::modeRead|GFile::typeBinary);

    //##ModelId=4375A6740203
    //##Documentation
    //## ���ܣ�
    //##   �ر��ļ�
    //## ������
    //##   ��
    //## ���أ���uint8��
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    virtual uint8 close();

    //##ModelId=4375A67402AF
    //##Documentation
    //## ���ܣ�
    //##      �����ļ����
    //## ������
    //##      FILE* fFile //�ļ����
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 attach(FILE* fFile);

    //##ModelId=4375A674036B
    //##Documentation
    //## ���ܣ�
    //##      �����ļ�������
    //## ������
    //##      int32 fDesc //�ļ�������
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 attach(int32 fDesc);

    //##ModelId=4375A675005D
    //##Documentation
    //## ���ܣ�
    //##      �����ļ����
    //## ������
    //##      ��
    //## ���أ�
    //##      if success return m_fFile
    //##      else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual void* detach();

    //##ModelId=4375A6750128
    //##Documentation
    //## ���ܣ�
    //##      ����ļ��Ƿ��β
    //## ������
    //##      ��
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 isEOF() const = 0;

    //##ModelId=4375A67501F4
    //##Documentation
    //## ���ܣ�
    //##      ����ļ��Ƿ��
    //## ������
    //##      ��
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 isOpen() const = 0;

    //##ModelId=4375A67502DE
    //##Documentation
    //## ���ܣ�
    //##      �ƶ��ļ����ļ���ʼλ��
    //## ������
    //##      ��
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 seekBegin() = 0;

    //##ModelId=4375A67503A9
    //##Documentation
    //## ���ܣ�
    //##      �ƶ��ļ����ļ�ĩβλ��
    //## ������
    //##      ��
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 seekEnd() = 0;

    //##ModelId=4375A676009C
    //##Documentation
    //## ���ܣ�
    //##      �����ļ�����
    //## ������
    //##      ��
    //## ���أ���int32��
    //##      if success return filesize
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 getLength() = 0;

    //##ModelId=4375A6760167
    //##Documentation
    //## ���ܣ�
    //##   ���õ�ǰ�ļ�λ��
    //## ������ 
    //##   int32 iPos //λ��
    //##   uint8  iFlag//��־ 
    //## ���أ���uint8��
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    virtual uint8 setPos(int32 iPos, uint8 iFlag = GFile::begin) = 0;

    //##ModelId=4375A6760271
    //##Documentation
    //## ���ܣ�
    //##      ȡ���ļ���ǰλ��
    //## ������
    //##      ��
    //## ���أ���int32��
    //##      if success return curr position
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 getPos() = 0;

    //##ModelId=4375A676033C
    //##Documentation
    //## ���ܣ�
    //##   ���浱ǰ�ļ���ȡλ��
    //## ������
    //##   ��
    //## ���أ���int32��
    //##   if success return file position
    //##   else return -1
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    virtual int32 save();

    //##ModelId=4375A677002E
    //##Documentation
    //## ���ܣ�
    //##   �ָ��ļ���¼λ��
    //## ������
    //##   int32 savePos
    //## ���أ���uint8��
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-4
    virtual uint8 restore(int32 savePos);

    //##ModelId=4375A6770109
    //##Documentation
    //## ���ܣ�
    //##     ��ȡ�ļ��ַ���
    //## ������
    //##      char* chBuffer//�����ַ���
    //##      int32 chNum   //�ַ�������
    //## ���أ���int32��
    //##      if success return ʵ�ʶ�ȡ��
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual char* readString(char* chBuffer, int32 chNum) = 0;

    //##ModelId=4375A67701D4
    //##Documentation
    //## ���ܣ�
    //##      д���ַ���
    //## ������
    //##      const char* chBuffer//�ַ���������
    //## ���أ���int32��
    //##      if success return real write size
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 writeString(const char* chBuffer) = 0;

    //##ModelId=4375A677029F
    //##Documentation
    //## ���ܣ�
    //##      �����Ʒֿ��ȡ�ڴ�����
    //## ������
    //##      void* buffer //��ȡ������
    //##      int32 size   //ÿ��ߴ�
    //##      int32 size_num//����
    //## ���أ���int32��
    //##      if success return ʵ�ʶ������
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 read(void* buffer, int32 size, int32 size_num = 1) = 0;

    //##ModelId=4375A677037A
    //##Documentation
    //## ���ܣ�
    //##      �����Ʒֿ�д���ڴ�����
    //## ������
    //##      void* buffer //д�뻺����
    //##      int32 size   //ÿ��ߴ�
    //##      int32 size_num//����
    //## ���أ���int32��
    //##      if success return ʵ��д�����
    //##      else return -1
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual int32 write(void* buffer, int32 size, int32 size_num = 1) = 0;

    //##ModelId=4375A678006D
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      uint8 v 
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(uint8 v);

    //##ModelId=4375A6780148
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      char v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(char v);

    //##ModelId=4375A6780222
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      uint16 v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(uint16 v);

    //##ModelId=4375A67802FD
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      int16 v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(int16 v);

    //##ModelId=4375A67803D8
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      uint32 v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(uint32 v);

    //##ModelId=4375A67900CB
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      int32 v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(int32 v);

    //##ModelId=4375A67901B5
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      uint64 v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(uint64 v);

    //##ModelId=4375A6790290
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      int64 v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(int64 v);

    //##ModelId=4375A679036B
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      FLOAT4 v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(FLOAT4 v);

    //##ModelId=4375A67A006D
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      FLOAT8 v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(FLOAT8 v);

    //##ModelId=4375A67A0157
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##       const GPoint& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(const GPoint& v);

    //##ModelId=4375A67A0232
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      const GRect& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(const GRect& v);

    //##ModelId=4375A67A030D
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      uint8& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(uint8& v);

    //##ModelId=4375A67B000F
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      char& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(char& v);

    //##ModelId=4375A67B00EA
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      uint16& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(uint16& v);

    //##ModelId=4375A67B01D4
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      int16& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(int16& v);

    //##ModelId=4375A67B02BF
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      uint32& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(uint32& v);

    //##ModelId=4375A67B03A9
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      int32& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(int32& v);

    //##ModelId=4375A67C00AB
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      uint64& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(uint64& v);

    //##ModelId=4375A67C0196
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      int64& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(int64& v);

    //##ModelId=4375A67C0271
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      FLOAT4& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(FLOAT4& v);

    //##ModelId=4375A67C036B
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      FLOAT8& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(FLOAT8& v);

    //##ModelId=4375A67D005D
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      GPoint& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(GPoint& v);

    //##ModelId=4375A67D0157
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      GRect& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(GRect& v);

    //##ModelId=4375A67D0242
    //##Documentation
    //## ���ܣ�
    //##      ��⵱ǰ�ļ��Ƿ���Զ�
    //## ������
    //##      ��
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 isRead();

    //##ModelId=4375A67D032C
    //##Documentation
    //## ���ܣ�
    //##      ��⵱ǰ�ļ��Ƿ����д
    //## ������
    //##      ��
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 isWrite();

    //##ModelId=4375A67E003E
    //##Documentation
    //## ���ܣ�
    //##      ��⵱ǰ�ļ�Ϊ����
    //## ������
    //##      ��
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 isAttach();

    //##ModelId=4375A67E0128
    //##Documentation
    //## ���ܣ�
    //##      ��⵱ǰ�ļ��Ƿ���Զ�д
    //## ������
    //##      ��
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 isReadWrite();

    //##ModelId=4375A67E0222
    //##Documentation
    //## ���ܣ�
    //##      ��⵱ǰ�ļ�ģʽ�Ƿ�Ϊ�ı���ʽ
    //## ������
    //##      ��
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-12
    virtual uint8 isText();

    //##ModelId=4375A67E030D
    //##Documentation
    //## ���ܣ�
    //##      ��⵱ǰ�ļ�ģʽ�Ƿ�Ϊ�����Ʒ�ʽ
    //## ������
    //##      ��
    //## ���أ���uint8��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-12
    virtual uint8 isBinary();

    //##ModelId=4375A67F001F
    //##Documentation
    //## ���ܣ�
    //##      ���ض�ȡ����
    //## ������
    //##      QString& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator>>(QString& v);

    //##ModelId=4375A69E0148
    //##Documentation
    //## ���ܣ�
    //##      ����д�뺯��
    //## ������
    //##      const QString& v
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    GFile& operator<<(const QString& v);
    //##ModelId=4375A8B902DE
    //##Documentation
    //## ���ܣ�
    //##      �������ݻ��壬�û������û��Լ��ͷ�
    //## ������
    //##      nt8 *dataBuffer;
    //##      uint32 dataSize ;
    //##      uint32 growSize//�����ߴ�
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##     2005-11-11
    virtual uint8 attach(char* dataBuffer, uint32 dataSize, uint32 growSize);
	//##ModelId=4375BF66035B
    //##Documentation
    //## ���ܣ�
    //##      дֵ���ļ�
    //## ������
    //##      void* value
    //##      int32 size
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-12
    GFile& writeV2F(void* value, int32 size);
    //##ModelId=4375BF7201D4
    //##Documentation
    //## ���ܣ�
    //##      ��ֵ���ļ�
    //## ������
    //##      void* value
    //##      int32 size
    //## ���أ���GFile&��
    //##      
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-12
    GFile& readF2V(void* value, int32 size);
	//##ModelId=437B202E02DE
    //##Documentation
    //## ���ܣ�
    //##      ȡ���ڲ����ݣ������Ͽ�����
    //##      ����Ǹö���򿪻�������ڴ�
    //##      �ö�����Զ��ͷţ���һ���detach��ͬ
    //##      detach�󣬸ö���ʧȥ��ϵ�����Զ���
    //##      �ڲ����ݽ����ͷţ���Ҫ�û��Լ��ͷ�
    //##      �������ڴ�й©
    //## ������
    //##      ��
    //## ���أ�
    //##      void*
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-12
    virtual void* getData() = 0;

};

#endif /* GFILE_H_HEADER_INCLUDED_BC8A4290 */
