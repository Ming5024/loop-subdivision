#pragma once
#include<vector>
#include<string>
#include<set>
#include<GL\glut.h>
using namespace std;

struct Vertice
{
	GLfloat X;
	GLfloat Y;
	GLfloat Z;
	set<GLint> neighbors;
};
struct face
{
	GLint v1;
	GLint v2;
	GLint v3;
};
struct edge
{
	GLint face1;
	GLint face2;

	GLint v1;
	GLint v2;
	GLint face1_anv;
	GLint face2_anv;
	GLint newVertice;
};

class offLoader
{
public:
	void read(string filename);
	void draw();
	void print();
	void read();
	void printEdge();
	void loop();
	void printnow();

private:
	vector<Vertice> vertices;
	vector<Vertice> vertices_now;
	vector<edge> edges;
	vector<face> faces;
	vector<face> faces_now;
};
