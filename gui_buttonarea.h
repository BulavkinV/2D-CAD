#ifndef GUI_BUTTONAREA_H
#define GUI_BUTTONAREA_H

#include <QToolBar>

#include <vector>
#include <string>

const std::vector<std::string> ba_actions{"cursor", "input", "parallel",
                                       "perpendicular", "angle", "distance",
                                      "fixed", "length", "belongs"};

class GUI_ButtonArea : public QToolBar
{
    Q_OBJECT
public:
    explicit GUI_ButtonArea(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // GUI_BUTTONAREA_H
