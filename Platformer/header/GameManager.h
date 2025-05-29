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

	SceneGraph<GameObject> scene;
	bool isRunning;
};