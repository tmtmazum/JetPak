#include "headers/Pos.h"
#include "headers/Draw.h"
#include "headers/DEBUG.h"
#include "headers/transform.h"
#include "headers/Mouse.h"

#include "headers/LEVEL.h"
#include "headers/fcommon.h"
#include "headers/player.h"
#include "headers/Util.h"
#include "headers/JetUtil.h"
#include "headers/WORLD.h"

#include "headers/GAME_CONSTANTS.h"
#include "headers/GAME_SETTINGS.h"

#include <GL/glut.h>

#include <cmath>
#include <vector>
#include <cassert>

using namespace std;

GAME_SETTINGS GameData;
LEVEL_TERRAIN LevelData;

PosXY WindowSize(800, 800);
MouseAB Mouse;
PosXY mouseDiff;
int chVar = 1;

PosXY CamXY(0, 0);

bitmask INPUT_STATE(0);

enum {
  KEYPRESS_A,
  KEYPRESS_D,
  KEYPRESS_SPACE
};

float player::g = -0.0005;
PosXY player::air_resistance(0.01, 0.06);
float player::dyingSpeed = 0.015;
    // Air resistance reduces the sideways acceleration to 60%
player P1;
LEVEL_BLOCK_SIMPLE L1;

int framesPerSecond = 60;
int msPerFrame = 1000*(float)1/framesPerSecond;	// 16 for 60

float screenZoom = 0.0;

TextureIF* backMain;
int DL_BACKGROUND_ID;

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

void drawBackground()
{
    struct LOCAL
    {
	static inline void tex( const PosXY& t )
	{
	    glTexCoord2f( t.X , t.Y );
	}
	static inline void swap(PosXY& t1 , PosXY& t2)
	{
	    PosXY tmp(t1);
	    t1 = t2;
	    t2 = tmp;
	}
    };
	
    float blockSizeX = (float)0.5;//0.625*(1/6); 0.15625;
    float blockSizeY = (float)0.5;
	
    PosXY texBottomLeft,texBottomRight,texTopLeft,texTopRight;
	
    texBottomLeft = PosXY(0*blockSizeX,1*blockSizeY);
    texBottomRight = PosXY(1*blockSizeX,1*blockSizeY);
    texTopRight = PosXY(1*blockSizeX,2*blockSizeY);
    texTopLeft = PosXY(0*blockSizeX,2*blockSizeY);
    
    LOCAL::swap( texBottomLeft , texTopLeft );
    LOCAL::swap( texBottomRight , texTopRight );
    
    // TextureIF backMain( ImagePNG("BMPs/backgroundSize1.png") );
    backMain->enable();
    Colors::White.get();
    glBegin( GL_QUADS );
	LOCAL::tex( texBottomLeft );
	glVertex2f(0.0, 0.0);
	LOCAL::tex( texBottomRight );
	glVertex2f(1.0, 0.0);
	LOCAL::tex( texTopRight );
	glVertex2f(1.0, 1.0);
	LOCAL::tex( texTopLeft );
	glVertex2f(0.0, 1.0);
    glEnd();   
    backMain->disable();
    
    /*
    Colors::White.get();
    glCallList( DL_BACKGROUND_ID );

    */
}

void registerBackground()
// register background as DisplayList
{ return;
    int index = glGenLists(1);
    DL_BACKGROUND_ID = index;
    struct LOCAL
    {
	static inline void tex( const PosXY& t )
	{
	    glTexCoord2f( t.X , t.Y );
	}
	static inline void swap(PosXY& t1 , PosXY& t2)
	{
	    PosXY tmp(t1);
	    t1 = t2;
	    t2 = tmp;
	}
    };
	
    float blockSizeX = (float)0.5;//0.625*(1/6); 0.15625;
    float blockSizeY = (float)0.5;
	
    PosXY texBottomLeft,texBottomRight,texTopLeft,texTopRight;
	
    texBottomLeft = PosXY(0*blockSizeX,1*blockSizeY);
    texBottomRight = PosXY(1*blockSizeX,1*blockSizeY);
    texTopRight = PosXY(1*blockSizeX,2*blockSizeY);
    texTopLeft = PosXY(0*blockSizeX,2*blockSizeY);
    
    LOCAL::swap( texBottomLeft , texTopLeft );
    LOCAL::swap( texBottomRight , texTopRight );
    
    // TextureIF* backMain =  new TextureIF( ImagePNG("BMPs/backgroundSize1.png") );
    // backMain.enable();
    // Colors::White.get();
    glNewList( index , GL_COMPILE );
    glBegin( GL_QUADS );
	LOCAL::tex( texBottomLeft );
	glVertex2f(0.0, 0.0);
	LOCAL::tex( texBottomRight );
	glVertex2f(1.0, 0.0);
	LOCAL::tex( texTopRight );
	glVertex2f(1.0, 1.0);
	LOCAL::tex( texTopLeft );
	glVertex2f(0.0, 1.0);
    glEndList();   
    // backMain.disable();
}

