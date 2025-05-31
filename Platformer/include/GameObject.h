#pragma once

#include "Model.h"
//#include "GLTexture.h"
#include "LibMath/Matrix.h"
#include "LibMath/Geometry3D.h"

#include <string>

class GameObject
{
public:
	GameObject(const std::string& name, std::shared_ptr<Model> model);

	void update(float deltaTime);
	void updateCollider();
	void draw(Shader* shader);
	void draw(Shader* shader, LibMath::Matrix4& worldTransform);

	const std::string& getName() const;

	// transform
	LibMath::Vector3 position;
	LibMath::Vector3 rotation;
	LibMath::Vector3 scale;

	LibMath::Matrix4 localTransform = LibMath::Matrix4::getIdentity();
	LibMath::Matrix4 worldTransform = LibMath::Matrix4::getIdentity();

	LibMath::Prism collider;

	LibMath::Vector4 color = LibMath::Vector4(0, 0, 0, 1);

private:
	LibMath::Vector3 centerColliderOffset;
	std::string name;
	std::shared_ptr<Model> model;
};