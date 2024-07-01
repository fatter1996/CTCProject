#include <QtWidgets/QApplication>
#include "CTCObject/CTCObject.h"
#include "CTCMainWindow/CTCMainWindow.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    CTCDoc::CTCObject CTCObject;
    QMainWindow* mainWnd = CTCObject.CreatCTCMainWnd();
    if (mainWnd) {
        mainWnd->showMaximized();
        return a.exec();
    }
    else {
        a.quit();
    }
}
