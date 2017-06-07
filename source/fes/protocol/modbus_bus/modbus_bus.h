#ifndef MODBUS_BUS_H
#define MODBUS_BUS_H

#include "modbus_bus_global.h"
#include "modbus_bus_define.h"
#include <vector>
#include <QTime>

class MODBUS_BUS_EXPORT CMbBusProto:public Base_protocol
{
public:
	CMbBusProto(const int& channel);
	~CMbBusProto();
public:
	virtual void init_protocol();
	virtual void protocol_proc(void);		
protected:
	virtual int send_yt_frame(unsigned char ytno, float set_value, unsigned char yt_flag);
	virtual int send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag)
	{
		return 0;
	}
	virtual int send_WaveCommand(unsigned char Type, unsigned char* cmd_buf)
	{
		return 0;
	}
	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value)
	{
		return 0;
	}
	virtual int send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd)
	{
		return 0;
	}
protected:
	int SendMbBuff(int nType,int nIndex);
	int ProcMbBuff(int nType,int nIndex);
	int GetFullMsg();
	int GetFunCode(int nType);
protected:
	int ParaseAnalog( int nIndex );
	int ParaseKwh( int nIndex );
	unsigned short CRC16(unsigned char* pBuff,unsigned long len);
private:
	int m_nTotalYXNum;
	int m_nTotalYCNum;
	int m_nTotalKwhNum;

	QTime m_TimeOut;
	QTime m_YcTimer ;
	QTime m_YxTimer ;
	QTime m_KwhTimer;

	bool m_bYCCallEnable ;
	bool m_bYXCallEnable ;
	bool m_bKwhCallEnable ;

private:
	std::vector<int> m_arrAnalogStartAddr;//��ʼ��ַ����(�����е㰴��������ÿ֡����PER_FRAME_*�����ݣ�ÿ֡��һ�����ݵ���ʼ��ַ)
	std::vector<int> m_arrAnalogCountPerFrame;

	std::vector<int> m_arrBinaryStartAddr;
	std::vector<int> m_arrBinaryCountPerFrame;

	std::vector<int> m_arrKwhStartAddr;
	std::vector<int> m_arrKwhCountPerFrame;

	unsigned short m_sCommAddr;
	unsigned char m_Buff[MAX_BUFF_LENGTH];
	const CModbusParaConfig* const	m_pConfig;		//��Լ�����޷��޸�����
};

/**
@brief ��ȡ��Լʵ����C���ӿ�
*/
extern "C" MODBUS_BUS_EXPORT Base_protocol* get_protocol(const int& channel);

/**
@brief ��ȡ���ݿ����õ�C���ӿ�
*/
extern "C" MODBUS_BUS_EXPORT void get_config(void);

/**
@brief ��ȡ�汾��C���ӿ�
*/
extern "C" MODBUS_BUS_EXPORT char* get_version(void);

/**
@brief ��ȡ������Ϣ��C���ӿ�
*/
extern "C" MODBUS_BUS_EXPORT char* get_desc(void);

#endif // MODBUS_BUS_H
