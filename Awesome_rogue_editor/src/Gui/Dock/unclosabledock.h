#ifndef UNCLOSABLEDOCK_H
#define UNCLOSABLEDOCK_H

#include <QDockWidget>
#include <QCloseEvent>

class UnclosableDock : public QDockWidget
{
public:
    explicit UnclosableDock(const QString & title, QWidget * parent = nullptr, Qt::WindowFlags flags = 0);
};

#endif // UNCLOSABLEDOCK_H
