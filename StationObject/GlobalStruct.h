#pragma once
#include <QObject>
#include <QDateTime>

#define ROUTE_TYPE_ARRIVAL	true
#define ROUTE_TYPE_DEPART	false

namespace Station {

	enum class VisibleDev : int {
		button = 1,				//按钮
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

		entrySignalName,		//进站信号机名称
		exitSignalName,			//出站信号机名称
		shuntSignalName,		//调车信号机名称
		sectionSignalName,		//区间信号机名称
		stateLempName,			//状态灯名称
		occludeLempName,		//闭塞灯名称
		RBCChengeName,			//RBC切换名称

		stateLemp,				//状态灯
		phaseSepara,			//分相
		notes,					//人工注释
		shuntSignal,			//调车信号机
		sectionSignal,			//区间信号机
		occludeLemp,			//闭塞灯
		RBCChenge,				//RBC切换
		auxiliaryLine,			//辅助线
		insulationNode,			//绝缘节
		platform,				//站台
		endMarker,				//尽头标
		preview,				//车次预告窗
		permillSix,				//千分之六标识
		routePlanWnd			//进路序列窗口
	};

	enum class Order : int {
		train = 1,
		stagePlan,
		trainRoute,
		trainDispatch,
		dispatch,
		trafficLog
	};

	struct StaOrder {
	};

	struct StaTrain : public StaOrder //列车属性
	{
		int m_nTrainId = 0; //车次ID 
		QString m_strArrivalTrainNum; //到达车次号
		QString m_strDepartTrainNum; //出发车次号
		bool m_bRight = true; //方向(左行-false 右行-true)
		int  m_nTrainLengh = 0; //编组长度
		int  m_nSpeed = 0; //速度
		bool m_bElectric = true; //电力
		int m_nOverLimitLevel = 0; //超限等级
		bool m_bFreightTrain = false; //列货类型(列车-false, 货车-true)
		int m_nPosCode = -1; //所在位置设备号
		int m_nTrainType = 0; //列车类型序号（管内动车组、通勤列车等）
		QString m_strTrainType;	//列车类型
		int m_nOperationType = 0; //运行类型序号（动车组、快速旅客列车等）
		QString m_strOperationType;	//运行类型
		bool m_bRunning = false; //是否正在运行

	public:
		static void Init(StaTrain* pTrain, const QJsonObject& subObj);
	};

	struct StaStagePlan : public StaOrder	//阶段计划
	{
		int m_nPlanId = 0; //计划ID
		QString m_strPlanNum; //计划编号

		int m_nPlanType = 0; //计划类型 (接发-0x01, 始发-0x02, 终到-0x03, 通过-0x04)

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

	public:
		static void Init(StaStagePlan* pStagePlan, const QJsonObject& subObj);
	};

	struct StaTrainRoute : public StaOrder	//列车进路序列
	{
		int m_nRouteId = 0; //进路序列ID
		int m_nTrainId = 0; //车次ID

		bool m_bArrivaRoute = true; //接发类型 (接车-true 发车-false)
		bool m_nAutoTouch = false;//触发方式(自动触发-true 人工触发-false)

		QDateTime m_tPlanTime; //计划时间
		QDateTime m_tTriggerTime; //触发时间
		QDateTime m_tCleanTime; //出清时间

		int m_nTrackCode = 0;//股道设备号
		QString m_strTrack;//股道名称
		int m_nSignalCode = -1; //进/出站信号机编号
		QString m_strSignal; //进/出站信号机

		QString m_strRouteDescrip; //进路描述
		QString m_strDirection; //进路方向
		int m_nRouteState = 0; //进路当前状态//0-等待、1-正在触发、2-触发完成、3-占用、4-出清、5-取消

		QVector<int> m_vecSubRouteId;	//子进路ID
		int m_nLogId = 0;	//关联行车日志ID

	public:
		StaTrainRoute() {}
		StaTrainRoute(StaStagePlan* pStaStagePlan, bool bArrivaRoute, int nTrainId);

	public:
		static void Init(StaTrainRoute* pTrainRoute, const QJsonObject& subObj);
		QString getStateStr();

	};

	struct StaDispatchOrder;
	struct StaTrainDispatch : public StaOrder	//机车调度命令
	{
		int m_nTrainId = 0;	//机车ID
		int m_nOrderId = 0;	//关联调度命令ID
		QString m_strTrainNum;//车次
		QString m_strLocomotive;//机车
		QDateTime m_timRecv;//机车接收时间
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
		QString m_strArrivalTrack; //到达股道
		QDateTime m_timProvArrivalTime; //计划到站时间
		QDateTime m_timRealArrivalTime; //实际到站时间
		int m_nArrivaSignalCode = 0; //进站信号机
		QDateTime m_timAgrAdjDepartTime; //同意邻站发车时间
		QDateTime m_timAdjDepartTime; //邻站发车时间
		int m_nArrivalLimit = 0; //到达超限等级
		int m_nArrivalRouteId = 0; //接车进路ID
		QString m_strArrivalLocomotive; //到达机车号
		QString m_strArrivalDriver; //到达司机
		int m_nArrivalLenght = 0; //到达车长
		int m_nArrivalTrainNum = 0; //到达车数
		int m_nArrivalChenge = 0; //到达换长
		int m_nArrivalWeight = 0; //到达总重

		//出发信息
		QString m_strDepartTrainNum; //出发车次号
		QString m_strDepartTrack;//发车股道
		QDateTime m_timProvDepartTime;//计划发车时间
		QDateTime m_timRealDepartTime;//实际发车时间
		int m_nDepartSignalCode = 0;//出站信号机
		QDateTime m_timAdjAgrDepartTime;//邻站同意发车时间
		QDateTime m_timAdjArrivalTime;//邻站到达时间
		int m_nDepartLimit = 0; //出发超限等级
		int m_nDepartRouteId = 0;	//发车进路ID
		QString m_strDepartLocomotive; //出发机车号
		QString m_strArrivalDriver; //到达司机
		int m_nDepartLenght = 0; //到达车长
		int m_nDepartTrainNum = 0; //到达车数
		int m_nDepartChenge = 0; //到达换长
		int m_nDepartWeight = 0; //到达总重


		bool m_bUpDown = false; //上下行车次(上行-true 下行-false)
		bool m_bDeleteFlag = false;//删除标志


		QString m_strNotes;//记事
		bool m_bPassenger = 0;//办理客运
		bool m_bAllowTrackNotMatch = 0;//允许股道与固定径路不同
		bool m_bAllowSignalNotMatch = 0;//允许出入口与固定进路不同
		bool m_bArmy = 0; //军运
		bool m_bImportant = 0; //重点

		bool m_bLJStatus = 0;//列检
		bool m_bJLStatus = 0;//交令
		bool m_bJPStatus = 0;//交票
		bool m_bLWStatus = 0;//列尾
		bool m_bJCStatus = 0;//机车
		bool m_bHJStatus = 0;//货检
		bool m_bCJStatus = 0;//乘降
		bool m_bSSStatus = 0;//上水
		bool m_bZGStatus = 0;//摘挂
		bool m_bHCStatus = 0;//换乘
		bool m_bZXStatus = 0;//装卸
		bool m_bXWStatus = 0;//吸污
		bool m_bDKStatus = 0;//道口
		bool m_bCHStatus = 0;//车号
		bool m_bZWStatus = 0;//站务
		bool m_bZKStatus = 0;//综控
		QString m_strTrainPosStatus;//列车位置信息（占线板）
		QString m_strProc;//计划下一个流程

	public:
		static void Init(StaTrain* pTrain, const QJsonObject& subObj);
	};
}