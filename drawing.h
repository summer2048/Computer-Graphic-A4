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

static void
drawBox(GLfloat size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(type);
    glNormal3fv(&n[i][0]);
    glTexCoord2f(1,1);
    glVertex3fv(&v[faces[i][0]][0]);
    glTexCoord2f(0,1);
    glVertex3fv(&v[faces[i][1]][0]);
    glTexCoord2f(1,0);
    glVertex3fv(&v[faces[i][2]][0]);
    glTexCoord2f(0,0);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

void APIENTRY
glutSolidCube(GLdouble size)
{
  drawBox(size, GL_QUADS);
}

void APIENTRY
glutSolidTetrahedron1()
{
    glBegin(GL_TRIANGLES);
       // front
       glTexCoord2d(0.5, 0.0);
       glVertex3d( 0.0, 2.0,  0.0);
       glTexCoord2d(0.0, 0.0); 
       glVertex3d(-1.0, 0.0,  1.0);
       glTexCoord2d(0.0, 1.0); 
       glVertex3d( 1.0, 0.0,  1.0);
       // right
       glTexCoord2d(0.5, 0.0); 
       glVertex3d( 0.0, 2.0,  0.0);
       glTexCoord2d(0.0, 1.0); 
       glVertex3d( 1.0, 0.0,  1.0);
       glTexCoord2d(0.5, 1.0); 
       glVertex3d( 0.0, 0.0, -1.0);
       // left
       glTexCoord2d(0.5, 0.0);
       glVertex3d( 0.0, 2.0,  0.0);
       glTexCoord2d(0.5, 1.0);
       glVertex3d( 0.0, 0.0, -1.0);
       glTexCoord2d(1.0, 0.0);
       glVertex3d(-1.0, 0.0,  1.0);
       // bottom
       glTexCoord2d(1.0, 1.0);
       glVertex3d( 1.0, 0.0,  1.0);
       glTexCoord2d(1.0, 0.0);
       glVertex3d(-1.0, 0.0,  1.0);
       glTexCoord2d(0.5, 1.0);
       glVertex3d( 0.0, 0.0, -1.0);
  glEnd();
}
