#include "gui_scene.h"

GUI_Scene::GUI_Scene(GUI_MainWindow *parent, GeometryInterface *gi): QGraphicsScene(Q_NULLPTR) {
    mw = parent;
    interface = gi;

    tmp_point = Q_NULLPTR;
    tmp_segment = Q_NULLPTR;
}

void GUI_Scene::beginLine(QPointF start_pos)
{
    tmp_point = new Point2D(start_pos);
    this -> addItem(tmp_point);
}

void GUI_Scene::endLine(QPointF end_pos, const QList<QGraphicsItem*>& itemsAtPos) {
    object_id_t first_id = interface -> add_point(tmp_point->getPos());
    object_id_t second_id = interface -> add_point(end_pos);

    Point2D* point_ptr = dynamic_cast<Point2D*>(interface ->getObjectById(first_id));
    this -> addItem(point_ptr);

    point_ptr = dynamic_cast<Point2D*>(interface ->getObjectById(second_id));
    this -> addItem(point_ptr);

    //add segment
    object_id_t segment_id = interface->addSegment(first_id, second_id);
    Segment2P* segment = dynamic_cast<Segment2P*>(interface -> getObjectById(segment_id));
    this -> addItem(segment);


    //make same_points constraint
    for (const auto& item: itemsAtPos) {
            GeometryObject* obj = dynamic_cast<GeometryObject *>(item);

            if (obj!=nullptr && obj->getType() == GeometryObjectType::Point) {
                Point2D* first_to_const = dynamic_cast<Point2D*>(interface->getObjectById(second_id));
                Point2D* second_to_const = dynamic_cast<Point2D*>(obj);

                interface->makeConstraintByPtrs(ConstraintType::SamePoint, first_to_const, second_to_const);
        }
    }

    delete tmp_segment;
    delete tmp_point;

    tmp_point = Q_NULLPTR;
    tmp_segment = Q_NULLPTR;
}

void GUI_Scene::movePoint(const QList<QGraphicsItem *> &list)
{
    if (list.empty()) {
        return;
    }

    for (const auto& item: list) {
            GeometryObject* obj = dynamic_cast<GeometryObject *>(item);

            if (obj!=nullptr && obj->getType() == GeometryObjectType::Point) {
                tmp_point = dynamic_cast<Point2D*>(obj);
        }
    }
}


void GUI_Scene::mousePressEventDispather(QPointF pos, const QList<QGraphicsItem*>& itemsAtPos) {
    GUI_MainWindow::States state = mw->getState();
    switch(state) {
        case(GUI_MainWindow::States::Cursor):
            if (tmp_point == Q_NULLPTR)
            {
                movePoint(itemsAtPos);
            }
            else {
                tmp_point = Q_NULLPTR;
            }
            break;
        case(GUI_MainWindow::States::Input):
            if (tmp_point == Q_NULLPTR) {
                beginLine(pos);
            }
            else {
                endLine(pos, itemsAtPos);
            }
            break;
        case(GUI_MainWindow::States::Constraints): {
                //make this
                mw->getCurrentConstraint();
                for(const auto& item : itemsAtPos)
                    qWarning() << item->pos();
            }
            break;
        case(GUI_MainWindow::States::None):
        default:
            throw MainWindowWrongState();
    }
}

void GUI_Scene::mouseMoveEventDispather(QPointF pos)
{
    GUI_MainWindow::States state = mw->getState();
    switch(state){
        case(GUI_MainWindow::States::Input):
            if (tmp_point != Q_NULLPTR) {
                if (tmp_segment == Q_NULLPTR) {
                    tmp_segment = new QGraphicsLineItem(tmp_point->getPos().x(), tmp_point->getPos().y(),
                                                        pos.x(), pos.y());
                    tmp_segment ->setPen(tmp_line_pen);
                    this -> addItem(tmp_segment);
                }
                else {
                    tmp_segment -> setLine(tmp_point->getPos().x(), tmp_point->getPos().y(),
                                           pos.x(), pos.y());
                }

            }
            break;
        case(GUI_MainWindow::States::Cursor):
            if (tmp_point != Q_NULLPTR) {
                interface->movePoint(tmp_point, pos);
            }
            break;
        case(GUI_MainWindow::States::Constraints):
        default:
            throw NotImplementedException();
            break;
    }

}
