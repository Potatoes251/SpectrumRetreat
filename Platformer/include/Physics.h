#pragma once

#include "SceneGraph.h"
#include "Player.h"
#include "Raycast.h"
#include "LibMath/Colision3D.h"
#include <vector>

void checkWalls(Player& player, const SceneGraph<GameObject>& gameObjectList);
void checkWall(Player& player, const std::shared_ptr<SceneNode<GameObject>>& gameObjectList);