#include "gui_centralwidget.h"

GUI_CentralWidget::GUI_CentralWidget(GUI_MainWindow *parent, GUI_Scene* scene) :
    QGraphicsView(static_cast<QGraphicsScene*>(scene))
{
    mw_ptr = parent;
    setSceneRect(0., 0., mw_ptr->width(), mw_ptr->height());
    setMouseTracking(true);
}

void GUI_CentralWidget::QGraphicsView::mousePressEvent(QMouseEvent *event) {
    QPointF point = mapToScene(event->pos());
    const QList<QGraphicsItem*> itemsAtPos = this->items(event->pos());
    dynamic_cast<GUI_Scene*>(scene())->mousePressEventDispather(point, itemsAtPos);
}

void GUI_CentralWidget::QGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    QPointF point = mapToScene(event->pos());
    dynamic_cast<GUI_Scene*>(scene())->mouseMoveEventDispather(point);
}
