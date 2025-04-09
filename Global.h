#pragma once
#include "StationObject/StationObject.h"
#include "BaseWndClass/CTCMainWindow.h"
#include "SocketObject/HttpClient.h"

#define MSGBOX_TITTLE	"CTC-STPC"
//设备类型
#define ALLDEVICE		"Device"		//全部设备
#define SWITCH			"DC"			//道岔
#define DERAILER		"TGQ"			//脱轨器
#define SIGNALLAMP		"XHD"			//信号机
#define SIGNALLAMP2  	"XH"			//信号机
#define TRACK			"GD"			//股道
#define SEMIAUTOBLOCK	"BS"			//半自动闭塞
#define LOCOMOTIVE		"Jwd"			//机务段
#define CONNECTION		"CL"			//场联
#define BUTTON			"BuRect"		//按钮
#define AUTOBLOCK		"ZDBS"			//自动闭塞 
#define SCATTERED		"LS"			//零散表示灯
#define HUMP			"TF"			//驼峰
#define BRAIDLINE		"BFX"			//编发线
#define ENDLINE			"JTX"			//尽头线
#define TEXT			"Text"			//文字
#define TEXT2			"TX"			//文字
#define PERMILLSIX		"PermillSix"	//千分之六标识
#define SECTION			"QD"			//区段 
#define LAMP			"Lamp"			//指示灯
#define NEUTRAL			"Neutral"		//分相区
#define MARK			"Mark"			//标志
#define TEXTSIGN		"TextSign"		//文字标注

//区段状态
#define SECTION_STATE_TAKEUP     0x01U	//占用
#define SECTION_STATE_BLOCK      0x02U	//封锁
#define SECTION_STATE_PRELOCK    0x04U	//预锁闭
#define SECTION_STATE_LOCK       0x08U	//锁闭
#define SECTION_STATE_FAULTLOCK  0x0CU	//故障锁闭

//道岔折点
#define SWITCH_KINK_CQ		0x08U		//岔前
#define SWITCH_KINK_DW		0x10U		//定位
#define SWITCH_KINK_FW		0x20U		//反位
//道岔位置
#define SWITCH_DRAW_DW		0x01U		//定位
#define SWITCH_DRAW_FW		0x02U		//反位
#define SWITCH_DRAW_CQ		0x04U		//岔前
//道岔状态
#define SWITCH_STATE_DW		0x01U		//定位
#define SWITCH_STATE_FW		0x02U		//反位
#define SWITCH_STATE_SK		0x03U		//四开
#define SWITCH_STATE_LOCK	0x04U		//锁闭
#define SWITCH_STATE_BLOCK	0x08U		//封锁

//信号机属性定义
#define SIGNAL_DCZD    0x00000001UL		//可做调车终端
#define SIGNAL_DCSD    0x00000002UL		//可做调车始端
#define SIGNAL_LCZD    0x00000004UL		//可做列车终端
#define SIGNAL_LCSD    0x00000008UL		//可做列车始端
#define SIGNAL_DCFSXH  0x00000010UL		//是调车复示信号机
#define SIGNAL_JZFSXH  0x00000020UL		//是进站复示信号机
#define SIGNAL_BGAN    0x00000040UL		//可做变更按钮
#define SIGNAL_YGXH    0x00000080UL		//是预告信号机
#define SIGNAL_JTXH    0x00000100UL		//尽头线信号机
#define SIGNAL_BZXH    0x00000200UL		//并置信号机
#define SIGNAL_CZXH    0x00000400UL		//差置信号机
#define SIGNAL_DZXH    0x00000800UL		//单置信号机
#define SIGNAL_JCXH    0x00001000UL		//接车方向
#define SIGNAL_FCXH    0x00002000UL		//发车方向
#define SIGNAL_TGJLXH  0x00004000UL		//是否是通过进路经过的信号机
#define SIGNAL_ISXXH   0x00008000UL		//是虚信号机
#define SIGNAL_YXSD    0x00100000UL		//可做延续始端
#define SIGNAL_YXZD    0x00200000UL		//可做延续终端
#define SIGNAL_PDJZ    0x00400000UL		//坡道进站信号机
#define SIGNAL_FCJLXH  0x00800000UL		//发车进路信号机
#define SIGNAL_LSXSXH  0x01000000UL		//临时限速信号机
#define SIGNAL_TFJLSD  0x02000000UL		//可做推送进路的始端
#define SIGNAL_TFJLZD  0x04000000UL		//可做推送进路的终端
#define SIGNAL_CLCJLXH 0x08000000UL		//长列车进路信号机，信号显示与另加进路信号机关联
#define SIGNAL_SYH     0x10000000UL		//上行咽喉
#define SIGNAL_AQH     0x20000000UL		//安全灯光为红灯
#define SIGNAL_LCZH    0x40000000UL		//列车主和
#define SIGNAL_TFDCXH  0x80000000UL		//驼峰进路中的调车信号机

