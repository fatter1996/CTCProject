#include "TableView.h"
#include <QScrollBar>
#include <QAbstractButton>
#include <QEvent>
#include <QStylePainter>

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
            setMinimumHeight(64);
        }

        TableView::~TableView()
        {

        }

        void TableView::SetSectionText(QString text, int nWidth)
        {
            QAbstractButton* bSectionBtn = findChild<QAbstractButton*>();
            if (bSectionBtn) {
                bSectionBtn->setText(text);
                bSectionBtn->installEventFilter(this);
                QStyleOptionHeader opt;
                opt.text = bSectionBtn->text();
                opt.textAlignment = Qt::AlignCenter;
            }
            verticalHeader()->setMinimumWidth(nWidth);
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
            SetTableItem(vecTableData, alignment, rowCount());
        }

        void TableView::ResetTableRows(QVector<QStringList> vecTableData, Qt::Alignment alignment)
        {
            RemoveAllRows();
            SetTableItem(vecTableData, alignment);
        }

        void TableView::SetTableItem(QVector<QStringList> vecTableData, Qt::Alignment alignment, int nStartRow)
        {
            setRowCount(vecTableData.size() + nStartRow);
            QTableWidgetItem* item = nullptr;
            QString strData;
            for (int i = nStartRow; i < vecTableData.size() + nStartRow; i++) {
                for (int j = 0; j < vecTableData[i].size(); j++) {
                    if (j < columnCount()) {
                        strData = vecTableData[i][j];
                        item = new QTableWidgetItem(vecTableData[i][j]);
                        item->setTextAlignment(alignment);
                        if (m_infoTableView.m_vecHHeadInfo.at(j).bCheckable && vecTableData[i][j] != "") {
                            item->setText("");
                            item->setFlags(item->flags() ^ Qt::ItemIsUserCheckable);
                            item->setCheckState(vecTableData[i][j] == "true" ? Qt::Checked : Qt::Unchecked);
                        }
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

        void TableView::SetRowItemsColor(int nRow, QColor bkcolor, QColor txtcolor, int nStartCol, int nEndCol)
        {
            SetRowItemsBKColor(nRow, bkcolor, nStartCol, (nEndCol == -1) ? columnCount() : nEndCol);
            SetRowItemsTextColor(nRow, txtcolor, nStartCol, (nEndCol == -1) ? columnCount() : nEndCol);
        }

        void TableView::SetRowItemsBKColor(int nRow, QColor bkcolor, int nStartCol, int nEndCol)
        {
            for (int i = nStartCol; i < nEndCol; i++) {
                QTableWidgetItem* pItem = item(nRow, i);
                if (pItem) {
                    pItem->setBackground(bkcolor);
                }
            }
        }

        void TableView::SetRowItemsTextColor(int nRow, QColor txtcolor, int nStartCol, int nEndCol)
        {
            for (int i = nStartCol; i < nEndCol; i++) {
                QTableWidgetItem* pItem = item(nRow, i);
                if (pItem) {
                    pItem->setData(Qt::TextColorRole, txtcolor);
                }
            }
        }

        void TableView::SetRowItemsTextFont(int nRow, QFont font, int nStartCol, int nEndCol)
        {
            for (int i = nStartCol; i < ((nEndCol == -1) ? columnCount() : nEndCol); i++) {
                QTableWidgetItem* pItem = item(nRow, i);
                if (pItem) {
                    if (pItem) pItem->setFont(font);
                }
            }
        }

        bool TableView::eventFilter(QObject* obj, QEvent* event)
        {
            if (event->type() == QEvent::Paint) {
                QAbstractButton* bSectionBtn = qobject_cast<QAbstractButton*>(obj);
                if (bSectionBtn) {
                    QStyleOptionHeader opt;
                    opt.init(bSectionBtn);
                    QStyle::State state = QStyle::State_None;
                    if (bSectionBtn->isEnabled()) {
                        state |= QStyle::State_Enabled;
                    }
                    if (bSectionBtn->isActiveWindow()) {
                        state |= QStyle::State_Active;
                    }
                    if (bSectionBtn->isDown()) {
                        state |= QStyle::State_Sunken;
                    }
                    opt.state = state;
                    opt.rect = bSectionBtn->rect();
                    opt.text = bSectionBtn->text();
                    opt.textAlignment = Qt::AlignCenter;
                    opt.position = QStyleOptionHeader::OnlyOneSection;
                    QStylePainter painter(bSectionBtn);
                    painter.drawControl(QStyle::CE_Header, opt);
                    return true;
                }
            }
            return QWidget::eventFilter(obj, event);
        }
    }
}