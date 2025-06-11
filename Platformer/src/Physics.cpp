#include "Physics.h"


void checkWalls(Player& player, const SceneGraph<GameObject>& gameObjectList, ImColor color, bool& restart)
{
    LibMath::Vector3 vecColor(color.Value.x, color.Value.y, color.Value.z);
    restart = false;
    for (auto& child : gameObjectList.root->children)
    {
        checkWall(player, child, vecColor, restart);
        if (restart)
        {
            return;
        }
    }
}

void checkWall(Player& player, const std::shared_ptr<SceneNode<GameObject>>& gameObject, LibMath::Vector3 color, bool& restart)
{
    LibMath::Vector3 objectColor(gameObject->object->color.m_x, gameObject->object->color.m_y, gameObject->object->color.m_z);
    bool doorOpened = false;
    if (objectColor == color && gameObject->object->tag == "door")
    {
        gameObject->object->color.m_w = 0.2f;
        doorOpened = true;
    }
    else if (gameObject->object->tag == "door")
    {
        gameObject->object->color.m_w = 0.5f;
    }

    //move collider to player position
    LibMath::Point3D capsuleStart(player.getCapsule().getStart() + player.getPosition());
    LibMath::Point3D capsuleEnd(player.getCapsule().getEnd() + player.getPosition());
    

    LibMath::Capsule playerCollider(capsuleStart, capsuleEnd, player.getCapsule().getRadius());

    LibMath::Vector3 normal;
    float penetrationDepth;

    LibMath::Vector3 correction = LibMath::Vector3::zero();

    LibMath::Prism wallCollider = gameObject->object->collider;

    if (LibMath::intersectionCapsulePrism(playerCollider, wallCollider, normal, penetrationDepth))    // look for collision
    {
        if (gameObject->object->tag == "endzone")
        {
            restart = true;
            return;
        }
        if (!doorOpened)  // dont apply collision for door of same color
        {
            player.move(correction);

            if (normal.dot(LibMath::Vector3::up()) >= 0.5f && player.velocity.m_y <= 0)  // detecting collision under the player
            {
                player.isJumping = false;
                player.velocity.m_y = 0.0f;
            }
            float dot = player.velocity.dot(normal);
            if (dot < 0.0f) // only cancel motion *into* the wall
            {
                player.velocity = player.velocity - normal * dot;   // project movement onto the wall
            }

        }
    }

    for (auto& child : gameObject->children)    // go through its children
    {
        checkWall(player, child, color, restart);
    }

}


