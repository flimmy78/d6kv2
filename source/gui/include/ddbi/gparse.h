#ifndef __GPARSE_H__
#define __GPARSE_H__

#include "ddef/ddes_comm.h"
/*
 * ���ܣ�
 *		��ָ���ַ������з���
 * ����(�Կհ��Ʊ���ŷָ�)
 *      ʶ��		����				��ʶ
 *      �ַ���		('*')("*")			GPST_STRING
 *		��ʶ		(L*)				GPST_ID     L��ʾ�����ֿ�ͷ
 *		����		(N+)				GPST_INT	N+���㿪ͷ�������
 *		С��		(N+|N+.N+)			GPST_FLOAT
 *		����		(<,>,=,>=,<=,<>,!=) GPST_LOGIC
 *      �����		(+,-,*,/)			GPST_OPER
 *		��β		('\n')				GPSE_EOL
 *		����							GPSE_ERR
 *		��С�������					GPSE_POTS
 *		ȱ�ٶ�Ӧ����					GPSE_COMMA
 *		�ָ���		(,)					GPST_COMMA
 *		��������	('(',')')			GPST_SYMB
 */

enum 
{
	GPSE_COMMA =0xfc,
	GPSE_POTS  =0xfd,
	GPSE_ERR   =0xfe,
	GPSE_EOL   =0xff,
	GPST_STRING=0x01,
	GPST_ID    =0x02,
	GPST_INT   =0x03,
	GPST_FLOAT =0x04,
	GPST_LOGIC =0x05,
	GPST_OPER  =0x06,
	GPST_COMMA =0x07,
	GPST_SYMB  =0x08
};

class GDE_EXPORT GParse
{
	char	m_currWord[GDB_STR_LEN] ;
	uint32	m_currPos  ;
	uint32  m_prevPos  ;
	char*	m_pBuffer  ;
	uint8	m_bcase    ;
	uint8	m_berror   ;
	uint32  m_expect   ;

public:

	GParse();
	GParse(const char* inBuf );
	
	void    setcase(uint8 bcase=1);
	uint8   iscase();

	uint8	is_expect( const char* w );
	uint8	not_expect(const char* w );

	const char* word() const;

	uint8	error();

	uint8	expis( uint32 exp );
	uint8	expnot( uint32 exp );

	void    set( const char* inBuf );

	uint32  getPos() { return m_currPos; }

	uint32  get();
	void	unget();
};

#endif
