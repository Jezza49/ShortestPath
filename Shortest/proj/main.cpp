#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <bits/stdc++.h>
#include <sstream>
#include <stack>
#include <math.h>
#include "tri.cpp"
#include "triangulate.h"
#include "monotone.cpp"
#include "construct.cpp"

typedef struct _point
{
	double x;
	double y;
	_point() : x(0.0), y(0.0) {}
	_point(double xp, double yp) : x(xp), y(yp){}
} point, vect;

typedef std::vector<point> points;
typedef std::vector<int> path;
typedef std::pair<int, int> diagnal;

points polygon_points;
inline double dist(const point &p, const point &q);
path simple_stupid_funnel(const std::vector<diagnal> &d_list, const point &start, const point &end);
inline int sign(double v);
inline double cross_product(const vect &v1, const vect &v2);
std::vector<diagnal> diagnals_list(const int triangles[][3], const std::vector<int> &triangles_list);
std::vector<int> dfs(const int triangles[][3], int triangles_count, int start_tri, int end_tri);
bool neighbor_triangles(const int triangles[][3], int triangles_count,int tri1, int tri2);
int locate_point(const int triangles[][3], int triangles_count, const point &k);
bool in_triangle(const point &p, const point &q, const point &r, const point &k);
bool to_left(const point &p, const point &q, const point &k);

int main(int argc, char *argv[])
{
	/*if (argc != 3)
	{
		std::cout << usage << std::endl;
		return -1;
	}*/
	clock_t t;
    int f;
    t = clock();
	std::string line;
	std::ifstream input(argv[1], std::ifstream::in);
	double sx, sy, ex, ey;
	int count;
	input >> count;
	for (int i = 0; i < count; i++)
	{
		double x, y;
		input >> x >> y;
		polygon_points.push_back(point(x, y));
	}
	input >> sx >> sy >> ex >> ey;
	point start(sx, sy), end(ex, ey);
	int vertices_count = polygon_points.size();
	int triangles_count = polygon_points.size() - 2;

	double (*vertices)[2] = new double[vertices_count + 1][2];
	int (*triangles)[3] = new int[triangles_count][3];

	for (int i = 1; i <= polygon_points.size(); i++)
	{
		vertices[i][0] = polygon_points[i - 1].x;
		vertices[i][1] = polygon_points[i - 1].y;
	}
	int np = polygon_points.size();
	triangulate_polygon(np, vertices, triangles);
	for (int i = 0; i < triangles_count; i++)
	{
		triangles[i][0]--;
		triangles[i][1]--;
		triangles[i][2]--;
	}
	std::ofstream output(argv[2], std::ofstream::out);
	int start_tri = locate_point(triangles, triangles_count, start);
	int end_tri = locate_point(triangles, triangles_count, end);
	if(start_tri==-1 || end_tri==-1)
	{
		std::cout<<"Points not in triangle";
		return 0;
	}

	if (start_tri == end_tri)
	{
		std::cout << "Distance: " << dist(start, end) << std::endl;
	}
	else
	{
		std::vector<int> triangles_list = dfs(triangles, triangles_count, start_tri, end_tri);
		std::vector<diagnal> d_list = diagnals_list(triangles, triangles_list);
		path shortest_path = simple_stupid_funnel(d_list, start, end);
		double total_dist = 0;

			for (int i = 0; i < shortest_path.size(); i++)
			{
				std::cout << shortest_path[i] << std::endl;
			}

			for (int i = 0; i < shortest_path.size(); i++)
			{
				point last;
				point current = polygon_points[shortest_path[i]];
				if (i == 0)
					last = start;
				else
					last = polygon_points[shortest_path[i - 1]];
				total_dist += dist(last, current);
			}
			if (shortest_path.size() > 0)
				total_dist += dist(polygon_points[shortest_path[shortest_path.size() - 1]], end);
			else
				total_dist += dist(start, end);
			std::cout << "Distance: " << (total_dist) << std::endl;
	}

	t = clock() - t;

  // printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
  output<<count<<" "<<t;

	delete[] vertices;
	delete[] triangles;
	return 0;
}

inline double dist(const point &p, const point &q)
{
	return sqrt((p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y));
}

