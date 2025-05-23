﻿#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QDockWidget>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

#include "StationView/StationCtrlDisp.h"
#include "StationView/StationMultiDisp.h"
#include "StationView/StationLogDisp.h"
#include "StationView/StaTrainDiagramWidget.h"
#include "ModuleWidget/StaRoutePlan.h"
#include "ModuleWidget/StaDispatchOrder.h"
#include "ModuleWidget/StaVisibleSet.h"

#define STAVIEW_TOOL  1
#define LOGVIEW_TOOL  2

namespace CTCWindows {
	enum class ViewFlag : int {
		StationView,
		MultiStaView,
		TifficLogView,
		DiagramView
	};
	enum class MouseState : int {
		Default,
		Amplify,
		Reduce,
		AddTrain
	};


	struct MenuInfo {
		int m_nLevel = 0;
		int m_nIndex = 0;

		QAction* m_pAction = nullptr;
		QVector<MenuInfo*> m_vecSubMenuInfo;
	public:
		QAction* addNewAction(const QString& text, int level, int index, bool Enabled = true, bool isCheckable = false, bool isChecked = false);
		QAction* addNewSubAction(const QString& text, int level, int index, bool Enabled = true, bool isCheckable = false, bool isChecked = false);
		void addSeparator();
	};

	struct MenuBarInfo {
		QMenu* m_pMenu = nullptr;
		QVector<MenuInfo*> m_vecSubMenuInfo;

	public:
		QAction* addNewAction(const QString& text, int level, int index, bool Enabled = true, bool isCheckable = false, bool isChecked = false);
		void addSeparator();
		MenuInfo* getSubActionByIndex(int level, ...);
	};

	struct MenuSyncAction {
		QAction* m_pStaRevertToolAction = nullptr;
		QAction* m_pMultiRevertToolAction = nullptr;
		QPushButton* m_pStaRevertBtn = nullptr;
		QPushButton* m_pMultiRevertBtn = nullptr;
		QAction* m_pStaRevertMenuAction = nullptr;
		QAction* m_pMultiRevertMenuAction = nullptr;

	public:
		void setDiploidActionChecked(int nType);
	};

	class CTCMainWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		CTCMainWindow(QWidget* parent = nullptr);
		~CTCMainWindow();

	public:
		//初始化主界面
		void InitStattionView();
		//菜单状态同步
		void MenuStateSync(int type, int nType = 0);
		//获取实际大小
		void setFixedSize(const QSize& size);
		void SetShowToolbar(bool bShowBtn, bool bShowLabel);

	public:
		virtual void ShowEditingInterface(void* pTextSign) {};
		void timerEvent(QTimerEvent* event);
	private:
		virtual BaseWnd::StationCtrlDisp* CreateStationCtrlDisp() = 0;
		virtual BaseWnd::StationMultiDisp* CreateMultiStationDisp() = 0;
		virtual BaseWnd::StationLogDisp* CreateTrafficLogDisp() = 0;
		virtual BaseWnd::StaRoutePlan* CreateStaRoutePlanWnd() = 0;
		virtual BaseWnd::StaDispatchOrder* CreateStaDispatchOrder() = 0;
		virtual BaseWnd::StaVisibleSet* CreateStaVisibleSet() = 0;
		virtual BaseWnd::StaTrainDiagramWidget* CreateStaTrainDiagramWidget() = 0;

		//初始化主菜单
		virtual void InitStationViewMenuBar() = 0;
		//初始化工具栏-主工具栏
		virtual void InitStationViewToolBar() = 0;
		//初始化工具栏-行车日志工具栏
		virtual void InitTrafficLogToolBar() = 0;
		//初始化工具栏-签收工具栏
		virtual void InitSignForToolBar() = 0;
		//初始化工具栏-状态工具栏
		virtual void InitStateToolBar() = 0;
		// 初始化工具栏-底部行车日志工具栏
		virtual void InitBottomToolBar() {}

