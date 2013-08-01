#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Pos.h"
#include "Draw.h"
#include "DEBUG.h"
#include "transform.h"
#include "Texture.h"
#include "GAME_CONSTANTS.h"
#include "GAME_SETTINGS.h"

#include <cassert>
#include <GL/glut.h>

struct player
{
    bool onGround; int running;
    bool thrustOn;
    
    PosXY position;
    PosXY size;
    
    float thrust, angle;
    float runSpeed; float runAngle;
    
    float energy; float energyDepletionRate, energyRechargeRate;
    
    PosXY speed, acceleration;
    
    static float g;
    static PosXY air_resistance;
    static float dyingSpeed;
    
    PosXY maxSpeed; TextureIF* TXR;
    ImagePWH* Ix;
    
    GAME_SETTINGS* GameData;
    
    void getSettings( GAME_SETTINGS* GD )
    {
	GameData = GD;
    }
    
    float get(std::string s)
    {
	assert( GameData != NULL );
	return GameData->DATA[s];
    }
    
    void initSettings(int i = 0)
    {
	assert(GameData != NULL);
	if(i==0)
	{
	    onGround = get("PLAYER_INIT_ONGROUND"); running = get("PLAYER_INIT_RUNNING");
	    thrustOn = get("PLAYER_INIT_THRUSTON");

	    speed = PosXY(0,0);
	    acceleration = PosXY(0, g);
	    
	    position = PosXY( get("PLAYER_INIT_POS_X") , get("PLAYER_INIT_POS_Y"));
	    // size = PosXY(0.05 , 0.05);
	    size = PosXY( get("PLAYER_SPRITE_SCALE") * 0.625, get("PLAYER_SPRITE_SCALE"));
	    energy = 1.0;
	    energyDepletionRate = get("PLAYER_ENERGY_DEPLETION_RATE");
	    energyRechargeRate =  get("PLAYER_ENERGY_RECHARGE_RATE");
	  
	    angle = 0;
	    thrust = -get("PLAYER_INIT_THRUST_MAGN")*g;
	    
	    runSpeed = get("PLAYER_INIT_RUN_SPEED"); 
	    runAngle= get("PLAYER_INIT_RUN_ANGLE");
	    
	    maxSpeed = PosXY( player::dyingSpeed*2 , player::dyingSpeed*2 );
	    
	    TXR = new TextureIF( ImagePNG( PLAYER_SPRITE_URL ) );
	
	    glClear(GL_ACCUM_BUFFER_BIT);

	}
	else if(i==1)
	{
	    onGround = true; running = false;
	    thrustOn = false;

	    speed = PosXY(0,0);
	    acceleration = PosXY(0, g);
	    
	    size = PosXY(get("PLAYER_SPRITE_SCALE") * 0.625, get("PLAYER_SPRITE_SCALE"));
	    
	}
    }
    
    player()    {
	GameData = NULL;
	// initSettings();
    }
    
    ~player()	{
	delete TXR;
	delete Ix;
    }
    void setDeath()
    {
	initSettings();
    }
    void setRun(int i)
    {
	int old_run = running;
	assert(i==-1 || i==1 || i==0);
	running = i;
	
	angle = i*runAngle*PI;
	if(angle==0 && old_run < 0)
	{
	    angle = -0.0001*PI;		// Face the other way
	}
    }
    
    void setLand(PosXY pos)
    {
	float an = angle;
	initSettings(1);
	position = pos;
	if(an < 0) angle = -0.0001*PI;	// Face the other way
	else angle = 0;
    }
    
    void setCollide(bool onLand, PosXY t)
    {
	float totalSpeed = getSpeedMagn();
	if(totalSpeed > dyingSpeed)
	{
	    DEBUG("DIE",0);
	    setDeath();
	    return;
	}
	else
	{
	    speed.X = -speed.X * 0.6;
	    speed.Y = -speed.Y * 0.6;
	    position = t; thrustOn = false;
	}   
	
	if(onLand)
	// Possible landing
	{
	    setLand(t);
	}
    }
    
    void updateFrame()
    {
	updateEnergy();
	if(onGround && !running) 
	{
	    return;
	}
	else if(running && onGround)
	{
	    assert(onGround);
	    updatePosition( running * runSpeed, 0);
	    return;
	}
	else if(running)
	{
	    running = 0;
	}
	updateAcceleration();
	updateSpeed( acceleration.X , acceleration.Y );
	updatePosition( speed.X , speed.Y );
    }
    
    void updateEnergy()
    {
	if( thrustOn && energy>-0.25 )
	{
	    energy -= energyDepletionRate;
	    DEBUG( "T", 1);
	}
	else if( onGround && running && energy<1.0 )
	{
	    energy += 3*energyRechargeRate;
	    DEBUG( "T", 2);
	}
	else if( energy<1.0 )
	{
	    energy += energyRechargeRate;
	    DEBUG( "T", 3);
	}
	else
	 DEBUG("energy", energy);
	
	if(energy < 0)
	{
	    thrustOn = false;
	}
    }
    