void updateAll()
{
    player::air_resistance = PosXY( GameData[ "WORLD_INIT_AIR_RESISTANCE_X" ] ,
				     GameData[ "WORLD_INIT_AIR_RESISTANCE_Y" ] );
}

void draw()
{ static int frameCount = 0; frameCount++;    
    static int timeElapsed=0;
    static PosXY prevPos = P1.position;
    static int JetState = 0;
    updateAll();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glViewport( 0 , 0 , WindowSize.X , WindowSize.Y );
    gluOrtho2D(0, 1.0, 0, 1.0);
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
	bool a,b,c;
	a =INPUT_STATE.get( KEYPRESS_A );
	b =INPUT_STATE.get( KEYPRESS_D ); 
	c = INPUT_STATE.get( KEYPRESS_SPACE );
	if(a)
	{
	    if(P1.onGround && !P1.running)
	    {
		P1.setRun(-1);
	    }
	}/*
	else
	{
	    if(P1.onGround && P1.running)
	    {
		P1.setRun(0);
	    }
	}*/
	if(b)
	{
	    if(P1.onGround && !P1.running)
	    {
		P1.setRun(1);
	    }
	}
	if(!a && !b)
	{
	    if(P1.onGround && P1.running)
	    {
		P1.setRun(0);
	    }
	}
	int jumpFrames = GameData["PLAYER_INIT_JUMP_FRAMES"];
	if( c )
	{
	    JetState++;
	    // if(P1.onGround)
	    // {
	    // }
	    if( JetState >= jumpFrames )
	    {
		if( GameData["PLAYER_JETPAK_ON"] == 1.0 )
		{
		    P1.thrustOn = true;
		    P1.onGround = false;
		}
	    JetState = -1;
	    }
	    glutWarpPointer( WindowSize.X / 2 , WindowSize.Y / 2 );
	    Mouse.setPosition(  WindowSize.X / 2 , WindowSize.Y / 2 );
	    Mouse.savePos();
	    glutSetCursor( GLUT_CURSOR_NONE );
	}
	else
	{
	    if( JetState>0 && (JetState<=jumpFrames||GameData["PLAYER_JETPAK_ON"]==0.0) && P1.onGround )
	    {
		int boost = ((jumpFrames+1) - JetState);
		DEBUG("Made jump. Boost is" , boost );
		P1.speed.Y += -boost*GameData["PLAYER_INIT_JUMP_SPEED"]*player::g * cos(2*P1.angle);
		P1.speed.X += -boost*GameData["PLAYER_INIT_JUMP_SPEED"]*player::g * sin(2*P1.angle);
		P1.onGround = false;
	    }
	    P1.thrustOn = false;
	    JetState = 0;
	}
	
	if(P1.onGround)
	{    
	    glutSetCursor( GLUT_CURSOR_CROSSHAIR );   
	}
	else { glutSetCursor( GLUT_CURSOR_NONE ); 
	    glutWarpPointer( WindowSize.X / 2 , WindowSize.Y / 2 );
	}
    }
    // drawWidth();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    if(GameData["BACKGROUND_ON"]) drawBackground();
    
    WO obj1;
    obj1.set(WO::POSITION , 0.5, 0.5, END_F);
    obj1.setGeometry( GO::RECT , 0.1 , 0.1 , END_F );
    obj1.set(WO::COLOR , 0.4 , 0.4 , 0.4 , END_F );
    obj1.set(WO::ALPHA , 0.5 , END_F );
    obj1.set(WO::ROTATION , GameData["TEST_ANGLE_ROTATE"] , END_F);
    draw2D( obj1 );
    
    if(!P1.onGround)
    {
	JetUtil::drawWarningSpeed(P1);
    }
    
    bool zoomOn = GameData["CAMERA_ZOOM_ON"];
    float zoomCam = 1;
    if(zoomOn)
    {
	zoomCam = screenZoom + GameData["CAMERA_INIT_ZOOM"];
	// DEBUG("screenZoom",screenZoom);
	glScalef( zoomCam, zoomCam, 1);
	GameData["CAMERA_CURRENT_WIDTH"] = zoomCam;
	GameData["CAMERA_CURRENT_HEIGHT"] = zoomCam;
    }
    bool follow = GameData["CAMERA_FOLLOW_PLAYER"];
    // PosXY zoomAdj(0,0);
    // PosXY zoomAdj = zoomOn ? PosXY(zoomCam/2,zoomCam/2): PosXY(0.5,0.5);
    PosXY translateCam( -(zoomCam/2) + P1.position.X -(1-zoomCam), -(zoomCam/2) + P1.position.Y + -(1-zoomCam)); 
    /*
    PosXY translateCam( follow ? P1.position.X * zoomCam : 0 ,
			follow ? P1.position.Y * zoomCam : 0 );
	*/		
    /*
    PosXY translateCam( (follow ? P1.position.X : 0) - GameData["CAMERA_INIT_POS_X"] - zoomAdj.X , 
			(follow ? P1.position.Y : 0) - GameData["CAMERA_INIT_POS_Y"] - zoomAdj.Y );
*/
    glTranslatef( -translateCam.X , 
		  -translateCam.Y , 0 );
    
    P1.drawSprite();
    P1.drawDir( WindowSize );
    // P1.drawTail();
    
    L1.drawLevel();
    
    // printToScreen( chVar , PosXY(0.8, 0.8) );
    // int iX = -1/(P1.speed.Y);
    // printToScreen( iX , PosXY(0.8, 0.7));
    glTranslatef( translateCam.X , 
		  translateCam.Y , 0 );
    if(zoomOn)
    {
	glScalef( 1/zoomCam, 1/zoomCam, 1);
    }
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    Colors::White.get(1.0);
    if( P1.energy < 0 ) { Colors::Red.get(1.0); JetUtil::drawProgress( -0.08 ); }
    else JetUtil::drawProgress( P1.energy );
    
    glutSwapBuffers();
    
    if(frameCount==1) { DEBUG("t", (int)-1/(P1.speed.Y)); }
}

