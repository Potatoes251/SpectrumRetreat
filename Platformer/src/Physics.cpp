#include "Physics.h"


void checkWalls(Player& player, const SceneGraph<GameObject>& gameObjectList, ImColor color)
{
    LibMath::Vector3 vecColor(color.Value.x, color.Value.y, color.Value.z);
    for (auto& child : gameObjectList.root->children)
    {
        checkWall(player, child, vecColor);
    }
}

void checkWall(Player& player, const std::shared_ptr<SceneNode<GameObject>>& gameObject, LibMath::Vector3 color)
{
    LibMath::Vector3 objectColor(gameObject->object->color.m_x, gameObject->object->color.m_y, gameObject->object->color.m_z);
    bool doorOpened = false;
    if (objectColor == color && gameObject->object->isDoor)
    {
        gameObject->object->color.m_w = 0.2f;
        doorOpened = true;
    }
    else if (gameObject->object->isDoor)
    {
        gameObject->object->color.m_w = 0.5f;
    }

    //move collider to player position
    LibMath::Point3D capsuleStart(player.getCapsule().getStart() + player.getPosition());
    LibMath::Point3D capsuleEnd(player.getCapsule().getEnd() + player.getPosition());
    

    LibMath::Capsule playerCollider(capsuleStart, capsuleEnd, player.getCapsule().getRadius());

    LibMath::Vector3 normal;

    LibMath::Prism wallCollider = gameObject->object->collider;

    if (LibMath::intersectionCapsulePrism(playerCollider, wallCollider, normal))    // look for collision
    {
        if (!doorOpened)  // dont apply collision for door of same color
        {
            float dot = player.velocity.dot(normal);
            if (dot < 0.0f) // only cancel motion *into* the wall
            {
                player.velocity = player.velocity - normal * dot;   // project movement onto the wall
            }
            if (normal.dot(LibMath::Vector3::up()) >= .9f)  // detecting collision under the player
            {
                player.isJumping = false;
            }
        }
    }

    for (auto& child : gameObject->children)    // go through its children
    {
        checkWall(player, child, color);
    }
}


