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

        public:
            TableViewHeadInfo() {}
            TableViewHeadInfo(QString data, int size)
                : strData(data), nSize(size) {}
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
            void SetHeadData(QVector<TableViewHeadInfo> vecHeadInfo, int nHeadType, Qt::Alignment alignment = Qt::AlignCenter, bool bStretchable = true);
            void AddTableRows(QVector<QStringList> vecTableData, Qt::Alignment alignment = Qt::AlignCenter);
            void ResetTableRows(QVector<QStringList> vecTableData, Qt::Alignment alignment = Qt::AlignCenter);
            void RemoveAllRows();
            QSize sizeHint() const { return QSize(width(), minimumHeight()); }

        private:
            TableViewInfo m_infoTableView;
        };
    }
}