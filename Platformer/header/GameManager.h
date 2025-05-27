#include "SceneGraph.h"
#include "GameObject.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	void init();
	void run();

private:
	void processInput();
	void update();
	void render();

	//Player player;
	SceneGraph<GameObject> scene;
	bool isRunning;
};