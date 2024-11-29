#pragma once
#include <QTableWidget>
#include <QHeaderView>

#define     HHead       0x01 
#define     VHead       0x02 

namespace CTCWindows {
    namespace  Control {
        struct TableViewHeadInfo
        {
            QString strData;
            int nSize = 0;
            bool bCheckable = false;

        public:
            TableViewHeadInfo() {}
            TableViewHeadInfo(QString data, int size, bool checkable = false)
                : strData(data), nSize(size), bCheckable(checkable) {}
        };

        struct TableViewInfo
        {
            QVector<TableViewHeadInfo> m_vecVHeadInfo;
            QVector<TableViewHeadInfo> m_vecHHeadInfo;
            QVector<QTableWidgetItem*> m_vecStandardItem;
        };

        class TableView : public QTableWidget
        {
            Q_OBJECT
        public:
            explicit TableView(QWidget* parent = nullptr);
            ~TableView();

        public:
            void SetSectionText(QString text, int nWidth);
            void SetHeadData(QVector<TableViewHeadInfo> vecHeadInfo, int nHeadType, Qt::Alignment alignment = Qt::AlignCenter, bool bStretchable = true);
            void AddTableRows(QVector<QStringList> vecTableData, Qt::Alignment alignment = Qt::AlignCenter);
            void ResetTableRows(QVector<QStringList> vecTableData, Qt::Alignment alignment = Qt::AlignCenter);
            void RemoveAllRows();
            void SetRowItemsColor(int nRow, QColor bkcolor, QColor txtcolor, int nStartCol = 0, int nEndCol = -1);
            void SetRowItemsBKColor(int nRow, QColor bkcolor, int nStartCol = 0, int nEndCol = -1);
            void SetRowItemsTextColor(int nRow, QColor txtcolor, int nStartCol = 0, int nEndCol = -1);
            void SetRowItemsTextFont(int nRow, QFont font, int nStartCol = 0, int nEndCol = -1);

            QSize sizeHint() const override { return QSize(width(), minimumHeight()); }

        private:
            void SetTableItem(QVector<QStringList> vecTableData, Qt::Alignment alignment, int nStartRow = 0);
            bool eventFilter(QObject* obj, QEvent* event) override;

        private:
            TableViewInfo m_infoTableView;
        };
    }
}