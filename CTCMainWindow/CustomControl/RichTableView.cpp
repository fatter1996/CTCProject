#include "RichTableView.h"
#include <QGridLayout>
#include <QScrollBar>
#include <QEvent>
#include <QDebug>

namespace CTCWindows {
    namespace  Control {

        RichTableView::RichTableView(QWidget* parent) 
        {
            InitTableView();

            m_pHorizontalScrollBar = new QScrollBar(Qt::Horizontal);
            connect(m_pHorizontalHeadTable->horizontalScrollBar(), &QScrollBar::rangeChanged, m_pHorizontalScrollBar, &QScrollBar::setRange);
            connect(m_pHorizontalHeadTable->horizontalScrollBar(), &QScrollBar::valueChanged, m_pHorizontalScrollBar, &QScrollBar::setValue);
            connect(m_pHorizontalScrollBar, &QScrollBar::valueChanged, m_pHorizontalHeadTable->horizontalScrollBar(), &QScrollBar::setValue);
            connect(m_pDataTable->horizontalScrollBar(), &QScrollBar::rangeChanged, m_pHorizontalScrollBar, &QScrollBar::setRange);
            connect(m_pDataTable->horizontalScrollBar(), &QScrollBar::valueChanged, m_pHorizontalScrollBar, &QScrollBar::setValue);
            connect(m_pHorizontalScrollBar, &QScrollBar::valueChanged, m_pDataTable->horizontalScrollBar(), &QScrollBar::setValue);

            m_pVerticalScrollBar = new QScrollBar(Qt::Vertical);
            connect(m_pVerticalHeadTable->verticalScrollBar(), &QScrollBar::rangeChanged, m_pVerticalScrollBar, &QScrollBar::setRange);
            connect(m_pVerticalHeadTable->verticalScrollBar(), &QScrollBar::valueChanged, m_pVerticalScrollBar, &QScrollBar::setValue);
            connect(m_pVerticalScrollBar, &QScrollBar::valueChanged, m_pVerticalHeadTable->verticalScrollBar(), &QScrollBar::setValue);
            connect(m_pDataTable->verticalScrollBar(), &QScrollBar::rangeChanged, m_pVerticalScrollBar, &QScrollBar::setRange);
            connect(m_pDataTable->verticalScrollBar(), &QScrollBar::valueChanged, m_pVerticalScrollBar, &QScrollBar::setValue);
            connect(m_pVerticalScrollBar, &QScrollBar::valueChanged, m_pDataTable->verticalScrollBar(), &QScrollBar::setValue);

            QGridLayout* pLayout = new QGridLayout;
            pLayout->setMargin(0);
            pLayout->setHorizontalSpacing(0);
            pLayout->setVerticalSpacing(0);
            pLayout->addWidget(m_pSectionTable, 0, 0);
            pLayout->addWidget(m_pHorizontalHeadTable, 0, 1);
            pLayout->addWidget(m_pVerticalHeadTable, 1, 0);
            pLayout->addWidget(m_pDataTable, 1, 1);
            pLayout->addWidget(m_pVerticalScrollBar, 0, 2, 0, 1);

            QVBoxLayout* pLayout2 = new QVBoxLayout(this);
            pLayout2->setMargin(0);
            pLayout2->setSpacing(0);
            pLayout2->addLayout(pLayout);
            pLayout2->addWidget(m_pHorizontalScrollBar);
            this->setLayout(pLayout2);

            connect(m_pVerticalHeadTable, &QTableWidget::itemPressed, [=](QTableWidgetItem* item) {
                m_pDataTable->selectRow(item->row());
                m_pDataTable->setFocus();
                m_pVerticalHeadTable->selectRow(item->row());
            });
        }

        RichTableView::~RichTableView()
        {

        }

