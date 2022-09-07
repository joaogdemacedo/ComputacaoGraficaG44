//
//  Rotate.hpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#ifndef Rotate_hpp
#define Rotate_hpp
#include "Transformation.hpp"


class Rotate : public Transformation{
public:
    int time;
    Rotate();
    Rotate(float,float,float,float,int);
    void transform();
    ~Rotate();
};
#endif /* Scale_hpp */

