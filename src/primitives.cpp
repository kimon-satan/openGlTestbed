//
//  primitives.cpp
//  gl_tutorial
//
//  Created by Simon on 05/04/2015.
//  Copyright (c) 2015 __SIMONCORP___. All rights reserved.
//

#include "primitives.h"


/*---------------------------------MESH-------------------------*/

Mesh::Mesh(){
    
    mNumVertices = 0;
    mNumFaces = 0;
    mNumValues = 0;
    mNumIndices = 0;
    mNumTexCoords = 0;
    mNumRows = 0;
    mNumCols = 0;
    mDrawMode = GL_TRIANGLE_STRIP;
    mVertices = NULL;
    mColors = NULL;
    mNormals = NULL;
    mFaceNormals = NULL;
    mTexCoords = NULL;
    mIndices = NULL;
    
    
}

void Mesh::destroy(){
    
    
    if(mVertices != NULL){
        delete [] mVertices;
    }
    
    if(mIndices != NULL){
        delete [] mIndices;
    }
    
    if(mColors != NULL){
        delete [] mColors;
    }
    
    if(mTexCoords != NULL){
        delete [] mTexCoords;
    }
    
    if(mNormals != NULL){
        delete [] mNormals;
    }
    
}


/*------------------------------------PRIMITIVES ------------------------------------*/



void Primitives::Quad(float * points, float w_scale, float h_scale, glm::vec3 offset){
    
    
    float x = offset.x;
    float y = offset.y;
    float z = offset.z;
    
    float w = w_scale/2;
    float h = h_scale/2;
    
    float t[] = {
        -w + x,   -h + y,   z,
        w + x,    -h + y,   z,
        -w + x,   h + y,    z,
        w + x,    h + y,    z
    };
    
    
    
    for(int i = 0; i < 12; i++){
        points[i] = t[i];
    }
    
    
}

void Primitives::Cube(float * points, float * colours, GLubyte * indices){
    
    //TODO move to a mesh method
    
    Primitives::Quad(&points[0], 1.0, 1.0, glm::vec3(0,0,-0.5));
    Primitives::Quad(&points[12], 1.0, 1.0, glm::vec3(0,0,0.5));
    
    GLubyte t_idxs[] = {
        0,1,2, 1,3,2,   // 36 of indices
        5,7,1, 1,7,3,
        0,4,1, 4,5,1,
        2,3,6, 6,3,7,
        0,6,4, 0,2,6,
        4,6,5, 6,7,5};
    
    for(int i= 0; i < 36; i ++){
        indices[i] = t_idxs[i];
    }
    
    for(int i = 0; i < 8; i++){
        colours[i * 3] = i * 1.0/8;
        colours[i * 3 + 1] = i * 1.0/8;
        colours[i * 3 + 2] = i * 1.0/8;
    }
    
    
}

void Primitives::Plane(float * points, GLuint * indices,  float * texCoords, float width, float height, int numcols, int numrows){
    
    
    float c_incr = (float)width/(numcols-1);
    float r_incr = (float)height/(numrows-1);
    
    int ic = 0;
    int tc = 0;
    
    for(int r = 0; r < numrows; r++){
        int r_idx = r * numcols * 3;
        
        
        for(int c = 0; c < numcols; c++){
            
            int idx = r_idx + c * 3;
            
            
            
            points[idx] = -width/2 + c * c_incr;
            points[idx + 1] = -height/2 + r * r_incr;
            points[idx + 2] = 0;
            
            texCoords[tc] = c * c_incr; //we can ignore indices for texcoords
            tc++;
            texCoords[tc] = r * r_incr;
            tc++;
            
            
        }
        
        
        
    }
    
    for(int r = 0; r < numrows - 1; r++){
        
        for(int c = 0; c < numcols; c++){
            
            indices[ic] = r * numcols + c;
            ic++;

            indices[ic] = indices[ic-1] + numcols;
            ic++;

      
            
        }
        
        if(r < numrows -1){
            indices[ic] = indices[ic-1];
            ic++;
            indices[ic] = (r + 1) * numcols;
            ic++;
        }
        
    }
    

    
    
}

