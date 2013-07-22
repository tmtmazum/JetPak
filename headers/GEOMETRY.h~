#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Texture.h"
#include "Pos.h"

struct GEOMETRY_OBJECT
{
    virtual ~GEOMETRY_OBJECT() {}
};

struct GO_RECT : public GEOMETRY_OBJECT
{
    PosXY position, size;
    ColorRGBA C; TextureIF* T;
    
    GO_RECT( PosXY p1 , PosXY p2 ): position(p1), size(p2), T(0) {}
    void setTexture( TextureIF* t )
    {
	T = t;
    }
    
    GO_RECT( float f1, float f2, float f3, float f4): position(f1,f2),size(f3,f4)
    {}
    
    ~GO_RECT()
    {
	/*
	if(T)
	    delete T;
	*/
    }
};

#endif