#pragma once

#include "SceneNode.h"

template<typename T>
class SceneGraph
{
public:
    using Node = SceneNode<T>;
    using Ptr = typename Node::Ptr;

    Ptr root;

    SceneGraph()
    {
        root = std::make_shared<Node>();
    }

    void update()
    {
        if (root)
            root->updateTransform();
    }

    void draw(Shader* shader)
    {
        if (root)
            root->draw(shader);
    }

    void addNode(const Ptr& node, const Ptr& parent = nullptr)
    {
        Ptr target = parent ? parent : root;
        target->addChild(node);
    }

    T* findObjectByName(const std::string& name) 
    {
        if (!root) return nullptr;

        return findRec(root, name);
    }


    Ptr findNodeByName(const std::string& name) {
        if (!root) return nullptr;
        return findNodeRec(root, name);
    }

private:
    T* findRec(const typename SceneNode<T>::Ptr& node, const std::string& name) 
    {
        if (node->object && node->object->getName() == name)
            return node->object.get();

        for (auto& child : node->children) 
        {
            if (T* found = findRec(child, name))
                return found;
        }

        return nullptr;
    }

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