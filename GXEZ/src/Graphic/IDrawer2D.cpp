#include "GXEZ/Graphic/IDrawer2D.h"

namespace GXEZ
{

	IDrawer2D::Rect::Rect()
	{
		width = 0;
		height = 0;
		radius = 0;
		radiusType = UnitDraw::ABSOLUTE_PX;
	}

	IDrawer2D::Rect::Rect(unsigned int _width, unsigned int _height, Color _color)
	{
		width = _width;
		height = _height;
		color = _color;
		radius = 0;
		radiusType = UnitDraw::ABSOLUTE_PX;
	}

	IDrawer2D::Circle::Circle()
	{
		thickness = 1;
		diameter = 2;
		color = Color(255, 255, 255);
		part = Circle::Part::FULL;
	}

	IDrawer2D::Circle::Circle(unsigned int _diameter, Color _color, unsigned int _thickness, Circle::Part _part)
	{
		part = _part;
		thickness = _thickness;
		diameter = _diameter;
		color = _color;
	}



}