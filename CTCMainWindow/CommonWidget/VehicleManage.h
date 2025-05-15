#pragma once

#include <QWidget>
#include "ui_VehicleManage.h"
#include <CustomControl/CarLocationEdit.h>

namespace CTCWindows {
	class VehicleManage : public QWidget
	{
		Q_OBJECT
	public:
		VehicleManage(QWidget* parent = nullptr);
		~VehicleManage();
		void timerEvent(QTimerEvent* event);
		void InitConnect();
		void InitRadio();

	private:
		int TimerId = 0;
	private:
		CTCWindows::Control::CarLocationEdit* m_pCarLocation = nullptr;
		Ui::VehicleManageClass ui;
	};
}