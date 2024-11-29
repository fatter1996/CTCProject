#include "StaDispatchOrder.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace BaseWnd {
		StaDispatchOrder::StaDispatchOrder(QWidget* parent)
		{

		}

		StaDispatchOrder::~StaDispatchOrder()
		{

		}

		void StaDispatchOrder::SignForDispatchOrder()
		{
			if (m_pCurDispatch) {
				QByteArray btResult;
				if (Http::HttpClient::SignForDispatchOrder(m_pCurDispatch->m_nOrderId, btResult)) {
					m_pCurDispatch->m_nStateDisOrder = 1;
				}
			}
			CurDispatchOrderUpData();
		}

		void StaDispatchOrder::AddNewTrainDispatch()
		{
			Station::StaTrainDispatch* pOrder = new Station::StaTrainDispatch;
			if (m_pCurDispatch) {
				pOrder->m_nOrderId = m_pCurDispatch->m_nOrderId;	//关联调度命令ID
				pOrder->m_tRecv = QDateTime::currentDateTime();//机车接收时间
				GetTrainInfo(pOrder);
				QByteArray btResult;
				int nTrainId = 1;
				while (btResult != "") {
					if (Http::HttpClient::SelectStaTrain(nTrainId, btResult)) {
						nTrainId++;
					}
				}
				pOrder->m_nTrainId = nTrainId;
				if (Http::HttpClient::AddStaTrainDispatch(pOrder, btResult)) {
					m_pCurDispatch->m_vecTrainDispatch.append(pOrder);
				}
				TrainDispatchOrderListUpData();
			}
		}

		void StaDispatchOrder::ModifyNewTrainDispatch()
		{
			if (m_pCurTrainDispatch) {
				GetTrainInfo(m_pCurTrainDispatch);
				QByteArray btResult;
				if (Http::HttpClient::ModifyStaTrainDispatch(m_pCurTrainDispatch, btResult)) {
					qDebug() << "修改机车信息成功";
				}
				TrainDispatchOrderListUpData();
			}
		}

		void StaDispatchOrder::DeleteNewTrainDispatch()
		{
			if (m_pCurTrainDispatch) {
				QByteArray btResult;
				if (Http::HttpClient::DeleteStaTrainDispatch(m_pCurTrainDispatch, btResult)) {
					qDebug() << "删除机车信息成功";
				}
				m_pCurTrainDispatch = nullptr;
				TrainDispatchOrderListUpData();
				CurTrainDispatchOrderUpData();
			}
		}
	}
}