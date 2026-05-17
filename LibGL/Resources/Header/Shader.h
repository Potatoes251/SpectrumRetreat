#pragma once

#include <string>
#include <filesystem>
#include <glad/glad.h>

#include "ResourceManager.h"
#include "../../LibMath/Header/LibMath/Matrix.h"

class Shader : public IResource 
{
public:
    Shader();
    ~Shader();

    bool    SetVertexShader(const std::filesystem::path& filename);
    bool    SetFragmentShader(const std::filesystem::path& filename);
    bool    Link();

    void    setMat4(const std::string& name, LibMath::Matrix4& mat);
    void    setVec4(const std::string& name, const LibMath::Vector4& value) const;

    void    Use() const;
    GLuint  GetID() const { return m_programID; }

private:
    GLuint  m_programID;
    GLuint  m_vertexShaderID;
    GLuint  m_fragmentShaderID;

    std::string     LoadFile(const std::filesystem::path& filename);

    bool    CompileShader(GLuint shaderID, const std::string& source);
};
