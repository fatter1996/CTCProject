#include "TableView.h"
#include <QScrollBar>

#pragma execution_character_set("utf-8")

namespace CTCWindows {
    namespace  Control {

        TableView::TableView(QWidget* parent) : QTableWidget(parent)
        {
            installEventFilter(this);
            verticalScrollBar()->setEnabled(true);
            horizontalScrollBar()->setEnabled(true);
            verticalHeader()->setMinimumWidth(32);
            horizontalHeader()->setMinimumHeight(16);
            setContextMenuPolicy(Qt::CustomContextMenu);
            setMinimumHeight(120);
        }

        TableView::~TableView()
        {

        }

        void TableView::SetHeadData(QVector<TableViewHeadInfo> vecHeadInfo, int nHeadType, Qt::Alignment alignment, bool bStretchable)
        {
            QStringList strHeadList;
            for (const TableViewHeadInfo& infoHead : vecHeadInfo) {
                strHeadList << infoHead.strData;
            }

            if (nHeadType == VHead) {   //纵向表头
                setRowCount(vecHeadInfo.size());
                verticalHeader()->setDefaultAlignment(alignment);
                setVerticalHeaderLabels(strHeadList);

                m_infoTableView.m_vecVHeadInfo.swap(vecHeadInfo);
                for (int i = 0; i < m_infoTableView.m_vecVHeadInfo.size(); i++) {
                    setRowHeight(i, m_infoTableView.m_vecVHeadInfo.at(i).nSize);
                }
            }
            else if (nHeadType == HHead) {  //横向表头
                setColumnCount(vecHeadInfo.size());
                horizontalHeader()->setDefaultAlignment(alignment);
                setHorizontalHeaderLabels(strHeadList);
                horizontalHeader()->setStretchLastSection(bStretchable);

                m_infoTableView.m_vecHHeadInfo.swap(vecHeadInfo);
                for (int i = 0; i < m_infoTableView.m_vecHHeadInfo.size(); i++) {
                    setColumnWidth(i, m_infoTableView.m_vecHHeadInfo.at(i).nSize);
                }
            }
        }

        void TableView::AddTableRows(QVector<QStringList> vecTableData, Qt::Alignment alignment)
        {
            int nRow = rowCount();
            setRowCount(vecTableData.size() + nRow);
            QTableWidgetItem* item = nullptr;
            for (int i = 0; i < vecTableData.size(); i++) {
                for (int j = 0; j < vecTableData[i].size(); j++) {
                    if (j < columnCount()) {
                        item = new QTableWidgetItem(vecTableData[i][j]);
                        item->setTextAlignment(alignment);
                        setItem(i + nRow, j, item);
                    }
                }
            }
        }

        void TableView::ResetTableRows(QVector<QStringList> vecTableData, Qt::Alignment alignment)
        {
            RemoveAllRows();
            setRowCount(vecTableData.size());
            QTableWidgetItem* item = nullptr;
            for (int i = 0; i < vecTableData.size(); i++) {
                for (int j = 0; j < vecTableData[i].size(); j++) {
                    if (j < columnCount()) {
                        item = new QTableWidgetItem(vecTableData[i][j]);
                        item->setTextAlignment(alignment);
                        setItem(i, j, item);
                    }
                }
            }
        }

        void TableView::RemoveAllRows()
        {
            while (rowCount() > 0) {
                removeRow(0); // 从第一行开始删除
            }
            setRowCount(0);
        }
    }
}