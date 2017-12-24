#include<iostream>
#include<fstream>
#include <tchar.h>
#include "offLoader.h"
using namespace std;
//setlocale(LC_ALL, "Chinese-simplified");//设置中文环境
string filename = "F:\\z.off";
offLoader offtest;
//setlocale(LC_ALL, "C");//还原
//实现移动鼠标观察模型所需变量
//实现移动鼠标观察模型所需变量
static float c = 3.1415926 / 180.0f;
static float r = 1.0f;
static int degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;

GLuint texture;  //我的纹理名称
#define BMP_Header_Length 54 

GLfloat xRotAngle = 0.0f;
GLfloat yRotAngle = 0.0f;

int modelnum;

int mi2(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
	{
		cout << "Load failed" << endl;
		return 0;
	}
	else
	{
		//cout << "Read in" << endl;
	}

	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!mi2(width) || !mi2(height) || width > max || height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256;
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}


			gluScaleImage(GL_RGB, width, height, GL_UNSIGNED_BYTE, pixels, new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, lastTextureID);
	free(pixels);
	return texture_ID;
}

//安置光源
void setLightRes() {
	GLfloat sun_light_position[] = { -100.0f, 100.0f, 0.0f, 0.0f };
	GLfloat sun_light_ambient[] = { 3.0f, 3.0f, 3.0f, 1.0f };
	GLfloat sun_light_diffuse[] = { 3.0f, 3.0f, 3.0f, 3.0f };
	GLfloat sun_light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
	glEnable(GL_LIGHTING); //启用光源
	glEnable(GL_LIGHT0);   //使用指定灯光
}

//初始化
void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("OffLoader");
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	setLightRes();
	glEnable(GL_DEPTH_TEST);
	offtest.read(filename);
}


int isDown = 0;
int OriX = 0;
int OriY = 0;

void initial()
{
	OriX = 0;
	OriY = 0;
}
void R_mouse(int button, int state, int x, int y)
{
	//cout << button << endl;
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		isDown = 1;
	}
	else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON)
	{
		initial();
		isDown = 0;
	}
	else if (state == GLUT_UP && button == GLUT_WHEEL_UP)
	{
		glScalef(1.2f, 1.2f, 1.2f);
		glutPostRedisplay();
	}

	else if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)
	{
		glScalef(0.8f, 0.8f, 0.8f);
		glutPostRedisplay();
	}

}
void mouseMove(int x, int y)
{
	if (isDown == 1)
	{
		if (OriX == 0 && OriY == 0)
		{
			OriX = x;
			OriY = y;
		}
		else
		{
			int changeX = x - OriX;
			int changeY = y - OriY;
			xRotAngle += changeY;
			yRotAngle += changeX;
			OriX = x;
			OriY = y;
			glutPostRedisplay();
		}
	}
}

void display()
{
	//cout << "display" << endl;
	glClearColor(0, 0, 0, 0);
	glColor3f(1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);
	setLightRes();
	//gluLookAt(r*cos(c*degree), 0, r*sin(c*degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//objtest.print();
	//glDisable(GL_LIGHTING);
	glRotatef(xRotAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yRotAngle, 0.0f, 1.0f, 0.0f);

	//画obj模型
	offtest.draw();
	glPopMatrix();
	glutSwapBuffers();
	//system("pause");
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);
	/*
	if (width <= height)
		gluOrtho2D(0.0, 30.0, 0.0, 30.0 * (GLfloat)height / (GLfloat)width);
	else
		gluOrtho2D(0.0, 30.0 * (GLfloat)width / (GLfloat)height, 0.0, 30.0);
	*/
	glMatrixMode(GL_MODELVIEW);
}

void ChangeSize(GLsizei w, GLsizei h)
{
	//避免高度为0
	if (h == 0) {
		h = 1;
	}
	//定义视口大小，宽高一致  
	glViewport(0, 0, w, h);
	float half = 250;
	//重置坐标系统，使投影变换复位
	glMatrixMode(GL_PROJECTION);
	//将当前的用户坐标系的原点移到了屏幕中心
	glLoadIdentity();
	//定义正交视域体  
	if (w<h) {
		//如果高度大于宽度，则将高度视角扩大，图形显示居中
		glOrtho(-half, half, -half*h / w, half*h / w, -half, half);
	}
	else {
		//如果宽度大于高度，则将宽度视角扩大，图形显示居中
		glOrtho(-half*w / h, half*w / h, -half, half, -half, half);
	}

}

//分形
void PressKey(int key, int x, int y) {
	//cout << key << endl;
	if (key == GLUT_KEY_UP) {
		offtest.read();
		offtest.loop();
	}
	else if (key == GLUT_KEY_DOWN) {
		offtest.read();
		offtest.loop();
	}

	glutPostRedisplay();
}

void processMenu(int n) {
	string fileName;
	switch (n) {
	case 0:
		modelnum = 0;
		setlocale(LC_ALL, "Chinese-simplified");//设置中文环境
		filename = "F:\\课件\\专选\\图形学\\第三次作业\\第三次作业数据\\Loop 细分模型\\z.off";
		setlocale(LC_ALL, "C");//还原
		offtest.read(filename);
		//init();
		break;
	case 1:
		modelnum = 1;
		setlocale(LC_ALL, "Chinese-simplified");//设置中文环境
		filename = "F:\\cone.off";
		setlocale(LC_ALL, "C");//还原
		offtest.read(filename);
		//init();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	//std::locale::global(std::locale(""));
	//locale loc = locale::global(locale(""));
	//objtest.print();
	//locale::global(loc);
	//cout << objtest.vertices.size() << endl;
	//offtest.print();
	//offtest.read();
	//offtest.printEdge();
	//objtest.print();
	//objtest.printEdge();
	//offtest.loop();
	//offtest.printnow();
	//system("pause");

	int modelmenu;
	int texturemenu;
	int mainmenu;
	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(PressKey);
	glutMotionFunc(mouseMove);
	glutMouseFunc(R_mouse);
	modelmenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("z", 0);
	glutAddMenuEntry("cone", 1);
	//glutAddMenuEntry("surface", 2);
	//texturemenu = glutCreateMenu(processMenu);
	//glutAddMenuEntry("texture1", 3);
	//glutAddMenuEntry("texture2", 4);
	//glutAddMenuEntry("texture3", 5);
	//mainmenu = glutCreateMenu(processMenu);
	//glutAddSubMenu("model", modelmenu);
	//glutAddSubMenu("texture", texturemenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();

	return 0;
}