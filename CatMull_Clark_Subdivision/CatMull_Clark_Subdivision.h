#ifndef _CATMULL_CLARK_
#define _CARMULL_CLARK_

#include <unordered_map>
#include <string>
#include "Read_3D_Model.h"

using namespace std;

struct Quad
{
	int v0;
	int v1;
	int v2;
	int v3;

	Quad() {}
	Quad(int v0_, int v1_, int v2_, int v3_) : v0(v0_), v1(v1_), v2(v2_), v3(v3_) {}
};


struct CatMull_Clark
{
	vector<Quad> quads;

	vector<vec3> vertices;
	
	vector<vec3> normals;

	CatMull_Clark() {}
	CatMull_Clark(string file_name)
	{
		vector<Simple_Quad> simple_quads;
		vector<vec3> new_vertices;
		Read_Object_Quad(file_name, simple_quads, new_vertices);

		vertices = new_vertices;

		for (int i = 0; i < simple_quads.size(); ++i)
		{
			int v0 = simple_quads[i].v0;
			int v1 = simple_quads[i].v1;
			int v2 = simple_quads[i].v2;
			int v3 = simple_quads[i].v3;

			quads.emplace_back(Quad(v0, v1, v2, v3));

			vec3 v01 = (vertices[v1] - vertices[v0]).norm();
			vec3 v02 = (vertices[v2] - vertices[v0]).norm();

			vec3 n0 = (v01.cross(v02)).norm();

			normals.push_back(n0);
		}
		cout << vertices.size() << "\n";
		cout << quads.size();
	}

	//vertices_adjacent_face_count count the number of faces adjacent to a vertex
	//faces_adjacent find the the faces that is adjacent to this find by an edge store in string

	void init_for_subdivide(unordered_map<int, int>& vertices_adjacent_face_count,
		                    unordered_map<string, int>& faces_adjacent)
	{
		int quad_size = quads.size();

		for (int i = 0; i < quad_size; ++i)
		{
			int v0 = quads[i].v0;
			int v1 = quads[i].v1;
			int v2 = quads[i].v2;
			int v3 = quads[i].v3;

			vertices_adjacent_face_count[v0]++;
			vertices_adjacent_face_count[v1]++;
			vertices_adjacent_face_count[v2]++;
			vertices_adjacent_face_count[v3]++;

			/*string s01 = to_string(v0) + "|" + to_string(v1);
			string s12 = to_string(v1) + "|" + to_string(v2);
			string s23 = to_string(v2) + "|" + to_string(v3);
			string s30 = to_string(v3) + "|" + to_string(v0);

			faces_adjacent[s01] = i;
			faces_adjacent[s12] = i;
			faces_adjacent[s23] = i;
			faces_adjacent[s30] = i;
			*/

			//adjacent map lam chieu nguoc
			
			string s10 = to_string(v1) + "|" + to_string(v0);
			string s21 = to_string(v2) + "|" + to_string(v1);
			string s32 = to_string(v3) + "|" + to_string(v2);
			string s03 = to_string(v0) + "|" + to_string(v3);

			faces_adjacent[s10] = i;
			faces_adjacent[s21] = i;
			faces_adjacent[s32] = i;
			faces_adjacent[s03] = i;
			
			/*
			string s01 = to_string(v0) + "|" + to_string(v1);
			string s12 = to_string(v1) + "|" + to_string(v2);
			string s23 = to_string(v2) + "|" + to_string(v3);
			string s30 = to_string(v3) + "|" + to_string(v0);

			

			faces_adjacent[s01] = i;
			faces_adjacent[s12] = i;
			faces_adjacent[s23] = i;
			faces_adjacent[s30] = i;
			*/
		}
	}

