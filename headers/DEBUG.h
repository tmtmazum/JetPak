#ifndef __DEBUG_H__

#define __DEBUG_H__

#include <iostream>
#include <sstream>
#include <string>

template <class ty>
void DEBUG( std::string s1, ty t1 )
{
    std::cout << "DEBUGGING from " << s1
	    << " :: " << t1 << std::endl;
}
template <class ty>
void DEBUG( std::string s1, ty t1, ty t2 )
{
    std::cout << "DEBUGGING from " << s1
	    << " :: " << t1 << ", "
	    << t2 << std::endl;
}

template <class ty>
void DEBUG( std::string s1, ty t1, ty t2, ty t3 )
{
    std::cout << "DEBUGGING from " << s1
	    << " :: " << t1 << ", "
	    << t2 << ", " << t3 << std::endl;
}


std::string inline toString(const float& f)
{
    std::ostringstream ss;
    ss << f;
    return ss.str();
}

#endif