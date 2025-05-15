#include "DeviceTrain.h"
#include "Global.h"
#include "CommonWidget/LeadSealDlg.h"
#include <QMessageBox>

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        DeviceRoute::DeviceRoute()
        {
        
        }

        DeviceRoute::~DeviceRoute()
        {
        
        }

        void DeviceRoute::ShowRoutewMenu(QMenu* pMenu, QString strTrainNum, StaTrainRoute* pRoute)
        {
            if (!pMenu) {
                return;
            }

            QAction* pAction1 = new QAction(strTrainNum);
            pAction1->setEnabled(false);
            pMenu->addAction(pAction1);
            pMenu->addSeparator();

            QVector<StaTrainRoute*> vecArrivaRouteList;
            QVector<StaTrainRoute*> vecDepartRouteList;
            if (pRoute->m_bArrivaRoute) {
                vecArrivaRouteList = pRoute->getSubTrainRouteList();
                StaTrainRoute* pRelatedTrainRoute = pRoute->getRelatedTrainRoute();
                if (pRelatedTrainRoute) {
                    vecDepartRouteList = pRelatedTrainRoute->getSubTrainRouteList();
                }
            }
            else {
                vecDepartRouteList = pRoute->getSubTrainRouteList();
            }

            if (vecArrivaRouteList.size() > 0) {
                AddArrivaRouteTriggerAction(pMenu, pRoute, vecArrivaRouteList, vecDepartRouteList);
            }
            else {
                AddDepartRouteTriggerAction(pMenu, pRoute, vecDepartRouteList);
            }

            pMenu->addSeparator();
            AddTrackChangeAction(pMenu, pRoute);
        }

        void DeviceRoute::AddArrivaRouteTriggerAction(QMenu* pMenu, StaTrainRoute* pCurRoute, const QVector<StaTrainRoute*>& vecArrivaRouteList, const QVector<StaTrainRoute*>& vecDepartRouteList)
        {
            QString strDescrip = MainStation()->getTriggerString(vecArrivaRouteList);
            QAction* pAction1 = new QAction(QString("����ӳ���·(%1)").arg(strDescrip));
            pMenu->addAction(pAction1);
            QObject::connect(pAction1, &QAction::triggered, [=]() {
                MainStation()->TrainRouteTrigger(pCurRoute, vecArrivaRouteList, "�ӳ�");
            });

            strDescrip.append(";");
            strDescrip.append(MainStation()->getTriggerString(vecDepartRouteList));
            QAction* pAction2 = new QAction(QString("����ͨ����·(%1)").arg(strDescrip));
            pMenu->addAction(pAction2);
            QObject::connect(pAction2, &QAction::triggered, [=]() {
                QVector<StaTrainRoute*> vecTrainRoute;
                vecTrainRoute.append(vecArrivaRouteList);
                vecTrainRoute.append(vecDepartRouteList);
                MainStation()->TrainRouteTrigger(pCurRoute, vecTrainRoute, "ͨ��");
            });

            bool bAllAutoTouch = true;
            strDescrip.clear();
            strDescrip.append(getAutoTriggerString(vecArrivaRouteList, bAllAutoTouch));
            QAction* pAction3 = new QAction(QString("�ӳ��Դ�(%1)").arg(strDescrip));
            pAction3->setCheckable(true);
            pAction3->setChecked(bAllAutoTouch);
            pMenu->addAction(pAction3);
            QObject::connect(pAction3, &QAction::triggered, [=]() {
                MainStation()->TrainRouteTriggerChange(vecArrivaRouteList, true);
            });

            strDescrip.append(";");
            strDescrip.append(getAutoTriggerString(vecDepartRouteList, bAllAutoTouch));
            QAction* pAction4 = new QAction(QString("ͨ���Դ�(%1)").arg(strDescrip));
            pAction4->setCheckable(true);
            pAction4->setChecked(bAllAutoTouch);
            pMenu->addAction(pAction4);
            QObject::connect(pAction4, &QAction::triggered, [=]() {
                QVector<StaTrainRoute*> vecTrainRoute;
                vecTrainRoute.append(vecArrivaRouteList);
                vecTrainRoute.append(vecDepartRouteList);
                MainStation()->TrainRouteTriggerChange(vecTrainRoute, true);
            });
        }

        void DeviceRoute::AddDepartRouteTriggerAction(QMenu* pMenu, StaTrainRoute* pCurRoute, const QVector<StaTrainRoute*>& vecDepartRouteList)
        {
            QAction* pAction1 = new QAction(QString("��������·(%1)").arg(MainStation()->getTriggerString(vecDepartRouteList)));
            pMenu->addAction(pAction1);
            QObject::connect(pAction1, &QAction::triggered, [=]() {
                MainStation()->TrainRouteTrigger(pCurRoute, vecDepartRouteList, "����");
            });

            QString strDescrip;
            bool bAllAutoTouch = true;
            strDescrip.append(getAutoTriggerString(vecDepartRouteList, bAllAutoTouch));
            QAction* pAction2 = new QAction(QString("�����Դ�(%1)").arg(strDescrip));
            pAction2->setCheckable(true);
            pAction2->setChecked(bAllAutoTouch);
            pMenu->addAction(pAction2);
            QObject::connect(pAction2, &QAction::triggered, [=]() {
                MainStation()->TrainRouteTriggerChange(vecDepartRouteList, true);
            });
        }

        void DeviceRoute::AddTrackChangeAction(QMenu* pMenu, StaTrainRoute* pCurTrainRoute)
        {
            for (DeviceBase* pTrack : MainStation()->getDeviceVectorByType(TRACK)) {
                if (dynamic_cast<StaTrack*>(pTrack)->TrackType() == "ZX_GD" || dynamic_cast<StaTrack*>(pTrack)->TrackType() == "GD_QD") {
                    QAction* pAction = new QAction(pTrack->getName());
                    pAction->setCheckable(true);
                    pAction->setChecked(pCurTrainRoute->m_nTrackCode == pTrack->getCode());
                    pAction->setEnabled(pCurTrainRoute->m_nTrackCode != pTrack->getCode());
                    pMenu->addAction(pAction);
                    QObject::connect(pAction, &QAction::triggered, [=]() {
                        QVector<StaTrainRoute*> vecTempRouteOrder;
                        if (pCurTrainRoute->m_bArrivaRoute) {
                            for (StaTrainRoute* pRoute : MainStation()->getStaTrainRouteByTrain(pCurTrainRoute->m_nTrainId)) {
                                vecTempRouteOrder.append(pRoute->getSubTrainRouteList());
                            }
                        }
                        else {
                            vecTempRouteOrder = pCurTrainRoute->getSubTrainRouteList();
                        }
                        MainStation()->TrainRouteTrackChange(vecTempRouteOrder, pTrack);
                    });
                }
            }
        }

        QString DeviceRoute::getAutoTriggerString(const QVector<StaTrainRoute*>& vecRouteList, bool& bAllAutoTouch)
        {
            QString strDescrip;
            for (StaTrainRoute* pRoute : vecRouteList) {
                bAllAutoTouch &= pRoute->m_bAutoTouch;
                strDescrip.append(pRoute->m_strCurRouteDescrip);
                strDescrip.append(QString("[%1]").arg(pRoute->m_bAutoTouch ? "��" : "��"));
                if (pRoute != vecRouteList.back()) {
                    strDescrip.append(";");
                }
            }
            return strDescrip;
        }


        int DeviceTrain::m_nInTrainFrame = false;
        DeviceTrain::DeviceTrain()
        {

        }
        DeviceTrain::~DeviceTrain()
        {

        }

        void DeviceTrain::DrawTrainFrame(QPainter& pPainter)
        {
            pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            pPainter.setBrush(Qt::NoBrush);
            double nDiploid = m_bMainStation ?
                MainStation()->getDiploid(DiploidRatio::StaTrainNumDiploid) :
                MainStation()->getDiploid(DiploidRatio::MultiTrainNumDiploid);

            QRectF rcFrame;
            if (m_bContains || MainStation()->IsVisible(VisibleDev::trainNumWnd)) {
                rcFrame = QRectF(
                    m_rcTrainFrame.center().x() - m_rcTrainFrame.width() * nDiploid / 2,
                    m_rcTrainFrame.center().y() - m_rcTrainFrame.height() * nDiploid / 2,
                    m_rcTrainFrame.width() * nDiploid,
                    m_rcTrainFrame.height() * nDiploid);
                pPainter.drawRect(Scale(rcFrame));
            }
        }

        void DeviceTrain::DrawTrain(QPainter& pPainter)
        {
            if (m_bTrainLeave) {
                m_pTrain = nullptr;
                m_bTrainLeave = false;
            }
            if (m_pTrain) {
                if (m_pTrain->m_bDelete) {
                    delete m_pTrain;
                    m_pTrain = nullptr;
                    m_bTrainLeave = false;
                    return;
                }

                double nDiploid = 0;
                if (m_bMainStation) {
                    nDiploid = MainStation()->getDiploid(DiploidRatio::StaTrainNumDiploid);
                }
                else {
                    nDiploid = MainStation()->getDiploid(DiploidRatio::MultiTrainNumDiploid);
                }
                QRectF rcFrame;
                rcFrame = QRectF(
                    m_rcTrainFrame.center().x() - m_rcTrainFrame.width() * nDiploid / 2,
                    m_rcTrainFrame.center().y() - m_rcTrainFrame.height() * nDiploid / 2,
                    m_rcTrainFrame.width() * nDiploid,
                    m_rcTrainFrame.height() * nDiploid);
                DrawTrainFrame(pPainter, rcFrame);
                DrawTrainHead(pPainter, rcFrame);
                DrawTrainTail(pPainter, rcFrame);
            }
            else {
                m_pTrain = nullptr;
                m_bTrainLeave = false;
            }
        }

        void DeviceTrain::DrawTrainFrame(QPainter& pPainter, QRectF rcFrame)
        {
            pPainter.setPen(Qt::NoPen);
            if (m_pTrain->m_bElectric) {
                pPainter.setBrush(Qt::green);
            }
            else {
                pPainter.setBrush(Qt::white);
            }

            pPainter.drawRect(Scale(rcFrame));
            if (m_pTrain->m_bFreightTrain) {
                pPainter.setPen(Qt::blue);
            }
            else {
                pPainter.setPen(Qt::red);
            }
            double nDiploid = 0;
            if (m_bMainStation) {
                nDiploid = MainStation()->getDiploid(DiploidRatio::StaTrainNumDiploid);
            }
            else {
                nDiploid = MainStation()->getDiploid(DiploidRatio::MultiTrainNumDiploid);
            }
            QFont font = pPainter.font();
            font.setPointSizeF(16 * nDiploid);
            pPainter.setFont(font);
            // ���ݷ�Χ���������С
            int nTextWidth = 0;
            QRectF rcTextRect;
            QFontMetrics fm(font);
            nTextWidth = fm.horizontalAdvance(m_pTrain->m_strTrainNum);
            rcTextRect = Scale(rcFrame);
            while (nTextWidth > rcTextRect.width()) {
                font.setPointSizeF(font.pointSizeF() - 0.2);
                pPainter.setFont(font);
                fm = QFontMetrics(font);
                nTextWidth = fm.horizontalAdvance(m_pTrain->m_strTrainNum);
            }
            pPainter.drawText(Scale(rcFrame), m_pTrain->m_strTrainNum, QTextOption(Qt::AlignCenter));
        }

        void DeviceTrain::DrawTrainHead(QPainter& pPainter, QRectF rcFrame)
        {
            pPainter.setPen(Qt::NoPen);
            if (m_pTrain->m_bRunning) {
                QPolygonF triangle;
                double nDiploid = MainStation()->getDiploid(DiploidRatio::StaTrainNumDiploid);
                pPainter.setRenderHint(QPainter::Antialiasing, true);
                if (m_pTrain->m_bRight) {
                    triangle << Scale(rcFrame.topRight()) << Scale(rcFrame.bottomRight())
                        << Scale(QPointF(rcFrame.right() + 16 * nDiploid, rcFrame.top() + 16 * nDiploid));
                }
                else {
                    triangle << Scale(rcFrame.topLeft()) << Scale(rcFrame.bottomLeft())
                        << Scale(QPointF(rcFrame.left() - 16 * nDiploid, rcFrame.top() + 16 * nDiploid));
                }
                pPainter.drawPolygon(triangle);
                pPainter.setRenderHint(QPainter::Antialiasing, false);
            }
        }

        void DeviceTrain::DrawTrainTail(QPainter& pPainter, QRectF rcFrame)
        {
            QBrush brush;
            int nMinutes = 0;
            if (m_pTrain->m_nEarlyOrLateTime > 0) { //���
                nMinutes = m_pTrain->m_nEarlyOrLateTime / 60;
                brush.setColor(Qt::blue);
            }
            else if (m_pTrain->m_nEarlyOrLateTime > 0) {    //���
                nMinutes = -m_pTrain->m_nEarlyOrLateTime / 60;
                brush.setColor(Qt::red);
            }

            if (nMinutes > 0) {
                double nDiploid = MainStation()->getDiploid(DiploidRatio::StaTrainNumDiploid);
                QRectF rcLabel;
                if (m_pTrain->m_bRight) {
                    rcLabel = QRectF(rcFrame.left() - 40 * nDiploid, rcFrame.top(), 40 * nDiploid, rcFrame.height());
                }
                else {
                    rcLabel = QRectF(rcFrame.right() + 40 * nDiploid, rcFrame.top(), 40 * nDiploid, rcFrame.height());
                }
                pPainter.setPen(Qt::NoPen);
                pPainter.setBrush(brush);
                pPainter.drawRect(Scale(rcLabel));

                QString strTime;
                if (nMinutes < 99) {
                    strTime = QString::number(nMinutes).rightJustified(2, '0');
                }
                else {
                    strTime = QString("%1:%2")
                        .arg(QString::number(nMinutes / 60).rightJustified(2, '0'))
                        .arg(QString::number(nMinutes % 60).rightJustified(2, '0'));
                }
                pPainter.setPen(Qt::white);
                pPainter.drawText(Scale(rcLabel), strTime);
            }
        }

        void DeviceTrain::ShowTrainMenu(QMenu* pMenu, int nCode)
        {
            QAction* pAction1 = new QAction("�ӳ��κ�");
            pAction1->setEnabled(!m_pTrain);
            pMenu->addAction(pAction1);
            QObject::connect(pAction1, &QAction::triggered, [=]() {
                StaTrain* pTrain = new StaTrain;
                pTrain->m_nPosCode = nCode;
                if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::AddTrain, pTrain)) {
                    qDebug() << "������ӳɹ�";
                }
            });

            QAction* pAction2 = new QAction("ɾ�����κ�");
            pAction2->setEnabled(m_pTrain);
            pMenu->addAction(pAction2);
            QObject::connect(pAction2, &QAction::triggered, [=]() {
                QDialog* pDialog = new QDialog;
                pDialog->setWindowIcon(QIcon("icon/icon.ico"));
                pDialog->setWindowTitle("���κ�ɾ��");
                pDialog->setAttribute(Qt::WA_DeleteOnClose);
                pDialog->setFixedSize(200, 120);
                QVBoxLayout* pLayout = new QVBoxLayout(pDialog);
                pLayout->setMargin(8);
                pLayout->setSpacing(8);
                pDialog->setLayout(pLayout);

                QHBoxLayout* pLayout1 = new QHBoxLayout(pDialog);
                pLayout1->setMargin(8);
                pLayout1->setSpacing(8);
                QLabel* pLabel = new QLabel(pDialog);
                pLabel->setFixedSize(56, 24);
                pLabel->setText("���κ�");
                pLayout1->addWidget(pLabel);
                QLineEdit* pLineEdit = new QLineEdit(pDialog);
                pLineEdit->setFixedHeight(24);
                pLineEdit->setEnabled(false);
                pLineEdit->setText(m_pTrain->m_strTrainNum);
                QFont font = pLineEdit->font();
                font.setPointSize(11);
                pLineEdit->setFont(font);
                pLineEdit->setStyleSheet("QLineEdit:disabled { background-color: rgb(250, 250, 250); }");
                pLayout1->addWidget(pLineEdit);
                pLayout->addLayout(pLayout1);

                QHBoxLayout* pLayout2 = new QHBoxLayout(pDialog);
                pLayout2->setMargin(0);
                pLayout2->setSpacing(8);
                QPushButton* pPushButton1 = new QPushButton(pDialog);
                pPushButton1->setFixedSize(64, 24);
                pPushButton1->setText("ȷ��");
                QObject::connect(pPushButton1, &QPushButton::clicked, [=]() {
                    if (MainStation()->DeleteTrain(m_pTrain)) {
                        m_pTrain = nullptr;
                    }
                    pDialog->close();
                });
                pLayout2->addWidget(pPushButton1);
                QPushButton* pPushButton2 = new QPushButton(pDialog);
                pPushButton2->setFixedSize(64, 24);
                pPushButton2->setText("ȡ��");
                QObject::connect(pPushButton1, &QPushButton::clicked, [=]() {
                    pDialog->close();
                });
                pLayout2->addWidget(pPushButton2);
                pLayout->addLayout(pLayout2);
                pDialog->exec();
            });

            QAction* pAction3 = new QAction("���κ�ȷ��");
            pAction3->setEnabled(m_pTrain);
            pMenu->addAction(pAction3);
            QObject::connect(pAction3, &QAction::triggered, [=]() {
                CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::ConfirmTrain, m_pTrain);
            });

            QAction* pAction4 = new QAction("�������κ�");
            pAction4->setEnabled(m_pTrain);
            pMenu->addAction(pAction4);
            QObject::connect(pAction4, &QAction::triggered, [=]() {
                CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::AmendTrain, m_pTrain);
            });

            QAction* pAction5 = new QAction("������κ�");
            pAction5->setEnabled(m_pTrain);
            pMenu->addAction(pAction5);
            QObject::connect(pAction5, &QAction::triggered, [=]() {
                CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::ChangeTrain, m_pTrain);
            });

            QAction* pAction6 = new QAction("�ó�������");
            pAction6->setEnabled(m_pTrain);
            pMenu->addAction(pAction6);
            QObject::connect(pAction6, &QAction::triggered, [=]() {
                CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::ChangeTrainAttr, m_pTrain);
            });

            QAction* pAction7 = new QAction("���ó���ͣ��");
            pAction7->setEnabled(m_pTrain);
            pMenu->addAction(pAction7);
            QObject::connect(pAction7, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("ȷ��Ҫ���ó���%1ͣ��?").arg(m_pTrain->m_strTrainNum), "��", "��") == 0) {
                    MainStation()->SetTrainRunning(m_pTrain, false);
                }
            });

            QAction* pAction8 = new QAction("���ó�������");
            pAction8->setEnabled(m_pTrain);
            pMenu->addAction(pAction8);
            QObject::connect(pAction8, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("ȷ��Ҫ���ó���%1����?").arg(m_pTrain->m_strTrainNum), "��", "��") == 0) {
                    MainStation()->SetTrainRunning(m_pTrain, true);
                }
            });
        }

        void DeviceTrain::onMouseMoveToTrainFrame(const QPoint& ptPos)
        {
            if (!MainStation()->IsAllowStaOperation()) {
                return;
            }
            QRectF reMouse;  //����ж�������С����ʾ���� �Է����źŻ���ť�ж������غ�
            reMouse = { m_rcTrainFrame.x(), m_rcTrainFrame.y() + 10, m_rcTrainFrame.width(), m_rcTrainFrame.height() - 20 };
            if (Scale(m_rcTrainFrame).contains(ptPos)) {
                CTCWindows::MainWindow()->setMouseState(CTCWindows::MouseState::AddTrain);
                if (!m_bContains) {
                    m_nInTrainFrame++;
                }
                m_bContains = true;
            }
            else {
                if (m_bContains) {
                    m_nInTrainFrame--;
                }
                m_bContains = false;
            }
            if (m_nInTrainFrame <= 0) {
                CTCWindows::MainWindow()->setMouseState(CTCWindows::MouseState::Default);
            }
        }

        void DeviceTrain::MoveTo(DeviceTrain* pNext)
        {
            if (pNext && m_pTrain) {
                pNext->SetTrain(m_pTrain);
                m_bTrainLeave = true;
            }
        }


        StaRoutePreview::StaRoutePreview()
        {
            
        }

        StaRoutePreview::~StaRoutePreview()
        {
        
        }

        void StaRoutePreview::InitRoutePreviewAttributeMap(QString strType, QMap<QString, AttrMap>& mapAttribute)
        {
            mapAttribute[strType].insert("m_RoutePoint", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaRoutePreview*>(pDevice)->m_ptRouteWnd = QStringToQPointF(strElement); });
        }

        void StaRoutePreview::InitDeviceAttribute()
        {
            m_RoutePreviewData[0].rcTeainNum = { m_ptRouteWnd.x(), m_ptRouteWnd.y(), 120, 32 };
            m_RoutePreviewData[1].rcTeainNum = { m_ptRouteWnd.x(), m_ptRouteWnd.y() + 32, 120, 32 };
            m_RoutePreviewData[2].rcTeainNum = { m_ptRouteWnd.x(), m_ptRouteWnd.y() + 64, 120, 32 };
        }

        void StaRoutePreview::DrawRoutePreviewWnd(QPainter& pPainter, const QString& strDirection)
        {
            int nIndex = 0;
            QVector<StaTrainRoute*> vecTrainRoute;
            for (StaTrainRoute* pRoute : MainStation()->TrainRouteList()) {
                if (pRoute->m_strSignal == strDirection) {
                    vecTrainRoute.append(pRoute);
                }
            }
            StaTrainRoute* pRoute = nullptr;

            QFont font = pPainter.font();
            font.setPointSizeF(14);
            pPainter.setFont(font);
            QFontMetrics fm(font);
            int nTextWidth = 0;
            QRectF rcTextRect;
            QString strContent;
            
            pPainter.setBrush(Qt::NoBrush);

            while (nIndex < 3) {
                pPainter.setPen(Qt::white);
                pPainter.drawRect(Scale(m_RoutePreviewData[nIndex].rcTeainNum));

                if (vecTrainRoute.size() > nIndex) {
                    pRoute = vecTrainRoute[nIndex];

                    m_RoutePreviewData[nIndex].pTrainRoute = pRoute;
                    
                    m_RoutePreviewData[nIndex].strTrainNum = pRoute->getTrainNum();
                    if (pRoute->m_nRouteState == 2) {//�����ɹ�
                        pPainter.setPen(QPen(COLOR_BTN_GREEN_TG, 1));
                    }
                    else {
                        pPainter.setPen(QPen(pRoute->m_bAutoTouch ? COLOR_BTN_YELLOW : COLOR_LIGHT_RED, 1));
                    }

                    if (pRoute->m_bArrivaRoute) {
                        strContent = m_RoutePreviewData[nIndex].strTrainNum + "  J" + pRoute->m_strTrack;
                    }
                    else if (!pRoute->m_bArrivaRoute) {
                        strContent = m_RoutePreviewData[nIndex].strTrainNum + "  F" + pRoute->m_strTrack;
                    }
                    else {
                        strContent = m_RoutePreviewData[nIndex].strTrainNum + "  T" + pRoute->m_strTrack;
                    }

                    // ���ݷ�Χ���������С
                    nTextWidth = fm.horizontalAdvance(strContent);
                    rcTextRect = Scale(m_RoutePreviewData[nIndex].rcTeainNum);
                    if (m_RoutePreviewData[nIndex].rcTeainNum != QRectF()) {
                        while (nTextWidth > rcTextRect.width()) {
                            font.setPointSizeF(font.pointSizeF() - 0.2);
                            pPainter.setFont(font);
                            fm = QFontMetrics(font);
                            nTextWidth = fm.horizontalAdvance(strContent);
                        }
                    }
                    pPainter.drawText(Scale(m_RoutePreviewData[nIndex].rcTeainNum), strContent, QTextOption(Qt::AlignCenter));
                }
                nIndex++;
            }
        }

        bool StaRoutePreview::Contains(const QPoint& ptPos)
        {
            bool bContains = false;
            int nIndex = 0;
            while (nIndex < 3) {
                bContains |= Scale(m_RoutePreviewData[nIndex].rcTeainNum).contains(ptPos);
                nIndex++;
            }
            return bContains;
        }

        void StaRoutePreview::ShowRoutePreviewMenu(QMenu* pMenu, const QPoint& ptPos)
        {
            int nIndex = 0;
            while (nIndex < 3) {
                if (Scale(m_RoutePreviewData[nIndex].rcTeainNum).contains(ptPos)) {
                    if (m_RoutePreviewData[nIndex].pTrainRoute) {
                        ShowRoutewMenu(pMenu, m_RoutePreviewData[nIndex].strTrainNum, m_RoutePreviewData[nIndex].pTrainRoute);
                        return;
                    }
                }
                nIndex++;
            }
        }
    }
}