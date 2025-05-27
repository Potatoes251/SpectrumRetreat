#ifndef LIBMATH_OBJECTS_
#define LIBMATH_OBJECTS_

#include "Angle.h"
#include "Vector.h"

namespace LibMath
{
    class Point2D
    {
    private:
        float m_x;
        float m_y;

    public:

        // Constructor to initialize a point
        Point2D() : m_x(0), m_y(0) {}
        Point2D(float x) : m_x(x), m_y(x) {}
        Point2D(float x, float y) : m_x(x), m_y(y) {}
        Point2D(const Point2D& other) : m_x(other.m_x), m_y(other.m_y) {}
        ~Point2D() = default;

        float X()const { return m_x; };
        float Y()const { return m_y; };
    };

    class Line2D
    {
    private:
        Point2D m_start;
        Point2D m_end;

    public:
        Line2D() : m_start(), m_end() {}
        Line2D(float x1, float y1, float x2, float y2) : m_start(x1, y1), m_end(x2, y2) {}
        Line2D(const Line2D& other) : m_start(other.m_start), m_end(other.m_end) {}
        ~Line2D() = default;

        float   lenght();
        float   lenghtSquare();

        float startX()const { return m_start.X(); };
        float startY()const { return m_start.Y(); };
        float endX()const { return m_end.X(); };
        float endY()const { return m_end.Y(); };

    };

    class RectangleAABB
    {
    private:
        Point2D m_min;
        Point2D m_max;

    public:
        RectangleAABB() : m_min(0), m_max(0) {}
        RectangleAABB(Point2D min, Point2D max) : m_min(min), m_max(max) {}
        RectangleAABB(const RectangleAABB& other) : m_min(other.m_min), m_max(other.m_max) {} 
        
        ~RectangleAABB() = default;

        Point2D min()const { return m_min; };
        Point2D max()const { return m_max; };
    };

    class RectangleOBB
    {
    public:
        Point2D m_center;

        float m_width;     // Width of the rectangle
        float m_height;    // Height of the rectangle
        float m_angle;     // Rotation angle in radians

        // Constructor to initialize the OBB
        RectangleOBB() : m_center(), m_width(0), m_height(0), m_angle(0) {}
        RectangleOBB(Point2D center, float width = 1.0f, float height = 1.0f, float angle = 0.0f)
            : m_center(center), m_width(width), m_height(height), m_angle(angle) { }
        RectangleOBB(const RectangleOBB& other) : m_center(other.m_center), m_width(other.m_width),
                                                  m_height(other.m_height), m_angle(other.m_angle) {}
        ~RectangleOBB() = default;

        Point2D center()const { return m_center; };

        float width()const { return m_width; };
        float height()const { return m_height; };
        float angle()const { return m_angle; };
    };

    class Circle
    {
    private:
        Point2D m_center;
        float m_radius;

    public:
        Circle() : m_center(), m_radius(0) {}
        Circle(Point2D center, float r = 0) : m_center(center), m_radius(r) {}
        Circle(Circle& other) : m_center(other.m_center), m_radius(other.m_radius) {}
        ~Circle() = default;

        Point2D center()const { return m_center; };

        float radius()const { return m_radius; };
    };

    float cross(float x1, float y1, float x2, float y2);

    bool collisionPoint2DCircle(const Point2D p, const Circle c);
    bool collisionPoint2DRectangleAABB(const Point2D p, const RectangleAABB r);
    bool collisionPoint2DLine2D(const Point2D p, const Line2D l);
    bool collisionLine2DCircle(const Line2D l, const Circle c);
    bool collisionLine2DRectangleAABB(const Line2D l, const RectangleAABB r);
    bool collisionLine2DLine2D(const Line2D l1, const Line2D l2);
    bool collisionRectanglesAABB(const RectangleAABB r1, const RectangleAABB r2);
    bool collisionCircleCircle(const Circle c1, const Circle c2);

    Point2D rotatePoint(const Point2D& point, const Point2D& center, float angle);
    void    projectPoints(const Point2D& axis, const Point2D* points, float& minProj, float& maxProj);
    void    getOBBAxes(const RectangleOBB& r, Point2D* axes);
    bool    collisionRectanglesOBB(const RectangleOBB& r1, const RectangleOBB& r2);
}



#endif