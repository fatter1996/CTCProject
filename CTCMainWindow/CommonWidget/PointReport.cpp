#include "PointReport.h"
#include "Global.h"

namespace CTCWindows {
	PointReport::PointReport(QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		connect(ui.newInfoBtn, &QPushButton::clicked, this, &PointReport::AddNewTraffic);
	}

	PointReport::~PointReport()
	{
	}

	QDateTime PointReport::getDateTimeByTime(const QString& strTime)
	{
		QTime time = QTime::fromString(strTime, "hh:mm");
		if (!time.isValid()) {
			time = QTime::currentTime();
		}
		return QDateTime(QDate::currentDate(), time);
	}

	void PointReport::AddNewTraffic()
	{
		Station::StaTrafficLog* pTrafficLog = new Station::StaTrafficLog;
		pTrafficLog->m_strArrivalTrainNum = ui.arrivalTrainNumEdit->text();
		pTrafficLog->m_strArrivalTrack = ui.arrivalTrackCombo->currentData().toString();
		pTrafficLog->m_strArrivalSignal = ui.arrivalSignalCombo->currentData().toString();
		pTrafficLog->m_tProvArrivalTime = getDateTimeByTime(ui.provArrivalTimeEdit->text());
		//pTrafficLog->m_tRealArrivalTime = getDateTimeByTime(ui.realArrivalTimeEdit->text());
		//pTrafficLog->m_tAgrAdjDepartTime = getDateTimeByTime(ui.agrAdjDepartEdit->text());
		//pTrafficLog->m_tAdjDepartTime = getDateTimeByTime(ui.adjDepartEdit->text());
		pTrafficLog->m_strArrivalLocomotive = ui.arrivalLocomotiveEdit->text();
		pTrafficLog->m_strArrivalDriver = ui.arrivalDriverComco->currentData().toString();
		pTrafficLog->m_nArrivalTrainMaster = ui.arrivalTrainMasterEdit->text();
		pTrafficLog->m_nArrivalTrainValue = ui.arrivalTrainValueEdit->text().toInt();
		pTrafficLog->m_nArrivalChange = ui.arrivalChangeEdit->text().toInt();
		pTrafficLog->m_nArrivalWeight = ui.arrivalWeightEdit->text().toInt();

		pTrafficLog->m_strDepartTrainNum = ui.departTrainNumEdit->text();
		pTrafficLog->m_strDepartTrack = ui.departTrackCombo->currentData().toString();
		pTrafficLog->m_strDepartSignal = ui.departSignalCombo->currentData().toString();
		pTrafficLog->m_tProvDepartTime = getDateTimeByTime(ui.provDepartTimeEdit->text());
		//pTrafficLog->m_tRealDepartTime = getDateTimeByTime(ui.realDepartTimeEdit->text());
		//pTrafficLog->m_tAdjAgrDepartTime = getDateTimeByTime(ui.adjAgrDepartEdit->text());
		//pTrafficLog->m_tAdjArrivalTime = getDateTimeByTime(ui.adjArrivalEdit->text());
		pTrafficLog->m_strDepartLocomotive = ui.departLocomotiveEdit->text();
		pTrafficLog->m_strDepartDriver = ui.departDriverComco->currentData().toString();
		pTrafficLog->m_nDepartTrainMaster = ui.departTrainMasterEdit->text();
		pTrafficLog->m_nDepartTrainValue = ui.departTrainValueEdit->text().toInt();
		pTrafficLog->m_nDepartChange = ui.departChangeEdit->text().toInt();
		pTrafficLog->m_nDepartWeight = ui.departWeightEdit->text().toInt();

		pTrafficLog->m_strDelayReason = ui.delayReasonEdit->text();
		pTrafficLog->m_strNotes = ui.notesEdit->text();
		if (pTrafficLog->m_tProvArrivalTime.isNull() && pTrafficLog->m_tProvDepartTime.isNull()) {
			return;
		}
		if (pTrafficLog->m_strArrivalTrainNum == "" && pTrafficLog->m_strDepartTrainNum == "") {
			return;
		}
		else if (pTrafficLog->m_strArrivalTrainNum == "" && pTrafficLog->m_strDepartTrainNum != "") {
			pTrafficLog->m_nPlanType = PLAN_TYPE_FINAL;
		}
		else if (pTrafficLog->m_strArrivalTrainNum != "" && pTrafficLog->m_strDepartTrainNum == "") {
			pTrafficLog->m_nPlanType = PLAN_TYPE_START;
		}
		else if (pTrafficLog->m_strArrivalTrainNum != "" && pTrafficLog->m_strDepartTrainNum != "") {
			if (pTrafficLog->m_tProvArrivalTime == pTrafficLog->m_tProvDepartTime) {
				pTrafficLog->m_nPlanType = PLAN_TYPE_THROUGH;
			}
			else {
				pTrafficLog->m_nPlanType = PLAN_TYPE_ARRIVED;
			}
		}
		pTrafficLog->m_nTrainId = Station::MainStation()->AddNewTrain(new Station::StaTrain(pTrafficLog));
		if (Station::MainStation()->AddNewTrafficLog(pTrafficLog)) {
			Station::MainStation()->CreatTrainRouteByTrafficLog(pTrafficLog);
		}
	}
}
