#pragma once

#include "BaseWndClass/CTCMainWindow.h"
#include "ui_StationViewKSK.h"
#include "./CTCMainWindow/StationViewKSK/ModuleWidget/TrainInformationInterfaceKSK.h"
#include "./CTCMainWindow/StationViewKSK/ModuleWidget/StaTraindiagramwidgetKSK.h"
#include "ModuleWidget/StaAddNewTrainKSK.h"
namespace CTCWindows {
	namespace CASCO {
		class StationViewKSK : public CTCMainWindow
		{
			Q_OBJECT
		
		public:
			StationViewKSK(QWidget *parent = nullptr);
			~StationViewKSK();

		public:
			//������˹��CTC
			static StationViewKSK* CreatStationView(QWidget* parent = nullptr);
			//������վ����
			BaseWnd::StationCtrlDisp* CreateStationCtrlDisp() override;
			//����վ��͸������
			BaseWnd::StationMultiDisp* CreateMultiStationDisp() override;
			//�����г���־����
			BaseWnd::StationLogDisp* CreateTrafficLogDisp() override;
			//������·���н���
			BaseWnd::StaRoutePlan* CreateStaRoutePlanWnd() override;
			//���������������
			BaseWnd::StaDispatchOrder* CreateStaDispatchOrder() override;
			//������ʾ���ý���
			BaseWnd::StaVisibleSet* CreateStaVisibleSet() override;
			// ��ʼ��������-�ײ��г���־������
			void InitbottomTrafficLogToolBar() override;

			BaseWnd::StaTraindiagramwidget* CreateStaTraindiagramwidget() override;
			//��ʼ�����˵�
			void InitStationViewMenuBar() override;
			//��ʼ��������-��������
			void InitStationViewToolBar() override;
			//��ʼ��������-�г���־������
			void InitTrafficLogToolBar() override;
			//��ʼ��������-ǩ�չ�����
			void InitSignForToolBar() override;
			//��ʼ��������-״̬������
			void InitStateToolBar() override;
			void InitStaTraindiagramwidget() override;
			QLayout* WidgetLayout() override { return ui.centralWidget->layout(); }
			void InitStatusBar() override ;
			StaAddNewTrainKSK* getInstance();

		private:
			StaTraindiagramwidgetKSK* m_pStaTraindiagram = nullptr;
			StaAddNewTrainKSK* m_pStaAddNewTrain = nullptr;
			TrainInformationInterfaceKSK*  m_pTrainFormation = nullptr;
			Ui::StationViewKSK ui;
		};
	}
}
