#include "gui_scene.h"

void QGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    addText("Hello, world!");
}
