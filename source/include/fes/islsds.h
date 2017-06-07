/*
* File: islsds.h
* Copyright (c)
*
* v1.00.0001  2009.07.15     
*/

#ifndef _ISL_SDS_H_
#define _ISL_SDS_H_ 1

#ifdef __cplusplus
	extern "C"{
#endif

#define SGD_API_VERSION		0x01000007 /*api version*/

/*�������Ͷ���*/
typedef char				SGD_CHAR;
typedef char				SGD_INT8;
typedef short				SGD_INT16;
typedef int					SGD_INT32;
typedef unsigned char		SGD_UCHAR;
typedef unsigned char		SGD_UINT8;
typedef unsigned short		SGD_UINT16;
typedef unsigned int		SGD_UINT32;
typedef unsigned int		SGD_RV;
typedef void*				SGD_OBJ;
typedef int					SGD_BOOL;
typedef void*				SGD_HANDLE;

#ifdef WIN32
typedef __int64				SGD_INT64;
typedef unsigned __int64	SGD_UINT64;
#else
typedef long long			SGD_INT64;
typedef unsigned long long	SGD_UINT64;
#endif

/*�豸��Ϣ*/
typedef struct DeviceInfo_st{
	unsigned char IssuerName[40];
	unsigned char DeviceName[16];
	unsigned char DeviceSerial[16];
	unsigned int  DeviceVersion;
	unsigned int  StandardVersion;
	unsigned int  AsymAlgAbility[2];
	unsigned int  SymAlgAbility;
	unsigned int  HashAlgAbility;
	unsigned int  BufferSize;
} DEVICEINFO;

/*RSA��Կ*/
#define RSAref_MAX_BITS    2048
#define RSAref_MAX_LEN     ((RSAref_MAX_BITS + 7) / 8)
#define RSAref_MAX_PBITS   ((RSAref_MAX_BITS + 1) / 2)
#define RSAref_MAX_PLEN    ((RSAref_MAX_PBITS + 7)/ 8)

typedef struct RSArefPublicKey_st
{
	unsigned int  bits;
	unsigned char m[RSAref_MAX_LEN];
	unsigned char e[RSAref_MAX_LEN];
} RSArefPublicKey;

typedef struct RSArefPrivateKey_st
{
	unsigned int  bits;
	unsigned char m[RSAref_MAX_LEN];
	unsigned char e[RSAref_MAX_LEN];
	unsigned char d[RSAref_MAX_LEN];
	unsigned char prime[2][RSAref_MAX_PLEN];
	unsigned char pexp[2][RSAref_MAX_PLEN];
	unsigned char coef[RSAref_MAX_PLEN];
} RSArefPrivateKey;

/*ECC��Կ*/
#define ECCref_MAX_BITS			256 
#define ECCref_MAX_LEN			((ECCref_MAX_BITS+7) / 8)

typedef struct ECCrefPublicKey_st
{
	unsigned int  bits;
	unsigned char x[ECCref_MAX_LEN]; 
	unsigned char y[ECCref_MAX_LEN]; 
} ECCrefPublicKey;

typedef struct ECCrefPrivateKey_st
{
    unsigned int  bits;
    unsigned char D[ECCref_MAX_LEN];
} ECCrefPrivateKey;

/*ECC ����*/
typedef struct ECCCipher_st
{
	unsigned char x[ECCref_MAX_LEN]; 
	unsigned char y[ECCref_MAX_LEN]; 
	unsigned char C[ECCref_MAX_LEN];
	unsigned char M[ECCref_MAX_LEN];
} ECCCipher;

/*ECC ǩ��*/
typedef struct ECCSignature_st
{
	unsigned char r[ECCref_MAX_LEN];	
	unsigned char s[ECCref_MAX_LEN];	
} ECCSignature;

typedef struct
{ 
	unsigned char p[ECCref_MAX_LEN];	//����p
	unsigned char a[ECCref_MAX_LEN];	//����a
	unsigned char b[ECCref_MAX_LEN];	//����b
	unsigned char gx[ECCref_MAX_LEN];	//����Gx: x coordinate of the base point G
	unsigned char gy[ECCref_MAX_LEN];	//����Gy: y coordinate of the base point G
	unsigned char n[ECCref_MAX_LEN];	//��N: order n of the base point G
	unsigned int  len;					//����λ��Len��Len����Ϊ160��192��224��256
}ECCrefCurveParam;

/*��������*/
#define SGD_TRUE	0x00000001
#define SGD_FALSE	0x00000000

/*�㷨��ʶ*/
#define SGD_SM1_ECB		0x00000101
#define SGD_SM1_CBC		0x00000102
#define SGD_SM1_CFB		0x00000104
#define SGD_SM1_OFB		0x00000108
#define SGD_SM1_MAC		0x00000110
#define SGD_SM1_CTR		0x00000120

#define SGD_SSF33_ECB	0x00000201
#define SGD_SSF33_CBC	0x00000202
#define SGD_SSF33_CFB	0x00000204
#define SGD_SSF33_OFB	0x00000208
#define SGD_SSF33_MAC	0x00000210
#define SGD_SSF33_CTR	0x00000220

#define SGD_AES_ECB		0x00000401
#define SGD_AES_CBC		0x00000402
#define SGD_AES_CFB		0x00000404
#define SGD_AES_OFB		0x00000408
#define SGD_AES_MAC		0x00000410
#define SGD_AES_CTR		0x00000420

#define SGD_3DES_ECB	0x00000801
#define SGD_3DES_CBC	0x00000802
#define SGD_3DES_CFB	0x00000804
#define SGD_3DES_OFB	0x00000808
#define SGD_3DES_MAC	0x00000810
#define SGD_3DES_CTR	0x00000820

#define SGD_SMS4_ECB	0x00002001
#define SGD_SMS4_CBC	0x00002002
#define SGD_SMS4_CFB	0x00002004
#define SGD_SMS4_OFB	0x00002008
#define SGD_SMS4_MAC	0x00002010
#define SGD_SMS4_CTR	0x00002020

#define SGD_DES_ECB		0x00004001
#define SGD_DES_CBC		0x00004002
#define SGD_DES_CFB		0x00004004
#define SGD_DES_OFB		0x00004008
#define SGD_DES_MAC		0x00004010
#define SGD_DES_CTR		0x00004020

#define SGD_RSA_SIGN	0x00010100
#define SGD_RSA_ENC		0x00010200

#define SGD_SM2_1		0x00020100
#define SGD_SM2_2		0x00020200
#define SGD_SM2_3		0x00020400

#define SGD_SM3			0x00000001
#define SGD_SHA1		0x00000002
#define SGD_SHA256		0x00000004
#define SGD_SHA512		0x00000008
#define SGD_SHA384		0x00000010
#define SGD_SHA224		0x00000020
#define SGD_MD2			0x00000040
#define SGD_MD5			0x00000080

#define KEY_TYPE_ECDSA          7
#define KEY_TYPE_DSA            6
#define KEY_TYPE_RSA_EX			5
#define KEY_TYPE_RSA			4		//��ȡ��Կ״̬ʱ��ӦRSA�ǶԳ���Կ����
#define KEY_TYPE_RSA_DER		0x14	//DER��ʽ��Կ
#define KEY_TYPE_ECC			3		//��ȡ��Կ״̬ʱ��ӦECC�ǶԳ���Կ����

#define KEY_TYPE_SESSION_KEY	2		//��ȡ��Կ״̬ʱ��Ӧ��ʧ�ԶԳ���Կ����
#define KEY_TYPE_KEK			1		//��ȡ��Կ״̬ʱ��Ӧ����ʧ�ԶԳ���Կ����
#define KEY_TYPE_EXTERNAL_KEY	0


/*============================================================*/
/*��׼�����붨��*/
#define SDR_OK					0x0						   /*�ɹ�*/
#define SDR_BASE				0x01000000
#define SDR_UNKNOWERR			(SDR_BASE + 0x00000001)	   /*δ֪����*/
#define SDR_NOTSUPPORT			(SDR_BASE + 0x00000002)	   /*��֧��*/
#define SDR_COMMFAIL			(SDR_BASE + 0x00000003)    /*ͨ�Ŵ���*/
#define SDR_HARDFAIL			(SDR_BASE + 0x00000004)    /*Ӳ������*/
#define SDR_OPENDEVICE			(SDR_BASE + 0x00000005)    /*���豸����*/
#define SDR_OPENSESSION			(SDR_BASE + 0x00000006)    /*�򿪻Ự�������*/
#define SDR_PARDENY				(SDR_BASE + 0x00000007)    /*Ȩ�޲�����*/
#define SDR_KEYNOTEXIST			(SDR_BASE + 0x00000008)    /*��Կ������*/
#define SDR_ALGNOTSUPPORT		(SDR_BASE + 0x00000009)    /*��֧�ֵ��㷨*/
#define SDR_ALGMODNOTSUPPORT	(SDR_BASE + 0x0000000A)    /*��֧�ֵ��㷨ģʽ*/
#define SDR_PKOPERR				(SDR_BASE + 0x0000000B)    /*��Կ�������*/
#define SDR_SKOPERR				(SDR_BASE + 0x0000000C)    /*˽Կ�������*/
#define SDR_SIGNERR				(SDR_BASE + 0x0000000D)    /*ǩ������*/
#define SDR_VERIFYERR			(SDR_BASE + 0x0000000E)    /*��֤����*/
#define SDR_SYMOPERR			(SDR_BASE + 0x0000000F)    /*�Գ��������*/
#define SDR_STEPERR				(SDR_BASE + 0x00000010)    /*�������*/
#define SDR_FILESIZEERR			(SDR_BASE + 0x00000011)    /*�ļ���С������������ݳ��ȷǷ�*/
#define SDR_FILENOEXIST			(SDR_BASE + 0x00000012)    /*�ļ�������*/
#define SDR_FILEOFSERR			(SDR_BASE + 0x00000013)    /*�ļ�����ƫ��������*/
#define SDR_KEYTYPEERR			(SDR_BASE + 0x00000014)    /*��Կ���ʹ���*/
#define SDR_KEYERR				(SDR_BASE + 0x00000015)    /*��Կ����*/

/*============================================================*/
/*��չ������*/
#define ISLR_BASE				(SDR_BASE + 0x00010000)	/*�Զ�����������ֵ*/
#define ISLR_INVALID_USER		(ISLR_BASE + 0x00000001)	/*��Ч���û���*/
#define ISLR_INVALID_AUTHENCODE	(ISLR_BASE + 0x00000002)	/*��Ч����Ȩ��*/
#define ISLR_PROTOCOL_VER_ERR	(ISLR_BASE + 0x00000003)	/*��֧�ֵ�Э��汾*/
#define ISLR_INVALID_COMMAND	(ISLR_BASE + 0x00000004)	/*�����������*/
#define ISLR_INVALID_PARAMETERS	(ISLR_BASE + 0x00000005)	/*����������������ݰ���ʽ*/
#define ISLR_FILE_ALREADY_EXIST	(ISLR_BASE + 0x00000006)	/*�Ѵ���ͬ���ļ�*/
#define ISLR_SYNCH_ERR			(ISLR_BASE + 0x00000007)	/*�࿨ͬ������*/
#define ISLR_SYNCH_LOGIN_ERR	(ISLR_BASE + 0x00000008)	/*�࿨ͬ�����¼����*/

#define ISLR_SOCKET_TIMEOUT		(ISLR_BASE + 0x00000100)	/*��ʱ����*/
#define ISLR_CONNECT_ERR		(ISLR_BASE + 0x00000101)	/*���ӷ���������*/
#define ISLR_SET_SOCKOPT_ERR	(ISLR_BASE + 0x00000102)	/*����Socket��������*/
#define ISLR_SOCKET_SEND_ERR	(ISLR_BASE + 0x00000104)	/*����LOGINRequest����*/
#define ISLR_SOCKET_RECV_ERR	(ISLR_BASE + 0x00000105)	/*����LOGINRequest����*/
#define ISLR_SOCKET_RECV_0		(ISLR_BASE + 0x00000106)	/*����LOGINRequest����*/

#define ISLR_SEM_TIMEOUT		(ISLR_BASE + 0x00000200)	/*��ʱ����*/
#define ISLR_NO_AVAILABLE_HSM	(ISLR_BASE + 0x00000201)	/*û�п��õļ��ܻ�*/
#define ISLR_NO_AVAILABLE_CSM	(ISLR_BASE + 0x00000202)	/*���ܻ���û�п��õļ���ģ��*/

#define ISLR_CONFIG_ERR			(ISLR_BASE + 0x00000301)	/*�����ļ�����*/

/*============================================================*/
/*���뿨������*/
#define ISLR_CARD_BASE					(SDR_BASE + 0x00020000)			/*���뿨������*/
#define ISLR_CARD_UNKNOWERR				(ISLR_CARD_BASE + 0x00000001)	//δ֪����
#define ISLR_CARD_NOTSUPPORT			(ISLR_CARD_BASE + 0x00000002)	//��֧�ֵĽӿڵ���
#define ISLR_CARD_COMMFAIL				(ISLR_CARD_BASE + 0x00000003)	//���豸ͨ��ʧ��
#define ISLR_CARD_HARDFAIL				(ISLR_CARD_BASE + 0x00000004)	//����ģ������Ӧ
#define ISLR_CARD_OPENDEVICE			(ISLR_CARD_BASE + 0x00000005)	//���豸ʧ��
#define ISLR_CARD_OPENSESSION			(ISLR_CARD_BASE + 0x00000006)	//�����Ựʧ��
#define ISLR_CARD_PARDENY				(ISLR_CARD_BASE + 0x00000007)	//��˽Կʹ��Ȩ��
#define ISLR_CARD_KEYNOTEXIST			(ISLR_CARD_BASE + 0x00000008)	//�����ڵ���Կ����
#define ISLR_CARD_ALGNOTSUPPORT			(ISLR_CARD_BASE + 0x00000009)	//��֧�ֵ��㷨����
#define ISLR_CARD_ALGMODNOTSUPPORT		(ISLR_CARD_BASE + 0x00000010)	//��֧�ֵ��㷨����
#define ISLR_CARD_PKOPERR				(ISLR_CARD_BASE + 0x00000011)	//��Կ����ʧ��
#define ISLR_CARD_SKOPERR				(ISLR_CARD_BASE + 0x00000012)	//˽Կ����ʧ��
#define ISLR_CARD_SIGNERR				(ISLR_CARD_BASE + 0x00000013)	//ǩ������ʧ��
#define ISLR_CARD_VERIFYERR				(ISLR_CARD_BASE + 0x00000014)	//��֤ǩ��ʧ��
#define ISLR_CARD_SYMOPERR				(ISLR_CARD_BASE + 0x00000015)	//�Գ��㷨����ʧ��
#define ISLR_CARD_STEPERR				(ISLR_CARD_BASE + 0x00000016)	//�ಽ���㲽�����
#define ISLR_CARD_FILESIZEERR			(ISLR_CARD_BASE + 0x00000017)	//�ļ����ȳ�������
#define ISLR_CARD_FILENOEXIST			(ISLR_CARD_BASE + 0x00000018)	//ָ�����ļ�������
#define ISLR_CARD_FILEOFSERR			(ISLR_CARD_BASE + 0x00000019)	//�ļ���ʼλ�ô���
#define ISLR_CARD_KEYTYPEERR			(ISLR_CARD_BASE + 0x00000020)	//��Կ���ʹ���
#define ISLR_CARD_KEYERR				(ISLR_CARD_BASE + 0x00000021)	//��Կ����
#define ISLR_CARD_BUFFER_TOO_SMALL		(ISLR_CARD_BASE + 0x00000101)	//���ղ����Ļ�����̫С
#define ISLR_CARD_DATA_PAD				(ISLR_CARD_BASE + 0x00000102)	//����û�а���ȷ��ʽ��䣬����ܵõ����������ݲ���������ʽ
#define ISLR_CARD_DATA_SIZE				(ISLR_CARD_BASE + 0x00000103)	//���Ļ����ĳ��Ȳ�������Ӧ���㷨Ҫ��
#define ISLR_CARD_CRYPTO_NOT_INIT		(ISLR_CARD_BASE + 0x00000104)	//�ô������û��Ϊ��Ӧ���㷨���ó�ʼ������

//01/03/09�����뿨Ȩ�޹��������
#define ISLR_CARD_MANAGEMENT_DENY		(ISLR_CARD_BASE + 0x00001001)	//����Ȩ�޲�����
#define ISLR_CARD_OPERATION_DENY		(ISLR_CARD_BASE + 0x00001002)	//����Ȩ�޲�����
#define ISLR_CARD_DEVICE_STATUS_ERR		(ISLR_CARD_BASE + 0x00001003)	//��ǰ�豸״̬���������в���
#define ISLR_CARD_LOGIN_ERR				(ISLR_CARD_BASE + 0x00001011)	//��¼ʧ��
#define ISLR_CARD_USERID_ERR			(ISLR_CARD_BASE + 0x00001012)	//�û�ID��Ŀ/�������
#define ISLR_CARD_PARAMENT_ERR			(ISLR_CARD_BASE + 0x00001013)	//��������

//05/06�����뿨Ȩ�޹��������
#define ISLR_CARD_MANAGEMENT_DENY_05	(ISLR_CARD_BASE + 0x00000801)	//����Ȩ�޲�����
#define ISLR_CARD_OPERATION_DENY_05		(ISLR_CARD_BASE + 0x00000802)	//����Ȩ�޲�����
#define ISLR_CARD_DEVICE_STATUS_ERR_05	(ISLR_CARD_BASE + 0x00000803)	//��ǰ�豸״̬���������в���
#define ISLR_CARD_LOGIN_ERR_05			(ISLR_CARD_BASE + 0x00000811)	//��¼ʧ��
#define ISLR_CARD_USERID_ERR_05			(ISLR_CARD_BASE + 0x00000812)	//�û�ID��Ŀ/�������
#define ISLR_CARD_PARAMENT_ERR_05		(ISLR_CARD_BASE + 0x00000813)	//��������

/*============================================================*/
/*����������*/
#define ISLR_CARD_READER_BASE				(SDR_BASE + 0x00030000)	//	���������ʹ���
#define ISLR_CARD_READER_PIN_ERROR			(ISLR_CARD_READER_BASE + 0x000063CE)  //�������
#define ISLR_CARD_READER_NO_CARD			(ISLR_CARD_READER_BASE + 0x0000FF01)	 //	ICδ����
#define ISLR_CARD_READER_CARD_INSERT		(ISLR_CARD_READER_BASE + 0x0000FF02)	 //	IC���뷽�����򲻵�λ
#define ISLR_CARD_READER_CARD_INSERT_TYPE	(ISLR_CARD_READER_BASE + 0x0000FF03)	 //	IC���ʹ���


/*�豸�����ຯ��*/
SGD_RV SDF_OpenDevice(SGD_HANDLE *phDeviceHandle);
SGD_RV SDF_CloseDevice(SGD_HANDLE hDeviceHandle);
SGD_RV SDF_OpenSession(SGD_HANDLE hDeviceHandle, SGD_HANDLE *phSessionHandle);
SGD_RV SDF_CloseSession(SGD_HANDLE hSessionHandle);
SGD_RV SDF_GetDeviceInfo(SGD_HANDLE hSessionHandle, DEVICEINFO *pstDeviceInfo);
SGD_RV SDF_GetDeviceInfoEx(SGD_HANDLE hSessionHandle, DEVICEINFO *pstDeviceInfo, SGD_UINT32 *puiDeviceCount);
SGD_RV SDF_GetFirmwareVersion(SGD_HANDLE hSessionHandle, SGD_UCHAR *sFirmware, SGD_UINT32 *ulFirmwareLen);
SGD_RV SDF_GetLibraryVersion(SGD_HANDLE hSessionHandle, SGD_UCHAR *sLibraryVersion, SGD_UINT32 *ulLibraryVersionLen);
SGD_RV SDF_GenerateRandom(SGD_HANDLE hSessionHandle, SGD_UINT32 uiLength, SGD_UCHAR *pucRandom);
SGD_RV SDF_GetPrivateKeyAccessRight(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyIndex, SGD_UCHAR *pucPassword, SGD_UINT32  uiPwdLength);
SGD_RV SDF_ReleasePrivateKeyAccessRight(SGD_HANDLE hSessionHandle, SGD_UINT32  uiKeyIndex);
SGD_RV SDF_GetKeyStatus(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyType, SGD_UINT32 *puiKeyStatus, SGD_UINT32 *puiKeyCount);

/*��Կ�����ຯ��*/
SGD_RV SDF_GenerateKeyPair_RSA(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyBits, RSArefPublicKey *pucPublicKey, RSArefPrivateKey *pucPrivateKey);
SGD_RV SDF_ExportSignPublicKey_RSA(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyIndex, RSArefPublicKey *pucPublicKey);
SGD_RV SDF_ExportEncPublicKey_RSA(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyIndex, RSArefPublicKey *pucPublicKey);
SGD_RV SDF_GenerateKeyWithIPK_RSA(SGD_HANDLE hSessionHandle, SGD_UINT32 uiIPKIndex, SGD_UINT32 uiKeyBits, SGD_UCHAR *pucKey, SGD_UINT32 *puiKeyLength, SGD_HANDLE *phKeyHandle);
SGD_RV SDF_GenerateKeyWithEPK_RSA(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyBits, RSArefPublicKey *pucPublicKey, SGD_UCHAR *pucKey, SGD_UINT32 *puiKeyLength, \
											 SGD_HANDLE *phKeyHandle);
SGD_RV SDF_ImportKeyWithISK_RSA(SGD_HANDLE hSessionHandle, SGD_UINT32 uiISKIndex, SGD_UCHAR *pucKey, SGD_UINT32 uiKeyLength, SGD_HANDLE *phKeyHandle);
SGD_RV SDF_ExchangeDigitEnvelopeBaseOnRSA(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyIndex, RSArefPublicKey *pucPublicKey, SGD_UCHAR *pucDEInput, SGD_UINT32 uiDELength, \
													 SGD_UCHAR *pucDEOutput, SGD_UINT32 *puiDELength);

SGD_RV SDF_ExportSignPublicKey_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyIndex, ECCrefPublicKey *pucPublicKey);
SGD_RV SDF_ExportEncPublicKey_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyIndex, ECCrefPublicKey *pucPublicKey);
SGD_RV SDF_GenerateKeyPair_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiAlgID, SGD_UINT32 uiKeyBits, ECCrefPublicKey *pucPublicKey, ECCrefPrivateKey *pucPrivateKey);
SGD_RV SDIF_KeyAgreement_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiFlag, SGD_UINT32 uiISKIndex, \
										ECCrefPublicKey *pucOwnTmpPublicKey, ECCrefPrivateKey *pucOwnTmpPrivateKey, \
										ECCrefPublicKey *pucOppositePublicKey, ECCrefPublicKey *pucOppositeTmpPublicKey, SGD_UINT32 uiKeyBits, \
										SGD_UINT32 uiOwnIDLength, SGD_UCHAR *pucOwnID, SGD_UINT32 uiOppositeIDLength, SGD_UCHAR *pucOppositeID, SGD_UCHAR *pucAgreementData);
