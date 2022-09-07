//
//  XMLParser.cpp
//  Project
//
//  Created by Trident on 16/03/2018.
//

#include "XMLParser.hpp"

void genModel(GLuint * idv, vector<float> points){
    glGenBuffers(1,idv);
    glBindBuffer(GL_ARRAY_BUFFER,*idv);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size() - 1 , &(points[0]), GL_STATIC_DRAW);
}


void genModelNormals(GLuint * idv, vector<float> normals){
    glGenBuffers(1,idv);
    glBindBuffer(GL_ARRAY_BUFFER,*idv);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), &(normals[0]), GL_STATIC_DRAW);
}

void genModelTexture(GLuint * idv, vector<float> texture){
    glGenBuffers(1,idv);
    glBindBuffer(GL_ARRAY_BUFFER,*idv);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texture.size(), &(texture[0]), GL_STATIC_DRAW);
}

int readBinaryModel(const char * fileName, vector<float>* points,vector<float>* normals,vector<float>* textures){
    int size = 0;
    ifstream fin(fileName, std::ios::binary);
    if(!fin) {cout<<"Error in read"<<endl; return 0;}
    while(!fin.eof()){
        float component;
        fin.read(reinterpret_cast<char*>(&component), sizeof(float));
        points->push_back(component);
        fin.read(reinterpret_cast<char*>(&component), sizeof(float));
        points->push_back(component);
        fin.read(reinterpret_cast<char*>(&component), sizeof(float));
        points->push_back(component);
        fin.read(reinterpret_cast<char*>(&component), sizeof(float));
        normals->push_back(component);
        fin.read(reinterpret_cast<char*>(&component), sizeof(float));
        normals->push_back(component);
        fin.read(reinterpret_cast<char*>(&component), sizeof(float));
        normals->push_back(component);
        fin.read(reinterpret_cast<char*>(&component), sizeof(float));
        textures->push_back(component);
        fin.read(reinterpret_cast<char*>(&component), sizeof(float));
        textures->push_back(component);
        size+=3;
    }
    return size;
}


/*int readBinaryModel(const char * fileName, vector<float>* points){
    int size = 0;
    ifstream fin(fileName, std::ios::binary);
    if(!fin) {cout<<"Error in read"<<endl; return 0;}
    while(!fin.eof()){
        float component;
        fin.read(reinterpret_cast<char*>(&component), sizeof(float));
        points->push_back(component);
        size++;
    }
    return size;
}
*/





void genTexture(Group *group, const char *textName){
    auto iter = group->load_file_textures.find(textName);
    if (iter != group->load_file_textures.end()) {
        return;
    }
    GLuint texID;
    unsigned int t, tw, th;
    unsigned char *texData;
    ilGenImages(1,&t);
    ilBindImage(t);
    ilLoadImage((ILstring)textName);
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();
    glGenTextures(1,&texID); // unsigned int texID - variavel global;
    glBindTexture(GL_TEXTURE_2D,texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    Group::load_file_textures[textName] = texID;
}




int loadModel(XMLElement* node_element, Group * group){
    GLuint idv, norm, text;
    bool txt = false;
    vector<float> points, normals, textures;
    float ambi[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float diff[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float emis[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float spec[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    const char * fileName; 
    const char * textName = nullptr;
    Primitive * p;
    const XMLAttribute* att = node_element->FirstAttribute();
    while(att){
        if(!strcmp(att->Name(),"file")){ 
            fileName = node_element->FirstAttribute()->Value();
            auto iter = group->load_models.find(fileName);
            if (iter == group->load_models.end() ) {
                readBinaryModel(fileName, &points,&normals,&textures);
                genModel(&idv,points);
                genModelNormals(&norm, normals);
                genModelTexture(&text, textures);
                group->load_models[fileName]= idv;
                group->load_normals[fileName]= norm;
                group->load_textures[fileName]= text;
            }else{
                idv = group->load_models[fileName];
                norm = group->load_normals[fileName];
                text = group->load_textures[fileName];
            }
        }
        else if(!strcmp(att->Name(),"diffR") ){

            diff[0] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"diffG")){

            diff[1] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"diffB")){

            diff[2] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"specR")){

            spec[0] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"specG")){
            spec[1] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"specB")){
            spec[2] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"ambiR")){
            ambi[0] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"ambiG")){
            ambi[1] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"ambiB")){
            ambi[2] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"emisR")){
            emis[0] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"emisG")){
            emis[1] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"emisB")){
            emis[2] = att->FloatValue();
        }
        else if(!strcmp(att->Name(),"texture")){
            textName = att->Value();
            txt = true;
            genTexture(group,textName);
        }else{
            return 0;
        }
        att = att->Next();
    }
    
    //genModelNormals(&norm, normals);
    if (!textName) {
        string dummy;
        p = new Primitive(idv, points.size(), norm, normals.size(), text, ambi, emis, spec, diff,dummy,txt);
    }else{
        p = new Primitive(idv, points.size(), norm, normals.size(), text, ambi, emis, spec, diff,textName,txt);
    }
    group->addPrimitive(p);
    return 1;
}

