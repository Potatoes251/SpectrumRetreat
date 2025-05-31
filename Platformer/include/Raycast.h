#pragma once

#include "LibMath/Colision3D.h"
#include "GameObject.h"
#include <vector>

struct RaycastHit 
{
	RaycastHit();
	RaycastHit(GameObject, LibMath::Vector3);

	GameObject m_gameObject;
	LibMath::Vector3 m_intersectionPoint;
};

bool raycast(LibMath::Line3D line, const std::vector<GameObject>& gameObjectList, RaycastHit& hit);