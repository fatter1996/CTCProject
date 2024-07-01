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
		void setServerAddress(const QHostAddress& hAddress, const quint16& nPort);
		bool PostRequest(QString strUrl, QString strContent, QByteArray& btResult);
		bool GetRequest(QString strUrl, QByteArray& btResult);

	public:
		//阶段计划
		static bool AddStaStagePlan(Station::StaStagePlan* pStagePlan, QByteArray& btResult);
		static bool SelectStaStagePlan(QByteArray& btResult);
		//车次
		static bool AddStaTrain(Station::StaTrain* pTrain, QByteArray& btResult);
		static bool SelectStaTrain(QByteArray& btResult);
		//进路序列
		static bool AddStaTrainRoute(Station::StaTrainRoute* pTrainRoute, QByteArray& btResult);
		static bool SelectStaTrainRoute(QByteArray& btResult);
		//调度命令
		static bool AddStaDispatchOrder(Station::StaDispatchOrder* pDispatchOrder, QByteArray& btResult);
		static bool SelectStaDispatchOrder(QByteArray& btResult);
		static bool SignForDispatchOrder(int nOrderId, QByteArray& btResult);
		//机车调度命令
		static bool AddStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult);
		static bool SelectStaTrainDispatch(int nDispatchOrderId, QByteArray& btResult);
		static bool ModifyStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult);
		static bool DeleteStaTrainDispatch(Station::StaTrainDispatch* pTrainDispatch, QByteArray& btResult);

	private:
		QNetworkAccessManager* m_pManager = nullptr;
		QHostAddress m_hServerIp;
		uint m_nServerPort = 0;

	private:
		static HttpClient* m_pHttpClient;
	};
}