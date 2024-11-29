#pragma once
#include <QObject>
#include <QDateTime>
#include "../StationObject/Device/StaDevice.h"

namespace CultivateObject {

	enum class SubjectType : int {
		LCJL = 0x01,
		DCJL,
		TGJL,
		XHFS,
		LAFS,
		DAFS,
		TAFS,
		XHJF,
		LAJF,
		DAJF,
		TAJF,
		YDJL,
		DCFLBL,

		DCDC = 0x10,
		DCDW,
		DCFW,
		DCDS = 0x14,
		DCDJ,
		DCCF,
		DCJF,

		FLBL = 0x20,
		JLZQX,
		JLZRJ,
		QGJ,
		YDZS
	};

	//����
	class Subject
	{
		struct ConversionFunction {
			uint8_t nType;
			uint8_t nObject;
			void operator()(const QStringList& strNameList, QByteArray& btAnswerArray) const;
		};

	public:
		Subject(int nSubjectId, QByteArray btTestPaperNum);
		Subject(int nSubjectId, QByteArray btTestPaperNum, QString strFlowId);
		~Subject();

	public:
		void SetSubjectArray(QStringList strNameList, SubjectType nType);
		void CompareResult(const QByteArray& btResult);

	private:
		static void SetSubjectDevArray(int nType, int nObject, const QStringList& strNameList, QByteArray& btAnswerArray);

	public:
		static void InitConversionFun();
		static void SetDevSubject(QStringList strNameList, SubjectType nType);
		static void ClearTips();
		
	public:
		void setTimerId(int nTimerId) { m_nTimerId = nTimerId;}
		int getTimerId() const { return m_nTimerId; }
		int getCultivateType() const { return m_nCultivateType; }
		int getSubjectId() const { return m_nSubjectId; }
		QByteArray getTestPaperNum() const { return m_btTestPaperNum; }
		QString getFlowId() const { return m_strFlowId; }
		void setResult(bool bResult) { m_bResult &= bResult; }
		bool getResult() const { return m_bResult; }

	private:
		static QMap<SubjectType, ConversionFunction> m_mapConversion;
		static QVector<Station::Device::DeviceBase*> m_vacSubjectTipDev;
		
	private:
		int m_nCultivateType = 0; //��������(0x01 - ��������, 0x02 - -���̲���)
		int m_nSubjectId = 0;	//������
		QByteArray m_btTestPaperNum;	//�Ծ���
		QString m_strFlowId;	//���̱��
		int m_nTrueResultNum = 0;
		int m_nTimerId = 0;
		bool m_bResult = true;
		QVector<QByteArray> m_vecAnswerArray;
	};
}