//
//  primitives.h
//  gl_tutorial
//
//  Created by Simon on 05/04/2015.
//  Copyright (c) 2015 __SIMONCORP___. All rights reserved.
//

#ifndef __gl_tutorial__primitives__
#define __gl_tutorial__primitives__

#include <iostream>


#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class Mesh{
    
public:
    
    Mesh();
    void destroy();
    
    
    int mNumVertices;
    int mNumValues;
    int mNumIndices;
    int mNumTexCoords;
    GLint mDrawMode;
    float * mVertices;
    float * mColors;
    float * mNormals;
    float * mTexCoords; //in pairs
    GLuint * mIndices;
    glm::mat4 mTransform;
    
    
    
};

class Primitives {
    
    public:
    
    static void Cube(float * points, float * colours, GLubyte * indices); //this will generate normalised points for a cube
    static void Quad(float * points, float w_scale = 1, float h_scale = 1, glm::vec3 offset = glm::vec3(0,0,0));
    static void Plane(float * points, GLuint * indices, float * texCoords, float width, float height, int numcols = 1, int numrows = 1);
    
    static Mesh CreatePlane(float width, float height,  int numcols = 1, int numrows = 1);
};

#endif /* defined(__gl_tutorial__primitives__) */
