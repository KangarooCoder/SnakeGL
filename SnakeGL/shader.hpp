//
//  shader.hpp
//  SnakeGL
//
//  Created by Keegan Bilodeau on 5/20/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include <stdio.h>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    // ID of the shader program that openGL will use
    unsigned int ID;
    
    // Creates the shader program
    Shader(const char* vertexPath, const char* fragmentPath);
    
    // Makes this shader the active shader program
    void use();
    
    // Various overloads for setting shader uniforms
    void setUniform(const char* name, bool value);
    void setUniform(const char* name, int value);
    void setUniform(const char* name, float value);
    void setUniform(const char* name, float x, float y, float z);
    void setUniform(const char* name, glm::mat4 value);
    void setUniform(const char* name, glm::vec3 value);
    
private:
    // Loads an individual shader
    void loadShader(unsigned int &shader, const char* shaderSource, int shaderType);
    
    // Loads a shader file
    void loadShaderFile(std::string &shaderCode, const char* filePath);
};

#endif /* shader_hpp */
