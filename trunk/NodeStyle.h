#ifndef NODESTYLE_H
#define NODESTYLE_H

#include <QColor>

class QRectF;
class QPainterPath;
class QPainter;
class QStyleOptionGraphicsItem;

namespace ForceDirectedLayout {

class NodeStyle
{
public:
    NodeStyle(const QColor& brushColor, const QColor& penColor);

    virtual int          getWidth()     const = 0;
    virtual QRectF       boundingRect() const = 0;
    virtual QPainterPath shape()        const = 0;
    virtual void         paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) = 0;

protected:
    QColor _brushColor;
    QColor _penColor;
};

class RoundNodeStyle : public NodeStyle
{
public:
    RoundNodeStyle(int radius = 10, const QColor& brushColor = QColor(Qt::yellow),
                                    const QColor& penColor   = QColor(Qt::black));
    int getRadius() const { return _radius; }

    virtual int          getWidth()     const { return getRadius() * 2; }
    virtual QRectF       boundingRect() const;
    virtual QPainterPath shape()        const;
    virtual void         paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

private:
    int _radius;
};

class RectangularNodeStyle : public NodeStyle
{
public:
    RectangularNodeStyle(int width = 20, const QColor& brushColor = QColor(Qt::yellow),
                                         const QColor& penColor   = QColor(Qt::black));

    virtual int          getWidth()     const { return _width; }
    virtual QRectF       boundingRect() const;
    virtual QPainterPath shape()        const;
    virtual void         paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

private:
    int _width;
};

}

#endif // NODESTYLE_H
