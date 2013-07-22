#ifndef LEVEL_H
#define LEVEL_H

#include "Pos.h"
#include "LAND.h"
#include "GEOMETRY.h"
#include <vector>
#include "Draw.h"
#include "player.h"
#include "DEBUG.h"

#include <cassert>
#include <GL/glut.h>

struct LEVEL_OBJECT
{
    virtual ~LEVEL_OBJECT() {}
};
/*
struct COLLISION_OBJECT
{
    GEOMETRY_OBJECT* GO;
    LAND_OBJECT* LO;
    RangeX range;
    
    bool ready;
    
    COLLISION_OBJECT() { ready = false; }
    
    void makeRectFloor( GO_RECT* GR )
    {
	GO = GR;
	LO = new LO_PLAIN_FLOOR( GR->position.X , GR->position.X + GR->size.X ,
				    GR->position.Y );
	range = RangeX( GR->position.X , GR->position.X );
	
	ready = true;
    }
};

bool lessThan( const COLLISION_OBJECT& c1,
	       const COLLISION_OBJECT& c2 )
{
    return c1.range < c2.range;
}

bool greaterThan( const COLLISION_OBJECT& c1,
	       const COLLISION_OBJECT& c2 )
{
    return c1.range > c2.range;
}

struct LEVEL_BLOCK
{
// Has size of screen
    std::vector< COLLISION_OBJECT* > left;
    std::vector< COLLISION_OBJECT* > right;
    
    LEVEL_BLOCK()
    {
	
    }
    
    PosXY checkCollision( PosXY current )
    {
	
    }
};*/

struct LEVEL_BLOCK_SIMPLE : public LEVEL_OBJECT
{
    std::vector< LAND_OBJECT* > LOs;
    std::vector< GEOMETRY_OBJECT* > GOs;
    
    LAND_OBJECT* lastCollision;
    
    LEVEL_BLOCK_SIMPLE() { lastCollision=0; }
    
    void checkCollision( player& r , PosXY& prevPos )
    {
	if(r.onGround && lastCollision!=0)
	{
	    PosXY nt = PosXY(r.position.X , r.position.Y - 0.00001);
	    LO_FLAT_FLOOR* Ls = dynamic_cast< LO_FLAT_FLOOR* >( lastCollision );
	    if( Ls!=0 && !Ls->isCollision(nt, prevPos) )
	    {
		r.onGround = false;
		lastCollision = 0;
		return;
	    }
	}
	else
	{
	
	    for(std::vector<LAND_OBJECT*>::iterator it = LOs.begin(); it != LOs.end(); ++it)
	    {
		LO_FLAT_FLOOR* Ls = dynamic_cast< LO_FLAT_FLOOR* >( *it );
		LO_FLAT_RECT* Lr = dynamic_cast< LO_FLAT_RECT* > (*it );
		if( Ls !=0 )
		{
		    if( Ls->isCollision(r.position , prevPos) )
		    {
			PosXY t(r.position.X , 0.00001 + Ls->Y );
			lastCollision = *it;
			r.setCollide( true , t );  
			return;
		    }
		}
		else if( Lr != 0)
		{
		    int c = Lr->isCollision( r.position , prevPos );
		    if( c )
		    {
			lastCollision = *it;
			PosXY nP(prevPos.X , prevPos.Y);
			if( c==(LAND_OBJECT::LANDING_COLLISION) )
			    nP.Y = Lr->Y.upper + LAND_OBJECT::elevation;
			r.setCollide( c==(LAND_OBJECT::LANDING_COLLISION), nP );
			return;
		    }
		}
	    }
	
	}
    }
    
    void drawLevel()
    {	
	assert( !GOs.empty() );
	Colors::Red.get();
	
	for(std::vector<GEOMETRY_OBJECT*>::iterator it = GOs.begin(); it != GOs.end(); ++it)
	{
	    Draw::Generic( *it );
	}
    }
    
    void add( LAND_OBJECT* lo, GEOMETRY_OBJECT* go)
    {
	LOs.push_back( lo );
	GOs.push_back( go ); 
    }
    
    void addRect( PosXY r , PosXY size )
    {
	LOs.push_back( new LO_FLAT_RECT(r.X , r.X + size.X , r.Y , r.Y + size.Y ) );
	GOs.push_back( new GO_RECT( PosXY( r.X , r.Y ) , PosXY( size.X , size.Y ) ) );
    }
    
    ~LEVEL_BLOCK_SIMPLE()
    {
	for(std::vector<LAND_OBJECT*>::iterator it = LOs.begin(); it != LOs.end(); ++it)
	{
	    delete (*it);
	}
	for(std::vector<GEOMETRY_OBJECT*>::iterator it = GOs.begin(); it != GOs.end(); ++it)
	{
	    delete ( *it );
	}
    }
};
/*
struct LEVEL_BLOCK_SIMPLE_1
{
    vector< GO_RECT > vG;
    LEVEL_BLOCK_SIMPLE_1()
    {
	vG.push_back( GO_RECT( PosXY(0.1, 0) , PosXY(0.4,0.1) ) );
	vG.push_back( GO_RECT( PosXY(0.6, 0), PosXY(0.4,0.2) ) );
	vG.push_back( GO_RECT( PosXY(1.0, 0), PosXY(0.4,0.4) ) );
    }
    void checkCollision( player r )
    {
	if(r.position.X == RangeX(0,1.0) )
	{
		
	    for( int i = 0; i < vG.size(); ++i)
	    {
	    }
	}
    }
    
    void drawLevel()
    {
	Colors::Red.get();
	Draw::Generic( GO_RECT( PosXY(0.1,0.1) , PosXY(0.4,0.1) ) );
	Draw::Generic( GO_RECT( PosXY(0.6, 0.2), PosXY(0.4,0.2) ) );
	Draw::Generic( GO_RECT( PosXY(0.6, 0.2), PosXY(0.4,0.2) ) );
	
    }
};
*/
#endif