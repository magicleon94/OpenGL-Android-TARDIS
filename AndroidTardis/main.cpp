//
//  main.cpp
//  Arduino Tardis
//
//  Created by Antonello Galipò on 26/08/17.
//  Copyright © 2017 Antonello Galipò. All rights reserved.
//

#include "utilities.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <sstream>
#include "Mesh.hpp"
#include "TARDIS.hpp"
#include "MagicSocket.hpp"
using namespace glm;
using namespace std;

int main(){
    
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window and create its OpenGL context
    GLFWwindow* window;
    
    window = glfwCreateWindow( 1024, 1024, "Time and Relative Dimension In Space", NULL, NULL);
    
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true;
    
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    glClearColor(0.2f, 0.2f, 0.5f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    
    GLuint programID = LoadShaders( "/Users/magicleon/Desktop/AndroidTardis/AndroidTardis/vertex_shader.glsl",
                                    "/Users/magicleon/Desktop/AndroidTardis/AndroidTardis/fragment_shader.glsl");
    TARDIS tardis("/Users/magicleon/Desktop/AndroidTardis/AndroidTardis/tardis.3ds");
    tardis.applyScaling(vec3(0.008,0.008,0.008));
    
    mat4 projectionMatrix = perspective(radians(45.0f), 4.0f/3.0f, 0.1f, 10000.0f);
    mat4 viewMatrix1 = lookAt(vec3(0,0,200),vec3(0,0,0),vec3(0,1,0));
    tardis.setViewMatrix(viewMatrix1);
    tardis.applyTranslation(vec3(0,0,100));
    tardis.startSpinning();
    
    
    
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    mat4 mvp;
    
    float dtx,dty,dtz,drx,dry,drz;
    mutex m;
    bool running = true;
    thread lullo([&dtx,&dty,&dtz,&drx,&dry,&drz,&m,&running](){
        MagicSocket s = new MagicSocket(true);
        s.Listen(1933);
        char buffer[20];
        
        while(running){
            MagicSocket newSocket = s.Accept();
            newSocket.Receive(buffer,20);
            istringstream ss(buffer);
            lock_guard<mutex> lg(m);
            
            for (int i = 0; i<3; i++){
                ss>>dtx;
                ss>>dty;
                ss>>dtz;
                dtx *= -1;
                dtz = 0;
            }
            
            
        
            newSocket.Close();
        }
        
    });
    float MULTIPLIER = 0.7;
    do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(programID);
        mvp = projectionMatrix * tardis.getViewMatrix() * tardis.getModelMatrix();
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &tardis.getModelMatrix()[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &tardis.getViewMatrix()[0][0]);
        
        glm::vec3 lightPos = glm::vec3(4,0,44);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);;
        {
            lock_guard<mutex> lg(m);
            tardis.applyTranslation(vec3(MULTIPLIER * dtx,MULTIPLIER * dty,MULTIPLIER * dtz));
        }
        tardis.render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
    
    running = false;
    lullo.join();
    glDeleteProgram(programID);
    glfwTerminate();
    return 0;

   
}

