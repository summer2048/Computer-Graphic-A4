#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

#include <vector>
#include <iostream>
#include "object.h"

#define Cube 0
#define Sphere 1
#define Cone 2
#define Tetrahedron 3
#define Octahedron 4

using namespace std;

vector<Object> models;

//Globals
float camPos[] = {10, 10, 10}; //where the camera is
float camTarget[] = {0, 0, 0};
float origin[] = {0,0,0};

double *m_start = new double[3];
double *m_end = new double[3];

double *Rd = new double[3];

// The index of picked object, -1 if none is picked.
int picked_object = -1;

void addModel()
{
    Object newModel(origin[0], origin[1], origin[2]);
    models.push_back(newModel);
    picked_object = models.size()-1;
}

void deleteModel(int i)
{
    models[i] = models[models.size()-1];
    models.pop_back();
}

void clear(){
    while (models.size() > 0){
        deleteModel(0);
    }
}

void move(int axis, int direction){
    if (picked_object == -1) return;
    models[picked_object].position[axis] += direction * 5;
}

//OpenGL functions
//keyboard stuff
void keyboard(unsigned char key, int xIn, int yIn)
{
    int mod = glutGetModifiers();
    switch (key)
    {
    case 'q':
    case 27: //27 is the esc key
        exit(0);
        break;
    case 'a': //Add to origin
        addModel();
        break;
    case 'r':
        clear();
        break;

    case 't':
        cout << "Models has" << models.size() << "Objects" << endl;
        if (picked_object != -1)
            cout << "Selected objects is of index " << picked_object << " of object " << models[picked_object].type << endl;
        break;


    case 'j':
        move(0,-1);
        break;
    case 'k':
        move(1,-1);
        break;
    case 'l':
        move(0,1);
        break;
    case 'i':
        move(1,1);
        break;
    case 'u':
        move(2,-1);
        break;
    case 'o':
        move(2,1);
        break;
    }
}

void special(int key, int xIn, int yIn)
{
    switch (key)
    {
    case GLUT_KEY_DOWN:
        camPos[1] -= 0.2f;
        camTarget[1] -= 0.2f;
        break;
    case GLUT_KEY_UP:
        camPos[1] += 0.2f;
        camTarget[1] += 0.2f;
        break;
    case GLUT_KEY_LEFT:
        camPos[0] -= 0.2f;
        camTarget[0] -= 0.2f;
        break;
    case GLUT_KEY_RIGHT:
        camPos[0] += 0.2f;
        camTarget[0] += 0.2f;
        break;
    }
}

void sphereRayPick(Object &object)
{
    double A = 1; // Rd is unit vector, so Rd * Rd = 1.
    double *R0Pc = new double[3];
    R0Pc[0] = m_start[0] - object.position[0];
    R0Pc[1] = m_start[1] - object.position[1];
    R0Pc[2] = m_start[2] - object.position[2];

    double B = 2 * (R0Pc[0] * Rd[0] + R0Pc[1] * Rd[1] + R0Pc[2] * Rd[2]);
    double C = (R0Pc[0] * R0Pc[0] + R0Pc[1] * R0Pc[1] + R0Pc[2] * R0Pc[2]) - pow(object.size, 2);

    double discriminant = B * B - 4 * A * C;
    if (discriminant < 0)
        return;
    double t0 = (-B + sqrt(discriminant)) / (2 * A);
    double t1 = (-B - sqrt(discriminant)) / (2 * A);
    if (t0 < t1)
        object.distToMouseRay = t0;
    else
        object.distToMouseRay = t1;
}

// plane = the plane which is constant e.g. x = 1, then plane = x.
// 0,1,2 represent x,y,z
void intersectDownLeft(Object &object, int plane){
    float planeConst,d;
    planeConst = object.DownLeft[plane];
    if (Rd[plane] == 0) return;
    d = (planeConst - m_start[plane])/Rd[plane];
    if (d < 0) return;
    if (m_start[(plane+4)%3]+d*Rd[(plane+4)%3] < object.DownLeft[(plane+4)%3] || m_start[(plane+4)%3]+d*Rd[(plane+4)%3] > object.UpperRight[(plane+4)%3]) return;
    if (m_start[(plane+5)%3]+d*Rd[(plane+5)%3] < object.DownLeft[(plane+5)%3] || m_start[(plane+5)%3]+d*Rd[(plane+5)%3] > object.UpperRight[(plane+5)%3]) return;
    if (object.distToMouseRay < 0 || d < object.distToMouseRay) {
        object.distToMouseRay = d;
        return;
    }
}