void handleKeyPress( unsigned char key, int x, int y )
{
    switch(key)
    {
	case ' ':
	    INPUT_STATE.set( KEYPRESS_SPACE , true );
	    break;
	case 'a':
	    INPUT_STATE.set( KEYPRESS_A , true );
	    break;
	case 'd':
	    INPUT_STATE.set( KEYPRESS_D , true );/*
	    if(GLUT_ACTIVE_CTRL & glutGetModifiers())
		P1.angle = copysign(P1.angle, 1);
	    else if(P1.onGround)
		P1.setRun(1);*/
	    break;
    }
}
void handleKeyUp( unsigned char key , int x , int y )
{
    switch(key)
    {
	case ' ':
	    INPUT_STATE.set( KEYPRESS_SPACE , false );
	    break;
	case 'a':
	    INPUT_STATE.set( KEYPRESS_A , false );/*
	    if(P1.onGround)
		P1.setRun(0);*/
	    break;
	case 'd':
	    INPUT_STATE.set( KEYPRESS_D , false ); /*
	    if(P1.onGround)
		P1.setRun(0);*/
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
	case GLUT_KEY_F2:
	    GameData.loadFromFile( "game_const.scm" );
	    P1.initSettings();
	    break;
    }
}
void handleMousePress( int button, int state, int x, int y )
{
    switch(button)
    {
	case 3:
	    screenZoom += 0.01;
	    break;
	case 4:
	    screenZoom -= 0.01;
	    break;
    }
}

