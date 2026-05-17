#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "LibMath/Matrix.h"
#include "Shader.h"

template<typename T>
class SceneNode
{
public:
    bool    dirty = true;
    
    Shader*     shader = nullptr;

    SceneNode*  parent = nullptr;

    using Ptr = std::shared_ptr<SceneNode<T>>;
    std::shared_ptr<T>  object;

    LibMath::Matrix4    localTransform = LibMath::Matrix4::getIdentity();
    LibMath::Matrix4    worldTransform = LibMath::Matrix4::getIdentity();

    std::vector<Ptr>    children;

    SceneNode() = default;

    
    // Construct a node with an object and optional shader
    
    SceneNode(std::shared_ptr<T> obj, Shader* shader = nullptr)
        : object(obj), shader(shader) 
    {
    }

    void    setParent(SceneNode<T>* newParent)
    {
        parent = newParent;
        markDirty();
    }

    void    addChild(const Ptr& child)
    {
        child->setParent(this);
        children.push_back(child);
    }

    void    removeChild(const Ptr& child)
    {
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
    }

    void    removeAllChildren()
    {
        children.clear();
    }

    // Mark this node and descendants as needing world transform recomputation

    void    markDirty()
    {
        dirty = true;
        for (auto& c : children)
            c->markDirty();
    }

    
    // Update the logic of the object and children
    // param deltaTime elapsed time since last frame

    void    update(float deltaTime)
    {
        if (object)
            object->update(deltaTime);

        for (auto& c : children)
            c->update(deltaTime);
    }

    
    // Recompute worldTransform if dirty or parent changed
    
    void    updateTransform()
    {
        if (dirty || (parent && parent->dirty))
        {
            if(object)
            worldTransform = parent ? parent->worldTransform * localTransform : localTransform;
            dirty = false;
        }

        for (auto& c : children)
            c->updateTransform();
    }

    
    // Draw this node's object and its children using overrideShader if this node has none
    // param overrideShader fallback shader if node has none
    
    void    draw(Shader* overrideShader)
    {
        Shader* useShader = shader ? shader : overrideShader;

        if (object && useShader)
        {
            shader->Use();
            shader->setVec4("objectColor", object -> m_color);
            object->draw(useShader, worldTransform);
        }

        for (auto& c : children)
            c->draw(overrideShader);
    }
};