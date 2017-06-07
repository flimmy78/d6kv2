#ifndef	SYS_STR_H
#define	SYS_STR_H

#include "sysdef.h"
//�������Ͷ���
#ifdef __unix
	#ifndef BOOL
	#define BOOL	unsigned char 
	#endif
#endif
//2�ֽ�
typedef union
{
	unsigned short int val1;
	struct
	{
		unsigned char b1;
		unsigned char b2;
	}val2;
}BYTE2;


//4�ֽ�
typedef union
{
	unsigned int val1;
	struct
	{
		unsigned char b1;
		unsigned char b2;
		unsigned char b3;
		unsigned char b4;
	}val2;
}BYTE4;


//8�ֽ�
typedef union
{
#if defined(WIN32)
	unsigned __int64 val1;
#elif defined(__linux)
	unsigned long long val1;
#elif defined(__unix)
#if	defined(__alpha)
	unsigned long val1;
#elif (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
	unsigned long long val1;
#elif (defined(__sun) && defined(__i386))
	unsigned long long val1;
#endif
#endif
	struct
	{
		unsigned char b1;
		unsigned char b2;
		unsigned char b3;
		unsigned char b4;
		unsigned char b5;
		unsigned char b6;
		unsigned char b7;
		unsigned char b8;
	}val2;
}BYTE8;


//�����ȷ���
typedef union
{
	float val1;
	struct
	{
		unsigned char b1;
		unsigned char b2;
		unsigned char b3;
		unsigned char b4;
	}val2;
}FLOATS;


//˫���ȷ���
typedef union
{
	double val1;
	struct
	{
		unsigned char b1;
		unsigned char b2;
		unsigned char b3;
		unsigned char b4;
		unsigned char b5;
		unsigned char b6;
		unsigned char b7;
		unsigned char b8;
	}val2;
}FLOATD;


//8λλ�򣬲��ã�AIX support int:1 only, so int8bit is not used after version v3.0��
struct int8bit
{
#ifdef _AIX
	unsigned int bit7:1;
	unsigned int bit6:1;
	unsigned int bit5:1;
	unsigned int bit4:1;
	unsigned int bit3:1;
	unsigned int bit2:1;
	unsigned int bit1:1;
	unsigned int bit0:1;

#elif (defined(__sun) && defined(__sparc) || defined(__hpux))
	unsigned char bit7:1;
	unsigned char bit6:1;
	unsigned char bit5:1;
	unsigned char bit4:1;
	unsigned char bit3:1;
	unsigned char bit2:1;
	unsigned char bit1:1;
	unsigned char bit0:1;
#else
	unsigned char bit0:1;
	unsigned char bit1:1;
	unsigned char bit2:1;
	unsigned char bit3:1;
	unsigned char bit4:1;
	unsigned char bit5:1;
	unsigned char bit6:1;
	unsigned char bit7:1;
#endif
};


//8λ�޷�������
struct int8u
{
	unsigned char byte;
	int8u(void) {}
	int8u(unsigned char b) { byte = b; }
	void operator = (unsigned char n) { byte = n; }
	operator unsigned char () { return byte; }
};


//8λ�з�������
struct int8s
{
	char byte;
	int8s(void) {}
	int8s(char b) { byte = b; }
	void operator = (char n) { byte = n; }
	operator char () { return byte; }
};


//16λ�޷�������
struct int16u
{
	BYTE2 word;
	void operator = (unsigned short int n)
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE2 one;
		one.val1 = n;
		word.val2.b1 = one.val2.b2;
		word.val2.b2 = one.val2.b1;
#else
		word.val1 = n;
#endif
	}

	operator unsigned short int ()
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE2 one;
		one.val2.b1 = word.val2.b2;
		one.val2.b2 = word.val2.b1;
		return one.val1;
#else
		return word.val1;
#endif
	}
};


//16λ�з�������
struct int16s
{
	BYTE2 word;
	void operator = (short int n)
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE2 two;
		two.val1 = n;
		word.val2.b1 = two.val2.b2;
		word.val2.b2 = two.val2.b1;
#else
		word.val1 = n;
#endif
	}
	operator short int () 
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE2 two;
		two.val2.b1 = word.val2.b2;
		two.val2.b2 = word.val2.b1;
		return two.val1;
#else
		return word.val1;
#endif
	}
};


//32λ�޷�������
struct int32u
{
	BYTE4 dword;
	void operator = (unsigned int n)
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE4 two;
		two.val1 = n;
		dword.val2.b1 = two.val2.b4;
		dword.val2.b2 = two.val2.b3;
		dword.val2.b3 = two.val2.b2;
		dword.val2.b4 = two.val2.b1;
#else
		dword.val1 = n; 
#endif
	}
	operator unsigned int ()
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE4 two;
		two.val2.b1 = dword.val2.b4;
		two.val2.b2 = dword.val2.b3;
		two.val2.b3 = dword.val2.b2;
		two.val2.b4 = dword.val2.b1;
		return two.val1;
