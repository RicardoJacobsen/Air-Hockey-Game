#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gl/freeglut.h>
#include <stdbool.h>
#include <math.h>

#include "Object.h"
#include "readFile.h"
#include "physics.h"
#include "AI.h"

bool* upPressed[256];

GLfloat viewer[9] = { 0,170,170,
                      0,70,0,
                      0,1,0};

float strikerSpeedLeft = 2.0;
float strikerSpeedRight = 2.0;
float strikerSpeedTop = 2.0;
float strikerSpeedBottom = 2.0;

static float defaultSpeed = 2.0;

bool flag_goal = false;

// Variables for puck animation! Current Position will be equal to initial at the beginning

GLfloat puckCurrentPosition[3] = {0.0, 0.0, 0.0};
GLfloat puckStartPosition[3] = {0.0, 0.0, 0.0};
GLfloat puckPreviousPosition[3] = {0.0, 0.0, 0.0};

GLfloat puckCurrentVelocity[3] = {0.0, 0.0, 0.0};
GLfloat puckPreviousVelocity[3] = {0.0, 0.0, 0.0};

GLfloat puckDeltaT;

//the half way float for zone 1
static float zone1 = 0;

// the x float for zones 2 and 4
static float zone2  = -24.2;

// the x float for zones 3 and 5
static float zone3 = 22.95;

//The z float for zones 4 and 5
static float zone4 = -37;

//distance - speed ratio
static float ratio = 0.3;

// position of players striker
GLfloat strikerCurrentPosition[3] = {0.0, 0.0, 70.0};
GLfloat strikerStartPosition[3] = {0.0, 0.0, 70.0};
GLfloat strikerCurrentVelocity[3] = {2.0 ,0 , 2.0};

//position of AI striker
GLfloat striker2CurrentPosition[3] = {0.0,0.0, -70.0};
GLfloat striker2PreviousPosition[3] = {0.0,0.0, -70.0};
GLfloat striker2CurrentVelocity[3] = {1.0,0.0,0.0};
GLfloat striker2PreviousVelocity[3] = {0.0,0.0,0.0};
GLfloat striker2StartPosition[3] = {0.0,0.0, -70.0};

//setting the boundaries variables for the game/ table
static float tableLeft = -47.199852, tableRight =47.099854, tableUp = -94.900078, tableDown = 93.0000, tableCenterLine = 0;

static float bottomGoalFront = 85.000, GoalRight = 24.000, GoalLeft = -24.000, topGoalFront = -88.900;

//temporary radius
static float strikerRad = 8.4;
static float puckRadius = 5.8;

static float strikerMass = 0.5;
static float puckMass = 0.2;

float Friction = -0.01;
float maxSpeed = 5;
float minSpeed2 = 1.5;
float minSpeed = 0.1;

//to keep track of timerSec
GLfloat currentTime = 0, previousTime = 0;
GLfloat collisionMoment = 0;

DrawStriker1()
{
    glPushMatrix();

        glTranslatef(strikerCurrentPosition[0],
                     strikerCurrentPosition[1],
                     strikerCurrentPosition[2]);
        glColor3f(0,1,3);
        glEnableClientState(GL_VERTEX_ARRAY);						            // Enable vertex arrays
        glEnableClientState(GL_NORMAL_ARRAY);						            // Enable normal arrays
        glVertexPointer(3, GL_FLOAT, 0, object[0].Faces_Triangles);			    // Vertex Pointer to triangle array
        glNormalPointer(GL_FLOAT, 0, object[0].normals);						// Normal pointer to normal array
        glDrawArrays(GL_TRIANGLES, 0, object[0].TotalConnectedTriangles);		// Draw the triangles
        glDisableClientState(GL_VERTEX_ARRAY);						            // Disable vertex arrays
        glDisableClientState(GL_NORMAL_ARRAY);						            // Disable normal arrays
    glPopMatrix();
}

DrawStriker2()
{
    glTranslatef(striker2CurrentPosition[0],
                 striker2CurrentPosition[1],
                 striker2CurrentPosition[2]);
    glColor3f(0,1,0);
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3, GL_FLOAT, 0, object[1].Faces_Triangles);			// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, object[1].normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, object[1].TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
	glPopMatrix();
}

DrawBase()
{   glColor3f(0.1,0.1,0.1);
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3, GL_FLOAT, 0, object[2].Faces_Triangles);			// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, object[2].normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, object[2].TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
}

DrawCorner()
{   glColor3f(3,3,3);
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3, GL_FLOAT, 0, object[3].Faces_Triangles);			// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, object[3].normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, object[3].TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
}

