#include "gui_centralwidget.h"

GUI_CentralWidget::GUI_CentralWidget(GUI_MainWindow *parent, GUI_Scene* scene) :
    QGraphicsView(static_cast<QGraphicsScene*>(scene))
{
    mw_ptr = parent;
}

void GUI_CentralWidget::QGraphicsView::mousePressEvent(QMouseEvent *event) {
    QPointF point = mapToScene(event->pos().x(), event->pos().y());
    dynamic_cast<GUI_Scene*>(scene())->mousePressEventDispather(point);
}



