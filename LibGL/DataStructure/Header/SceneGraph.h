#pragma once

#include "SceneNode.h"

template<typename T>
class SceneGraph
{
public:
    using Node = SceneNode<T>;
    using Ptr = typename Node::Ptr;

    // Root node of the scene graph
    Ptr root;

    SceneGraph()
    {
        root = std::make_shared<Node>();
    }

    /**
     * Add a node to the graph; if parent is null, attach to root
     */
    void addNode(const Ptr& node, const Ptr& parent = nullptr)
    {
        Ptr target = parent ? parent : root;
        target->addChild(node);
    }

    /**
     * Update all nodes: logic and transforms
     * @param deltaTime elapsed time since last frame
     */
    void update(float deltaTime)
    {
        if (root)
        {
            root->update(deltaTime);
            root->updateTransform();
        }
    }

    /**
     * Draw all nodes with the provided shader
     * @param shader Shader program to use for drawing
     */
    void draw(Shader* shader)
    {
        if (root)
            root->draw(shader);
    }

    //T* findObjectByName(const std::string& name) 
    //{
    //    if (!root) return nullptr;

    //    return findRec(root, name);
    //}


    Ptr findNodeByName(const std::string& name) {
        if (!root) return nullptr;
        return findNodeRec(root, name);
    }

private:
    //T* findRec(const typename SceneNode<T>::Ptr& node, const std::string& name) 
    //{
    //    if (node->object && node->object->getName() == name)
    //        return node->object.get();

    //    for (auto& child : node->children) 
    //    {
    //        if (T* found = findRec(child, name))
    //            return found;
    //    }

    //    return nullptr;
    //}

    Ptr findNodeRec(const Ptr& node, const std::string& name) {
        if (node->object && node->object->getName() == name)
            return node;
        for (auto& child : node->children) {
            if (auto found = findNodeRec(child, name))
                return found;
        }
        return nullptr;
    }
};