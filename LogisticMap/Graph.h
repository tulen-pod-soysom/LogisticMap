#pragma once
#include <vector>
#include <string>
#include <gdiplus.h>
#include "Program.h"

// Graph

class Graph : public CStatic
{
	DECLARE_DYNAMIC(Graph)

public:
	Graph();
	virtual ~Graph();

private:

	//std::vector<Gdiplus::PointF*> points;

	//std::vector<std::vector<LinkedPoint>>* points;



	std::vector<size_t> counts;
	Gdiplus::GdiplusStartupInput input;
	ULONG_PTR token;

	Gdiplus::Font* font1;
	Gdiplus::Font* font2;
	Gdiplus::Font* font3;


	Gdiplus::Pen* gridPen;
	Gdiplus::Pen* mainGridPen;

	Gdiplus::Pen* graphPen1;
	Gdiplus::Pen* graphPen2	;
	Gdiplus::Pen* graphPen3;
	Gdiplus::Pen* graphPen4	;
	Gdiplus::Pen* graphPen5	;


	Gdiplus::REAL offsetX = 30;
	Gdiplus::REAL offsetY = 30;

	Gdiplus::StringFormat* stringformat;


	void DrawGrid(
		Gdiplus::Graphics& graphics,
		Gdiplus::Matrix& matrix,
		unsigned nGridX, unsigned nGridY
	);
	void DrawGridMainLines(
		Gdiplus::Graphics& graphics,
		Gdiplus::Matrix& matrix
	);
	void DrawTitle(
		Gdiplus::Graphics& graphics,
		Gdiplus::Rect rect
	);
	void DrawCaptions(
		Gdiplus::Graphics& graphics,
		Gdiplus::Rect rect
	);
	void DrawValues(
		Gdiplus::Graphics& graphics,
		Gdiplus::Matrix& matrix
	);
public:    

    Gdiplus::RectF worldRect = {0,-1,2,2};

    CString title;
    CString captionX;
    CString captionY;

	Program* program;

	int graph_type;
	//void SetWorldRect(std::initializer_list<std::vector<double>> values, float width);
	//void SetPoints(std::initializer_list<std::vector<double>> values);



	// Useful methods
	template<typename T>
	static T ClosestMantissa(T value)
	{

		T power = pow(10, floor(log10(value)));
		T mantissa = ceil(pow(10, log10(value) - floor(log10(value))));
			return	power * mantissa;
	}

    template<typename InputIt, typename T = InputIt::value_type>
	static T ClosestMantissa(InputIt begin, InputIt end)
	{
		T fmax = *(std::max_element(begin, end));
		T fmin = *(std::min_element(begin, end));
		T fextr = (fmax > -fmin) ? fmax : -fmin;
		return ClosestMantissa(fextr);
	}

	static void DeflateRect(Gdiplus::Rect& rect, Gdiplus::REAL offset_x, Gdiplus::REAL offset_y)
	{
		rect.X += offset_x; rect.Y += offset_y; rect.Width -= offset_x * 2; rect.Height -= offset_y * 2;
	};



	template<typename T>
	static std::pair<Gdiplus::PointF*, size_t> VectorToPoints(std::vector<T> values, Gdiplus::RectF worldRect)
	{
		PointF* points = new PointF[values.size()];
		for (size_t i = 0; i < values.size(); i++)
		{
			points[i].X = worldRect.X + i * worldRect.Width / values.size();
			points[i].Y = values[i];
		}

		return { points,values.size() };
	};


protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};



