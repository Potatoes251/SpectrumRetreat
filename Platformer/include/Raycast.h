#pragma once

#include "LibMath/Colision3D.h"
#include "GameObject.h"
#include "SceneGraph.h"
#include <vector>

struct RaycastHit 
{
	std::shared_ptr<GameObject>		m_gameObject;
	LibMath::Vector3				m_intersectionPoint;

	RaycastHit() = default;
	RaycastHit(std::shared_ptr<GameObject>, LibMath::Vector3);
};

bool	raycast(const LibMath::Line3D& ray, SceneGraph<GameObject>& scene, RaycastHit& hit, float maxRange);