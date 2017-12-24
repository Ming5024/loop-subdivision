#include<fstream>
#include<iostream>
#include<cmath>
#include "offLoader.h"
using namespace std;

#define PI 3.1415926

void offLoader::read(string filename)
{
	vertices.resize(0);
	faces.resize(0);
	string line;
	fstream f;
	f.open(filename, ios::in);
	if (!f.is_open()) {
		cout << "Something Went Wrong When Opening Objfiles" << endl;
	}
	//cout << f.rdbuf();
	/*
	char buffer[256];
	while (!f.eof())
	{
	f.getline(buffer, 100);
	cout << buffer << endl;
	}
	*/
	//cout << "read" << endl;
	int count = 0;
	while (!f.eof())
	{
		getline(f, line);
		//cout << line << endl;
		if (line == "OFF" || line == "")
		{
			continue;
		}
		vector<string> parameters;
		string tail = " ";
		string num = "";

		line = line.append(tail);
		//cout << line << endl;
		for (int i = 0; i < line.length(); i++)
		{
			char temp = line[i];
			if (temp != ' ')
			{
				num += temp;
			}
			else if (temp == ' ' && num != "")
			{
				//cout << num << endl;
				parameters.push_back(num);
				num = "";
			}
		}
		/*cout << parameters.size() << " ";
		for (int i = 0; i < parameters.size(); i++) 
		{
			cout << parameters[i] << " ";
		}
		cout << endl;*/
		/*
		cout << parameters.size() << " ";
		for (int i = 0; i < parameters.size(); i++)
		{
		cout << parameters[i] << " ";
		}
		cout << endl;
		*/
		if (parameters.size() == 3)		//如果是顶点的话
		{
			vector<GLfloat> Point;
			for (int i = 0; i < parameters.size(); i++)			//从1开始，将顶点的xyz三个坐标放入顶点vector
			{
				GLfloat point = atof(parameters[i].c_str()) * 100;
				//cout << point << " ";
				Point.push_back(point);
			}
			/*
			cout << endl;
			for (int i = 0; i < Point.size(); i++)
			{
			cout << Point[i] << " ";
			}
			cout << endl;
			*/
			Vertice v_temp = Vertice{ Point[0], Point[1], Point[2] };
			vertices.push_back(v_temp);
		}
		else if (parameters.size() == 4)
		{
			vector<GLint> Point;
			for (int i = 1; i < parameters.size(); i++)			//从1开始，将顶点的xyz三个坐标放入顶点vector
			{
				GLint point = atoi(parameters[i].c_str());
				//cout << point << " ";
				Point.push_back(point);
			}
			/*
			cout << endl;
			for (int i = 0; i < Point.size(); i++)
			{
			cout << Point[i] << " ";
			}
			cout << endl;
			*/
			face face_temp = face{ Point[0], Point[1], Point[2] };
			faces.push_back(face_temp);
			//faces_now.push_back(face_temp);
		}
	}
	f.close();
	vertices.erase(vertices.begin());
	//cout << "finish" << endl;
	vertices_now = vertices;
	faces_now = faces;
}

