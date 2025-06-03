#include "GameObject.h"



GameObject::GameObject()
    : name(""),
    tag(""),
    model(nullptr),
    position(0.0f, 0.0f, 0.0f),
    rotation(0.0f, 0.0f, 0.0f),
    scale(1.0f, 1.0f, 1.0f),
    centerColliderOffset(0.0f, 0.0f, 0.0f),
    collider(),
    color(0, 0, 0, 1),
    localTransform(LibMath::Matrix4::getIdentity()),
    worldTransform(LibMath::Matrix4::getIdentity())
{
}

GameObject::GameObject(const std::string& name, std::shared_ptr<Model> model)
    : name(name), tag("null"), model(std::move(model)), position(0), rotation(0), scale(1), color(1, 1, 1, 1)
{
    computeColliderFromModel();
}

GameObject::GameObject(const std::string& name, const std::string& tag, std::shared_ptr<Model> model)
	: name(name), tag(tag), model(std::move(model)), position(0), rotation(0), scale(1), color(1, 1, 1, 1)
{
    computeColliderFromModel();
}

void GameObject::computeColliderFromModel()
{
    if (!model || model->vertices.empty())
        return;

    LibMath::Vector3 minPointCollider = model->vertices[0].Position;
    LibMath::Vector3 maxPointCollider = minPointCollider;

    for (const auto& vertex : model->vertices)
    {
        LibMath::Vector3 position = vertex.Position;
        for (int j = 0; j < 3; ++j)
        {
            if (minPointCollider[j] > position[j])
                minPointCollider[j] = position[j];
            if (maxPointCollider[j] < position[j])
                maxPointCollider[j] = position[j];
        }
    }

    LibMath::Vector3 colliderSize = maxPointCollider - minPointCollider;
    LibMath::Point3D center(
        minPointCollider.m_x + colliderSize.m_x / 2.f,
        minPointCollider.m_y + colliderSize.m_y / 2.f,
        minPointCollider.m_z + colliderSize.m_z / 2.f
    );

    centerColliderOffset = LibMath::Vector3(center.getX(), center.getY(), center.getZ());
    collider = LibMath::Prism(center, colliderSize.m_x, colliderSize.m_y, colliderSize.m_z);

    std::cout << collider.getDepth() << '\t' << collider.getWidth() << '\t' << collider.getHeight() << '\n';
}

void GameObject::update(float deltaTime)
{
	rotation.m_y += 1.0f * deltaTime;
}

void GameObject::updateCollider()
{
    LibMath::Point3D position(
        worldTransform[3][0] + centerColliderOffset.m_x,
        worldTransform[3][1] + centerColliderOffset.m_y,
        worldTransform[3][2] + centerColliderOffset.m_z);

    LibMath::Vector3 colliderSize(
        worldTransform[0][0] * collider.getWidth(),
        worldTransform[1][1] * collider.getHeight(),
        worldTransform[1][2] * collider.getDepth());

    collider = LibMath::Prism(position, colliderSize.m_x, colliderSize.m_y, colliderSize.m_z);
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
    updateCollider();
    if (!model) return;

    shader->setMat4("model", worldTransform);
    model->draw();
}

const std::string& GameObject::getName() const
{
	return name;
}
