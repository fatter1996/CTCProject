#include "HttpClient.h"
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Http {
	HttpClient* HttpClient::m_pHttpClient = nullptr;

	HttpClient::HttpClient()
	{
		if (!m_pHttpClient) {
			m_pHttpClient = this;
		}
		m_pManager = new QNetworkAccessManager;
	}

	HttpClient::~HttpClient()
	{
		delete m_pManager;
		m_pManager = nullptr;
	}

	void HttpClient::setServerAddress(const QHostAddress& hAddress, const quint16& nPort)
	{
		m_hServerIp = hAddress;
		m_nServerPort = nPort;
	}

	bool HttpClient::PostRequest(QString strUrl, QString strContent, QByteArray& btResult)
	{
		QString strCompleteUrl = QString("http://%1:%2%3").arg(m_hServerIp.toString())
			.arg(m_nServerPort).arg(strUrl);
		QNetworkRequest postRequest(strCompleteUrl);
		postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
		postRequest.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);

		QNetworkReply* postReply = m_pManager->post(postRequest, strContent.toUtf8());

		// 创建一个局部的事件循环，等待请求完成
		QEventLoop loop;
		QTimer timer;
		timer.setSingleShot(true); // 只触发一次
		timer.start(2000); // 2秒后超时
		QObject::connect(postReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
		QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
		loop.exec();

		//if (timer.isActive()) {	//超时处理
		//	qDebug() << "Timeout!";
		//	timer.stop();
		//	return false;
		//}

		if (postReply->error() == QNetworkReply::NoError) {
			btResult = postReply->readAll();
			qDebug() << QString::fromUtf8(btResult);
			postReply->deleteLater(); //删除reply对象
			return true;
		}
		else {
			// 处理错误...
			postReply->deleteLater(); //删除reply对象
			return false;
		}
	}

	bool HttpClient::GetRequest(QString strUrl, QByteArray& btResult)
	{
		QNetworkRequest getRequest(strUrl);
		QNetworkReply* getReply = m_pManager->get(getRequest);

		// 创建一个局部的事件循环，等待请求完成
		QEventLoop loop;
		QTimer timer;
		timer.setSingleShot(true); // 只触发一次
		timer.start(5000); // 5秒后超时
		QObject::connect(getReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
		QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
		loop.exec();

		if (timer.isActive()) {	//超时处理
			qDebug() << "Timeout!";
			timer.stop();
			return false;
		}

		if (getReply->error() == QNetworkReply::NoError) {
			btResult = getReply->readAll();
			getReply->deleteLater(); //删除reply对象
			return true;
		}
		else {
			// 处理错误...
			getReply->deleteLater(); //删除reply对象
			return false;
		}
	}

	bool  HttpClient::AddStaStagePlan(Station::StaStagePlan* pStagePlan, QByteArray& btResult)
	{
		QString m_strArrivaTrainNum; //到达车次号
		QString m_strDepartTrainNum; //出发车次号
		QString strContent = QString("stationId=%1&planNum=%2&planType=%3"
			"&arriveTrainNum=%4&arriveTrackCode=%5&entrySignalCode=%6&arriveTime=%7"
			"&departTrainNum=%8&departTrackCode=%9&exitSignalCode=%10&departTime=%11")
			.arg(Station::MainStation()->getStationId())
			.arg(pStagePlan->m_strPlanNum)
			.arg(pStagePlan->m_nPlanType)
			.arg(pStagePlan->m_strArrivalTrainNum)
			.arg(pStagePlan->m_nArrivalTrackCode)
			.arg(pStagePlan->m_nEntrySignalCode)
			.arg(pStagePlan->m_tArrivalTime.toString(Qt::ISODate))
			.arg(pStagePlan->m_strDepartTrainNum)
			.arg(pStagePlan->m_nDepartTrackCode)
			.arg(pStagePlan->m_nExitSignalCode)
			.arg(pStagePlan->m_tDepartTime.toString(Qt::ISODate));

		return m_pHttpClient->PostRequest("/stagePlan/saveStagePlan", strContent, btResult);
	}

	bool HttpClient::SelectStaStagePlan(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return m_pHttpClient->PostRequest("/stagePlan/selectStagePlanList", strContent, btResult);
	}

	bool HttpClient::AddStaTrain(Station::StaTrain* pTrain, QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1&arriveTrainNum=%2&departTrainNum=%3"
			"&direction=%4&trainLength=%5&speed=%6&electric=%7&overLimitLevel=%8&freightTrain=%9"
			"&posCode=%10&trainTypeNumber=%11&operationTypeNumber=%12&isRun=%13")
			.arg(Station::MainStation()->getStationId())
			.arg(pTrain->m_strArrivalTrainNum)
			.arg(pTrain->m_strDepartTrainNum)
			.arg(pTrain->m_bRight)
			.arg(pTrain->m_nTrainLengh) 
			.arg(pTrain->m_nSpeed)
			.arg(pTrain->m_bElectric)
			.arg(pTrain->m_nOverLimitLevel)
			.arg(pTrain->m_bFreightTrain)
			.arg(pTrain->m_nPosCode)
			.arg(pTrain->m_nTrainType)
			.arg(pTrain->m_nOperationType)
			.arg(pTrain->m_bRunning);

		return m_pHttpClient->PostRequest("/trainProperty/saveTrainProperty", strContent, btResult);
	}

	bool HttpClient::SelectStaTrain(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return m_pHttpClient->PostRequest("/trainProperty/selectTrainPropertyList", strContent, btResult);
	}

	bool HttpClient::AddStaTrainRoute(Station::StaTrainRoute* pTrainRoute, QByteArray& btResult)
	{
		QString strSubRouteId;
		for (int id : pTrainRoute->m_vecSubRouteId) {
			strSubRouteId.append(id); 
			strSubRouteId.append(",");
		}
		QString strContent = QString("stationId=%1&trainId=%2&arriveRoute=%3&autoTouch=%4"
			"&planTime=%5&triggerTime=%6&cleanTime=%7&trackCode=%8&signalCode=%9"
			"&routeDepict=%10&direction=%11&routeState=%12&subRouteId=%13")
			.arg(Station::MainStation()->getStationId())
			.arg(pTrainRoute->m_nTrainId)
			//.arg(pTrainRoute->m_nPlanType)
			.arg(pTrainRoute->m_bArrivaRoute)
			.arg(pTrainRoute->m_nAutoTouch)
			.arg(pTrainRoute->m_tPlanTime.toString(Qt::ISODate))
			.arg(pTrainRoute->m_tTriggerTime.toString(Qt::ISODate))
			.arg(pTrainRoute->m_tCleanTime.toString(Qt::ISODate))
			.arg(pTrainRoute->m_nTrackCode)
			.arg(pTrainRoute->m_nSignalCode)
			.arg(pTrainRoute->m_strRouteDescrip)
			.arg(pTrainRoute->m_strDirection)
			.arg(pTrainRoute->m_nRouteState)
			.arg(strSubRouteId.left(strSubRouteId.length() - 1));

		return m_pHttpClient->PostRequest("/trainRouteSequence/saveTrainRouteSequence", strContent, btResult);
	}

	bool HttpClient::SelectStaTrainRoute(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return m_pHttpClient->PostRequest("/trainRouteSequence/selectTrainRouteSequenceList", strContent, btResult);
	}

	bool HttpClient::AddStaDispatchOrder(Station::StaDispatchOrder* pDispatchOrder, QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1&orderType=%2&sendState=%3&strOrderNum=%4"
			"&strType=%5&strOrderTip=%6&strContent=%7&strSendAgency=%8&strSendStarter=%9"
			"&strOrderUnit=%10&strSignName=%11&strSignTime=%12&stress=%13&stateDisOrder=%14")
			.arg(Station::MainStation()->getStationId())
			.arg(pDispatchOrder->m_nOrderType)
			.arg(pDispatchOrder->m_nSendState)
			.arg(pDispatchOrder->m_strOrderNum)
			.arg(pDispatchOrder->m_strType)
			.arg(pDispatchOrder->m_strOrderTip)
			.arg(pDispatchOrder->m_strContent)
			.arg(pDispatchOrder->m_strSendAgency)
			.arg(pDispatchOrder->m_tSendTime.toString(Qt::ISODate))
			.arg(pDispatchOrder->m_strOrderedUnit)
			.arg(pDispatchOrder->m_strSignName)
			.arg(pDispatchOrder->m_tSignTime.toString(Qt::ISODate))
			.arg(pDispatchOrder->m_bImportant)
			.arg(pDispatchOrder->m_nStateDisOrder);
		return m_pHttpClient->PostRequest("/dispatchingOrder/saveDisPatchingOrderData", strContent, btResult);
	}

	bool HttpClient::SelectStaDispatchOrder(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return m_pHttpClient->PostRequest("/dispatchingOrder/selectDispatchingOrderList", strContent, btResult);
	}

	bool HttpClient::SignForDispatchOrder(int nOrderId, QByteArray& btResult)
	{
		QString strContent = QString("orderId=%1&stateDisOrder=1").arg(nOrderId);
		return m_pHttpClient->PostRequest("/dispatchingOrder/updateDispatchingOrder", strContent, btResult);
	}

	bool HttpClient::AddStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1&orderId=%2&strTrainNum=%3"
			"&strLocomotiveNumber=%4&receiveTime=%5&state=%6")
			.arg(Station::MainStation()->getStationId())
			.arg(pTrainDispatch->m_nOrderId)
			.arg(pTrainDispatch->m_strTrainNum)
			.arg(pTrainDispatch->m_strLocomotive)
			.arg(pTrainDispatch->m_timRecv.toString(Qt::ISODate))
			.arg(pTrainDispatch->m_nState);
		return m_pHttpClient->PostRequest("/dispatchingOrderTrain/saveDisPatchingOrderTrainData", strContent, btResult);
	}

	bool HttpClient::SelectStaTrainDispatch(int nDispatchOrderId, QByteArray& btResult)
	{
		QString strContent = QString("orderId=%1").arg(nDispatchOrderId);
		return m_pHttpClient->PostRequest("/dispatchingOrderTrain/selectDispatchingOrderTrainList", strContent, btResult);
	}

	bool HttpClient::ModifyStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult)
	{
		QString strContent = QString("orderId=%1&strTrainNum=%2&strLocomotiveNumber=%3")
			.arg(pTrainDispatch->m_nOrderId)
			.arg(pTrainDispatch->m_strTrainNum)
			.arg(pTrainDispatch->m_strLocomotive);
		return m_pHttpClient->PostRequest("/dispatchingOrderTrain/updateDispatchingOrderTrain", strContent, btResult);
	}

	bool HttpClient::DeleteStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult)
	{
		QString strContent = QString("orderId=%1").arg(pTrainDispatch->m_nOrderId);
		return m_pHttpClient->PostRequest("/dispatchingOrderTrain/deleteDispatchingOrderTrain", strContent, btResult);
	}
}