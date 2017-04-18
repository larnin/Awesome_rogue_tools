#include "animationwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnimationWindow w;
    w.show();

    return a.exec();
}
