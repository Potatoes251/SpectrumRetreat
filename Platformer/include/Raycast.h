#pragma once

#include "LibMath/Geometry3D.h"
#include <vector>

struct GameObject
{
	LibMath::Prism box;
};

struct RaycastHit 
{
	RaycastHit(GameObject, LibMath::Point3D);

	GameObject* m_gameObject;
	LibMath::Point3D m_intersectionPoint;
};

RaycastHit raycast(LibMath::Line3D line, std::vector<GameObject> gameObjectList);