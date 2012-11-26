#include "View.h"
#include "Node.h"
#include <QKeyEvent>

namespace ForceDirectedLayout {


View::View(QWidget* parent)
    : QGraphicsView(parent),
      _root(0)
{}

void View::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Plus)
        scale(1.2, 1.2);
    else if(event->key() == Qt::Key_Minus)
        scale(0.8, 0.8);
    QGraphicsView::keyPressEvent(event);
}


} // namespace
