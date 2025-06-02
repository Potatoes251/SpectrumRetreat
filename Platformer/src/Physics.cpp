#include "Physics.h"


void checkWalls(Player& player, const SceneGraph<GameObject>& gameObjectList)
{
    for (auto& child : gameObjectList.root->children)
    {
        checkWall(player, child);
    }
}

void checkWall(Player& player, const std::shared_ptr<SceneNode<GameObject>>& gameObject)
{
    LibMath::Point3D capsuleStart(player.getCapsule().getStart() + player.getPosition());
    LibMath::Point3D capsuleEnd(player.getCapsule().getEnd() + player.getPosition());
    

    LibMath::Capsule playerCollider(capsuleStart, capsuleEnd, player.getCapsule().getRadius());

    LibMath::Vector3 normal;

    LibMath::Prism wallCollider = gameObject->object->collider;

    if (LibMath::intersectionCapsulePrism(playerCollider, wallCollider, normal))
    {
        //std::cout << gameObject->object->getName();
        float dot = player.velocity.dot(normal);
        if (dot < 0.0f) // only cancel motion *into* the wall
        {
            player.velocity = player.velocity - normal * dot;
        }
        if (normal.dot(LibMath::Vector3::up()) >= .9f)
        {
            player.isJumping = false;
        }
    }

    for (auto& child : gameObject->children)
    {
        checkWall(player, child);
    }
}


