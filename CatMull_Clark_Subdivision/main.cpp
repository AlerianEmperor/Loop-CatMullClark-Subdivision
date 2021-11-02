#include <Windows.h>
#include "vec3.h"
#include <gl\GLU.h>
#include <gl\glut.h>
#include <gl\GL.h>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <math.h>
#include "Loop_Subdivision.h"
#include "CatMull_Clark_Subdivision.h"

#define kWindowWidth 800
#define kWindowHeight 600

using namespace std;

//Global variable

Loop loop;
CatMull_Clark catmul;

float xrot = 0.0f;
float yrot = 0.0f;
float zrot = 0.0f;
float xrotspeed = 0.5f;
float yrotspeed = 0.4f;
float zrotspeed = 0.0f;

float curx = 0, cury = 0, curz = 0;

bool arcball_on = false;
bool mouse_down = false;
int xDown = 0, yDown = 0;
int xrotStart = 0, yrotStart = 0;
float scale = 1.0f;

GLfloat r, ang, dang, rot = 0, drot = 1.45f;


GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightPosition[] = { 50.0f, -40.0f, -20.0f, 1.0f };
GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 0.5f, 0.6f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };


GLvoid InitGL(GLvoid)
{
	glClearColor(0.1f, 0.2f, 0.5f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)kWindowWidth / (GLfloat)kWindowHeight, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

GLvoid ReSizeGLScene(int Width, int Height)
{
	glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (GLfloat)Width / (GLfloat)Height, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Object obj;


//string filename_CatMull_Clark = "shapething.obj";


GLvoid DrawGLScene_CatMull_Clark(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//glEnable(GL_FRONT);
	//glFrontFace(GL_CW);
	glTranslatef(0.0f, 0.0f, -10.0f);

	
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glRotatef(zrot, 0.0f, 0.0f, 1.0f);
	
	glScalef(scale, scale, scale);
	//if (arcball_on)
	//{
	//if (xrotspeed || yrotspeed || zrotspeed)
	//{
	if(mouse_down)
	{ 
		xrot += xrotspeed;
		yrot += yrotspeed;
		zrot += zrotspeed;
	}
	//}
	
		
	//glMultMatrixf(rotation_matrix);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	
	//int s = obj.trs.size();

	//int s = loop.trs.size();

	int s = catmul.quads.size();
	//cout << s << "\n";

	for (int i = 0; i < s; ++i)
	{
		int v0 = catmul.quads[i].v0, v1 = catmul.quads[i].v1, 
			v2 = catmul.quads[i].v2, v3 = catmul.quads[i].v3;



		vec3 p0 = catmul.vertices[v0], p1 = catmul.vertices[v1],
			 p2 = catmul.vertices[v2], p3 = catmul.vertices[v3];
		vec3 n = catmul.normals[i];

		glNormal3f(n.x, n.y, n.z);
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
	}

	
	glEnd();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

//string filename_loop = "star.ply";
//string filename = "icos.ply";
//string filename = "octa.ply";
//string filename = "tetra.ply";
//string filename = "torus.ply";

GLvoid DrawGLScene_Loop(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//glEnable(GL_FRONT);
	//glFrontFace(GL_CW);
	glTranslatef(0.0f, 0.0f, -10.0f);


	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glRotatef(zrot, 0.0f, 0.0f, 1.0f);

	glScalef(scale, scale, scale);
	//if (arcball_on)
	//{
	//if (xrotspeed || yrotspeed || zrotspeed)
	//{
	if (mouse_down)
	{
		xrot += xrotspeed;
		yrot += yrotspeed;
		zrot += zrotspeed;
	}
	//}


	//glMultMatrixf(rotation_matrix);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	//int s = obj.trs.size();

	int s = loop.trs.size();

	for (int i = 0; i < s; ++i)
	{
		int v0 = loop.trs[i].v[0], v1 = loop.trs[i].v[1], v2 = loop.trs[i].v[2];
		vec3 p0 = loop.pts[v0], p1 = loop.pts[v1], p2 = loop.pts[v2];
		vec3 n = loop.trs[i].n;
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
	}


	glEnd();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

bool use_loop_subdivision;// = false;
bool use_catmul_clark_subdivision;// = !use_loop_subdivision;


void Keypress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
	case 32:
		if (drot == 0.25f)
			drot = 0.0f;
		else drot = 0.25f;
		//case 82:
			//<vec3>().swap(obj.pts);
			//vector<Triangle>().swap(obj.trs);
			//unordered_map<int, unordered_set<int>>().swap(obj.vertex_map);

			//Read_PLY(filename, obj);
		//case 114:
		//	Read_PLY(filename, obj);
	case 83:
	{
		if (use_loop_subdivision)
			loop.loop_subdivision();
		else
			catmul.catmull_clark_subdivision();
	}
	//loop3(obj);
	case 115:
	{
		if (use_loop_subdivision)
			loop.loop_subdivision();
		else
			catmul.catmull_clark_subdivision();
		//loop3(obj);
	}
	}
}
/*
void onMouse(int button, int state, int x, int y)
{
	int xrotStart, yrotStart;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x != curx && y != cury)
	{
		arcball_on = true;
		//xrotspeed = yrotspeed = 0;
		xrotStart = xrot;
		yrotStart = yrot;
		curx = x;
		cury = y;
	}
	else
	{
		arcball_on = false;
		xrotspeed = 0;
		yrotspeed = 0;
	}
}

void onMotion(int x, int y)
{

	if (curx || x || cury != y && curz != z)
	{
		xrotspeed = (curx - x) / 10.0f;
		yrotspeed = (cury - y) / 10.0f;
		zrotspeed = (curz - z) / 10.0f;
		curx = x;
		cury = y;
	}
}
*/



//good version
/*
void onMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		xrotspeed = yrotspeed = 0;
		
		xrotStart = xrot;
		yrotStart = yrot;
		xDown = x;
		yDown = y;
		mouse_down = true;

	}
	else
	{
		mouse_down = false;
		//xrotspeed = 0;
		//yrotspeed = 0;
	}
}

void onMotion(int x, int y)
{
	if (mouse_down)
	{
		int dx = x - xDown;
		int dy = y - yDown;
		
		xrot = dy / 3.0f + xrotStart;
		yrot = dx / 3.0f + yrotStart;
	}
}
*/


void onMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		xrotspeed = yrotspeed = 0;

		xrotStart = xrot;
		yrotStart = yrot;
		xDown = x;
		yDown = y;
		mouse_down = true;

	}
	else if (button = GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		scale = max(xDown / kWindowWidth - 0.5, yDown/kWindowWidth - 0.5);
	}
	else
	{
		mouse_down = false;
		//xrotspeed = 0;
		//yrotspeed = 0;
	}
}

