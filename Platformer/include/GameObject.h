#pragma once

#include "Model.h"
//#include "GLTexture.h"
#include "LibMath/Matrix.h"
#include "LibMath/Geometry3D.h"

#include <string>

class GameObject
{
public:
	GameObject();  
	GameObject(const std::string& name, std::shared_ptr<Model> model);
	GameObject(const std::string& name, const std::string& tag, std::shared_ptr<Model> model);

	void	computeColliderFromModel();
	void	update(float deltaTime);
	void	updateCollider(LibMath::Matrix4& worldTransform);
	void	draw(Shader* shader);
	void	draw(Shader* shader, LibMath::Matrix4& worldTransform);

	const std::string&	getName() const;

	// transform
	LibMath::Vector3 position;

	LibMath::Prism collider;

	bool isDoor = false;
	bool isMoving = false;

	LibMath::Vector4 color = LibMath::Vector4(0, 0, 0, 1);
	std::string tag;


private:
	LibMath::Prism		colliderAtOrigin;
	std::string			name;

	std::shared_ptr<Model>	model;
};