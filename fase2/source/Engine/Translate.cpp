//
//  Translate.cpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#include "Translate.hpp"
Translate::Translate(){
    Transformation::x = 0;
    Transformation::y = 0;
    Transformation::z = 0;
    Transformation::angle = 0;
}
Translate::Translate(float x_, float y_, float z_, float angle_){
    Transformation::x = x_;
    Transformation::y = y_;
    Transformation::z = z_;
    Transformation::angle = angle_;
}
void Translate::transform(){
    glTranslatef(x, y, z);
}
Translate::~Translate(){
    Transformation::x = 0;
    Transformation::y = 0;
    Transformation::z = 0;
    Transformation::angle = 0;
}
