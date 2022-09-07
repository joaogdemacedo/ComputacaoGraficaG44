#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <iterator>
#include <algorithm>
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

std::vector<std::string> string_split(std::string s, const char delimiter)
{
    size_t start=0;
    size_t end=s.find_first_of(delimiter);
    
    std::vector<std::string> output;
    
    while (end <= std::string::npos)
    {
        output.emplace_back(s.substr(start, end-start));
        
        if (end == std::string::npos)
            break;
        
        start=end+1;
        end = s.find_first_of(delimiter, start);
    }
    
    return output;
}


vector<vector<float>> prod(vector<vector<float>> A, vector<vector<float>> B){
    int row = A.size();
    int cols = B.at(0).size();
    int shared = A.at(0).size();
    int check = B.size();
    vector<vector<float>> C(row,vector<float>(cols,0));
    if (shared != check) {
        return vector<vector<float>>(0,vector<float>(0,0));
    }
    
    for (int i = 0 ; i < row; i++) {
        for (int j = 0; j < cols ; j++){
            float sum = 0;
            for (int k = 0; k < shared ; k++){
                sum += A.at(i).at(k)*B.at(k).at(j);
            }
            C.at(i).at(j) = sum;
        }
    }
    
    return C;
}




void generateUV(vector<vector<float>> *UM, vector<vector<float>> *VM, float ui, float vi){
    vector<vector<float>> um(1,vector<float>(4,0));
    vector<vector<float>> vm(4,vector<float>(1,0));
    um.at(0).at(0)=powf(static_cast<float>(ui),3.0 );;
    um.at(0).at(1)=powf(static_cast<float>(ui),2.0);;
    um.at(0).at(2)=ui;
    um.at(0).at(3)=1;
    vm.at(0).at(0)=powf(static_cast<float>(vi),3.0 );
    vm.at(1).at(0)=powf(static_cast<float>(vi),2.0);
    vm.at(2).at(0)=vi;
    vm.at(3).at(0)=1;
    
    *UM = um;
    *VM = vm;
}



void patchBuild(vector<int> patch, vector<vector<float>> xyzcomp, vector<vector<float>> *x, vector<vector<float>>* y, vector<vector<float>>* z){
    for(int i = 0; i < patch.size(); i++){
        x->at(i/4).at(i%4) = xyzcomp.at(patch.at(i)).at(0);
        y->at(i/4).at(i%4) = xyzcomp.at(patch.at(i)).at(1);
        z->at(i/4).at(i%4) = xyzcomp.at(patch.at(i)).at(2);
    }
}



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
                mesh.push_back(circleA.at(o));
            }
        }
        circleA=circleB;
        circleB.clear();
    }
    write3DModel(fileName, mesh);
}

void doSquare(float u, float v,
               vector<vector<float>> x,
               vector<vector<float>> y,
               vector<vector<float>> z, vector<Point> *points, float step){
    Point tmp;
    float ui,vi;
    ui = u;
    vi = v;
    for (int i = 0; i< 2; i++) {
        for (int j = 0; j<2; j++) {
            vector<vector<float>> um;
            vector<vector<float>> vm;
            vector<vector<float>> ptx;
            vector<vector<float>> pty;
            vector<vector<float>> ptz;
            ui = u + i*step;
            vi = v + j*step;
            generateUV(&um, &vm, ui, vi);
            
            ptx = prod(um,x);
            ptx = prod(ptx,vm);
            pty = prod(um,y);
            pty = prod(pty,vm);
            ptz = prod(um,z);
            ptz = prod(ptz,vm);
            
            Point point{ptx.at(0).at(0),pty.at(0).at(0),ptz.at(0).at(0)};
            if( i == 0 && j == 1){
                tmp = point;
            }else if( i == 1 && j == 0){
                points->push_back(point);
            }else if( i == 1 && j == 1){
                points->push_back(tmp);
            }
            points->push_back(point);
        }
    }
}


static std::ifstream patchIndexBuild(string &line, int &npatches, const string &patch, vector<vector<int> > &patch_index) {
    std::ifstream input( patch );
    getline(input,line);
    npatches = stoi(line);
    for (int i = 0; i<npatches; i++) {
        vector<string> fields;
        vector<int> index;
        getline(input, line);
        fields = string_split(line,',');
        std::transform(fields.begin(), fields.end(), std::back_inserter(index),
                       [](const std::string& str) { return std::stoi(str);});
        patch_index.push_back(index);
    }
    return input;
}

static void xyzCompBuild(int &cpoints, std::ifstream &input, string &line, vector<vector<float> > &xyzcomponent) {
    getline(input, line);
    cpoints = stoi(line);
    for (int i = 0; i< cpoints; i++) {
        vector<string> fields;
        vector<float> comp;
        getline(input, line);
        fields = string_split(line,',');
        std::transform(fields.begin(), fields.end(), std::back_inserter(comp),
                       [](const std::string& str) { return std::stof(str); });
        xyzcomponent.push_back(comp);
    }
}

static void patchPointsBuild(const vector<vector<float>> &M, int npatches,
                             vector<vector<int> > &patch_index, vector<Point> &points,
                             const vector<vector<float> > &xyzcomponent, int tesselation) {
    float step = 0.1/tesselation;
    for (int i = 0; i<npatches; i++){
        vector<vector<float>> x(4,vector<float>(4,0));
        vector<vector<float>> y(4,vector<float>(4,0));
        vector<vector<float>> z(4,vector<float>(4,0));
        patchBuild(patch_index.at(i),xyzcomponent, &x,&y,&z);
        
        x = prod(M,x);
        x = prod(x,M);
        y = prod(M,y);
        y = prod(y,M);
        z = prod(M,z);
        z = prod(z,M);
        for (float u = 0; u<1; u+=step) {
            for (float v = 0; v<1; v+=step) {
                doSquare(u,v,x,y,z,&points,step);
            }
        }
    }
}

void generateBezier(const string fileName, const string patch, int tesselation){
    string line;
    int cpoints;
    int npatches;
    vector<Point> points;
    vector<vector<float>> M = {
        {-1,3,-3,1},
        {3,-6,3,0},
        {-3,3,0,0},
        {1,0,0,0}
    };
    vector<vector<int>> patch_index;
    vector<vector<float>> xyzcomponent;
    std::ifstream input = patchIndexBuild(line, npatches, patch, patch_index);
    xyzCompBuild(cpoints, input, line, xyzcomponent);
    patchPointsBuild(M, npatches, patch_index, points, xyzcomponent, tesselation);
    write3DModel(fileName, points);
    return;
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
        }if (strcmp("bezier", argv[1])==0) {
            generateBezier(argv[2],argv[3],atoi(argv[4]));
        }
    }
    
    return 0;
}

