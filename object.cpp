//#include <math.h>
#include "object.h"
#include <iostream>

Object::Object(float pX, float pY, float pZ)
	: position {pX,pY,pZ}
	, orientation {0,0,0}
	, scale {0,0,0}
	, material(1)
	, type(rand()%5)
	, size(1)
	, DownLeft {0,0,0}
	, UpperRight {0,0,0}
	, distToMouseRay(-1)
{
	initCorner();
}

void Object::initCorner() {
	switch (type) {
	default:
		DownLeft[0] = position[0] - size / 2;
		DownLeft[1] = position[1] - size / 2;
		DownLeft[2] = position[2] - size / 2;
		UpperRight[0] = position[0] + size / 2;
		UpperRight[1] = position[1] + size / 2;
		UpperRight[2] = position[2] + size / 2;
		break;
	}
}

// Reset corner after the object is moved or rotated.
void Object::resetCorner(){
	// Lazy approach.
	initCorner();
}
