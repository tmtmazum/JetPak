#ifndef __UTIL_H__
#define __UTIL_H__

#include <cmath>
#include <cassert>
#include <string>
#include <cstdlib>

#include <limits>

#define END_F std::numeric_limits<float>::max()

typedef std::vector<float> VecF;
typedef std::map<std::string, float> MapSF;
typedef std::map<int,float> MapIF;
typedef std::vector<string> VecS;

namespace Util
{
    
    inline float root_sum_of_squares( float t1, float t2, float t3 )
    {
	return std::sqrt( std::pow(t1, 2) + std::pow(t2, 2) + std::pow(t3, 2) );
    }
    
    inline float min(float f1, float f2)
    {
	return f1 < f2 ? f1 : f2;
    }
    
    inline float min(float f1, float f2, float f3, float f4)
    {
	return min( min( min(f1,f2), f3) , f4 ); 
    }
    
    inline float stringToFloat( std::string s )
    {
	return ::atof( s.c_str() );
    }

};

struct bitmask
{
    int value;
    bitmask(int i)
    {
	value = i;
    }
    
    void set(int i , bool b)
    {
	i = pow(2,i);
	if(b)
	    value = value | i;
	else
	    value = value & ~i;
    }
    
    bool get(int i)
    {
	i = pow(2,i);
	return value & i;
    }
};

#endif