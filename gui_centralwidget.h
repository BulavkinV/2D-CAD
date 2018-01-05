#ifndef GUI_CENTRALWIDGET_H
#define GUI_CENTRALWIDGET_H

#include <QGraphicsView>

class GUI_CentralWidget : public QGraphicsView
{
public:
    //GUI_CentralWidget(QWidget *parent = Q_NULLPTR);
    using QGraphicsView::QGraphicsView;
};

#endif // GUI_CENTRALWIDGET_H
