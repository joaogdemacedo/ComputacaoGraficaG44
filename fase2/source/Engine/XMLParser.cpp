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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &(points[0]), GL_STATIC_DRAW);
}


int readBinaryModel(const char * fileName, vector<float>* points){
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

int loadModel(XMLElement* node_element, Group * group){
    const char * fileName = node_element->FirstAttribute()->Value();
    if (fileName == nullptr) return 0;
    Primitive * p;
    if ((*group->load_models)[fileName] == nullptr) {
        int size;
        GLuint idv;
        vector<float> points;
        size = readBinaryModel(fileName, &points);
        genModel(&idv, points);
        p = new Primitive(idv,size);
        (*group->load_models)[fileName] = p;
    }else{
        p = (*group->load_models)[fileName];
    }
    
    group->addPrimitive(p);
    return 1;
}

int loadTranslatePoints(XMLElement* node, vector<float>* points){
    if (node == nullptr) return 0;
    float x = 0, y = 0, z = 0;
    do{
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
            points->push_back(x);
            points->push_back(y);
            points->push_back(z);
        }
        node = node->NextSiblingElement();
    }while (node != nullptr);
    return 1;
}

int loadTranslate(XMLElement* node_element, Group * group){
    const XMLAttribute * att = node_element->FirstAttribute();
    if (att == nullptr) return 0;
    float x = 0, y = 0, z = 0, angle = 0, time;
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
            vector<float> points;
            time = stof(att->Value());
            loadTranslatePoints(node_element->FirstChildElement(),&points);
        }
        att = att->Next();
    }while (att != nullptr);
    Translate *t = new Translate(x,y,z,angle);
    return group->addTransformation("translate", t);
}



int loadRotate(XMLElement* node_element, Group * group){
    const XMLAttribute * att = node_element->FirstAttribute();
    if (att == nullptr) return 0;
    float x = 0, y = 0, z = 0, angle = 0, time = 0;
    
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

int readTree(XMLElement* node, Group * group){
    if (node == nullptr) return 0;
    int error = 1;
    while (node && error) {
        if (strcmp(((XMLElement*) node)->Name(), "group")==0) {
            Group *childGroup = new Group(group->load_models);
            error *= readTree(node->FirstChildElement(),childGroup);
            group->addGroup(childGroup);
        }else if (strcmp(node->Name(), "translate")==0){
            error *= loadTranslate(node, group);
        }else if (strcmp(node->Name(), "rotate")==0){
            error *= loadRotate(node, group);
        }
        else if (strcmp(node->Name(), "scale")==0){
            error *= loadScale(node, group);
        }else if (strcmp(node->Name(), "models")==0){
            error *=  loadModels(node->FirstChildElement(), group);
        }else if (strcmp(node->Name(), "model")==0){
            error *= loadModel(node, group);
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
