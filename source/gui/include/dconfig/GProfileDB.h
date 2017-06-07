#ifndef _GPROFILEDB_H
#define _GPROFILEDB_H

#include "GProfile.h"

class Ddbi;

//##ModelId=43422ED200CB
//##Documentation
//## ���壺
//##   �������ݿ��д
//## ���ܣ�
//##   ����ϵͳ�������ݿ��д����
//##   �������ݿ�������¸�ʽ���д洢
//##   #------ע�����
//##   func_block1 keyword1 value
//##   func_block1 keyword2 value
//##   func_block2 keyword1 value
//##   func_block2 keyword2 value
//## ��ƣ�
//##   ������
//## ���ڣ�
//##   2005-10-06
class GDE_EXPORT_CONF GProfileDB:public GProfile
{
public:
    //##ModelId=434E050B03A7
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
    //##   2005-10-13
    GProfileDB(Ddbi* dbi);    
	  //##ModelId=434E0502029D
    //##Documentation
    //## ���ܣ�
    //##   ��������
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    ~GProfileDB();
		//##ModelId=434E0AE102FA
    //##Documentation
    //## ���ܣ�
    //##   ���������ļ�
    //## ������
    //##   const char* profile //�����ļ�
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    //##  
    virtual uint8 create(const char* profile);
    //##ModelId=434E0AE102FA
    //##Documentation
    //## ���ܣ�
    //##   �������ļ�
    //## ������
    //##   const char* profile //�����ļ�
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-13
    //##  
    virtual uint8 open(const char* profile);
	//##ModelId=434E0D8B029B
    //##Documentation
    //## ���ܣ�
    //##   �ر������ļ�
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
		virtual void  close();
    //##ModelId=434E0D8B029B
    //##Documentation
    //## ���ܣ�
    //##   �����ļ��Ƿ����ʹ��
    //## ������
    //##   ��
    //## ���أ�
    //##   ��
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-3
    virtual uint8 isValid();
		//##ModelId=4345C41C0177
    //##Documentation
    //## ���ܣ�
    //##   ���ע��
    //## ������
    //##   const char* comment //ע��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    //##   
	virtual uint8 addComment(const char* comment);
    //##ModelId=4345C41C0177
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�����ļ�
    //## ������
    //##   const char* entry //��ڶ���
    //##   const char* key   //�ؼ���
    //##   char* value       //ֵ
    //##   const char* def   //ȱʡֵ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    //##   
    virtual uint8 getString(const char* entry, const char* key, char* value, const char* def = NULL);

    //##ModelId=4345C42502FD
    //##Documentation
    //## ���ܣ�
    //##   д�������ļ�
    //## ������
    //##   const char* entry //��ڶ���
    //##   const char* key   //�ؼ���
    //##   char* value       //ֵ
    //##   const char* def   //ȱʡֵ
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    virtual uint8 writeString(const char* entry, const char* key, const char* value);
    //##ModelId=4345C59D02CE
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�����ļ���������
    //## ������
    //##   const char* entry //��ڶ��� 
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    virtual uint8 readBlock(const char* entry);
	//##ModelId=4345C59D02CE
    //##Documentation
    //## ���ܣ�
    //##   ��ȡ�����ļ����ж�
    //## ������
    //##   ��
    //## ���أ�
    //##   if success return 1
    //##   else return 0
    //## ��ƣ�
    //##   ������
    //## ���ڣ�
    //##   2005-10-7
    virtual uint8 readEntries();
    
protected:
		//##ModelId=43508BB70157
    //##Documentation
    //##���ݿ���ʲ��
		Ddbi*		m_ddbi;
};

#endif
