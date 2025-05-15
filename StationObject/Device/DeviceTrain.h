#pragma once
#include <QPainter>
#include "DeviceBase.h"
#include "StationObject/GlobalStruct.h"

namespace Station {
    namespace Device {
        class DeviceRoute {

        public:
            DeviceRoute();
            ~DeviceRoute();

        protected:
            void ShowRoutewMenu(QMenu* pMenu, QString strTrainNum, StaTrainRoute* pRoute);
            void AddArrivaRouteTriggerAction(QMenu* pMenu, StaTrainRoute* pCurRoute, const QVector<StaTrainRoute*>& vecArrivaRouteList, const QVector<StaTrainRoute*>& vecDepartRouteList);
            void AddDepartRouteTriggerAction(QMenu* pMenu, StaTrainRoute* pCurRoute, const QVector<StaTrainRoute*>& vecDepartRouteList);
            void AddTrackChangeAction(QMenu* pMenu, StaTrainRoute* pCurTrainRoute);
            QString getAutoTriggerString(const QVector<StaTrainRoute*>& vecRouteList, bool& bAllAutoTouch);
        };

        class DeviceTrain : virtual public DeviceScale, public DeviceRoute {

        public:
            DeviceTrain();
            ~DeviceTrain();

        public:
            //设置车次
            void SetTrain(StaTrain* pTrain) { m_pTrain = pTrain; }
            //车次移动
            void MoveTo(DeviceTrain* pNext);

        protected:
            //绘制车次窗
            void DrawTrainFrame(QPainter& pPainter);
            //绘制车次
            void DrawTrain(QPainter& pPainter);
            void DrawTrainFrame(QPainter& pPainter, QRectF rcFrame);
            void DrawTrainHead(QPainter& pPainter, QRectF rcFrame);
            void DrawTrainTail(QPainter& pPainter, QRectF rcFrame);
            void ShowTrainMenu(QMenu* pMenu, int nCode);
            void onMouseMoveToTrainFrame(const QPoint& ptPos);

        protected:
            QRectF m_rcTrainFrame;
            bool m_bContains = false;
            StaTrain* m_pTrain = nullptr;
            bool m_bTrainLeave = false;

        private:
            static int m_nInTrainFrame;
        };


        class StaRoutePreview : virtual public DeviceScale, public DeviceRoute {
        
        public:
            StaRoutePreview();
            ~StaRoutePreview();

        public:
            struct RoutePreviewData {
                QString strTrainNum;
                QRectF rcTeainNum;
                StaTrainRoute* pTrainRoute = nullptr;
            };

        protected:
            void InitRoutePreviewAttributeMap(QString strType, QMap<QString, AttrMap>& mapAttribute);
            void InitDeviceAttribute();
            void DrawRoutePreviewWnd(QPainter& pPainter, const QString& strDirection);
            bool Contains(const QPoint& ptPos);
            void ShowRoutePreviewMenu(QMenu* pMenu, const QPoint& ptPos);

        protected:
            QPointF m_ptRouteWnd;
            RoutePreviewData m_RoutePreviewData[3];
        };
    }
}