void offLoader::draw()
{
	//cout << "draw" << endl;
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < faces_now.size(); i++)
	{
		//cout << faces[i].v1 << " " << faces[i].v2 << " " << faces[i].v3 << endl;
		GLfloat VN[3];
		//三个顶点
		GLfloat SV1[3];
		GLfloat SV2[3];
		GLfloat SV3[3];

		GLint firstVertexIndex = faces_now[i].v1;//取出顶点索引
		GLint secondVertexIndex = faces_now[i].v2;
		GLint thirdVertexIndex = faces_now[i].v3;

		SV1[0] = vertices_now[firstVertexIndex].X;//第一个顶点
		SV1[1] = vertices_now[firstVertexIndex].Y;
		SV1[2] = vertices_now[firstVertexIndex].Z;

		SV2[0] = vertices_now[secondVertexIndex].X;//第二个顶点
		SV2[1] = vertices_now[secondVertexIndex].Y;
		SV2[2] = vertices_now[secondVertexIndex].Z;

		SV3[0] = vertices_now[thirdVertexIndex].X;//第三个顶点
		SV3[1] = vertices_now[thirdVertexIndex].Y;
		SV3[2] = vertices_now[thirdVertexIndex].Z;
		
		GLfloat vec1[3], vec2[3], vec3[3];//计算法向量
		/*
		//(x2-x1,y2-y1,z2-z1)
		vec1[0] = SV1[0] - SV2[0];
		vec1[1] = SV1[1] - SV2[1];
		vec1[2] = SV1[2] - SV2[2];

		//(x3-x2,y3-y2,z3-z2)
		vec2[0] = SV1[0] - SV3[0];
		vec2[1] = SV1[1] - SV3[1];
		vec2[2] = SV1[2] - SV3[2];
		*/
		//(x2-x1,y2-y1,z2-z1)
		vec1[0] = SV1[0] - SV2[0];
		vec1[1] = SV1[1] - SV2[1];
		vec1[2] = SV1[2] - SV2[2];

		//(x3-x2,y3-y2,z3-z2)
		vec2[0] = SV1[0] - SV3[0];
		vec2[1] = SV1[1] - SV3[1];
		vec2[2] = SV1[2] - SV3[2];
		//(x3-x1,y3-y1,z3-z1)
		vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
		vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
		vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

		GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));
		
		VN[0] = vec3[0] / D;
		VN[1] = vec3[1] / D;
		VN[2] = vec3[2] / D;
		/*
		VN[0] += (SV3[1] - SV1[1]) * (SV3[2] + SV1[2]);
		VN[1] += (SV3[2] - SV1[2]) * (SV3[0] + SV1[0]);
		VN[2] += (SV3[0] - SV1[0]) * (SV3[1] + SV1[1]);
		VN[0] += (SV3[1] - SV2[1]) * (SV3[2] + SV2[2]);
		VN[1] += (SV3[2] - SV2[2]) * (SV3[0] + SV2[0]);
		VN[2] += (SV3[0] - SV2[0]) * (SV3[1] + SV2[1]);

		float len = 0;
		len += VN[0] * VN[0];
		len += VN[1] * VN[1];
		len += VN[2] * VN[2];
		float normal_len = sqrt(len);
		if (normal_len > 1.0E-6) {
			VN[0] /= normal_len;
			VN[1] /= normal_len;
			VN[2] /= normal_len;
		}
		*/
		glNormal3f(VN[0], VN[1], VN[2]);//绘制法向量

		glVertex3f(SV1[0], SV1[1], SV1[2]);//绘制三角面片
		glVertex3f(SV2[0], SV2[1], SV2[2]);
		glVertex3f(SV3[0], SV3[1], SV3[2]);
	}
	glEnd();
}

void offLoader::print()
{
	for (int i = 0; i < vertices.size(); i++)
	{
		//Vertice temp = vertices[i];
		cout << "v " << vertices[i].X << " " << vertices[i].Y << " " << vertices[i].Z << endl;
	}
	cout << vertices.size() << endl;

	for (int i = 0; i < faces.size(); i++)
	{
		//Vertice temp = vertices[i];
		cout << "face " << faces[i].v1 << " " << faces[i].v2 << " " << faces[i].v3 << endl;
	}
	cout << faces.size() << endl;
}