#else
		return dword.val1; 
#endif
	}
};


//32λ�з�������
struct int32s
{
	BYTE4 dword;
	int32s(int n) 
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE4 two;
		two.val1 = n;
		dword.val2.b1 = two.val2.b4;
		dword.val2.b2 = two.val2.b3;
		dword.val2.b3 = two.val2.b2;
		dword.val2.b4 = two.val2.b1;
#else
		dword.val1 = n; 
#endif
	}
	int32s(void) {}
	void operator = (int n) 
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE4 two;
		two.val1 = n;
		dword.val2.b1 = two.val2.b4;
		dword.val2.b2 = two.val2.b3;
		dword.val2.b3 = two.val2.b2;
		dword.val2.b4 = two.val2.b1;
#else
		dword.val1 = n; 
#endif
	}
	operator int ()
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE4 two;
		two.val2.b1 = dword.val2.b4;
		two.val2.b2 = dword.val2.b3;
		two.val2.b3 = dword.val2.b2;
		two.val2.b4 = dword.val2.b1;
		return two.val1;
#else
		return dword.val1; 
#endif
	}
};


//64λ�޷�������
struct int64u
{
	BYTE8 ddword;
#if defined(WIN32)
	void operator = (unsigned __int64 n) { ddword.val1 = n; }
	operator unsigned __int64 () { return ddword.val1; }
#elif defined(__linux)
	void operator = (unsigned long long n) { ddword.val1 = n; }
	operator unsigned long long () { return ddword.val1; }
#elif defined(__unix)
#if defined(__alpha)
	void operator = (unsigned long n) { ddword.val1 = n; }
	operator unsigned long () { return ddword.val1; }
#elif (defined(__sun) && defined(__i386))
	void operator = (unsigned long long n) { ddword.val1 = n; }
	operator unsigned long long () { return ddword.val1; }
#elif (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
	void operator = (unsigned long long n)
	{
		BYTE8 one;
		one.val1 = n;
		ddword.val2.b1 = one.val2.b8;
		ddword.val2.b2 = one.val2.b7;
		ddword.val2.b3 = one.val2.b6;
		ddword.val2.b4 = one.val2.b5;
		ddword.val2.b5 = one.val2.b4;
		ddword.val2.b6 = one.val2.b3;
		ddword.val2.b7 = one.val2.b2;
		ddword.val2.b8 = one.val2.b1;
	}
	operator unsigned long long()
	{
		BYTE8 one;
		one.val2.b1 = ddword.val2.b8;
		one.val2.b2 = ddword.val2.b7;
		one.val2.b3 = ddword.val2.b6;
		one.val2.b4 = ddword.val2.b5;
		one.val2.b5 = ddword.val2.b4;
		one.val2.b6 = ddword.val2.b3;
		one.val2.b7 = ddword.val2.b2;
		one.val2.b8 = ddword.val2.b1;
		return one.val1;
	}
#endif
#endif
};


//64λ�з�������
struct int64s
{
	BYTE8 ddword;
#if defined(WIN32)
	void operator = (__int64 n) { ddword.val1 = n; }
	operator __int64 () { return ddword.val1; }
#elif defined(__linux)
	void operator = (long long n) { ddword.val1 = n; }
	operator long long () { return ddword.val1; }
#elif defined(__unix)
#if defined(__alpha)
	void operator = (long n) { ddword.val1 = n; }
	operator long () { return ddword.val1; }
#elif (defined(__sun) && defined(__i386))
	void operator = (long long n) { ddword.val1 = n; }
	operator long long () { return ddword.val1; }
#elif (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
	void operator = (long long n)
	{
		BYTE8 one;
		one.val1 = n;
		ddword.val2.b1 = one.val2.b8;
		ddword.val2.b2 = one.val2.b7;
		ddword.val2.b3 = one.val2.b6;
		ddword.val2.b4 = one.val2.b5;
		ddword.val2.b5 = one.val2.b4;
		ddword.val2.b6 = one.val2.b3;
		ddword.val2.b7 = one.val2.b2;
		ddword.val2.b8 = one.val2.b1;
	}
	operator long long ()
	{
		BYTE8 one;
		one.val2.b1 = ddword.val2.b8;
		one.val2.b2 = ddword.val2.b7;
		one.val2.b3 = ddword.val2.b6;
		one.val2.b4 = ddword.val2.b5;
		one.val2.b5 = ddword.val2.b4;
		one.val2.b6 = ddword.val2.b3;
		one.val2.b7 = ddword.val2.b2;
		one.val2.b8 = ddword.val2.b1;
		return one.val1;
	}
#endif
#endif
};


