#include "StaTextSign.h"
#include <QMenu>
#include <QMouseEvent>
#include "Global.h"
#pragma execution_character_set("utf-8")
namespace Station {
    namespace Device {

        StaTextSign::StaTextSign(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_bShowName = false;
        }

        StaTextSign::~StaTextSign()
        {

        }

        void StaTextSign::InitAttributeMap()
        {

           
        }

        void StaTextSign::Draw(bool isMulti)
        {
            DrawTextSign();
            return DeviceBase::Draw(isMulti);
        }

        void StaTextSign::InitTextSign(const QString& strText, const QPoint& ptPos, const QColor& colFont, const QColor& colBackground, int nSize)
        {
            m_strText = strText;
            m_ptShowPos = ptPos;
            m_colText = colFont;
            m_colBackground = colBackground;
            m_nFontSize = nSize;
            m_rcBackground = QRect(m_ptShowPos.x() - 4, m_ptShowPos.y() - 4, 8, 8);

            QFont font;
            font.setFamily("Î¢ÈíÑÅºÚ");
            font.setPixelSize(Scale(m_nFontSize));//×ÖºÅ

            // ¸ù¾Ý·¶Î§µ÷Õû×ÖÌå´óÐ¡
            QFontMetrics fm(font);
            int textWidth = fm.horizontalAdvance(m_strText);
            int textHeight = fm.height();
            if (m_rcTextRect != QRectF()) {
                while (textWidth > Scale(m_rcTextRect).width() || textHeight > Scale(m_rcTextRect).height()) {
                    font.setPixelSize(font.pixelSize() - 1);
                    m_pPainter.setFont(font);
                    fm = QFontMetrics(font);
                    textWidth = fm.horizontalAdvance(m_strText);
                    textHeight = fm.height();
                }
            }
            m_rcBackground.setWidth(textWidth + 8);
            m_rcBackground.setHeight(textHeight + 8);
        }

        bool StaTextSign::IsMouseWheel(const QPoint& ptPos)
        {
            return false;
        }

        void StaTextSign::ShowDeviceMenu(const QPoint& ptPos)
        {
            QMenu* pMenu = new QMenu();
            pMenu->setAttribute(Qt::WA_DeleteOnClose);
            QAction* pAction1 = new QAction("±à¼­");
            pMenu->addAction(pAction1);
            QObject::connect(pAction1, &QAction::triggered, [=]() {
                CTCWindows::MainWindow()->ShowEditingInterface(this);
            });
            QAction* pAction2 = new QAction("É¾³ý");
            pMenu->addAction(pAction2);
            QObject::connect(pAction2, &QAction::triggered, [=]() {
                MainStation()->DeleteTextSign(this);
            });
            pMenu->exec(QCursor::pos());
        }

        bool StaTextSign::Contains(const QPoint& ptPos)
        {
            return Scale(m_rcTextRect).contains(ptPos) || Scale(m_rcBackground).contains(ptPos);
        }

        void StaTextSign::DrawTextSign()
        {
            m_pPainter.setPen(m_colBackground);
            m_pPainter.setBrush(m_colBackground);
            m_pPainter.drawRect(m_rcBackground);

            QFont font;
            font.setFamily("Î¢ÈíÑÅºÚ"); 
            font.setPixelSize(Scale(m_nFontSize));//×ÖºÅ
            m_pPainter.setFont(font);//ÉèÖÃ×ÖÌå
            m_pPainter.setPen(m_colText);
            m_pPainter.setBrush(Qt::NoBrush);
            m_pPainter.drawText(m_rcBackground, m_strText, QTextOption(Qt::AlignCenter));
        }
    }
}