        void RichTableView::InitTableView()
        {
            m_pSectionTable = new QTableWidget(this);
            m_pSectionTable->verticalHeader()->hide();
            m_pSectionTable->horizontalHeader()->hide();
            m_pSectionTable->setRowCount(1);
            m_pSectionTable->setColumnCount(0);
            m_pSectionTable->setRowCount(1);
            m_pSectionTable->setColumnCount(1);
            m_pSectionTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_pSectionTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_pSectionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
            m_pSectionTable->setFixedSize(QSize(72, 32));
            m_pSectionTable->setRowHeight(0, 32);
            m_pSectionTable->setColumnWidth(0, 72);
            m_pSectionTable->setStyleSheet(
                "QTableWidget { "
                    "border-style:none;"
                    "background-color: rgb(240, 240, 240);"
                "}"
                "QTableWidget:item {"
                    "background-color: rgb(240, 240, 240);"
                    "border-width:1px;"
                    "border-style:solid;"
                    "border-left-color: rgb(250, 250, 250);"
                    "border-top-color: rgb(250, 250, 250);"
                    "border-right-color: rgb(180, 180, 180);"
                    "border-bottom-color: rgb(180, 180, 180);"
                "}");

            m_pHorizontalHeadTable = new QTableWidget(this);
            m_pHorizontalHeadTable->verticalHeader()->hide();
            m_pHorizontalHeadTable->horizontalHeader()->hide();
            m_pHorizontalHeadTable->setRowCount(1);
            m_pHorizontalHeadTable->setColumnCount(0);
            m_pHorizontalHeadTable->horizontalHeader()->setStretchLastSection(true);
            m_pHorizontalHeadTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_pHorizontalHeadTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_pHorizontalHeadTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
            m_pHorizontalHeadTable->setRowHeight(0, m_pSectionTable->size().height());
            m_pHorizontalHeadTable->setFixedHeight(m_pSectionTable->size().height());
            m_pHorizontalHeadTable->setStyleSheet(
                "QTableWidget { "
                    "border-style:none;"
                    "background-color: rgb(240, 240, 240);"
                "}"
                "QTableWidget::item {"
                    "background-color: rgb(240, 240, 240);"
                    "border-width:1px;"
                    "border-style:solid;"
                    "border-left-color: rgb(250, 250, 250);"
                    "border-top-color: rgb(250, 250, 250);"
                    "border-right-color: rgb(180, 180, 180);"
                    "border-bottom-color: rgb(180, 180, 180);"
                "}"
                "QTableWidget::item:selected  {"
                    "color: rgb(0, 0, 0);"
                "}");

            m_pVerticalHeadTable = new QTableWidget(this);
            m_pVerticalHeadTable->verticalHeader()->hide();
            m_pVerticalHeadTable->horizontalHeader()->hide();
            m_pVerticalHeadTable->setColumnCount(0);
            m_pVerticalHeadTable->setRowCount(0);
            m_pVerticalHeadTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_pVerticalHeadTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_pVerticalHeadTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
            m_pVerticalHeadTable->setColumnWidth(0, m_pSectionTable->size().width());
            m_pVerticalHeadTable->setFixedWidth(m_pSectionTable->size().width());
            m_pVerticalHeadTable->setStyleSheet(
                "QTableWidget { "
                    "border-style:none;"
                    "background-color: rgb(240, 240, 240);"
                "}"
                "QTableWidget:item {"
                    "background-color: rgb(240, 240, 240);"
                    "border-width:1px;"
                    "border-style:solid;"
                    "border-left-color: rgb(250, 250, 250);"
                    "border-top-color: rgb(250, 250, 250);"
                    "border-right-color: rgb(180, 180, 180);"
                    "border-bottom-color: rgb(180, 180, 180);"
                "}"
                "QTableWidget::item:selected  {"
                    "color: rgb(0, 0, 0);"
                "}");

            m_pDataTable = new QTableWidget(this);
            m_pDataTable->verticalHeader()->hide();
            m_pDataTable->horizontalHeader()->hide();
            m_pDataTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_pDataTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            m_pDataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
            m_pDataTable->setStyleSheet(
                "QTableWidget {"
                    "border-style:none;"
                    "background-color: rgb(250, 250, 250);"
                "}");
        }

        void RichTableView::TableHeadInit(int nRow, int nCol, QVector<TableHeadInfo> vecTableHeadInfo)
        {
            m_pDataTable->setColumnCount(nCol);
            m_pHorizontalHeadTable->setColumnCount(nCol);
            m_pHorizontalHeadTable->setRowCount(nRow);

            for (TableHeadInfo& headInfo : vecTableHeadInfo) {
                m_pHorizontalHeadTable->setSpan(headInfo.cStart.nRow, headInfo.cStart.nCol, headInfo.nRowCell, headInfo.nColCell);
                m_pHorizontalHeadTable->setItem(headInfo.cStart.nRow, headInfo.cStart.nCol, new QTableWidgetItem(headInfo.strData));
                m_pHorizontalHeadTable->item(headInfo.cStart.nRow, headInfo.cStart.nCol)->setTextAlignment(Qt::AlignCenter);
            }
        }

