#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QMenu>

#include "BaseWndClassInterface/StationViewInterface.h"
#include "BaseWndClass/StationCtrlDisp.h"
#include "BaseWndClass/StationMultiDisp.h"
#include "BaseWndClass/StationLogDisp.h"
#include "StationObject/StationObject.h"

namespace CTCWindows {
	class CTCMainWindow : public QMainWindow, public StationViewInterface
	{
		Q_OBJECT
	public:
		CTCMainWindow(QWidget* parent = nullptr);
		~CTCMainWindow();
		//初始化主界面
		void InitStattionView();
		
	private:
		virtual StationCtrlDisp* CreateStationCtrlDisp() = 0;
		virtual StationMultiDisp* CreateMultiStationDisp() = 0;
		virtual StationLogDisp* CreateTrafficLogManage() = 0;

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

	public:
		StationCtrlDisp* GetStationCtrlDisp() { return m_pStationCtrlDisp; }
	
	
		
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

	private:
		StationCtrlDisp* m_pStationCtrlDisp = nullptr;
		StationMultiDisp* m_pStationMultiDisp = nullptr;
		StationLogDisp* m_pStationLogDisp = nullptr;

	protected:
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

		bool bRoutepreWndShow = true; //是否显示进路序列窗
	};
}
