#ifndef __UTIL_H__
#define __UTIL_H__

#include <cmath>

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

};

#endif