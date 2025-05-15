#include "StationLogDisp.h"
#include <QMessageBox>
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace CTCWindows {
    namespace BaseWnd {

        StationLogDisp::StationLogDisp(QWidget* parent)
        {
            m_mapPointReportType = {
                { TrafficLogInfo::AgrAdjDepartTime, "agrAdjDepartTime" },
                { TrafficLogInfo::AdjDepartTime, "adjDepartTime" },
                { TrafficLogInfo::ProvArrivalTime, "provArrivalTime" },
                { TrafficLogInfo::RealArrivalTime, "realArrivalTime" },
                { TrafficLogInfo::AdjAgrDepartTime, "adjAgrDepartTime" },
                { TrafficLogInfo::ProvDepartTime, "provDepartTime" },
                { TrafficLogInfo::RealDepartTime, "realDepartTime" },
                { TrafficLogInfo::AdjArrivalTime, "adjArrivalTime" },
            };
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
            connect(pTrafficLogTable->VerticalHeadTable(), &QTableWidget::clicked, [=](const QModelIndex& index) {
                qDebug() << index.row() << Station::MainStation()->TrafficLogList().size();
                QPoint globalPos = QCursor::pos();
                if (index.row() >= 0 && index.row() < Station::MainStation()->TrafficLogList().size()) {
                    // 获取当前选中的交通日志
                    m_pCurTrafficLog = Station::MainStation()->TrafficLogList().at(index.row());
                    ClickMenu(globalPos, m_pCurTrafficLog);
                }
                else {
                    // 处理越界情况
                    qDebug() << "Error: Invalid row index clicked on vertical header:" << index.row();
                }
            });
        }
        
        void StationLogDisp::AdjacentStationPreview(const QDateTime& tReportTime)
        {
            if (!m_pCurTrafficLog) {
                QMessageBox::information(this, MSGBOX_TITTLE, "请选定车次");
                return;
            }

            if (m_pCurTrafficLog->m_nPlanType == 0x02) {
                return;
            }  
            QDateTime tAdjAgrDepartTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            m_pCurTrafficLog->m_tAdjAgrDepartTime = tAdjAgrDepartTime;
            int nRow = Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog);
            pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                m_mapLogICol[TrafficLogInfo::AdjAgrDepartTime], tAdjAgrDepartTime.toString("hh:mm"));

            if (m_pTrafficMsgLog) {
                m_pTrafficMsgLog->AddDispatchDeskMsg(QString("%1,%2,要牌预告-%3")
                    .arg(Station::MainStation()->getStationName())
                    .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
                    .arg(tAdjAgrDepartTime.toString("hh:mm")));
                m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,同意%2次发车-%3")
                    .arg(Station::MainStation()->getStationName())
                    .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
                    .arg(tAdjAgrDepartTime.toString("hh:mm")));
            }


            QByteArray btResult;
            QMap<QString, QByteArray>m_mapLogValue = { {m_mapPointReportType[TrafficLogInfo::AdjAgrDepartTime],m_pCurTrafficLog->m_tAdjAgrDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                emit Station::MainStation()->TrafficLogTableUpData();
            }
         
            //if (Http::HttpClient::UpdataPointReportTime(m_pCurTrafficLog->m_nLogId, m_mapPointReportType[TrafficLogInfo::AdjAgrDepartTime], tAdjAgrDepartTime, btResult)) {
            //    m_pCurTrafficLog->m_tAdjAgrDepartTime = tAdjAgrDepartTime;
            //    int nRow = Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog);
            //    pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog), 
            //        m_mapLogICol[TrafficLogInfo::AdjAgrDepartTime], tAdjAgrDepartTime.toString("hh:mm"));
            //
            //    if (m_pTrafficMsgLog) {
            //        m_pTrafficMsgLog->AddDispatchDeskMsg(QString("%1,%2,要牌预告-%3")
            //            .arg(Station::MainStation()->getStationName())
            //            .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
            //            .arg(tAdjAgrDepartTime.toString("hh:mm")));
            //        m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,同意%2次发车-%3")
            //            .arg(Station::MainStation()->getStationName())
            //            .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
            //            .arg(tAdjAgrDepartTime.toString("hh:mm")));
            //    }
            //}
            m_pCurTrafficLog = nullptr;
        }

        void StationLogDisp::AgreeAdjacentStations(const QDateTime& tReportTime)
        {
            if (!m_pCurTrafficLog) {
                QMessageBox::information(this, MSGBOX_TITTLE, "请选定车次");
                return;
            }

            if (m_pCurTrafficLog->m_nPlanType == 0x03) {
                return;
            }
            QDateTime tAgrAdjDepartTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            m_pCurTrafficLog->m_tAgrAdjDepartTime = tAgrAdjDepartTime;
            pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                m_mapLogICol[TrafficLogInfo::AgrAdjDepartTime], tAgrAdjDepartTime.toString("hh:mm"));

            if (m_pTrafficMsgLog) {
                m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,同意%2次接车-%3")
                    .arg(Station::MainStation()->getStationName())
                    .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
                    .arg(tAgrAdjDepartTime.toString("hh:mm")));
            }

            QByteArray btResult;
            QMap<QString, QByteArray>m_mapLogValue = { {m_mapPointReportType[TrafficLogInfo::AgrAdjDepartTime],m_pCurTrafficLog->m_tAgrAdjDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                emit Station::MainStation()->TrafficLogTableUpData();
            }
           
          //  if (Http::HttpClient::UpdataPointReportTime(m_pCurTrafficLog->m_nLogId, m_mapPointReportType[TrafficLogInfo::AgrAdjDepartTime], tAgrAdjDepartTime, btResult)) {
          //      m_pCurTrafficLog->m_tAgrAdjDepartTime = tAgrAdjDepartTime;
          //      pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog), 
          //          m_mapLogICol[TrafficLogInfo::AgrAdjDepartTime], tAgrAdjDepartTime.toString("hh:mm"));
          //
          //      if (m_pTrafficMsgLog) {
          //          m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,同意%2次接车-%3")
          //              .arg(Station::MainStation()->getStationName())
          //              .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
          //              .arg(tAgrAdjDepartTime.toString("hh:mm")));
          //      }
          //  }
            m_pCurTrafficLog = nullptr;
        }

        void StationLogDisp::TrainArrival(const QDateTime& tReportTime)
        {
            if (!m_pCurTrafficLog) {
                QMessageBox::information(this, MSGBOX_TITTLE, "请选定车次");
                return;
            }

            if (m_pCurTrafficLog->m_nPlanType == 0x02) {
                return;
            }

            QDateTime tRealArrivalTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            m_pCurTrafficLog->m_tRealArrivalTime = tRealArrivalTime;
            pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                m_mapLogICol[TrafficLogInfo::RealArrivalTime], tRealArrivalTime.toString("hh:mm"));

            if (m_pTrafficMsgLog) {
                m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,%2次到达-%3")
                    .arg(Station::MainStation()->getStationName())
                    .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
                    .arg(tRealArrivalTime.toString("hh:mm")));
            }

            QByteArray btResult;
            QMap<QString, QByteArray>m_mapLogValue = { {m_mapPointReportType[TrafficLogInfo::RealArrivalTime],m_pCurTrafficLog->m_tRealArrivalTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                emit Station::MainStation()->TrafficLogTableUpData();
            }


          // if (Http::HttpClient::UpdataPointReportTime(m_pCurTrafficLog->m_nLogId, m_mapPointReportType[TrafficLogInfo::RealArrivalTime], tRealArrivalTime, btResult)) {
          //     m_pCurTrafficLog->m_tRealArrivalTime = tRealArrivalTime;
          //     pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog), 
          //         m_mapLogICol[TrafficLogInfo::RealArrivalTime], tRealArrivalTime.toString("hh:mm"));
          //
          //     if (m_pTrafficMsgLog) {
          //         m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,%2次到达-%3")
          //             .arg(Station::MainStation()->getStationName())
          //             .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
          //             .arg(tRealArrivalTime.toString("hh:mm")));
          //     }
          // }
            m_pCurTrafficLog = nullptr;
        }

        void StationLogDisp::TrainDeparture(const QDateTime& tReportTime)
        {
            if (!m_pCurTrafficLog) {
                QMessageBox::information(this, MSGBOX_TITTLE, "请选定车次");
                return;
            }

            if (m_pCurTrafficLog->m_nPlanType == 0x03) {
                return;
            }
            QDateTime tRealDepartTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            m_pCurTrafficLog->m_tRealDepartTime = tRealDepartTime;
            pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                m_mapLogICol[TrafficLogInfo::RealDepartTime], tRealDepartTime.toString("hh:mm"));

            if (m_pTrafficMsgLog) {
                m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,%2次出发-%3")
                    .arg(Station::MainStation()->getStationName())
                    .arg(m_pCurTrafficLog->m_strDepartTrainNum)
                    .arg(tRealDepartTime.toString("hh:mm")));
            }
            QByteArray btResult;
            QMap<QString, QByteArray>m_mapLogValue = { {m_mapPointReportType[TrafficLogInfo::RealDepartTime],m_pCurTrafficLog->m_tRealDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                emit Station::MainStation()->TrafficLogTableUpData();
            }

            //if (Http::HttpClient::UpdataPointReportTime(m_pCurTrafficLog->m_nLogId, m_mapPointReportType[TrafficLogInfo::RealDepartTime], tRealDepartTime, btResult)) {
            //    m_pCurTrafficLog->m_tRealDepartTime = tRealDepartTime;
            //    pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
            //        m_mapLogICol[TrafficLogInfo::RealDepartTime], tRealDepartTime.toString("hh:mm"));
            //
            //    if (m_pTrafficMsgLog) {
            //        m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,%2次出发-%3")
            //            .arg(Station::MainStation()->getStationName())
            //            .arg(m_pCurTrafficLog->m_strDepartTrainNum)
            //            .arg(tRealDepartTime.toString("hh:mm")));
            //    }
            //}
            m_pCurTrafficLog = nullptr;
        }
        
        void StationLogDisp::TrainPassThrough(const QDateTime& tReportTime)
        {
            if (!m_pCurTrafficLog) {
                QMessageBox::information(this, MSGBOX_TITTLE, "请选定车次");
                return;
            }

            if (m_pCurTrafficLog->m_nPlanType == 0x02 || m_pCurTrafficLog->m_nPlanType == 0x03) {
                return ;
            }
            QDateTime tPassThroughTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            m_pCurTrafficLog->m_tRealDepartTime = tPassThroughTime;
            pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                m_mapLogICol[TrafficLogInfo::RealArrivalTime], "通过");

            if (m_pTrafficMsgLog) {
                m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,%2次通过-%3")
                    .arg(Station::MainStation()->getStationName())
                    .arg(m_pCurTrafficLog->m_strDepartTrainNum)
                    .arg(tPassThroughTime.toString("hh:mm")));
            }
            QByteArray btResult;
            QMap<QString, QByteArray>m_mapLogValue = { {m_mapPointReportType[TrafficLogInfo::RealArrivalTime],m_pCurTrafficLog->m_tRealDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                emit Station::MainStation()->TrafficLogTableUpData();
            }
           
           // if (Http::HttpClient::UpdataPointReportTime(m_pCurTrafficLog->m_nLogId, m_mapPointReportType[TrafficLogInfo::RealArrivalTime], tPassThroughTime, btResult)) {
           //     m_pCurTrafficLog->m_tRealDepartTime = tPassThroughTime;
           //     pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
           //         m_mapLogICol[TrafficLogInfo::RealArrivalTime], "通过");
           //
           //     if (m_pTrafficMsgLog) {
           //         m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,%2次通过-%3")
           //             .arg(Station::MainStation()->getStationName())
           //             .arg(m_pCurTrafficLog->m_strDepartTrainNum)
           //             .arg(tPassThroughTime.toString("hh:mm")));
           //     }
           // }
            m_pCurTrafficLog = nullptr;
        }
        
        void StationLogDisp::AdjacentStation(const QDateTime& tReportTime)
        {
            if (!m_pCurTrafficLog) {
                QMessageBox::information(this, MSGBOX_TITTLE, "请选定车次");
                return;
            }

            if (m_pCurTrafficLog->m_nPlanType == 0x02) {
                return;
            }


            QDateTime tAdjAgrDepartTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            m_pCurTrafficLog->m_tAdjAgrDepartTime = tAdjAgrDepartTime;
            pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                m_mapLogICol[TrafficLogInfo::AdjAgrDepartTime], tAdjAgrDepartTime.toString("hh:mm"));

            if (m_pTrafficMsgLog) {
                m_pTrafficMsgLog->AddDispatchDeskMsg(QString("%1,%2,要牌预告-%3")
                    .arg(Station::MainStation()->getStationName())
                    .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
                    .arg(tAdjAgrDepartTime.toString("hh:mm")));
                m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,同意%2次发车-%3")
                    .arg(Station::MainStation()->getStationName())
                    .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
                    .arg(tAdjAgrDepartTime.toString("hh:mm")));
            }

            QByteArray btResult;
            QMap<QString, QByteArray>m_mapLogValue = { {m_mapPointReportType[TrafficLogInfo::AdjAgrDepartTime],m_pCurTrafficLog->m_tAdjAgrDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                emit Station::MainStation()->TrafficLogTableUpData();
            }

            //if (Http::HttpClient::UpdataPointReportTime(m_pCurTrafficLog->m_nLogId, m_mapPointReportType[TrafficLogInfo::AdjAgrDepartTime], tAdjAgrDepartTime, btResult)) {
            //    m_pCurTrafficLog->m_tAdjAgrDepartTime = tAdjAgrDepartTime;
            //    pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
            //        m_mapLogICol[TrafficLogInfo::AdjAgrDepartTime], tAdjAgrDepartTime.toString("hh:mm"));
            //
            //    if (m_pTrafficMsgLog) {
            //        m_pTrafficMsgLog->AddDispatchDeskMsg(QString("%1,%2,要牌预告-%3")
            //            .arg(Station::MainStation()->getStationName())
            //            .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
            //            .arg(tAdjAgrDepartTime.toString("hh:mm")));
            //        m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,同意%2次发车-%3")
            //            .arg(Station::MainStation()->getStationName())
            //            .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
            //            .arg(tAdjAgrDepartTime.toString("hh:mm")));
            //    }
            //}
            m_pCurTrafficLog = nullptr;
        }

        void StationLogDisp::CancelReceiving()
        {
            if (!m_pCurTrafficLog) {
                QMessageBox::information(this, MSGBOX_TITTLE, "请选定车次");
                return;
            }

            if (m_pCurTrafficLog->m_nPlanType == 0x02) {
                return;
            }
            m_pCurTrafficLog->m_tRealArrivalTime = QDateTime();
            pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                m_mapLogICol[TrafficLogInfo::RealArrivalTime], "");
            QByteArray btResult;
            QMap<QString, QByteArray>m_mapLogValue = { {m_mapPointReportType[TrafficLogInfo::RealArrivalTime],m_pCurTrafficLog->m_tRealArrivalTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                emit Station::MainStation()->TrafficLogTableUpData();
            }

           // if (Http::HttpClient::UpdataPointReportTime(m_pCurTrafficLog->m_nLogId, m_mapPointReportType[TrafficLogInfo::RealArrivalTime], QDateTime(), btResult)) {
           //     m_pCurTrafficLog->m_tRealArrivalTime = QDateTime();
           //     pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog), 
           //         m_mapLogICol[TrafficLogInfo::RealArrivalTime], "");
           // }
            m_pCurTrafficLog = nullptr;
        }

        void StationLogDisp::CancelBlock()
        {
            if (!m_pCurTrafficLog) {
                QMessageBox::information(this, MSGBOX_TITTLE, "请选定车次");
                return;
            }

            if (m_pCurTrafficLog->m_nPlanType == 0x03) {
                return;
            }
            m_pCurTrafficLog->m_tAdjAgrDepartTime = QDateTime();
            pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                m_mapLogICol[TrafficLogInfo::AdjAgrDepartTime], "");
            QByteArray btResult;
            QMap<QString, QByteArray>m_mapLogValue = { {m_mapPointReportType[TrafficLogInfo::AdjAgrDepartTime],m_pCurTrafficLog->m_tAdjAgrDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                emit Station::MainStation()->TrafficLogTableUpData();
            }
          // if (Http::HttpClient::UpdataPointReportTime(m_pCurTrafficLog->m_nLogId, m_mapPointReportType[TrafficLogInfo::AdjAgrDepartTime], QDateTime(), btResult)) {
          //     m_pCurTrafficLog->m_tAdjAgrDepartTime = QDateTime();
          //     pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
          //         m_mapLogICol[TrafficLogInfo::AdjAgrDepartTime], "");
          // }
            m_pCurTrafficLog = nullptr;
        }

        void StationLogDisp::CancelDepart()
        {
            if (!m_pCurTrafficLog) {
                QMessageBox::information(this, MSGBOX_TITTLE, "请选定车次");
                return;
            }

            if (m_pCurTrafficLog->m_nPlanType == 0x02) {
                return;
            }
            m_pCurTrafficLog->m_tRealDepartTime = QDateTime();
            pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                m_mapLogICol[TrafficLogInfo::RealDepartTime], "");

            QByteArray btResult;
            QMap<QString, QByteArray>m_mapLogValue = { {m_mapPointReportType[TrafficLogInfo::RealDepartTime],m_pCurTrafficLog->m_tRealDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                emit Station::MainStation()->TrafficLogTableUpData();
            }

           //if (Http::HttpClient::UpdataPointReportTime(m_pCurTrafficLog->m_nLogId, m_mapPointReportType[TrafficLogInfo::RealDepartTime], QDateTime(), btResult)) {
           //    m_pCurTrafficLog->m_tRealDepartTime = QDateTime();
           //    pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
           //        m_mapLogICol[TrafficLogInfo::RealDepartTime], "");
           //}
            m_pCurTrafficLog = nullptr;
        }

        void StationLogDisp::AdjacentStationDepart(const QDateTime& tReportTime)
        {
            if (!m_pCurTrafficLog) {
                QMessageBox::information(this, MSGBOX_TITTLE, "请选定车次");
                return;
            }

            if (m_pCurTrafficLog->m_nPlanType == 0x02) {
                return;
            }

            QDateTime tAdjDepartTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            m_pCurTrafficLog->m_tAdjDepartTime = tAdjDepartTime;
            pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                m_mapLogICol[TrafficLogInfo::AdjDepartTime], tAdjDepartTime.toString("hh:mm"));

            QByteArray btResult;
            QMap<QString, QByteArray>m_mapLogValue = { {m_mapPointReportType[TrafficLogInfo::AdjDepartTime],m_pCurTrafficLog->m_tAdjDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                emit Station::MainStation()->TrafficLogTableUpData();
            }

          //  if (Http::HttpClient::UpdataPointReportTime(m_pCurTrafficLog->m_nLogId, m_mapPointReportType[TrafficLogInfo::AdjDepartTime], QDateTime(), btResult)) {
          //      m_pCurTrafficLog->m_tAdjDepartTime = tAdjDepartTime;
          //      pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
          //          m_mapLogICol[TrafficLogInfo::AdjDepartTime], tAdjDepartTime.toString("hh:mm"));
          //  }
            m_pCurTrafficLog = nullptr;
        }

        void StationLogDisp::AdjacentStationArrival(const QDateTime& tReportTime)
        {
            if (!m_pCurTrafficLog) {
                QMessageBox::information(this, MSGBOX_TITTLE, "请选定车次");
                return;
            }

            if (m_pCurTrafficLog->m_nPlanType == 0x03) {
                return;
            }
            QDateTime tAdjArrivalTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            m_pCurTrafficLog->m_tAdjArrivalTime = tAdjArrivalTime;
            pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                m_mapLogICol[TrafficLogInfo::AdjArrivalTime], tAdjArrivalTime.toString("hh:mm"));
            QByteArray btResult;
            QMap<QString, QByteArray>m_mapLogValue = { {m_mapPointReportType[TrafficLogInfo::AdjArrivalTime],m_pCurTrafficLog->m_tAdjArrivalTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::ChangeStaTrafficLogData(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                emit Station::MainStation()->TrafficLogTableUpData();
            }
           // QDateTime tAdjArrivalTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
           // if (Http::HttpClient::UpdataPointReportTime(m_pCurTrafficLog->m_nLogId, m_mapPointReportType[TrafficLogInfo::AdjArrivalTime], QDateTime(), btResult)) {
           //     m_pCurTrafficLog->m_tAdjArrivalTime = tAdjArrivalTime;
           //     pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
           //         m_mapLogICol[TrafficLogInfo::AdjArrivalTime], tAdjArrivalTime.toString("hh:mm"));
           // }
            m_pCurTrafficLog = nullptr;
        }
    }
}