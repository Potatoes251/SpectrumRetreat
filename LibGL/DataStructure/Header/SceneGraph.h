#pragma once

#include "SceneNode.h"

//template<typename T>
//class SceneGraph
//{
//public:
//    using Node = SceneNode<T>;
//    using Ptr = typename Node::Ptr;
//
//    Ptr root;
//
//    SceneGraph()
//    {
//        root = std::make_shared<Node>();
//    }
//
//    // Add a node to the graph; if parent is null, attach to root
//    void addNode(const Ptr& node, const Ptr& parent = nullptr)
//    {
//        Ptr target = parent ? parent : root;
//        target->addChild(node);
//    }
//
//    void update(float deltaTime)
//    {
//        if (root)
//            root->update(deltaTime);
//            root->updateTransform();
//    }
//
//    void draw(Shader* shader)
//    {
//        if (root)
//            root->draw(shader);
//    }
//};

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
};