#include "Physics.h"


void checkWalls(Player& player, const std::vector<GameObject>& gameObjectList)
{
    LibMath::Point3D capsuleStart (player.getCapsule().getStart() + player.getPosition());
    LibMath::Point3D capsuleEnd (player.getCapsule().getEnd() + player.getPosition());


    LibMath::Capsule playerCollider (capsuleStart, capsuleEnd, player.getCapsule().getRadius());

    LibMath::Vector3 correction;

    for (int i = 0; i < gameObjectList.size(); ++i)
    {
        if (LibMath::intersectionCapsulePrism(player.getCapsule(), gameObjectList[i].collider, correction))
        {
            player.move(correction);
        }
    }
}
