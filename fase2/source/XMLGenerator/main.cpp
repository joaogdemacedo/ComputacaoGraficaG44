#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

typedef struct point {
    float x;
    float y;
    float z;
}Point;

struct pointComp{
    bool operator ()(const Point& x, const Point& y) {
        return x.x<y.x && x.y<y.y && x.z<y.z;
    }

};


void write3DModel(const string fileName, vector<Point> points){
    ofstream fout(fileName,std::ios::binary);
    float x;
    for (auto p:points) {
        x = p.x;
        fout.write(reinterpret_cast<char*>(&x), sizeof(float));
        x= p.y;
        fout.write(reinterpret_cast<char*>(&x), sizeof(float));
        x= p.z;
        fout.write(reinterpret_cast<char*>(&x), sizeof(float));
    }
    fout.close();
    return;
}

void generatePlane(const string fileName, float size){
    Point tmp;
    float alpha;
    float rho;
    vector<Point> points;
    
    rho = (size/2)*sqrt(2);
    
    for (int i = 0; i<4; i++) {
        alpha = (i*90 + 45) * 2 * M_PI / 360;
        Point pt1 = {rho*sinf(alpha),0,(rho*cosf(alpha))};
        if (i == 0) {
            tmp = pt1;
        }else if(i == 2){
            points.push_back(pt1);
        }
        points.push_back(pt1);
    }
    points.push_back(tmp);
    vector<Point>::const_iterator first = points.begin();
    vector<Point>::const_iterator middle = points.begin() + 3;
    vector<Point>::const_iterator last = points.begin() + 6;
    vector<Point> newVec(first, middle);
    vector<Point> newVec2(middle, last);
    reverse(newVec.begin(),newVec.end());
    reverse(newVec2.begin(),newVec2.end());
    
    points.insert(points.end(), newVec.begin(), newVec.end());
    points.insert(points.end(), newVec2.begin(), newVec2.end());
    //writeFile(fileName, points);
    write3DModel(fileName, points);
    return;
}

void generateBoxFrontBack(float sizex, float sizey, float sizez, float divisions, vector<Point>* points){
    float stepx = sizex/divisions;
    float stepy = sizey/divisions;
    float z = sizez/2;
    float div = divisions/2;
    
    
    for (float yi = -div; yi < div; yi++) {
        for (float xi = -div; xi < div; xi++ ){
            float y1 = stepy*yi;
            float y2 = stepy*(yi+1);
            float x1 = stepx*xi;
            float x2 = stepx*(xi+1);
            Point pt1 = {x1, y1, z};
            Point pt2 = {x2, y1, z};
            Point pt3 = {x2, y2, z};
            Point pt4 = {x1, y2, z};
            points->push_back(pt1), points->push_back(pt2), points->push_back(pt3);
            points->push_back(pt3), points->push_back(pt4), points->push_back(pt1);
            pt1.z = -z, pt2.z = -z, pt3.z = -z, pt4.z = -z;
            points->push_back(pt3), points->push_back(pt2), points->push_back(pt1);
            points->push_back(pt1), points->push_back(pt4), points->push_back(pt3);
        }
    }
    return;
}
void generateBoxUpDown(float sizex, float sizey, float sizez, float divisions, vector<Point> *points){
    float stepx = sizex/divisions;
    float stepz = sizez/divisions;
    float y = sizey/2;
    float div = divisions/2;
    
    for (float zi = -div; zi < div; zi++) {
        for (float xi = -div; xi < div; xi++ ){
            float x1 = stepx*xi;
            float x2 = stepx*(xi+1);
            float z1 = stepz*zi;
            float z2 = stepz*(zi+1);
            Point pt1 = {x1, y, z1};
            Point pt2 = {x2, y, z1};
            Point pt3 = {x2, y, z2};
            Point pt4 = {x1, y, z2};
            points->push_back(pt3), points->push_back(pt2), points->push_back(pt1);
            points->push_back(pt1), points->push_back(pt4), points->push_back(pt3);
            pt1.y = -y, pt2.y = -y, pt3.y = -y, pt4.y = -y;
            points->push_back(pt1), points->push_back(pt2), points->push_back(pt3);
            points->push_back(pt3), points->push_back(pt4), points->push_back(pt1);
            
        }
    }
    return;
}

