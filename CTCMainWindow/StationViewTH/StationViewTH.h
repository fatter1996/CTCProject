#pragma once

#include "BaseWndClass/CTCMainWindow.h"
#include "ui_StationViewTH.h"

namespace CTCWindows {
	namespace TH {
		class StationViewTH : public CTCMainWindow
		{
			Q_OBJECT

		public:
			StationViewTH(QWidget* parent = nullptr);
			~StationViewTH();

		public:
			//创建卡斯柯CTC
			static StationViewTH* CreatStationView(QWidget* parent = nullptr);
			// 初始化工具栏-底部行车日志工具栏
			void InitbottomTrafficLogToolBar() override;

			BaseWnd::StaTraindiagramwidget* CreateStaTraindiagramwidget() override;
			
			//创建单站界面
			BaseWnd::StationCtrlDisp* CreateStationCtrlDisp() override;
			//创建站间透明界面
			BaseWnd::StationMultiDisp* CreateMultiStationDisp() override;
			//创建行车日志界面
			BaseWnd::StationLogDisp* CreateTrafficLogDisp() override;
			//创建进路序列界面
			BaseWnd::StaRoutePlan* CreateStaRoutePlanWnd() override;
			//创建调度命令界面
			BaseWnd::StaDispatchOrder* CreateStaDispatchOrder() override;
			//创建显示设置界面
			BaseWnd::StaVisibleSet* CreateStaVisibleSet() override;

			void InitStaTraindiagramwidget() override;
			//初始化主菜单
			void InitStationViewMenuBar() override;
			//初始化工具栏-主工具栏
			void InitStationViewToolBar() override;
			//初始化工具栏-行车日志工具栏
			void InitTrafficLogToolBar() override;
			//初始化工具栏-签收工具栏
			void InitSignForToolBar() override;
			//初始化工具栏-状态工具栏
			void InitStateToolBar() override;
			QLayout* WidgetLayout() override { return ui.centralWidget->layout(); }
			void InitStatusBar() override {};
		private:
			Ui::StationViewTH ui;
		};
	}
}