SGD_RV SDF_GenerateAgreementDataWithECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiISKIndex, SGD_UINT32 uiKeyBits, SGD_UCHAR *pucSponsorID, SGD_UINT32 uiSponsorIDLength, \
												   ECCrefPublicKey *pucSponsorPublicKey, ECCrefPublicKey *pucSponsorTmpPublicKey, SGD_HANDLE *phAgreementHandle);
SGD_RV SDF_GenerateKeyWithECC(SGD_HANDLE hSessionHandle, SGD_UCHAR *pucResponseID, SGD_UINT32 uiResponseIDLength, ECCrefPublicKey *pucResponsePublicKey, \
										 ECCrefPublicKey *pucResponseTmpPublicKey, SGD_HANDLE hAgreementHandle, SGD_HANDLE *phKeyHandle);
SGD_RV SDF_GenerateAgreementDataAndKeyWithECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiISKIndex, SGD_UINT32 uiKeyBits, SGD_UCHAR *pucResponseID, SGD_UINT32 uiResponseIDLength, \
														 SGD_UCHAR *pucSponsorID, SGD_UINT32 uiSponsorIDLength, ECCrefPublicKey *pucSponsorPublicKey, ECCrefPublicKey *pucSponsorTmpPublicKey, \
														 ECCrefPublicKey  *pucResponsePublicKey, ECCrefPublicKey  *pucResponseTmpPublicKey, SGD_HANDLE *phKeyHandle);
