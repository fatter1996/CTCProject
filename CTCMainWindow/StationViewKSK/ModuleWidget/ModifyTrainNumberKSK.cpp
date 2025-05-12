#include "ModifyTrainNumberKSK.h"

ModifyTrainNumberKSK::ModifyTrainNumberKSK(Station::StaTrafficLog* m_pCurTrafficLog,QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle("修改车次号");
	ui.setupUi(this);
	ui.comboBox->addItem(Station::MainStation()->getStationName());

	ui.lineEdit->setText(m_pCurTrafficLog->m_strArrivalTrainNum); 
	ui.lineEdit_2->setText(m_pCurTrafficLog->m_strDepartTrainNum);

	connect(ui.pushButton, &QPushButton::clicked, [=]() {
		QVector<Station::StaTrainRoute*> vecTrainRoute;
		m_pCurTrafficLog->m_strArrivalTrainNum = ui.lineEdit_3->text();
		m_pCurTrafficLog->m_strDepartTrainNum = ui.lineEdit_4->text();//行车日志修改车次号
		vecTrainRoute = Station::MainStation()->getStaTrainRouteByTrain(m_pCurTrafficLog->m_nTrainId);//列车进路序列

		Station::MainStation()->getStaTrainById(m_pCurTrafficLog->m_nTrainId)->m_strTrainNum = ui.lineEdit_3->text();//车次信息修改车次号

		});
	}

ModifyTrainNumberKSK::~ModifyTrainNumberKSK()
{}
