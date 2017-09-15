//
//  utilities.h
//  learning OpenGL
//
//  Created by Antonello Galipò on 23/08/17.
//  Copyright © 2017 Antonello Galipò. All rights reserved.
//

#ifndef utilities_h
#include <iostream>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

#define utilities_h


#endif /* utilities_h */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
GLuint loadBMP_custom(const char* imagepath);


