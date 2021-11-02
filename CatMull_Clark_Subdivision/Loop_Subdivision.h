#ifndef _LOOP_SUBDIVISION_H_
#define _LOOP_SUBDIVISION_H_
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>
#include <vector>
#include "vec3.h"

using namespace std;

struct Triangle
{
	Triangle() {}
	Triangle(int v0, int v1, int v2)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}
	Triangle(int v0, int v1, int v2, vec3 n_)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
		n = n_;
	}

	int v[3];
	int t[3] = { -1 };
	vec3 n;
};

struct Edge
{
	int t0 = -1;
	int t1 = -1;
};

struct Object
{
	Object() {}
	vector<vec3> pts;
	vector<Triangle> trs;
	unordered_map<int, unordered_set<int>> vertex_map;
};

void getfilename(const string& filepath, string& filename)
{
	size_t found = filepath.find_last_of('/\\');
	filename = filepath.substr(found + 1);
}
 

struct SimplePoint
{
	SimplePoint() {}
	SimplePoint(vec3 c_, int ind_) : c(c_), ind(ind_) {}
	vec3 c;
	int ind;
};

//Object& obj
struct Loop
{
	vector<vec3> pts;
	vector<Triangle> trs;
	unordered_map<int, unordered_set<int>> vertex_map;

	//Object obj;

	Loop() {}
	Loop(string file_name)
	{
		Read_PLY(file_name);

	}

	void Read_PLY(const string& path)
	{
		string filename;
		getfilename(path, filename);


		ifstream f(filename);

		string dummy;

		int vertex_num, face_num;
		f >> dummy >> vertex_num >> dummy >> face_num;

		//vector<vec3> pts;
		for (int i = 0; i < vertex_num; ++i)
		{
			vec3 tmp;
			f >> tmp.x >> tmp.y >> tmp.z;
			pts.emplace_back(tmp);
		}
		//vector<Triangle> trs;
		//unordered_map<int, unordered_set<int>> vertex_map;
		for (int i = 0; i < face_num; ++i)
		{
			int n;
			f >> n;
			if (n != 3)
			{
				cout << "only support triangle!";
				return;
			}

			Triangle t;
			f >> t.v[0] >> t.v[1] >> t.v[2];
			int v0 = t.v[0], v1 = t.v[1], v2 = t.v[2];

			//t.t[0] = t.t[1] = t.t[2] = -1;
			vec3 p0 = pts[v0], p1 = pts[v1], p2 = pts[v2];
			vec3 normal = (p0 - p1).cross(p0 - p2).norm();
			t.n = normal;
			trs.emplace_back(t);
		}

		pts = pts;
		trs = trs;
		//obj.vertex_map = vertex_map;

		update_adjacent_triangle();
	}

	void update_adjacent_triangle()
	{
		int s = trs.size();
		unordered_map<string, vector<int>> face_map;
		unordered_map<int, unordered_set<int>> local_vertex_map;//neu khong dung vertex_map cuc bo se xay ra loi ???
		int p = pts.size();
		vector<string> vertex;
		vertex.resize(p);
		for (int i = 0; i < p; ++i)
		{
			vertex[i] = to_string(i);
		}

		for (int i = 0; i < s; ++i)
		{
			int v0 = trs[i].v[0], v1 = trs[i].v[1], v2 = trs[i].v[2];
			local_vertex_map[v0].emplace(v1);
			local_vertex_map[v0].emplace(v2);
			local_vertex_map[v1].emplace(v0);
			local_vertex_map[v1].emplace(v2);
			local_vertex_map[v2].emplace(v0);
			local_vertex_map[v2].emplace(v1);


			string s01 = vertex[v0] + "|" + vertex[v1];
			string s12 = vertex[v1] + "|" + vertex[v2];
			string s20 = vertex[v2] + "|" + vertex[v0];


			face_map[s01].emplace_back(i);
			face_map[s12].emplace_back(i);
			face_map[s20].emplace_back(i);

		}
		vertex_map = local_vertex_map;
		for (int i = 0; i < s; ++i)
		{
			int v0 = trs[i].v[0], v1 = trs[i].v[1], v2 = trs[i].v[2];

			string s10 = vertex[v1] + "|" + vertex[v0];
			string s21 = vertex[v2] + "|" + vertex[v1];
			string s02 = vertex[v0] + "|" + vertex[v2];

			int t0, t1, t2;

			t0 = face_map[s10][0] != i ? face_map[s10][0] : face_map[s10][1];

			t1 = face_map[s21][0] != i ? face_map[s21][0] : face_map[s21][1];

			t2 = face_map[s02][0] != i ? face_map[s02][0] : face_map[s02][1];

			trs[i].t[0] = t0;
			trs[i].t[1] = t1;
			trs[i].t[2] = t2;
		}
	}

