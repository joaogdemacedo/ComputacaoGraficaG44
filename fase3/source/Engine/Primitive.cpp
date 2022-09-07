//
//  Primitive.cpp
//  Project
//
//  Created by Trident on 14/03/2018.
//
#include "Primitive.hpp"

Primitive::Primitive(){
    Primitive::id = 0;
    Primitive::size = 0;
}

Primitive::Primitive(GLuint id_, int size_){
    Primitive::id = id_;
    Primitive::size = size_;
}

Primitive::~Primitive(){
    Primitive::id = 0;
    Primitive::size = 0;
}

GLuint Primitive:: getId(){
    return id;
}

int Primitive:: getSize(){
    return size;
}

void Primitive::drawModel(){
    glBindBuffer(GL_ARRAY_BUFFER,Primitive::id);
    glVertexPointer(3,GL_FLOAT,0,0);
    glDrawArrays(GL_TRIANGLES, 0, Primitive::size);
}
