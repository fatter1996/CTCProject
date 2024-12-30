#pragma once

#include <QWidget>
#include <QEvent>
#include <QMap>
#include <QMap>
#include <QPushButton>
#include <QButtonGroup>
#include "CommonWidget/AuxiliaryMenuWnd.h"

namespace CTCWindows {
    //���ܰ�ť����
    enum class FunType : int {
        RouteBuild = 0x01,      //��·����
        GuideBtn,               //������ť
        SignalReopen,           //�ź��ؿ�
        TotalCancel,            //��ȡ��
        GuideClock,             //��������
        TotalPosition,          //�ܶ�λ
        TotalReverse,           //�ܷ�λ
        SingleLock,             //����
        SingleUnlock,           //����
        Blockade,               //����
        UnBlockade,             //���
        TotalRelieve,           //���˽�
        RegionRelieve,          //���ʽ�
        Lighting,               //���
        UnLighting,             //���
        RampUnlock,             //�µ�����
        PoorRoute,              //��·����
        IdleConfirm,            //ȷ�Ͽ���
        FunBtn = 0x20,          //���ܰ�ť
        CommandClear,           //�������
        CommandIssued,          //�����´�
        AuxiliaryMenu = 0x30,   //�����˵�
        MethodConvert = 0x40,   //��ʽת��
        StateChange,            //��ʽת��

        STrackBlock,            //����������
        XTrackBlock             //����������
    };

    enum class OperObjType : int {
        Defult = 0x00,
        Train,            //�г�
        Shunt,            //����
        Though,           //ͨ����ť
        Flexibility,      //��ͨ��ť
        Guide,            //������ť
                          
        Switch = 0x11,    //����
        Track,            //�ɵ�
        Signal,           //�źŵ�
                          
        Ascend = 0x21,    //�����ʺ�
        Descend,          //�����ʺ�
                          
        Blockage = 0x31,  //����
        Restoration,      //��ԭ
        Accident,         //�¹�
                          
        TotalAux = 0x41,  //�ܸ���
        PickUpAux,        //�ӳ�����
        DepartureAux,     //��������
        DirectionChange,  //�ķ�   
        TotalAuxUp,       //�ܸ�������
        Allow = 0x51,     //����ٶ�����
        Hump              //�շ�
    };

    namespace BaseWnd {
        //��վ���湦�ܰ�ť������
        class StaFunBtnToolBar : public QWidget
        {
            Q_OBJECT
        public:
            StaFunBtnToolBar(QWidget* parent = nullptr);
            ~StaFunBtnToolBar();

            virtual void InitAuxiliaryMenu(AuxiliaryMenuWnd* pAuxiliary) = 0;
            void ButtonClicked(FunType eSelectType);
            void timerEvent(QTimerEvent* event) override;

        public:
            static FunType getCurrFunType() { return m_SelectFunType; }
            static void setOperObjType(OperObjType eType) { m_nOperObjType = eType; }
            static OperObjType getOperObjType() { return m_nOperObjType; }

        public slots:
            void onButtonClicked(QAbstractButton* pButton);
            void onAuxiliaryMenuBtnClicked();
            void onMethodConvertBtnClicked();
            void onFunBtnStateReset();

        signals:
            void OrderClear(int);
            void OrderIssued();

        protected:
            QMap<QAbstractButton*, FunType> m_mapFunBtnType;
            QButtonGroup* m_pButtonGroup = nullptr;
            QPushButton* m_pCommandClearBtn = nullptr;
            QPushButton* m_pCommandIssuedBtn = nullptr;
            QPushButton* m_pRouteBuildBtn = nullptr;

        private:
            int m_nTimerID_500 = -1;

        protected:
            static FunType m_SelectFunType;
            static OperObjType m_nOperObjType;
        };
    }
}