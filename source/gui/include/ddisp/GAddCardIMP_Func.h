#ifndef GADDCARDIMP_FUNC_H_HEADER_INCLUDED
#define GADDCARDIMP_FUNC_H_HEADER_INCLUDED
//################################################
//############���������˵��######################
//################################################


#include "ddes/GBaseObject.h"

//������־��ѡ��
//FUNCSTRUCT funcstruct;
//int success=sendCMD( GVKF_LOCK,(uint32)&funcstruct,0 );
#define	GACF_START		0x400


typedef struct _tagAddCardStruct
{
	GBaseObject		*obj;
	GPoint			pt;
	QString			graphname;
}ADDCARDSTRUCT,*PADDCARDSTRUCT;
//����
//ADDCARDSTRUCT card;
//int success=sendCMD( GACF_ADDCARD,(uint32)&card,0 );
#define	GACF_ADDCARD		(GACF_START+1)

//����
//GFuncStruct funcstruct;
//ADDCARDSTRUCT card;
//int success=sendCMD( GACF_ADDCARD,(uint32)&card,0 );
#define	GACF_DELCARD		(GACF_START+2)

//����
//GFuncStruct funcstruct;
//ADDCARDSTRUCT card;
//int success=sendCMD( GACF_FINDCARD,(uint32)&card,0 );
#define	GACF_FINDCARD		(GACF_START+3)

//modify by lipeng for ems&scada
//�����Ƶ���Ϣ
#define	GACF_MODIFYCARD		(GACF_START+6)

//�رմ���
//sendCMD( GACF_CLOSEWIN,0,0 );
#define	GACF_CLOSEWIN		(GACF_START+4)

//��ѯ�����Ƿ�ر�
//int closed = sendCMD( GACF_CLOSED,0,0 );
#define	GACF_CLOSED			(GACF_START+5)


#define	GACF_MODIFYCARD		(GACF_START+6)

#endif
