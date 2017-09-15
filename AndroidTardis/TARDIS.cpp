//
//  TARDIS.cpp
//  Arduino Tardis
//
//  Created by Antonello Galipò on 26/08/17.
//  Copyright © 2017 Antonello Galipò. All rights reserved.
//

#include "TARDIS.hpp"
#include "Mesh.hpp"
#include "utilities.h"
using namespace std;
using namespace glm;


TARDIS::TARDIS(const char *filename) : Mesh(filename){
    modelMatrix = mat4(1.0f);
    quaternionAxis = quat(vec3(0,0,radians(-20.0f)));
    
    modelMatrix = toMat4(quaternionAxis) * modelMatrix;
    spinningAxis = vec3(0,1,0);//rotate(quaternionAxis, vec3(0,1,0));
    
    spinningAngle = 2.0f;
    spinningQuaternion = angleAxis(radians(spinningAngle), spinningAxis);
    spinning = false;
    
}


void TARDIS::setModelMatrix(mat4 newModelMatrix){
    modelMatrix = newModelMatrix;
}
void TARDIS::setViewMatrix(mat4 newViewMatrix){
    viewMatrix = newViewMatrix;
}

void TARDIS::applyRotation(float angle, vec3 axis){
    
}

void TARDIS::applyTranslation(vec3 translationVector){
    vec3 res = vec3(1,1,1);//(glm::greaterThan(glm::abs(translationVector) - vec3(1,1,1),vec3(1,1,1)));
    viewMatrix = translate(viewMatrix,translationVector*res);
    
}
void TARDIS::applyScaling(vec3 scaleVector){
    modelMatrix = scale(modelMatrix,scaleVector);
    
}
void TARDIS::startSpinning(){
    spinning = true;
    
}

mat4& TARDIS::getModelMatrix(){
    if (spinning){
        modelMatrix = toMat4(spinningQuaternion) * modelMatrix;
    }
    return modelMatrix;
}
mat4& TARDIS::getViewMatrix(){
    return viewMatrix;
}
