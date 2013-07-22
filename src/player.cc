#include "../headers/player.h"

void player::drawBlock()
{
    vector< PosXY > PL;
    
    PosXY pivot(position.X , position.Y + (size.Y/2));
    PL.push_back( transform::rotateXY( PosXY(position.X - (size.X/2) , position.Y) , pivot , angle) );
    PL.push_back( transform::rotateXY( PosXY(position.X + (size.X/2) , position.Y) , pivot , angle) );
    PL.push_back( transform::rotateXY( PosXY(position.X - (size.X/2), position.Y + size.Y), pivot , angle) );
    PL.push_back( transform::rotateXY( PosXY(position.X + (size.X/2), position.Y + size.Y), pivot , angle) );
    
    Draw::TriangleStrip( PL );
}