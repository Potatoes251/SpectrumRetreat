#pragma once
#include "ResourceManager.h"
#include "Vertex.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <tuple>

#include "Buffer.h"
#include "VertexAttributes.h"
#include "../../LibMath/Header/LibMath/Matrix.h"
#include "Shader.h"

class Model : public IResource {
public:
    std::vector<Vertex>     vertices;
    std::vector<uint32_t>   indices;

    ~Model()
    {
        unloadFromGPU();
    }

    bool    load(const std::string& filename);
    void    reset();
    void    processLine(const std::string& line);
    void    parseVertex(std::istringstream& iss);
    void    parseUV(std::istringstream& iss);
    void    parseNormal(std::istringstream& iss);
    void    parseFace(std::istringstream& iss);
    void    parseFaceVertex(const std::string& token);

    // GPU integration
    void    uploadToGPU();
    void    unloadFromGPU();
    void    draw() const;
    void    draw(LibMath::Matrix4& transform, Shader* shader) const;

private:
    std::vector<LibMath::Vector3>   positions;
    std::vector<LibMath::Vector3>   normals;
    std::vector<LibMath::Vector2>   uvs;
    std::vector<Vertex>             tempVertices;

    std::map<std::tuple<int, int, int>, uint32_t>   uniqueVertexMap;

    Buffer  m_vertexBuffer;          // VBO
    Buffer  m_indexBuffer;           // Index Buffer Object (IBO)

    VertexAttributes    m_vertexArray;  // VAO
};