DrawField()
{   glColor3f(1,1,1);
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3, GL_FLOAT, 0, object[4].Faces_Triangles);			// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, object[4].normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, object[4].TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
}

DrawGoal1()
{   glColor3f(1,0,0);
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3, GL_FLOAT, 0, object[5].Faces_Triangles);			// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, object[5].normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, object[5].TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
}

DrawGoal2()
{   glColor3f(0,0,1);
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3, GL_FLOAT, 0, object[6].Faces_Triangles);			// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, object[6].normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, object[6].TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
}

DrawPuck()
{
    glPushMatrix();
        glTranslatef(puckCurrentPosition[0],
                     puckCurrentPosition[1],
                     puckCurrentPosition[2]);
        glColor3f(1,0,0);
        glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
        glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
        glVertexPointer(3, GL_FLOAT, 0, object[7].Faces_Triangles);			// Vertex Pointer to triangle array
        glNormalPointer(GL_FLOAT, 0, object[7].normals);						// Normal pointer to normal array
        glDrawArrays(GL_TRIANGLES, 0, object[7].TotalConnectedTriangles);		// Draw the triangles
        glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
        glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
	glPopMatrix();
}
typedef GLfloat point3[3];

GLfloat camposx=0,camposy=2,camposz=3;

float floorHeight;

//CODE FOR GL
void initialize()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);

	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	puckCurrentVelocity[0] = (rand() % (1 + 1 - -1) + -1);
	puckCurrentVelocity[2] = (rand() % (1 + 1 - -1) + -1);
}

void reset()
{
    puckCurrentPosition[0] = puckStartPosition[0];
    puckCurrentPosition[1] = puckStartPosition[1];
    puckCurrentPosition[2] = puckStartPosition[2];

    strikerCurrentPosition[0] = strikerStartPosition[0];
    strikerCurrentPosition[1] = strikerStartPosition[1];
    strikerCurrentPosition[2] = strikerStartPosition[2];

    striker2CurrentPosition[0] = striker2StartPosition[0];
    striker2CurrentPosition[1] = striker2StartPosition[1];
    striker2CurrentPosition[2] = striker2StartPosition[2];

    puckCurrentVelocity[0] = (rand() % (1 + 1 - -1) + -1);
    puckCurrentVelocity[1] = 0;
	puckCurrentVelocity[2] = (rand() % (1 + 1 - -1) + -1);

}

bool isColliding( )
{
    int distance = sqrt((strikerCurrentPosition[0] - puckCurrentPosition[0]) * (strikerCurrentPosition[0] - puckCurrentPosition[0]) +
                        (strikerCurrentPosition[1] - puckCurrentPosition[1]) * (strikerCurrentPosition[1] - puckCurrentPosition[1]) +
                        (strikerCurrentPosition[2] - puckCurrentPosition[2]) * (strikerCurrentPosition[2] - puckCurrentPosition[2]));
    return (distance < (strikerRad + puckRadius));
}

bool isColliding2( )
{
    int distance = sqrt((striker2CurrentPosition[0] - puckCurrentPosition[0]) * (striker2CurrentPosition[0] - puckCurrentPosition[0]) +
                        (striker2CurrentPosition[1] - puckCurrentPosition[1]) * (striker2CurrentPosition[1] - puckCurrentPosition[1]) +
                        (striker2CurrentPosition[2] - puckCurrentPosition[2]) * (striker2CurrentPosition[2] - puckCurrentPosition[2]));
    return (distance < (strikerRad + puckRadius));
}

