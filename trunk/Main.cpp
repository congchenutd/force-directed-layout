#include <QApplication>
#include <QGraphicsView>
#include <QTime>
#include <QDir>
#include "Node.h"
#include "Edge.h"
#include "Engine.h"
#include "NodeStyle.h"
#include "EdgeStyle.h"
#include "View.h"
#include "NodeTopology.h"
#include "BoundaryGuard.h"

using namespace ForceDirectedLayout;

void loadDir(Node* parent, const QString& path, QGraphicsScene& scene)
{
    QFileInfoList entries = QDir(path).entryInfoList(QStringList(), QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    foreach(const QFileInfo& entry, entries)
    {
        if(entry.isDir())
        {
            Node* node = new Node;
            node->setStyle(new RoundNodeStyle(10));
            node->setTopolopy(new TreeNodeTopology);
            node->setPos(100 - qrand() % 200, 100 - qrand() % 200);
            scene.addItem(node);
            scene.addItem(new Edge(parent, node));
            loadDir(node, entry.absoluteFilePath(), scene);
        }
        else
        {
            Node* node = new Node;
            node->setStyle(new RoundNodeStyle(10, Qt::green));
            node->setTopolopy(new TreeNodeTopology);
            node->setPos(100 - qrand() % 200, 100 - qrand() % 200);
            scene.addItem(node);
            scene.addItem(new Edge(parent, node));
        }
    }

}

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
    TriangularBoundary boundary;
    QPolygonF triangle;
    triangle << QPointF(0, 0) << QPointF(0, 300) << QPointF(300, 0);
    boundary.setPolygon(triangle);

//    RectangularBoundary boundary;
////    boundary.setPen(QPen(Qt::transparent));
//    boundary.setRect(-200, -300, 250, 500);

    scene.addItem(&boundary);

    qsrand(QTime::currentTime().msec());

    // root
    Node* root = new Node;
    root->setTopolopy(new TreeNodeTopology);
    scene.addItem(root);
    root->setPos(0, 0);
    view.setRoot(root);

//    loadDir(root, "../Movie", scene);

    for(int i = 0; i < 1; ++i)
    {
        Node* node = new Node;
        NodeStyle* style = new RoundNodeStyle();
        node->setStyle(style);
        node->setTopolopy(new TreeNodeTopology);
        node->setPos(100 - qrand() % 200, 100 - qrand() % 200);
        scene.addItem(node);
        scene.addItem(new Edge(root, node));

//        for(int j = 0; j < 10; ++j)
//        {
//            Node* node2 = new Node;
//            NodeStyle* style = new RoundNodeStyle(10, QColor(Qt::blue).lighter());
//            node2->setStyle(style);
//            node2->setTopolopy(new TreeNodeTopology);
//            node2->setPos(100 - qrand() % 200, 100 - qrand() % 200);
//            scene.addItem(node2);
//            scene.addItem(new Edge(node, node2));
//        }
    }

    // other nodes
//    qsrand(QTime::currentTime().msec());
//    for(int i = 0; i < 10; ++i)
//    {
//        Node* node = new Node;
//        QColor color(qrand() % 255, qrand() % 255, qrand() % 255);
//        NodeStyle* style;
//        if(qrand() % 2 == 0)
//            style = new RoundNodeStyle      (qrand() % 10 + 5, color);
//        else
//            style = new RectangularNodeStyle(qrand() % 10 + 10, color);
//        node->setStyle(style);
//        node->setTopolopy(new TreeNodeTopology);
//        node->setPos(300 - qrand() % 600, 300 - qrand() % 600);
//        scene.addItem(node);
//        scene.addItem(new Edge(root, node));
//    }

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
    engine->setSensitivity(0.01);
//    engine->setDistortion(0.5);
    engine->setBoundaryGuard(new AdhesiveBoundaryGuard(&boundary));
    Engine::setCurrent(engine);
    engine->start();
    
    return app.exec();
}
