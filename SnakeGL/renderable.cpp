//
//  renderable.cpp
//  SnakeGL
//
//  Created by Keegan Bilodeau on 5/20/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#include "renderable.hpp"

#include <stb_image.h>
#include <string>

void loadTexture(std::string textureName, unsigned int &texture, bool alpha)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    
    unsigned char* data = stbi_load(textureName.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        if (alpha)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        puts("Failed to load texture!");
    
    stbi_image_free(data);
}

void generateTriVAO(unsigned int &VAO, float w, float h)
{
        generateTriVAO(VAO, w, h, 1.0f, 1.0f, 1.0f);
}

void generateTriVAO(unsigned int &VAO, float w, float h, float r, float g, float b, bool texture)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    Vertex vertices[] = {
        Vertex{{0.0f,  h, 0.0f}, {r, g, b}, {1.0f, 0.0f}},
        Vertex{{   w, -h, 0.0f}, {r, g, b}, {1.0f, 1.0f}},
        Vertex{{  -w, -h, 0.0f}, {r, g, b}, {0.0f, 1.0f}}
    };
    
    unsigned int VBO;
    
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    if (texture)
    {
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    
    glBindVertexArray(0);
    
    glDeleteBuffers(1, &VBO);
}

void generateQuadVAO(unsigned int &VAO, float w, float h, bool texture)
{
    generateQuadVAO(VAO, w, h, 1.0f, 1.0f, 1.0f, texture);
}

void generateQuadVAO(unsigned int &VAO, float w, float h, float r, float g, float b, bool texture)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    Quad vertices[] = {
        Quad {
            Vertex{{ w,  h, 0.0f}, {r, g, b}, {1.0f, 0.0f}},
            Vertex{{ w, -h, 0.0f}, {r, g, b}, {1.0f, 1.0f}},
            Vertex{{-w, -h, 0.0f}, {r, g, b}, {0.0f, 1.0f}},
            Vertex{{-w,  h, 0.0f}, {r, g, b}, {0.0f, 0.0f}}
        }
    };
    
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    
    unsigned int VBO, EBO;
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    if (texture)
    {
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    
    glBindVertexArray(0);
    
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void generateCubeVAO(unsigned int &VAO, float w, float h, bool texture)
{
    generateCubeVAO(VAO, w, h, 1.0f, 1.0f, 1.0f, texture);
}

void generateCubeVAO(unsigned int &VAO, float w, float h, float r, float g, float b, bool texture)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    Cube vertices[] = {
        Cube
        {
            // Back
            Quad
            {
                Vertex{{ w,  h, -0.5f}, {r, g, b}, {1.0f, 0.0f}},
                Vertex{{ w, -h, -0.5f}, {r, g, b}, {1.0f, 1.0f}},
                Vertex{{-w, -h, -0.5f}, {r, g, b}, {0.0f, 1.0f}},
                Vertex{{-w,  h, -0.5f}, {r, g, b}, {0.0f, 0.0f}}
            },
            // Front
            Quad
            {
                Vertex{{ w,  h, 0.5f}, {r, g, b}, {1.0f, 0.0f}},
                Vertex{{ w, -h, 0.5f}, {r, g, b}, {1.0f, 1.0f}},
                Vertex{{-w, -h, 0.5f}, {r, g, b}, {0.0f, 1.0f}},
                Vertex{{-w,  h, 0.5f}, {r, g, b}, {0.0f, 0.0f}}
            },
            // Bottom
            Quad
            {
                Vertex{{ w, -h, -0.5f}, {r, g, b}, {1.0f, 0.0f}},
                Vertex{{ w, -h,  0.5f}, {r, g, b}, {1.0f, 1.0f}},
                Vertex{{-w, -h,  0.5f}, {r, g, b}, {0.0f, 1.0f}},
                Vertex{{-w, -h, -0.5f}, {r, g, b}, {0.0f, 0.0f}}
            },
            // Top
            Quad
            {
                Vertex{{ w,  h, -0.5f}, {r, g, b}, {1.0f, 0.0f}},
                Vertex{{ w,  h,  0.5f}, {r, g, b}, {1.0f, 1.0f}},
                Vertex{{-w,  h,  0.5f}, {r, g, b}, {0.0f, 1.0f}},
                Vertex{{-w,  h, -0.5f}, {r, g, b}, {0.0f, 0.0f}}
            },
            // Right
            Quad
            {
                Vertex{{ w,  h, -0.5f}, {r, g, b}, {1.0f, 0.0f}},
                Vertex{{ w, -h, -0.5f}, {r, g, b}, {1.0f, 1.0f}},
                Vertex{{ w, -h,  0.5f}, {r, g, b}, {0.0f, 1.0f}},
                Vertex{{ w,  h,  0.5f}, {r, g, b}, {0.0f, 0.0f}}
            },
            Quad
            {
                Vertex{{-w,  h,  0.5f}, {r, g, b}, {1.0f, 0.0f}},
                Vertex{{-w, -h,  0.5f}, {r, g, b}, {1.0f, 1.0f}},
                Vertex{{-w, -h, -0.5f}, {r, g, b}, {0.0f, 1.0f}},
                Vertex{{-w,  h, -0.5f}, {r, g, b}, {0.0f, 0.0f}}
            }
        }
    };
    
    unsigned int indices[] = {
         0,  1, 3,
         1,  2, 3,
         4,  5, 7,
         5,  6, 7,
         8,  9, 11,
         9, 10, 11,
        12, 13, 15,
        13, 14, 15,
        16, 17, 19,
        17, 18, 19,
        20, 21, 23,
        21, 22, 23
    };
    
    unsigned int VBO, EBO;
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    if (texture)
    {
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    
    glBindVertexArray(0);
    
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
