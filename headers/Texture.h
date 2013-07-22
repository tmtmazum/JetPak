#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Pos.h"
#include <string>
#include <vector>
#include "DEBUG.h"
#include "imageloader.h"
#include "lodepng.h"

#include <GL/glut.h>

struct ImagePNG
{
    unsigned width, height;
    std::vector< unsigned char > image;
    ImagePNG( std::string s)
    {
	unsigned error = lodepng::decode( image , width , height , s.c_str() );
	
	// If there's an error, display it.
	if(error != 0)
	{
	    std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
	}
    }
    
};

struct TextureIF
// 2D Texture struct with an ID and tilingFactor
{
    // std::vector< char >	 ImagePixels;
    // PosXY ImageSize;
    
    // GL Variables:
    // GLuint texBufferID;
    GLuint ID;
    PosXY tilingFactor;
    
    TextureIF()
    {
	ID = -1;
	tilingFactor = PosXY(1.0,1.0);
    }
    
    TextureIF( const ImagePNG& p )
    {
	// Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
	size_t u2 = 1; while(u2 < p.width) u2 *= 2;
	size_t v2 = 1; while(v2 < p.height) v2 *= 2;
	
	// Ratio for power of two version compared to actual version, to render the non power of two image with proper size.
	double u3 = (double)p.width / u2;
	double v3 = (double)p.height / v2;

	// Make power of two version of the image.
	std::vector<unsigned char> image2(u2 * v2 * 4);
	for(size_t y = 0; y < p.height; y++)
	for(size_t x = 0; x < p.width; x++)
	for(size_t c = 0; c < 4; c++)
	{
	    image2[4 * u2 * y + 4 * x + c] = p.image[4 * p.width * y + 4 * x + c];
	}
	
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(
	    GL_TEXTURE_2D,
	    0,
	    4,
	    u2,
	    v2,
	    0,
	    GL_RGBA,
	    GL_UNSIGNED_BYTE,
	    &image2[0]
	);
    }
    
    TextureIF( const char* filepath, PosXY tile=PosXY(1.0,1.0) )
    {
	ImagePWH* myImage = ImagePWH::loadBMP( filepath );
	this->ID= this->load( myImage );
	delete myImage;
	
	tilingFactor = tile;
    }
    TextureIF( const TextureIF& T1 )
    {
	this->ID = T1.ID;
	this->tilingFactor = T1.tilingFactor;
    }
    
    void setFiltersToNearest() const
    {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
    }
    
    void setFiltersToLinear() const
    // For blurry
    {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    
    void drawSample() const
    {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ID);
	
	//Use blurry texture mapping (replace GL_LINEAR with GL_NEAREST for blocky)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glColor3f(1.0f, 1.0f, 1.0f); //Don't use special coloring
	
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 2.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(2.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
	
	glDisable(GL_TEXTURE_2D); //Switch back to using colors instead of textures
    }
     
    void enable() const
    {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->ID);
	this->setFiltersToLinear();
    }
    void disable() const
    {
	glDisable(GL_TEXTURE_2D);
    }
    
    ~TextureIF()
    {
	glDeleteTextures( 1, &ID );
    }
    
protected:
    GLuint load(ImagePWH* image)
    {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGB,
			image->width, image->height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			image->pixels);
	return textureId;
    }
    
};

#endif