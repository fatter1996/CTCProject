#include "StationLogDisp.h"
namespace CTCWindows {
    namespace BaseWnd {

        StationLogDisp::StationLogDisp(QWidget* parent)
        {
        
        }

        StationLogDisp::~StationLogDisp()
        {

        }

        void StationLogDisp::InitTrafficLogTable()
        {
            pTrafficLogTable = new Control::RichTableView;
            InitTrafficLogTableHead();
            pTrafficLogTable->SetHorizontalHeadHeight(24, AVERAGE);
            pTrafficLogTable->SetVerticalHeadWidth(64, AVERAGE);
            pTrafficLogTable->SetTableColWidth(AVERAGE, 64);
            pTrafficLogTable->SetTableRowlHeight(AVERAGE, 24);
            pTrafficLogTable->ShowHorizontalHeadIndex(true, LAST);
            pTrafficLogTable->ShowVerticalHeadIndex(false);
            
            pTrafficLogTable->AddRows(50);
            pTrafficLogTable->VerticalHeadInsterCol(0, true, 72);
           
            pTrafficLogTable->setStretchLastSection(ALLHEAD, false);
            
            AddTrafficLogTable();
        }
    }
}