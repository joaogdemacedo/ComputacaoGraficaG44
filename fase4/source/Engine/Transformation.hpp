//
//  Transformation.hpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#ifndef Transformation_hpp
#define Transformation_hpp
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <stdio.h>
class Transformation{
    
public:
    float x;
    float y;
    float z;
    float angle;
    
    
    Transformation();
    Transformation(float,float,float,float);
    virtual void transform(int)=0;
    ~Transformation();
};

#endif /* Transformation_hpp */

