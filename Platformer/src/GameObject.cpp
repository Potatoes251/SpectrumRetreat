#include "GameObject.h"



GameObject::GameObject()
    : name(""),
    model(nullptr),
    position(0.0f, 0.0f, 0.0f),
    collider(),
    color(0, 0, 0, 1)
{
}

GameObject::GameObject(const std::string& name, std::shared_ptr<Model> model)
    : name(name), tag("null"), model(std::move(model)), position(0), color(1, 1, 1, 1)
{
    computeColliderFromModel();
}

GameObject::GameObject(const std::string& name, const std::string& tag, std::shared_ptr<Model> model)
    : name(name), tag(tag), model(std::move(model)), position(0), color(1, 1, 1, 1)
{
    computeColliderFromModel();
}

void    GameObject::computeColliderFromModel()
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
    LibMath::Point3D center(minPointCollider.m_x + colliderSize.m_x / 2.f,
        minPointCollider.m_y + colliderSize.m_y / 2.f,
        minPointCollider.m_z + colliderSize.m_z / 2.f);

    collider = LibMath::Prism(center, colliderSize.m_x, colliderSize.m_y, colliderSize.m_z);
    colliderAtOrigin = collider;
}

void GameObject::updateCollider(LibMath::Matrix4& worldTransform)
{
    LibMath::Point3D position(
        worldTransform[3][0] + colliderAtOrigin.getCenter().getX(),
        worldTransform[3][1] + colliderAtOrigin.getCenter().getY(),
        worldTransform[3][2] + colliderAtOrigin.getCenter().getZ());

    LibMath::Vector3 colliderSize(
        worldTransform[0][0] * colliderAtOrigin.getWidth(),
        worldTransform[1][1] * colliderAtOrigin.getHeight(),
        worldTransform[2][2] * colliderAtOrigin.getDepth());

    collider = LibMath::Prism(position, colliderSize.m_x, colliderSize.m_y, colliderSize.m_z);

    this -> position = LibMath::Vector3(worldTransform[3][0], worldTransform[3][1], worldTransform[3][2]);
}

void GameObject::draw(Shader* shader, LibMath::Matrix4& worldTransform) 
{
    updateCollider(worldTransform);
    if (!model) return;

    shader -> setMat4("model", worldTransform);
    model -> draw();
}

const   std::string& GameObject::getName() const
{
	return name;
}
