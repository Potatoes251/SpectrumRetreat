#include "GameObject.h"



GameObject::GameObject()
    : m_name(""),
    m_model(nullptr),
    m_position(0.0f, 0.0f, 0.0f),
    m_collider(),
    m_color(0, 0, 0, 1)
{
}

GameObject::GameObject(const std::string& name, std::shared_ptr<Model> model)
    : m_name(name), m_tag("null"), m_model(std::move(model)), m_position(0), m_color(1, 1, 1, 1)
{
    computeColliderFromModel();
}

GameObject::GameObject(const std::string& name, const std::string& tag, std::shared_ptr<Model> model)
    : m_name(name), m_tag(tag), m_model(std::move(model)), m_position(0), m_color(1, 1, 1, 1)
{
    computeColliderFromModel();
}

void    GameObject::computeColliderFromModel()
{
    // take the first point
    LibMath::Vector3 minPointCollider = this->m_model->vertices[0].Position;
    LibMath::Vector3 maxPointCollider = minPointCollider;

    for (int i = 0; i < this->m_model->vertices.size(); ++i)
    {
        // try to find the min and max coordinate of the collider on x, y, z
        LibMath::Vector3 position = this->m_model->vertices[i].Position;
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
    // set the size as the difference between min and max
    LibMath::Vector3 colliderSize = maxPointCollider - minPointCollider;
    // center is half way point between min and max
    LibMath::Point3D center(minPointCollider.m_x + colliderSize.m_x / 2.f,
        minPointCollider.m_y + colliderSize.m_y / 2.f,
        minPointCollider.m_z + colliderSize.m_z / 2.f);

    // create the collider and the one at the origin
    m_collider = LibMath::Prism(center, colliderSize.m_x, colliderSize.m_y, colliderSize.m_z);
    m_colliderAtOrigin = m_collider;
}

void GameObject::updateCollider(LibMath::Matrix4& worldTransform)
{
    // move the collider to the position of the player
    LibMath::Point3D position(
        worldTransform[3][0] + m_colliderAtOrigin.getCenter().getX(),
        worldTransform[3][1] + m_colliderAtOrigin.getCenter().getY(),
        worldTransform[3][2] + m_colliderAtOrigin.getCenter().getZ());

    LibMath::Vector3 colliderSize(
        worldTransform[0][0] * m_colliderAtOrigin.getWidth(),
        worldTransform[1][1] * m_colliderAtOrigin.getHeight(),
        worldTransform[2][2] * m_colliderAtOrigin.getDepth());

    m_collider = LibMath::Prism(position, colliderSize.m_x, colliderSize.m_y, colliderSize.m_z);

    this -> m_position = LibMath::Vector3(worldTransform[3][0], worldTransform[3][1], worldTransform[3][2]);
}

void    GameObject::draw(Shader* shader, LibMath::Matrix4& worldTransform) 
{
    updateCollider(worldTransform);

    if (!m_model) return;

    shader -> setMat4("model", worldTransform);
    m_model -> draw();
}

const std::string&      GameObject::getName() const
{
	return m_name;
}
