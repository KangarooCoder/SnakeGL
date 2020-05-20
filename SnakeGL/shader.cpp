//
//  shader.cpp
//  SnakeGL
//
//  Created by Keegan Bilodeau on 5/20/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#include "shader.hpp"

#include <fstream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // Temporary strings to hold source code
    std::string vertexCode, fragmentCode;
    
    // Loads source from both of the shaders' files
    loadShaderFile(vertexCode, vertexPath);
    loadShaderFile(fragmentCode, fragmentPath);
    
    // Stores the shader source in the apporpriate format
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    // IDs of both shaders
    unsigned int vertexShader, fragmentShader;
    
    // Loads and initializes both shaders
    loadShader(vertexShader, vShaderCode, GL_VERTEX_SHADER);
    loadShader(fragmentShader, fShaderCode, GL_FRAGMENT_SHADER);
    
    // Creates shader program
    ID = glCreateProgram();
    // Attaches both shaders
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Connects the program and shaders together
    glLinkProgram(ID);
    
    // Linking status check variables
    int success;
    char infoLog[512];
    
    // Get the program's linking status and check for failure
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        // Get program's info log
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        // Output failure to console
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
    
    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::loadShader(unsigned int &shader, const char* shaderSource, int shaderType)
{
    // Creates the shader
    shader = glCreateShader(shaderType);
    // Attaches the source code
    glShaderSource(shader, 1, &shaderSource, NULL);
    // Comiles the source code
    glCompileShader(shader);
    
    // Compilation check variables
    int success;
    char infoLog[512];
    
    // Retrieve compilation status and check for failure
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        // Retrieve log regarding compilation failure and store it
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        
        // Ouput failure to console with info log attached
        if (shaderType == GL_VERTEX_SHADER)
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        else if (shaderType == GL_FRAGMENT_SHADER)
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
        else
            printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
}

void Shader::loadShaderFile(std::string &shaderCode, const char *filePath)
{
    // File stream for loading shader source
    std::ifstream shaderFile;
    
    // Enables the throwing of exceptions
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    // Attempt to read file and load the source into string
    try
    {
        // Opens the file
        shaderFile.open(filePath);
        
        // String stream for reading the file source
        std::stringstream shaderStream;
        
        // Reads the file buffer into the string stream
        shaderStream << shaderFile.rdbuf();
        
        // Closes the file stream
        shaderFile.close();
        
        // Stores the newly read source into output variable
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        // Output failure to the console
        printf("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n%s\n", e.what());
    }
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setUniform(const char *name, int value)
{
    int uniformLoc = glGetUniformLocation(ID, name);
    glUniform1i(uniformLoc, value);
}

void Shader::setUniform(const char *name, bool value)
{
    setUniform(name, (int) value);
}

void Shader::setUniform(const char *name, float value)
{
    int uniformLoc = glGetUniformLocation(ID, name);
    glUniform1f(uniformLoc, value);
}

void Shader::setUniform(const char *name, float x, float y, float z)
{
    int uniformLoc = glGetUniformLocation(ID, name);
    glUniform3f(uniformLoc, x, y, z);
}

void Shader::setUniform(const char *name, glm::mat4 value)
{
    int uniformLoc = glGetUniformLocation(ID, name);
    glUniform3fv(uniformLoc, 1, glm::value_ptr(value));
}

void Shader::setUniform(const char *name, glm::vec3 value)
{
    int uniformLoc = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(value));
}
