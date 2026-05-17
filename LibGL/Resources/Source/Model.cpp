#include "Model.h"

bool Model::load(const std::string& filename)
{
    std::ifstream file(filename);
    
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open OBJ file: " << filename << std::endl;

        return false;
    }
    reset();

    std::string line;

    while (std::getline(file, line))  processLine(line);
    vertices = std::move(tempVertices);

    return true;
}

void    Model::reset()
{
    positions.clear();
    normals.clear();
    uvs.clear();
    tempVertices.clear();
}

void    Model::processLine(const std::string& line)
{
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;
    if (prefix == "v")  parseVertex(iss);
    else if (prefix == "vt") parseUV(iss);
    else if (prefix == "vn") parseNormal(iss);
    else if (prefix == "f")  parseFace(iss);
}

void    Model::parseVertex(std::istringstream& iss)
{
    LibMath::Vector3 pos; iss >> pos.m_x >> pos.m_y >> pos.m_z;
    positions.push_back(pos);
}

void    Model::parseUV(std::istringstream& iss)
{
    LibMath::Vector2 uv; iss >> uv.m_x >> uv.m_y;
    uvs.push_back(uv);
}

void    Model::parseNormal(std::istringstream& iss)
{
    LibMath::Vector3 norm; iss >> norm.m_x >> norm.m_y >> norm.m_z;
    normals.push_back(norm);
}

void    Model::parseFace(std::istringstream& iss)
{
    std::string token;
    while (iss >> token) parseFaceVertex(token);
}

void    Model::parseFaceVertex(const std::string& token)
{
    int posIdx = 0, uvIdx = 0, normIdx = 0;

    // Find slashes
    size_t firstSlash = token.find('/');
    size_t secondSlash = token.find('/', firstSlash + 1);

    // Case: only position (v)
    if (firstSlash == std::string::npos) 
    {
        posIdx = std::stoi(token);
    }
    // Case: position + uv (v/vt)
    else if (secondSlash == std::string::npos) 
    {
        posIdx = std::stoi(token.substr(0, firstSlash));
        uvIdx = std::stoi(token.substr(firstSlash + 1));
    }
    // Case: position + normal (v//vn)
    else if (secondSlash == firstSlash + 1)
    {
        posIdx = std::stoi(token.substr(0, firstSlash));
        normIdx = std::stoi(token.substr(secondSlash + 1));
    }
    // Case: position + uv + normal (v/vt/vn)
    else 
    {
        posIdx = std::stoi(token.substr(0, firstSlash));
        uvIdx = std::stoi(token.substr(firstSlash + 1, secondSlash - firstSlash - 1));
        normIdx = std::stoi(token.substr(secondSlash + 1));
    }

    // Convert OBJ 1-based index to 0-based
    auto fixIndex = [](int idx, int size)
    {
        return (idx > 0) ? (idx - 1) : (size + idx); // supports negative indices
    };

    int pi = fixIndex(posIdx, (int)positions.size());
    int ti = fixIndex(uvIdx, (int)uvs.size());
    int ni = fixIndex(normIdx, (int)normals.size());

    if (pi < 0 || pi >= (int)positions.size()) return;

    Vertex v;

    v.Position = positions[pi];
    v.TextureUV = (ti >= 0 && ti < (int)uvs.size()) ? uvs[ti] : LibMath::Vector2();
    v.Normal = (ni >= 0 && ni < (int)normals.size()) ? normals[ni] : LibMath::Vector3();

    tempVertices.push_back(v);
}

void    Model::uploadToGPU()
{
    // 1) Create and fill VBO
    m_vertexBuffer.init(GL_ARRAY_BUFFER);
    m_vertexBuffer.bind();
    m_vertexBuffer.setData(vertices.data(), vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);
    m_vertexBuffer.unbind();

    // 2) Setup VAO
    m_vertexArray.init();
    m_vertexArray.bind();
    m_vertexBuffer.bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureUV));
    glEnableVertexAttribArray(2);

    m_vertexBuffer.unbind();
    m_vertexArray.unbind();
}

void    Model::unloadFromGPU()
{
    m_vertexBuffer.destroy();   // Calls glDeleteBuffers internally
    m_vertexArray.destroy();    // Calls glDeleteVertexArrays internally
}

void    Model::draw() const
{
    m_vertexArray.bind();
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
    m_vertexArray.unbind();
}

void    Model::draw(LibMath::Matrix4& transform, Shader* shader) const
{
    shader->setMat4("model", transform);

    draw();  // Calls your regular draw() that issues OpenGL draw calls
}