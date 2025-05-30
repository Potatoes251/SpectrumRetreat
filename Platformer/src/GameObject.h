#include "Model.h"

#include <string.h>

class GameObject
{
public:
	GameObject(const std::string& name, std::shared_ptr<Model> model);

	//void draw(const LibMath::Matrix4 worldTransform);

	//void setCollisionBox(const LibMath::Prism box);
	//const LibMath::Prism& getCollisionBox() const;

	void update(float deltaTime);
	void draw(Shader* shader);
	void draw(Shader* shader, LibMath::Matrix4& worldTransform);

	const std::string& getName() const;

	// transform
	LibMath::Vector3 position;
	LibMath::Vector3 rotation;
	LibMath::Vector3 scale;

private:
	std::string name;
	std::shared_ptr<Model> model;
	//LibMath::Prism collider;
};