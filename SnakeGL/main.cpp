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
#include <filesystem>

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

// Wrapper for openGL shaders
#include "shader.hpp"

// VAO generators for various shapes
#include "renderable.hpp"

// Contains the game's camera class
#include "camera.hpp"

// Game window
GLFWwindow* window;

// Game camera
Camera camera(true);

// Screen width and heigh constants
const float SCREEN_WIDTH = 750.0f;
const float SCREEN_HEIGHT = 750.0f;

// Time handling variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;
// Last x position of the cursor
float lastX = 325, lastY = 325;
// Keeps the screen from jerking on the first mouse input
bool firstMouseInput = true;

// Global light position
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Function predefinitions
bool initWindow();
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int modes);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

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
    
    // Creates a shader and sets it as the active shader
    Shader shader("resources/vShader.vert", "resources/fShader.frag");
    shader.use();
    
    // Sets uniforms neccessary for light calculations
    shader.setUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform("lightPos", camera.getCameraPos());
    
    // Created the matrixes for use in the main game loop
    glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f), projection = glm::mat4(1.0f);
    
    // Main game loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check for input once per frame (separate from window callback)
        processInput(window);
        
        // Sets this shaders as the active shader
        shader.use();
        
        // Sets the model matrix uniform for the conversion from NDC to model coords
        model = glm::mat4(1.0f);
        shader.setUniform("model", model);
        
        // Sets the view matrix uniform for the conversion from model coords to view coords
        view = glm::mat4(1.0f);
        view = camera.getViewMatrix();
        shader.setUniform("view", view);
        
        // Sets the projection matrix uniform for the conversion from view coords to projection coords
        projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.getFOV()), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);
        shader.setUniform("projection", projection);
        
        // Clear the screen with a nice gray color
        glClearColor(0.138f, 0.138f, 0.138f, 1.0f);
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Swap the frame buffers
        glfwSwapBuffers(window);
        // Pump glfw's event queue
        glfwPollEvents();
    }
    
    // Free buffers
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
    
    // Moves cursor to the center of the window
    glfwSetCursorPos(window, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    return true;
}

// Handle window input
void processInput(GLFWwindow *window)
{
    // Allows the user to exit when cursor is captured
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
    
    // Processes keyboard input into directions used by the camera for movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processInput(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processInput(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processInput(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processInput(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Sets the GL viewport to the proper size upon window resize
    glViewport(0, 0, width, height);
    
    // Sets the cursor to the correct position
    glfwSetCursorPos(window, (float)(width) / 2.0f, (float)(height) / 2.0f);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int modes)
{
    
}

void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
    // Check if the mouse is moving for the first time
    if (firstMouseInput)
    {
        // Set the lastX and lastY to the current position to avoid the camera jerking
        lastX = xPos;
        lastY = yPos;
        
        // Set this to false to allow for normal input calculation from now on
        firstMouseInput = false;
    }
    
    // Calculate the offset from the last mouse position to the current mouse position
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    // Set the lastX and lastY variables for next input calculation
    lastX = xPos;
    lastY = yPos;
    
    // Pass the offsets to the camera to calculate direction vectors
    camera.processMouseInput(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
{
    // Pass the yOffset to the camera to allow for a zoom effect
    camera.processMouseScroll(yOffset);
}
