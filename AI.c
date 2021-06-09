#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gl/freeglut.h>

#include "AI.h"

extern GLfloat puckCurrentPosition[3], puckCurrentVelocity[3];

extern GLfloat striker2CurrentPosition[3], striker2CurrentVelocity[3],
striker2PreviousPosition[3], striker2PreviousVelocity[3], striker2StartPosition[3];

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

static float tableLeft = -47.199852, tableRight =47.099854, tableUp = -94.900078, tableDown = 93.0000, tableCenterLine = 0;

static float bottomGoalFront = 85.000, GoalRight = 24.000, GoalLeft = -24.000, topGoalFront = -88.900;

//temporary radius
static float strikerRad = 8.4;
static float puckRadius = 5.8;

static float strikerMass = 0.5;
static float puckMass = 0.2;

extern float Friction = -0.01;
extern float maxSpeed = 5;
extern float minSpeed2 = 1.5;
extern float minSpeed = 0.1;

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

