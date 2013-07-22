#ifndef __POS_H__
#define __POS_H__

#include "Color.h"
#include "angle.h"
#include <math.h>
#include <string>
#include <sstream>

#include "Util.h"

// #define RT_SUM_OF_SQRS(a, b, c) sqrt( pow(a, 2), pow(b, 2), pow(c, 2) )

class PosRPT;
const float PI = 4.0 * atan( 1.0 );

class Pos;
class PosXY;
class PosRP;
class PosXYZ;
class PosRPT;

// float root_sum_of_squares( float t1, float t2, float t3=0 );

typedef float PosX;
typedef float PosY;
typedef float PosZ;

class Pos
{
    protected:
	ColorRGBA C;
    public:
		virtual void getVertex() = 0;	// Calls OpenGL function for plotting a vertex
										// Example: glVertex2f( X, Y );
		void getColor() { C.get(); }
		void setColor( Color& C1 ) 
		    { C = C1; }
		    
		// Pos() : C() {}
};



class PosXY : public Pos
{
	public:
		PosX X;
		PosY Y;
	
	public:
		PosXY(float f1, float f2): Pos(), X(f1), Y(f2) {}
		PosXY(): Pos(), X(0), Y(0) {}
		PosXY(const PosRP& P1);
		PosXY(const PosXY& P1) : X( P1.X ), Y(P1.Y) {}
		
		float operator-( const PosXY& P1 )
		// PosXYZ - PosXYZ computes the distance 
		// between the two points
		{
		    return Util::root_sum_of_squares(P1.X - X, P1.Y - Y, 0);
		}
	    
		void getVertex();
}; 

class PosRP : public Pos
// Polar Co-ordinates w/ r (distance from origin), phi (angle of x->y)
{
public:
    float radius;
    angleRAD angle;
    
    PosRP(float f1, float f2): Pos(), radius(f1), angle(f2) {}
    PosRP(): Pos(), radius(0), angle(0) {}
    PosRP(const PosRP& P1) : radius( P1.radius ), angle( P1.getAngle() ) {}
    PosRP(const PosXY& P1);
    
    float getAngle() const { return angle.value; }
    
    void getVertex();
};

class PosXYZ : public Pos
{
	public:
		PosX X;
		PosY Y;
		PosZ Z;
		
		float* array;
		
	public:
		PosXYZ(float f1=0, float f2=0, float f3=0): 
			    Pos(), X(f1), Y(f2), Z(f3) 
		{ 
		    array = 0;
		    /*
		    array = new float[3];
		    array[0] = X;
		    array[1] = Y;
		    array[2] = Z;*/
		}
			    
		PosXYZ( const PosXYZ& P1 ):
			    Pos(), X(P1.X), Y(P1.Y), Z(P1.Z) {}
		PosXYZ( const PosRPT& P1 );	// DEFINED
		
		PosXYZ( const PosXY& P1 ):
			    Pos(), X(P1.X), Y(P1.Y), Z(1.0f) {}
		
		PosXYZ operator+( const PosXYZ& P1 )
		// The + Operator represents all translations
		// (1,1,1) + (2,2,2) = (3,3,3)
		{ 
		    return PosXYZ( this->X + P1.X, 
				    this->Y + P1.Y, 
				    this->Z + P1.Z );
		}
		
		PosXYZ operator+( const float& f1 )
		{
			return PosXYZ( X + f1, Y + f1, Z + f1 ); 
		}
		
		float operator-( const PosXYZ& P1 )
		// PosXYZ - PosXYZ computes the distance between the two points
		{
		    return Util::root_sum_of_squares(P1.X - X, P1.Y - Y, P1.Z - Z);
		}
		
		void operator+=( const PosXYZ& P1 )
		{
		    X += P1.X;
		    Y += P1.Y;
		    Z += P1.Z;
		}
		
		PosXY toXY() { return PosXY( X, Y ); }
		
		
		float* toArray()
		{
		    array = new float[3];
		    array[0] = X;
		    array[1] = Y;
		    array[2] = Z;
		    return array;
		}
		
		float* load(float* f)
		{
		    f[0] = X;
		    f[1] = Y;
		    f[2] = Z;
		    return f;
		}
		
		~PosXYZ() { /* if(array) delete array;*/ }
		void getVertex();
		void getVertex(float nX, float nY, float nZ);
};

class PosRPT : public Pos
// Spherical Co-ordinate class
{
public:
    float radius;
    float phi;		/* Angle from x -> y */
    float theta;	/* Angle from z -> xy */

    PosRPT(float f1, float f2, float f3): 
	Pos(), radius(f1), phi(f2), theta(f3) {}
    PosRPT(): 
	Pos(), radius(0), phi(0), theta(0) {}
    PosRPT(const PosRPT& P1) : 
	radius( P1.radius ), phi( P1.getPhi() ), theta( P1.getTheta() ) {}
    
    PosRPT(const PosXYZ& P1);	// DEFINED
    
    float getPhi() const { return phi; }
    float getTheta() const { return theta; }
    
    void getVertex();	// DEFINE THIS
};

struct RangeX
{
    float lower, upper;
    RangeX(float f1, float f2)
    {
	lower = f1 < f2 ? f1 : f2;
	upper = f1 < f2 ? f2 : f1;
    }
    
    inline bool operator==( float f )
    {
	return (f >= lower) && (f <= upper); 
    }
    inline bool operator!=( float f1 ) 
    { 
	return (f1 < lower) || (f1 > upper);
    }
    inline bool operator<( float f1 ) 
    { 
	return (f1 < lower);
    }
    inline bool operator>( float f1 ) 
    { 
	return (f1 > upper);
    }
    
};

inline bool operator==( float f1 , RangeX x )
{
    return x==f1;
}
inline bool operator!=( float f1 , RangeX x )
{
    return x!=f1;
}
inline bool operator<( float f1 , RangeX x )
{
    return x<f1;
}
inline bool operator>( float f1 , RangeX x )
{
    return x>f1;
}

#endif