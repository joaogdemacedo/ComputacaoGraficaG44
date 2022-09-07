//
//  Scale.cpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#include "Scale.hpp"
Scale::Scale(){
    Transformation::x = 0;
    Transformation::y = 0;
    Transformation::z = 0;
    Transformation::angle = 0;
}
Scale::Scale(float x_, float y_, float z_, float angle_){
    Transformation::x = x_;
    Transformation::y = y_;
    Transformation::z = z_;
    Transformation::angle = angle_;
}
void Scale::transform(int routes){
    glScalef(Scale::x, Scale::y, Scale::z);
}
Scale::~Scale(){
    Transformation::x = 0;
    Transformation::y = 0;
    Transformation::z = 0;
    Transformation::angle = 0;
}
