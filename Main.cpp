#include <QApplication>
#include <QGraphicsView>
#include "Node.h"
#include "Edge.h"
#include "Engine.h"
#include "NodeStyle.h"
#include "EdgeStyle.h"

using namespace ForceDirectedLayout;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QGraphicsView view;
    view.setRenderHints(QPainter::Antialiasing);
    QGraphicsScene scene(0, 0, 300, 300);
    view.setScene(&scene);
    view.resize(600, 600);
    view.show();

    Node* n1 = new Node;
    Node* n2 = new Node;
    Node* n3 = new Node;
    Node* n4 = new Node;
    n2->setStyle(new RoundNodeStyle(20, Qt::red));
    n3->setStyle(new RectangularNodeStyle(20, Qt::blue));
    n4->setStyle(new RectangularNodeStyle(30, Qt::green));
    scene.addItem(n1);
    scene.addItem(n2);
    scene.addItem(n3);
    scene.addItem(n4);
    n1->setPos(100, 100);
    n2->setPos(200, 100);
    n3->setPos(200, 200);
    n4->setPos(100, 200);

    scene.addItem(new Edge(n1, n2));
    scene.addItem(new Edge(n2, n3));
    scene.addItem(new Edge(n3, n4));
    scene.addItem(new Edge(n1, n4));

    GlobalEngine engine(&scene);
    Engine::setCurrent(&engine);
    engine.start();
    
    return app.exec();
}
