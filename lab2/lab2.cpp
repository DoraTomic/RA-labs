
#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

GLuint window;
GLuint sub_width = 512, sub_height = 512;

typedef struct {
	float x, y, z;
	int q;
	float cR, cG, cB;
	double size;
} Izvor;

typedef struct {
	float x;
	float y;
	float z;
} Vrh;

typedef struct {
	float x, y, z;
	float r, g, b;
	float v;
	int t;
	float sX, sY, sZ;
	float osX, osY, osZ;
	double kut; 
	double size;
} Cestica;

vector<Cestica> cestice;
Izvor i;

double maxH = 42.0;
double pi = 3.14159;
Vrh ociste = Vrh{ 0.0, 0.0, 65.0 };

int t = 0;
int currentTime = 0;
int previousTime = 0;


void myDisplay			();
void myIdle				();
void myReshape			(int width, int height);
void myKeyboard			(unsigned char theKey, int mouseX, int mouseY);

void nacrtajCestice		();
void nacrtajCesticu		(Cestica c);


int main(int argc, char ** argv){

    i.x = 0.0; i.y = 0.0; i.z = 0.0;
    i.q = 5;
    i.cB = 1.0; i.cG = 0.0; i.cR = 1.0;
    i.size = 0.2;
    srand (time(NULL));
  


    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(sub_width,sub_height);
	glutInitWindowPosition(100,100);
	glutInit(&argc, argv);

	window = glutCreateWindow("2. laboratorijska vjezba iz Racunalne Grafike");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glEnable(GL_BLEND);

	glutMainLoop();
    return 0;
}

void myDisplay()
{

 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslatef(ociste.x, ociste.y, -ociste.z);

	nacrtajCestice();

	glutSwapBuffers();
}

void myIdle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;
	if(timeInterval > 100) {
		
		// Stvori nove cestice
		int n = rand() % i.q + 1;
		for (int j = 0; j < n; j++) {
			double x, y, z, norm;
			x = (rand()%2000- (double)1000) / (double)1000;
			y = (rand()%2000- (double)1000) / (double)1000;
			z = (rand()%2000- (double)1000) / (double)1000;
			norm = pow(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0), 0.5);
			x /= norm; y /= norm; z /= norm;
			Cestica c;
			c.x = i.x; c.z = i.z; c.y = i.y; 
			c.r = i.cR; c.g = i.cG; c.b = i.cB; 
			c.v = 0.4;
			c.sX = x; c.sY = y; c.sZ = z;
			c.t = 80;
			c.size = i.size;
			cestice.push_back(c);
		}
		
		// Pomakni cestice
		for (int j =  cestice.size() - 1; j >= 0; j--) {

			//
			Vrh s = Vrh{ 0.0, 0.0, 1.0 };
			Vrh os = Vrh{ 0.0, 0.0, 0.0};
			Vrh e = Vrh{ 0.0, 0.0, 0.0 };
			
			e.x = cestice.at(j).x - ociste.x;
			e.y = cestice.at(j).y - ociste.y;
			e.z = cestice.at(j).z - ociste.z;

			os.x = s.y*e.z - e.y*s.z;
			os.y = e.x*s.z - s.x*e.z;
			os.z = s.x*e.y - s.y*e.x;

			double apsS = pow(pow((double)s.x, 2.0) + pow((double)s.y, 2.0) + pow((double)s.z, 2.0), 0.5);
			double apsE = pow(pow((double)e.x, 2.0) + pow((double)e.y, 2.0) + pow((double)e.z, 2.0), 0.5);
			double se = s.x*e.x + s.y*e.y + s.z*e.z;
			double kut = acos(se/(apsS*apsE));
			cestice.at(j).kut = kut / (2*pi) * 360;
			cestice.at(j).osX = os.x; cestice.at(j).osY = os.y; cestice.at(j).osZ = os.z;

			// 
			cestice.at(j).y += cestice.at(j).v * cestice.at(j).sY;
			cestice.at(j).z += cestice.at(j).v * cestice.at(j).sZ;
			cestice.at(j).x += cestice.at(j).v * cestice.at(j).sX;

			cestice.at(j).t--;

			if (cestice.at(j).r > 0) cestice.at(j).r -= 0.008;
			if (cestice.at(j).b < 1.0) cestice.at(j).b += 0.008;
			if (cestice.at(j).t <= 0) {
				cestice.erase(cestice.begin()+j);
			}
		}

		myDisplay();
		previousTime = currentTime;
	}
}

void nacrtajCestice() {
	for (int j = 0; j < cestice.size(); j++) {
		nacrtajCesticu(cestice.at(j));
	}
}

void nacrtajCesticu(Cestica c) {


	glTranslatef(c.x, c.y, c.z);
	glRotatef(c.kut, c.osX, c.osY, c.osZ);
	glColor3f(c.r, c.g, c.b);

	glBegin(GL_QUADS);
	glVertex3f(-c.size, -c.size, 0.0);
	glVertex3f(-c.size, +c.size, 0.0);
	glVertex3f(+c.size, +c.size, 0.0);
	glVertex3f(+c.size, -c.size, 0.0);
	glEnd();

	glRotatef(-c.kut, c.osX, c.osY, c.osZ);
	glTranslatef(-c.x, -c.y, -c.z);
}

void myKeyboard(unsigned char theKey, int m, int n) {
	if(theKey == 'a') i.x -= 0.5;
	if(theKey == 'd') i.x += 0.5;
	if(theKey == 'w') i.z -= 0.5;
	if(theKey == 's') i.z += 0.5;
	if(theKey == 'y') i.y -= 0.5;
	if(theKey == 'x') i.y += 0.5;

	if(theKey == 'q' && i.cG > 0.0) i.cG -= 0.02;
	if(theKey == 'e' && i.cG < 1.0) i.cG += 0.02;

	if(theKey == 'i') ociste.y += 0.1;
	if(theKey == 'k') ociste.y -= 0.1;
	if(theKey == 'j') ociste.x -= 0.1;
	if(theKey == 'l') ociste.x += 0.1;
}

void myReshape(int width, int height)
{
	width = width;
	height = height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 150.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1.0);
	glColor3f(0.0f, 0.0f, 0.0f);
}
