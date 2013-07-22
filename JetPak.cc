#include "headers/Pos.h"
#include "headers/Draw.h"
#include "headers/DEBUG.h"
#include "headers/transform.h"
#include "headers/Mouse.h"

#include "headers/LEVEL.h"
#include "headers/fcommon.h"
#include "headers/player.h"
#include "headers/JetUtil.h"

#include <GL/glut.h>

#include <cmath>
#include <vector>
#include <cassert>

using namespace std;

PosXY WindowSize(800, 800);
MouseAB Mouse;
PosXY mouseDiff;
int chVar = 1;

PosXY CamXY(0, 0);

float player::g = -0.0005;
float player::air_resistance = 0.6;
float player::dyingSpeed = 0.015;
    // Air resistance reduces the sideways acceleration to 60%
player P1;
LEVEL_BLOCK_SIMPLE L1;

int framesPerSecond = 60;
int msPerFrame = 1000*(float)1/framesPerSecond;	// 16 for 60


void testDraw()
{
    vector<PosXY> GR1;
    GR1.push_back( PosXY(0.1,0) );
    GR1.push_back( PosXY(0.1,0.2) );
    GR1.push_back( PosXY(0.9,0) );
    GR1.push_back( PosXY(0.9,0.2) );
    
    vector<PosXY> test1;
    test1.push_back( PosXY(0.9, 0.3) );
    test1.push_back( PosXY(0.9, 0.5) );
    test1.push_back( PosXY(1.0, 0.3) );
    test1.push_back( PosXY(1.0, 0.5) );
    
    Colors::Yellow.get();
    Draw::TriangleStrip( GR1 );
    
    Colors::Red.get();
    
    if(P1.position.X == RangeX(0.9, 1.0))
	Colors::Blue.get();
    else
	Colors::Black.get();
    Draw::TriangleStrip( test1 );
}

void draw()
{ static int frameCount = 0; frameCount++;    
    static int timeElapsed=0;
    static PosXY prevPos = P1.position;
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glViewport( 0 , 0 , WindowSize.X , WindowSize.Y );
    gluOrtho2D(0, 1, 0, 1);
    // glViewport( P1.position.X , P1.position.Y , WindowSize.X , WindowSize.Y );
    
    // glOrtho(0, 1, 0, 1, 0, 1);
    //  glViewport( CamXY.X , CamXY.Y , WindowSize.X, WindowSize.Y );   

    glClear( GL_COLOR_BUFFER_BIT );
        
    if(glutGet(GLUT_ELAPSED_TIME)-timeElapsed >= msPerFrame)
    {
	P1.updateFrame();
	timeElapsed = glutGet( GLUT_ELAPSED_TIME );
	
	L1.checkCollision( P1 , prevPos );
	
	prevPos = P1.position;
	/*
	if(P1.position.Y > 0-P1.size.Y && P1.position.Y < 0.2 && P1.position.X > 0.1-(P1.size.X/2) && P1.position.X < 0.9+(P1.size.X/2))
	{
	    P1.setCollide( prevPos.Y > 0.2 , prevPos );
	}
	*/
    }
    
    if(!P1.onGround)
    {
	JetUtil::drawWarningSpeed(P1);
	/*
	drawSpeed();
	if(P1.position.Y > 1 || P1.position.Y < -1)   
	    drawHeight();
	*/
    }
    // drawWidth();
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( -(P1.position.X - 0.4) , -(P1.position.Y - 0.4) , 0 );

    
    P1.drawSprite();
    P1.drawDir( WindowSize );
    // P1.drawTail();
    
    L1.drawLevel();
    
    // printToScreen( chVar , PosXY(0.8, 0.8) );
    // int iX = -1/(P1.speed.Y);
    // printToScreen( iX , PosXY(0.8, 0.7));
    
    
    glTranslatef( P1.position.X - 0.4 , P1.position.Y - 0.4 , 0 );

    
    glutSwapBuffers();
    
    if(frameCount==1) { DEBUG("t", (int)-1/(P1.speed.Y)); }
}

