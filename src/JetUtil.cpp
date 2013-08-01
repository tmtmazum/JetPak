#include "../headers/JetUtil.h"

#include "../headers/Pos.h"
#include "../headers/player.h"

#include <GL/glut.h>

void JetUtil::drawWarningSpeed(player& P1)
{    
    if(P1.getSpeedMagn() > player::dyingSpeed*0.6 || P1.getAccelerMagn() > (4*abs(player::g)))
    {
	if(P1.getSpeedMagn() > player::dyingSpeed)
	    Colors::Red.get();
	else
	    Colors::Yellow.get();
	glBegin(GL_QUADS);
	    glVertex2f( 0.9, 0.9 );
	    glVertex2f( 0.9, 0.92 );
	    glVertex2f( 0.92, 0.92 );
	    glVertex2f( 0.92, 0.9 );
	glEnd();
    }
}

void JetUtil::drawSpeed(player& P1)
{
    float len = P1.getSpeedMagn() / player::dyingSpeed;
    
    ColorRGBA initC;
    ColorRGBA finalC;
    if(P1.speed.Y < 0) 
    {
	len = -len;
	initC = Colors::White;
    }
    else
    {
	initC = Colors::Blue;
	initC.R += 0.6;
	initC.G += 0.6;
	initC.B += 0.6;
    }
    finalC = initC;
    finalC.R -= 0.4;
    finalC.G -= 0.4;
    finalC.B -= 0.4;
    
    PosXY wholeRect(0.05 , 0.01);
    PosXY place(0.85, 0.7);
    
	//	glShadeModel(GL_FLAT);
    glBegin(GL_QUADS);
	finalC.get();
	// Colors::Black.get();
	// glColor3f(0.0, 0.0, 0.0);
	glVertex2f( place.X , place.Y );
	glVertex2f( place.X + (wholeRect.X*len) , place.Y);
	initC.get();
	// glColor3f(1.0, 1.0, 1.0);
	glVertex2f( place.X + (wholeRect.X*len) , place.Y + wholeRect.Y );
	glVertex2f( place.X  , place.Y + wholeRect.Y );
    glEnd();
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
    Colors::Black.get(0.2);
    glBegin(GL_QUADS);
	glVertex2f( place.X + wholeRect.X , place.Y );
	glVertex2f( place.X + wholeRect.X , place.Y + wholeRect.Y);
	glVertex2f( place.X - wholeRect.X , place.Y + wholeRect.Y);
	glVertex2f( place.X - wholeRect.X , place.Y );
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
}

void JetUtil::drawHeight(player& P1)
{
    float h = 0.9; float w = 0.02;
    float len = P1.position.Y / 5;
    if(len > 1) len = 1.02;
    PosXY t(0.95 , 0.5);
    
    Colors::Black.get(0.5);
    
    glBegin(GL_LINES);
	glVertex2f( t.X , t.Y - h/2 );
	glVertex2f( t.X , t.Y + h/2 );
	glVertex2f( t.X - w , t.Y + h/2 );
	glVertex2f( t.X + w , t.Y + h/2 );
	glVertex2f( t.X - w , t.Y - h/2 );
	glVertex2f( t.X + w , t.Y - h/2 );
    glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
    glBegin( GL_TRIANGLE_STRIP );
	glVertex2f( t.X , t.Y + len*(h/2));
	glVertex2f( t.X - w, t.Y + len*(h/2) + w/2);
	glVertex2f( t.X - w, t.Y + len*(h/2) - w/2);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
}

void JetUtil::drawWidth(player& P1)
{
    float h = 0.9; float w = 0.02;
    float len = P1.position.X / 5;
    if(len > 1) len = 1.02;
    PosXY t(0.5 , 0.05);
    
    Colors::Black.get(0.5);
    
    // glRotatef(90 , 0 , 0 , 1);
    glBegin(GL_LINES);
	glVertex2f( t.X - h/2 , t.Y );
	glVertex2f( t.X + h/2, t.Y );
	glVertex2f( t.X + h/2 , t.Y - w );
	glVertex2f( t.X + h/2 , t.Y + w );
	glVertex2f( t.X - h/2, t.Y - w );
	glVertex2f( t.X - h/2, t.Y + w );
    glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
    glBegin( GL_TRIANGLE_STRIP );
	glVertex2f( t.X + len*(h/2) , t.Y);
	glVertex2f( t.X+ len*(h/2)+w/2 , t.Y + w);
	glVertex2f( t.X+ len*(h/2)- w/2 , t.Y + w);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
    // glRotatef(-90 , 0 , 0 , 1);
}

void JetUtil::drawProgress( float f )
{
    float xBegin = 0.75;
    float xLength = 0.2;
    float xEnd = xBegin + xLength;
    
    float yBegin = 0.8;
    float yHeight = 0.02;
    
    PosXY X2( xBegin , yBegin );
    PosXY Y2( xBegin + xLength*f, yBegin );
    PosXY X1( xBegin , yBegin - yHeight );
    PosXY Y1( xBegin + xLength*f, yBegin - yHeight );
    glBegin( GL_TRIANGLE_STRIP );
	X2.getVertex();
	Y2.getVertex();
	X1.getVertex();
	Y1.getVertex();
    glEnd();
    
    Y2.X = xEnd;
    Y1.X = xEnd;
    Colors::Black.get();
    glBegin( GL_LINES );
	X1.getVertex();
	X2.getVertex();
	Y2.getVertex();
	Y1.getVertex();
    glEnd();
}