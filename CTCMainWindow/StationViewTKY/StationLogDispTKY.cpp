#include "StationLogDispTKY.h"
#include "../CustomControl/RichTableView.h"

#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CARS {
		StationLogDispTKY::StationLogDispTKY(QWidget* parent)
			: StationLogDisp(parent)
		{
			ui.setupUi(this);
			connect(ui.pushButton, &QPushButton::clicked, [=]() {

			});
		}

		StationLogDispTKY::~StationLogDispTKY()
		{
		}

		void StationLogDispTKY::InitTrafficLogTableHead()
		{
			QVector<Control::TableHeadInfo> vecTableHeadInfo = {
				{ 0, 0, 13, 1, "到达"},
					{ 1, 0, 1, 4, "列车车次"},
					{ 1, 1, 1, 4, "到达股道"},
					{ 1, 2, 5, 1, "时分"},
						{ 2, 2, 1, 3, "同意邻站发车"},
						{ 2, 3, 1, 3, "邻站发车"},
						{ 2, 4, 1, 1, "本站到达"},
							{ 3, 4, 1, 2, "规定"},
							{ 3, 5, 1, 2, "实际"},
						{ 2, 6, 1, 3, "机车入库"},
					{ 1, 7, 1, 4, "机车型号"},
					{ 1, 8, 2, 1, "乘务员姓名"},
						{ 2, 8, 1, 3, "司机"},
						{ 2, 9, 1, 3, "车长"},
					{ 1, 10, 3, 1, "列车编组"},
						{ 2, 10, 1, 3, "车数"},
						{ 2, 11, 1, 3, "换长"},
						{ 2, 12, 1, 3, "总重"},

				{ 0, 13, 14, 1, "出发"},
					{ 1, 13, 1, 4, "列车车次"},
					{ 1, 14, 1, 4, "出发股道"},
					{ 1, 15, 5, 1, "时分"},
						{ 2, 15, 1, 3, "机车出库"},
						{ 2, 16, 1, 3, "邻站同意发车"},
						{ 2, 17, 1, 1, "本站出发"},
							{ 3, 17, 1, 2, "规定"},
							{ 3, 18, 1, 2, "实际"},
						{ 2, 19, 1, 3, "邻站到达"},
					{ 1, 20, 1, 4, "机车型号"},
					{ 1, 21, 2, 1, "乘务员姓名"},
						{ 2, 21, 1, 3, "司机"},
						{ 2, 22, 1, 3, "车长"},
					{ 1, 23, 3, 1, "列车编组"},
						{ 2, 23, 1, 3, "车数"},
						{ 2, 24, 1, 3, "换长"},
						{ 2, 25, 1, 3, "总重"},
					{ 1, 26, 1, 4, "列车出发晚点原因"},
				{ 0, 27, 1, 5, "列车属性"},
				{ 0, 28, 1, 5, "记事"},
			};
			pTrafficLogTable->TableHeadInit(HHEAD, 5, 29, vecTableHeadInfo);
		}

		void StationLogDispTKY::AddTrafficLogTable()
		{
			ui.frame->layout()->addWidget(pTrafficLogTable);
		}
	}
}