void Primitives::RecalcNormals(Mesh & mesh ){
    int faceCount = 0;
    
    //NB this really does only half the faces but it makes a normal for every vertice anyway
    
    for(int r = 0; r < mesh.mNumRows - 1; r++){
        
        for(int c = 0; c < mesh.mNumCols -1; c++){
            
            int idx1 = r * mesh.mNumCols + c;
            int idx2 = idx1 + 1;
            int idx3 = idx1 + mesh.mNumCols;
            int idx4 = idx3 + 1;
            
            idx1 *= 3;
            idx2 *= 3;
            idx3 *= 3;
            idx4 *= 3;
            
            vec3 v1 = vec3(mesh.mVertices[idx1], mesh.mVertices[idx1 + 1], mesh.mVertices[idx1 +2]);
            vec3 v2 = vec3(mesh.mVertices[idx2], mesh.mVertices[idx2 + 1], mesh.mVertices[idx2 +2]);
            vec3 v3 = vec3(mesh.mVertices[idx3], mesh.mVertices[idx3 + 1], mesh.mVertices[idx3 +2]);
            vec3 v4 = vec3(mesh.mVertices[idx4], mesh.mVertices[idx4 + 1], mesh.mVertices[idx4 +2]);
            
            vec3 e1 = vec3(v2 - v1);
            e1 = normalise(e1);
            vec3 e2 = vec3(v1 - v3);
            e2 = normalise(e2);
            vec3 e3 = vec3(v2 - v4);
            e3 = normalise(e3);
            vec3 e4 = vec3(v4 - v3);
            e4 = normalise(e4);
            
            vec3 nf1 = cross( e2, e1);
            vec3 nf2 = cross( e3, e4);
            
            //vec3 nf = vec3(0,0,1);
            
            for(int i = 0; i < 3; i++){
                mesh.mFaceNormals[faceCount * 3 + i]= nf1.v[i];
                mesh.mFaceNormals[faceCount * 3 + i + 3]= nf2.v[i];
                mesh.mNormals[idx1 + i] = nf1.v[i];
                mesh.mNormals[idx2 + i] = nf1.v[i];
                mesh.mNormals[idx3 + i] = nf2.v[i];
                mesh.mNormals[idx4 + i] = nf2.v[i];
                
            }
            
            
            faceCount += 2;
            
            
        }
    }
    
    
   /* for(int i = 0; i < mesh.mNumFaces; i+=3){
        std::cout << mesh.mFaceNormals[i] << "," << mesh.mFaceNormals[i+1] << ","<< mesh.mFaceNormals[i+2]  << std::endl;
    }*/
    
    /*
     
        /d|c/b|
      |e/f|a
     
     */
    
    
    for(int i = 0; i < mesh.mNumVertices; i++){
        
        int r = floor(i/mesh.mNumCols);
        int a = 2 * (i%mesh.mNumCols) + 2 * r * (mesh.mNumCols - 1);
        int c = 2 * (i%mesh.mNumCols) + 2 * (r-1) * (mesh.mNumCols - 1);
        int b = c + 1;
        int d = c - 1;
        int f = a - 1;
        int e = a - 2;
        
        float norms[3] = {0,0,0};
        
        int count = 0;
        
        if(i%mesh.mNumCols < mesh.mNumCols - 2)
        {
            
            if(r > 0){
                //c b
                count += 2;
                for(int j = 0; j < 3; j++)norms[j] += mesh.mFaceNormals[b * 3 + j];
                for(int j = 0; j < 3; j++)norms[j] += mesh.mFaceNormals[c * 3 + j];
            }
            
            if(r < mesh.mNumRows -1){
                //add a
                count += 1;
                for(int j = 0; j < 3; j++)norms[j] += mesh.mFaceNormals[a * 3 + j];
            }
        }
        
        if(i%mesh.mNumCols > 1)
        {
            
            if(r > 0){
                //d
                count += 1;
                for(int j = 0; j < 3; j++)norms[j] += mesh.mFaceNormals[d * 3 + j];
            }
            
            if(r < mesh.mNumRows -1){
                //add e f
                
                count += 2;
                for(int j = 0; j < 3; j++)norms[j] += mesh.mFaceNormals[e * 3 + j];
                for(int j = 0; j < 3; j++)norms[j] += mesh.mFaceNormals[f * 3 + j];
            }
            
        }
        
        if(count > 0){
            for(int j = 0; j < 3; j++)norms[j] /= count;
        }
        
        for(int j = 0; j < 3; j++)mesh.mNormals[i * 3 + j]=norms[j];
        
        
        
    }

}

Mesh Primitives::CreatePlane(float width, float height, int numcols, int numrows){
    
    Mesh mesh;
    
    mesh.mNumRows = numrows;
    mesh.mNumCols = numcols;
    mesh.mNumFaces = (numcols - 1) * (numrows -1) * 2;
    mesh.mNumValues = numcols * numrows * 3;
    mesh.mNumVertices = numcols * numrows;
    mesh.mVertices = new float[mesh.mNumValues];
    mesh.mNumIndices = (numrows-1) * (numcols * 2 + 2) - 2;
    mesh.mIndices = new GLuint[mesh.mNumIndices];
    mesh.mNumTexCoords = numcols * numrows;
    mesh.mTexCoords = new float[mesh.mNumTexCoords * 2];
    mesh.mFaceNormals = new float[mesh.mNumFaces * 3];
    mesh.mNormals = new float[mesh.mNumValues];
    
    Plane(mesh.mVertices, mesh.mIndices, mesh.mTexCoords, width, height, numcols, numrows);
    mesh.mDrawMode = GL_TRIANGLE_STRIP;
    mesh.mTransform = glm::mat4(1.0);
    
    RecalcNormals(mesh);
    
    return mesh;
    
}
