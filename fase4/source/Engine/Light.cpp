//
//  Light.cpp
//  CG
//
//  Created by Trident on 09/05/2018.
//

#include "Light.hpp"


Light::Light(int type, int number, float cutoff, float * dir, float * pos, float *ambi , float* diff, float *spec){
    Light::type = type;
    Light::nr = number;
    Light::cutoff = cutoff;
    for (int i = 0;i<4; i++) {
        Light::pos[i] = pos[i];
        Light::ambi[i] = ambi[i];
        Light::diff[i] = diff[i];
        Light::spec[i] = spec[i];
        if (i<3) {
            Light::dir[i] =dir[i];
        }
    }
}

void Light::lightUp(){
    
    if (type==2) {
        glLightfv(nr+GL_LIGHT0, GL_SPOT_DIRECTION, dir);
        glLightf(nr+GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
    }
    glLightfv(nr+GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(nr+GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(nr+GL_LIGHT0, GL_POSITION, pos);
    glLightfv(nr+GL_LIGHT0, GL_SPECULAR, spec);
}