        void RichTableView::SetHorizontalHeadHeight(int nHeight, int nRow)
        {
            if (nRow == TOTAL) {
                for (int i = 0; i < m_pHorizontalHeadTable->rowCount(); i++) {
                    m_pHorizontalHeadTable->setRowHeight(i, nHeight / m_pHorizontalHeadTable->rowCount());
                }
                m_pHorizontalHeadTable->setFixedHeight(nHeight);
            }
            else if (nRow == AVERAGE) {
                for (int i = 0; i < m_pHorizontalHeadTable->rowCount(); i++) {
                    m_pHorizontalHeadTable->setRowHeight(i, nHeight);
                }
                m_pHorizontalHeadTable->setFixedHeight(nHeight * m_pHorizontalHeadTable->rowCount());
            }
            else {
                int differ = nHeight - m_pHorizontalHeadTable->rowHeight(nRow);
                m_pHorizontalHeadTable->setRowHeight(nRow, nHeight);
                m_pHorizontalHeadTable->setFixedHeight(differ + m_pHorizontalHeadTable->height());
            }
            m_pSectionTable->setFixedHeight(m_pHorizontalHeadTable->height());
            m_pSectionTable->setRowHeight(0, m_pHorizontalHeadTable->height());
        }

        void RichTableView::SetVerticalHeadWidth(int nWidth, int nCol)
        {
            if (nCol == TOTAL) {
                for (int i = 0; i < m_pVerticalHeadTable->rowCount(); i++) {
                    m_pVerticalHeadTable->setColumnWidth(i, nWidth / m_pVerticalHeadTable->rowCount());
                }
                m_pVerticalHeadTable->setFixedWidth(nWidth);
            }
            else if (nCol == AVERAGE) {
                for (int i = 0; i < m_pVerticalHeadTable->rowCount(); i++) {
                    m_pVerticalHeadTable->setColumnWidth(i, nWidth);
                }
                m_pVerticalHeadTable->setFixedWidth(nWidth * m_pVerticalHeadTable->columnCount());
            }
            else {
                int differ = nWidth - m_pVerticalHeadTable->columnWidth(nCol);
                m_pVerticalHeadTable->setColumnWidth(nCol, nWidth);
                m_pVerticalHeadTable->setFixedWidth(differ + m_pVerticalHeadTable->height());
            }
            m_pSectionTable->setFixedWidth(m_pVerticalHeadTable->width());
            m_pSectionTable->setColumnWidth(0, m_pVerticalHeadTable->width());
        }

        void RichTableView::SetTableColWidth(int nCol, int nWidth)
        {
            if (nCol == TOTAL) {
                for (int i = 0; i < m_pHorizontalHeadTable->columnCount(); i++) {
                    m_pHorizontalHeadTable->setColumnWidth(i, nWidth / m_pHorizontalHeadTable->columnCount());
                    m_pDataTable->setColumnWidth(i, nWidth / m_pHorizontalHeadTable->columnCount());
                }
            }
            else if (nCol == AVERAGE) {
                nItemDefaultWidth = nWidth;
                for (int i = 0; i < m_pHorizontalHeadTable->columnCount(); i++) {
                    m_pHorizontalHeadTable->setColumnWidth(i, nWidth);
                    m_pDataTable->setColumnWidth(i, nWidth);
                }
            }
            else {
                m_pHorizontalHeadTable->setColumnWidth(nCol, nWidth);
                m_pDataTable->setColumnWidth(nCol, nWidth);
            }
        }

        void RichTableView::SetTableRowlHeight(int nRow, int nHeight)
        {
            if (nRow == TOTAL) {
                for (int i = 0; i < m_pVerticalHeadTable->rowCount(); i++) {
                    m_pVerticalHeadTable->setRowHeight(i, nHeight / m_pVerticalHeadTable->rowCount());
                    m_pDataTable->setRowHeight(i, nHeight / m_pVerticalHeadTable->rowCount());
                }
            }
            else if (nRow == AVERAGE) {
                nItemDefaultHeight = nHeight;
                for (int i = 0; i < m_pVerticalHeadTable->rowCount(); i++) {
                    m_pVerticalHeadTable->setRowHeight(i, nHeight);
                    m_pDataTable->setRowHeight(i, nHeight);
                }
            }
            else {
                m_pVerticalHeadTable->setRowHeight(nRow, nHeight);
                m_pDataTable->setRowHeight(nRow, nHeight);
            }
        }

