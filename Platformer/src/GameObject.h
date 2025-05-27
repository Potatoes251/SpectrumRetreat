#include "Model.h"
#include "../header/Collider.h"

#include <string.h>

class GameObject
{
public:
	GameObject(const std::string& name, Model* model);

	void draw(const LibMath::Matrix4 worldTransform);

	//void setCollisionBox(const LibMath::Prism box);
	//const LibMath::Prism& getCollisionBox() const;

	const std::string& getName() const;

private:
	std::string name;
	Model* model;
	//LibMath::Prism collider;
};