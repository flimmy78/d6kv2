//*********************************************
//
//Created by hongxiang 20160701
//
//*********************************************

/**
@brief ��ȡ�汾��Ϣ
@param in char * ver appname,version
@return 
*/
#if defined(WIN32)
#if defined(APPFUNC_EXPORTDLL)
void  __declspec(dllexport)  writever( char * ver ); 
#else
void __declspec(dllimport)   writever( char * ver );
#endif
#else
void  writever( char * ver );
#endif