	void loop_subdivision()
	{
		int p = pts.size();

		//vector<vec3> even_vertex;
		//even_vertex.resize(p);
		vector<vec3> new_pts;

		//even vertex;
		for (int i = 0; i < p; ++i)
		{
			int k = vertex_map[i].size();
			float beta;
			if (k == 3)
				beta = 0.1875f;

			else if (k > 3)
				beta = 3.0f / (8.0f * k);

			vec3 surrounding;

			for (auto& vp : vertex_map[i])
			{
				surrounding += pts[vp];
			}
			//even_vertex[i] = obj.pts[i] * (1.0f - k * beta) + surrounding * beta;
			new_pts.emplace_back((1.0f - k * beta) * pts[i] + surrounding * beta);
		}


		vector<Triangle> new_trs;

		//odd vertex
		unordered_map<string, SimplePoint> edge_points;
		int s = trs.size();

		int count_ind = p - 1;

		//unordered_map<int, unordered_set<int>> vertex_map;
		vec3 odd_vertex[3];
		int odd_ind[3];
		for (int i = 0; i < s; ++i)
		{
			for (int j = 0; j < 3; ++j)//3 canh
			{
				int v0 = trs[i].v[j];
				int v1 = trs[i].v[(j + 1) % 3];

				string s0 = to_string(v0), s1 = to_string(v1);
				string s01 = s0 + "|" + s1;
				string s10 = s1 + "|" + s0;


				if (edge_points.find(s10) != edge_points.end())
				{
					odd_vertex[j] = edge_points[s10].c;
					odd_ind[j] = edge_points[s10].ind;
				}
				else
				{
					//edge_points[s01]
					int adj = trs[i].t[j];
					int adj_op;
					for (int m = 0; m < 3; ++m)
					{
						int tmp_ind = trs[adj].v[m];
						if (tmp_ind != v0 && tmp_ind != v1)
						{
							adj_op = tmp_ind;
							break;
						}
					}

					int cur_op = trs[i].v[(j + 2) % 3];
					odd_vertex[j] = (3.0f * (pts[v0] + pts[v1]) + (pts[adj_op] + pts[cur_op])) * 0.125f;

					//odd_vertex[j] = (0.375f * (new_pts[v0] + new_pts[v1]) + 0.125f * (new_pts[adj_op] + new_pts[cur_op]));// *0.125f;

					++count_ind;
					odd_ind[j] = count_ind;

					edge_points[s01] = { odd_vertex[j], odd_ind[j] };
					//edge_points[s10] = { odd_vertex[j], odd_ind[j] };
					new_pts.emplace_back(odd_vertex[j]);
				}
			}

			//add 4 triangles
			int v0 = trs[i].v[0];
			int v1 = trs[i].v[1];
			int v2 = trs[i].v[2];

			vec3 n1 = (new_pts[v0] - new_pts[odd_ind[0]]).cross(new_pts[v0] - new_pts[odd_ind[2]]).norm();
			vec3 n2 = (new_pts[v1] - new_pts[odd_ind[1]]).cross(new_pts[v1] - new_pts[odd_ind[0]]).norm();
			vec3 n3 = (new_pts[v2] - new_pts[odd_ind[2]]).cross(new_pts[v2] - new_pts[odd_ind[1]]).norm();
			vec3 n4 = (new_pts[odd_ind[0]] - new_pts[odd_ind[1]]).cross(new_pts[odd_ind[0]] - new_pts[odd_ind[2]]).norm();


			Triangle tr1(v0, odd_ind[0], odd_ind[2], n1);
			Triangle tr2(v1, odd_ind[1], odd_ind[0], n2);
			Triangle tr3(v2, odd_ind[2], odd_ind[1], n3);
			Triangle tr4(odd_ind[0], odd_ind[1], odd_ind[2], n4);

			new_trs.emplace_back(tr1);
			new_trs.emplace_back(tr2);
			new_trs.emplace_back(tr3);
			new_trs.emplace_back(tr4);

		}

		//vector<vec3>().swap(obj.pts);
		//vector<Triangle>().swap(obj.trs);
		pts = new_pts;
		trs = new_trs;
		update_adjacent_triangle();
	}

};
#endif // !_LOOP_SUBDIVISION_H_
