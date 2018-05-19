#include "mainwindow.h"
#include "cap_anlayze.h"
#include "cap_public.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //analayze_cap("/","/");
    return a.exec();
}
