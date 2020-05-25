#pragma once

#include "GXEZ/Graphic/Color.h"
// Use Vec for coordinates
#include "MXEZ/Vec/Vec.h" 

using namespace MXEZ;

namespace GXEZ
{
	class IDrawer2D
	{
	public:

		enum class UnitDraw
		{
			ABSOLUTE_PX,
			RELATIVE_TO_SHAPE,
		};

		////////////
		// Shapes //
		////////////

		// Rect 
		struct Rect
		{
			Rect();
			Rect(unsigned int width, unsigned int height, Color color);

			unsigned int	width;
			unsigned int	height;
			Color			color;
			float			radius;
			UnitDraw		radiusType;
		};

		// Rect Border
		struct RectBorder : public Rect
		{
			RectBorder() {
				size = 0;
			}
			float			size;
		};

		// Circle
		struct Circle
		{
			enum class Part {
				FULL = 0,
				TOP,
				TOP_LEFT,
				TOP_RIGHT,
				BOTTOM,
				BOTTOM_LEFT,
				BOTTOM_RIGHT
			};

			Circle();
			Circle(unsigned int diameter, Color color, unsigned int thickness = 1, Circle::Part part = IDrawer2D::Circle::Part::FULL);

			Part			part;
			unsigned int	diameter;
			unsigned int	thickness;
			Color			color;
		};


		// Text
		struct Text
		{

		};


		struct Texture
		{

		};

		/////////////////////////
		// Shapes DRAW Methods //
		/////////////////////////
	
		// Point
		virtual void					DrawPoint(const Vec2i& pos, const Color& color) = 0;
		// Line
		virtual void					DrawLine(const Vec2i& a, const Vec2i& b, const Color& color) = 0;
		// Rect
		virtual void					DrawRectBorder(const Vec2i& pos, const IDrawer2D::RectBorder& rectBorder) = 0;
		virtual void					DrawRect(const Vec2i& pos, const IDrawer2D::Rect& rect) = 0;
		// Circle
		virtual void					DrawCircle(const Vec2i& pos, const IDrawer2D::Circle& circle) = 0;
		// Text
		virtual void					DrawText(const Vec2i& pos, const IDrawer2D::Text& text) = 0;
	};
}

