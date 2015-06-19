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
#include "maths_funcs.h"

class Mesh{
    
public:
    
    Mesh();
    void destroy();
    
    
    int mNumVertices;
    int mNumValues;
    int mNumIndices;
    int mNumTexCoords;
    int mNumFaces;
    int mNumRows;
    int mNumCols;
    GLint mDrawMode;
    float * mVertices;
    float * mColors;
    float * mNormals;
    float * mTexCoords; //in pairs
    float * mFaceNormals;
    GLuint * mIndices;
    glm::mat4 mTransform;
    
    vec3 Ks, Kd, Ka;
    
    float specular_exponent;
    
    
    
};

class Primitives {
    
    public:
    
    static void Cube(float * points, float * colours, GLubyte * indices); //this will generate normalised points for a cube
    static void Quad(float * points, float w_scale = 1, float h_scale = 1, glm::vec3 offset = glm::vec3(0,0,0));
    static void Plane(float * points, GLuint * indices, float * texCoords, float width, float height, int numcols = 1, int numrows = 1);
    static void RecalcNormals(Mesh & mesh );
    
    static Mesh CreatePlane(float width, float height,  int numcols = 1, int numrows = 1);
};

#endif /* defined(__gl_tutorial__primitives__) */
