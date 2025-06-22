#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();  // ✅ Harus ada ini agar GUI muncul
    return a.exec();
}
