#include "NodeStyle.h"
#include <QRect>
#include <QPainterPath>
#include <QPainter>

namespace ForceDirectedLayout {

NodeStyle::NodeStyle(const QColor& brushColor, const QColor& penColor)
    : _brushColor(brushColor),
      _penColor(penColor)
{}


/////////////////////////////////////////////////////////////////////////////////////
RoundNodeStyle::RoundNodeStyle(int radius, const QColor& brushColor, const QColor& penColor)
    : NodeStyle(brushColor, penColor), _radius(radius)
{}

QRectF RoundNodeStyle::boundingRect() const {
    return QRectF(-getRadius(), -getRadius(), getWidth(), getWidth());
}

QPainterPath RoundNodeStyle::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void RoundNodeStyle::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QRadialGradient gradient(-5, -5, getRadius()*1.5);
    gradient.setColorAt(0, QColor(Qt::white));
    gradient.setColorAt(0.3, _brushColor.lighter(150));
    gradient.setColorAt(0.7, _brushColor);
    gradient.setColorAt(1, _brushColor.darker(120));
    painter->setBrush(gradient);
    painter->setPen(QPen(_penColor));
    painter->drawEllipse(boundingRect());
}


/////////////////////////////////////////////////////////////////////////////////////
RectangularNodeStyle::RectangularNodeStyle(int width, const QColor& brushColor, const QColor& penColor)
    : NodeStyle(brushColor, penColor), _width(width)
{}

QRectF RectangularNodeStyle::boundingRect() const {
    return QRectF(-getWidth() / 2, -getWidth() / 2, getWidth(), getWidth());
}

QPainterPath RectangularNodeStyle::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void RectangularNodeStyle::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QLinearGradient gradient(-getWidth() / 2, -getWidth() / 2, getWidth(), getWidth());
    gradient.setColorAt(0, _brushColor.lighter());
    gradient.setColorAt(1, _brushColor.darker());
    painter->setBrush(gradient);
    painter->setPen(QPen(_penColor));
    painter->drawRect(boundingRect());
}

} // namespace
