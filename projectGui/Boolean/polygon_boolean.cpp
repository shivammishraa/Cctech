#include "polygon_boolean.h"
#include <QDebug>
#include <algorithm>
#include <cmath>

using namespace std;

using VertexType = PolygonBoolean::GHVertex;

// Determines if a point is inside a polygon using ray casting
bool isPointInsidePolygon(const QPointF& pt, const vector<QPointF>& poly) {
    int crossings = 0;
    for (size_t i = 0; i < poly.size(); ++i) {
        const QPointF& v1 = poly[i];
        const QPointF& v2 = poly[(i + 1) % poly.size()];
        if ((v1.y() > pt.y()) != (v2.y() > pt.y())) {
            double xCross = (v2.x() - v1.x()) * (pt.y() - v1.y()) / (v2.y() - v1.y()) + v1.x();
            if (pt.x() < xCross)
                ++crossings;
        }
    }
    return (crossings % 2) == 1;
}

// Checks if two segments intersect and returns intersection point in 'out'
bool edgeSegmentsIntersect(const QPointF& p1, const QPointF& p2, const QPointF& q1, const QPointF& q2, QPointF& out) {
    double dxA = p2.x() - p1.x();
    double dyA = p2.y() - p1.y();
    double dxB = q2.x() - q1.x();
    double dyB = q2.y() - q1.y();

    double denominator = dxA * dyB - dyA * dxB;
    if (fabs(denominator) < 1e-8) return false;

    double ua = ((q1.x() - p1.x()) * dyB - (q1.y() - p1.y()) * dxB) / denominator;
    double ub = ((q1.x() - p1.x()) * dyA - (q1.y() - p1.y()) * dxA) / denominator;

    if (ua < 0 || ua > 1 || ub < 0 || ub > 1) return false;

    out = QPointF(p1.x() + ua * dxA, p1.y() + ua * dyA);
    return true;
}

// Adds a point to a vector if not already present
void insertUniquePoint(vector<QPointF>& pts, const QPointF& pt) {
    if (find(pts.begin(), pts.end(), pt) == pts.end()) {
        pts.push_back(pt);
    }
}

// Splits polygon edges at intersection points
vector<QPointF> splitPolygonEdges(const vector<QPointF>& poly, const vector<QPointF>& inters) {
    vector<QPointF> out;
    for (size_t i = 0; i < poly.size(); ++i) {
        QPointF v1 = poly[i];
        QPointF v2 = poly[(i + 1) % poly.size()];
        out.push_back(v1);

        for (const auto& ip : inters) {
            QPointF dummy;
            if (edgeSegmentsIntersect(v1, v2, ip, ip, dummy)) {
                out.push_back(ip);
            }
        }
    }
    sort(out.begin(), out.end(), [](const QPointF& a, const QPointF& b) {
        return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
    });
    return out;
}

namespace {
    bool fuzzyPointEqual(const QPointF& a, const QPointF& b, double tol = 1e-6) {
        return std::abs(a.x() - b.x()) < tol && std::abs(a.y() - b.y()) < tol;
    }
}

// Intersection of two polygons
vector<QPointF> PolygonBoolean::intersect(const vector<QPointF>& polyA, const vector<QPointF>& polyB) {
    vector<QPointF> interPoints;

    for (size_t i = 0; i < polyA.size(); ++i) {
        QPointF p1 = polyA[i];
        QPointF p2 = polyA[(i + 1) % polyA.size()];
        for (size_t j = 0; j < polyB.size(); ++j) {
            QPointF q1 = polyB[j];
            QPointF q2 = polyB[(j + 1) % polyB.size()];
            QPointF ip;
            if (edgeSegmentsIntersect(p1, p2, q1, q2, ip)) {
                insertUniquePoint(interPoints, ip);
            }
        }
    }

    for (const auto& pt : polyA) {
        if (isPointInsidePolygon(pt, polyB)) {
            insertUniquePoint(interPoints, pt);
        }
    }
    for (const auto& pt : polyB) {
        if (isPointInsidePolygon(pt, polyA)) {
            insertUniquePoint(interPoints, pt);
        }
    }

    QPointF ctr(0, 0);
    for (const auto& pt : interPoints) ctr += pt;
    if (!interPoints.empty())
        ctr /= interPoints.size();

    sort(interPoints.begin(), interPoints.end(), [&ctr](const QPointF& a, const QPointF& b) {
        double angA = atan2(a.y() - ctr.y(), a.x() - ctr.x());
        double angB = atan2(b.y() - ctr.y(), b.x() - ctr.x());
        return angA < angB;
    });

    return interPoints;
}

