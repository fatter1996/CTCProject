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
				{ 0, 0, 22, 1, "����"},
					{ 1, 0, 2, 2, "�г�����"},
						{ 3, 0, 1, 2, "����"},
						{ 3, 1, 1, 2, "����"},
					{ 1, 2, 1, 4, "�ӳ���"},
					{ 1, 3, 1, 4, "�ӳ��ɵ�"},
					{ 1, 4, 4, 1, "ʱ��"},
						{ 2, 4, 1, 3, "ͬ����վ����"},
						{ 2, 5, 1, 3, "��վ����"},
						{ 2, 6, 2, 1, "��վ����"},
							{ 3, 6, 1, 2, "�涨"},
							{ 3, 7, 1, 2, "ʵ��"},
					{ 1, 8, 4, 1, "ժ������"},
						{ 2, 8, 2, 1, "���ó�"},
							{ 3, 8, 1, 2, "�س�"},
							{ 3, 9, 1, 2, "�ճ�"},
						{ 2, 10, 1, 3, "�����ó�"},
						{ 2, 11, 1, 3, "�س�"},
					{ 1, 12, 1, 4, "ռ������ƾ֤����"},
					{ 1, 13, 1, 4, "�����ͺ�"},
					{ 1, 14, 3, 1, "�г�����"},
						{ 2, 14, 1, 3, "����"},
						{ 2, 15, 1, 3, "����"},
						{ 2, 16, 1, 3, "����"},
					{ 1, 17, 5, 1, "�绰��¼����"},
						{ 2, 17, 1, 3, "���ϱ���"},
						{ 2, 18, 1, 3, "�г����ﲹ������"},
						{ 2, 19, 1, 3, "ȡ������"},
						{ 2, 20, 1, 3, "��վ���ٵ���"},
						{ 2, 21, 1, 3, "��վ���ٵ������"},
				{ 0, 22, 22, 1, "����"},
					{ 1, 22, 2, 2, "�г�����"},
						{ 3, 22, 1, 2, "����"},
						{ 3, 23, 1, 2, "����"},
					{ 1, 24, 1, 4, "�����ɵ�"},
					{ 1, 25, 1, 4, "������"},
					{ 1, 26, 4, 1, "ʱ��"},
						{ 2, 26, 1, 3, "��վͬ�ⷢ��"},
						{ 2, 27, 2, 1, "��վ����"},
							{ 3, 27, 1, 2, "�涨"},
							{ 3, 28, 1, 2, "ʵ��"},
						{ 2, 29, 1, 3, "��վ����"},
					{ 1, 30, 4, 1, "ժ������"},
						{ 2, 30, 2, 1, "���ó�"},
							{ 3, 30, 1, 2, "�س�"},
							{ 3, 31, 1, 2, "�ճ�"},
						{ 2, 32, 1, 3, "�����ó�"},
						{ 2, 33, 1, 3, "�س�"},
					{ 1, 34, 1, 4, "ռ������ƾ֤����"},
					{ 1, 35, 1, 4, "�����ͺ�"},
					{ 1, 36, 3, 1, "�г�����"},
						{ 2, 36, 1, 3, "����"},
						{ 2, 37, 1, 3, "����"},
						{ 2, 38, 1, 3, "����"},
					{ 1, 39, 5, 1, "�绰��¼����"},
						{ 2, 39, 1, 3, "���ϱ���"},
						{ 2, 40, 1, 3, "�г����ﲹ������"},
						{ 2, 41, 1, 3, "ȡ������"},
						{ 2, 42, 1, 3, "��վ���ٵ���"},
						{ 2, 43, 1, 3, "��վ���ٵ������"},
				{ 0, 44, 1, 5, "�����������ʱ��"},
				{ 0, 45, 1, 5, "�г�ͣվ�����涨ʱ��ԭ��"},
				{ 0, 46, 1, 5, "�ƻ�����"},
				{ 0, 47, 1, 5, "����"},
				{ 0, 48, 16, 1, "����"},
					{ 1, 48, 1, 4, "�м�"},
					{ 1, 49, 1, 4, "����"},
					{ 1, 50, 1, 4, "��Ʊ"},
					{ 1, 51, 1, 4, "��β"},
					{ 1, 52, 1, 4, "����"},
					{ 1, 53, 1, 4, "����"},
					{ 1, 54, 1, 4, "�˽�"},
					{ 1, 55, 1, 4, "��ˮ"},
					{ 1, 56, 1, 4, "ժ��"},
					{ 1, 57, 1, 4, "����"},
					{ 1, 58, 1, 4, "װж"},
					{ 1, 59, 1, 4, "����"},
					{ 1, 60, 1, 4, "����"},
					{ 1, 61, 1, 4, "����"},
					{ 1, 62, 1, 4, "վ��"},
					{ 1, 63, 1, 4, "�ۿ�"},
				{ 0, 64, 1, 5, "����"},
				{ 0, 65, 1, 5, "����"},
				{ 0, 66, 1, 5, "�ص�"},	
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
						<< "" << "" << "" << ""	//ժ������
						<< "" //ռ������ƾ֤����
						<< pTrain->m_strTrainType
						<< QString("%1").arg(pTrafficLog->m_nArrivalTrainValue)
						<< QString("%1").arg(pTrafficLog->m_nArrivalChange)
						<< QString("%1").arg(pTrafficLog->m_nArrivalWeight)
						<< "" << "" << "" << "" << "" //�绰��¼����
						<< (pTrafficLog->m_bUpDown ? "" : pTrafficLog->m_strDepartTrainNum)
						<< (!pTrafficLog->m_bUpDown ? "" : pTrafficLog->m_strDepartTrainNum)
						<< pTrafficLog->m_strDepartTrack
						<< pTrafficLog->m_strDepartSignal
						<< pTrafficLog->m_tAdjAgrDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tProvDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tRealDepartTime.toString("hh:mm")
						<< pTrafficLog->m_tAdjArrivalTime.toString("hh:mm")
						<< "" << "" << "" << ""	//ժ������
						<< "" //ռ������ƾ֤����
						<< pTrain->m_strTrainType
						<< QString("%1").arg(pTrafficLog->m_nDepartTrainValue)
						<< QString("%1").arg(pTrafficLog->m_nDepartChange)
						<< QString("%1").arg(pTrafficLog->m_nDepartWeight)
						<< "" << "" << "" << "" << "" //�绰��¼����
						<< "" //�����������ʱ��
						<< pTrafficLog->m_strDelayReason
						<< pTrafficLog->m_strTrainArr
						<< pTrafficLog->m_strNotes
						<< (pTrafficLog->m_bLJStatus ? "�м�" : "")
						<< (pTrafficLog->m_bJLStatus ? "����" : "")
						<< (pTrafficLog->m_bJPStatus ? "��Ʊ" : "")
						<< (pTrafficLog->m_bLWStatus ? "��β" : "")
						<< (pTrafficLog->m_bJCStatus ? "����" : "")
						<< (pTrafficLog->m_bHJStatus ? "����" : "")
						<< (pTrafficLog->m_bCJStatus ? "�˽�" : "")
						<< (pTrafficLog->m_bSSStatus ? "��ˮ" : "")
						<< (pTrafficLog->m_bZGStatus ? "ժ��" : "")
						<< (pTrafficLog->m_bHCStatus ? "����" : "")
						<< (pTrafficLog->m_bZXStatus ? "װж" : "")
						<< (pTrafficLog->m_bXWStatus ? "����" : "")
						<< (pTrafficLog->m_bDKStatus ? "����" : "")
						<< (pTrafficLog->m_bCHStatus ? "����" : "")
						<< (pTrafficLog->m_bZWStatus ? "վ��" : "")
						<< (pTrafficLog->m_bZKStatus ? "�ۿ�" : "")
						<< pTrain->OverLimitLevel()
						<< (pTrain->m_bArmy ? "����" : "")
						<< (pTrain->m_bImportant ? "�ص�" : "");

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
