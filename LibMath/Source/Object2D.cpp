#define _USE_MATH_DEFINES
#include <cmath>
#include "./LibMath/Object2D.h"

#pragma region Line2D

float LibMath::Line2D::lenght()
{
    float dx = endX() - startX();
    float dy = endY() - startY();

    return std::sqrt(dx * dx + dy * dy);
}

float LibMath::Line2D::lenghtSquare()
{
    float dx = endX() - startX();
    float dy = endY() - startY();

    return (dx * dx + dy * dy);
}

#pragma endregion All Line2D functions

#pragma region Collision detection


float LibMath::cross(float x1, float y1, float x2, float y2)
{
    return x1 * y2 - y1 * x2;
}

bool LibMath::collisionPoint2DCircle(const Point2D p, const Circle c) 
{
    float dx = p.X() - c.center().X();
    float dy = p.Y() - c.center().Y();
    float distanceSquared = dx * dx + dy * dy;

    return distanceSquared <= c.radius() * c.radius();
}

bool LibMath::collisionPoint2DRectangleAABB(const Point2D p, const RectangleAABB r)
{
    return (p.X() >= r.min().X() && p.X() <= r.max().X()) &&
           (p.Y() >= r.min().Y() && p.Y() <= r.max().Y());
}

bool LibMath::collisionPoint2DLine2D(const Point2D p, const Line2D l)
{
    float dx1 = p.X() - l.startX();
    float dy1 = p.Y() - l.startY();
    float dx2 = l.endX() - l.startX();
    float dy2 = l.endY() - l.startY();

    float crossProd = cross(dx1, dy1, dx2, dy2);

    if (crossProd != 0)
    {
        return false; // Not collinear
    }
    // Check if point is between the line segment's endpoints
    if (dx1 * dx2 + dy1 * dy2 < 0)
    {
        return false;
    }

    if ((dx1 * dx1 + dy1 * dy1) > (dx2 * dx2 + dy2 * dy2))
    {
        return false;
    }

    return true;
}

bool LibMath::collisionLine2DCircle(const Line2D l, const Circle c)
{
    // Vector from line start to end
    float dx = l.endX() - l.startX();
    float dy = l.endY() - l.startY();

    // Vector from circle center to line start
    float fx = l.startX() - c.center().X();
    float fy = l.startY() - c.center().Y();

    float x = dx * dx + dy * dy;
    float y = 2 * (fx * dx + fy * dy);
    float z = fx * fx + fy * fy - c.radius() * c.radius();

    float discriminant = y * y - 4 * x * z;

    return discriminant >= 0;
}

bool LibMath::collisionLine2DRectangleAABB(const Line2D l, const RectangleAABB r)
{
    float minX = r.min().X();
    float maxX = r.max().X();
    float minY = r.min().Y();
    float maxY = r.max().Y();

    // Check if the line segment intersects any of the rectangle's sides
    return (l.startX() < maxX && l.endX() > minX) && (l.startY() < maxY && l.endY() > minY);
}

bool LibMath::collisionLine2DLine2D(const Line2D l1, const Line2D l2)
{
    float x1 = l1.startX(), y1 = l1.startY(), x2 = l1.endX(), y2 = l1.endY();
    float x3 = l2.startX(), y3 = l2.startY(), x4 = l2.endX(), y4 = l2.endY();

    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (denom == 0) // Lines are parallel or coincident
    {
        
        return false;
    }

    // Calculate the intersection point (using the line intersection formula)
    float intersectX = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denom;
    float intersectY = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denom;

    // Check if the intersection is within the bounds of both line segments
    if (std::min(x1, x2) <= intersectX && intersectX <= std::max(x1, x2) &&
        std::min(y1, y2) <= intersectY && intersectY <= std::max(y1, y2) &&
        std::min(x3, x4) <= intersectX && intersectX <= std::max(x3, x4) &&
        std::min(y3, y4) <= intersectY && intersectY <= std::max(y3, y4)) 
    {
        return true;
    }

    // If intersection point is out of bounds of either segment
    return false;
}

