#include "Player.h"

Player::Player()
    : collider(
        LibMath::Point3D(0.0f, 1.0f, 0.0f), 
        LibMath::Point3D(0.0f, 0.1f, 0.0f), 
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

void Player::Update(float deltaTime)
{
    // Apply gravity
    if (isJumping)
    {
        yVelocity += gravity * deltaTime;
    }

    // Apply vertical velocity (y-axis)
    velocity.m_y = yVelocity;

    // Damp horizontal movement
    velocity.m_x *= 0.8f;
    velocity.m_z *= 0.8f;
}

void Player::move(float deltaTime)
{
    camera.position += velocity * deltaTime;

    if (camera.position.m_y <= 0.0f)
    {
        camera.position.m_y = 0.0f;
        yVelocity = 0.0f;
        isJumping = false;
    }

    position = camera.position;
}

void Player::processInput(GLFWwindow* window, float deltaTime, SceneGraph<GameObject>& scene, CircleUI& phone)
{
    LibMath::Vector3 inputVelocity(0.0f, 0.0f, 0.0f);

    float speed = movementSpeed;

    // Flattened directions
    LibMath::Vector3 flatFront = camera.front;

    flatFront.m_y = 0;
    flatFront.normalize();

    LibMath::Vector3 flatRight = camera.right;

    flatRight.m_y = 0;
    flatRight.normalize();

    // Check for escape key to close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping)
    {
        yVelocity = jumpStrength;
        isJumping = true;
    }

    bool isEKeyPressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

    if (isEKeyPressed && !EKeyAlreadyPressed)
    {
        checkForwardRaycast(scene, phone); // Only triggers once per key press
    }

    EKeyAlreadyPressed = isEKeyPressed;

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

void Player::checkForwardRaycast(SceneGraph<GameObject>& scene, CircleUI& phone)
{
    LibMath::Point3D rayOrigin = camera.position;
    LibMath::Vector3 rayDirection = camera.front;
    rayDirection.normalize();

    LibMath::Line3D ray(rayOrigin, rayDirection);
    RaycastHit hit;

    const float maxRange = 10.0f;  // Max raycast distance

    //std::cout << phone.getColor().Value.x << std::endl;
    //&& hit.m_gameObject->getName() == "blue lapinou"
    if (raycast(ray, scene, hit, maxRange) )
    {
        LibMath::Vector4 temp(hit.m_gameObject->color);

        std::cout << temp.m_x << " " << temp.m_y << " " << temp.m_z << std::endl;

        ImColor phoneColor = phone.getColor();

        hit.m_gameObject->color = LibMath::Vector4(
            phoneColor.Value.x,
            phoneColor.Value.y,
            phoneColor.Value.z,
            phoneColor.Value.w
        );



        phone.setColor(ImColor(temp.m_x, temp.m_y, temp.m_z, temp.m_w));

        std::cout << hit.m_gameObject->color.m_x << " " << hit.m_gameObject->color.m_y << " " << hit.m_gameObject->color.m_z << std::endl;

        std::cout << phoneColor.Value.x << " " << phoneColor.Value.y<< " " << phoneColor.Value.z << std::endl;
    }
    else
    {
        std::cout << "Raycast missed." << std::endl;
    }
}


