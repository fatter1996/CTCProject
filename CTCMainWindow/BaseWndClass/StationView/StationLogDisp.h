#pragma once
#include <QWidget>
#include <QDateTime>
#include "CommonWidget/TrafficMsgLog.h"
#include "CustomControl/RichTableView.h"
#include "StationObject/GlobalStruct.h" 

namespace CTCWindows {

	enum class TrafficLogInfo
	{
		ArrivalTrainNum,		//到达车次号
		ArrivalTrainNumS,		//到达车次号
		ArrivalTrainNumX,		//到达车次号
		ArrivalTrackCode,		//到达股道编号
		ArrivalTrack,			//到达股道
		ArrivalSignalCode,		//进站信号机编号
		ArrivaSignal,			//进站信号机
		ProvArrivalTime,		//计划到站时间
		RealArrivalTime,		//实际到站时间
		ArrivaHeavyVehicle,		//到达-重车
		ArrivaEmpty,			//到达-空车
		ArrivaUonuse,			//到达-非运用车
		ArrivaCaboose,			//到达-守车
		ArrivaSecVoucher,		//到达-占用区间凭证
		AgrAdjDepartTime,		//同意邻站发车时间
		AdjDepartTime,			//邻站发车时间
		ArrivalLimit,			//到达超限等级
		ArrivalLocomotive,		//到达-机车号
		ArrivalDriver,			//到达-司机
		ArrivalTrainMaster,		//到达-车长
		ArrivalTrainValue,		//到达-车数
		ArrivalChange,			//到达-换长
		ArrivalWeight,			//到达-总重
		ArrivalAdmitBlock,		//到达-承认闭塞
		ArrivalReturn,			//到达-列车到达补机返回
		ArrivalCancelBlock,		//到达-取消闭塞
		ArrivalShunting,		//到达-出站跟踪调车
		ArrivalShuntingEnd,		//到达-出站跟踪调车完毕

		DepartTrainNum,			//出发车次号
		DepartTrainNumS,		//出发车次号
		DepartTrainNumX,		//出发车次号
		DepartTrackCode,		//发车股道编号
		DepartTrack,			//发车股道
		DepartSignalCode,		//出站信号机编号
		DepartSignal,			//出站信号机
		ProvDepartTime,			//计划发车时间
		RealDepartTime,			//实际发车时间
		DepartHeavyVehicle,		//出站-重车
		DepartEmpty,			//出站-空车
		DepartUonuse,			//出站-非运用车
		DepartCaboose,			//出站-守车
		DepartSecVoucher,		//到达-占用区间凭证
		AdjAgrDepartTime,		//邻站同意发车时间
		AdjArrivalTime,			//邻站到达时间
		DelayReason,			//晚点原因
		DepartLimit,			//出站超限等级
		DepartLocomotive,		//出站-机车号
		DepartDriver,			//出站-司机
		DepartTrainMaster,		//出站-车长
		DepartTrainValue,		//出站-车数
		DepartChange,			//出站-换长
		DepartWeight,			//出站-总重
		DepartAdmitBlock,		//出站-承认闭塞
		DepartReturn,			//出站-列车到达补机返回
		DepartCancelBlock,		//出站-取消闭塞
		DepartShunting,			//出站-出站跟踪调车
		DepartShuntingEnd,		//出站-出站跟踪调车完毕

		Notes,					//记事
		Passenger,				//办理客运
		LJStatus,				//列检
		JLStatus,				//交令
		JPStatus,				//交票
		LWStatus,				//列尾
		JCStatus,				//机车
		HJStatus,				//货检
		CJStatus,				//乘降
		SSStatus,				//上水
		ZGStatus,				//摘挂
		HCStatus,				//换乘
		ZXStatus,				//装卸
		XWStatus,				//吸污
		DKStatus,				//道口
		CHStatus,				//车号
		ZWStatus,				//站务
		ZKStatus,				//综控
		LimitOver,				//超限等级
		Army,					//军运
		Important,				//重点
		ShuntingTime,			//本务机车调车时分
		PlanAttributes			//计划属性
	};

    namespace BaseWnd {
        //行车日志界面基类
        class StationLogDisp : public QWidget
        {
            Q_OBJECT
        public:
            explicit StationLogDisp(QWidget* parent);
            ~StationLogDisp();

        public:
            void InitTrafficLogTable();
            virtual void InitTrafficLogTableHead() = 0;
            virtual void AddTrafficLogTable() = 0;

        public slots:
			virtual void OnTrafficLogTableUpData() = 0;
			virtual void ShowTableHead(bool bShow = true) {};
            //邻站预告
            virtual void AdjacentStationPreview(const QDateTime& tReportTime = QDateTime());
            //同意
            virtual void AgreeAdjacentStations(const QDateTime& tReportTime = QDateTime());
            //到达
            virtual void TrainArrival(const QDateTime& tReportTime = QDateTime());
            //出发
            virtual void TrainDeparture(const QDateTime& tReportTime = QDateTime());
            //通过
            virtual void TrainPassThrough(const QDateTime& tReportTime = QDateTime());
            //邻站
            virtual void AdjacentStation(const QDateTime& tReportTime = QDateTime());
            //取消接车
            virtual void CancelReceiving();
            //取消闭塞
            virtual void CancelBlock();
            //取消发车
            virtual void CancelDepart();
            //邻站出发
            virtual void AdjacentStationDepart(const QDateTime& tReportTime = QDateTime());
            //邻站到达
            virtual void AdjacentStationArrival(const QDateTime& tReportTime = QDateTime());
        protected:
            Control::RichTableView* pTrafficLogTable = nullptr;
            Station::StaTrafficLog* m_pCurTrafficLog = nullptr;
			TrafficMsgLog* m_pTrafficMsgLog = nullptr;

            QMap<TrafficLogInfo, int> m_mapLogICol;
			QMap<TrafficLogInfo, QString> m_mapPointReportType;
        };
    }
}