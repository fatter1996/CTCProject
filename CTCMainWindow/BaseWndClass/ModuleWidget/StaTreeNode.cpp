#include "StaTreeNode.h"

StaTreeNode::StaTreeNode(QTreeWidgetItem* item, int col, QWidget* parent)
{
	m_pTreeItem = item;
    m_nCol = col;
}

StaTreeNode::~StaTreeNode()
{
}

void StaTreeNode::Add(StaTreeNode* pItem)
{
	m_ListSubTreeList.append(pItem);
}

void StaTreeNode::UpDataState()
{
    if (bShow)
    {
        m_pTreeItem->setCheckState(0, Qt::Checked);
    }
    else
    {
        m_pTreeItem->setCheckState(0, Qt::Unchecked);
    }
    for (int i = 0; i < m_ListSubTreeList.size(); i++)
    {
        m_ListSubTreeList.at(i)->UpDataState();
    }
}
void StaTreeNode::SetState()
{
    if (m_pTreeItem->checkState(0) == Qt::Checked)
    {
        bShow = true;
    }
    else
    {
        bShow = false;
    }
    for (int i = 0; i < m_ListSubTreeList.size(); i++)
    {
        m_ListSubTreeList.at(i)->SetState();
    }
}

void StaTreeNode::SetShowState(QTableWidget* pTable)
{
    if (m_nCol != -1)
    {
        if (bShow)
        {
            pTable->setColumnWidth(m_nCol, 50);
        }
        else
        {
            pTable->setColumnWidth(m_nCol, 0);

        }
        pTable->setColumnHidden(m_nCol, !bShow);
    }

    for (int i = 0; i < m_ListSubTreeList.size(); i++)
    {
        m_ListSubTreeList.at(i)->SetShowState(pTable);
    }
}

void StaTreeNode::SetDisabled(bool b)
{
    m_pTreeItem->setDisabled(b);
}

QTreeWidgetItem* StaTreeNode::getTreeItem()
{
    return m_pTreeItem;
}