path simple_stupid_funnel(const std::vector<diagnal> &d_list, const point &start, const point &end)
{
	path shortest_path;
	point apex = start;
	int apex_id = -1;
	int left = d_list[0].first;
	int right = d_list[0].second;
	int lo = left;
	int ro = right;
	int left_id = 0;
	int right_id = 0;
	bool add_left = false;
	for (int i = 1; i <= d_list.size(); i++)
	{
		vect lv, rv, nv;
		lv = vect(polygon_points[left].x - apex.x, polygon_points[left].y - apex.y);
		rv = vect(polygon_points[right].x - apex.x, polygon_points[right].y - apex.y);
		int new_p = -1;
		bool last_end = i == d_list.size();
		if (last_end)
			nv = vect(end.x - apex.x, end.y - apex.y);
		else
		{
			if (d_list[i].first == left)
			{
				new_p = d_list[i].second;
				add_left = false;
			}
			else if (d_list[i].second == left)
			{
				new_p = d_list[i].first;
				add_left = false;
			}
			else if (d_list[i].first == right)
			{
				new_p = d_list[i].second;
				add_left = true;
			}
			else if (d_list[i].second == right)
			{
				new_p = d_list[i].first;
				add_left = true;
			}
			else if (d_list[i].first == lo)
			{
				new_p = d_list[i].second;
				add_left = false;
			}
			else if (d_list[i].second == lo)
			{
				new_p = d_list[i].first;
				add_left = false;
			}
			else if (d_list[i].first == ro)
			{
				new_p = d_list[i].second;
				add_left = true;
			}
			else if (d_list[i].second == ro)
			{
				new_p = d_list[i].first;
				add_left = true;
			}
			nv = vect(polygon_points[new_p].x - apex.x, polygon_points[new_p].y - apex.y);
		}
		bool cond1 = sign(cross_product(lv, nv)) == sign(cross_product(lv, rv));
		bool cond2 = sign(cross_product(rv, nv)) == sign(cross_product(rv, lv));
		if (add_left || last_end)
		{
			if (cond1)
			{
				if (cond2)
				{
					left = new_p;
					left_id = i;
				}
				else
				{
					shortest_path.push_back(right);
					apex = polygon_points[right];
					apex_id = right;
					for (i = right_id; i < d_list.size(); i++)
					{
						if (d_list[i].first != right && d_list[i].second != right)
						{
							left = d_list[i].first;
							right = d_list[i].second;
							left_id = right_id = i;
							break;
						}
					}
				}
			}
			else
				lo = new_p;
		}
		if (!add_left || last_end)
		{
			if (cond2)
			{
				if (cond1)
				{
					right = new_p;
					right_id = i;
				}
				else
				{
					shortest_path.push_back(left);
					apex = polygon_points[left];
					apex_id = left;
					for (i = left_id; i < d_list.size(); i++)
					{
						if (d_list[i].first != left && d_list[i].second != left)
						{
							left = d_list[i].first;
							right = d_list[i].second;
							left_id = right_id = i;
							break;
						}
					}
				}
			}
			else
				ro = new_p;
		}
	}
	return shortest_path;
}

inline int sign(double v)
{
	if (v > 0) return 1;
	else if (v < 0) return -1;
	else return 0;
}

inline double cross_product(const vect &v1, const vect &v2)
{
	return v1.x * v2.y - v2.x * v1.y;
}

std::vector<diagnal> diagnals_list(const int triangles[][3], const std::vector<int> &triangles_list)
{
	std::vector<diagnal> d_list;
	for (int i = 1; i < triangles_list.size(); i++)
	{
		const int *last_tri = triangles[triangles_list[i - 1]];
		const int *current_tri = triangles[triangles_list[i]];
		for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			bool cond1 = current_tri[i] == last_tri[j] && current_tri[(i + 1) % 3] == last_tri[(j + 1) % 3];
			bool cond2 = current_tri[i] == last_tri[(j + 1) % 3] && current_tri[(i + 1) % 3] == last_tri[j];
			if (cond1 || cond2)
				d_list.push_back(std::make_pair(current_tri[i], current_tri[(i + 1) % 3]));
		}
	}
	return d_list;
}

std::vector<int> dfs(const int triangles[][3], int triangles_count, int start_tri, int end_tri)
{
	std::vector<int> triangles_list;
	bool *visited = new bool[triangles_count];
	int *parent = new int[triangles_count];
	memset(visited, false, triangles_count * sizeof(bool));
	memset(parent, -1, triangles_count * sizeof(int));

	std::stack<int> s;
	s.push(start_tri);
	while (!s.empty())
	{
		int v = s.top(); s.pop();
		if (!visited[v])
		{
			visited[v] = true;
			for (int u = 0; u < triangles_count; u++)
			if (neighbor_triangles(triangles, triangles_count, v, u) && !visited[u])
			{
				s.push(u);
				parent[u] = v;
			}
		}
		if (parent[end_tri] != -1)
			break;
	}
	int p = end_tri;
	while (parent[p] != -1)
	{
		triangles_list.push_back(p);
		p = parent[p];
	}
	triangles_list.push_back(start_tri);
	std::reverse(triangles_list.begin(), triangles_list.end());
	delete[] visited;
	delete[] parent;
	return triangles_list;
}

bool neighbor_triangles(const int triangles[][3], int triangles_count,int tri1, int tri2)
{
	if (tri1 != tri2)
	{
		const int *t1 = triangles[tri1];
		const int *t2 = triangles[tri2];
		for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			bool cond1 = t1[i] == t2[j] && t1[(i + 1) % 3] == t2[(j + 1) % 3];
			bool cond2 = t1[i] == t2[(j + 1) % 3] && t1[(i + 1) % 3] == t2[j];
			if (cond1 || cond2)
				return true;
		}
	}
	return false;
}

int locate_point(const int triangles[][3], int triangles_count, const point &k)
{
	int i;int flag=0;
	for (i = 0; i < triangles_count; i++)
	{
		point p = polygon_points[triangles[i][0]];
		point q = polygon_points[triangles[i][1]];
		point r = polygon_points[triangles[i][2]];

		if (in_triangle(p, q, r, k))
			{flag=1;break;}
	}
	if(flag==0){return -1;}
	return i;
}

bool in_triangle(const point &p, const point &q, const point &r, const point &k)
{
	bool pq_left = to_left(p, q, k);
	bool qr_left = to_left(q, r, k);
	bool rp_left = to_left(r, p, k);

	return (pq_left == qr_left) && (qr_left == rp_left);
}

bool to_left(const point &p, const point &q, const point &k)
{
	double px = p.x; double py = p.y;
	double qx = q.x; double qy = q.y;
	double kx = k.x; double ky = k.y;

	double area2 =px * qy - py * qx + qx * ky - qy * kx + kx * py - ky * px;
	return area2 >= 0;
}