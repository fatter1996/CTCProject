#pragma once
#include <QObject>
#include <QDateTime>

#define ROUTE_TYPE_ARRIVAL	true
#define ROUTE_TYPE_DEPART	false

#define PLAN_TYPE_ARRIVED	0x01
#define PLAN_TYPE_START		0x02
#define PLAN_TYPE_FINAL		0x03
#define PLAN_TYPE_THROUGH	0x04

namespace Station {
	enum class LimitsOfAuthority : int {
		admin,		
		employee    
	};

	enum class VisibleDev : int {
		button,					//��ť
		trainButton,			//�г���ť
		shuntButton,			//������ť
		trackName,				//�ɵ�����
		unswitchSectionName,	//�޲���������
		sectionName,			//��������
		auxiliaryLineName,		//����������
		switchName,				//��������
		direction,				//����
		phaseSeparaName,		//��������
		switchSectionName,		//������������
		stationName,			//վ��
		text,					//����
		funButtonName,			//���ܰ�ť����

		entrySignalName,		//��վ�źŻ�����
		exitSignalName,			//��վ�źŻ�����
		shuntSignalName,		//�����źŻ�����
		sectionSignalName,		//�����źŻ�����
		stateLempName,			//״̬������
		occludeLempName,		//����������
		RBCChangeName,			//RBC�л�����

		stateLemp,				//״̬��
		phaseSepara,			//����
		notes,					//�˹�ע��
		shuntSignal,			//�����źŻ�
		sectionSignal,			//�����źŻ�
		occludeLemp,			//������
		squeezeLamp,			//��������
		RBCChange,				//RBC�л�
		auxiliaryLine,			//������
		insulationNode,			//��Ե��
		platform,				//վ̨
		endMarker,				//��ͷ��
		trainNumWnd,			//���δ�
		preview,				//����Ԥ�洰
		previewFrame,			//����Ԥ�洰���
		permillSix,				//ǧ��֮����ʶ
		counter					//������
	};

	enum class Order : int {
		train,
		stagePlan,
		trainRoute,
		trainDispatch,
		dispatch,
		trafficLog
	};

	enum class Limits : int {
		RouteLimits,
		AutoSignStage,
		ExStaControl,
		ControlMode,
		PlanMode,
		PlanControl,
		ApplyControlMode,
		ActiveApplyControlMode
	};

	struct RailwayLine {
		QString startStation;
		QString middleStation;
		QString endStation;
		int startY;
		int middleY;
		int endY;
	};

	struct TrainDiagramInfo //����ͼ���涨��
	{
		int width = 0;
		int height = 0;
		int linenum = 0;
		int miniteDistance = 0; //ʮ�ָ����
		int stationDistance = 0; //��վ�����
		int lintDistance = 0; //��·�����
		QVector<RailwayLine> vectRailwayLine;
	}; //����ͼ

	struct StaOrder {
	};

	struct StaStagePlan;
	struct StaTrafficLog;
	struct StaDispatchOrder;

	struct StaTrain : public StaOrder //�г�����
	{
		int m_nTrainId = 0; //����ID 
		QString m_strTrainNum; //���κ�
		bool m_bRight = true; //����(����-false ����-true)
		int m_nTrainLengh = 0; //���鳤��
		int m_nSpeed = 0; //�ٶ�
		bool m_bElectric = true; //����
		int m_nOverLimitLevel = 0; //���޵ȼ�
		bool m_bFreightTrain = false; //�л�����(�г�-false, ����-true)
		int m_nPosCode = -1; //����λ���豸��
		int m_nTrainType = 0; //�г�������ţ����ڶ����顢ͨ���г��ȣ�
		QString m_strTrainType;	//�г�����
		int m_nOperationType = 0; //����������ţ������顢�����ÿ��г��ȣ�
		QString m_strOperationType;	//��������
		bool m_bRunning = false; //�Ƿ���������

		bool m_bArmy = false; //����
		bool m_bImportant = false; //�ص�

		QString m_strLocomotive; //������
		QString m_strDriver; //˾��
		QString m_nTrainMaster; //����
		int m_nTrainValue = 0; //����
		int m_nChangeLengh = 0; //����
		int m_nWeight = 0; //����
		uint m_nEarlyOrLateTime = 0;
		bool m_bDelete = false;
		bool m_bRealTrain = true;

	public:
		StaTrain() {}
		StaTrain(StaStagePlan* pStagePlan);
		StaTrain(StaTrafficLog* pTrafficLog);
		QString OverLimitLevel();
		static void Init(StaTrain* pTrain, const QJsonObject& subObj);
	};

