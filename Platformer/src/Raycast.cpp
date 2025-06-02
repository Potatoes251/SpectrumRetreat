#include "Raycast.h"

bool raycast(LibMath::Line3D ray, const std::vector<GameObject>& gameObjectList, RaycastHit& hit)
{
    bool foundHit = false;
    
    float   distanceMin = std::numeric_limits<float>::infinity(); // set min distance to infinity

    LibMath::Vector3    intersectionPoint;

    LibMath::Vector3    rayCenter(ray.getCenter().getX(), ray.getCenter().getY(), ray.getCenter().getZ());

    for (int i = 0; i < gameObjectList.size(); ++i)
    {
        if (LibMath::intersectionRayPrism(ray, gameObjectList[i].collider, intersectionPoint))
        {
            float newDistance = intersectionPoint.distanceSquaredFrom(rayCenter);

            if (newDistance < distanceMin)
            {
                distanceMin = newDistance;
                foundHit = true;
                hit = RaycastHit(gameObjectList[i], intersectionPoint);
            }
        }
    }

    return foundHit;
}

RaycastHit::RaycastHit() :
    m_gameObject(),
    m_intersectionPoint() {}

RaycastHit::RaycastHit(GameObject gameObject, LibMath::Vector3 point) :
    m_gameObject(gameObject), 
    m_intersectionPoint(point){}
