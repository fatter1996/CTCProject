#pragma once

#include "CTCMainWindow/CTCMainWindow.h"
#include "ui_StationViewKSK.h"

namespace CTCWindows {
	namespace CASCO {
		class StationViewKSK : public CTCMainWindow
		{
			Q_OBJECT

		public:
			StationViewKSK(QWidget* parent = nullptr);
			~StationViewKSK();
		public:
			//创建卡斯柯CTC
			static StationViewKSK* CreatStationView(QWidget* parent = nullptr);

			//创建单站界面
			BaseWnd::StationCtrlDisp* CreateStationCtrlDisp();
			//创建单站间透明界面
			BaseWnd::StationMultiDisp* CreateMultiStationDisp();
			//创建行车日志界面
			BaseWnd::StationLogDisp* CreateTrafficLogManage();
			//创建进路序列窗口
			BaseWnd::StaRoutePlan* CreateStaRoutePlanWnd();

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
			Ui::StationViewKSK ui;
		};
	}
}