	struct StaStagePlan : public StaOrder	//�׶μƻ�
	{
		int m_nPlanId = 0; //�ƻ�ID
		QString m_strPlanNum; //�ƻ����

		int m_nPlanType = 0; //�ƻ����� (�ӷ�-0x01, ʼ��-0x02, �յ�-0x03, ͨ��-0x04)

		bool m_bElectric = true; //����
		int m_nOverLimitLevel = 0; //���޵ȼ�
		bool m_bFreightTrain = false; //�л�����(�г�-false, ����-true)

		QString m_strArrivalTrainNum; //���ﳵ�κ�
		int m_nArrivalTrackCode = -1; //����ɵ����
		QString m_strArrivalTrack; //����ɵ�
		int m_nEntrySignalCode = -1; //��վ�źŻ����
		QString m_strEntrySignal; //��վ�źŻ�
		QDateTime m_tArrivalTime; //����ʱ��

		QString m_strDepartTrainNum; //�������κ�
		int m_nDepartTrackCode = -1; //�����ɵ����
		QString m_strDepartTrack; //�����ɵ�
		int m_nExitSignalCode = -1; //��վ�źŻ����
		QString m_strExitSignal; //��վ�źŻ�
		QDateTime m_tDepartTime; //����ʱ��
		int m_nJJQDCode = 0;
		QDateTime m_tOrderedTime; //����ʱ��
		int m_nOrderState = 0;//ǩ��״̬ 0:δǩ�� 1:ǩ��  2:��ǩ 
		

	public:
		static void Init(StaStagePlan* pStagePlan, const QJsonObject& subObj);
	};
	
	struct StaTrainRoute : public StaOrder	//�г���·����
	{
		int m_nRouteId = 0; //��·����ID
		int m_nTrainId = 0; //����ID

		bool m_bArrivaRoute = true; //�ӷ����� (�ӳ�-true ����-false)
		bool m_bAutoTouch = false;//������ʽ(�Զ�����-true �˹�����-false)

		QDateTime m_tPlanTime; //�ƻ�ʱ��
		QDateTime m_tTriggerTime; //����ʱ��
		QDateTime m_tCleanTime; //����ʱ��

		int m_nTrackCode = 0;//�ɵ��豸��
		QString m_strTrack;//�ɵ�����
		int m_nSignalCode = -1; //��/��վ�źŻ����
		QString m_strSignal; //��/��վ�źŻ�

		QString m_strCurRouteDescrip; //��·����
		QStringList m_strRouteDescripList; //��·����
		QString m_strDirection; //��·����
		int m_nRouteState = 0; //��·��ǰ״̬//0-�ȴ���1-���ڴ�����2-������ɡ�3-ռ�á�4-���塢5-ȡ��

		QVector<int> m_vecSubRouteId;	//�ӽ�·ID
		QVector<QString> m_vecFlexibleRoute;	//��ͨ��·
		int m_nLogId = 0;	//�����г���־ID
		bool m_bSunTrainRoute = false;
		int m_nRowIndex = 0;
		bool m_bTempRoute = false;

	public:
		StaTrainRoute() {}
		StaTrainRoute(StaTrafficLog* pTrafficLog, bool bArrivaRoute);

	public:
		static void Init(StaTrainRoute* pTrainRoute, const QJsonObject& subObj);
		void ChangeTrack(int nCode, const QString& strName);
		QString getStateStr();
		void getRouteDescrip();
	};

	struct StaTrainDispatch : public StaOrder	//������������
	{
		int m_nTrainId = 0;	//����ID
		int m_nOrderId = 0;	//������������ID
		QString m_strTrainNum;//����
		QString m_strLocomotive;//����
		QDateTime m_tRecv;//��������ʱ��
		int m_nState = 0;//����״̬ 1:���� 0:δǩ��

	public:
		static void Init(StaTrainDispatch* pTrainDispatch, const QJsonObject& subObj);
		StaDispatchOrder* getParentOrder();
		QString getStateStr();
	};

	struct StaDispatchOrder : public StaOrder //��������
	{
		int m_nOrderId = 0; //����ID
		int m_nOrderType = 1; //��������(1-��վ 2-����̨ 3-����)
		int m_nSendState = 2; //����״̬(1-����, 2-����, 3-����)
		QString m_strOrderNum; //������

		QString m_strType;//��������
		QString m_strOrderTip;//�������
		QString m_strContent;//��������

		QString m_strSendAgency; //�������
		QString m_strSendName; //����Ա
		QDateTime m_tSendTime; //����ʱ��

