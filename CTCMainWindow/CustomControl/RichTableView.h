#pragma once
#include <QTableWidget>
#include <QHeaderView>

#define     HHEAD       0x01 
#define     VHEAD       0x02 
#define     ALLHEAD     0x03

#define     TOTAL     -1 
#define     AVERAGE   -2

#define     LAST      -1 

namespace CTCWindows {
    namespace  Control {
        //坐标
        struct Coordinate
        {
            int nX = 0;
            int nY = 0;

        public:
            Coordinate() {}
            Coordinate(int x, int y) 
            {
                nX = x;
                nY = y;
            }
        };

        struct TableHeadInfo
        {
            Coordinate cStart;  //起始坐标
            int nColCell = 0; //占用列数
            int nRowCell = 0; //占用行数
            QString strData;

            TableHeadInfo(int x, int y, int nCol, int nRow, QString data)
            {
                cStart = { x, y };
                nColCell = nCol;
                nRowCell = nRow;
                strData = data;
            }
        };

        class RichTableView : public QWidget
        {
            Q_OBJECT
        public:
            explicit RichTableView(QWidget* parent = nullptr);
            ~RichTableView();

        private:
            void InitTableView();
            bool eventFilter(QObject* obj, QEvent* event);

        public:
            void TableHeadInit(int nType, int nRow, int nCol, QVector<TableHeadInfo> vecTableHeadInfo);

            void SetHorizontalHeadHeight(int nHeight, int nRow = TOTAL);
            void SetVerticalHeadWidth(int nWidth, int nCol = TOTAL);
            void SetTableColWidth(int nCol, int nWidth);
            void SetTableRowlHeight(int nRow, int nHeight);
            void SetTableCellSize(int nCol, int nRow, QSize size);
            void ShowHorizontalHeadIndex(bool bShow = true, int nRow = 0);
            void ShowVerticalHeadIndex(bool bShow = true, int nCol = 0);
            void UpdataVerticalHeadIndex();
            void setStretchLastSection(int nType, bool bStretch);

            void AddRows(int nRowNum);
            void VerticalHeadInsterCol(int nCol, bool bShowIndex = true, int nWidth = 0);

        public:
            void SetHorizontalHeadShowIndex(bool bShow = true) { m_bHorizontalHeadShowIndex = bShow; }
            void SetVerticalHeadShowIndex(bool bShow = true) { m_bVerticalHeadShowIndex = bShow; }
            void SetDeItemDefaultHeight(int nHeight) { nItemDefaultHeight = nHeight; }
            void SetDeItemDefaultWidth(int nWidth) { nItemDefaultWidth = nWidth; }
            void SetHorizontalIndexDefaultHeight(int nHeight) { nHorizontalIndexDefaultHeight = nHeight; }
            void SetVerticalIndexDefaultWidth(int nWidth) { nVerticalIndexDefaultWidth = nWidth; }

        private:
            QScrollBar* m_pHorizontalScrollBar = nullptr;
            QScrollBar* m_pVerticalScrollBar = nullptr;
            QTableWidget* m_pSectionTable = nullptr;        //左上角表格
            QTableWidget* m_pHorizontalHeadTable = nullptr; //横向表头
            QTableWidget* m_pVerticalHeadTable = nullptr;   //纵向表头
            QTableWidget* m_pDataTable = nullptr;           //数据表格    

            int nItemDefaultHeight = 24;
            int nItemDefaultWidth = 64;

            bool m_bHorizontalHeadShowIndex = false;
            bool m_bVerticalHeadShowIndex = false;
            int nHorizontalIndexRow = 0;
            int nVerticalIndexCol = 0;
            int nHorizontalIndexDefaultHeight = 24;
            int nVerticalIndexDefaultWidth = 48;
        };
    }
}