SGD_RV SDF_GenerateKeyWithIPK_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiIPKIndex, SGD_UINT32 uiKeyBits, ECCCipher *pucKey, SGD_HANDLE *phKeyHandle);
SGD_RV SDF_GenerateKeyWithEPK_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyBits, SGD_UINT32 uiAlgID, ECCrefPublicKey *pucPublicKey, ECCCipher *pucKey, SGD_HANDLE *phKeyHandle);
SGD_RV SDF_ImportKeyWithISK_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiISKIndex, ECCCipher *pucKey, SGD_HANDLE *phKeyHandle);
SGD_RV SDF_ExchangeDigitEnvelopeBaseOnECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyIndex, SGD_UINT32 uiAlgID, ECCrefPublicKey *pucPublicKey, ECCCipher *pucEncDataIn, ECCCipher *pucEncDataOut);

SGD_RV SDF_GetSymmKeyHandle(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyIndex, SGD_HANDLE *phKeyHandle);
SGD_RV SDF_GenerateKeyWithKEK(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyBits, SGD_UINT32 uiAlgID, SGD_UINT32 uiKEKIndex, SGD_UCHAR *pucKey, SGD_UINT32 *puiKeyLength, \
										 SGD_HANDLE *phKeyHandle);
SGD_RV SDF_ImportKeyWithKEK(SGD_HANDLE hSessionHandle, SGD_UINT32 uiAlgID, SGD_UINT32 uiKEKIndex, SGD_UCHAR *pucKey, SGD_UINT32 uiKeyLength, SGD_HANDLE *phKeyHandle);
SGD_RV SDF_ImportKey(SGD_HANDLE hSessionHandle, SGD_UCHAR *pucKey, SGD_UINT32 *uiKeyLength, SGD_HANDLE *phKeyHandle);
SGD_RV SDF_DestroyKey(SGD_HANDLE hSessionHandle, SGD_HANDLE hKeyHandle);
SGD_RV SDF_DestoryKey(SGD_HANDLE hSessionHandle, SGD_HANDLE hKeyHandle);
SGD_RV SDF_DelKey(SGD_HANDLE hSessionHandle, SGD_HANDLE hKeyHandle);

