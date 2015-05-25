//
//  noise.h
//  gl_scratch
//
//  Created by Simon on 25/05/2015.
//  Copyright (c) 2015 __SIMONCORP___. All rights reserved.
//

#ifndef __gl_scratch__noise__
#define __gl_scratch__noise__

#include <stdio.h>

typedef float (* function_ptr)(int , int);

class noise {
    
    public:
    
    static float PerlinNoise_2D(float x, float y);
    
    static int mNumOctaves;
    static float mPersistance;
    
private:
    
    static float GenNoise1(int x);
    static float GenNoise2(int x);
    static float GenNoise3(int x);
    static float GenNoise1(int x, int y);
    static float GenNoise2(int x, int y);
    static float GenNoise3(int x, int y);
    static float LinearInterpolate(float a, float b, float x);
    static float CosineInterpolate(float a, float b, float x);
    static float Cubic_Interpolate(float v0, float v1, float v2, float v3, float x);
    static float SmoothNoise_2D(float x, float y, function_ptr n);
    static float InterpolatedNoise(float x, float y, function_ptr n);
 

    
};

#endif /* defined(__gl_scratch__noise__) */
