//
//  renderable.hpp
//  SnakeGL
//
//  Created by Keegan Bilodeau on 5/20/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#ifndef renderable_hpp
#define renderable_hpp

#include <stdio.h>
#include <vector>

#include <glad/glad.h>

struct Vertex
{
    float position[3];
    float color[3];
    float texCoord[2];
};

struct Quad
{
    Vertex vertices[4];
};

struct Cube
{
    Quad quads[6];
};

void loadTexture(std::string textureName, unsigned int &texture, bool alpha = false);

void generateTriVAO(unsigned int &VAO, float w, float h);
void generateTriVAO(unsigned int &VAO, float w, float h, float r, float g, float b, bool texture = false);

void generateQuadVAO(unsigned int &VAO, float w, float h, bool texture = false);
void generateQuadVAO(unsigned int &VAO, float w, float h, float r, float g, float b, bool texture = false);

void generateCubeVAO(unsigned int &VAO, float w, float h, bool texture = false);
void generateCubeVAO(unsigned int &VAO, float w, float h, float r, float g, float b, bool texture = false);

#endif /* renderable_hpp */
