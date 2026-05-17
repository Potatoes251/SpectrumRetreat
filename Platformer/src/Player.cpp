#include "Player.h"
#include "GameManager.h"

Player::Player()
    : collider(
        LibMath::Point3D(0.0f, 0.0f, 0.0f),
        LibMath::Point3D(0.0f, -1.5f, 0.0f),
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
    position = camera.position;
}

void    Player::update(float deltaTime)
{
    // apply gravity
    velocity.m_y += gravity * deltaTime;

    // Damp horizontal movement
    velocity.m_x *= 0.8f;
    velocity.m_z *= 0.8f;
    position = camera.position;
}

void    Player::move(float deltaTime)
{
    // apply the velocity
    camera.position += velocity * deltaTime;

    position = camera.position;
}

void Player::processInput(GLFWwindow* window, float deltaTime, SceneGraph<GameObject>& scene, CircleUI& phone, CircleUI& crosshair)
{
    // process the inpus
    LibMath::Vector3 inputVelocity(0.0f, 0.0f, 0.0f);

    float speed = movementSpeed;

    // Flattened directions
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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping)
    {
        velocity.m_y = jumpStrength;
        isJumping = true;
    }
    
    checkForwardRaycast(window);

    update(deltaTime);
}

void    Player::checkForwardRaycast(GLFWwindow* window)
{
    bool isEKeyPressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

    LibMath::Point3D rayOrigin = camera.position;
    LibMath::Vector3 rayDirection = camera.front;

    rayDirection.normalize();

    LibMath::Line3D ray(rayOrigin, rayDirection);

    RaycastHit hit;

    const float maxRange = 1000.0f;

    if (raycast(ray, gameManager -> scene, hit, maxRange) && hit.m_gameObject -> m_tag == "interactable")
    {
        gameManager -> crosshairPtr -> setRadius(4);

        LibMath::Vector4 objColor = hit.m_gameObject -> m_color;
        LibMath::Vector4 phoneColor(
            gameManager -> phonePtr -> getColor().Value.x,
            gameManager -> phonePtr -> getColor().Value.y,
            gameManager -> phonePtr -> getColor().Value.z,
            gameManager -> phonePtr -> getColor().Value.w
        );

        // Only swap colors on E press
        if (isEKeyPressed && !EKeyAlreadyPressed)
        {
            if (objColor != phoneColor)
            {
                // Swap colors
                hit.m_gameObject -> m_color = phoneColor;

                gameManager -> phonePtr -> setColor(ImColor(objColor.m_x, objColor.m_y, objColor.m_z, objColor.m_w));
                gameManager -> soundManager.playSound("../../Assets/Sounds/changeColor.wav");
            }
        }

        // This part runs *every frame* if the colors match
        if (objColor == phoneColor && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            // Pull player toward object
            LibMath::Vector3 toObject = hit.m_gameObject -> m_position - camera.position;

            float distance = toObject.magnitude();

            if (distance > 1.0f) // stop pulling when close
            {
                toObject.normalize();

                const float attractionSpeed = 15.0f;

                velocity.m_x = toObject.m_x * attractionSpeed;
                velocity.m_z = toObject.m_z * attractionSpeed;
                velocity.m_y = toObject.m_y * attractionSpeed;
            }
        }
    }
    else
    {
        gameManager -> crosshairPtr -> setRadius(2);
    }

    EKeyAlreadyPressed = isEKeyPressed;
}


