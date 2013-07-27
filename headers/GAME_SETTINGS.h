#ifndef __GAME_SETTINGS_H__
#define __GAME_SETTINGS_H__

#include <fstream>
#include <string>
#include <cassert>
#include <map>
#include <iostream>
#include "GAME_CONSTANTS.h"
#include "Util.h"
#include "DEBUG.h"

struct GAME_SETTINGS
{
    std::map<std::string, float> DATA;
    /*float operator[] (const char*& s)
    {
	std::string c;
	c.assign(s);
	return DATA[c];
    }*/
    float& operator[] (std::string s)
    {
	std::map<std::string, float>::iterator it = DATA.find( s );
	if( it == DATA.end() )
	{
	    DEBUG("GAME_SETTINGS[..]", "string not found in Settings:");
	    DEBUG("GAME_SETTINGS[..]", s);
	}
	return it->second;
    }
    
    void loadFromDefault()
    {
	DATA.clear();
	
	DATA[ "BACKGROUND_ON" ] = 1;
	
    }
    
    void printAll()
    {
	for(std::map<std::string,float>::iterator it = DATA.begin();
	    it != DATA.end();
	    it++)
	    {
		std::cout << "\'" << it->first << "\'" << " | " << it->second << std::endl;
	    }
    }
    
    void loadFromFile(std::string s)
    {
	DATA.clear();
	
	std::ifstream iFile( s.c_str() , std::ifstream::in);
	
	char line[256]; std::string lstr;
	
	iFile.getline( line , 256 );
	lstr.assign( line );
	
	if( lstr.compare("(BEGIN)") )
	{
	    DEBUG("FATAL ERROR: 'Begin' not found",0);
	    assert(false);
	    loadFromDefault();
	    return;
	}
	
	while( true )
	{
	    iFile.getline( line , 256 );
	    
	    if( !iFile.good() ) break;
	    
	    lstr.assign( line );

	    if( !lstr.compare("(END)") )
	    {
		return;
	    }
	    
	    // Get {
	    std::string CONST_ID;
	    std::string CONST_VALUE;
	    // } from
	    {
		std::size_t start_loc = lstr.find("(");
		if( start_loc == std::string::npos )
		{
		    continue;
		}
		
		std::size_t div_loc = lstr.find( ' ' , start_loc+1 );
		assert( div_loc != std::string::npos );
		
		
		CONST_ID = lstr.substr( start_loc+1 , (div_loc)-(start_loc+1) );
		
		std::size_t end_loc = lstr.find( ")" );
		assert( end_loc != std::string::npos );
		
		CONST_VALUE = lstr.substr( div_loc , end_loc-div_loc );
	    }
	    
	    float const_value_f = Util::stringToFloat( CONST_VALUE );
	    DATA.insert( std::pair<std::string,float>(CONST_ID, const_value_f) );
	    
	}
    }
};

#endif