/*�ǶԳ��㷨�����ຯ��*/
SGD_RV SDF_ExternalPublicKeyOperation_RSA(SGD_HANDLE hSessionHandle, RSArefPublicKey *pucPublicKey, SGD_UCHAR *pucDataInput, SGD_UINT32 uiInputLength, SGD_UCHAR *pucDataOutput, \
													 SGD_UINT32 *puiOutputLength);
SGD_RV SDF_ExternalPrivateKeyOperation_RSA(SGD_HANDLE hSessionHandle, RSArefPrivateKey *pucPrivateKey, SGD_UCHAR *pucDataInput, SGD_UINT32 uiInputLength, \
													  SGD_UCHAR *pucDataOutput, SGD_UINT32  *puiOutputLength);
SGD_RV SDF_InternalPublicKeyOperation_RSA(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyIndex, SGD_UINT32 uiKeyUsage, SGD_UCHAR *pucDataInput, SGD_UINT32 uiInputLength, \
													 SGD_UCHAR *pucDataOutput, SGD_UINT32  *puiOutputLength);
SGD_RV SDF_InternalPrivateKeyOperation_RSA(SGD_HANDLE hSessionHandle, SGD_UINT32 uiKeyIndex, SGD_UINT32 uiKeyUsage, SGD_UCHAR *pucDataInput, SGD_UINT32 uiInputLength, \
													  SGD_UCHAR *pucDataOutput, SGD_UINT32 *puiOutputLength);
