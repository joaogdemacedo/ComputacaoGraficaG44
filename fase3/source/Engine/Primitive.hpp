//
//  Primitive.hpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#ifndef Primitive_hpp
#define Primitive_hpp

#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

class Primitive{
public:
    GLuint id;
    GLuint idi;
    int size;
    
    Primitive();
    Primitive(GLuint, int);
    Primitive(GLuint, GLuint, int);
    ~Primitive();
    GLuint getId();
    int getSize();
    void drawModel();
    
};

#endif /* Primitive_hpp */
