/**
@file fesdev.h
@brief �豸ͨѶ�ӿڿ�

�ṩTCP��UDP�����ڡ�MOXA���豸��ͨ��ͨѶ�ӿڣ�ʹ���ߵ���

@author qianjin 
@version 1.0.0
@date 2012-10-22
*/

#ifndef _FES_DEV_H_
#define _FES_DEV_H_

#include <ace/OS.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>
#include "ace/DEV_Addr.h"
#include <ace/DEV_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/TTY_IO.h>

#ifdef WIN32
#ifdef FESDEV_DLL
#define	FESDEV_EXPORT __declspec(dllexport) 
#else
#define	FESDEV_EXPORT __declspec(dllimport) 
#endif
#else
#define	FESDEV_EXPORT
#endif

/**
@brief �豸���Ͷ���
*/
enum DEV_TYPE
{
	NET_DEV = 0,
	SERIAL_DEV,
	TCP_CLINET_DEV,
	TCP_SERVER_DEV,
	UDP_DEV
};

/**
@brief �豸��������

Dev_base���ṩ�Ĵ򿪡��رա���д�ӿ�

Dev_base����������ͨѶ�豸��ͨ�ýӿڣ��ӿڵ�ʵ�������������

������豸������ɡ�
*/
class FESDEV_EXPORT Dev_base
{
public:
	virtual ~Dev_base();
	/**
	@brief ���豸
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	virtual int open_dev(void) = 0;
	/**
	@brief �ر��豸
	*/
	virtual void close_dev(void) = 0;
	/**
	@brief ���豸
	@param out unsigned char* buf ���ջ�����
	@param in int len Ҫ��ȡ���ֽ���,
	@return �������ֽ��������ɶ�ʱ����0��ʧ�ܷ���-1
	*/
	virtual int read_dev(unsigned char* buf, int len) = 0;
	/**
	@brief д�豸
	@param in unsigned char* buf ������
	@param in int len Ҫд����ֽ���
	@return �ɹ�д����ֽ���������дʱ����0��ʧ�ܷ���-1
	*/
	virtual int write_dev(unsigned char* buf, int len) = 0;
	/**
	@brief ��ȡ״̬
	@return ��״̬����0���ر�״̬����-1
	*/
	virtual int get_stat(void) = 0;

	/**
	@brief ��ȡ���������
	@return ���������
	*/
	char* get_error(void);

	
protected:
	int valid_addr_num;		//��Ч��ַ�ĸ���
	int addr_index;			//��ǰʹ�õ�ַ������
};

class FESDEV_EXPORT Net_dev : public Dev_base
{
public:
	enum
	{
		NET_NUM = 2,
		DEVICE_TX_BUF_SIZE = 16384,
		DEVICE_RX_BUF_SIZE = 16384,
		SOCK_TIMEOUT = 5,
		CLOSE_RETRY = 5
	};

public:
	Net_dev(const char* dev1, const int& port1, const char* dev2, const int& port2);

	~Net_dev();
protected:
	ACE_INET_Addr addr[NET_NUM];
};

/**
@brief Udp�ӿ���
*/
class FESDEV_EXPORT Udp_dev: public Net_dev
{
	
public:
	/**
	@brief ���캯��
	@param in const char* dev1 ��ַ1
	@param in const int& port1 ��ַ1�Ķ˿�
	@param in const char* dev2 = NULL ��ַ2
	@param in const const int& port2 ��ַ2�Ķ˿�
	*/
	Udp_dev(const char* local_dev, const int& local_port, const char* dev1, 
		    const int& port1, const char* dev2, const int& port2);
	virtual int open_dev(void);
	virtual void close_dev(void);
	virtual int read_dev(unsigned char* buf, int len);
	virtual int write_dev(unsigned char* buf, int len); 
	virtual int get_stat(void);		//add by chris

private:
	ACE_INET_Addr local_addr;
	ACE_SOCK_Dgram peer;

};

/**
@brief Tcp�ͻ��˽ӿ���

*/
class FESDEV_EXPORT Tcp_client: public Net_dev
{
public:
	/**
	@brief ���캯��
	@param in const char* dev1 ��ַ1
	@param in const int& port1 ��ַ1�Ķ˿�
	@param in const char* dev2 = NULL ��ַ2
	@param in const const int& port2 ��ַ2�Ķ˿�
	*/
	Tcp_client(const char* dev1, const int& port1, const char* dev2, const int& port2);
	virtual ~Tcp_client();

	virtual int open_dev(void);
	virtual void close_dev(void);
	virtual int read_dev(unsigned char* buf, int len);
	virtual int write_dev(unsigned char* buf, int len); 
	virtual int get_stat(void);

protected:
	/**
	@brief ����socket�Ĳ���
	*/
	void set_fd_opt(void) const;

protected:
	ACE_SOCK_Stream peer;
};

/**
@brief Tcp����˽ӿ���

*/
class FESDEV_EXPORT Tcp_server: public Tcp_client
{
public:
	/**
	@brief ���캯��

	@param in const char* local_ip ���ؼ�����IP��ַ�����ΪNULL���������������IP
	@param in const int& local_port ���ؼ����Ķ˿ں�
	@param in const char* peer_ip1 = NULL �������ӵĶԶ�IP��ַ1
	@param in const char* peer_ip2 = NULL �������ӵĶԶ�IP��ַ2
	*/
	Tcp_server(const char* local_ip, const int& local_port, const char* peer_ip1 = NULL, const char* peer_ip2 = NULL);
	~Tcp_server();

	virtual int open_dev(void);
	/**
	@brief �����˿�
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int listen_port(void);

	int close_listen(void);
private:
	ACE_SOCK_Acceptor acceptor;
	ACE_INET_Addr local_addr;
	bool listen_flag ;
};

/**
@brief Moxa���ڷ������ӿ���
*/
class FESDEV_EXPORT Moxa_dev: public Tcp_client
{
public:
	Moxa_dev(const char* dev1, const int& port1, const char* dev2, const int& port2);
};

/**
@brief ���ڽӿ���

ACE���ڵ�ʹ�òο�ACE_wrappers\examples\IPC_SAP\DEV_SAP
*/
class FESDEV_EXPORT Serial: public Dev_base
{
public:
	enum
	{
		DEV_NUM = 2,
		READ_TIME_OUT = 10 * 1000, //����ʱʱ��10��
		CLOSE_RETRY = 5
	};
public:
	Serial(const char* dev1, const char* dev2, const int& band_rate, 
		   const int& check_bit, const int& stop_bit, const int& data_bit, const int& flow);
	virtual int open_dev(void);
	virtual void close_dev(void);
	virtual int read_dev(unsigned char* buf, int len);
	virtual int write_dev(unsigned char* buf, int len);
	virtual int get_stat(void);
private:
	ACE_TTY_IO::Serial_Params param;
	ACE_DEV_Connector connector;
	ACE_TTY_IO dev;
	ACE_DEV_Addr dev_addr[DEV_NUM];
};
#endif