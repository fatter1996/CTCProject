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
						<< ""
						<< "" << "" << ""
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
						<< ""
						<< "" << "" << ""
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

		void StationLogDispKSK::ClickMenu(QPoint pos, Station::StaTrafficLog* m_pCurTrafficLog)
		{
			QMenu* pMenu = new QMenu();
			QAction* pAction = new QAction("�ϱ������");
			QAction* pAction_2 = new QAction("�ϱ�������");
			QAction* pAction_3 = new QAction("�ϱ�ͨ����");
			QAction* pAction_4 = new QAction("Ϊʼ����");
			QAction* pAction_5 = new QAction("Ϊ�յ���");
			QAction* pAction_6 = new QAction("�ϱ��ٱ���Ϣ");
			QAction* pAction_7 = new QAction("�޸ĳ��κ�");
			QAction* pAction_8 = new QAction("�޸������վ");
			QAction* pAction_9 = new QAction("ɾ��");
			QAction* pAction_10 = new QAction("ɾ����˸");
			QAction* pAction_11 = new QAction("ȫ����Ϣ");
			QAction* pAction_12 = new QAction("�޸��г��ƻ�");
			QAction* pAction_13 = new QAction("����/ȡ���ص��г�");
			QAction* pAction_14 = new QAction("����ɾ�����");
			QAction* pAction_15 = new QAction("����ɵ��������·��һ��");
			QAction* pAction_16 = new QAction("���������������·��һ��");
			pMenu->addAction(pAction);
			connect(pAction, &QAction::triggered, [=] (){//�ϱ������
				TrainArrival();
				});
			pMenu->addAction(pAction_2);
			connect(pAction_2, &QAction::triggered, [=]() {//�ϱ�������
				TrainDeparture();
				});
			pMenu->addAction(pAction_3);
			connect(pAction_3, &QAction::triggered, [=]() {//�ϱ�ͨ����
				TrainPassThrough();
				});
			pMenu->addAction(pAction_4);
			connect(pAction_4, &QAction::triggered, [=]() {//Ϊʼ����
				});
			pMenu->addAction(pAction_5);
			connect(pAction_5, &QAction::triggered, [=]() {//Ϊ�յ���
				});
			pMenu->addSeparator();
			pMenu->addAction(pAction_6);
			connect(pAction_6, &QAction::triggered, [=]() {//�ϱ��ٱ���Ϣ
				if (m_pCurTrafficLog) {
					QuickReportKSK* Report = new QuickReportKSK(m_pCurTrafficLog);
					Report->show();
				}
				});
			pMenu->addAction(pAction_7);
			connect(pAction_7, &QAction::triggered, [=]() {//�޸ĳ��κ�
				if (m_pCurTrafficLog) {
					ModifyTrainNumberKSK* pTrainNumber = new ModifyTrainNumberKSK(m_pCurTrafficLog);
					pTrainNumber->show();
				}
				});
			pMenu->addAction(pAction_8);
			connect(pAction_8, &QAction::triggered, [=]() {//�޸������վ
				if (m_pCurTrafficLog) {
					ModifyRelevantStationsKSK* m_pRelevantStations = new ModifyRelevantStationsKSK(m_pCurTrafficLog);
					m_pRelevantStations->show();
				}
				});
			pMenu->addAction(pAction_9);
			connect(pAction_9, &QAction::triggered, [=]() {//ɾ��
			
				if (m_pCurTrafficLog) {
					QMessageBox msgBox(QMessageBox::Warning,
						tr("CTC"),
						tr("�Ƿ�ȷ��ɾ����"),
						QMessageBox::Ok | QMessageBox::Cancel,
						this);

					// �Զ��尴ť�ı�
					msgBox.setButtonText(QMessageBox::Ok, tr("ȷ��"));
					msgBox.setButtonText(QMessageBox::Cancel, tr("ȡ��"));

					// ��ʾ�Ի��򲢻�ȡ���
					int ret = msgBox.exec();

					if (ret == QMessageBox::Ok) {
						qDebug() << "�û������ȷ��";

						QVector<Station::StaTrainRoute*> vecTrainRoute;
						bool bAllClear = true;
						QByteArray btResult;
						vecTrainRoute = Station::MainStation()->getStaTrainRouteByTrain(m_pCurTrafficLog->m_nTrainId);
						if (Http::HttpClient::DeleteStaTrafficLog(m_pCurTrafficLog->m_nLogId, btResult)) {
							qDebug() << "��ɾ���г���־-->" << m_pCurTrafficLog->m_nLogId;
						}
						for (Station::StaTrainRoute* pTrainRoute : vecTrainRoute) {
							if (Http::HttpClient::DeleteStaTrainRoute(pTrainRoute->m_nRouteId, btResult)) {
								qDebug() << "��ɾ����·����-->" << pTrainRoute->m_nRouteId;
							}
						}
						if (Http::HttpClient::DeleteStaTrain(m_pCurTrafficLog->m_nTrainId, btResult)) {
							qDebug() << "��ɾ��������Ϣ-->" << m_pCurTrafficLog->m_nTrainId;
						}
						Station::MainStation()->RemoveLogVecOne(m_pCurTrafficLog);
					}
				}
				});
			pMenu->addSeparator();
			pMenu->addAction(pAction_10);
			connect(pAction_10, &QAction::triggered, [=]() {//ɾ����˸

				});
			pMenu->addAction(pAction_11);
			connect(pAction_11, &QAction::triggered, [=]() {//ȫ����Ϣ

				});
			pMenu->addSeparator();
			pMenu->addAction(pAction_12);
			connect(pAction_12, &QAction::triggered, [=]() {//�޸��г��ƻ�
				//if (m_pCurTrafficLog) {
					StaAddNewTrainKSK* pAddNewTrain = new StaAddNewTrainKSK;
					pAddNewTrain->InitAddView(m_pCurTrafficLog);
					pAddNewTrain->show();
				//}
				});
			pMenu->addAction(pAction_13);
			connect(pAction_13, &QAction::triggered, [=]() {//����/ȡ���ص��г�
				if (m_pCurTrafficLog) {
					Station::StaTrain* pTrain = Station::MainStation()->getStaTrainById(m_pCurTrafficLog->m_nTrainId);
					QString strImportant;
					if (pTrain->m_bImportant) {
						strImportant = "��ǰΪ�ص��г��Ƿ�ȡ���ص��г�";
					}
					else {
						strImportant = "��ǰΪ���ص��г��Ƿ������ص��г�";
					}
					QMessageBox msgBox(QMessageBox::Warning,
						tr("CTC"),
						tr("�Ƿ�ȷ��ɾ����"),
						QMessageBox::Ok | QMessageBox::Cancel,
						this);

					msgBox.setButtonText(QMessageBox::Ok, tr("ȷ��"));
					msgBox.setButtonText(QMessageBox::Cancel, tr("ȡ��"));

					int ret = msgBox.exec();

					if (ret == QMessageBox::Ok) {
						pTrain->m_bImportant = !pTrain->m_bImportant;
					}
				}
				});
			pMenu->addAction(pAction_14);
			connect(pAction_14, &QAction::triggered, [=]() {//����ɾ�����

				});
			pMenu->addAction(pAction_15);
			connect(pAction_15, &QAction::triggered, [=]() {//����ɵ��������·��һ��

				});
			pMenu->addAction(pAction_16);
			connect(pAction_16, &QAction::triggered, [=]() {//���������������·��һ��

				});
			pMenu->exec(pos);
		}

		
		void StationLogDispKSK::ShowTableHead(bool bShow)
		{
			ui.tableHeadFrame->setHidden(!bShow);	
		}
	}
}
