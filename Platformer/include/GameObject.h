#include "Model.h"
//#include "GLTexture.h"
#include "../../LibMath/Header/LibMath/Matrix.h"

#include <string>

class GameObject
{
public:
	GameObject(const std::string& name, std::shared_ptr<Model> model);

	void update(float deltaTime);
	void draw(Shader* shader);
	void draw(Shader* shader, LibMath::Matrix4& worldTransform);

	const std::string& getName() const;

	// transform
	LibMath::Vector3 position;
	LibMath::Vector3 rotation;
	LibMath::Vector3 scale;

	LibMath::Matrix4 localTransform = LibMath::Matrix4::getIdentity();
	LibMath::Matrix4 worldTransform = LibMath::Matrix4::getIdentity();

private:
	std::string name;
	std::shared_ptr<Model> model;
	//LibMath::Prism collider;
};