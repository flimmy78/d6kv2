#ifndef GMEMFILE_H_HEADER_INCLUDED_BC8A5FAC
#define GMEMFILE_H_HEADER_INCLUDED_BC8A5FAC
#include "ddes_comm.h"
#include "GFile.h"
class GString;

//##ModelId=437571BD00CB
//##Documentation
//## ���壺
//##   ͼ���ڴ�洢��
//## ���ܣ�  
//##   ���ڴ������з�װ
//##   ���ڴ洢�����Ƹ�ʽ�ļ�
//##   �洢�ֽ���ΪLittleEndian
//##   ���ΪBigEndian�ģ�������Զ�ת��
//##   �ṩ��̬�ж��ֽ���ĺ���
//## ��ƣ�
//##   ������
//## ���ڣ�
//##   2005-10-2
class GDE_EXPORT_DEF GMemFile : public GFile
{
  public:
    //##ModelId=43757309036B
    //##Documentation
    //## ���ܣ�
    //##      �ڴ��ļ�����
    //## ������
    //##      uint32 growSize//�����ߴ�
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##     2005-11-11
    GMemFile(uint32 growSize = 1024);

    //##ModelId=437573730157
    //##Documentation
    //## ���ܣ�
    //##      �ڴ��ļ�����
    //## ������
    //##      char *dataBuffer;
    //##      uint32 dataSize ;
    //##      uint32 growSize ;//�����ߴ�
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##     2005-11-11
    GMemFile(char* dataBuffer, uint32 dataSize, uint32 growSize = 0);
	//##ModelId=437575390109
    //##Documentation
    //## ���ܣ�
    //##      �ڴ��ļ�����
    //## ������
    //##      ��
    //## ���أ�
    //##      ��
    //## ��ƣ�
    //##      ������
    //## ���ڣ�
    //##     2005-11-11
    virtual ~GMemFile();


    //##ModelId=437573E60290
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
    uint8 attach(char* dataBuffer, uint32 dataSize, uint32 growSize);
	 //##ModelId=4379C8F80232
    //##Documentation
    //## ���ܣ�
    //##      ���ػ�������
    //## ������
    //##      ��
    //## ���أ�
    //##      if success return databuf
    //##      else return NULL
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-11-15
    void* detach();
	//##ModelId=437B211A037A
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

    
    //##ModelId=43757636032C
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

    //##ModelId=4375763E0119
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

    //##ModelId=4375764502AF
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

    //##ModelId=4375764B0271
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

    //##ModelId=437576630138
    //##Documentation
    //## ���ܣ�
    //##   ���õ�ǰ�ļ�λ��
    //## ������ 
    //##   int32 iPos //λ��
    //##   char  iFlag//��־ 
    //## ���أ���char��
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    virtual uint8 setPos(int32 iPos, uint8 iFlag = GFile::begin);

    //##ModelId=4375766E0251
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

    //##ModelId=4375767C01A5
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

    //##ModelId=4375768801B5
    //##Documentation
    //## ���ܣ�
    //##      �ƶ��ļ����ļ�ĩβλ��
    //## ������
    //##      ��
    //## ���أ���char��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 seekEnd();

    //##ModelId=4375768E0261
    //##Documentation
    //## ���ܣ�
    //##      �ƶ��ļ����ļ���ʼλ��
    //## ������
    //##      ��
    //## ���أ���char��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 seekBegin();

    //##ModelId=4375769502EE
    //##Documentation
    //## ���ܣ�
    //##      ����ļ��Ƿ��
    //## ������
    //##      ��
    //## ���أ���char��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 isOpen() const;

    //##ModelId=437576A5033C
    //##Documentation
    //## ���ܣ�
    //##      ����ļ��Ƿ��β
    //## ������
    //##      ��
    //## ���أ���char��
    //##      if success return 1
    //##      else return 0
    //## ��ƣ�
    //##       ������
    //## ���ڣ�
    //##       2005-10-2
    virtual uint8 isEOF() const;

    //##ModelId=437576AF02BF
    //##Documentation
    //## ���ܣ�
    //##   �ر��ļ�
    //## ������
    //##   ��
    //## ���أ���char��
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    virtual uint8 close();



  protected:
    //##ModelId=437575340000
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

    //##ModelId=43758DCE0290
    //##Documentation
    //## ���ܣ�
    //##   �����ڴ�
    //## ������
    //##   uint32 size
    //## ���أ�
    //##   if success return char*
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    char* gmalloc(uint32 size);

    //##ModelId=43758E210242
    //##Documentation
    //## ���ܣ�
    //##   �����ڴ�
    //## ������
    //##   char*  ptr
    //##   uint32 size
    //## ���أ�
    //##   if success return char*
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    char* grealloc(char* ptr, uint32 size);

    //##ModelId=43758E550148
    //##Documentation
    //## ���ܣ�
    //##   ��ֵ�ڴ��
    //## ������
    //##   char*  memDst
    //##   const char*  memSrc
    //##   uint32 size
    //## ���أ�
    //##   if success return char*
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    void gmemcpy(char* memDst, const char* memSrc, uint32 size);

    //##ModelId=43758F1D01C5
    //##Documentation
    //## ���ܣ�
    //##   �Ƿ��ڴ�
    //## ������
    //##   char* ptr
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    void gfree(char* ptr);

    //##ModelId=43758F7A0290
    //##Documentation
    //## ���ܣ�
    //##   �����ļ�
    //## ������
    //##   uint32 newsize
    //## ���أ�
    //##   if  success return 1
    //##   else return 0;
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    uint8 growFile(uint32 newsize);

    //##ModelId=43758FD4034B
    //##Documentation
    //## ���ܣ�
    //##   ����ڴ�Ϊ��
    //## ������
    //##   char*  ptr
    //##   uint32 size
    //## ���أ�
    //##   if success return char*
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    void gmemset(char* ptr, uint32 size);

    //##ModelId=437593200138
    //##Documentation
    //## ���ܣ�
    //##   ��ȫȡ�ַ�������
    //## ������
    //##   const char*  ptr
    //## ���أ�
    //##   if success return length
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    uint32 gstrlen(const char* ptr);

    //##ModelId=437597A8036B
    //##Documentation
    //## ���ܣ�
    //##   �ж��Ƿ���������ļ�
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-2
    uint8 canGrow();

    //##ModelId=437572220177
    //##Documentation
    //## ���ݻ�����
    char* m_hData;

    //##ModelId=4375725F02BF
    //##Documentation
    //## �����Զ������ߴ�
    uint32 m_growSize;

    //##ModelId=437572F10177
    //##Documentation
    //## ��ǰ��дλ��ָ��
    uint32 m_curPos;

    //##ModelId=437574C7037A
    //##Documentation
    //## �ڴ����ݳ���
    uint32 m_dataLength;

    //##ModelId=4375729E0000
    //##Documentation
    //## ���ݻ�����󳤶�
    uint32 m_maxLength;

    //##ModelId=4375756002AF
    //##Documentation
    //## �Ƿ������ⲿ����
    uint8 m_uAttach;

};

#endif /* GMEMFILE_H_HEADER_INCLUDED_BC8A5FAC */
