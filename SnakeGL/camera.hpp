//
//  camera.hpp
//  SnakeGL
//
//  Created by Keegan Bilodeau on 5/21/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Abstraction of window input
enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Basic camera default constants
const float YAW = -90.0f, PITCH = 0.0f, SPEED = 2.5, SENSITIVITY = 0.1f, ZOOM = 45.0f;

class Camera
{
public:
    // Constructs the camera in a way that is not the prettiest but works great
    Camera(bool fixed = false, glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    
    // Handles keyboard input for camera movement (unless camera's fixed variable is true)
    void processInput(CameraMovement moveDirection, float deltaTime);
    
    // Handles mouse input for direction vectors (unless camera's fixed variable is true)
    void processMouseInput(float xOffset, float yOffset, bool constrainPitch = true);
    
    // Handles scroll input for zoom effect (unless camera's fixed variable is true)
    void processMouseScroll(float yOffset);
    
    // Returns the camera's view matrix so that it can be passed to a shader
    glm::mat4 getViewMatrix();
    
    // Returns the camera's current position
    glm::vec3 &getCameraPos();
    
    // Returns the camera's fov for the projection matrix calculation
    float &getFOV();
    
private:
    // Used by the camera to update its vectors after movement calculations
    void updateCameraVectors();
    
    // If true, all functions regarding camera movement of any kind are disabled
    bool mCameraFixed;
    
    // Various vectors that handle the camera's direction, position, and movement
    glm::vec3 mCameraPos, mCameraFront, mCameraRight, mCameraUp, mWorldUp;
    
    // Pitch and yaw are the camera's yeuler angles
    // FOV, movementSpeed, and mouseSensitivity are for calculating zoom and handling input respectively
    float mPitch, mYaw, mFov, mMovementSpeed, mMouseSensitivity;
};

#endif /* camera_hpp */
