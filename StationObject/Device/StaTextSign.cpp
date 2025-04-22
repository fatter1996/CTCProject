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
            
            QFontMetrics fm(font);
            int textWidth = fm.horizontalAdvance(m_strText);
            int textHeight = fm.height();
            connect(CTCWindows::MainWindow(), &CTCWindows::CTCMainWindow::ModifyContent, [=](QString Content, int TextColor, int BackColor, StaTextSign* pTextSign) {
                if (pTextSign == this) {
                    qDebug() << Content << TextColor << BackColor << m_colBackground << m_colText << m_strText;
                    if (Content != "") {
                        m_strText = Content;
                    }
                    QFont font;
                    QFontMetrics fm(font);
                    int textWidth = fm.horizontalAdvance(m_strText);
                    int textHeight = fm.height();
                    m_rcBackground = QRect(m_ptShowPos.x() - 4, m_ptShowPos.y() - 4, 8, 8);

                    m_rcBackground.setWidth(textWidth + 8);
                    m_rcBackground.setHeight(textHeight + 8);
                    switch (TextColor)
                    {
                    case 0:
                        m_colText = Qt::black;
                        break;
                    case 1:
                        m_colText = Qt::white;
                        break;
                    case 2:
                        m_colText = Qt::red;
                        break;
                    case 3:
                        m_colText = Qt::yellow;
                        break;
                    case 4:
                        m_colText = Qt::blue;
                        break;
                    case 5:
                        m_colText = Qt::green;
                        break;
                    default:
                        m_colText = Qt::black;
                        break;
                    }

                    switch (BackColor)
                    {
                    case 0:
                        m_colBackground = Qt::white;
                        break;
                    case 1:
                        m_colBackground = Qt::yellow;
                        break;
                    case 2:
                        m_colBackground = Qt::blue;
                        break;
                    case 3:
                        m_colBackground = Qt::green;
                        break;
                    case 4:
                        m_colBackground = Qt::red;
                        break;
                    case 5:
                        m_colBackground = Qt::black;
                        break;
                    default:
                        m_colBackground = Qt::white;
                        break;
                    }
                    Draw();
                }
                });
            // ¸ù¾Ý·¶Î§µ÷Õû×ÖÌå´óÐ¡
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
                emit Station::MainStation()->TextSignEdit(m_strText,this);
                });
            QAction* pAction2 = new QAction("É¾³ý");
            pMenu->addAction(pAction2);
            QObject::connect(pAction2, &QAction::triggered, [=]() {
                //m_rcBackground = QRect(0,0,0,0);
                
                emit Station::MainStation()->DeleteTextSign(this);
                
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