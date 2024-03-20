// Graph.cpp: файл реализации
//

#include "pch.h"

#include "Graph.h"

#include <numeric>
#include <algorithm>

#undef max
using namespace Gdiplus;

// Graph

IMPLEMENT_DYNAMIC(Graph, CStatic)

Graph::Graph()
{
	Status s = GdiplusStartup(&token, &input, NULL);
	if (s != Status::Ok) {}

	font1 = new Gdiplus::Font{ L"Arial", 9, Gdiplus::FontStyleRegular };
	font2 = new Gdiplus::Font{ L"Arial", 11, Gdiplus::FontStyleRegular };
	font3 = new Gdiplus::Font{ L"Arial", 11, Gdiplus::FontStyleItalic };


	gridPen = new Gdiplus::Pen{ Gdiplus::Color(128, 128, 128), 0.005 };
	mainGridPen = new Gdiplus::Pen{ Gdiplus::Color(0, 0, 0), 0.01 };


	graphPen1= new Gdiplus::Pen{ Gdiplus::Color(255, 0, 0), 0.001 };
	graphPen2= new Gdiplus::Pen{ Gdiplus::Color(0, 0, 255), 0.001 };
	graphPen3= new Gdiplus::Pen{ Gdiplus::Color(0, 255, 0), 0.001 };
	graphPen4= new Gdiplus::Pen{ Gdiplus::Color(255, 255, 0), 0.001 };
	graphPen5= new Gdiplus::Pen{ Gdiplus::Color(0x1f, 0x3a, 0x3d), 0.001 };

	stringformat = new StringFormat();
	stringformat->SetAlignment(StringAlignmentCenter);
	stringformat->SetLineAlignment(StringAlignmentCenter);
}

Graph::~Graph()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(Graph, CStatic)
END_MESSAGE_MAP()



// Обработчики сообщений Graph




void Graph::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	auto& a = lpDrawItemStruct->rcItem;
	Rect outerrect{ a.left, a.top, a.right - a.left, a.bottom - a.top };
	Rect rect = outerrect; DeflateRect(rect, offsetX, offsetY);

	Graphics graphicsDC(lpDrawItemStruct->hDC);
	Bitmap bitmap(outerrect.Width,outerrect.Height,&graphicsDC);
    Graphics graphics(&bitmap);

	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.Clear(Color(255, 255, 255));

	Matrix matrix;
	matrix.Translate(0.5 * outerrect.Width, 0.5 * outerrect.Height);
	matrix.Scale((REAL)rect.Width / worldRect.Width, -(REAL)rect.Height / worldRect.Height);
	matrix.Translate(-(worldRect.X + worldRect.Width / 2.0), -(worldRect.Y + worldRect.Height / 2.0));
	
	double y_max = std::max(worldRect.Y,worldRect.Y + worldRect.Height);

    int nGridX = 10; 
    int nGridY = 2 * round(pow(10, log10(y_max) - floor(log10(y_max)))) * ((ceil(pow(10, log10(y_max) - floor(log10(y_max)))) == 1) ? 5 : 1);    //ymax = [0,1,2,3,4,5,6,7,8,9] * 10^n!!!

	DrawGrid(graphics,matrix,nGridX,nGridY);
	DrawValues(graphics, matrix);
	DrawGridMainLines(graphics, matrix);

	DrawTitle(graphics, rect);
	DrawCaptions(graphics, rect);


	graphicsDC.DrawImage(&bitmap, outerrect);
}

void Graph::DrawGrid(Graphics& graphics, Matrix& matrix, unsigned nGridX,unsigned nGridY)
{
	for (int var = 1; var < nGridX; ++var)
	{
		PointF p1 = { worldRect.X + (REAL)var / nGridX * worldRect.Width ,worldRect.Y };
		PointF p2 = { worldRect.X + (REAL)var / nGridX * worldRect.Width ,worldRect.Y + worldRect.Height };
		CString caption; caption.Format(L"%g", p1.X);

		matrix.TransformPoints(&p1);
		matrix.TransformPoints(&p2);

		graphics.DrawLine(gridPen, p1, p2);

		RectF r = { PointF{ p1.X - offsetX, p1.Y }, SizeF{ 2 * offsetX, offsetY } };
		graphics.DrawString(caption.GetString(), caption.GetLength(), font1, r, stringformat, &SolidBrush(Color(0, 0, 0)));
	}
	for (int var = 1; var < nGridY; ++var)
	{
		PointF p1 = { worldRect.X,                    worldRect.Y + (REAL)var / nGridY * worldRect.Height };
		PointF p2 = { worldRect.X + worldRect.Width , worldRect.Y + (REAL)var / nGridY * worldRect.Height };
		CString caption; caption.Format(L"%g", p1.Y);

		matrix.TransformPoints(&p1);
		matrix.TransformPoints(&p2);

		graphics.DrawLine(gridPen, p1, p2);


		RectF r = { PointF{ 0, p1.Y - 30 }, SizeF{ offsetX, 30 * 2 } };
		graphics.DrawString(caption.GetString(), caption.GetLength(), font1, r, stringformat, &SolidBrush(Color(0, 0, 0)));
	}
}


