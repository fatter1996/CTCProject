#include "TrainInformationInterfaceKSK.h"
#include <QTreeWidgetItem>
#include "CTCMainWindow/BaseWndClass/CTCMainWindow.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
    namespace CASCO {
        TrainInformationInterfaceKSK::TrainInformationInterfaceKSK(QWidget* parent)
            : QWidget(parent)
        {
            ui.setupUi(this);


        }

        TrainInformationInterfaceKSK::~TrainInformationInterfaceKSK()
        {}

        void TrainInformationInterfaceKSK::InitMyTree()
        {
            ui.treeWidget->setColumnCount(1);
            QTreeWidgetItem* m_pItem = new QTreeWidgetItem();
            m_pItem->setText(0, "01 行车日志");
            m_pItem->setCheckState(0, Qt::Checked);
            m_pItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
            ui.treeWidget->insertTopLevelItem(0, m_pItem);
            m_pStaTreeNode = new CTCWindows::BaseWnd::StaTreeNode(m_pItem);
            //子节点
            {
                CTCWindows::BaseWnd::StaTreeNode* node11 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pItem, "01 到达"));
                {
                    CTCWindows::BaseWnd::StaTreeNode* node21 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "01 列车车次"));
                    {
                        node21->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node21->getTreeItem(), "01 下行"), 0));
                        node21->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node21->getTreeItem(), "02 上行"), 1));
                    }
                    node11->Add(node21);
                    CTCWindows::BaseWnd::StaTreeNode* nodeD1 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "02 接车口"), 2);
                    CTCWindows::BaseWnd::StaTreeNode* nodeD2 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "03 接车股道"), 3);
                    nodeD1->getTreeItem()->setDisabled(true);
                    nodeD2->getTreeItem()->setDisabled(true);
                    node11->Add(nodeD1);
                    node11->Add(nodeD2);
                    CTCWindows::BaseWnd::StaTreeNode* node22 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "04 时分"));
                    node22->getTreeItem()->setDisabled(true);
                    {
                        node22->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "01 同意邻站发车"), 4));
                        node22->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "02 邻站出发"), 5));
                        CTCWindows::BaseWnd::StaTreeNode* node31 = new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "03 本站到达"));
                        {
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "01 规定"), 6));
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "02 实际"), 7));
                        }
                        node22->Add(node31);
                    }
                    node11->Add(node22);
                    CTCWindows::BaseWnd::StaTreeNode* node23 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "05 摘车辆数"));
                    {
                        CTCWindows::BaseWnd::StaTreeNode* node31 = new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "01 运用车"));
                        {
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "01 重车"), 8));
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "02 空车"), 9));
                        }
                        node23->Add(node31);
                        node23->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "02 非运用车"), 10));
                        node23->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "03 守车"), 11));
                    }
                    node11->Add(node23);
                    node11->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "06 占用区间凭证号码"), 12));
                    node11->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "07 机车型号"), 13));
                    CTCWindows::BaseWnd::StaTreeNode* node24 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "08 列车编组"));
                    {
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "01 车数"), 14));
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "02 换长"), 15));
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "03 总重"), 16));
                    }
                    node11->Add(node24);
                    CTCWindows::BaseWnd::StaTreeNode* node25 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "09 电话记录号码"));
                    {
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "01 承认闭塞"), 17));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "02 列车到达补机返回"), 18));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "03 取消闭塞"), 19));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "01 出站跟踪调车"), 20));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "02 出站跟踪调车完毕"), 21));
                    }
                    node11->Add(node25);
                }
                m_pStaTreeNode->Add(node11);
                CTCWindows::BaseWnd::StaTreeNode* node12 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pItem, "02 出发"));
                {
                    CTCWindows::BaseWnd::StaTreeNode* node21 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "01 列车车次"));
                    {
                        node21->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node21->getTreeItem(), "01 下行"), 22));
                        node21->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node21->getTreeItem(), "02 上行"), 23));
                    }
                    node12->Add(node21);
                    CTCWindows::BaseWnd::StaTreeNode* nodeD3 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "02 发车股道"), 24);
                    CTCWindows::BaseWnd::StaTreeNode* nodeD4 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "03 发车口"), 25);
                    nodeD3->getTreeItem()->setDisabled(true);
                    nodeD4->getTreeItem()->setDisabled(true);
                    node12->Add(nodeD3);
                    node12->Add(nodeD4);
                    CTCWindows::BaseWnd::StaTreeNode* node22 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "04 时分"));
                    node22->getTreeItem()->setDisabled(true);
                    {
                        node22->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "01 邻站同意发车"), 26));
                        CTCWindows::BaseWnd::StaTreeNode* node31 = new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "02 本站出发"));
                        {
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "01 规定"), 27));
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "02 实际"), 28));
                        }
                        node22->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "03 邻站到达"), 29));
                        node22->Add(node31);
                    }
                    node12->Add(node22);
                    CTCWindows::BaseWnd::StaTreeNode* node23 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "05 摘车辆数"));
                    {
                        CTCWindows::BaseWnd::StaTreeNode* node31 = new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "01 运用车"));
                        {
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "01 重车"), 30));
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "02 空车"), 31));
                        }
                        node23->Add(node31);
                        node23->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "02 非运用车"), 32));
                        node23->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "03 守车"), 33));
                    }
                    node12->Add(node23);
                    node12->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "06 占用区间凭证号码"), 34));
                    node12->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "07 机车型号"), 35));
                    CTCWindows::BaseWnd::StaTreeNode* node24 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "08 列车编组"));
                    {
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "01 车数"), 36));
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "02 换长"), 37));
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "03 总重"), 38));
                    }
                    node12->Add(node24);
                    CTCWindows::BaseWnd::StaTreeNode* node25 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "09 电话记录号码"));
                    {
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "01 承认闭塞"), 39));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "02 列车到达补机返回"), 40));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "03 取消闭塞"), 41));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "01 出站跟踪调车"), 42));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "02 出站跟踪调车完毕"), 43));
                    }
                    node12->Add(node25);
                }
                m_pStaTreeNode->Add(node12);
                m_pStaTreeNode->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "03 本务机车调车时分"), 44));
                CTCWindows::BaseWnd::StaTreeNode* nodeD5 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "04 列车停战超过规定时间原因"), 45);
                CTCWindows::BaseWnd::StaTreeNode* nodeD6 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "05 计划属性"), 46);
                CTCWindows::BaseWnd::StaTreeNode* nodeD7 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "06 记事"), 47);
                nodeD5->getTreeItem()->setDisabled(true);
                nodeD6->getTreeItem()->setDisabled(true);
                nodeD7->getTreeItem()->setDisabled(true);
                m_pStaTreeNode->Add(nodeD5);
                m_pStaTreeNode->Add(nodeD6);
                m_pStaTreeNode->Add(nodeD7);
                CTCWindows::BaseWnd::StaTreeNode* node13 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "07 任务"));
                {
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "01 列检"), 48));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "02 交令"), 49));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "03 交票"), 50));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "04 列尾"), 51));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "05 机车"), 52));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "06 货检"), 53));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "07 乘降"), 54));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "08 上水"), 55));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "09 摘挂"), 56));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "10 换乘"), 57));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "11 装卸"), 58));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "12 吸污"), 59));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "13 道口"), 60));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "14 车号"), 61));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "15 站务"), 62));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "16 综控"), 63));
                }
                m_pStaTreeNode->Add(node13);
                CTCWindows::BaseWnd::StaTreeNode* nodeD8 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "08 超限属性"), 64);
                nodeD8->getTreeItem()->setDisabled(true);
                m_pStaTreeNode->Add(nodeD8);
                m_pStaTreeNode->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "09 军运"), 65));
                m_pStaTreeNode->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "10 重点"), 66));
            }
            m_pItem->setExpanded(true);//默认展开全部
            ConnectButton();
        }

        void TrainInformationInterfaceKSK::ConnectButton()
        {
            connect(ui.show, &QPushButton::clicked, [=]() {
                QTreeWidgetItem* item = ui.treeWidget->currentItem();
                if (item)
                {
                    //item->setExpanded(true);
                    item->setCheckState(0, Qt::Checked);
                }
                });
            connect(ui.hide, &QPushButton::clicked, [=]() {
                QTreeWidgetItem* item = ui.treeWidget->currentItem();
                if (item)
                {
                    //item->setExpanded(false);
                    item->setCheckState(0, Qt::Unchecked);
                }
                });
            connect(ui.allshow, &QPushButton::clicked, [=]() {
                QTreeWidgetItemIterator It(ui.treeWidget);
                while (*It)
                {
                    (*It)->setCheckState(0, Qt::Checked);
                    It++;
                }
                });
            connect(ui.allhide, &QPushButton::clicked, [=]() {
                QTreeWidgetItemIterator It(ui.treeWidget);
                while (*It)
                {
                    (*It)->setCheckState(0, Qt::Unchecked);
                    if ((*It)->isDisabled())
                        (*It)->setCheckState(0, Qt::Checked);
                    It++;
                }
                });
            connect(ui.sure, &QPushButton::clicked, [=]() {
                emit CheckComplete(this);
                });
            connect(ui.cancel, &QPushButton::clicked, [=]() {

                });
        }

        QTreeWidgetItem* TrainInformationInterfaceKSK::Item(QTreeWidgetItem* parent, QString text)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem(parent);
            item->setText(0, text);
            item->setCheckState(0, Qt::Checked);
            item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
            item->setExpanded(true);//默认展开全部
            parent->addChild(item);
            return item;
        }
    }
}