#ifndef LIB_AUTU_H_
#define LIB_AUTU_H_

#if defined(WIN32)
#if defined(AUTH_EXPORTDLL)
#define	AUTH_IMEXPORT				__declspec(dllexport) 
#else
#define	AUTH_IMEXPORT				__declspec(dllimport) 
#endif
#else
#define	AUTH_IMEXPORT
#endif

const int HASH_STRING_LEN = 20;///<�����md5���ĵĳ���

/**
@brief �û�����У��
@param in const char* user_name �û���
@param in const char* input_pwd ����Ŀ���
@param in const char* db_pwd ���ݿ��ж����Ŀ���
@return У��ɹ�����0��ʧ�ܷ���-1
*/
int AUTH_IMEXPORT user_auth(const char* user_name, const char* input_pwd, const char* db_pwd);

/**
@brief �ѿ���ת��Ϊ����MD5���ܹ����ִ����������������
@param in const char* input_pwd ����Ŀ���
@param out char* out_pwd ���ݿ��ж����Ŀ���
@param in const int& out_lenth ����������ĳ���,����С��HASH_STRING_LEN
@return �ɹ�����0��ʧ�ܷ���-1
*/
int AUTH_IMEXPORT convert_md5(const char* input_pwd, char* out_pwd, const int& out_lenth);

/**
@brief ��֤�����Ƿ����㸴�Ӷ�Ҫ��
@param in const char* input_pwd ����Ŀ���
@return �ɹ�����true��ʧ�ܷ���false
*/
bool AUTH_IMEXPORT verify_password_auth(const char* input_pwd);

/**
@brief ��ȡ�������ô��������
@return ���ش�����Ϣ
*/
AUTH_IMEXPORT char* get_error_desc_auth();

/**
@brief ������У��

ָ�ơ�����ȣ�����չ
@param in user_name �û���
@return �ɹ�����0��ʧ�ܷ���-1
*/
int thirt_party_auth(const char* user_name);


#endif