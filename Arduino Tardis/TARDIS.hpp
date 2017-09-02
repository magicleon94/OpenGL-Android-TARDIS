//
//  TARDIS.hpp
//  Arduino Tardis
//
//  Created by Antonello Galipò on 26/08/17.
//  Copyright © 2017 Antonello Galipò. All rights reserved.
//

#ifndef TARDIS_hpp
#define TARDIS_hpp

#include <iostream>

//#ifndef GLEW_STATIC
//#define GLEW_STATIC
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "utilities.h"
//#endif
#include "mesh.hpp"

#endif /* TARDIS_hpp */
using namespace std;
using namespace glm;

class TARDIS : public Mesh {
private:
    mat4 modelMatrix;
    mat4 viewMatrix;
    vec3 spinningAxis;
    quat quaternionAxis;
    float spinningAngle;
    quat spinningQuaternion;
    bool spinning;


public:
    TARDIS(const char* filename);
    void setModelMatrix(mat4 newModelViewMatrix);
    void setViewMatrix(mat4 newViewMatrix);
    void setSpinningQuaternion(quat newQuaternion);
    void applyRotation(float angle, vec3 axis);
    void applyTranslation(vec3 translationVector);
    void applyScaling(vec3 scaleVector);
    void startSpinning();
    mat4& getModelMatrix();
    mat4& getViewMatrix();
    
    
};
