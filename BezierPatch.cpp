#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include  "trackball.h"
#include <iostream>
#include <fstream>
using namespace std;

//GLfloat CtrlPoints[4][2] = { {200., 200.}, {200., 300.},{300., 300.}, {300., 200.}};

GLfloat BezMatrix[4][4] = {
        { -1.0,  3.0, -3.0, 1.0},
        {  3.0, -6.0,  3.0, 0.0},       
        { -3.0,  3.0,  0.0, 0.0},
        {  1.0,  0.0,  0.0, 0.0}};
GLfloat BP_X1[32][4][4]; //the bezier points after multi with bez matrix and Ctrl points
GLfloat BP_Y1[32][4][4];
GLfloat BP_Z1[32][4][4];

GLfloat BP_X2[32][4][4]; // the bezier points after multi with BP_1 and transposed bez matrix_
GLfloat BP_Y2[32][4][4];
GLfloat BP_Z2[32][4][4];

GLfloat CtrlPointsX[32][4][4];//actual control poits
GLfloat CtrlPointsY[32][4][4];
GLfloat CtrlPointsZ[32][4][4];

int in_box = -1;
int off_x, off_y, off_z;
//multiply bez matrix * control points matrix * bez matrix 
void init()
{
    int q=0, i=0, j=0, k=0;
    for(q=0; q<32; q++){
    for(i=0; i<4; i++)
        for(j=0; j<4; j++) {
            BP_X1[q][i][j] = 0.0;
            BP_Y1[q][i][j] = 0.0;
            BP_Z1[q][i][j] = 0.0;
            for(k=0; k < 4; k++) {
                BP_X1[q][i][j] += BezMatrix[i][k] * CtrlPointsX[q][j][k];
                BP_Y1[q][i][j] += BezMatrix[i][k] * CtrlPointsY[q][j][k];
                BP_Z1[q][i][j] += BezMatrix[i][k] * CtrlPointsZ[q][j][k];
            }
//	printf("bp_1 %8.4f %8.4f %8.4f\n ",BP_X1[x][i][j], BP_Y1[x][i][j], BP_Z1[x][i][j] );
        }
//	printf("\n %d \n", x);
}

    for(q=0; q<32; q++){
    for(i=0; i<4; i++)
        for(j=0; j<4; j++) {
            BP_X2[q][i][j] = 0.0;
            BP_Y2[q][i][j] = 0.0;
            BP_Z2[q][i][j] = 0.0;
            for(k=0; k < 4; k++) {
                BP_X2[q][i][j] += BP_X1[q][i][k]* BezMatrix[j][k];
                BP_Y2[q][i][j] += BP_Y1[q][i][k]* BezMatrix[j][k];
                BP_Z2[q][i][j] += BP_Z1[q][i][k]* BezMatrix[j][k];
            }
//	printf("bp_2 %8.4f %8.4f %8.4f\n ",BP_X2[x][i][j], BP_Y2[x][i][j], BP_Z2[x][i][j] );
        }
//	printf("\n\n");
}

}
//it draws the control points 
void draw_Ctrl_Points()
{
    int i,k,j;
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    glPointSize(4.0);
    for(k=0; k<32; k++){
    for(i=0; i<4; i++)
    for(j=0; j<4; j++)
        glVertex3f(CtrlPointsX[k][i][j], CtrlPointsY[k][i][j], CtrlPointsZ[k][i][j]);
}
    glEnd();
}
//draws the bezier curve
void draw_curve()
{
    float u, v;
    int i,j,k,q, uCurrent, vCurrent; 
    GLfloat xx[32];//final bezier points after multiplied with u and v
    GLfloat yy[32];
    GLfloat zz[32];
    GLfloat xInit[32][4];//bezier points after multiplied with u
    GLfloat yInit[32][4];
    GLfloat zInit[32][4];
    GLfloat uu[4];
    GLfloat vv[4];
    GLfloat xArray[32][101][101];//array of all the bezier points
    GLfloat yArray[32][101][101];
    GLfloat zArray[32][101][101];
    GLfloat lastNumX =0, lastNumY =0, lastNumZ = 0;//last bezier points of each patch
    GLfloat lastX[4], lastY[4], lastZ[4];//last bezier points before multiply with u and v
    uCurrent = 0; //tracking the u and v value 
    vCurrent = 0;
   // q = 0;
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_LINE_STRIP);
    //glPointSize(2.0);
    for(u=0.0; u<1.0; u+=0.01) {
 	uu[3] = 1.;
	uu[2] = u;
        uu[1] = uu[2]*u;
        uu[0] = uu[1]*u;
    for(v=0.0; v<1.0; v+=0.01) {
 	vv[3] = 1.;
	vv[2] = v;
	vv[1] = vv[2]*v;
	vv[0] = vv[1]*v;
	for(q=0; q<32; q++){
        for(i=0; i < 4; i++){
	 xInit[q][i] = 0.0;	
	 yInit[q][i] = 0.0;
	 zInit[q][i] = 0.0;
	for(k=0; k<4; k++){ // multiply BP_2 with u value
           xInit[q][i] += uu[k] * BP_X2[q][k][i];
           yInit[q][i] += uu[k] * BP_Y2[q][k][i];
           zInit[q][i] += uu[k] * BP_Z2[q][k][i];
//	printf("%f\n", BP_X2[x][k][j]);
	}}}
/*	xx[0] = lastNumX; //set the start bezier point to last patch's last bezier points
	yy[0] = lastNumY;
	zz[0] = lastNumZ;
  */	for(q=1; q<32; q++){ // multiple []Init with v value
	 xx[q] = 0;
	 yy[q] = 0;
	 zz[q] = 0;
	for(k=0; k<4; k++){
           xx[q] += xInit[q][k] * vv[k];
           yy[q] += yInit[q][k] * vv[k];
           zz[q] += zInit[q][k] * vv[k];
//	printf("%8.4f \t", xx);
} }
/*	lastNumX = xx[32];
	lastNumY = yy[32];
	lastNumZ = zz[32];
*/	
	for(q = 0; q < 32; q++){
	    if(q != 0){
		 xArray[q][0][0] = xArray[q-1][100][100];
		 yArray[q][0][0] = yArray[q-1][100][100];
		 zArray[q][0][0] = zArray[q-1][100][100];
	     }
		xArray[q][uCurrent][vCurrent] = xx[q];
	     yArray[q][uCurrent][vCurrent] = yy[q];
	     zArray[q][uCurrent][vCurrent] = zz[q];
//	printf("%d  %10.6f %10.6f %10.6f\n",q,  xx[q],yy[q],zz[q]);
}
vCurrent = (vCurrent++ % 101);
}uCurrent = (uCurrent++ % 101);
}
//actual code for drawing the bezier curve
	for(q =0; q < 32; q++){
	for(i=0; i <101; i++){
	for(j=0;j < 101; j++){
	//	printf("%8.4f %8.4f %8.4f\n", xArray[q][i][j], yArray[q][i][j], zArray[q][i][j]);
	glVertex3f(xArray[q][i][j], yArray[q][i][j], zArray[q][i][j]);
}}}
    glEnd();
}
void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);
    draw_Ctrl_Points();

    draw_curve();

    glFlush();
}

