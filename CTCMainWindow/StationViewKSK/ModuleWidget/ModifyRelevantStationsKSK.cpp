#include "ModifyRelevantStationsKSK.h"
#pragma execution_character_set("utf-8")
ModifyRelevantStationsKSK::ModifyRelevantStationsKSK(Station::StaTrafficLog* m_pCurTrafficLog, QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	ui.label->setText(QString("%1").arg(m_pCurTrafficLog->m_strArrivalTrainNum) + "次列车");
	QVector<Station::StaTrainRoute*> vecTrainRoute;
	vecTrainRoute = Station::MainStation()->getStaTrainRouteByTrain(m_pCurTrafficLog->m_nTrainId);
	setWindowTitle("修改相关邻站");
	Station::Device::StaSignal* pSignal = nullptr;
	for (Station::Device::DeviceBase* pDevice : Station::MainStation()->getDeviceVectorByType(SIGNALLAMP)) {
		pSignal = dynamic_cast<Station::Device::StaSignal*>(pDevice);
		if (pSignal->getXHDTYpe() == "JZ_XHJ") {
			int index = ui.comboBox->findText(pSignal->getDirection()+"-->");
			int index_2 = ui.comboBox_2->findText("-->"+pSignal->getDirection());

			if (index == -1) {
				ui.comboBox->addItem(pSignal->getDirection()+"-->");
			}
			if (index_2 == -1) {
				ui.comboBox_2->addItem("-->"+pSignal->getDirection());
			}
		}
	}
	ui.comboBox->addItem(pSignal->getDirection());
	int index = ui.comboBox->findText(vecTrainRoute[0]->m_strDirection);
	int index_2 = ui.comboBox_2->findText(vecTrainRoute[1]->m_strDirection);
	// 如果找到，则设置为当前项
	if (index != -1) {
		ui.comboBox->setCurrentIndex(index);
	}
	if (index_2 != -1)
	{
		ui.comboBox_2->setCurrentIndex(index_2);
	}
	connect(ui.pushButton, &QPushButton::clicked, [=]() {
		vecTrainRoute[0]->m_strDirection == ui.comboBox->currentText();
		vecTrainRoute[1]->m_strDirection == ui.comboBox_2->currentText();
		});

}

ModifyRelevantStationsKSK::~ModifyRelevantStationsKSK()
{}
