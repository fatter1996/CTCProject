#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QDockWidget>
#include <QMenu>

#include "BaseWndClass/StationCtrlDisp.h"
#include "BaseWndClass/StationMultiDisp.h"
#include "BaseWndClass/StationLogDisp.h"
#include "BaseWndClass/RoutePlanWidget/StaRoutePlan.h"
#include "BaseWndClass/DispatchOrderWidget/StaDispatchOrder.h"
#include "BaseWndClass/VisibleSetWidget/StaVisibleSet.h"

#define STAVIEW_TOOL  1
#define LOGVIEW_TOOL  2

namespace CTCWindows {
	class CTCMainWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		CTCMainWindow(QWidget* parent = nullptr);
		~CTCMainWindow();
		//初始化主界面
		void InitStattionView();
		
	private:
		virtual BaseWnd::StationCtrlDisp* CreateStationCtrlDisp() = 0;
		virtual BaseWnd::StationMultiDisp* CreateMultiStationDisp() = 0;
		virtual BaseWnd::StationLogDisp* CreateTrafficLogManage() = 0;
		virtual BaseWnd::StaRoutePlan* CreateStaRoutePlanWnd() = 0;
		virtual BaseWnd::StaDispatchOrder* CreateStaDispatchOrder() = 0;
		virtual BaseWnd::StaVisibleSet* CreateStaVisibleSet() = 0;

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
		//初始化界面布局
		virtual void InitViewLayout() = 0;

	public slots:
		void ShowDispatchOrderWnd();
		void ShowVisibleSetWnd();

	protected:
		QPushButton* AddToolBarBtn(QString iconFile, QString toolTip, int nType, bool checkable = false, bool checked = false);

	public:
		BaseWnd::StationCtrlDisp* StaCtrlDisp() { return m_pStationCtrl; }
		QWidget* StaPaintView();
		QWidget* StaFunBtnToolBar();
		BaseWnd::StaRoutePlan* RoutePlanWnd() { return m_pRoutePlanWnd; }
		void setFixedSize(const QSize& size);

	protected:
		struct MenuInfo {
			int nLevel = 0;
			int nIndex = 0;

			QAction* pAction = nullptr;
			QVector<MenuInfo*> vecSubMenuInfo;
		public:
			QAction* addNewAction(QWidget* parent, const int& level, const int& index, const QString& text, const bool& Enabled = true, const bool& isCheckable = false, const bool& isChecked = false);
			QAction* addNewSubAction(QWidget* parent, const int& level, const int& index, const QString& text, const bool& Enabled = true, const bool& isCheckable = false, const bool& isChecked = false);
			void addSeparator();
		};

		struct MenuBarInfo {
			QMenu* pMenu = nullptr;
			QVector<MenuInfo*> vecSubMenuInfo;

		public:
			QAction* addNewAction(QWidget* parent, const int& level, const int& index, const QString& text, const bool& Enabled = true, const bool& isCheckable = false, const bool& isChecked = false);
			void addSeparator();
			MenuInfo* getSubActionByIndex(int level, ...);
		};

	protected:
		//单站界面
		BaseWnd::StationCtrlDisp* m_pStationCtrl = nullptr;
		//站间透明
		BaseWnd::StationMultiDisp* m_pStationMulti = nullptr;
		//行车日志
		BaseWnd::StationLogDisp* m_pStationLog = nullptr;
		//主菜单
		QMenuBar* m_pMenuBar = nullptr;
		QVector<MenuBarInfo*> m_vecMenuBarInfo;
		//主工具栏
		QToolBar* m_pStationViewToolBar = nullptr;
		//行车日志界面工具栏
		QToolBar* m_pTrafficLogToolBar = nullptr;
		//签收工具栏
		QToolBar* m_pSignForToolBar = nullptr;
		//状态工具栏
		QToolBar* m_pStateToolBar = nullptr;
		//进路序列停靠窗口
		QDockWidget* m_pPlanDock = nullptr;
		//进路序列窗口
		BaseWnd::StaRoutePlan* m_pRoutePlanWnd = nullptr;
	};
}
