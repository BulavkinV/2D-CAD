#include "gui_scene.h"

void GUI_Scene::beginLine(QPointF start_pos)
{
    //debug
    object_id_t point_id = interface -> add_point(start_pos);
    Point2D* point_ptr = dynamic_cast<Point2D*>(interface ->getObjectById(point_id));

    dynamic_cast<Point2D*>(point_ptr);
    qWarning() << start_pos;
    //point_ptr -> setPos({10, 10});
    point_ptr -> mapToScene(0.,0.);
    this->addItem(point_ptr);
    //Point2D *temp_point = new Point2D(start_pos);
    //this -> addItem();
    //обозначить точку
    //включить буксировку
    //закрепить линию особого вида за концом указателя мыши
}

GUI_Scene::GUI_Scene(GUI_MainWindow *parent, GeometryInterface *gi): QGraphicsScene(Q_NULLPTR) {
    mw = parent;
    interface = gi;
}

/*
void GUI_Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    GUI_MainWindow::States state = mw->getState();
    switch(state) {
        case(GUI_MainWindow::States::Cursor):
            throw NotImplementedException();
            break;
        case(GUI_MainWindow::States::Input):
            beginLine(event->scenePos());
            break;
        case(GUI_MainWindow::States::Constraints):
            throw NotImplementedException();
        case(GUI_MainWindow::States::None):
        default:
            throw MainWindowWrongState();
    }
}*/

void GUI_Scene::mousePressEventDispather(QPointF pos) {
   beginLine(pos);
}
