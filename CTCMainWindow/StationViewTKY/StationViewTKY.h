#pragma once

#include "CTCMainWindow/CTCMainWindow.h"
#include "StationCtrlDispTKY.h"
#include "StationMultiDispTKY.h"
#include "StationLogDispTKY.h"
#include "ui_StationViewTKY.h"

namespace CTCWindows {
	class StationViewTKY : public CTCMainWindow
	{
		Q_OBJECT

	public:
		StationViewTKY(QWidget* parent = nullptr);
		~StationViewTKY();
	public:
		//创建卡斯柯CTC
		static StationViewTKY* CreatStationView(QWidget* parent = nullptr);

		//创建单站界面
		StationCtrlDispTKY* CreateStationCtrlDisp();
		//创建站间透明界面
		StationMultiDispTKY* CreateMultiStationDisp();
		//创建行车日志界面
		StationLogDispTKY* CreateTrafficLogManage();

		//初始化主菜单
		void InitStationViewMenuBar();
		//初始化工具栏-主工具栏
		void InitStationViewToolBar();
		//初始化工具栏-行车日志工具栏
		void InitTrafficLogToolBar();
		//初始化工具栏-签收工具栏
		void InitSignForToolBar();
		//初始化工具栏-状态工具栏
		void InitStateToolBar();

	private:
		Ui::StationViewTKYClass ui;
		StationCtrlDispTKY* m_pStationCtrl = nullptr;
		StationMultiDispTKY* m_pMultiStation = nullptr;
		StationLogDispTKY* m_pTrafficLogManage = nullptr;

	};
}
