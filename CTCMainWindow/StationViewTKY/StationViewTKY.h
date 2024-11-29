#pragma once

#include "BaseWndClass/CTCMainWindow.h"
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
			//创建CTC
			static StationViewTKY* CreatStationView(QWidget* parent = nullptr);
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
			//初始化界面布局
			QLayout* WidgetLayout() override { return ui.centralWidget->layout(); }

		signals:
			void clickFunbutton(FunType eSelectType);

		private:
			Ui::StationViewTKY ui;
		};
	}
}