void animate()
{
    if(puckCurrentPosition[0] > tableRight)
        puckCurrentVelocity[0] = - puckCurrentVelocity[0];

    if(puckCurrentPosition[0] < tableLeft)
        puckCurrentVelocity[0] = - puckCurrentVelocity[0];

    if(puckCurrentPosition[2] > tableUp)
        puckCurrentVelocity[2] = - puckCurrentVelocity[2];

    if(puckCurrentPosition[2] < tableDown)
        puckCurrentVelocity[2] = - puckCurrentVelocity[2];

    if(puckCurrentPosition[2] > tableDown && puckCurrentPosition[0] > GoalLeft + puckRadius && puckCurrentPosition[0] < GoalRight - puckRadius)
    {
        flag_goal = true;
        reset();
    }
    if(puckCurrentPosition[2] < tableUp && puckCurrentPosition[0] > GoalLeft + puckRadius && puckCurrentPosition[0] < GoalRight - puckRadius)
    {
        flag_goal = true;
        reset();
    }

    puckCurrentPosition[0] += puckCurrentVelocity[0];
    puckCurrentPosition[1] += puckCurrentVelocity[1];
    puckCurrentPosition[2] += puckCurrentVelocity[2];

    // Check collision on left part.
    if((strikerCurrentPosition[0]) + strikerSpeedLeft < tableLeft)
        strikerCurrentPosition[0] = tableLeft;

    if(strikerCurrentPosition[0] == tableLeft)
        strikerSpeedLeft = 0;
    else
        strikerSpeedLeft = defaultSpeed;

    // Check collision on right part.
    if(strikerCurrentPosition[0] + strikerSpeedRight > tableRight)
        strikerCurrentPosition[0] = tableRight;

    if(strikerCurrentPosition[0] == tableRight)
        strikerSpeedRight = 0;
    else
        strikerSpeedRight = defaultSpeed;

    // Check collision on up part.
    if(strikerCurrentPosition[2] + strikerSpeedTop < tableUp)
        strikerCurrentPosition[2] = tableUp;

    if(strikerCurrentPosition[2] == tableUp)
        strikerSpeedTop = 0;
    else
        strikerSpeedTop = defaultSpeed;

    // Check collision on down part.
    if(strikerCurrentPosition[2] + strikerSpeedRight > bottomGoalFront)
        strikerCurrentPosition[2] = bottomGoalFront;

    if(strikerCurrentPosition[2] == bottomGoalFront)
        strikerSpeedBottom = 0;
    else
        strikerSpeedBottom = defaultSpeed;

    if(isColliding())
    {
        puckCurrentVelocity[0] = (puckCurrentPosition[0] - strikerCurrentPosition[0])/2;
        puckCurrentVelocity[2] = (puckCurrentPosition[2] - strikerCurrentPosition[2])/2;
    }

    if(isColliding2())
    {
        puckCurrentVelocity[0] = (puckCurrentPosition[0] - striker2CurrentPosition[0])/2;
        puckCurrentVelocity[2] = (puckCurrentPosition[2] - striker2CurrentPosition[2])/2;
    }

    if(sqrt(puckCurrentVelocity[0]*puckCurrentVelocity[0] + puckCurrentVelocity[2]*puckCurrentVelocity[2]) < 1e-12)
    {
        puckCurrentVelocity[0] = rand();
		puckCurrentVelocity[2] = rand();
    }

    AI();
    boundaries2();

    striker2CurrentPosition[0] = striker2PreviousPosition[0] + striker2CurrentVelocity[0];
    striker2CurrentPosition[2] = striker2PreviousPosition[2] + striker2CurrentVelocity[2];

    striker2PreviousPosition[0] = striker2CurrentPosition[0];
    striker2PreviousPosition[2] = striker2CurrentPosition[2];
    //striker2CurrentVelocity[0] = 0;
    //striker2CurrentVelocity[1] = 0;
    //striker2CurrentVelocity[2] = 0;

    glutPostRedisplay();
    glutTimerFunc(TIMERSEC, animate, 0);
}

void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, win.width, win.height);
	GLfloat aspect = (GLfloat)win.width / win.height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, 1000);
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
    keyOperations();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], viewer[3], viewer[4], viewer[5], viewer[6], viewer[7], viewer[8]);

	glPushMatrix();

	glRotatef(0, 0, 1, 0);
	glRotatef(0, 0, 1, 0);

	DrawStriker1();
	DrawStriker2();
	DrawBase();
	DrawCorner();
	DrawField();
	DrawGoal1();
	DrawGoal2();
	DrawPuck();

	drawRoom();

	glPopMatrix();
	glutSwapBuffers();

}

void mainMenu( int id )
{

    switch(id)
    {
    case 1:
        printf("Option 1");
        break;
    case 2:
        printf("Option 2");
        break;
    case 3:
        printf("Option 3");
        break;
    }

    glutPostRedisplay();
}

static float verticePoint = 400.0;
static float maxWallHeight = 500.0;

