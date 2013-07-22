#include "../headers/Color.h"
#include <GL/glut.h>

void ColorRGBA::get()
{
	glColor4f( R, G, B, A );
}

void ColorRGBA::get(float al)
{
	glColor4f( R, G, B, al );
}