bool LibMath::collisionRectanglesAABB(const RectangleAABB r1, const RectangleAABB r2)
{
    return !(r1.max().X() < r2.min().X() || r1.min().X() > r2.max().X() ||
             r1.max().Y() < r2.min().Y() || r1.min().Y() > r2.max().Y());
}

bool LibMath::collisionCircleCircle(const Circle c1, const Circle c2)
{
    float dx = c1.center().X() - c2.center().X();
    float dy = c1.center().Y() - c2.center().Y();
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = c1.radius() + c2.radius();

    return distanceSquared <= radiusSum * radiusSum;
}

LibMath::Point2D LibMath::rotatePoint(const Point2D& point, const Point2D& center, float angle) 
{
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    float dx = point.X() - center.X();
    float dy = point.Y() - center.Y();

    return Point2D(cosAngle * dx - sinAngle * dy + center.X(), sinAngle * dx + cosAngle * dy + center.Y());
}


void LibMath::projectPoints(const Point2D& axis, const Point2D* points, float& minProj, float& maxProj)
{
    minProj = axis.X() * points[0].X() + axis.Y() * points[0].Y();
    maxProj = minProj;

    for (int i = 1; i < 4; i++) 
    {  
        float proj = axis.X() * points[i].X() + axis.Y() * points[i].Y();

        minProj = std::min(minProj, proj);
        maxProj = std::max(maxProj, proj);
    }
}


void LibMath::getOBBAxes(const RectangleOBB& r, Point2D* axes) 
{
    // The two axes are the directions of the rectangle's sides
    axes[0] = Point2D(cos(r.angle()), sin(r.angle()));         // First axis: along the width
    axes[1] = Point2D(-sin(r.angle()), cos(r.angle()));        // Second axis: perpendicular to the first one (along the height)
}

bool LibMath::collisionRectanglesOBB(const RectangleOBB& r1, const RectangleOBB& r2) 
{
    // Calculate the corner points of both OBBs
    Point2D r1Corners[4] = 
    {
        rotatePoint(Point2D(r1.center().X() - r1.width() / 2, r1.center().Y() - r1.height() / 2), r1.center(), r1.angle()),
        rotatePoint(Point2D(r1.center().X() + r1.width() / 2, r1.center().Y() - r1.height() / 2), r1.center(), r1.angle()),
        rotatePoint(Point2D(r1.center().X() + r1.width() / 2, r1.center().Y() + r1.height() / 2), r1.center(), r1.angle()),
        rotatePoint(Point2D(r1.center().X() - r1.width() / 2, r1.center().Y() + r1.height() / 2), r1.center(), r1.angle())
    };

    Point2D r2Corners[4] = 
    {
        rotatePoint(Point2D(r2.center().X() - r2.width() / 2, r2.center().Y() - r2.height() / 2), r2.center(), r2.angle()),
        rotatePoint(Point2D(r2.center().X() + r2.width() / 2, r2.center().Y() - r2.height() / 2), r2.center(), r2.angle()),
        rotatePoint(Point2D(r2.center().X() + r2.width() / 2, r2.center().Y() + r2.height() / 2), r2.center(), r2.angle()),
        rotatePoint(Point2D(r2.center().X() - r2.width() / 2, r2.center().Y() + r2.height() / 2), r2.center(), r2.angle())
    };

    // Get the axes of both OBBs
    Point2D r1Axes[2];
    Point2D r2Axes[2];

    getOBBAxes(r1, r1Axes);
    getOBBAxes(r2, r2Axes);

    // Combine the axes into one array
    Point2D axes[4] = { r1Axes[0], r1Axes[1], r2Axes[0], r2Axes[1] };

    for (int i = 0; i < 4; i++) 
    {
        float r1Min, r1Max, r2Min, r2Max;

        projectPoints(axes[i], r1Corners, r1Min, r1Max);
        projectPoints(axes[i], r2Corners, r2Min, r2Max);

        // Check if projections overlap, return false if no overlap
        if (r1Max < r2Min || r2Max < r1Min) 
        {
            return false;  
        }
    }

    return true;
}

#pragma endregion All Collision functions