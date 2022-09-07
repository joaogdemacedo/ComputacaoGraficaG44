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
    Primitive::normal = -1;
    Primitive::nsize = 0;
    Primitive::texture = -1;
}

Primitive::~Primitive(){
    Primitive::id = 0;
    Primitive::size = 0;
}


Primitive::Primitive(GLuint id_, int size_, GLuint normal_, int nsize_, GLuint textures_, float* ambi_, float* emis_, float* spec_, float * diff_,string text, bool type_){
    id = id_;
    size = size_;
    normal = normal_;
    nsize = nsize_;
    texture = textures_;
    type = type_;
    if(type){
        Primitive::ntexture = text;
    }
    else{
        for (int i = 0;i<4; i++) {
            ambi[i] = ambi_[i];
            diff[i] = diff_[i];
            spec[i] = spec_[i];
            emis[i] = emis_[i];
        }
    }
}

void Primitive::drawModel(GLuint text){
    
    glBindBuffer(GL_ARRAY_BUFFER,Primitive::id);
    glVertexPointer(3,GL_FLOAT,0,0);
    glBindBuffer(GL_ARRAY_BUFFER, Primitive::normal);
    glNormalPointer(GL_FLOAT, 0, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambi);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT, GL_EMISSION, emis);

    
    if (type) {
        glBindBuffer(GL_ARRAY_BUFFER, Primitive::texture);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
        glBindTexture(GL_TEXTURE_2D, text);
    }

    glDrawArrays(GL_TRIANGLES, 0, Primitive::size/3);
    glBindTexture(GL_TEXTURE_2D, 0);
}