// Union of two polygons
vector<QPointF> PolygonBoolean::unionPolygons(const vector<QPointF>& polyA, const vector<QPointF>& polyB) {
    vector<QPointF> inters;

    for (size_t i = 0; i < polyA.size(); ++i) {
        QPointF p1 = polyA[i];
        QPointF p2 = polyA[(i + 1) % polyA.size()];
        for (size_t j = 0; j < polyB.size(); ++j) {
            QPointF q1 = polyB[j];
            QPointF q2 = polyB[(j + 1) % polyB.size()];
            QPointF ip;
            if (edgeSegmentsIntersect(p1, p2, q1, q2, ip)) {
                insertUniquePoint(inters, ip);
            }
        }
    }

    vector<QPointF> splitA = splitPolygonEdges(polyA, inters);
    vector<QPointF> splitB = splitPolygonEdges(polyB, inters);

    vector<QPointF> merged;

    for (const auto& pt : splitA) {
        if (!isPointInsidePolygon(pt, polyB)) {
            merged.push_back(pt);
        }
    }
    for (const auto& pt : splitB) {
        if (!isPointInsidePolygon(pt, polyA)) {
            merged.push_back(pt);
        }
    }
    for (const auto& ip : inters) {
        merged.push_back(ip);
    }

    QPointF ctr(0, 0);
    for (const auto& pt : merged) ctr += pt;
    if (!merged.empty())
        ctr /= merged.size();

    sort(merged.begin(), merged.end(), [&ctr](const QPointF& a, const QPointF& b) {
        double angA = atan2(a.y() - ctr.y(), a.x() - ctr.x());
        double angB = atan2(b.y() - ctr.y(), b.x() - ctr.x());
        return angA < angB;
    });

    merged.erase(unique(merged.begin(), merged.end(), [](const QPointF& a, const QPointF& b) {
        return qFuzzyCompare(a.x(), b.x()) && qFuzzyCompare(a.y(), b.y());
    }), merged.end());

    return merged;
}

// Subtract polyB from polyA
std::vector<QPointF> PolygonBoolean::subtractPolygons(const std::vector<QPointF>& polyA, const std::vector<QPointF>& polyB) {
    std::vector<QPointF> inters;

    for (size_t i = 0; i < polyA.size(); ++i) {
        QPointF p1 = polyA[i];
        QPointF p2 = polyA[(i + 1) % polyA.size()];
        for (size_t j = 0; j < polyB.size(); ++j) {
            QPointF q1 = polyB[j];
            QPointF q2 = polyB[(j + 1) % polyB.size()];
            QPointF ip;
            if (edgeSegmentsIntersect(p1, p2, q1, q2, ip)) {
                insertUniquePoint(inters, ip);
            }
        }
    }

    std::vector<QPointF> output;

    QPointF startPt;
    bool found = false;
    for (const auto& pt : polyA) {
        if (!isPointInsidePolygon(pt, polyB)) {
            startPt = pt;
            found = true;
            break;
        }
    }
    if (!found) return {};

    bool started = false;
    size_t idx = 0;
    while (idx < polyA.size()) {
        QPointF curr = polyA[idx];
        QPointF next = polyA[(idx + 1) % polyA.size()];
        QPointF mid((curr.x() + next.x()) / 2.0, (curr.y() + next.y()) / 2.0);

        if (!started && fuzzyPointEqual(curr, startPt)) {
            started = true;
        }

        if (started) {
            if (!isPointInsidePolygon(mid, polyB)) {
                output.push_back(curr);
            } else {
                while (isPointInsidePolygon(mid, polyB) && idx < polyA.size()) {
                    idx++;
                    curr = polyA[idx % polyA.size()];
                    next = polyA[(idx + 1) % polyA.size()];
                    mid = QPointF((curr.x() + next.x()) / 2.0, (curr.y() + next.y()) / 2.0);
                }
                output.push_back(curr);
            }
        }

        idx++;
        if (started && fuzzyPointEqual(polyA[idx % polyA.size()], startPt)) break;
    }

    if (!output.empty() && !qFuzzyCompare(output.front(), output.back())) {
        output.push_back(output.front());
    }

    output.erase(std::unique(output.begin(), output.end(), [](const QPointF& a, const QPointF& b) {
        return qFuzzyCompare(a.x(), b.x()) && qFuzzyCompare(a.y(), b.y());
    }), output.end());

    return output;
}

// Expose for external use
bool pointInPolygon(const QPointF& point, const std::vector<QPointF>& polygon) {
    return isPointInsidePolygon(point, polygon);
}




