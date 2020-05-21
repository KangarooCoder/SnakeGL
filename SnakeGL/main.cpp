//
//  main.cpp
//  SnakeGL
//
//  Created by Keegan Bilodeau on 5/20/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//
#define GL_SILENCE_DEPRECIATION
#define STB_IMAGE_IMPLEMENTATION

// Ignores library documentation issues
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

// Base libraries
#include <math.h>
#include <iostream>
#include <time.h>
#include <vector>

// Window handler libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Image libraries
#include <stb_image.h>

// Matrix and vector handling library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Restores clang to its normal state
#pragma clang diagnostic pop

// Callback functions for input handling
#include "callback.hpp"

// Wrapper for openGL shaders
#include "shader.hpp"

// VAO generators for various shapes
#include "renderable.hpp"

// Game window
GLFWwindow* window;

// Screen width and heigh constants
const float SCREEN_WIDTH = 750.0f;
const float SCREEN_HEIGHT = 750.0f;

// Function predefinitions
bool initWindow();
void processInput(GLFWwindow* window);

int main(int argc, const char * argv[])
{
    // Initialize game window and check for failure
    if (!initWindow())
    {
        // Output failure to console
        puts("Failed to initialize application!\n");
        
        // Return failure
        return EXIT_FAILURE;
    }
    
    unsigned int VAO;
    generateCubeVAO(VAO, 0.5, 0.5, true);
    
    unsigned int texture;
    loadTexture("resources/sprites.png", texture);
    
    Shader shader("resources/vShader.vert", "resources/fShader.frag");
    shader.use();
    shader.setUniform("texture1", 0);
    
    glm::vec3 direction;
    
    direction.x = cos(glm::radians(-90.0f)) * cos(glm::radians(0.0f));
    direction.y = sin(glm::radians(0.0f));
    direction.z = sin(glm::radians(-90.0f)) * cos(glm::radians(0.0f));
    
    glm::vec3 mCameraFront = glm::normalize(direction);
    
    glm::vec3 mCameraRight = glm::normalize(glm::cross(mCameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 mCameraUp = glm::normalize(glm::cross(mCameraRight, mCameraFront));
    
    glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f), projection = glm::mat4(1.0f);
    
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 3.0f) + mCameraFront, mCameraUp);
    
    projection = glm::perspective(glm::radians(45.0f), (SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 100.0f);
    
    shader.use();
    shader.setUniform("model", model);
    shader.setUniform("view", view);
    shader.setUniform("projection", projection);
    
    // Main game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check for input once per frame (separate from window callback)
        processInput(window);
        
        shader.use();
        
        // Clear the screen with a nice gray color
        glClearColor(0.138f, 0.138f, 0.138f, 1.0f);
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(20.0f * (float) glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
        
        shader.setUniform("model", model);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        // Swap the frame buffers
        glfwSwapBuffers(window);
        // Pump glfw's event queue
        glfwPollEvents();
    }
    
    // Free buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shader.ID);
    
    
    // Shutdown GLFW
    glfwTerminate();
    
    // Return application success
    return EXIT_SUCCESS;
}

// Initialize glfw, glad, and the game window
bool initWindow()
{
    // Initialize GLFW
    glfwInit();
    
    // Sets openGL profile settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    
    // Creates the window
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SnakeGL", nullptr, nullptr);
    // Check if window was created successfully
    if (window == nullptr)
    {
        // Output failure to console
        puts("Failed to create window!\n");
        // Terminate glfw subsystem
        glfwTerminate();
        
        // Return failure
        return false;
    }
    // Set the openGL context to the newly created window
    glfwMakeContextCurrent(window);
    
    // Initialize GLAD and check for failure
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        // Output failure to console
        puts("Failed to initialize GLAD!\n");
        
        // Return failure
        return false;
    }
    
    // Attaches cursor to window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Enable and set window callback functions:
    // Resize window callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Keyboard input callback
    glfwSetKeyCallback(window, key_callback);
    // Cursor movement callback
    glfwSetCursorPosCallback(window, mouse_callback);
    // Scroll movement callback
    glfwSetScrollCallback(window, scroll_callback);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    return true;
}

// Handle window input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
}
