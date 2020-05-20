//
//  callback.hpp
//  SnakeGL
//
//  Created by Keegan Bilodeau on 5/20/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#ifndef callback_hpp
#define callback_hpp

#include <stdio.h>

// Ignores library documentation issues
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <GLFW/glfw3.h>

// Restores clang to its normal state
#pragma clang diagnostic pop

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int modes);

void mouse_callback(GLFWwindow* window, double xPos, double yPos);

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

#endif /* callback_hpp */
