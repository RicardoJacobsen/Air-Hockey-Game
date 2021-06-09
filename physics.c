#include <stdio.h>
#include <gl/freeglut.h>
#include <stdbool.h>
#include <math.h>

#include "physics.h"

static float defaultSpeed = 2.0;

static float strikerSpeedLeft = 2.0;
static float strikerSpeedRight = 2.0;
static float strikerSpeedTop = 2.0;
static float strikerSpeedBottom = 2.0;

extern bool flag_goal;

//AI zone variables

// Variables for puck animation! Current Position will be equal to initial at the beginning

extern GLfloat puckCurrentPosition[3], puckStartPosition[3], puckPreviousPosition[3],
puckCurrentVelocity[3], puckPreviousVelocity[3];

extern GLfloat puckDeltaT;

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
extern GLfloat strikerCurrentPosition[3], strikerStartPosition[3], strikerCurrentVelocity[3] = {2.0 ,0 , 2.0};

//position of AI striker
extern GLfloat striker2CurrentPosition[3], striker2PreviousPosition[3], striker2CurrentVelocity[3] = {1.0,0.0,0.0},
striker2PreviousVelocity[3], striker2StartPosition[3];

//setting the boundaries variables for the game/ table
static float tableLeft = -47.199852, tableRight =47.099854, tableUp = -94.900078, tableDown = 93.0000, tableCenterLine = 0;

static float bottomGoalFront = 85.000, GoalRight = 24.000, GoalLeft = -24.000, topGoalFront = -88.900;

//temporary radius
static float strikerRad = 8.4;
static float puckRadius = 5.8;

static float strikerMass = 0.5;
static float puckMass = 0.2;

extern float maxSpeed = 5, minSpeed2 = 1.5, minSpeed = 0.1;

void boundaries1()
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
                striker2CurrentVelocity[0] = -maxSpeed;
            else if(tempSpeedx < minSpeed)
                    striker2CurrentVelocity[0] = -minSpeed2;
                else
                    striker2CurrentVelocity[0] =  -tempSpeedx;

        }
        else if( striker2CurrentPosition[0] < puckCurrentPosition[0] + puckCurrentVelocity[0] + puckRadius )
        {

            if(tempSpeedx > maxSpeed)
                striker2CurrentVelocity[0] = maxSpeed;
            else if(tempSpeedx < minSpeed)
                    striker2CurrentVelocity[0] = minSpeed2;
                else
                    striker2CurrentVelocity[0] =  tempSpeedx;
        }
        else
            striker2CurrentVelocity[0] = 0;

        if(striker2CurrentPosition[2] > puckCurrentPosition[2] + puckCurrentVelocity[2] + puckRadius )
        {

            if(tempSpeedz > maxSpeed)
                striker2CurrentVelocity[2] = -maxSpeed;
            else if(tempSpeedx < minSpeed)
                    striker2CurrentVelocity[2] = -minSpeed2;
                else
                    striker2CurrentVelocity[2] =  -tempSpeedx;

        }
        else if(striker2CurrentPosition[2] < puckCurrentPosition[2] + puckCurrentVelocity[2] - puckRadius )
        {
            if(tempSpeedz > maxSpeed)
                striker2CurrentVelocity[2] = maxSpeed;
            else if(tempSpeedx < minSpeed)
                    striker2CurrentVelocity[2] = minSpeed2;
                else
                    striker2CurrentVelocity[2] =  tempSpeedx;
        }
        else
            striker2CurrentVelocity[2] = 0;
    }
    else
    {
        // have striker move back to default position
        distanceToStart[0] = striker2CurrentPosition[0] - striker2StartPosition[0];
        distanceToStart[2] = striker2CurrentPosition[2] - striker2StartPosition[2];

        tempSpeedx = distanceToStart[0]*ratio;
        tempSpeedz = distanceToStart[2]*ratio;

        if(striker2CurrentPosition[0] > striker2StartPosition[0])
        {

            if(tempSpeedx > maxSpeed)
                striker2CurrentVelocity[0] = -maxSpeed;
            else if(tempSpeedx < minSpeed)
                    striker2CurrentVelocity[0] = -minSpeed;
                else
                    striker2CurrentVelocity[0] =  -tempSpeedx;

        }
        else if(striker2CurrentPosition[0] < striker2StartPosition[0])
        {
            if(tempSpeedx > maxSpeed)
                striker2CurrentVelocity[0] = maxSpeed;
            else if(tempSpeedx < minSpeed)
                    striker2CurrentVelocity[0] = minSpeed;
                else
                striker2CurrentVelocity[0] =  tempSpeedx;

        }
        else
            striker2CurrentVelocity[0] = 0;

        if(striker2CurrentPosition[2] > striker2StartPosition[2])
        {

            if(tempSpeedz > maxSpeed)
                striker2CurrentVelocity[2] = -maxSpeed;
            else if(tempSpeedz < minSpeed)
                    striker2CurrentVelocity[2] = -minSpeed;
                else
                    striker2CurrentVelocity[2] =  -tempSpeedz;

        }
        else if(striker2CurrentPosition[2] < striker2StartPosition[2])
        {
            if(tempSpeedz > maxSpeed)
                striker2CurrentVelocity[2] = maxSpeed;
            else if(tempSpeedz < minSpeed)
                    striker2CurrentVelocity[2] = minSpeed;
            else
                striker2CurrentVelocity[2] =  tempSpeedz;
        }
        else
            striker2CurrentVelocity[2] = 0;
    }

    striker2CurrentPosition[0] = striker2PreviousPosition[0] + striker2CurrentVelocity[0];
    striker2CurrentPosition[2] = striker2PreviousPosition[2] + striker2CurrentVelocity[2];

    striker2PreviousPosition[0] = striker2CurrentPosition[0];
    striker2PreviousPosition[2] = striker2CurrentPosition[2];

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

    striker2CurrentVelocity[0] = 0;
    striker2CurrentVelocity[1] = 0;
    striker2CurrentVelocity[2] = 0;

    puckCurrentVelocity[0] = (rand() % (1 + 1 - -1) + -1);
    puckCurrentVelocity[1] = 0;
	puckCurrentVelocity[2] = (rand() % (1 + 1 - -1) + -1);

}
