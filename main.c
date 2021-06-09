#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include "Object.h"
#include "readFile.h"

//extern glutWindow win;

#define TIMERSEC 20

extern float previousTime;

int main(int argc, char** argv)
{
	// set window values
	win.width = 1200;
	win.height = 800;
	win.title = "OpenGL/GLUT OBJ Loader.";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;


	char Files[8][50]= {"Striker1.obj","Striker1.obj","base.obj","corner.obj","field.obj","goal1.obj","goal2.obj","Puck.obj"};

    for(int i=0; i<8; i++)
    {
        readOFFFile(Files[i],&object[i]);
    }

	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  // Display Mode
	glutInitWindowSize(win.width, win.height);					// set window size
	glutCreateWindow(win.title);								// create Window

	glutDisplayFunc(display);									// register Display Function
    glutIdleFunc(display);									// register Idle Function
    glutReshapeFunc( reshape );

    glutSpecialFunc(specialfunc);
    glutSpecialUpFunc(specialfuncUp);

    glutKeyboardFunc(keyboard);								// register Keyboard Handler
    glutKeyboardUpFunc(keyboardUp);

    previousTime = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(TIMERSEC, animate, 0);

	initialize();
	glutMainLoop();												// run GLUT mainloop
	return 0;
}




