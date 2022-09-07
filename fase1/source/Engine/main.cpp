#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;
#define LINE 1
#define FILL 2
#define BACK 3
#define FRONT 4



enum Transformation_Type {TRANSLATE, ROTATE, SCALE, COLOR};

typedef struct controls {
    float rad = 5.0;
    float theta = 90*(2*M_PI)/360;
    float phi = 0;
    int frame=0;
    int time;
    int timebase=0;
    float x;
    float y;
    float z;
    char * name;
}Controls;


typedef struct point{
    double x;
    double y;
    double z;
}Point;

typedef vector<Point> Primitive;
typedef vector<Primitive> Model;
typedef vector<struct transformation> Tft;
Model models;
Controls ctrl;


void drawAxis(){
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 10);
    glVertex3f(0, 0, 0);
    glVertex3f(10, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 10, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, -10);
    glVertex3f(0, 0, 0);
    glVertex3f(-10, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, -10, 0);
    glEnd();
}
void drawModels(Model models){
    int i = 0;
    
    for (auto const& primitive: models) {
        glBegin(GL_TRIANGLES);
        for(auto const& pt: primitive){
            if (i < 3) {
                glColor3f(((float)51/255), ((float)153/255), ((float)51/255));
                i = (i+1);
            }else{
                glColor3f(0.0f, ((float)200/255), 0.0f);
                i = (i+1)%6;
            }
            glVertex3f(pt.x, pt.y, pt.z);
        }
        glEnd();
    }
}


void renderBitmapString(
                        float x,
                        float y,
                        float z,
                        void *font,
                        char *string) {
    
    char *c;
    glRasterPos3f(x, y,z);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


void keyboardHandler(unsigned char key, int x , int y){
    switch (key) {
        case '+':
            ctrl.rad -= 0.2;
            break;
        case '-':
            ctrl.rad += 0.2;
            break;
        case 'w':
            ctrl.y += 1;
            break;
        case 's':
            ctrl.y -= 1;
            break;
        case 'd':
            ctrl.x += 1;
            break;
        case 'a':
            ctrl.x -= 1;
            break;
        case 'z':
            ctrl.z += 1;
            break;
        case 'x':
            ctrl.z -= 1;
            break;
    }
    glutPostRedisplay();
}
void specialKeyHandler(int key_code, int x, int y){
    switch (key_code) {
        case GLUT_KEY_LEFT:
            ctrl.phi = ctrl.phi- (5*(2*M_PI)/360);
            break;
        case GLUT_KEY_RIGHT:
            ctrl.phi = ctrl.phi + (5*(2*M_PI)/360);
            break;
        case GLUT_KEY_UP:
            ctrl.theta = ctrl.theta - (5*(M_PI)/180);
            break;
        case GLUT_KEY_DOWN:
            ctrl.theta = ctrl.theta + (5*(M_PI)/180);
            break;
        default:
            break;
    }
    if (ctrl.theta > 180) {
        ctrl.theta = (179*(2*M_PI)/360);
    }
    if (ctrl.theta < 0) {
        ctrl.theta = (1*(2*M_PI)/360);
    }
    
    
    glutPostRedisplay();
}
void changeSize(int w,int h) {
    
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;
    
    // compute window's aspect ratio
    float ratio = w * 1.0 / h;
    
    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}
void renderScene(void) {
    // clear buffers
    ctrl.frame++;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    
    // set camera
    glLoadIdentity();
    gluLookAt(ctrl.rad*sin(ctrl.theta)*sin(ctrl.phi), ctrl.rad*cos(ctrl.theta), ctrl.rad*sin(ctrl.theta)*cos(ctrl.phi),
              ctrl.x, ctrl.y, ctrl.z,
              0.0f, 1.0f, 0.0f);
    glPushMatrix();
    
    drawModels(models);
    drawAxis();
    glPopMatrix();
    glutSwapBuffers();
    //cout << ctrl.x <<" "<< ctrl.y <<" "<< ctrl.z << endl;
}
void processMenuEvents(int option) {
    
    switch (option) {
        case FILL :
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case LINE :
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case BACK:
            glCullFace(GL_FRONT);
            break;
        case FRONT:
            glCullFace(GL_BACK);
            break;
    }
    
    glutPostRedisplay();
}
void createGLUTMenus() {
    
    int menu;
    
    // create the menu and
    // tell glut that "processMenuEvents" will
    // handle the events
    menu = glutCreateMenu(processMenuEvents);
    
    //add entries to our menu
    glutAddMenuEntry("LINE",LINE);
    glutAddMenuEntry("FILL",FILL);
    glutAddMenuEntry("BACK",BACK);
    glutAddMenuEntry("FRONT",FRONT);
    // attach the menu to the right button
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void loadModel(const char* fileName){
    Primitive p;
    ifstream file(fileName);
    if (file) {
        string triangle;
        while (getline(file,triangle)) {
            string sx, sy, sz;
            istringstream st_triangle(triangle);
            cout << sx << sy << sz<< endl;
            st_triangle >> sx >> sy >> sz;
            Point pt = {stof(sx),stof(sy),stof(sz)};
            p.push_back(pt);
        }
    }
    models.push_back(p);
}


int loadXML(const char* filename ){
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile(filename);
    XMLCheckResult(eResult);
    XMLNode * pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr) return XML_ERROR_FILE_READ_ERROR;
    XMLElement* node_element = pRoot->FirstChildElement();
    while(node_element != nullptr){
        const char * readfile = NULL;
        node_element->QueryStringAttribute("file", &readfile);
        loadModel(readfile);
        node_element = node_element->NextSiblingElement();
    }
    xmlDoc.Clear();
    return 0;
}
void fps(){
    ctrl.time=glutGet(GLUT_ELAPSED_TIME);
    char *fps;
    fps =(char*)malloc(100);
    if (ctrl.time - ctrl.timebase > 1000) {
        sprintf(fps,"FPS:%4.2f",
                ctrl.frame*1000.0/(ctrl.time-ctrl.timebase));
        ctrl.timebase = ctrl.time;
        ctrl.frame = 0;
        strcpy(&ctrl.name[7],fps);
        glutSetWindowTitle(ctrl.name);
        free(fps);
    }
    
    
}
int main(int argc, char **argv) {   
    loadXML(argv[1]);
    ctrl.name = (char*)malloc(100);
    strcat(ctrl.name, "Engine@");
// put GLUT init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow(ctrl.name);
// put callback registration here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutSpecialFunc(specialKeyHandler);
    glutKeyboardFunc(keyboardHandler);
    glutIdleFunc(fps);
    createGLUTMenus();
// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    
	glClearColor(255.0f,255.0f,255.0f,0.0f);
    //glClearColor(0.0f,0.0f,0.0f,0.0f);

// enter GLUT's main loop
	glutMainLoop();
	
	return 1;
}

