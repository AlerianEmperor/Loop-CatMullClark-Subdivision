#ifndef _READ_3D_MODEL_H_
#define _READ_3D_MODEL_H_

#include <vector>
#include <fstream>
#include "vec3.h"

using namespace std;

//why create Simple_Triangle and Simple_Quad
//why triangle and quads alreadu exist

//the reason is triangle and quads should be define near the main struct of Loop and Catmull-clark
//if we want to use triangle and quads directly, they have to be declared in this file

struct Simple_Triangle
{
	int v0;
	int v1;
	int v2;
	Simple_Triangle() {}
	Simple_Triangle(int v0_, int v1_, int v2_) : v0(v0_), v1(v1_), v2(v2_) {}
};

struct Simple_Quad
{
	int v0;
	int v1;
	int v2;
	int v3;
	Simple_Quad() {}
	Simple_Quad(int v0_, int v1_, int v2_, int v3_) : v0(v0_), v1(v1_), v2(v2_), v3(v3_) {}
};
/*
void fixIndex(int& v, const int& n)
{
	v = v < 0 ? v + n : v > 0 ? v - 1 : -1;
}

void fixIndex(int v[3], const int& n)
{
	v[0] = v[0] < 0 ? v[0] + n : v[0] > 0 ? v[0] - 1 : -1;
	v[1] = v[1] < 0 ? v[1] + n : v[1] > 0 ? v[1] - 1 : -1;
	v[2] = v[2] < 0 ? v[2] + n : v[2] > 0 ? v[2] - 1 : -1;

}

//Reuse Utility file in Ray Tracing Super Project

void SkipSpace(char *&t)
{
	t += strspn(t, " \t");
}

void getdirection(const string& filepath, string& direction)
{
	size_t found = filepath.find_last_of('/\\');
	direction = filepath.substr(0, found + 1);
}

void getfilename(const string& filepath, string& filename)
{

	size_t found = filepath.find_last_of('/\\');
	filename = filepath.substr(found + 1);
}

void getdDirectionAndName(const string& filepath, string& direction, string& filename)
{
	size_t found = filepath.find_last_of('/\\');
	direction = filepath.substr(0, found + 1);
	filename = filepath.substr(found + 1);
}

struct Geometry
{
	Geometry() {}
	vector<vec3> v;
	vector<vec3> vt;
	vector<vec3> vn;
};

struct Face
{
	Face() {}
	Face(int v0, int vt0, int v1, int vt1, int v2, int vt2)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;

		vt[0] = vt0;
		vt[1] = vt1;
		vt[2] = vt2;

		vn[0] = -1;
		vn[1] = -1;
		vn[2] = -1;
	}

	Face(int v0, int vt0, int vn0, int v1, int vt1, int vn1, int v2, int vt2, int vn2)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;

		vt[0] = vt0;
		vt[1] = vt1;
		vt[2] = vt2;

		vn[0] = vn0;
		vn[1] = vn1;
		vn[2] = vn2;
	}

	int v[3] = { -1 };
	int vt[3] = { -1 };
	int vn[3] = { -1 };
};

enum face
{
	Single_Line, Double_Line
};


void get_face_index(char*& t, const int& vs, const int& vts, const int& vns, vector<Face>& faces)
{
	string s = t;
	int length = s.find_last_of("0123456789");
	s = s.substr(0, length + 1);

	int sign = 1;
	int count = 0;
	vector<int> index;
	//vector<int> indices;
	int face_type = Single_Line;

	int num_data_per_vertex = 0;
	bool found_num_data_per_vertex = false;

	for (int i = 0; i <= length + 1; ++i)
	{
		if (s[i] == '-')
		{
			sign = -1;
		}
		else if (isdigit(s[i]))
		{
			count = 10 * count + s[i] - '0';
		}
		else if (s[i] == '/')
		{
			if (!found_num_data_per_vertex)
			{
				++num_data_per_vertex;
			}
			face_type = Single_Line;
			index.emplace_back(sign * count);
			sign = 1;
			count = 0;

			if (s[i + 1] == '/')
			{
				face_type = Double_Line;
				++i;
			}
		}
		else if (s[i] == ' ')
		{

			index.emplace_back(sign * count);
			sign = 1;
			count = 0;
			if (!found_num_data_per_vertex)
			{
				++num_data_per_vertex;
				found_num_data_per_vertex = true;
			}
		}
		else if (i == length + 1)
		{
			index.emplace_back(sign * count);
			sign = 1;
			break;
		}
	}

	//cout << index.size() << "\n";
	//cout << "\n";
	int size = index.size();

	//cout << num_data_per_vertex << "\n";
	//if (size % 2 == 0 && size % 3 == 0) //6 12 18 24 30
	//{
	if (num_data_per_vertex == 3) // v/vt/vn case   12 18 24 30 
	{
		//cout << "line 3\n";
		for (int i = 0; i < size; i += 3)
		{
			fixIndex(index[i], vs);
			fixIndex(index[i + 1], vts);
			fixIndex(index[i + 2], vns);
		}

		int start_v = 0;
		int start_vt = 1;
		int start_vn = 2;

		int num_Triangle = size / 3 - 2;

		for (int i = 0; i < num_Triangle; ++i)
		{
			Face f(index[start_v], index[start_vt], index[start_vn], index[3 * i + 3], index[3 * i + 4], index[3 * i + 5], index[3 * i + 6], index[3 * i + 7], index[3 * i + 8]);
			faces.emplace_back(f);
		}
	}
	else if (num_data_per_vertex == 2)  //  v/vt or v//vn
	{
		//cout << "line 2\n";
		if (face_type == Single_Line) // v / vt
		{
			//cout << "single\n";
			for (int i = 0; i < size; i += 2)
			{
				fixIndex(index[i], vs);
				fixIndex(index[i + 1], vts);
			}

			int num_Triangle = size / 2 - 2;

			int start_v = 0;
			int start_vt = 1;

			for (int i = 0; i < num_Triangle; ++i)
			{
				//01 23 45    01 45 67
				Face f(index[start_v], index[start_vt], -1, index[2 * i + 2], index[2 * i + 3], -1, index[2 * i + 4], index[2 * i + 5], -1);
				faces.emplace_back(f);
			}
		}
		else if (face_type == Double_Line)// v // vn
		{
			//cout << "double\n";
			for (int i = 0; i < size; i += 2)
			{
				fixIndex(index[i], vs);
				fixIndex(index[i + 1], vns);
			}

			int num_Triangle = size / 2 - 2;

			int start_v = 0;
			int start_vn = 1;

			for (int i = 0; i < num_Triangle; ++i)
			{
				Face f(index[start_v], -1, index[start_vn], index[2 * i + 2], -1, index[2 * i + 3], index[2 * i + 4], -1, index[2 * i + 5]);
				faces.emplace_back(f);
			}
		}
	}
}

void Read_Obj_Triangle(string file_path, vector<Simple_Triangle>& trs, vector<vec3>& vertices)
{
	string filename;
	string direction;
	getdDirectionAndName(file_path, direction, filename);
	getdirection(file_path, direction);


	ifstream f(file_path);
	if (!f)
	{
		cout << "obj file not exist";
		return;
	}

	char line[1024];

	while (f.getline(line, 1024))
	{
		char* t = line;
		int prev_space = strspn(t, " \t");
		t += prev_space;

		if (strncmp(t, "v ", 2) == 0)
		{
			t += 1;
			float x, y, z;

			int post_space = strspn(t, " \t");
			t += post_space;
			sscanf(t, "%f %f %f", &x, &y, &z);

			vec3 vertex(x, y, z);

			vertices.emplace_back(vertex);
			++num_v;

		}
		else if (strncmp(t, "f", 1) == 0)
		{
			//t += 1;
			int post_space = strspn(t + 1, " \t");

			t += post_space + 1;


			int face_type;
			vector<Face> faces;
			get_face_index(t, num_v, num_vt, num_vn, faces);


		}
	}
}

void Read_Object_Quad(vector<Simple_Quad>& quads, vector<vec3>& vertices)
{

}
*/

