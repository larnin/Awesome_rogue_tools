#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Animation/animationwindow.h"
#include "Animator/animatorwindow.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() = default;

signals:

public slots:
    void onDeleteAnimation(unsigned int index);

    void onNew();
    void onLoad();
    void onSave();
    void onSaveAs();
    void onBackColorChange();

private:
    AnimationWindow * m_animationWindow;
    AnimatorWindow * m_animatorWindow;

    QString m_filename;
};

#endif // MAINWINDOW_H