void reshape(int w, int h)
{
glViewport(0, 0, (GLsizei) w, (GLsizei) h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
if (w <= h)
glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
else
glOrtho(-5.0*(GLfloat)w/(GLfloat)h,
5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
glRotatef(-90,1,0,0);
//glScalef(2,2,2);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
}
/*
void mouse(int button, int state, int x, int y, int z)
{
    int i, j, q;
   switch (button) {
      case GLUT_LEFT_BUTTON:
                
        if (state == GLUT_UP) {

            if(in_box != -1) {
                CtrlPointsX[q][in_box][j] = (float)(x-off_x);
                CtrlPointsY[q][in_box][j] = (float)(y-off_y);
                CtrlPointsZ[q][in_box][j] = (float)(z-off_z);
		init();
            }
            in_box = -1;
        
        }
        if (state == GLUT_DOWN) {
printf("x=%d, y=%d z=%d \n", x, y, z);
            in_box = -1;  
	for(q=0; q<32; q++){
	for(i=0; i<4; i++) {
	for(j=0; j<4; j++){
                if( (x > (int)(CtrlPointsX[q][i][j] - 15.))
		 && (y > (int)(CtrlPointsY[q][i][j] - 15.)) 
		 && (z > (int)(CtrlPointsZ[q][i][j] - 15.))
                 && (x < (int)(CtrlPointsX[q][i][j] + 15.))
		 && (y < (int)(CtrlPointsY[q][i][j] + 15.))
		 && (z < (int)(CtrlPointsZ[q][i][j] + 15.)))
            {
                in_box = i;

                off_x = x - (int)(CtrlPointsX[q][i][j]);
                off_y = y - (int)(CtrlPointsY[q][i][j]);
		off_z = z - (int)(CtrlPointsZ[q][i][j]);
printf("inside_box %d!\n", in_box);
printf("%d %d %d %d %d %d \n",  (int)(CtrlPointsX[q][i][j] -5.), 
                          (int)(CtrlPointsY[q][i][j] - 5.),
                          (int)(CtrlPointsZ[q][i][j] - 5.),
                          (int)(CtrlPointsX[q][i][j] + 5.), 
                          (int)(CtrlPointsY[q][i][j] + 5.),
                          (int)(CtrlPointsZ[q][i][j] + 5.));
                break;
             }
            }}} 
         }
         break;
      default:
         break;
   }

   glFlush();
   display();
}

*/
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

int main(int argc, char **argv)
{//read the file and get the control points from it	
	FILE *file;
	int NumOfBezier, i, j,k,q, current;	
	float numbers[1601];	
	file= fopen("./data/teapotCGA.bpt", "r");
	if(file == NULL){
		printf("Error\n");
		exit(1);
		}
	fscanf(file, "%d", &NumOfBezier);
	for(i=0; i<1601; i++)
{
		fscanf(file,"%f", &numbers[i]);
//		printf("%8.4f n", &numbers[i]);
}	for(q=0; q<NumOfBezier;q++){
	//	printf("\n x: %d \n\n",x );
		current+=2;
		for(j=0; j<4; j++){
		for(k=0; k<4; k++){
			CtrlPointsX[q][j][k] = numbers[current++];
			CtrlPointsY[q][j][k] = numbers[current++];
			CtrlPointsZ[q][j][k] = numbers[current++];
//	printf("%8.4f %8.4f %8.4f\n", CtrlPointsX[q][j][k], CtrlPointsY[q][j][k], CtrlPointsZ[q][j][k]);
		}
		}//printf("\n q: %f \n\n",q );
	}
 fclose(file);
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
//   glutMouseFunc(mouse);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0;
 
}