int loadTranslatePoints(XMLElement* node, vector<vector<float> > * points){
    if (node == nullptr) return 0;
    do{
        float x = 0, y = 0, z = 0;
        vector<float> sub_points;
        const XMLAttribute *att =  node->FirstAttribute();
        if (!strcmp(att->Name(),"point")) att = att->Next();
        while (att != nullptr) {
            if( !strcmp(att->Name(),"X") ){
                x = stof(att->Value());
            }else if( !strcmp(att->Name(),"Y") ){
                y = stof(att->Value());
            }else if( !strcmp(att->Name(),"Z") ){
                z = stof(att->Value());
            }
            att = att->Next();
        }
        sub_points.push_back(x);
        sub_points.push_back(y);
        sub_points.push_back(z);
        points->push_back(sub_points);
        node = node->NextSiblingElement();
    }while (node != nullptr);

    return 1;
}

int loadTranslate(XMLElement* node_element, Group * group){
    const XMLAttribute * att = node_element->FirstAttribute();
    if (att == nullptr) return 0;
    float x = 0, y = 0, z = 0, angle = 0, time=0;
    bool derived=false, catmull=false;
    vector<vector<float> > points;
    do{
        if( !strcmp(att->Name(),"X") ){
            x = stof(att->Value());
        }else if( !strcmp(att->Name(),"Y") ){
            y = stof(att->Value());
        }else if( !strcmp(att->Name(),"Z") ){
            z = stof(att->Value());
        }else if( !strcmp(att->Name(),"angle") ){
            angle = stof(att->Value());
        }else if( !strcmp(att->Name(),"time") ){
            time = stof(att->Value());
            loadTranslatePoints(node_element->FirstChildElement(),&points);
            catmull = true;
        }else if( !strcmp(att->Name(), "deriv")){
            if (!strcmp(att->Value(), "true")) {
                derived = true;
            }else{
                derived = false;
            }
        }
        att = att->Next();
    }while (att != nullptr);
    Translate *t;
    if (catmull) {
        t = new Translate(points,time,derived);
    }else{
        t = new Translate(x,y,z,angle);
    }
    
    return group->addTransformation("translate", t);
}



int loadRotate(XMLElement* node_element, Group * group){
    const XMLAttribute * att = node_element->FirstAttribute();
    if (att == nullptr) return 0;
    float x = 0, y = 0, z = 0, angle = 0, time = 0;
    
    do{
        if( !strcmp(att->Name(),"axisX") ){
            x = stof(att->Value());
        }else if( !strcmp(att->Name(),"axisY") ){
            y = stof(att->Value());
        }else if( !strcmp(att->Name(),"axisZ") ){
            z = stof(att->Value());
        }else if( !strcmp(att->Name(),"ANGLE") ){
            angle = stof(att->Value());
        }else if( !strcmp(att->Name(),"time") ){
            time = stof(att->Value());
            angle = (float) 360/(time*1000);
        }
        att = att->Next();
    }
    while (att != nullptr);
    
    Rotate *r = new Rotate(x,y,z,angle,time);
    return group->addTransformation("rotate", r);
}