void handleMousePassiveMotion( int x, int y )
{
    if( P1.onGround ) return;
    Mouse.setPosition( x , y );
    
    mouseDiff = Mouse.getDiffEither( Mouse.At , Mouse.loadPos() , WindowSize );
    
    if(mouseDiff.X!=0)
    {
	P1.angle += mouseDiff.X * PI * GameData["PLAYER_CHANGE_ANGLE_SENSITIVITY"];
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
    // GameData.loadFromDefault();
    GameData.loadFromFile("game_const.scm");
    GameData.printAll();
    
    LevelData.loadFromFile("level1.scm");
    
    // Background Color
    glClearColor( 0.7f, 0.7f, 0.7f , 0.5f );
    
    // Enable Transparency
    glEnable (GL_BLEND); 
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glutWarpPointer( WindowSize.X / 2 , WindowSize.Y / 2 );
    Mouse.setPosition(  WindowSize.X / 2 , WindowSize.Y / 2 );
    Mouse.savePos();
    
    gluOrtho2D(0, 1, 0, 1);
    
    // CONST::BACKGROUND_1_URL = BACKGROUND_1_URL;
    // registerBackground();
   backMain = new TextureIF( ImagePNG( BACKGROUND_1_URL ) );
    /*
   L1.addRect( PosXY(0.1, 0.1), PosXY(0.8, 0.1) );
   L1.addRect( PosXY(0.6, 0.3), PosXY(0.4, 0.3) );
    */
    
   L1.addRect( PosXY(0.1,0.1), PosXY(0.4, -10.0) );
   
   L1.addRect( PosXY(0.0, 1.0), PosXY(0.1, -11.0) );
   // L1.addRect( PosXY(0.1, 0), PosXY(0.4, 0.1) );
   L1.addRect( PosXY(0.6, 0.2), PosXY(0.4, -9.8) );
   
   L1.addRect( PosXY(1.0, 0), PosXY(0.4, 0.4) );
   L1.addRect( PosXY(1.6, 0), PosXY(0.4, 0.3) );
   
   L1.addRect( PosXY(2.0, 0), PosXY(0.2, 0.3) );
   L1.addRect( PosXY(2.8, 0), PosXY(0.2, 0.3) );
   
   L1.addRect( PosXY(3.0, 0), PosXY(1.0, 0.3) );
   L1.addRect( PosXY(3.1, 0.3), PosXY(0.9, 0.2) );
   L1.addRect( PosXY(3.2, 0.5), PosXY(0.7, 0.2) );
   L1.addRect( PosXY(3.3, 0.7), PosXY(0.5, 0.2) );
   L1.addRect( PosXY(3.4, 0.9), PosXY(0.3, 0.2) );
   /*
   L1.add( dynamic_cast<LAND_OBJECT*>(new LO_FLAT_FLOOR(0.1,0.5,0.1)) , 
	    dynamic_cast<GEOMETRY_OBJECT*>(new GO_RECT(0.1,0,0.4,0.1)) );
    
    L1.add( dynamic_cast<LAND_OBJECT*>(new LO_FLAT_FLOOR(0.6,1.0,0.2)) , 
	    dynamic_cast<GEOMETRY_OBJECT*>(new GO_RECT(0.6,0.0,0.4,0.2)) );
    
    L1.addRect( PosXY(1.0, 0) , PosXY(0.4, 0.4) );
    /*
    L1.add( dynamic_cast<LAND_OBJECT*>(new LO_FLAT_FLOOR(1.0,1.4,0.4)) , 
	    dynamic_cast<GEOMETRY_OBJECT*>(new GO_RECT(1.0,0,0.4,0.4)) );
    
    L1.add( dynamic_cast<LAND_OBJECT*>(new LO_FLAT_FLOOR(1.6,2.0,0.3)) , 
	    dynamic_cast<GEOMETRY_OBJECT*>(new GO_RECT(1.6,0,0.4,0.3)) );
    */
    P1.getSettings(&GameData);
    P1.initSettings();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH );
 
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
    // glutMouseWheelFunc(handleMouseWheel);
    
    glutMainLoop();
    delete backMain;
}