//通信目标软件类型
#define	 TARGET_INTERLOCK	0xAA		//联锁
#define	 TARGET_TEACHER		0xBB		//教师机
#define	 TARGET_CTC			0xCC		//CTC
#define	 TARGET_CULTIVATE	0xDD		//培训软件
#define	 TARGET_SERVER	0xEE		//通信服务

//自定义颜色
#define COLOR_TRACK_DEEPBLUE			QColor("#202040")
#define COLOR_TRACK_BLUE				QColor("#104E8B")
#define COLOR_TRACK_PRELOCK_BLUE		QColor("#0000EE")
#define COLOR_TRACK_WHITE				QColor("#FAFAFA")
#define COLOR_TRACK_GREEN				QColor("#00CD00")
#define COLOR_TRACK_RED					QColor("#FD0C0C")
#define COLOR_TRACK_YELLOW 				QColor("#FDFD0C")
#define COLOR_TRACK_SELECT_BLUE			QColor("#00E5EE")
#define COLOR_TRACK_OUTSIDE_GREEN		QColor("#7FFF00")
#define COLOR_TRACK_OUTSIDE_YELLOW		QColor("#EEC900")
#define COLOR_TRACK_OUTSIDE_RED			QColor("#EE00EE")

#define COLOR_BTN_WHITE					QColor("#FAFAFA")
#define COLOR_BTN_GRAY					QColor("#666666")
#define COLOR_BTN_DEEPGRAY				QColor("#A6A6A6")
#define COLOR_BTN_YELLOW				QColor("#FFFF33")
#define COLOR_BTN_GREEN					QColor("#458B00")
#define COLOR_BTN_GREEN_TG				QColor("#00EE00")
#define COLOR_BTN_BLUE					QColor("#0000EE")
#define COLOR_BTN_BLUE_YD				QColor("#4169E1")
#define COLOR_BTN_RED					QColor("#FD0C0C")

#define COLOR_LIGHT_BLACK				Qt::NoBrush
#define COLOR_LIGHT_GREEN				QColor("#33FF00")
#define COLOR_LIGHT_RED					QColor("#EE0000")
#define COLOR_LIGHT_YELLOW				QColor("#FFFF33")
#define COLOR_LIGHT_WHITE				QColor("#FCFCFC")
#define COLOR_LIGHT_BLUE				QColor("#4876FF")

#define PASSENGE_TYPE		0x01
#define FREIGH_TYPE			0x02
#define TRAIN_TYPE			0x03


namespace CTCWindows {
	void SetMainWindow(CTCMainWindow* pMainWindow);
	CTCMainWindow* MainWindow();
}

namespace Station {
	void SetMainStation(MainStationObject* pStation);
	MainStationObject* MainStation();
	

	
	void InsterTrainType(int nType, int nIndex, QString strtype);
}