	void catmull_clark_subdivision()
	{
		unordered_map<int, int> vertices_adjacent_face_count;
		unordered_map<string, int> faces_adjacent;

		init_for_subdivide(vertices_adjacent_face_count, faces_adjacent);

		int quad_size = quads.size();

		vector<vec3> face_points;

		//FACE POINTS
		//compute face points first
		for (int i = 0; i < quad_size; ++i)
		{
			int v0 = quads[i].v0;
			int v1 = quads[i].v1;
			int v2 = quads[i].v2;
			int v3 = quads[i].v3;

			vec3 face_point = (vertices[v0] + vertices[v1] + vertices[v2] + vertices[v3]) * 0.25f;

			face_points.push_back(face_point);
		}

		//EDGE POINTS
		//compute edge points second
		unordered_map<string, vec3> edge_points;// store edge points
		//unordered_map<int, vec3> edge_point_cumulation;//sum of edgepoints 
		                                                 //used for vertex point calculation

		
		for (int i = 0; i < quad_size; ++i)
		{
			int v0 = quads[i].v0;
			int v1 = quads[i].v1;
			int v2 = quads[i].v2;
			int v3 = quads[i].v3;

			
			string s01 = to_string(v0) + "|" + to_string(v1);
			string s12 = to_string(v1) + "|" + to_string(v2);
			string s23 = to_string(v2) + "|" + to_string(v3);
			string s30 = to_string(v3) + "|" + to_string(v0);
			

			
			string s03 = to_string(v0) + "|" + to_string(v3);
			string s32 = to_string(v3) + "|" + to_string(v2);
			string s21 = to_string(v2) + "|" + to_string(v1);
			string s10 = to_string(v1) + "|" + to_string(v0);
			

			int adjacent_f03 = faces_adjacent[s30];

			vec3 edge_point_03 = (face_points[i] + face_points[adjacent_f03] + 
								  vertices[v0] + vertices[v3]) * 0.25f;
			
			edge_points[s30] = edge_point_03;
			edge_points[s03] = edge_point_03;

			int adjacent_f32 = faces_adjacent[s23];

			vec3 edge_point_32 = (face_points[i] + face_points[adjacent_f32] +
				vertices[v3] + vertices[v2]) * 0.25f;

			edge_points[s23] = edge_point_32;
			edge_points[s32] = edge_point_32;

			int adjacent_f21 = faces_adjacent[s12];

			vec3 edge_point_21 = (face_points[i] + face_points[adjacent_f21] +
				vertices[v2] + vertices[v1]) * 0.25f;

			edge_points[s12] = edge_point_21;
			edge_points[s21] = edge_point_21;

			int adjacent_f10 = faces_adjacent[s01];

			vec3 edge_point_10 = (face_points[i] + face_points[adjacent_f10] +
				vertices[v1] + vertices[v0]) * 0.25f;

			edge_points[s01] = edge_point_10;
			edge_points[s10] = edge_point_10;

			/*edge_point_cumulation[v0] += edge_point_10;
			edge_point_cumulation[v0] += edge_point_03;

			edge_point_cumulation[v1] += edge_point_21;
			edge_point_cumulation[v1] += edge_point_10;

			edge_point_cumulation[v2] += edge_point_32;
			edge_point_cumulation[v2] += edge_point_21;

			edge_point_cumulation[v3] += edge_point_03;
			edge_point_cumulation[v3] += edge_point_32;*/
		}

		//VERTEX POINTS
		//finally compute vertex point
		int vertex_size = vertices.size();

		vector<vec3> vertex_points;

		vertex_points.resize(vertex_size);

		for (int i = 0; i < quad_size; ++i)
		{
			int v0 = quads[i].v0;
			int v1 = quads[i].v1;
			int v2 = quads[i].v2;
			int v3 = quads[i].v3;

			int n0 = vertices_adjacent_face_count[v0];
			int n1 = vertices_adjacent_face_count[v1];
			int n2 = vertices_adjacent_face_count[v2];
			int n3 = vertices_adjacent_face_count[v3];

			string s01 = to_string(v0) + "|" + to_string(v1);
			string s12 = to_string(v1) + "|" + to_string(v2);
			string s23 = to_string(v2) + "|" + to_string(v3);
			string s30 = to_string(v3) + "|" + to_string(v0);
		
			vertex_points[v0] += face_points[i] * 1 / n0;
			vertex_points[v0] += (vertices[v0] + vertices[v1]) *0.5f / n0;
			vertex_points[v0] += (vertices[v3] + vertices[v0]) *0.5f / n0;
			
			//vertex_points[v0] += edge_points[s01] * 0.5 / n0;
			//vertex_points[v0] += edge_points[s30] * 0.5 / n0;

			vertex_points[v1] += face_points[i] * 1 / n1;
			vertex_points[v1] += (vertices[v0] + vertices[v1]) *0.5f / n1;
			vertex_points[v1] += (vertices[v1] + vertices[v2]) *0.5f / n1;
			
			//vertex_points[v1] += edge_points[s01] * 0.5 / n1;
			//vertex_points[v1] += edge_points[s12] * 0.5 / n1;

			vertex_points[v2] += face_points[i] * 1 / n2;
			vertex_points[v2] += (vertices[v1] + vertices[v2]) *0.5f / n2;
			vertex_points[v2] += (vertices[v2] + vertices[v3]) *0.5f / n2;
			
			//vertex_points[v2] += edge_points[s12] * 0.5 / n2;
			//vertex_points[v2] += edge_points[s23] * 0.5 / n2;

			vertex_points[v3] += face_points[i] * 1 / n3;
			vertex_points[v3] += (vertices[v3] + vertices[v0]) *0.5f / n3;
			vertex_points[v3] += (vertices[v2] + vertices[v3]) *0.5f / n3;
			
			//vertex_points[v3] += edge_points[s30] * 0.5 / n3;
			//vertex_points[v3] += edge_points[s23] * 0.5 / n3;
		}

		//all these point have to be average
		//since number of edge and face adjacent to a point have to be equal
		//all we need to do is divide them by number of adjacent face to get the average
		//before adding the value of original vertex


		//finally adding (n - 3) * P where P is the original value to complete the equaltion
		//what interesing here is edge point is compute 2 times
		//for each face that adjacent to the edge containing this point
		//and thus match the equation F + 2 * R 
		//https://en.wikipedia.org/wiki/Catmull%E2%80%93Clark_subdivision_surface

		for (int i = 0; i < vertex_size; ++i)
		{
			int n = vertices_adjacent_face_count[i];
			//vertex_points[i] /= n;

			vertex_points[i] = (vertex_points[i] +  (n - 3) * vertices[i]) / n;
		}

		//Finally update faces 
		vector<vec3> new_vertices;
		vector<Quad> new_quads;
		vector<vec3> new_normals;

		int vertex_ind = 0;

		unordered_map<string, int> process_vertex;

		//Quad
		//3           2  
		//
		//
		//
		//0           1

		//4 quads

		//3    e23    2  
		//
		//e30  f     e12
		//
		//0   e01     1

		//0 1 2 3 are new vertex points

		//only edge-vertex and point-vertex are push into map
		//face-vertex are exclude because they only process once

		for (int i = 0; i < quad_size; ++i)
		{
			

			
			int ind_f = vertex_ind;

			//add this line cause wrong
			//string p_ind_f = to_string(ind_f);
			//process_vertex[p_ind_f] = vertex_ind;

			vertex_ind++;
			
			new_vertices.emplace_back(face_points[i]);

			int v0 = quads[i].v0;
			int v1 = quads[i].v1;
			int v2 = quads[i].v2;
			int v3 = quads[i].v3;

			string e01 = to_string(v0) + "|" + to_string(v1);
			string e12 = to_string(v1) + "|" + to_string(v2);
			string e23 = to_string(v2) + "|" + to_string(v3);
			string e30 = to_string(v3) + "|" + to_string(v0);
			
			string e10 = to_string(v1) + "|" + to_string(v0);
			string e21 = to_string(v2) + "|" + to_string(v1);
			string e32 = to_string(v3) + "|" + to_string(v2);
			string e03 = to_string(v0) + "|" + to_string(v3);

			//string e21 = to_string(v2) + "|" + to_string(v1);
			//string e32 = to_string(v3) + "|" + to_string(v2);
			//string e03 = to_string(v0) + "|" + to_string(v3);

			//check edge vertex
			int ind_e01;
			int ind_e12;
			int ind_e23;
			int ind_e30;

			if (process_vertex.find(e01) != process_vertex.end() || process_vertex.find(e10) != process_vertex.end())
			{
				if (process_vertex.find(e01) != process_vertex.end())
					ind_e01 = process_vertex[e01];
				else
					ind_e01 = process_vertex[e10];
			}
			else
			{
				ind_e01 = vertex_ind;

				string e10 = to_string(v1) + "|" + to_string(v0);

				process_vertex[e01] = vertex_ind;
				//process_vertex[e10] = vertex_ind;
				
				if (edge_points.find(e01) != edge_points.end())
					new_vertices.emplace_back(edge_points[e01]);
				else
					new_vertices.emplace_back(edge_points[e10]);

				++vertex_ind;
			}

			if (process_vertex.find(e12) != process_vertex.end() || process_vertex.find(e21) != process_vertex.end())
			{
				if (process_vertex.find(e12) != process_vertex.end())
					ind_e12 = process_vertex[e12];
				else
					ind_e12 = process_vertex[e21];
			}
			else
			{
				ind_e12 = vertex_ind;

				string e21 = to_string(v2) + "|" + to_string(v1);

				process_vertex[e12] = vertex_ind;
				//process_vertex[e21] = vertex_ind;

				if (edge_points.find(e12) != edge_points.end())
					new_vertices.emplace_back(edge_points[e12]);
				//if (edge_points.find(e21) != edge_points.end())
				else
					new_vertices.emplace_back(edge_points[e21]);
				++vertex_ind;
			}

			if (process_vertex.find(e23) != process_vertex.end() || process_vertex.find(e32) != process_vertex.end())
			{
				if (process_vertex.find(e23) != process_vertex.end())
					ind_e23 = process_vertex[e23];
				else
					ind_e23 = process_vertex[e32];
			}
			else
			{
				ind_e23 = vertex_ind;

				string e32 = to_string(v3) + "|" + to_string(v2);

				process_vertex[e23] = vertex_ind;
				//process_vertex[e32] = vertex_ind;

				if (edge_points.find(e23) != edge_points.end())
				new_vertices.emplace_back(edge_points[e23]);
				//if (edge_points.find(e32) != edge_points.end())
				else
					new_vertices.emplace_back(edge_points[e32]);

				++vertex_ind;
			}

			if (process_vertex.find(e30) != process_vertex.end() || process_vertex.find(e03) != process_vertex.end())
			{
				if (process_vertex.find(e30) != process_vertex.end())
					ind_e30 = process_vertex[e30];
				else
					ind_e30 = process_vertex[e03];
			}
			else
			{
				ind_e30 = vertex_ind;

				string e03 = to_string(v0) + "|" + to_string(v3);

				process_vertex[e30] = vertex_ind;
				//process_vertex[e03] = vertex_ind;

				if (edge_points.find(e30) != edge_points.end())
					new_vertices.emplace_back(edge_points[e30]);
				//if (edge_points.find(e03) != edge_points.end())
				else
					new_vertices.emplace_back(edge_points[e03]);
				++vertex_ind;
			}

			//check point vertex
			int ind_v0;
			int ind_v1;
			int ind_v2;
			int ind_v3;

			string p0 = to_string(v0);
			string p1 = to_string(v1);
			string p2 = to_string(v2);
			string p3 = to_string(v3);

			if (process_vertex.find(p0) != process_vertex.end())
				ind_v0 = process_vertex[p0];
			else
			{
				new_vertices.emplace_back(vertex_points[v0]);
				process_vertex[p0] = vertex_ind;
				ind_v0 = vertex_ind++;
			}

			if (process_vertex.find(p1) != process_vertex.end())
				ind_v1 = process_vertex[p1];
			else
			{
				new_vertices.emplace_back(vertex_points[v1]);
				process_vertex[p1] = vertex_ind;
				ind_v1 = vertex_ind++;
			}

			if (process_vertex.find(p2) != process_vertex.end())
				ind_v2 = process_vertex[p2];
			else
			{
				new_vertices.emplace_back(vertex_points[v2]);
				process_vertex[p2] = vertex_ind;
				ind_v2 = vertex_ind++;
			}

			if (process_vertex.find(p3) != process_vertex.end())
				ind_v3 = process_vertex[p3];
			else
			{
				new_vertices.emplace_back(vertex_points[v3]);
				process_vertex[p3] = vertex_ind;
				ind_v3 = vertex_ind++;
			}
			//3    e23    2  
			//
			//e30  f     e12
			//
			//0   e01     1
			//Final Add Quads
			
			/*
			Quad quad1(ind_f, ind_e30, ind_v0, ind_e01);
			Quad quad2(ind_f, ind_e01, ind_v1, ind_e12);
			Quad quad3(ind_f, ind_e12, ind_v2, ind_e23);
			Quad quad4(ind_f, ind_e23, ind_v3, ind_e30);
			*/

			
			Quad quad1(ind_f, ind_e01, ind_v0, ind_e30);
			Quad quad2(ind_f, ind_e12, ind_v1, ind_e01);
			Quad quad3(ind_f, ind_e23, ind_v2, ind_e12);
			Quad quad4(ind_f, ind_e30, ind_v3, ind_e23);
			

			new_quads.emplace_back(quad1);
			new_quads.emplace_back(quad2);
			new_quads.emplace_back(quad3);
			new_quads.emplace_back(quad4);

			vec3 n0 = ((new_vertices[ind_f] - new_vertices[ind_e30]).cross(new_vertices[ind_f] - new_vertices[ind_e01])).norm();
			vec3 n1 = ((new_vertices[ind_f] - new_vertices[ind_e01]).cross(new_vertices[ind_f] - new_vertices[ind_e12])).norm();
			vec3 n2 = ((new_vertices[ind_f] - new_vertices[ind_e12]).cross(new_vertices[ind_f] - new_vertices[ind_e23])).norm();
			vec3 n3 = ((new_vertices[ind_f] - new_vertices[ind_e23]).cross(new_vertices[ind_f] - new_vertices[ind_e30])).norm();

			new_normals.push_back(n0);
			new_normals.push_back(n1);
			new_normals.push_back(n2);
			new_normals.push_back(n3);
		}

		

		/*
		unordered_map<int, int> vertices_adjacent_face_count;
		unordered_map<string, int> faces_adjacent;
		unordered_map<string, vec3> edge_points;
		vector<vec3> vertex_points;
		vector<vec3> new_vertices;
		vector<Quad> new_quads;
		int vertex_ind = 0;

		unordered_map<string, int> process_vertex;
		*/

		vertices_adjacent_face_count.clear();
		faces_adjacent.clear();
		edge_points.clear();

		vector<vec3>().swap(vertex_points);

		//vector<vec3>().swap(vertices);
		vertices = new_vertices;

		//vector<Quad>().swap(quads);
		quads = new_quads;

		//vector<vec3>().swap(normals);
		normals = new_normals;

		vector<vec3>().swap(new_vertices);
		vector<Quad>().swap(new_quads);
		vector<vec3>().swap(new_normals);

		/*for (int i = 0; i < vertex_size; ++i)
		{
			int n = vertices_adjacent_face_count[i];
			vertex_points[i] = (vertex_points[i] + )
		}*/
	}


};


#endif // !_CATMULL_CLARK_