void generateBoxRightLeft(float sizex, float sizey, float sizez, float divisions, vector<Point> *points){
    vector<Point> right, left;
    float stepy = sizey/divisions;
    float stepz = sizez/divisions;
    float x = sizex/2;
    float div = divisions/2;
    
    
    for (float yi = -div; yi < div; yi++) {
        for (float zi = -div; zi < div; zi++ ){
            float y1 = stepy*yi;
            float y2 = stepy*(yi+1);
            float z1 = stepz*zi;
            float z2 = stepz*(zi+1);
            Point pt1 = {x, y1, z1};
            Point pt2 = {x, y1, z2};
            Point pt3 = {x, y2, z2};
            Point pt4 = {x, y2, z1};
            points->push_back(pt3), points->push_back(pt2), points->push_back(pt1);
            points->push_back(pt1), points->push_back(pt4), points->push_back(pt3);
            pt1.x = -x, pt2.x = -x, pt3.x = -x, pt4.x = -x;
            points->push_back(pt1), points->push_back(pt2), points->push_back(pt3);
            points->push_back(pt3), points->push_back(pt4), points->push_back(pt1);
        }
    }
    return;
}

void generateBox(const string fileName, float sizex, float sizey, float sizez, int divisions){
    vector<Point> points;
    generateBoxUpDown(sizex, sizey, sizez, divisions, &points);
    generateBoxFrontBack(sizex, sizey, sizez, divisions, &points);
    generateBoxRightLeft(sizex, sizey, sizez, divisions, &points);
    write3DModel(fileName, points);
}

void generateCone(const string fileName, float radius, float height, int slices, int stacks){
    vector<Point> points;
    vector<Point> base;
    Point orig = {0,0,0};
    
    // a1    a2
    // |      |
    // 2------1 -- h2|r2
    // |\ _   |
    // |    \ |
    // 3------4 -- h1|r1
    //
    
    
    for (int st = 0; st < stacks; st++) {
        float stack_relation1 = ((float)st)/stacks;
        float stack_relation2 = ((float)st+1)/stacks;
        float cur_height1 = height * stack_relation1;
        float cur_height2 = height * stack_relation2;
        float rad1 = radius * (1-stack_relation1);
        float rad2 = radius * (1-stack_relation2);
        for (int sl = 0; sl < slices; sl++) {
            
            float angle1 = ((float) sl) * 2 * M_PI / slices;
            float angle2 = ((float) sl+1) * 2 * M_PI / slices;
           
            
            Point pt1 = {sinf(angle2)*rad2, cur_height2, cosf(angle2)*rad2};
            Point pt2 = {sinf(angle1)*rad2, cur_height2, cosf(angle1)*rad2};
            Point pt3 = {sinf(angle1)*rad1, cur_height1, cosf(angle1)*rad1};
            Point pt4 = {sinf(angle2)*rad1, cur_height1, cosf(angle2)*rad1};
            
            
            if (st+1 == stacks) {
                points.push_back(pt2), points.push_back(pt3), points.push_back(pt4);
            }
            else {
                if (st == 0) {
                    base.push_back(orig), base.push_back(pt4), base.push_back(pt3);
                }
                points.push_back(pt2), points.push_back(pt3), points.push_back(pt4);
                points.push_back(pt1), points.push_back(pt2), points.push_back(pt4);
                
            }
        }
    }
    points.insert(points.end(), base.begin(), base.end());
    write3DModel(fileName, points);
}

void generateCylinder(const string fileName, float radius, float height, int slices, int stacks){
    vector<Point> points;
    vector<Point> base;
    vector<Point> top;
    Point base_orig = {0,0,0};
    Point top_orig = {0,height,0};
    
    
    // a1    a2
    // |      |
    // 2------1 -- h2
    // |\ _   |
    // |    \ |
    // 3------4 -- h1
    //
    
    
    for (int st = 0; st < stacks; st++) {
        float stack_relation1 = ((float)st)/stacks;
        float stack_relation2 = ((float)st+1)/stacks;
        float cur_height1 = height * stack_relation1;
        float cur_height2 = height * stack_relation2;
        for (int sl = 0; sl < slices; sl++) {
            
            float angle1 = ((float) sl) * 2 * M_PI / slices;
            float angle2 = ((float) sl+1) * 2 * M_PI / slices;
            
            
            Point pt1 = {sinf(angle2)*radius, cur_height2, cosf(angle2)*radius};
            Point pt2 = {sinf(angle1)*radius, cur_height2, cosf(angle1)*radius};
            Point pt3 = {sinf(angle1)*radius, cur_height1, cosf(angle1)*radius};
            Point pt4 = {sinf(angle2)*radius, cur_height1, cosf(angle2)*radius};
            
            
            if (st+1 == stacks) {
                top.push_back(pt2), top.push_back(pt1), top.push_back(top_orig);
            }
            else  if (st == 0) {
                base.push_back(base_orig), base.push_back(pt4), base.push_back(pt3);
            }
            
            points.push_back(pt2), points.push_back(pt3), points.push_back(pt4);
            points.push_back(pt1), points.push_back(pt2), points.push_back(pt4);
        }
    }
    points.insert(points.end(), base.begin(), base.end());
    points.insert(points.end(), top.begin(), top.end());
    write3DModel(fileName, points);
}

