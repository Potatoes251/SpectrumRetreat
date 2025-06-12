#ifndef PHYSICS_H
#define PHYSICS_H

#include "SceneGraph.h"
#include "Player.h"
#include "Raycast.h"
#include "LibMath/Colision3D.h"

void	checkWalls(Player& player, const SceneGraph<GameObject>& gameObjectList, ImColor color, bool& restart);

void	checkWall(Player& player,  const std::shared_ptr<SceneNode<GameObject>>& gameObjectList, 
					LibMath::Vector3 color, bool& restart);

#endif // PHYSICS_H