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
}

void Player::Update(float deltaTime)
{
    if (isJumping)
    {
        yVelocity += gravity * deltaTime;
        camera.position.m_y += yVelocity * deltaTime;

        if (camera.position.m_y <= 0.0f)
        {
            camera.position.m_y = 0.0f;
            yVelocity = 0.0f;
            isJumping = false;
        }
    }
    position = camera.position;
}

void Player::move(LibMath::Vector3 movement)
{
    camera.position += movement;
    position = camera.position;
}

void Player::processInput(GLFWwindow* window, float deltaTime)
{
    // Check for escape key to close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // WASD movement controls - move camera depending on keys pressed and delta time for smooth movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        ProcessKeyboard('W', deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        ProcessKeyboard('S', deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        ProcessKeyboard('A', deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        ProcessKeyboard('D', deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping)
    {
        yVelocity = jumpStrength;
        isJumping = true;
    }

    Update(deltaTime);
}

void Player::ProcessKeyboard(char direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;

    // Project front and right onto the XZ plane
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

    // Jump
    if (direction == ' ') // Space key
    {
        if (!isJumping)
        {
            yVelocity = jumpStrength;
            isJumping = true;
        }
    }
}


