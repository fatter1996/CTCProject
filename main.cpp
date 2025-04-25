#include "CTCObject/CTCObject.h"
#include <QtWidgets/QApplication>
#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CTCDoc::CTCObject CTCObject;
    QMainWindow* mainWnd = CTCObject.CreatCTCMainWnd();
    if (mainWnd) {
        mainWnd->setWindowIcon(QIcon("icon/icon.ico"));
        mainWnd->setWindowTitle("CTC-车务终端仿真培训系统");
        mainWnd->showMaximized();
        return a.exec();
    }
    else {
        a.quit();
    }
}