void intersectUpperRight(Object &object, int plane){
    float planeConst,d;
    planeConst = object.UpperRight[plane];
    if (Rd[plane] == 0) return;
    d = (planeConst - m_start[plane])/Rd[plane];
    if (d < 0) return;
    if (m_start[(plane+4)%3]+d*Rd[(plane+4)%3] < object.DownLeft[(plane+4)%3] || m_start[(plane+4)%3]+d*Rd[(plane+4)%3] > object.UpperRight[(plane+4)%3]) return;
    if (m_start[(plane+5)%3]+d*Rd[(plane+5)%3] < object.DownLeft[(plane+5)%3] || m_start[(plane+5)%3]+d*Rd[(plane+5)%3] > object.UpperRight[(plane+5)%3]) return;
    if (object.distToMouseRay < 0 || d < object.distToMouseRay) {
        object.distToMouseRay = d;
        return;
    }
}


void boxRayPick(Object &object)
{
    intersectDownLeft(object, 0);
    intersectDownLeft(object, 1);
    intersectDownLeft(object, 2);
    intersectUpperRight(object, 0);
    intersectUpperRight(object, 1);
    intersectUpperRight(object, 2);
}

void rayPick()
{
    // Set disToMouseRay for exist objects
    for (int i = 0; i < models.size(); i++)
    {
        if (models[i].type == Sphere) {
            sphereRayPick(models[i]);
        } else {
            boxRayPick(models[i]);
        }
    }

    // Find the closest object to select
    int idx = -1;
    float distance = 10000;
    for (int i = 0; i < models.size(); i++)
    {
        if (models[i].distToMouseRay > 0 && models[i].distToMouseRay < distance)
        {
            idx = i;
            distance = models[i].distToMouseRay;
        }
    }

    picked_object = idx;
}

void mouse(int btn, int state, int x, int y)
{
    // Clear previous select data
    if (picked_object != -1){
        models[picked_object].distToMouseRay = -1;
    }

    // New Raypick
    if (state == GLUT_DOWN)
    {
        double matModelView[16], matProjection[16];
        int viewport[4];

        glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
        glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        double winX = (double)x;
        double winY = viewport[3] - (double)y;

        gluUnProject(winX, winY, 0.0, matModelView, matProjection,
                     viewport, &m_start[0], &m_start[1], &m_start[2]);

        gluUnProject(winX, winY, 1.0, matModelView, matProjection,
                     viewport, &m_end[0], &m_end[1], &m_end[2]);

        // Set Rd vector
        double xDiff = m_end[0] - m_start[0];
        double yDiff = m_end[1] - m_start[1];
        double zDiff = m_end[2] - m_start[2];

        double mag = sqrt(pow(xDiff, 2) + pow(yDiff, 2) + pow(zDiff, 2));

        Rd[0] = xDiff / mag;
        Rd[1] = yDiff / mag;
        Rd[2] = zDiff / mag;


        rayPick();

        if (btn == GLUT_RIGHT_BUTTON && picked_object != -1){
            deleteModel(picked_object);
        }
    }
}

//initialization
void init(void)
{
    glClearColor(0, 0, 0, 0);
    glColor3f(1, 1, 1);
    glEnable(GL_DOUBLEBUFFER);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 100);

    //initialize the ray values
    m_start[0] = 0;
    m_start[1] = 0;
    m_start[2] = 0;

    m_end[0] = 0;
    m_end[1] = 0;
    m_end[2] = 0;
}

void draw(Object &object)
{
    glPushMatrix();
    glTranslatef(object.position[0], object.position[1], object.position[2]);
    glRotatef(object.orientation[0],1,0,0);
    glRotatef(object.orientation[1],0,1,0);
    glRotatef(object.orientation[2],0,0,1);
    glFrontFace(GL_CW);
	switch (object.type)
	{
	case Cube:
	    glutSolidCube(1);
        break;
	case Sphere:
		glutSolidSphere(1,16,16);
        break;
	case Cone:
		glutSolidCone(1,2,16,16);
        break;
	case Tetrahedron:
		glutSolidTetrahedron();
        break;
	case Octahedron:
		glutSolidOctahedron();
        break;
	default:
		break;
	}
    glPopMatrix();
}


/* display function - GLUT display callback function
 *		clears the screen, sets the camera position
 */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], 0, 1, 0);

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex3f(m_start[0], m_start[1], m_start[2]);
    glVertex3f(m_end[0], m_end[1], m_end[2]);
    glEnd();

    glColor3f(0,1,1);
    for (int i = 0; i < models.size(); i++){
        draw(models[i]);
    }
    
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, (float)((w + 0.0f) / h), 1, 100);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void FPSTimer(int value)
{ //60fps
    glutPostRedisplay();
    glutTimerFunc(17, FPSTimer, 0);
}

/* main function - program entry point */
int main(int argc, char **argv)
{
    glutInit(&argc, argv); //starts up GLUT
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(400, 400);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("3GC3 Code for Ray Picking"); //creates the window

    //display callback
    glutDisplayFunc(display);

    //keyboard callback
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    //mouse callbacks
    glutMouseFunc(mouse);

    //resize callback
    glutReshapeFunc(reshape);

    //fps timer callback
    glutTimerFunc(17, FPSTimer, 0);

    init();

    glutMainLoop(); //starts the event glutMainLoop
    return (0);     //return may not be necessary on all compilers
}
