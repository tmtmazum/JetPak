#include "headers/Pos.h"
#include "headers/Draw.h"
#include "headers/DEBUG.h"
#include "headers/transform.h"
#include "headers/Mouse.h"

#include "headers/fcommon.h"
#include "headers/player.h"

#include <GL/glut.h>

#include <cmath>
#include <vector>

using namespace std;

PosXY WindowSize(800, 600);

PosXY GreenPos(30,30);

void draw()
{    
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WindowSize.X , 0 , WindowSize.Y );
    
    glClear( GL_COLOR_BUFFER_BIT );
    
    Colors::Red.get();
    glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(0.5*WindowSize.X, 0);
	glVertex2f(0.5*WindowSize.X, 0.5*WindowSize.Y);
	glVertex2f(0, 0.5*WindowSize.Y);
    glEnd();
    
    Colors::Blue.get();
    glBegin(GL_QUADS);
	glVertex2f(0.5*WindowSize.X, 0);
	glVertex2f(1.0*WindowSize.X, 0);
	glVertex2f(1.0*WindowSize.X, 0.5*WindowSize.Y);
	glVertex2f(0.5*WindowSize.X, 0.5*WindowSize.Y);
    glEnd();
    
    
    DEBUG("G1", 20);
    Colors::Green.get();
    glBegin(GL_QUADS);
	glVertex2f(GreenPos.X,GreenPos.Y);
	glVertex2f(GreenPos.X+20,GreenPos.Y);
	glVertex2f(GreenPos.X+20,GreenPos.Y+20);
	glVertex2f(GreenPos.X,GreenPos.Y+20);
    glEnd();
    
    Colors::Yellow.get();
    glBegin(GL_QUADS);
	glVertex2f(GreenPos.X + 30, GreenPos.Y);
	glVertex2f(GreenPos.X+20 + 30, GreenPos.Y);
	glVertex2f(GreenPos.X+20 + 30, GreenPos.Y + WindowSize.Y/30);
	glVertex2f(GreenPos.X + 30, GreenPos.Y + WindowSize.Y/30);
    glEnd();
    DEBUG("G2", WindowSize.Y/30);
    
    glutSwapBuffers();
}

void handleKeyPress(unsigned char key, int x, int y)
{
  
}

void handleSpecialPress(int key, int x, int y)
{
  switch(key)
  {
      case GLUT_KEY_UP:
	  GreenPos.Y += 10;
	  break;
      case GLUT_KEY_DOWN:
	  GreenPos.Y -= 10;
	  break;
  }
}

void handleScreenResize(int width, int height)
{
    WindowSize.X = width;
    WindowSize.Y = height;
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WindowSize.X,WindowSize.Y);
  glutInitWindowPosition(10,10);
  glutCreateWindow("glTest");

  glutDisplayFunc(draw);
  /* where draw(): lambda<void> */
  
  glutKeyboardFunc(handleKeyPress);
  /* where handleKeyPress(
   *		key: unsigned-char, 
   * 		x:int, 
   * 		y:int): lambda<void> */
  
  glutSpecialFunc(handleSpecialPress);
  /* where handleSpecialPress(
   *		key: int, 
   * 		x:int, 
   * 		y:int): lambda<void> */
  
  glutIdleFunc(draw);
  /* where draw(): lambda */
  
  glutMainLoop();

  return 0;
}