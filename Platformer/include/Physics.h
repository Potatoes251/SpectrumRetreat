#pragma once

#include "Player.h"
#include "Raycast.h"
#include "LibMath/Colision3D.h"
#include <vector>

void checkWalls(Player& player, const std::vector<GameObject>& gameObjectList);