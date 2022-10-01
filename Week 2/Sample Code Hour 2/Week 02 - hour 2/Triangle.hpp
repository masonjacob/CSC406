//
//  Triangle.hpp
//  Week 01 - New
//
//  Created by Jean-Yves Hervé on 9/13/22.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include <stdio.h>


class Triangle {

	private:
	
		float xy[3][2];

	public:
	
		Triangle(float theXy[][2]);
		~Triangle(void) = default;
	
		// basic suite of constructors and operators
		Triangle(void) = delete;	// default
		Triangle(const Triangle& obj) = delete;	// copy
		Triangle(Triangle&& obj) = delete;	// move constructor
		Triangle operator = (const Triangle& obj) = delete;	// copy operator
		Triangle operator = (Triangle&& obj) = delete;	// move operator
		
		void draw(void) const;
		
};

#endif /* Triangle_hpp */






