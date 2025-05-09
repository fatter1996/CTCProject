#pragma once
#include <QNetworkAccessManager>
#include <QHostAddress>
#include "StationObject/GlobalStruct.h"

namespace Http {
	class HttpClient {

	public:
		HttpClient();
		~HttpClient();

	public:
		static void setServerAddress(const QHostAddress& hAddress, const quint16& nPort);
		static bool PostRequest(QString strUrl, QString strContent, QByteArray& btResult);
		static bool GetRequest(QString strUrl, QByteArray& btResult);

		//阶段计划
		static bool AddStaStagePlan(Station::StaStagePlan* pStagePlan, QByteArray& btResult);
		static bool SelectStaStagePlan(QByteArray& btResult);
		static bool DeleteStaStagePlan(int nPlanId, QByteArray& btResult);
		static bool ClearStaStagePlan(QByteArray& btResult);
		//车次
		static bool AddStaTrain(Station::StaTrain* pTrain, QByteArray& btResult);
		static bool SelectStaTrainList(QByteArray& btResult);
		static bool SelectStaTrain(int nTrainId, QByteArray& btResult);
		static bool DeleteStaTrain(int nTrainId, QByteArray& btResult);
		static bool ChangeTrainNum(int nTrainId, QString strTrainNum, QByteArray& btResult);
		static bool SetTrainRunState(int nTrainId, bool bRunning, QByteArray& btResult);
		static bool UpdataTrainPos(int nTrainId, int nPosCode, QByteArray& btResult);
		static bool ChangeTrainAttr(int nTrainId, int nSpeed, QString strLocomotive, bool bElectric, QByteArray& btResult);
		static bool ClearStaTrain(QByteArray& btResult);
		//进路序列
		static bool AddStaTrainRoute(Station::StaTrainRoute* pTrainRoute, QByteArray& btResult);
		static bool SelectStaTrainRoute(QByteArray& btResult);
		static bool UpdataRouteState(int nRouteId, int nState, QByteArray& btResult);
		static bool ChangeRouteTrack(int nRouteId, int nTrackCode, QByteArray& btResult);
		static bool ChangeTriggerType(int nRouteId, int nTriggerType, QByteArray& btResult);
		static bool ChangeRoute(int nRouteId, QString strRoute, QByteArray& btResult);
		static bool DeleteStaTrainRoute(int nRouteId, QByteArray& btResult);
		static bool ClearStaTrainRoute(QByteArray& btResult);
		//调度命令
		static bool AddStaDispatchOrder(Station::StaDispatchOrder* pDispatchOrder, QByteArray& btResult);
		static bool SelectStaDispatchOrder(QByteArray& btResult);
		static bool SignForDispatchOrder(int nOrderId, QByteArray& btResult);
		static bool DeleteDispatchOrder(int nOrderId, QByteArray& btResult);
		static bool ClearStaDispatchOrder(QByteArray& btResult);
		//机车调度命令
		static bool AddStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult);
		static bool SelectStaTrainDispatch(int nDispatchOrderId, QByteArray& btResult);
		static bool ModifyStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult);
		static bool DeleteStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult);
		static bool ClearStaTrainDispatch(QByteArray& btResult);
		//行车日志
		static bool AddStaTrafficLog(Station::StaTrafficLog* pTrafficLog, QByteArray& btResult);
		static bool SelectStaTrafficLog(QByteArray& btResult);
		static bool UpdataRouteId(int nLogId, int nArrivalRouteId, int nDepartRouteId, QByteArray& btResult);
		static bool UpdataPointReportTime(int nLogId, QString strPointType, QDateTime tPointTime, QByteArray& btResult);
		static bool DeleteStaTrafficLog(int nLogId, QByteArray& btResult);
		static bool ClearStaTrafficLog(QByteArray& btResult);
		//权限
		static bool SelectStaLimits(QByteArray& btResult);
		static bool UpdataStaLimits(Station::Limits eType, int nLimits, QByteArray& btResult);

	private:
		static QHostAddress m_hServerIp;
		static uint m_nServerPort;
	};
}