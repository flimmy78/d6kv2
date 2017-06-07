#ifndef LICENSE_GEN_DEF_H
#define LICENSE_GEN_DEF_H

#define _Q2C( str ) ( ( const char * )( ( str ).toLocal8Bit(  ).data(  ) ) ) 
#define _C2Q( str ) ( QString::fromLocal8Bit( str ) )

enum
{
	E_THREE_DAY,				// 3��
	E_ONE_MONTH,				// 1����
	E_THREE_MONTH,        // 3����
	E_HALF_YEAR,				// ����
	E_ONE_YEAR,					// 1��
	E_TWO_YEAR,				// 2��
	E_UNLIMITED					// ����
}TIME_LEVEL;

// AES ���ܽ����㷨key
unsigned char aes_key[] = \
{ \
0x2b, 0x7e, 0x15, 0x16, \
0x28, 0xae, 0xd2, 0xa6, \
0xab, 0xf7, 0x15, 0x88, \
0x09, 0xcf, 0x4f, 0x3c \
};

#endif