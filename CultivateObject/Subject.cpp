#include "Subject.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CultivateObject {
	QMap<SubjectType, Subject::ConversionFunction> Subject::m_mapConversion;
	QVector<Station::Device::DeviceBase*> Subject::m_vacSubjectTipDev;

	void Subject::ConversionFunction::operator()(const QStringList& strNameList, QByteArray& btAnswerArray) const {
		SetSubjectDevArray(nType, nObject, strNameList, btAnswerArray);
	}

	Subject::Subject(int nSubjectId, QByteArray btTestPaperNum)
	{
		m_nCultivateType = 0x01; 
		m_nSubjectId = nSubjectId;	
		m_btTestPaperNum = btTestPaperNum;
		QString m_strFlowId;	
	}

	Subject::Subject(int nSubjectId, QByteArray btTestPaperNum, QString strFlowId)
	{
		m_nCultivateType = 0x02;
		m_nSubjectId = nSubjectId;
		m_btTestPaperNum = btTestPaperNum;
		m_strFlowId = strFlowId;
	}

	Subject::~Subject()
	{
	}

	void Subject::InitConversionFun()
	{
		m_mapConversion = {
			//信号机相关操作 
			{SubjectType::LCJL, {0x01, 0x01}},
			{SubjectType::DCJL, {0x01, 0x02}},
			{SubjectType::TGJL, {0x01, 0x03}},
			{SubjectType::XHFS, {0x0A, 0x13}},
			{SubjectType::LAFS, {0x0A, 0x01}},
			{SubjectType::DAFS, {0x0A, 0x02}},
			{SubjectType::TAFS, {0x0A, 0x03}},
			{SubjectType::XHJF, {0x0B, 0x13}},
			{SubjectType::LAJF, {0x0B, 0x01}},
			{SubjectType::DAJF, {0x0B, 0x02}},
			{SubjectType::TAJF, {0x0B, 0x03}},
			{SubjectType::YDJL, {0x02, 0x05}},
			//道岔相关操作
			{SubjectType::DCDC, {0xF7, 0x11}},
			{SubjectType::DCDW, {0x06, 0x11}},
			{SubjectType::DCFW, {0x07, 0x11}},
			{SubjectType::DCDS, {0x08, 0x11}},
			{SubjectType::DCDJ, {0x09, 0x11}},
			{SubjectType::DCCF, {0x0A, 0x11}},
			{SubjectType::DCJF, {0x0B, 0x11}},
			{SubjectType::DCFLBL, {0x11, 0x11}},
			// 其他
			{SubjectType::FLBL, {0x11, 0x12}},
			{SubjectType::JLZQX, {0x04, 0x13}},
			{SubjectType::JLZRJ, {0x04, 0x13}},
			{SubjectType::QGJ, {0x0d, 0x14}},
			{SubjectType::YDZS, {0x05, 0x21}}
		};
	}

	void Subject::SetSubjectDevArray(int nType, int nObject, const QStringList& strNameList, QByteArray& btAnswerArray)
	{
		btAnswerArray[1] = nType;
		btAnswerArray[2] = nObject;
		Station::Device::DeviceBase* pDevice = nullptr;
		QByteArray btDeviceCode;
		for (QString strName : strNameList) {
			pDevice = Station::MainStation()->getDeviceByName(strName);
			if (!pDevice) {
				break;
			}
			btAnswerArray.append(pDevice->getCode() % 256);
			btAnswerArray.append(pDevice->getCode() / 256);
			if (nType == 0xF7) {	//道岔单操特殊处理
				if (pDevice->getState() & SWITCH_STATE_DW) {
					btAnswerArray[1] = 0x07;
				}
				else if (pDevice->getState() & SWITCH_STATE_FW) {
					btAnswerArray[1] = 0x06;
				}
			}
		}
		if (nType == 0x05) {	//引导总锁特殊处理
			if (strNameList[0] == "S") {
				btAnswerArray[2] = 0x21;
			}
			else if (strNameList[0] == "X") {
				btAnswerArray[2] = 0x22;
			}
		}
		btAnswerArray.replace(4, strNameList.size() * 2, btDeviceCode);
	}

	void Subject::SetDevSubject(QStringList strNameList, SubjectType nType)
	{
		int nIndex = 1;
		Station::Device::DeviceBase* pDevice = nullptr;
		if (nType == SubjectType::LCJL || nType == SubjectType::DCJL || nType == SubjectType::TGJL) {	//进路办理
			for (QString strName : strNameList) {
				pDevice = Station::MainStation()->getDeviceByName(strName);
				if (pDevice) {
					pDevice->setShowTips(static_cast<int>(nType) | (nIndex << 4));
					nIndex++;
				}
			}
		}
		else if (nType == SubjectType::YDZS) {
			pDevice = Station::MainStation()->getDeviceByName(strNameList[0] + "引导总锁");
			if (pDevice) {
				pDevice->setShowTips(static_cast<int>(nType));
			}
		}
		else {
			for (QString strName : strNameList) {
				pDevice = Station::MainStation()->getDeviceByName(strName);
				if (pDevice) {
					pDevice->setShowTips(static_cast<int>(nType));
				}
			}
		}
		if (pDevice) {
			m_vacSubjectTipDev.append(pDevice);
		}
	}

	void Subject::ClearTips()
	{
		for (Station::Device::DeviceBase* pDevice : m_vacSubjectTipDev) {
			pDevice->setShowTips(0);
		}
		QVector<Station::Device::DeviceBase*>().swap(m_vacSubjectTipDev);
	}

	void Subject::SetSubjectArray(QStringList strNameList, SubjectType nType)
	{
		QByteArray btAnswerArray;
		btAnswerArray.resize(strNameList.size() * 2 + 4);
		btAnswerArray[0] = 0x40;
		btAnswerArray[3] = strNameList.size();
		m_mapConversion[nType](strNameList, btAnswerArray);
		m_vecAnswerArray.append(btAnswerArray);
		m_bResult &= false;
	}

	void Subject::CompareResult(const QByteArray& btResult)
	{
		for (int i = 0; i < this->m_vecAnswerArray.size(); i++) {
			if (btResult.mid(10, btResult.length() - 14) == m_vecAnswerArray.at(i)) {
				m_nTrueResultNum++;
				m_vecAnswerArray[i] = QByteArray();
			}
		}
		m_bResult = (m_nTrueResultNum == m_vecAnswerArray.size());
	}
}