#pragma once
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Socket {

	class SerialPort{
	
	public:
		SerialPort();
		~SerialPort();

	public:
		//初始化串口
		void InitSerial(const QString& strSerialName);
		QSerialPort* Serial() { return m_pSerial; }
	private:

		QSerialPort* m_pSerial = nullptr;
	};
}