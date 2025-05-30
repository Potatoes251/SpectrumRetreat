#include "../header/GameObject.h"
//#include "GameObject.h"
//#include "GLTexture.h"

//GameObject::GameObject(const std::string& name, std::shared_ptr<Model> model)
//    : name(name), model(std::move(model)), position(0), rotation(0), scale(1) {}

GameObject::GameObject(const std::string& name, std::shared_ptr<Model> model/*,
    std::shared_ptr<GLTexture> texture*/)
	: name(name), model(std::move(model))/*, texture(std::move(texture))*/, position(0), rotation(0), scale(1) { }

//void GameObject::draw(const LibMath::Matrix4 worldTransform)
//{std::cou
//	model->draw();
//}

//void setCollisionBox(const LibMath::Prism box)
//{
//	collider = box;
//}

//const LibMath::Prism& GameObject::getCollisionBox() const
//{
//	return collider;
//}

void GameObject::update(float deltaTime)
{
	rotation.m_y += 1.0f * deltaTime;
}
	
void GameObject::draw(Shader* shader)
{
    if (!model) return;

    LibMath::Matrix4 modelMatrix =
        LibMath::Matrix4::createScale(scale) *
        LibMath::Matrix4::createRotationY(rotation.m_y) *
        LibMath::Matrix4::createTranslation(position);

    shader->setMat4("model", modelMatrix);
    model->draw();
}

void GameObject::draw(Shader* shader, LibMath::Matrix4& worldTransform) 
{
    if (!model) return;

    shader->setMat4("model", worldTransform);
    model->draw();
}

const std::string& GameObject::getName() const
{
	return name;
}
