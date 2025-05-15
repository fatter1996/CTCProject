#include "HttpClient.h"
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Http {
	QHostAddress HttpClient::m_hServerIp;
	uint HttpClient::m_nServerPort = 0;

	HttpClient::HttpClient()
	{
	}

	HttpClient::~HttpClient()
	{
	}

	void HttpClient::setServerAddress(const QHostAddress& hAddress, const quint16& nPort)
	{
		m_hServerIp = hAddress;
		m_nServerPort = nPort;
	}

	bool HttpClient::PostRequest(QString strUrl, QString strContent, QByteArray& btResult)
	{
		QNetworkAccessManager* m_pManager = new QNetworkAccessManager;
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

		if (postReply->error() == QNetworkReply::NoError) {
			btResult = postReply->readAll();
			qDebug() << strUrl << strContent << QString::fromUtf8(btResult);
			postReply->deleteLater(); //删除reply对象
			delete m_pManager;
			return true;
		}
		else {
			// 处理错误...
			postReply->deleteLater(); //删除reply对象
			delete m_pManager;
			return false;
		}
	}

	bool HttpClient::GetRequest(QString strUrl, QByteArray& btResult)
	{
		QNetworkAccessManager* m_pManager = new QNetworkAccessManager;
		QNetworkRequest getRequest(strUrl);
		QNetworkReply* getReply = m_pManager->get(getRequest);
		// 创建一个局部的事件循环，等待请求完成
		QEventLoop loop;
		QTimer timer;
		timer.setSingleShot(true); // 只触发一次
		timer.start(1000); // 5秒后超时
		QObject::connect(getReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
		QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
		loop.exec();

		if (timer.isActive()) {	//超时处理
			qDebug() << "Timeout!";
			timer.stop();
			delete m_pManager;
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
			delete m_pManager;
			return false;
		}
	}

	bool  HttpClient::AddStaStagePlan(Station::StaStagePlan* pStagePlan, QByteArray& btResult)
	{
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

		return PostRequest("/stagePlan/saveStagePlan", strContent, btResult);
	}

	bool HttpClient::SelectStaStagePlan(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/stagePlan/selectStagePlanList", strContent, btResult);
	}

	bool HttpClient::DeleteStaStagePlan(int nPlanId, QByteArray& btResult)
	{
		QString strContent = QString("planId=%1").arg(nPlanId);
		return PostRequest("/stagePlan/deleteStagePlan", strContent, btResult);
	}

	bool HttpClient::ClearStaStagePlan(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/stagePlan/clearStagePlanData", strContent, btResult);
	}

	bool HttpClient::AddStaTrain(Station::StaTrain* pTrain, QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1&trainNum=%2&direction=%3&trainLength=%4"
			"&speed=%5&electric=%6&overLimitLevel=%7&freightTrain=%8&posCode=%9"
			"&trainTypeNumber=%10&operationTypeNumber=%11&isRun=%12&army=%13&keynote=%14&trainId=%15")
			.arg(Station::MainStation()->getStationId())
			.arg(pTrain->m_strTrainNum)
			.arg(pTrain->m_bRight)
			.arg(pTrain->m_nTrainLengh) 
			.arg(pTrain->m_nSpeed)
			.arg(pTrain->m_bElectric)
			.arg(pTrain->m_nOverLimitLevel)
			.arg(pTrain->m_bFreightTrain)
			.arg(pTrain->m_nPosCode)
			.arg(pTrain->m_nTrainType)
			.arg(pTrain->m_nOperationType)
			.arg(pTrain->m_bRunning)
			.arg(pTrain->m_bArmy)
			.arg(pTrain->m_bImportant)
			.arg(pTrain->m_nTrainId);
		return PostRequest("/trainProperty/saveTrainProperty", strContent, btResult);
	}

	bool HttpClient::ChangeStaTrain(int stationId, QMap<QString, QByteArray> m_mapTrainValue, QByteArray& btResult)
	{

		QString strContent = QString("stationId=%1").arg(stationId);
		QString key;
		QString Value;
		QMap<QString, QByteArray>::iterator it;
		for (it = m_mapTrainValue.begin(); it != m_mapTrainValue.end(); ++it) {
			key = it.key();
			Value = it.value();
			strContent += QString("&%1=%2").arg(key).arg(Value);
		}
		qDebug() << strContent;
		return PostRequest("/trainRouteSequence/updateTrainRouteSequence", strContent, btResult);
	}

	bool HttpClient::SelectStaTrainList(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/trainProperty/selectTrainPropertyList", strContent, btResult);
	}

	bool HttpClient::SelectStaTrain(int nTrainId, QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1&trainId=%2")
			.arg(Station::MainStation()->getStationId()).arg(nTrainId);
		return PostRequest("/trainProperty/selectTrainPropertyByTrainIdAndStationId", strContent, btResult);
	}

	bool HttpClient::DeleteStaTrain(int nTrainId, QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1&trainId=%2").arg(Station::MainStation()->getStationId()).arg(nTrainId);
		return PostRequest("/trainProperty/deleteTrainProperty", strContent, btResult);
	}

	//bool HttpClient::ChangeTrainNum(int nTrainId, QString strTrainNum, QByteArray& btResult)
	//{
	//	QString strContent = QString("stationId=%1&trainId=%2&trainNum=%3")
	//		.arg(Station::MainStation()->getStationId()).arg(nTrainId).arg(strTrainNum);
	//	return PostRequest("/trainProperty/updateTrainProperty", strContent, btResult);
	//}
	//
	bool HttpClient::SetTrainRunState(int nTrainId, bool bRunning, QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1&trainId=%2&isRun=%3")
			.arg(Station::MainStation()->getStationId()).arg(nTrainId).arg(bRunning);
		return PostRequest("/trainProperty/updateTrainProperty", strContent, btResult);
	}

	//bool HttpClient::UpdataTrainPos(int nTrainId, int nPosCode, QByteArray& btResult)
	//{
	//	QString strContent = QString("stationId=%1&trainId=%2&posCode=%3")
	//		.arg(Station::MainStation()->getStationId()).arg(nTrainId).arg(nPosCode);
	//	return PostRequest("/trainProperty/updateTrainProperty", strContent, btResult);
	//}
	//
	//bool HttpClient::ChangeTrainAttr(int nTrainId, int nSpeed, QString strLocomotive, bool bElectric, QByteArray& btResult)
	//{
	//	QString strContent = QString("stationId=%1&trainId=%2&speed=%3&strLocomotive=%4&electric=%5")
	//		.arg(Station::MainStation()->getStationId()).arg(nTrainId).arg(nSpeed).arg(strLocomotive).arg(bElectric);
	//	return PostRequest("/trainProperty/updateTrainProperty", strContent, btResult);
	//}
	//
	bool HttpClient::ClearStaTrain(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/trainProperty/clearTrainPropertyData", strContent, btResult);
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
			"&routeDepict=%10&direction=%11&routeState=%12&subRouteId=%13&logId=%14")
			.arg(Station::MainStation()->getStationId())
			.arg(pTrainRoute->m_nTrainId)
			.arg(pTrainRoute->m_bArrivaRoute)
			.arg(pTrainRoute->m_bAutoTouch)
			.arg(pTrainRoute->m_tPlanTime.toString(Qt::ISODate))
			.arg(pTrainRoute->m_tTriggerTime.toString(Qt::ISODate))
			.arg(pTrainRoute->m_tCleanTime.toString(Qt::ISODate))
			.arg(pTrainRoute->m_nTrackCode)
			.arg(pTrainRoute->m_nSignalCode)
			.arg(pTrainRoute->m_strCurRouteDescrip)
			.arg(pTrainRoute->m_strDirection)
			.arg(pTrainRoute->m_nRouteState)
			.arg(strSubRouteId.left(strSubRouteId.length() - 1))
			.arg(pTrainRoute->m_nLogId);

		return PostRequest("/trainRouteSequence/saveTrainRouteSequence", strContent, btResult);
	}

	bool HttpClient::ChangeStaTraffRouteData(int nRouteId, QMap<QString, QByteArray> m_mapRouteValue, QByteArray& btResult)
	{
		QString strContent = QString("id=%1").arg(nRouteId);
		QString key;
		QString Value;
		QMap<QString, QByteArray>::iterator it;
		for (it = m_mapRouteValue.begin(); it != m_mapRouteValue.end(); ++it) {
			key = it.key();
			Value = it.value();
			strContent += QString("&%1=%2").arg(key).arg(Value);
		}
		qDebug() << strContent;
		return PostRequest("/trainRouteSequence/updateTrainRouteSequence", strContent, btResult);
	}


	

	bool HttpClient::SelectStaTrainRoute(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/trainRouteSequence/selectTrainRouteSequenceList", strContent, btResult);
	}

	//bool HttpClient::UpdataRouteState(int nRouteId, int nState, QByteArray& btResult)
	//{
	//	QString strContent = QString("routeId=%1&routeState=%2")
	//		.arg(nRouteId).arg(nState);
	//	return PostRequest("/trainRouteSequence/updateTrainRouteSequence", strContent, btResult);
	//}
	//
	//bool HttpClient::ChangeRouteTrack(int nRouteId, int nTrackCode, QByteArray& btResult)
	//{
	//	QString strContent = QString("routeId=%1&trackCode=%2")
	//		.arg(nRouteId).arg(nTrackCode);
	//	return PostRequest("/trainRouteSequence/updateTrainRouteSequence", strContent, btResult);
	//}
	//
	//bool HttpClient::ChangeTriggerType(int nRouteId, int nTriggerType, QByteArray& btResult)
	//{
	//	QString strContent = QString("routeId=%1&autoTouch=%2")
	//		.arg(nRouteId).arg(nTriggerType);
	//	return PostRequest("/trainRouteSequence/updateTrainRouteSequence", strContent, btResult);
	//}
	//
	//bool HttpClient::ChangeRoute(int nRouteId, QString strRoute, QByteArray& btResult)
	//{
	//	QString strContent = QString("routeId=%1&routeDepict=%2")
	//		.arg(nRouteId).arg(strRoute);
	//	return PostRequest("/trainRouteSequence/updateTrainRouteSequence", strContent, btResult);
	//}

	bool HttpClient::DeleteStaTrainRoute(int nRouteId, QByteArray& btResult)
	{
		QString strContent = QString("routeId=%1").arg(nRouteId);
		return PostRequest("/trainRouteSequence/deleteTrainRouteSequence", strContent, btResult);
	}

	bool HttpClient::ClearStaTrainRoute(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/trainRouteSequence/clearTrainRouteSequenceData", strContent, btResult);
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
		return PostRequest("/dispatchingOrder/saveDisPatchingOrderData", strContent, btResult);
	}

	bool HttpClient::SelectStaDispatchOrder(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/dispatchingOrder/selectDispatchingOrderList", strContent, btResult);
	}

	bool HttpClient::SignForDispatchOrder(int nOrderId, QByteArray& btResult)
	{
		QString strContent = QString("orderId=%1&stateDisOrder=1").arg(nOrderId);
		return PostRequest("/dispatchingOrder/updateDispatchingOrder", strContent, btResult);
	}

	bool  HttpClient::DeleteDispatchOrder(int nOrderId, QByteArray& btResult)
	{
		QString strContent = QString("orderId=%1").arg(nOrderId);
		return PostRequest("/dispatchingOrder/deleteDispatchingOrder", strContent, btResult);
	}

	bool HttpClient::ClearStaDispatchOrder(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/dispatchingOrder/clearDispatchingOrde", strContent, btResult);
	}

	bool HttpClient::AddStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1&orderId=%2&strTrainNum=%3"
			"&strLocomotiveNumber=%4&receiveTime=%5&state=%6")
			.arg(Station::MainStation()->getStationId())
			.arg(pTrainDispatch->m_nOrderId)
			.arg(pTrainDispatch->m_strTrainNum)
			.arg(pTrainDispatch->m_strLocomotive)
			.arg(pTrainDispatch->m_tRecv.toString(Qt::ISODate))
			.arg(pTrainDispatch->m_nState);
		return PostRequest("/dispatchingOrderTrain/saveDisPatchingOrderTrainData", strContent, btResult);
	}

	bool HttpClient::SelectStaTrainDispatch(int nDispatchOrderId, QByteArray& btResult)
	{
		QString strContent = QString("orderId=%1").arg(nDispatchOrderId);
		return PostRequest("/dispatchingOrderTrain/selectDispatchingOrderTrainList", strContent, btResult);
	}

	bool HttpClient::ModifyStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult)
	{
		QString strContent = QString("orderId=%1&strTrainNum=%2&strLocomotiveNumber=%3")
			.arg(pTrainDispatch->m_nOrderId)
			.arg(pTrainDispatch->m_strTrainNum)
			.arg(pTrainDispatch->m_strLocomotive);
		return PostRequest("/dispatchingOrderTrain/updateDispatchingOrderTrain", strContent, btResult);
	}

	bool HttpClient::DeleteStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult)
	{
		QString strContent = QString("orderId=%1").arg(pTrainDispatch->m_nOrderId);
		return PostRequest("/dispatchingOrderTrain/deleteDispatchingOrderTrain", strContent, btResult);
	}

	bool HttpClient::ClearStaTrainDispatch(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/dispatchingOrderTrain/clearDispatchingOrderTrainData", strContent, btResult);
	}

	bool HttpClient::AddStaTrafficLog(Station::StaTrafficLog* pTrafficLog, QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1&trainId=%2&planType=%3"
			"&arrivalTrainNumber=%4&arrivalTrack=%5&provArrivalTime=%6&realArrivalTime=%7&homeSignalCode=%8"
			"&agrAdjDepartTime=%9&adjDepartTime=%10&arrivalLimit=%11&arrivalRouteId=%12&arrivalLocomotive=%13"
			"&arrivalDriver=%14&arrivalLength=%15&arrivalTrainNum=%16&arrivalChange=%17&arrivalWeight=%18"
			"&departTrainNumber=%19&departTrack=%20&provDepartTime=%21&realDepartTime=%22&startingSignalCode=%23"
			"&adjAgrDepartTime=%24&adjArrivalTime=%25&delayReason=%26&departLimit=%27&departRouteId=%28"
			"&departLocomotive=%29&departDriver=%30&departLength=%31&departTrainNum=%32&departChange=%33"
			"&departWeight=%34&upDown=%35&deleteFlag=%36&notes=%37&passenger=%38&allowTrackNotMatch=%39"
			"&allowSignalNotMatch=%40&ljStatus=%41&jlStatus=%42&jpStatus=%43&lwStatus=%44&jcStatus=%45"
			"&hjStatus=%46&cjStatus=%47&ssStatus=%48&zgStatus=%49&hcStatus=%50&zxStatus=%51&xwStatus=%52"
			"&dkStatus=%53&chStatus=%54&zwStatus=%55&zkStatus=%56&trainPosStatus=%57&proc=%58&strTrainArr=%59")
			.arg(Station::MainStation()->getStationId())
			.arg(pTrafficLog->m_nTrainId)
			.arg(pTrafficLog->m_nPlanType)
			.arg(pTrafficLog->m_strArrivalTrainNum)
			.arg(pTrafficLog->m_nArrivalTrackCode)
			.arg(pTrafficLog->m_tProvArrivalTime.toString(Qt::ISODate))
			.arg(pTrafficLog->m_tRealArrivalTime.toString(Qt::ISODate))
			.arg(pTrafficLog->m_nArrivalSignalCode)
			.arg(pTrafficLog->m_tAgrAdjDepartTime.toString(Qt::ISODate))
			.arg(pTrafficLog->m_tAdjDepartTime.toString(Qt::ISODate))
			.arg(pTrafficLog->m_nArrivalLimit)
			.arg(pTrafficLog->m_nArrivalRouteId)
			.arg(pTrafficLog->m_strArrivalLocomotive)
			.arg(pTrafficLog->m_strArrivalDriver)
			.arg(pTrafficLog->m_nArrivalTrainMaster)
			.arg(pTrafficLog->m_nArrivalTrainValue)
			.arg(pTrafficLog->m_nArrivalChange)
			.arg(pTrafficLog->m_nArrivalWeight)

			.arg(pTrafficLog->m_strDepartTrainNum)
			.arg(pTrafficLog->m_nDepartTrackCode)
			.arg(pTrafficLog->m_tProvDepartTime.toString(Qt::ISODate))
			.arg(pTrafficLog->m_tRealDepartTime.toString(Qt::ISODate))
			.arg(pTrafficLog->m_nDepartSignalCode)
			.arg(pTrafficLog->m_tAdjAgrDepartTime.toString(Qt::ISODate))
			.arg(pTrafficLog->m_tAdjArrivalTime.toString(Qt::ISODate))
			.arg(pTrafficLog->m_strDelayReason)
			.arg(pTrafficLog->m_nDepartLimit)
			.arg(pTrafficLog->m_nDepartRouteId)
			.arg(pTrafficLog->m_strDepartLocomotive)
			.arg(pTrafficLog->m_strDepartDriver)
			.arg(pTrafficLog->m_nDepartTrainMaster)
			.arg(pTrafficLog->m_nDepartTrainValue)
			.arg(pTrafficLog->m_nDepartChange)
			.arg(pTrafficLog->m_nDepartWeight)

			.arg(pTrafficLog->m_bUpDown)
			.arg(pTrafficLog->m_bDeleteFlag)
			.arg(pTrafficLog->m_strNotes)
			.arg(pTrafficLog->m_bPassenger)

			.arg(pTrafficLog->m_bAllowTrackNotMatch)
			.arg(pTrafficLog->m_bAllowSignalNotMatch)
			.arg(pTrafficLog->m_bLJStatus)
			.arg(pTrafficLog->m_bJLStatus)
			.arg(pTrafficLog->m_bJPStatus)
			.arg(pTrafficLog->m_bLWStatus)
			.arg(pTrafficLog->m_bJCStatus)
			.arg(pTrafficLog->m_bHJStatus)
			.arg(pTrafficLog->m_bCJStatus)
			.arg(pTrafficLog->m_bSSStatus)
			.arg(pTrafficLog->m_bZGStatus)
			.arg(pTrafficLog->m_bHCStatus)
			.arg(pTrafficLog->m_bZXStatus)
			.arg(pTrafficLog->m_bXWStatus)
			.arg(pTrafficLog->m_bDKStatus)
			.arg(pTrafficLog->m_bCHStatus)
			.arg(pTrafficLog->m_bZWStatus)
			.arg(pTrafficLog->m_bZKStatus)
			.arg(pTrafficLog->m_strTrainPosStatus)
			.arg(pTrafficLog->m_strProc)
			.arg(pTrafficLog->m_strTrainArr);
		return PostRequest("/trainLog/insertTrainLogData", strContent, btResult);
	}

	bool HttpClient::ChangeStaTrafficLogData(int nLogId, QMap<QString, QByteArray> m_mapLogValue, QByteArray& btResult)
	{
		QString strContent = QString("id=%1").arg(nLogId);
		QString key;
		QString Value;
		QMap<QString, QByteArray>::iterator it;
		for (it = m_mapLogValue.begin(); it != m_mapLogValue.end(); ++it) {
			key = it.key();
			Value = it.value();
			strContent += QString("&%1=%2").arg(key).arg(Value);
		}
		qDebug() << strContent;
		return PostRequest("/trainLog/updateTrainLogData", strContent, btResult);
	}

	bool HttpClient::SelectStaTrafficLog(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/trainLog/selectTrainLogList", strContent, btResult);
	}

	bool HttpClient::UpdataRouteId(int nLogId, int nArrivalRouteId, int nDepartRouteId, QByteArray& btResult)
	{
		QString strContent = QString("id=%1&arrivalRouteId=%2&departRouteId=%3")
			.arg(nLogId).arg(nArrivalRouteId).arg(nDepartRouteId);
		return PostRequest("/trainLog/updateTrainLogData", strContent, btResult);
	}

	bool HttpClient::UpdataPointReportTime(int nLogId, QString strPointType, QDateTime tPointTime, QByteArray& btResult)
	{
		QString strContent = QString("id=%1&%2=%3")
			.arg(nLogId).arg(strPointType).arg(tPointTime.toString(Qt::ISODate));
		return PostRequest("/trainLog/updateTrainLogData", strContent, btResult);
	}

	bool HttpClient::DeleteStaTrafficLog(int nLogId, QByteArray& btResult)
	{
		QString strContent = QString("id=%1").arg(nLogId);
		return PostRequest("/trainLog/deleteTrainLogList", strContent, btResult);
	}

	bool HttpClient::ClearStaTrafficLog(QByteArray& btResult)
	{
		QString strContent = QString("stationId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/trainLog/clearTrainLogData", strContent, btResult);
	}

	bool HttpClient::SelectStaLimits(QByteArray& btResult)
	{
		QString strContent = QString("deptId=%1").arg(Station::MainStation()->getStationId());
		return PostRequest("/system/dept/selectStationById", strContent, btResult);
	}

	bool HttpClient::UpdataStaLimits(Station::Limits eType, int nLimits, QByteArray& btResult)
	{
		QString strType;
		switch (eType)
		{
		case Station::Limits::RouteLimits : strType = "routePermit"; break;
		case Station::Limits::AutoSignStage : strType = "autoSignStage"; break;
		case Station::Limits::ExStaControl: strType = "fczkMode"; break;
		case Station::Limits::ControlMode: strType = "fszlMode"; break;
		case Station::Limits::PlanMode: strType = "planMode"; break;
		case Station::Limits::PlanControl: strType = "planCtrl"; break;
		default: break;
		}
		QString strContent = QString("deptId=%1&%2=%3")
			.arg(Station::MainStation()->getStationId())
			.arg(strType).arg(nLimits);
		return PostRequest("/system/dept/updateStationInfo", strContent, btResult);
	}
}