void offLoader::read()
{
	edges.resize(0);
	for (int i = 0; i < faces_now.size(); i++)
	{
		GLint firstVertexIndex = faces_now[i].v1;//取出顶点索引
		GLint secondVertexIndex = faces_now[i].v2;
		GLint thirdVertexIndex = faces_now[i].v3;
		//cout << firstVertexIndex << " " << secondVertexIndex << " " << thirdVertexIndex << endl;
		vertices_now[firstVertexIndex].neighbors.insert(secondVertexIndex);
		vertices_now[firstVertexIndex].neighbors.insert(thirdVertexIndex);
		vertices_now[secondVertexIndex].neighbors.insert(firstVertexIndex);
		vertices_now[secondVertexIndex].neighbors.insert(thirdVertexIndex);
		vertices_now[thirdVertexIndex].neighbors.insert(firstVertexIndex);
		vertices_now[thirdVertexIndex].neighbors.insert(secondVertexIndex);
		bool e1 = false;
		bool e2 = false;
		bool e3 = false;
		for (int j = 0; j < edges.size(); j++)
		{
			if ((firstVertexIndex == edges[j].v1 && secondVertexIndex == edges[j].v2) || (firstVertexIndex == edges[j].v2 && secondVertexIndex == edges[j].v1))
			{
				edges[j].face2 = i;
				edges[j].face2_anv = thirdVertexIndex;
				e1 = true;
			}
			if ((secondVertexIndex == edges[j].v1 && thirdVertexIndex == edges[j].v2) || (secondVertexIndex == edges[j].v2 && thirdVertexIndex == edges[j].v1))
			{
				edges[j].face2 = i;
				edges[j].face2_anv = firstVertexIndex;
				e2 = true;
			}
			if ((firstVertexIndex == edges[j].v1 && thirdVertexIndex == edges[j].v2) || (firstVertexIndex == edges[j].v2 && thirdVertexIndex == edges[j].v1))
			{
				edges[j].face2 = i;
				edges[j].face2_anv = secondVertexIndex;
				e3 = true;
			}
		}
		if (e1 == false)
		{
			edge e_temp;
			e_temp.face1 = i;
			e_temp.face2 = -1;
			e_temp.v1 = firstVertexIndex;
			e_temp.v2 = secondVertexIndex;
			e_temp.face1_anv = thirdVertexIndex;
			edges.push_back(e_temp);
		}
		if (e2 == false)
		{
			edge e_temp;
			e_temp.face1 = i;
			e_temp.face2 = -1;
			e_temp.v1 = secondVertexIndex;
			e_temp.v2 = thirdVertexIndex;
			e_temp.face1_anv = firstVertexIndex;
			edges.push_back(e_temp);
		}
		if (e3 == false)
		{
			edge e_temp;
			e_temp.face1 = i;
			e_temp.face2 = -1;
			e_temp.v1 = firstVertexIndex;
			e_temp.v2 = thirdVertexIndex;
			e_temp.face1_anv = secondVertexIndex;
			edges.push_back(e_temp);
		}
	}

	for (int i = 0; i < edges.size(); i++)
	{
		Vertice new_v;
		if (edges[i].face2 == -1)
		{
			new_v.X = (vertices_now[edges[i].v1].X + vertices_now[edges[i].v2].X) / 2;
			new_v.Y = (vertices_now[edges[i].v1].Y + vertices_now[edges[i].v2].Y) / 2;
			new_v.Z = (vertices_now[edges[i].v1].Z + vertices_now[edges[i].v2].Z) / 2;
		}
		else
		{
			new_v.X = (vertices_now[edges[i].v1].X + vertices_now[edges[i].v2].X) * 3 / 8 + (vertices_now[edges[i].face1_anv].X + vertices_now[edges[i].face2_anv].X) / 8;
			new_v.Y = (vertices_now[edges[i].v1].Y + vertices_now[edges[i].v2].Y) * 3 / 8 + (vertices_now[edges[i].face1_anv].Y + vertices_now[edges[i].face2_anv].Y) / 8;
			new_v.Z = (vertices_now[edges[i].v1].Z + vertices_now[edges[i].v2].Z) * 3 / 8 + (vertices_now[edges[i].face1_anv].Z + vertices_now[edges[i].face2_anv].Z) / 8;
		}
		vertices_now.push_back(new_v);
		edges[i].newVertice = vertices_now.size() - 1;
	}
}