		QString m_strOrderedUnit; //���λ
		QString m_strSignName; //ǩ����
		QDateTime m_tSignTime;//ǩ��ʱ��
		bool m_bImportant = false; //�ص�/��������
		int m_nStateDisOrder = 0;//��������ǩ��״̬ 0:δǩ�� 1:ǩ��  2:��ǩ 

		QVector<StaTrainDispatch*> m_vecTrainDispatch;

	public:
		static void Init(StaDispatchOrder* pDispatch, const QJsonObject& subObj);
	};

	struct StaTrafficLog : public StaOrder //�г���־
	{
		int m_nLogId = 0; //��־ID 
		int m_nTrainId = 0; //����ID 
		int m_nPlanType = 0; //�ƻ����� (�ӷ�-0x01,ʼ��-0x02, �յ�-0x03, ͨ��-0x04)

		//������Ϣ
		QString m_strArrivalTrainNum; //���ﳵ�κ�
		int m_nArrivalTrackCode = 0; //����ɵ����
		QString m_strArrivalTrack;//����ɵ�
		int m_nArrivalSignalCode = 0; //��վ�źŻ����
		QString m_strArrivaSignal; //��վ�źŻ�
		QDateTime m_tProvArrivalTime; //�ƻ���վʱ��
		QDateTime m_tRealArrivalTime; //ʵ�ʵ�վʱ��
		QDateTime m_tAgrAdjDepartTime; //ͬ����վ����ʱ��
		QDateTime m_tAdjDepartTime; //��վ����ʱ��
		int m_nArrivalLimit = 0; //���ﳬ�޵ȼ�
		int m_nArrivalRouteId = 0; //�ӳ���·ID
		QString m_strArrivalLocomotive; //���������
		QString m_strArrivalDriver; //����˾��
		QString m_nArrivalTrainMaster; //���ﳵ��
		int m_nArrivalTrainValue = 0; //���ﳵ��
		int m_nArrivalChange = 0; //���ﻻ��
		int m_nArrivalWeight = 0; //��������

		//������Ϣ
		QString m_strDepartTrainNum; //�������κ�
		int m_nDepartTrackCode = 0;//�����ɵ����
		QString m_strDepartTrack;//�����ɵ�
		int m_nDepartSignalCode = 0;//��վ�źŻ����
		QString m_strDepartSignal; //��վ�źŻ�
		QDateTime m_tProvDepartTime;//�ƻ�����ʱ��
		QDateTime m_tRealDepartTime;//ʵ�ʷ���ʱ��
		QDateTime m_tAdjAgrDepartTime;//��վͬ�ⷢ��ʱ��
		QDateTime m_tAdjArrivalTime;//��վ����ʱ��
		int m_nDepartLimit = 0; //�������޵ȼ�
		int m_nDepartRouteId = 0;	//������·ID
		QString m_strDepartLocomotive; //����������
		QString m_strDepartDriver; //����˾��
		QString m_nDepartTrainMaster; //���ﳵ��
		int m_nDepartTrainValue = 0; //���ﳵ��
		int m_nDepartChange = 0; //���ﻻ��
		int m_nDepartWeight = 0; //��������

		QString m_strDelayReason;//���ԭ��
		bool m_bUpDown = false; //�����г���(����-true ����-false)
		bool m_bDeleteFlag = false;//ɾ����־
		QString m_strTrainArr;//�г�����
		QString m_strNotes;//����
		bool m_bPassenger = false;//�������
		bool m_bAllowTrackNotMatch = false;//����ɵ���̶���·��ͬ
		bool m_bAllowSignalNotMatch = false;//����������̶���·��ͬ
		
		bool m_bLJStatus = false;//�м�
		bool m_bJLStatus = false;//����
		bool m_bJPStatus = false;//��Ʊ
		bool m_bLWStatus = false;//��β
		bool m_bJCStatus = false;//����
		bool m_bHJStatus = false;//����
		bool m_bCJStatus = false;//�˽�
		bool m_bSSStatus = false;//��ˮ
		bool m_bZGStatus = false;//ժ��
		bool m_bHCStatus = false;//����
		bool m_bZXStatus = false;//װж
		bool m_bXWStatus = false;//����
		bool m_bDKStatus = false;//����
		bool m_bCHStatus = false;//����
		bool m_bZWStatus = false;//վ��
		bool m_bZKStatus = false;//�ۿ�
		QString m_strTrainPosStatus;//�г�λ����Ϣ
		QString m_strProc;//�ƻ���һ������

	public:
		StaTrafficLog() {}
		StaTrafficLog(StaStagePlan* pStaStagePlan, int nTrainId);
		bool IsReportedPoints();
		static void Init(StaTrafficLog* pTrafficLog, const QJsonObject& subObj);
	};
}