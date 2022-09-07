//
//  Group.hpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#ifndef Group_hpp
#define Group_hpp
#include <map>
#include <string>
#include <algorithm>
#include "Scale.hpp"
#include "Translate.hpp"
#include "Rotate.hpp"
#include "Primitive.hpp"
using namespace std;
class Group{
public:
    vector<string> tft_names;
    vector<Transformation*> tft;
    vector<Group*> childGroups;
    vector<Primitive*> models;
    map<string, Primitive*>* load_models;
    
    Group();
    Group(map<string, Primitive*>*);
    Group(vector<string>,vector<Transformation*>, vector<Group*>, vector<Primitive*>, map<string, Primitive*>*);
    void drawGroup();
    int addTransformation(string name, Transformation* t);
    int addPrimitive(Primitive* p);
    int addGroup(Group* g);
};
#endif /* Group_hpp */