//�����ȸ�����
struct floats
{
	FLOATS value;
	void operator = (float n)
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		FLOATS val;
		val.val1 = n;
		value.val2.b1 = val.val2.b4;
		value.val2.b2 = val.val2.b3;
		value.val2.b3 = val.val2.b2;
		value.val2.b4 = val.val2.b1;
#else
		value.val1 = n; 
#endif
	}
	operator float ()
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		FLOATS val;
		val.val2.b1 = value.val2.b4;
		val.val2.b2 = value.val2.b3;
		val.val2.b3 = value.val2.b2;
		val.val2.b4 = value.val2.b1;
		return val.val1;
#else
		return value.val1;
#endif
	}
};


//˫���ȸ�����
struct floatd
{
	FLOATD value;
	void operator = (double n)
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		FLOATD val;
		val.val1 = n;
		value.val2.b1 = val.val2.b8;
		value.val2.b2 = val.val2.b7;
		value.val2.b3 = val.val2.b6;
		value.val2.b4 = val.val2.b5;
		value.val2.b5 = val.val2.b4;
		value.val2.b6 = val.val2.b3;
		value.val2.b7 = val.val2.b2;
		value.val2.b8 = val.val2.b1;
#else
		value.val1 = n;
#endif
	}
	operator double () 
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		FLOATD val;
		val.val2.b1 = value.val2.b8;
		val.val2.b2 = value.val2.b7;
		val.val2.b3 = value.val2.b6;
		val.val2.b4 = value.val2.b5;
		val.val2.b5 = value.val2.b4;
		val.val2.b6 = value.val2.b3;
		val.val2.b7 = value.val2.b2;
		val.val2.b8 = value.val2.b1;
		return val.val1;
#else
		return value.val1;
#endif
	}
};

typedef struct
{
	int8u  grpno;					//0���ֺ���
	int16u sort;					//1���¼������
    int16u type;					//2���¼�����
	int32u ymd;						//3��������
	int32u hmsms;					//4��ʱ�������
	int8u security;					//5����ȫ��
	int8u volgrade;					//6	��ѹ�ȼ�
	int8u  testflag;				//7	ʵ���־
//	int8bit output;					//8�������
	unsigned char PrintOut;
	unsigned char DiskOut;
	unsigned char ToneOut;
	unsigned char GraphOut;
	unsigned char DispOut;
	int16u state;
	int16u state1;
	int16u state2;
	floats val;						//9.��ǰ����ֵ
    floats val1;
    floats val2;
	floats val3;
	char Object[SHORTNAME_LEN];		//13.�¼���������
	char member0[SHORTNAME_LEN];	//14.�¼�������0
	char member1[SHORTNAME_LEN];	//15.�¼�������1
	char member2[SHORTNAME_LEN];	//16.�¼�������2
	char member3[SHORTNAME_LEN];	//17.�¼�������3
	char graph[GRAP_LEN];			//18����ͼ��
	char tone[FULLDESCRIBE_LEN];	//19������������Ϣ
	char text[FULLDESCRIBE_LEN];	//20������������Ϣ
}SYS_EVENT;

typedef struct {
	SYS_EVENT   evt;
	char value_des[EVENTDESCRIBE_LEN];
	char group_des[EVENTDESCRIBE_LEN];
	char member_des0[EVENTDESCRIBE_LEN];
	char member_des1[EVENTDESCRIBE_LEN];
	char member_des2[EVENTDESCRIBE_LEN];
	char member_des3[EVENTDESCRIBE_LEN];
	char member_des4[EVENTDESCRIBE_LEN];
	char member_des5[EVENTDESCRIBE_LEN];
	char otherdes0[EVENTDESCRIBE_LEN];
	char otherdes1[EVENTDESCRIBE_LEN];
	char otherdes2[EVENTDESCRIBE_LEN];
	char otherdes3[EVENTDESCRIBE_LEN];
	char otherdes4[EVENTDESCRIBE_LEN];
	char otherdes5[EVENTDESCRIBE_LEN];
	char longdes[120];
}SYS_EVENT_EX;

typedef struct
{
	char	flag;						//������־
	int     count;
	SYS_EVENT sys_event;
}EVENT;

//�浽��ʷ���ݿ��е��¼��ṹ
typedef struct
{
	short int grpno;			//0���ֺ���
	short int sort;				//1���¼������
    short int type;				//2���¼�����
	int ymd;					//3��������
	int hmsms;					//4��ʱ�������
	unsigned char security;		//5����ȫ��
	unsigned char volgrade;		//6	��ѹ�ȼ�
	unsigned char testflag;		//7	ʵ���־
	unsigned char output;		//8�������
	int16u state;
	int16u state1;
	int16u state2;
	float val;					//9.��ǰ����ֵ
	float val1;
	float val2;
	float val3;        
	char Object[SHORTNAME_LEN];			//13.�¼���������
	char member0[SHORTNAME_LEN];		//14.�¼�������0
	char member1[SHORTNAME_LEN];		//15.�¼�������1
	char member2[SHORTNAME_LEN];		//16.�¼�������2
	char member3[SHORTNAME_LEN];		//17.�¼�������3
	char text[FULLDESCRIBE_LEN];		//18������������Ϣ
}HIS_EVENT;


#endif		//all of end
