//
//  camera.cpp
//  SnakeGL
//
//  Created by Keegan Bilodeau on 5/21/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#include "camera.hpp"

Camera::Camera(bool fixed, glm::vec3 cameraPos, glm::vec3 worldUp, float yaw, float pitch) : mCameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mFov(ZOOM)
{
    // Sets the camera's various vectors based one the values set in the args
    mCameraPos = cameraPos;
    mCameraFixed = fixed;
    mWorldUp = worldUp;
    mPitch = pitch;
    mYaw = yaw;
    
    // Calculates the values of the vectors not set in the constructor
    updateCameraVectors();
}

void Camera::processInput(CameraMovement moveDirection, float deltaTime)
{
    // Checks to make sure the camera is not fixed
    if (!mCameraFixed)
    {
        // Calculates the speed the camera will move based off the time since the last frame
        float velocity = mMovementSpeed * deltaTime;
        
        // CameraFront is scaled by the speed and then applied to the CameraPos vector to get the new CameraPos
        if (moveDirection == FORWARD)
            mCameraPos += mCameraFront * velocity;
        else if (moveDirection == BACKWARD)
            mCameraPos -= mCameraFront * velocity;
        else if (moveDirection == LEFT)
            mCameraPos -= mCameraRight * velocity;
        else if (moveDirection == RIGHT)
            mCameraPos += mCameraRight * velocity;
    }
}

void Camera::processMouseInput(float xOffset, float yOffset, bool constrainPitch)
{
    // Check to make sure the camera is not fixed
    if (!mCameraFixed)
    {
        // Modifies the offset by the mouseSensitivity to decrease jerkiness
        xOffset *= mMouseSensitivity;
        yOffset *= mMouseSensitivity;
        
        // Increase the yaw and pitch based off the mouse movement
        mYaw += xOffset;
        mPitch += yOffset;
        
        // Keep the camera from doing disorienting backflips
        if (constrainPitch)
        {
            if (mPitch > 89.0f)
                mPitch = 89.0f;
            else if (mPitch < -89.0f)
                mPitch = -89.0f;
        }
        
        // Update the camera's direction vectors to reflect the new pitch and yaw values
        updateCameraVectors();
    }
}

void Camera::processMouseScroll(float yOffset)
{
    // Check to make sure the camera is not fixed
    if (!mCameraFixed)
    {
        // Change the fov based off the yOffset (-= because scrolling up zooms in)
        mFov -= yOffset;
        
        // Keep the zoom from imploding by going negative
        if (mFov < 1.0f)
            mFov = 1.0f;
        // Keep the camera from zooming farther out than the default fov
        else if (mFov > 45.0f)
            mFov = 45.0f;
    }
}

glm::mat4 Camera::getViewMatrix()
{
    // LookAt creates a view matrix based off the cameraPos, its target, and the camera's up vector
    return glm::lookAt(mCameraPos, mCameraPos + mCameraFront, mCameraUp);
}

glm::vec3 &Camera::getCameraPos()
{
    return mCameraPos;
}

float &Camera::getFOV()
{
    return mFov;
}

void Camera::updateCameraVectors()
{
    glm::vec3 direction;
    
    // The cosine of yaw times the cosine of pitch will give the change on the x-axis
    direction.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    // The sine of pitch will give the change on the y-axis
    direction.y = sin(glm::radians(mPitch));
    // The sine of yaw times the cosine of pitch will give the change one the z-axis
    direction.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    
    // Camera front is a unit vector so direction has to be normalized
    mCameraFront = glm::normalize(direction);
    
    // The cross product of the camera's front directional vector
    // and the world up vector will generate the camera's right vector
    mCameraRight = glm::normalize(glm::cross(mCameraFront, mWorldUp));
    // The cross product of the camera's right vector and the camera's front vector
    // will give the camera's up vector
    mCameraUp = glm::normalize(glm::cross(mCameraRight, mCameraFront));
}
