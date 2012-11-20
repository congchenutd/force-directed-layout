#include <QApplication>
#include <QGraphicsView>
#include <QTime>
#include "Node.h"
#include "Edge.h"
#include "Engine.h"
#include "NodeStyle.h"
#include "EdgeStyle.h"
#include "View.h"
#include "NodeTopology.h"

using namespace ForceDirectedLayout;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	// scene and view
    View view;
    view.setRenderHints(QPainter::Antialiasing);
	QGraphicsScene scene(-300, -300, 600, 600);
    view.setScene(&scene);
    view.resize(800, 800);
    view.show();

	// frame
    scene.addItem(new FrameEdge(-300, -300,  300, -300, FrameEdge::TOP));
    scene.addItem(new FrameEdge(-300,  300,  300,  300, FrameEdge::BOTTOM));
    scene.addItem(new FrameEdge(-300, -300, -300,  300, FrameEdge::LEFT));
    scene.addItem(new FrameEdge( 300, -300,  300,  300, FrameEdge::RIGHT));

    // root
    Node* root = new Node;
    root->setTopolopy(new TreeNodeTopology);
    scene.addItem(root);
    root->setPos(0, 0);
    view.setRoot(root);

    // other nodes
    qsrand(QTime::currentTime().msec());
    for(int i = 0; i < 30; ++i)
    {
        Node* node = new Node;
        QColor color(qrand() % 255, qrand() % 255, qrand() % 255);
        NodeStyle* style;
        if(qrand() % 2 == 0)
            style = new RoundNodeStyle      (qrand() % 10 + 10, color);
        else
            style = new RectangularNodeStyle(qrand() % 10 + 10, color);
        node->setStyle(style);
        node->setTopolopy(new TreeNodeTopology);
        node->setPos(300 - qrand() % 600, 300 - qrand() % 600);
        scene.addItem(node);
        scene.addItem(new Edge(root, node));
    }

//    Node* n2 = new Node;
//    Node* n3 = new Node;
//    Node* n4 = new Node;
//    n2->setStyle(new RoundNodeStyle(20, Qt::red));
//    n3->setStyle(new RectangularNodeStyle(20, Qt::blue));
//    n4->setStyle(new RectangularNodeStyle(30, Qt::green));
//    n2->setTopolopy(new TreeNodeTopology);
//    n3->setTopolopy(new TreeNodeTopology);
//    n4->setTopolopy(new TreeNodeTopology);
//    scene.addItem(n2);
//    scene.addItem(n3);
//    scene.addItem(n4);
//    n2->setPos(200, 100);
//    n3->setPos(200, 200);
//    n4->setPos(100, 200);

	// edges
//    scene.addItem(new Edge(root, n2));
//    scene.addItem(new Edge(root, n3));
//    scene.addItem(new Edge(root, n4));

	// engine
    IterativeEngine* engine = new GlobalEngine(&view);
    engine->setPushingAmplifier(1000);
    engine->setToughness(0.1);
    Engine::setCurrent(engine);
    engine->start();
    
    return app.exec();
}
