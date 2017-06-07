#ifndef LIC_GENR_DEFINE_H
#define LIC_GENR_DEFINE_H

#include <QString>
#include <QTextStream>

#include "AES.h"

#define  LICENSE_FILE_NAME      "license"

// ��Ŀ����


// ��Ȩ����key
#define  AUTHORIZATION_PERIOD           "Authorization period"
// �ļ�����ʱ��key
#define  FILE_GENERATION_TIME           "File generation time"
// ϵͳ����ʱ��key
#define  SYSTEM_RUNNING_TIME            "System running time"
// ���س��μ���ʱ��key
#define  LOCAL_INITIAL_LOADING_TIME     "Local initial loading time"
// ���ص�ǰ����key
#define  LOCAL_CURRENT_DATE             "Local current date"
// Mac��ַkey
#define  MAC_ADDRESS                    "Mac address"
// �澯����
#define  WARNING_LEVEL                  "Warning level"

// �����澯
#define  PRIMARY_ALARM                  "Primary alarm"
// �μ��澯
#define  SECONDARY_ALARM                "Secondary alarm"
// �߼��澯
#define  ADVANCED_ALARM                 "Advanced alarm"

// ��Ȩģ��
// to do:�Ժ�����

// 1�������
#define ONE_DAY     24 * 60

// ������Ȩ���
#define UNLIMITED_FLAG       0xFF

// ���key����
#define  KEY_MAX_LEN                    32
// ���value����
#define  VALUE_MAX_LEN                  32
// �ļ�keyֵ���16���Ƴ���
#define  HEX_KEY_MAX_LEN                100
// �ļ�valueֵ���16���Ƴ���
#define  HEX_VALUE_MAX_LEN              HEX_KEY_MAX_LEN

#define  THREE_DAY				"Three day"
#define  ONE_MONTH			"One month"
#define  THREE_MONTH		"Three month"
#define  HALF_YEAR				"Half year"
#define  ONE_YEAR				"One year"
#define  TWO_YEAR				"Two year"
#define  UNLIMITED				"Unlimited"

// �޷���charת����16�����ַ���
LICENSELIB_EXPORT void unsigned_char_to_hexString(unsigned char* source, char* dest, unsigned int destLen);

// 16����תunsigned char*
LICENSELIB_EXPORT void HexStrToByte(const char* source, unsigned char* dest, int sourceLen);

// ����mac��ַ
LICENSELIB_EXPORT QString SearchMacAddr();

// license�ļ�key����
LICENSELIB_EXPORT char* FindKey(const char* src);

// license�ļ�value����
LICENSELIB_EXPORT char* FindValue(const char* src);

// �ж��Ƿ�����
LICENSELIB_EXPORT bool isDigitStr(const char* src);

// 20xx-xx-xx xx:xx:xx ��׼ʱ���ʽ���
LICENSELIB_EXPORT bool TimeCheck(const char* src);

// �ļ��������ü��
LICENSELIB_EXPORT bool FileImportCheck(QTextStream& stream, AES& aes);

// ��Ȩ�ӿ��ļ����ü��
LICENSELIB_EXPORT bool FileCheckForAuthInterface(QTextStream& stream, AES& aes);

// ʱ���ʽ��20xx-xx-xx xx:xx:xx���뵱ǰϵͳʱ���ֵ���� 
LICENSELIB_EXPORT int TimeFormatCalc(QString& time);

#endif
