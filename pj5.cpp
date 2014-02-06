#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>

using std::stringstream;
using std::string;
using namespace std;

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif
//#include "readX.h"

//GLfloat camX=0, camY=25, camZ=78;
GLfloat camX=0, camY=0, camZ=500;
GLfloat focX = 0.0, focY = 0, focZ = 0, angleX = 0.0, angleY = 0.0, angleZ = 0.0;

GLfloat lightpos[4] = { 0.0, 3.0, 2.0, 0.0 };
GLfloat lightamb[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightdif[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lmodel_ambient[]={0.4, 0.4, 0.4, 1.0};
GLfloat local_view[]={0.0};




void init(void){
  glShadeModel (GL_SMOOTH);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();
    glClearColor(0, 0, 0, 1.0); 
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdif);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
ifstream theFile ("region_s3.data");
if(!theFile){
	cerr << "Error: Open file error!" << endl;
	exit(1);
}else{
cout << "Loading file: " << "region_s3.data" << endl;
float segNum, vrtNum;
int i, j;
theFile >> segNum;
cout << segNum << endl;
for( i = 0; i < segNum; i++){
	theFile >> vrtNum;
	//cout << vrtNum << endl;
	glColor3f(1, 0, 1);
	glBegin(GL_LINE_STRIP);

	for( j =0; j < vrtNum; j++){
		float x, y, z, a1, a2, a3;
		theFile >> x >> y >>  z >> a1 >> a2 >> a3;
		//glColor4f(a1, a2, a3, 1);
		glVertex3f(x, y, z);
		//cout <<"X: "<< x << " Y: "<<  y <<" Z: "<< z <<" A1: "<< a1 <<" A2: "<< a2 <<" A3: "<< a3 << endl;
	}
	glEnd();
}
cout << "end" << endl;
}
  

}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}
    switch (key)
    {
	case 27: exit(0);
	case 'w': 
	    camX +=0.5;
		break;
	case 's': 
	    camX -=0.5 ;
		break;
	case 'd': 
	    camZ -=0.5;
		break;
	case 'a': 
	    camZ +=0.5;
		break;
	case 27:
         exit(0);
         break;
    default: 
	    return;
    }
}

void Reshape(int w, int h)
{
glViewport(0, 0, (GLsizei) w, (GLsizei) h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
if (w <= h)
glOrtho(-200, 200, -200*(GLfloat)h/(GLfloat)w, 200*(GLfloat)h/(GLfloat)w, -200, 200);
else
glOrtho(-200*(GLfloat)w/(GLfloat)h, 200*(GLfloat)w/(GLfloat)h, -200, 200, -200, 200);
//glRotatef(-90,1,0,0);
//glScalef(2,2,2);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
}
void Display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   init();
  //  draw_Ctrl_Points();

   // draw_curve();

    glFlush();
}

int main(int argc, char **argv){
 glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("color");
 //   init();

    glutDisplayFunc(display);
   glutReshapeFunc(reshape);
//   glutMouseFunc(mouse);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
	return 0;
}