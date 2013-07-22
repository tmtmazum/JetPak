#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "Pos.h"
#include <GL/glut.h>

struct MouseAB
{
    enum BUTTONS
    {
	LMB = 1, MMB = 2, RMB = 4, ALL_BUTTONS=7
    };
    
    PosXY At;
    int buttonsPressed;
    
    // --- OPTIONAL DATA ---
    int lastButton;	// The last button that was switched
			// switched on or off
    PosXY savedPos;
		
    MouseAB(): At(0,0), buttonsPressed(0)
    {}
			
    void setPosition(int x, int y)
    {
	At = PosXY(x,y);
    }
    
    void toggleButton(int gl_button, int gl_state)
    {
	bool new_state = (gl_state==GLUT_DOWN ? true : false);
	
	switch(gl_button)
	{
	    case GLUT_LEFT_BUTTON:
		buttonsPressed = (new_state ? buttonsPressed | LMB : 
					buttonsPressed & ~LMB);
		lastButton = LMB;
		break;
	    case GLUT_MIDDLE_BUTTON:
		buttonsPressed = (new_state ? buttonsPressed | MMB : 
					buttonsPressed & ~MMB);
		lastButton = MMB;
		break;
	    case GLUT_RIGHT_BUTTON:
		buttonsPressed = (new_state ? buttonsPressed | RMB : 
					buttonsPressed & ~RMB);
		lastButton = RMB;
		break;
	}
    }
    
    bool isPressed( int button )
    {
	return buttonsPressed & button;
	/*
	if(button == LMB)
	{
	    return buttonsPressed & LMB;
	}
	else if (button == MMB)
	{
	    return buttonsPressed & MMB;
	}
	else if(button == RMB)
	{
	    return buttonsPressed & RMB;
	}*/
    }
    void savePos()
    {
	savedPos = At; 
    }
    
    PosXY loadPos()
    {
	return savedPos;
    }
    
    static PosXY getDiffEither( PosXY Current , PosXY Old , PosXY WindowSize )
    {
	
	PosXY MouseDiff( -(Old.X - Current.X), -(Old.Y -Current.Y) );
	
	if(abs(MouseDiff.X) > abs(MouseDiff.Y)) MouseDiff = PosXY( MouseDiff.X, 0 );
	else MouseDiff = PosXY( 0, MouseDiff.Y );
	
	return PosXY( MouseDiff.X/WindowSize.X, MouseDiff.Y/WindowSize.Y );
    }
};

#endif