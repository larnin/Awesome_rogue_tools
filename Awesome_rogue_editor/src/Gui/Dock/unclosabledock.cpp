#include "unclosabledock.h"

UnclosableDock::UnclosableDock(const QString &title, QWidget *parent, Qt::WindowFlags flags)
    :QDockWidget(title, parent, flags)
{
    setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
}
