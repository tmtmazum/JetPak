#include "../headers/Draw.h"
// #include "../headers/PosManager.h"
#include "../headers/DEBUG.h"

#include <cassert>
#include <cstring>
#include <GL/glut.h>

void Draw::TriangleStrip( const std::vector< PosXY >& V )
{
    int n = V.size();
    float pts[ n ] [ 2 ];
    
    for(int i = 0; i < n; ++i)
    {
	pts[i][0] = V.at(i).X;
	pts[i][1] = V.at(i).Y;
    }
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer(2, GL_FLOAT, 0, pts);
    glDrawArrays( GL_TRIANGLE_STRIP , 0 , n );
    glDisableClientState( GL_VERTEX_ARRAY );
}

void Draw::Generic( GEOMETRY_OBJECT* R )
{
    GO_RECT* gr = dynamic_cast<GO_RECT*> ( R );
    if(gr != 0) Draw::Generic( *gr );
}

void Draw::Generic( const GO_RECT& R )
{
    PosXY t1(R.position.X , R.position.Y);
    PosXY t2(R.position.X , R.position.Y+ R.size.Y);
    PosXY t3(R.position.X + R.size.X , R.position.Y+ R.size.Y);
    PosXY t4(R.position.X+ R.size.X , R.position.Y);
	
    glBegin( GL_QUADS );
	    t1.getVertex();
	    t2.getVertex();
	    t3.getVertex();
	    t4.getVertex();
    glEnd();
    
    return;
}
