#include "ModifyTrainNumberKSK.h"

ModifyTrainNumberKSK::ModifyTrainNumberKSK(Station::StaTrafficLog* m_pCurTrafficLog,QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle("�޸ĳ��κ�");
	ui.setupUi(this);
	ui.comboBox->addItem(Station::MainStation()->getStationName());

	ui.lineEdit->setText(m_pCurTrafficLog->m_strArrivalTrainNum); 
	ui.lineEdit_2->setText(m_pCurTrafficLog->m_strDepartTrainNum);

	connect(ui.pushButton, &QPushButton::clicked, [=]() {
		QVector<Station::StaTrainRoute*> vecTrainRoute;
		m_pCurTrafficLog->m_strArrivalTrainNum = ui.lineEdit_3->text();
		m_pCurTrafficLog->m_strDepartTrainNum = ui.lineEdit_4->text();//�г���־�޸ĳ��κ�
		vecTrainRoute = Station::MainStation()->getStaTrainRouteByTrain(m_pCurTrafficLog->m_nTrainId);//�г���·����

		Station::MainStation()->getStaTrainById(m_pCurTrafficLog->m_nTrainId)->m_strTrainNum = ui.lineEdit_3->text();//������Ϣ�޸ĳ��κ�

		});
	}

ModifyTrainNumberKSK::~ModifyTrainNumberKSK()
{}