void handleKeyPress( unsigned char key, int x, int y )
{
    switch(key)
    {
	case ' ':
	    P1.thrustOn = true;
	    P1.onGround = false;    
	    glutWarpPointer( WindowSize.X / 2 , WindowSize.Y / 2 );
	    Mouse.setPosition(  WindowSize.X / 2 , WindowSize.Y / 2 );
	    Mouse.savePos();
	    glutSetCursor( GLUT_CURSOR_NONE );
	    break;
	case 'a':
	    if(GLUT_ACTIVE_CTRL & glutGetModifiers())
	    {
		P1.angle = copysign(P1.angle, -1);
	    }
	    else if(P1.onGround)
	    {
		P1.setRun(-1);
	    }
	    break;
	case 'd':
	    if(GLUT_ACTIVE_CTRL & glutGetModifiers())
		P1.angle = copysign(P1.angle, 1);
	    else if(P1.onGround)
		P1.setRun(1);
	    break;
    }
}
void handleKeyUp( unsigned char key , int x , int y )
{
    switch(key)
    {
	case ' ':
	    P1.thrustOn = false;
	    glutSetCursor( GLUT_CURSOR_CROSSHAIR );
	    break;
	case 'a':
	    if(P1.onGround)
		P1.setRun(0);
	    break;
	case 'd':
	    if(P1.onGround)
		P1.setRun(0);
	    break;
    }
}
void handleSpecialPress( int key, int x, int y )
{
    switch(key)
    {
	case GLUT_KEY_UP:
	    chVar += 1;
	    break;
	case GLUT_KEY_DOWN:
	    chVar -= 1;
	    break;
	case GLUT_KEY_LEFT:
	    if(!P1.onGround)
		P1.angle -= 0.07 * PI;
	    break;
	case GLUT_KEY_RIGHT:
	    if(!P1.onGround)
		P1.angle += 0.07 * PI;
	    break;
	case GLUT_KEY_F1:
	    P1.initSettings();
	    break;
    }
}
void handleMousePress( int button, int state, int x, int y )
{
}

void handleMousePassiveMotion( int x, int y )
{
    if( P1.onGround ) return;
    Mouse.setPosition( x , y );
    
    mouseDiff = Mouse.getDiffEither( Mouse.At , Mouse.loadPos() , WindowSize );
    
    if(mouseDiff.X!=0)
    {
	P1.angle += mouseDiff.X * PI;
    }
    if(mouseDiff.Y!=0)
    {
	//P1.angle += mouseDiff.Y * PI/2;
    }
    
    Mouse.savePos();
}

void handleScreenResize(int width, int height)
{
    WindowSize.X = width;
    WindowSize.Y = height;
    
    glViewport( CamXY.X , CamXY.Y , WindowSize.X, WindowSize.Y );   
}

int initGame()
{
    // Background Color
    glClearColor( 0.7f, 0.7f, 0.7f , 0.5f );
    
    // Enable Transparency
    glEnable (GL_BLEND); 
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glutWarpPointer( WindowSize.X / 2 , WindowSize.Y / 2 );
    Mouse.setPosition(  WindowSize.X / 2 , WindowSize.Y / 2 );
    Mouse.savePos();
    
    gluOrtho2D(0, 1, 0, 1);
   
    /*
   L1.addRect( PosXY(0.1, 0.1), PosXY(0.8, 0.1) );
   L1.addRect( PosXY(0.6, 0.3), PosXY(0.4, 0.3) );
    */
    
    L1.add( dynamic_cast<LAND_OBJECT*>(new LO_FLAT_FLOOR(0.1,0.5,0.1)) , 
	    dynamic_cast<GEOMETRY_OBJECT*>(new GO_RECT(0.1,0,0.4,0.1)) );
    
    L1.add( dynamic_cast<LAND_OBJECT*>(new LO_FLAT_FLOOR(0.6,1.0,0.2)) , 
	    dynamic_cast<GEOMETRY_OBJECT*>(new GO_RECT(0.6,0.0,0.4,0.2)) );
    
    L1.addRect( PosXY(1.0, 0) , PosXY(0.4, 0.4) );
    /*
    L1.add( dynamic_cast<LAND_OBJECT*>(new LO_FLAT_FLOOR(1.0,1.4,0.4)) , 
	    dynamic_cast<GEOMETRY_OBJECT*>(new GO_RECT(1.0,0,0.4,0.4)) );
    */
    L1.add( dynamic_cast<LAND_OBJECT*>(new LO_FLAT_FLOOR(1.6,2.0,0.3)) , 
	    dynamic_cast<GEOMETRY_OBJECT*>(new GO_RECT(1.6,0,0.4,0.3)) );
    
    P1.initSettings();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGBA );
 
    glutInitWindowSize(WindowSize.X, WindowSize.Y);
    glutInitWindowPosition(10,10);
    glutCreateWindow("JetPak");
    
    initGame();
    
    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    
    glutKeyboardFunc(handleKeyPress);
    glutKeyboardUpFunc(handleKeyUp);
    glutSpecialFunc(handleSpecialPress);
    glutMouseFunc(handleMousePress);
    glutPassiveMotionFunc(handleMousePassiveMotion);
    glutReshapeFunc(handleScreenResize);
    
    glutMainLoop();
}