        void RichTableView::SetTableCellSize( int nCol, int nRow, QSize size)
        {
            SetTableColWidth(nCol, size.width());
            SetTableRowlHeight(nRow, size.height());
        }

        void RichTableView::ShowHorizontalHeadIndex(bool bShow, int nRow)
        {
            if (m_bHorizontalHeadShowIndex == bShow) {
                return;
            }
            else {
                m_bHorizontalHeadShowIndex = bShow;
            }

            if (m_bHorizontalHeadShowIndex) {
                if (nRow == LAST) {
                    nHorizontalIndexRow = m_pHorizontalHeadTable->rowCount();
                }
                else {
                    nHorizontalIndexRow = nRow;
                }
                m_pHorizontalHeadTable->insertRow(nHorizontalIndexRow);
                m_pHorizontalHeadTable->setRowHeight(nHorizontalIndexRow, nHorizontalIndexDefaultHeight);
                m_pHorizontalHeadTable->setFixedHeight(m_pHorizontalHeadTable->height() + nHorizontalIndexDefaultHeight);
                for (int i = 0; i < m_pHorizontalHeadTable->columnCount(); i++) {
                    m_pHorizontalHeadTable->setItem(nHorizontalIndexRow, i, new QTableWidgetItem(QString::number(i + 1)));
                    m_pHorizontalHeadTable->item(nHorizontalIndexRow, i)->setTextAlignment(Qt::AlignCenter);
                }
            }
            else {
                m_pHorizontalHeadTable->removeRow(nHorizontalIndexRow);
                m_pHorizontalHeadTable->setFixedHeight(m_pHorizontalHeadTable->height() - nHorizontalIndexDefaultHeight);
            }
            
            m_pSectionTable->setFixedHeight(m_pHorizontalHeadTable->height());
            m_pSectionTable->setRowHeight(0, m_pHorizontalHeadTable->height());
        }

        void RichTableView::ShowVerticalHeadIndex(bool bShow, int nCol)
        {
            if (m_bVerticalHeadShowIndex == bShow) {
                return;
            }
            else {
                m_bVerticalHeadShowIndex = bShow;
            }
                
            if (bShow) {
                if (nCol == LAST) {
                    nVerticalIndexCol = m_pHorizontalHeadTable->rowCount();
                }
                else {
                    nVerticalIndexCol = nCol;
                }
                m_pVerticalHeadTable->insertColumn(nVerticalIndexCol);
                m_pVerticalHeadTable->setColumnWidth(nVerticalIndexCol, nVerticalIndexDefaultWidth);
                m_pVerticalHeadTable->setFixedWidth(m_pVerticalHeadTable->width() + nVerticalIndexDefaultWidth);
                for (int i = 0; i < m_pVerticalHeadTable->rowCount(); i++) {
                    m_pVerticalHeadTable->setItem(i, nVerticalIndexCol, new QTableWidgetItem(QString::number(i + 1)));
                    m_pVerticalHeadTable->item(i, nVerticalIndexCol)->setTextAlignment(Qt::AlignCenter);
                }
            }
            else {
                m_pVerticalHeadTable->removeColumn(nVerticalIndexCol);
                m_pVerticalHeadTable->setFixedWidth(m_pVerticalHeadTable->width() - nVerticalIndexDefaultWidth);
            }
            m_pSectionTable->setFixedWidth(m_pVerticalHeadTable->width());
            m_pSectionTable->setColumnWidth(0, m_pVerticalHeadTable->width());
        }

        void RichTableView::UpdataVerticalHeadIndex()
        {
            if (m_bVerticalHeadShowIndex) {
                QTableWidgetItem* pItem = nullptr;
                for (int i = 0; i < m_pDataTable->rowCount(); i++) {
                    pItem = m_pVerticalHeadTable->item(i, nVerticalIndexCol);
                    if (pItem) {
                        pItem->setText(QString::number(i + 1));
                    }
                    else {
                        m_pVerticalHeadTable->setItem(i, nVerticalIndexCol, new QTableWidgetItem(QString::number(i + 1)));
                        m_pVerticalHeadTable->item(i, nVerticalIndexCol)->setTextAlignment(Qt::AlignCenter);
                    }
                }
            }
        }

