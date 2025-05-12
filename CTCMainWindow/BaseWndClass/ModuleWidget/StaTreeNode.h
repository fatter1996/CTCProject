#pragma once
#include <QWidget>
#include <QList>
#include <QTreeWidgetItem>
#include <QTableWidget>
class StaTreeNode :
    public QWidget
{
    Q_OBJECT
public:
    StaTreeNode(QTreeWidgetItem* item, int col = -1,QWidget* parent = nullptr);
    ~StaTreeNode();
    void Add(StaTreeNode* pItem);
    void SetShowState(QTableWidget* pTable);
    void SetState();
    void UpDataState();
    void SetDisabled(bool b);
    QTreeWidgetItem* getTreeItem();
protected:
    QList<StaTreeNode*> m_ListSubTreeList;
    QTreeWidgetItem* m_pTreeItem = nullptr;
    bool bShow = true;
    int m_nCol = -1;

};

