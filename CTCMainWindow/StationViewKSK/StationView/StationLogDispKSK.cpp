#include "StationLogDispKSK.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CASCO {
		StationLogDispKSK::StationLogDispKSK(QWidget* parent)
			: StationLogDisp(parent)
		{
			m_mapLogICol = {
				{ TrafficLogInfo::ArrivalTrainNumX, 0 },
				{ TrafficLogInfo::ArrivalTrainNumS, 1 },
				{ TrafficLogInfo::ArrivalSignalCode, 2 },
				{ TrafficLogInfo::ArrivalTrack, 3 },
				{ TrafficLogInfo::AgrAdjDepartTime, 4 },
				{ TrafficLogInfo::AdjDepartTime, 5 },
				{ TrafficLogInfo::ProvArrivalTime, 6 },
				{ TrafficLogInfo::RealArrivalTime, 7 },
				{ TrafficLogInfo::ArrivaHeavyVehicle, 8 },
				{ TrafficLogInfo::ArrivaEmpty, 9 },
				{ TrafficLogInfo::ArrivaUonuse, 10 },
				{ TrafficLogInfo::ArrivaCaboose, 11 },
				{ TrafficLogInfo::ArrivaSecVoucher, 12 },
				{ TrafficLogInfo::ArrivalLocomotive, 13 },
				{ TrafficLogInfo::ArrivalTrainValue, 14 },
				{ TrafficLogInfo::ArrivalChange, 15 },
				{ TrafficLogInfo::ArrivalWeight, 16 },
				{ TrafficLogInfo::ArrivalAdmitBlock, 17 },
				{ TrafficLogInfo::ArrivalReturn, 18 },
				{ TrafficLogInfo::ArrivalCancelBlock, 19 },
				{ TrafficLogInfo::ArrivalShunting, 20 },
				{ TrafficLogInfo::ArrivalShuntingEnd, 21 },
				{ TrafficLogInfo::DepartTrainNumS, 22 },
				{ TrafficLogInfo::DepartTrainNumX, 23 },
				{ TrafficLogInfo::DepartTrack, 24 },
				{ TrafficLogInfo::DepartSignalCode, 25 },
				{ TrafficLogInfo::AdjAgrDepartTime, 26 },
				{ TrafficLogInfo::ProvDepartTime, 27 },
				{ TrafficLogInfo::RealDepartTime, 28 },
				{ TrafficLogInfo::AdjArrivalTime, 29 },
				{ TrafficLogInfo::DepartHeavyVehicle, 30 },
				{ TrafficLogInfo::DepartEmpty, 31 },
				{ TrafficLogInfo::DepartUonuse, 32 },
				{ TrafficLogInfo::DepartCaboose, 33 },
				{ TrafficLogInfo::DepartSecVoucher, 34 },
				{ TrafficLogInfo::DepartLocomotive, 35 },
				{ TrafficLogInfo::DepartTrainValue, 36 },
				{ TrafficLogInfo::DepartChange, 37 },
				{ TrafficLogInfo::DepartWeight, 38 },
				{ TrafficLogInfo::DepartAdmitBlock, 39 },
				{ TrafficLogInfo::DepartReturn, 40 },
				{ TrafficLogInfo::DepartCancelBlock, 41 },
				{ TrafficLogInfo::DepartShunting, 42 },
				{ TrafficLogInfo::DepartShuntingEnd, 43 },
				{ TrafficLogInfo::ShuntingTime, 44 },
				{ TrafficLogInfo::DelayReason, 45 },
				{ TrafficLogInfo::PlanAttributes, 46 },
				{ TrafficLogInfo::Notes, 47 },
				{ TrafficLogInfo::LJStatus, 48 },
				{ TrafficLogInfo::JLStatus, 49 },
				{ TrafficLogInfo::JPStatus, 50 },
				{ TrafficLogInfo::LWStatus, 51 },
				{ TrafficLogInfo::JCStatus, 52 },
				{ TrafficLogInfo::HJStatus, 53 },
				{ TrafficLogInfo::CJStatus, 54 },
				{ TrafficLogInfo::SSStatus, 55 },
				{ TrafficLogInfo::ZGStatus, 56 },
				{ TrafficLogInfo::HCStatus, 57 },
				{ TrafficLogInfo::ZXStatus, 58 },
				{ TrafficLogInfo::XWStatus, 59 },
				{ TrafficLogInfo::DKStatus, 60 },
				{ TrafficLogInfo::CHStatus, 61 },
				{ TrafficLogInfo::ZWStatus, 62 },
				{ TrafficLogInfo::LimitOver, 64 },
				{ TrafficLogInfo::Army, 65 },
				{ TrafficLogInfo::Important, 66 }
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

		StationLogDispKSK::~StationLogDispKSK()
		{
		
		}

		void StationLogDispKSK::InitTrafficLogTableHead()
		{
			QVector<Control::TableHeadInfo> vecTableHeadInfo = {
				{ 0, 0, 22, 1, "到达"},
					{ 1, 0, 2, 2, "列车车次"},
						{ 3, 0, 1, 2, "下行"},
						{ 3, 1, 1, 2, "上行"},
					{ 1, 2, 1, 4, "接车口"},
					{ 1, 3, 1, 4, "接车股道"},
					{ 1, 4, 4, 1, "时分"},
						{ 2, 4, 1, 3, "同意邻站发车"},
						{ 2, 5, 1, 3, "邻站出发"},
						{ 2, 6, 2, 1, "本站到达"},
							{ 3, 6, 1, 2, "规定"},
							{ 3, 7, 1, 2, "实际"},
					{ 1, 8, 4, 1, "摘车辆数"},
						{ 2, 8, 2, 1, "运用车"},
							{ 3, 8, 1, 2, "重车"},
							{ 3, 9, 1, 2, "空车"},
						{ 2, 10, 1, 3, "非运用车"},
						{ 2, 11, 1, 3, "守车"},
					{ 1, 12, 1, 4, "占用区间凭证号码"},
					{ 1, 13, 1, 4, "机车型号"},
					{ 1, 14, 3, 1, "列车编组"},
						{ 2, 14, 1, 3, "车数"},
						{ 2, 15, 1, 3, "换长"},
						{ 2, 16, 1, 3, "总重"},
					{ 1, 17, 5, 1, "电话记录号码"},
						{ 2, 17, 1, 3, "承认闭塞"},
						{ 2, 18, 1, 3, "列车到达补机返回"},
						{ 2, 19, 1, 3, "取消闭塞"},
						{ 2, 20, 1, 3, "出站跟踪调车"},
						{ 2, 21, 1, 3, "出站跟踪调车完毕"},
				{ 0, 22, 22, 1, "出发"},
					{ 1, 22, 2, 2, "列车车次"},
						{ 3, 22, 1, 2, "下行"},
						{ 3, 23, 1, 2, "上行"},
					{ 1, 24, 1, 4, "发车股道"},
					{ 1, 25, 1, 4, "发车口"},
					{ 1, 26, 4, 1, "时分"},
						{ 2, 26, 1, 3, "邻站同意发车"},
						{ 2, 27, 2, 1, "本站出发"},
							{ 3, 27, 1, 2, "规定"},
							{ 3, 28, 1, 2, "实际"},
						{ 2, 29, 1, 3, "邻站到达"},
					{ 1, 30, 4, 1, "摘车辆数"},
						{ 2, 30, 2, 1, "运用车"},
							{ 3, 30, 1, 2, "重车"},
							{ 3, 31, 1, 2, "空车"},
						{ 2, 32, 1, 3, "非运用车"},
						{ 2, 33, 1, 3, "守车"},
					{ 1, 34, 1, 4, "占用区间凭证号码"},
					{ 1, 35, 1, 4, "机车型号"},
					{ 1, 36, 3, 1, "列车编组"},
						{ 2, 36, 1, 3, "车数"},
						{ 2, 37, 1, 3, "换长"},
						{ 2, 38, 1, 3, "总重"},
					{ 1, 39, 5, 1, "电话记录号码"},
						{ 2, 39, 1, 3, "承认闭塞"},
						{ 2, 40, 1, 3, "列车到达补机返回"},
						{ 2, 41, 1, 3, "取消闭塞"},
						{ 2, 42, 1, 3, "出站跟踪调车"},
						{ 2, 43, 1, 3, "出站跟踪调车完毕"},
				{ 0, 44, 1, 5, "本务机车调车时分"},
				{ 0, 45, 1, 5, "列车停站超过规定时间原因"},
				{ 0, 46, 1, 5, "计划属性"},
				{ 0, 47, 1, 5, "记事"},
				{ 0, 48, 16, 1, "任务"},
					{ 1, 48, 1, 4, "列检"},
					{ 1, 49, 1, 4, "交令"},
					{ 1, 50, 1, 4, "交票"},
					{ 1, 51, 1, 4, "列尾"},
					{ 1, 52, 1, 4, "机车"},
					{ 1, 53, 1, 4, "货检"},
					{ 1, 54, 1, 4, "乘降"},
					{ 1, 55, 1, 4, "上水"},
					{ 1, 56, 1, 4, "摘挂"},
					{ 1, 57, 1, 4, "换乘"},
					{ 1, 58, 1, 4, "装卸"},
					{ 1, 59, 1, 4, "吸污"},
					{ 1, 60, 1, 4, "道口"},
					{ 1, 61, 1, 4, "车号"},
					{ 1, 62, 1, 4, "站务"},
					{ 1, 63, 1, 4, "综控"},
				{ 0, 64, 1, 5, "超限"},
				{ 0, 65, 1, 5, "军运"},
				{ 0, 66, 1, 5, "重点"},	
			};
			pTrafficLogTable->TableHeadInit(5, 67, vecTableHeadInfo);
		}

		void StationLogDispKSK::AddTrafficLogTable()
		{
			ui.tableFrame->layout()->addWidget(pTrafficLogTable);
		}

		void StationLogDispKSK::OnTrafficLogTableUpData()
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
						<< (pTrafficLog->m_bUpDown ? "" : pTrafficLog->m_strArrivalTrainNum)
						<< (!pTrafficLog->m_bUpDown ? "" : pTrafficLog->m_strArrivalTrainNum)
						<< pTrafficLog->m_strArrivaSignal
						<< pTrafficLog->m_strArrivalTrack
						<< pTrafficLog->m_tAgrAdjDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tAdjDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tProvArrivalTime.toString("hh:mm")
						<< pTrafficLog->m_tRealArrivalTime.toString("hh:mm")
						<< "" << "" << "" << ""	//摘车辆数
						<< "" //占用区间凭证号码
						<< pTrain->m_strTrainType
						<< QString("%1").arg(pTrafficLog->m_nArrivalTrainValue)
						<< QString("%1").arg(pTrafficLog->m_nArrivalChange)
						<< QString("%1").arg(pTrafficLog->m_nArrivalWeight)
						<< "" << "" << "" << "" << "" //电话记录号码
						<< (pTrafficLog->m_bUpDown ? "" : pTrafficLog->m_strDepartTrainNum)
						<< (!pTrafficLog->m_bUpDown ? "" : pTrafficLog->m_strDepartTrainNum)
						<< pTrafficLog->m_strDepartTrack
						<< pTrafficLog->m_strDepartSignal
						<< pTrafficLog->m_tAdjAgrDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tProvDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tRealDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tAdjArrivalTime.toString("hh:mm")
						<< "" << "" << "" << ""	//摘车辆数
						<< "" //占用区间凭证号码
						<< pTrain->m_strTrainType
						<< QString("%1").arg(pTrafficLog->m_nDepartTrainValue)
						<< QString("%1").arg(pTrafficLog->m_nDepartChange)
						<< QString("%1").arg(pTrafficLog->m_nDepartWeight)
						<< "" << "" << "" << "" << "" //电话记录号码
						<< "" //本务机车调车时分
						<< pTrafficLog->m_strDelayReason
						<< pTrafficLog->m_strTrainArr
						<< pTrafficLog->m_strNotes
						<< (pTrafficLog->m_bLJStatus ? "列检" : "")
						<< (pTrafficLog->m_bJLStatus ? "交令" : "")
						<< (pTrafficLog->m_bJPStatus ? "交票" : "")
						<< (pTrafficLog->m_bLWStatus ? "列尾" : "")
						<< (pTrafficLog->m_bJCStatus ? "机车" : "")
						<< (pTrafficLog->m_bHJStatus ? "货检" : "")
						<< (pTrafficLog->m_bCJStatus ? "乘降" : "")
						<< (pTrafficLog->m_bSSStatus ? "上水" : "")
						<< (pTrafficLog->m_bZGStatus ? "摘挂" : "")
						<< (pTrafficLog->m_bHCStatus ? "换乘" : "")
						<< (pTrafficLog->m_bZXStatus ? "装卸" : "")
						<< (pTrafficLog->m_bXWStatus ? "吸污" : "")
						<< (pTrafficLog->m_bDKStatus ? "道口" : "")
						<< (pTrafficLog->m_bCHStatus ? "车号" : "")
						<< (pTrafficLog->m_bZWStatus ? "站务" : "")
						<< (pTrafficLog->m_bZKStatus ? "综控" : "")
						<< pTrain->OverLimitLevel()
						<< (pTrain->m_bArmy ? "军运" : "")
						<< (pTrain->m_bImportant ? "重点" : "");

					vecTableData.append(Control::TableRowDataInfo(strDataList, txtColor));
				}
			}
			pTrafficLogTable->ClearData();
			pTrafficLogTable->SetTableData(vecTableData);
		}

		void StationLogDispKSK::ShowTableHead(bool bShow)
		{
			ui.tableHeadFrame->setHidden(!bShow);	
		}
	}
}