void drawRoom()
{
    floorHeight = 0.0;

    point3 roomVertices[8] =
    {
    {-verticePoint, floorHeight, verticePoint},
    {verticePoint, floorHeight, verticePoint},
    {verticePoint, floorHeight, -verticePoint},
    {-verticePoint, floorHeight, -verticePoint},
    {-verticePoint, floorHeight+maxWallHeight, verticePoint},
    {verticePoint, floorHeight+maxWallHeight, verticePoint},
    {verticePoint, floorHeight+maxWallHeight, -verticePoint},
    {-verticePoint, floorHeight+maxWallHeight, -verticePoint}
    };

    glColor3f(0.5, 0, 0);

    glBegin(GL_POLYGON);
        glVertex3fv(roomVertices[0]);
        glVertex3fv(roomVertices[1]);
        glVertex3fv(roomVertices[2]);
        glVertex3fv(roomVertices[3]);
    glEnd();

    glColor3f(0.5, 0.5, 0.0);

    glBegin(GL_POLYGON);
        glVertex3fv(roomVertices[3]);
        glVertex3fv(roomVertices[2]);
        glVertex3fv(roomVertices[6]);
        glVertex3fv(roomVertices[7]);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex3fv(roomVertices[1]);
        glVertex3fv(roomVertices[2]);
        glVertex3fv(roomVertices[6]);
        glVertex3fv(roomVertices[5]);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex3fv(roomVertices[0]);
        glVertex3fv(roomVertices[3]);
        glVertex3fv(roomVertices[7]);
        glVertex3fv(roomVertices[4]);
    glEnd();
}


void drawAxis()
{
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
    glEnd();

     glBegin(GL_LINES);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 1.0);
    glEnd();

}

void keyboard( unsigned char key )
{
    upPressed[key] = true;
}

void keyboardUp( unsigned char key )
{
    upPressed[key] = false;
}

void specialfunc( int key )
{
    upPressed[key] = true;
}

void specialfuncUp( int key )
{
    upPressed[key] = false;
}

void keyOperations( )
{
    if(upPressed['a'] || upPressed['A'])
        if(viewer[0] >= -50 && viewer[1] != 340)
            viewer[0] -= 10;

    if(upPressed['x'] || upPressed['X'])
        if(viewer[0] <= 50 && viewer[1] != 340)
            viewer[0] += 10;

    if(upPressed['w'] || upPressed['W'])
        if(viewer[2] >= 130 && viewer[1] != 340)
            viewer[2] -= 10;

    if(upPressed['s'] || upPressed['S'])
        if(viewer[2] <= 250 && viewer[1] != 340)
            viewer[2] += 10;

    if(upPressed['b'] || upPressed['B'])
    {
        viewer[0] = 0;
        viewer[1] = 340;
        viewer[2] = 10;
    }

    if(upPressed['v'] || upPressed['V'])
    {
        viewer[0] = 0;
        viewer[1] = 170;
        viewer[2] = 170;
    }

    if(upPressed['q'] || upPressed['Q'])
        exit(0);

    if(upPressed['r'] || upPressed['R'])
    {
        puckCurrentPosition[0] = puckStartPosition[0];
        puckCurrentPosition[2] = puckStartPosition[2];
    }

    if(upPressed[GLUT_KEY_LEFT])
        strikerCurrentPosition[0] -= strikerSpeedLeft;

    if(upPressed[GLUT_KEY_RIGHT])
        strikerCurrentPosition[0] += strikerSpeedRight;

    if(upPressed[GLUT_KEY_UP])
        strikerCurrentPosition[2] -= strikerSpeedTop;

    if(upPressed[GLUT_KEY_DOWN])
        strikerCurrentPosition[2] += strikerSpeedBottom;
}

