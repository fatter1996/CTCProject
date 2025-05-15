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
            m_pItem->setText(0, "01 �г���־");
            m_pItem->setCheckState(0, Qt::Checked);
            m_pItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
            ui.treeWidget->insertTopLevelItem(0, m_pItem);
            m_pStaTreeNode = new CTCWindows::BaseWnd::StaTreeNode(m_pItem);
            //�ӽڵ�
            {
                CTCWindows::BaseWnd::StaTreeNode* node11 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pItem, "01 ����"));
                {
                    CTCWindows::BaseWnd::StaTreeNode* node21 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "01 �г�����"));
                    {
                        node21->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node21->getTreeItem(), "01 ����"), 0));
                        node21->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node21->getTreeItem(), "02 ����"), 1));
                    }
                    node11->Add(node21);
                    CTCWindows::BaseWnd::StaTreeNode* nodeD1 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "02 �ӳ���"), 2);
                    CTCWindows::BaseWnd::StaTreeNode* nodeD2 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "03 �ӳ��ɵ�"), 3);
                    nodeD1->getTreeItem()->setDisabled(true);
                    nodeD2->getTreeItem()->setDisabled(true);
                    node11->Add(nodeD1);
                    node11->Add(nodeD2);
                    CTCWindows::BaseWnd::StaTreeNode* node22 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "04 ʱ��"));
                    node22->getTreeItem()->setDisabled(true);
                    {
                        node22->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "01 ͬ����վ����"), 4));
                        node22->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "02 ��վ����"), 5));
                        CTCWindows::BaseWnd::StaTreeNode* node31 = new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "03 ��վ����"));
                        {
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "01 �涨"), 6));
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "02 ʵ��"), 7));
                        }
                        node22->Add(node31);
                    }
                    node11->Add(node22);
                    CTCWindows::BaseWnd::StaTreeNode* node23 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "05 ժ������"));
                    {
                        CTCWindows::BaseWnd::StaTreeNode* node31 = new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "01 ���ó�"));
                        {
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "01 �س�"), 8));
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "02 �ճ�"), 9));
                        }
                        node23->Add(node31);
                        node23->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "02 �����ó�"), 10));
                        node23->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "03 �س�"), 11));
                    }
                    node11->Add(node23);
                    node11->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "06 ռ������ƾ֤����"), 12));
                    node11->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "07 �����ͺ�"), 13));
                    CTCWindows::BaseWnd::StaTreeNode* node24 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "08 �г�����"));
                    {
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "01 ����"), 14));
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "02 ����"), 15));
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "03 ����"), 16));
                    }
                    node11->Add(node24);
                    CTCWindows::BaseWnd::StaTreeNode* node25 = new CTCWindows::BaseWnd::StaTreeNode(Item(node11->getTreeItem(), "09 �绰��¼����"));
                    {
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "01 ���ϱ���"), 17));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "02 �г����ﲹ������"), 18));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "03 ȡ������"), 19));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "01 ��վ���ٵ���"), 20));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "02 ��վ���ٵ������"), 21));
                    }
                    node11->Add(node25);
                }
                m_pStaTreeNode->Add(node11);
                CTCWindows::BaseWnd::StaTreeNode* node12 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pItem, "02 ����"));
                {
                    CTCWindows::BaseWnd::StaTreeNode* node21 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "01 �г�����"));
                    {
                        node21->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node21->getTreeItem(), "01 ����"), 22));
                        node21->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node21->getTreeItem(), "02 ����"), 23));
                    }
                    node12->Add(node21);
                    CTCWindows::BaseWnd::StaTreeNode* nodeD3 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "02 �����ɵ�"), 24);
                    CTCWindows::BaseWnd::StaTreeNode* nodeD4 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "03 ������"), 25);
                    nodeD3->getTreeItem()->setDisabled(true);
                    nodeD4->getTreeItem()->setDisabled(true);
                    node12->Add(nodeD3);
                    node12->Add(nodeD4);
                    CTCWindows::BaseWnd::StaTreeNode* node22 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "04 ʱ��"));
                    node22->getTreeItem()->setDisabled(true);
                    {
                        node22->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "01 ��վͬ�ⷢ��"), 26));
                        CTCWindows::BaseWnd::StaTreeNode* node31 = new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "02 ��վ����"));
                        {
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "01 �涨"), 27));
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "02 ʵ��"), 28));
                        }
                        node22->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node22->getTreeItem(), "03 ��վ����"), 29));
                        node22->Add(node31);
                    }
                    node12->Add(node22);
                    CTCWindows::BaseWnd::StaTreeNode* node23 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "05 ժ������"));
                    {
                        CTCWindows::BaseWnd::StaTreeNode* node31 = new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "01 ���ó�"));
                        {
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "01 �س�"), 30));
                            node31->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node31->getTreeItem(), "02 �ճ�"), 31));
                        }
                        node23->Add(node31);
                        node23->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "02 �����ó�"), 32));
                        node23->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node23->getTreeItem(), "03 �س�"), 33));
                    }
                    node12->Add(node23);
                    node12->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "06 ռ������ƾ֤����"), 34));
                    node12->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "07 �����ͺ�"), 35));
                    CTCWindows::BaseWnd::StaTreeNode* node24 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "08 �г�����"));
                    {
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "01 ����"), 36));
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "02 ����"), 37));
                        node24->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node24->getTreeItem(), "03 ����"), 38));
                    }
                    node12->Add(node24);
                    CTCWindows::BaseWnd::StaTreeNode* node25 = new CTCWindows::BaseWnd::StaTreeNode(Item(node12->getTreeItem(), "09 �绰��¼����"));
                    {
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "01 ���ϱ���"), 39));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "02 �г����ﲹ������"), 40));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "03 ȡ������"), 41));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "01 ��վ���ٵ���"), 42));
                        node25->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node25->getTreeItem(), "02 ��վ���ٵ������"), 43));
                    }
                    node12->Add(node25);
                }
                m_pStaTreeNode->Add(node12);
                m_pStaTreeNode->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "03 �����������ʱ��"), 44));
                CTCWindows::BaseWnd::StaTreeNode* nodeD5 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "04 �г�ͣս�����涨ʱ��ԭ��"), 45);
                CTCWindows::BaseWnd::StaTreeNode* nodeD6 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "05 �ƻ�����"), 46);
                CTCWindows::BaseWnd::StaTreeNode* nodeD7 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "06 ����"), 47);
                nodeD5->getTreeItem()->setDisabled(true);
                nodeD6->getTreeItem()->setDisabled(true);
                nodeD7->getTreeItem()->setDisabled(true);
                m_pStaTreeNode->Add(nodeD5);
                m_pStaTreeNode->Add(nodeD6);
                m_pStaTreeNode->Add(nodeD7);
                CTCWindows::BaseWnd::StaTreeNode* node13 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "07 ����"));
                {
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "01 �м�"), 48));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "02 ����"), 49));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "03 ��Ʊ"), 50));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "04 ��β"), 51));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "05 ����"), 52));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "06 ����"), 53));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "07 �˽�"), 54));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "08 ��ˮ"), 55));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "09 ժ��"), 56));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "10 ����"), 57));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "11 װж"), 58));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "12 ����"), 59));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "13 ����"), 60));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "14 ����"), 61));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "15 վ��"), 62));
                    node13->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(node13->getTreeItem(), "16 �ۿ�"), 63));
                }
                m_pStaTreeNode->Add(node13);
                CTCWindows::BaseWnd::StaTreeNode* nodeD8 = new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "08 ��������"), 64);
                nodeD8->getTreeItem()->setDisabled(true);
                m_pStaTreeNode->Add(nodeD8);
                m_pStaTreeNode->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "09 ����"), 65));
                m_pStaTreeNode->Add(new CTCWindows::BaseWnd::StaTreeNode(Item(m_pStaTreeNode->getTreeItem(), "10 �ص�"), 66));
            }
            m_pItem->setExpanded(true);//Ĭ��չ��ȫ��
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
            item->setExpanded(true);//Ĭ��չ��ȫ��
            parent->addChild(item);
            return item;
        }
    }
}