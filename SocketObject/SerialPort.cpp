#include "SerialPort.h"

namespace Socket {

	SerialPort::SerialPort()
	{
		m_pSerial = new QSerialPort;
	}

	SerialPort::~SerialPort()
	{
        m_pSerial->clear();
        m_pSerial->close();
        m_pSerial->deleteLater();
	}

	//初始化串口
	void SerialPort::InitSerial(const QString& strSerialName)
	{
        //设置串口名
        m_pSerial->setPortName(strSerialName);
        //打开串口
        m_pSerial->open(QIODevice::ReadWrite);
        //设置波特率
        m_pSerial->setBaudRate(QSerialPort::Baud115200);//设置波特率为115200
        //设置数据位数
        m_pSerial->setDataBits(QSerialPort::Data8);//设置数据位8
        //设置校验位
        m_pSerial->setParity(QSerialPort::NoParity);
        //设置停止位
        m_pSerial->setStopBits(QSerialPort::OneStop);//停止位设置为1
        //设置流控制
        m_pSerial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制
	}
}