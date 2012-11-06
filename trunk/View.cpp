#include "View.h"
#include "Node.h"

namespace ForceDirectedLayout {


View::View(QWidget* parent)
    : QGraphicsView(parent),
      _root(0)
{
}

void View::setRoot(Node* root)
{
    _root = root;
}

}
