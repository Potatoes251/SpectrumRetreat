#include "Raycast.h"

//RaycastHit raycast(LibMath::Line3D line, std::vector<GameObject> gameObjectList)
//{
//    for (int i = 0; i < gameObjectList.size(); ++i)
//    {
//
//    }
//
//    //return RaycastHit();
//}

RaycastHit::RaycastHit(GameObject gameObject, LibMath::Point3D point) : 
    //m_gameObject(gameObject), 
    m_intersectionPoint(point){}
