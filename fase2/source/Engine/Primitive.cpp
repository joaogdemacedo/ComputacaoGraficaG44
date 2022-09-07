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
    Primitive::idi = 0;
}

Primitive::Primitive(GLuint id_, GLuint idi_, int size_){
    Primitive::id = id_;
    Primitive::idi = idi_;
    Primitive::size = size_;
}
Primitive::~Primitive(){
    Primitive::id = 0;
    Primitive::idi = 0;
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
    if (idi != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idi);
        glDrawElements(GL_TRIANGLES, Primitive::size ,GL_UNSIGNED_INT, NULL);
    }
    else{
        glDrawArrays(GL_TRIANGLES, 0, Primitive::size);
    }
}
