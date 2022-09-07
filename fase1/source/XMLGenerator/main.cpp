#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;


typedef struct point{
    double x;
    double y;
    double z;
}Point;


void writeFile(const string fileName, vector<Point> points){
    
    string text = "";
    for (vector<Point>::iterator point = points.begin(); point != points.end(); ++point){
        text.append(to_string(point->x));
        text.append(" ");
        text.append(to_string(point->y));
        text.append(" ");
        text.append(to_string(point->z));
        text.append("\n");
    }
    
    ofstream outfile(fileName, ofstream::binary);
    if(outfile){
        outfile << text;
        outfile.close();
    }
    else{
        cout << "File not created" << endl;
    }
    
}



void generatePlane(const string fileName, double size){
    Point tmp;
    float alpha;
    float rho;
    vector<Point> points;
    
    rho = (size/2)*sqrt(2)/2;
    
    for (int i = 0; i<4; i++) {
        alpha = (i*90 + 45) * 2 * M_PI / 360;
        Point pt1 = {rho*sin(alpha),0,rho*cos(alpha)};
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
    vector<Point> newVec2(middle-1, last);
    reverse(newVec.begin(),newVec.end());
    reverse(newVec2.begin(),newVec2.end());

    points.insert(points.end(), newVec.begin(), newVec.end());
    points.insert(points.end(), newVec2.begin(), newVec2.end());
    writeFile(fileName, points);
    return;
}

void generateBoxFrontBack(double sizex, double sizey, double sizez, double divisions, vector<Point>* points){
    double stepx = sizex/divisions;
    double stepy = sizey/divisions;
    double z = sizez/2;
    double div = divisions/2;
    
    
    for (double yi = -div; yi < div; yi++) {
        for (double xi = -div; xi < div; xi++ ){
            double y1 = stepy*yi;
            double y2 = stepy*(yi+1);
            double x1 = stepx*xi;
            double x2 = stepx*(xi+1);
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
void generateBoxUpDown(double sizex, double sizey, double sizez, double divisions, vector<Point> *points){
    double stepx = sizex/divisions;
    double stepz = sizez/divisions;
    double y = sizey/2;
    double div = divisions/2;
    
    for (double zi = -div; zi < div; zi++) {
        for (double xi = -div; xi < div; xi++ ){
            double x1 = stepx*xi;
            double x2 = stepx*(xi+1);
            double z1 = stepz*zi;
            double z2 = stepz*(zi+1);
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

void generateBoxRightLeft(double sizex, double sizey, double sizez, double divisions, vector<Point> *points){
    vector<Point> right, left;
    double stepy = sizey/divisions;
    double stepz = sizez/divisions;
    double x = sizex/2;
    double div = divisions/2;
    
    
    for (double yi = -div; yi < div; yi++) {
        for (double zi = -div; zi < div; zi++ ){
            double y1 = stepy*yi;
            double y2 = stepy*(yi+1);
            double z1 = stepz*zi;
            double z2 = stepz*(zi+1);
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
void generateBox(const string fileName, double sizex, double sizey, double sizez, int divisions){
    vector<Point> points;
    generateBoxUpDown(sizex, sizey, sizez, divisions, &points);
    generateBoxFrontBack(sizex, sizey, sizez, divisions, &points);
    generateBoxRightLeft(sizex, sizey, sizez, divisions, &points);
    writeFile(fileName, points);
}

void generateCone(const string fileName, double radius, double height, int slices, int stacks){
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
        double stack_relation1 = ((double)st)/stacks;
        double stack_relation2 = ((double)st+1)/stacks;
        double cur_height1 = height * stack_relation1;
        double cur_height2 = height * stack_relation2;
        double rad1 = radius * (1-stack_relation1);
        double rad2 = radius * (1-stack_relation2);
        for (int sl = 0; sl < slices; sl++) {
            
            double angle1 = ((double) sl) * 2 * M_PI / slices;
            double angle2 = ((double) sl+1) * 2 * M_PI / slices;
           
            
            Point pt1 = {sin(angle2)*rad2, cur_height2, cos(angle2)*rad2};
            Point pt2 = {sin(angle1)*rad2, cur_height2, cos(angle1)*rad2};
            Point pt3 = {sin(angle1)*rad1, cur_height1, cos(angle1)*rad1};
            Point pt4 = {sin(angle2)*rad1, cur_height1, cos(angle2)*rad1};
            
            
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
    writeFile(fileName, points);
}

void generateCylinder(const string fileName, double radius, double height, int slices, int stacks){
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
        double stack_relation1 = ((double)st)/stacks;
        double stack_relation2 = ((double)st+1)/stacks;
        double cur_height1 = height * stack_relation1;
        double cur_height2 = height * stack_relation2;
        for (int sl = 0; sl < slices; sl++) {
            
            double angle1 = ((double) sl) * 2 * M_PI / slices;
            double angle2 = ((double) sl+1) * 2 * M_PI / slices;
            
            
            Point pt1 = {sin(angle2)*radius, cur_height2, cos(angle2)*radius};
            Point pt2 = {sin(angle1)*radius, cur_height2, cos(angle1)*radius};
            Point pt3 = {sin(angle1)*radius, cur_height1, cos(angle1)*radius};
            Point pt4 = {sin(angle2)*radius, cur_height1, cos(angle2)*radius};
            
            
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
    writeFile(fileName, points);
}

void generateSphere(const string fileName, double radius, double slices, double stacks){
    vector<Point> mesh;
    
    for( double t = 0 ; t < stacks ; t = t + 1.0 ) // stacks are ELEVATION so they count theta
    {
        double theta1 = t*M_PI/stacks ;
        double theta2 = ( (t+1.0)*M_PI/stacks ) ;
        
        for( double p = 0 ; p < slices ; p = p + 1.0 ) // slices are ORANGE SLICES so the count azimuth
        {
            double phi1 = (p*2*M_PI /slices); // azimuth goes around 0 .. 2*PI
            double phi2 = (p+1.0)*2*M_PI/slices ;
            
            //phi1   phi2
            // |      |
            // 1------2 -- theta1
            // |\ _   |
            // |    \ |
            // 4------3 -- theta2
            //
            
            Point pt1 = {radius * sin(theta1) * sin(phi1),
                radius * cos(theta1),
                radius*sin(theta1)*cos(phi1)};
            Point pt2 = {radius * sin(theta1) * sin(phi2),
                radius * cos(theta1),
                radius*sin(theta1)*cos(phi2)};
            Point pt3 = {radius * sin(theta2) * sin(phi2),
                radius * cos(theta2),
                radius*sin(theta2)*cos(phi2)};
            Point pt4 = {radius * sin(theta2) * sin(phi1),
                radius * cos(theta2),
                radius*sin(theta2)*cos(phi1)};
            
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
    writeFile(fileName, mesh);
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
        }
    }
    
    return 0;
}

