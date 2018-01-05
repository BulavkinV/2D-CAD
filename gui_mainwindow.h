#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStatusBar>

#include <constraint.h>

#ifdef ENABLE_DEBUG
#include <QDebug>
#endif

const QString window_title{"2D CAD"};

class GUI_MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum class States {
        None,
        Cursor,
        Input,
        Constraints
    };

    explicit GUI_MainWindow(QWidget *parent = nullptr);

    States getState();
    QString getStateString();
    void setState(States, bool writeToStatusBar = true);

    ConstraintType getCurrentConstraint();
    QString getCurrentConstraintString();
    void setCurrentConstraint(ConstraintType, bool writeToStatusBar = true);
private:
    States state;
    ConstraintType current_constraint;
signals:

public slots:
    void actionDispather(QString);
};

const GUI_MainWindow::States init_state{GUI_MainWindow::States::Cursor};


#endif // GUI_MAINWINDOW_H
