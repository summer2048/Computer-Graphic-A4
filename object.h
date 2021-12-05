#ifndef OBJECT_H
#define OBJECT_H


#define Cube 0
#define Sphere 1
#define Cone 2
#define Cylinder 3
#define Teapot 4

class Object
{
public:
	float position[3];
	float orientation[3];
	float scale[3];
	int material;
	int type;
	float size;
    float DownLeft[3]; // Minimal x,y,z of surronding box
    float UpperRight[3]; // Maximal x,y,z of surronding box
    float distToMouseRay; // Set it to -1 if no intersection

	Object(float pX, float pY, float pZ);
    void resetCorner();
private:
    void initCorner();
};

#endif
