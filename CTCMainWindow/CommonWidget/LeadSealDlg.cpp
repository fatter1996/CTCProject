#include "LeadSealDlg.h"
#include <QGraphicsDropShadowEffect>
#include <qmath.h>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QGridLayout>

#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	bool LeadSealDlg::m_bResult = false;
	LeadSealDlg::LeadSealDlg(QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		setAttribute(Qt::WA_DeleteOnClose);
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
		connect(ui.enterBtn, &QPushButton::clicked, [&]() {
			emit ui.confirmBtn->clicked();
		});
		connect(ui.cancelBtn, &QPushButton::clicked, [&]() {
			m_bResult = false;
			this->close();
		});
	}

	LeadSealDlg::~LeadSealDlg()
	{

	}

	void LeadSealDlg::Init(KeyInputType type, void* pAttrObject)
	{
		if (type == KeyInputType::LeadSeal) {
			QLabel* pTxetLabel = new QLabel(this);
			pTxetLabel->setFixedHeight(24);
			pTxetLabel->setText("请输入密码");
			ui.contentWidget->layout()->addWidget(pTxetLabel);
			QLineEdit* pEdit = new QLineEdit(this);
			pEdit->setFixedHeight(24);
			m_pCurrLineEdit = pEdit;
			ui.contentWidget->layout()->addWidget(pEdit);

			connect(ui.confirmBtn, &QPushButton::clicked, [&]() {
				m_bResult = (m_pCurrLineEdit->text() == PASSWORD);
				this->close();
			});
		}
		else if (type == KeyInputType::InputTrain) {
			ui.tiplabel->setText("请输入车次号");
			QLabel* pTxetLabel = new QLabel(this);
			pTxetLabel->setFixedHeight(24);
			pTxetLabel->setText("请输入车次号");
			ui.contentWidget->layout()->addWidget(pTxetLabel);
			QLineEdit* pEdit = new QLineEdit(this);
			pEdit->setFixedHeight(24);
			m_pCurrLineEdit = pEdit;
			ui.contentWidget->layout()->addWidget(pEdit);
			connect(ui.confirmBtn, &QPushButton::clicked, this, &LeadSealDlg::close);
		}
		else if (type == KeyInputType::InputShuntingTime) {
			ui.tiplabel->setText("调车作业钩时分");
			QBoxLayout* pLayout = qobject_cast<QBoxLayout*>(ui.contentWidget->layout());
			QLabel* pTxetLabel = new QLabel(this);
			pTxetLabel->setFixedHeight(24);
			pTxetLabel->setText("调车作业预计时长:");
			QLineEdit* pEdit = new QLineEdit(this);
			pEdit->setFixedHeight(24);
			m_pCurrLineEdit = pEdit;
			QHBoxLayout* LabelEditLayout = new QHBoxLayout();
			LabelEditLayout->addWidget(pTxetLabel);
			LabelEditLayout->addWidget(pEdit);
			pLayout->addLayout(LabelEditLayout);
		    pCheck = new QCheckBox(this);
			pCheck->setText("强制执行");
			pLayout->addWidget(pCheck, 0, Qt::AlignCenter);

			connect(ui.confirmBtn, &QPushButton::clicked, this, &LeadSealDlg::close);
		}
		else {
			Station::StaTrain* pTrain = static_cast<Station::StaTrain*>(pAttrObject);
			if (!pTrain) {
				return;
			}
	
			if (type == KeyInputType::AddTrain) {
				ui.tiplabel->setText("添加车次");
				InitAddTrain(pTrain);
			}

			if (type == KeyInputType::ConfirmTrain || type == KeyInputType::AmendTrain) {
				ui.tiplabel->setText("确认车次号");
				InitChangeTrainNum(pTrain);
				connect(ui.confirmBtn, &QPushButton::clicked, [=]() {
					if (pTrain->m_strTrainNum != m_pCurrLineEdit->text()) {
						
					}
					m_bResult = true;
					this->close();
				});
			}

			if (type == KeyInputType::ChangeTrain) {
				ui.tiplabel->setText("修改车次号");
				InitChangeTrainNum(pTrain);
				connect(ui.confirmBtn, &QPushButton::clicked, [=]() {
					m_bResult = Station::MainStation()->ChangeTrainNum(pTrain, m_pCurrLineEdit->text());
					this->close();
				});
			}
			if (type == KeyInputType::ChangeTrainAttr) {
				ui.tiplabel->setText("修改车次属性");
				InitChangeTrainAttr(pTrain);
			}
		}
	}

	void LeadSealDlg::InitAddTrain(Station::StaTrain* pTrain)
	{
		QGridLayout* pLayout = new QGridLayout();
		pLayout->setHorizontalSpacing(8);
		pLayout->setVerticalSpacing(8);
		pLayout->setMargin(0);

		QLabel* pTxetLabel1 = new QLabel(this);
		pTxetLabel1->setFixedSize(72, 24);
		pTxetLabel1->setText("车次号");
		pLayout->addWidget(pTxetLabel1, 0, 0);
		QLineEdit* pEdit1 = new QLineEdit(this);
		pEdit1->setFixedHeight(24);
		m_pCurrLineEdit = pEdit1;
		pLayout->addWidget(pEdit1, 0, 1);

		QLabel* pTxetLabel2 = new QLabel(this);
		pTxetLabel2->setFixedSize(72, 24);
		pTxetLabel2->setText("车  站");
		pLayout->addWidget(pTxetLabel2, 1, 0);
		QLineEdit* pEdit2 = new QLineEdit(this);
		pEdit2->setFixedHeight(24);
		pEdit2->setText(Station::MainStation()->getStationName());
		pEdit2->setEnabled(false);
		pLayout->addWidget(pEdit2, 1, 1);

		QLabel* pTxetLabel3 = new QLabel(this);
		pTxetLabel3->setFixedSize(72, 24);
		pTxetLabel3->setText("股  道");
		pLayout->addWidget(pTxetLabel3, 2, 0);
		QLineEdit* pEdit3 = new QLineEdit(this);
		pEdit3->setFixedHeight(24);
		pEdit3->setText(Station::MainStation()->getDeviceByCode(pTrain->m_nPosCode)->getName());
		pEdit3->setEnabled(false);
		pLayout->addWidget(pEdit3, 2, 1);

		static_cast<QVBoxLayout*>(ui.contentWidget->layout())->addLayout(pLayout);


		QHBoxLayout* pLayout2 = new QHBoxLayout(this);
		pLayout2->setSpacing(8);
		pLayout2->setMargin(0);

		QCheckBox* pCheckBox1 = new QCheckBox(this);
		pCheckBox1->setFixedHeight(24);
		pCheckBox1->setText("电力牵引");
		pLayout2->addWidget(pCheckBox1);
		QCheckBox* pCheckBox2 = new QCheckBox(this);
		pCheckBox2->setFixedHeight(24);
		pCheckBox2->setText("模拟行车");
		pLayout2->addWidget(pCheckBox2);
		static_cast<QVBoxLayout*>(ui.contentWidget->layout())->addLayout(pLayout2);
		setFixedHeight(360);

		connect(ui.confirmBtn, &QPushButton::clicked, [=]() {
			pTrain->m_strTrainNum = m_pCurrLineEdit->text();
			switch (pTrain->m_strTrainNum.at(0).toUpper().toLatin1())
			{
			case 'K':
			case 'T':
			case 'Z':
			case 'D':
			case 'G':
			case 'L':	pTrain->m_bFreightTrain = false; break;
			default:	pTrain->m_bFreightTrain = true;  break;
			}

			pTrain->m_bElectric = pCheckBox1->isChecked();
			pTrain->m_bRealTrain = pCheckBox2->isChecked();
			m_bResult = Station::MainStation()->AddNewTrain(pTrain);
			this->close();
		});
	}

	void LeadSealDlg::InitChangeTrainNum(Station::StaTrain* pTrain)
	{
		QGridLayout* pLayout = new QGridLayout();
		pLayout->setHorizontalSpacing(8);
		pLayout->setVerticalSpacing(8);
		pLayout->setMargin(0);

		QLabel* pTxetLabel1 = new QLabel(this);
		pTxetLabel1->setFixedSize(72, 24);
		pTxetLabel1->setText("旧车次号");
		pLayout->addWidget(pTxetLabel1, 0, 0);
		QLineEdit* pEdit1 = new QLineEdit(this);
		pEdit1->setFixedHeight(24);
		pEdit1->setText(pTrain->m_strTrainNum);
		pEdit1->setEnabled(false);
		pLayout->addWidget(pEdit1, 0, 1);

		QLabel* pTxetLabel2 = new QLabel(this);
		pTxetLabel2->setFixedSize(72, 24);
		pTxetLabel2->setText("新车次号");
		pLayout->addWidget(pTxetLabel2, 1, 0);
		QLineEdit* pEdit2 = new QLineEdit(this);
		pEdit2->setFixedHeight(24);
		m_pCurrLineEdit = pEdit2;
		pLayout->addWidget(pEdit2, 1, 1);

		static_cast<QVBoxLayout*>(ui.contentWidget->layout())->addLayout(pLayout);
	}

	void LeadSealDlg::InitChangeTrainAttr(Station::StaTrain* pTrain)
	{
		QGridLayout* pLayout = new QGridLayout();
		pLayout->setHorizontalSpacing(8);
		pLayout->setVerticalSpacing(8);
		pLayout->setMargin(0);

		QLabel* pTxetLabel1 = new QLabel(this);
		pTxetLabel1->setFixedSize(72, 24);
		pTxetLabel1->setText("车次号");
		pLayout->addWidget(pTxetLabel1, 0, 0);
		QLineEdit* pEdit1 = new QLineEdit(this);
		pEdit1->setFixedHeight(24);
		pEdit1->setEnabled(false);
		pEdit1->setText(pTrain->m_strTrainNum);
		pLayout->addWidget(pEdit1, 0, 1);

		QLabel* pTxetLabel2 = new QLabel(this);
		pTxetLabel2->setFixedSize(72, 24);
		pTxetLabel2->setText("机车速率");
		pLayout->addWidget(pTxetLabel2, 1, 0);
		QLineEdit* pEdit2 = new QLineEdit(this);
		pEdit2->setFixedHeight(24);
		m_pCurrLineEdit = pEdit1;
		pLayout->addWidget(pEdit2, 1, 1);

		QLabel* pTxetLabel3 = new QLabel(this);
		pTxetLabel3->setFixedSize(72, 24);
		pTxetLabel3->setText("机车号");
		pLayout->addWidget(pTxetLabel3, 2, 0);
		QLineEdit* pEdit3 = new QLineEdit(this);
		pEdit3->setFixedHeight(24);
		pLayout->addWidget(pEdit3, 2, 1);
		static_cast<QVBoxLayout*>(ui.contentWidget->layout())->addLayout(pLayout);

		QHBoxLayout* pLayout2 = new QHBoxLayout(this);
		pLayout2->setSpacing(8);
		pLayout2->setMargin(0);
		QCheckBox* pCheckBox1 = new QCheckBox(this);
		pCheckBox1->setFixedHeight(24);
		pCheckBox1->setText("电力牵引");
		pLayout2->addWidget(pCheckBox1);
		static_cast<QVBoxLayout*>(ui.contentWidget->layout())->addLayout(pLayout2);
		setFixedHeight(360);

		connect(ui.confirmBtn, &QPushButton::clicked, [=]() {
			m_bResult = Station::MainStation()->ChangeTrainAttr(pTrain, pEdit2->text().toInt(), pEdit3->text(), pCheckBox1->isChecked());
			this->close();
		});
	}

	bool LeadSealDlg::LeadSealPassword(KeyInputType type, void* pAttrObject)
	{
		LeadSealDlg* pLeadSeal = new LeadSealDlg;
		pLeadSeal->setAttribute(Qt::WA_TranslucentBackground);
		pLeadSeal->setWindowFlags(Qt::FramelessWindowHint);
		pLeadSeal->Init(type, pAttrObject);
		pLeadSeal->exec();
		return m_bResult;
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
			if (m_pCurrLineEdit) {
				QApplication::sendEvent(m_pCurrLineEdit, &keyPress);
				QApplication::sendEvent(m_pCurrLineEdit, &keyRelease);
			}
		}
	}

	void LeadSealDlg::onNumberButtonClicked()
	{
		QPushButton* pBtn = (QPushButton*)sender();
		QKeyEvent keyPress(QEvent::KeyPress, pBtn->text().toInt() + 48, Qt::NoModifier, pBtn->text());
		QKeyEvent keyRelease(QEvent::KeyRelease, pBtn->text().toInt() + 48, Qt::NoModifier, pBtn->text());
		if (m_pCurrLineEdit) {
			QApplication::sendEvent(m_pCurrLineEdit, &keyPress);
			QApplication::sendEvent(m_pCurrLineEdit, &keyRelease);
		}
	}

	void LeadSealDlg::onOtherButtonClicked()
	{
		QPushButton* pBtn = (QPushButton*)sender();
		if (pBtn->text().contains("Back\nSpace")) {
			QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
			QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
			if (m_pCurrLineEdit) {
				QApplication::sendEvent(m_pCurrLineEdit, &keyPress);
				QApplication::sendEvent(m_pCurrLineEdit, &keyRelease);
			}
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
	}

}