void Graph::DrawTitle(Gdiplus::Graphics& graphics, Gdiplus::Rect rect)
{
	graphics.TranslateTransform(offsetX, offsetY);
	graphics.DrawString(title.GetString(), title.GetLength(), font3, RectF{ PointF{0.0f,-offsetY },SizeF{(REAL)rect.Width,offsetY} }, stringformat, &SolidBrush(Color(0, 0, 0)));
	graphics.ResetTransform();
}

void Graph::DrawGridMainLines(Gdiplus::Graphics& graphics, Gdiplus::Matrix& matrix)
{
	// Отрисовка главных осей 
	PointF xl{ worldRect.X, 0 }, xr{ worldRect.X + worldRect.Width, 0 };
	PointF yb{ 0, worldRect.Y }, yt{ 0, worldRect.Y + worldRect.Height };

	matrix.TransformPoints(&xl); matrix.TransformPoints(&xr);
	matrix.TransformPoints(&yb); matrix.TransformPoints(&yt);

	graphics.DrawLine(mainGridPen, xl, xr);
	graphics.DrawLine(mainGridPen, yb, yt);
}

void Graph::DrawCaptions(Gdiplus::Graphics& graphics, Gdiplus::Rect rect)
{
	graphics.TranslateTransform(offsetX, offsetY);
	graphics.DrawString(captionY.GetString(), captionY.GetLength(), font2, PointF{ 0.0f, 0.0f }, &SolidBrush(Color(0, 0, 0)));
	graphics.DrawString(captionX.GetString(), captionX.GetLength(), font2, PointF{ (REAL)rect.Width, (REAL)rect.Height / 2.0f }, &SolidBrush(Color(0, 0, 0)));
	graphics.ResetTransform();
}

void Graph::DrawValues(Gdiplus::Graphics& graphics, Gdiplus::Matrix& matrix)
{

	graphics.SetTransform(&matrix);
	/*if (!points.empty())
		for (size_t var = 0; var < points.size(); ++var)
		{
			if (counts[var] == 0) continue;
			Pen* graphPen;
			switch (var) {
			case 0: {graphPen = graphPen1; break; }
			case 1: {graphPen = graphPen2; break; }
			case 2: {graphPen = graphPen3; break; }
			case 3: {graphPen = graphPen4; break; }
			case 4: {graphPen = graphPen5; break; }
			default: {continue; break; }
			}

			graphics.DrawCurve(graphPen, points[var], counts[var], 0.1f);
		}*/

	if (graph_type == 1)
	if (program != nullptr)
	{
		auto& points = program->points;
		for (auto i = points.rbegin(); i != points.rend(); i++)
		{
			for (auto& j : *i)
			{
				if (j.parent != nullptr)	graphics.DrawLine(graphPen1, PointF(j.x, j.y), PointF(j.parent->x, j.parent->y));
			}
		}

		float r = program->epsilon;

		auto& R1 = program->R1;
		graphics.DrawLine(&Pen(Color(0, 0, 255), 0.002),
			(float)R1.x, (float)R1.y - r,
			(float)R1.x, (float)R1.y + r);
		CString str1; str1.Format(L"R1{%f,%f}", (float)R1.x, (float)R1.y);

		auto& R2 = program->R2;
		graphics.DrawLine(&Pen(Color(0, 0, 255), 0.002),
			(float)R2.first.x, (float)R2.first.y - r,
			(float)R2.first.x, (float)R2.first.y + r);
		CString str2; str2.Format(L"R2{%f,%f}", (float)R2.first.x, (float)R2.first.y);

		graphics.DrawLine(&Pen(Color(0, 0, 255), 0.002),
			(float)R2.second.x, (float)R2.second.y - r,
			(float)R2.second.x, (float)R2.second.y + r);
		CString str3; str3.Format(L"R2{%f,%f}", (float)R2.second.x, (float)R2.second.y);

		graphics.ResetTransform();

		PointF p1 = { (float)R1.x, (float)R1.y };
		PointF p2 = { (float)R2.first.x, (float)R2.first.y };
		PointF p3 = { (float)R2.second.x, (float)R2.second.y };

		matrix.TransformPoints(&p1);
		matrix.TransformPoints(&p2);
		matrix.TransformPoints(&p3);


		graphics.DrawString(str1, str1.GetLength(), font1, p1, &SolidBrush(Color::BlueViolet));
		graphics.DrawString(str2, str2.GetLength(), font1, p2, &SolidBrush(Color::BlueViolet));
		graphics.DrawString(str3, str3.GetLength(), font1, p3, &SolidBrush(Color::BlueViolet));
	}
	if (graph_type == 2)
	{
		graphics.DrawCurve(graphPen1, program->pointsR.data(), program->pointsR.size());
	}

	graphics.ResetTransform();
}
