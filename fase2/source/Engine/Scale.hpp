//
//  Scale.hpp
//  Project
//
//  Created by Trident on 14/03/2018.
//

#ifndef Scale_hpp
#define Scale_hpp
#include "Transformation.hpp"


class Scale : public Transformation{
public:
    Scale();
    Scale(float,float,float,float);
    void transform();
    ~Scale();
};
#endif /* Scale_hpp */
