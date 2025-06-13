#include "Raycast.h"


bool    raycast(const LibMath::Line3D& ray, SceneGraph<GameObject>& scene, RaycastHit& hit, float maxRange = 0.0f)
{
    bool foundHit = false;
    float minDistance = maxRange * maxRange;  // squared max range for comparison

    LibMath::Vector3 rayOrigin(ray.getCenter().getX(), ray.getCenter().getY(), ray.getCenter().getZ());

    // Use traverseSceneGraph to iterate all objects
    traverseSceneGraph<GameObject>(scene.root, [&](std::shared_ptr<GameObject>& obj)
        {
            LibMath::Vector3 intersectionPoint;
            if (LibMath::intersectionRayPrism(ray, obj -> m_collider, intersectionPoint))
            {
                float distSq = intersectionPoint.distanceSquaredFrom(rayOrigin);

                if (distSq < minDistance)
                {
                    minDistance = distSq;
                    hit = RaycastHit(obj, intersectionPoint);
                    foundHit = true;
                }
            }
        });

    return foundHit;
}

RaycastHit::RaycastHit(std::shared_ptr<GameObject> gameObject, LibMath::Vector3 point) :
    m_gameObject(gameObject), 
    m_intersectionPoint(point){}
