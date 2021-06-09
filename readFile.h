#ifndef READFILE_H_INCLUDED
#define READFILE_H_INCLUDED

#include <gl/freeglut.h>
#include <math.h>
#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9

typedef struct Objects3D{

    float* normals;							// Stores the normals
    float* Faces_Triangles;					// Stores the triangles
    float* vertexBuffer;					// Stores the points which make the object
    long TotalConnectedPoints;				// Stores the total number of connected verteces
    long TotalConnectedTriangles;			// Stores the total number of connected triangles
	float rad;

}Object3D;

Object3D object[8];

void readOFFFile(char *Fname,Object3D *object);


#endif // READFILE_H_INCLUDED
