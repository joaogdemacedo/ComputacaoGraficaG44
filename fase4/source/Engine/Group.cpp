//
//  Group.cpp
//  Project
//
//  Created by Trident on 14/03/2018.
//


#include "Group.hpp"
map<string, GLuint> Group:: load_models;
map<string, GLuint> Group:: load_textures;
map<string, GLuint> Group:: load_file_textures;
map<string, GLuint> Group:: load_normals;
vector<Light> Group:: lights;
Group::Group(){};

void Group :: drawGroup(int routes){
    glPushMatrix();
    for (auto x : lights) x.lightUp();
    for (auto t : tft) t->transform(routes);
    for (auto model : models){
        GLuint text = load_file_textures[model->ntexture];
        model->drawModel(text);
    }
    for (auto child : childGroups) child->drawGroup(routes);
    glPopMatrix();
};
int Group :: addTransformation(string name, Transformation* t){
    
    if (find(this->tft_names.begin(),this->tft_names.end(), name) == this->tft_names.end()) {
        this->tft.push_back(t);
        this->tft_names.push_back(name);
        return 1;
    }
    return 0;
}
int Group :: addPrimitive(Primitive* p){
    this->models.push_back(p);
    return 1;
}
int Group :: addGroup(Group* g){
    this->childGroups.push_back(g);
    return 1;
}
