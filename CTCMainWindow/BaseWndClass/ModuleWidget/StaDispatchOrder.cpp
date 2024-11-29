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
				pOrder->m_nOrderId = m_pCurDispatch->m_nOrderId;	//������������ID
				pOrder->m_tRecv = QDateTime::currentDateTime();//��������ʱ��
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
					qDebug() << "�޸Ļ�����Ϣ�ɹ�";
				}
				TrainDispatchOrderListUpData();
			}
		}

		void StaDispatchOrder::DeleteNewTrainDispatch()
		{
			if (m_pCurTrainDispatch) {
				QByteArray btResult;
				if (Http::HttpClient::DeleteStaTrainDispatch(m_pCurTrainDispatch, btResult)) {
					qDebug() << "ɾ��������Ϣ�ɹ�";
				}
				m_pCurTrainDispatch = nullptr;
				TrainDispatchOrderListUpData();
				CurTrainDispatchOrderUpData();
			}
		}
	}
}