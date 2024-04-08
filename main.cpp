#include <QtWidgets/QApplication>
#include "CTCObject/CTCObject.h"
#include "CTCMainWindow/CTCMainWindow.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    CTCDoc::CTCObject CTCObject;
    
    QMainWindow* mainWnd = CTCObject.CreatCTCMainWnd();
    mainWnd->showMaximized();

    return a.exec();
}
