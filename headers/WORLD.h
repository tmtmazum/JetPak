#ifndef __WORLD_OBJECT_H__
#define __WORLD_OBJECT_H__

#include "Util.h"
#include "angle.h"
#include "Pos.h"
#include "transform.h"
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <cstdarg>
#include <GL/glut.h>

typedef std::vector<float> VecF;
typedef std::map<std::string, float> MapSF;
typedef std::map<int,float> MapIF;

struct GO
{
    enum
    {
    LINE,
    RECT
    };
};

struct WO
{
    int GO_type;
    VecF GO_data;

    std::map< int, std::vector<float> > property_data;

    enum property
    {
	POSITION,
	COLOR,
	ALPHA,
	ROTATION,
	SCALE,
	NP_SCALE,
	GEOMETRY,
	COLLISION
    };
        
    static int getPropertyEnum(std::string s)
    {
	switch(s)
	{
	    case "POSITION":
		return POSITION;
	    case "COLOR":
		return COLOR;
	    case "ALPHA":
		return ALPHA;
	    case "ROTATION":
		return ROTATION;
	    case "GEOMETRY":
		return GEOMETRY;
	    case "SCALE":
		return SCALE;
	    case "NP_SCALE":
		return NP_SCALE;
	}
    }
    
    WO() {}
    
    void setGeometry( int i , ... )
    {
	GO_type = i;
	VecF PD;
	va_list ARGs;
	va_start( ARGs , i );
	while( true )
	{
	    float g = va_arg( ARGs , double );
	    if( g == END_F ) 
	    break;
	    GO_data.push_back( g );
	}
	va_end( ARGs );
    }
    void setGeometry( int i , const VecF& dt )
    {
	GO_type = i;
	GO_data = dt;
    }
    
    
    void set( property p , ... )
    {
	VecF PD;
	va_list ARGs;
	va_start( ARGs , p );
	while( true )
	{
	    float g = va_arg( ARGs , double );
	    if( g == END_F ) break;
	    PD.push_back( g );
	}
	va_end( ARGs );
	property_data[ p ] = PD; 
    }
    void set( property p , const VecF& VF )
    {
	property_data[p] = VF;
    }
    
    VecF get(property p) 
    {
	VecF V = property_data[ p ];
	return V;
    }
    int getGeometryType() const
    {
	return GO_type;
    }
    VecF getGeometryData() const
    {
	return GO_data;
    }
};

inline void draw2D( WO& obj )
{
    VecF Rot = obj.get(WO::ROTATION);
    VecF V = obj.get(WO::COLOR);
    if( !V.empty() && V.size()==3 )
    {
	glColor4f( V.at(0) , V.at(1) , V.at(2) , obj.get(WO::ALPHA).at(0) );
    }
    else if( !V.empty() && V.size()==4)
    {
	glColor4f( V.at(0) , V.at(1) , V.at(2) , V.at(3) );
    }
    else if( !V.empty() ) assert(false);
    
    int t = obj.getGeometryType();
    switch( t )
    {
	case GO::RECT:
	    VecF P = obj.get(WO::POSITION);
	    V = obj.getGeometryData();
	    PosXY Pt( P.at(0) , P.at(1) );
	    PosXY Pt2( P.at(0) + V.at(0) , P.at(1) );
	    PosXY Pt3( P.at(0) + V.at(0) , P.at(1) + V.at(1) );
	    PosXY Pt4( P.at(0) , P.at(1) + V.at(1) );
	    if( !Rot.empty() )
	    {
		float angl = Rot.at(0) * PI;
		Pt2 = transform::rotateXY( Pt2 , Pt , angl , false );
		Pt3 = transform::rotateXY( Pt3 , Pt , angl , false );
		Pt4 = transform::rotateXY( Pt4 , Pt , angl , false );
	    }
	    glBegin( GL_QUADS );
		Pt.getVertex();
		Pt2.getVertex();
		Pt3.getVertex();
		Pt4.getVertex();
	    glEnd();
    }
}


#endif