#include "initcmake.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InitCMake w;
    w.init();
    w.show();

    return a.exec();
}
