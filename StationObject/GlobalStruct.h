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
		button,					//按钮
		trainButton,			//列车按钮
		shuntButton,			//调车按钮
		trackName,				//股道名称
		unswitchSectionName,	//无岔区段名称
		sectionName,			//区间名称
		auxiliaryLineName,		//辅助线名称
		switchName,				//道岔名称
		direction,				//方向
		phaseSeparaName,		//分相名称
		switchSectionName,		//道岔区段名称
		stationName,			//站名
		text,					//文字
		funButtonName,			//功能按钮名称

		entrySignalName,		//进站信号机名称
		exitSignalName,			//出站信号机名称
		shuntSignalName,		//调车信号机名称
		sectionSignalName,		//区间信号机名称
		stateLempName,			//状态灯名称
		occludeLempName,		//闭塞灯名称
		RBCChangeName,			//RBC切换名称

		stateLemp,				//状态灯
		phaseSepara,			//分相
		notes,					//人工注释
		shuntSignal,			//调车信号机
		sectionSignal,			//区间信号机
		occludeLemp,			//闭塞灯
		squeezeLamp,			//挤岔报警灯
		RBCChange,				//RBC切换
		auxiliaryLine,			//辅助线
		insulationNode,			//绝缘节
		platform,				//站台
		endMarker,				//尽头标
		trainNumWnd,			//车次窗
		preview,				//车次预告窗
		previewFrame,			//车次预告窗外框
		permillSix,				//千分之六标识
		counter					//计数器
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

	struct TrainDiagramInfo //运行图界面定义
	{
		int width = 0;
		int height = 0;
		int linenum = 0;
		int miniteDistance = 0; //十分格距离
		int stationDistance = 0; //车站间距离
		int lintDistance = 0; //线路间距离
		QVector<RailwayLine> vectRailwayLine;
	}; //运行图

	struct StaOrder {
	};

	struct StaStagePlan;
	struct StaTrafficLog;
	struct StaDispatchOrder;

	struct StaTrain : public StaOrder //列车属性
	{
		int m_nTrainId = 0; //车次ID 
		QString m_strTrainNum; //车次号
		bool m_bRight = true; //方向(左行-false 右行-true)
		int m_nTrainLengh = 0; //编组长度
		int m_nSpeed = 0; //速度
		bool m_bElectric = true; //电力
		int m_nOverLimitLevel = 0; //超限等级
		bool m_bFreightTrain = false; //列货类型(列车-false, 货车-true)
		int m_nPosCode = -1; //所在位置设备号
		int m_nTrainType = 0; //列车类型序号（管内动车组、通勤列车等）
		QString m_strTrainType;	//列车类型
		int m_nOperationType = 0; //运行类型序号（动车组、快速旅客列车等）
		QString m_strOperationType;	//运行类型
		bool m_bRunning = false; //是否正在运行

		bool m_bArmy = false; //军运
		bool m_bImportant = false; //重点

		QString m_strLocomotive; //机车号
		QString m_strDriver; //司机
		QString m_nTrainMaster; //车长
		int m_nTrainValue = 0; //车数
		int m_nChangeLengh = 0; //换长
		int m_nWeight = 0; //总重
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

	struct StaStagePlan : public StaOrder	//阶段计划
	{
		int m_nPlanId = 0; //计划ID
		QString m_strPlanNum; //计划编号

		int m_nPlanType = 0; //计划类型 (接发-0x01, 始发-0x02, 终到-0x03, 通过-0x04)

		bool m_bElectric = true; //电力
		int m_nOverLimitLevel = 0; //超限等级
		bool m_bFreightTrain = false; //列货类型(列车-false, 货车-true)

		QString m_strArrivalTrainNum; //到达车次号
		int m_nArrivalTrackCode = -1; //到达股道编号
		QString m_strArrivalTrack; //到达股道
		int m_nEntrySignalCode = -1; //进站信号机编号
		QString m_strEntrySignal; //进站信号机
		QDateTime m_tArrivalTime; //到达时间

		QString m_strDepartTrainNum; //出发车次号
		int m_nDepartTrackCode = -1; //出发股道编号
		QString m_strDepartTrack; //出发股道
		int m_nExitSignalCode = -1; //出站信号机编号
		QString m_strExitSignal; //出站信号机
		QDateTime m_tDepartTime; //出发时间
		int m_nJJQDCode = 0;
		QDateTime m_tOrderedTime; //接收时间
		int m_nOrderState = 0;//签收状态 0:未签收 1:签收  2:拒签 
		

	public:
		static void Init(StaStagePlan* pStagePlan, const QJsonObject& subObj);
	};
	
	struct StaTrainRoute : public StaOrder	//列车进路序列
	{
		int m_nRouteId = 0; //进路序列ID
		int m_nTrainId = 0; //车次ID

		bool m_bArrivaRoute = true; //接发类型 (接车-true 发车-false)
		bool m_bAutoTouch = false;//触发方式(自动触发-true 人工触发-false)

		QDateTime m_tPlanTime; //计划时间
		QDateTime m_tTriggerTime; //触发时间
		QDateTime m_tCleanTime; //出清时间

		int m_nTrackCode = 0;//股道设备号
		QString m_strTrack;//股道名称
		int m_nSignalCode = -1; //进/出站信号机编号
		QString m_strSignal; //进/出站信号机

		QString m_strCurRouteDescrip; //进路描述
		QStringList m_strRouteDescripList; //进路描述
		QString m_strDirection; //进路方向
		int m_nRouteState = 0; //进路当前状态//0-等待、1-正在触发、2-触发完成、3-占用、4-出清、5-取消

		QVector<int> m_vecSubRouteId;	//子进路ID
		QVector<QString> m_vecFlexibleRoute;	//变通进路
		int m_nLogId = 0;	//关联行车日志ID
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

	struct StaTrainDispatch : public StaOrder	//机车调度命令
	{
		int m_nTrainId = 0;	//机车ID
		int m_nOrderId = 0;	//关联调度命令ID
		QString m_strTrainNum;//车次
		QString m_strLocomotive;//机车
		QDateTime m_tRecv;//机车接收时间
		int m_nState = 0;//接收状态 1:接收 0:未签收

	public:
		static void Init(StaTrainDispatch* pTrainDispatch, const QJsonObject& subObj);
		StaDispatchOrder* getParentOrder();
		QString getStateStr();
	};

	struct StaDispatchOrder : public StaOrder //调度命令
	{
		int m_nOrderId = 0; //命令ID
		int m_nOrderType = 1; //命令类型(1-车站 2-调度台 3-机车)
		int m_nSendState = 2; //发送状态(1-发送, 2-接收, 3-待发)
		QString m_strOrderNum; //命令编号

		QString m_strType;//命令类型
		QString m_strOrderTip;//命令标题
		QString m_strContent;//命令内容

		QString m_strSendAgency; //发令机构
		QString m_strSendName; //发令员
		QDateTime m_tSendTime; //发令时间

		QString m_strOrderedUnit; //受令单位
		QString m_strSignName; //签收人
		QDateTime m_tSignTime;//签收时间
		bool m_bImportant = false; //重点/长期命令
		int m_nStateDisOrder = 0;//调度命令签收状态 0:未签收 1:签收  2:拒签 

		QVector<StaTrainDispatch*> m_vecTrainDispatch;

	public:
		static void Init(StaDispatchOrder* pDispatch, const QJsonObject& subObj);
	};

	struct StaTrafficLog : public StaOrder //行车日志
	{
		int m_nLogId = 0; //日志ID 
		int m_nTrainId = 0; //车次ID 
		int m_nPlanType = 0; //计划类型 (接发-0x01,始发-0x02, 终到-0x03, 通过-0x04)

		//到达信息
		QString m_strArrivalTrainNum; //到达车次号
		int m_nArrivalTrackCode = 0; //到达股道编号
		QString m_strArrivalTrack;//到达股道
		int m_nArrivalSignalCode = 0; //进站信号机编号
		QString m_strArrivaSignal; //进站信号机
		QDateTime m_tProvArrivalTime; //计划到站时间
		QDateTime m_tRealArrivalTime; //实际到站时间
		QDateTime m_tAgrAdjDepartTime; //同意邻站发车时间
		QDateTime m_tAdjDepartTime; //邻站发车时间
		int m_nArrivalLimit = 0; //到达超限等级
		int m_nArrivalRouteId = 0; //接车进路ID
		QString m_strArrivalLocomotive; //到达机车号
		QString m_strArrivalDriver; //到达司机
		QString m_nArrivalTrainMaster; //到达车长
		int m_nArrivalTrainValue = 0; //到达车数
		int m_nArrivalChange = 0; //到达换长
		int m_nArrivalWeight = 0; //到达总重

		//出发信息
		QString m_strDepartTrainNum; //出发车次号
		int m_nDepartTrackCode = 0;//发车股道编号
		QString m_strDepartTrack;//发车股道
		int m_nDepartSignalCode = 0;//出站信号机编号
		QString m_strDepartSignal; //出站信号机
		QDateTime m_tProvDepartTime;//计划发车时间
		QDateTime m_tRealDepartTime;//实际发车时间
		QDateTime m_tAdjAgrDepartTime;//邻站同意发车时间
		QDateTime m_tAdjArrivalTime;//邻站到达时间
		int m_nDepartLimit = 0; //出发超限等级
		int m_nDepartRouteId = 0;	//发车进路ID
		QString m_strDepartLocomotive; //出发机车号
		QString m_strDepartDriver; //到达司机
		QString m_nDepartTrainMaster; //到达车长
		int m_nDepartTrainValue = 0; //到达车数
		int m_nDepartChange = 0; //到达换长
		int m_nDepartWeight = 0; //到达总重

		QString m_strDelayReason;//晚点原因
		bool m_bUpDown = false; //上下行车次(上行-true 下行-false)
		bool m_bDeleteFlag = false;//删除标志
		QString m_strTrainArr;//列车属性
		QString m_strNotes;//记事
		bool m_bPassenger = false;//办理客运
		bool m_bAllowTrackNotMatch = false;//允许股道与固定径路不同
		bool m_bAllowSignalNotMatch = false;//允许出入口与固定进路不同
		
		bool m_bLJStatus = false;//列检
		bool m_bJLStatus = false;//交令
		bool m_bJPStatus = false;//交票
		bool m_bLWStatus = false;//列尾
		bool m_bJCStatus = false;//机车
		bool m_bHJStatus = false;//货检
		bool m_bCJStatus = false;//乘降
		bool m_bSSStatus = false;//上水
		bool m_bZGStatus = false;//摘挂
		bool m_bHCStatus = false;//换乘
		bool m_bZXStatus = false;//装卸
		bool m_bXWStatus = false;//吸污
		bool m_bDKStatus = false;//道口
		bool m_bCHStatus = false;//车号
		bool m_bZWStatus = false;//站务
		bool m_bZKStatus = false;//综控
		QString m_strTrainPosStatus;//列车位置信息
		QString m_strProc;//计划下一个流程

	public:
		StaTrafficLog() {}
		StaTrafficLog(StaStagePlan* pStaStagePlan, int nTrainId);
		bool IsReportedPoints();
		static void Init(StaTrafficLog* pTrafficLog, const QJsonObject& subObj);
	};
}