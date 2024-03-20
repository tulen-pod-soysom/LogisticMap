#pragma once

#include <vector>
#include <set>
#include <algorithm>

#include <gdiplus.h>


struct LinkedPoint
{
	double x = 0.0, y = 0.0;
	const LinkedPoint* parent = nullptr;

	LinkedPoint() {};
	LinkedPoint(double x, double y, LinkedPoint* parent) :x(x), y(y), parent(parent) { }
};

class Program
{
public:
	double x0 = 0.5;
	double R0 = 0.0;
	double epsilon = 0.8e-2;
	double point_shrink_epsilon = 1e-7;

	const unsigned N = 400;		// Кол-во итераций для X
	const unsigned M = 2048;	// Кол-во отсчетов по R
	const unsigned L = 100;		// Кол-во отображаемых точек на отсчете R-i

	std::vector<std::vector<LinkedPoint>> points;

	std::vector<Gdiplus::PointF> pointsR;

	LinkedPoint R1;
	std::pair<LinkedPoint, LinkedPoint> R2;

	bool double_precision_equal(double a, double b, double epsilon) { return fabs(a - b) < epsilon; }

	void evaluate_2()
	{
		pointsR.resize(N);

		pointsR[0] = { (float)0,(float)x0 };
		for (size_t j = 1; j < N; j++)
		{
			pointsR[j] = { (float)j,(float)R0 * pointsR[j - 1].Y * (1 - pointsR[j - 1].Y) };
		}
	}

	void evaluate()
	{
		points.clear();

		points.resize(M);
		std::for_each(points.begin(), points.end(), [&](auto& a) {a.reserve(L); });


		R1 = {};	R2 = {};
		bool R1_found = false;
		bool R2_found = false;

		std::vector<LinkedPoint> x(N);


		for (size_t i = 0; i < M; i++)
		{
			double R = i * 4.0 / M;

			x[0] = { 0,x0,nullptr };
			for (size_t j = 1; j < N; j++)
			{
				x[j] = { R,R * x[j - 1].y * (1 - x[j - 1].y) ,nullptr };
			}


			if (i == 0) continue;



			// Отсеивание лишних точек
			points[i].push_back(*x.rbegin());
			for (auto it = x.rbegin() + 1; it != x.rbegin() + L; it++)
			{
				if (!double_precision_equal(it->y, (it - 1)->y, point_shrink_epsilon)) 
					points[i].push_back(*it);
			}


			// Связь с предыдущим отсчетом
			std::for_each(points[i].begin(), points[i].end(),
				[&](LinkedPoint& a) {
					if (points[i - 1].empty()) return;
					a.parent = std::addressof(*std::min_element(points[i - 1].begin(), points[i - 1].end(),
						[&](LinkedPoint b, LinkedPoint c) {return abs(b.y - a.y) < abs(c.y - a.y); }
					));
				}
			);

			// Поиск особых точек R1, R2
			if (!R1_found)
				for (auto it = points[i].begin() + 1; it != points[i].end(); it++)
				{
					if (!double_precision_equal(it->y, (it - 1)->y, epsilon)) { R1 = *it; R1_found = true; break; }
				}
			else if (!R2_found)
			{
				LinkedPoint* unique_points[4] = {
					std::addressof(points[i][0]),
					nullptr,
					nullptr,
					nullptr
				};

				auto equal_to_any_unique_point = [&](LinkedPoint& p)
				{
					unsigned a = 0;
					for (size_t i = 0; i < 4; i++)
					{
						if (unique_points[i] != nullptr)
							if (!double_precision_equal(p.y, unique_points[i]->y, epsilon))
							{
								a++; if (a == 3) return false;

								if ((i < 3) && (a == i + 1) && (unique_points[i + 1] == nullptr))
									unique_points[i + 1] = std::addressof(p);
							}
							else break;
					}

					return true;
				};

				for (auto it = points[i].begin() + 1; it != points[i].end(); it++)
				{
					if (!equal_to_any_unique_point(*it))
					{
						R2 = { *unique_points[0],*unique_points[1] };
						//R2 = { *unique_points[0]->parent,*unique_points[1]->parent };
						R2_found = true;
						break;
					}
				}
			}
		}
	}
};

