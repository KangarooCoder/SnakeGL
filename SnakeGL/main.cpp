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

// Game window
GLFWwindow* window;

// Screen width and heigh constants
const int SCREEN_WIDTH = 750;
const int SCREEN_HEIGHT = 750;

// Function predefinitions
bool initWindow();
void processInput(GLFWwindow* window);
void close();

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
    
    // Main game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check for input once per frame (separate from window callback)
        processInput(window);
        
        // Clear the screen with a nice gray color
        glClearColor(0.138f, 0.138f, 0.138f, 1.0f);
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        
        // Swap the frame buffers
        glfwSwapBuffers(window);
        // Pump glfw's event queue
        glfwPollEvents();
    }
    
    // Shuts everything down and frees anything neccessary
    close();
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
    
    return true;
}

// Handle window input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
}

// Shutdown library subsystems and free data
void close()
{
    // Terminate GLFW subsystems
    glfwTerminate();
}