void offLoader::printEdge()
{
	for (int i = 0; i < vertices_now.size(); i++)
	{
		cout << vertices_now[i].X << " " << vertices_now[i].Y << " " << vertices_now[i].Z << "      ";
		set<int>::iterator ite1 = vertices_now[i].neighbors.begin();
		set<int>::iterator ite2 = vertices_now[i].neighbors.end();
		for (; ite1 != ite2; ite1++)
		{
			cout << *ite1 << " ";
		}
		cout << endl;
	}

	for (int i = 0; i < edges.size(); i++)
	{
		cout << edges[i].face1 << " " << edges[i].face2 << " " << edges[i].v1 << " " << edges[i].v2 << " " << edges[i].face1_anv << " " << edges[i].face2_anv << endl;
	}
	cout << edges.size() << endl;
}

void offLoader::loop()
{
	vector<face> face_temp;
	vector<Vertice> vertice_temp = vertices_now;
	int size = faces_now.size();

	for (int i = 0; i < size; i++)
	{
		GLint v1_index = faces_now[i].v1;
		GLint v2_index = faces_now[i].v2;
		GLint v3_index = faces_now[i].v3;
		GLint e1_index, e2_index, e3_index;
		GLfloat temp_x1, temp_y1, temp_z1, temp_x2, temp_y2, temp_z2, temp_x3, temp_y3, temp_z3;
		//建立边顶点
		for (int j = 0; j < edges.size(); j++)
		{
			if (edges[j].face2 == -1)
			{
				continue;
			}
			if ((v1_index == edges[j].v1 && v2_index == edges[j].v2) || (v1_index == edges[j].v2 && v2_index == edges[j].v1))
			{
				e1_index = edges[j].newVertice;
				//cout << e1_index << endl;
				vertice_temp[v1_index].neighbors.erase(v2_index);
				vertice_temp[v2_index].neighbors.erase(v1_index);
				vertice_temp[v1_index].neighbors.insert(e1_index);
				vertice_temp[v2_index].neighbors.insert(e1_index);
				vertice_temp[e1_index].neighbors.insert(v1_index);
				vertice_temp[e1_index].neighbors.insert(v2_index);
			}
			if ((v2_index == edges[j].v1 && v3_index == edges[j].v2) || (v2_index == edges[j].v2 && v3_index == edges[j].v1))
			{
				e2_index = edges[j].newVertice;
				vertice_temp[v2_index].neighbors.erase(v3_index);
				vertice_temp[v3_index].neighbors.erase(v2_index);
				vertice_temp[v2_index].neighbors.insert(e2_index);
				vertice_temp[v3_index].neighbors.insert(e2_index);
				vertice_temp[e2_index].neighbors.insert(v2_index);
				vertice_temp[e2_index].neighbors.insert(v3_index);
			}
			if ((v1_index == edges[j].v1 && v3_index == edges[j].v2) || (v1_index == edges[j].v2 && v3_index == edges[j].v1))
			{
				e3_index = edges[j].newVertice;
				vertice_temp[v1_index].neighbors.erase(v3_index);
				vertice_temp[v3_index].neighbors.erase(v1_index);
				vertice_temp[v1_index].neighbors.insert(e3_index);
				vertice_temp[v3_index].neighbors.insert(e3_index);
				vertice_temp[e3_index].neighbors.insert(v1_index);
				vertice_temp[e3_index].neighbors.insert(v3_index);
			}
		}

		//更新三个顶点
		//顶点v1
		int size1 = vertices_now[v1_index].neighbors.size();
		//cout << "size1 = " << size1 << endl;
		float beta = (5.0 / 8 - pow((3.0 / 8 + 1.0 / 4 * cos(2 * PI / size1)), 2)) / size1;
		//cout << "beta = " << beta << endl;
		temp_x1 = (1 - size1 * beta) * vertices_now[v1_index].X;
		temp_y1 = (1 - size1 * beta) * vertices_now[v1_index].Y;
		temp_z1 = (1 - size1 * beta) * vertices_now[v1_index].Z;
		set<int>::iterator ite1 = vertices_now[v1_index].neighbors.begin();
		set<int>::iterator ite2 = vertices_now[v1_index].neighbors.end();
		//cout << "temp_x1_1:" << temp_x1 << endl;
		for (; ite1 != ite2; ite1++)
		{
			temp_x1 += beta * vertices_now[*ite1].X;
			temp_y1 += beta * vertices_now[*ite1].Y;
			temp_z1 += beta * vertices_now[*ite1].Z;
		}
		//cout << "temp_x1_2:" << temp_x1 << endl;
		vertice_temp[v1_index].X = temp_x1;
		vertice_temp[v1_index].Y = temp_y1;
		vertice_temp[v1_index].Z = temp_z1;
		//顶点v2
		int size2 = vertices_now[v2_index].neighbors.size();
		beta = (5.0 / 8 - pow((3.0 / 8 + 1.0 / 4 * cos(2 * PI / size2)), 2)) / size2;
		temp_x2 = (1 - size2 * beta) * vertices_now[v2_index].X;
		temp_y2 = (1 - size2 * beta) * vertices_now[v2_index].Y;
		temp_z2 = (1 - size2 * beta) * vertices_now[v2_index].Z;
		ite1 = vertices_now[v2_index].neighbors.begin();
		ite2 = vertices_now[v2_index].neighbors.end();
		for (; ite1 != ite2; ite1++)
		{
			temp_x2 += beta * vertices_now[*ite1].X;
			temp_y2 += beta * vertices_now[*ite1].Y;
			temp_z2 += beta * vertices_now[*ite1].Z;
		}
		vertice_temp[v2_index].X = temp_x2;
		vertice_temp[v2_index].Y = temp_y2;
		vertice_temp[v2_index].Z = temp_z2;
		//顶点v3
		int size3 = vertices_now[v3_index].neighbors.size();
		beta = (5.0 / 8 - pow((3.0 / 8 + 1.0 / 4 * cos(2 * PI / size3)), 2)) / size3;
		temp_x3 = (1 - size3 * beta) * vertices_now[v3_index].X;
		temp_y3 = (1 - size3 * beta) * vertices_now[v3_index].Y;
		temp_z3 = (1 - size3 * beta) * vertices_now[v3_index].Z;
		ite1 = vertices_now[v3_index].neighbors.begin();
		ite2 = vertices_now[v3_index].neighbors.end();
		for (; ite1 != ite2; ite1++)
		{
			temp_x3 += beta * vertices_now[*ite1].X;
			temp_y3 += beta * vertices_now[*ite1].Y;
			temp_z3 += beta * vertices_now[*ite1].Z;
		}
		vertice_temp[v3_index].X = temp_x3;
		vertice_temp[v3_index].Y = temp_y3;
		vertice_temp[v3_index].Z = temp_z3;

		vertices_now[e1_index].neighbors.insert(e2_index);
		vertices_now[e1_index].neighbors.insert(e3_index);
		vertices_now[e2_index].neighbors.insert(e1_index);
		vertices_now[e2_index].neighbors.insert(e3_index);
		vertices_now[e3_index].neighbors.insert(e1_index);
		vertices_now[e3_index].neighbors.insert(e2_index);

		face F1{ v1_index, e1_index, e3_index };
		face F2{ e1_index, v2_index, e2_index };
		face F3{ e3_index, e2_index, v3_index };
		face F4{ e1_index, e2_index, e3_index };
		face_temp.push_back(F1);
		face_temp.push_back(F2);
		face_temp.push_back(F3);
		face_temp.push_back(F4);

	}
	vertices_now = vertice_temp;
	faces_now = face_temp;
}

void offLoader::printnow()
{
	for (int i = 0; i < vertices_now.size(); i++)
	{
		//Vertice temp = vertices[i];
		cout << "v_now " << vertices_now[i].X << " " << vertices_now[i].Y << " " << vertices_now[i].Z << endl;
	}
	cout << vertices_now.size() << endl;

	for (int i = 0; i < faces_now.size(); i++)
	{
		//Vertice temp = vertices[i];
		cout << "face_now " << faces_now[i].v1 << " " << faces_now[i].v2 << " " << faces_now[i].v3 << endl;
	}
	cout << faces_now.size() << endl;
}
