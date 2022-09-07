//
//  Group.cpp
//  Project
//
//  Created by Trident on 14/03/2018.
//


#include "Group.hpp"
Group::Group(){
    map<string, Primitive*> * map = new ::map<string, Primitive*>();
    Group::load_models = map ;
    
};
Group::Group(map<string, Primitive*>* map_models){
    Group::load_models = map_models;
};
Group::Group(vector<string> tft_names, vector<Transformation*>tft , vector<Group*> cg, vector<Primitive*> p, map<string, Primitive*>* map_models){
    Group::tft = tft;
    Group::childGroups = cg;
    Group::models = p;
    Group::load_models = map_models;
};
void Group :: drawGroup(){
    glPushMatrix();
    for (auto t : tft) t->transform();
    for (auto model : models) model->drawModel();
    for (auto child : childGroups) child->drawGroup();
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
