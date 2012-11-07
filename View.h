#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

namespace ForceDirectedLayout {

class Node;

// a graphicsview working with tree
class View : public QGraphicsView
{
public:
    explicit View(QWidget* parent = 0);
    void setRoot(Node* root) { _root = root; }
    Node* getRoot() const { return _root; }
    
protected:
    Node* _root;
};

}

#endif // VIEW_H
