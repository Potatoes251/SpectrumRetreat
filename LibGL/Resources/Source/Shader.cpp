#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"
#include "../../LibMath/Header/LibMath/Matrix.h"

Shader::Shader()
    : m_programID(glCreateProgram()), m_vertexShaderID(0), m_fragmentShaderID(0) {}

Shader::~Shader() 
{
    if (m_vertexShaderID) glDeleteShader(m_vertexShaderID);
    if (m_fragmentShaderID) glDeleteShader(m_fragmentShaderID);
    if (m_programID) glDeleteProgram(m_programID);
}

std::string     Shader::LoadFile(const std::filesystem::path& filename) 
{
    std::ifstream file(filename);

    if (!file.is_open()) 
    {
        std::cerr << "Failed to open shader file: " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

bool    Shader::CompileShader(GLuint shaderID, const std::string& source) 
{
    const char* src = source.c_str();

    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);

    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        char infoLog[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);

        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;

        return false;
    }

    return true;
}

bool    Shader::SetVertexShader(const std::filesystem::path& filename) 
{
    std::string source = LoadFile(filename);

    if (source.empty()) return false;

    m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    if (!CompileShader(m_vertexShaderID, source)) return false;

    glAttachShader(m_programID, m_vertexShaderID);

    return true;
}

bool    Shader::SetFragmentShader(const std::filesystem::path& filename) 
{
    std::string source = LoadFile(filename);

    if (source.empty()) return false;

    m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    if (!CompileShader(m_fragmentShaderID, source)) return false;

    glAttachShader(m_programID, m_fragmentShaderID);

    return true;
}

bool    Shader::Link() 
{
    glLinkProgram(m_programID);

    int success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);

    if (!success) 
    {
        char infoLog[512];
        glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);

        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;

        return false;
    }

    if (m_vertexShaderID) 
    {
        glDeleteShader(m_vertexShaderID);
        m_vertexShaderID = 0;
    }

    if (m_fragmentShaderID) 
    {
        glDeleteShader(m_fragmentShaderID);
        m_fragmentShaderID = 0;
    }

    return true;
}

void    Shader::setMat4(const std::string& name, LibMath::Matrix4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat.m_values[0][0]);
}

void    Shader::Use() const 
{
    glUseProgram(m_programID);
}

void    Shader::setVec4(const std::string& name, const LibMath::Vector4& value) const
{
    glUniform4fv(glGetUniformLocation(m_programID, name.c_str()), 1, (float*)&value);
}