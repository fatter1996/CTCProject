#pragma once

#include "CTCMainWindow/CTCMainWindow.h"
#include "ui_StationViewTKY.h"

namespace CTCWindows {
	namespace CARS {
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
			BaseWnd::StationCtrlDisp* CreateStationCtrlDisp();
			//创建站间透明界面
			BaseWnd::StationMultiDisp* CreateMultiStationDisp();
			//创建行车日志界面
			BaseWnd::StationLogDisp* CreateTrafficLogManage();
			//创建进路序列界面
			BaseWnd::StaRoutePlan* CreateStaRoutePlanWnd();
			//创建调度命令界面
			BaseWnd::StaDispatchOrder* CreateStaDispatchOrder();
			//创建显示设置界面
			BaseWnd::StaVisibleSet* CreateStaVisibleSet();

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
			//初始化界面布局
			void InitViewLayout();

		private:
			Ui::StationViewTKY ui;
		};
	}
}
