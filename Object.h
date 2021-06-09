#include <gl/freeglut.h>
#include <windows.h>
#include <math.h>

#define TIMERSEC 20

/************************************************************************
  Window
 ************************************************************************/

typedef struct {
	int width;
	int height;
	char* title;

	float field_of_view_angle;
	float z_near;
	float z_far;

} glutWindow;

/***************************************************************************
 * Program code
 ***************************************************************************/


float g_rotation;

glutWindow win;

//METHODS

void Draw();

void initialize();

void animate( );

void reshape( );

void display( );

void mainMenu( );

void keyOperations( );

void specialfunc( int key );

void specialfuncUp( int key );

void keyboard( unsigned char key );

void keyboardUp( unsigned char key );

void boundaries2();

void AI();

