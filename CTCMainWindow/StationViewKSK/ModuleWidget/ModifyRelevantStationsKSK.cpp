#include "ModifyRelevantStationsKSK.h"
#pragma execution_character_set("utf-8")


namespace CTCWindows {
	namespace CASCO {
		ModifyRelevantStationsKSK::ModifyRelevantStationsKSK(Station::StaTrafficLog* m_pCurTrafficLog, QWidget* parent)
			: QWidget(parent)
		{
			ui.setupUi(this);

			ui.label->setText(QString("%1").arg(m_pCurTrafficLog->m_strArrivalTrainNum) + "次列车");
			QVector<Station::StaTrainRoute*> vecTrainRoute;
			vecTrainRoute = Station::MainStation()->getStaTrainRouteByTrain(m_pCurTrafficLog->m_nTrainId);
			Station::Device::StaSignal* pArrivalSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByCode(m_pCurTrafficLog->m_nArrivalSignalCode));
			Station::Device::StaSignal* pDepartSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByCode(m_pCurTrafficLog->m_nDepartSignalCode));

			setWindowTitle("修改相关邻站");
			Station::Device::StaSignal* pSignal = nullptr;
			for (Station::Device::DeviceBase* pDevice : Station::MainStation()->getDeviceVectorByType(SIGNALLAMP)) {
				pSignal = dynamic_cast<Station::Device::StaSignal*>(pDevice);
				if (pSignal->getXHDTYpe() == "JZ_XHJ") {
					int index = ui.Come->findText(pSignal->getDirection() + "-->");
					int index_2 = ui.Sail->findText("-->" + pSignal->getDirection());

					if (index == -1) {
						ui.Come->addItem(pSignal->getDirection() + "-->");
					}
					if (index_2 == -1) {
						ui.Sail->addItem("-->" + pSignal->getDirection());
					}
				}
			}
			int index = ui.Come->findText(pArrivalSignal->getDirection() + "-->");
			int index_2 = ui.Sail->findText("-->" + pDepartSignal->getDirection());
			// 如果找到，则设置为当前项
			if (index != -1) {
				ui.Come->setCurrentIndex(index);
			}
			if (index_2 != -1)
			{
				ui.Sail->setCurrentIndex(index_2);
			}
			connect(ui.Confirm, &QPushButton::clicked, [=]() {

				QByteArray btResult;
				vecTrainRoute[0]->m_strDirection = ui.Come->currentText();
				vecTrainRoute[1]->m_strDirection = ui.Sail->currentText();
				for (int i = 0; i < vecTrainRoute.size(); i++) {
					QMap<QString, QByteArray> m_mapRoute = { {"direction",vecTrainRoute[i]->m_strDirection.toLocal8Bit()} };
					Http::HttpClient::ChangeStaTraffRouteData(vecTrainRoute[i]->m_nRouteId, m_mapRoute, btResult);

				}
				emit  Station::MainStation()->TrainRouteUpData();
				this->close();
				});
			connect(ui.Cancel, &QPushButton::clicked, [=]() {
				this->close();
				});

		}

		ModifyRelevantStationsKSK::~ModifyRelevantStationsKSK()
		{}
	}
}