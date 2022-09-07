//
//  Translate.hpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#ifndef Translate_hpp
#define Translate_hpp
#include <vector>
#include "Transformation.hpp"

using namespace std;
class Translate : public Transformation{
public:
    vector<float> route;
    float time;
    
    Translate();
    Translate(float,float,float,float);
    void transform();
    ~Translate();
};
#endif /* Scale_hpp */

