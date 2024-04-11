#pragma once

#include "StaFunBtnToolBarTKY.h"
#include "../BaseWndClass/StationCtrlDisp.h"
#include "ui_StationCtrlDispTKY.h"

namespace CTCWindows {
	class StationCtrlDispTKY : public StationCtrlDisp
	{
		Q_OBJECT

	public:
		StationCtrlDispTKY(QWidget* parent = nullptr);
		~StationCtrlDispTKY();

	public:
		void CreatStaFunBtnToolBar();

	public:
		QWidget* GetStationPaintView();

	public:
		void timerEvent(QTimerEvent* event);

	private:
		Ui::StationCtrlDispTKYClass ui;
		int m_nTimerId_500 = -1;
	};
}