void generateSphere(const string fileName, float radius, float slices, float stacks){
    vector<Point> mesh;
    for( float t = 0 ; t < stacks ; t = t + 1.0 ) // stacks are ELEVATION so they count theta
    {
        float theta1 = t*M_PI/stacks ;
        float theta2 = ( (t+1.0)*M_PI/stacks ) ;
        
        for( float p = 0 ; p < slices ; p = p + 1.0 ) // slices are ORANGE SLICES so the count azimuth
        {
            float phi1 = (p*2*M_PI /slices); // azimuth goes around 0 .. 2*PI
            float phi2 = (p+1.0)*2*M_PI/slices ;
            
            //phi1   phi2
            // |      |
            // 1------2 -- theta1
            // |\ _   |
            // |    \ |
            // 4------3 -- theta2
            //
            
            Point pt1 = {radius * sinf(theta1) * sinf(phi1),
                radius * cosf(theta1),
                radius*sinf(theta1)*cosf(phi1)};
            Point pt2 = {radius * sinf(theta1) * sinf(phi2),
                radius * cosf(theta1),
                radius*sinf(theta1)*cosf(phi2)};
            Point pt3 = {radius * sinf(theta2) * sinf(phi2),
                radius * cosf(theta2),
                radius*sinf(theta2)*cosf(phi2)};
            Point pt4 = {radius * sinf(theta2) * sinf(phi1),
                radius * cosf(theta2),
                radius*sinf(theta2)*cosf(phi1)};

            
            if( t == 0 ) {// top cap
               mesh.push_back(pt1);
                mesh.push_back(pt4);
                mesh.push_back(pt3);
            }
            else if( t + 1 == stacks ) {//end cap
                mesh.push_back(pt3);
                mesh.push_back(pt2);
                mesh.push_back(pt1);
            }
            else
            {
                mesh.push_back(pt3), mesh.push_back(pt2), mesh.push_back(pt1);
                mesh.push_back(pt4), mesh.push_back(pt3), mesh.push_back(pt1);
            }
        }
    }
    write3DModel(fileName, mesh);
}

void generateTorus(const string fileName, float innerRadius, float outerRadius, float slices, float stacks){
    vector<Point> mesh, circleA, circleB;
    float alpha=(2*M_PI)/(stacks*2), beta=(2*M_PI)/slices;
    float circRadius=(outerRadius-innerRadius)/2;
    float circCenter=(innerRadius+outerRadius)/2;
    for( float t = 0 ; t < slices+1 ; t = t + 1.0 ) {
        Point center= {circCenter*sinf(beta*t),0,circCenter*cosf(beta*t)};
        for( float p = 0 ; p < stacks*2+1 ; p = p + 1.0 ) {
            Point pt= {
                center.x+(circRadius*cosf(alpha*p)*sinf(beta*t)),
                circRadius*sinf(alpha*p),
                center.z+(circRadius*cosf(alpha*p)*cosf(beta*t))};
            circleB.push_back(pt);
        }
        if(t>0) {
            for(float o = 0 ; o < stacks*2 ; o = o + 1.0 ) {
                mesh.push_back(circleA.at(o));
                mesh.push_back(circleB.at(o));
                mesh.push_back(circleA.at(o+1));
                mesh.push_back(circleB.at(o));
                mesh.push_back(circleB.at(o+1));
                mesh.push_back(circleA.at(o+1));
            }
        }
        circleA=circleB;
        circleB.clear();
    }
    write3DModel(fileName, mesh);
}

int main(int argc, char **argv){
    if (argc<2) {
        return 0;
    }
    else{
        if (strcmp("cone", argv[1])==0) {
            generateCone(argv[argc-1], atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
        }if (strcmp("box", argv[1])==0) {
            generateBox(argv[argc-1], atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
        }if (strcmp("plane", argv[1])==0) {
            generatePlane(argv[argc-1], atof(argv[2]));
        }if (strcmp("sphere", argv[1])==0) {
            generateSphere(argv[argc-1], atof(argv[2]), atof(argv[3]), atof(argv[4]));
        }if (strcmp("cylinder", argv[1])==0) {
            generateCylinder(argv[argc-1], atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
        }if (strcmp("torus", argv[1])==0) {
            generateTorus(argv[argc-1], atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
        }
    }
    
    return 0;
}

