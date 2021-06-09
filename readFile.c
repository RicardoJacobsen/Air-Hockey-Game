#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h> // to get the file size

#include "readFile.h"


void readOFFFile(char *Fname, Object3D *object)
{
    int intdump;
    char strdump[100];
    char buffer[100];
    bool flag= true;
    int posv=0;
    int posf=0;
    int posvt=0;
    int posvn=0;


    FILE *myFile;
    myFile = fopen(Fname, "r");

    struct stat st;
    stat(Fname, &st);
    long fileSize = st.st_size;

    object->vertexBuffer = (float*) malloc (fileSize);							// Allocate memory for the verteces
    object->Faces_Triangles = (float*) malloc(fileSize*sizeof(float));			// Allocate memory for the triangles
    object->normals  = (float*) malloc(fileSize*sizeof(float));					// Allocate memory for the object->normals
    int triangle_index = 0;												// Set triangle index to zero
	int normal_index = 0;

    while(flag==true)
    {
    fscanf(myFile,"%s",&strdump);
    if(strcmp(strdump,"v")==0)
    {
        float x,y,z;
        fscanf(myFile,"%f %f %f",&x,&y,&z);
        object->vertexBuffer[object->TotalConnectedPoints]=x;
        object->vertexBuffer[object->TotalConnectedPoints + 1]=y;
        object->vertexBuffer[object->TotalConnectedPoints + 2]=z;
        object->TotalConnectedPoints += POINTS_PER_VERTEX;
        posv++;
    }
    else if(strcmp(strdump,"f")==0)
       {
            int vertexNumber[3] = { 0, 0, 0 };
            int x1, x2, x3, y1, y2, y3, z1, z2, z3;
            fscanf(myFile,"%s",&strdump);
            sscanf(&strdump,"%d/%d/%d",&x1,&y1,&z1);

            fscanf(myFile,"%s",&strdump);
            sscanf(&strdump,"%d/%d/%d",&x2,&y2,&z2);

            fscanf(myFile,"%s",&strdump);
            sscanf(&strdump,"%d/%d/%d",&x3,&y3,&z3);

            // Read integers from the line:  f 1 2 3
            vertexNumber[0]=x1;				// First point of our triangle. This is an
			vertexNumber[1]=x2;				// pointer to our object->vertexBuffer list
			vertexNumber[2]=x3;				// each point represents an X,Y,Z.

            vertexNumber[0] -= 1;				// OBJ file starts counting from 1
            vertexNumber[1] -= 1;				// OBJ file starts counting from 1
            vertexNumber[2] -= 1;				// OBJ file starts counting from 1

            /********************************************************************
			* Create triangles (f 1 2 3) from points: (v X Y Z) (v X Y Z) (v X Y Z).
			 The object->vertexBuffer contains all verteces
			* The triangles will be created using the verteces we read previously
			*/

			int tCounter = 0;
			for (int i = 0; i < POINTS_PER_VERTEX; i++)
			{
                object->Faces_Triangles[triangle_index + tCounter] = object->vertexBuffer[3 * vertexNumber[i]];
				object->Faces_Triangles[triangle_index + tCounter + 1] = object->vertexBuffer[3 * vertexNumber[i] + 1];
				object->Faces_Triangles[triangle_index + tCounter + 2] = object->vertexBuffer[3 * vertexNumber[i] + 2];
				tCounter += POINTS_PER_VERTEX;
            }

			/*********************************************************************
			 * Calculate all object->normals, used for lighting
			 */

			float coord1[3] = { object->Faces_Triangles[triangle_index], object->Faces_Triangles[triangle_index + 1],object->Faces_Triangles[triangle_index + 2] };
			float coord2[3] = { object->Faces_Triangles[triangle_index + 3],object->Faces_Triangles[triangle_index + 4],object->Faces_Triangles[triangle_index + 5] };
			float coord3[3] = { object->Faces_Triangles[triangle_index + 6],object->Faces_Triangles[triangle_index + 7],object->Faces_Triangles[triangle_index + 8] };

            /* calculate Vector1 and Vector2 */
            float va[3], vb[3], vr[3], val;
            va[0] = coord1[0] - coord2[0];
            va[1] = coord1[1] - coord2[1];
            va[2] = coord1[2] - coord2[2];

            vb[0] = coord1[0] - coord3[0];
            vb[1] = coord1[1] - coord3[1];
            vb[2] = coord1[2] - coord3[2];

            /* cross product */
            vr[0] = va[1] * vb[2] - vb[1] * va[2];
            vr[1] = vb[0] * va[2] - va[0] * vb[2];
            vr[2] = va[0] * vb[1] - vb[0] * va[1];

            /* normalization factor */
            val = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

            float norm[3];
            norm[0] = vr[0] / val;
            norm[1] = vr[1] / val;
            norm[2] = vr[2] / val;


			tCounter = 0;
			for (int i = 0; i < POINTS_PER_VERTEX; i++)
			{
                object->normals[normal_index + tCounter] = norm[0];
                object->normals[normal_index + tCounter + 1] = norm[1];
                object->normals[normal_index + tCounter + 2] = norm[2];
                tCounter += POINTS_PER_VERTEX;
            }

				triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
				normal_index += TOTAL_FLOATS_IN_TRIANGLE;
				object->TotalConnectedTriangles += TOTAL_FLOATS_IN_TRIANGLE;

            posf++;
            }

            //calculateRad(object);

            else
            {
                fscanf(myFile, "%[^\n]\n", buffer);
            }
        if(strcmp(strdump,"EOF")==0){
            //free(object->vertexBuffer);
            //free(object->Faces_Triangles);
            //free(object->normals);
            //printf("Nubmer of vertices read: %d\n",object->TotalConnectedPoints);
            //printf("Nubmer of faces read: %d\n",posf);
            //printf("Reached the EOF %s\n",strdump);
            break;
        }
    }//end of while
    fclose(myFile);

} //end of readOFFFile
