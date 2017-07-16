#include "Gui/mainwindow.h"
#include "Utilities/configs.h"
#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    Configs::load(Configs::configsFilename);

    QApplication a(argc, argv);

    QString message;
    if(!Configs::tiles.texture.isValid())
        message += "Aucune texture chargée !\n";
    if(Configs::tiles.groundIDs.empty())
        message += "Aucune tile de sol configurée !\n";
    if(Configs::tiles.walls.empty())
        message += "Aucune tile de mur configurée !\n";

    if(!message.isEmpty())
        QMessageBox::information(nullptr, "Configuration", message);

    MainWindow w;
    w.show();

    auto v(a.exec());
    if(v == 0)
        Configs::save(Configs::configsFilename);
    return v;
}
