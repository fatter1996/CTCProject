#include "StationLogDispKSK.h"
#include "./CTCMainWindow/StationViewKSK/ModuleWidget/QuickReportKSK.h"
#include "./CTCMainWindow/StationViewKSK/ModuleWidget/ModifyRelevantStationsKSK.h"
#include "./CTCMainWindow/StationViewKSK/ModuleWidget/ModifyTrainNumberKSK.h"
#include "./CTCMainWindow/StationViewKSK/ModuleWidget/StaAddNewTrainKSK.h"
#include "Global.h"
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QMessageBox>
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
			m_pTrafficMsgLog = ui.widget_2;
			connect(ui.adjPreviewBtn, &QPushButton::clicked, [=]() {
				AdjacentStationPreview();
			});
			connect(ui.agreeAdjBtn, &QPushButton::clicked, [=]() {
				AgreeAdjacentStations();
			});

			connect(ui.arrivalBtn, &QPushButton::clicked, [=]() {
				TrainArrival();
			});
			connect(ui.departureBtn, &QPushButton::clicked, [=]() {
				TrainDeparture();
			});
			connect(ui.throughBtn, &QPushButton::clicked, [=]() {
				TrainPassThrough();
			});
			connect(ui.adjAgrBtn, &QPushButton::clicked, [=]() {
				AdjacentStation();
			});
			
			//connect(ui.supPointBtn, &QPushButton::clicked, [=]() {AdjacentStation();
		//});
			connect(ui.cancelBlockBtn, &QPushButton::clicked, [=]() {
				CancelBlock();
			});
			connect(ui.cancelDepartBtn, &QPushButton::clicked, [=]() {
				CancelDepart();
			});
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
						<< ""
						<< "" << "" << ""
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
						<< ""
						<< "" << "" << ""
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

		void StationLogDispKSK::ClickMenu(QPoint pos, Station::StaTrafficLog* m_pCurTrafficLog)
		{
			QMenu* pMenu = new QMenu();
			QAction* pAction = new QAction("上报到达点");
			QAction* pAction_2 = new QAction("上报出发点");
			QAction* pAction_3 = new QAction("上报通过点");
			QAction* pAction_4 = new QAction("为始发车");
			QAction* pAction_5 = new QAction("为终到车");
			QAction* pAction_6 = new QAction("上报速报信息");
			QAction* pAction_7 = new QAction("修改车次号");
			QAction* pAction_8 = new QAction("修改相关邻站");
			QAction* pAction_9 = new QAction("删除");
			QAction* pAction_10 = new QAction("删除闪烁");
			QAction* pAction_11 = new QAction("全体信息");
			QAction* pAction_12 = new QAction("修改列车计划");
			QAction* pAction_13 = new QAction("设置/取消重点列车");
			QAction* pAction_14 = new QAction("设置删除标记");
			QAction* pAction_15 = new QAction("允许股道与基本径路不一致");
			QAction* pAction_16 = new QAction("允许出入口与基本径路不一致");
			pMenu->addAction(pAction);
			connect(pAction, &QAction::triggered, [=] (){//上报到达点
				TrainArrival();
				});
			pMenu->addAction(pAction_2);
			connect(pAction_2, &QAction::triggered, [=]() {//上报出发点
				TrainDeparture();
				});
			pMenu->addAction(pAction_3);
			connect(pAction_3, &QAction::triggered, [=]() {//上报通过点
				TrainPassThrough();
				});
			pMenu->addAction(pAction_4);
			connect(pAction_4, &QAction::triggered, [=]() {//为始发车
				});
			pMenu->addAction(pAction_5);
			connect(pAction_5, &QAction::triggered, [=]() {//为终到车
				});
			pMenu->addSeparator();
			pMenu->addAction(pAction_6);
			connect(pAction_6, &QAction::triggered, [=]() {//上报速报信息
				if (m_pCurTrafficLog) {
					QuickReportKSK* Report = new QuickReportKSK(m_pCurTrafficLog);
					Report->show();
				}
				});
			pMenu->addAction(pAction_7);
			connect(pAction_7, &QAction::triggered, [=]() {//修改车次号
				if (m_pCurTrafficLog) {
					ModifyTrainNumberKSK* pTrainNumber = new ModifyTrainNumberKSK(m_pCurTrafficLog);
					pTrainNumber->show();
				}
				});
			pMenu->addAction(pAction_8);
			connect(pAction_8, &QAction::triggered, [=]() {//修改相关邻站
				if (m_pCurTrafficLog) {
					ModifyRelevantStationsKSK* m_pRelevantStations = new ModifyRelevantStationsKSK(m_pCurTrafficLog);
					m_pRelevantStations->show();
				}
				});
			pMenu->addAction(pAction_9);
			connect(pAction_9, &QAction::triggered, [=]() {//删除
			
				if (m_pCurTrafficLog) {
					QMessageBox msgBox(QMessageBox::Warning,
						tr("CTC"),
						tr("是否确定删除！"),
						QMessageBox::Ok | QMessageBox::Cancel,
						this);

					// 自定义按钮文本
					msgBox.setButtonText(QMessageBox::Ok, tr("确定"));
					msgBox.setButtonText(QMessageBox::Cancel, tr("取消"));

					// 显示对话框并获取结果
					int ret = msgBox.exec();

					if (ret == QMessageBox::Ok) {
						qDebug() << "用户点击了确定";

						QVector<Station::StaTrainRoute*> vecTrainRoute;
						bool bAllClear = true;
						QByteArray btResult;
						vecTrainRoute = Station::MainStation()->getStaTrainRouteByTrain(m_pCurTrafficLog->m_nTrainId);
						if (Http::HttpClient::DeleteStaTrafficLog(m_pCurTrafficLog->m_nLogId, btResult)) {
							qDebug() << "已删除行车日志-->" << m_pCurTrafficLog->m_nLogId;
						}
						for (Station::StaTrainRoute* pTrainRoute : vecTrainRoute) {
							if (Http::HttpClient::DeleteStaTrainRoute(pTrainRoute->m_nRouteId, btResult)) {
								qDebug() << "已删除进路序列-->" << pTrainRoute->m_nRouteId;
							}
						}
						if (Http::HttpClient::DeleteStaTrain(m_pCurTrafficLog->m_nTrainId, btResult)) {
							qDebug() << "已删除车次信息-->" << m_pCurTrafficLog->m_nTrainId;
						}
						Station::MainStation()->RemoveLogVecOne(m_pCurTrafficLog);
					}
				}
				});
			pMenu->addSeparator();
			pMenu->addAction(pAction_10);
			connect(pAction_10, &QAction::triggered, [=]() {//删除闪烁

				});
			pMenu->addAction(pAction_11);
			connect(pAction_11, &QAction::triggered, [=]() {//全体信息

				});
			pMenu->addSeparator();
			pMenu->addAction(pAction_12);
			connect(pAction_12, &QAction::triggered, [=]() {//修改列车计划
				//if (m_pCurTrafficLog) {
					StaAddNewTrainKSK* pAddNewTrain = new StaAddNewTrainKSK;
					pAddNewTrain->InitAddView(m_pCurTrafficLog);
					pAddNewTrain->show();
				//}
				});
			pMenu->addAction(pAction_13);
			connect(pAction_13, &QAction::triggered, [=]() {//设置/取消重点列车
				if (m_pCurTrafficLog) {
					Station::StaTrain* pTrain = Station::MainStation()->getStaTrainById(m_pCurTrafficLog->m_nTrainId);
					QString strImportant;
					if (pTrain->m_bImportant) {
						strImportant = "当前为重点列车是否取消重点列车";
					}
					else {
						strImportant = "当前为非重点列车是否设置重点列车";
					}
					QMessageBox msgBox(QMessageBox::Warning,
						tr("CTC"),
						tr("是否确定删除！"),
						QMessageBox::Ok | QMessageBox::Cancel,
						this);

					msgBox.setButtonText(QMessageBox::Ok, tr("确定"));
					msgBox.setButtonText(QMessageBox::Cancel, tr("取消"));

					int ret = msgBox.exec();

					if (ret == QMessageBox::Ok) {
						pTrain->m_bImportant = !pTrain->m_bImportant;
					}
				}
				});
			pMenu->addAction(pAction_14);
			connect(pAction_14, &QAction::triggered, [=]() {//设置删除标记

				});
			pMenu->addAction(pAction_15);
			connect(pAction_15, &QAction::triggered, [=]() {//允许股道与基本径路不一致

				});
			pMenu->addAction(pAction_16);
			connect(pAction_16, &QAction::triggered, [=]() {//允许出入口与基本径路不一致

				});
			pMenu->exec(pos);
		}

		
		void StationLogDispKSK::ShowTableHead(bool bShow)
		{
			ui.tableHeadFrame->setHidden(!bShow);	
		}
	}
}
