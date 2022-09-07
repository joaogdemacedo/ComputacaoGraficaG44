//
//  Primitive.hpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#ifndef Primitive_hpp
#define Primitive_hpp

#include <stdio.h>
#include <vector>
#include <string>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;


class Primitive{
public:
    GLuint id;
    GLuint normal;
    GLuint texture;
    string ntexture;
    bool type;
    int size;
    int nsize;
    float ambi[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float diff[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float spec[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float emis[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    Primitive();
    Primitive(GLuint, int);
    Primitive(GLuint, int, GLuint, int, GLuint, float*, float*, float*, float*, string, bool);
    ~Primitive();
    void drawModel(GLuint);
    
};

#endif /* Primitive_hpp */