void onMotion(int x, int y)
{
	if (mouse_down)
	{
		int dx = x - xDown;
		int dy = y - yDown;

		//int d = dx * dx + dy * dy;



		xrot = dy / 3.0f + xrotStart;
		yrot = dx / 3.0f + yrotStart;
		
	}
}

void init_object(string file_name_Loop, string file_name_Catmull_Clark)
{
	if (use_loop_subdivision)
	{
		//Loop loop_1(file_name_Loop);
		//loop = loop_1;
		//cout << loop_1.trs.size();

		loop = Loop(file_name_Loop);
	}
	else
		catmul = CatMull_Clark(file_name_Catmull_Clark);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(kWindowWidth, kWindowHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	InitGL();

	string filename_Loop = "star.ply";
	//string filename_CatMull_Clark = "shapething.obj";

	string filename_CatMull_Clark = "cube-quads.obj";

	use_loop_subdivision = false;
	use_catmul_clark_subdivision = !use_loop_subdivision;


	init_object(filename_Loop, filename_CatMull_Clark);

	
	if (use_loop_subdivision)
		glutDisplayFunc(DrawGLScene_Loop);
	else
		glutDisplayFunc(DrawGLScene_CatMull_Clark);

	glutReshapeFunc(ReSizeGLScene);
	glutKeyboardFunc(Keypress);
	glutMotionFunc(onMotion);
	glutMouseFunc(onMouse);
	glutMainLoop();


	return 0;
}