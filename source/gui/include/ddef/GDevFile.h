#ifndef GDEVFILE_H_HEADER_INCLUDED_BC8A1093
#define GDEVFILE_H_HEADER_INCLUDED_BC8A1093
#include <QtCore/QString>
#include <QtCore/QObject>
#include "ddes_comm.h"
#include "GFile.h"

//##ModelId=433FC90C00BB
//##Documentation
//## ���壺
//##   ͼ���ļ��洢��
//## ���ܣ�  
//##   ���ļ�������з�װ
//##   ���ڴ洢�����Ƹ�ʽ�ļ�
//##   �洢�ֽ���ΪLittleEndian
//##   ���ΪBigEndian�ģ�������Զ�ת��
//##   �ṩ��̬�ж��ֽ���ĺ���
//## ��ƣ�
//##   ������
//## ���ڣ�
//##   2005-10-2
class GDE_EXPORT_DEF GDevFile : public GFile
{
  public:
    //##ModelId=433FCF3F03B9
    //##Documentation
    //## ���ܣ�
    //##       ������ļ�
    //## ������
    //##      ��
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##    2005-10-2
    GDevFile();

    //##ModelId=433FCF45008C
    //##Documentation
    //## ���ܣ�
    //##       �����ļ�
    //## ������
    //##      const char* fName//�ļ����ƣ����԰���·��
    //##      uint32 uOpenMode //��ģʽ
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##    2005-10-2
    GDevFile(const char* fName, uint32 uOpenMode = GFile::modeRead|GFile::typeBinary);

    //##ModelId=433FD00F0000
    //##Documentation
    //## ���ܣ�
    //##      �����ļ�
    //## ������
    //##      int32  fDesc //�ļ�������
    //##      uint32 uOpenMode//�򿪷�ʽ
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##    2005-10-2
    GDevFile(int32 fDesc, uint32 uOpenMode = GFile::modeRead|GFile::typeBinary);

    //##ModelId=433FD11F0157
    //##Documentation
    //## ���ܣ�
    //##       �����ļ�
    //## ������
    //##      FILE* fFile//�Ѿ��򿪵��ļ�
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-2
    GDevFile(FILE* fFile);

    //##ModelId=433FD19702AF
    //##Documentation
    //## ���ܣ�
    //##       �����ļ�
    //## ������
    //##      int32 fDesc //�ļ�������
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##    2005-10-2
    GDevFile(int32 fDesc);

    //##ModelId=433FD1E402EE
    //##Documentation
    //## ���ܣ�
    //##    �����ļ�
    //## ������
    //##    ��
    //## ���أ�
    //##    ��
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-2
    virtual ~GDevFile();

    //##ModelId=433FD24C0213
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

    //##ModelId=433FD32C008C
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

    //##ModelId=433FD3B800FA
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

    //##ModelId=433FD4BB02FD
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

    //##ModelId=433FD567037A
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

    //##ModelId=433FD5C002BF
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

    //##ModelId=433FD60E006D
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
    virtual uint8 isEOF() const;

    //##ModelId=433FD614038A
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
    virtual uint8 isOpen() const;

    //##ModelId=433FD700033C
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
    virtual uint8 seekBegin();

    //##ModelId=433FD736029F
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
    virtual uint8 seekEnd();

    //##ModelId=433FD77002CE
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
    virtual int32 getLength();

    //##ModelId=4342485D03A9
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
    virtual uint8 setPos(int32 iPos, uint8 iFlag = GFile::begin);

    //##ModelId=433FD7B702EE
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
    virtual int32 getPos();

    //##ModelId=433FD871033C
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
    virtual char* readString(char* chBuffer, int32 chNum);

    //##ModelId=433FD9590157
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
    virtual int32 writeString(const char* chBuffer);

    //##ModelId=433FDAB201B5
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
    virtual int32 read(void* buffer, int32 size, int32 size_num = 1);

    //##ModelId=433FDABB0000
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
    virtual int32 write(void* buffer, int32 size, int32 size_num = 1);

    //##ModelId=434BD3E60049
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

    //##ModelId=434BD4150123
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

    //##ModelId=434BD42901DF
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

    //##ModelId=434BD46E01CF
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

    //##ModelId=434BD4810152
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

    //##ModelId=434BD4A503A4
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

    //##ModelId=435C615802CE
    //##Documentation
    //## ���ܣ�
    //##    ��ȡ�ļ�·������
    //## ������
    //##    ��
    //## ���أ�
    //##    �����ļ�����
    //## ��ƣ�
    //##    ������
    //## ���ڣ�
    //##    2005-10-24
    virtual const QString& getFileName() const;
	//##ModelId=437B21430222
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
    virtual void* getData();

  protected:
    //##ModelId=434BC9FB009B
    //##Documentation
    //## ���ܣ�
    //##   ��ʼ����Ա����
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-11
    virtual void init();

    //##ModelId=433FCE4702AF
    //##Documentation
    //## �ļ����
    FILE* m_fFile;

    //##ModelId=433FCE730000
    //##Documentation
    //## �ļ��򿪱�־����Ϊ1������Ϊ0
    uint8 m_uFileOpen;

    //##ModelId=433FCEDE0109
    //##Documentation
    //## ��ģʽ
    uint32 m_uOpenMode;

    //##ModelId=434BC95A0261
    //##Documentation
    //## ���ӱ�־�����������ӣ�����ʱ���ͷž��
    uint8 m_uAttach;

    //##ModelId=435C613603B9
    //##Documentation
    //## �ļ����ƣ�����Open�ļ�����
    QString m_fileName;

};

#endif /* GDEVFILE_H_HEADER_INCLUDED_BC8A1093 */
