#include "gui_mainwindow.h"

GUI_MainWindow::GUI_MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(window_title);
    setMinimumSize(800, 600);
    setState(init_state);
}

GUI_MainWindow::States GUI_MainWindow::getState() {
    return state;
}

QString GUI_MainWindow::getStateString() {
    switch(state) {
        case(GUI_MainWindow::States::Constraints):
            return "Constraints";
        case(GUI_MainWindow::States::Cursor):
            return "Cursor";
        case(GUI_MainWindow::States::Input):
            return "Input";
        case(GUI_MainWindow::States::None):
        default:
            return "None";
    }
}

void GUI_MainWindow::setState(GUI_MainWindow::States _state, bool writeToStatusBar) {
    if (writeToStatusBar) {
        if (_state == States::Cursor) {
            statusBar()->showMessage("Режим редактирования");
        }
        else if (_state == States::Input) {
            statusBar()->showMessage("Режим ввода");
        }
        else if (_state == States::Constraints) {
            statusBar()->showMessage("Режим наложения ограничений");
        }
    }
    state = _state;
}

ConstraintType GUI_MainWindow::getCurrentConstraint()
{
    return current_constraint;
}

QString GUI_MainWindow::getCurrentConstraintString() {
    switch(current_constraint) {
        case(ConstraintType::AngleBetween):
            return "Angle";
        case(ConstraintType::BelongsTo):
            return "Belongs";
        case(ConstraintType::DistanceBetween):
            return "Distance";
        case(ConstraintType::FixedLength):
            return "Length";
        case(ConstraintType::FixedPoint):
            return "Fixed";
        case(ConstraintType::Parallel):
            return "Parallel";
        case(ConstraintType::Perpendicular):
            return "Perpendicular";
        case(ConstraintType::SamePoint):
            return "SamePoint";
        case(ConstraintType::None):
        default:
            return "None";
    }
}

void GUI_MainWindow::setCurrentConstraint(ConstraintType _newct, bool writeToStatusBar)
{
    if (writeToStatusBar) {
        QString msg = statusBar()->currentMessage();
        msg += " (ограничение - ";
        switch(_newct) {
            case(ConstraintType::AngleBetween):
                msg += "угол между отрезками";
                break;
            case(ConstraintType::DistanceBetween):
                msg += "расстояние между отрезками";
                break;
            case(ConstraintType::FixedLength):
                msg += "длина отрезка";
                break;
            case(ConstraintType::FixedPoint):
                msg += "фиксация точки";
                break;
            case(ConstraintType::None):
                msg += "отсутствует";
                break;
            case(ConstraintType::Parallel):
                msg += "параллельнотсь";
                break;
            case(ConstraintType::Perpendicular):
                msg += "перпендикулярность";
                break;
            case(ConstraintType::SamePoint):
                msg += "совпадение точек";
                break;
            case(ConstraintType::BelongsTo):
                msg += "принадлежность отрезку";
                break;
            default:
                msg += "неопределено";
                break;
        }
        msg += ")";
        statusBar()->showMessage(msg);
    }
    current_constraint = _newct;
}


void GUI_MainWindow::actionDispather(QString action)
{
    if (action == "Cursor") {
        setState(States::Cursor);
        setCurrentConstraint(ConstraintType::None, false);
    }
    else if (action == "Input") {
        setState(States::Input);
        setCurrentConstraint(ConstraintType::None, false);
    }
    else {
        setState(States::Constraints);
        if (action == "Angle"){
            setCurrentConstraint(ConstraintType::AngleBetween);
        }
        else if (action == "Distance") {
            setCurrentConstraint(ConstraintType::DistanceBetween);
        }
        else if (action == "Length") {
            setCurrentConstraint(ConstraintType::FixedLength);
        }
        else if (action == "Fixed") {
            setCurrentConstraint(ConstraintType::FixedPoint);
        }
        else if (action == "Parallel") {
            setCurrentConstraint(ConstraintType::Parallel);
        }
        else if (action == "Perpendicular") {
            setCurrentConstraint(ConstraintType::Perpendicular);
        }
        else if (action == "Belongs") {
            setCurrentConstraint(ConstraintType::BelongsTo);
        }
        else {
            setCurrentConstraint(ConstraintType::None);
        }
    }
#ifdef ENABLE_DEBUG
    qWarning() << getStateString() << " " << getCurrentConstraintString() ;
#endif
}