int loadScale(XMLElement* node_element, Group * group){
    const XMLAttribute * att = node_element->FirstAttribute();
    if (att == nullptr) return 0;
    float x = 0, y = 0, z = 0;
    
    do{
        if( !strcmp(att->Name(),"X") ){
            x = stof(att->Value());
        }else if( !strcmp(att->Name(),"Y") ){
            y = stof(att->Value());
        }else if( !strcmp(att->Name(),"Z") ){
            z = stof(att->Value());
        }
        att = att->Next();
    }while (att != nullptr);
    Scale *s = new Scale(x,y,z,0);
    return group->addTransformation("scale", s);
    
    
}

int loadModels(XMLElement* node, Group * group) {
    if (node == nullptr) return 0;
    int error = 1;
    while (node && error) {
        if (strcmp(node->Name(), "model")==0){
            error *= loadModel(node, group);
        }
        else {
            return 0;
        }
        node = node->NextSiblingElement();
    }
    return error;
}

int loadLights(XMLElement* node, Group * group){
    int type;
    float cutoff = 0;
    float dir[3] = { 0.0f, 0.0f, -1.0f };
    float pos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
    float ambi[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diff[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float spec[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const XMLAttribute * att = node->FirstAttribute();
    node = node->FirstChildElement("light");


    
    while(node){
        int nlight = 0;
        while(att){
            if(!strcmp(att->Name(),"type")){ 
                if(!strcmp(att->Value(),"Directional")){
                    type = 1;
                }
                if(!strcmp(att->Value(),"Spotlight")){
                    type = 2;
                }
                if(!strcmp(att->Value(),"Point")){
                    type = 3;
                }
            }

            else if(!strcmp(att->Name(),"cutoff") ){

                cutoff = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"diffR") ){

                diff[0] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"diffG")){

                diff[1] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"diffB")){

                diff[2] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"specR")){

                spec[0] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"specG")){
                spec[1] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"specB")){
                spec[2] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"ambiR")){
                ambi[0] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"ambiG")){
                ambi[1] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"ambiB")){
                ambi[2] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"px")){
                pos[0] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"py")){
                pos[1] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"pz")){
                pos[2] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"dirx")){
                dir[0] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"diry")){
                dir[1] = att->FloatValue();
            }
            else if(!strcmp(att->Name(),"dirz")){
                dir[2] = att->FloatValue();
            }
            att = att->Next();
        }
        Light l = *new Light(type,nlight,cutoff,dir,pos,ambi,diff,spec);
        group->lights.push_back(l);
        node = node->NextSiblingElement();
        nlight++;
    }

    return 1;
}

int readTree(XMLElement* node, Group * group){
    if (node == nullptr) return 0;
    int error = 1;
    while (node && error) {
        if (strcmp(((XMLElement*) node)->Name(), "group")==0) {
            Group *childGroup = new Group();
            error *= readTree(node->FirstChildElement(),childGroup);
            group->addGroup(childGroup);
        }else if (strcmp(node->Name(), "translate")==0){
            error *= loadTranslate(node, group);
        }else if (strcmp(node->Name(), "rotate")==0){
            error *= loadRotate(node, group);
        }else if (strcmp(node->Name(), "scale")==0){
            error *= loadScale(node, group);
        }else if (strcmp(node->Name(), "models")==0){
            error *=  loadModels(node->FirstChildElement(), group);
        }else if (strcmp(node->Name(), "model")==0){
            error *= loadModel(node, group);
        }else if (strcmp(node->Name(), "lights")==0){
            error *= loadLights(node, group);
        }
        node = node->NextSiblingElement();
    }
    return error;
}

int loadXML(const char* filename, Controls* ctrl ){
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile(filename);
    XMLCheckResult(eResult);
    XMLNode * pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr) return XML_ERROR_FILE_READ_ERROR;
    int error = readTree(pRoot->FirstChildElement(), ctrl->scene);
    xmlDoc.Clear();
    return error;
}
