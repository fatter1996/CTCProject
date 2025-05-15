#include "ModifyTrainNumberKSK.h"


namespace CTCWindows {
	namespace CASCO {
		ModifyTrainNumberKSK::ModifyTrainNumberKSK(Station::StaTrafficLog* m_pCurTrafficLog, QWidget* parent)
			: QWidget(parent)
		{
			setWindowTitle("ÐÞ¸Ä³µ´ÎºÅ");
			ui.setupUi(this);
			ui.stationName->addItem(Station::MainStation()->getStationName());

			ui.OldArrival->setText(m_pCurTrafficLog->m_strArrivalTrainNum);
			ui.OldSetOff->setText(m_pCurTrafficLog->m_strDepartTrainNum);

			connect(ui.Confirm, &QPushButton::clicked, [=]() {
				
				QByteArray btResult;
				Station::StaTrain* pTrain = Station::MainStation()->getStaTrainById(m_pCurTrafficLog->m_nTrainId);
				if (!pTrain) {
					return;
				}
				QString strTrainNum = (m_pCurTrafficLog->m_nPlanType == 0x02) ? ui.Arrival->text() : ui.OldSetOff->text();
				if (!Station::MainStation()->ChangeTrainNum(pTrain, strTrainNum)) {
					return;
				}

				QMap<QString, QByteArray> m_mapLogValue = { 
					{ "arrivalTrainNumber", ui.Arrival->text().toLocal8Bit() },
					{ "departTrainNumber", ui.Setoff->text().toLocal8Bit() }
				};
				if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
					m_pCurTrafficLog->m_strArrivalTrainNum = ui.Arrival->text();
					m_pCurTrafficLog->m_strDepartTrainNum = ui.Setoff->text();
					emit Station::MainStation()->TrafficLogTableUpData();
				}
				emit  Station::MainStation()->TrainRouteUpData();
				this->close();
			});
			connect(ui.Cancel, &QPushButton::clicked, this, &ModifyTrainNumberKSK::close);
		}

		ModifyTrainNumberKSK::~ModifyTrainNumberKSK()
		{}
	}
}