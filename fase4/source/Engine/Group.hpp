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
#include "Light.hpp"
using namespace std;
class Group{
public:
    vector<string> tft_names;
    vector<Transformation*> tft;
    vector<Group*> childGroups;
    vector<Primitive*> models;
    static map<string, GLuint> load_models;
    static map<string, GLuint> load_textures;
    static map<string, GLuint> load_normals;
    static map<string, GLuint> load_file_textures;
    static vector<Light> lights;

    
    Group();
    void drawGroup(int );
    int addTransformation(string name, Transformation* t);
    int addPrimitive(Primitive* p);
    int addTexture(GLuint texture);
    int addGroup(Group* g);
};
#endif /* Group_hpp */
