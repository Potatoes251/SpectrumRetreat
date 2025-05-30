#include "SceneGraph.h"
#include "GameObject.h"
#include "Camera.h"
#include "ResourceManager.h"

class GameManager 
{
public:
    GameManager();
    ~GameManager();

    // Initialize resources and scene
    void init();

    // Update logic (scene, camera) each frame
    void update(float deltaTime);

    // Render the scene
    void draw();

    // Handle mouse movement for camera
    void processMouseMovement(double xpos, double ypos);

    SceneGraph<GameObject> scene;
private:
    ResourceManager resourceManager;

    std::unique_ptr<Shader> shader;
    CameraFPS camera = CameraFPS(
        LibMath::Vector3(0.0f, 0.0f, 3.0f),
        LibMath::Vector3(0.0f, 1.0f, 0.0f));
};