        void RichTableView::setStretchLastSection(int nType, bool bStretch)
        {
            if (nType == HHEAD) {
                m_pHorizontalHeadTable->horizontalHeader()->setStretchLastSection(bStretch);
            }
            else if (nType == VHEAD) {
                m_pVerticalHeadTable->verticalHeader()->setStretchLastSection(bStretch);
            }
            else if (nType == ALLHEAD){
                m_pHorizontalHeadTable->horizontalHeader()->setStretchLastSection(bStretch);
                m_pVerticalHeadTable->verticalHeader()->setStretchLastSection(bStretch);
            }
        }

        void RichTableView::AddRows(int nRowNum)
        {
            int nStartRow = m_pDataTable->rowCount();
            m_pDataTable->setRowCount(nStartRow + nRowNum);
            if (m_pVerticalHeadTable->rowCount() < m_pDataTable->rowCount()) {
                m_pVerticalHeadTable->setRowCount(m_pDataTable->rowCount());
            }
            UpdataVerticalHeadIndex();
            for (int i = nStartRow; i < m_pDataTable->rowCount(); i++) {
                m_pVerticalHeadTable->setRowHeight(i, nItemDefaultHeight);
                m_pDataTable->setRowHeight(i, nItemDefaultHeight);
            }
        }

        void RichTableView::VerticalHeadInsterCol(int nCol, bool bShowIndex, int nWidth)
        {
            m_pVerticalHeadTable->insertColumn(nCol);
            m_pVerticalHeadTable->setColumnWidth(nCol, nWidth ? nWidth : nItemDefaultWidth);
            m_listVerticalHead.insert(nCol, bShowIndex);
            VerticalHeadUpdataCol(nCol, bShowIndex);
            m_pVerticalHeadTable->setFixedWidth(m_pVerticalHeadTable->width() + nWidth);
            m_pSectionTable->setFixedWidth(m_pVerticalHeadTable->width());
            m_pSectionTable->setColumnWidth(0, m_pVerticalHeadTable->width());
        }

        void RichTableView::VerticalHeadUpdataCol(int nCol, bool bShowIndex)
        {
            for (int i = 0; i < m_pVerticalHeadTable->rowCount(); i++) {
                if (!m_pVerticalHeadTable->item(i, nCol)) {
                    m_pVerticalHeadTable->setItem(i, nCol, new QTableWidgetItem(m_listVerticalHead[nCol] ? QString::number(i + 1) : QString("")));
                    m_pVerticalHeadTable->item(i, nCol)->setTextAlignment(Qt::AlignCenter);
                }
            }
        }

        void RichTableView::AddVerticalHeadRow(int nNeedRow)
        {
            if (nNeedRow > m_pVerticalHeadTable->rowCount()) {
                AddRows(nNeedRow - m_pVerticalHeadTable->rowCount());
                for (int i = 0; i < m_pVerticalHeadTable->columnCount() - m_bVerticalHeadShowIndex; i++) {
                    VerticalHeadUpdataCol(i, m_listVerticalHead[i]);
                }
            }
        }

        void RichTableView::AddTableData(QVector<TableRowDataInfo> vecTableData, QVector<QStringList> vecHeadData, Qt::Alignment alignment)
        {
            AddVerticalHeadRow(vecTableData.size());

            QTableWidgetItem* item = nullptr;
            for (int i = 0; i < vecTableData.size(); i++) {
                for (int j = 0; j < vecTableData[i].vecTableRowData.size(); j++) {
                    if (j < m_pDataTable->columnCount()) {
                        item = new QTableWidgetItem(vecTableData[i].vecTableRowData[j]);
                        item->setTextAlignment(alignment);
                        item->setFont(vecTableData[i].font);
                        item->setTextColor(vecTableData[i].cTextColor);
                        item->setBackgroundColor(vecTableData[i].cBkColor);
                        m_pDataTable->setItem(i + m_pDataTable->rowCount(), j, item);
                    }
                }

                if (vecHeadData.size() > 0) {
                    for (int j = 0; j < vecHeadData[i].size(); j++) {
                        if (j < m_pVerticalHeadTable->columnCount() - m_bVerticalHeadShowIndex && !vecHeadData[i][j].isEmpty()) {
                            item = new QTableWidgetItem(vecHeadData[i][j]);
                            item->setTextAlignment(alignment);
                            m_pVerticalHeadTable->setItem(i + m_pDataTable->rowCount(), j, item);
                        }
                    }
                }
            }
        }