SGD_RV SDF_InternalEncrypt_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiAlgID, SGD_UINT32 uiIPKIndex, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, ECCCipher *pucEncData);
SGD_RV SDF_InternalDecrypt_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiAlgID, SGD_UINT32 uiISKIndex, ECCCipher *pucEncData, SGD_UCHAR *pucData, SGD_UINT32 *puiDataLength);
SGD_RV SDF_InternalEncrypt_ECC_General(SGD_HANDLE hSessionHandle, SGD_UINT32 uiIPKIndex, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, ECCCipher *pucEncData);
SGD_RV SDF_InternalDecrypt_ECC_General(SGD_HANDLE hSessionHandle, SGD_UINT32 uiISKIndex, ECCCipher *pucEncData, SGD_UCHAR *pucData, SGD_UINT32 *puiDataLength);
SGD_RV SDF_ExternalEncrypt_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiAlgID, ECCrefPublicKey *pucPublicKey, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, ECCCipher *pucEncData);
SGD_RV SDF_ExternalDecrypt_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiAlgID, ECCrefPrivateKey *pucPrivateKey, ECCCipher *pucEncData, SGD_UCHAR *pucData, SGD_UINT32 *puiDataLength);
SGD_RV SDF_ExternalSign_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiAlgID, ECCrefPrivateKey *pucPrivateKey, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, ECCSignature *pucSignature);
SGD_RV SDF_ExternalVerify_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiAlgID, ECCrefPublicKey *pucPublicKey, SGD_UCHAR *pucDataInput, SGD_UINT32 uiInputLength, \
										 ECCSignature *pucSignature);
