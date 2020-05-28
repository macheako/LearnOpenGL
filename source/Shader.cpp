//
//  Shader.cpp
//  Shaders
//
//  Created by Crunchy on 5/26/20.
//  Copyright © 2020 AnOrganization. All rights reserved.
//

#include <glad/3.3/glad.h>
#include "Shader.h"

Shader::Shader(const char * vertexPath, const char * fragmentPath)
{
    // Check file(s) exist
    if (!FileExists(vertexPath))
    {
        std::cerr << "ERROR::SHADER::VERTEX::FILE_NOT_FOUND Path=" << vertexPath << std::endl;
        return;
    }
    
    if (!FileExists(fragmentPath))
    {
        std::cerr << "ERROR::SHADER::FRAGMENT::FILE_NOT_FOUND Path=" << fragmentPath << std::endl;
        return;
    }
    
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    // ensure ifstream objects can throw exceptions
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return;
    }
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    // 2. Compile shaders
    unsigned int vertex, fragment;
    int success;
    char errlog[512];
    
    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");
    
    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");
    
    // Shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    
    // Check linking errors
    CheckCompileErrors(ID, "PROGRAM");
    
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

bool Shader::FileExists(const std::string& path)
{
    if (FILE *file = ::fopen(path.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

void Shader::CheckCompileErrors(GLuint shader, const std::string& type)
{
    GLint success;
    GLchar infoLog[1024];

    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            std::cout 
            << "ERROR::SHADER_COMPILATION_ERROR of type: " 
            << type << "\n" << infoLog 
            << "\n -- --------------------------------------------------- -- " 
            << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            std::cout 
            << "ERROR::PROGRAM_LINKING_ERROR of type: " 
            << type << "\n" << infoLog 
            << "\n -- --------------------------------------------------- -- " 
            << std::endl;
        }
    }
}