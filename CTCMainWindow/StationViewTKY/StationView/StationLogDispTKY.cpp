#include "StationLogDispTKY.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CARS {
		StationLogDispTKY::StationLogDispTKY(QWidget* parent)
			: StationLogDisp(parent)
		{
			m_mapLogICol = {
				{ TrafficLogInfo::ArrivalTrainNum, 0 },
				{ TrafficLogInfo::ArrivalTrack, 1 },
				{ TrafficLogInfo::AgrAdjDepartTime, 2 },
				{ TrafficLogInfo::AdjDepartTime, 3 },
				{ TrafficLogInfo::ProvArrivalTime, 4 },
				{ TrafficLogInfo::RealArrivalTime, 5 },
				{ TrafficLogInfo::ArrivalDriver, 8 },
				{ TrafficLogInfo::ArrivalTrainMaster, 9 },
				{ TrafficLogInfo::ArrivalTrainValue, 10 },
				{ TrafficLogInfo::ArrivalChange, 11 },
				{ TrafficLogInfo::ArrivalWeight, 12 },
				{ TrafficLogInfo::DepartTrainNum, 13 },
				{ TrafficLogInfo::DepartTrack, 14 },
				{ TrafficLogInfo::AdjAgrDepartTime, 16 },
				{ TrafficLogInfo::ProvDepartTime, 17 },
				{ TrafficLogInfo::RealDepartTime, 18 },
				{ TrafficLogInfo::AdjArrivalTime, 19 },
				{ TrafficLogInfo::DepartDriver, 21 },
				{ TrafficLogInfo::DepartTrainMaster, 22 },
				{ TrafficLogInfo::DepartTrainValue, 23 },
				{ TrafficLogInfo::DepartChange, 24 },
				{ TrafficLogInfo::DepartWeight, 25 },
				{ TrafficLogInfo::DelayReason, 26 },
				{ TrafficLogInfo::PlanAttributes, 27 },
				{ TrafficLogInfo::Notes, 28 },
			};
			ui.setupUi(this);
			InitTrafficLogTable();

			//connect(ui.adjPreviewBtn, &QPushButton::clicked, this, &StationLogDisp::AdjacentStationPreview);
			//connect(ui.agreeAdjBtn, &QPushButton::clicked, this, &StationLogDisp::AgreeAdjacentStations);
			//connect(ui.arrivalBtn, &QPushButton::clicked, this, &StationLogDisp::TrainArrival);
			//connect(ui.departureBtn, &QPushButton::clicked, this, &StationLogDisp::TrainDeparture);
			//connect(ui.throughBtn, &QPushButton::clicked, this, &StationLogDisp::TrainPassThrough);
			//connect(ui.adjAgrBtn, &QPushButton::clicked, this, &StationLogDisp::AdjacentStation);
			//
			////connect(ui.supPointBtn, &QPushButton::clicked, this, &StationLogDisp::AdjacentStation);
			//connect(ui.cancelBlockBtn, &QPushButton::clicked, this, &StationLogDisp::CancelBlock);
			//connect(ui.cancelDepartBtn, &QPushButton::clicked, this, &StationLogDisp::CancelDepart);
		}

		StationLogDispTKY::~StationLogDispTKY()
		{
		}

		void StationLogDispTKY::InitTrafficLogTableHead()
		{
			QVector<Control::TableHeadInfo> vecTableHeadInfo = {
				{ 0, 0, 13, 1, "到达"},
					{ 1, 0, 1, 4, "列车车次"},
					{ 1, 1, 1, 4, "到达股道"},
					{ 1, 2, 5, 1, "时分"},
						{ 2, 2, 1, 3, "同意邻站发车"},
						{ 2, 3, 1, 3, "邻站发车"},
						{ 2, 4, 2, 1, "本站到达"},
							{ 3, 4, 1, 2, "规定"},
							{ 3, 5, 1, 2, "实际"},
						{ 2, 6, 1, 3, "机车入库"},
					{ 1, 7, 1, 4, "机车型号"},
					{ 1, 8, 2, 1, "乘务员姓名"},
						{ 2, 8, 1, 3, "司机"},
						{ 2, 9, 1, 3, "车长"},
					{ 1, 10, 3, 1, "列车编组"},
						{ 2, 10, 1, 3, "车数"},
						{ 2, 11, 1, 3, "换长"},
						{ 2, 12, 1, 3, "总重"},

				{ 0, 13, 14, 1, "出发"},
					{ 1, 13, 1, 4, "列车车次"},
					{ 1, 14, 1, 4, "出发股道"},
					{ 1, 15, 5, 1, "时分"},
						{ 2, 15, 1, 3, "机车出库"},
						{ 2, 16, 1, 3, "邻站同意发车"},
						{ 2, 17, 2, 1, "本站出发"},
							{ 3, 17, 1, 2, "规定"},
							{ 3, 18, 1, 2, "实际"},
						{ 2, 19, 1, 3, "邻站到达"},
					{ 1, 20, 1, 4, "机车型号"},
					{ 1, 21, 2, 1, "乘务员姓名"},
						{ 2, 21, 1, 3, "司机"},
						{ 2, 22, 1, 3, "车长"},
					{ 1, 23, 3, 1, "列车编组"},
						{ 2, 23, 1, 3, "车数"},
						{ 2, 24, 1, 3, "换长"},
						{ 2, 25, 1, 3, "总重"},
					{ 1, 26, 1, 4, "列车出发晚点原因"},
				{ 0, 27, 1, 5, "列车属性"},
				{ 0, 28, 1, 5, "记事"},
			};
			pTrafficLogTable->TableHeadInit(5, 29, vecTableHeadInfo);
		}

		void StationLogDispTKY::AddTrafficLogTable()
		{
			ui.frame->layout()->addWidget(pTrafficLogTable);
		}

		void StationLogDispTKY::OnTrafficLogTableUpData()
		{
			QVector<Control::TableRowDataInfo> vecTableData;
			Station::StaTrain* pTrain = nullptr; 
			QColor txtColor;
			for (Station::StaTrafficLog* pTrafficLog : Station::MainStation()->TrafficLogList()) {
				pTrain = Station::MainStation()->getStaTrainById(pTrafficLog->m_nTrainId);
				if (pTrain) {
					if (pTrafficLog->IsReportedPoints()) {
						txtColor = pTrain->m_bFreightTrain ? QColor(213, 30, 218) : QColor(85, 120, 182);
					}
					else {
						txtColor = pTrain->m_bFreightTrain ? Qt::red : Qt::blue;
					}
					
					QStringList strDataList = QStringList()
						<< pTrafficLog->m_strArrivalTrainNum
						<< pTrafficLog->m_strArrivalTrack
						<< pTrafficLog->m_tAgrAdjDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tAdjDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tProvArrivalTime.toString("hh:mm")
						<< pTrafficLog->m_tRealArrivalTime.toString("hh:mm")
						<< "" //机车入库
						<< pTrain->m_strTrainType
						<< pTrafficLog->m_strArrivalDriver
						<< pTrafficLog->m_nArrivalTrainMaster
						<< QString("%1").arg(pTrafficLog->m_nArrivalTrainValue)
						<< QString("%1").arg(pTrafficLog->m_nArrivalChange)
						<< QString("%1").arg(pTrafficLog->m_nArrivalWeight)
						<< pTrafficLog->m_strDepartTrainNum
						<< pTrafficLog->m_strDepartTrack
						<< "" //机车出库
						<< pTrafficLog->m_tAdjAgrDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tProvDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tRealDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tAdjArrivalTime.toString("hh:mm")
						<< pTrain->m_strTrainType
						<< pTrafficLog->m_strDepartDriver
						<< QString("%1").arg(pTrafficLog->m_nDepartTrainMaster)
						<< QString("%1").arg(pTrafficLog->m_nDepartTrainValue)
						<< QString("%1").arg(pTrafficLog->m_nDepartChange)
						<< QString("%1").arg(pTrafficLog->m_nDepartWeight)
						<< pTrafficLog->m_strDelayReason
						<< pTrain->m_strOperationType
						<< pTrafficLog->m_strNotes;

					vecTableData.append(Control::TableRowDataInfo(strDataList, txtColor));
				}
			}
			pTrafficLogTable->ClearData();
			pTrafficLogTable->SetTableData(vecTableData);
		}
	}
}
