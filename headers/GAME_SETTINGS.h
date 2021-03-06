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

#include "WORLD.h"

struct LEVEL_TERRAIN
{
private:
    VecS getAll(std::string st)
    {
	std::size_t begin, end;
	
	VecS output;
	
	begin = st.find('(');
	while(true)
	{
	    end = st.find_first_of(") " , begin+1);
	    if( end==std::string::npos ) assert(false&&"ERROR:Couldn't find \')\'");
	    if( st.at(end)==')') return output;
	    output.push_back( st.substr(begin+1,end-(begin+1)) );
	    begin = end;
	}
    }
    
    void getWO(std::ifstream& iF)
    {
	char line[256];
	std::string lstr;
	while(true)
	{
	    WO out;
	    iF.getline( line , 256 );
	    lstr.assign( line );
	    std::size_t search = lstr.find("(END WO)");
	    if(search != std::string::npos )
	    {
		return;
	    }
	    VecS VS = getAll( lstr );
	    int prop = WO::getPropertyEnum( VS.front() );
	    
	    VecF VF;
	    for(VecS::iterator it = VS.begin(); it != VS.end(); ++it)
	    {
		if(it==VS.begin()) continue;
		VF.push_back( Util::stringToFloat( VS.at(i) ) );
	    }
	    
	    if(prop==WO::GEOMETRY)
	    {    
		out.setGeometry( prop , VF );
	    }
	    else
	    {
		out.set( prop
	    }
	}
    }
public:
    std::vector< WO& > DATA;
    // std::map< std::string , VecF > DATA;
    
    void loadFromFile(std::string s)
    {
	DATA.clear();
	
	std::ifstream iFile( s.c_str() , std::ifstream::in);
	
	char line[256]; std::string lstr;
	
	while(true)
	// Find beginning point
	{
	    iFile.getline( line , 256 );
	    if( !iFile.good() ) { return; }
	    lstr.assign( line );
	    
	    std::size_t start = lstr.find("(BEGIN WO)");
	    if(start != std::string::npos)
	    {
		getWO( iFile );
	    }
	}
	
	while(true)
	{
	    iFile.getline( line , 256 );
	    
	    if( !iFile.good() ) break;
	    
	    lstr.assign( line );

	    if( !lstr.compare("(END LEVEL)") )
	    {
		return;
	    }
	    std::string CONST_ID;
	    std::string CONST_VALUE;
	    
	    VecF Values;
	    {
		std::size_t start_loc = lstr.find("(");
		if( start_loc == std::string::npos )
		{
		    continue;
		}
		
		std::size_t div_loc = lstr.find( ' ' , start_loc+1 );
		assert( div_loc != std::string::npos );
		
		CONST_ID = lstr.substr( start_loc+1 , (div_loc)-(start_loc+1) );
		start_loc = div_loc;
		
		loop:
		
		div_loc = lstr.find_first_of( ") " , start_loc+1 );
		assert( div_loc != std::string::npos );
		if(lstr.at(div_loc)==')') continue;
		
		CONST_VALUE = lstr.substr( start_loc+1 , (div_loc) - (start_loc+1));
		float const_value_f = Util::stringToFloat( CONST_VALUE );
		Values.push_back( const_value_f );
		start_loc = div_loc;
		goto loop;
	    }
	    DATA.insert( std::pair<std::string,VecF>( CONST_ID , Values ) );
	}
    }
};

struct GAME_SETTINGS
{
    std::map<std::string, float> DATA;
    
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