#ifndef GVLAUELOCKIMP_FUNC_H_HEADER_INCLUDED
#define GVLAUELOCKIMP_FUNC_H_HEADER_INCLUDED
//################################################
//############���������˵��######################
//################################################

#include "ddisp/ddispdef.h"


//ֵ����
//FUNCSTRUCT funcstruct;
//int success=sendCMD( GVKF_LOCK,(GWPARAM)&funcstruct,0 );
#define	GVKF_LOCK		0x300


//ֵ��������
//FUNCSTRUCT funcstruct;
//int success=sendCMD( GVKF_UNLOCK,(GWPARAM)&funcstruct,0 );
#define	GVKF_UNLOCK		(GVKF_LOCK+1)

//��������
//FUNCSTRUCT funcstruct;
//int success=sendCMD( GVKF_MANSET,(GWPARAM)&funcstruct,0 );
#define GVKF_MANSET		(GVKF_UNLOCK+1)


#endif
