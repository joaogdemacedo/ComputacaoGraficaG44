//
//  Translate.cpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#include "Translate.hpp"
Translate::Translate(){
    Transformation::x = 0;
    Transformation::y = 0;
    Transformation::z = 0;
    Transformation::angle = 0;
}
Translate::Translate(vector<vector<float>> route, float time, bool deriv){
    Translate::route  = *new vector<vector<float>>(route);
    Translate::time = time;
    Translate::ypos = *new vector<float>(route.at(0));
    Translate::time_bool = true;
    Translate::deriv = deriv;
}
Translate::Translate(float x_, float y_, float z_, float angle_){
    Transformation::x = x_;
    Transformation::y = y_;
    Transformation::z = z_;
    Transformation::angle = angle_;
}



void Translate::transform(int routes){
    if (Translate::time_bool) {
        if (routes) {
            drawRoute();
        }
        catmullRomTranslate();
    }else {
        glTranslatef(Transformation::x, Transformation::y, Transformation::z);
    }
}
Translate::~Translate(){
    Transformation::x = 0;
    Transformation::y = 0;
    Transformation::z = 0;
    Transformation::angle = 0;
}

void Translate::multMatrixVector(float *m, float *v, float *res) {
    
    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }
    
}


void Translate::buildRotMatrix(float *x, float *y, float *z, float *m) {
    
    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void Translate::cross(float *a, float *b, float *res) {
    
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}


void Translate::normalize(float *a) {
    
    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    if (l == 0) {
        a[0] = 0;
        a[1] = 0;
        a[2] = 0;
        return;
    }
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

void Translate::getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {
    
    // catmull-rom matrix
    float m[4][4] = {    {-0.5f,  1.5f, -1.5f,  0.5f},
        { 1.0f, -2.5f,  2.0f, -0.5f},
        {-0.5f,  0.0f,  0.5f,  0.0f},
        { 0.0f,  1.0f,  0.0f,  0.0f}};
    
    // Compute A = M * P
    float x[4] = {p0[0],p1[0],p2[0],p3[0]};
    float y[4] = {p0[1],p1[1],p2[1],p3[1]};
    float z[4] = {p0[2],p1[2],p2[2],p3[2]};
    float A[4][4];
    
    multMatrixVector(*m, x, *A);
    multMatrixVector(*m, y, *(A+1));
    multMatrixVector(*m, z, *(A+2));
    // Compute pos = T * A
    float T[4] = {powf(t, 3.0),powf(t, 2),t,1};
    multMatrixVector(*A, T, pos);
    float TL[4] = {3*powf(t, 2.0),2*t,1,0};
    // compute deriv = T' * A
    multMatrixVector(*A, TL, deriv);
    
}


// given  global t, returns the point in the curve

void Translate:: getGlobalCatmullRomPoint(float gt, float *pos, float *deriv) {
    int POINT_COUNT = Translate::route.size();
    float t = gt * POINT_COUNT; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment
    
    
    // indices store the points
    int indices[4];
    indices[0] = (index + POINT_COUNT-1)%POINT_COUNT;
    indices[1] = (indices[0]+1)%POINT_COUNT;
    indices[2] = (indices[1]+1)%POINT_COUNT;
    indices[3] = (indices[2]+1)%POINT_COUNT;
    
    getCatmullRomPoint(t, &(route.at(indices[0])[0]), &(route.at((indices[1]))[0]), &(route.at((indices[2]))[0]), &(route.at((indices[3]))[0]), pos, deriv);
}


void Translate::catmullRomTranslate() {
    float z[4];
    float deriv[4];
    float m[16];
    float pos[4] = {0,0,0,0};
    float *y = &ypos[0];
    getGlobalCatmullRomPoint((glutGet(GLUT_ELAPSED_TIME)/(1000*Translate::time)), pos, deriv);

    glTranslatef(pos[0], pos[1], pos[2]);
    
    normalize(deriv);
    normalize(y);
    cross(deriv,y,z);
    normalize(z);
    cross(z,deriv,y);
    if (Translate::deriv) {
        buildRotMatrix(deriv, y, z, m);
        glMultMatrixf(m);
    }
    
}





void Translate::drawRoute(){
    float pos[4]={0,0,0,0};
    float deriv[4]={0,0,0,0};
    glBegin(GL_LINE_LOOP);
    for (int t = 0 ; t<1000; t++) {
        float dt = t*0.001;
        getGlobalCatmullRomPoint(dt, pos, deriv);
        glVertex3f(pos[0], pos[1], pos[2]);
    }
    glEnd();
    
    return;
}








