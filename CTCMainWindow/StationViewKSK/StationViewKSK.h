#pragma once

#include "CTCMainWindow/CTCMainWindow.h"
#include "StationCtrlDispKSK.h"
#include "StationMultiDispKSK.h"
#include "StationLogDispKSK.h"
#include "ui_StationViewKSK.h"

namespace CTCWindows {
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
		StationCtrlDispKSK* CreateStationCtrlDisp();
		//创建单站间透明界面
		StationMultiDispKSK* CreateMultiStationDisp();
		//创建行车日志界面
		StationLogDispKSK* CreateTrafficLogManage();

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
		Ui::StationViewKSKClass ui;

		//工具栏-显示
		bool bShow_JYJ = true;//绝缘节
		bool bShow_LCAN = true;//列车按钮
		bool bShow_DCAN = true;//调车按钮
		bool bShow_JLYG = true;//进路预告窗
	};
}
