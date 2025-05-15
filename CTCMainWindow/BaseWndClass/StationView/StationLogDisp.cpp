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
                if (index.row() >= 0 && index.row() < Station::MainStation()->TrafficLogList().size()) {
                    m_pCurTrafficLog = Station::MainStation()->TrafficLogList().at(index.row());
                    if (m_pCurTrafficLog) {
                        ShowHeadTableClickMenu(QCursor::pos());
                    }
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

            QByteArray btResult;
            QDateTime tAdjAgrDepartTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            QMap<QString, QByteArray> m_mapLogValue = { 
                { m_mapPointReportType[TrafficLogInfo::AdjAgrDepartTime], m_pCurTrafficLog->m_tAdjAgrDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
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
            }
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

            QByteArray btResult;
            QDateTime tAgrAdjDepartTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            QMap<QString, QByteArray> m_mapLogValue = { 
                { m_mapPointReportType[TrafficLogInfo::AgrAdjDepartTime], m_pCurTrafficLog->m_tAgrAdjDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                m_pCurTrafficLog->m_tAgrAdjDepartTime = tAgrAdjDepartTime;
                pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                    m_mapLogICol[TrafficLogInfo::AgrAdjDepartTime], tAgrAdjDepartTime.toString("hh:mm"));

                if (m_pTrafficMsgLog) {
                    m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,同意%2次接车-%3")
                        .arg(Station::MainStation()->getStationName())
                        .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
                        .arg(tAgrAdjDepartTime.toString("hh:mm")));
                }
            }
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

            QByteArray btResult;
            QDateTime tRealArrivalTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            QMap<QString, QByteArray> m_mapLogValue = { 
                { m_mapPointReportType[TrafficLogInfo::RealArrivalTime], m_pCurTrafficLog->m_tRealArrivalTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                m_pCurTrafficLog->m_tRealArrivalTime = tRealArrivalTime;
                pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                    m_mapLogICol[TrafficLogInfo::RealArrivalTime], tRealArrivalTime.toString("hh:mm"));

                if (m_pTrafficMsgLog) {
                    m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,%2次到达-%3")
                        .arg(Station::MainStation()->getStationName())
                        .arg(m_pCurTrafficLog->m_strArrivalTrainNum)
                        .arg(tRealArrivalTime.toString("hh:mm")));
                }
            }
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

            QByteArray btResult;
            QDateTime tRealDepartTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            QMap<QString, QByteArray> m_mapLogValue = { 
                { m_mapPointReportType[TrafficLogInfo::RealDepartTime], m_pCurTrafficLog->m_tRealDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                m_pCurTrafficLog->m_tRealDepartTime = tRealDepartTime;
                pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                    m_mapLogICol[TrafficLogInfo::RealDepartTime], tRealDepartTime.toString("hh:mm"));

                if (m_pTrafficMsgLog) {
                    m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,%2次出发-%3")
                        .arg(Station::MainStation()->getStationName())
                        .arg(m_pCurTrafficLog->m_strDepartTrainNum)
                        .arg(tRealDepartTime.toString("hh:mm")));
                }
            }
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
            
            QByteArray btResult;
            QDateTime tPassThroughTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            QMap<QString, QByteArray> m_mapLogValue = { 
                { m_mapPointReportType[TrafficLogInfo::RealArrivalTime], m_pCurTrafficLog->m_tRealDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                m_pCurTrafficLog->m_tRealDepartTime = tPassThroughTime;
                pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                    m_mapLogICol[TrafficLogInfo::RealArrivalTime], "通过");

                if (m_pTrafficMsgLog) {
                    m_pTrafficMsgLog->AddAdjacentStationMsg(QString("%1,%2次通过-%3")
                        .arg(Station::MainStation()->getStationName())
                        .arg(m_pCurTrafficLog->m_strDepartTrainNum)
                        .arg(tPassThroughTime.toString("hh:mm")));
                }
            }
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

            QByteArray btResult;
            QDateTime tAdjAgrDepartTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            QMap<QString, QByteArray> m_mapLogValue = { 
                { m_mapPointReportType[TrafficLogInfo::AdjAgrDepartTime], m_pCurTrafficLog->m_tAdjAgrDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
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
            }
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

            QByteArray btResult;
            QMap<QString, QByteArray> m_mapLogValue = { 
                { m_mapPointReportType[TrafficLogInfo::RealArrivalTime], m_pCurTrafficLog->m_tRealArrivalTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                m_pCurTrafficLog->m_tRealArrivalTime = QDateTime();
                pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                    m_mapLogICol[TrafficLogInfo::RealArrivalTime], "");
            }
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

            QByteArray btResult;
            QMap<QString, QByteArray> m_mapLogValue = { 
                { m_mapPointReportType[TrafficLogInfo::AdjAgrDepartTime], m_pCurTrafficLog->m_tAdjAgrDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                m_pCurTrafficLog->m_tAdjAgrDepartTime = QDateTime();
                pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                    m_mapLogICol[TrafficLogInfo::AdjAgrDepartTime], "");
            }
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

            QByteArray btResult;
            QMap<QString, QByteArray> m_mapLogValue = { 
                { m_mapPointReportType[TrafficLogInfo::RealDepartTime], m_pCurTrafficLog->m_tRealDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                m_pCurTrafficLog->m_tRealDepartTime = QDateTime();
                pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                    m_mapLogICol[TrafficLogInfo::RealDepartTime], "");
            }
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

            QByteArray btResult;
            QDateTime tAdjDepartTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            QMap<QString, QByteArray> m_mapLogValue = { 
                { m_mapPointReportType[TrafficLogInfo::AdjDepartTime], m_pCurTrafficLog->m_tAdjDepartTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                m_pCurTrafficLog->m_tAdjDepartTime = tAdjDepartTime;
                pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                    m_mapLogICol[TrafficLogInfo::AdjDepartTime], tAdjDepartTime.toString("hh:mm"));
            }
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

            QByteArray btResult;
            QDateTime tAdjArrivalTime = tReportTime.isNull() ? QDateTime::currentDateTime() : tReportTime;
            QMap<QString, QByteArray> m_mapLogValue = { 
                { m_mapPointReportType[TrafficLogInfo::AdjArrivalTime], m_pCurTrafficLog->m_tAdjArrivalTime.toString(Qt::ISODate).toLocal8Bit() }
            };
            if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pCurTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
                m_pCurTrafficLog->m_tAdjArrivalTime = tAdjArrivalTime;
                pTrafficLogTable->SetItemText(Station::MainStation()->TrafficLogList().indexOf(m_pCurTrafficLog),
                    m_mapLogICol[TrafficLogInfo::AdjArrivalTime], tAdjArrivalTime.toString("hh:mm"));
            }
            m_pCurTrafficLog = nullptr;
        }
    }
}