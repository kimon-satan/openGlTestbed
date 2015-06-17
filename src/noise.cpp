//
//  noise.cpp
//  gl_scratch
//
//  Created by Simon on 25/05/2015.
//  Copyright (c) 2015 __SIMONCORP___. All rights reserved.
//

#include "noise.h"
#include <math.h>

int noise::mNumOctaves = 8;
float noise::mPersistance = 0.5;

float noise::GenNoise1(int x){

    x = (x<<13) ^ x;
    return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741827.0);

}

float noise::GenNoise2(int x){
    
    x = (x<<13) ^ x;
    return ( 1.0 - ( (x * (x * x * 15749 + 789227) + 1376312627) & 0x7fffffff) / 1073741831.0);
    
}

float noise::GenNoise3(int x){
    
    x = (x<<13) ^ x;
    return ( 1.0 - ( (x * (x * x * 15761 + 789251) + 1376312629) & 0x7fffffff) / 1073741833.0);
    
}

float noise::GenNoise1(int x, int y)
{
    int n = x + y * 57;
    n = (n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741827.0);
    
}

float noise::GenNoise2(int x, int y)
{
    int n = x + y * 59;
    n = (n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15749 + 789227) + 1376312627) & 0x7fffffff) / 1073741831.0);
}

float noise::GenNoise3(int x, int y)
{
    int n = x + y * 61;
    n = (n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15761 + 789251) + 1376312629) & 0x7fffffff) / 1073741833.0);
}

float noise::LinearInterpolate(float a, float b, float x){
    return  a*(1-x) + b*x;
}

float noise::CosineInterpolate(float a, float b, float x){
    
    float ft = x * 3.1415927;
    float f = (1 - cos(ft)) * .5;

    return  a*(1-f) + b*f;

}

float noise::Cubic_Interpolate(float v0, float v1, float v2, float v3, float x){

    float P = (v3 - v2) - (v0 - v1);
    float Q = (v0 - v1) - P;
    float R = v2 - v0;
    float S = v1;

    return P * pow(x,3) + Q * x * x + R * x + S;
}

float noise::SmoothNoise_1D(float x, function_ptr_1d p_noise ){
    
    return p_noise(x)/2  +  p_noise(x-1)/4  +  p_noise(x+1)/4;
    
    
}

float noise::SmoothNoise_2D(float x, float y, function_ptr p_noise ){

    float corners = ( p_noise(x-1, y-1)+ p_noise(x+1, y-1)+ p_noise(x-1, y+1)+ p_noise(x+1, y+1) ) / 16;
    float sides   = ( p_noise(x-1, y)  + p_noise(x+1, y)  + p_noise(x, y-1)  + p_noise(x, y+1) ) /  8;
    float center  =  p_noise(x, y) / 4;

    return corners + sides + center;

}


float noise::InterpolatedNoise(float x, float y, function_ptr n)
{

    int integer_X = floor(x);
    float fractional_X = x - integer_X;

    int integer_Y    = floor(y);
    float fractional_Y = y - integer_Y;

    float v1 = SmoothNoise_2D(integer_X,     integer_Y, n);
    float v2 = SmoothNoise_2D(integer_X + 1, integer_Y, n);
    float v3 = SmoothNoise_2D(integer_X,     integer_Y + 1, n);
    float v4 = SmoothNoise_2D(integer_X + 1, integer_Y + 1, n);

    float i1 = CosineInterpolate(v1 , v2 , fractional_X);
    float i2 = CosineInterpolate(v3 , v4 , fractional_X);

    return CosineInterpolate(i1 , i2 , fractional_Y);

}

float noise::InterpolatedNoise(float x, function_ptr_1d p){
    
    int integer_X    = floor(x);
    float fractional_X = x - integer_X;
    
    float v1 = SmoothNoise_1D(integer_X, p);
    float v2 = SmoothNoise_1D(integer_X + 1, p);
    
    return CosineInterpolate(v1 , v2 , fractional_X);
    
}


float noise::PerlinNoise_1D(float x){
    
    float total = 0;

    
    for(int i = 0; i < mNumOctaves - 1; i++){
    
        float frequency = pow(2,i);
        float amplitude = pow(mPersistance,i);
        
        function_ptr_1d p;
        
        switch (i%3) {
            case 0: p = &noise::GenNoise1; break;
            case 1: p = &noise::GenNoise2; break;
            case 2: p = &noise::GenNoise3; break;
        }
    
        total = total + InterpolatedNoise(x * frequency, p) * amplitude;
    
    }
    
    return total;
}

float noise::PerlinNoise_2D(float x, float y){

    float total = 0;

    for(int i = 0; i < mNumOctaves - 1; i++){

        float frequency = pow(2 ,i);
        float amplitude = pow(mPersistance, i);
        
        function_ptr p;
        
        switch (i%3) {
            case 0: p = &noise::GenNoise1; break;
            case 1: p = &noise::GenNoise2; break;
            case 2: p = &noise::GenNoise3; break;
        }

        total += InterpolatedNoise(x * frequency, y * frequency, &noise::GenNoise1) * amplitude;

    }

    return total;

}