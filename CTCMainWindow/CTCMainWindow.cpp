#include "CTCMainWindow.h"
#include <QPushbutton>
#include <QToolBar>
#include <QDebug>


namespace CTCWindows {
	
	CTCMainWindow::CTCMainWindow(QWidget* parent) : QMainWindow(parent)
	{
        
	}

	CTCMainWindow::~CTCMainWindow()
	{

	}
    //初始化主界面
	void CTCMainWindow::InitStattionView()
	{
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

        //创建单站界面
        m_pStationCtrl = CreateStationCtrlDisp();
        //创建功能按钮栏
        m_pStationCtrl->CreatStaFunBtnToolBar();
        //创建站间透明界面
        m_pStationMulti = CreateMultiStationDisp();
        //创建行车日志界面
        m_pStationLog = CreateTrafficLogManage();
        //创建进路序列窗口
        m_pRoutePlanWnd = CreateStaRoutePlanWnd();
        //初始化界面布局
        InitViewLayout();
	}

    QWidget* CTCMainWindow::StaPaintView() 
    { 
        return m_pStationCtrl->StaPaintView();
    }

    QWidget* CTCMainWindow::StaFunBtnToolBar()
    {
        return m_pStationCtrl->StaFunBtnBar();
    }

    void CTCMainWindow::setFixedSize(const QSize& size)
    {
        QSize tempSize = QSize();
        tempSize.setWidth(size.width() < 1900 ? 1900 : size.width());
        tempSize.setHeight(size.height() < 926 ? 926 : size.height());
        StaPaintView()->setFixedSize(tempSize);
    }

    void CTCMainWindow::ShowDispatchOrderWnd()
    {
        BaseWnd::StaDispatchOrder* pDispatchOrderWnd = CreateStaDispatchOrder();
        pDispatchOrderWnd->setAttribute(Qt::WA_DeleteOnClose);
        pDispatchOrderWnd->exec();
    }

    void CTCMainWindow::ShowVisibleSetWnd()
    {
        BaseWnd::StaVisibleSet* pVisibleSetWnd = CreateStaVisibleSet();
        pVisibleSetWnd->setAttribute(Qt::WA_DeleteOnClose);
        pVisibleSetWnd->exec();
    }

    QPushButton* CTCMainWindow::AddToolBarBtn(QString iconFile, QString toolTip, int nType, bool checkable, bool checked)
    {
        QPushButton* pButton = new QPushButton;
        pButton->setStyleSheet(QString("QPushButton{"
            "background-image:url(%1);"
            "background-origin: content;"
            "background-position: center;"
            "background-repeat: no-repeat;"
            "}").arg(iconFile));
        pButton->setCheckable(true);
        pButton->setToolTip(toolTip);
        pButton->setFixedSize(32, 32);
        pButton->setIconSize(QSize(24, 24));
        pButton->setFlat(true);
        pButton->setCheckable(checkable);
        if (checkable) {
            pButton->setChecked(checked);
        }
        if (nType == STAVIEW_TOOL) {
            m_pStationViewToolBar->addWidget(pButton);
            addToolBar(m_pStationViewToolBar);
        }
        else if (nType == LOGVIEW_TOOL) {
            m_pTrafficLogToolBar->addWidget(pButton);
            addToolBar(m_pTrafficLogToolBar);
        }
        

        return pButton;
    }

    QAction* CTCMainWindow::MenuInfo::addNewAction(QWidget* parent, const int& level, const int& index, const QString& text, const bool& Enabled, const bool& isCheckable, const bool& isChecked)
    {
        nLevel = level;
        nIndex = index;

        pAction = new QAction(parent);
        pAction->setText(text);
        pAction->setEnabled(Enabled);
        pAction->setCheckable(isCheckable);
        
        if (isCheckable) {
            pAction->setChecked(isChecked);
        }
        return pAction;
    }

    QAction* CTCMainWindow::MenuInfo::addNewSubAction(QWidget* parent, const int& level, const int& index, const QString& text, 
        const bool& Enabled, const bool& isCheckable, const bool& isChecked)
    {
        QMenu* menu = pAction->menu();
        if (!menu) {
            menu = new QMenu(parent);
            pAction->setMenu(menu);
        }

        MenuInfo* info = new MenuInfo;
        info->nLevel = level;
        info->nIndex = index;

        info->pAction = new QAction(parent);
        info->pAction->setText(text);
        info->pAction->setEnabled(Enabled);
        info->pAction->setCheckable(isCheckable);
        if (isCheckable) {
            info->pAction->setChecked(isChecked);
        }
            
        vecSubMenuInfo.append(info);
        menu->addAction(info->pAction);
        return info->pAction;
    }

    void CTCMainWindow::MenuInfo::addSeparator()
    {
        QMenu* menu = pAction->menu();
        if (menu) {
            menu->addSeparator();
        }
    }

    QAction* CTCMainWindow::MenuBarInfo::addNewAction(QWidget* parent, const int& level, const int& index, const QString& text, const bool& Enabled, const bool& isCheckable, const bool& isChecked)
    {
        MenuInfo* menuInfo = new MenuInfo;
        QAction* action = menuInfo->addNewAction(parent, level, index, text, Enabled, isCheckable, isChecked);
        pMenu->addAction(action);
        vecSubMenuInfo.append(menuInfo);
        return action;
    }

    void CTCMainWindow::MenuBarInfo::addSeparator()
    {
        if (pMenu) {
            pMenu->addSeparator();
        }
    }

    CTCMainWindow::MenuInfo* CTCMainWindow::MenuBarInfo::getSubActionByIndex(int level, ...)
    {
        va_list ptr; // 定义一个指向可变参数的指针
        va_start(ptr, level); // 初始化指针，使得指针指向固定参数lever后面的可变参数列表
        int index = va_arg(ptr, int);
        int i = 0;
        QVector<MenuInfo*>* tempMenuInfoList = &vecSubMenuInfo;
        while (i < level) {
            if (index < tempMenuInfoList->size()) {
                if (tempMenuInfoList->at(index)->nLevel == level) {
                    va_end(ptr);
                    return tempMenuInfoList->at(index);
                }
                else {
                    tempMenuInfoList = &tempMenuInfoList->at(index)->vecSubMenuInfo;
                    index = va_arg(ptr, int);
                    i++;
                }
            }
            else return nullptr;
        }
        return nullptr;
    }
}
