#pragma once

#include <vector>
#include <algorithm>

/* ear clipper */
namespace ear_clipper
{
	/* vertex class*/
	template<typename T>
	struct vertex
	{
		vertex() : x(T(0)), y(T(0))
		{

		}
		vertex(T _x, T _y) : x(_x), y(_y)
		{

		}

		inline vertex<T> operator+(const vertex<T>& rhs)
		{
			return { x + rhs.x, y + rhs.y };
		}

		inline vertex<T> operator-(const vertex<T>& rhs)
		{
			return { x - rhs.x, y - rhs.y };
		}

		T x;
		T y;
		bool is_ear{ false };
		int index{ 0 };
	};

	/* triangulation class for polygon triangulation */
	template<class T>
	class triangulation
	{
	public:
		using vtype = std::vector<vertex<T>>;
		triangulation() = default;
		triangulation(vtype _vertices)
		{
			vertices = _vertices;

			//reserve space for indices
			indices.reserve((vertices.size() - 2) * 3);

			//initialize vertex data
			init_data();
		}

		/* function to triangulate polygon*/
		std::vector<int>& triangulate()
		{
			int sz = 0, idx1 = 0, idx2 = 0;

			/* loop until the polygon has only 3 vertices remaining*/
			while (vertices.size() >= 3)
			{
				sz = vertices.size();
				for (int i = 0; i < sz; i++)
				{
					/* calculate adjacent vertex indices */
					idx1 = (i + 1) % sz; 
					idx2 = (i + 2) % sz;

					/* check if vertex is an ear */
					if (vertices[idx1].is_ear)
					{
						/* store the vertices in an array/list/vector */
						indices.emplace_back(vertices[i].index);
						indices.emplace_back(vertices[idx1].index);
						indices.emplace_back(vertices[idx2].index);

						/* remove the vertex from the polygon */
						vertices.erase(vertices.begin() + idx1);
						sz--;

						/* check if adjacent vertices changed their ear/non-ear configuration and update*/
						idx1 = (i + 1) % sz;
						evaluate(i == 0 ? sz - 1 : i - 1, i, idx1);

						idx2 = (i + 2) % sz;
						evaluate(idx1, idx2, (idx2 + 1) % sz);

						break;
					}
				}


			}
			return indices;
		}

	private:
		void init_data()
		{
			const int sz = vertices.size();

			/* loop through all vertices and check them for ear/non-ear property */
			for (int i = 0; i < sz; i++)
			{
				evaluate(i, (i + 1) % sz, (i + 2) % sz);
				vertices[i].index = i;
			}

		}

		/* ear evaluation is done here*/
		void evaluate(int i, int idx1, int idx2)
		{
			int sz = vertices.size();

			/* calculate the determinant */
			vertex<T> v1 = (vertices[i] - vertices[idx1]);
			vertex<T> v2 = (vertices[idx1] - vertices[idx2]);
			T det = (v1.x * v2.y - v1.y * v2.x);

			/* if positive, we have an ear and set the vertex ear property to true */
			if (det < T(0))
				return;

			/* check if there is any point in the triangle */
			if (!has_points_in_tri(i, idx1, idx2))
				vertices[idx1].is_ear = true;
		}

		/* bayer coordinates used here to check if a point is in the given triangle, a different approach can also be used */
		bool is_in_triangle(const vertex<T>& p, const vertex<T>& v1, const vertex<T>& v2, const vertex<T>& v3)
		{
			T alpha = ((v2.y - v3.y) * (p.x - v3.x) + (v3.x - v2.x) * (p.y - v3.y)) / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
			T beta = ((v3.y - v1.y) * (p.x - v3.x) + (v1.x - v3.x) * (p.y - v3.y)) / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
			T gamma = 1.0f - alpha - beta;

			return (alpha > T(0) && beta > T(0) && gamma > T(0));
		}

		/* function to check if any point lies within the given triangle*/
		bool has_points_in_tri(int i, int idx1, int idx2)
		{
			int sz = vertices.size();

			for (int j = 0; j < sz; j++)
			{
				if (j == idx1 || j == idx2 || j == i)
					continue;
				if (is_in_triangle(vertices[j], vertices[i], vertices[idx1], vertices[idx2]))
					return true;

			}
			return false;
		}

	private:
		/* vector to hold vertices */
		vtype vertices;

		/* vector to hold indices */
		std::vector<int> indices;
	};
}