//these are just simplify version of reading object
//for more complicate object, refere to Reading Object Function of Ray Tracing Super Project

void Read_Object_Triangle(string file_name, vector<Simple_Triangle>& trs, vector<vec3>& vertices)
{
	ifstream f(file_name);
	if (!f)
	{
		cout << "obj file not exist";
		return;
	}

	char line[1024];

	while (f.getline(line, 1024))
	{
		char* t = line;

		int prev_space = strspn(t, " \t");
		t += prev_space;

		if (strncmp(t, "v ", 2) == 0)
		{
			float x, y, z;
			sscanf_s(t, "%f %f %f", &x, &y, &z);
			vertices.emplace_back(vec3(x, y, z));
		}
		else if (strncmp(t, "f", 1) == 0)
		{
			int x, y, z;
			sscanf_s(t, "%d %d %d", &x, &y, &z);
			Simple_Triangle simple(x - 1, y - 1, z - 1);

			trs.emplace_back(simple);
		}
	}
}

void Read_Object_Quad(string file_name, vector<Simple_Quad>& quads, vector<vec3>& vertices)
{
	ifstream f(file_name);
	if (!f)
	{
		cout << "obj file not exist";
		return;
	}

	char line[1024];

	while (f.getline(line, 1024))
	{
		char* t = line;

		int prev_space = strspn(t, " \t");
		t += prev_space;

		if (strncmp(t, "v ", 2) == 0)
		{
			t += 2;
			float x, y, z;
			sscanf_s(t, "%f %f %f", &x, &y, &z);
			vertices.emplace_back(vec3(x, y, z));
		}
		else if (strncmp(t, "f ", 2) == 0)
		{
			t += 2;
			int x, y, z, w;
			sscanf_s(t, "%d %d %d %d", &x, &y, &z, &w);
			cout << x << " " << y << " " << z << " " << w << "\n";

			Simple_Quad simple(x - 1, y - 1, z - 1, w - 1);

			quads.emplace_back(simple);
		}
	}
}

#endif // !_READ_3D_MODEL_H_
