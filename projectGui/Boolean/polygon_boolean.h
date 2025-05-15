#ifndef POLYGON_BOOLEAN_H
#define POLYGON_BOOLEAN_H

#include <QPointF>
#include <vector>

class PolygonBoolean
{
public:
    using GHVertex = QPointF;

    // Returns intersection points of two polygons
    static std::vector<QPointF> intersect(const std::vector<QPointF> &polyA, const std::vector<QPointF> &polyB);

    // Returns the union of two polygons
    static std::vector<QPointF> unionPolygons(const std::vector<QPointF> &polyA, const std::vector<QPointF> &polyB);

    // Returns the subtraction of polyB from polyA
    static std::vector<QPointF> subtractPolygons(const std::vector<QPointF> &polyA, const std::vector<QPointF> &polyB);
};

// Exposed utility for point-in-polygon
bool pointInPolygon(const QPointF &point, const std::vector<QPointF> &polygon);

#endif // POLYGON_BOOLEAN_H
