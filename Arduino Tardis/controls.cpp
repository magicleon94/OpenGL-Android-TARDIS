//
//  controls.cpp
//  learning OpenGL
//
//  Created by Antonello Galipò on 24/08/17.
//  Copyright © 2017 Antonello Galipò. All rights reserved.
//
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "controls.hpp"
#include <unistd.h>

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 ModelMatrix(1.0f);

glm::mat4 getModelMatrix(){
    return ModelMatrix;
}

glm::mat4 getViewMatrix(){
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
    return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 4, 3, 3 );
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.000005f;

void computeMatricesFromInputs(GLFWwindow* window){
    static double lastTime = glfwGetTime();
    
    double currentTime = glfwGetTime();
    float deltaTime = (float) currentTime - lastTime;
    
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    
    horizontalAngle += mouseSpeed * float(1024/2 - xpos);
    verticalAngle += mouseSpeed * float(768/2 - ypos);
    std::cout<<float(1024/2 - xpos)<<"\t"<<float(768/2 - ypos) - ypos<<std::endl;
    vec3 direction( cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle)*cos(horizontalAngle));
    
    vec3 right(sin(horizontalAngle - 3.14f/2.0f),0,cos(horizontalAngle-3.14f/2.0f));
    
    vec3 up = cross(right,direction);
    
    if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS){
        position += direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        position -= direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        position += right * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        position -= right * deltaTime * speed;
    }
    
    ProjectionMatrix = perspective(initialFoV, 4.0f/3.0f, 0.1f, 100.0f);
    ViewMatrix = lookAt(position,position + direction,up);
    
    glm::rotate(ModelMatrix,10.0f,vec3(0,1,0));
    
    lastTime = currentTime;
//    glfwSetCursorPos(window, 1024/2, 768/2);
}
