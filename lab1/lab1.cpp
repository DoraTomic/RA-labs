#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <GL/freeglut.h>

using namespace std;

GLuint window;
GLuint width = 300, height = 300;

typedef struct {
	float x;
	float y;
	float z;
} iTocka;

typedef struct {
	iTocka Vrh1;
	iTocka Vrh2;
	iTocka Vrh3;
} iPolyElem;

iTocka* tocke;
iPolyElem* elem;
iTocka* tockeS;
iTocka* bSplajn;
iTocka* tangente;
float V[3];
iTocka srediste;

iTocka os;
iTocka s = iTocka{ 0.0, 0.0, 1.0 };
iTocka e = iTocka{ 0.0, 0.0, 0.0 };

int brSeg = 0;
int v = 0;
int f = 0;

void myDisplay();
void myIdle();
void myReshape(int width, int height);
void myMouse(int button, int state, int x, int y);


int main(int argc, char** argv) {

	ifstream file("C:\\IRG\\kocka.txt");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			if (line[0] == 'v') {
				v++;
			}
			else if (line[0] == 'f') {
				f++;
			}
		}
	}

	tocke = new iTocka[v];
	elem = new iPolyElem[f];
	int i = 0;

	ifstream file1("C:\\IRG\\kocka.txt");
	if (file1.is_open()) {
		string line;
		int j = 0;
		while (getline(file1, line)) {
			if (line[0] == 'v') {
				int k = 0;
				istringstream ss(line);
				do {
					string word;
					ss >> word;

					if (k != 0) {
						if (k == 1) {
							tocke[i].x = stof(word)*4;
						}
						else if (k == 2) {
							tocke[i].y = stof(word)*4;
						}
						else if (k == 3) {
							tocke[i].z = stof(word)*4;
						}
					}
					k++;
				} while (ss);
			}
			else if (line[0] == 'f') {
				elem[j].Vrh1.x = ((line[2] - '0') - 1);
				elem[j].Vrh1.y = ((line[2] - '0') - 1);
				elem[j].Vrh1.z = ((line[2] - '0') - 1);
				elem[j].Vrh2.x = ((line[4] - '0') - 1);
				elem[j].Vrh2.y = ((line[4] - '0') - 1);
				elem[j].Vrh2.z = ((line[4] - '0') - 1);
				elem[j].Vrh3.x = ((line[6] - '0') - 1);
				elem[j].Vrh3.y = ((line[6] - '0') - 1);
				elem[j].Vrh3.z = ((line[6] - '0') - 1);
				j++;
			}
			i++;
		}
	}
	//ispitivanje min i max
	iTocka* minmax = new iTocka[2];
	//v--;
	minmax[0].x = tocke[0].x;
	minmax[0].y = tocke[0].y;
	minmax[0].z = tocke[0].z;
	minmax[1].x = tocke[0].x;
	minmax[1].y = tocke[0].y;
	minmax[1].z = tocke[0].z;
	for (int i = 0; i < v; i++) {
		if (minmax[0].x > tocke[i].x && minmax[0].y > tocke[i].y && minmax[0].z > tocke[i].z) {
			minmax[0] = tocke[i];
		}
		if (minmax[1].x < tocke[i].x && minmax[1].y < tocke[i].y && minmax[1].z < tocke[i].z) {
			minmax[1] = tocke[i];
		}
	}
	//cout << "Min koordinate: " << minmax[0].x << " " << minmax[0].y << " " << minmax[0].z << endl;
	//cout << "Max koordinate: " << minmax[1].x << " " << minmax[1].y << " " << minmax[1].z << endl;

	//srediste

	srediste.x = (minmax[1].x + minmax[0].x) / 2;
	srediste.y = (minmax[1].y + minmax[0].y) / 2;
	srediste.z = (minmax[1].z + minmax[0].z) / 2;
	cout << "Srediste tijela: " << srediste.x << " " << srediste.y << " " << srediste.z << endl;

	
	//sredivanje poligona
	for (int i = 0; i < f; i++) {
		int broj1 = elem[i].Vrh1.x;
		int broj2 = elem[i].Vrh2.x;
		int broj3 = elem[i].Vrh3.x;
		elem[i].Vrh1 = tocke[broj1];
		elem[i].Vrh2 = tocke[broj2];
		elem[i].Vrh3 = tocke[broj3];
	}


	//ucitavanje tocaka spirale
	int n = 0;
	ifstream file3("C:\\IRG\\spirala.txt");
	if (file3.is_open()) {
		string line;
		while (getline(file3, line)) {
			if (line[0] == 'v') {
				n++;
			}
		}
	}
	tockeS = new iTocka[n];
	brSeg = n - 3;
	i = 0;

	ifstream file4("C:\\IRG\\spirala.txt");
	if (file4.is_open()) {
		string line;
		while (getline(file4, line)) {
			if (line[0] == 'v') {
				int k = 0;
				istringstream ss(line);
				do {
					string word;
					ss >> word;

					if (k != 0) {
						if (k == 1) {
							tockeS[i].x = stof(word);
						}
						else if (k == 2) {
							tockeS[i].y = stof(word);
						}
						else if (k == 3) {
							tockeS[i].z = stof(word);
						}
					}
					k++;
				} while (ss);
			}
			i++;
		}
	}
	
	bSplajn = new iTocka[100 * brSeg];
	tangente = new iTocka[100 * 2 * brSeg];
	int brTangenta = 0;

	for (int i = 0; i < brSeg; i++) {
		iTocka v0 = tockeS[i];
		iTocka v1 = tockeS[i + 1];
		iTocka v2 = tockeS[i + 2];
		iTocka v3 = tockeS[i + 3];

		for (int t = 0; t < 100; t++) {
			double j = t / 100.0;
			float pom1 = (-pow(j, 3.0) + 3 * pow(j, 2.0) - 3 * j + 1) / 6.0;
			float pom2 = (3 * pow(j, 3.0) - 6 * pow(j, 2.0) + 4) / 6.0;
			float pom3 = (-3 * pow(j, 3.0) + 3 * pow(j, 2.0) + 3 * j + 1) / 6.0;
			float pom4 = pow(j, 3.0) / 6.0;

			bSplajn[100 * i + t].x = pom1 * v0.x + pom2 * v1.x + pom3 * v2.x + pom4 * v3.x;
			bSplajn[100 * i + t].y = pom1 * v0.y + pom2 * v1.y + pom3 * v2.y + pom4 * v3.y;
			bSplajn[100 * i + t].z = pom1 * v0.z + pom2 * v1.z + pom3 * v2.z + pom4 * v3.z;

			//tangente
			float t1 = 0.5 * (-pow(j, 2.0) + 2 * j - 1);
			float t2 = 0.5 * (3 * pow(j, 2.0) - 4 * j);
			float t3 = 0.5 * (-3 * pow(j, 2.0) + 2 * j + 1);
			float t4 = 0.5 * (pow(j, 2.0));

			tangente[brTangenta].x = pom1 * v0.x + pom2 * v1.x + pom3 * v2.x + pom4 * v3.x;
			tangente[brTangenta].y = pom1 * v0.y + pom2 * v1.y + pom3 * v2.y + pom4 * v3.y;
			tangente[brTangenta].z = pom1 * v0.z + pom2 * v1.z + pom3 * v2.z + pom4 * v3.z;
			brTangenta++;

			float vx = t1 * v0.x + t2 * v1.x + t3 * v2.x + t4 * v3.x;
			float vy = t1 * v0.y + t2 * v1.y + t3 * v2.y + t4 * v3.y;
			float vz = t1 * v0.z + t2 * v1.z + t3 * v2.z + t4 * v3.z;

			tangente[brTangenta].x = tangente[brTangenta - 1].x + vx;// 3;
			tangente[brTangenta].y = tangente[brTangenta - 1].y + vy;// 3;
			tangente[brTangenta].z = tangente[brTangenta - 1].z + vz;// 3;
			brTangenta++;
		}


	}

	//pozicioniranje u ishodiste
	/*for (int i = 0; i < v; i++) {
		tocke[i].x = tocke[i].x - srediste.x;
		tocke[i].y = tocke[i].y - srediste.y;
		tocke[i].z = tocke[i].z - srediste.z;
	}*/


	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	window = glutCreateWindow("Glut OpenGL Linija");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutIdleFunc(myIdle);

	glutMainLoop();
	return 0;
}

