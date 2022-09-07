#include "Group.hpp"
#include "Controls.hpp"
#include "XMLParser.hpp"
Controls ctrl;




void fps(){
    ctrl.frame++;
    ctrl.time=glutGet(GLUT_ELAPSED_TIME);
    char *fps;
    fps = (char*)malloc(100);
    if (ctrl.time - ctrl.timebase > 1000) {
        sprintf(fps,"Engine@FPS:%4.2f",
                ctrl.frame*1000.0/(ctrl.time-ctrl.timebase));
        ctrl.timebase = ctrl.time;
        ctrl.frame = 0;
        glutSetWindowTitle(fps);
        free(fps);
    }else{free(fps);}
    
    
}

void drawAxis(){
    glBegin(GL_LINES);
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
void keyboardHandler(unsigned char key, int x , int y){
    switch (key) {
        case '+':
            ctrl.rad -= 2;
            break;
        case '-':
            ctrl.rad += 2;
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
    if (ctrl.theta > M_PI) {
        ctrl.theta = M_PI-0.01;
    }
    if (ctrl.theta < 0) {
        ctrl.theta = 0.01;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    fps();
    // set camera
    glLoadIdentity();
    gluLookAt(ctrl.rad*sin(ctrl.theta)*sin(ctrl.phi),
              ctrl.rad*cos(ctrl.theta),
              ctrl.rad*sin(ctrl.theta)*cos(ctrl.phi),
              ctrl.x, ctrl.y, ctrl.z,
              0.0f, 1.0f, 0.0f);
    glPushMatrix();
    ctrl.scene->drawGroup();
    glColor3f(0, 0, 0);
    //drawAxis();
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



int main(int argc, char **argv) {
// put GLUT init here
    ctrl.scene = new Group();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("Engine@");
// put callback registration here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutSpecialFunc(specialKeyHandler);
    glutKeyboardFunc(keyboardHandler);
    glutIdleFunc(renderScene);
    createGLUTMenus();
#ifndef __APPLE__
    glewInit();
#endif
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    int error = loadXML(argv[1], &ctrl);
    if (!error) {
        perror("Invalid XML file");
        return 1;
    }
// OpenGL settings 
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
	glClearColor(255.0f,255.0f,255.0f,0.0f);
    //glClearColor(0.0f,0.0f,0.0f,0.0f);

// enter GLUT's main loop
	glutMainLoop();
	
	return 1;
}