		virtual QLayout* WidgetLayout() = 0;
		virtual void InitStatusBar() = 0;

	private:
		//初始化界面布局
		void InitViewLayout();

	signals:
		void ModifyContent(QString Content, int TextColor, int BackColor, Station::Device::StaTextSign* pTextSign);

	public slots:
		void TurnToStationCtrlDisp();
		void TurnToStationMultiDisp();
		void TurnToTraindiagramDisp();
		void TurnToTrafficLogDisp();
		void ShowStagePlanSignWnd();
		void ShowStaRoutePlanWnd(bool bShow = true);
		void ShowDispatchOrderSignWnd();
		void ShowDispatchOrderWnd();
		void ShowVisibleSetWnd();

	protected:
		QPushButton* AddToolBarBtn(const QString& strIconFile, const QString& strToolTip, int nType, 
			bool bCheckable = false, bool bChecked = false, bool bEnabled = true, const QString& strIconFile2 = "");
		void AddToolBarSeparator(int nType);
		void UpdataDateTime();

	public:
		BaseWnd::StationCtrlDisp* StaCtrlDisp() const { return m_pStationCtrl; }
		BaseWnd::StationMultiDisp* MultiDisp() const { return m_pStationMulti; }
		QWidget* StaPaintView() { return m_pStationCtrl->StaPaintView(); }
		BaseWnd::StaFunBtnToolBar* StaFunBtnToolBar() { return m_pStationCtrl->StaFunBtnBar(); }
		BaseWnd::StaRoutePlan* RoutePlanWnd() const { return m_pRoutePlanWnd; }
		BaseWnd::StationLogDisp* StationLogWnd() const { return m_pStationLog; }
		ViewFlag CurrViewFlag() const { return m_eViewFlag; }
		bool IsMultiStaView() const { return m_eViewFlag == ViewFlag::MultiStaView; }
		void setMouseState(MouseState state) { m_eMouseState = state; }
		MouseState getMouseState() const { return m_eMouseState; }
		bool IsShowToolbar() const { return !m_pStationViewToolBar->isHidden(); }

	protected:
		QLabel* m_pBottomTimeLabel = nullptr;
		BaseWnd::StationCtrlDisp* m_pStationCtrl = nullptr;	//单站界面
		BaseWnd::StationMultiDisp* m_pStationMulti = nullptr; //站间透明
		BaseWnd::StationLogDisp* m_pStationLog = nullptr; //行车日志
		BaseWnd::StaTrainDiagramWidget* m_pStaTrainDiagram = nullptr;//图标转换
		QMenuBar* m_pMenuBar = nullptr; //主菜单
		QVector<MenuBarInfo*> m_vecMenuBarInfo;
		QStatusBar* m_pStatusBar = nullptr;//底部状态栏
		QToolBar* m_pStationViewToolBar = nullptr; //主工具栏
		QToolBar* m_pTrafficLogToolBar = nullptr; //行车日志界面工具栏
		QToolBar* m_pSignForToolBar = nullptr; //签收工具栏
		QToolBar* m_pStateToolBar = nullptr; //状态工具栏
		QToolBar* m_pBottomToolBar = nullptr; //底部行车日志工具栏
		QDockWidget* m_pPlanDock = nullptr; //进路序列停靠窗口
		BaseWnd::StaRoutePlan* m_pRoutePlanWnd = nullptr; //进路序列窗口
		QWidget* m_pCurShowView = nullptr;
		QToolBar* m_pCurToolBar = nullptr;
		ViewFlag m_eViewFlag = ViewFlag::StationView;
		MouseState m_eMouseState = MouseState::Default;
		MenuSyncAction m_stuMenuSyncAction;
		QAction* m_pRoutePlanAction = nullptr;

		int m_buttonTimerId = 0;
		bool m_bShowToolbarBtn = true;
		bool m_bShowToolbarLabel = true;
	};
}
