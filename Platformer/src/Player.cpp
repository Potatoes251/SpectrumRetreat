#include "Player.h"

Player::Player()
    : collider(
        LibMath::Point3D(0.0f, 1.0f, 0.0f),
        LibMath::Point3D(0.0f, 2.0f, 0.0f),
        0.5f
    ),
    camera(
        LibMath::Vector3(0.0f, 2.0f, 5.0f),
        LibMath::Vector3(0.0f, 1.0f, 0.0f),
        -90.0f,
        0.0f
    )
{
    velocity = LibMath::Vector3(0.0f);
}

void Player::Update(float deltaTime, GLFWwindow* window, const std::vector<GameObject>& gameObjectList)
{
    handleMovementInput(window);
    handleJumpInput(window);
    handleMouseRaycast(window, gameObjectList);

    applyGravity(deltaTime);
    updatePosition(deltaTime);
    dampHorizontalVelocity();

    position = camera.position;
}

void Player::applyGravity(float deltaTime)
{
    if (isJumping)
        yVelocity += gravity * deltaTime;
    velocity.m_y = yVelocity;
}

void Player::updatePosition(float deltaTime)
{
    camera.position += velocity * deltaTime;

    if (camera.position.m_y <= 0.0f)
    {
        camera.position.m_y = 0.0f;
        yVelocity = 0.0f;
        isJumping = false;
    }
}

void Player::dampHorizontalVelocity()
{
    velocity.m_x = 0.0f;
    velocity.m_z = 0.0f;
}

void Player::move(LibMath::Vector3 movement)
{
    camera.position += movement;
    position = camera.position;
}

void Player::handleMovementInput(GLFWwindow* window)
{
    float speed = movementSpeed;
    LibMath::Vector3 inputVelocity(0.0f);

    LibMath::Vector3 flatFront = camera.front;
    flatFront.m_y = 0;
    flatFront.normalize();

    LibMath::Vector3 flatRight = camera.right;
    flatRight.m_y = 0;
    flatRight.normalize();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        inputVelocity += flatFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        inputVelocity -= flatFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        inputVelocity -= flatRight;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        inputVelocity += flatRight;

    inputVelocity.normalize();
    velocity.m_x = inputVelocity.m_x * speed;
    velocity.m_z = inputVelocity.m_z * speed;
}

void Player::handleJumpInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping)
    {
        yVelocity = jumpStrength;
        isJumping = true;
    }
}

void Player::handleMouseRaycast(GLFWwindow* window, const std::vector<GameObject>& gameObjectList)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        LibMath::Point3D start(camera.position);
        LibMath::Vector3 dir = camera.front;
        dir.normalize();

        LibMath::Line3D ray(start, dir);
        RaycastHit hit;

        if (raycast(ray, gameObjectList, hit))
        {
            std::cout << "Ray hit object at: "
                << hit.m_intersectionPoint.m_x << ", "
                << hit.m_intersectionPoint.m_y << ", "
                << hit.m_intersectionPoint.m_z << std::endl;
        }

        debugRayOrigin = start;
        debugRayDirection = dir;
        drawDebugRay = true;
    }
}

void Player::ProcessKeyboard(char direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;

    LibMath::Vector3 flatFront = camera.front;
    flatFront.m_y = 0;
    flatFront.normalize();

    LibMath::Vector3 flatRight = camera.right;
    flatRight.m_y = 0;
    flatRight.normalize();

    if (direction == 'W') camera.position += flatFront * velocity;
    if (direction == 'S') camera.position -= flatFront * velocity;
    if (direction == 'A') camera.position -= flatRight * velocity;
    if (direction == 'D') camera.position += flatRight * velocity;

    if (direction == ' ' && !isJumping)
    {
        yVelocity = jumpStrength;
        isJumping = true;
    }
}