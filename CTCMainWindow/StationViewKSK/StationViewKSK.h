#pragma once

#include "BaseWndClass/CTCMainWindow.h"
#include "ui_StationViewKSK.h"

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
			// ��ʼ��������-�ײ��г���־������
			void InitbottomTrafficLogToolBar() override;
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
			QLayout* WidgetLayout() override { return ui.centralWidget->layout(); }
		
		private:
			Ui::StationViewKSK ui;
		};
	}
}
