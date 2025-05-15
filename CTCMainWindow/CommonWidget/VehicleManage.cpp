#include "VehicleManage.h"
#include <QIcon>
#include <QDateTime>
#include <QDebug>
#pragma execution_character_set("utf-8")
#include <QPushButton>

namespace CTCWindows {
	VehicleManage::VehicleManage(QWidget* parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		setWindowIcon(QIcon(":/CTCProject/icon/icon.ico"));
		setWindowTitle("现存车管理");
		ui.radioButton->setEnabled(false);
		ui.radioButton_2->setEnabled(false);
		ui.radioButton_3->setEnabled(false);
		ui.radioButton_4->setEnabled(false);
		InitConnect();
		InitRadio();
		TimerId = startTimer(1000);
	}

	VehicleManage::~VehicleManage()
	{}

	void VehicleManage::timerEvent(QTimerEvent* event)
	{
		if (TimerId == event->timerId()) {
			InitRadio();
		}
		return QWidget::timerEvent(event);
	}

	void VehicleManage::InitConnect()
	{
		connect(ui.locationedit, &QPushButton::clicked, [&]() {
			if (m_pCarLocation == nullptr) {
				m_pCarLocation = new CTCWindows::Control::CarLocationEdit;
			}
			m_pCarLocation->show();
			});
		connect(ui.quit, &QPushButton::clicked, [&]() {close(); });
		connect(ui.read, &QPushButton::clicked, [&]() {});
		connect(ui.phrase, &QPushButton::clicked, [&]() {});
		connect(ui.readdition, &QPushButton::clicked, [&]() {});
		connect(ui.interposition, &QPushButton::clicked, [&]() {});
		connect(ui.edit, &QPushButton::clicked, [&]() {});
		connect(ui.delete_2, &QPushButton::clicked, [&]() {});
		connect(ui.report, &QPushButton::clicked, [&]() {});
	}

	void VehicleManage::InitRadio()
	{
		QDateTime currentDateTime = QDateTime::currentDateTime();
		QString timeStr = currentDateTime.toString("hh");
		int time = timeStr.toInt();
		if (time < 6 && time >= 0) {
			ui.radioButton->setChecked(true);
			ui.radioButton_2->setChecked(false);
			ui.radioButton_3->setChecked(false);
			ui.radioButton_4->setChecked(false);
		}
		else if (time >= 6 && time < 12) {
			ui.radioButton->setChecked(false);
			ui.radioButton_2->setChecked(true);
			ui.radioButton_3->setChecked(false);
			ui.radioButton_4->setChecked(false);
		}
		else if (time >= 12 && time < 18) {
			ui.radioButton->setChecked(false);
			ui.radioButton_2->setChecked(false);
			ui.radioButton_3->setChecked(true);
			ui.radioButton_4->setChecked(false);
		}
		else if (time >= 18) {
			ui.radioButton->setChecked(false);
			ui.radioButton_2->setChecked(false);
			ui.radioButton_3->setChecked(false);
			ui.radioButton_4->setChecked(true);
		}
	}
}