void AI()
{
    float distance[3];
    float distanceToStart[3];

    float tempSpeedx = 0, tempSpeedz = 0;


    if(puckCurrentPosition[2] < zone1)
    {
        distance[0] = (puckCurrentPosition[0] - striker2CurrentPosition[0]);
        distance[2] = (puckCurrentPosition[2] - striker2CurrentPosition[2]);

        tempSpeedx = distance[0]*ratio;
        tempSpeedz = distance[2]*ratio;

        if(striker2CurrentPosition[0] > puckCurrentPosition[0] + puckCurrentVelocity[0] - puckRadius)
        {

            if(tempSpeedx > maxSpeed)
            {
                striker2CurrentVelocity[0] = -maxSpeed;

            }else if(tempSpeedx < minSpeed)
            {
                striker2CurrentVelocity[0] = -minSpeed2;

            }else
            {
                striker2CurrentVelocity[0] =  -tempSpeedx;

            }

        }else if( striker2CurrentPosition[0] < puckCurrentPosition[0] + puckCurrentVelocity[0] + puckRadius )
        {

            if(tempSpeedx > maxSpeed)
            {
                striker2CurrentVelocity[0] = maxSpeed;

            }else if(tempSpeedx < minSpeed)
            {
                striker2CurrentVelocity[0] = minSpeed2;

            }else
            {
                striker2CurrentVelocity[0] =  tempSpeedx;

            }

        }else
        {
            striker2CurrentVelocity[0] = 0;
        }

        if(striker2CurrentPosition[2] > puckCurrentPosition[2] + puckCurrentVelocity[2] + puckRadius )
        {

            if(tempSpeedz > maxSpeed)
            {
                striker2CurrentVelocity[2] = -maxSpeed;

            }else if(tempSpeedx < minSpeed)
            {
                striker2CurrentVelocity[2] = -minSpeed2;
            }else
            {
                striker2CurrentVelocity[2] =  -tempSpeedx;
            }

        }else if(striker2CurrentPosition[2] < puckCurrentPosition[2] + puckCurrentVelocity[2] - puckRadius )
        {

            if(tempSpeedz > maxSpeed)
            {
                striker2CurrentVelocity[2] = maxSpeed;

            }else if(tempSpeedx < minSpeed)
            {
                striker2CurrentVelocity[2] = minSpeed2;
            }else
            {
                striker2CurrentVelocity[2] =  tempSpeedx;
            }

        }else
        {
            striker2CurrentVelocity[2] = 0;
        }


    }else
    {
        // have striker move back to default position
        distanceToStart[0] = striker2CurrentPosition[0] - striker2StartPosition[0];
        distanceToStart[2] = striker2CurrentPosition[2] - striker2StartPosition[2];

        tempSpeedx = distanceToStart[0]*ratio;
        tempSpeedz = distanceToStart[2]*ratio;

        if(striker2CurrentPosition[0] > striker2StartPosition[0])
        {

            if(tempSpeedx > maxSpeed)
            {
                striker2CurrentVelocity[0] = -maxSpeed;

            }else if(tempSpeedx < minSpeed)
            {
                striker2CurrentVelocity[0] = -minSpeed;

            }else
            {
                striker2CurrentVelocity[0] =  -tempSpeedx;

            }

        }else if(striker2CurrentPosition[0] < striker2StartPosition[0])
        {

            if(tempSpeedx > maxSpeed)
            {
                striker2CurrentVelocity[0] = maxSpeed;

            }else if(tempSpeedx < minSpeed)
            {
                striker2CurrentVelocity[0] = minSpeed;

            }else
            {
                striker2CurrentVelocity[0] =  tempSpeedx;
            }

        }else
        {
            striker2CurrentVelocity[0] = 0;
        }

         if(striker2CurrentPosition[2] > striker2StartPosition[2])
        {

            if(tempSpeedz > maxSpeed)
            {
                striker2CurrentVelocity[2] = -maxSpeed;

            }else if(tempSpeedz < minSpeed)
            {
                striker2CurrentVelocity[2] = -minSpeed;

            }else
            {
                striker2CurrentVelocity[2] =  -tempSpeedz;

            }

        }else if(striker2CurrentPosition[2] < striker2StartPosition[2])
        {
            if(tempSpeedz > maxSpeed)
            {
                striker2CurrentVelocity[2] = maxSpeed;

            }else if(tempSpeedz < minSpeed)
            {
                striker2CurrentVelocity[2] = minSpeed;

            }else
            {
                striker2CurrentVelocity[2] =  tempSpeedz;
            }

        }else
        {
            striker2CurrentVelocity[2] = 0;
        }
    }



}

void boundaries2()
{
    // Check collision on left part.
    if(striker2CurrentPosition[0] + striker2CurrentVelocity[0] < tableLeft)
        striker2CurrentPosition[0] = tableLeft;

    if(striker2CurrentPosition[0] == tableLeft)
        striker2CurrentVelocity[0] = minSpeed;

    // Check collision on right part.
    if(striker2CurrentPosition[0] + striker2CurrentVelocity[0] > tableRight)
        striker2CurrentPosition[0] = tableRight;

    if(striker2CurrentPosition[0] == tableRight)
        striker2CurrentVelocity[0] = -minSpeed;

    // Check collision on up part.
    if(striker2CurrentPosition[2] + striker2CurrentVelocity[2] > tableCenterLine )
        striker2CurrentPosition[2] = tableCenterLine;

    if(striker2CurrentPosition[2] == tableCenterLine)
        striker2CurrentVelocity[2] =- minSpeed;

    // Check collision on down part.
    if(striker2CurrentPosition[2] + striker2CurrentVelocity[2] < topGoalFront)
        striker2CurrentPosition[2] = topGoalFront;

    if(striker2CurrentPosition[2] == topGoalFront)
        striker2CurrentVelocity[2] = minSpeed;
}




