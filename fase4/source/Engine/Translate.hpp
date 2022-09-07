//
//  Translate.hpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#ifndef Translate_hpp
#define Translate_hpp
#include <vector>
#include <math.h>
#include "Transformation.hpp"

using namespace std;
class Translate : public Transformation{
public:
    vector<vector<float>> route;
    vector<float> ypos;
    float time;
    bool time_bool = false;
    bool deriv = false;
    
    
    
    
    Translate();
    Translate(float,float,float,float);
    Translate(vector<vector<float>>, float, bool);
    void transform(int routes);
    ~Translate();
    void multMatrixVector(float *m, float *v, float *res);
    void buildRotMatrix(float *x, float *y, float *z, float *m);
    void cross(float *a, float *b, float *res);
    void normalize(float *a);
    void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv);
    void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv);
    void catmullRomTranslate();
    void drawRoute();
};
#endif /* Scale_hpp */

