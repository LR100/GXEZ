#ifndef ADRAWER2DEXTENDED_H_
#define ADRAWER2DEXTENDED_H_

// GXEZ
#include "GXEZ/Graphic/IDrawer2D.h"

namespace GXEZ
{
	class ADrawer2DExtended : public IDrawer2D
	{
	public:
		ADrawer2DExtended();
		virtual ~ADrawer2DExtended() {};
		// Inherited via IDrawer2D

		virtual void DrawRectBorder(const Vec2i& pos, const IDrawer2D::RectBorder& rectBorder) override;
		virtual void DrawRect(const Vec2i& pos, const IDrawer2D::Rect& rect) override;
		virtual void DrawCircle(const Vec2i& pos, const IDrawer2D::Circle& circle) override;

	protected:

		// Point
		virtual void					InternDrawPoint(const Vec2i& pos, const Color& color) = 0;
		// Line
		virtual void					InternDrawLine(const Vec2i& a, const Vec2i& b, const Color& color) = 0;
		// Rect
		virtual void					InternDrawRectBorder(const Vec2i& pos, const IDrawer2D::RectBorder& borderdef) = 0;
		virtual void					InternDrawRect(const Vec2i& pos, const IDrawer2D::Rect& rect) = 0;

		struct InfoRect
		{
			bool	Init(const int& xr, const int& yr, const unsigned int& widthMax, const unsigned int& heightMax, const unsigned int& width, const unsigned int& height);

			int		xMax, yMax;
			int		xi, yi;
			int		iw, ih;
			int		cX, cY;
			int		cXMax, cYMax;
		};


	private:

		class DrawerCircleBressenham
		{
		public:
			DrawerCircleBressenham(ADrawer2DExtended* drawer);
			// Use X and Y as center for each parts
			void	Draw(const int& _x, const int& _y, const IDrawer2D::Circle& circle);
			// Use Part Parts Position
			void	Draw(const IDrawer2D::Circle& circle);
			void	SetPartsPosition(const Vec2i& pos, const IDrawer2D::Circle::Part& part);

		private:

			Vec2i	pTL, pTR, pBL, pBR;
			int		xo, xi;
			
			int		r;
			int		ym;
			int		erro, erri;
			int		inner;
			

			typedef			void(DrawerCircleBressenham::* DrawCircleBressenhamFunction)();

			inline void DrawXLine(int x1, int x2, int y, const Color& color);
			inline void DrawYLine(int x, int y1, int y2, const Color& color);

			inline void DrawFull();
			inline void	DrawTop();
			inline void	DrawTopLeft();
			inline void	DrawTopRight();
			inline void	DrawBottom();
			inline void	DrawBottomLeft();
			inline void	DrawBottomRight();

			Circle							_circle;
			DrawCircleBressenhamFunction	_drawFct;
			ADrawer2DExtended*				_drawer;
		};


		class IDrawerRect
		{
		public:
			IDrawerRect(ADrawer2DExtended* drawer);

		protected:
			virtual void	DrawStart(const int& _x, const int& _y, Rect& rect);
			// Can verify additional things on init if everything was fine on classic init
			virtual bool	OnDrawInit() { return (true); };

			virtual void	DrawClassic() = 0;
			virtual void	DrawRadius() = 0;

			Vec2i			_posA, _posB;
			Rect			_rectI;

			Vec2i					_pos;
			ADrawer2DExtended*		_drawer;
			DrawerCircleBressenham	_drawerCircle;

		private:
			bool			DrawInit(const int& _x, const int& _y, Rect& rect);
		};

		class DrawerRect : public IDrawerRect
		{
		public:
			DrawerRect(ADrawer2DExtended* drawer) : IDrawerRect(drawer) {};
			void	Draw(const int& _x, const int& _y, const Rect& rect);
		private:
			virtual void	DrawClassic() override;
			virtual void	DrawRadius() override;
			
			Rect			_rect;
		};

		class DrawerRectBorder : public IDrawerRect
		{
		public:
			DrawerRectBorder(ADrawer2DExtended* drawer) : IDrawerRect(drawer) {};
			void	Draw(const int& _x, const int& _y, const RectBorder& rectBorder);
		private:
			virtual void	DrawClassic() override;
			virtual void	DrawRadius() override;

			RectBorder		_rectBorder;
		};

		DrawerCircleBressenham	_dCB;
		DrawerRect				_dR;
		DrawerRectBorder		_dRB;
	};
}

#endif // !ADRAWER2DEXTENDED_H_