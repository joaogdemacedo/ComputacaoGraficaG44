//
//  Light.hpp
//  CG
//
//  Created by Trident on 09/05/2018.
//

#ifndef Light_hpp
#define Light_hpp
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#endif
using namespace std;
class Light{
public:
    int type;
    int nr;
    float cutoff;
    float dir[3];
    float pos[4];
    float ambi[4];
    float diff[4];
    float spec[4];
    
    Light(int type, int number, float cutoff, float * dir, float * pos, float *ambi , float* diff,float *spec);
    void lightUp();
};
#endif /* Light_hpp */
