#include "LeadSealDlg.h"
#include <QGraphicsDropShadowEffect>
#include <qmath.h>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

namespace CTCWindows {
	bool LeadSealDlg::m_bCorrect = false;
	LeadSealDlg::LeadSealDlg(QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		ui.passwordWidget->installEventFilter(this);
		ui.keyboardWidget->installEventFilter(this);
		ui.tiplabel->installEventFilter(this);

		for(QPushButton * pBtn : ui.keyboardWidget->findChildren<QPushButton*>()) {
			pBtn->setAutoRepeat(true);    //允许自动重复
			pBtn->setAutoRepeatDelay(500);//设置重复操作的时延

			if (pBtn->text().length() == 1) {
				if ((pBtn->text() >= 'a' && pBtn->text() <= 'z') || (pBtn->text() >= 'A' && pBtn->text() <= 'Z')) {
					m_vecLetterKeys.append(pBtn);
					connect(pBtn, &QPushButton::clicked, this, &LeadSealDlg::onLetterButtonClicked);
				}
				else if (pBtn->text().toInt() >= 0 && pBtn->text().toInt() <= 9) {
					connect(pBtn, &QPushButton::clicked, this, &LeadSealDlg::onNumberButtonClicked);
				}
			}
			else {
				connect(pBtn, &QPushButton::clicked, this, &LeadSealDlg::onOtherButtonClicked);
			}
		}

		connect(ui.enterBtn, &QPushButton::clicked, this, &LeadSealDlg::onEnter);
		connect(ui.confirmBtn, &QPushButton::clicked, this, &LeadSealDlg::onEnter);
		connect(ui.cancelBtn, &QPushButton::clicked, [&]() {
			m_bCorrect = false;
			this->close();
		});
	}

	LeadSealDlg::~LeadSealDlg()
	{

	}

	bool LeadSealDlg::LeadSealPassword()
	{
		LeadSealDlg* pLeadSeal = new LeadSealDlg;
		pLeadSeal->setAttribute(Qt::WA_TranslucentBackground);
		pLeadSeal->setWindowFlags(Qt::FramelessWindowHint);
		pLeadSeal->exec();

		return m_bCorrect;
	}

	bool LeadSealDlg::eventFilter(QObject* obj, QEvent* event)
	{
		if (event->type() == QEvent::Paint) {
			QWidget* pWidget = nullptr;
			if (obj == static_cast<QObject*>(ui.passwordWidget)) {
				pWidget = ui.passwordWidget;
			}
			else if (obj == static_cast<QObject*>(ui.keyboardWidget)) {
				pWidget = ui.keyboardWidget;
			}
			if (pWidget) {
				QColor color(Qt::gray);
				QPainterPath path;
				path.setFillRule(Qt::WindingFill);
				
				path.addRoundedRect(QRect(4, 4, pWidget->width() - 8, pWidget->height() - 8), 4, 4);
				qDebug() << path;
				QPainter painter(pWidget);
				painter.setRenderHint(QPainter::Antialiasing, true);
				painter.fillPath(path, QBrush(QColor("#FAFAFA")));

				for (int i = 0; i < 4; i++) {
					QPainterPath path;
					path.setFillRule(Qt::WindingFill);
					path.addRoundedRect(QRect(4 - i, 4 - i, pWidget->width() - (4 - i) * 2, pWidget->height() - (4 - i) * 2), qreal(4) + i, qreal(4) + i);
					color.setAlpha(80 - qSqrt(i) * 40);
					painter.setPen(color);
					painter.drawPath(path);
				}
			}
		}

		if (obj == static_cast<QObject*>(ui.tiplabel)) {
			if (event->type() == QEvent::MouseButtonPress) {
				m_ptDragPos = dynamic_cast<QMouseEvent*>(event)->pos();
				m_bDragging = true;
			}
			else if (event->type() == QEvent::MouseButtonRelease) {
				m_ptDragPos = dynamic_cast<QMouseEvent*>(event)->pos();
				m_bDragging = false;
			}
			else if (event->type() == QEvent::MouseMove) {
				QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(event);
				if (m_bDragging && (pMouseEvent->buttons() & Qt::LeftButton)) {
					// 计算窗口的新位置
					move(x() + pMouseEvent->x() - m_ptDragPos.x(), y() + pMouseEvent->y() - m_ptDragPos.y());
				}
			}
		}
		
		return QDialog::eventFilter(obj, event);
	}

	void LeadSealDlg::onLetterButtonClicked()
	{
		QPushButton* pBtn = (QPushButton*)sender();
		if (pBtn->text() >= 'a' && pBtn->text() <= 'z') {
			QKeyEvent keyPress(QEvent::KeyPress, int(pBtn->text().at(0).toLatin1()) - 32, Qt::NoModifier, pBtn->text());
			QKeyEvent keyRelease(QEvent::KeyRelease, int(pBtn->text().at(0).toLatin1()) - 32, Qt::NoModifier, pBtn->text());
			QApplication::sendEvent(ui.passwordWidget->focusWidget(), &keyPress);
			QApplication::sendEvent(ui.passwordWidget->focusWidget(), &keyRelease);
		}
		else if (pBtn->text() >= 'A' && pBtn->text() <= 'Z') {
			QKeyEvent keyPress(QEvent::KeyPress, int(pBtn->text().at(0).toLatin1()), Qt::NoModifier, pBtn->text());
			QKeyEvent keyRelease(QEvent::KeyRelease, int(pBtn->text().at(0).toLatin1()), Qt::NoModifier, pBtn->text());
			QApplication::sendEvent(ui.lineEdit, &keyPress);
			QApplication::sendEvent(ui.lineEdit, &keyRelease);
		}
	}

	void LeadSealDlg::onNumberButtonClicked()
	{
		QPushButton* pBtn = (QPushButton*)sender();
		QKeyEvent keyPress(QEvent::KeyPress, pBtn->text().toInt() + 48, Qt::NoModifier, pBtn->text());
		QKeyEvent keyRelease(QEvent::KeyRelease, pBtn->text().toInt() + 48, Qt::NoModifier, pBtn->text());
		QApplication::sendEvent(ui.lineEdit, &keyPress);
		QApplication::sendEvent(ui.lineEdit, &keyRelease);
	}

	void LeadSealDlg::onOtherButtonClicked()
	{
		QPushButton* pBtn = (QPushButton*)sender();
		if (pBtn->text().contains("Back\nSpace")) {
			QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
			QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
			QApplication::sendEvent(ui.lineEdit, &keyPress);
			QApplication::sendEvent(ui.lineEdit, &keyRelease);
		}
		else if (pBtn->text().contains("Caps")) {
			if (m_bCapsLock) {
				for (QPushButton* pBtnKey : m_vecLetterKeys) {
					pBtnKey->setText(pBtnKey->text().toUpper());
				}
			}
			else {
				for (QPushButton* pBtnKey : m_vecLetterKeys) {
					pBtnKey->setText(pBtnKey->text().toLower());
				}
			}
			m_bCapsLock = !m_bCapsLock;
		}
		else if (pBtn->text().contains("Enter")) {
			onEnter();
		}
	}

	void LeadSealDlg::onEnter()
	{
		m_bCorrect = ui.lineEdit->text() == PASSWORD;
		this->close();
	}
}