        void RichTableView::SetTableData(QVector<TableRowDataInfo> vecTableData, int nStartRow, QVector<QStringList> vecHeadData, Qt::Alignment alignment)
        {
            AddVerticalHeadRow(nStartRow + vecHeadData.size());

            QTableWidgetItem* item = nullptr;
            for (int i = 0; i < vecTableData.size(); i++) {
                for (int j = 0; j < vecTableData[i].vecTableRowData.size(); j++) {
                    if (j < m_pDataTable->columnCount()) {
                        item = new QTableWidgetItem(vecTableData[i].vecTableRowData[j]);
                        item->setTextAlignment(alignment);
                        item->setFont(vecTableData[i].font);
                        item->setTextColor(vecTableData[i].cTextColor);
                        item->setBackgroundColor(vecTableData[i].cBkColor);
                        m_pDataTable->setItem(i + nStartRow, j, item);
                    }
                }
                if (vecHeadData.size() > 0) {
                    for (int j = 0; j < vecHeadData[i].size(); j++) {
                        if (j < m_pVerticalHeadTable->columnCount() - m_bVerticalHeadShowIndex && !vecHeadData[i][j].isEmpty()) {
                            item = new QTableWidgetItem(vecHeadData[i][j]);
                            item->setTextAlignment(alignment);
                            m_pVerticalHeadTable->setItem(i + nStartRow, j, item);
                        }
                    }
                }
            }
        }

        void RichTableView::SetTableRowData(int nRow, TableRowDataInfo infoTableRowData, QStringList strHeadData, Qt::Alignment alignment)
        {
            if (nRow >= m_pDataTable->rowCount()) {
                return;
            }
            QTableWidgetItem* item = nullptr;
            for (int i = 0; i < infoTableRowData.vecTableRowData.size(); i++) {
                if (i < m_pDataTable->columnCount()) {
                    item = new QTableWidgetItem(infoTableRowData.vecTableRowData[i]);
                    item->setTextAlignment(alignment);
                    item->setFont(infoTableRowData.font);
                    item->setTextColor(infoTableRowData.cTextColor);
                    item->setBackgroundColor(infoTableRowData.cBkColor);
                    m_pDataTable->setItem(nRow, i, item);
                }
            }
            for (int i = 0; i < strHeadData.size(); i++) {
                if (i < m_pVerticalHeadTable->columnCount() - m_bVerticalHeadShowIndex && !strHeadData[i].isEmpty()) {
                    item = new QTableWidgetItem(strHeadData[i]);
                    item->setTextAlignment(alignment);
                    m_pVerticalHeadTable->setItem(nRow, i, item);
                }
            }
        }

        void RichTableView::SetTableRowTextColor(int row, QColor cColor)
        {
            for (int i = 0; i < m_pDataTable->columnCount(); i++) {
                m_pDataTable->item(row, i)->setData(Qt::TextColorRole, cColor);
            }
        }

        void RichTableView::SetItemText(int nRow, int nCol, QString strText)
        {
            QTableWidgetItem* item = m_pDataTable->item(nRow, nCol);
            if (item) {
                item->setText(strText);
            }
        }

        void RichTableView::ClearData()
        {
            QTableWidgetItem* item = nullptr;
            for (int i = 0; i < m_pDataTable->rowCount(); i++) {
                for (int j = 0; j < m_pDataTable->columnCount(); j++) {
                    item = m_pDataTable->takeItem(i, j);
                    if (item) {
                        item->setText("");
                        delete item;
                        item = nullptr;
                    }
                }

                for (int j = 0; j < m_pVerticalHeadTable->columnCount(); j++) {
                    item = m_pVerticalHeadTable->item(i, j);
                    if (item) {
                        item->setText(m_listVerticalHead[j] ? QString::number(i + 1) : "");
                    }
                }
            }
        }

        void RichTableView::AddComboBox(int nRow, int nCol, const QStringList& strList, const QString& currText, std::function<void(const QString&)> CallBackFun)
        {
            QComboBox* pCombox = new QComboBox(this); // 下拉选择框控件
            pCombox->installEventFilter(this);
            pCombox->addItems(strList);
            m_pDataTable->setCellWidget(nRow, nCol, pCombox);
            pCombox->setCurrentText(currText);
            connect(pCombox, &QComboBox::currentTextChanged, CallBackFun);
        }
    }
}