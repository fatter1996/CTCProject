#include "ModifyRelevantStationsKSK.h"
#pragma execution_character_set("utf-8")


namespace CTCWindows {
	namespace CASCO {
		ModifyRelevantStationsKSK::ModifyRelevantStationsKSK(Station::StaTrafficLog* m_pCurTrafficLog, QWidget* parent)
			: QWidget(parent)
		{
			ui.setupUi(this);

			ui.label->setText(QString("%1").arg(m_pCurTrafficLog->m_strArrivalTrainNum) + "次列车");
			
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
				QString strDirection;
				QMap<QString, QByteArray> m_mapRoute;
				QVector<Station::StaTrainRoute*> vecTrainRoute = Station::MainStation()->getStaTrainRouteByTrain(m_pCurTrafficLog->m_nTrainId);
				for (Station::StaTrainRoute* pTrainRoute : Station::MainStation()->getStaTrainRouteByTrain(m_pCurTrafficLog->m_nTrainId)) {
					for (Station::StaTrainRoute* pSubTrainRoute : pTrainRoute->getSubTrainRouteList()) {
						if (pTrainRoute->m_bArrivaRoute) {
							strDirection = ui.Come->currentText();
						}
						else {
							strDirection = ui.Sail->currentText();
						}
						
						m_mapRoute = {
							{ "direction", strDirection.toLocal8Bit() }
						};
						if (Http::HttpClient::UpdataStaTrainRouteAttr(pSubTrainRoute->m_nRouteId, m_mapRoute, btResult)) {
							pSubTrainRoute->m_strDirection = strDirection;
						}
					}
				}
				emit  Station::MainStation()->TrainRouteUpData();
				this->close();
			});
			connect(ui.Cancel, &QPushButton::clicked, this, &ModifyRelevantStationsKSK::close);
		}

		ModifyRelevantStationsKSK::~ModifyRelevantStationsKSK()
		{}
	}
}