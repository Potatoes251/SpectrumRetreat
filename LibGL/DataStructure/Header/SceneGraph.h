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

    void draw()
    {
        if (root)
            root->draw();
    }
};