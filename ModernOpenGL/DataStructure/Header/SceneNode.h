#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "../../LibMath/Header/LibMath/Matrix.h"
#include "Shader.h"

template<typename T>
class SceneNode
{
public:
    using Ptr = std::shared_ptr<SceneNode<T>>;

    T* model = nullptr;
    Shader* shader = nullptr;

    LibMath::Matrix4 localTransform = LibMath::Matrix4::getIdentity();
    LibMath::Matrix4 worldTransform = LibMath::Matrix4::getIdentity();

    SceneNode<T>* parent = nullptr;
    std::vector<Ptr> children;

    SceneNode() = default;
    SceneNode(T* model, Shader* shader = nullptr)
        : model(model), shader(shader) {
    }

    void setParent(SceneNode<T>* newParent)
    {
        parent = newParent;
    }

    void addChild(const Ptr& child)
    {
        child -> setParent(this);
        children.push_back(child);
    }

    void removeChild(const Ptr& child)
    {
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
    }

    void updateTransform()
    {
        worldTransform = (parent ? parent -> worldTransform * localTransform : localTransform);

        for (auto& child : children)
        {
            child -> updateTransform();
        } 
    }

    void draw(Shader* shader)
    {
        if (model && shader)
        {
            shader -> Use();  // Activate the node's own shader
            model -> draw(worldTransform, shader);  // Use this shader
        }

        for (auto& child : children)
            child -> draw(shader);
    }
};