    void updateAcceleration()
    {
	float x1 = getThrustRight();
	float xred = speed.X*air_resistance.X;
	float y1 = getThrustUp() + g;
	float yred = speed.Y*air_resistance.Y;
	acceleration = PosXY( x1 - xred , y1 - yred);
    }
    
    void updateSpeed( float fX , float fY )
    {
	float nX = speed.X + fX;
	float nY = speed.Y + fY;
	
	/*
	if(thrustOn)
	{
	    angle += copysign(thrust,angle) * PI;
	}*/
	if(nX < maxSpeed.X)
	    speed.X = nX;
	else
	    speed.X = maxSpeed.X;
	
	if(nY < maxSpeed.Y)
	    speed.Y = nY;
	else
	    speed.Y = maxSpeed.Y;
	
    }
    
    void updatePosition( float fX, float fY )
    {
	float nX = position.X + fX;
	position.X = nX;
	
	float nY = position.Y + fY;
	position.Y = nY;
	
	/*
	if( nY >= 0.2 && nY <= 1 )
	    position.Y = nY;
	else if( !thrustOn )
	{	    
	    onGround = true; running = false;
	    thrustOn = false;

	    speed = PosXY(0,0);
	    acceleration = PosXY(0, g);
	    
	    position.Y = 0.2;
	    // position = PosXY(0.1 , 0.2);
	    // size = PosXY(0.05 , 0.05);
	    size = PosXY(0.1* 0.625, 0.1);
	    
	  
	    angle = 0;
	    //initSettings();
	}*/
    }
    
public:
    
    float inline getThrustUp()
    {
	if( !thrustOn ) return 0;
	else return thrust*cos(angle);
    }
    
    float inline getThrustRight()
    {                              
	if( !thrustOn ) return 0;
	else return thrust*sin(angle);
    }
    
	struct LOCAL
	{
	    static float inline sqr(float f)
	    { return f*f; }
	};
	
    float inline getSpeedMagn()
    {
	return sqrt( LOCAL::sqr( speed.X ) + LOCAL::sqr( speed.Y ) );
    }
    
    float inline getAccelerMagn()
    {
	return sqrt( LOCAL::sqr( acceleration.X ) + LOCAL::sqr( acceleration.Y ) );
    }
    
    void drawBlock(PosXY WindowSize)
    {
	drawSprite(); return;
	std::vector< PosXY > PL;
	
	PosXY pivot(position.X , position.Y + (size.Y/2));
	PL.push_back( transform::rotateXY( PosXY(position.X - (size.X/2) , position.Y) , pivot , angle) );
	PL.push_back( transform::rotateXY( PosXY(position.X + (size.X/2) , position.Y) , pivot , angle) );
	PL.push_back( transform::rotateXY( PosXY(position.X - (size.X/2), position.Y + size.Y), pivot , angle) );
	PL.push_back( transform::rotateXY( PosXY(position.X + (size.X/2), position.Y + size.Y), pivot , angle) );
	
	Draw::TriangleStrip( PL );
	
    }
    
