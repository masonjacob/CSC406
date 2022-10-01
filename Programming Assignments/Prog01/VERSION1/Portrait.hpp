
#ifndef Portrait_hpp
#define Portrait_hpp

#include <stdio.h>
#include <cmath>


class Portrait {

	private:
	
		float portrait_center[2];

        // skin color, hair color, eye color
        float colors[3][4];

        float scale;

        float orientation;

		float hair[][2];

	public:

		~Portrait(void) = default;	
		// basic suite of constructors and operators
		Portrait(void) = delete;	// default
		Portrait(const Portrait& obj) = delete;	// copy
		Portrait(Portrait&& obj) = delete;	// move constructor
		Portrait operator = (const Portrait& obj) = delete;	// copy operator
		Portrait operator = (Portrait&& obj) = delete;	// move operator

        Portrait(float portrait_center[2], float scale, float orientation);
        Portrait(float portrait_center[2], float sclae, float orientation, float colors[3][4]);
		
		void draw(void) const;

        void scale_portrait(void) const;
        
        void rotate_portrait(void) const;

		void draw_circle() const;

		void draw_ellipse(int x,int y,float start_angle,float end_angle,int radius_x, int radius_y, bool fill) const;

		void draw_square() const;

		void draw_triagle() const;


		
};



#endif