SGD_RV SDF_InternalSign_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiISKIndex, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, ECCSignature *pucSignature);
SGD_RV SDF_InternalVerify_ECC(SGD_HANDLE hSessionHandle, SGD_UINT32 uiISKIndex, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, ECCSignature *pucSignature);
SGD_RV SDF_InternalSign_ECC_Ex(SGD_HANDLE hSessionHandle, SGD_UINT32 uiISKIndex, SGD_UINT32 uiAlgID, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, ECCSignature *pucSignature);
SGD_RV SDF_InternalVerify_ECC_Ex(SGD_HANDLE hSessionHandle, SGD_UINT32 uiISKIndex, SGD_UINT32 uiAlgID, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, ECCSignature *pucSignature);
SGD_RV SDF_InternalSign_ECC_General(SGD_HANDLE hSessionHandle, SGD_UINT32 uiISKIndex, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, ECCSignature *pucSignature);
SGD_RV SDF_InternalVerify_ECC_General(SGD_HANDLE hSessionHandle, SGD_UINT32 uiISKIndex, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, ECCSignature *pucSignature);

/*�Գ��㷨�����ຯ��*/
SGD_RV SDF_Encrypt(SGD_HANDLE hSessionHandle, SGD_HANDLE hKeyHandle, SGD_UINT32 uiAlgID, SGD_UCHAR *pucIV, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, SGD_UCHAR *pucEncData, \
							  SGD_UINT32 *puiEncDataLength);
