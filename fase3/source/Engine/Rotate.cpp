//
//  Rotate.cpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#include "Rotate.hpp"
Rotate::Rotate(){
    Transformation::x = 0;
    Transformation::y = 0;
    Transformation::z = 0;
    Transformation::angle = 0;
}
Rotate::Rotate(float x_, float y_, float z_, float angle_, int time){
    Transformation::x = x_;
    Transformation::y = y_;
    Transformation::z = z_;
    Transformation::angle = angle_;
    Rotate::time = time;
}
void Rotate::transform(int routes){
    if (Rotate::time) {
        glRotatef(glutGet(GLUT_ELAPSED_TIME)*Rotate::angle, Rotate::x, Rotate::y, Rotate::z);
    }else{
        glRotatef(Rotate::angle, Rotate::x, Rotate::y, Rotate::z);
    }
}
Rotate::~Rotate(){
    Transformation::x = 0;
    Transformation::y = 0;
    Transformation::z = 0;
    Transformation::angle = 0;
}
