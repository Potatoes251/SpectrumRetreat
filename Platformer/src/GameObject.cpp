#include "GameObject.h"



GameObject::GameObject(const std::string& name, std::shared_ptr<Model> model)
	: name(name), model(std::move(model)), position(0), rotation(0), scale(1) 
{
    //this->model;
    LibMath::Vector3 minPointCollider = this->model->vertices[0].Position;
    LibMath::Vector3 maxPointCollider = minPointCollider;

    for (int i = 0; i < this->model->vertices.size(); ++i)
    {
        LibMath::Vector3 position = this->model->vertices[i].Position;
        for (int j = 0; j < 3; ++j)
        {
            if (minPointCollider[j] > position[j])
            {
                minPointCollider[j] = position[j];
            }
            if (maxPointCollider[j] < position[j])
            {
                maxPointCollider[j] = position[j];
            }
        }
    }

    LibMath::Vector3 colliderSize = maxPointCollider - minPointCollider;
    LibMath::Point3D center (minPointCollider.m_x + colliderSize.m_x, 
                            minPointCollider.m_y + colliderSize.m_y, 
                            minPointCollider.m_z + colliderSize.m_z);

    collider = LibMath::Prism(center, colliderSize.m_x, colliderSize.m_y, colliderSize.m_z);
}

void GameObject::update(float deltaTime)
{
	rotation.m_y += 1.0f * deltaTime;

    collider.setCenter(position);
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
