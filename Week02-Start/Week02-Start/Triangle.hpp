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
	
		void draw(void) const;

};
#endif /* Triangle_hpp */
