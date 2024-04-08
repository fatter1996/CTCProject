#pragma once
#include <QByteArray>

namespace Station {
    namespace Protocol {
        class StaStateProtocol {

        public:
            StaStateProtocol();
            ~StaStateProtocol();

        public:
            void UnpackStaViewState(QByteArray& dataAyyay);
        };
    }
}