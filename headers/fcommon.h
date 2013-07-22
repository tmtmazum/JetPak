#ifndef __FCOMMON_H__
#define __FCOMMON_H__

#include "Pos.h"

#include <string>

#include <GL/glut.h>

void printToScreen( int i , PosXY p )
{
    glRasterPos2f(p.X , p.Y);
    if(i < 10 && i >= 0)
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, i + '0');	
    else if(i >= 10)
    {
	do
	{
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '0' + i%10);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ' ');
	i = i/10;
	} while(i > 0);
    }
}

void printToScreen( char c , PosXY p )
{
    glRasterPos2f(p.X , p.Y);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
}

void printText(	std::string text, PosXY p )
{
    glRasterPos2f(p.X , p.Y);
    for(std::string::iterator i = text.begin(); i != text.end(); ++i)
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *i);
}


#endif