#ifndef GCURVEIMP_FUNC_H_HEADER_INCLUDED
#define GCURVEIMP_FUNC_H_HEADER_INCLUDED
//################################################
//############���������˵��######################
//################################################

//���߲鿴
//FUNCSTRUCT funcstruct;
//uint32 curvetype;
//int success=sendCMD( GYKF_STARTYK,(uint32)&funcstruct,curvetype );

enum
{
	CURVETYPE_TODAY,// ��������
	CURVETYPE_YES,//��������
	CURVETYPE_PLAN//�ƻ�����
};

#define GHCF_VIEWCURVE	0x600

#endif