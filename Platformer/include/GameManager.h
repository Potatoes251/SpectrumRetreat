#include "SceneGraph.h"
#include "GameObject.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Player.h"
#include "Physics.h"

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
    Player player;
    /*std::unique_ptr<Shader>*/Shader* shader;

private:
    ResourceManager resourceManager;
};