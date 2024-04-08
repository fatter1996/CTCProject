#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //ÍÑ¹ìÆ÷
        class StaText : public DeviceBase
        {
        public:
            explicit StaText(QObject* parent = nullptr);
            ~StaText();

        private:
            //Õ¾³¡»æÖÆ
            void Draw(const bool& bElapsed, const bool& isMulti = false);

            //Õ¾³¡·­×ª
            void setVollover(const QPoint& ptBase);
            //×´Ì¬ÖØÖÃ
            void ResetDevState();

        private:
            bool m_bIsTitle = false;
        };
    }
}