#ifndef __COLOR_H__
#define __COLOR_H__

class Color
{public:
      virtual void get() = 0;
      
      Color() {}
};

class ColorRGBA : public Color
{    public: 
    float R,G,B,A;
    float* array;

    /* Constructors */
	ColorRGBA(float f1, float f2, float f3, float f4) : Color(),
			R(f1), G(f2), B(f3), A(f4) 
	{}
		
	ColorRGBA(float f1, float f2, float f3) : Color(),
			R(f1), G(f2), B(f3), A(1.0) 
	{
	    array = new float[4];
	    
	    array[0] = f1;
	    array[1] = f2;
	    array[2] = f3;
	    array[3] = A;
	}
		
	ColorRGBA(const ColorRGBA& C1) : Color(),
			R(C1.R), G(C1.G), B(C1.B), A(C1.A) 
	{}
	
	ColorRGBA(): Color(),
			R(255.0), G(255.0), B(255.0), A(255.0) 
	{}
	
	ColorRGBA(float f): Color(),
			R(f), G(f), B(f), A(f) 
	{}
	

    /* Overloaded Operators */
	void operator=(const Color& C1) { (*this) = (ColorRGBA&) C1; }
	void operator=(const ColorRGBA& C1) { this->R = C1.R; this->G = C1.G;
					this->B = C1.B; this->A = C1.A; }
	
	~ColorRGBA() { /*delete array;*/ }
      void get();
      void get(float al);
      float* toArray()
      {
	    return array;
      }
};

namespace Colors
{
    static ColorRGBA Red(1.0, 0.0, 0.0);
    static ColorRGBA Green(0.0, 1.0, 0.0);
    static ColorRGBA Blue(0.0, 0.0, 1.0);
    static ColorRGBA Grey(0.5, 0.5, 0.5);
    static ColorRGBA White(1.0, 1.0, 1.0);
    static ColorRGBA Black(0.0, 0.0, 0.0);
    static ColorRGBA Yellow(1.0 , 1.0, 0.0);
    static ColorRGBA Orange(1.0 , 0.6, 0.0);
    static ColorRGBA DarkOrange(1.0, 0.4, 0.0);
};

#endif