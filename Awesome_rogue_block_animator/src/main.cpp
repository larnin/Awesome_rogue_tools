#include "animatorconfig.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnimatorConfig w;
    w.show();

    return a.exec();
}