int t = 0;

void myDisplay()
{
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(-5.0, -5.0, -75.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 100 * brSeg; i++) {
		glVertex3f(bSplajn[i].x, bSplajn[i].y, bSplajn[i].z);
	}
	glEnd();
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(tangente[2 * t].x, tangente[2 * t].y, tangente[2 * t].z);
	glVertex3f(tangente[2 * t + 1].x, tangente[2 * t + 1].y, tangente[2 * t + 1].z);
	glEnd();

	glTranslatef(bSplajn[t].x, bSplajn[t].y, bSplajn[t].z);

	//os
	e.x = tangente[2 * t + 1].x - tangente[2 * t].x;
	e.y = tangente[2 * t + 1].y - tangente[2 * t].y;
	e.z = tangente[2 * t + 1].z - tangente[2 * t].z;

	os.x = s.y * e.z - e.y * s.z;
	os.y = e.x * s.z - s.x * e.z;
	os.z = s.x * e.y - s.y * e.x;

	double apsS = pow(pow((double)s.x, 2.0) + pow((double)s.y, 2.0) + pow((double)s.z, 2.0), 0.5);
	double apsE = pow(pow((double)e.x, 2.0) + pow((double)e.y, 2.0) + pow((double)e.z, 2.0), 0.5);
	double se = s.x * e.x + s.y * e.y + s.z * e.z;
	double kut = acos(se / (apsS * apsE));
	kut = kut / (2 * 3.14159265) * 360;
	glRotatef(kut, os.x, os.y, os.z);
	glTranslatef(-srediste.x, -srediste.y, -srediste.z);
	
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for (int i = 0; i < f; i++) {
		iTocka v1 = elem[i].Vrh1;
		iTocka v2 = elem[i].Vrh2;
		iTocka v3 = elem[i].Vrh3;
		
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);

		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);

		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v1.x, v1.y, v1.z);
	}
	

	glColor3f(0.0, 0.0, 0.0);
	glEnd();
	t++;
	if (t == 100 * brSeg) t = 0;

	glFlush();
}

int currentTime = 0;
int previousTime = 0;

void myIdle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;
	//printf("%d\n", timeInterval);
	if (timeInterval > 10) {
		myDisplay();
		previousTime = currentTime;
	}
}

void myReshape(int w, int h) {
	width = w; height = h;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);		//	matrica projekcije
	glLoadIdentity();					//	jedinicna matrica
	//gluOrtho2D(0, width, 0, height); 	//	okomita projekcija
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);			//	matrica pogleda
	glLoadIdentity();
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // boja pozadine
	glClear(GL_COLOR_BUFFER_BIT);		//	brisanje pozadine
	glPointSize(1.0);
	glColor3f(0.0f, 0.0f, 0.0f);
}

void myMouse(int button, int state, int x, int y) {

}