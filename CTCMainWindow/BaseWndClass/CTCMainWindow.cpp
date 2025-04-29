#include "CTCMainWindow.h"
#include "CommonWidget/DispatchOrderSign.h"
#include "CommonWidget/StagePlanSign.h"
#include <QPushbutton>
#include <QToolBar>
#include <QMessageBox>
#include <QDebug>
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {

    CTCMainWindow::CTCMainWindow(QWidget* parent) : QMainWindow(parent)
    {
        this->setContextMenuPolicy(Qt::NoContextMenu);
    }

    CTCMainWindow::~CTCMainWindow()
    {

    }
    //初始化主界面
    void CTCMainWindow::InitStattionView()
    {

        //创建单站界面
        m_pStationCtrl = CreateStationCtrlDisp();
        //创建功能按钮栏
        m_pStationCtrl->CreatStaFunBtnToolBar();
        m_pStationCtrl->StaFunBtnBar()->InitConnect();
        //创建站间透明界面
        m_pStationMulti = CreateMultiStationDisp();
        //创建行车日志界面
        m_pStationLog = CreateTrafficLogDisp();
        //创建进路序列窗口
        m_pRoutePlanWnd = CreateStaRoutePlanWnd();
        //初始化主菜单
        InitStationViewMenuBar();
        //初始化工具栏-主工具栏
        InitStationViewToolBar();
        //初始化工具栏-行车日志工具栏
        InitTrafficLogToolBar();
        //初始化工具栏-签收工具栏
        InitSignForToolBar();
        //初始化工具栏-签收工具栏
        InitStateToolBar();
        InitBottomToolBar();
        InitStatusBar();
        //初始化界面布局
        InitViewLayout();
	}

    void CTCMainWindow::InitViewLayout()
    {
        if (m_pStationCtrl) {
            WidgetLayout()->addWidget(m_pStationCtrl);
            m_pCurShowView = m_pStationCtrl;
            m_pCurToolBar = m_pStationViewToolBar;
            m_pTrafficLogToolBar->hide();
            m_pCurToolBar->show();
            QPushButton* pSendPlanBtn = m_pStateToolBar->findChild<QPushButton*>("sendPlanBtn");
            if (pSendPlanBtn) {
                pSendPlanBtn->hide();
            }
            QLabel* pServerComLabel = m_pStateToolBar->findChild<QLabel*>("serverComLabel");
            if (pServerComLabel) {
                pServerComLabel->hide();
            }
        }
        if (m_pStationLog) {
            WidgetLayout()->addWidget(m_pStationLog);
            m_pStationLog->hide();
        }

        if (m_pStationMulti) {
            WidgetLayout()->addWidget(m_pStationMulti);
            m_pStationMulti->hide();
        }

        if (m_pRoutePlanWnd) {
            m_pPlanDock = new QDockWidget(this);
            QList<QAbstractButton*> buttons2 = m_pPlanDock->titleBarWidget()->findChildren<QAbstractButton*>();
            QAbstractButton* pCloseBtn = m_pPlanDock->findChild<QAbstractButton*>("qt_dockwidget_closebutton");
            if (pCloseBtn) {
                connect(pCloseBtn, &QAbstractButton::clicked, [&]() {
                    if (m_pRoutePlanAction) {
                        m_pRoutePlanAction->setChecked(false);
                    }
                    m_vecMenuBarInfo[1]->getSubActionByIndex(2, 0, 3)->m_pAction->setChecked(false);
                });
            }
            m_pPlanDock->setWidget(m_pRoutePlanWnd);
            m_pPlanDock->setFloating(false);
            m_pPlanDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetVerticalTitleBar);
            m_pPlanDock->setAllowedAreas(Qt::BottomDockWidgetArea);
            addDockWidget(Qt::BottomDockWidgetArea, m_pPlanDock);
        }

        if (!m_bShowToolbarBtn) {
            m_pStationViewToolBar->hide();
        }
      
        if (!m_bShowToolbarLabel) {
            m_pStateToolBar->hide();
        }
    }

    void CTCMainWindow::SetShowToolbar(bool bShowBtn, bool bShowLabel)
    {
        m_pStationViewToolBar->setHidden(!bShowBtn);
        m_pStateToolBar->setHidden(!bShowLabel);
    }

    void CTCMainWindow::MenuStateSync(int type, int nType)
    {
        if (type == 1) {    //缩放
            m_stuMenuSyncAction.setDiploidActionChecked(nType);
        }
        if (type == 2) {    //绝缘节
            QPushButton* pNodesBtn = m_pStationViewToolBar->findChild<QPushButton*>("nodes");
            if (pNodesBtn) {
                pNodesBtn->setChecked(!pNodesBtn->isChecked());
            }
        }
    }

    void CTCMainWindow::setFixedSize(const QSize& size)
    {
        QSize tempSize = QSize();
        tempSize.setWidth(size.width() < 1900 ? 1900 : size.width());
        tempSize.setHeight(size.height() < 926 ? 926 : size.height());
        StaPaintView()->setFixedSize(tempSize);
    }
    
    void CTCMainWindow::UpdataDateTime()
    {
        if (!m_pBottomTimeLabel) {
            return;
        }
        QDateTime date = QDateTime::currentDateTime();
        QString strWeek;
        switch (date.date().dayOfWeek()) {
        case 1: strWeek = "星期一"; break;
        case 2: strWeek = "星期二"; break;
        case 3: strWeek = "星期三"; break;
        case 4: strWeek = "星期四"; break;
        case 5: strWeek = "星期五"; break;
        case 6: strWeek = "星期六"; break;
        case 7: strWeek = "星期日"; break;
        }
        m_pBottomTimeLabel->setText(QString("%1  %2").arg(date.toString("yyyy-MM-dd hh:mm:ss")).arg(strWeek));
    }

    void CTCMainWindow::TurnToStationCtrlDisp()
    {
        if (m_pCurShowView != m_pStationCtrl) {
            m_pCurShowView->hide();
            m_pCurShowView = m_pStationCtrl;
            m_pCurShowView->show();
            ShowStaRoutePlanWnd();
        }

        if (m_pCurToolBar != m_pStationViewToolBar) {
            m_pCurToolBar->hide();
            m_pCurToolBar = m_pStationViewToolBar;
            m_pCurToolBar->show();
        }

        QPushButton* pSendPlanBtn = m_pStateToolBar->findChild<QPushButton*>("sendPlanBtn");
        if (pSendPlanBtn) {
            pSendPlanBtn->hide();
        }
        QLabel* pSyncComLabel = m_pStateToolBar->findChild<QLabel*>("centerComLabel");
        if (pSyncComLabel) {
            pSyncComLabel->show();
        }
        QLabel* pServerComLabel = m_pStateToolBar->findChild<QLabel*>("serverComLabel");
        if (pServerComLabel) {
            pServerComLabel->hide();
        }

        m_eViewFlag = ViewFlag::StationView;
        setMouseState(MouseState::Default);
    }

    void CTCMainWindow::TurnToStationMultiDisp()
    {
        if (m_pCurShowView != m_pStationMulti) {
            ShowStaRoutePlanWnd(false);
            m_pCurShowView->hide();
            m_pCurShowView = m_pStationMulti;
            m_pCurShowView->show();
        }

        if (m_pCurToolBar != m_pStationViewToolBar) {
            m_pCurToolBar->hide();
            m_pCurToolBar = m_pStationViewToolBar;
            m_pCurToolBar->show();
        }

        QPushButton* pSendPlanBtn = m_pStateToolBar->findChild<QPushButton*>("sendPlanBtn");
        if (pSendPlanBtn) {
            pSendPlanBtn->hide();
        }
        QLabel* pSyncComLabel = m_pStateToolBar->findChild<QLabel*>("centerComLabel");
        if (pSyncComLabel) {
            pSyncComLabel->show();
        }
        QLabel* pServerComLabel = m_pStateToolBar->findChild<QLabel*>("serverComLabel");
        if (pServerComLabel) {
            pServerComLabel->hide();
        }

        m_eViewFlag = ViewFlag::MultiStaView;
        setMouseState(MouseState::Default);
    }

    void CTCMainWindow::TurnToTrafficLogDisp()
    {
        if (m_pCurShowView != m_pStationLog) {
            ShowStaRoutePlanWnd(false);
            m_pCurShowView->hide();
            m_pCurShowView = m_pStationLog;
            m_pCurShowView->show();
        }

        if (m_pCurToolBar != m_pTrafficLogToolBar) {
            m_pCurToolBar->hide();
            m_pCurToolBar = m_pTrafficLogToolBar;
            m_pCurToolBar->show();
        }

        QPushButton* pSendPlanBtn = m_pStateToolBar->findChild<QPushButton*>("sendPlanBtn");
        if (pSendPlanBtn) {
            pSendPlanBtn->show();
        }
        QLabel* pSyncComLabel = m_pStateToolBar->findChild<QLabel*>("centerComLabel");
        if (pSyncComLabel) {
            pSyncComLabel->hide();
        }
        QLabel* pServerComLabel = m_pStateToolBar->findChild<QLabel*>("serverComLabel");
        if (pServerComLabel) {
            pServerComLabel->show();
        }
        m_pStationLog->OnTrafficLogTableUpData();
        m_eViewFlag = ViewFlag::TifficLogView;
        setMouseState(MouseState::Default);
    }

    void CTCMainWindow::ShowStagePlanSignWnd()
    {
        StagePlanSign* pStagePlanSign = new StagePlanSign(this);
        pStagePlanSign->setAttribute(Qt::WA_DeleteOnClose);
        pStagePlanSign->InitStagePlan();
        pStagePlanSign->exec();
    }

    void CTCMainWindow::ShowStaRoutePlanWnd(bool bShow)
    {
        if (bShow) {
            if (m_pCurShowView == m_pStationCtrl) {
                m_pPlanDock->show();
                addDockWidget(Qt::BottomDockWidgetArea, m_pPlanDock);
            }
        }
        else {
            removeDockWidget(m_pPlanDock);
            m_pPlanDock->close();
        }
    }

    void CTCMainWindow::ShowDispatchOrderWnd()
    {
        BaseWnd::StaDispatchOrder* pDispatchOrderWnd = CreateStaDispatchOrder();
        pDispatchOrderWnd->setAttribute(Qt::WA_DeleteOnClose);
        pDispatchOrderWnd->ViewPermission(Station::LimitsOfAuthority::employee);
        pDispatchOrderWnd->exec();
    }

    void CTCMainWindow::ShowDispatchOrderSignWnd()
    {
        if (!Station::MainStation()->NewDispatchOrder()) {
            QMessageBox::warning(this, MSGBOX_TITTLE, "没有收到调度命令", "确定");
            return;
        }
        DispatchOrderSign* pDispatchOrderSign = new DispatchOrderSign(this);
        pDispatchOrderSign->setAttribute(Qt::WA_DeleteOnClose);
        pDispatchOrderSign->InitNewDispatchOrder(Station::MainStation()->NewDispatchOrder());
        pDispatchOrderSign->exec();
    }

    void CTCMainWindow::ShowVisibleSetWnd()
    {
        BaseWnd::StaVisibleSet* pVisibleSetWnd = CreateStaVisibleSet();
        pVisibleSetWnd->setAttribute(Qt::WA_DeleteOnClose);
        pVisibleSetWnd->exec();
    }

    QPushButton* CTCMainWindow::AddToolBarBtn(const QString& strIconFile, const QString& strToolTip, int nType,
        bool bCheckable, bool bChecked, bool bEnabled, const QString& strIconFile2)
    {
        QPushButton* pButton = new QPushButton;
        QString strStyleSheet = QString(
            "QPushButton {"
            "background-image:url(%1);"
            "background-origin: content;"
            "background-position: center;"
            "background-repeat: no-repeat;"
            "}").arg(strIconFile);
        if (strIconFile2 != "") {
            strStyleSheet.append(QString(
                "QPushButton:disabled {"
                "background-image:url(%1);"
                "}").arg(strIconFile2));
        }

        pButton->setEnabled(bEnabled);
        pButton->setStyleSheet(strStyleSheet);
        pButton->setObjectName(strIconFile.mid(strIconFile.lastIndexOf('/') + 1, strIconFile.length() - strIconFile.lastIndexOf('/') - 5));
        pButton->setCheckable(true);
        pButton->setToolTip(strToolTip);
        pButton->setFixedSize(32, 32);
        pButton->setFlat(true);
        pButton->setCheckable(bCheckable);
        if (bCheckable) {
            pButton->setChecked(bChecked);
        }
        if (nType == STAVIEW_TOOL) {
            m_pStationViewToolBar->addWidget(pButton);
        }
        else if (nType == LOGVIEW_TOOL) {
            m_pTrafficLogToolBar->addWidget(pButton);
        }
        return pButton;
    }

    void CTCMainWindow::AddToolBarSeparator(int nType)
    {
        if (nType == STAVIEW_TOOL) {
            m_pStationViewToolBar->addSeparator();
        }
        else if (nType == LOGVIEW_TOOL) {
            m_pTrafficLogToolBar->addSeparator();
        }
    }

    QAction* MenuInfo::addNewAction(const QString& text, int level, int index, bool Enabled, bool isCheckable, bool isChecked)
    {
        m_nLevel = level;
        m_nIndex = index;
        m_pAction = new QAction();
        m_pAction->setText(text);
        m_pAction->setEnabled(Enabled);
        m_pAction->setCheckable(isCheckable);

        if (isCheckable) {
            m_pAction->setChecked(isChecked);
        }
        return m_pAction;
    }

    QAction* MenuInfo::addNewSubAction(const QString& text, int level, int index,
        bool Enabled, bool isCheckable, bool isChecked)
    {
        QMenu* menu = m_pAction->menu();
        if (!menu) {
            menu = new QMenu();
            m_pAction->setMenu(menu);
        }

        MenuInfo* info = new MenuInfo;
        info->m_nLevel = level;
        info->m_nIndex = index;
        info->m_pAction = new QAction();
        info->m_pAction->setText(text);
        info->m_pAction->setEnabled(Enabled);
        info->m_pAction->setCheckable(isCheckable);
        if (isCheckable) {
            info->m_pAction->setChecked(isChecked);
        }

        m_vecSubMenuInfo.append(info);
        menu->addAction(info->m_pAction);
        return info->m_pAction;
    }

    void MenuInfo::addSeparator()
    {
        QMenu* menu = m_pAction->menu();
        if (menu) {
            menu->addSeparator();
        }
    }

    QAction* MenuBarInfo::addNewAction(const QString& text, int level, int index, bool Enabled, bool isCheckable, bool isChecked)
    {
        MenuInfo* pMenuInfo = new MenuInfo;
        QAction* pAction = pMenuInfo->addNewAction(text, level, index, Enabled, isCheckable, isChecked);
        m_pMenu->addAction(pAction);
        m_vecSubMenuInfo.append(pMenuInfo);
        return pAction;
    }

    void MenuBarInfo::addSeparator()
    {
        if (m_pMenu) {
            m_pMenu->addSeparator();
        }
    }

    MenuInfo* MenuBarInfo::getSubActionByIndex(int level, ...)
    {
        va_list ptr; // 定义一个指向可变参数的指针
        va_start(ptr, level); // 初始化指针，使得指针指向固定参数lever后面的可变参数列表
        int index = va_arg(ptr, int);
        int i = 0;
        QVector<MenuInfo*>* tempMenuInfoList = &m_vecSubMenuInfo;
        while (i < level) {
            if (index < tempMenuInfoList->size()) {
                if (tempMenuInfoList->at(index)->m_nLevel == level) {
                    va_end(ptr);
                    return tempMenuInfoList->at(index);
                }
                else {
                    tempMenuInfoList = &tempMenuInfoList->at(index)->m_vecSubMenuInfo;
                    index = va_arg(ptr, int);
                    i++;
                }
            }
            else return nullptr;
        }
        return nullptr;
    }

    void MenuSyncAction::setDiploidActionChecked(int nType)
    {
        double nDiploid = 1.0;
        if (nType == STAVIEW) {
            if (MainWindow()->IsMultiStaView()) {
                nDiploid = Station::MainStation()->getDiploid(Station::DiploidRatio::MultiDiploid);
                if (m_pMultiRevertToolAction) {
                    m_pMultiRevertToolAction->setEnabled(nDiploid != 1.0);
                }
                if (m_pMultiRevertBtn) {
                    m_pMultiRevertBtn->setEnabled(nDiploid != 1.0);
                }
                if (m_pMultiRevertMenuAction) {
                    m_pMultiRevertMenuAction->setEnabled(nDiploid != 1.0);
                }
            }
            else {
                nDiploid = Station::MainStation()->getDiploid(Station::DiploidRatio::StaDiploid);
                if (m_pStaRevertToolAction) {
                    m_pStaRevertToolAction->setEnabled(nDiploid != 1.0);
                }
                if (m_pStaRevertBtn) {
                    m_pStaRevertBtn->setEnabled(nDiploid != 1.0);
                }
                if (m_pStaRevertMenuAction) {
                    m_pStaRevertMenuAction->setEnabled(nDiploid != 1.0);
                }
            }
        }
        else if (nType == TRAINNUM) {

        }
    }
}