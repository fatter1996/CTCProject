#include "ModifyTrainNumberKSK.h"


namespace CTCWindows {
	namespace CASCO {
		ModifyTrainNumberKSK::ModifyTrainNumberKSK(Station::StaTrafficLog* m_pCurTrafficLog, QWidget* parent)
			: QWidget(parent)
		{
			setWindowTitle("�޸ĳ��κ�");
			ui.setupUi(this);
			ui.stationName->addItem(Station::MainStation()->getStationName());

			ui.OldArrival->setText(m_pCurTrafficLog->m_strArrivalTrainNum);
			ui.OldSetOff->setText(m_pCurTrafficLog->m_strDepartTrainNum);

			connect(ui.Confirm, &QPushButton::clicked, [=]() {
				m_pCurTrafficLog->m_strArrivalTrainNum = ui.Arrival->text();
				m_pCurTrafficLog->m_strDepartTrainNum = ui.Setoff->text();//�г���־�޸ĳ��κ�
			
				Station::MainStation()->getStaTrainById(m_pCurTrafficLog->m_nTrainId)->m_strTrainNum = ui.Arrival->text();//������Ϣ�޸ĳ��κ�
				QByteArray btResult;
				QMap<QString, QByteArray>m_mapLogValue = { {"arrivalTrainNumber",m_pCurTrafficLog->m_strArrivalTrainNum.toLocal8Bit()},
				{"departTrainNumber",m_pCurTrafficLog->m_strDepartTrainNum.toLocal8Bit()} };
				if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
					emit Station::MainStation()->TrafficLogTableUpData();
				}
				//Station::MainStation()->ChangeStaTrafficLogData(m_pCurTrafficLog);
				emit  Station::MainStation()->TrainRouteUpData();
				this->close();
				});
			connect(ui.Cancel, &QPushButton::clicked, [=]() {
				this->close();
				});
		}

		ModifyTrainNumberKSK::~ModifyTrainNumberKSK()
		{}
	}
}