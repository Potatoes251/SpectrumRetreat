//#include "../header/GameObject.h"
#include "GameObject.h"

GameObject::GameObject(const std::string& name, Model* model)
	: name(name), model(model) {}

void GameObject::draw(const LibMath::Matrix4 worldTransform)
{
	model->draw();
}

//void setCollisionBox(const LibMath::Prism box)
//{
//	collider = box;
//}

//const LibMath::Prism& GameObject::getCollisionBox() const
//{
//	return collider;
//}
	
const std::string& GameObject::getName() const
{
	return name;
}
