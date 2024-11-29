#pragma once

#include "BaseWndClass/CTCMainWindow.h"
#include "ui_StationViewTH.h"

namespace CTCWindows {
	namespace TH {
		class StationViewTH : public CTCMainWindow
		{
			Q_OBJECT

		public:
			StationViewTH(QWidget* parent = nullptr);
			~StationViewTH();
		public:
			//������˹��CTC
			static StationViewTH* CreatStationView(QWidget* parent = nullptr);

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
			Ui::StationViewTH ui;
		};
	}
}