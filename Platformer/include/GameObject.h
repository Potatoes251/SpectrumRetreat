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
	// set the collider based on the model used
	void	computeColliderFromModel();

	// move the collider to the position of the player
	void	updateCollider(LibMath::Matrix4& worldTransform);
	void	draw(Shader* shader, LibMath::Matrix4& worldTransform);

	const std::string&		getName() const;

	// transform
	LibMath::Vector3		m_position;

	LibMath::Prism			m_collider;


	LibMath::Vector4		m_color = LibMath::Vector4(0, 0, 0, 1);
	std::string				m_tag;
private:
	LibMath::Prism			m_colliderAtOrigin;
	std::string				m_name;

	std::shared_ptr<Model>	m_model;
};