SGD_RV SDF_Decrypt(SGD_HANDLE hSessionHandle, SGD_HANDLE hKeyHandle, SGD_UINT32 uiAlgID, SGD_UCHAR *pucIV, SGD_UCHAR *pucEncData, SGD_UINT32 uiEncDataLength, SGD_UCHAR *pucData, \
							  SGD_UINT32 *puiDataLength);
SGD_RV SDF_CalculateMAC(SGD_HANDLE hSessionHandle, SGD_HANDLE hKeyHandle, SGD_UINT32 uiAlgID, SGD_UCHAR *pucIV, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength, SGD_UCHAR *pucMAC, \
								   SGD_UINT32 *puiMACLength);

/*�Ӵ������ຯ��*/
SGD_RV SDF_HashInit(SGD_HANDLE hSessionHandle, SGD_UINT32 uiAlgID, ECCrefPublicKey *pucPublicKey, SGD_UCHAR *pucID, SGD_UINT32 uiIDLength);
SGD_RV SDF_HashUpdate(SGD_HANDLE hSessionHandle, SGD_UCHAR *pucData, SGD_UINT32 uiDataLength);
SGD_RV SDF_HashFinal(SGD_HANDLE hSessionHandle, SGD_UCHAR *pucHash, SGD_UINT32 *puiHashLength);

/*�ļ������ຯ��*/
SGD_RV SDF_CreateFile(SGD_HANDLE hSessionHandle, SGD_UCHAR *pucFileName, SGD_UINT32 uiNameLen, SGD_UINT32 uiFileSize);
SGD_RV SDF_ReadFile(SGD_HANDLE hSessionHandle, SGD_UCHAR *pucFileName, SGD_UINT32 uiNameLen, SGD_UINT32 uiOffset, SGD_UINT32 *puiReadLength, SGD_UCHAR *pucBuffer);
SGD_RV SDF_WriteFile(SGD_HANDLE hSessionHandle, SGD_UCHAR *pucFileName, SGD_UINT32 uiNameLen, SGD_UINT32 uiOffset, SGD_UINT32 uiWriteLength, SGD_UCHAR *pucBuffer);
SGD_RV SDF_DeleteFile(SGD_HANDLE hSessionHandle, SGD_UCHAR *pucFileName, SGD_UINT32 uiNameLen);

#ifdef __cplusplus
}
#endif

#endif /*#ifndef _ISL_SDS_H_*/