    void drawSprite()
    {
	static int count = 0; count++;
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
	PosXY pivot(position.X , position.Y + (size.Y/2));
	PosXY TopLeft = transform::rotateXY( PosXY(position.X - (size.X/2), position.Y + size.Y), pivot , angle);
	PosXY BottomLeft = transform::rotateXY( PosXY(position.X - (size.X/2) , position.Y) , pivot , angle);
	PosXY TopRight = transform::rotateXY( PosXY(position.X + (size.X/2), position.Y + size.Y), pivot , angle);
	PosXY BottomRight = transform::rotateXY( PosXY(position.X + (size.X/2) , position.Y) , pivot , angle);
	
	Colors::White.get();
	TXR->enable();
	
	PosXY texBottomLeft,texBottomRight,texTopLeft,texTopRight;
	
	float blockSizeX = (float)0.15625;//0.625*(1/6); 0.15625;
	float blockSizeY = (float)1/4;
	if(onGround && running)
	{
	    if(count%400 < 100)
	    {
	    texBottomLeft = PosXY(2*blockSizeX,2*blockSizeY);
	    texBottomRight = PosXY(3*blockSizeX,2*blockSizeY);
	    texTopRight = PosXY(3*blockSizeX,1*blockSizeY);
	    texTopLeft = PosXY(2*blockSizeX,1*blockSizeY);
	    }
	    else if(count%400 < 200 || count%400 >= 300)
	    {
	    texBottomLeft = PosXY(3*blockSizeX,2*blockSizeY);
	    texBottomRight = PosXY(4*blockSizeX,2*blockSizeY);
	    texTopRight = PosXY(4*blockSizeX,1*blockSizeY);
	    texTopLeft = PosXY(3*blockSizeX,1*blockSizeY);
	    }
	    else if(count%400 < 300)
	    {
	    texBottomLeft = PosXY(4*blockSizeX,2*blockSizeY);
	    texBottomRight = PosXY(5*blockSizeX,2*blockSizeY);
	    texTopRight = PosXY(5*blockSizeX,1*blockSizeY);
	    texTopLeft = PosXY(4*blockSizeX,1*blockSizeY);
	    }
	}
	else if(thrustOn && count%100 < 50)
	{
	    float fidget = (abs(thrust) - abs(g))* 15 * (blockSizeX);
	    fidget = (fidget < (blockSizeX/15) ? blockSizeX/15 : 
			fidget > (blockSizeX/5) ? blockSizeX/5 : 
			fidget);
	    if(energy <= 0) fidget = blockSizeX/5;
	    texBottomLeft = PosXY(-fidget + (float)1*blockSizeX,(float)1*blockSizeY);
	    texBottomRight = PosXY(-fidget + (float)2*blockSizeX,(float)1*blockSizeY);
	    texTopRight = PosXY(-fidget + (float)2*blockSizeX,(float)0*blockSizeY);
	    texTopLeft = PosXY(-fidget + (float)1*blockSizeX,(float)0*blockSizeY);
	}
	else if(thrustOn)
	{
	    texBottomLeft = PosXY(0,1*blockSizeY);
	    texBottomRight = PosXY(1*blockSizeX,1*blockSizeY);
	    texTopRight = PosXY(1*blockSizeX,0);
	    texTopLeft = PosXY(0,0);
	}/*
	else if(count%200 < 100)
	{
	    float fidget = getSpeedMagn() * 5 * (blockSizeX);
	    fidget = (fidget > (blockSizeX/3) ? blockSizeX/3 : fidget);
	    texBottomLeft = PosXY(-fidget + 0,2*blockSizeY);
	    texBottomRight = PosXY(-fidget + 1*blockSizeX,2*blockSizeY);
	    texTopRight = PosXY(-fidget + 1*blockSizeX,1*blockSizeY);
	    texTopLeft = PosXY(-fidget + 0,1*blockSizeY);
	}*/
	else
	{
	    texBottomLeft = PosXY(0,2*blockSizeY);
	    texBottomRight = PosXY(1*blockSizeX,2*blockSizeY);
	    texTopRight = PosXY(1*blockSizeX,1*blockSizeY);
	    texTopLeft = PosXY(0,1*blockSizeY);
	}    
	    
	if(angle < 0)
	{
	    LOCAL::swap(texBottomLeft , texBottomRight);
	    LOCAL::swap(texTopRight , texTopLeft );
	}
	
	glBegin( GL_QUADS );
	    LOCAL::tex( texBottomLeft );
	    BottomLeft.getVertex();
	    
	    LOCAL::tex( texBottomRight );
	    BottomRight.getVertex();
	    
	    LOCAL::tex( texTopRight );
	    TopRight.getVertex();
	    
	    LOCAL::tex( texTopLeft );
	    TopLeft.getVertex();
	glEnd();
	TXR->disable();
    }
    
    void drawDir(PosXY WindowSize)
    {
	if(onGround) return;
	
	PosXY pivot(position.X , position.Y + (size.Y/2));
	
	PosXY lineStart = transform::rotateXY( PosXY(position.X , position.Y + size.Y) , pivot , angle);
	PosXY lineEnd = transform::rotateXY( PosXY(position.X , position.Y + 0.3*WindowSize.Y) , pivot , angle);
	
	glEnable( GL_LINE_STIPPLE );
	unsigned short pattern = 43690;
	glLineStipple(5 , pattern);
	
	Colors::White.get(0.7);
	glBegin( GL_LINE_STRIP );
	    lineStart.getVertex();
	    lineEnd.getVertex();
	glEnd();
	
	glDisable( GL_LINE_STIPPLE );
    }
    
    void drawTail()
    {
	if(onGround || !thrustOn) return;
	
	PosXY pivot(position.X , position.Y + (size.Y/2));
	
	PosXY drawPoint(position.X , position.Y + (size.Y/2));
	drawPoint = transform::rotateXY( drawPoint , pivot , angle );
	
	float pixel[2] = { drawPoint.X , drawPoint.Y };
	PosXY sz(0.01, 0.01);
	Colors::Yellow.get();
	glBegin(GL_TRIANGLE_STRIP);
	    glVertex2f( drawPoint.X - sz.X/2 , drawPoint.Y - sz.Y/2);
	    glVertex2f( drawPoint.X + sz.X/2 , drawPoint.Y - sz.Y/2);
	    glVertex2f( drawPoint.X - sz.X/2 , drawPoint.Y + sz.Y/2);
	    glVertex2f( drawPoint.X + sz.X/2 , drawPoint.Y + sz.Y/2);
	glEnd();
	// glDrawPixels(0.1,0.1,GL_RGBA, GL_FLOAT, pixel);
	
	glAccum(GL_ACCUM , 0.25);
	
	glDrawBuffer(GL_FRONT);
	glAccum(GL_RETURN,1.0);
	glDrawBuffer(GL_BACK);
    }
    
    void drawSprite2()
    {
	Colors::White.get();
	glRasterPos2f( position.X , position.Y );
	glBitmap(10 , 10, 0 , 0 , 0 , 0, (GLubyte*) Ix->pixels );
    }
};

#endif