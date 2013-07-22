#ifndef __DRAW_H__
#define __DRAW_H__


#include "GEOMETRY.h"
#include "Pos.h"
#include "Color.h"

#include <vector>

class PosManager;

namespace Draw
{
	void TriangleStrip( const std::vector< PosXY >& V );
	void Generic( GEOMETRY_OBJECT* );
	void Generic( const GO_RECT& R );
};

#endif
