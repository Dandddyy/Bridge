#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    //w.showFullScreen();
    w.setWindowTitle("Bridge");
    w.setWindowIcon(QIcon(":/img/PNG-cards-1.3/icon.png"));
    w.show();
    return a.exec();
}
