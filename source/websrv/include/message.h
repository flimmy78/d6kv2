

#if !defined(_MESSAGE_STRUCT)
#define _MESSAGE_STRUCT
#include "webconstant.h"
#include "ThreadStreamData.h"

//ͨ�ű��Ľṹ
typedef struct {
	int type;						//֡����
	int length;						//���ĳ���
	char  registerf;				//ע���־��1-->ע��;2-->������0-->��Ч��
	char  reserved[3];				//Ϊ�����ֽڶ���
	char  messagebuf[MAXBUFSIZE];	//���Ļ���������ĳɶ�̬����Ļ����ڴ�����ǳ�Ƶ����Ӱ��Ч��
	char *hugebuf;
}MESSAGE;


//ÿ�̵߳Ķ�������,����1������
struct MULTISTREAM
{
	int winno;//���ں�
	CThreadStreamData *pthreadstreamdata;
	struct MULTISTREAM *plist;
	MULTISTREAM()
	{
		pthreadstreamdata = NULL;
		plist = NULL;
	}
	~MULTISTREAM()
	{
		if(pthreadstreamdata) {delete pthreadstreamdata; pthreadstreamdata = NULL;}
		plist = NULL;
	}
};


#endif // !defined(_MESSAGE_STRUCT)